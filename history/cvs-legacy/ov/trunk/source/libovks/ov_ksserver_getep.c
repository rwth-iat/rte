/*
*   $Id: ov_ksserver_getep.c,v 1.4 1999-08-27 16:37:09 dirk Exp $
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
*	01-Jun-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*/

#define OV_COMPILE_LIBOVKS

#include "libovks/ov_ksserver.h"
#include "libov/ov_path.h"
#include "libov/ov_string.h"
#include "libov/ov_vendortree.h"
#include "libov/ov_macros.h"

/*	----------------------------------------------------------------------	*/

/*
*	Execute the GetEP service (subroutine)
*/
void ov_ksserver_getep(
	const OV_UINT		version,
	const OV_TICKET		*pticket,
	const OV_GETEP_PAR	*params,
	OV_GETEP_RES		*result
) {
	/*
	*	local variables
	*/
	OV_PATH			path;
	OV_STRING		name_mask = params->name_mask;
	OV_ELEMENT		child;
	OV_ELEM_TYPE	mask;
	/*
	*	initialization
	*/
	result->items_len = 0;
	result->pfirst = NULL;
	result->plast = NULL;
	/*
	*	resolve path (must be absolute)
	*/
	result->result = ov_path_resolve(&path, NULL, params->path, version);
	if(Ov_Fail(result->result)) {
		return;
	}
	if(!*name_mask) {
		/*
		*	we are asked about the engineered properties
		*	of the object specified in the path
		*/
		result->result = ov_ksserver_getep_additem(version, pticket, result,
			&path.elements[path.size-1], params->type_mask, NULL);
		return;
	}
	/*
	*	we are asked bout the engineered properties of the children
	*	and/or parts of the object specified in the path
	*/
	if(params->scope_flags & KS_EPF_PARTS) {
		/*
		*	TODO & FIXME! flatten modus...
		*/
		/*
		*	figure out the type mask
		*/
		mask = OV_ET_NONE;
		if(params->type_mask & KS_OT_DOMAIN) {
			mask |= OV_ET_OBJECT | OV_ET_VARIABLE | OV_ET_MEMBER;	/* FIXME! structures? */
		}
		if(params->type_mask & KS_OT_VARIABLE) {
			mask |= OV_ET_VARIABLE | OV_ET_MEMBER;	/* FIXME! structures? */
		}
		if(params->type_mask & KS_OT_LINK) {
			mask |= OV_ET_HEAD | OV_ET_ANCHOR;
		}
		if(mask == OV_ET_NONE) {
			/*
			*	for now, we do not have histories...
			*/
			result->result = OV_ERR_OK;
			return;
		}
		/*
		*	search through the parts
		*/
		child.elemtype = OV_ET_NONE;
		while(TRUE) {
			/*
			*	get next part
			*/
			if(Ov_Fail(ov_element_getnextpart(&path.elements[path.size-1], &child, mask))) {
				Ov_Warning("internal error");
				result->result = OV_ERR_GENERIC;
				return;
			}
			/*
			*	test if we are finished
			*/
			if(child.elemtype == OV_ET_NONE) {
				break;
			}
			/*
			*	add part to list of items
			*/
			result->result = ov_ksserver_getep_additem(version, pticket, result,
				&child, params->type_mask, params->name_mask);
			if(Ov_Fail(result->result)) {
				return;
			}
		}
	}
	if((params->scope_flags & KS_EPF_CHILDREN) && (params->type_mask & KS_OT_DOMAIN)) {
		/*
		*	search through the children
		*/
		child.elemtype = OV_ET_NONE;
		while(TRUE) {
			/*
			*	get next child
			*/
			if(Ov_Fail(ov_element_getnextchild(&path.elements[path.size-1], &child))) {
				Ov_Warning("internal error");
				result->result = OV_ERR_GENERIC;
				return;
			}
			/*
			*	test if we are finished
			*/
			if(child.elemtype == OV_ET_NONE) {
				break;
			}
			/*
			*	add child to list of items
			*/
			result->result = ov_ksserver_getep_additem(version, pticket, result,
				&child, params->type_mask, params->name_mask);
			if(Ov_Fail(result->result)) {
				return;
			}
		}
	}
	/*
	*	finished
	*/
	return;
}

