/*
*   $Id: ov_memstack.h,v 1.2 1999-09-15 10:48:14 dirk Exp $
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

#ifndef OV_MEMSTACK_H_INCLUDED
#define OV_MEMSTACK_H_INCLUDED

#include <stdlib.h>

#include "libov/ov_ov.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
*	Size of a stack memory page
*/
#define OV_MEMSTACK_SIZE	4096	/* should be a power of two */

/* TODO add with new libov release!
*/
/*
*	Defines to use the logger if wanted
*/
//#if defined MEMSTACK_LOG && MEMSTACK_LOG != 0
	/*	yes, this variable will cause warnings when memstack.h is included and lock / unlock is not used in that file	*/
//	static OV_INT localRefCount = 0;
//	#if MEMSTACK_LOG >= 2
		/*	high loglevel: log locks and unlocks, print out if refcount is 0	*/
		/*	use do{}while(0) to make the statement one block with the need of a ; at the end	*/
/*
		#define ov_memstack_lock() 																			\
		do{																									\
			localRefCount++;																				\
			printf("memstack_lock: %s line %i, local refcount is %i\n", __FILE__, __LINE__, localRefCount);	\
			_F_ov_memstack_lock();																			\
		}while(0)

		#define ov_memstack_unlock()																			\
		do{																										\
			localRefCount--;																					\
			printf("memstack_unlock: %s line %i, local refcount is %i\n", __FILE__, __LINE__, localRefCount);	\
			if(!localRefCount)																					\
				printf("memstack warning: local refcount is 0!\n");																	\
			_F_ov_memstack_unlock();																			\
		}while(0)
*/
//	#elif MEMSTACK_LOG == 1
		/*	low loglevel: print out if refcount is 0	*/
/*
		#define ov_memstack_lock() 																			\
		do{																									\
			localRefCount++;																				\
			_F_ov_memstack_lock();																			\
		}while(0)

		#define ov_memstack_unlock()																			\
		do{																										\
			localRefCount--;																					\
			if(!localRefCount)																					\
				printf("localRefCount is 0: %s line %i\n", __FILE__, __LINE__);									\
			_F_ov_memstack_unlock();																			\
		}while(0)
*/
//	#endif
//#else
//	/*	no logging	*/
//	#define ov_memstack_lock() _F_ov_memstack_lock()
//	#define ov_memstack_unlock() _F_ov_memstack_unlock()
//#endif

/* end of TODO add with new libov release! 
  *
  */ 

/*
*	OV_MEMSTACK:
*	------------
*	Memory page on the memory stack
*/
struct OV_MEMSTACK {
	union {
		struct OV_MEMSTACK	*pnext;
		double				unused;
	}	memalignunion;
	char	memory[OV_MEMSTACK_SIZE-sizeof(double)];
};
typedef struct OV_MEMSTACK OV_MEMSTACK;

/*
*	Increment the reference count of the stack and initialize
*	if necessary
*/
/* TODO add with new libov release! and remove old function
 *	OV_DLLFNCEXPORT void _F_ov_memstack_lock(void);
 */
OV_DLLFNCEXPORT void ov_memstack_lock(void);

/*
*	Allocate memory on the stack
*/
OV_DLLFNCEXPORT OV_POINTER ov_memstack_alloc(
	OV_UINT		size
);

/*
*	Decrement the reference count of the stack and free the
*	stack memory if necessary
*/
/* TODO add with new libov release! and remove old function
 *	OV_DLLFNCEXPORT void _F_ov_memstack_unlock(void);
 */
OV_DLLFNCEXPORT void ov_memstack_unlock(void);

#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif
/*
*	End of file
*/

