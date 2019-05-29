/*
 *   $Id: ov_database.c,v 1.27 2008-12-10 14:30:55 martin Exp $
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
 *	13-Apr-1999 Dirk Meyer   <dirk@plt.rwth-aachen.de>: File created.
 *	07-Jun-2001 J.Nagelmann  <nagelmann@ltsoft.de>: Changes for Sun Solaris.
 *	30-Apr-2008 A.Neugebauer <neugebauer@ltsoft.de>: Changes for libml.
 *   02-Jun-2013 Sten Gruener <s.gruener@plt.rwth-aachen.de>: Notes on valgrind.
 */

#define OV_COMPILE_LIBOV

// for getpagesize
#define _BSD_SOURCE
#define _DEFAULT_SOURCE

#include "libov/ov_database.h"
#include "libov/ov_object.h"
#include "libov/ov_library.h"
#include "libov/ov_string.h"
#include "libov/ov_time.h"
#include "libov/ov_macros.h"
#include "libov/ov_logfile.h"
#include "libov/ov_path.h"
#if TLSF
#include "libov/tlsf.h"
#include <sys/time.h>
#include <unistd.h>
#if OV_SYSTEM_UNIX
#include <sys/resource.h>
#include <sys/mman.h>
#endif
#endif

#if NO_LIBML && !TLSF
#error enable TLSF or define NO_LIBML=0 when calling make
#endif

#if OV_SYNC_PTHREAD && (!TLSF || !TLSF_USE_LOCKS)
#include <pthread.h>
static pthread_mutex_t databaseMutex = PTHREAD_MUTEX_INITIALIZER;
#define database_mutex_init()
#define database_mutex_destroy()
#define database_mutex_lock() pthread_mutex_lock(&databaseMutex)
#define database_mutex_unlock() pthread_mutex_unlock(&databaseMutex)

#elif OV_SYNC_NTMUTEX  && (!TLSF || !TLSF_USE_LOCKS)
#include <windows.h>
static HANDLE databaseMutex = NULL;
#define database_mutex_init() databaseMutex = CreateMutexA(NULL, FALSE, NULL)
#define database_mutex_destroy() CloseHandle(databaseMutex)
#define database_mutex_lock() WaitForSingleObject(databaseMutex, INFINITE)
#define database_mutex_unlock() ReleaseMutex(databaseMutex)

#else
#define database_mutex_lock()
#define database_mutex_unlock()
#define database_mutex_init()
#define database_mutex_destroy()
#endif

#if OV_SYSTEM_SOLARIS
#include "/usr/ucbinclude/sys/file.h"
#include <sys/file.h>
#include <fcntl.h>
#include <unistd.h>
int flock_solaris (int filedes, int oper)
{
	struct flock the_lock;
	switch (oper) {
		case LOCK_EX:
		the_lock.l_type = F_WRLCK;
		break;
		case LOCK_UN:
		the_lock.l_type = F_UNLCK;
		break;
		default:
		return (-1);
		break;
	}
	the_lock.l_start=0;
	the_lock.l_whence=SEEK_SET;
	the_lock.l_len=0;
	the_lock.l_pid=getpid();
	return (fcntl(filedes,F_SETLKW, &the_lock));
}
#endif

#if OV_SYSTEM_UNIX
#include <sys/mman.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

#if OV_DEBUG || OV_VALGRIND
static void ov_freelist_free();
#else
#define ov_freelist_free()
#endif

#if OV_SYSTEM_NT || OV_SYSTEM_RMOS
#include <fcntl.h>
#include <io.h>
#endif

#if OV_SYSTEM_OPENVMS
#include <rms.h>
#include <rmsdef.h>
#include <secdef.h>
#include <ssdef.h>
#include <starlet.h>
#endif

#if !TLSF
#ifndef __STDC__
#define __STDC__ 1
#include "ml_malloc.h"
#undef __STDC__
#else
#include "ml_malloc.h"
#endif
#endif

#if OV_SYSTEM_MC164
#define malloc	xmalloc
#define free	xfree
#define realloc	xrealloc
#define memset xmemset
#define memcpy xmemcpy
#endif

/*	----------------------------------------------------------------------	*/

/*
 *	Global variable: database pointer, file handles and stuff
 */
OV_DLLVAREXPORT OV_DATABASE_INFO OV_MEMSPEC *pdb = NULL;
OV_UINT dbFlags = 0;

#if OV_SYSTEM_UNIX
static int fd = 0;
#endif

#if OV_SYSTEM_NT
static HANDLE hfile = NULL;
static HANDLE hmap = NULL;
#endif

#if OV_SYSTEM_RMOS
FILE *file = NULL;
#endif

#if OV_SYSTEM_OPENVMS
unsigned short channel = 0;
#endif

#if OV_SYSTEM_MC164
OV_DATABASE_INFO OV_MEMSPEC *pdbmem;
#endif

#if TLSF
static void* dbpool;
#endif
/*
 *	VTable of any object in case we dont start up the database
 */
static OV_VTBL_ov_object nostartupvtable = { ov_object_constructor_nostartup,
		ov_object_checkinit, ov_object_destructor, ov_object_startup,
		ov_object_shutdown, ov_object_rename, ov_object_getaccess_nostartup,
		ov_object_getflags, ov_object_getcomment, ov_object_gettechunit,
		ov_object_getvar, ov_object_setvar };

/*	----------------------------------------------------------------------	*/

/*
 *	Helper macro: Round up the size to the next n*BLOCKSIZE
 *	TODO_ADJUST_WHEN_LARGE_DB
 */
#define Ov_Roundup(size, block) (((size_t)(size)+block-1)&~(block-1))

static size_t ov_database_roundupFileSize(size_t size){
	size_t pagesize = 8192;
#if OV_SYSTEM_UNIX
	pagesize = getpagesize();
#elif OV_SYSTEM_NT
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	pagesize = sysInfo.dwPageSize;
#endif
	return Ov_Roundup(size, pagesize);
}
/*	----------------------------------------------------------------------	*/

#if !TLSF
/*
 *	Macro: pointer to memory mempool info structure
 */
#define pmpinfo ((ml_info OV_MEMSPEC *)(pdb+1))

/*	----------------------------------------------------------------------	*/


/*
 *	Get more core memory from the memory mempool (subroutine)
 */
__ml_ptr ov_database_morecore(__ml_size_t size) {
	/*
	 *	local variables
	 */
	__ml_ptr pmem = (__ml_ptr) (pdb->pcurr);
	OV_BYTE OV_MEMSPEC *psoon = pdb->pcurr + size;
#if OV_DYNAMIC_DATABASE
#if OV_SYSTEM_UNIX
	char null = 0;
#endif
#endif

	/*
	 *	test if we can get/release memory
	 */
	if (psoon < pdb->pstart) {
		return NULL;
	}
	if (psoon > pdb->pend) {
#if OV_DYNAMIC_DATABASE
		/*
		 *	check database file size
		 */
		if(pdb->size+size > (OV_UINT) OV_DATABASE_MAXSIZE) {
			return NULL;
		}
#if OV_SYSTEM_UNIX
		/*
		 *	extend the database file in size
		 */
		if(lseek(fd, pdb->size+size-sizeof(char), SEEK_SET) < 0) {
			return NULL;
		}
		if(write(fd, (void*)&null, sizeof(char)) == -1) {
			return NULL;
		}
#endif
#if OV_SYSTEM_NT
		/*
		 *	extend the database file in size
		 */
		if(SetFilePointer(hfile, pdb->size+size, NULL, FILE_BEGIN)
				== 0xFFFFFFFF
				// TODO_ADJUST_WHEN_LARGE_DB
		) {
			return NULL;
		}
		if(!SetEndOfFile(hfile)) {
			return NULL;
		}
#endif
		pdb->pend += size;
		pdb->size += size;
#else
		return NULL;
#endif
	}
	/*
	 *	yes we can. get/release it.
	 */
	pdb->pcurr = psoon;
	return (__ml_ptr) pmem;
}
#endif

/*	----------------------------------------------------------------------	*/

/*	
 *	Get a numeric id and increment the internal counter
 */
OV_DLLFNCEXPORT OV_BOOL ov_database_getId(OV_UINT *idH, OV_UINT *idL) {
	if (idH) {
		*idH = pdb->idCounter1;
	}
	if (idL) {
		*idL = pdb->idCounter0;
	}

	if (pdb->idCounter0 < OV_VL_MAXUINT) {
		pdb->idCounter0++;
		return TRUE;
	} else {
		pdb->idCounter0 = 0;
		if (pdb->idCounter1 < OV_VL_MAXUINT) {
			pdb->idCounter1++;
			return TRUE;
		} else {
			/*	error --> too many objects, I wonder if this will ever happen^^	*/
			return FALSE;
		}
	}
}

/*
 *	Get a numeric id being set next time and do not increment the internal counter
 *	INTERNAL USE ONLY --> create instances with unique identifier variables mirroring the numeric id
 */
void ov_database_getId_noInc(OV_UINT *idH, OV_UINT *idL) {
	if (idH) {
		*idH = pdb->idCounter1;
	}
	if (idL) {
		*idL = pdb->idCounter0;
	}
}

/*
 *	Convert a high low dword pair to a 64bit id
 */
OV_DLLFNCEXPORT OV_UINT64 ov_database_convertId(const OV_UINT idH,
		const OV_UINT idL) {
	OV_UINT64 result;
	result = (OV_UINT64) idH << 32;
	result += idL;
	return result;
}

/*	compare 2 ids returns -1 if id1 < id2, 1 if id1 > id2 and else 0	*/
OV_INT ov_database_compareIds(const OV_UINT id1H, const OV_UINT id1L,
		const OV_UINT id2H, const OV_UINT id2L) {
	if (id1H > id2H) {
		return 1;
	} else if (id1H < id2H) {
		return -1;
	} else {
		if (id1L > id2L) {
			return 1;
		} else if (id1L < id2L) {
			return -1;
		} else {
			return 0;
		}
	}
}

/*	get the node (chunk) an id is in
 *	walk along the nodes and check if id is between min and max for this node
 *	if id > idList->maxId / 2 walk from the end, otherwise walkl from the beginning
 */
OV_IDLIST_NODE* ov_database_idListGetNode(const OV_UINT idH, const OV_UINT idL) {
	OV_IDLIST_NODE* node = NULL;
	OV_UINT64 id64;
	OV_DOUBLE idFraction;
	OV_UINT64 nodeNumber;
#if OV_IDLIST_PERFORMANCEPRINTS
	OV_UINT cycleCounter = 0;
#endif
	id64 = ov_database_convertId(idH, idL);
	if (id64 > pdb->idList->maxId) {
		return NULL;
	}

	/*	calculate starting point for node search
	 multiply the idFraction (searchedId / maxId) with the
	 the result with the number of nodes to get the starting point
	 if this one does not match directly walk along the list*/
	idFraction = id64 / pdb->idList->maxId;
	nodeNumber = (OV_UINT64) (idFraction * (pdb->idList->nodeCount - 1));

	node = pdb->idList->pNodes[nodeNumber];
	while (node) {
		if (node->maxId < id64) {
			node = node->pNext;
#if OV_IDLIST_PERFORMANCEPRINTS
			cycleCounter++;
#endif
			continue;
		} else if (node->minId > id64) {
			node = node->pPrevious;
#if OV_IDLIST_PERFORMANCEPRINTS
			cycleCounter++;
#endif
			continue;
		} else {
#if OV_IDLIST_PERFORMANCEPRINTS
			if(cycleCounter == 0) {
				ov_logfile_debug("getNode: direct hit! (%llu nodes)", pdb->idList->nodeCount);
			} else {
				ov_logfile_debug("getNode: %u cycles needed (%llu nodes)", cycleCounter, pdb->idList->nodeCount);
			}
#endif
			break;
		}
	}
	return node;
}

