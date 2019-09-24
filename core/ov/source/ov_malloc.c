/*
*   $Id: ov_malloc.c,v 1.2 1999-09-15 10:48:22 dirk Exp $
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
*	13-Jan-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*	13-Apr-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: Major revision.
*/
/*
*	Description:
*	------------
*	The routines included in this file are a wrapper for the standard
*	malloc(), realloc() and free() functions, which may cause problems
*	with DLLs/shared libraries, if they have an own allocator.
*	Usage: free any heap memory allocated in function of LIBOV using the
*	function ov_free() and (re)allocate any heap memory freed later in
*	LIBOV using ov_malloc() or ov_realloc().
*/

#define OV_COMPILE_LIBOV

#include "ov_malloc.h"
#include "ov_logfile.h"
#include "tlsf.h"

#if TLSF_HEAP
#include <sys/time.h>
#include <unistd.h>
#if OV_SYSTEM_UNIX
#include <sys/resource.h>
#include <sys/mman.h>
#endif
#endif

#if OV_SYSTEM_MC164
#define malloc	xmalloc
#define free	xfree
#define realloc	xrealloc
#endif

/*	----------------------------------------------------------------------	*/
#if TLSF_HEAP
static void *heappool = NULL;
static OV_BOOL freeHeappool = FALSE;

OV_DLLFNCEXPORT void ov_initHeap(size_t size){

	if(size) {
		heappool = malloc(size);
		freeHeappool = TRUE;

#if OV_SYSTEM_UNIX
		if (mlockall(MCL_CURRENT | MCL_FUTURE ))
		{
			ov_logfile_error("mlockall failed:");
		}

		size_t  page_size = sysconf(_SC_PAGESIZE);
		for (size_t i=0; i < size; i+=page_size)
		{
			((char*)heappool)[i] = 0;
		}
#endif

		init_memory_pool(size,heappool);
		tlsf_set_pool(ov_heap, heappool);
	}
#if USE_MMAP || USE_SBRK || USE_VIRTALLOC
	else
		heappool = tlsf_activate();
#endif
}

OV_DLLFNCEXPORT void ov_destroyHeap(){

	destroy_memory_pool(heappool);
	if(freeHeappool){
		free(heappool);
		freeHeappool = FALSE;
	}
		heappool = NULL;

}

OV_DLLFNCEXPORT OV_UINT ov_heapGetPoolSize(){
	return get_pool_size(heappool);
}

OV_DLLFNCEXPORT OV_UINT ov_heapGetUsedSize(){
	return get_used_size(heappool);
}

OV_DLLFNCEXPORT OV_UINT ov_heapGetFreeSize(){
	return get_free_size(heappool);
}

OV_DLLFNCEXPORT OV_DOUBLE ov_heapGetFragmentation(){
	return get_fragmentation(heappool)*100.0;
}
#else
OV_DLLFNCEXPORT void ov_initHeap(size_t size) {};
OV_DLLFNCEXPORT void ov_destroyHeap(){};
OV_DLLFNCEXPORT OV_UINT ov_heapGetPoolSize(){return 0;}
OV_DLLFNCEXPORT OV_UINT ov_heapGetUsedSize(){return 0;}
OV_DLLFNCEXPORT OV_UINT ov_heapGetFreeSize(){return 0;}
OV_DLLFNCEXPORT OV_DOUBLE ov_heapGetFragmentation(){return 0.0;}
#endif

/*
*	Allocate memory on the heap
*/
OV_DLLFNCEXPORT OV_POINTER ov_malloc(
	OV_UINT		size
) {
#if TLSF_HEAP
	return tlsf_malloc(size, ov_heap);
#endif
	return malloc(size);
}

/*	----------------------------------------------------------------------	*/

/*
*	Free memory allocated in the heap
*/
OV_DLLFNCEXPORT void ov_free(
	OV_POINTER	ptr
) {
#if TLSF_HEAP
	tlsf_free(ptr, ov_heap);
#else
	free(ptr);
#endif
}

/*	----------------------------------------------------------------------	*/

/*
*	Reallocate memory on the heap
*/
OV_DLLFNCEXPORT OV_POINTER ov_realloc(
	OV_POINTER	ptr,
	OV_UINT		size
) {
#if TLSF_HEAP
	return tlsf_realloc(ptr, size, ov_heap);
#endif
	return realloc(ptr, size);
}

/*	----------------------------------------------------------------------	*/

/*
*	Duplicate a string on the heap
*/
OV_DLLFNCEXPORT OV_STRING ov_strdup(
	OV_STRING	string
) {
	/*
	*	local variables
	*/
    OV_STRING result;

	if(!string)
		return NULL;

#if TLSF_HEAP
    result = (OV_STRING)tlsf_malloc(strlen(string)+1, ov_heap);
#else
	result = (OV_STRING)malloc(strlen(string)+1);
#endif
    if(!result) {
        return NULL;
    }
    strcpy(result, string);
    return result;
}

/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/

