/*
*   $Id: ov_database.c,v 1.27 2008-12-10 14:30:55 martin Exp $
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
*	13-Apr-1999 Dirk Meyer   <dirk@plt.rwth-aachen.de>: File created.
*	07-Jun-2001 J.Nagelmann  <nagelmann@ltsoft.de>: Changes for Sun Solaris.
*	30-Apr-2008 A.Neugebauer <neugebauer@ltsoft.de>: Changes for libml.
*   02-Jun-2013 Sten Gruener <s.gruener@plt.rwth-aachen.de>: Notes on valgrind.
*/

#define OV_COMPILE_LIBOV

#include "libov/ov_database.h"
#include "libov/ov_object.h"
#include "libov/ov_library.h"
#include "libov/ov_string.h"
#include "libov/ov_time.h"
#include "libov/ov_macros.h"
#include "libov/ov_logfile.h"


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

#ifndef __STDC__
#define __STDC__ 1
#include "ml_malloc.h"
#undef __STDC__
#else
#include "ml_malloc.h"
#endif

#if OV_SYSTEM_MC164
#define memset xmemset
#define memcpy xmemcpy
#endif

/*	----------------------------------------------------------------------	*/

/*
*	Global variable: database pointer, file handles and stuff
*/
OV_DLLVAREXPORT OV_DATABASE_INFO OV_MEMSPEC	*pdb = NULL;

#if OV_SYSTEM_UNIX
static int			fd;
#endif

#if OV_SYSTEM_NT
static HANDLE		hfile;
static HANDLE		hmap;
#endif

#if OV_SYSTEM_RMOS
FILE				*file;
#endif

#if OV_SYSTEM_OPENVMS
unsigned short		channel;
#endif

#if OV_SYSTEM_MC164
OV_DATABASE_INFO OV_MEMSPEC	*pdbmem;
#endif

/*
*	VTable of any object in case we dont start up the database
*/
static OV_VTBL_ov_object nostartupvtable = {
	ov_object_constructor_nostartup,
	ov_object_checkinit,
	ov_object_destructor,
	ov_object_startup,
	ov_object_shutdown,
	ov_object_rename,
	ov_object_getaccess_nostartup,
	ov_object_getflags,
	ov_object_getcomment,
	ov_object_gettechunit,
	ov_object_getvar,
	ov_object_setvar
};

/*	----------------------------------------------------------------------	*/

/*
*	Helper macro: Round up the size to the next n*BLOCKSIZE
*	TODO_ADJUST_WHEN_LARGE_DB
*/
#define Ov_Roundup(size) (((OV_UINT)(size)+BLOCKSIZE-1)&~(BLOCKSIZE-1))

/*	----------------------------------------------------------------------	*/

/*
*	Macro: pointer to memory pool info structure
*/
#define pmpinfo ((ml_info OV_MEMSPEC *)(pdb+1))

/*	----------------------------------------------------------------------	*/

