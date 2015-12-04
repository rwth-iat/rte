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


/* ----------------------------------------------------------------------------
 * Object types (flags). When browsing through the object tree, these flags
 * can be combined (or-ed) to find any object belonging to one of the types
 * listed in the flag set.
 */
typedef OV_OBJ_TYPE      KS_OBJ_TYPE;
#define KS_OT_DOMAIN     OV_OT_DOMAIN    /* object is a domain    */
#define KS_OT_VARIABLE   OV_OT_VARIABLE  /* object is a variable  */
#define KS_OT_HISTORY    OV_OT_HISTORY   /* object is a history   */
#define KS_OT_LINK       OV_OT_LINK      /* object is a link      */
#define KS_OT_STRUCTURE  OV_OT_STRUCTURE /* object is a structure */
#define KS_OT_ANY        OV_OT_ANY

/* ----------------------------------------------------------------------------
 * Link types can be either 1:1, 1:m or m:n and in addition with local scope or
 * global scope ("global" means currently "within the same ACPLT/KS server").
 */
typedef OV_LINK_TYPE            KS_LINK_TYPE;
#define KS_LT_LOCAL_1_1         OV_LT_LOCAL_1_1
#define KS_LT_LOCAL_1_MANY      OV_LT_LOCAL_1_MANY
#define KS_LT_LOCAL_MANY_MANY   OV_LT_LOCAL_MANY_MANY
#define KS_LT_LOCAL_MANY_1      OV_LT_LOCAL_MANY_1
#define KS_LT_GLOBAL_1_1        OV_LT_GLOBAL_1_1
#define KS_LT_GLOBAL_1_MANY     OV_LT_GLOBAL_1_MANY
#define KS_LT_GLOBAL_MANY_MANY  OV_LT_GLOBAL_MANY_MANY
#define KS_LT_GLOBAL_MANY_1     OV_LT_GLOBAL_MANY_1

/*
*	KS_VAR_TYPE:
*	------------
*	Variable types (indicate the datatypes of variables).
*/
typedef OV_VAR_TYPE KS_VAR_TYPE;
#define KS_VT_VOID             OV_VT_VOID

#define KS_VT_BOOL             OV_VT_BOOL
#define KS_VT_INT              OV_VT_INT
#define KS_VT_UINT             OV_VT_UINT
#define KS_VT_SINGLE           OV_VT_SINGLE
#define KS_VT_DOUBLE           OV_VT_DOUBLE
#define KS_VT_STRING           OV_VT_STRING
#define KS_VT_TIME             OV_VT_TIME
#define KS_VT_TIME_SPAN        OV_VT_TIME_SPAN
#define KS_VT_STATE            OV_VT_STATE
#define KS_VT_STRUCT           OV_VT_STRUCT

#define KS_VT_BYTE_VEC         OV_VT_BYTE_VEC
#define KS_VT_BOOL_VEC         OV_VT_BOOL_VEC
#define KS_VT_INT_VEC          OV_VT_INT_VEC
#define KS_VT_UINT_VEC         OV_VT_UINT_VEC
#define KS_VT_SINGLE_VEC       OV_VT_SINGLE_VEC
#define KS_VT_DOUBLE_VEC       OV_VT_DOUBLE_VEC
#define KS_VT_STRING_VEC       OV_VT_STRING_VEC
#define KS_VT_TIME_VEC         OV_VT_TIME_VEC
#define KS_VT_TIME_SPAN_VEC    OV_VT_TIME_SPAN_VEC
#define KS_VT_TIME_SERIES      OV_VT_TIME_SERIES
#define KS_VT_STATE_VEC        OV_VT_STATE_VEC

#define KS_VT_BOOL_PV          OV_VT_BOOL_PV
#define KS_VT_INT_PV           OV_VT_INT_PV
#define KS_VT_UINT_PV          OV_VT_UINT_PV
#define KS_VT_SINGLE_PV        OV_VT_SINGLE_PV
#define KS_VT_DOUBLE_PV        OV_VT_DOUBLE_PV
#define KS_VT_STRING_PV        OV_VT_STRING_PV
#define KS_VT_TIME_PV          OV_VT_TIME_PV
#define KS_VT_TIME_SPAN_PV     OV_VT_TIME_SPAN_PV

#define KS_VT_BOOL_PV_VEC      OV_VT_BOOL_PV_VEC
#define KS_VT_INT_PV_VEC       OV_VT_INT_PV_VEC
#define KS_VT_UINT_PV_VEC      OV_VT_UINT_PV_VEC
#define KS_VT_SINGLE_PV_VEC    OV_VT_SINGLE_PV_VEC
#define KS_VT_DOUBLE_PV_VEC    OV_VT_DOUBLE_PV_VEC
#define KS_VT_STRING_PV_VEC    OV_VT_STRING_PV_VEC
#define KS_VT_TIME_PV_VEC      OV_VT_TIME_PV_VEC
#define KS_VT_TIME_SPAN_PV_VEC OV_VT_TIME_SPAN_PV_VEC

#define KS_VT_ANY              OV_VT_ANY


typedef OV_SEMANTIC_FLAGS	KS_SEMANTIC_FLAGS;

/* ----------------------------------------------------------------------------
 * Placement hint used with object management systems which support ordered
 * sets of children/parts.
 */
typedef OV_PLACEMENT_HINT KS_PLACEMENT_HINT;
#define KS_PMH_DEFAULT  OV_PMH_DEFAULT
#define KS_PMH_BEGIN    OV_PMH_BEGIN
#define KS_PMH_END      OV_PMH_END
#define KS_PMH_BEFORE   OV_PMH_BEFORE
#define KS_PMH_AFTER    OV_PMH_AFTER

