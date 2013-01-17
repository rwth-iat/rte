/*
*   $Id: ov_ksserver_stripped.h,v 1.1 2011-06-09 13:54:40 henning Exp $
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
*	28-Jul-1998 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*	21-Apr-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: Major revision.
*	02-Apr-2001 Ansgar M�nnemann <ansgar@plt.rwth-aachen.de>: adjustings for KsGetHist-service.
*/

#ifndef OV_KSSERVER_H_INCLUDED
#define OV_KSSERVER_H_INCLUDED

//#include "libovks/ov_ksserver_xdr.h"
#include "libov/ov_ov.h"
#include "libov/ov_element.h"
#include "libov/ov_memstack.h"
#include "libov/ov_logfile.h"


/*
*	OV_FNC_SIGHANDLER:
*	------------------
*	Signal hander function prototype for server shutdown
*/
typedef OV_DLLFNCEXPORT void OV_FNC_SIGHANDLER(
	int signal
);

/*
*	Flags for installing the signal handler
*/
#define OV_SIGHANDLER_DEFAULT	((OV_FNC_SIGHANDLER *)0)
#define OV_SIGHANDLER_NONE		((OV_FNC_SIGHANDLER *)-1)

/*
*	Create a ticket from an incoming XDR stream (internal)
*/
OV_TICKET *ov_ksserver_ticket_create(XDR *xdr);

//by hmersch
OV_TICKET *ov_ksserver_ticket_defaultticket_createticket_wrap(XDR *xdr, OV_TICKET_TYPE type);

/*
*	Register a new ticket
*/
OV_DLLFNCEXPORT OV_RESULT ov_ksserver_ticket_register(
	const OV_TICKET_TYPE	type,
	const OV_TICKET_VTBL	*vtbl
);

/*
*	Unregister a given ticket
*/
OV_DLLFNCEXPORT OV_RESULT ov_ksserver_ticket_unregister(
	const OV_TICKET_TYPE	type
);

/*
*	OV_SVCLOG_VTBL:
*	---------------
*	VTable for service logging
*/
struct OV_SVCLOG_VTBL {
	void 		(* logsvc)(OV_TICKET *pticket, OV_SVC svc, void* ppar, OV_RESULT *pres, OV_RESULT svcres);
};
typedef struct OV_SVCLOG_VTBL	OV_SVCLOG_VTBL;

/* Register service logging */
OV_DLLFNCEXPORT OV_RESULT ov_ksserver_svclog_register(
	const OV_SVCLOG_VTBL	*vtbl
);
/* Unregister service logging */
OV_DLLFNCEXPORT OV_RESULT ov_ksserver_svclog_unregister(void);


/*
*	Set access rights for default tickets
*/
OV_DLLFNCEXPORT void ov_ksserver_ticket_setdefaultaccess(OV_ACCESS access);


/*
*	Execute the GetPP service (subroutine)
*/
void ov_ksserver_getpp(
	const OV_UINT		version,
	const OV_TICKET		*pticket,
	const OV_GETPP_PAR	*params,
	OV_GETPP_RES		*result
);

/*
*	Add an item to the list of projected properties (subroutine)
*/
OV_RESULT ov_ksserver_getpp_additem(
	const OV_UINT		version,
	const OV_TICKET		*pticket,
	OV_GETPP_RES		*result,
	const OV_ELEMENT	*pelem,
	const KS_OBJ_TYPE	typemask,
	const OV_STRING		namemask
);

/*
*	Execute the GetVar service (subroutine)
*/
void ov_ksserver_getvar(
	const OV_UINT		version,
	const OV_TICKET		*pticket,
	const OV_GETVAR_PAR	*params,
	OV_GETVAR_RES		*result
);

/*
*	Get a variable item of the GetVar service (subroutine)
*/
void ov_ksserver_getvar_getitem(
	const OV_UINT		version,
	const OV_TICKET		*pticket,
	OV_GETVAR_ITEM		*pitem,
	const OV_ELEMENT	*pelem
);

/*
*	Execute the SetVar service (subroutine)
*/
void ov_ksserver_setvar(
	const OV_UINT		version,
	const OV_TICKET		*pticket,
	const OV_SETVAR_PAR	*params,
	OV_SETVAR_RES		*result
);