/*
*	Get more core memory from the memory pool (subroutine)
*/
__ml_ptr ov_database_morecore(
	__ml_size_t	size
) {
	/*
	*	local variables
	*/
	__ml_ptr	pmem = (__ml_ptr)(pdb->pcurr);
	OV_BYTE	OV_MEMSPEC *psoon = pdb->pcurr+size;
#if OV_DYNAMIC_DATABASE
#if OV_SYSTEM_UNIX
	char	null = 0;
#endif
#endif
	/*
	*	test if we can get/release memory
	*/
	if (psoon<pdb->pstart) {
		return NULL;
	}
	if(psoon > pdb->pend) {
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
	return (__ml_ptr)pmem;
}

/*	----------------------------------------------------------------------	*/

/*	
 *	Get a numeric id and increment the internal counter
 */

 OV_DLLFNCEXPORT OV_BOOL ov_database_getId(OV_UINT *idH, OV_UINT *idL){
	if(idH){
		*idH = pdb->idCounter1;
	}
	if(idL){
		*idL = pdb->idCounter0;
	}
	
	if(pdb->idCounter0 < OV_VL_MAXUINT){
		pdb->idCounter0++;
		return TRUE;
	} else {
		pdb->idCounter0 = 0;
		if(pdb->idCounter1 < OV_VL_MAXUINT){
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
 void ov_database_getId_noInc(OV_UINT *idH, OV_UINT *idL){
	if(idH){
		*idH = pdb->idCounter1;
	}
	if(idL){
		*idL = pdb->idCounter0;
	}
}

/*
 *	Convert a high low dword pair to a 64bit id
 */
 OV_DLLFNCEXPORT OV_UINT64 ov_database_convertId(const OV_UINT idH, const OV_UINT idL){
	OV_UINT64 result;
	result = (OV_UINT64)idH << 32;
	result += idL;
	return result;
 }
 
 /*	compare 2 ids returns -1 if id1 < id2, 1 if id1 > id2 and else 0	*/
 OV_INT ov_database_compareIds(const OV_UINT id1H, const OV_UINT id1L, const OV_UINT id2H, const OV_UINT id2L){
	if(id1H > id2H){
		return 1;
	} else if(id1H < id2H){
		return -1;
	} else {
		if(id1L > id2L){
			return 1;
		} else if(id1L < id2L){
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
OV_IDLIST_NODE* ov_database_idListGetNode(const OV_UINT idH, const OV_UINT idL){
	OV_IDLIST_NODE* node = NULL;
	OV_UINT64	id64;
	OV_DOUBLE	idFraction;
	OV_UINT64	nodeNumber;
#if OV_IDLIST_PERFORMANCEPRINTS
	OV_UINT cycleCounter = 0;
#endif
	id64 = ov_database_convertId(idH, idL);
	if(id64 > pdb->idList->maxId){
		return NULL;
	} 

	/*	calculate starting point for node search
		multiply the idFraction (searchedId / maxId) with the 
		the result with the number of nodes to get the starting point
		if this one does not match directly walk along the list*/
	idFraction = id64 / pdb->idList->maxId;
	nodeNumber = (OV_UINT64)(idFraction * (pdb->idList->nodeCount - 1));

	node = pdb->idList->pNodes[nodeNumber];
	while(node){
		if(node->maxId < id64){
			node = node->pNext;
#if OV_IDLIST_PERFORMANCEPRINTS
			cycleCounter++;
#endif
			continue;
		} else if(node->minId > id64){
			node = node->pPrevious;
#if OV_IDLIST_PERFORMANCEPRINTS
			cycleCounter++;
#endif
			continue;
		} else {
#if OV_IDLIST_PERFORMANCEPRINTS
			if(cycleCounter == 0){
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
OV_RESULT ov_database_idListGetRelationIndex(const OV_UINT idH, const OV_UINT idL, const struct OV_IDLIST_NODE *const node, OV_UINT* index){
	OV_UINT currMin = 0;
	OV_UINT currMax;
	OV_INT cmpRes;
	OV_UINT64 id64 = ov_database_convertId(idH, idL);
#if OV_IDLIST_PERFORMANCEPRINTS
	OV_UINT cycleCounter = 0;
#endif
	if(!index){
		return OV_ERR_BADPARAM;
	}
	
	if(!node->relationCount || (id64 > node->maxId) || (id64 < node->minId)){
		return OV_ERR_BADVALUE;
	}
	currMax = node->relationCount - 1;
	do{
		*index = (currMax - currMin) / 2 + currMin;
		cmpRes = ov_database_compareIds(idH, idL, node->relations[*index].idHigh, node->relations[*index].idLow);
		if(cmpRes > 0){
			currMin = *index;
#if OV_IDLIST_PERFORMANCEPRINTS
			cycleCounter++;
#endif
		} else if(cmpRes < 0){
			currMax = * index;
#if OV_IDLIST_PERFORMANCEPRINTS
			cycleCounter++;
#endif
		} else {
#if OV_IDLIST_PERFORMANCEPRINTS
			ov_logfile_debug("getIndex: boundary hit after %u cycles", cycleCounter);
#endif
			return OV_ERR_OK;
		}
	}while((currMax - currMin) > 1);
	*index = currMin;
	if(ov_database_compareIds(idH, idL, node->relations[*index].idHigh, node->relations[*index].idLow) == 0){
		return OV_ERR_OK;
	} else {
		(*index)++;
		if(ov_database_compareIds(idH, idL, node->relations[*index].idHigh, node->relations[*index].idLow) == 0){
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
void idList_printstats(){
	OV_UINT64 i;

	ov_logfile_debug("-----------");
	ov_logfile_debug("basedata:\n\tnodeCount:\t\t%llu\n\tminId:\t\t\t%llu\n\tmaxId:\t\t\t%llu\n\tpFirst:\t\t\t%p\n\tpFirst->nodeNumber:\t%llu\n\tpLast:\t\t\t%p\n\tpLast->nodeNumber:\t%llu",
			pdb->idList->nodeCount, pdb->idList->minId, pdb->idList->maxId, pdb->idList->pFirst, pdb->idList->pFirst->nodeNumber, pdb->idList->pLast, pdb->idList->pLast->nodeNumber);
	ov_logfile_debug("-----\nnodes:");
	for(i=0; i<pdb->idList->nodeCount;i++){
		ov_logfile_debug("entry %llu:\n\taddress:\t%p\n\tnodeNumber:\t%llu\n\tminId:\t\t%llu\n\tmaxId:\t\t%llu\n\trelationCount:\t%u\n\tpPrevious:\t%p\n\tpNext:\t\t%p\n",
				i, pdb->idList->pNodes[i], pdb->idList->pNodes[i]->nodeNumber, pdb->idList->pNodes[i]->minId, pdb->idList->pNodes[i]->maxId, pdb->idList->pNodes[i]->relationCount, pdb->idList->pNodes[i]->pPrevious, pdb->idList->pNodes[i]->pNext);
	}

}
#endif
 /*	
 *	Relate an object pointer  with an id (creates a new entry in the idList relating object pointer and id)
 *	is called on every object creation
 */
OV_DLLFNCEXPORT OV_RESULT ov_database_idListInsert(const OV_UINT idH, const OV_UINT idL, const OV_INSTPTR_ov_object pInstance){
	/*	as this function is called on object creation and ids are counted up, a look at the very last element in the list should suffice
	 *	and if not throw an error
	 */
	OV_UINT iterator;
	OV_UINT64 newId = ov_database_convertId(idH, idL);
	OV_IDLIST_NODE	*pNode = NULL;
	
	if(!pInstance){
		return OV_ERR_BADPARAM;
	}	/*	check if id is lager than the largest id in the list or if it ist the very first object	*/
	if((newId > pdb->idList->maxId) || ((!idH) && (!idL) && (!pdb->idList->maxId))){
		/*	expected case --> append to the end	*/
		if(pdb->idList->pLast->relationCount >= OV_IDLIST_RELATIONSPERCHUNK){
#if OV_IDLIST_DEBUG
			ov_logfile_debug("insert - line: %u - before", __LINE__);
			idList_printstats();
#endif
			/*	node is full, create e new one	*/
			pdb->idList->pLast->pNext = ov_database_malloc(sizeof(OV_IDLIST_NODE));
			if(!pdb->idList->pLast->pNext){
				return OV_ERR_DBOUTOFMEMORY;
			}
			pdb->idList->pLast->pNext->pPrevious = pdb->idList->pLast;
			pdb->idList->pLast = pdb->idList->pLast->pNext;
			pdb->idList->pLast->pNext = NULL;
			if(pdb->idList->pLast->pPrevious->nodeNumber == OV_VL_MAXUINT64){
				return OV_ERR_DBOUTOFMEMORY;
			}
			pdb->idList->pLast->nodeNumber = pdb->idList->pLast->pPrevious->nodeNumber + 1;
			pdb->idList->nodeCount = pdb->idList->pLast->nodeNumber;
						
			for(iterator = 0; iterator < OV_IDLIST_RELATIONSPERCHUNK; iterator++){
				pdb->idList->pLast->relations[iterator].idLow = 0;
				pdb->idList->pLast->relations[iterator].idHigh = 0;
				pdb->idList->pLast->relations[iterator].pobj = NULL;
			}
			pdb->idList->pLast->minId = newId;
			pdb->idList->pLast->relationCount = 0;
			/*	insert node in array	*/
			if((pdb->idList->nodeCount % OV_IDLIST_NODELISTCHUNKSIZE) == 0){
				/*	pNodes array is full --> allocate a new chunk	*/
				ov_database_free(pdb->idList->pNodes); 
				pdb->idList->pNodes = ov_database_malloc((pdb->idList->nodeCount + OV_IDLIST_NODELISTCHUNKSIZE) * sizeof(OV_IDLIST_NODE*));
				if(!pdb->idList->pNodes){
					return OV_ERR_DBOUTOFMEMORY;
				}
				pNode = pdb->idList->pFirst;
				while(pNode){
					pdb->idList->pNodes[pNode->nodeNumber - 1] = pNode;
					pNode = pNode->pNext;
				}
			}
			pdb->idList->pNodes[pdb->idList->nodeCount - 1] = pdb->idList->pLast;
#if OV_IDLIST_DEBUG
			ov_logfile_debug("insert - line: %u - after", __LINE__);
			idList_printstats();
#endif
		}
		/*	here pLast is surely the chunk we need --> insert 	*/
		pdb->idList->pLast->relations[pdb->idList->pLast->relationCount].idLow = idL;
		pdb->idList->pLast->relations[pdb->idList->pLast->relationCount].idHigh = idH;
		pdb->idList->pLast->relations[pdb->idList->pLast->relationCount].pobj = pInstance;
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
OV_DLLFNCEXPORT OV_RESULT ov_database_idListRelease(const OV_UINT idH, const OV_UINT idL){
	OV_IDLIST_NODE* node = NULL;
	OV_IDLIST_NODE* nextNode = NULL;
	OV_UINT index;
	OV_RESULT result;
	OV_UINT iterator;
	/*	find relation	*/
	node = ov_database_idListGetNode(idH, idL);
	if(!node){
		return OV_ERR_GENERIC;
	}
	result = ov_database_idListGetRelationIndex(idH, idL, node, &index);
	if(Ov_Fail(result)){
		return result;
	}
#if OV_IDLIST_DEBUG
	ov_logfile_debug("release - line: %u - before", __LINE__);
	idList_printstats();
#endif
	/*	move all relations in that node one step backwards	*/
	
	for(iterator = index + 1; iterator < node->relationCount; iterator++, index++){
		node->relations[index].idLow = node->relations[iterator].idLow;
		node->relations[index].idHigh = node->relations[iterator].idHigh;
		node->relations[index].pobj = node->relations[iterator].pobj;
	}
	node->relationCount--;
	if(node->relationCount){
		node->minId = ov_database_convertId(node->relations[0].idHigh, node->relations[0].idLow);
		node->maxId = ov_database_convertId(node->relations[node->relationCount - 1].idHigh, node->relations[node->relationCount - 1].idLow);
	}
	/*	if the node and the next node are less than half full put them together, if not do the same test on the previous node	*/
	if(node->pNext && ((node->relationCount / (OV_IDLIST_RELATIONSPERCHUNK / 2)) < 1) && ((node->pNext->relationCount / (OV_IDLIST_RELATIONSPERCHUNK / 2)) < 1)){
		nextNode = node->pNext;
#if OV_IDLIST_DEBUG
		ov_logfile_debug("putting together node %llu (min %llu, max %llu, count %lu) and node %llu (next) (min %llu, max %llu, count %lu)", node->nodeNumber, node->minId, node->maxId, node->relationCount, nextNode->nodeNumber, nextNode->minId, nextNode->maxId, nextNode->relationCount);
#endif
		for(index = node->relationCount, iterator = 0; (index < OV_IDLIST_RELATIONSPERCHUNK) && (iterator < nextNode->relationCount); iterator++, index++){
			node->relations[index].idLow = nextNode->relations[iterator].idLow;
			node->relations[index].idHigh = nextNode->relations[iterator].idHigh;
			node->relations[index].pobj = nextNode->relations[iterator].pobj;
			node->relationCount++;
		}
		node->minId = ov_database_convertId(node->relations[0].idHigh, node->relations[0].idLow);
		node->maxId = ov_database_convertId(node->relations[node->relationCount - 1].idHigh, node->relations[node->relationCount - 1].idLow);
		nextNode->relationCount = 0;
	} else if(node->pPrevious && ((node->relationCount / (OV_IDLIST_RELATIONSPERCHUNK / 2)) < 1) && ((node->pPrevious->relationCount / (OV_IDLIST_RELATIONSPERCHUNK / 2)) < 1)){
		node = node->pPrevious;
		nextNode = node->pNext;
#if OV_IDLIST_DEBUG
		ov_logfile_debug("putting together node %llu (previous) (min %llu, max %llu, count %lu) and node %llu (min %llu, max %llu, count %lu)", nextNode->nodeNumber, nextNode->minId, nextNode->maxId, nextNode->relationCount, node->nodeNumber, node->minId, node->maxId, node->relationCount);
#endif
		for(index = node->relationCount, iterator = 0; (index < OV_IDLIST_RELATIONSPERCHUNK) && (iterator < nextNode->relationCount); iterator++, index++){
			node->relations[index].idLow = nextNode->relations[iterator].idLow;
			node->relations[index].idHigh = nextNode->relations[iterator].idHigh;
			node->relations[index].pobj = nextNode->relations[iterator].pobj;
			node->relationCount++;
		}
		node->minId = ov_database_convertId(node->relations[0].idHigh, node->relations[0].idLow);
		node->maxId = ov_database_convertId(node->relations[node->relationCount - 1].idHigh, node->relations[node->relationCount - 1].idLow);
		nextNode->relationCount = 0;
	} else if(node->relationCount == 0){
		nextNode = node;
	}
	
	if(nextNode && (!nextNode->relationCount)){
#if OV_IDLIST_DEBUG
		ov_logfile_debug("deleting node %llu (min %llu, max %llu, count %lu)", nextNode->nodeNumber, nextNode->minId, nextNode->maxId, nextNode->relationCount);
#endif
		nextNode->pPrevious->pNext = nextNode->pNext;
		if(nextNode->pNext){
			nextNode->pNext->pPrevious = nextNode->pPrevious;
		}
		for(iterator = nextNode->nodeNumber; (iterator <  pdb->idList->nodeCount); iterator++){
			pdb->idList->pNodes[iterator-1] = pdb->idList->pNodes[iterator];
			pdb->idList->pNodes[iterator-1]->nodeNumber--;
		}
		pdb->idList->pNodes[iterator-1] = NULL;
		pdb->idList->nodeCount--;
		ov_database_free(nextNode);
		pdb->idList->pLast = pdb->idList->pNodes[pdb->idList->nodeCount - 1];
		
		if((pdb->idList->nodeCount % OV_IDLIST_NODELISTCHUNKSIZE) == 0){
			/*	a complete chunk is empty now --> release memory	*/
			pdb->idList->pNodes = ov_database_realloc(pdb->idList->pNodes, pdb->idList->nodeCount * sizeof(OV_IDLIST_NODE*));
			if(!pdb->idList->pNodes){
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

OV_DLLFNCEXPORT OV_RESULT ov_database_create(
	OV_STRING	filename,
	OV_UINT		size /* TODO_ADJUST_WHEN_LARGE_DB */
) {
	/*
	*	local variables
	*/
	OV_RESULT		result;
	OV_UINT			iterator;
	
#if !OV_SYSTEM_MC164
	FILE			*fp;
#endif
#if OV_SYSTEM_UNIX
	static char		null = 0;
#endif
#if OV_SYSTEM_OPENVMS
	struct FAB		fab;
	OV_BYTE			*inaddr[2], *retaddr[2];
	unsigned long	status;
#endif
	/*
	*	check parameters
	*/
	if(pdb) {
		return OV_ERR_GENERIC;
	}
	// TODO_ADJUST_WHEN_LARGE_DB
	if(!size || (size > (OV_UINT) OV_DATABASE_MAXSIZE) || !filename) {
		return OV_ERR_BADPARAM;
	}
	/* Rundup file size */
	size = ml_rundupFileSize(size);
	
#if !OV_SYSTEM_MC164
	/*
	*	check, if file already exists
	*/
	fp = fopen(filename, "r");
	if(fp) {
		fclose(fp); 
		return OV_ERR_FILEALREADYEXISTS;
	}
#endif
#if OV_SYSTEM_UNIX
	/*
	*	create the new database file
	*/
	fd = open(filename, O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR | S_IRGRP);
	if(fd == -1) {
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
	if(flock(fd, LOCK_EX | LOCK_NB)) {
		close(fd);
		return OV_ERR_CANTLOCKFILE;
	}
#endif
	/*
	*	make the file "size" bytes long (rounded up)
	*	TODO_ADJUST_WHEN_LARGE_DB
	*/
	if(lseek(fd, size-sizeof(char), SEEK_SET) < 0) {
		close(fd);
		return OV_ERR_CANTWRITETOFILE;
	}
	if(write(fd, (void*)&null, sizeof(char)) == -1) {
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
			PROT_READ | PROT_WRITE,	MAP_PRIVATE, fd, 0);
	#else
		pdb = (OV_DATABASE_INFO*)mmap(0, size,
			PROT_READ | PROT_WRITE,	MAP_SHARED, fd, 0);
	#endif
#endif
	if(pdb == (OV_DATABASE_INFO*)-1) {
		pdb = NULL;
		close(fd);
		return OV_ERR_CANTMAPFILE;
	}

#endif
#if OV_SYSTEM_NT
	/*
	*	check file size
	*/
	if(size > (OV_UINT) OV_DATABASE_MAXSIZE) {
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
		== 0xFFFFFFFF
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
	pdb = (OV_DATABASE_INFO*)Ov_HeapMalloc(size);
	if(!pdb) {
		return OV_ERR_HEAPOUTOFMEMORY;
	}
	file = fopen(filename, "wb+");
	if(!file) {
		Ov_HeapFree(pdb);
		return OV_ERR_CANTCREATEFILE;
	}
#endif
#if OV_SYSTEM_OPENVMS
	/*
	*	initialize file access block (FAB)
	*/
	fab = cc$rms_fab;
	fab.fab$v_ufo = 1;						/* user file open */
	fab.fab$l_fna = filename;				/* filename address */
	fab.fab$b_fns = strlen(filename);		/* filename size */
	fab.fab$l_alq = size >> 9;	            /* Allocation quantity (file size) */
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
		ov_database_unmap();
		return OV_ERR_CANTCREATEFILE;
	}
	/*
	*	assign database pointer
	*/
	Ov_AbortIfNot(size == retaddr[1]-retaddr[0]+1);
	pdb = (OV_DATABASE_INFO*)retaddr[0];
#endif
	/*
	*	initialize database structure
	*/
	Ov_AbortIfNot(pdb);
#if OV_DYNAMIC_DATABASE
	memset(pdb, 0, sizeof(OV_DATABASE_INFO));
#else
	memset(pdb, 0, size);
#endif
	pdb->baseaddr = (OV_POINTER)pdb;
	pdb->size = size;
	
	pdb->pstart = pdb->pcurr = (OV_BYTE*)((OV_BYTE*)pdb->baseaddr + BLOCKIFY(sizeof(OV_DATABASE_INFO) + sizeof(ml_info)) * BLOCKSIZE);

	pdb->pend = (OV_BYTE*)((OV_BYTE*)pdb+size);
	pdb->started = FALSE;
		
	/*
	*	initialize the database memory pool
	*	    
	*/	
	if(!ml_initialize(pmpinfo, pdb->pstart, ov_database_morecore)) {
		ov_database_unmap();
		return OV_ERR_DBOUTOFMEMORY;
	}
	
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
	for(iterator = 0; iterator < OV_IDLIST_RELATIONSPERCHUNK; iterator++){
		pdb->idList->pFirst->relations[iterator].idLow = 0;
		pdb->idList->pFirst->relations[iterator].idHigh = 0;
		pdb->idList->pFirst->relations[iterator].pobj = NULL;
	}
	pdb->idList->pNodes = ov_database_malloc(OV_IDLIST_NODELISTCHUNKSIZE * sizeof(OV_IDLIST_NODE*));
	Ov_AbortIfNot(pdb->idList->pNodes);
	pdb->idList->pNodes[0] = pdb->idList->pFirst;
	for(iterator = 1; iterator < OV_IDLIST_NODELISTCHUNKSIZE; iterator++){
		pdb->idList->pNodes[iterator] = NULL;
	}
	
	
	/*
	*	initialize the root domain object
	*/
	Ov_AbortIfNot(Ov_OK(ov_string_setvalue(&pdb->root.v_identifier, 
		OV_OBJNAME_ROOT)));
	ov_time_gettime(&pdb->root.v_creationtime);
	/*	as this is the first getId call for this database it will return 0 0	*/
	ov_database_getId(&(pdb->root.v_idH), &(pdb->root.v_idL));
	ov_database_idListInsert(pdb->root.v_idH, pdb->root.v_idL, Ov_PtrUpCast(ov_object, &(pdb->root)));
	/*	call get id once more to reserve 0 1 for the vendortree (has to be set hard coded as it is not stored in the database)	*/
	ov_database_getId(NULL, NULL);	// call with NULL-pointers just increments the counter
	ov_database_idListInsert(0, 1, Ov_PtrUpCast(ov_object, &(pdb->vendordom)));
	/*
	*	initialize the acplt domain object
	*/
	Ov_AbortIfNot(Ov_OK(ov_string_setvalue(&pdb->acplt.v_identifier, 
		OV_OBJNAME_ACPLT)));
	ov_time_gettime(&pdb->acplt.v_creationtime);
	ov_database_getId(&(pdb->acplt.v_idH), &(pdb->acplt.v_idL));
	ov_database_idListInsert(pdb->acplt.v_idH, pdb->acplt.v_idL, Ov_PtrUpCast(ov_object, &(pdb->acplt)));
	/*
	*	initialize the OV library object
	*/
	Ov_AbortIfNot(Ov_OK(ov_string_setvalue(&pdb->ov.v_identifier, 
		OV_OBJNAME_OV)));
	ov_time_gettime(&pdb->ov.v_creationtime);
	ov_database_getId(&(pdb->ov.v_idH), &(pdb->ov.v_idL));
	ov_database_idListInsert(pdb->ov.v_idH, pdb->ov.v_idL, Ov_PtrUpCast(ov_object, &(pdb->ov)));
	/*
	*	load the OV library
	*/
	result = ov_library_load(&pdb->ov, &OV_LIBRARY_DEF_ov);
	if(Ov_Fail(result)) {
		ov_database_unmap();
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

	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
*	Map an existing database
*/
OV_DLLFNCEXPORT OV_RESULT ov_database_map(
	OV_STRING	filename
) {
	/*
	*	local variables
	*/
	OV_RESULT	result;

	result = ov_database_map_loadfile(filename);
	if (result!=OV_ERR_OK) return result;
	result = ov_database_map_loadlib(filename);
	return result;
}

/*	----------------------------------------------------------------------	*/

/*
*	Map an existing database part 1
*/
OV_DLLFNCEXPORT OV_RESULT ov_database_map_loadfile(
	OV_STRING	filename
) {
	/*
	*	local variables
	*/
	OV_RESULT	result;
	OV_POINTER	baseaddr = NULL;
	OV_UINT		size = 0;
#if OV_SYSTEM_NT
	DWORD		bytesread;
#endif
#if OV_SYSTEM_OPENVMS
	struct FAB		fab;
	OV_BYTE			*inaddr[2], *retaddr[2];
	unsigned long	status;
#endif
	/*
	*	check parameters
	*/
	if(pdb) {
		return OV_ERR_GENERIC;
	}
	if(!filename) {
		return OV_ERR_BADPARAM;
	}
#if OV_SYSTEM_UNIX
	/*
	*	open the database file
	*/
	fd = open((const char*) filename, O_RDWR);
	if(fd == -1) {
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
	if(flock(fd, LOCK_EX | LOCK_NB)) {
		close(fd);
		return OV_ERR_CANTLOCKFILE;
	}
#endif
	/*
	*	read the base address
	*/
	if(read(fd, (void*)&baseaddr, sizeof(OV_POINTER)) == -1) {
		close(fd);
		return OV_ERR_CANTREADFROMFILE;
	}
	/*
	*	read the filemapping size
	*/
	if(read(fd, (void*)&size, sizeof(OV_UINT)) == -1) {
		close(fd);
		return OV_ERR_CANTREADFROMFILE;
	}

	/*
	*	map the file to memory
	*/
#if OV_DATABASE_FLASH | OV_ARCH_NOMMU
	pdb = (OV_DATABASE_INFO*)mmap(0, size,
		PROT_READ | PROT_WRITE,	MAP_PRIVATE, fd, 0);
#else
	pdb = (OV_DATABASE_INFO*)mmap(baseaddr, size,
		PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
#endif
	if(pdb == (OV_DATABASE_INFO*)-1) {
		pdb = NULL;
		close(fd);
		return OV_ERR_CANTMAPFILE;
	}
#endif
#if OV_SYSTEM_NT
	/*
	*	open the database file
	*/
	hfile = CreateFile((const char*)filename, GENERIC_READ | GENERIC_WRITE,
		0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
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
	if(!ReadFile(hfile, (LPVOID)&size, sizeof(OV_UINT),
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
	if(fread((char*)&size, sizeof(OV_UINT), 1, file) != 1) {
		fclose(file);
		return OV_ERR_CANTREADFROMFILE;
	}
	/*
	*	get database memory
	*/
	pdb = (OV_DATABASE_INFO*)Ov_HeapMalloc(size);
	if(!pdb) {
		return OV_ERR_HEAPOUTOFMEMORY;
	}
	/*
	*	read the file to memory
	*/
	if(fseek(file, 0, SEEK_SET) < 0) {
		Ov_HeapFree(pdb);
		fclose(file);
		return OV_ERR_CANTREADFROMFILE;
	}
	if(fread((char*)pdb, size, 1, file) != 1) {
		Ov_HeapFree(pdb);
		fclose(file);
		return OV_ERR_CANTREADFROMFILE;
	}
#endif
#if OV_SYSTEM_OPENVMS
	/*
	*	initialize FAB
	*/
	fab = cc$rms_fab;
	fab.fab$v_ufo = 1;								/* user file open */
	fab.fab$l_fna = (const char*) filename;			/* filename address */
	fab.fab$b_fns = strlen(filename);				/* filename size */
	fab.fab$v_upd = 1;								/* update access */
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
		ov_database_unmap();
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
		ov_database_unmap();
		return OV_ERR_BADDATABASE;
	}
#if OV_SYSTEM_MC164
	/* nothing to do */
#else
	if(size != pdb->size) {
		ov_database_unmap();
		return OV_ERR_BADDATABASE;
	}
#endif
	/*
	*	test if we got the same base address
	*/
	if((OV_POINTER)pdb != pdb->baseaddr) {
		result = ov_database_move((OV_BYTE*)pdb-(OV_BYTE*)(pdb->baseaddr));
		if(Ov_Fail(result)) {
			ov_database_unmap();
			return result;
		}
	}
	/*
	*	restart the database memory pool
	*/
	if(!ml_restart(pmpinfo, pdb->pstart, ov_database_morecore)) {
		ov_database_unmap();
		return OV_ERR_BADDATABASE;
	}
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
*	Map an existing database part 2
*/
OV_DLLFNCEXPORT OV_RESULT ov_database_map_loadlib(
	OV_STRING	filename
) {
	/*
	*	local variables
	*/
	OV_RESULT	result;
	/*
	*	load the OV library
	*/
	result = ov_library_compare(&pdb->ov, &OV_LIBRARY_DEF_ov);
	if(Ov_Fail(result)) {
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
OV_DLLFNCEXPORT void ov_database_unmap(void) {
	/*
	*	local variables
	*/
#if OV_SYSTEM_OPENVMS
	OV_BYTE	*inaddr[2];
#endif
	/*
	*	instructions
	*/
	/*
	*	shut down database if not already done
	*/
	if (pdb)	ov_database_shutdown();
#if OV_SYSTEM_UNIX
	/*
	*	unmap the file and close it
	*/
#if OV_SYSTEM_SOLARIS
	if (pdb)	munmap((caddr_t )pdb, pdb->size);
#else
	if (pdb) munmap(pdb, pdb->size);
#endif
	if (fd) close(fd);
#endif
#if OV_SYSTEM_NT
	/*
	*	unmap the file and close all handles
	*/
	if (pdb)   UnmapViewOfFile(pdb);
	if (hmap)  CloseHandle(hmap);
	if (hfile) CloseHandle(hfile);
#endif
#if OV_SYSTEM_RMOS
	/*
	*	flush the database file, close it and free the database memory
	*/
	if (pdb)  ov_database_flush();
	if (file) fclose(file);
	if (pdb)  Ov_HeapFree(pdb);
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
	pdb = NULL;
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
#if OV_SYSTEM_OPENVMS
	OV_BYTE	*inaddr[2];
#endif

	/*
	*	instructions
	*/
	if(pdb) {
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
	msync((void*)pdb, pdb->size, MS_SYNC);
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

	FILE				*backupfile;
	
	if (!dbname) return OV_ERR_BADPARAM;
	backupfile = fopen(dbname, "wb+");
	if(!backupfile) {
		return OV_ERR_CANTOPENFILE;
	}
	fseek(backupfile, 0, SEEK_SET);
	fwrite((char*)pdb, pdb->size, 1, backupfile);
	fclose(backupfile);
	return OV_ERR_OK;
}
/*	----------------------------------------------------------------------	*/

/*
*	Initialize the database (subroutine)
*/
void ov_database_init(void) {
	/*
	*	local variables
	*/
	OV_INSTPTR_ov_library		plib;
	OV_INSTPTR_ov_association	passoc;
	OV_INSTPTR_ov_class			pclass;
	OV_INSTPTR_ov_variable		pvar;
	/*
	*	iterate over all libraries except the OV library
	*/
	Ov_ForEachChildEx(ov_instantiation, pclass_ov_library, plib, ov_library) {
		if(plib != &pdb->ov) {
			/*
			*	Set the DLL/shared library handle to zero
			*/
			plib->v_handle = 0;
			/*
			*	Iterate over all associations of the libray
			*/
			Ov_ForEachChildEx(ov_containment, plib, passoc, ov_association) {
				/*
				*	Set function pointers to NULL
				*/
				passoc->v_linkfnc = NULL;
				passoc->v_unlinkfnc = NULL;
				passoc->v_getaccessfnc = NULL;
			}	/* associations */
			/*
			*	Iterate over all classes of the libray
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
				}	/* variables */
			}	/* classes */
		}	/* if */
	}	/* libraries */
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
	OV_INSTPTR_ov_library	plib;
	OV_LIBRARY_DEF			*plibdef;
	OV_RESULT				result;
	/*
	*	don't start if already started
	*/
	if(pdb->started) {
		return OV_ERR_GENERIC;
	}

	if((pdb->root.v_objectstate & OV_OS_STARTED))
	{
		ov_logfile_warning("Trying to start database with root object marked as started. Probably the database was NOT shut down properly before and is corrupted. Will clear the flag and abort. The next try to start the database will succeed, but the server may crash.");
		pdb->root.v_objectstate &= ~OV_OS_STARTED;
		return OV_ERR_BADDATABASE;
	}
	/*
	*	open and compare all libraries 
	*/
	Ov_ForEachChildEx(ov_instantiation, pclass_ov_library, plib, ov_library) {
		plibdef = ov_library_open(plib);
		if(!plibdef) {
			return OV_ERR_CANTOPENLIBRARY;
		}
		result = ov_library_compare(plib, plibdef);
		if(Ov_Fail(result)) {
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
	OV_INSTPTR_ov_library	plib;
	/*
	*	shut down database
	*/
	if(pdb->started) {
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
		*/
		Ov_ForEachChildEx(ov_instantiation, pclass_ov_library, plib, ov_library) {
			ov_library_close(plib);
		}
		pdb->started = FALSE;
	}
}

/*	----------------------------------------------------------------------	*/

/*
*	Allocate memory in the database
*/
OV_DLLFNCEXPORT OV_POINTER ov_database_malloc(
	OV_UINT		size
) {
#ifdef OV_VALGRIND
    if (ov_path_getobjectpointer("/acplt/malloc", 2)) {
		return malloc(size);
	}
#endif
	if(pdb) {
		return ml_malloc(size);
	}
	return NULL;
}

/*	----------------------------------------------------------------------	*/

/*
*	Reallocate memory in the database
*/
OV_DLLFNCEXPORT OV_POINTER ov_database_realloc(
	OV_POINTER	ptr,
	OV_UINT		size
) {
#ifdef OV_VALGRIND
	if(pdb && ptr != 0 && ov_path_getobjectpointer("/acplt/malloc", 2)==NULL){
		if(!((uintptr_t)ptr >= (uintptr_t)pdb->baseaddr && (uintptr_t)ptr <= (uintptr_t)pdb->baseaddr+(uintptr_t)pdb->size)){
			printf("realloc missed database, install a breakpoint here\n");
		}
	}
	if (ov_path_getobjectpointer("/acplt/malloc", 2)) {
		if(ptr != 0 && pdb && ptr >= pdb->baseaddr && ptr <= pdb->baseaddr+pdb->size){
			return ml_realloc(ptr, size);
		}else{
			return realloc(ptr, size);
		}
	}
#endif
	if(pdb) {
		return ml_realloc(ptr, size);
	}
	return NULL;
}

/*	----------------------------------------------------------------------	*/

/*
*	Free memory in the database
*/
OV_DLLFNCEXPORT void ov_database_free(
	OV_POINTER	ptr
) {
#ifdef OV_VALGRIND
	if(pdb && ptr != 0 && ov_path_getobjectpointer("/acplt/malloc", 2)==NULL){
		if(!((uintptr_t)ptr >= (uintptr_t)pdb->baseaddr && (uintptr_t)ptr <= (uintptr_t)pdb->baseaddr+(uintptr_t)pdb->size)){
			printf("free missed database, install a breakpoint here\n");
		}
	}
	if (ov_path_getobjectpointer("/acplt/malloc", 2)) {
		if(ptr != 0 && pdb && ptr >= pdb->baseaddr && ptr <= pdb->baseaddr+pdb->size){
			ml_free(ptr);
		}else{
			free(ptr);
		}
		return;
	}
#endif
	if(pdb) {
		ml_free(ptr);
	}
}

/*	----------------------------------------------------------------------	*/

/*
*	Get size of the database
*/
OV_DLLFNCEXPORT OV_UINT ov_database_getsize(void) {
	if(pdb) {
		return pdb->size;
	}
	return 0;
}

/*	----------------------------------------------------------------------	*/

/*
*	Get free storage in the database
*/
OV_DLLFNCEXPORT OV_UINT ov_database_getfree(void) {
	if(pdb) {
		#ifdef _STDINT_H
			uintptr_t free;
		#else
			OV_UINT free;
			#define uintptr_t OV_UINT
		#endif
		free = (uintptr_t)pdb->pend - (uintptr_t)pdb->pcurr + (uintptr_t)pmpinfo->bytes_free;
		if(free > OV_VL_MAXUINT){
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
	if(pdb) {
		used = (uintptr_t)pdb->pstart - (uintptr_t)pdb->baseaddr + (uintptr_t)pmpinfo->bytes_used - (uintptr_t)pmpinfo->bytes_free;
		if(used > OV_VL_MAXUINT){
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
OV_DLLFNCEXPORT OV_UINT ov_database_getfrag(void) {
	/*
	*	local variables
	*/
	OV_UINT					     i, num, sumsize, avgsize, logavgsize, sum;
	OV_INSTPTR_ov_class		     pclass;
	struct __ml_list OV_MEMSPEC	*pnext;
	/*
	*	calculate average instance size
	*/
	if(pdb) {
		num = sumsize = 0;
		Ov_ForEachChildEx(ov_instantiation, pclass_ov_class, pclass, ov_class) {
			num++;
			sumsize += pclass->v_size;
		}
		avgsize = sumsize/num;
		/*
		*	calculate the log (base of 2) of the average instance size
		*/
		for(logavgsize=0; avgsize; avgsize >>= 1) {
			logavgsize++;
		}
		avgsize = sumsize/num;
		/*
		*	find out how many blocks of the average size there are
		*/
		sum = 0;
		for(i=logavgsize; i<BLOCKLOG; i++) {
			num = 0;
			for(pnext = pmpinfo->fraghead[i].next; pnext; pnext=pnext->next) {
				num++;
			}
			sum += num*(1<<(i-logavgsize));
		}
		sum += (pdb->pend-pdb->pcurr)/avgsize;
		return ((ov_database_getfree()-sum*avgsize)*100)/ov_database_getfree();
	}
	return 100;
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
OV_RESULT ov_database_move(
	const OV_INT	distance
) {
	/*
	*	local variables
	*/
	OV_DATABASE_INFO	*pdbcopy;
	int					i;
	struct __ml_list	*phead;
	OV_RESULT			result;
	OV_INSTPTR_ov_object 		pobj;
	OV_INST_ov_association 		assoc_inheritance;
	OV_INST_ov_association 		assoc_childrelationship;
	OV_INST_ov_association 		assoc_parentrelationship;
	OV_IDLIST_NODE				*pCurrNode = NULL;
	OV_UINT						iterator;
	/*
	*	search inheritance, childrelationship, parentrelationship association
	*/
	passoc_ov_containment = &pdb->containment;
	passoc_ov_instantiation = &pdb->instantiation;

	assoc_inheritance.v_assoctype = OV_ASSOCIATION_DEF_ov_inheritance.assoctype;
	assoc_inheritance.v_assocprops = OV_ASSOCIATION_DEF_ov_inheritance.assocprops;
	assoc_childrelationship.v_assoctype = OV_ASSOCIATION_DEF_ov_childrelationship.assoctype;
	assoc_childrelationship.v_assocprops = OV_ASSOCIATION_DEF_ov_childrelationship.assocprops;
	assoc_parentrelationship.v_assoctype = OV_ASSOCIATION_DEF_ov_parentrelationship.assoctype;
	assoc_parentrelationship.v_assocprops = OV_ASSOCIATION_DEF_ov_parentrelationship.assocprops;

	pobj = (OV_INSTPTR_ov_object) (((OV_HEAD*) (pdb->ov.v_linktable + distance + passoc_ov_containment->v_parentoffset))->pfirst);
	while (pobj) { 
		pobj = (OV_INSTPTR_ov_object) ((char*)pobj + distance);
		if (!strcmp(pobj->v_identifier+distance, "inheritance")) {
			assoc_inheritance.v_parentoffset = ((OV_INSTPTR_ov_association) pobj)->v_parentoffset;
			assoc_inheritance.v_childoffset = ((OV_INSTPTR_ov_association) pobj)->v_childoffset;
		}
		if (!strcmp(pobj->v_identifier+distance, "childrelationship")) {
			assoc_childrelationship.v_parentoffset = ((OV_INSTPTR_ov_association) pobj)->v_parentoffset;
			assoc_childrelationship.v_childoffset = ((OV_INSTPTR_ov_association) pobj)->v_childoffset;
		}
		if (!strcmp(pobj->v_identifier+distance, "parentrelationship")) {
			assoc_parentrelationship.v_parentoffset = ((OV_INSTPTR_ov_association) pobj)->v_parentoffset;
			assoc_parentrelationship.v_childoffset = ((OV_INSTPTR_ov_association) pobj)->v_childoffset;
		}
		pobj = (OV_INSTPTR_ov_object) (((OV_ANCHOR*) (pobj->v_linktable + distance + passoc_ov_containment->v_childoffset))->pnext);
	}

	passoc_ov_inheritance = &assoc_inheritance;
	passoc_ov_childrelationship = &assoc_childrelationship;
	passoc_ov_parentrelationship = &assoc_parentrelationship;
	/*
	*	make a copy of the original database
	*/
	pdbcopy = (OV_DATABASE_INFO*)Ov_HeapMalloc(pdb->size);
	if(!pdbcopy) {
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
	if(pdb->idList->pFirst){
		Ov_Adjust(OV_IDLIST_NODE*, pdb->idList->pFirst);
	}
	if(pdb->idList->pLast){
		Ov_Adjust(OV_IDLIST_NODE*, pdb->idList->pLast);
	}
	if(pdb->idList->pNodes){
		Ov_Adjust(OV_IDLIST_NODE**, pdb->idList->pNodes);
	}
	pCurrNode = pdb->idList->pFirst;
	while(pCurrNode){
		if(pCurrNode->pNext){
			Ov_Adjust(OV_IDLIST_NODE*, pCurrNode->pNext);
		}
		if(pCurrNode->pPrevious){
			Ov_Adjust(OV_IDLIST_NODE*, pCurrNode->pPrevious);
		}
		if(pdb->idList->pNodes[pCurrNode->nodeNumber-1]){
			Ov_Adjust(OV_IDLIST_NODE*, pdb->idList->pNodes[pCurrNode->nodeNumber-1]);
		}
		for(iterator = 0; iterator < pCurrNode->relationCount; iterator++){
			Ov_Adjust(OV_INSTPTR_ov_object, pCurrNode->relations[iterator].pobj);
			if((((OV_POINTER)pCurrNode->relations[iterator].pobj) < pdb->baseaddr) || (((OV_BYTE*)pCurrNode->relations[iterator].pobj) > pdb->pend)){
				pCurrNode->relations[iterator].pobj = NULL;
				if(pCurrNode->relations[iterator].idLow != 1 || pCurrNode->relations[iterator].idHigh != 0){ /*	exception for vendortree	*/
					ov_logfile_warning("Pointer in id --> pointer relation points outside database. id: %#X%08X", pCurrNode->relations[iterator].idHigh, pCurrNode->relations[iterator].idLow);
				}
			}
		}
		pCurrNode = pCurrNode->pNext;
	}
	
	/*
	*	adjust pointers of the memory pool
	*/
	Ov_Adjust(__ml_byte_t*, pmpinfo->heapbase);
	Ov_Adjust(__ml_byte_t*, pmpinfo->heapend);
	for(i=0; i<BLOCKLOG; i++) {
		for(phead=&pmpinfo->fraghead[i]; phead; phead=phead->next) {
			Ov_Adjust(struct __ml_list*, phead->next);
			Ov_Adjust(struct __ml_list*, phead->prev);
		}
	}
	for(phead=&pmpinfo->free_blocks; phead; phead=phead->next) {
	   Ov_Adjust(struct __ml_list*, phead->next);
	   Ov_Adjust(struct __ml_list*, phead->prev);
	}
	/*
	*	adjust pointers of the ACPLT/OV objects
	*/
	result = ov_object_move(Ov_PtrUpCast(ov_object, &pdb->root),
		Ov_PtrUpCast(ov_object, &pdbcopy->root), distance);
	if(Ov_Fail(result)) {
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

/*
*	End of file
*/

