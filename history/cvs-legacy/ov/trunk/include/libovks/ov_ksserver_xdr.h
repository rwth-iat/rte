/*
*   $Id: ov_ksserver_xdr.h,v 1.3 1999-08-30 15:24:22 dirk Exp $
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
*	20-Apr-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*/
/*
*	Description:
*	------------
*	These XDR routines are mostly based on the output of the RPC code
*	generator rpcgen. They are designed for the use with the ACPLT/KS server
*	of ACPLT/OV only and optimized for speed, i.e. they are use the memory
*	allocator ov_memstack_alloc() only.
*	This means, that ov_memstack_lock() and ov_memstack_unlock() have to
*	be called outside the XDR routines and that the XDR mode XDR_FREE is not
*	available.
*	If you introduce changes here, you should probably take a look at
*	ov_ksclient_xdr.h too.
*/

#ifndef OV_KSSERVER_XDR_H_INCLUDED
#define OV_KSSERVER_XDR_H_INCLUDED

#include "libov/ov_ov.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
*	shared datatypes ACPLT/KS -- ACPLT/OV
*/
typedef KS_LINK_TYPE		OV_LINK_TYPE;
typedef KS_OBJ_TYPE			OV_OBJ_TYPE;
typedef KS_SVC				OV_SVC;
typedef KS_SEMANTIC_FLAGS	OV_SEMANTIC_FLAGS;
typedef KS_EP_FLAGS			OV_EP_FLAGS;

typedef OV_TICKET			OV_TICKET_PAR;
typedef OV_TICKET			OV_TICKET_RES;

/*
*	OV_VAR_PROJECTED_PROPS:
*	-----------------------
*	Projected properites of a variable.
*/
typedef struct {
	OV_STRING	tech_unit;
	OV_VAR_TYPE vartype;
}	OV_VAR_PROJECTED_PROPS;

/*
*	OV_LINK_PROJECTED_PROPS:
*	------------------------
*	Projected properites of a link.
*/
typedef struct {
	OV_LINK_TYPE	linktype;
	OV_STRING		opposite_role_identifier;
	OV_STRING		association_identifier;
}	OV_LINK_PROJECTED_PROPS;

/*
*	OV_OBJ_PROJECTED_PROPS:
*	-----------------------
*	Projected properties of an communication object.
*	Note, that in GetPP they are be linked to a list, NOT an array.
*/
struct OV_OBJ_PROJECTED_PROPS {
	struct OV_OBJ_PROJECTED_PROPS	*pnext;
	OV_OBJ_TYPE	objtype;
	union {
		OV_VAR_PROJECTED_PROPS	var_projected_props;
		OV_LINK_PROJECTED_PROPS	link_projected_props;
	}	OV_OBJ_PROJECTED_PROPS_u;
	OV_STRING	identifier;
	OV_TIME		creation_time;
	OV_STRING	comment;
	OV_ACCESS	access;
};
typedef struct OV_OBJ_PROJECTED_PROPS	OV_OBJ_PROJECTED_PROPS;

/*
*	OV_GETPP_PAR:
*	-------------
*	Parameters of a GetPP service.
*/
typedef struct {
	OV_STRING	path;
	OV_OBJ_TYPE type_mask;
	OV_STRING	name_mask;
}	OV_GETPP_PAR;

/*
*	OV_GETPP_RES:
*	-------------
*	Result of a GetPP service.
*/
typedef struct {
	OV_RESULT				result;
	OV_UINT					items_len;	/* only, if result == OV_ERR_OK */
	OV_OBJ_PROJECTED_PROPS	*pfirst;	/* only, if result == OV_ERR_OK */
	OV_OBJ_PROJECTED_PROPS	*plast;		/* only, if result == OV_ERR_OK */
}	OV_GETPP_RES;

/*
*	OV_GETVAR_ITEM:
*	---------------
*	Item of the GetVar/DataExchange service result.
*/
typedef struct {
	OV_RESULT				result;
	OV_VAR_CURRENT_PROPS	var_current_props;	/* only, if result == OV_ERR_OK */
}	OV_GETVAR_ITEM;