/*
*	KS_HIST_TYPE:
*	-------------
*	History types for use with ACPLT/KS histories
*/
typedef OV_HIST_TYPE         KS_HIST_TYPE;
#define KS_HT_LOG            OV_HT_LOG
//#define KS_HT_TABLE        OV_HT_TABLE	/* already in ov_ov.h */
#define KS_HT_BOOL           OV_HT_BOOL
#define KS_HT_INT            OV_HT_INT
#define KS_HT_UINT           OV_HT_UINT
#define KS_HT_SINGLE         OV_HT_SINGLE
#define KS_HT_DOUBLE         OV_HT_DOUBLE
#define KS_HT_STRING         OV_HT_STRING
#define KS_HT_TIME           OV_HT_TIME
#define KS_HT_TIME_SPAN      OV_HT_TIME_SPAN

#define KS_HT_TYPE_MASK      OV_HT_TYPE_MASK

#define KS_HT_TIME_DRIVEN    OV_HT_TIME_DRIVEN
#define KS_HT_CHANGE_DRIVEN  OV_HT_CHANGE_DRIVEN

/* ----------------------------------------------------------------------------
 * Authentification & verification (aka A/V) stuff.
 */
typedef OV_AUTH_TYPE    KS_AUTH_TYPE;
#define KS_AUTH_NONE    OV_TT_NONE /* NONE-A/V-Module   */
#define KS_AUTH_SIMPLE  OV_TT_SIMPLE /* SIMPLE-A/V-Module */

/* ----------------------------------------------------------------------------
 * Access modes -- this does not only embrace the "classical" access modes like
 * read & write, but also accessing parts, instantiability, etc...
 */
typedef OV_ACCESS KS_ACCESS;
#define KS_AC_NONE          OV_AC_NONE
#define KS_AC_READ          OV_AC_READ
#define KS_AC_WRITE         OV_AC_WRITE
#define KS_AC_READWRITE     OV_AC_READWRITE

#define KS_AC_DELETEABLE    OV_AC_DELETEABLE
#define KS_AC_RENAMEABLE    OV_AC_RENAMEABLE
#define KS_AC_LINKABLE      OV_AC_LINKABLE
#define KS_AC_UNLINKABLE    OV_AC_UNLINKABLE

    /* object can act as a factory */
#define KS_AC_INSTANTIABLE  OV_AC_INSTANTIABLE
    /* object is part of another object */
#define KS_AC_PART          OV_AC_PART

/*
*	KS_INTERPOLATION_MODE:
*	----------------------
*	Interpolation modes for use with ACPLT/KS histories
*/
typedef OV_INTERPOLATION_MODE KS_INTERPOLATION_MODE;
#define KS_IPM_NONE     OV_IPM_NONE
#define KS_IPM_LINEAR   OV_IPM_LINEAR
#define KS_IPM_MIN      OV_IPM_MIN
#define KS_IPM_MAX      OV_IPM_MAX
#define KS_IPM_HOLD     OV_IPM_HOLD
#define KS_IPM_DEFAULT  OV_IPM_DEFAULT

/* ----------------------------------------------------------------------------
 * Option flags for the GETEP service.
 */
ENUMDEF(KS_EP_FLAGS)
#define KS_EPF_PARTS     ENUMVAL(KS_EP_FLAGS, 0x80000000)
#define KS_EPF_CHILDREN  ENUMVAL(KS_EP_FLAGS, 0x40000000)
#define KS_EPF_FLATTEN   ENUMVAL(KS_EP_FLAGS, 0x00000001)
#define KS_EPF_DEFAULT   ENUMVAL(KS_EP_FLAGS, KS_EPF_PARTS | KS_EPF_CHILDREN)

typedef KS_EP_FLAGS OV_EP_FLAGS; //needed in xdrClient

/* ----------------------------------------------------------------------------
 * Service codes. Dial 1-800-ACPLTKS for more information =:)
 */
ENUMDEF(KS_SVC)
#define KS_NULL           OV_NULL
    /*
     * Now for the manager part of the KS protocol
     */
#define KS_REGISTER       ENUMVAL(KS_SVC, 0x0000FF01)
#define KS_UNREGISTER     ENUMVAL(KS_SVC, 0x0000FF02)
#define KS_GETSERVER      ENUMVAL(KS_SVC, 0x0000FF03)

    /*
     * The GetPP/GetEP services group.
     */
#define KS_GETPP          ENUMVAL(KS_SVC, 0x00000001)
#define KS_GETEP          ENUMVAL(KS_SVC, 0x00000002)
#define KS_GETCANONICALPATH ENUMVAL(KS_SVC, 0x00000003)

    /*
     * The variable access service group.
     */
#ifndef KS_GETVAR
	#define KS_GETVAR         OV_GETVAR
	#define KS_SETVAR         OV_SETVAR
#endif
#define KS_EXGDATA        ENUMVAL(KS_SVC, 0x00000103)

    /*
     * Object management service group.
     */
#ifndef KS_CREATEOBJECT
	#define KS_CREATEOBJECT   OV_CREATEOBJECT
	#define KS_DELETEOBJECT   OV_DELETEOBJECT
	#define KS_RENAMEOBJECT   OV_RENAMEOBJECT
#endif

    /*
     * Structure management service group.
     */
#ifndef KS_LINK
	#define KS_LINK           OV_LINK
	#define KS_UNLINK         OV_UNLINK
#endif

    /*
     * History access service group.
     */
#ifndef KS_GETHIST
#define KS_GETHIST        OV_GETHIST
#endif



/*
*	shared datatypes ACPLT/KS -- ACPLT/OV
*/
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

#endif

/*
*	End of file
*/

