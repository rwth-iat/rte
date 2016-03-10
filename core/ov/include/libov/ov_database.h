/*
*   $Id: ov_database.h,v 1.13 2007-04-24 14:11:29 martin Exp $
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
*	13-Apr-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*/

#ifndef OV_DATABASE_H_INCLUDED
#define OV_DATABASE_H_INCLUDED

#include "ov.h"
#include "libov/ov_vendortree.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
*	Maximum size of a database file
*/
#ifndef OV_DATABASE_MAXSIZE
/* #define OV_DATABASE_MAXSIZE	2147483648  2 GByte */
#define OV_DATABASE_MAXSIZE	524288000UL	/* 500 MByte */
#endif


/*
*  Database-Path-Prefix
*  --------------------
*/
#if OV_SYSTEM_NT
#define FOLDER_DELIMITER 			"\\"
#else
#define FOLDER_DELIMITER 			"/"
#endif


#define CONCATENATE_DATABASE_PATH(varname, helpname) \
	helpname = (OV_STRING)malloc(strlen(varname)+strlen(getenv("ACPLT_HOME"))+2); \
	sprintf(helpname, "%s%s%s", getenv("ACPLT_HOME"), FOLDER_DELIMITER, varname); \
	varname = helpname

/*		
 *	List structure for objectId -> object-pointer relation
 */
typedef struct {
	OV_UINT						idHigh;	/*	high dWord of id	*/
	OV_UINT						idLow;	/*	low dWord of id	*/
	OV_INSTPTR_ov_object		pobj;	/* address of object in db */
}	OV_ID_POINTER_RELATION;

#define OV_IDLIST_DEBUG				0			/*	enable debug prints in idList functions when set to 1	*/
#define OV_IDLIST_PERFORMANCEPRINTS	0			/*	enable printing of hit / cycle statistics 
													(to determine performance and optimize CHUNKSIZES)	*/

#define OV_IDLIST_RELATIONSPERCHUNK	512			/*	relations stored in one node
													powers of 2 enable compiler optimizations --> ludicrous speed
													512 seem be good enough to have a more or less even 
													distribution (which gives better performance)
													for large amounts of data and not so many delete operations
													increase this number for better performance
													performance tradeof: higher number --> faster search, slower deletes 
														(and inserts) and vice versa	*/
#define OV_IDLIST_NODELISTCHUNKSIZE	32			/*	size of the chunks in which 
													the nodes-array in headNode is allocated	*/

typedef struct OV_IDLIST_NODE{
	OV_UINT64					minId;		/*	smallest id in this node	*/
	OV_UINT64					maxId;		/*	largest id in this node	*/
	struct OV_IDLIST_NODE		*pPrevious;	/*	pointer to previous node	*/
	struct OV_IDLIST_NODE		*pNext;		/*	pointer to next node	*/
	OV_UINT64					nodeNumber;	/*	this node's number	*/
	OV_UINT						relationCount;	/*	number of relations currently filled	*/
	OV_ID_POINTER_RELATION		relations [OV_IDLIST_RELATIONSPERCHUNK];	/* chunk of relations */
}	OV_IDLIST_NODE;	

typedef struct {
	OV_UINT64					minId;		/*	smallest id	*/
	OV_UINT64					maxId;		/*	largest id	*/
	OV_IDLIST_NODE				*pFirst;	/*	pointer to first node	*/
	OV_IDLIST_NODE				*pLast;		/*	pointer to last node	*/
	OV_IDLIST_NODE				**pNodes;	/*	array of pointers to nodes	*/
	OV_UINT64					nodeCount;	/*	number of nodes	*/
	OV_UINT64					relationCount;	/*	number of relations	*/
}	OV_IDLIST_HEADNODE;	
	
/*
*	OV_DATABASE_INFO:
*	-----------------
*	General information about a database
*/
typedef struct {
	OV_POINTER			baseaddr;	/* base address of the filemapping */
	OV_UINT				size;		/* size of the filemapping TODO_ADJUST_WHEN_LARGE_DB */
	OV_BYTE OV_MEMSPEC		*pstart;	/* beginning of the database memory */
	OV_BYTE OV_MEMSPEC		*pend;		/* end of the database memory */
	OV_BYTE OV_MEMSPEC		*pcurr;		/* current position of the core pointer */
	OV_INST_ov_domain		root;		/* root domain object */
	OV_INST_ov_domain		acplt;		/* the acplt domain object */
	OV_INST_ov_library		ov;		/* OV library object */
	OV_INST_ov_association		containment;	/* OV containment object */
	OV_INST_ov_association		instantiation;	/* OV instantiation object */
	OV_INST_ov_domain		vendordom;	/* vendor domain object */
	OV_INST_ov_object		vendorobj[OV_NUM_VENDOROBJECTS];	/* vendor object */
	OV_STRING			serverpassword;	/* password for server access */
	OV_TIME_SPAN			timeoffset;	/* time offset of servertime to systemtime */
	OV_BOOL				started;	/* true, if database is started up */
	OV_UINT				idCounter1;	/*	high dword of idCounter (is incremtented with each new instance)
									 *	--> numeric id of newest instance in db	*/
	OV_UINT				idCounter0;	/*	low dword of idCounter (is incremtented with each new instance)
									 *	--> numeric id of newest instance in db	*/
	OV_IDLIST_HEADNODE	*idList;		/*	starting point for id -> pointer relations	*/
}	OV_DATABASE_INFO;