/*
*	OV_GETVAR_PAR:
*	--------------
*	Parameters of a GetVar service.
*/
typedef struct {
	OV_UINT		identifiers_len;
	OV_STRING	*identifiers_val;
}	OV_GETVAR_PAR;

/*
*	OV_GETVAR_RES:
*	--------------
*	Result of a GetVar service.
*/
typedef struct {
	OV_RESULT		result;
	OV_UINT			items_len;			/* only, if result == OV_ERR_OK */
	OV_GETVAR_ITEM	*items_val;			/* only, if result == OV_ERR_OK */
}	OV_GETVAR_RES;

/*
*	OV_SETVAR_ITEM:
*	---------------
*	Item of the SetVar/DataExchange service parameters.
*/
typedef struct {
	OV_STRING				path_and_name;
	OV_VAR_CURRENT_PROPS	var_current_props;
}	OV_SETVAR_ITEM;

/*
*	OV_SETVAR_PAR:
*	--------------
*	Parameters of a SetVar service.
*/
typedef struct {
	OV_UINT			items_len;
	OV_SETVAR_ITEM	*items_val;
}	OV_SETVAR_PAR;

/*
*	OV_SETVAR_RES:
*	--------------
*	Result of a SetVar service.
*/
typedef struct {
	OV_RESULT	result;
	OV_UINT		results_len;					/* only, if result == OV_ERR_OK */
	OV_RESULT	*results_val;					/* only, if result == OV_ERR_OK */
}	OV_SETVAR_RES;

/*
*	OV_EXGDATA_PAR:
*	---------------
*	Parameters of a DataExchange service.
*/
typedef struct {
	OV_UINT			set_vars_len;
	OV_SETVAR_ITEM	*set_vars_val;
	OV_UINT			get_vars_len;
	OV_STRING		*get_vars_val;
}	OV_EXGDATA_PAR;

/*
*	OV_EXGDATA_RES:
*	---------------
*	Result of DataExchange service.
*/
typedef struct {
	OV_RESULT 		result;
	OV_UINT			results_len;		/* only, if result == OV_ERR_OK */
	OV_RESULT		*results_val		/* only, if result == OV_ERR_OK */;
	OV_UINT			items_len;			/* only, if result == OV_ERR_OK */
	OV_GETVAR_ITEM	*items_val;			/* only, if result == OV_ERR_OK */
}	OV_EXGDATA_RES;

/*
*	OV_DOMAIN_ENGINEERED_PROPS:
*	--------------------------
*	Engineered properites of a domain.
*/
typedef struct {
	OV_STRING	class_identifier;
}	OV_DOMAIN_ENGINEERED_PROPS;

/*
*	OV_VAR_ENGINEERED_PROPS:
*	-----------------------
*	Engineered properites of a variable.
*/
typedef struct {
	OV_STRING	tech_unit;
	OV_VAR_TYPE vartype;
}	OV_VAR_ENGINEERED_PROPS;

/*
*	OV_LINK_ENGINEERED_PROPS:
*	------------------------
*	Engineered properites of a link.
*/
typedef struct {
	OV_LINK_TYPE	linktype;
	OV_STRING		opposite_role_identifier;
	OV_STRING		association_identifier;
}	OV_LINK_ENGINEERED_PROPS;

/*
*	OV_OBJ_ENGINEERED_PROPS:
*	-----------------------
*	Engineered properties of an communication object.
*	Note, that in GetEP they are be linked to a list, NOT an array.
*/
struct OV_OBJ_ENGINEERED_PROPS {
	struct OV_OBJ_ENGINEERED_PROPS	*pnext;
	OV_OBJ_TYPE	objtype;
	union {
		OV_DOMAIN_ENGINEERED_PROPS	domain_engineered_props;
		OV_VAR_ENGINEERED_PROPS		var_engineered_props;
		OV_LINK_ENGINEERED_PROPS	link_engineered_props;
	}	OV_OBJ_ENGINEERED_PROPS_u;
	OV_STRING			identifier;
	OV_TIME				creation_time;
	OV_STRING			comment;
	OV_ACCESS			access;
	OV_SEMANTIC_FLAGS	semantic_flags;
};
typedef struct OV_OBJ_ENGINEERED_PROPS	OV_OBJ_ENGINEERED_PROPS;

