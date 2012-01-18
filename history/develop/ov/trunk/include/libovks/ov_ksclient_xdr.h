/*
*   $Id: ov_ksclient_xdr.h,v 1.4 1999-09-06 12:16:57 dirk Exp $
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
*	03-May-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*/
/*
*	Description:
*	------------
*	These XDR routines are mostly based on the output of the RPC code
*	generator rpcgen. In contrast to the routines declared in the file
*	ov_ksserver_xdr.h, these routines work with the heap memory.
*
*	If you introduce changes here, you should probably take a look at
*	ov_ksserver_xdr.h too.
*/

#ifndef OV_KSCLIENT_XDR_H_INCLUDED
#define OV_KSCLIENT_XDR_H_INCLUDED

#include "libovks/ov_ksserver_xdr.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
*	shared datatypes ACPLT/KS -- ACPLT/OV
*/
typedef OV_BOOL					KS_BOOL;
typedef OV_UINT					KS_UINT;
typedef OV_STRING				KS_STRING;
typedef OV_TIME					KS_TIME;
typedef OV_TIME_SPAN			KS_TIME_SPAN;
typedef OV_VAR_VALUE			KS_VAR_VALUE;
typedef OV_TICKET				KS_AVMODULE;
typedef OV_VAR_CURRENT_PROPS	KS_VAR_CURRENT_PROPS;

typedef KS_AVMODULE				KS_AVMODULE_PAR;
typedef KS_AVMODULE				KS_AVMODULE_RES;

/*
*	KS_VAR_PROJECTED_PROPS:
*	-----------------------
*	Projected properites of a variable.
*/
typedef OV_VAR_PROJECTED_PROPS	KS_VAR_PROJECTED_PROPS;

/*
*	KS_LINK_PROJECTED_PROPS:
*	------------------------
*	Projected properites of a link.
*/
typedef OV_LINK_PROJECTED_PROPS	KS_LINK_PROJECTED_PROPS;

/*
*	KS_OBJ_PROJECTED_PROPS:
*	-----------------------
*	Projected properties of an communication object.
*/
typedef struct {
	KS_OBJ_TYPE	objtype;
	union {
		KS_VAR_PROJECTED_PROPS	var_projected_props;
		KS_LINK_PROJECTED_PROPS	link_projected_props;
	}	KS_OBJ_PROJECTED_PROPS_u;
	KS_STRING	identifier;
	KS_TIME		creation_time;
	KS_STRING	comment;
	KS_ACCESS	access;
}	KS_OBJ_PROJECTED_PROPS;

/*
*	KS_GETPP_PAR:
*	-------------
*	Parameters of a GetPP service.
*/
typedef OV_GETPP_PAR	KS_GETPP_PAR;

/*
*	KS_GETPP_RES:
*	-------------
*	Result of a GetPP service.
*/
typedef struct {
	KS_RESULT				result;
	KS_UINT					items_len;	/* only, if result == KS_ERR_OK */
	KS_OBJ_PROJECTED_PROPS	*items_val;	/* only, if result == KS_ERR_OK */
}	KS_GETPP_RES;

/*
*	KS_GETVAR_ITEM:
*	---------------
*	Item of the GetVar/DataExchange service result.
*/
typedef OV_GETVAR_ITEM	KS_GETVAR_ITEM;

/*
*	KS_GETVAR_PAR:
*	--------------
*	Parameters of a GetVar service.
*/
typedef OV_GETVAR_PAR	KS_GETVAR_PAR;

/*
*	KS_GETVAR_RES:
*	--------------
*	Result of a GetVar service.
*/
typedef OV_GETVAR_RES	KS_GETVAR_RES;

/*
*	KS_SETVAR_ITEM:
*	---------------
*	Item of the SetVar/DataExchange service parameters.
*/
typedef OV_SETVAR_ITEM	KS_SETVAR_ITEM;

/*
*	KS_SETVAR_PAR:
*	--------------
*	Parameters of a SetVar service.
*/
typedef OV_SETVAR_PAR	KS_SETVAR_PAR;

/*
*	KS_SETVAR_RES:
*	--------------
*	Result of a SetVar service.
*/
typedef OV_SETVAR_RES	KS_SETVAR_RES;

/*
*	KS_EXGDATA_PAR:
*	---------------
*	Parameters of a DataExchange service.
*/
typedef OV_EXGDATA_PAR	KS_EXGDATA_PAR;

