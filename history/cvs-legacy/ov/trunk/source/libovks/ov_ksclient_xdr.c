/*
*   $Id: ov_ksclient_xdr.c,v 1.1 1999-07-19 15:02:16 dirk Exp $
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
KS_BOOL ov_ksclient_xdr_KS_TIME(
	XDR		*xdrs,
	KS_TIME	*objp
) {
	if(!ov_ksclient_xdr_KS_UINT(xdrs, &objp->secs)) {
		return FALSE;
	}
	return ov_ksclient_xdr_KS_UINT(xdrs, &objp->usecs);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_TIME_SPAN
*/
KS_BOOL ov_ksclient_xdr_KS_TIME_SPAN(
	XDR				*xdrs,
	KS_TIME_SPAN	*objp
) {
	if(!ov_ksclient_xdr_KS_INT(xdrs, &objp->secs)) {
		return FALSE;
	}
	return ov_ksclient_xdr_KS_INT(xdrs, &objp->usecs);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_STRING
*/
KS_BOOL ov_ksclient_xdr_KS_STRING(
	XDR			*xdrs,
	KS_STRING	*objp
) {
	return xdr_string(xdrs, objp, ~0);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_AVMODULE_PAR
*/
KS_BOOL ov_ksclient_xdr_KS_AVMODULE_PAR(
	XDR			*xdrs,
	KS_AVMODULE	*objp
) {
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
KS_BOOL ov_ksclient_xdr_KS_AVMODULE_RES(
	XDR			*xdrs,
	KS_AVMODULE	*objp
) {
	return ov_ksclient_xdr_KS_TICKET_TYPE(xdrs, &objp->tickettype);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_VAR_VALUE
*/
KS_BOOL ov_ksclient_xdr_KS_VAR_VALUE(
	XDR				*xdrs,
	KS_VAR_VALUE	*objp
) {
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
KS_BOOL ov_ksclient_xdr_KS_VAR_PROJECTED_PROPS(
	XDR						*xdrs,
	KS_VAR_PROJECTED_PROPS	*objp
) {
	if(!xdr_string(xdrs, &objp->tech_unit, KS_TECHUNIT_MAXLEN)) {
		return FALSE;
	}
	return ov_ksclient_xdr_KS_VAR_TYPE(xdrs, &objp->vartype);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_VAR_CURRENT_PROPS
*/
KS_BOOL ov_ksclient_xdr_KS_VAR_CURRENT_PROPS(
	XDR						*xdrs,
	KS_VAR_CURRENT_PROPS	*objp
) {
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
KS_BOOL ov_ksclient_xdr_KS_LINK_PROJECTED_PROPS(
	XDR									*xdrs,
	KS_LINK_PROJECTED_PROPS	*objp
) {
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
KS_BOOL ov_ksclient_xdr_KS_OBJ_PROJECTED_PROPS(
	XDR						*xdrs,
	KS_OBJ_PROJECTED_PROPS	*objp
) {
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
KS_BOOL ov_ksclient_xdr_KS_GETPP_PAR(
	XDR				*xdrs,
	KS_GETPP_PAR	*objp
) {
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
KS_BOOL ov_ksclient_xdr_KS_GETPP_RES(
	XDR				*xdrs,
	KS_GETPP_RES	*objp
) {
	if(!ov_ksclient_xdr_KS_RESULT(xdrs, &objp->result)) {
		return FALSE;
	}
	switch(objp->result) {
		case KS_ERR_OK:
			return ov_ksclient_xdr_array(xdrs, (char **)&objp->items_val,
				&objp->items_len, ~0, sizeof(KS_OBJ_PROJECTED_PROPS),
				(xdrproc_t)ov_ksclient_xdr_KS_OBJ_PROJECTED_PROPS);
		default:
			break;
	}
	return TRUE;
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_GETVAR_ITEM
*/
KS_BOOL ov_ksclient_xdr_KS_GETVAR_ITEM(
	XDR				*xdrs,
	KS_GETVAR_ITEM	*objp
) {
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
KS_BOOL ov_ksclient_xdr_KS_GETVAR_PAR(
	XDR				*xdrs,
	KS_GETVAR_PAR	*objp
) {
	return ov_ksclient_xdr_array(xdrs, (char **)&objp->identifiers_val,
		&objp->identifiers_len, ~0, sizeof(KS_STRING), (xdrproc_t)ov_ksclient_xdr_KS_STRING);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_GETVAR_RES
*/
KS_BOOL ov_ksclient_xdr_KS_GETVAR_RES(
	XDR				*xdrs,
	KS_GETVAR_RES	*objp
) {
	if(!ov_ksclient_xdr_KS_RESULT(xdrs, &objp->result)) {
		return FALSE;
	}
	switch(objp->result) {
		case KS_ERR_OK:
			return ov_ksclient_xdr_array(xdrs, (char **)&objp->items_val, &objp->items_len,
				~0, sizeof(KS_GETVAR_ITEM), (xdrproc_t)ov_ksclient_xdr_KS_GETVAR_ITEM);
		default:
			break;
	}
	return TRUE;
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_SETVAR_ITEM
*/
KS_BOOL ov_ksclient_xdr_KS_SETVAR_ITEM(
	XDR				*xdrs,
	KS_SETVAR_ITEM	*objp
) {
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
KS_BOOL ov_ksclient_xdr_KS_SETVAR_PAR(
	XDR				*xdrs,
	KS_SETVAR_PAR	*objp
) {
	return ov_ksclient_xdr_array(xdrs, (char **)&objp->items_val, &objp->items_len, ~0,
		sizeof(KS_SETVAR_ITEM), (xdrproc_t)ov_ksclient_xdr_KS_SETVAR_ITEM);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_SETVAR_RES
*/
KS_BOOL ov_ksclient_xdr_KS_SETVAR_RES(
	XDR				*xdrs,
	KS_SETVAR_RES	*objp
) {
	if(!ov_ksclient_xdr_KS_RESULT(xdrs, &objp->result)) {
		return FALSE;
	}
	switch(objp->result) {
		case KS_ERR_OK:
			return ov_ksclient_xdr_array(xdrs, (char **)&objp->results_val, &objp->results_len,
				~0, sizeof(KS_RESULT), (xdrproc_t)ov_ksclient_xdr_KS_RESULT);
		default:
			break;
	}
	return TRUE;
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_EXGDATA_PAR
*/
KS_BOOL ov_ksclient_xdr_KS_EXGDATA_PAR(
	XDR				*xdrs,
	KS_EXGDATA_PAR	*objp
) {
	if(!ov_ksclient_xdr_array(xdrs, (char **)&objp->set_vars_val, &objp->set_vars_len,
		~0, sizeof(KS_SETVAR_ITEM), (xdrproc_t)ov_ksclient_xdr_KS_SETVAR_ITEM)
	) {
		return FALSE;
	}
	return ov_ksclient_xdr_array(xdrs, (char **)&objp->get_vars_val, &objp->get_vars_len,
		~0, sizeof(KS_STRING), (xdrproc_t)ov_ksclient_xdr_KS_STRING);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_EXGDATA_RES
*/
KS_BOOL ov_ksclient_xdr_KS_EXGDATA_RES(
	XDR				*xdrs,
	KS_EXGDATA_RES	*objp
) {
	if(!ov_ksclient_xdr_KS_RESULT(xdrs, &objp->result)) {
		return FALSE;
	}
	switch(objp->result) {
		case KS_ERR_OK:
			if(!ov_ksclient_xdr_array(xdrs, (char **)&objp->results_val, &objp->results_len,
				~0, sizeof(KS_RESULT), (xdrproc_t)ov_ksclient_xdr_KS_RESULT)
			) {
				return FALSE;
			}
			return ov_ksclient_xdr_array(xdrs, (char **)&objp->items_val, &objp->items_len,
				~0, sizeof(KS_GETVAR_ITEM), (xdrproc_t)ov_ksclient_xdr_KS_GETVAR_ITEM);
		default:
			break;
	}
	return TRUE;
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_DOMAIN_ENGINEERED_PROPS
*/
KS_BOOL ov_ksclient_xdr_KS_DOMAIN_ENGINEERED_PROPS(
	XDR							*xdrs,
	KS_DOMAIN_ENGINEERED_PROPS	*objp
) {
	return xdr_string(xdrs, &objp->class_identifier, ~0);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_VAR_ENGINEERED_PROPS
*/
KS_BOOL ov_ksclient_xdr_KS_VAR_ENGINEERED_PROPS(
	XDR						*xdrs,
	KS_VAR_ENGINEERED_PROPS	*objp
) {
	if(!xdr_string(xdrs, &objp->tech_unit, KS_TECHUNIT_MAXLEN)) {
		return FALSE;
	}
	return ov_ksclient_xdr_KS_VAR_TYPE(xdrs, &objp->vartype);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_LINK_ENGINEERED_PROPS
*/
KS_BOOL ov_ksclient_xdr_KS_LINK_ENGINEERED_PROPS(
	XDR									*xdrs,
	KS_LINK_ENGINEERED_PROPS	*objp
) {
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
KS_BOOL ov_ksclient_xdr_KS_OBJ_ENGINEERED_PROPS(
	XDR						*xdrs,
	KS_OBJ_ENGINEERED_PROPS	*objp
) {
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
KS_BOOL ov_ksclient_xdr_KS_GETEP_PAR(
	XDR				*xdrs,
	KS_GETEP_PAR	*objp
) {
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
KS_BOOL ov_ksclient_xdr_KS_GETEP_RES(
	XDR				*xdrs,
	KS_GETEP_RES	*objp
) {
	if(!ov_ksclient_xdr_KS_RESULT(xdrs, &objp->result)) {
		return FALSE;
	}
	switch(objp->result) {
		case KS_ERR_OK:
			return ov_ksclient_xdr_array(xdrs, (char **)&objp->items_val,
				&objp->items_len, ~0, sizeof(KS_OBJ_ENGINEERED_PROPS),
				(xdrproc_t)ov_ksclient_xdr_KS_OBJ_ENGINEERED_PROPS);
		default:
			break;
	}
	return TRUE;
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_PLACEMENT
*/
KS_BOOL ov_ksclient_xdr_KS_PLACEMENT(
	XDR				*xdrs,
	KS_PLACEMENT	*objp
) {
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
KS_BOOL ov_ksclient_xdr_KS_LINK_ITEM(
	XDR				*xdrs,
	KS_LINK_ITEM	*objp
) {
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
KS_BOOL ov_ksclient_xdr_KS_UNLINK_ITEM(
	XDR				*xdrs,
	KS_UNLINK_ITEM	*objp
) {
	if(!xdr_string(xdrs, &objp->link_path, ~0)) {
		return FALSE;
	}
	return xdr_string(xdrs, &objp->element_path, ~0);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_CREATEOBJ_ITEM
*/
KS_BOOL ov_ksclient_xdr_KS_CREATEOBJ_ITEM(
	XDR					*xdrs,
	KS_CREATEOBJ_ITEM	*objp
) {
	if(!xdr_string(xdrs, &objp->factory_path, ~0)) {
		return FALSE;
	}
	if(!xdr_string(xdrs, &objp->new_path, ~0)) {
		return FALSE;
	}
	if(!ov_ksclient_xdr_KS_PLACEMENT(xdrs, &objp->place)) {
		return FALSE;
	}
	if(!ov_ksclient_xdr_array(xdrs, (char **)&objp->parameters_val, &objp->parameters_len,
		~0, sizeof(KS_SETVAR_ITEM), (xdrproc_t)ov_ksclient_xdr_KS_SETVAR_ITEM)
	) {
		return FALSE;
	}
	return ov_ksclient_xdr_array(xdrs, (char **)&objp->links_val, &objp->links_len,
		~0, sizeof(KS_LINK_ITEM), (xdrproc_t)ov_ksclient_xdr_KS_LINK_ITEM);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_CREATEOBJECT_PAR
*/
KS_BOOL ov_ksclient_xdr_KS_CREATEOBJECT_PAR(
	XDR					*xdrs,
	KS_CREATEOBJECT_PAR	*objp
) {
	return ov_ksclient_xdr_array(xdrs, (char **)&objp->items_val, &objp->items_len,
		~0, sizeof(KS_CREATEOBJ_ITEM), (xdrproc_t)ov_ksclient_xdr_KS_CREATEOBJ_ITEM);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_CREATEOBJECTITEM_RES
*/
KS_BOOL ov_ksclient_xdr_KS_CREATEOBJECTITEM_RES(
	XDR						*xdrs,
	KS_CREATEOBJECTITEM_RES	*objp
) {
	if(!ov_ksclient_xdr_KS_RESULT(xdrs, &objp->result)) {
		return FALSE;
	}
	switch(objp->result) {
		case KS_ERR_BADINITPARAM:
			if(!ov_ksclient_xdr_array(xdrs, (char **)&objp->params_results_val,
				&objp->params_results_len, ~0, sizeof(KS_RESULT),
				(xdrproc_t)ov_ksclient_xdr_KS_RESULT)
			) {
				return FALSE;
			}
			return ov_ksclient_xdr_array(xdrs, (char **)&objp->link_results_val,
				&objp->link_results_len, ~0, sizeof(KS_RESULT),
				(xdrproc_t)ov_ksclient_xdr_KS_RESULT);
		default:
			break;
	}
	return TRUE;
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_CREATEOBJECT_RES
*/
KS_BOOL ov_ksclient_xdr_KS_CREATEOBJECT_RES(
	XDR					*xdrs,
	KS_CREATEOBJECT_RES	*objp
) {
	if(!ov_ksclient_xdr_KS_RESULT(xdrs, &objp->result)) {
		return FALSE;
	}
	switch(objp->result) {
		case KS_ERR_OK:
			return ov_ksclient_xdr_array(xdrs, (char **)&objp->obj_results_val,
				&objp->obj_results_len, ~0, sizeof(KS_CREATEOBJECTITEM_RES),
				(xdrproc_t)ov_ksclient_xdr_KS_CREATEOBJECTITEM_RES);
		default:
			break;
	}
	return TRUE;
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_DELETEOBJECT_PAR
*/
KS_BOOL ov_ksclient_xdr_KS_DELETEOBJECT_PAR(
	XDR					*xdrs,
	KS_DELETEOBJECT_PAR	*objp
) {
	return ov_ksclient_xdr_array(xdrs, (char **)&objp->paths_val, &objp->paths_len,
		~0, sizeof(KS_STRING), (xdrproc_t)ov_ksclient_xdr_KS_STRING);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_DELETEOBJECT_RES
*/
KS_BOOL ov_ksclient_xdr_KS_DELETEOBJECT_RES(
	XDR					*xdrs,
	KS_DELETEOBJECT_RES	*objp
) {
	if(!ov_ksclient_xdr_KS_RESULT(xdrs, &objp->result)) {
		return FALSE;
	}
	switch(objp->result) {
		case KS_ERR_OK:
			return ov_ksclient_xdr_array(xdrs, (char **)&objp->results_val,
				&objp->results_len, ~0, sizeof(KS_RESULT),
				(xdrproc_t)ov_ksclient_xdr_KS_RESULT);
		default:
			break;
	}
	return TRUE;
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_RENAMEOBJECT_ITEM
*/
KS_BOOL ov_ksclient_xdr_KS_RENAMEOBJECT_ITEM(
	XDR						*xdrs,
	KS_RENAMEOBJECT_ITEM	*objp
) {
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
KS_BOOL ov_ksclient_xdr_KS_RENAMEOBJECT_PAR(
	XDR					*xdrs,
	KS_RENAMEOBJECT_PAR	*objp
) {
	return ov_ksclient_xdr_array(xdrs, (char **)&objp->items_val,
		&objp->items_len, ~0, sizeof(KS_RENAMEOBJECT_ITEM),
		(xdrproc_t)ov_ksclient_xdr_KS_RENAMEOBJECT_ITEM);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_RENAMEOBJECT_RES
*/
KS_BOOL ov_ksclient_xdr_KS_RENAMEOBJECT_RES(
	XDR					*xdrs,
	KS_RENAMEOBJECT_RES	*objp
) {
	if(!ov_ksclient_xdr_KS_RESULT(xdrs, &objp->result)) {
		return FALSE;
	}
	switch(objp->result) {
		case KS_ERR_OK:
			return ov_ksclient_xdr_array(xdrs, (char **)&objp->results_val,
				&objp->results_len, ~0, sizeof(KS_RESULT),
				(xdrproc_t)ov_ksclient_xdr_KS_RESULT);
		default:
			break;
	}
	return TRUE;
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_GETCANONICALPATH_PAR
*/
KS_BOOL ov_ksclient_xdr_KS_GETCANONICALPATH_PAR(
	XDR						*xdrs,
	KS_GETCANONICALPATH_PAR	*objp
) {
	return ov_ksclient_xdr_array(xdrs, (char **)&objp->paths_val, &objp->paths_len, ~0,
		sizeof(KS_STRING), (xdrproc_t)ov_ksclient_xdr_KS_STRING);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_GETCANONICALPATHITEM_RES
*/
KS_BOOL ov_ksclient_xdr_KS_GETCANONICALPATHITEM_RES(
	XDR							*xdrs,
	KS_GETCANONICALPATHITEM_RES	*objp
) {
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
KS_BOOL ov_ksclient_xdr_KS_GETCANONICALPATH_RES(
	XDR						*xdrs,
	KS_GETCANONICALPATH_RES	*objp
) {
	if(!ov_ksclient_xdr_KS_RESULT(xdrs, &objp->result)) {
		return FALSE;
	}
	switch(objp->result) {
		case KS_ERR_OK:
		return ov_ksclient_xdr_array(xdrs, (char **)&objp->results_val,
			&objp->results_len, ~0, sizeof(KS_GETCANONICALPATHITEM_RES),
			(xdrproc_t)ov_ksclient_xdr_KS_GETCANONICALPATHITEM_RES);
		default:
			break;
	}
	return TRUE;
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_LINK_PAR
*/
KS_BOOL ov_ksclient_xdr_KS_LINK_PAR(
	XDR			*xdrs,
	KS_LINK_PAR	*objp
) {
	return ov_ksclient_xdr_array(xdrs, (char **)&objp->items_val, &objp->items_len,
		~0, sizeof(KS_LINK_ITEM), (xdrproc_t)ov_ksclient_xdr_KS_LINK_ITEM);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_LINK_RES
*/
KS_BOOL ov_ksclient_xdr_KS_LINK_RES(
	XDR			*xdrs,
	KS_LINK_RES	*objp
) {
	if(!ov_ksclient_xdr_KS_RESULT(xdrs, &objp->result)) {
		return FALSE;
	}
	switch(objp->result) {
		case KS_ERR_OK:
			return ov_ksclient_xdr_array(xdrs, (char **)&objp->results_val,
				&objp->results_len, ~0, sizeof(KS_RESULT),
				(xdrproc_t)ov_ksclient_xdr_KS_RESULT);
		default:
			break;
	}
	return TRUE;
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_UNLINK_PAR
*/
KS_BOOL ov_ksclient_xdr_KS_UNLINK_PAR(
	XDR				*xdrs,
	KS_UNLINK_PAR	*objp
) {
	return ov_ksclient_xdr_array(xdrs, (char **)&objp->items_val, &objp->items_len,
		~0, sizeof(KS_UNLINK_ITEM), (xdrproc_t)ov_ksclient_xdr_KS_UNLINK_ITEM);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for KS_UNLINK_RES
*/
KS_BOOL ov_ksclient_xdr_KS_UNLINK_RES(
	XDR				*xdrs,
	KS_UNLINK_RES	*objp
) {
	if(!ov_ksclient_xdr_KS_RESULT(xdrs, &objp->result)) {
		return FALSE;
	}
	switch(objp->result) {
		case KS_ERR_OK:
			return ov_ksclient_xdr_array(xdrs, (char **)&objp->results_val,
				&objp->results_len, ~0, sizeof(KS_RESULT),
				(xdrproc_t)ov_ksclient_xdr_KS_RESULT);
		default:
			break;
	}
	return TRUE;
}

/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/

