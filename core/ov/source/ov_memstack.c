/*
*   $Id: ov_memstack.c,v 1.4 2000-02-23 12:57:30 dirk Exp $
*
*   Copyright (C) 1998
*   Lehrstuhl fuer Prozessleittechnik,
*   D-52056 Aachen, Germany.
*   All rights reserved.
*
*   Author: Dirk Meyer <dirk@plt.rwth-aachen.de>
*
*   This file is part of the ACPLT/OV Package 
*   Licensed under the Apache License, Version 2.0 (the "License");
*   you may not use this file except in compliance with the License.
*   You may obtain a copy of the License at
*
*       http://www.apache.org/licenses/LICENSE-2.0
*
*   Unless required by applicable law or agreed to in writing, software
*   distributed under the License is distributed on an "AS IS" BASIS,
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*   See the License for the specific language governing permissions and
*   limitations under the License.
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

#if OV_SYNC_PTHREAD
#include <pthread.h>
static pthread_mutex_t memstackMutex = PTHREAD_MUTEX_INITIALIZER;

#define memstack_mutex_init()
#define memstack_mutex_destroy()
#define memstack_mutex_lock() pthread_mutex_lock(&memstackMutex)
#define memstack_mutex_unlock() pthread_mutex_unlock(&memstackMutex)

#elif OV_SYNC_NTMUTEX
#include <windows.h>
static HANDLE memstackMutex = NULL;

#define memstack_mutex_init() memstackMutex=CreateMutexA(NULL, FALSE, NULL)
#define memstack_mutex_destroy() CloseHandle(memstackMutex)
// first lock creates mutex object because there is no good place to initialize it
#define memstack_mutex_lock() \
		if(!memstackMutex){memstackMutex=CreateMutexA(NULL, FALSE, NULL);}\
		WaitForSingleObject(memstackMutex, INFINITE)
#define memstack_mutex_unlock() ReleaseMutex(memstackMutex)

#else
#define memstack_mutex_init()
#define memstack_mutex_destroy()
#define memstack_mutex_lock()
#define memstack_mutex_unlock()
#endif


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
OV_DLLFNCEXPORT OV_INT _F_ov_memstack_lock(void) {
	/*
	*	lock mutex
	*/
	memstack_mutex_lock();
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
	/*
	*	unlock mutex
	*/
	memstack_mutex_unlock();
	return refcount;
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
	OV_BYTE		*ptr;
	OV_MEMSTACK	*pnewpage;

	/*
	*	lock mutex
	*/
	memstack_mutex_lock();

	ptr = pmem;

	/*
	*	test reference count
	*/
	if(!refcount) {
		Ov_WarnIfNot(refcount);
		memstack_mutex_unlock();
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
		memstack_mutex_unlock();
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
		memstack_mutex_unlock();
		return ptr;
	}
	/*
	*	no memory
	*	unlock mutex
	*/
	memstack_mutex_unlock();
	return NULL;
}

/*	----------------------------------------------------------------------	*/

/*
*	Decrement the reference count of the stack and free the
*	stack memory if necessary
*/
OV_DLLFNCEXPORT OV_INT _F_ov_memstack_unlock(void) {
	/*
	*	lock mutex
	*/
	memstack_mutex_lock();
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
	/*
	*	unlock mutex
	*/
	memstack_mutex_unlock();
	return refcount;
}

/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/