/*
*	KS_EXGDATA_RES:
*	---------------
*	Result of DataExchange service.
*/
typedef OV_EXGDATA_RES	KS_EXGDATA_RES;

/*
*	KS_DOMAIN_ENGINEERED_PROPS:
*	--------------------------
*	Engineered properites of a domain.
*/
typedef OV_DOMAIN_ENGINEERED_PROPS	KS_DOMAIN_ENGINEERED_PROPS;

/*
*	KS_VAR_ENGINEERED_PROPS:
*	-----------------------
*	Engineered properites of a variable.
*/
typedef OV_VAR_ENGINEERED_PROPS	KS_VAR_ENGINEERED_PROPS;

/*
*	KS_LINK_ENGINEERED_PROPS:
*	------------------------
*	Engineered properites of a link.
*/
typedef OV_LINK_ENGINEERED_PROPS	KS_LINK_ENGINEERED_PROPS;

/*
*	KS_OBJ_ENGINEERED_PROPS:
*	-----------------------
*	Engineered properties of an communication object.
*/
typedef struct {
	KS_OBJ_TYPE	objtype;
	union {
		KS_DOMAIN_ENGINEERED_PROPS	domain_engineered_props;
		KS_VAR_ENGINEERED_PROPS		var_engineered_props;
		KS_LINK_ENGINEERED_PROPS	link_engineered_props;
	}	KS_OBJ_ENGINEERED_PROPS_u;
	KS_STRING			identifier;
	KS_TIME				creation_time;
	KS_STRING			comment;
	KS_ACCESS			access;
	KS_SEMANTIC_FLAGS	semantic_flags;
}	KS_OBJ_ENGINEERED_PROPS;

/*
*	KS_GETEP_PAR:
*	-------------
*	Parameters of a GetEP service.
*/
typedef OV_GETEP_PAR	KS_GETEP_PAR;

/*
*	KS_GETEP_RES:
*	-------------
*	Result of a GetEP service.
*/
typedef struct {
	KS_RESULT				result;
	KS_UINT					items_len;	/* only, if result == KS_ERR_OK */
	KS_OBJ_ENGINEERED_PROPS	*items_val;	/* only, if result == KS_ERR_OK */
}	KS_GETEP_RES;

/*
*	KS_PLACEMENT:
*	-------------
*	Placement of a Link/CreateObject service.
*/
typedef OV_PLACEMENT	KS_PLACEMENT;

/*
*	KS_LINK_ITEM:
*	-------------
*	Item of the Link/CreateObject service parameters.
*/
typedef OV_LINK_ITEM	KS_LINK_ITEM;	

/*
*	KS_UNLINK_ITEM:
*	---------------
*	Item of the Link service parameters.
*/
typedef OV_UNLINK_ITEM	KS_UNLINK_ITEM;

/*
*	KS_CREATEOBJ_ITEM:
*	------------------
*	Item of the CreateObject service parameters.
*/
typedef OV_CREATEOBJ_ITEM	KS_CREATEOBJ_ITEM;

/*
*	KS_CREATEOBJECT_PAR:
*	--------------------
*	Parameters of the CreateObject service.
*/
typedef OV_CREATEOBJECT_PAR	KS_CREATEOBJECT_PAR;

/*
*	KS_CREATEOBJECTITEM_RES:
*	------------------------
*	Result item of a CreateObject service result.
*/
typedef OV_CREATEOBJECTITEM_RES	KS_CREATEOBJECTITEM_RES;

/*
*	KS_CREATEOBJECT_RES:
*	--------------------
*	Result of a CreateObject service.
*/
typedef OV_CREATEOBJECT_RES	KS_CREATEOBJECT_RES;

/*
*	KS_DELETEOBJECT_PAR:
*	--------------------
*	Parameters of a DeleteObject service.
*/
typedef OV_DELETEOBJECT_PAR	KS_DELETEOBJECT_PAR;

/*
*	KS_DELETEOBJECT_RES:
*	--------------------
*	Result of a DeleteObject service.
*/
typedef OV_DELETEOBJECT_RES	KS_DELETEOBJECT_RES;

/*
*	KS_RENAMEOBJECT_ITEM:
*	---------------------
*	Item of the RenameObject service parameters.
*/
typedef OV_RENAMEOBJECT_ITEM	KS_RENAMEOBJECT_ITEM;

