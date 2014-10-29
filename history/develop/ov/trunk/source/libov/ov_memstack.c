/*
*   $Id: ov_memstack.c,v 1.4 2000-02-23 12:57:30 dirk Exp $
*
*   Copyright (C) 1998-1999
*   Lehrstuhl fuer Prozessleittechnik,
*   RWTH Aachen, D-52056 Aachen, Germany.
*   All rights reserved.
*
*   Author: Dirk Meyer <dirk@plt.rwth-aachen.de>
*
*   This file is part of the ACPLT/OV Package which is licensed as open
*   source under the Artistic License; you can use, redistribute and/or
*   modify it under the terms of that license.
*
*   You should have received a copy of the Artistic License along with
*   this Package; see the file ARTISTIC-LICENSE. If not, write to the
*   Copyright Holder.
*
*   THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR IMPLIED
*   WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES
*   OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*/
/*
*	History:
*	--------
*	21-Apr-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*/
/*
*	Description:
*	------------
*	The routines included in this file implement a stack memory which is
*	designed for very short living memory with a very fast allocation and an
*	even faster free operation.
*	In a routine needing temporary memory or calling a function allocating
*	stack memory, the following steps are necessary:
*	1. Call ov_memstack_lock():
*	   This will initialize the stack and ensure that the memory is not
*	   freed until it is unlocked with ov_memstack_unlock(). As the function
*	   ov_memstack_lock() holds a reference counter, which is incremented by
*	   this function, it is reentrant. However, you should hold the lock only
*	   for a short time!
*	2. Call ov_memstack_alloc() or functions allocating stack memory:
*	   This function will allocate aligned memory for you on the stack. The
*	   memory can not be explicitly freed.
*	3. Call ov_memstack_unlock();
*	   This call will decrement the reference counter of the stack memory.
*	   If it reaches zero, the stack memory is freed.
*/

#define OV_COMPILE_LIBOV

#include "libov/ov_memstack.h"
#include "libov/ov_malloc.h"
#include "libov/ov_macros.h"

/*	----------------------------------------------------------------------	*/

/*
*	Global variables
*/
static OV_MEMSTACK	memstack;
static OV_MEMSTACK	*pmemstack = &memstack;
static OV_BYTE		*pmem = (OV_BYTE*)&memstack.memory;
static OV_INT		refcount = 0;


/*	----------------------------------------------------------------------	*/

/*
*	Increment the reference count of the stack and initialize
*	if necessary
*/
OV_DLLFNCEXPORT void _F_ov_memstack_lock(void) {
	/*
	*	initialize if necessary
	*/
	if(!refcount) {
		pmemstack = &memstack;
		pmemstack->memalignunion.pnext = NULL;
		pmem = (OV_BYTE*)&memstack.memory;
	}
	/*
	*	increment the reference count
	*/
	refcount++;
	Ov_WarnIf(refcount > 16);
}

/*	----------------------------------------------------------------------	*/

/*
*	Allocate memory on the stack
*/
OV_DLLFNCEXPORT OV_POINTER ov_memstack_alloc(
	OV_UINT		size
) {
	/*
	*	local variables
	*/
	OV_BYTE		*ptr = pmem;
	OV_MEMSTACK	*pnewpage;
	/*
	*	test reference count
	*/
	if(!refcount) {
		Ov_WarnIfNot(refcount);
		return NULL;
	}
	/*
	*	add aligned size
	*/
	pmem += Ov_AlignTo(sizeof(double), size);
	/*
	*	return pointer to memory if new block fits in
	*/
	if(pmem <= ((OV_BYTE*)pmemstack)+OV_MEMSTACK_SIZE) {
		return ptr;
	}
	/*
	*	allocate a new memory page
	*/
	if(size+offsetof(OV_MEMSTACK, memory) > OV_MEMSTACK_SIZE) {
		pnewpage = (OV_MEMSTACK*)Ov_HeapMalloc(size+offsetof(OV_MEMSTACK, memory));
	} else {
		pnewpage = (OV_MEMSTACK*)Ov_HeapMalloc(sizeof(OV_MEMSTACK));
	}
	if(pnewpage) {
		pnewpage->memalignunion.pnext = NULL;
		pmemstack->memalignunion.pnext = pnewpage;
		pmemstack = pnewpage;
		ptr = (OV_BYTE*)&pmemstack->memory;
		pmem = ptr + Ov_AlignTo(sizeof(double), size);
		return ptr;
	}
	/*
	*	no memory
	*/
	return NULL;
}

/*	----------------------------------------------------------------------	*/

/*
*	Decrement the reference count of the stack and free the
*	stack memory if necessary
*/
OV_DLLFNCEXPORT void _F_ov_memstack_unlock(void) {
	/*
	*	decrement the reference count
	*/
	refcount--;
	Ov_WarnIf(refcount < 0);
	/*
	*	free additionally allocated memory pages if necessary
	*/
	if(!refcount) {
		/*
		*	local variables
		*/
		OV_MEMSTACK	*pmemstack2, *pmemstack3;
		/*
		*	free memory
		*/
		pmemstack2=memstack.memalignunion.pnext;
		while(pmemstack2) {
			pmemstack3 = pmemstack2->memalignunion.pnext;
			Ov_HeapFree(pmemstack2);
			pmemstack2 = pmemstack3;
		}			
		/*
		*	reset pointers
		*/
		pmemstack = &memstack;
		pmemstack->memalignunion.pnext = NULL;
		pmem = (OV_BYTE*)&memstack.memory;
#if MEMSTACK_LOG
		printf("memstack freed\n");
#endif
	}
}

/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/