/*	get the index of a relation within a node (chunk) by its id
 *	use bisection to find it
 *	returns OV_ERR_GENERIC if it was not found
 */
OV_RESULT ov_database_idListGetRelationIndex(const OV_UINT idH,
		const OV_UINT idL, const struct OV_IDLIST_NODE * const node,
		OV_UINT* index) {
	OV_UINT currMin = 0;
	OV_UINT currMax;
	OV_INT cmpRes;
	OV_UINT64 id64 = ov_database_convertId(idH, idL);
#if OV_IDLIST_PERFORMANCEPRINTS
	OV_UINT cycleCounter = 0;
#endif
	if (!index) {
		return OV_ERR_BADPARAM;
	}

	if (!node->relationCount || (id64 > node->maxId) || (id64 < node->minId)) {
		return OV_ERR_BADVALUE;
	}
	currMax = node->relationCount - 1;
	do {
		*index = (currMax - currMin) / 2 + currMin;
		cmpRes = ov_database_compareIds(idH, idL,
				node->relations[*index].idHigh, node->relations[*index].idLow);
		if (cmpRes > 0) {
			currMin = *index;
#if OV_IDLIST_PERFORMANCEPRINTS
			cycleCounter++;
#endif
		} else if (cmpRes < 0) {
			currMax = *index;
#if OV_IDLIST_PERFORMANCEPRINTS
			cycleCounter++;
#endif
		} else {
#if OV_IDLIST_PERFORMANCEPRINTS
			ov_logfile_debug("getIndex: boundary hit after %u cycles", cycleCounter);
#endif
			return OV_ERR_OK;
		}
	} while ((currMax - currMin) > 1);
	*index = currMin;
	if (ov_database_compareIds(idH, idL, node->relations[*index].idHigh,
			node->relations[*index].idLow) == 0) {
		return OV_ERR_OK;
	} else {
		(*index)++;
		if (ov_database_compareIds(idH, idL, node->relations[*index].idHigh,
				node->relations[*index].idLow) == 0) {
#if OV_IDLIST_PERFORMANCEPRINTS
			ov_logfile_debug("getIndex: hit after %u cycles", cycleCounter);
#endif
			return OV_ERR_OK;
		} else {
#if OV_IDLIST_PERFORMANCEPRINTS
			cycleCounter++;
			ov_logfile_debug("getIndex: hit after %u cycles", cycleCounter);
#endif
			return OV_ERR_GENERIC;
		}
	}
}

#if OV_IDLIST_DEBUG
void idList_printstats() {
	OV_UINT64 i;

	ov_logfile_debug("-----------");
	ov_logfile_debug("basedata:\n\tnodeCount:\t\t%llu\n\tminId:\t\t\t%llu\n\tmaxId:\t\t\t%llu\n\tpFirst:\t\t\t%p\n\tpFirst->nodeNumber:\t%llu\n\tpLast:\t\t\t%p\n\tpLast->nodeNumber:\t%llu",
			pdb->idList->nodeCount, pdb->idList->minId, pdb->idList->maxId, pdb->idList->pFirst, pdb->idList->pFirst->nodeNumber, pdb->idList->pLast, pdb->idList->pLast->nodeNumber);
	ov_logfile_debug("-----\nnodes:");
	for(i=0; i<pdb->idList->nodeCount;i++) {
		ov_logfile_debug("entry %llu:\n\taddress:\t%p\n\tnodeNumber:\t%llu\n\tminId:\t\t%llu\n\tmaxId:\t\t%llu\n\trelationCount:\t%u\n\tpPrevious:\t%p\n\tpNext:\t\t%p\n",
				i, pdb->idList->pNodes[i], pdb->idList->pNodes[i]->nodeNumber, pdb->idList->pNodes[i]->minId, pdb->idList->pNodes[i]->maxId, pdb->idList->pNodes[i]->relationCount, pdb->idList->pNodes[i]->pPrevious, pdb->idList->pNodes[i]->pNext);
	}

}
#endif
/*
 *	Relate an object pointer  with an id (creates a new entry in the idList relating object pointer and id)
 *	is called on every object creation
 */
OV_DLLFNCEXPORT OV_RESULT ov_database_idListInsert(const OV_UINT idH,
		const OV_UINT idL, const OV_INSTPTR_ov_object pInstance) {
	/*	as this function is called on object creation and ids are counted up, a look at the very last element in the list should suffice
	 *	and if not throw an error
	 */
	OV_UINT iterator;
	OV_UINT64 newId = ov_database_convertId(idH, idL);
	OV_IDLIST_NODE *pNode = NULL;

	if (!pInstance) {
		return OV_ERR_BADPARAM;
	} /*	check if id is lager than the largest id in the list or if it ist the very first object	*/
	if ((newId > pdb->idList->maxId)
			|| ((!idH) && (!idL) && (!pdb->idList->maxId))) {
		/*	expected case --> append to the end	*/
		if (pdb->idList->pLast->relationCount >= OV_IDLIST_RELATIONSPERCHUNK) {
#if OV_IDLIST_DEBUG
			ov_logfile_debug("insert - line: %u - before", __LINE__);
			idList_printstats();
#endif
			/*	node is full, create e new one	*/
			pdb->idList->pLast->pNext = ov_database_malloc(
					sizeof(OV_IDLIST_NODE));
			if (!pdb->idList->pLast->pNext) {
				return OV_ERR_DBOUTOFMEMORY;
			}
			pdb->idList->pLast->pNext->pPrevious = pdb->idList->pLast;
			pdb->idList->pLast = pdb->idList->pLast->pNext;
			pdb->idList->pLast->pNext = NULL;
			if (pdb->idList->pLast->pPrevious->nodeNumber == OV_VL_MAXUINT64) {
				return OV_ERR_DBOUTOFMEMORY;
			}
			pdb->idList->pLast->nodeNumber =
					pdb->idList->pLast->pPrevious->nodeNumber + 1;
			pdb->idList->nodeCount = pdb->idList->pLast->nodeNumber;

			for (iterator = 0; iterator < OV_IDLIST_RELATIONSPERCHUNK;
					iterator++) {
				pdb->idList->pLast->relations[iterator].idLow = 0;
				pdb->idList->pLast->relations[iterator].idHigh = 0;
				pdb->idList->pLast->relations[iterator].pobj = NULL;
			}
			pdb->idList->pLast->minId = newId;
			pdb->idList->pLast->relationCount = 0;
			/*	insert node in array	*/
			if ((pdb->idList->nodeCount % OV_IDLIST_NODELISTCHUNKSIZE) == 0) {
				/*	pNodes array is full --> allocate a new chunk	*/
				ov_database_free(pdb->idList->pNodes);
				pdb->idList->pNodes = ov_database_malloc(
						(pdb->idList->nodeCount + OV_IDLIST_NODELISTCHUNKSIZE)
								* sizeof(OV_IDLIST_NODE*));
				if (!pdb->idList->pNodes) {
					return OV_ERR_DBOUTOFMEMORY;
				}
				pNode = pdb->idList->pFirst;
				while (pNode) {
					pdb->idList->pNodes[pNode->nodeNumber - 1] = pNode;
					pNode = pNode->pNext;
				}
			}
			pdb->idList->pNodes[pdb->idList->nodeCount - 1] =
					pdb->idList->pLast;
#if OV_IDLIST_DEBUG
			ov_logfile_debug("insert - line: %u - after", __LINE__);
			idList_printstats();
#endif
		}
		/*	here pLast is surely the chunk we need --> insert 	*/
		pdb->idList->pLast->relations[pdb->idList->pLast->relationCount].idLow =
				idL;
		pdb->idList->pLast->relations[pdb->idList->pLast->relationCount].idHigh =
				idH;
		pdb->idList->pLast->relations[pdb->idList->pLast->relationCount].pobj =
				pInstance;
		pdb->idList->pLast->relationCount++;
		pdb->idList->pLast->maxId = newId;
		pdb->idList->maxId = newId;
		pdb->idList->relationCount++;
		return OV_ERR_OK;
	} else {
		return OV_ERR_BADVALUE;
	}

}

/*	
 *	Releases an id (deletes it from the list)
 *	is called on every object deletion
 */
OV_DLLFNCEXPORT OV_RESULT ov_database_idListRelease(const OV_UINT idH,
		const OV_UINT idL) {
	OV_IDLIST_NODE* node = NULL;
	OV_IDLIST_NODE* nextNode = NULL;
	OV_UINT index;
	OV_RESULT result;
	OV_UINT iterator;
	/*	find relation	*/
	node = ov_database_idListGetNode(idH, idL);
	if (!node) {
		return OV_ERR_GENERIC;
	}
	result = ov_database_idListGetRelationIndex(idH, idL, node, &index);
	if (Ov_Fail(result)) {
		return result;
	}
#if OV_IDLIST_DEBUG
	ov_logfile_debug("release - line: %u - before", __LINE__);
	idList_printstats();
#endif
	/*	move all relations in that node one step backwards	*/

	for (iterator = index + 1; iterator < node->relationCount;
			iterator++, index++) {
		node->relations[index].idLow = node->relations[iterator].idLow;
		node->relations[index].idHigh = node->relations[iterator].idHigh;
		node->relations[index].pobj = node->relations[iterator].pobj;
	}
	node->relationCount--;
	if (node->relationCount) {
		node->minId = ov_database_convertId(node->relations[0].idHigh,
				node->relations[0].idLow);
		node->maxId = ov_database_convertId(
				node->relations[node->relationCount - 1].idHigh,
				node->relations[node->relationCount - 1].idLow);
	}
	/*	if the node and the next node are less than half full put them together, if not do the same test on the previous node	*/
	if (node->pNext
			&& ((node->relationCount / (OV_IDLIST_RELATIONSPERCHUNK / 2)) < 1)
			&& ((node->pNext->relationCount / (OV_IDLIST_RELATIONSPERCHUNK / 2))
					< 1)) {
		nextNode = node->pNext;
#if OV_IDLIST_DEBUG
		ov_logfile_debug("putting together node %llu (min %llu, max %llu, count %lu) and node %llu (next) (min %llu, max %llu, count %lu)", node->nodeNumber, node->minId, node->maxId, node->relationCount, nextNode->nodeNumber, nextNode->minId, nextNode->maxId, nextNode->relationCount);
#endif
		for (index = node->relationCount, iterator = 0;
				(index < OV_IDLIST_RELATIONSPERCHUNK)
						&& (iterator < nextNode->relationCount);
				iterator++, index++) {
			node->relations[index].idLow = nextNode->relations[iterator].idLow;
			node->relations[index].idHigh =
					nextNode->relations[iterator].idHigh;
			node->relations[index].pobj = nextNode->relations[iterator].pobj;
			node->relationCount++;
		}
		node->minId = ov_database_convertId(node->relations[0].idHigh,
				node->relations[0].idLow);
		node->maxId = ov_database_convertId(
				node->relations[node->relationCount - 1].idHigh,
				node->relations[node->relationCount - 1].idLow);
		nextNode->relationCount = 0;
	} else if (node->pPrevious
			&& ((node->relationCount / (OV_IDLIST_RELATIONSPERCHUNK / 2)) < 1)
			&& ((node->pPrevious->relationCount
					/ (OV_IDLIST_RELATIONSPERCHUNK / 2)) < 1)) {
		node = node->pPrevious;
		nextNode = node->pNext;
#if OV_IDLIST_DEBUG
		ov_logfile_debug("putting together node %llu (previous) (min %llu, max %llu, count %lu) and node %llu (min %llu, max %llu, count %lu)", nextNode->nodeNumber, nextNode->minId, nextNode->maxId, nextNode->relationCount, node->nodeNumber, node->minId, node->maxId, node->relationCount);
#endif
		for (index = node->relationCount, iterator = 0;
				(index < OV_IDLIST_RELATIONSPERCHUNK)
						&& (iterator < nextNode->relationCount);
				iterator++, index++) {
			node->relations[index].idLow = nextNode->relations[iterator].idLow;
			node->relations[index].idHigh =
					nextNode->relations[iterator].idHigh;
			node->relations[index].pobj = nextNode->relations[iterator].pobj;
			node->relationCount++;
		}
		node->minId = ov_database_convertId(node->relations[0].idHigh,
				node->relations[0].idLow);
		node->maxId = ov_database_convertId(
				node->relations[node->relationCount - 1].idHigh,
				node->relations[node->relationCount - 1].idLow);
		nextNode->relationCount = 0;
	} else if (node->relationCount == 0) {
		nextNode = node;
	}

	if (nextNode && (!nextNode->relationCount)) {
#if OV_IDLIST_DEBUG
		ov_logfile_debug("deleting node %llu (min %llu, max %llu, count %lu)", nextNode->nodeNumber, nextNode->minId, nextNode->maxId, nextNode->relationCount);
#endif
		nextNode->pPrevious->pNext = nextNode->pNext;
		if (nextNode->pNext) {
			nextNode->pNext->pPrevious = nextNode->pPrevious;
		}
		for (iterator = nextNode->nodeNumber;
				(iterator < pdb->idList->nodeCount); iterator++) {
			pdb->idList->pNodes[iterator - 1] = pdb->idList->pNodes[iterator];
			pdb->idList->pNodes[iterator - 1]->nodeNumber--;
		}
		pdb->idList->pNodes[iterator - 1] = NULL;
		pdb->idList->nodeCount--;
		ov_database_free(nextNode);
		pdb->idList->pLast = pdb->idList->pNodes[pdb->idList->nodeCount - 1];

		if ((pdb->idList->nodeCount % OV_IDLIST_NODELISTCHUNKSIZE) == 0) {
			/*	a complete chunk is empty now --> release memory	*/
			pdb->idList->pNodes = ov_database_realloc(pdb->idList->pNodes,
					pdb->idList->nodeCount * sizeof(OV_IDLIST_NODE*));
			if (!pdb->idList->pNodes) {
				return OV_ERR_DBOUTOFMEMORY;
			}
		}
	}

	pdb->idList->relationCount--;
	pdb->idList->maxId = pdb->idList->pLast->maxId;
#if OV_IDLIST_DEBUG
	ov_logfile_debug("release - line: %u - after", __LINE__);
	idList_printstats();
#endif
	return OV_ERR_OK;
}

