/*
*   $Id: ov_ksserver_xdr.c,v 1.4 2000-02-10 13:07:04 dirk Exp $
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
*	ov_ksclient_xdr.c too.
*/

#define OV_COMPILE_LIBOVKS

#include "libovks/ov_ksserver_xdr.h"
#include "libov/ov_memstack.h"

#if OV_COMPILER_BORLAND
#pragma warn -pro
#endif

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for bytes
*/
OV_BOOL ov_ksserver_xdr_bytes(
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
	char	*sp = *cpp;
	u_int	_nodesize;
	/*
	*	first deal with the length since XDR bytes are counted
	*/
	switch(xdrs->x_op) {
	case XDR_ENCODE:
		_size = *sizep;
		if(!xdr_u_int(xdrs, &_size)) {
			return FALSE;
		}
		break;
	case XDR_DECODE:
		if(!xdr_u_int(xdrs, &_size)) {
			return FALSE;
		}
		*sizep = _size;
		break;
	default:
		return FALSE;
	}
	_nodesize = _size;
	if(_nodesize>_maxsize) {
		return FALSE;
	}
	/*
	*	now deal with the actual bytes
	*/
	switch(xdrs->x_op) {
	case XDR_DECODE:
		if(!_nodesize) {
			return TRUE;
		}
		/*
		*	if decoding, allocate memory in any case
		*/
		*cpp = sp = (OV_STRING)ov_memstack_alloc(_nodesize);
		if(!sp) {
			return FALSE;
		}
		/* fall into ... */
	case XDR_ENCODE:
		return xdr_opaque(xdrs, sp, _nodesize);
	default:
		break;
	}
	return FALSE;
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for strings
*/
OV_BOOL ov_ksserver_xdr_string(
	XDR			*xdrs,
	OV_STRING	*cpp,
	OV_UINT		maxsize
) {
	/*
	*	local variables
	*/
	OV_STRING	sp = *cpp;
	u_int		_maxsize = maxsize;
	u_int		_size;
	u_int		_nodesize;
	/*
	*	first deal with the length since XDR strings are counted-strings
	*/
	switch(xdrs->x_op) {
	case XDR_ENCODE:
		if(sp) {
			_size = strlen(sp);
		} else {
			_size = 0;
			sp = "";
		}
		break;
	case XDR_FREE:
		return FALSE;
	default:
		break;
	}
	if(!xdr_u_int(xdrs, &_size)) {
		return FALSE;
	}
	if(_size > _maxsize) {
		return FALSE;
	}
	_nodesize = _size + 1;
	/*
	*	now deal with the actual bytes
	*/
	switch(xdrs->x_op) {
	case XDR_DECODE:
		if(!_nodesize) {
			return TRUE;
		}
		/*
		*	if decoding, allocate memory in any case
		*/
		*cpp = sp = (OV_STRING)ov_memstack_alloc(_nodesize);
		if(!sp) {
			return FALSE;
		}
		sp[_size] = 0;
		/* fall into ... */
	case XDR_ENCODE:
		return xdr_opaque(xdrs, sp, _size);
	default:
		break;
	}
	return FALSE;
}

/*	----------------------------------------------------------------------	*/

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
) {
	/*
	*	local variables
	*/
	u_int	_i;
	char	*target = *addrp;
	u_int	_c;
	OV_BOOL	stat = TRUE;
	u_int	_nodesize;
	u_int	_size;
	u_int	_maxsize = maxsize;
	u_int	_elsize = elsize;
	/*
	*	like strings, arrays are really counted arrays
	*/
	switch(xdrs->x_op) {
	case XDR_ENCODE:
		_size = *sizep;
		if(!xdr_u_int(xdrs, &_size)) {
			return FALSE;
		}
		break;
	case XDR_DECODE:
		if(!xdr_u_int(xdrs, &_size)) {
			return FALSE;
		}
		*sizep = _size;
		break;
	default:
		return FALSE;
	}
	_c = _size;
	if(_c>_maxsize) {
		return FALSE;
	}
	_nodesize = _c * _elsize;
	/*
	*	if we are deserializing, we need to allocate an array.
	*/
	if(xdrs->x_op == XDR_DECODE) {
		if(!_c) {
			return TRUE;
		}
		*addrp = target = (char*)ov_memstack_alloc(_nodesize);
		if(!target) {
			return FALSE;
		}
	}
	/*
	*	now we XDR each element of the array
	*/
	for(_i=0; (_i<_c)&&stat; _i++) {
		stat = (*elproc)(xdrs, target, ((u_int)-1));
		target += elsize;
	}
	return stat;
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_TIME
*/
OV_KSSERVER_DECL_XDRFNC(OV_TIME) {
	if(!ov_ksserver_xdr_OV_UINT(xdrs, &objp->secs)) {
		return FALSE;
	}
	return ov_ksserver_xdr_OV_UINT(xdrs, &objp->usecs);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_TIME_SPAN
*/
OV_KSSERVER_DECL_XDRFNC(OV_TIME_SPAN) {
	if(!ov_ksserver_xdr_OV_INT(xdrs, &objp->secs)) {
		return FALSE;
	}
	return ov_ksserver_xdr_OV_INT(xdrs, &objp->usecs);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_STRING
*/
OV_KSSERVER_DECL_XDRFNC(OV_STRING) {
	return ov_ksserver_xdr_string(xdrs, objp, ~0);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_TICKET_PAR
*/
OV_KSSERVER_DECL_XDRFNC(OV_TICKET_PAR) {
	if(!ov_ksserver_xdr_OV_TICKET_TYPE(xdrs, &objp->tickettype)) {
		return FALSE;
	}
	switch(objp->tickettype) {
	case OV_TT_NONE:
		return TRUE;
	case OV_TT_SIMPLE:
		return ov_ksserver_xdr_string(xdrs, &objp->id, KS_SIMPLEID_MAXLEN);
	default:
		break;
	}
	return FALSE;
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_TICKET_RES
*/
OV_KSSERVER_DECL_XDRFNC(OV_TICKET_RES) {
	return ov_ksserver_xdr_OV_TICKET_TYPE(xdrs, &objp->tickettype);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_VAR_VALUE
*/
OV_KSSERVER_DECL_XDRFNC(OV_VAR_VALUE) {
	if(!ov_ksserver_xdr_OV_VAR_TYPE(xdrs, &objp->vartype)) {
		return FALSE;
	}
	switch(objp->vartype) {
	case OV_VT_BOOL:
		return ov_ksserver_xdr_OV_BOOL(xdrs, &objp->valueunion.val_bool);
	case OV_VT_INT:
		return ov_ksserver_xdr_OV_INT(xdrs, &objp->valueunion.val_int);
	case OV_VT_UINT:
		return ov_ksserver_xdr_OV_UINT(xdrs, &objp->valueunion.val_uint);
	case OV_VT_SINGLE:
		return ov_ksserver_xdr_OV_SINGLE(xdrs, &objp->valueunion.val_single);
	case OV_VT_DOUBLE:
		return ov_ksserver_xdr_OV_DOUBLE(xdrs, &objp->valueunion.val_double);
	case OV_VT_STRING:
		return ov_ksserver_xdr_OV_STRING(xdrs, &objp->valueunion.val_string);
	case OV_VT_TIME:
		return ov_ksserver_xdr_OV_TIME(xdrs, &objp->valueunion.val_time);
	case OV_VT_TIME_SPAN:
		return ov_ksserver_xdr_OV_TIME_SPAN(xdrs, &objp->valueunion.val_time_span);
	case OV_VT_BOOL_VEC:
		return ov_ksserver_xdr_array(xdrs, (char **)&objp->valueunion.val_bool_vec.value,
			&objp->valueunion.val_bool_vec.veclen, ~0, sizeof(bool_t),
			(xdrproc_t)ov_ksserver_xdr_OV_BOOL);
	case OV_VT_INT_VEC:
		return ov_ksserver_xdr_array(xdrs, (char **)&objp->valueunion.val_int_vec.value,
			&objp->valueunion.val_int_vec.veclen, ~0, sizeof(long),
			(xdrproc_t)ov_ksserver_xdr_OV_INT);
	case OV_VT_UINT_VEC:
		return ov_ksserver_xdr_array(xdrs, (char **)&objp->valueunion.val_uint_vec.value,
			&objp->valueunion.val_uint_vec.veclen, ~0, sizeof(u_long),
			(xdrproc_t)ov_ksserver_xdr_OV_UINT);
	case OV_VT_SINGLE_VEC:
		return ov_ksserver_xdr_array(xdrs, (char **)&objp->valueunion.val_single_vec.value,
			&objp->valueunion.val_single_vec.veclen, ~0, sizeof(float),
			(xdrproc_t)ov_ksserver_xdr_OV_SINGLE);
	case OV_VT_DOUBLE_VEC:
		return ov_ksserver_xdr_array(xdrs, (char **)&objp->valueunion.val_double_vec.value,
			&objp->valueunion.val_double_vec.veclen, ~0, sizeof(double),
			(xdrproc_t)ov_ksserver_xdr_OV_DOUBLE);
	case OV_VT_STRING_VEC:
		return ov_ksserver_xdr_array(xdrs, (char **)&objp->valueunion.val_string_vec.value,
			&objp->valueunion.val_string_vec.veclen, ~0, sizeof(OV_STRING),
			(xdrproc_t)ov_ksserver_xdr_OV_STRING);
	case OV_VT_TIME_VEC:
		return ov_ksserver_xdr_array(xdrs, (char **)&objp->valueunion.val_time_vec.value,
			&objp->valueunion.val_time_vec.veclen, ~0, sizeof(OV_TIME),
			(xdrproc_t)ov_ksserver_xdr_OV_TIME);
	case OV_VT_TIME_SPAN_VEC:
		return ov_ksserver_xdr_array(xdrs, (char **)&objp->valueunion.val_time_span_vec.value,
			&objp->valueunion.val_time_span_vec.veclen, ~0, sizeof(OV_TIME_SPAN),
			(xdrproc_t)ov_ksserver_xdr_OV_TIME_SPAN);
	case OV_VT_VOID:
		return TRUE;
	case OV_VT_BYTE_VEC:
		return ov_ksserver_xdr_bytes(xdrs, (char **)&objp->valueunion.val_byte_vec.value,
			&objp->valueunion.val_byte_vec.veclen, ~0);
	default:
		break;
	}
	return FALSE;
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_VAR_PROJECTED_PROPS
*/
OV_KSSERVER_DECL_XDRFNC(OV_VAR_PROJECTED_PROPS) {
	if(!ov_ksserver_xdr_string(xdrs, &objp->tech_unit, KS_TECHUNIT_MAXLEN)) {
		return FALSE;
	}
	return ov_ksserver_xdr_OV_VAR_TYPE(xdrs, &objp->vartype);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_VAR_CURRENT_PROPS
*/
OV_KSSERVER_DECL_XDRFNC(OV_VAR_CURRENT_PROPS) {
	if(!ov_ksserver_xdr_OV_VAR_VALUE(xdrs, &objp->value)) {
		return FALSE;
	}
	if(!ov_ksserver_xdr_OV_TIME(xdrs, &objp->time)) {
		return FALSE;
	}
	return ov_ksserver_xdr_OV_STATE(xdrs, &objp->state);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_LINK_PROJECTED_PROPS
*/
OV_KSSERVER_DECL_XDRFNC(OV_LINK_PROJECTED_PROPS) {
	if(!ov_ksserver_xdr_OV_LINK_TYPE(xdrs, &objp->linktype)) {
		return FALSE;
	}
	if(!ov_ksserver_xdr_string(xdrs, &objp->opposite_role_identifier, ~0)) {
		return FALSE;
	}
	return ov_ksserver_xdr_string(xdrs, &objp->association_identifier, ~0);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_OBJ_PROJECTED_PROPS
*/
OV_KSSERVER_DECL_XDRFNC(OV_OBJ_PROJECTED_PROPS) {
	if(!ov_ksserver_xdr_OV_OBJ_TYPE(xdrs, &objp->objtype)) {
		return FALSE;
	}
	switch(objp->objtype) {
	case KS_OT_VARIABLE:
		if(!ov_ksserver_xdr_OV_VAR_PROJECTED_PROPS(xdrs, &objp->OV_OBJ_PROJECTED_PROPS_u.
			var_projected_props)
		) {
			return FALSE;
		}
		break;
	case KS_OT_LINK:
		if(!ov_ksserver_xdr_OV_LINK_PROJECTED_PROPS(xdrs, &objp->OV_OBJ_PROJECTED_PROPS_u.
			link_projected_props)
		) {
			return FALSE;
		}
		break;
	default:
		break;
	}
	if(!ov_ksserver_xdr_string(xdrs, &objp->identifier, KS_NAME_MAXLEN)) {
		return FALSE;
	}
	if(!ov_ksserver_xdr_OV_TIME(xdrs, &objp->creation_time)) {
		return FALSE;
	}
	if(!ov_ksserver_xdr_string(xdrs, &objp->comment, KS_COMMENT_MAXLEN)) {
		return FALSE;
	}
	return ov_ksserver_xdr_OV_ACCESS(xdrs, &objp->access);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_GETPP_PAR
*/
OV_KSSERVER_DECL_XDRFNC(OV_GETPP_PAR) {
	if(!ov_ksserver_xdr_string(xdrs, &objp->path, ~0)) {
		return FALSE;
	}
	if(!ov_ksserver_xdr_OV_OBJ_TYPE(xdrs, &objp->type_mask)) {
		return FALSE;
	}
	return ov_ksserver_xdr_string(xdrs, &objp->name_mask, ~0);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_GETPP_RES
*/
OV_KSSERVER_DECL_XDRFNC(OV_GETPP_RES) {
	/*
	*	local variables
	*/
	u_int					i;
	u_int					items_len = objp->items_len;
	OV_OBJ_PROJECTED_PROPS*	pprops;
	/*
	*	Note, that we do not have an array of items, but a linked list;
	*	this routine may be used for encoding only!
	*/
	if(xdrs->x_op != XDR_ENCODE) {
		return FALSE;
	}
	if(!ov_ksserver_xdr_OV_RESULT(xdrs, &objp->result)) {
		return FALSE;
	}
	switch(objp->result) {
	case OV_ERR_OK:
		/*
		*	serialize the array size first
		*/
		if(!xdr_u_int(xdrs, &items_len)) {
			return FALSE;
		}
		/*
		*	now serialize the array elements
		*/
		pprops = objp->pfirst;
		for(i=0; i<items_len; i++) {
			if(!pprops) {
				return FALSE;
			}
			if(!ov_ksserver_xdr_OV_OBJ_PROJECTED_PROPS(xdrs, pprops)) {
				return FALSE;
			}
			pprops = pprops->pnext;
		}
		break;
	default:
		break;
	}
	return TRUE;
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_GETVAR_ITEM
*/
OV_KSSERVER_DECL_XDRFNC(OV_GETVAR_ITEM) {
	OV_OBJ_TYPE	objtype = KS_OT_VARIABLE;
	if(!ov_ksserver_xdr_OV_RESULT(xdrs, &objp->result)) {
		return FALSE;
	}
	switch(objp->result) {
	case OV_ERR_OK:
		if(!ov_ksserver_xdr_OV_OBJ_TYPE(xdrs, &objtype)) {
			return FALSE;
		}
		if(objtype != KS_OT_VARIABLE) {
			return FALSE;
		}
		return ov_ksserver_xdr_OV_VAR_CURRENT_PROPS(xdrs, &objp->var_current_props);
	default:
		break;
	}
	return TRUE;
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_GETVAR_PAR
*/
OV_KSSERVER_DECL_XDRFNC(OV_GETVAR_PAR) {
	return OV_KSSERVER_XDR_ARRAY(OV_STRING, identifiers);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_GETVAR_RES
*/
OV_KSSERVER_DECL_XDRFNC(OV_GETVAR_RES) {
	if(!ov_ksserver_xdr_OV_RESULT(xdrs, &objp->result)) {
		return FALSE;
	}
	switch(objp->result) {
	case OV_ERR_OK:
		return OV_KSSERVER_XDR_ARRAY(OV_GETVAR_ITEM, items);
	default:
		break;
	}
	return TRUE;
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_SETVAR_ITEM
*/
OV_KSSERVER_DECL_XDRFNC(OV_SETVAR_ITEM) {
	OV_OBJ_TYPE	objtype = KS_OT_VARIABLE;
	if(!ov_ksserver_xdr_string(xdrs, &objp->path_and_name, ~0)) {
		return FALSE;
	}
	if(!ov_ksserver_xdr_OV_OBJ_TYPE(xdrs, &objtype)) {
		return FALSE;
	}
	if(objtype != KS_OT_VARIABLE) {
		return FALSE;
	}
	return ov_ksserver_xdr_OV_VAR_CURRENT_PROPS(xdrs, &objp->var_current_props);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_SETVAR_PAR
*/
OV_KSSERVER_DECL_XDRFNC(OV_SETVAR_PAR) {
	return OV_KSSERVER_XDR_ARRAY(OV_SETVAR_ITEM, items);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_SETVAR_RES
*/
OV_KSSERVER_DECL_XDRFNC(OV_SETVAR_RES) {
	if(!ov_ksserver_xdr_OV_RESULT(xdrs, &objp->result)) {
		return FALSE;
	}
	switch(objp->result) {
	case OV_ERR_OK:
		return OV_KSSERVER_XDR_ARRAY(OV_RESULT, results);
	default:
		break;
	}
	return TRUE;
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_EXGDATA_PAR
*/
OV_KSSERVER_DECL_XDRFNC(OV_EXGDATA_PAR) {
	if(!OV_KSSERVER_XDR_ARRAY(OV_SETVAR_ITEM, set_vars)) {
		return FALSE;
	}
	return OV_KSSERVER_XDR_ARRAY(OV_STRING, get_vars);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_EXGDATA_RES
*/
OV_KSSERVER_DECL_XDRFNC(OV_EXGDATA_RES) {
	if(!ov_ksserver_xdr_OV_RESULT(xdrs, &objp->result)) {
		return FALSE;
	}
	switch(objp->result) {
	case OV_ERR_OK:
		if(!OV_KSSERVER_XDR_ARRAY(OV_RESULT, results)) {
			return FALSE;
		}
		return OV_KSSERVER_XDR_ARRAY(OV_GETVAR_ITEM, items);
	default:
		break;
	}
	return TRUE;
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_DOMAIN_ENGINEERED_PROPS
*/
OV_KSSERVER_DECL_XDRFNC(OV_DOMAIN_ENGINEERED_PROPS) {
	return ov_ksserver_xdr_string(xdrs, &objp->class_identifier, ~0);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_VAR_ENGINEERED_PROPS
*/
OV_KSSERVER_DECL_XDRFNC(OV_VAR_ENGINEERED_PROPS) {
	if(!ov_ksserver_xdr_string(xdrs, &objp->tech_unit, KS_TECHUNIT_MAXLEN)) {
		return FALSE;
	}
	return ov_ksserver_xdr_OV_VAR_TYPE(xdrs, &objp->vartype);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_LINK_ENGINEERED_PROPS
*/
OV_KSSERVER_DECL_XDRFNC(OV_LINK_ENGINEERED_PROPS) {
	if(!ov_ksserver_xdr_OV_LINK_TYPE(xdrs, &objp->linktype)) {
		return FALSE;
	}
	if(!ov_ksserver_xdr_string(xdrs, &objp->opposite_role_identifier, ~0)) {
		return FALSE;
	}
	return ov_ksserver_xdr_string(xdrs, &objp->association_identifier, ~0);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_OBJ_ENGINEERED_PROPS
*/
OV_KSSERVER_DECL_XDRFNC(OV_OBJ_ENGINEERED_PROPS) {
	if(!ov_ksserver_xdr_OV_OBJ_TYPE(xdrs, &objp->objtype)) {
		return FALSE;
	}
	switch(objp->objtype) {
	case KS_OT_DOMAIN:
		if(!ov_ksserver_xdr_OV_DOMAIN_ENGINEERED_PROPS(xdrs, &objp->OV_OBJ_ENGINEERED_PROPS_u.
			domain_engineered_props)
		) {
			return FALSE;
		}
		break;
	case KS_OT_VARIABLE:
		if(!ov_ksserver_xdr_OV_VAR_ENGINEERED_PROPS(xdrs, &objp->OV_OBJ_ENGINEERED_PROPS_u.
			var_engineered_props)
		) {
			return FALSE;
		}
		break;
	case KS_OT_LINK:
		if(!ov_ksserver_xdr_OV_LINK_ENGINEERED_PROPS(xdrs, &objp->OV_OBJ_ENGINEERED_PROPS_u.
			link_engineered_props)
		) {
			return FALSE;
		}
		break;
	default:
		break;
	}
	if(!ov_ksserver_xdr_string(xdrs, &objp->identifier, KS_NAME_MAXLEN)) {
		return FALSE;
	}
	if(!ov_ksserver_xdr_OV_TIME(xdrs, &objp->creation_time)) {
		return FALSE;
	}
	if(!ov_ksserver_xdr_string(xdrs, &objp->comment, KS_COMMENT_MAXLEN)) {
		return FALSE;
	}
	if(!ov_ksserver_xdr_OV_ACCESS(xdrs, &objp->access)) {
		return FALSE;
	}
	return ov_ksserver_xdr_OV_SEMANTIC_FLAGS(xdrs, &objp->semantic_flags);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_GETEP_PAR
*/
OV_KSSERVER_DECL_XDRFNC(OV_GETEP_PAR) {
	if(!ov_ksserver_xdr_string(xdrs, &objp->path, ~0)) {
		return FALSE;
	}
	if(!ov_ksserver_xdr_OV_OBJ_TYPE(xdrs, &objp->type_mask)) {
		return FALSE;
	}
	if(!ov_ksserver_xdr_string(xdrs, &objp->name_mask, ~0)) {
		return FALSE;
	}
	return ov_ksserver_xdr_OV_EP_FLAGS(xdrs, &objp->scope_flags);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_GETEP_RES
*/
OV_KSSERVER_DECL_XDRFNC(OV_GETEP_RES) {
	/*
	*	local variables
	*/
	u_int						i;
	u_int						items_len = objp->items_len;
	OV_OBJ_ENGINEERED_PROPS*	pprops;
	/*
	*	Note, that we do not have an array of items, but a linked list;
	*	this routine may be used for encoding only!
	*/
	if(xdrs->x_op != XDR_ENCODE) {
		return FALSE;
	}
	if(!ov_ksserver_xdr_OV_RESULT(xdrs, &objp->result)) {
		return FALSE;
	}
	switch(objp->result) {
	case OV_ERR_OK:
		/*
		*	serialize the array size first
		*/
		if(!xdr_u_int(xdrs, &items_len)) {
			return FALSE;
		}
		/*
		*	now serialize the array elements
		*/
		pprops = objp->pfirst;
		for(i=0; i<items_len; i++) {
			if(!pprops) {
				return FALSE;
			}
			if(!ov_ksserver_xdr_OV_OBJ_ENGINEERED_PROPS(xdrs, pprops)) {
				return FALSE;
			}
			pprops = pprops->pnext;
		}
		break;
	default:
		break;
	}
	return TRUE;
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_PLACEMENT
*/
OV_KSSERVER_DECL_XDRFNC(OV_PLACEMENT) {
	if(!ov_ksserver_xdr_OV_PLACEMENT_HINT(xdrs, &objp->hint)) {
		return FALSE;
	}
	switch(objp->hint) {
	case OV_PMH_BEFORE:
	case OV_PMH_AFTER:
		return ov_ksserver_xdr_string(xdrs, &objp->place_path, ~0);
	default:
		break;
	}
	return TRUE;
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_LINK_ITEM
*/
OV_KSSERVER_DECL_XDRFNC(OV_LINK_ITEM) {
	if(!ov_ksserver_xdr_string(xdrs, &objp->link_path, ~0)) {
		return FALSE;
	}
	if(!ov_ksserver_xdr_string(xdrs, &objp->element_path, ~0)) {
		return FALSE;
	}
	if(!ov_ksserver_xdr_OV_PLACEMENT(xdrs, &objp->place)) {
		return FALSE;
	}
	return ov_ksserver_xdr_OV_PLACEMENT(xdrs, &objp->opposite_place);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_UNLINK_ITEM
*/
OV_KSSERVER_DECL_XDRFNC(OV_UNLINK_ITEM) {
	if(!ov_ksserver_xdr_string(xdrs, &objp->link_path, ~0)) {
		return FALSE;
	}
	return ov_ksserver_xdr_string(xdrs, &objp->element_path, ~0);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_CREATEOBJ_ITEM
*/
OV_KSSERVER_DECL_XDRFNC(OV_CREATEOBJ_ITEM) {
	if(!ov_ksserver_xdr_string(xdrs, &objp->factory_path, ~0)) {
		return FALSE;
	}
	if(!ov_ksserver_xdr_string(xdrs, &objp->new_path, ~0)) {
		return FALSE;
	}
	if(!ov_ksserver_xdr_OV_PLACEMENT(xdrs, &objp->place)) {
		return FALSE;
	}
	if(!OV_KSSERVER_XDR_ARRAY(OV_SETVAR_ITEM, parameters)) {
		return FALSE;
	}
	return OV_KSSERVER_XDR_ARRAY(OV_LINK_ITEM, links);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_CREATEOBJECT_PAR
*/
OV_KSSERVER_DECL_XDRFNC(OV_CREATEOBJECT_PAR) {
	return OV_KSSERVER_XDR_ARRAY(OV_CREATEOBJ_ITEM, items);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_CREATEOBJECTITEM_RES
*/
OV_KSSERVER_DECL_XDRFNC(OV_CREATEOBJECTITEM_RES) {
	if(!ov_ksserver_xdr_OV_RESULT(xdrs, &objp->result)) {
		return FALSE;
	}
	switch(objp->result) {
	case OV_ERR_BADINITPARAM:
		if(!OV_KSSERVER_XDR_ARRAY(OV_RESULT, params_results)) {
			return FALSE;
		}
		return OV_KSSERVER_XDR_ARRAY(OV_RESULT, link_results);
	default:
		break;
	}
	return TRUE;
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_CREATEOBJECT_RES
*/
OV_KSSERVER_DECL_XDRFNC(OV_CREATEOBJECT_RES) {
	if(!ov_ksserver_xdr_OV_RESULT(xdrs, &objp->result)) {
		return FALSE;
	}
	switch(objp->result) {
	case OV_ERR_OK:
		return OV_KSSERVER_XDR_ARRAY(OV_CREATEOBJECTITEM_RES, obj_results);
	default:
		break;
	}
	return TRUE;
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_DELETEOBJECT_PAR
*/
OV_KSSERVER_DECL_XDRFNC(OV_DELETEOBJECT_PAR) {
	return OV_KSSERVER_XDR_ARRAY(OV_STRING, paths);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_DELETEOBJECT_RES
*/
OV_KSSERVER_DECL_XDRFNC(OV_DELETEOBJECT_RES) {
	if(!ov_ksserver_xdr_OV_RESULT(xdrs, &objp->result)) {
		return FALSE;
	}
	switch(objp->result) {
	case OV_ERR_OK:
		return OV_KSSERVER_XDR_ARRAY(OV_RESULT, results);
	default:
		break;
	}
	return TRUE;
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_RENAMEOBJECT_ITEM
*/
OV_KSSERVER_DECL_XDRFNC(OV_RENAMEOBJECT_ITEM) {
	if(!ov_ksserver_xdr_string(xdrs, &objp->old_path, ~0)) {
		return FALSE;
	}
	if(!ov_ksserver_xdr_string(xdrs, &objp->new_path, ~0)) {
		return FALSE;
	}
	return ov_ksserver_xdr_OV_PLACEMENT(xdrs, &objp->place);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_RENAMEOBJECT_PAR
*/
OV_KSSERVER_DECL_XDRFNC(OV_RENAMEOBJECT_PAR) {
	return OV_KSSERVER_XDR_ARRAY(OV_RENAMEOBJECT_ITEM, items);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_RENAMEOBJECT_RES
*/
OV_KSSERVER_DECL_XDRFNC(OV_RENAMEOBJECT_RES) {
	if(!ov_ksserver_xdr_OV_RESULT(xdrs, &objp->result)) {
		return FALSE;
	}
	switch(objp->result) {
	case OV_ERR_OK:
		return OV_KSSERVER_XDR_ARRAY(OV_RESULT, results);
	default:
		break;
	}
	return TRUE;
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_GETCANONICALPATH_PAR
*/
OV_KSSERVER_DECL_XDRFNC(OV_GETCANONICALPATH_PAR) {
	return OV_KSSERVER_XDR_ARRAY(OV_STRING, paths);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_GETCANONICALPATHITEM_RES
*/
OV_KSSERVER_DECL_XDRFNC(OV_GETCANONICALPATHITEM_RES) {
	if(!ov_ksserver_xdr_OV_RESULT(xdrs, &objp->result)) {
		return FALSE;
	}
	switch(objp->result) {
	case OV_ERR_OK:
		return ov_ksserver_xdr_string(xdrs, &objp->canonical_path, ~0);
	default:
		break;
	}
	return TRUE;
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_GETCANONICALPATH_RES
*/
OV_KSSERVER_DECL_XDRFNC(OV_GETCANONICALPATH_RES) {
	if(!ov_ksserver_xdr_OV_RESULT(xdrs, &objp->result)) {
		return FALSE;
	}
	switch(objp->result) {
	case OV_ERR_OK:
		return OV_KSSERVER_XDR_ARRAY(OV_GETCANONICALPATHITEM_RES, results);
	default:
		break;
	}
	return TRUE;
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_LINK_PAR
*/
OV_KSSERVER_DECL_XDRFNC(OV_LINK_PAR) {
	return OV_KSSERVER_XDR_ARRAY(OV_LINK_ITEM, items);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_LINK_RES
*/
OV_KSSERVER_DECL_XDRFNC(OV_LINK_RES) {
	if(!ov_ksserver_xdr_OV_RESULT(xdrs, &objp->result)) {
		return FALSE;
	}
	switch(objp->result) {
	case OV_ERR_OK:
		return OV_KSSERVER_XDR_ARRAY(OV_RESULT, results);
	default:
		break;
	}
	return TRUE;
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_UNLINK_PAR
*/
OV_KSSERVER_DECL_XDRFNC(OV_UNLINK_PAR) {
	return OV_KSSERVER_XDR_ARRAY(OV_UNLINK_ITEM, items);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_UNLINK_RES
*/
OV_KSSERVER_DECL_XDRFNC(OV_UNLINK_RES) {
	if(!ov_ksserver_xdr_OV_RESULT(xdrs, &objp->result)) {
		return FALSE;
	}
	switch(objp->result) {
	case OV_ERR_OK:
		return OV_KSSERVER_XDR_ARRAY(OV_RESULT, results);
	default:
		break;
	}
	return TRUE;
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_ABSRELTIME
*/
OV_KSSERVER_DECL_XDRFNC(OV_ABSRELTIME) {
	if(!ov_ksserver_xdr_OV_TIME_TYPE(xdrs, &objp->timetype)) {
		return FALSE;
	}
	switch(objp->timetype) {
	case OV_TT_ABSOLUTE:
		return ov_ksserver_xdr_OV_TIME(xdrs,
			&objp->OV_ABSRELTIME_u.abstime);
	case OV_TT_RELATIVE:
		return ov_ksserver_xdr_OV_TIME_SPAN(xdrs,
			&objp->OV_ABSRELTIME_u.reltime);
	default:
		break;
	}
	return FALSE;
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_TIMEHISTSELECTOR
*/
OV_KSSERVER_DECL_XDRFNC(OV_TIMEHISTSELECTOR) {
	if(!ov_ksserver_xdr_OV_INTERPOLATION_MODE(xdrs, &objp->ip_mode)) {
		return FALSE;
	}
	if(!ov_ksserver_xdr_OV_ABSRELTIME(xdrs, &objp->from)) {
		return FALSE;
	}
	if(!ov_ksserver_xdr_OV_ABSRELTIME(xdrs, &objp->to)) {
		return FALSE;
	}
	return ov_ksserver_xdr_OV_TIME_SPAN(xdrs, &objp->delta);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_STRINGHISTSELECTOR
*/
OV_KSSERVER_DECL_XDRFNC(OV_STRINGHISTSELECTOR) {
	return ov_ksserver_xdr_OV_STRING(xdrs, &objp->mask);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_HISTSELECTOR
*/
OV_KSSERVER_DECL_XDRFNC(OV_HISTSELECTOR) {
	if(!ov_ksserver_xdr_OV_HSEL_TYPE(xdrs, &objp->hseltype)) {
		return FALSE;
	}
	switch(objp->hseltype) {
	case OV_HSELT_NONE:
		return TRUE;
	case OV_HSELT_TIME:
		return ov_ksserver_xdr_OV_TIMEHISTSELECTOR(xdrs,
			&objp->OV_HISTSELECTOR_u.ths);
	case OV_HSELT_STRING:
		return ov_ksserver_xdr_OV_STRINGHISTSELECTOR(xdrs,
			&objp->OV_HISTSELECTOR_u.shs);
	default:
		break;
	}
	return FALSE;
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_GETHIST_ITEM
*/
OV_KSSERVER_DECL_XDRFNC(OV_GETHIST_ITEM) {
	if(!ov_ksserver_xdr_OV_STRING(xdrs, &objp->part)) {
		return FALSE;
	}
	return ov_ksserver_xdr_OV_HISTSELECTOR(xdrs, &objp->selector);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_GETHISTRESULT_ITEM
*/
OV_KSSERVER_DECL_XDRFNC(OV_GETHISTRESULT_ITEM) {
	if(!ov_ksserver_xdr_OV_RESULT(xdrs, &objp->result)) {
		return FALSE;
	}
	switch(objp->result) {
	case OV_ERR_OK:
		return ov_ksserver_xdr_OV_VAR_VALUE(xdrs, &objp->value);
	default:
		break;
	}
	return TRUE;
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_GETHISTSINGLERESULT
*/
OV_KSSERVER_DECL_XDRFNC(OV_GETHISTSINGLERESULT) {
	if(!ov_ksserver_xdr_OV_RESULT(xdrs, &objp->result)) {
		return FALSE;
	}
	switch(objp->result) {
	case OV_ERR_OK:
		return OV_KSSERVER_XDR_ARRAY(OV_GETHISTRESULT_ITEM, items);
	default:
		break;
	}
	return TRUE;
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_GETHIST_PAR
*/
OV_KSSERVER_DECL_XDRFNC(OV_GETHIST_PAR) {
	if(!OV_KSSERVER_XDR_ARRAY(OV_STRING, paths)) {
		return FALSE;
	}
	if(!ov_ksserver_xdr_OV_UINT(xdrs, &objp->max_answers)) {
		return FALSE;
	}
	return OV_KSSERVER_XDR_ARRAY(OV_GETHIST_ITEM, items);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_GETHIST_RES
*/
OV_KSSERVER_DECL_XDRFNC(OV_GETHIST_RES) {
	if(!ov_ksserver_xdr_OV_RESULT(xdrs, &objp->result)) {
		return FALSE;
	}
	switch(objp->result) {
	case OV_ERR_OK:
		return OV_KSSERVER_XDR_ARRAY(OV_GETHISTSINGLERESULT, results);
	default:
		break;
	}
	return TRUE;
}

/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/

