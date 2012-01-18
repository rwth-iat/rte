/*
*   $Id: ov_ksserver_getvar.c,v 1.15 2007-04-25 13:59:03 martin Exp $
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
*	23-Jul-1998 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*	21-Apr-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: Major revision.
*/

#define OV_COMPILE_LIBOVKS

#include "libovks/ov_ksserver.h"
#include "libov/ov_path.h"
#include "libov/ov_string.h"
#include "libov/ov_time.h"
#include "libov/ov_macros.h"

/*	----------------------------------------------------------------------	*/

/*
*	Execute the GetVar service (subroutine)
*/
void ov_ksserver_getvar(
	const OV_UINT		version,
	const OV_TICKET		*pticket,
	const OV_GETVAR_PAR	*params,
	OV_GETVAR_RES		*result
) {
	/*
	*	local variables
	*/
	OV_UINT			len = params->identifiers_len;
	OV_GETVAR_ITEM	*pitem;
	OV_STRING		*pidentifier = params->identifiers_val;
	OV_PATH			lastpath, path;
	OV_UINT			i;
	/*
	*	initialization
	*/
	result->result = OV_ERR_OK;
	result->items_len = 0;
	result->items_val = NULL;
	/*
	*	if there are no items, we are done
	*/
	if(!len) {
		return;
	}
	/*
	*   test (grand) access
	*/
	if(!(pticket->vtbl->getaccess(pticket) & OV_AC_READ) ) {
		result->result = OV_ERR_NOACCESS;
		return;
	}
	/*
	*	allocate memory for the result items
	*/
	pitem = (OV_GETVAR_ITEM*)ov_memstack_alloc(len*sizeof(OV_GETVAR_ITEM));
	if(!pitem) {
		result->result = OV_ERR_TARGETGENERIC;
		return;
	}
	result->items_val = pitem;
	result->items_len = len;
	/*
	*	iterate over the identifiers given
	*/
	for(i=0; i<len; i++, pidentifier++, pitem++) {
		/*
		*	resolve path to variable item, the first time the path is absolute
		*/
		if(i) {
			pitem->result = ov_path_resolve(&path, &lastpath, *pidentifier, version);
		} else {
			pitem->result = ov_path_resolve(&path, NULL, *pidentifier, version);
		}
		if(Ov_OK(pitem->result)) {
			/*
			*	path is ok, get variable item
			*/
			ov_ksserver_getvar_getitem(version, pticket, pitem,
				&path.elements[path.size-1]);
		} else {
			/*
			*	path is bad, skip all following relative paths
			*/
			while(i+1<len) {
				if(**(pidentifier+1) == '/') {
					break;
				}
				i++;
				pidentifier++;
				pitem++;
				pitem->result = OV_ERR_BADPATH;
			}
		}
		lastpath = path;
	}	/* for */
	/*
	*	we are finished.
	*/
	return;
}

/*	----------------------------------------------------------------------	*/
/*
*	Get a structure variable for the GetVar service (subroutine)
*/
#define GETSTRUCTCASE(vartype, VARTYPE)	\
	case OV_VT_##VARTYPE:	\
		pstructelem->value.valueunion.val_##vartype = *((OV_##VARTYPE*) pvalue);	\
		pvalue += pvar->v_size;	\
		break;

OV_NAMED_ELEMENT* ov_ksserver_getvar_getstruct(
	OV_INSTPTR_ov_structure	pstruct,
	OV_STRUCT		*povstruct
) {
	OV_INSTPTR_ov_object	pobj;
	OV_INSTPTR_ov_variable	pvar;
	OV_NAMED_ELEMENT	*pstructelem;
	OV_NAMED_ELEMENT	*pstructelem2;
	OV_UINT			num;
	OV_BYTE			*pvalue;

	num = 0;
	Ov_ForEachChild(ov_containment, pstruct, pobj) num++;
	povstruct->elements = num;
	pvalue = povstruct->value;
	pvar = Ov_StaticPtrCast(ov_variable, Ov_GetFirstChild(ov_containment, pstruct));
	pstructelem = (OV_NAMED_ELEMENT*) ov_memstack_alloc(sizeof(OV_NAMED_ELEMENT)*num);
	pstructelem2 = pstructelem;
	while (pvar) {
		pstructelem->identifier = (OV_STRING)ov_memstack_alloc(ov_path_percentsize(pvar->v_identifier));
		pstructelem->value.vartype = pvar->v_vartype;
		switch(pstructelem->value.vartype) {
			GETSTRUCTCASE(bool, BOOL);
			GETSTRUCTCASE(int, INT);
			GETSTRUCTCASE(uint, UINT);
			GETSTRUCTCASE(single, SINGLE);
			GETSTRUCTCASE(double, DOUBLE);
			GETSTRUCTCASE(time, TIME);
			GETSTRUCTCASE(time_span, TIME_SPAN);
			GETSTRUCTCASE(string, STRING);

			GETSTRUCTCASE(bool_vec, BOOL_VEC);
			GETSTRUCTCASE(int_vec, INT_VEC);
			GETSTRUCTCASE(uint_vec, UINT_VEC);
			GETSTRUCTCASE(single_vec, SINGLE_VEC);
			GETSTRUCTCASE(double_vec, DOUBLE_VEC);
			GETSTRUCTCASE(time_vec, TIME_VEC);
			GETSTRUCTCASE(time_span_vec, TIME_SPAN_VEC);
			GETSTRUCTCASE(string_vec, STRING_VEC);
			case OV_VT_STRUCT:
				pstructelem->value.valueunion.val_struct.value = 
					(OV_BYTE*) ov_ksserver_getvar_getstruct(
					Ov_GetParent(ov_construction, pvar), &pstructelem->value.valueunion.val_struct);
				pvalue += Ov_GetParent(ov_construction, pvar)->v_size;
				break;
			default:
				Ov_Warning("internal error");
		}
		pvar = Ov_StaticPtrCast(ov_variable, Ov_GetNextChild(ov_containment, pvar));
		pstructelem++;
	}
	return pstructelem2;
}