/*
*	Set a variable item of the SetVar service (subroutine)
*/
OV_RESULT ov_ksserver_setvar_setitem(
	const OV_TICKET		*pticket,
	OV_SETVAR_ITEM		*pitem,
	const OV_ELEMENT	*pelem,
	OV_BOOL				init
);

/*
*	Execute the GetEP service (subroutine)
*/
void ov_ksserver_getep(
	const OV_UINT		version,
	const OV_TICKET		*pticket,
	const OV_GETEP_PAR	*params,
	OV_GETEP_RES		*result
);

/*
*	Add an item to the list of engineered properties (subroutine)
*/
OV_RESULT ov_ksserver_getep_additem(
	const OV_UINT		version,
	const OV_TICKET		*pticket,
	OV_GETEP_RES		*result,
	const OV_ELEMENT	*pelem,
	const KS_OBJ_TYPE	typemask,
	const OV_STRING		namemask
);


/*
*	Execute the Link service (subroutine)
*/
void ov_ksserver_link(
	const OV_UINT		version,
	const OV_TICKET		*pticket,
	const OV_LINK_PAR	*params,
	OV_LINK_RES			*result
);

/*
*	Link a link item of the Link service (subroutine)
*/
OV_RESULT ov_ksserver_link_linkitem(
	const OV_TICKET		*pticket,
	OV_ELEMENT			*plinkelem,
	OV_ELEMENT			*pelementelem,
	OV_ELEMENT			*pplaceelem,
	OV_ELEMENT			*poppositeplaceelem,
	OV_PLACEMENT_HINT	hint,
	OV_PLACEMENT_HINT	oppositehint
);

/*
*	Execute the DataExchange service (subroutine)
*/
void ov_ksserver_exgdata(
	const OV_UINT			version,
	const OV_TICKET			*pticket,
	const OV_EXGDATA_PAR	*params,
	OV_EXGDATA_RES			*result
);

/*
*	Execute the CreateObject service (subroutine)
*/
void ov_ksserver_createobject(
	const OV_UINT				version,
	const OV_TICKET				*pticket,
	const OV_CREATEOBJECT_PAR	*params,
	OV_CREATEOBJECT_RES			*result
);

/*
*	Initialize an object during instantiation in the CreateObject service
*/
OV_RESULT ov_ksserver_createobject_initobj(
	OV_INSTPTR_ov_object	pobj,
	OV_POINTER				pinfo
);

/*
*	Execute the DeleteObject service (subroutine)
*/
void ov_ksserver_deleteobject(
	const OV_UINT				version,
	const OV_TICKET				*pticket,
	const OV_DELETEOBJECT_PAR	*params,
	OV_DELETEOBJECT_RES			*result
);

/*
*	Execute the RenameObject service (subroutine)
*/
void ov_ksserver_renameobject(
	const OV_UINT				version,
	const OV_TICKET				*pticket,
	const OV_RENAMEOBJECT_PAR	*params,
	OV_RENAMEOBJECT_RES			*result
);

/*
*	Execute the GetCanonicalPath service (subroutine)
*/
void ov_ksserver_getcanonicalpath(
	const OV_UINT					version,
	const OV_TICKET					*pticket,
	const OV_GETCANONICALPATH_PAR	*params,
	OV_GETCANONICALPATH_RES			*result
);

/*
*	Execute the Unlink service (subroutine)
*/
void ov_ksserver_unlink(
	const OV_UINT		version,
	const OV_TICKET		*pticket,
	const OV_UNLINK_PAR	*params,
	OV_UNLINK_RES		*result
);

/*
*	Unlink an unlink item of the Unlink service (subroutine)
*/
OV_RESULT ov_ksserver_unlink_unlinkitem(
	const OV_TICKET		*pticket,
	OV_ELEMENT			*plinkelem,
	OV_ELEMENT			*pelementelem
);

/*
*	Execute the GetHist service (subroutine)
*/
void ov_ksserver_gethist(
	const OV_UINT			version,
	const OV_TICKET			*pticket,
	const OV_GETHIST_PAR	*params,
	OV_GETHIST_RES			*result
);

OV_DLLFNCEXPORT OV_RESULT ov_supervised_database_startup(void);

OV_DLLFNCEXPORT OV_RESULT ov_supervised_database_map(
	OV_STRING 	dbname
);



#endif

/*
*	End of file
*/