/*
*	OV_GETEP_PAR:
*	-------------
*	Parameters of a GetEP service.
*/
typedef struct {
	OV_STRING	path;
	OV_OBJ_TYPE type_mask;
	OV_STRING	name_mask;
	OV_EP_FLAGS	scope_flags;
}	OV_GETEP_PAR;

/*
*	OV_GETEP_RES:
*	-------------
*	Result of a GetEP service.
*/
typedef struct {
	OV_RESULT				result;
	OV_UINT					items_len;	/* only, if result == OV_ERR_OK */
	OV_OBJ_ENGINEERED_PROPS	*pfirst;	/* only, if result == OV_ERR_OK */
	OV_OBJ_ENGINEERED_PROPS	*plast;		/* only, if result == OV_ERR_OK */
}	OV_GETEP_RES;

/*
*	OV_PLACEMENT:
*	-------------
*	Placement of a Link/CreateObject service.
*/
typedef struct {
	OV_PLACEMENT_HINT	hint;
	OV_STRING			place_path;				/* only, if hint == OV_PMH_BEFORE/AFTER */
}	OV_PLACEMENT;

/*
*	OV_LINK_ITEM:
*	-------------
*	Item of the Link/CreateObject service parameters.
*/
typedef struct {
	OV_STRING		link_path;
	OV_STRING		element_path;
	OV_PLACEMENT	place;
	OV_PLACEMENT	opposite_place;
}	OV_LINK_ITEM;

/*
*	OV_UNLINK_ITEM:
*	---------------
*	Item of the Link service parameters.
*/
typedef struct {
	OV_STRING	link_path;
	OV_STRING	element_path;
}	OV_UNLINK_ITEM;

/*
*	OV_CREATEOBJ_ITEM:
*	------------------
*	Item of the CreateObject service parameters.
*/
typedef struct {
	OV_STRING		factory_path;
	OV_STRING		new_path;
	OV_PLACEMENT	place;
	OV_UINT			parameters_len;
	OV_SETVAR_ITEM	*parameters_val;
	OV_UINT			links_len;
	OV_LINK_ITEM	*links_val;
}	OV_CREATEOBJ_ITEM;

/*
*	OV_CREATEOBJECT_PAR:
*	--------------------
*	Parameters of the CreateObject service.
*/
typedef struct {
	OV_UINT				items_len;
	OV_CREATEOBJ_ITEM	*items_val;
}	OV_CREATEOBJECT_PAR;

/*
*	OV_CREATEOBJECTITEM_RES:
*	------------------------
*	Result item of a CreateObject service result.
*/
typedef struct {
	OV_RESULT	result;
	OV_UINT		params_results_len;				/* only, if result == OV_ERR_BADINITPARAM */
	OV_RESULT	*params_results_val;			/* only, if result == OV_ERR_BADINITPARAM */
	OV_UINT		link_results_len;				/* only, if result == OV_ERR_BADINITPARAM */
	OV_RESULT	*link_results_val;				/* only, if result == OV_ERR_BADINITPARAM */
}	OV_CREATEOBJECTITEM_RES;

/*
*	OV_CREATEOBJECT_RES:
*	--------------------
*	Result of a CreateObject service.
*/
typedef struct {
	OV_RESULT				result;
	OV_UINT					obj_results_len;	/* only, if result == OV_ERR_OK */
	OV_CREATEOBJECTITEM_RES	*obj_results_val;	/* only, if result == OV_ERR_OK */
}	OV_CREATEOBJECT_RES;