/*
 *	Create a new database
 *
 * 0                                4096
 * |--------------------------------|----------------/ ... /-------------------|
 * | OV_DATABASE_INFO | ml_info |   |                                          |
 * |--------------------------------|----------------/ ... /-------------------|
 * |<= pdb->baseaddr                |<= pdb->pstart                 pdb->end =>|
 *
 */

OV_DLLFNCEXPORT OV_RESULT ov_database_create(OV_STRING filename, size_t size, /* TODO_ADJUST_WHEN_LARGE_DB */
OV_UINT flags) {
	/*
	 *	local variables
	 */
	OV_RESULT result;
	OV_UINT iterator;

#if !OV_SYSTEM_MC164
	FILE *fp;
#endif
#if OV_SYSTEM_UNIX
	static char null = 0;
#endif
#if OV_SYSTEM_OPENVMS
	struct FAB fab;
	OV_BYTE *inaddr[2], *retaddr[2];
	unsigned long status;
#endif
	/*
	 * update database flags
	 */
	if (flags)
		dbFlags = flags;
	/*
	 *	check parameters
	 */
	if (pdb) {
		return OV_ERR_GENERIC;
	}

	// TODO_ADJUST_WHEN_LARGE_DB
	if (!size || (size > (size_t) OV_DATABASE_MAXSIZE)
			|| (!filename && !(dbFlags & (OV_DBOPT_NOFILE | OV_DBOPT_NOMAP)))) {
		return OV_ERR_BADPARAM;
	}

	if ((dbFlags & OV_DBOPT_VERBOSE)) {
		if (dbFlags & (OV_DBOPT_NOMAP | OV_DBOPT_NOFILE)) {
			ov_logfile_info("Creating database in memory...");
		} else {
			ov_logfile_info("Creating database \"%s\" ...", filename);
		}
	}

	/* Roundup file size */
	size = ov_database_roundupFileSize(size);

	/*
	 * check if we want to create database in memory
	 */
	if (dbFlags & (OV_DBOPT_NOMAP | OV_DBOPT_NOFILE)) {
#if OV_DYNAMIC_DATABASE
		return OV_ERR_BADPARAM;
#endif
		/* Always use malloc to  */
		pdb = malloc(size);
		if (!pdb)
			return OV_ERR_DBOUTOFMEMORY;
	} else {
#if !OV_SYSTEM_MC164
		/*
		 *	check, if file already exists
		 */
		fp = fopen(filename, "r");
		if (fp) {
			fclose(fp);
			/*
			 * if force create is active try to remove old db file
			 */
			if (dbFlags & OV_DBOPT_FORCECREATE) {
				if (remove(filename))
					return OV_ERR_CANTWRITETOFILE;
			} else {
				return OV_ERR_FILEALREADYEXISTS;
			}
		}
#endif
#if OV_SYSTEM_UNIX
		/*
		 *	create the new database file
		 */
		fd = open(filename, O_RDWR | O_CREAT | O_EXCL,
				S_IRUSR | S_IWUSR | S_IRGRP);
		if (fd == -1) {
			return OV_ERR_CANTCREATEFILE;
		}
		/*
		 *	lock the database file
		 */
#if OV_SYSTEM_SOLARIS
		if(flock_solaris(fd, LOCK_EX)) {
			close(fd);
			return OV_ERR_CANTLOCKFILE;
		}
#else
		if (flock(fd, LOCK_EX | LOCK_NB)) {
			close(fd);
			return OV_ERR_CANTLOCKFILE;
		}
#endif
		/*
		 *	make the file "size" bytes long (rounded up)
		 *	TODO_ADJUST_WHEN_LARGE_DB
		 */
		if (lseek(fd, size - sizeof(char), SEEK_SET) < 0) {
			close(fd);
			return OV_ERR_CANTWRITETOFILE;
		}
		if (write(fd, (void*) &null, sizeof(char)) == -1) {
			close(fd);
			return OV_ERR_CANTWRITETOFILE;
		}
		/*
		 *	map the file to memory
		 *	TODO_ADJUST_WHEN_LARGE_DB
		 */
#if OV_ARCH_NOMMU
		pdb = (OV_DATABASE_INFO*) mmap(0, size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
#else
#if OV_DATABASE_FLASH
		pdb = (OV_DATABASE_INFO*)mmap(0, size,
				PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
#else
		pdb = (OV_DATABASE_INFO*) mmap(0, size, PROT_READ | PROT_WRITE,
				MAP_SHARED, fd, 0);
#endif
#endif
		if (pdb == (OV_DATABASE_INFO*) -1) {
			pdb = NULL;
			close(fd);
			return OV_ERR_CANTMAPFILE;
		}

#endif
#if OV_SYSTEM_NT
		/*
		 *	check file size
		 */
		if(size > (size_t) OV_DATABASE_MAXSIZE) {
			return OV_ERR_CANTCREATEFILE;
		}
		/*
		 *	create the new database file
		 *	TODO_ADJUST_WHEN_LARGE_DB
		 */
		hfile = CreateFile(filename, GENERIC_READ | GENERIC_WRITE,
				0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
		if(hfile == INVALID_HANDLE_VALUE) {
			return OV_ERR_CANTCREATEFILE;
		}
		/*
		 *	make the file size approriate (rounded up size)
		 */
		if(SetFilePointer(hfile, size, NULL, FILE_BEGIN)
				== INVALID_SET_FILE_POINTER
				// TODO_ADJUST_WHEN_LARGE_DB
		) {
			CloseHandle(hfile);
			return OV_ERR_CANTWRITETOFILE;
		}
		if(!SetEndOfFile(hfile)) {
			CloseHandle(hfile);
			return OV_ERR_CANTWRITETOFILE;
		}
		/*
		 *	create a file mapping object
		 *	TODO_ADJUST_WHEN_LARGE_DB
		 */
#if OV_DYNAMIC_DATABASE
		hmap = CreateFileMapping(hfile, NULL, PAGE_READWRITE,
				0, OV_DATABASE_MAXSIZE, NULL);
#else
		hmap = CreateFileMapping(hfile, NULL, PAGE_READWRITE,
				0, size, NULL);
#endif
		if(!hmap) {
			CloseHandle(hfile);
			return OV_ERR_CANTMAPFILE;
		}
		/*
		 *	map the file to memory
		 */
		pdb = (OV_DATABASE_INFO*)MapViewOfFile(hmap,
				FILE_MAP_ALL_ACCESS, 0, 0, 0);
		if(!pdb) {
			CloseHandle(hmap);
			CloseHandle(hfile);
			return OV_ERR_CANTMAPFILE;
		}
#endif
#if OV_SYSTEM_MC164
		pdb = pdbmem;
#endif
#if OV_SYSTEM_RMOS
		/*
		 *	get database memory
		 */
		pdb = (OV_DATABASE_INFO*)malloc(size);
		if(!pdb) {
			return OV_ERR_HEAPOUTOFMEMORY;
		}
		file = fopen(filename, "wb+");
		if(!file) {
			free(pdb);
			return OV_ERR_CANTCREATEFILE;
		}
#endif
#if OV_SYSTEM_OPENVMS
		/*
		 *	initialize file access block (FAB)
		 */
		fab = cc$rms_fab;
		fab.fab$v_ufo = 1; /* user file open */
		fab.fab$l_fna = filename; /* filename address */
		fab.fab$b_fns = strlen(filename); /* filename size */
		fab.fab$l_alq = size >> 9; /* Allocation quantity (file size) */
		/*
		 *	create database file if none exists
		 */
		status = sys$create(&fab);
		if(!(status & 1)) {
			return OV_ERR_CANTCREATEFILE;
		}
		/*
		 *	get channel from the FAB
		 */
		channel = fab.fab$l_stv;
		/*
		 *	initialize address hint: just an address in P0 space
		 */
		inaddr[0] = inaddr[1] = NULL;
		/*
		 *	map the file to memory
		 */
		status = sys$crmpsc(inaddr, retaddr, 0, SEC$M_WRT | SEC$M_DZRO | SEC$M_EXPREG,
				0, 0, 0, channel, 0, 0, 0, 0);
		if(!(status & 1)) {
			sys$dassgn(channel);
			return OV_ERR_CANTMAPFILE;
		}
		/*
		 *	ensure that we are the first opener of the database file
		 */
		if(status != SS$_CREATED) {
			ov_database_unload();
			return OV_ERR_CANTCREATEFILE;
		}
		/*
		 *	assign database pointer
		 */
		Ov_AbortIfNot(size == retaddr[1]-retaddr[0]+1);
		pdb = (OV_DATABASE_INFO*)retaddr[0];
#endif
	}
	/*
	 *	initialize database structure
	 */
	Ov_AbortIfNot(pdb);
#if OV_DYNAMIC_DATABASE
	memset(pdb, 0, sizeof(OV_DATABASE_INFO));
#else
	memset(pdb, 0, size);
#endif
	pdb->baseaddr = (OV_POINTER) pdb;
	pdb->size = size;
#if TLSF
   pdb->pstart = pdb->pcurr = (OV_BYTE*) ((OV_BYTE*) pdb->baseaddr
			+ Ov_Roundup(sizeof(OV_DATABASE_INFO), 4*sizeof(void*)));
#else
	pdb->pstart = pdb->pcurr = (OV_BYTE*) ((OV_BYTE*) pdb->baseaddr
			+ BLOCKIFY(sizeof(OV_DATABASE_INFO) + sizeof(ml_info)) * BLOCKSIZE);
#endif
	pdb->pend = (OV_BYTE*) ((OV_BYTE*) pdb + size);
	pdb->started = FALSE;

	/*
	 *	initialize the database memory mempool
	 *
	 */
#if TLSF
	dbpool= pdb->pstart;

#if OV_SYSTEM_UNIX
	if (mlockall(MCL_CURRENT | MCL_FUTURE )){
		perror("mlockall failed:");
	}

	size_t page_size = sysconf(_SC_PAGESIZE);

	struct rusage usage;
	for (size_t i=0; (OV_BYTE*)dbpool+i < pdb->pend; i+=page_size)
	{
		((OV_BYTE*)dbpool)[i] = 0;
		getrusage(RUSAGE_SELF, &usage);
	}
#endif

	init_memory_pool(pdb->pend-pdb->pstart,dbpool);
	tlsf_set_pool(ov_database, dbpool);
	tlsf_set_static(TRUE, dbpool);
#else
	if(!ml_initialize(pmpinfo, pdb->pstart, ov_database_morecore)) {
		ov_database_unload();
		return OV_ERR_DBOUTOFMEMORY;
	}
#endif
	/*
	 *	initialize idCounter
	 */
	pdb->idCounter0 = 0;
	pdb->idCounter1 = 0;

	/*
	 *	initialize the idLIst
	 */
	pdb->idList = ov_database_malloc(sizeof(OV_IDLIST_HEADNODE));
	Ov_AbortIfNot(pdb->idList);
	pdb->idList->minId = 0;
	pdb->idList->maxId = 0;
	pdb->idList->pFirst = ov_database_malloc(sizeof(OV_IDLIST_NODE));
	Ov_AbortIfNot(pdb->idList->pFirst);
	pdb->idList->pLast = pdb->idList->pFirst;
	pdb->idList->nodeCount = 1;
	pdb->idList->pFirst->minId = 0;
	pdb->idList->pFirst->maxId = 0;
	pdb->idList->pFirst->pPrevious = NULL;
	pdb->idList->pFirst->pNext = NULL;
	pdb->idList->pFirst->nodeNumber = 1;
	pdb->idList->pFirst->relationCount = 0;
	for (iterator = 0; iterator < OV_IDLIST_RELATIONSPERCHUNK; iterator++) {
		pdb->idList->pFirst->relations[iterator].idLow = 0;
		pdb->idList->pFirst->relations[iterator].idHigh = 0;
		pdb->idList->pFirst->relations[iterator].pobj = NULL;
	}
	pdb->idList->pNodes = ov_database_malloc(
			OV_IDLIST_NODELISTCHUNKSIZE * sizeof(OV_IDLIST_NODE*));
	Ov_AbortIfNot(pdb->idList->pNodes);
	pdb->idList->pNodes[0] = pdb->idList->pFirst;
	for (iterator = 1; iterator < OV_IDLIST_NODELISTCHUNKSIZE; iterator++) {
		pdb->idList->pNodes[iterator] = NULL;
	}

	/*
	 *	initialize the root domain object
	 */
	Ov_AbortIfNot(
			Ov_OK(ov_string_setvalue(&pdb->root.v_identifier, OV_OBJNAME_ROOT)));
	ov_time_gettime(&pdb->root.v_creationtime);
	/*	as this is the first getId call for this database it will return 0 0	*/
	ov_database_getId(&(pdb->root.v_idH), &(pdb->root.v_idL));
	ov_database_idListInsert(pdb->root.v_idH, pdb->root.v_idL,
			Ov_PtrUpCast(ov_object, &(pdb->root)));
	/*	call get id once more to reserve 0 1 for the vendortree (has to be set hard coded as it is not stored in the database)	*/
	ov_database_getId(NULL, NULL); // call with NULL-pointers just increments the counter
	ov_database_idListInsert(0, 1, Ov_PtrUpCast(ov_object, &(pdb->vendordom)));
	/*
	 *	initialize the acplt domain object
	 */
	Ov_AbortIfNot(
			Ov_OK(ov_string_setvalue(&pdb->acplt.v_identifier, OV_OBJNAME_ACPLT)));
	ov_time_gettime(&pdb->acplt.v_creationtime);
	ov_database_getId(&(pdb->acplt.v_idH), &(pdb->acplt.v_idL));
	ov_database_idListInsert(pdb->acplt.v_idH, pdb->acplt.v_idL,
			Ov_PtrUpCast(ov_object, &(pdb->acplt)));
	/*
	 *	initialize the OV library object
	 */
	Ov_AbortIfNot(
			Ov_OK(ov_string_setvalue(&pdb->ov.v_identifier, OV_OBJNAME_OV)));
	ov_time_gettime(&pdb->ov.v_creationtime);
	ov_database_getId(&(pdb->ov.v_idH), &(pdb->ov.v_idL));
	ov_database_idListInsert(pdb->ov.v_idH, pdb->ov.v_idL,
			Ov_PtrUpCast(ov_object, &(pdb->ov)));
	/*
	 *	load the OV library
	 */
	result = ov_library_load(&pdb->ov, &OV_LIBRARY_DEF_ov);
	if (Ov_Fail(result)) {
		ov_database_unload();
		return result;
	}
	/*
	 *	initialize the vendor tree
	 */
	ov_vendortree_setdatabasename(filename);
	Ov_AbortIfNot(Ov_OK(ov_vendortree_init()));
#if OV_SYSTEM_RMOS
	/*
	 *	flush the database file
	 */
	ov_database_flush();
#endif

#if OV_ARCH_NOMMU

	ov_database_flush();

#endif

	if ((dbFlags & OV_DBOPT_VERBOSE)) {
		ov_logfile_info("Database created.");
	}

	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
 *	load an existing database part 1
 */
OV_DLLFNCEXPORT OV_RESULT ov_database_loadfile(OV_STRING filename) {
	/*
	 *	local variables
	 */
	OV_RESULT result;
	OV_POINTER baseaddr = NULL;
	size_t size = 0;
#if OV_SYSTEM_NT
	DWORD bytesread;
#endif
#if OV_SYSTEM_OPENVMS
	struct FAB fab;
	OV_BYTE *inaddr[2], *retaddr[2];
	unsigned long status;
#endif
	/*
	 *	check parameters
	 */
	if (pdb) {
		return OV_ERR_GENERIC;
	}
	if (!filename) {
		return OV_ERR_BADPARAM;
	}

	if ((dbFlags & OV_DBOPT_VERBOSE)) {
		if (!(dbFlags & OV_DBOPT_BACKUP)) {
			if (dbFlags & OV_DBOPT_NOMAP) {
				ov_logfile_info("Loading database \"%s\" into memory...",
						filename);
			} else {
				ov_logfile_info("Mapping database \"%s\"...", filename);
			}
		} else {
			if (dbFlags & OV_DBOPT_NOMAP) {
				ov_logfile_info("Loading backup-database \"%s\" into memory...",
						filename);
			} else {
				ov_logfile_info("Mapping backup-database \"%s\"...", filename);
			}
		}
	}

#if OV_SYSTEM_UNIX
	/*
	 *	open the database file
	 */
	if (dbFlags & OV_DBOPT_NOMAP) {
		fd = open((const char*) filename, O_RDONLY);
	} else {
		fd = open((const char*)filename, O_RDWR);
	}
	if (fd == -1) {
		return OV_ERR_CANTOPENFILE;
	}
	/*
	 *	lock the database file
	 */
#if OV_SYSTEM_SOLARIS
	if(flock_solaris(fd, LOCK_EX)) {
		close(fd);
		return OV_ERR_CANTLOCKFILE;
	}
#else 
	if (flock(fd, LOCK_EX | LOCK_NB)) {
		close(fd);
		return OV_ERR_CANTLOCKFILE;
	}
#endif
	/*
	 *	read the base address
	 */
	if (read(fd, (void*) &baseaddr, sizeof(OV_POINTER)) == -1) {
		close(fd);
		return OV_ERR_CANTREADFROMFILE;
	}
	/*
	 *	read the filemapping size
	 */
	if (read(fd, (void*) &size, sizeof(size)) == -1) {
		close(fd);
		return OV_ERR_CANTREADFROMFILE;
	}

	if (dbFlags & OV_DBOPT_NOMAP) {

		OV_UINT pos = 0;
		OV_UINT nread = 0;

		pdb = malloc(size);
		if (!pdb)
			return OV_ERR_HEAPOUTOFMEMORY;

		lseek(fd, 0, SEEK_SET);

		do {
			nread = read(fd, ((OV_BYTE*) pdb) + pos, size - pos);
			pos += nread;
		} while (pos < size && nread);

		close(fd);
		fd = 0;

	} else {
		/*
		 *	map the file to memory
		 */
#if OV_DATABASE_FLASH | OV_ARCH_NOMMU
		pdb = (OV_DATABASE_INFO*)mmap(0, size,
				PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
#else
		pdb = (OV_DATABASE_INFO*) mmap(baseaddr, size, PROT_READ | PROT_WRITE,
				MAP_SHARED, fd, 0);
#endif
		if (pdb == (OV_DATABASE_INFO*) -1) {
			pdb = NULL;
			close(fd);
			return OV_ERR_CANTMAPFILE;
		}
	}
#endif
#if OV_SYSTEM_NT
	/*
	 *	open the database file
	 */
	if(dbFlags&OV_DBOPT_NOMAP) {
		hfile = CreateFile((const char*)filename, GENERIC_READ,
				0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	} else {
		hfile = CreateFile((const char*)filename, GENERIC_READ | GENERIC_WRITE,
				0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	}
	if(hfile == INVALID_HANDLE_VALUE) {
		return OV_ERR_CANTOPENFILE;
	}

	/*
	 *	read the base address
	 */
	if(!ReadFile(hfile, (LPVOID)&baseaddr, sizeof(OV_POINTER),
					&bytesread, NULL)
	) {
		CloseHandle(hfile);
		return OV_ERR_CANTREADFROMFILE;
	}
	/*
	 *	read the filemapping size
	 */
	if(!ReadFile(hfile, (LPVOID)&size, sizeof(size),
					&bytesread, NULL)
	) {
		CloseHandle(hfile);
		return OV_ERR_CANTREADFROMFILE;
	}
	/*
	 *	check file size
	 */
	if(GetFileSize(hfile, NULL) != size) {
		CloseHandle(hfile);
		return OV_ERR_BADDATABASE;
	}
	if(dbFlags&OV_DBOPT_NOMAP) {

		if(SetFilePointer(hfile, 0, NULL, FILE_BEGIN)==INVALID_SET_FILE_POINTER)
			return OV_ERR_CANTREADFROMFILE;

		pdb = malloc(size);
		if(!pdb)
			return OV_ERR_DBOUTOFMEMORY;

		if(!ReadFile(hfile, (LPVOID)pdb, size, &bytesread, NULL)) {
			free(pdb);
			pdb = NULL;
			return OV_ERR_CANTREADFROMFILE;
		}

		CloseHandle(hfile);
		hfile = NULL;

	} else {
		/*
		 *	create a file mapping object
		 */
#if OV_DYNAMIC_DATABASE
		hmap = CreateFileMapping(hfile, NULL, PAGE_READWRITE,
				0, OV_DATABASE_MAXSIZE, NULL);
#else
		hmap = CreateFileMapping(hfile, NULL, PAGE_READWRITE,
				0, size, NULL);
#endif
		if(!hmap) {
			CloseHandle(hfile);
			return OV_ERR_CANTMAPFILE;
		}
		/*
		 *	map the file to memory
		 */
		pdb = (OV_DATABASE_INFO*)MapViewOfFileEx(hmap,
				FILE_MAP_ALL_ACCESS, 0, 0, 0, (LPVOID)baseaddr);
		if(!pdb) {
			pdb = (OV_DATABASE_INFO*)MapViewOfFile(hmap,
					FILE_MAP_ALL_ACCESS, 0, 0, 0);
			if(!pdb) {
				CloseHandle(hmap);
				CloseHandle(hfile);
				return OV_ERR_CANTMAPFILE;
			}
		}
	}
#endif
#if OV_SYSTEM_MC164
	pdb = pdbmem;
#endif
#if OV_SYSTEM_RMOS
	/*
	 *	open the database file
	 */
	file = fopen((const char*)filename, "rb+");
	if(!file) {
		return OV_ERR_CANTOPENFILE;
	}
	/*
	 *	read the base address
	 */
	if(fread((char*)&baseaddr, sizeof(OV_POINTER), 1, file) != 1) {
		fclose(file);
		return OV_ERR_CANTREADFROMFILE;
	}
	/*
	 *	read the filemapping size
	 */
	if(fread((char*)&size, sizeof(size), 1, file) != 1) {
		fclose(file);
		return OV_ERR_CANTREADFROMFILE;
	}
	/*
	 *	get database memory
	 */
	pdb = (OV_DATABASE_INFO*)malloc(size);
	if(!pdb) {
		return OV_ERR_HEAPOUTOFMEMORY;
	}
	/*
	 *	read the file to memory
	 */
	if(fseek(file, 0, SEEK_SET) < 0) {
		free(pdb);
		fclose(file);
		return OV_ERR_CANTREADFROMFILE;
	}
	if(fread((char*)pdb, size, 1, file) != 1) {
		free(pdb);
		fclose(file);
		return OV_ERR_CANTREADFROMFILE;
	}
#endif
#if OV_SYSTEM_OPENVMS
	/*
	 *	initialize FAB
	 */
	fab = cc$rms_fab;
	fab.fab$v_ufo = 1; /* user file open */
	fab.fab$l_fna = (const char*) filename; /* filename address */
	fab.fab$b_fns = strlen(filename); /* filename size */
	fab.fab$v_upd = 1; /* update access */
	/*
	 *	open database file
	 */
	status = sys$open(&fab);
	if(!(status & 1)) {
		return OV_ERR_CANTOPENFILE;
	}
	/*
	 *	get channel from the FAB
	 */
	channel = fab.fab$l_stv;
	/*
	 *	initialize address hint: just an address in P0 space
	 */
	inaddr[0] = inaddr[1] = NULL;
	/*
	 *	map the file to memory
	 */
	status = sys$crmpsc(inaddr, retaddr, 0, SEC$M_WRT | SEC$M_EXPREG,
			0, 0, 0, channel, 0, 0, 0, 0);
	if(!(status & 1)) {
		sys$dassgn(channel);
		return OV_ERR_CANTMAPFILE;
	}
#if 0	/* how??? */
	/*
	 *	ensure that we are the first opener of the database file
	 */
	if(status != SS$_CREATED) {
		ov_database_unload();
		return OV_ERR_CANTOPENFILE;
	}
#endif
	/*
	 *	assign database pointer and database size
	 */
	pdb = (OV_DATABASE_INFO*)retaddr[0];
	size = retaddr[1]-retaddr[0]+1;
#endif

	/*
	 *	test if we got the same filemapping size
	 */
	if (!pdb) {
		ov_database_unload();
		return OV_ERR_BADDATABASE;
	}
#if OV_SYSTEM_MC164
	/* nothing to do */
#else
	if (size != pdb->size) {
		ov_database_unload();
		return OV_ERR_BADDATABASE;
	}
#endif
	/*
	 *	test if we got the same base address
	 */
	if ((OV_POINTER) pdb != pdb->baseaddr) {
		result = ov_database_move((OV_BYTE*) pdb - (OV_BYTE*) (pdb->baseaddr));
		if (Ov_Fail(result)) {
			ov_database_unload();
			return result;
		}
	}
#if TLSF
	dbpool = pdb->pstart;
	tlsf_set_pool(ov_database, dbpool);
	init_memory_pool((size_t)(pdb->pend-pdb->pstart), dbpool);
#else
	/*
	 *	restart the database memory mempool
	 */
	if (!ml_restart(pmpinfo, pdb->pstart, ov_database_morecore)) {
		ov_database_unload();
		return OV_ERR_BADDATABASE;
	}
#endif

	if ((dbFlags & OV_DBOPT_VERBOSE)) {
		if (dbFlags & OV_DBOPT_NOMAP) {
			ov_logfile_info("Database loaded.");
		} else {
			ov_logfile_info("Database mapped.");
		}
	}

	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
 *	load an existing database part 2
 */
OV_DLLFNCEXPORT OV_RESULT ov_database_loadlib(OV_STRING filename) {
	/*
	 *	local variables
	 */
	OV_RESULT result;
	/*
	 *	load the OV library
	 */
	result = ov_library_compare(&pdb->ov, &OV_LIBRARY_DEF_ov);
	if (Ov_Fail(result)) {
		return result;
	}
	/*
	 *	initialize the database
	 */
	ov_database_init();
	/*
	 *	finally initialize the vendor tree
	 */
	ov_vendortree_setdatabasename(filename);
	return result;
}

/*	----------------------------------------------------------------------	*/

/*
 *	Unmap the database
 */
OV_DLLFNCEXPORT void ov_database_unload(void) {
	/*
	 *	local variables
	 */
#if OV_SYSTEM_OPENVMS
	OV_BYTE *inaddr[2];
#endif
	/*
	 *	instructions
	 */
	/*
	 *	shut down database if not already done
	 */
	if (pdb)
		ov_database_shutdown();

	// mark reachable pointer as freed
	ov_freelist_free();

	if (dbFlags & (OV_DBOPT_NOMAP | OV_DBOPT_NOFILE)) {
#if TLSF
		destroy_memory_pool(dbpool);
#endif
		free(pdb);
	} else {
		if (dbFlags & OV_DBOPT_VERBOSE) {
			ov_logfile_info("Unmapping database ...");
		}
#if OV_SYSTEM_UNIX
		/*
		 *	unmap the file and close it
		 */
#if OV_SYSTEM_SOLARIS
		if (pdb) munmap((caddr_t )pdb, pdb->size);
#else
		if (pdb)
			munmap(pdb, pdb->size);
#endif
		if (fd)
			close(fd);
#endif
#if OV_SYSTEM_NT
		/*
		 *	unmap the file and close all handles
		 */
		if (pdb) UnmapViewOfFile(pdb);
		if (hmap) CloseHandle(hmap);
		if (hfile) CloseHandle(hfile);
#endif
#if OV_SYSTEM_RMOS
		/*
		 *	flush the database file, close it and free the database memory
		 */
		if (pdb) ov_database_flush();
		if (file) fclose(file);
		if (pdb) free(pdb);
#endif
#if OV_SYSTEM_OPENVMS
		/*
		 *	delete virtual address space and close the channel
		 */
		if (pdb) {
			inaddr[0] = (OV_BYTE*)pdb;
			inaddr[1] = inaddr[0]+pdb->size-1;
			sys$deltva(inaddr, 0, 0);
		}
		if (channel) sys$dassgn(channel);
#endif
	}

	pdb = NULL;

	if ((dbFlags & OV_DBOPT_VERBOSE)) {
		if (dbFlags & (OV_DBOPT_NOMAP | OV_DBOPT_NOFILE)) {
			ov_logfile_info("Database freed.");
		} else {
			ov_logfile_info("Database unmapped.");
		}
	}

	ov_vendortree_setdatabasename(NULL);
}

/*	----------------------------------------------------------------------	*/

/*
 *	Flush the contents of a database
 */
OV_DLLFNCEXPORT void ov_database_flush(void) {
	/*
	 *	local variables
	 */

	if (dbFlags & (OV_DBOPT_NOMAP | OV_DBOPT_NOFILE))
		return;

#if OV_SYSTEM_OPENVMS
	OV_BYTE *inaddr[2];
#endif

	/*
	 *	instructions
	 */
	if (pdb) {
#if OV_ARCH_NOMMU
		if(lseek(fd, 0, SEEK_SET) < 0)
		{
			Ov_Warning("can't flush database");
			return;
		}
		if(write(fd, (void*) pdb, pdb->size) == -1)
		{
			Ov_Warning("can't flush database");
			return;
		}
#endif
#if OV_SYSTEM_UNIX
		msync((void*) pdb, pdb->size, MS_SYNC);
#endif
#if OV_SYSTEM_NT
		FlushViewOfFile((LPCVOID)pdb, 0);
#endif
#if OV_SYSTEM_RMOS
		/*
		 *	write the database memory to the file
		 */
		if(fseek(file, 0, SEEK_SET) < 0) {
			Ov_Warning("can't flush database");
			return;
		}
		if(fwrite((char*)pdb, pdb->size, 1, file) != 1) {
			Ov_Warning("can't flush database");
			return;
		}
		if(fflush(file)) {
			Ov_Warning("can't flush database");
			return;
		}
#endif
#if OV_SYSTEM_OPENVMS
		/*
		 *	synchronously flush the virtual address space
		 */
		inaddr[0] = (OV_BYTE*)pdb;
		inaddr[1] = inaddr[0]+pdb->size-1;
		sys$updsecw(inaddr, 0, 0, 0, 0, 0, 0, 0);
#endif
	}
}
/*	----------------------------------------------------------------------	*/

/*
 *	Write database to backupfile
 */
OV_DLLFNCEXPORT OV_RESULT ov_database_write(OV_STRING dbname) {

	FILE *backupfile;

	if (!dbname)
		return OV_ERR_BADPARAM;
	backupfile = fopen(dbname, "wb+");
	if (!backupfile) {
		return OV_ERR_CANTOPENFILE;
	}
	fseek(backupfile, 0, SEEK_SET);
	fwrite((char*) pdb, pdb->size, 1, backupfile);
	fclose(backupfile);
	return OV_ERR_OK;
}
/*	----------------------------------------------------------------------	*/

/*
 *	Load database
 */
OV_DLLFNCEXPORT OV_RESULT ov_database_load(OV_STRING filename, size_t size,
		OV_UINT flags) {
	dbFlags = flags;
	OV_RESULT result;

	if (ov_string_compare(filename, "-") == OV_STRCMP_EQUAL) {
		dbFlags |= OV_DBOPT_NOFILE;
	}

#if OV_DYNAMIC_DATABASE
	/*
	 * dynamic database needs mapped file
	 */
	if(dbFlags&(OV_DBOPT_NOFILE|OV_DBOPT_NOMAP))
	return OV_ERR_BADPARAM;
#endif

#if !(OV_SYSTEM_UNIX || OV_SYSTEM_NT)
	/*
	 * NOFILE and NOMAP only supported for Unix and NT
	 */
	if(dbFlags&(OV_DBOPT_NOFILE|OV_DBOPT_NOMAP))
	return OV_ERR_BADPARAM;
#endif

	if ((dbFlags & OV_DBOPT_FORCECREATE) || (dbFlags & OV_DBOPT_NOFILE)) {
		result = ov_database_create(filename, size, 0);
	} else {
		result = ov_database_loadfile(filename);
		if (Ov_Fail(result))
			return result;
		result = ov_database_loadlib(filename);
	}

	return result;
}
/*	----------------------------------------------------------------------	*/

/*
 *	Initialize the database (subroutine)
 */
void ov_database_init(void) {
	/*
	 *	local variables
	 */
	OV_INSTPTR_ov_library plib;
	OV_INSTPTR_ov_association passoc;
	OV_INSTPTR_ov_class pclass;
	OV_INSTPTR_ov_variable pvar;
	/*
	 *	iterate over all libraries except the OV library
	 */Ov_ForEachChildEx(ov_instantiation, pclass_ov_library, plib, ov_library) {
		if (plib != &pdb->ov) {
			/*
			 *	Set the DLL/shared library handle to zero
			 */
			plib->v_handle = 0;
			/*
			 *	Iterate over all associations of the library
			 */Ov_ForEachChildEx(ov_containment, plib, passoc, ov_association) {
				/*
				 *	Set function pointers to NULL
				 */
				passoc->v_linkfnc = NULL;
				passoc->v_unlinkfnc = NULL;
				passoc->v_getaccessfnc = NULL;
			} /* associations */
			/*
			 *	Iterate over all classes of the library
			 */
			Ov_ForEachChildEx(ov_containment, plib, pclass, ov_class) {
				/*
				 *	Set the vtable pointer to the "nostartup" vtable pointer
				 */
				pclass->v_pvtable = &nostartupvtable;
				/*
				 *	Iterate for all variables of the class
				 */
				Ov_ForEachChildEx(ov_containment, pclass, pvar, ov_variable) {
					/*
					 *	Set function pointers to NULL
					 */
					pvar->v_setfnc = NULL;
					pvar->v_getfnc = NULL;
				} /* variables */
			} /* classes */
		} /* if */
	} /* libraries */
	/*
	 *	library is not started up
	 */
	pdb->started = FALSE;
}

/*	----------------------------------------------------------------------	*/

/*
 *	Start up the database
 */
OV_DLLFNCEXPORT OV_RESULT ov_database_startup(void) {
	/*
	 *	local variables
	 */
	OV_INSTPTR_ov_library plib;
	OV_LIBRARY_DEF *plibdef;
	OV_RESULT result;
	/*
	 *	don't start if already started
	 */
	if (pdb->started) {
		return OV_ERR_GENERIC;
	}

	if ((pdb->root.v_objectstate & OV_OS_STARTED)) {
		ov_logfile_warning(
				"Trying to start database with root object marked as started. Probably the database was NOT shut down properly before and is corrupted. Will clear the flag and abort. The next try to start the database will succeed, but the server may crash.");
		pdb->root.v_objectstate &= ~OV_OS_STARTED;
		return OV_ERR_BADDATABASE;
	}

	if (dbFlags & OV_DBOPT_VERBOSE) {
		ov_logfile_info("Starting up database...");
	}

	/*
	 *	open and compare all libraries
	 */Ov_ForEachChildEx(ov_instantiation, pclass_ov_library, plib, ov_library) {
		plibdef = ov_library_open(plib);
		if (!plibdef) {
			return OV_ERR_CANTOPENLIBRARY;
		}
		result = ov_library_compare(plib, plibdef);
		if (Ov_Fail(result)) {
			ov_database_init();
			return result;
		}
	}
	/*
	 *	call startup method of root object
	 */
	ov_object_startup(Ov_PtrUpCast(ov_object, &pdb->root));
	/*
	 *	library is started
	 */
	if (dbFlags & OV_DBOPT_VERBOSE) {
		ov_logfile_info("Database started up.");
	} database_mutex_init();
	pdb->started = TRUE;
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
 *	Shut down the database
 */
OV_DLLFNCEXPORT void ov_database_shutdown(void) {

	/*
	 *	local variables
	 */
	OV_INSTPTR_ov_library plib;
	/*
	 *	shut down database
	 */
	if (pdb->started) {
		if (dbFlags & OV_DBOPT_VERBOSE) {
			ov_logfile_info("Shutting down database...");
		}
		/*
		 *	Flush the contents of a database
		 */
		ov_database_flush();
		/*
		 *	call shutdown method of root object
		 */
		ov_object_shutdown(Ov_PtrUpCast(ov_object, &pdb->root));
		/*
		 *	close all libraries
		 */Ov_ForEachChildEx(ov_instantiation, pclass_ov_library, plib, ov_library) {
			ov_library_close(plib);
		} database_mutex_destroy();
		pdb->started = FALSE;

		if (dbFlags & OV_DBOPT_VERBOSE) {
			ov_logfile_info("Database shut down.");
		}
	}
}

/*	----------------------------------------------------------------------	*/

/*
 *	Allocate memory in the database
 */
OV_DLLFNCEXPORT OV_POINTER ov_database_malloc(size_t size) {
#if !TLSF
	__ml_ptr ptmp = NULL;
#endif

#ifdef OV_VALGRIND
	if (ov_path_getobjectpointer("/acplt/malloc", 2)) {
		return malloc(size);
	}
#endif

	if (pdb) {
#if TLSF
		return tlsf_malloc(size, ov_database);
#else
		database_mutex_lock();
		ptmp = ml_malloc(size);
		database_mutex_unlock();
		return ptmp;
#endif
	}
	return NULL;
}

/*	----------------------------------------------------------------------	*/

/*
 *	Reallocate memory in the database
 */
OV_DLLFNCEXPORT OV_POINTER ov_database_realloc(OV_POINTER ptr, size_t size) {
#if !TLSF
	__ml_ptr ptmp = NULL;
#endif

#ifdef OV_VALGRIND
	if(pdb && ptr != 0 && ov_path_getobjectpointer("/acplt/malloc", 2)==NULL) {
		if(!((uintptr_t)ptr >= (uintptr_t)pdb->baseaddr && (uintptr_t)ptr <= (uintptr_t)pdb->baseaddr+(uintptr_t)pdb->size)) {
			printf("realloc missed database, install a breakpoint here\n");
		}
	}
	if (ov_path_getobjectpointer("/acplt/malloc", 2)) {
		if(ptr == 0 || !pdb || ptr < pdb->baseaddr || ptr > pdb->baseaddr+pdb->size) {
			return realloc(ptr, size);
		}
	}
#endif

	if (pdb) {
#if TLSF
		return tlsf_realloc(ptr, size, ov_database);
#else
		database_mutex_lock();
		ptmp = ml_realloc(ptr, size);
		database_mutex_unlock();
		return ptmp;
#endif
	}
	return NULL;
}

/*	----------------------------------------------------------------------	*/

/*
 *	Free memory in the database
 */
OV_DLLFNCEXPORT void ov_database_free(OV_POINTER ptr) {
#ifdef OV_VALGRIND
	if(pdb && ptr != 0 && ov_path_getobjectpointer("/acplt/malloc", 2)==NULL) {
		if(!((uintptr_t)ptr >= (uintptr_t)pdb->baseaddr && (uintptr_t)ptr <= (uintptr_t)pdb->baseaddr+(uintptr_t)pdb->size)) {
			printf("free missed database, install a breakpoint here\n");
		}
	}
	if (ov_path_getobjectpointer("/acplt/malloc", 2)) {
		if(ptr == NULL || !pdb || ptr < pdb->baseaddr || ptr > pdb->baseaddr+pdb->size) {
			free(ptr);
			return;
		}
	}
#endif

	if (pdb) {
#if TLSF
		tlsf_free(ptr, ov_database);
#else
		database_mutex_lock();
		ml_free(ptr);
		database_mutex_unlock();
#endif
	}

}

/*	----------------------------------------------------------------------	*/

/*
 *	Get size of the database
 */
OV_DLLFNCEXPORT OV_UINT ov_database_getsize(void) {

	if (pdb) {
#if TLSF
		return get_pool_size(dbpool);
#else
		return pdb->size;
#endif
	}
	return 0;
}

/*	----------------------------------------------------------------------	*/

/*
 *	Get free storage in the database
 */
OV_DLLFNCEXPORT OV_UINT ov_database_getfree(void) {

	if (pdb) {

#ifdef _STDINT_H
		uintptr_t free;
#else
		OV_UINT free;
#define uintptr_t OV_UINT
#endif
#if TLSF
		free = get_free_size(dbpool);
#else
		database_mutex_lock();
		free = (uintptr_t) pdb->pend - (uintptr_t) pdb->pcurr
				+ (uintptr_t)pmpinfo->bytes_free;
		database_mutex_unlock();
#endif

		if (free > OV_VL_MAXUINT) {
			return OV_VL_MAXUINT;
		}
		return (OV_UINT)(free);
	}
	return 0;
}

/*	----------------------------------------------------------------------	*/

/*
 *	Get used storage in the database
 */
OV_DLLFNCEXPORT OV_UINT ov_database_getused(void) {

#ifdef _STDINT_H
	uintptr_t used;
#else
	OV_UINT used;
#endif
	if (pdb) {
#if TLSF
		used = get_used_size(dbpool);
#else
		database_mutex_lock();
		used = (uintptr_t) pdb->pstart - (uintptr_t) pdb->baseaddr
				+ (uintptr_t)pmpinfo->bytes_used
				- (uintptr_t)pmpinfo->bytes_free;
		database_mutex_unlock();
#endif
		if (used > OV_VL_MAXUINT) {
			return OV_VL_MAXUINT;
		}
		return (OV_UINT)(used);
	}
	return 0;
}

/*	----------------------------------------------------------------------	*/

/*
 *	Get fragmentation of the database
 */
OV_DLLFNCEXPORT OV_DOUBLE ov_database_getfrag(void) {

#if TLSF
	return (get_fragmentation(dbpool) * 100.0);
#else
	/*
	 *	local variables
	 */
	OV_UINT i, num;
	size_t sumsize, avgsize, logavgsize, sum;
	OV_INSTPTR_ov_class pclass;
	struct __ml_list OV_MEMSPEC *pnext;
	/*
	 *	calculate average instance size
	 */
	if (pdb) {
		num = sumsize = 0;
		Ov_ForEachChildEx(ov_instantiation, pclass_ov_class, pclass, ov_class) {
			num++;
			sumsize += pclass->v_size;
		}
		avgsize = sumsize / num;
		/*
		 *	calculate the log (base of 2) of the average instance size
		 */
		for (logavgsize = 0; avgsize; avgsize >>= 1) {
			logavgsize++;
		}
		avgsize = sumsize / num;
		/*
		 *	find out how many blocks of the average size there are
		 */
		sum = 0;
		database_mutex_lock();
		for (i = logavgsize; i < BLOCKLOG; i++) {
			num = 0;
			for (pnext = pmpinfo->fraghead[i].next; pnext; pnext =
					pnext->next) {
				num++;
			}
			sum += num * (1 << (i - logavgsize));
		} database_mutex_unlock();
		sum += (pdb->pend - pdb->pcurr) / avgsize;
		return ((ov_database_getfree() - sum * avgsize) * 100)
				/ ov_database_getfree();
	}
	return 100;
#endif
}

/*	----------------------------------------------------------------------	*/

/*
 *	Helper macros for moving the database to a new base address
 */
#define Ov_Adjust(type, ptr) 											\
	ptr = (type)(((ptr)?(((OV_BYTE*)(ptr))+distance):(NULL)))

/*	----------------------------------------------------------------------	*/

/*
 *	Move the database to a new base address (subroutine)
 *
 *	When using linktables, you can't access the links without adjusting the linktablepointer before.
 *	Also the association offsets in this linktable may have been changed (in comparison to the standard
 *	offsets you get by using 'ov_association_getparentoffset' and 'ov_association_getchildoffset')
 *	in a mapped database. So at least we need the knowledge about the associations 'containment' and
 *	'instantiation' which are stored in the database info object . We will search the 'ov' domain
 *	for a contained object 'association' and will then look for it's instances, hence the other ov-associations.
 *	After this we can use the standard ov_object_move, which uses the knowledge about the associations in the ov-model.
 *
 */
OV_RESULT ov_database_move(const OV_PTRDIFF distance) {
	//return OV_ERR_OK; /* not implemented */

	/*
	 *	local variables
	 */
	OV_DATABASE_INFO *pdbcopy;
	OV_RESULT result = OV_ERR_OK;
	OV_INSTPTR_ov_object pobj;
	OV_INST_ov_association assoc_inheritance;
	OV_INST_ov_association assoc_childrelationship;
	OV_INST_ov_association assoc_parentrelationship;
	OV_IDLIST_NODE *pCurrNode = NULL;
	OV_UINT iterator;
#if !TLSF
	int i;
	struct __ml_list *phead;
#endif

	/*
	 *	search inheritance, childrelationship, parentrelationship association
	 */
	passoc_ov_containment = &pdb->containment;
	passoc_ov_instantiation = &pdb->instantiation;

	assoc_inheritance.v_assoctype = OV_ASSOCIATION_DEF_ov_inheritance.assoctype;
	assoc_inheritance.v_assocprops =
			OV_ASSOCIATION_DEF_ov_inheritance.assocprops;
	assoc_childrelationship.v_assoctype =
			OV_ASSOCIATION_DEF_ov_childrelationship.assoctype;
	assoc_childrelationship.v_assocprops =
			OV_ASSOCIATION_DEF_ov_childrelationship.assocprops;
	assoc_parentrelationship.v_assoctype =
			OV_ASSOCIATION_DEF_ov_parentrelationship.assoctype;
	assoc_parentrelationship.v_assocprops =
			OV_ASSOCIATION_DEF_ov_parentrelationship.assocprops;

	pobj = (OV_INSTPTR_ov_object) (((OV_HEAD*) (pdb->ov.v_linktable + distance
			+ passoc_ov_containment->v_parentoffset))->pfirst);
	while (pobj) {
		pobj = (OV_INSTPTR_ov_object) ((char*) pobj + distance);
		if (!strcmp(pobj->v_identifier + distance, "inheritance")) {
			assoc_inheritance.v_parentoffset =
					((OV_INSTPTR_ov_association) pobj)->v_parentoffset;
			assoc_inheritance.v_childoffset =
					((OV_INSTPTR_ov_association) pobj)->v_childoffset;
		}
		if (!strcmp(pobj->v_identifier + distance, "childrelationship")) {
			assoc_childrelationship.v_parentoffset =
					((OV_INSTPTR_ov_association) pobj)->v_parentoffset;
			assoc_childrelationship.v_childoffset =
					((OV_INSTPTR_ov_association) pobj)->v_childoffset;
		}
		if (!strcmp(pobj->v_identifier + distance, "parentrelationship")) {
			assoc_parentrelationship.v_parentoffset =
					((OV_INSTPTR_ov_association) pobj)->v_parentoffset;
			assoc_parentrelationship.v_childoffset =
					((OV_INSTPTR_ov_association) pobj)->v_childoffset;
		}
		pobj = (OV_INSTPTR_ov_object) (((OV_ANCHOR*) (pobj->v_linktable
				+ distance + passoc_ov_containment->v_childoffset))->pnext);
	}

	passoc_ov_inheritance = &assoc_inheritance;
	passoc_ov_childrelationship = &assoc_childrelationship;
	passoc_ov_parentrelationship = &assoc_parentrelationship;
	/*
	 *	make a copy of the original database
	 */
	pdbcopy = (OV_DATABASE_INFO*) Ov_HeapMalloc(pdb->size);
	if (!pdbcopy) {
		return OV_ERR_HEAPOUTOFMEMORY;
	}
	memcpy(pdbcopy, pdb, pdb->size);
	/*
	 *	adjust database info pointers
	 */
	Ov_Adjust(OV_POINTER, pdb->baseaddr);
	Ov_Adjust(OV_BYTE*, pdb->pstart);
	Ov_Adjust(OV_BYTE*, pdb->pend);
	Ov_Adjust(OV_BYTE*, pdb->pcurr);
	Ov_Adjust(OV_STRING, pdb->serverpassword);
	Ov_Adjust(OV_IDLIST_HEADNODE*, pdb->idList);
	if (pdb->idList->pFirst) {
		Ov_Adjust(OV_IDLIST_NODE*, pdb->idList->pFirst);
	}
	if (pdb->idList->pLast) {
		Ov_Adjust(OV_IDLIST_NODE*, pdb->idList->pLast);
	}
	if (pdb->idList->pNodes) {
		Ov_Adjust(OV_IDLIST_NODE**, pdb->idList->pNodes);
	}
	pCurrNode = pdb->idList->pFirst;
	while (pCurrNode) {
		if (pCurrNode->pNext) {
			Ov_Adjust(OV_IDLIST_NODE*, pCurrNode->pNext);
		}
		if (pCurrNode->pPrevious) {
			Ov_Adjust(OV_IDLIST_NODE*, pCurrNode->pPrevious);
		}
		if (pdb->idList->pNodes[pCurrNode->nodeNumber - 1]) {
			Ov_Adjust(OV_IDLIST_NODE*,
					pdb->idList->pNodes[pCurrNode->nodeNumber-1]);
		}
		for (iterator = 0; iterator < pCurrNode->relationCount; iterator++) {
			Ov_Adjust(OV_INSTPTR_ov_object,
					pCurrNode->relations[iterator].pobj);
			if ((((OV_POINTER) pCurrNode->relations[iterator].pobj)
					< pdb->baseaddr)
					|| (((OV_BYTE*) pCurrNode->relations[iterator].pobj)
							> pdb->pend)) {
				pCurrNode->relations[iterator].pobj = NULL;
				if (pCurrNode->relations[iterator].idLow != 1
						|| pCurrNode->relations[iterator].idHigh != 0) { /*	exception for vendortree	*/
					ov_logfile_warning(
							"Pointer in id --> pointer relation points outside database. id: %#X%08X",
							pCurrNode->relations[iterator].idHigh,
							pCurrNode->relations[iterator].idLow);
				}
			}
		}
		pCurrNode = pCurrNode->pNext;
	}

	/*
	 *	adjust pointers of the memory mempool
	 */
#if TLSF
	if(tlsf_move_pool(pdb->pstart, distance)){
		result = OV_ERR_BADDATABASE;
	}
#else
	Ov_Adjust(__ml_byte_t*, pmpinfo->heapbase);
	Ov_Adjust(__ml_byte_t*, pmpinfo->heapend);
	for (i = 0; i < BLOCKLOG; i++) {
		for (phead = &pmpinfo->fraghead[i]; phead; phead = phead->next) {
			Ov_Adjust(struct __ml_list*, phead->next);
			Ov_Adjust(struct __ml_list*, phead->prev);
		}
	}
	for (phead = &pmpinfo->free_blocks; phead; phead = phead->next) {
		Ov_Adjust(struct __ml_list*, phead->next);
		Ov_Adjust(struct __ml_list*, phead->prev);
	}
#endif

	/*
	 *	adjust pointers of the ACPLT/OV objects
	 */
	if(Ov_OK(result)){
		result = ov_object_move(Ov_PtrUpCast(ov_object, &pdb->root),
				Ov_PtrUpCast(ov_object, &pdbcopy->root), distance);
	}

	if (Ov_Fail(result)) {
		/*
		 *	copy back old database contents
		 */
		memcpy(pdb, pdbcopy, pdb->size);
	}
	/*
	 *	free the database copy memory
	 */
	Ov_HeapFree(pdbcopy);
	/*
	 *	we are through
	 */
	return result;
}

/*	----------------------------------------------------------------------	*/

#if OV_DEBUG || OV_VALGRIND

#if OV_VALGRIND
#include <valgrind/memcheck.h>
#endif

#define OV_FREELIST_SIZE 512
#define OV_FL_DESCBUFFER 255

typedef struct freelist{
	OV_UINT				count;
	struct freelist*	pnext;
	OV_POINTER			pfree[OV_FREELIST_SIZE];
	char*				desc[OV_FREELIST_SIZE];
} ov_freelist;

static OV_INLINE void ov_freelist_add(ov_freelist** ppfree, OV_POINTER ptr, char* desc){
	char* temp;

	// add next chunk
	if((*ppfree)->count>=OV_FREELIST_SIZE){
		(*ppfree)->pnext = ov_malloc(sizeof(ov_freelist));
		(*ppfree) = (*ppfree)->pnext;
		memset(*ppfree, 0, sizeof(ov_freelist));
	}

	// add pointer to list
	(*ppfree)->pfree[(*ppfree)->count] = ptr;
	if(desc){
		temp = malloc(strlen(desc)+1);
		strcpy(temp, desc);
		(*ppfree)->desc[(*ppfree)->count] = temp;
	}
	(*ppfree)->count++;
}

static OV_RESULT ov_freelist_collect_object(ov_freelist** ppfree,OV_INSTPTR pobj, OV_INT mode){
	OV_ELEMENT				part;
	OV_UINT					iter;
	OV_ANY*					pAny;
	char					pathbuf[255] = {0};
	char					descbuf[OV_FL_DESCBUFFER] = {0};
	Ov_DefineIteratorNM(NULL, pit);

	part.elemtype = OV_ET_NONE;

	ov_memstack_lock();
	snprintf(pathbuf, 255, "(%u,%u)%s", pobj->v_idH, pobj->v_idL, ov_path_getcanonicalpath(pobj, 2));
	ov_memstack_unlock();

	snprintf(descbuf, OV_FL_DESCBUFFER, "%s.linktable", pathbuf);
	ov_freelist_add(ppfree, pobj->v_linktable, descbuf);

	if(	pobj==(OV_INSTPTR)(&pdb->root) ||
		pobj==(OV_INSTPTR)(&pdb->vendordom) ||
		pobj==(OV_INSTPTR)(&pdb->acplt) ||
		pobj==(OV_INSTPTR)(&pdb->containment) ||
		pobj==(OV_INSTPTR)(&pdb->instantiation) ||
		pobj==(OV_INSTPTR)(&pdb->ov)){
		mode = 0x0;
	}

	ov_element_getnextpart_object(pobj, &part, OV_ET_ANY);
	while(part.elemtype!=OV_ET_NONE){
		switch(part.elemtype){
		case OV_ET_VARIABLE:
			if(part.elemunion.pvar->v_varprops&OV_VP_DERIVED){
				break;
			}

			if(Ov_CanCastTo(ov_variable, pobj) && !strcmp(part.elemunion.pvar->v_identifier, "initialvalue")){
				// initial values are not allocated
				break;
			}

			switch(part.elemunion.pvar->v_veclen){
			case 1:
				switch(part.elemunion.pvar->v_vartype){
				case OV_VT_STRING:
					snprintf(descbuf, OV_FL_DESCBUFFER, "%s.%s (str)", pathbuf, part.elemunion.pvar->v_identifier);
					ov_freelist_add(ppfree, *((OV_STRING*)part.pvalue), descbuf);
					break;

				case OV_VT_ANY:
					pAny = (OV_ANY*)part.pvalue;
					if((pAny->value.vartype&OV_VT_KSMASK)==OV_VT_STRING){
						snprintf(descbuf, OV_FL_DESCBUFFER, "%s.%s (any str)", pathbuf, part.elemunion.pvar->v_identifier);
						ov_freelist_add(ppfree, pAny->value.valueunion.val_string, descbuf);
					}
					if(pAny->value.vartype&OV_VT_ISVECTOR){
						snprintf(descbuf, OV_FL_DESCBUFFER, "%s.%s (any vec base)", pathbuf, part.elemunion.pvar->v_identifier);
						ov_freelist_add(ppfree, pAny->value.valueunion.val_generic_vec.value, descbuf);
						if((pAny->value.vartype&OV_VT_KSMASK)==OV_VT_STRING_VEC){
							for(iter=0; iter<pAny->value.valueunion.val_string_vec.veclen; iter++){
								snprintf(descbuf, OV_FL_DESCBUFFER, "%s.%s (any vec str) %u", pathbuf, part.elemunion.pvar->v_identifier, iter);
								ov_freelist_add(ppfree, pAny->value.valueunion.val_string_vec.value[iter], descbuf);
							}
						}
					}
				}
				break;

			case 0:
				// dynamic vector
				snprintf(descbuf, OV_FL_DESCBUFFER, "%s.%s (vec)", pathbuf, part.elemunion.pvar->v_identifier);
				ov_freelist_add(ppfree, ((OV_GENERIC_VEC*)part.pvalue)->value, descbuf);
				if((part.elemunion.pvar->v_vartype&OV_VT_KSMASK&(~OV_VT_ISVECTOR))==OV_VT_STRING){
					for(iter=0; iter<((OV_STRING_VEC*)part.pvalue)->veclen; iter++){
						snprintf(descbuf, OV_FL_DESCBUFFER, "%s.%s (vec str) %u", pathbuf, part.elemunion.pvar->v_identifier, iter);
						ov_freelist_add(ppfree, ((OV_STRING_VEC*)part.pvalue)->value[iter], descbuf);
					}
				}
				break;

			default:
				switch(part.elemunion.pvar->v_vartype&OV_VT_KSMASK){
				case OV_VT_STRING:
				case OV_VT_STRING_VEC:
					for(iter=0; iter<part.elemunion.pvar->v_veclen; iter++){
						snprintf(descbuf, OV_FL_DESCBUFFER, "%s.%s (static str) %u", pathbuf, part.elemunion.pvar->v_identifier, iter);
						ov_freelist_add(ppfree, ((OV_STRING_VEC*)part.pvalue)->value[iter], descbuf);
					}
					break;

				default:
					break;
				}
			}
			break;
		case OV_ET_OBJECT:
			ov_freelist_collect_object(ppfree, part.elemunion.pobj, 0x0);
			break;
			// fixme N:M associations not thoroughly tested
		case OV_ET_PARENTLINK:
			if(part.elemunion.passoc->v_assoctype == OV_AT_MANY_TO_MANY){
				for(Ov_Association_GetFirstChildNM(part.elemunion.passoc, pit, pobj); pit; pit = pit->parent.pnext){
					snprintf(descbuf, OV_FL_DESCBUFFER, "%s.%s (OV_NMLINK)", pathbuf, part.elemunion.passoc->v_identifier);
					ov_freelist_add(ppfree, pit, descbuf);
				}
			}
			break;
		case OV_ET_CHILDLINK:
			if(part.elemunion.passoc->v_assoctype == OV_AT_MANY_TO_MANY){
				for(Ov_Association_GetFirstParentNM(part.elemunion.passoc, pit, pobj); pit; pit = pit->child.pnext ){
					// skip if linked to itself to avoid double free
					if(pit->parent.pparent==pobj)
						continue;
					snprintf(descbuf, OV_FL_DESCBUFFER, "%s.%s (OV_NMLINK)", pathbuf, part.elemunion.passoc->v_identifier);
					ov_freelist_add(ppfree, pit, descbuf);
				}
			}
			break;
		}

		ov_element_getnextpart_object(pobj, &part, OV_ET_ANY);
	}

	if(mode&0x1){ // free object pointer
		snprintf(descbuf, OV_FL_DESCBUFFER, "%s (objptr)", pathbuf);
		ov_freelist_add(ppfree, pobj, descbuf);
	}

	return OV_ERR_OK;
}

static OV_RESULT ov_freelist_collect(ov_freelist* freeFirst){
	ov_freelist*	pFreeCur = freeFirst;
	OV_IDLIST_NODE* pNodeCur = NULL;
	OV_INSTPTR		pobj = NULL;
	OV_UINT			iterator = 0;
	OV_UINT			nodecount = 0;
	char			descbuf[50] = {0};

	ov_freelist_add(&pFreeCur, pdb->idList, "pdb.idList");
	ov_freelist_add(&pFreeCur, pdb->idList->pNodes, "pdb.idList.pNodes");


	// vendor objects are not in idList
	Ov_ForEachChild(ov_containment, &pdb->vendordom, pobj){
		ov_freelist_collect_object(&pFreeCur, pobj, 0x0);
	}

	pNodeCur = pdb->idList->pFirst;

	while(pNodeCur){
		for(iterator = 0; iterator<pNodeCur->relationCount; iterator++){
			if(!pNodeCur->relations[iterator].pobj)
				continue;

			ov_freelist_collect_object(&pFreeCur, pNodeCur->relations[iterator].pobj, 0x1);

		}
		snprintf(descbuf, 50, "pdb.idList.pNodes[%u]", nodecount);
		ov_freelist_add(&pFreeCur, pNodeCur, descbuf);
		pNodeCur = pNodeCur->pNext;
		nodecount++;
	}

	return OV_ERR_OK;
}

OV_DLLFNCEXPORT void ov_freelist_print(){
	ov_freelist		freelist = {0};
	ov_freelist*	freelistCur = NULL;
	ov_freelist*	freelistNext;
	OV_UINT64		iter = 0;
	OV_POINTER		ptr = NULL;
#if TLSF
	void**			poolPtrs = NULL;
	void**			pc = NULL;
	size_t*			sizes = NULL;
	size_t			tmpSize;
	OV_BOOL			found = FALSE;
#endif

	ov_freelist_collect(&freelist);

	// print allocated pointers found via ov meta model
	for(freelistCur=&freelist; freelistCur; freelistCur=freelistCur->pnext){
		for(iter=0; iter<freelistCur->count; iter++){
			ptr = freelistCur->pfree[iter];
			if(!ptr){
				continue;
			}

			if(ptr>=(OV_POINTER)pdb->pstart && ptr<=(OV_POINTER)pdb->pend){
				ov_logfile_debug("pointer inside db  %p -> %s", ptr-(void*)pdb, freelistCur->desc[iter]);
			} else {
				ov_logfile_debug("pointer outside db %p -> %s", ptr-(void*)pdb, freelistCur->desc[iter]);

			}
		}
	}

	// look in tlsf pool for blocks that are not reachable via ov meta model
#if TLSF
	tlsf_getPointerList(dbpool, &poolPtrs, &sizes);
	pc = poolPtrs;

	while(*pc){
		found = FALSE;
		freelistCur = &freelist;
		while(freelistCur&&!found){
			for(iter=0; iter<freelistCur->count && !found; iter++){
				if(freelistCur->pfree[iter]==*pc)
					found = TRUE;
			}
			freelistCur = freelistCur->pnext;
		}

		if(!found){
			tmpSize = sizes[pc - poolPtrs];
			ov_logfile_debug(
					"block pointer %p not found in pointer list (content: %*.*s; length: %u; block nr. %u)",
					*pc - (void*) pdb, tmpSize, tmpSize, *pc, tmpSize, pc - poolPtrs);
		}

		pc++;
	}

	free(poolPtrs);
	free(sizes);
#endif

	freelistCur = &freelist;
	while(freelistCur){
		freelistNext = freelistCur->pnext;
		for(iter=0; iter<freelistCur->count; iter++){
			free(freelistCur->desc[iter]);
		}
		if(freelistCur!=&freelist)
			ov_free(freelistCur);
		freelistCur = freelistNext;
	}
}

static void ov_freelist_free(){
	ov_freelist		freelist = {0};
	ov_freelist*	freelistCur = NULL;
	ov_freelist*	freelistNext = NULL;
	OV_UINT64		iter = 0;
	OV_POINTER		ptr = NULL;
	OV_BOOL			mallocActive = FALSE;

	if (ov_path_getobjectpointer("/acplt/malloc", 2))
		mallocActive = TRUE;

	ov_freelist_collect(&freelist);

	for(freelistCur=&freelist; freelistCur; freelistCur=freelistCur->pnext){
		for(iter=0; iter<freelistCur->count; iter++){
			ptr = freelistCur->pfree[iter];
			if(!ptr){
				continue;
			}

			if(ptr>=(OV_POINTER)pdb->pstart && ptr<=(OV_POINTER)pdb->pend){
//				ov_logfile_debug("pointer inside db  %p -> %s", ptr-(void*)pdb, freelistCur->desc[iter]);
#if TLSF && OV_VALGRIND
				// set pointer as freed
				VALGRIND_MEMPOOL_FREE(dbpool, ptr);
#endif
			} else {
				// free pointer outside of database
				if(mallocActive)
					free(ptr);
				else
					ov_logfile_warning("pointer outside db %p -> %s", ptr-(void*)pdb, freelistCur->desc[iter]);
			}
		}
	}

	size_t* sizes;
	void** poolPtrs = NULL;
	tlsf_getPointerList(dbpool, &poolPtrs, &sizes);
	void** pc = poolPtrs;
	OV_BOOL found = FALSE;

	while(*pc){
		found = FALSE;
		freelistCur = &freelist;
		while(freelistCur&&!found){
			for(iter=0; iter<freelistCur->count && !found; iter++){
				if(freelistCur->pfree[iter]==*pc)
					found = TRUE;
			}
			freelistCur = freelistCur->pnext;
		}

		if(!found){
			ov_logfile_warning(
					"block pointer %p not found in pointer list (content: %s; length: %u; block nr. %u)",
					*pc - (void*) pdb, *pc, sizes[pc - poolPtrs], pc - poolPtrs);
		}

		pc++;
	}

	free(poolPtrs);
	free(sizes);

	// free freelists
	freelistCur = &freelist;
	while(freelistCur){
		freelistNext = freelistCur->pnext;
		for(iter=0; iter<freelistCur->count; iter++){
			free(freelistCur->desc[iter]);
		}
		if(freelistCur!=&freelist)
			ov_free(freelistCur);
		freelistCur = freelistNext;
	}
}

#else

OV_DLLFNCEXPORT void ov_freelist_print(){
	fprintf(stderr, "print pointer is not available if not compiled with OV_DEBUG\n");
}

#endif


/*
 *	End of file
 */

