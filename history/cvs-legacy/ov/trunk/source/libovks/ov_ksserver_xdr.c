/*
*   $Id: ov_ksserver_xdr.c,v 1.1 1999-07-19 15:02:16 dirk Exp $
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
OV_BOOL ov_ksserver_xdr_OV_TIME(
	XDR		*xdrs,
	OV_TIME	*objp
) {
	if(!ov_ksserver_xdr_OV_UINT(xdrs, &objp->secs)) {
		return FALSE;
	}
	return ov_ksserver_xdr_OV_UINT(xdrs, &objp->usecs);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_TIME_SPAN
*/
OV_BOOL ov_ksserver_xdr_OV_TIME_SPAN(
	XDR				*xdrs,
	OV_TIME_SPAN	*objp
) {
	if(!ov_ksserver_xdr_OV_INT(xdrs, &objp->secs)) {
		return FALSE;
	}
	return ov_ksserver_xdr_OV_INT(xdrs, &objp->usecs);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_STRING
*/
OV_BOOL ov_ksserver_xdr_OV_STRING(
	XDR			*xdrs,
	OV_STRING	*objp
) {
	return ov_ksserver_xdr_string(xdrs, objp, ~0);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_TICKET_PAR
*/
OV_BOOL ov_ksserver_xdr_OV_TICKET_PAR(
	XDR			*xdrs,
	OV_TICKET	*objp
) {
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
OV_BOOL ov_ksserver_xdr_OV_TICKET_RES(
	XDR			*xdrs,
	OV_TICKET	*objp
) {
	return ov_ksserver_xdr_OV_TICKET_TYPE(xdrs, &objp->tickettype);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_VAR_VALUE
*/
OV_BOOL ov_ksserver_xdr_OV_VAR_VALUE(
	XDR				*xdrs,
	OV_VAR_VALUE	*objp
) {
	if(!ov_ksserver_xdr_OV_VAR_TYPE(xdrs, &objp->vartype)) {
		return FALSE;
	}
	if(xdrs->x_op == XDR_DECODE) {
		objp->veclen = 1;
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
			return ov_ksserver_xdr_array(xdrs, (char **)&objp->valueunion.val_bool_vec,
				&objp->veclen, ~0, sizeof(bool_t), (xdrproc_t)ov_ksserver_xdr_OV_BOOL);
		case OV_VT_INT_VEC:
			return ov_ksserver_xdr_array(xdrs, (char **)&objp->valueunion.val_int_vec,
				&objp->veclen, ~0, sizeof(long), (xdrproc_t)ov_ksserver_xdr_OV_INT);
		case OV_VT_UINT_VEC:
			return ov_ksserver_xdr_array(xdrs, (char **)&objp->valueunion.val_uint_vec,
				&objp->veclen, ~0, sizeof(u_long), (xdrproc_t)ov_ksserver_xdr_OV_UINT);
		case OV_VT_SINGLE_VEC:
			return ov_ksserver_xdr_array(xdrs, (char **)&objp->valueunion.val_single_vec,
				&objp->veclen, ~0, sizeof(float), (xdrproc_t)ov_ksserver_xdr_OV_SINGLE);
		case OV_VT_DOUBLE_VEC:
			return ov_ksserver_xdr_array(xdrs, (char **)&objp->valueunion.val_double_vec,
				&objp->veclen, ~0, sizeof(double), (xdrproc_t)ov_ksserver_xdr_OV_DOUBLE);
		case OV_VT_STRING_VEC:
			return ov_ksserver_xdr_array(xdrs, (char **)&objp->valueunion.val_string_vec,
				&objp->veclen, ~0, sizeof(OV_STRING), (xdrproc_t)ov_ksserver_xdr_OV_STRING);
		case OV_VT_TIME_VEC:
			return ov_ksserver_xdr_array(xdrs, (char **)&objp->valueunion.val_time_vec,
				&objp->veclen, ~0, sizeof(OV_TIME), (xdrproc_t)ov_ksserver_xdr_OV_TIME);
		case OV_VT_TIME_SPAN_VEC:
			return ov_ksserver_xdr_array(xdrs, (char **)&objp->valueunion.val_time_span_vec,
				&objp->veclen, ~0, sizeof(OV_TIME_SPAN), (xdrproc_t)ov_ksserver_xdr_OV_TIME_SPAN);
		case OV_VT_VOID:
			return TRUE;
		case OV_VT_BYTE_VEC:
			return ov_ksserver_xdr_bytes(xdrs, (char **)&objp->valueunion.val_byte_vec,
				&objp->veclen, ~0);
		default:
			break;
	}
	return FALSE;
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_VAR_PROJECTED_PROPS
*/
OV_BOOL ov_ksserver_xdr_OV_VAR_PROJECTED_PROPS(
	XDR						*xdrs,
	OV_VAR_PROJECTED_PROPS	*objp
) {
	if(!ov_ksserver_xdr_string(xdrs, &objp->tech_unit, KS_TECHUNIT_MAXLEN)) {
		return FALSE;
	}
	return ov_ksserver_xdr_OV_VAR_TYPE(xdrs, &objp->vartype);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_VAR_CURRENT_PROPS
*/
OV_BOOL ov_ksserver_xdr_OV_VAR_CURRENT_PROPS(
	XDR						*xdrs,
	OV_VAR_CURRENT_PROPS	*objp
) {
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
OV_BOOL ov_ksserver_xdr_OV_LINK_PROJECTED_PROPS(
	XDR									*xdrs,
	OV_LINK_PROJECTED_PROPS	*objp
) {
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
OV_BOOL ov_ksserver_xdr_OV_OBJ_PROJECTED_PROPS(
	XDR						*xdrs,
	OV_OBJ_PROJECTED_PROPS	*objp
) {
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
OV_BOOL ov_ksserver_xdr_OV_GETPP_PAR(
	XDR				*xdrs,
	OV_GETPP_PAR	*objp
) {
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
OV_BOOL ov_ksserver_xdr_OV_GETPP_RES(
	XDR				*xdrs,
	OV_GETPP_RES	*objp
) {
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
OV_BOOL ov_ksserver_xdr_OV_GETVAR_ITEM(
	XDR				*xdrs,
	OV_GETVAR_ITEM	*objp
) {
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
OV_BOOL ov_ksserver_xdr_OV_GETVAR_PAR(
	XDR				*xdrs,
	OV_GETVAR_PAR	*objp
) {
	return ov_ksserver_xdr_array(xdrs, (char **)&objp->identifiers_val,
		&objp->identifiers_len, ~0, sizeof(OV_STRING), (xdrproc_t)ov_ksserver_xdr_OV_STRING);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_GETVAR_RES
*/
OV_BOOL ov_ksserver_xdr_OV_GETVAR_RES(
	XDR				*xdrs,
	OV_GETVAR_RES	*objp
) {
	if(!ov_ksserver_xdr_OV_RESULT(xdrs, &objp->result)) {
		return FALSE;
	}
	switch(objp->result) {
		case OV_ERR_OK:
			return ov_ksserver_xdr_array(xdrs, (char **)&objp->items_val, &objp->items_len,
				~0, sizeof(OV_GETVAR_ITEM), (xdrproc_t)ov_ksserver_xdr_OV_GETVAR_ITEM);
		default:
			break;
	}
	return TRUE;
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_SETVAR_ITEM
*/
OV_BOOL ov_ksserver_xdr_OV_SETVAR_ITEM(
	XDR				*xdrs,
	OV_SETVAR_ITEM	*objp
) {
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
OV_BOOL ov_ksserver_xdr_OV_SETVAR_PAR(
	XDR				*xdrs,
	OV_SETVAR_PAR	*objp
) {
	return ov_ksserver_xdr_array(xdrs, (char **)&objp->items_val, &objp->items_len, ~0,
		sizeof(OV_SETVAR_ITEM), (xdrproc_t)ov_ksserver_xdr_OV_SETVAR_ITEM);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_SETVAR_RES
*/
OV_BOOL ov_ksserver_xdr_OV_SETVAR_RES(
	XDR				*xdrs,
	OV_SETVAR_RES	*objp
) {
	if(!ov_ksserver_xdr_OV_RESULT(xdrs, &objp->result)) {
		return FALSE;
	}
	switch(objp->result) {
		case OV_ERR_OK:
			return ov_ksserver_xdr_array(xdrs, (char **)&objp->results_val, &objp->results_len,
				~0, sizeof(OV_RESULT), (xdrproc_t)ov_ksserver_xdr_OV_RESULT);
		default:
			break;
	}
	return TRUE;
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_EXGDATA_PAR
*/
OV_BOOL ov_ksserver_xdr_OV_EXGDATA_PAR(
	XDR				*xdrs,
	OV_EXGDATA_PAR	*objp
) {
	if(!ov_ksserver_xdr_array(xdrs, (char **)&objp->set_vars_val, &objp->set_vars_len,
		~0, sizeof(OV_SETVAR_ITEM), (xdrproc_t)ov_ksserver_xdr_OV_SETVAR_ITEM)
	) {
		return FALSE;
	}
	return ov_ksserver_xdr_array(xdrs, (char **)&objp->get_vars_val, &objp->get_vars_len,
		~0, sizeof(OV_STRING), (xdrproc_t)ov_ksserver_xdr_OV_STRING);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_EXGDATA_RES
*/
OV_BOOL ov_ksserver_xdr_OV_EXGDATA_RES(
	XDR				*xdrs,
	OV_EXGDATA_RES	*objp
) {
	if(!ov_ksserver_xdr_OV_RESULT(xdrs, &objp->result)) {
		return FALSE;
	}
	switch(objp->result) {
		case OV_ERR_OK:
			if(!ov_ksserver_xdr_array(xdrs, (char **)&objp->results_val, &objp->results_len,
				~0, sizeof(OV_RESULT), (xdrproc_t)ov_ksserver_xdr_OV_RESULT)
			) {
				return FALSE;
			}
			return ov_ksserver_xdr_array(xdrs, (char **)&objp->items_val, &objp->items_len,
				~0, sizeof(OV_GETVAR_ITEM), (xdrproc_t)ov_ksserver_xdr_OV_GETVAR_ITEM);
		default:
			break;
	}
	return TRUE;
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_DOMAIN_ENGINEERED_PROPS
*/
OV_BOOL ov_ksserver_xdr_OV_DOMAIN_ENGINEERED_PROPS(
	XDR							*xdrs,
	OV_DOMAIN_ENGINEERED_PROPS	*objp
) {
	return ov_ksserver_xdr_string(xdrs, &objp->class_identifier, ~0);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_VAR_ENGINEERED_PROPS
*/
OV_BOOL ov_ksserver_xdr_OV_VAR_ENGINEERED_PROPS(
	XDR						*xdrs,
	OV_VAR_ENGINEERED_PROPS	*objp
) {
	if(!ov_ksserver_xdr_string(xdrs, &objp->tech_unit, KS_TECHUNIT_MAXLEN)) {
		return FALSE;
	}
	return ov_ksserver_xdr_OV_VAR_TYPE(xdrs, &objp->vartype);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_LINK_ENGINEERED_PROPS
*/
OV_BOOL ov_ksserver_xdr_OV_LINK_ENGINEERED_PROPS(
	XDR							*xdrs,
	OV_LINK_ENGINEERED_PROPS	*objp
) {
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
OV_BOOL ov_ksserver_xdr_OV_OBJ_ENGINEERED_PROPS(
	XDR						*xdrs,
	OV_OBJ_ENGINEERED_PROPS	*objp
) {
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
OV_BOOL ov_ksserver_xdr_OV_GETEP_PAR(
	XDR				*xdrs,
	OV_GETEP_PAR	*objp
) {
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
OV_BOOL ov_ksserver_xdr_OV_GETEP_RES(
	XDR				*xdrs,
	OV_GETEP_RES	*objp
) {
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
OV_BOOL ov_ksserver_xdr_OV_PLACEMENT(
	XDR				*xdrs,
	OV_PLACEMENT	*objp
) {
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
OV_BOOL ov_ksserver_xdr_OV_LINK_ITEM(
	XDR				*xdrs,
	OV_LINK_ITEM	*objp
) {
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
OV_BOOL ov_ksserver_xdr_OV_UNLINK_ITEM(
	XDR				*xdrs,
	OV_UNLINK_ITEM	*objp
) {
	if(!ov_ksserver_xdr_string(xdrs, &objp->link_path, ~0)) {
		return FALSE;
	}
	return ov_ksserver_xdr_string(xdrs, &objp->element_path, ~0);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_CREATEOBJ_ITEM
*/
OV_BOOL ov_ksserver_xdr_OV_CREATEOBJ_ITEM(
	XDR					*xdrs,
	OV_CREATEOBJ_ITEM	*objp
) {
	if(!ov_ksserver_xdr_string(xdrs, &objp->factory_path, ~0)) {
		return FALSE;
	}
	if(!ov_ksserver_xdr_string(xdrs, &objp->new_path, ~0)) {
		return FALSE;
	}
	if(!ov_ksserver_xdr_OV_PLACEMENT(xdrs, &objp->place)) {
		return FALSE;
	}
	if(!ov_ksserver_xdr_array(xdrs, (char **)&objp->parameters_val, &objp->parameters_len,
		~0, sizeof(OV_SETVAR_ITEM), (xdrproc_t)ov_ksserver_xdr_OV_SETVAR_ITEM)
	) {
		return FALSE;
	}
	return ov_ksserver_xdr_array(xdrs, (char **)&objp->links_val, &objp->links_len,
		~0, sizeof(OV_LINK_ITEM), (xdrproc_t)ov_ksserver_xdr_OV_LINK_ITEM);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_CREATEOBJECT_PAR
*/
OV_BOOL ov_ksserver_xdr_OV_CREATEOBJECT_PAR(
	XDR					*xdrs,
	OV_CREATEOBJECT_PAR	*objp
) {
	return ov_ksserver_xdr_array(xdrs, (char **)&objp->items_val, &objp->items_len,
		~0, sizeof(OV_CREATEOBJ_ITEM), (xdrproc_t)ov_ksserver_xdr_OV_CREATEOBJ_ITEM);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_CREATEOBJECTITEM_RES
*/
OV_BOOL ov_ksserver_xdr_OV_CREATEOBJECTITEM_RES(
	XDR						*xdrs,
	OV_CREATEOBJECTITEM_RES	*objp
) {
	if(!ov_ksserver_xdr_OV_RESULT(xdrs, &objp->result)) {
		return FALSE;
	}
	switch(objp->result) {
		case OV_ERR_BADINITPARAM:
			if(!ov_ksserver_xdr_array(xdrs, (char **)&objp->params_results_val,
				&objp->params_results_len, ~0, sizeof(OV_RESULT),
				(xdrproc_t)ov_ksserver_xdr_OV_RESULT)
			) {
				return FALSE;
			}
			return ov_ksserver_xdr_array(xdrs, (char **)&objp->link_results_val,
				&objp->link_results_len, ~0, sizeof(OV_RESULT),
				(xdrproc_t)ov_ksserver_xdr_OV_RESULT);
		default:
			break;
	}
	return TRUE;
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_CREATEOBJECT_RES
*/
OV_BOOL ov_ksserver_xdr_OV_CREATEOBJECT_RES(
	XDR					*xdrs,
	OV_CREATEOBJECT_RES	*objp
) {
	if(!ov_ksserver_xdr_OV_RESULT(xdrs, &objp->result)) {
		return FALSE;
	}
	switch(objp->result) {
		case OV_ERR_OK:
			return ov_ksserver_xdr_array(xdrs, (char **)&objp->obj_results_val,
				&objp->obj_results_len, ~0, sizeof(OV_CREATEOBJECTITEM_RES),
				(xdrproc_t)ov_ksserver_xdr_OV_CREATEOBJECTITEM_RES);
		default:
			break;
	}
	return TRUE;
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_DELETEOBJECT_PAR
*/
OV_BOOL ov_ksserver_xdr_OV_DELETEOBJECT_PAR(
	XDR					*xdrs,
	OV_DELETEOBJECT_PAR	*objp
) {
	return ov_ksserver_xdr_array(xdrs, (char **)&objp->paths_val, &objp->paths_len,
		~0, sizeof(OV_STRING), (xdrproc_t)ov_ksserver_xdr_OV_STRING);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_DELETEOBJECT_RES
*/
OV_BOOL ov_ksserver_xdr_OV_DELETEOBJECT_RES(
	XDR					*xdrs,
	OV_DELETEOBJECT_RES	*objp
) {
	if(!ov_ksserver_xdr_OV_RESULT(xdrs, &objp->result)) {
		return FALSE;
	}
	switch(objp->result) {
		case OV_ERR_OK:
			return ov_ksserver_xdr_array(xdrs, (char **)&objp->results_val,
				&objp->results_len, ~0, sizeof(OV_RESULT),
				(xdrproc_t)ov_ksserver_xdr_OV_RESULT);
		default:
			break;
	}
	return TRUE;
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_RENAMEOBJECT_ITEM
*/
OV_BOOL ov_ksserver_xdr_OV_RENAMEOBJECT_ITEM(
	XDR						*xdrs,
	OV_RENAMEOBJECT_ITEM	*objp
) {
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
OV_BOOL ov_ksserver_xdr_OV_RENAMEOBJECT_PAR(
	XDR					*xdrs,
	OV_RENAMEOBJECT_PAR	*objp
) {
	return ov_ksserver_xdr_array(xdrs, (char **)&objp->items_val,
		&objp->items_len, ~0, sizeof(OV_RENAMEOBJECT_ITEM),
		(xdrproc_t)ov_ksserver_xdr_OV_RENAMEOBJECT_ITEM);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_RENAMEOBJECT_RES
*/
OV_BOOL ov_ksserver_xdr_OV_RENAMEOBJECT_RES(
	XDR					*xdrs,
	OV_RENAMEOBJECT_RES	*objp
) {
	if(!ov_ksserver_xdr_OV_RESULT(xdrs, &objp->result)) {
		return FALSE;
	}
	switch(objp->result) {
		case OV_ERR_OK:
			return ov_ksserver_xdr_array(xdrs, (char **)&objp->results_val,
				&objp->results_len, ~0, sizeof(OV_RESULT),
				(xdrproc_t)ov_ksserver_xdr_OV_RESULT);
		default:
			break;
	}
	return TRUE;
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_GETCANONICALPATH_PAR
*/
OV_BOOL ov_ksserver_xdr_OV_GETCANONICALPATH_PAR(
	XDR						*xdrs,
	OV_GETCANONICALPATH_PAR	*objp
) {
	return ov_ksserver_xdr_array(xdrs, (char **)&objp->paths_val, &objp->paths_len, ~0,
		sizeof(OV_STRING), (xdrproc_t)ov_ksserver_xdr_OV_STRING);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_GETCANONICALPATHITEM_RES
*/
OV_BOOL ov_ksserver_xdr_OV_GETCANONICALPATHITEM_RES(
	XDR							*xdrs,
	OV_GETCANONICALPATHITEM_RES	*objp
) {
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
OV_BOOL ov_ksserver_xdr_OV_GETCANONICALPATH_RES(
	XDR						*xdrs,
	OV_GETCANONICALPATH_RES	*objp
) {
	if(!ov_ksserver_xdr_OV_RESULT(xdrs, &objp->result)) {
		return FALSE;
	}
	switch(objp->result) {
		case OV_ERR_OK:
		return ov_ksserver_xdr_array(xdrs, (char **)&objp->results_val,
			&objp->results_len, ~0, sizeof(OV_GETCANONICALPATHITEM_RES),
			(xdrproc_t)ov_ksserver_xdr_OV_GETCANONICALPATHITEM_RES);
		default:
			break;
	}
	return TRUE;
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_LINK_PAR
*/
OV_BOOL ov_ksserver_xdr_OV_LINK_PAR(
	XDR			*xdrs,
	OV_LINK_PAR	*objp
) {
	return ov_ksserver_xdr_array(xdrs, (char **)&objp->items_val, &objp->items_len,
		~0, sizeof(OV_LINK_ITEM), (xdrproc_t)ov_ksserver_xdr_OV_LINK_ITEM);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_LINK_RES
*/
OV_BOOL ov_ksserver_xdr_OV_LINK_RES(
	XDR			*xdrs,
	OV_LINK_RES	*objp
) {
	if(!ov_ksserver_xdr_OV_RESULT(xdrs, &objp->result)) {
		return FALSE;
	}
	switch(objp->result) {
		case OV_ERR_OK:
			return ov_ksserver_xdr_array(xdrs, (char **)&objp->results_val,
				&objp->results_len, ~0, sizeof(OV_RESULT),
				(xdrproc_t)ov_ksserver_xdr_OV_RESULT);
		default:
			break;
	}
	return TRUE;
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_UNLINK_PAR
*/
OV_BOOL ov_ksserver_xdr_OV_UNLINK_PAR(
	XDR				*xdrs,
	OV_UNLINK_PAR	*objp
) {
	return ov_ksserver_xdr_array(xdrs, (char **)&objp->items_val, &objp->items_len,
		~0, sizeof(OV_UNLINK_ITEM), (xdrproc_t)ov_ksserver_xdr_OV_UNLINK_ITEM);
}

/*	----------------------------------------------------------------------	*/

/*
*	XDR routine for OV_UNLINK_RES
*/
OV_BOOL ov_ksserver_xdr_OV_UNLINK_RES(
	XDR				*xdrs,
	OV_UNLINK_RES	*objp
) {
	if(!ov_ksserver_xdr_OV_RESULT(xdrs, &objp->result)) {
		return FALSE;
	}
	switch(objp->result) {
		case OV_ERR_OK:
			return ov_ksserver_xdr_array(xdrs, (char **)&objp->results_val,
				&objp->results_len, ~0, sizeof(OV_RESULT),
				(xdrproc_t)ov_ksserver_xdr_OV_RESULT);
		default:
			break;
	}
	return TRUE;
}

/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/