/*
*   Database properties for MC164
*/
#if OV_SYSTEM_MC164
extern OV_DATABASE_INFO OV_MEMSPEC	*pdbmem;
#endif

/*
*	Global variable: database pointer and database backup file name
*/
#ifdef OV_COMPILE_LIBOV
/* Manual fix */
#if OV_COMPILER_BORLAND
#define OV_EXTERN extern
#else
#define OV_EXTERN OV_DLLVAREXPORT
#endif
#else
#define OV_EXTERN OV_DLLVARIMPORT
#endif

OV_EXTERN OV_DATABASE_INFO OV_MEMSPEC	*pdb;
OV_EXTERN OV_STRING db_backup_filename;
#undef OV_EXTERN

/**	
 *	Get a numeric id and increment the internal counter
 */
OV_DLLFNCEXPORT OV_BOOL ov_database_getId(OV_UINT *idH, OV_UINT *idL);
/**	
 *	Get a numeric id being set next time and do not increment the internal counter
 *	INTERNAL USE ONLY --> create instances with unique identifier variables mirroring the numeric id
 */
void ov_database_getId_noInc(OV_UINT *idH, OV_UINT *idL);
/**
 *	Convert a high low dword pair to a 64bit id
 */
 OV_DLLFNCEXPORT OV_UINT64 ov_database_convertId(const OV_UINT idH, const OV_UINT idL);
/**	
 *	Relate an object pointer  with an id (creates a new entry in the idList relating object pointer and id)
 *	is called on every object creation
 */
OV_DLLFNCEXPORT OV_RESULT ov_database_idListInsert(const OV_UINT idH, const OV_UINT idL, const OV_INSTPTR_ov_object pInstance);

/**	
 *	Releases an id (deletes it from the list)
 *	is called on every objet deletion
 */
OV_DLLFNCEXPORT OV_RESULT ov_database_idListRelease(const OV_UINT idH, const OV_UINT idL);

/**	get the node (chunk) an id is in
 *	walk along the nodes and check if id is between min and max for this node
 *	if id > idList->maxId / 2 walk from the end, otherwise walkl from the beginning
 */
OV_IDLIST_NODE* ov_database_idListGetNode(const OV_UINT idH, const OV_UINT idL);

/**	get the index of a relation within a node (chunk) by its id
 *	use bisection to find it
 *	returns OV_ERR_GENERIC if it was not found
 */
OV_RESULT ov_database_idListGetRelationIndex(const OV_UINT idH, const OV_UINT idL, const struct OV_IDLIST_NODE *const node, OV_UINT* index);

/*
*	Create a new database
*/
OV_DLLFNCEXPORT OV_RESULT ov_database_create(
	OV_STRING	filename,
	OV_UINT		size
);

/*
*	Map an existing database
*/
OV_DLLFNCEXPORT OV_RESULT ov_database_map(
	OV_STRING	filename
);

/*
*	Map an existing database part 1
*/
OV_DLLFNCEXPORT OV_RESULT ov_database_map_loadfile(
	OV_STRING	filename
);

/*
*	Map an existing database part 2
*/
OV_DLLFNCEXPORT OV_RESULT ov_database_map_loadlib(
	OV_STRING	filename
);

/*
*	Unmap the database
*/
OV_DLLFNCEXPORT void ov_database_unmap(void);

/*
*	Flush the contents of a database
*/
OV_DLLFNCEXPORT void ov_database_flush(void);

/*
*	Write the contents of a database to a backupfile
*/
OV_DLLFNCEXPORT OV_RESULT ov_database_write(OV_STRING dbname);

/*
*	Initialize the database (subroutine)
*/
void ov_database_init(void);

/*
*	Start up the database
*/
OV_DLLFNCEXPORT OV_RESULT ov_database_startup(void);

/*
*	Shut down the database
*/
OV_DLLFNCEXPORT void ov_database_shutdown(void);

/**
*	Allocate memory in the database
*	you should use the macro Ov_DbMalloc() in your code
*/
OV_DLLFNCEXPORT OV_POINTER ov_database_malloc(
	OV_UINT		size
);

/**
*	Reallocate memory in the database
*	you should use the macro Ov_DbRealloc() in your code
*/
OV_DLLFNCEXPORT OV_POINTER ov_database_realloc(
	OV_POINTER	ptr,
	OV_UINT		size
);

/**
*	Free memory in the database
*	you should use the macro Ov_DbFree() in your code
*/
OV_DLLFNCEXPORT void ov_database_free(
	OV_POINTER	ptr
);

/*
*	Get size of the database
*/
OV_DLLFNCEXPORT OV_UINT ov_database_getsize(void);

/*
*	Get free storage in the database
*/
OV_DLLFNCEXPORT OV_UINT ov_database_getfree(void);

/*
*	Get used storage in the database
*/
OV_DLLFNCEXPORT OV_UINT ov_database_getused(void);

/*
*	Get fragmentation of the database
*/
OV_DLLFNCEXPORT OV_UINT ov_database_getfrag(void);

/*
*	Move the database to a new base address (subroutine)
*/
OV_RESULT ov_database_move(
	const OV_INT	distance
);

#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif
/*
*	End of file
*/