/*
*	KS_RENAMEOBJECT_PAR:
*	--------------------
*	Parameters of a RenameObject Service
*/
typedef OV_RENAMEOBJECT_PAR	KS_RENAMEOBJECT_PAR;

/*
*	KS_RENAMEOBJECT_RES:
*	--------------------
*	Result of a RenameObject Service
*/
typedef OV_RENAMEOBJECT_RES	KS_RENAMEOBJECT_RES;

/*
*	KS_GETCANONICALPATH_PAR:
*	------------------------
*	Parameters of a GetCanonicalPath Service
*/
typedef OV_GETCANONICALPATH_PAR	KS_GETCANONICALPATH_PAR;

/*
*	KS_GETCANONICALPATHITEM_RES:
*	----------------------------
*	Result item of a GetCanonicalPath Service
*/
typedef OV_GETCANONICALPATHITEM_RES	KS_GETCANONICALPATHITEM_RES;

/*
*	KS_GETCANONICALPATH_RES:
*	------------------------
*	Result of a GetCanonicalPath Service
*/
typedef OV_GETCANONICALPATH_RES	KS_GETCANONICALPATH_RES;

/*
*	KS_LINK_PAR:
*	------------
*	Parameters of a Link service.
*/
typedef OV_LINK_PAR	KS_LINK_PAR;

/*
*	KS_LINK_RES:
*	------------
*	Result of a Link service.
*/
typedef OV_LINK_RES	KS_LINK_RES;

/*
*	KS_UNLINK_PAR:
*	--------------
*	Parameters of an Unlink service.
*/
typedef OV_UNLINK_PAR	KS_UNLINK_PAR;

/*
*	KS_UNLINK_RES:
*	--------------
*	Result of an Unlink service.
*/
typedef OV_UNLINK_RES	KS_UNLINK_RES;

/*
*	KS_ABSRELTIME:
*	--------------
*	Absolute or relative time.
*/
#define KS_ABSRELTIME_u	OV_ABSRELTIME_u
typedef OV_ABSRELTIME	KS_ABSRELTIME;

/*
*	KS_TIMEHISTSELECTOR:
*	--------------------
*	Time selector for histories.
*/
typedef OV_TIMEHISTSELECTOR	KS_TIMEHISTSELECTOR;

/*
*	KS_STRINGHISTSELECTOR:
*	----------------------
*	String selector for histories.
*/
typedef OV_STRINGHISTSELECTOR	KS_STRINGHISTSELECTOR;

/*
*	KS_HISTSELECTOR:
*	----------------
*	Selector for histories.
*/
#define KS_HISTSELECTOR_u	OV_HISTSELECTOR_u
typedef OV_HISTSELECTOR	KS_HISTSELECTOR;

/*
*	KS_GETHIST_ITEM:
*	----------------
*	Item of a GetHist service parameters.
*/
typedef OV_GETHIST_ITEM	KS_GETHIST_ITEM;

/*
*	KS_GETHISTRESULT_ITEM:
*	----------------------
*	Item of a single GetHist service result.
*/
typedef OV_GETHISTRESULT_ITEM	KS_GETHISTRESULT_ITEM;

/*
*	KS_GETHISTSINGLERESULT:
*	-----------------------
*	Single result of a GetHist service result.
*/
typedef OV_GETHISTSINGLERESULT	KS_GETHISTSINGLERESULT;

/*
*	KS_GETHIST_PAR:
*	---------------
*	Parameters of a GetHist service.
*/
typedef OV_GETHIST_PAR	KS_GETHIST_PAR;

/*
*	KS_GETHIST_RES:
*	---------------
*	Result of a GetHist service.
*/
typedef OV_GETHIST_RES	KS_GETHIST_RES;

/*
*	The following part is only visible inside the library
*	-----------------------------------------------------
*/
#ifdef OV_COMPILE_LIBOVKS

/*
*	XDR routine for bytes
*/
OV_BOOL ov_ksclient_xdr_bytes(
	XDR		*xdrs,
	char	**cpp,
	OV_UINT	*sizep,
	OV_UINT	maxsize
);

/*
*	XDR routine for arrays
*/
OV_BOOL ov_ksclient_xdr_array(
	XDR			*xdrs,
	char		**addrp,
	OV_UINT		*sizep,
	OV_UINT		maxsize,
	OV_UINT		elsize,
	xdrproc_t	elproc
);

