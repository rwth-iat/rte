/*
*   $Id: ov_vendortree.h,v 1.9 2002-06-26 07:13:02 ansgar Exp $
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
*	05-May-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*/

#ifndef OV_VENDORTREE_H_INCLUDED
#define OV_VENDORTREE_H_INCLUDED

#include "ov.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
*	Number of objects in the vendor tree
*/
#define	OV_NUM_VENDOROBJECTS	24

/*
*	Global variables
*/
#ifdef OV_COMPILE_LIBOV
#define OV_EXTERN extern
#else
#define OV_EXTERN OV_DLLVARIMPORT
#endif
OV_EXTERN OV_BOOL activitylock;
OV_EXTERN OV_BOOL backup;
#undef OV_EXTERN

/*
*	OV_FNC_GETVENDORVAR:
*	--------------------
*	Function prototype for getting vendor variables
*/
typedef OV_DLLFNCEXPORT OV_RESULT OV_FNC_GETVENDORVAR(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
);

/*
*	OV_FNC_SETVENDORVAR:
*	--------------------
*	Function prototype for setting vendor variables
*/
typedef OV_DLLFNCEXPORT OV_RESULT OV_FNC_SETVENDORVAR(
	const OV_ANY	*pvarcurrprops,
	const OV_TICKET	*pticket
);

/*
*	OV_VENDORTREE_INFO:
*	-------------------
*	Information to a vendor tree object
*/
typedef struct {
	OV_STRING			identifier;
	OV_STRING			unit;
	OV_FNC_GETVENDORVAR	*getvarfnc;
	OV_FNC_SETVENDORVAR	*setvarfnc;
}	OV_VENDORTREE_INFO;

/*
*	Initialize the vendor tree
*/
OV_RESULT ov_vendortree_init(void);

/*
*	Get unit of a vendor object
*/
OV_DLLFNCEXPORT OV_STRING ov_vendortree_getunit(
	OV_INSTPTR_ov_object	pobj
);

/*
*	Get variable of a vendor object
*/
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getvar(
	OV_INSTPTR_ov_object	pobj,
	OV_ANY			*pvarcurrprops,
	const OV_TICKET		*pticket
);

/*
*	Set variable of a vendor object
*/
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_setvar(
	OV_INSTPTR_ov_object	pobj,
	const OV_ANY		*pvarcurrprops,
	const OV_TICKET		*pticket
);

/*
*	Set database name
*/
OV_DLLFNCEXPORT void ov_vendortree_setdatabasename(
	OV_STRING	name
);

/*
*	Set vendor name
*/
OV_DLLFNCEXPORT void ov_vendortree_setname(
	OV_STRING	name
);

/*
*	Set semantic flag
*/
OV_DLLFNCEXPORT void ov_vendortree_setsemanticflag(
	OV_UINT		flagnum,
	OV_STRING	flagvalue
);

/*
*	Set server name
*/
OV_DLLFNCEXPORT void ov_vendortree_setservername(
	OV_STRING	name
);

/*
*	Set server description
*/
OV_DLLFNCEXPORT void ov_vendortree_setserverdescription(
	OV_STRING	name
);

/*
*	Set server version
*/
OV_DLLFNCEXPORT void ov_vendortree_setserverversion(
	OV_STRING	name
);

/*
*	Set startup time
*/
OV_DLLFNCEXPORT void ov_vendortree_setstartuptime(
	OV_TIME		*ptime
);

/*
*	Get list of associations in the database
*/
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getassociations(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
);

/*
*	Get list of classes in the database
*/
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getclasses(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
);

/*
*	Get fragmentation of the database
*/
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getdatabasefrag(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
);

/*
*	Get free storage of the database
*/
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getdatabasefree(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
);

/*
*	Get database name
*/
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getdatabasename(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
);

/*
*	Get size of the database
*/
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getdatabasesize(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
);

/*
*	Get whether the database is started or not
*/
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getdatabasestarted(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
);

/*
*	Get used storage of the database
*/
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getdatabaseused(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
);

/*
*	Get vendor name
*/
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getname(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
);

/*
*	Get libks version
*/
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getlibksversion(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
);

/*
*	Get libov version
*/
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getlibovversion(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
);

/*
*	Get libovks version
*/
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getlibovksversion(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
);

/*
*	Get list of libraries in the database
*/
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getlibraries(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
);

/*
*	Get list of semantic flags in the database
*/
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getsemanticflags(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
);

/*
*	Get server description
*/
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getserverdescription(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
);

/*
*	Get server name
*/
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getservername(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
);

/*
*	Get server time
*/
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getservertime(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
);

/*
*	Get server version
*/
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getserverversion(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
);

/*
*	Get startup time
*/
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getstartuptime(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
);

/*
*	Get list of structures in the database
*/
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getstructures(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
);

/*
*	Get activitylock
*/
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getactivitylock(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
);

/*
*	Set activitylock
*/
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_setactivitylock(
	const OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
);

/*
*	Get backup
*/
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getbackup(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
);

/*
*	Set serverpassword
*/
OV_DLLFNCEXPORT void ov_vendortree_setserverpassword(
	OV_STRING	password
);

/*
*	Set serverpassword external
*/
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_setserverpassword_ext(
	const OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
);

/*
*	Get serverpassword
*/
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_getserverpassword(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
);

/*
*	Get timeoffset
*/
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_gettimeoffset(
	OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
);

/*
*	Set timeoffset
*/
OV_DLLFNCEXPORT OV_RESULT ov_vendortree_settimeoffset(
	const OV_ANY			*pvarcurrprops,
	const OV_TICKET	*pticket
);

#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif
/*
*	End of file
*/