/*	----------------------------------------------------------------------	*/
/*
*	Get a variable item of the GetVar service (subroutine)
*/
void ov_ksserver_getvar_getitem(
	const OV_UINT		version,
	const OV_TICKET		*pticket,
	OV_GETVAR_ITEM		*pitem,
	const OV_ELEMENT	*pelem
) {
	/*
	*	local variables
	*/
	OV_INSTPTR_ov_object	pobj = pelem->pobj;
	OV_VTBLPTR_ov_object	pvtable;
	OV_INSTPTR_ov_object	pchild, pparent;
	OV_UINT					len;
	OV_UINT					i;
	OV_STRING				*pstring;
	OV_STRING				pathname;
	OV_STRUCT				*povstruct;
	OV_INSTPTR_ov_structure	pstruct;
	Ov_Association_DefineIteratorNM(pit);
	/*
	*	get the vtable pointer of the object the variable belongs to
	*/
	Ov_GetVTablePtr(ov_object, pvtable, pobj);
	if((!pvtable) || (ov_activitylock)){
		pvtable = pclass_ov_object->v_pvtable;
	}
	/*
	*	test if we have access to this variable
	*/
	if(!(pvtable->m_getaccess(pobj, pelem, pticket) & OV_AC_READ)) {
		pitem->result = OV_ERR_NOACCESS;
		return;
	}
	/*
	*	switch based on the element's type
	*/
	switch(pelem->elemtype) {
	case OV_ET_OBJECT:
		/*
		*	object may be a vendor object
		*/
		if((pobj < &pdb->vendorobj[OV_NUM_VENDOROBJECTS])
			&& (pobj >= &pdb->vendorobj[0])
		) {
			pitem->result = ov_vendortree_getvar(pobj, 
				&pitem->var_current_props, pticket);
			pitem->var_current_props.value.vartype &= OV_VT_KSMASK;
			return;
		}
		break;
	case OV_ET_MEMBER:
	case OV_ET_VARIABLE:
		/*
		*	get the variable's value, timestamp and time
		*/
		pitem->result = (pvtable->m_getvar)(pobj, pelem, &pitem->var_current_props);
		pitem->var_current_props.value.vartype &= OV_VT_KSMASK;
		if (pitem->var_current_props.value.vartype == OV_VT_STRUCT) {
			povstruct = &pitem->var_current_props.value.valueunion.val_struct;
			pstruct = Ov_GetParent(ov_construction, (pelem->elemunion.pvar));
			pitem->var_current_props.value.valueunion.val_struct.value = (OV_BYTE*) 
				ov_ksserver_getvar_getstruct(pstruct, povstruct);
		}
		return;
	case OV_ET_PARENTLINK:
		/*
		*	get value of a parent link (string vector with children's paths)
		*/
		pitem->var_current_props.value.vartype = OV_VT_STRING_VEC;
		len = pitem->var_current_props.value.valueunion.val_string_vec.veclen
			= ov_association_getchildcount(pelem->elemunion.passoc, pobj);
		if(len) {
			pstring = pitem->var_current_props.value.valueunion.val_string_vec.value
				= (OV_STRING*)ov_memstack_alloc(len*sizeof(OV_STRING));
			if(!pstring) {
				pitem->result = OV_ERR_TARGETGENERIC;	/* TODO! out of heap memory */
				return;
			}
			switch(pelem->elemunion.passoc->v_assoctype) {
			case OV_AT_ONE_TO_ONE:
				pchild = Ov_Association_GetChild(pelem->elemunion.passoc, pobj);
				break;
			case OV_AT_ONE_TO_MANY:
				pchild = Ov_Association_GetFirstChild(pelem->elemunion.passoc, pobj);
				break;
			case OV_AT_MANY_TO_MANY:
				pchild = Ov_Association_GetFirstChildNM(pelem->elemunion.passoc, pit, pobj);
				break;
			default:
				Ov_Warning("no such association type");
				pitem->result = OV_ERR_TARGETGENERIC;
				return;
			}
			for(i=0; i<len; i++) {
				Ov_AbortIfNot(pchild);
				pathname = ov_path_getcanonicalpath(pchild, version);
				if(!pathname) {
					pitem->result = OV_ERR_TARGETGENERIC;	/* TODO! out of heap memory */
					return;
				}
				*pstring = pathname;
				pstring++;
				switch(pelem->elemunion.passoc->v_assoctype) {
				case OV_AT_ONE_TO_ONE:
					pchild = NULL;
					break;
				case OV_AT_ONE_TO_MANY:
					pchild = Ov_Association_GetNextChild(pelem->elemunion.passoc, pchild);
					break;
				case OV_AT_MANY_TO_MANY:
					pchild = Ov_Association_GetNextChildNM(pelem->elemunion.passoc, pit);
					break;
				default:
					Ov_Warning("no such association type");
					pitem->result = OV_ERR_TARGETGENERIC;
					return;
				}
			}
			Ov_WarnIf(pchild);
		}
		/*
		*	set timestamp and state
		*/
		ov_time_gettime(&pitem->var_current_props.time);
		pitem->var_current_props.state = OV_ST_NOTSUPPORTED;
		return;
	case OV_ET_CHILDLINK:
		/*
		*	get value of a child link (string with the parent's paths)
		*/
		switch(pelem->elemunion.passoc->v_assoctype) {
		case OV_AT_ONE_TO_ONE:
			pitem->var_current_props.value.vartype = OV_VT_STRING;
			pparent = Ov_Association_GetParent(pelem->elemunion.passoc, pobj);
			if(pparent) {
				pathname = ov_path_getcanonicalpath(pparent, version);
				if(!pathname) {
					pitem->result = OV_ERR_TARGETGENERIC;	/* TODO! out of heap memory */
					return;
				}
				pitem->var_current_props.value.valueunion.val_string = pathname;
			} else {
				pitem->var_current_props.value.valueunion.val_string = NULL;
			}
			break;
		case OV_AT_ONE_TO_MANY:
			pitem->var_current_props.value.vartype = OV_VT_STRING;
			pparent = Ov_Association_GetParent(pelem->elemunion.passoc, pobj);
			if(pparent) {
				pathname = ov_path_getcanonicalpath(pparent, version);
				if(!pathname) {
					pitem->result = OV_ERR_TARGETGENERIC;	/* TODO! out of heap memory */
					return;
				}
				pitem->var_current_props.value.valueunion.val_string = pathname;
			} else {
				pitem->var_current_props.value.valueunion.val_string = NULL;
			}
			break;
		case OV_AT_MANY_TO_MANY:
			pitem->var_current_props.value.vartype = OV_VT_STRING_VEC;
			len = pitem->var_current_props.value.valueunion.val_string_vec.veclen
				= ov_association_getparentcount(pelem->elemunion.passoc, pobj);
			if(len) {
				pstring = pitem->var_current_props.value.valueunion.val_string_vec.value
					= (OV_STRING*)ov_memstack_alloc(len*sizeof(OV_STRING));
				if(!pstring) {
					pitem->result = OV_ERR_TARGETGENERIC;	/* TODO! out of heap memory */
					return;
				}
				pparent = Ov_Association_GetFirstParentNM(pelem->elemunion.passoc, pit, pobj);
				for(i=0; i<len; i++) {
					Ov_AbortIfNot(pparent);
					pathname = ov_path_getcanonicalpath(pparent, version);
					if(!pathname) {
						pitem->result = OV_ERR_TARGETGENERIC;	/* TODO! out of heap memory */
						return;
					}
					*pstring = pathname;
					pstring++;
					pparent = Ov_Association_GetNextParentNM(pelem->elemunion.passoc, pit);
				}
				Ov_WarnIf(pparent);
			}
			break;
		default:
			Ov_Warning("no such association type");
			pitem->result = OV_ERR_TARGETGENERIC;
			break;
		}
		/*
		*	set timestamp and state
		*/
		ov_time_gettime(&pitem->var_current_props.time);
		pitem->var_current_props.state = OV_ST_NOTSUPPORTED;
		return;
	default:
		break;
	}
	pitem->result = OV_ERR_BADOBJTYPE;
	return;
}

/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/

