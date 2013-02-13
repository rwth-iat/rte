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

#include "libov/ov_ov.h"
#include "libov/ov_element.h"
#include "libov/ov_memstack.h"
#include "libov/ov_logfile.h"


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

typedef OV_ANY				OV_VAR_CURRENT_PROPS;

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
*	OV_NAMED_ELEMENT:
*	-----------------
*/
typedef struct {
	OV_STRING			identifier;
	OV_VAR_VALUE			value;
}	OV_NAMED_ELEMENT;


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
*	OV_STRUCT_ENGINEERED_PROPS:
*	-----------------------------
*	Engineered properites of a structure.
*/
typedef struct {
	OV_STRING	type_identifier;
}	OV_STRUCT_ENGINEERED_PROPS;

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
*	OV_HIST_ENGINEERED_PROPS:
*	-----------------------
*	Engineered properites of a history object.
*/
typedef struct {
	OV_HIST_TYPE		historytype;
	OV_INTERPOLATION_MODE 	default_interpolation;
	OV_INTERPOLATION_MODE 	supported_interpolation;
	OV_STRING		type_identifier;
}	OV_HISTORY_ENGINEERED_PROPS;


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
		OV_STRUCT_ENGINEERED_PROPS	struct_engineered_props;
		OV_VAR_ENGINEERED_PROPS		var_engineered_props;
		OV_LINK_ENGINEERED_PROPS	link_engineered_props;
		OV_HISTORY_ENGINEERED_PROPS	history_engineered_props;
	}	OV_OBJ_ENGINEERED_PROPS_u;
	OV_STRING			identifier;
	OV_TIME				creation_time;
	OV_STRING			comment;
	OV_ACCESS			access;
	OV_SEMANTIC_FLAGS		semantic_flags;
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

OV_DLLVARIMPORT OV_TICKET_VTBL noneticketvtbl;

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