/*
*	OV_DELETEOBJECT_PAR:
*	--------------------
*	Parameters of a DeleteObject service.
*/
typedef struct {
	OV_UINT		paths_len;
	OV_STRING	*paths_val;
}	OV_DELETEOBJECT_PAR;

/*
*	OV_DELETEOBJECT_RES:
*	--------------------
*	Result of a DeleteObject service.
*/
typedef struct {
	OV_RESULT	result;
	OV_UINT		results_len;				/* only, if result == OV_ERR_OK */
	OV_RESULT	*results_val;				/* only, if result == OV_ERR_OK */
}	OV_DELETEOBJECT_RES;

/*
*	OV_RENAMEOBJECT_ITEM:
*	---------------------
*	Item of the RenameObject service parameters.
*/
typedef struct {
	OV_STRING		old_path;
	OV_STRING		new_path;
	OV_PLACEMENT	place;
}	OV_RENAMEOBJECT_ITEM;

/*
*	OV_RENAMEOBJECT_PAR:
*	--------------------
*	Parameters of a RenameObject Service
*/
typedef struct {
	OV_UINT					items_len;
	OV_RENAMEOBJECT_ITEM	*items_val;
}	OV_RENAMEOBJECT_PAR;

/*
*	OV_RENAMEOBJECT_RES:
*	--------------------
*	Result of a RenameObject Service
*/
typedef struct {
	OV_RESULT	result;
	OV_UINT		results_len;				/* only, if result == OV_ERR_OK */
	OV_RESULT	*results_val;				/* only, if result == OV_ERR_OK */
}	OV_RENAMEOBJECT_RES;

/*
*	OV_GETCANONICALPATH_PAR:
*	------------------------
*	Parameters of a GetCanonicalPath Service
*/
typedef struct {
	OV_UINT		paths_len;
	OV_STRING	*paths_val;
}	OV_GETCANONICALPATH_PAR;

/*
*	OV_GETCANONICALPATHITEM_RES:
*	----------------------------
*	Result item of a GetCanonicalPath Service
*/
typedef struct {
	OV_RESULT	result;
	OV_STRING	canonical_path;				/* only, if result == OV_ERR_OK */
}	OV_GETCANONICALPATHITEM_RES;


/*
*	OV_GETCANONICALPATH_RES:
*	------------------------
*	Result of a GetCanonicalPath Service
*/
typedef struct {
	OV_RESULT					result;
	OV_UINT						results_len;	/* only, if result == OV_ERR_OK */
	OV_GETCANONICALPATHITEM_RES	*results_val;	/* only, if result == OV_ERR_OK */
}	OV_GETCANONICALPATH_RES;

/*
*	OV_LINK_PAR:
*	------------
*	Parameters of a Link service.
*/
typedef struct {
	OV_UINT			items_len;
	OV_LINK_ITEM	*items_val;
}	OV_LINK_PAR;

/*
*	OV_LINK_RES:
*	------------
*	Result of a Link service.
*/
typedef struct {
	OV_RESULT	result;
	OV_UINT		results_len;					/* only, if result == OV_ERR_OK */
	OV_RESULT	*results_val;					/* only, if result == OV_ERR_OK */
}	OV_LINK_RES;

/*
*	OV_UNLINK_PAR:
*	--------------
*	Parameters of an Unlink service.
*/
typedef struct {
	OV_UINT			items_len;
	OV_UNLINK_ITEM	*items_val;
}	OV_UNLINK_PAR;

/*
*	OV_UNLINK_RES:
*	--------------
*	Result of an Unlink service.
*/
typedef struct {
	OV_RESULT	result;
	OV_UINT		results_len;					/* only, if result == OV_ERR_OK */
	OV_RESULT	*results_val;					/* only, if result == OV_ERR_OK */
}	OV_UNLINK_RES;

/*
*	OV_ABSRELTIME:
*	--------------
*	Absolute or relative time.
*/
typedef struct {
	OV_TIME_TYPE	timetype;
	union {
		OV_TIME			abstime;
		OV_TIME_SPAN	reltime;
	}	OV_ABSRELTIME_u;
}	OV_ABSRELTIME;

