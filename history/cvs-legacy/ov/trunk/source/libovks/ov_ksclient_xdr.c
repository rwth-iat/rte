/*
*   $Id: ov_ksclient_xdr.c,v 1.2 1999-07-26 16:14:15 dirk Exp $
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

#define OV_COMPILE_LIBOVKS

#include "libovks/ov_ksclient_xdr.h"

#include <rpc/types.h>
#include <rpc/xdr.h>

#if OV_COMPILER_BORLAND
#pragma warn -pro
#endif

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for bytes
*/
OV_BOOL ov_ksclient_xdr_bytes(
	XDR		*xdrs,
	char	**cpp,
	OV_UINT	*sizep,
	OV_UINT	maxsize
) {
	/*
	*	local variables
	*/
	u_int	_size;
	u_int	_maxsize = maxsize;
	/*
	*	instructions
	*/
	switch(xdrs->x_op) {
		case XDR_ENCODE:
			_size = *sizep;
			return xdr_bytes(xdrs, cpp, &_size, _maxsize);
		case XDR_DECODE:
			if(!xdr_bytes(xdrs, cpp, &_size, _maxsize)) {
				return FALSE;
			}
			*sizep = _size;
			return TRUE;
		default:
			break;
	}
	return FALSE;
}

/*	----------------------------------------------------------------------	*/

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
) {
	/*
	*	local variables
	*/
	u_int	_size;
	u_int	_maxsize = maxsize;
	u_int	_elsize = elsize;
	/*
	*	instructions
	*/
	switch(xdrs->x_op) {
		case XDR_ENCODE:
			_size = *sizep;
			return xdr_array(xdrs, addrp, &_size, _maxsize, _elsize, elproc);
		case XDR_DECODE:
			if(!xdr_array(xdrs, addrp, &_size, _maxsize, _elsize, elproc)) {
				return FALSE;
			}
			*sizep = _size;
			return TRUE;
		default:
			break;
	}
	return FALSE;
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_TIME
*/
OV_KSCLIENT_DECL_XDRFNC(KS_TIME) {
	if(!ov_ksclient_xdr_KS_UINT(xdrs, &objp->secs)) {
		return FALSE;
	}
	return ov_ksclient_xdr_KS_UINT(xdrs, &objp->usecs);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_TIME_SPAN
*/
OV_KSCLIENT_DECL_XDRFNC(KS_TIME_SPAN) {
	if(!ov_ksclient_xdr_KS_INT(xdrs, &objp->secs)) {
		return FALSE;
	}
	return ov_ksclient_xdr_KS_INT(xdrs, &objp->usecs);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_STRING
*/
OV_KSCLIENT_DECL_XDRFNC(KS_STRING) {
	return xdr_string(xdrs, objp, ~0);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_AVMODULE_PAR
*/
OV_KSCLIENT_DECL_XDRFNC(KS_AVMODULE_PAR) {
	if(!ov_ksclient_xdr_KS_TICKET_TYPE(xdrs, &objp->tickettype)) {
		return FALSE;
	}
	switch(objp->tickettype) {
		case KS_AUTH_NONE:
			return TRUE;
		case KS_AUTH_SIMPLE:
			return xdr_string(xdrs, &objp->id, KS_SIMPLEID_MAXLEN);
		default:
			break;
	}
	return FALSE;
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_AVMODULE_RES
*/
OV_KSCLIENT_DECL_XDRFNC(KS_AVMODULE_RES) {
	return ov_ksclient_xdr_KS_TICKET_TYPE(xdrs, &objp->tickettype);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_VAR_VALUE
*/
OV_KSCLIENT_DECL_XDRFNC(KS_VAR_VALUE) {
	if(!ov_ksclient_xdr_KS_VAR_TYPE(xdrs, &objp->vartype)) {
		return FALSE;
	}
	if(xdrs->x_op == XDR_DECODE) {
		objp->veclen = 1;
	}
	switch(objp->vartype) {
		case KS_VT_BOOL:
			return ov_ksclient_xdr_KS_BOOL(xdrs, &objp->valueunion.val_bool);
		case KS_VT_INT:
			return ov_ksclient_xdr_KS_INT(xdrs, &objp->valueunion.val_int);
		case KS_VT_UINT:
			return ov_ksclient_xdr_KS_UINT(xdrs, &objp->valueunion.val_uint);
		case KS_VT_SINGLE:
			return ov_ksclient_xdr_KS_SINGLE(xdrs, &objp->valueunion.val_single);
		case KS_VT_DOUBLE:
			return ov_ksclient_xdr_KS_DOUBLE(xdrs, &objp->valueunion.val_double);
		case KS_VT_STRING:
			return ov_ksclient_xdr_KS_STRING(xdrs, &objp->valueunion.val_string);
		case KS_VT_TIME:
			return ov_ksclient_xdr_KS_TIME(xdrs, &objp->valueunion.val_time);
		case KS_VT_TIME_SPAN:
			return ov_ksclient_xdr_KS_TIME_SPAN(xdrs, &objp->valueunion.val_time_span);
		case KS_VT_BOOL_VEC:
			return ov_ksclient_xdr_array(xdrs, (char **)&objp->valueunion.val_bool_vec,
				&objp->veclen, ~0, sizeof(bool_t), (xdrproc_t)ov_ksclient_xdr_KS_BOOL);
		case KS_VT_INT_VEC:
			return ov_ksclient_xdr_array(xdrs, (char **)&objp->valueunion.val_int_vec,
				&objp->veclen, ~0, sizeof(long), (xdrproc_t)ov_ksclient_xdr_KS_INT);
		case KS_VT_UINT_VEC:
			return ov_ksclient_xdr_array(xdrs, (char **)&objp->valueunion.val_uint_vec,
				&objp->veclen, ~0, sizeof(u_long), (xdrproc_t)ov_ksclient_xdr_KS_UINT);
		case KS_VT_SINGLE_VEC:
			return ov_ksclient_xdr_array(xdrs, (char **)&objp->valueunion.val_single_vec,
				&objp->veclen, ~0, sizeof(float), (xdrproc_t)ov_ksclient_xdr_KS_SINGLE);
		case KS_VT_DOUBLE_VEC:
			return ov_ksclient_xdr_array(xdrs, (char **)&objp->valueunion.val_double_vec,
				&objp->veclen, ~0, sizeof(double), (xdrproc_t)ov_ksclient_xdr_KS_DOUBLE);
		case KS_VT_STRING_VEC:
			return ov_ksclient_xdr_array(xdrs, (char **)&objp->valueunion.val_string_vec,
				&objp->veclen, ~0, sizeof(KS_STRING), (xdrproc_t)ov_ksclient_xdr_KS_STRING);
		case KS_VT_TIME_VEC:
			return ov_ksclient_xdr_array(xdrs, (char **)&objp->valueunion.val_time_vec,
				&objp->veclen, ~0, sizeof(KS_TIME), (xdrproc_t)ov_ksclient_xdr_KS_TIME);
		case KS_VT_TIME_SPAN_VEC:
			return ov_ksclient_xdr_array(xdrs, (char **)&objp->valueunion.val_time_span_vec,
				&objp->veclen, ~0, sizeof(KS_TIME_SPAN), (xdrproc_t)ov_ksclient_xdr_KS_TIME_SPAN);
		case KS_VT_VOID:
			return TRUE;
		case KS_VT_BYTE_VEC:
			return ov_ksclient_xdr_bytes(xdrs, (char **)&objp->valueunion.val_byte_vec,
				&objp->veclen, ~0);
		default:
			break;
	}
	return FALSE;
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_VAR_PROJECTED_PROPS
*/
OV_KSCLIENT_DECL_XDRFNC(KS_VAR_PROJECTED_PROPS) {
	if(!xdr_string(xdrs, &objp->tech_unit, KS_TECHUNIT_MAXLEN)) {
		return FALSE;
	}
	return ov_ksclient_xdr_KS_VAR_TYPE(xdrs, &objp->vartype);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_VAR_CURRENT_PROPS
*/
OV_KSCLIENT_DECL_XDRFNC(KS_VAR_CURRENT_PROPS) {
	if(!ov_ksclient_xdr_KS_VAR_VALUE(xdrs, &objp->value)) {
		return FALSE;
	}
	if(!ov_ksclient_xdr_KS_TIME(xdrs, &objp->time)) {
		return FALSE;
	}
	return ov_ksclient_xdr_KS_STATE(xdrs, &objp->state);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_LINK_PROJECTED_PROPS
*/
OV_KSCLIENT_DECL_XDRFNC(KS_LINK_PROJECTED_PROPS) {
	if(!ov_ksclient_xdr_KS_LINK_TYPE(xdrs, &objp->linktype)) {
		return FALSE;
	}
	if(!xdr_string(xdrs, &objp->opposite_role_identifier, ~0)) {
		return FALSE;
	}
	return xdr_string(xdrs, &objp->association_identifier, ~0);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_OBJ_PROJECTED_PROPS
*/
OV_KSCLIENT_DECL_XDRFNC(KS_OBJ_PROJECTED_PROPS) {
	if(!ov_ksclient_xdr_KS_OBJ_TYPE(xdrs, &objp->objtype)) {
		return FALSE;
	}
	switch(objp->objtype) {
		case KS_OT_VARIABLE:
			if(!ov_ksclient_xdr_KS_VAR_PROJECTED_PROPS(xdrs, &objp->KS_OBJ_PROJECTED_PROPS_u.
				var_projected_props)
			) {
				return FALSE;
			}
			break;
		case KS_OT_LINK:
			if(!ov_ksclient_xdr_KS_LINK_PROJECTED_PROPS(xdrs, &objp->KS_OBJ_PROJECTED_PROPS_u.
				link_projected_props)
			) {
				return FALSE;
			}
			break;
		default:
			break;
	}
	if(!xdr_string(xdrs, &objp->identifier, KS_NAME_MAXLEN)) {
		return FALSE;
	}
	if(!ov_ksclient_xdr_KS_TIME(xdrs, &objp->creation_time)) {
		return FALSE;
	}
	if(!xdr_string(xdrs, &objp->comment, KS_COMMENT_MAXLEN)) {
		return FALSE;
	}
	return ov_ksclient_xdr_KS_ACCESS(xdrs, &objp->access);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_GETPP_PAR
*/
OV_KSCLIENT_DECL_XDRFNC(KS_GETPP_PAR) {
	if(!xdr_string(xdrs, &objp->path, ~0)) {
		return FALSE;
	}
	if(!ov_ksclient_xdr_KS_OBJ_TYPE(xdrs, &objp->type_mask)) {
		return FALSE;
	}
	return xdr_string(xdrs, &objp->name_mask, ~0);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_GETPP_RES
*/
OV_KSCLIENT_DECL_XDRFNC(KS_GETPP_RES) {
	if(!ov_ksclient_xdr_KS_RESULT(xdrs, &objp->result)) {
		return FALSE;
	}
	switch(objp->result) {
		case KS_ERR_OK:
			return OV_KSCLIENT_XDR_ARRAY(KS_OBJ_PROJECTED_PROPS, items);
		default:
			break;
	}
	return TRUE;
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_GETVAR_ITEM
*/
OV_KSCLIENT_DECL_XDRFNC(KS_GETVAR_ITEM) {
	KS_OBJ_TYPE	objtype = KS_OT_VARIABLE;
	if(!ov_ksclient_xdr_KS_RESULT(xdrs, &objp->result)) {
		return FALSE;
	}
	switch(objp->result) {
		case KS_ERR_OK:
			if(!ov_ksclient_xdr_KS_OBJ_TYPE(xdrs, &objtype)) {
				return FALSE;
			}
			if(objtype != KS_OT_VARIABLE) {
				return FALSE;
			}
			return ov_ksclient_xdr_KS_VAR_CURRENT_PROPS(xdrs, &objp->var_current_props);
		default:
			break;
	}
	return TRUE;
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_GETVAR_PAR
*/
OV_KSCLIENT_DECL_XDRFNC(KS_GETVAR_PAR) {
	return OV_KSCLIENT_XDR_ARRAY(KS_STRING, identifiers);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_GETVAR_RES
*/
OV_KSCLIENT_DECL_XDRFNC(KS_GETVAR_RES) {
	if(!ov_ksclient_xdr_KS_RESULT(xdrs, &objp->result)) {
		return FALSE;
	}
	switch(objp->result) {
		case KS_ERR_OK:
			return OV_KSCLIENT_XDR_ARRAY(KS_GETVAR_ITEM, items);
		default:
			break;
	}
	return TRUE;
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_SETVAR_ITEM
*/
OV_KSCLIENT_DECL_XDRFNC(KS_SETVAR_ITEM) {
	KS_OBJ_TYPE	objtype = KS_OT_VARIABLE;
	if(!xdr_string(xdrs, &objp->path_and_name, ~0)) {
		return FALSE;
	}
	if(!ov_ksclient_xdr_KS_OBJ_TYPE(xdrs, &objtype)) {
		return FALSE;
	}
	if(objtype != KS_OT_VARIABLE) {
		return FALSE;
	}
	return ov_ksclient_xdr_KS_VAR_CURRENT_PROPS(xdrs, &objp->var_current_props);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_SETVAR_PAR
*/
OV_KSCLIENT_DECL_XDRFNC(KS_SETVAR_PAR) {
	return OV_KSCLIENT_XDR_ARRAY(KS_SETVAR_ITEM, items);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_SETVAR_RES
*/
OV_KSCLIENT_DECL_XDRFNC(KS_SETVAR_RES) {
	if(!ov_ksclient_xdr_KS_RESULT(xdrs, &objp->result)) {
		return FALSE;
	}
	switch(objp->result) {
		case KS_ERR_OK:
			return OV_KSCLIENT_XDR_ARRAY(KS_RESULT, results);
		default:
			break;
	}
	return TRUE;
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_EXGDATA_PAR
*/
OV_KSCLIENT_DECL_XDRFNC(KS_EXGDATA_PAR) {
	if(!OV_KSCLIENT_XDR_ARRAY(KS_SETVAR_ITEM, set_vars)) {
		return FALSE;
	}
	return OV_KSCLIENT_XDR_ARRAY(KS_STRING, get_vars);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_EXGDATA_RES
*/
OV_KSCLIENT_DECL_XDRFNC(KS_EXGDATA_RES) {
	if(!ov_ksclient_xdr_KS_RESULT(xdrs, &objp->result)) {
		return FALSE;
	}
	switch(objp->result) {
		case KS_ERR_OK:
			if(!OV_KSCLIENT_XDR_ARRAY(KS_RESULT, results)) {
				return FALSE;
			}
			return OV_KSCLIENT_XDR_ARRAY(KS_GETVAR_ITEM, items);
		default:
			break;
	}
	return TRUE;
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_DOMAIN_ENGINEERED_PROPS
*/
OV_KSCLIENT_DECL_XDRFNC(KS_DOMAIN_ENGINEERED_PROPS) {
	return xdr_string(xdrs, &objp->class_identifier, ~0);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_VAR_ENGINEERED_PROPS
*/
OV_KSCLIENT_DECL_XDRFNC(KS_VAR_ENGINEERED_PROPS) {
	if(!xdr_string(xdrs, &objp->tech_unit, KS_TECHUNIT_MAXLEN)) {
		return FALSE;
	}
	return ov_ksclient_xdr_KS_VAR_TYPE(xdrs, &objp->vartype);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_LINK_ENGINEERED_PROPS
*/
OV_KSCLIENT_DECL_XDRFNC(KS_LINK_ENGINEERED_PROPS) {
	if(!ov_ksclient_xdr_KS_LINK_TYPE(xdrs, &objp->linktype)) {
		return FALSE;
	}
	if(!xdr_string(xdrs, &objp->opposite_role_identifier, ~0)) {
		return FALSE;
	}
	return xdr_string(xdrs, &objp->association_identifier, ~0);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_OBJ_ENGINEERED_PROPS
*/
OV_KSCLIENT_DECL_XDRFNC(KS_OBJ_ENGINEERED_PROPS) {
	if(!ov_ksclient_xdr_KS_OBJ_TYPE(xdrs, &objp->objtype)) {
		return FALSE;
	}
	switch(objp->objtype) {
		case KS_OT_DOMAIN:
			if(!ov_ksclient_xdr_KS_DOMAIN_ENGINEERED_PROPS(xdrs, &objp->KS_OBJ_ENGINEERED_PROPS_u.
				domain_engineered_props)
			) {
				return FALSE;
			}
			break;
		case KS_OT_VARIABLE:
			if(!ov_ksclient_xdr_KS_VAR_ENGINEERED_PROPS(xdrs, &objp->KS_OBJ_ENGINEERED_PROPS_u.
				var_engineered_props)
			) {
				return FALSE;
			}
			break;
		case KS_OT_LINK:
			if(!ov_ksclient_xdr_KS_LINK_ENGINEERED_PROPS(xdrs, &objp->KS_OBJ_ENGINEERED_PROPS_u.
				link_engineered_props)
			) {
				return FALSE;
			}
			break;
		default:
			break;
	}
	if(!xdr_string(xdrs, &objp->identifier, KS_NAME_MAXLEN)) {
		return FALSE;
	}
	if(!ov_ksclient_xdr_KS_TIME(xdrs, &objp->creation_time)) {
		return FALSE;
	}
	if(!xdr_string(xdrs, &objp->comment, KS_COMMENT_MAXLEN)) {
		return FALSE;
	}
	if(!ov_ksclient_xdr_KS_ACCESS(xdrs, &objp->access)) {
		return FALSE;
	}
	return ov_ksclient_xdr_KS_SEMANTIC_FLAGS(xdrs, &objp->semantic_flags);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_GETEP_PAR
*/
OV_KSCLIENT_DECL_XDRFNC(KS_GETEP_PAR) {
	if(!xdr_string(xdrs, &objp->path, ~0)) {
		return FALSE;
	}
	if(!ov_ksclient_xdr_KS_OBJ_TYPE(xdrs, &objp->type_mask)) {
		return FALSE;
	}
	if(!xdr_string(xdrs, &objp->name_mask, ~0)) {
		return FALSE;
	}
	return ov_ksclient_xdr_KS_EP_FLAGS(xdrs, &objp->scope_flags);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_GETEP_RES
*/
OV_KSCLIENT_DECL_XDRFNC(KS_GETEP_RES) {
	if(!ov_ksclient_xdr_KS_RESULT(xdrs, &objp->result)) {
		return FALSE;
	}
	switch(objp->result) {
		case KS_ERR_OK:
			return OV_KSCLIENT_XDR_ARRAY(KS_OBJ_ENGINEERED_PROPS, items);
		default:
			break;
	}
	return TRUE;
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_PLACEMENT
*/
OV_KSCLIENT_DECL_XDRFNC(KS_PLACEMENT) {
	if(!ov_ksclient_xdr_KS_PLACEMENT_HINT(xdrs, &objp->hint)) {
		return FALSE;
	}
	switch(objp->hint) {
		case KS_PMH_BEFORE:
		case KS_PMH_AFTER:
			return xdr_string(xdrs, &objp->place_path, ~0);
		default:
			break;
	}
	return TRUE;
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_LINK_ITEM
*/
OV_KSCLIENT_DECL_XDRFNC(KS_LINK_ITEM) {
	if(!xdr_string(xdrs, &objp->link_path, ~0)) {
		return FALSE;
	}
	if(!xdr_string(xdrs, &objp->element_path, ~0)) {
		return FALSE;
	}
	if(!ov_ksclient_xdr_KS_PLACEMENT(xdrs, &objp->place)) {
		return FALSE;
	}
	return ov_ksclient_xdr_KS_PLACEMENT(xdrs, &objp->opposite_place);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_UNLINK_ITEM
*/
OV_KSCLIENT_DECL_XDRFNC(KS_UNLINK_ITEM) {
	if(!xdr_string(xdrs, &objp->link_path, ~0)) {
		return FALSE;
	}
	return xdr_string(xdrs, &objp->element_path, ~0);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_CREATEOBJ_ITEM
*/
OV_KSCLIENT_DECL_XDRFNC(KS_CREATEOBJ_ITEM) {
	if(!xdr_string(xdrs, &objp->factory_path, ~0)) {
		return FALSE;
	}
	if(!xdr_string(xdrs, &objp->new_path, ~0)) {
		return FALSE;
	}
	if(!ov_ksclient_xdr_KS_PLACEMENT(xdrs, &objp->place)) {
		return FALSE;
	}
	if(!OV_KSCLIENT_XDR_ARRAY(KS_SETVAR_ITEM, parameters)) {
		return FALSE;
	}
	return OV_KSCLIENT_XDR_ARRAY(KS_LINK_ITEM, links);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_CREATEOBJECT_PAR
*/
OV_KSCLIENT_DECL_XDRFNC(KS_CREATEOBJECT_PAR) {
	return OV_KSCLIENT_XDR_ARRAY(KS_CREATEOBJ_ITEM, items);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_CREATEOBJECTITEM_RES
*/
OV_KSCLIENT_DECL_XDRFNC(KS_CREATEOBJECTITEM_RES) {
	if(!ov_ksclient_xdr_KS_RESULT(xdrs, &objp->result)) {
		return FALSE;
	}
	switch(objp->result) {
		case KS_ERR_BADINITPARAM:
			if(!OV_KSCLIENT_XDR_ARRAY(KS_RESULT, params_results)) {
				return FALSE;
			}
			return OV_KSCLIENT_XDR_ARRAY(KS_RESULT, link_results);
		default:
			break;
	}
	return TRUE;
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_CREATEOBJECT_RES
*/
OV_KSCLIENT_DECL_XDRFNC(KS_CREATEOBJECT_RES) {
	if(!ov_ksclient_xdr_KS_RESULT(xdrs, &objp->result)) {
		return FALSE;
	}
	switch(objp->result) {
		case KS_ERR_OK:
			return OV_KSCLIENT_XDR_ARRAY(KS_CREATEOBJECTITEM_RES, obj_results);
		default:
			break;
	}
	return TRUE;
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_DELETEOBJECT_PAR
*/
OV_KSCLIENT_DECL_XDRFNC(KS_DELETEOBJECT_PAR) {
	return OV_KSCLIENT_XDR_ARRAY(KS_STRING, paths);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_DELETEOBJECT_RES
*/
OV_KSCLIENT_DECL_XDRFNC(KS_DELETEOBJECT_RES) {
	if(!ov_ksclient_xdr_KS_RESULT(xdrs, &objp->result)) {
		return FALSE;
	}
	switch(objp->result) {
		case KS_ERR_OK:
			return OV_KSCLIENT_XDR_ARRAY(KS_RESULT, results);
		default:
			break;
	}
	return TRUE;
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_RENAMEOBJECT_ITEM
*/
OV_KSCLIENT_DECL_XDRFNC(KS_RENAMEOBJECT_ITEM) {
	if(!xdr_string(xdrs, &objp->old_path, ~0)) {
		return FALSE;
	}
	if(!xdr_string(xdrs, &objp->new_path, ~0)) {
		return FALSE;
	}
	return ov_ksclient_xdr_KS_PLACEMENT(xdrs, &objp->place);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_RENAMEOBJECT_PAR
*/
OV_KSCLIENT_DECL_XDRFNC(KS_RENAMEOBJECT_PAR) {
	return OV_KSCLIENT_XDR_ARRAY(KS_RENAMEOBJECT_ITEM, items);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_RENAMEOBJECT_RES
*/
OV_KSCLIENT_DECL_XDRFNC(KS_RENAMEOBJECT_RES) {
	if(!ov_ksclient_xdr_KS_RESULT(xdrs, &objp->result)) {
		return FALSE;
	}
	switch(objp->result) {
		case KS_ERR_OK:
			return OV_KSCLIENT_XDR_ARRAY(KS_RESULT, results);
		default:
			break;
	}
	return TRUE;
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_GETCANONICALPATH_PAR
*/
OV_KSCLIENT_DECL_XDRFNC(KS_GETCANONICALPATH_PAR) {
	return OV_KSCLIENT_XDR_ARRAY(KS_STRING, paths);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_GETCANONICALPATHITEM_RES
*/
OV_KSCLIENT_DECL_XDRFNC(KS_GETCANONICALPATHITEM_RES) {
	if(!ov_ksclient_xdr_KS_RESULT(xdrs, &objp->result)) {
		return FALSE;
	}
	switch(objp->result) {
		case KS_ERR_OK:
			return xdr_string(xdrs, &objp->canonical_path, ~0);
		default:
			break;
	}
	return TRUE;
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_GETCANONICALPATH_RES
*/
OV_KSCLIENT_DECL_XDRFNC(KS_GETCANONICALPATH_RES) {
	if(!ov_ksclient_xdr_KS_RESULT(xdrs, &objp->result)) {
		return FALSE;
	}
	switch(objp->result) {
		case KS_ERR_OK:
			return OV_KSCLIENT_XDR_ARRAY(KS_GETCANONICALPATHITEM_RES, results);
		default:
			break;
	}
	return TRUE;
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_LINK_PAR
*/
OV_KSCLIENT_DECL_XDRFNC(KS_LINK_PAR) {
	return OV_KSCLIENT_XDR_ARRAY(KS_LINK_ITEM, items);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_LINK_RES
*/
OV_KSCLIENT_DECL_XDRFNC(KS_LINK_RES) {
	if(!ov_ksclient_xdr_KS_RESULT(xdrs, &objp->result)) {
		return FALSE;
	}
	switch(objp->result) {
		case KS_ERR_OK:
			return OV_KSCLIENT_XDR_ARRAY(KS_RESULT, results);
		default:
			break;
	}
	return TRUE;
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_UNLINK_PAR
*/
OV_KSCLIENT_DECL_XDRFNC(KS_UNLINK_PAR) {
	return OV_KSCLIENT_XDR_ARRAY(KS_UNLINK_ITEM, items);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_UNLINK_RES
*/
OV_KSCLIENT_DECL_XDRFNC(KS_UNLINK_RES) {
	if(!ov_ksclient_xdr_KS_RESULT(xdrs, &objp->result)) {
		return FALSE;
	}
	switch(objp->result) {
		case KS_ERR_OK:
			return OV_KSCLIENT_XDR_ARRAY(KS_RESULT, results);
		default:
			break;
	}
	return TRUE;
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_ABSRELTIME
*/
OV_KSCLIENT_DECL_XDRFNC(KS_ABSRELTIME) {
	if(!ov_ksclient_xdr_KS_TIME_TYPE(xdrs, &objp->timetype)) {
		return FALSE;
	}
	switch(objp->timetype) {
		case KS_TT_ABSOLUTE:
			return ov_ksclient_xdr_KS_TIME(xdrs,
				&objp->KS_ABSRELTIME_u.abstime);
		case KS_TT_RELATIVE:
			return ov_ksclient_xdr_KS_TIME_SPAN(xdrs,
				&objp->KS_ABSRELTIME_u.reltime);
		default:
			break;
	}
	return FALSE;
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_TIMEHISTSELECTOR
*/
OV_KSCLIENT_DECL_XDRFNC(KS_TIMEHISTSELECTOR) {
	if(!ov_ksclient_xdr_KS_INTERPOLATION_MODE(xdrs, &objp->ip_mode)) {
		return FALSE;
	}
	if(!ov_ksclient_xdr_KS_ABSRELTIME(xdrs, &objp->from)) {
		return FALSE;
	}
	if(!ov_ksclient_xdr_KS_ABSRELTIME(xdrs, &objp->to)) {
		return FALSE;
	}
	return ov_ksclient_xdr_KS_TIME_SPAN(xdrs, &objp->delta);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_STRINGHISTSELECTOR
*/
OV_KSCLIENT_DECL_XDRFNC(KS_STRINGHISTSELECTOR) {
	return ov_ksclient_xdr_KS_STRING(xdrs, &objp->mask);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_HISTSELECTOR
*/
OV_KSCLIENT_DECL_XDRFNC(KS_HISTSELECTOR) {
	if(!ov_ksclient_xdr_KS_HSEL_TYPE(xdrs, &objp->hseltype)) {
		return FALSE;
	}
	switch(objp->hseltype) {
		case KS_HSELT_NONE:
			return TRUE;
		case KS_HSELT_TIME:
			return ov_ksclient_xdr_KS_TIMEHISTSELECTOR(xdrs,
				&objp->KS_HISTSELECTOR_u.ths);
		case KS_HSELT_STRING:
			return ov_ksclient_xdr_KS_STRINGHISTSELECTOR(xdrs,
				&objp->KS_HISTSELECTOR_u.shs);
		default:
			break;
	}
	return FALSE;
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_GETHIST_ITEM
*/
OV_KSCLIENT_DECL_XDRFNC(KS_GETHIST_ITEM) {
	if(!ov_ksclient_xdr_KS_STRING(xdrs, &objp->part)) {
		return FALSE;
	}
	return ov_ksclient_xdr_KS_HISTSELECTOR(xdrs, &objp->selector);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_GETHISTRESULT_ITEM
*/
OV_KSCLIENT_DECL_XDRFNC(KS_GETHISTRESULT_ITEM) {
	if(!ov_ksclient_xdr_KS_RESULT(xdrs, &objp->result)) {
		return FALSE;
	}
	switch(objp->result) {
		case KS_ERR_OK:
			return ov_ksclient_xdr_KS_VAR_VALUE(xdrs, &objp->value);
		default:
			break;
	}
	return TRUE;
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_GETHISTSINGLERESULT
*/
OV_KSCLIENT_DECL_XDRFNC(KS_GETHISTSINGLERESULT) {
	if(!ov_ksclient_xdr_KS_RESULT(xdrs, &objp->result)) {
		return FALSE;
	}
	switch(objp->result) {
		case KS_ERR_OK:
			return OV_KSCLIENT_XDR_ARRAY(KS_GETHISTRESULT_ITEM, items);
		default:
			break;
	}
	return TRUE;
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_GETHIST_PAR
*/
OV_KSCLIENT_DECL_XDRFNC(KS_GETHIST_PAR) {
	if(!OV_KSCLIENT_XDR_ARRAY(KS_STRING, paths)) {
		return FALSE;
	}
	if(!ov_ksclient_xdr_KS_UINT(xdrs, &objp->max_answers)) {
		return FALSE;
	}
	return OV_KSCLIENT_XDR_ARRAY(KS_GETHIST_ITEM, items);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_GETHIST_RES
*/
OV_KSCLIENT_DECL_XDRFNC(KS_GETHIST_RES) {
	if(!ov_ksclient_xdr_KS_RESULT(xdrs, &objp->result)) {
		return FALSE;
	}
	switch(objp->result) {
		case KS_ERR_OK:
			return OV_KSCLIENT_XDR_ARRAY(KS_GETHISTSINGLERESULT, results);
		default:
			break;
	}
	return TRUE;
}

/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/

