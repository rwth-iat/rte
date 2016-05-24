/*
*   $Id: ov_ksserver_getep.c,v 1.2 2011-06-01 14:08:19 lars Exp $
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
*	04-Nov-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: variable type ANY added.
*	09-Apr-2001 Ansgar M�nnemann <ansgar@plt.rwth-aachen.de>: special GetEP answer for KsHistory and KsHistoryTrack objects
*	13-Jul-2001 Ansgar M�nnemann <ansgar@plt.rwth-aachen.de>: History concept changed
*/

#include "ov_ksserver_backend.h"

#include "libov/ov_path.h"
#include "libov/ov_string.h"
#include "libov/ov_vendortree.h"
#include "libov/ov_macros.h"


/*
*	Makro wrapping the address resolution of a symbol in a DLL/shared library
*/
#if OV_DYNAMIC_LIBRARIES
#if OV_SYSTEM_UNIX
#define Ov_Library_GetAddr(handle, symbolname)	dlsym(handle, symbolname)
#elif OV_SYSTEM_NT
#define Ov_Library_GetAddr(handle, symbolname)	GetProcAddress(handle, symbolname)
#endif
#endif

#if OV_DYNAMIC_LIBRARIES
/*
*	Function prototype for method getephist 
*/

typedef OV_RESULT OV_FNC_GETEPHIST (
	OV_INSTPTR_ov_object 	pobj,
	OV_STRING*		type_identifier,
	OV_HIST_TYPE*		historytype,
	OV_INTERPOLATION_MODE*  default_interpolation,
	OV_INTERPOLATION_MODE*	supported_interpolation
);

/*
*	Function prototype for method getephistelem 
*/

typedef OV_RESULT OV_FNC_GETEPHISTELEM (
	OV_INSTPTR_ov_object 	pobj,
	const OV_TICKET		*pticket,
	OV_GETEP_RES		*result,
	KS_EP_FLAGS		scope_flags,
	const OV_OBJ_TYPE	type_mask,
	const OV_STRING		name_mask
);

#else
#include "kshist.h"
#endif

/*	----------------------------------------------------------------------	*/

