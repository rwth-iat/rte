/*
*   $Id: ov_vendortree.h,v 1.2 1999-08-19 11:54:49 dirk Exp $
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
#define	OV_NUM_VENDOROBJECTS	20

/*
*	OV_FNC_GETVENDORVAR:
*	--------------------
*	Function prototype for getting vendor variables
*/
typedef OV_RESULT OV_DLLFNCEXPORT OV_FNC_GETVENDORVAR(
	OV_VAR_CURRENT_PROPS	*pvarcurrprops,
	const OV_TICKET			*pticket
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
}	OV_VENDORTREE_INFO;

/*
*	Initialize the vendor tree
*/
void ov_vendortree_init(void);

/*
*	Get unit of a vendor object
*/
OV_STRING OV_DLLFNCEXPORT ov_vendortree_getunit(
	OV_INSTPTR_ov_object	pobj
);

/*
*	Get variable of a vendor object
*/
OV_RESULT OV_DLLFNCEXPORT ov_vendortree_getvar(
	OV_INSTPTR_ov_object	pobj,
	OV_VAR_CURRENT_PROPS	*pvarcurrprops,
	const OV_TICKET			*pticket
);

/*
*	Set database name
*/
void OV_DLLFNCEXPORT ov_vendortree_setdatabasename(
	OV_STRING	name
);

/*
*	Set vendor name
*/
void OV_DLLFNCEXPORT ov_vendortree_setname(
	OV_STRING	name
);

/*
*	Set semantic flag
*/
void OV_DLLFNCEXPORT ov_vendortree_setsemanticflag(
	OV_UINT		flagnum,
	OV_STRING	flagvalue
);

/*
*	Set server name
*/
void OV_DLLFNCEXPORT ov_vendortree_setservername(
	OV_STRING	name
);

/*
*	Set server description
*/
void OV_DLLFNCEXPORT ov_vendortree_setserverdescription(
	OV_STRING	name
);

/*
*	Set server version
*/
void OV_DLLFNCEXPORT ov_vendortree_setserverversion(
	OV_STRING	name
);

/*
*	Set startup time
*/
void OV_DLLFNCEXPORT ov_vendortree_setstartuptime(
	OV_TIME		*ptime
);

/*
*	Get list of associations in the database
*/
OV_RESULT OV_DLLFNCEXPORT ov_vendortree_getassociations(
	OV_VAR_CURRENT_PROPS	*pvarcurrprops,
	const OV_TICKET			*pticket
);

/*
*	Get list of classes in the database
*/
OV_RESULT OV_DLLFNCEXPORT ov_vendortree_getclasses(
	OV_VAR_CURRENT_PROPS	*pvarcurrprops,
	const OV_TICKET			*pticket
);

/*
*	Get fragmentation of the database
*/
OV_RESULT OV_DLLFNCEXPORT ov_vendortree_getdatabasefrag(
	OV_VAR_CURRENT_PROPS	*pvarcurrprops,
	const OV_TICKET			*pticket
);

/*
*	Get free storage of the database
*/
OV_RESULT OV_DLLFNCEXPORT ov_vendortree_getdatabasefree(
	OV_VAR_CURRENT_PROPS	*pvarcurrprops,
	const OV_TICKET			*pticket
);

/*
*	Get database name
*/
OV_RESULT OV_DLLFNCEXPORT ov_vendortree_getdatabasename(
	OV_VAR_CURRENT_PROPS	*pvarcurrprops,
	const OV_TICKET			*pticket
);

/*
*	Get size of the database
*/
OV_RESULT OV_DLLFNCEXPORT ov_vendortree_getdatabasesize(
	OV_VAR_CURRENT_PROPS	*pvarcurrprops,
	const OV_TICKET			*pticket
);

/*
*	Get whether the database is started or not
*/
OV_RESULT OV_DLLFNCEXPORT ov_vendortree_getdatabasestarted(
	OV_VAR_CURRENT_PROPS	*pvarcurrprops,
	const OV_TICKET			*pticket
);

/*
*	Get used storage of the database
*/
OV_RESULT OV_DLLFNCEXPORT ov_vendortree_getdatabaseused(
	OV_VAR_CURRENT_PROPS	*pvarcurrprops,
	const OV_TICKET			*pticket
);

/*
*	Get vendor name
*/
OV_RESULT OV_DLLFNCEXPORT ov_vendortree_getname(
	OV_VAR_CURRENT_PROPS	*pvarcurrprops,
	const OV_TICKET			*pticket
);

/*
*	Get libks version
*/
OV_RESULT OV_DLLFNCEXPORT ov_vendortree_getlibksversion(
	OV_VAR_CURRENT_PROPS	*pvarcurrprops,
	const OV_TICKET			*pticket
);

/*
*	Get libov version
*/
OV_RESULT OV_DLLFNCEXPORT ov_vendortree_getlibovversion(
	OV_VAR_CURRENT_PROPS	*pvarcurrprops,
	const OV_TICKET			*pticket
);

/*
*	Get libovks version
*/
OV_RESULT OV_DLLFNCEXPORT ov_vendortree_getlibovksversion(
	OV_VAR_CURRENT_PROPS	*pvarcurrprops,
	const OV_TICKET			*pticket
);

/*
*	Get list of libraries in the database
*/
OV_RESULT OV_DLLFNCEXPORT ov_vendortree_getlibraries(
	OV_VAR_CURRENT_PROPS	*pvarcurrprops,
	const OV_TICKET			*pticket
);

/*
*	Get list of semantic flags in the database
*/
OV_RESULT OV_DLLFNCEXPORT ov_vendortree_getsemanticflags(
	OV_VAR_CURRENT_PROPS	*pvarcurrprops,
	const OV_TICKET			*pticket
);

/*
*	Get server description
*/
OV_RESULT OV_DLLFNCEXPORT ov_vendortree_getserverdescription(
	OV_VAR_CURRENT_PROPS	*pvarcurrprops,
	const OV_TICKET			*pticket
);

/*
*	Get server name
*/
OV_RESULT OV_DLLFNCEXPORT ov_vendortree_getservername(
	OV_VAR_CURRENT_PROPS	*pvarcurrprops,
	const OV_TICKET			*pticket
);

/*
*	Get server time
*/
OV_RESULT OV_DLLFNCEXPORT ov_vendortree_getservertime(
	OV_VAR_CURRENT_PROPS	*pvarcurrprops,
	const OV_TICKET			*pticket
);

/*
*	Get server version
*/
OV_RESULT OV_DLLFNCEXPORT ov_vendortree_getserverversion(
	OV_VAR_CURRENT_PROPS	*pvarcurrprops,
	const OV_TICKET			*pticket
);

/*
*	Get startup time
*/
OV_RESULT OV_DLLFNCEXPORT ov_vendortree_getstartuptime(
	OV_VAR_CURRENT_PROPS	*pvarcurrprops,
	const OV_TICKET			*pticket
);

/*
*	Get list of structures in the database
*/
OV_RESULT OV_DLLFNCEXPORT ov_vendortree_getstructures(
	OV_VAR_CURRENT_PROPS	*pvarcurrprops,
	const OV_TICKET			*pticket
);

#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif
/*
*	End of file
*/