/*
*	Macro: declare function prototype of an XDR routine
*/
#define OV_KSCLIENT_DECL_XDRFNC(type)							\
	OV_BOOL ov_ksclient_xdr_##type (							\
		XDR		*xdrs,											\
		type	*objp											\
	)

/*
*	Macro: XDR an array of given type
*/
#define OV_KSCLIENT_XDR_ARRAY(type, member)						\
	ov_ksclient_xdr_array(xdrs, (char **)&objp->member##_val,	\
		&objp->member##_len, ~0, sizeof(type),					\
		(xdrproc_t)ov_ksclient_xdr_##type)

/*
*	XDR routines for basic datatypes
*/
#define ov_ksclient_xdr_KS_ENUM		xdr_enum
#define ov_ksclient_xdr_KS_BOOL		xdr_bool
#define ov_ksclient_xdr_KS_INT		xdr_long
#define ov_ksclient_xdr_KS_UINT		xdr_u_long
#define ov_ksclient_xdr_KS_SINGLE	xdr_float
#define ov_ksclient_xdr_KS_DOUBLE	xdr_double
#define ov_ksclient_xdr_KS_RESULT	ov_ksclient_xdr_KS_ENUM

OV_KSCLIENT_DECL_XDRFNC(OV_TIME);
OV_KSCLIENT_DECL_XDRFNC(OV_TIME_SPAN);
OV_KSCLIENT_DECL_XDRFNC(OV_STRING);

#define ov_ksclient_xdr_KS_SEMANTIC_FLAGS	xdr_u_long

/*
*	XDR routines for enumeration values
*/
#define ov_ksclient_xdr_KS_TICKET_TYPE			ov_ksclient_xdr_KS_ENUM
#define ov_ksclient_xdr_KS_VAR_TYPE				ov_ksclient_xdr_KS_ENUM
#define ov_ksclient_xdr_KS_STATE				ov_ksclient_xdr_KS_ENUM
#define ov_ksclient_xdr_KS_OBJ_TYPE				ov_ksclient_xdr_KS_ENUM
#define ov_ksclient_xdr_KS_ACCESS				ov_ksclient_xdr_KS_ENUM
#define ov_ksclient_xdr_KS_LINK_TYPE			ov_ksclient_xdr_KS_ENUM
#define ov_ksclient_xdr_KS_PLACEMENT_HINT		ov_ksclient_xdr_KS_ENUM
#define ov_ksclient_xdr_KS_EP_FLAGS				ov_ksclient_xdr_KS_ENUM
#define ov_ksclient_xdr_KS_TIME_TYPE			ov_ksclient_xdr_KS_ENUM
#define ov_ksclient_xdr_KS_INTERPOLATION_MODE	ov_ksclient_xdr_KS_ENUM
#define ov_ksclient_xdr_KS_HSEL_TYPE			ov_ksclient_xdr_KS_ENUM

/*
*	XDR routines for A/V modules
*/
OV_KSCLIENT_DECL_XDRFNC(KS_AVMODULE_PAR);
OV_KSCLIENT_DECL_XDRFNC(KS_AVMODULE_RES);

/*
*	XDR routine for variable values
*/
OV_KSCLIENT_DECL_XDRFNC(KS_VAR_VALUE);

/*
*	XDR routines for properties
*/
OV_KSCLIENT_DECL_XDRFNC(KS_VAR_CURRENT_PROPS);

OV_KSCLIENT_DECL_XDRFNC(KS_VAR_PROJECTED_PROPS);
OV_KSCLIENT_DECL_XDRFNC(KS_LINK_PROJECTED_PROPS);
OV_KSCLIENT_DECL_XDRFNC(KS_OBJ_PROJECTED_PROPS);

OV_KSCLIENT_DECL_XDRFNC(KS_DOMAIN_ENGINEERED_PROPS);
OV_KSCLIENT_DECL_XDRFNC(KS_VAR_ENGINEERED_PROPS);
OV_KSCLIENT_DECL_XDRFNC(KS_LINK_ENGINEERED_PROPS);
OV_KSCLIENT_DECL_XDRFNC(KS_OBJ_ENGINEERED_PROPS);

/*
*	XDR routines for GetPP
*/
OV_KSCLIENT_DECL_XDRFNC(KS_GETPP_PAR);
OV_KSCLIENT_DECL_XDRFNC(KS_GETPP_RES);

/*
*	XDR routines for GetVar
*/
OV_KSCLIENT_DECL_XDRFNC(KS_GETVAR_ITEM);
OV_KSCLIENT_DECL_XDRFNC(KS_GETVAR_PAR);
OV_KSCLIENT_DECL_XDRFNC(KS_GETVAR_RES);

/*
*	XDR routines for SetVar
*/
OV_KSCLIENT_DECL_XDRFNC(KS_SETVAR_ITEM);
OV_KSCLIENT_DECL_XDRFNC(KS_SETVAR_PAR);
OV_KSCLIENT_DECL_XDRFNC(KS_SETVAR_RES);

/*
*	XDR routines for ExgData
*/
OV_KSCLIENT_DECL_XDRFNC(KS_EXGDATA_PAR);
OV_KSCLIENT_DECL_XDRFNC(KS_EXGDATA_RES);

/*
*	XDR routines for GetEP
*/
OV_KSCLIENT_DECL_XDRFNC(KS_GETEP_PAR);
OV_KSCLIENT_DECL_XDRFNC(KS_GETEP_RES);

/*
*	XDR routines for links
*/
OV_KSCLIENT_DECL_XDRFNC(KS_PLACEMENT);
OV_KSCLIENT_DECL_XDRFNC(KS_LINK_ITEM);
OV_KSCLIENT_DECL_XDRFNC(KS_UNLINK_ITEM);

/*
*	XDR routines for CreateObject
*/
OV_KSCLIENT_DECL_XDRFNC(KS_CREATEOBJ_ITEM);
OV_KSCLIENT_DECL_XDRFNC(KS_CREATEOBJECT_PAR);
OV_KSCLIENT_DECL_XDRFNC(KS_CREATEOBJECTITEM_RES);
OV_KSCLIENT_DECL_XDRFNC(KS_CREATEOBJECT_RES);

/*
*	XDR routines for DeleteObject
*/
OV_KSCLIENT_DECL_XDRFNC(KS_DELETEOBJECT_PAR);
OV_KSCLIENT_DECL_XDRFNC(KS_DELETEOBJECT_RES);

/*
*	XDR routines for RenameObject
*/
OV_KSCLIENT_DECL_XDRFNC(KS_RENAMEOBJECT_ITEM);
OV_KSCLIENT_DECL_XDRFNC(KS_RENAMEOBJECT_PAR);
OV_KSCLIENT_DECL_XDRFNC(KS_RENAMEOBJECT_RES);

/*
*	XDR routines for GetCanonicalPath
*/
OV_KSCLIENT_DECL_XDRFNC(KS_GETCANONICALPATH_PAR);
OV_KSCLIENT_DECL_XDRFNC(KS_GETCANONICALPATHITEM_RES);
OV_KSCLIENT_DECL_XDRFNC(KS_GETCANONICALPATH_RES);

/*
*	XDR routines for Link
*/
OV_KSCLIENT_DECL_XDRFNC(KS_LINK_PAR);
OV_KSCLIENT_DECL_XDRFNC(KS_LINK_RES);

/*
*	XDR routines for Unlink
*/
OV_KSCLIENT_DECL_XDRFNC(KS_UNLINK_PAR);
OV_KSCLIENT_DECL_XDRFNC(KS_UNLINK_RES);

/*
*	XDR routines for GetHist
*/
OV_KSCLIENT_DECL_XDRFNC(KS_ABSRELTIME);
OV_KSCLIENT_DECL_XDRFNC(KS_TIMEHISTSELECTOR);
OV_KSCLIENT_DECL_XDRFNC(KS_STRINGHISTSELECTOR);
OV_KSCLIENT_DECL_XDRFNC(KS_HISTSELECTOR);
OV_KSCLIENT_DECL_XDRFNC(KS_GETHIST_ITEM);
OV_KSCLIENT_DECL_XDRFNC(KS_GETHISTRESULT_ITEM);
OV_KSCLIENT_DECL_XDRFNC(KS_GETHISTSINGLERESULT);
OV_KSCLIENT_DECL_XDRFNC(KS_GETHIST_PAR);
OV_KSCLIENT_DECL_XDRFNC(KS_GETHIST_RES);

#endif	/* OV_COMPILE_LIBOVKS */

#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif
/*
*	End of file
*/