/*
*	Execute the GetEP service (subroutine)
*	needs ov_memstack_lock, because of class_identifier (and ov_pathresolve crashes)
*/
OV_DLLFNCEXPORT void ov_ksserver_getep(
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
	OV_ELEMENT		child={OV_ET_NONE, NULL, NULL, NULL, {NULL}};
	OV_ELEM_TYPE		mask;
	OV_INSTPTR_ov_class	pclass;
	OV_ELEMENT 		searchedelement;
	OV_FNC_GETEPHISTELEM	*getephistelem = NULL;
#if OV_DYNAMIC_LIBRARIES
	OV_INSTPTR_ov_library	plib;
	char			tmpstring[256];
#endif
	/*
	*	initialization
	*/
	result->items_len = 0;
	result->pfirst = NULL;
	result->plast = NULL;
	/*
	*   test (grand) access
	*/
	if(!(pticket->vtbl->getaccess(pticket) & OV_AC_READ) ) {
		result->result = OV_ERR_NOACCESS;
		return;
	}
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
	*	test if we have a object derived from class KsHistoryRO (representation object).
	*	if we have, call the getephistelem function of this object
	*/
	if (path.elements[path.size-1].elemtype == OV_ET_OBJECT) {
		pclass=Ov_GetParent(ov_instantiation, path.elements[path.size-1].pobj);
		while (pclass && (ov_string_compare(pclass->v_identifier, "KsHistoryRO")!=0)) 
			pclass=Ov_GetParent(ov_inheritance, pclass);

		/*
		*	we have ...
		*/
		if (pclass) {
			pclass=Ov_GetParent(ov_instantiation, path.elements[path.size-1].pobj);
			child.elemtype = OV_ET_OBJECT;
			child.pobj = Ov_PtrUpCast(ov_object, pclass);
			while (pclass) {
				/*
				*	search for operation element "getephistelem"
				*/
				if (Ov_OK(ov_element_searchpart(&child, &searchedelement, OV_ET_OPERATION, "getephistelem"))) break;
				pclass=Ov_GetParent(ov_inheritance, pclass);
				child.pobj = Ov_PtrUpCast(ov_object, pclass);
			}
			if (pclass) {
#if OV_DYNAMIC_LIBRARIES
				plib=Ov_StaticPtrCast(ov_library,Ov_GetParent(ov_containment, Ov_PtrUpCast(ov_object,pclass)));
				sprintf(tmpstring, "%s_%s_%s", plib->v_identifier, pclass->v_identifier, "getephistelem");
				getephistelem = (OV_FNC_GETEPHISTELEM*)Ov_Library_GetAddr((OV_DLLHANDLE)plib->v_handle, tmpstring);
#else
				getephistelem = (OV_FNC_GETEPHISTELEM*) ((OV_VTBLPTR_kshist_KsHistoryRO) pclass->v_pvtable)->m_getephistelem;	
				
#endif				
				result->result = getephistelem(path.elements[path.size-1].pobj, pticket, result, params->scope_flags, params->type_mask, name_mask);
			}
			return;
		}
	}
	/*
	*	we are asked bout the engineered properties of the children
	*	and/or parts of the object specified in the path
	*/
	if(params->scope_flags & KS_EPF_PARTS) {

		/*
		*	TODO & FIXME! flatten modus...
		*	note: flatten should have been a recursive request (Albrecht december 2013)
		*/
		/*
		*	figure out the type mask
		*/
		mask = OV_ET_NONE;
		if(params->type_mask & KS_OT_STRUCTURE) {
			// 2013 note: structure is a c struct within the OV object.
			// it was superseded with OV_PARTS therefore never activated in KS
			mask |= OV_ET_MEMBER;
		}
		if(params->type_mask & KS_OT_DOMAIN) {
			mask |= OV_ET_OBJECT | OV_ET_VARIABLE | OV_ET_MEMBER; 
		}
		if(params->type_mask & KS_OT_VARIABLE) {
			mask |= OV_ET_VARIABLE | OV_ET_MEMBER;
		}
		if(params->type_mask & KS_OT_LINK) {
			mask |= OV_ET_PARENTLINK | OV_ET_CHILDLINK;
		}
		if(params->type_mask & KS_OT_HISTORY) {
			mask |= OV_ET_OBJECT;
		}
		if(mask == OV_ET_NONE) {
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
	if((params->scope_flags & KS_EPF_CHILDREN) && ((params->type_mask & (KS_OT_DOMAIN | KS_OT_VARIABLE | KS_OT_HISTORY)))) {

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
	OV_OBJ_TYPE				objtype=0;
	OV_ACCESS				access;
	OV_INSTPTR_ov_object			pobj = pelem->pobj;
	OV_VTBLPTR_ov_object			pvtable;
	OV_BOOL					vendorobj = FALSE;
	OV_BOOL					kshistobj = FALSE;
	OV_INSTPTR_ov_class			pclass;
	OV_STRING				identifier;
	OV_ELEMENT 				searchedelement;
	OV_FNC_GETEPHIST			*getephist = NULL;
	OV_ELEMENT				child;
#if OV_DYNAMIC_LIBRARIES	
	OV_INSTPTR_ov_library			plib;
	char					tmpstring[256];
#endif

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
	access = pvtable->m_getaccess(pobj, pelem, pticket);
	if(!(access & OV_AC_READ)) {
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
			pclass=Ov_GetParent(ov_instantiation, pelem->pobj);
			while (pclass && (!kshistobj)) {
				if (ov_string_compare(pclass->v_identifier, "KsHistoryRO")==0) {
					objtype = KS_OT_HISTORY;
					kshistobj = TRUE;
				}
				else {
					pclass=Ov_GetParent(ov_inheritance, pclass);
				}
			}
			if (!kshistobj) {
				objtype = KS_OT_DOMAIN;
			}
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
		switch(pelem->elemunion.pvar->v_vartype & OV_VT_KSMASK) {
			case OV_VT_STRUCT:
			case OV_VT_STRUCT_VEC:
				objtype = KS_OT_DOMAIN;  // *** KS_OT_STRUCTURE ***  ? KS ?
				// 2013 note: structure is a c struct within the OV object.
				// it was superseded with OV_PARTS therefore never activated in KS
				break;
			default:
				objtype = KS_OT_VARIABLE;
				break;
		}
		access |= OV_AC_PART;
		break;
	case OV_ET_PARENTLINK:
		objtype = KS_OT_LINK;
		identifier = pelem->elemunion.passoc->v_childrolename;
		access |= OV_AC_PART;
		break;
	case OV_ET_CHILDLINK:
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
	identifier = ov_path_topercent(ov_element_getidentifier(pelem));
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
			OV_RESULT				res;

			res = ov_vendortree_getvar(pobj, &varcurrprops, pticket);
			if(res==OV_ERR_NOACCESS) {
				pprops->access = OV_AC_NONE;
			} else if (Ov_Fail(res)) {
				return OV_ERR_GENERIC;
			}
			pprops->comment = "";
			pprops->OV_OBJ_ENGINEERED_PROPS_u.var_engineered_props
				.tech_unit = ov_vendortree_getunit(pobj);
			pprops->OV_OBJ_ENGINEERED_PROPS_u.var_engineered_props
				.vartype = varcurrprops.value.vartype & OV_VT_KSMASK;
			return OV_ERR_OK;
		}
		if(kshistobj) {

			OV_RESULT res = OV_ERR_NOTIMPLEMENTED;
			pclass=Ov_GetParent(ov_instantiation, pelem->pobj);
			child.elemtype = OV_ET_OBJECT;
			child.pobj = Ov_PtrUpCast(ov_object, pclass);
			while (pclass) {
				/*
				*	search for operation element "getephist"
				*/
				if (Ov_OK(ov_element_searchpart(&child, &searchedelement, OV_ET_OPERATION, "getephist"))) break;
				pclass=Ov_GetParent(ov_inheritance, pclass);
				child.pobj = Ov_PtrUpCast(ov_object, pclass);
			}
			if (pclass) {
#if OV_DYNAMIC_LIBRARIES				

				plib=Ov_StaticPtrCast(ov_library,Ov_GetParent(ov_containment, Ov_PtrUpCast(ov_object,pclass)));
				sprintf(tmpstring, "%s_%s_%s", plib->v_identifier, pclass->v_identifier, "getephist");
				getephist = (OV_FNC_GETEPHIST*)Ov_Library_GetAddr((OV_DLLHANDLE)plib->v_handle, tmpstring);
#else
				getephist = (OV_FNC_GETEPHIST*) ((OV_VTBLPTR_kshist_KsHistoryRO) pclass->v_pvtable)->m_getephist;	
				
#endif					
				pprops->OV_OBJ_ENGINEERED_PROPS_u.history_engineered_props.type_identifier = NULL;
				pprops->OV_OBJ_ENGINEERED_PROPS_u.history_engineered_props.historytype = 0;
				pprops->OV_OBJ_ENGINEERED_PROPS_u.history_engineered_props.default_interpolation = 0;
				pprops->OV_OBJ_ENGINEERED_PROPS_u.history_engineered_props.supported_interpolation = 0;
				res = getephist(pelem->pobj, &pprops->OV_OBJ_ENGINEERED_PROPS_u.history_engineered_props.type_identifier, 
							&pprops->OV_OBJ_ENGINEERED_PROPS_u.history_engineered_props.historytype,
							&pprops->OV_OBJ_ENGINEERED_PROPS_u.history_engineered_props.default_interpolation,
							&pprops->OV_OBJ_ENGINEERED_PROPS_u.history_engineered_props.supported_interpolation);
			}
			if (res != OV_ERR_OK) {
				pprops->OV_OBJ_ENGINEERED_PROPS_u.history_engineered_props.type_identifier = NULL;
				pprops->OV_OBJ_ENGINEERED_PROPS_u.history_engineered_props.historytype = 0;
				pprops->OV_OBJ_ENGINEERED_PROPS_u.history_engineered_props.default_interpolation = 0;
				pprops->OV_OBJ_ENGINEERED_PROPS_u.history_engineered_props.supported_interpolation = 0;
			}
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
				= (pelem->elemunion.pvar->v_vartype != OV_VT_ANY)
				?(pelem->elemunion.pvar->v_vartype & OV_VT_KSMASK)
				:(((OV_ANY*)pelem->pvalue)->value.vartype & OV_VT_KSMASK);
			return OV_ERR_OK;
		case KS_OT_DOMAIN:   // *** KS_OT_STRUCTURE ***  ? KS ?
			// 2013 note: structure is a c struct within the OV object.
			// it was superseded with OV_PARTS therefore never activated in KS
//			pprops->OV_OBJ_ENGINEERED_PROPS_u.struct_engineered_props.type_identifier
//				= ov_path_getcanonicalpath(Ov_PtrUpCast(ov_object,
//				Ov_GetParent(ov_instantiation, pobj)), version);
			pprops->OV_OBJ_ENGINEERED_PROPS_u.domain_engineered_props.class_identifier
				= ov_path_getcanonicalpath(Ov_PtrUpCast(ov_object,
				Ov_GetParent(ov_construction, pelem->elemunion.pvar)), version);
			return OV_ERR_OK;
		default:
			break;
		}
		break;
	case OV_ET_PARENTLINK:
		switch(pelem->elemunion.passoc->v_assoctype) {
		case OV_AT_ONE_TO_ONE:
			pprops->OV_OBJ_ENGINEERED_PROPS_u.link_engineered_props.linktype
				= (pelem->elemunion.passoc->v_assocprops & OV_AP_LOCAL)?
				(KS_LT_LOCAL_1_1):(KS_LT_GLOBAL_1_1);
			break;
		case OV_AT_ONE_TO_MANY:
			pprops->OV_OBJ_ENGINEERED_PROPS_u.link_engineered_props.linktype
				= (pelem->elemunion.passoc->v_assocprops & OV_AP_LOCAL)?
				(KS_LT_LOCAL_1_MANY):(KS_LT_GLOBAL_1_MANY);
			break;
		case OV_AT_MANY_TO_MANY:
			pprops->OV_OBJ_ENGINEERED_PROPS_u.link_engineered_props.linktype
				= KS_LT_GLOBAL_MANY_MANY;
			break;
		default:
			Ov_Warning("no such association type");
			return OV_ERR_TARGETGENERIC;
		}
		pprops->OV_OBJ_ENGINEERED_PROPS_u.link_engineered_props.
			opposite_role_identifier = pelem->elemunion.passoc
			->v_parentrolename;
		pprops->OV_OBJ_ENGINEERED_PROPS_u.link_engineered_props.association_identifier
			= ov_path_getcanonicalpath(Ov_PtrUpCast(ov_object,pelem->elemunion.passoc),
			version);
		return OV_ERR_OK;
	case OV_ET_CHILDLINK:
		switch(pelem->elemunion.passoc->v_assoctype) {
		case OV_AT_ONE_TO_ONE:
			pprops->OV_OBJ_ENGINEERED_PROPS_u.link_engineered_props.linktype
				= KS_LT_GLOBAL_1_1;
			break;
		case OV_AT_ONE_TO_MANY:
			pprops->OV_OBJ_ENGINEERED_PROPS_u.link_engineered_props.linktype
				= KS_LT_GLOBAL_MANY_1;
			break;
		case OV_AT_MANY_TO_MANY:
			pprops->OV_OBJ_ENGINEERED_PROPS_u.link_engineered_props.linktype
				= KS_LT_GLOBAL_MANY_MANY;
			break;
		default:
			Ov_Warning("no such association type");
			return OV_ERR_TARGETGENERIC;
		}
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