/*
*	OV_TIMEHISTSELECTOR:
*	--------------------
*	Time selector for histories.
*/
typedef struct {
	OV_INTERPOLATION_MODE	ip_mode;
	OV_ABSRELTIME			from;
	OV_ABSRELTIME			to;
	OV_TIME_SPAN			delta;
}	OV_TIMEHISTSELECTOR;

/*
*	OV_STRINGHISTSELECTOR:
*	----------------------
*	String selector for histories.
*/
typedef struct {
	OV_STRING	mask;
}	OV_STRINGHISTSELECTOR;

/*
*	OV_HISTSELECTOR:
*	----------------
*	Selector for histories.
*/
typedef struct {
	OV_HSEL_TYPE	hseltype;
	union {
		OV_TIMEHISTSELECTOR		ths;
		OV_STRINGHISTSELECTOR	shs;
	}	OV_HISTSELECTOR_u;
}	OV_HISTSELECTOR;

/*
*	OV_GETHIST_ITEM:
*	----------------
*	Item of a GetHist service parameters.
*/
typedef struct {
	OV_STRING		part;
	OV_HISTSELECTOR	selector;
}	OV_GETHIST_ITEM;

/*
*	OV_GETHISTRESULT_ITEM:
*	----------------------
*	Item of a single GetHist service result.
*/
typedef struct {
	OV_RESULT		result;
	OV_VAR_VALUE	value;				/* only, if result == OV_ERR_OK */
}	OV_GETHISTRESULT_ITEM;

/*
*	OV_GETHISTSINGLERESULT:
*	-----------------------
*	Single result of a GetHist service result.
*/
typedef struct {
	OV_RESULT				result;
	OV_UINT					items_len;	/* only, if result == OV_ERR_OK */
	OV_GETHISTRESULT_ITEM	*items_val;	/* only, if result == OV_ERR_OK */
}	OV_GETHISTSINGLERESULT;

/*
*	OV_GETHIST_PAR:
*	---------------
*	Parameters of a GetHist service.
*/
typedef struct {
	OV_UINT			paths_len;
	OV_STRING		*paths_val;
	OV_UINT			max_answers;
	OV_UINT			items_len;
	OV_GETHIST_ITEM	*items_val;
}	OV_GETHIST_PAR;

/*
*	OV_GETHIST_RES:
*	---------------
*	Result of a GetHist service.
*/
typedef struct {
	OV_RESULT				result;
	OV_UINT					results_len;	/* only, if result == OV_ERR_OK */
	OV_GETHISTSINGLERESULT	*results_val;	/* only, if result == OV_ERR_OK */
}	OV_GETHIST_RES;

/*
*	The following part is only visible inside the library
*	-----------------------------------------------------
*/
#ifdef OV_COMPILE_LIBOVKS

/*
*	XDR routine for bytes
*/
OV_BOOL ov_ksserver_xdr_bytes(
	XDR		*xdrs,
	char	**cpp,
	OV_UINT	*sizep,
	OV_UINT	maxsize
);

/*
*	XDR routine for strings
*/
OV_BOOL ov_ksserver_xdr_string(
	XDR			*xdrs,
	char		**cpp,
	OV_UINT		maxsize
);

/*
*	XDR routine for arrays
*/
OV_BOOL ov_ksserver_xdr_array(
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
#define OV_KSSERVER_DECL_XDRFNC(type)							\
	OV_BOOL ov_ksserver_xdr_##type (							\
		XDR		*xdrs,											\
		type	*objp											\
	)