/*	----------------------------------------------------------------------	*/

/*
*	Add an item to the list of engineered properties (subroutine)
*/
OV_RESULT ov_ksserver_getep_additem(
	const OV_UINT		version,
	const OV_TICKET		*pticket,
	OV_GETEP_RES		*result,
	const OV_ELEMENT	*pelem,
	const OV_OBJ_TYPE	typemask,
	const OV_STRING		namemask
) {
	/*
	*	local variables
	*/
	OV_OBJ_ENGINEERED_PROPS	*pprops;
	OV_OBJ_TYPE				objtype;
	OV_ACCESS				access;
	OV_INSTPTR_ov_object	pobj = pelem->pobj;
	OV_VTBLPTR_ov_object	pvtable;
	OV_BOOL					vendorobj = FALSE;
	OV_STRING				identifier;
	/*
	*	get the vtable of the object the element belongs to
	*/
	Ov_GetVTablePtr(ov_object, pvtable, pobj);
	if(!pvtable) {
		pvtable = pclass_ov_object->v_pvtable;
	}
	/*
	*	test if we have access to this object
	*/
	access = (pvtable->m_getaccess)(pobj, pelem, pticket);
	if(!(access & OV_AC_READWRITE)) {
		return OV_ERR_OK;
	}
	/*
	*	map OV element type to KS object type
	*/
	switch(pelem->elemtype) {
		case OV_ET_OBJECT:
			/*
			*	vendor objects must be treated specially
			*/
			if((pelem->pobj < &pdb->vendorobj[OV_NUM_VENDOROBJECTS])
				&& (pelem->pobj >= &pdb->vendorobj[0])
			) {
				objtype = KS_OT_VARIABLE;
				vendorobj = TRUE;
			} else {
				objtype = KS_OT_DOMAIN;
			}
			/*
			*	embedded objects are parts
			*/
			if(pobj->v_pouterobject) {
				access |= OV_AC_PART;
			}
			break;
		case OV_ET_VARIABLE:
		case OV_ET_MEMBER:
			/*
			*	FIXME! structure?
			*/
			if(pelem->elemunion.pvar->v_vartype == OV_VT_STRUCT) {
				objtype = KS_OT_DOMAIN;
			} else {
				objtype = KS_OT_VARIABLE;
			}
			access |= OV_AC_PART;
			break;
		case OV_ET_HEAD:
			objtype = KS_OT_LINK;
			identifier = pelem->elemunion.passoc->v_childrolename;
			access |= OV_AC_PART;
			break;
		case OV_ET_ANCHOR:
			objtype = KS_OT_LINK;
			identifier = pelem->elemunion.passoc->v_parentrolename;
			access |= OV_AC_PART;
			break;
		case OV_ET_OPERATION:
			/*
			*	for now, we do not show operations
			*/
			return OV_ERR_OK;	/* TODO! bad path? */
		default:
			/* this should not happen... */
			Ov_Warning("internal error");
			return OV_ERR_TARGETGENERIC;
	}
	/*
	*	test the type of the element
	*/
	if(!(objtype & typemask)) {
		/*
		*	element is not of specified type
		*/
		return OV_ERR_OK;
	}
	/*
	*	test the name mask
	*/
	identifier = ov_element_getidentifier(pelem);
	if(namemask) {
		if(!ov_string_match(identifier, namemask)) {
			return OV_ERR_OK;
		}
	}
	/*
	*	create new result item and insert into result item list
	*/
	pprops = Ov_MemStackAlloc(OV_OBJ_ENGINEERED_PROPS);
	if(!pprops) {
		return OV_ERR_TARGETGENERIC;	/* TODO! out of heap memory */
	}
	pprops->pnext = NULL;
	if(result->plast) {
		result->plast->pnext = pprops;
		result->plast = pprops;
	} else {
		result->pfirst = result->plast = pprops;
	}
	result->items_len++;
	/*
	*	set engineered properties
	*/
	pprops->objtype = objtype;
	pprops->identifier = identifier;
	pprops->creation_time = pobj->v_creationtime;
	pprops->comment = (pvtable->m_getcomment)(pobj, pelem);
	pprops->access = access;
	pprops->semantic_flags = (pvtable->m_getflags)(pobj, pelem);
	switch(pelem->elemtype) {
		case OV_ET_OBJECT:
			if(vendorobj) {
				OV_VAR_CURRENT_PROPS	varcurrprops;
				if(Ov_Fail(ov_vendortree_getvar(pobj, &varcurrprops, pticket))) {
					return OV_ERR_GENERIC;
				}
				pprops->comment = "";
				pprops->OV_OBJ_ENGINEERED_PROPS_u.var_engineered_props
					.tech_unit = ov_vendortree_getunit(pobj);
				pprops->OV_OBJ_ENGINEERED_PROPS_u.var_engineered_props
					.vartype = varcurrprops.value.vartype & OV_VT_KSMASK;
				return OV_ERR_OK;
			}
			pprops->OV_OBJ_ENGINEERED_PROPS_u.domain_engineered_props.class_identifier
				= ov_path_getcanonicalpath(Ov_PtrUpCast(ov_object,
				Ov_GetParent(ov_instantiation, pobj)), version);
			return OV_ERR_OK;
		case OV_ET_VARIABLE:
		case OV_ET_MEMBER:
			switch(objtype) {
				case KS_OT_VARIABLE:
					pprops->OV_OBJ_ENGINEERED_PROPS_u.var_engineered_props.tech_unit
						= (pvtable->m_gettechunit)(pobj, pelem);
					pprops->OV_OBJ_ENGINEERED_PROPS_u.var_engineered_props.vartype
						= pelem->elemunion.pvar->v_vartype & OV_VT_KSMASK;
					return OV_ERR_OK;
				case KS_OT_DOMAIN:
					return OV_ERR_OK;
				default:
					break;
			}
			break;
		case OV_ET_HEAD:
			pprops->OV_OBJ_ENGINEERED_PROPS_u.link_engineered_props.linktype
				= (pelem->elemunion.passoc->v_assocprops & OV_AP_LOCAL)?
				(KS_LT_LOCAL_1_MANY):(KS_LT_GLOBAL_1_MANY);
			pprops->OV_OBJ_ENGINEERED_PROPS_u.link_engineered_props.
				opposite_role_identifier = pelem->elemunion.passoc
				->v_parentrolename;
			pprops->OV_OBJ_ENGINEERED_PROPS_u.link_engineered_props.association_identifier
				= ov_path_getcanonicalpath(Ov_PtrUpCast(ov_object,pelem->elemunion.passoc),
				version);
			return OV_ERR_OK;
		case OV_ET_ANCHOR:
			pprops->OV_OBJ_ENGINEERED_PROPS_u.link_engineered_props.linktype
				= KS_LT_GLOBAL_1_1;
			pprops->OV_OBJ_ENGINEERED_PROPS_u.link_engineered_props.
				opposite_role_identifier = pelem->elemunion.passoc
				->v_childrolename;
			pprops->OV_OBJ_ENGINEERED_PROPS_u.link_engineered_props.association_identifier
				= ov_path_getcanonicalpath(Ov_PtrUpCast(ov_object,pelem->elemunion.passoc),
				version);
			return OV_ERR_OK;
		case OV_ET_OPERATION:
		default:
			break;
	}
	/* this should not happen... */
	Ov_Warning("internal error");
	return OV_ERR_GENERIC;
}

/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/

