/*
*   $Id: ov_database.h,v 1.9 2003-01-31 10:04:46 ansgar Exp $
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
#define OV_DATABASE_MAXSIZE	2147483648	/* 2 GByte */
#endif

/*
*	OV_DATABASE_INFO:
*	-----------------
*	General information about a database
*/
typedef struct {
	OV_POINTER			baseaddr;	/* base address of the filemapping */
	OV_UINT				size;		/* size of the filemapping */
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
}	OV_DATABASE_INFO;

/*
*   Database properties for MC164
*/
#if OV_SYSTEM_MC164
extern OV_DATABASE_INFO OV_MEMSPEC	*pdbmem;
#endif

/*
*	Global variable: database pointer
*/
#ifdef OV_COMPILE_LIBOV
#define OV_EXTERN extern
#else
#define OV_EXTERN OV_DLLVARIMPORT
#endif
OV_EXTERN OV_DATABASE_INFO OV_MEMSPEC	*pdb;
#undef OV_EXTERN

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

/*
*	Allocate memory in the database
*/
OV_DLLFNCEXPORT OV_POINTER ov_database_malloc(
	OV_UINT		size
);

/*
*	Reallocate memory in the database
*/
OV_DLLFNCEXPORT OV_POINTER ov_database_realloc(
	OV_POINTER	ptr,
	OV_UINT		size
);

/*
*	Free memory in the database
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