/*
*	Macro: XDR an array of given type
*/
#define OV_KSSERVER_XDR_ARRAY(type, member)						\
	ov_ksserver_xdr_array(xdrs, (char **)&objp->member##_val,	\
		&objp->member##_len, ~0, sizeof(type),					\
		(xdrproc_t)ov_ksserver_xdr_##type)

/*
*	XDR routines for basic datatypes
*/
#define ov_ksserver_xdr_OV_ENUM		xdr_enum
#define ov_ksserver_xdr_OV_BOOL		xdr_bool
#define ov_ksserver_xdr_OV_INT		xdr_long
#define ov_ksserver_xdr_OV_UINT		xdr_u_long
#define ov_ksserver_xdr_OV_SINGLE	xdr_float
#define ov_ksserver_xdr_OV_DOUBLE	xdr_double
#define ov_ksserver_xdr_OV_RESULT	ov_ksserver_xdr_OV_ENUM

OV_KSSERVER_DECL_XDRFNC(OV_TIME);
OV_KSSERVER_DECL_XDRFNC(OV_TIME_SPAN);
OV_KSSERVER_DECL_XDRFNC(OV_STRING);

#define ov_ksserver_xdr_OV_SEMANTIC_FLAGS	xdr_u_long

/*
*	XDR routines for enumeration values
*/
#define ov_ksserver_xdr_OV_TICKET_TYPE			ov_ksserver_xdr_OV_ENUM
#define ov_ksserver_xdr_OV_VAR_TYPE				ov_ksserver_xdr_OV_ENUM
#define ov_ksserver_xdr_OV_STATE				ov_ksserver_xdr_OV_ENUM
#define ov_ksserver_xdr_OV_OBJ_TYPE				ov_ksserver_xdr_OV_ENUM
#define ov_ksserver_xdr_OV_ACCESS				ov_ksserver_xdr_OV_ENUM
#define ov_ksserver_xdr_OV_LINK_TYPE			ov_ksserver_xdr_OV_ENUM
#define ov_ksserver_xdr_OV_PLACEMENT_HINT		ov_ksserver_xdr_OV_ENUM
#define ov_ksserver_xdr_OV_EP_FLAGS				ov_ksserver_xdr_OV_ENUM
#define ov_ksserver_xdr_OV_TIME_TYPE			ov_ksserver_xdr_OV_ENUM
#define ov_ksserver_xdr_OV_INTERPOLATION_MODE	ov_ksserver_xdr_OV_ENUM
#define ov_ksserver_xdr_OV_HSEL_TYPE			ov_ksserver_xdr_OV_ENUM

/*
*	XDR routines for tickets
*/
OV_KSSERVER_DECL_XDRFNC(OV_TICKET_PAR);
OV_KSSERVER_DECL_XDRFNC(OV_TICKET_RES);

/*
*	XDR routine for variable values
*/
OV_KSSERVER_DECL_XDRFNC(OV_VAR_VALUE);

/*
*	XDR routines for properties
*/
OV_KSSERVER_DECL_XDRFNC(OV_VAR_CURRENT_PROPS);

OV_KSSERVER_DECL_XDRFNC(OV_VAR_PROJECTED_PROPS);
OV_KSSERVER_DECL_XDRFNC(OV_LINK_PROJECTED_PROPS);
OV_KSSERVER_DECL_XDRFNC(OV_OBJ_PROJECTED_PROPS);

OV_KSSERVER_DECL_XDRFNC(OV_DOMAIN_ENGINEERED_PROPS);
OV_KSSERVER_DECL_XDRFNC(OV_VAR_ENGINEERED_PROPS);
OV_KSSERVER_DECL_XDRFNC(OV_LINK_ENGINEERED_PROPS);
OV_KSSERVER_DECL_XDRFNC(OV_OBJ_ENGINEERED_PROPS);

/*
*	XDR routines for GetPP
*/
OV_KSSERVER_DECL_XDRFNC(OV_GETPP_PAR);
OV_KSSERVER_DECL_XDRFNC(OV_GETPP_RES);

/*
*	XDR routines for GetVar
*/
OV_KSSERVER_DECL_XDRFNC(OV_GETVAR_ITEM);
OV_KSSERVER_DECL_XDRFNC(OV_GETVAR_PAR);
OV_KSSERVER_DECL_XDRFNC(OV_GETVAR_RES);

/*
*	XDR routines for SetVar
*/
OV_KSSERVER_DECL_XDRFNC(OV_SETVAR_ITEM);
OV_KSSERVER_DECL_XDRFNC(OV_SETVAR_PAR);
OV_KSSERVER_DECL_XDRFNC(OV_SETVAR_RES);

/*
*	XDR routines for ExgData
*/
OV_KSSERVER_DECL_XDRFNC(OV_EXGDATA_PAR);
OV_KSSERVER_DECL_XDRFNC(OV_EXGDATA_RES);

/*
*	XDR routines for GetEP
*/
OV_KSSERVER_DECL_XDRFNC(OV_GETEP_PAR);
OV_KSSERVER_DECL_XDRFNC(OV_GETEP_RES);

/*
*	XDR routines for links
*/
OV_KSSERVER_DECL_XDRFNC(OV_PLACEMENT);
OV_KSSERVER_DECL_XDRFNC(OV_LINK_ITEM);
OV_KSSERVER_DECL_XDRFNC(OV_UNLINK_ITEM);

/*
*	XDR routines for CreateObject
*/
OV_KSSERVER_DECL_XDRFNC(OV_CREATEOBJ_ITEM);
OV_KSSERVER_DECL_XDRFNC(OV_CREATEOBJECT_PAR);
OV_KSSERVER_DECL_XDRFNC(OV_CREATEOBJECTITEM_RES);
OV_KSSERVER_DECL_XDRFNC(OV_CREATEOBJECT_RES);

/*
*	XDR routines for DeleteObject
*/
OV_KSSERVER_DECL_XDRFNC(OV_DELETEOBJECT_PAR);
OV_KSSERVER_DECL_XDRFNC(OV_DELETEOBJECT_RES);

/*
*	XDR routines for RenameObject
*/
OV_KSSERVER_DECL_XDRFNC(OV_RENAMEOBJECT_ITEM);
OV_KSSERVER_DECL_XDRFNC(OV_RENAMEOBJECT_PAR);
OV_KSSERVER_DECL_XDRFNC(OV_RENAMEOBJECT_RES);

/*
*	XDR routines for GetCanonicalPath
*/
OV_KSSERVER_DECL_XDRFNC(OV_GETCANONICALPATH_PAR);
OV_KSSERVER_DECL_XDRFNC(OV_GETCANONICALPATHITEM_RES);
OV_KSSERVER_DECL_XDRFNC(OV_GETCANONICALPATH_RES);

/*
*	XDR routines for Link
*/
OV_KSSERVER_DECL_XDRFNC(OV_LINK_PAR);
OV_KSSERVER_DECL_XDRFNC(OV_LINK_RES);

/*
*	XDR routines for Unlink
*/
OV_KSSERVER_DECL_XDRFNC(OV_UNLINK_PAR);
OV_KSSERVER_DECL_XDRFNC(OV_UNLINK_RES);

/*
*	XDR routines for GetHist
*/
OV_KSSERVER_DECL_XDRFNC(OV_ABSRELTIME);
OV_KSSERVER_DECL_XDRFNC(OV_TIMEHISTSELECTOR);
OV_KSSERVER_DECL_XDRFNC(OV_STRINGHISTSELECTOR);
OV_KSSERVER_DECL_XDRFNC(OV_HISTSELECTOR);
OV_KSSERVER_DECL_XDRFNC(OV_GETHIST_ITEM);
OV_KSSERVER_DECL_XDRFNC(OV_GETHISTRESULT_ITEM);
OV_KSSERVER_DECL_XDRFNC(OV_GETHISTSINGLERESULT);
OV_KSSERVER_DECL_XDRFNC(OV_GETHIST_PAR);
OV_KSSERVER_DECL_XDRFNC(OV_GETHIST_RES);

#endif	/* OV_COMPILE_LIBOVKS */

#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif
/*
*	End of file
*/

