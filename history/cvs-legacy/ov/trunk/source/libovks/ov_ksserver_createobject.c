/*
*   $Id: ov_ksserver_createobject.c,v 1.1 1999-07-19 15:02:16 dirk Exp $
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
*	13-Oct-1998 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*	21-Apr-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: Major revision.
*/

#define OV_COMPILE_LIBOVKS

#include "libovks/ov_ksserver.h"
#include "libov/ov_path.h"
#include "libov/ov_macros.h"

/*	----------------------------------------------------------------------	*/

/*
*	Information for the initfunction used durig object creation
*/
typedef struct {
	OV_UINT					version;
	const OV_TICKET			*pticket;
	OV_CREATEOBJ_ITEM		*pitem;
	OV_CREATEOBJECTITEM_RES	*presult;
}	OV_KSSERVER_CREATEOBJECT_INFO;	

/*	----------------------------------------------------------------------	*/

/*
*	Execute the CreateObject service (subroutine)
*/
void ov_ksserver_createobject(
	const OV_UINT				version,
	const OV_TICKET				*pticket,
	const OV_CREATEOBJECT_PAR	*params,
	OV_CREATEOBJECT_RES			*result
) {
	/*
	*	local variables
	*/
	OV_UINT							len = params->items_len;
	OV_CREATEOBJECTITEM_RES			*presult;
	OV_CREATEOBJ_ITEM				*pitem = params->items_val;
	OV_UINT							i;
	OV_PATH							path;
	OV_INSTPTR_ov_class				pclass;
	OV_INSTPTR_ov_domain			pdom;
	OV_INSTPTR_ov_object			prelchild;
	OV_INSTPTR_ov_object			pobj;
	OV_STRING						objpath, identifier;
	OV_PLACEMENT_HINT				hint;
	OV_VTBLPTR_ov_domain			pvtable;
	OV_KSSERVER_CREATEOBJECT_INFO	*pinfo;
	/*
	*	initialization
	*/
	result->result = OV_ERR_OK;
	result->obj_results_len = 0;
	result->obj_results_val = NULL;
	/*
	*	if there are no items, we are done
	*/
	if(!len) {
		return;
	}
	/*
	*	allocate memory for the info object and initialize it
	*/
	pinfo = Ov_MemStackAlloc(OV_KSSERVER_CREATEOBJECT_INFO);
	if(!pinfo) {
		result->result = OV_ERR_TARGETGENERIC;	/* TODO! no heap memory */
		return;
	}
	pinfo->version = version;
	pinfo->pticket = pticket;
	/*
	*	allocate memory for the results
	*/
	presult = (OV_CREATEOBJECTITEM_RES*)ov_memstack_alloc
		(len*sizeof(OV_CREATEOBJECTITEM_RES));
	if(!presult) {
		result->result = OV_ERR_TARGETGENERIC;	/* TODO! no heap memory */
		return;
	}
	result->obj_results_val = presult;
	result->obj_results_len = len;
	/*
	*	iterate over the items given
	*/
	for(i=0; i<len; i++, pitem++, presult++) {
		/*
		*	resolve factory path (which must be absolute)
		*/
		presult->result = ov_path_resolve(&path, NULL, pitem->factory_path, version);
		if(Ov_Fail(presult->result)) {
			continue;
		}
		/*
		*	test, if it is a valid factory
		*/
		if(path.elements[path.size-1].elemtype != OV_ET_OBJECT) {
			presult->result = OV_ERR_BADFACTORY;
			continue;
		}
		pclass = Ov_DynamicPtrCast(ov_class, path.elements[path.size-1].pobj);
		if(!pclass) {
			presult->result = OV_ERR_BADFACTORY;
			continue;
		}
		/*
		*	now take a look at the path of the new object. We got to separate
		*	the path to the domain object which shall contain the new object
		*	and the identifier of the new object.
		*/
		objpath = pitem->new_path;
		for(identifier=objpath+strlen(objpath)-1; identifier>=objpath; identifier--) {
			if(*identifier == '/') {
				*identifier = 0;
				identifier++;
				break;
			}
		}
		if(identifier<objpath) {
			presult->result = OV_ERR_BADPATH;
			continue;
		}
		if(!*objpath) {
			/*
			*	new_path is "/xxxx" and we removed the '/';
			*/
			objpath = "/";
		}
		/*
		*	resolve the path to the domain containing the new object (absolute path)
		*/
		presult->result = ov_path_resolve(&path, NULL, objpath, version);
		if(Ov_Fail(presult->result)) {
			continue;
		}
		/*
		*	test, if it is a domain
		*/
		if(path.elements[path.size-1].elemtype != OV_ET_OBJECT) {
			presult->result = OV_ERR_BADPATH;
			continue;
		}
		pdom = Ov_DynamicPtrCast(ov_domain, path.elements[path.size-1].pobj);
		if(!pdom) {
			presult->result = OV_ERR_BADPATH;
			continue;
		}
		/*
		*	test if we have write access on the domain
		*/
		Ov_GetVTablePtr(ov_domain, pvtable, pdom);
		if(!pvtable) {
			pvtable = (OV_VTBLPTR_ov_domain)pclass_ov_domain->v_pvtable;
		}
		if(!((pvtable->m_getaccess)(Ov_PtrUpCast(ov_object, pdom),
			&path.elements[path.size-1], pticket) & OV_AC_WRITE)
		) {
			presult->result = OV_ERR_NOACCESS;
			continue;
		}
		/*
		*	figure out the placement
		*/
		hint = pitem->place.hint;
		prelchild = NULL;
		if(hint == OV_PMH_BEFORE || hint == OV_PMH_AFTER) {
			if(*pitem->place.place_path == '/') {
				/* absolute path */
				presult->result = ov_path_resolve(&path, NULL,
					pitem->place.place_path, version);
				if(Ov_Fail(presult->result)) {
					continue;
				}
				if(path.elements[path.size-1].elemtype != OV_ET_OBJECT) {
					presult->result = OV_ERR_BADPLACEMENT;
					continue;
				}
				prelchild = path.elements[path.size-1].pobj;
				if(Ov_GetParent(ov_containment, prelchild) != pdom) {
					presult->result = OV_ERR_BADPLACEMENT;
					continue;
				}
			} else {
				/* relative path */
				prelchild = Ov_SearchChild(ov_containment, pdom,
					pitem->place.place_path);
				if(!prelchild) {
					presult->result = OV_ERR_BADPLACEMENT;
					continue;
				}
			}
		}
		/*
		*	instantiate the object
		*/
		pinfo->pitem = pitem;
		pinfo->presult = presult;
		presult->result = ov_class_createobject(pclass, pdom, identifier,
			hint, prelchild, ov_ksserver_createobject_initobj, pinfo, &pobj);
	}	/* for */
	/*
	*	we are finished.
	*/
	return;
}

/*	----------------------------------------------------------------------	*/

/*
*	Initialize an object during instantiation in the CreateObject service
*/
OV_RESULT ov_ksserver_createobject_initobj(
	OV_INSTPTR_ov_object	pobj,
	OV_POINTER				pinfo
) {
	/*
	*	local variables
	*/
	OV_UINT					version = ((OV_KSSERVER_CREATEOBJECT_INFO*)pinfo)->version;
	const OV_TICKET			*pticket = ((OV_KSSERVER_CREATEOBJECT_INFO*)pinfo)->pticket;
	OV_CREATEOBJ_ITEM		*pitem = ((OV_KSSERVER_CREATEOBJECT_INFO*)pinfo)->pitem;
	OV_CREATEOBJECTITEM_RES	*presult = ((OV_KSSERVER_CREATEOBJECT_INFO*)pinfo)->presult;
	OV_SETVAR_ITEM			*pparitem = pitem->parameters_val;
	OV_LINK_ITEM			*plnkitem = pitem->links_val;
	OV_RESULT				*pparresult = NULL;
	OV_RESULT				*plnkresult = NULL;
	OV_UINT					parlen = pitem->parameters_len;
	OV_UINT					lnklen = pitem->links_len;
	OV_UINT					i;
	OV_ELEMENT				elem;
	OV_PATH					path, relpath, linkpath, elementpath, placepath, oppositeplacepath;
	/*
	*	allocate memory for the parameter and link results
	*	(just in case we have to come up with OV_ERR_BADINITPARAM)
	*/
	if(parlen) {
		pparresult = (OV_RESULT*)ov_memstack_alloc(parlen*sizeof(OV_RESULT));
		if(!pparresult) {
			return OV_ERR_TARGETGENERIC;	/* TODO! out of heap memory */
		}
	}
	if(lnklen) {
		plnkresult = (OV_RESULT*)ov_memstack_alloc(parlen*sizeof(OV_RESULT));
		if(!plnkresult) {
			return OV_ERR_TARGETGENERIC;	/* TODO! out of heap memory */
		}
	}
	presult->params_results_len = parlen;
	presult->params_results_val = pparresult;
	presult->link_results_len = lnklen;
	presult->link_results_val = plnkresult;
	/*
	*	initialize the parameter and link results
	*/
	for(i=0; i<parlen; i++) {
		pparresult[i] = OV_ERR_GENERIC;
	}
	for(i=0; i<lnklen; i++) {
		plnkresult[i] = OV_ERR_GENERIC;
	}
	/*
	*	initialize the relative path
	*/
	elem.elemtype = OV_ET_OBJECT;
	elem.pobj = pobj;
	relpath.size = 1;
	relpath.elements = &elem;
	/*
	*	initialize the object's parameters
	*/
	for(i=0; i<parlen; i++, pparitem++, pparresult++) {
		/*
		*	resolve the parameter's path, which must be relative
		*	to the new object's path
		*/
		if(*pparitem->path_and_name == '/') {
			return OV_ERR_BADINITPARAM;
		}
		*pparresult = ov_path_resolve(&path, &relpath, pparitem->path_and_name,
			version);
		if(Ov_OK(*pparresult)) {
			/*
			*	path is ok, set variable item
			*/
			*pparresult = ov_ksserver_setvar_setitem(pticket, pparitem,
				&path.elements[path.size-1], TRUE);
		}
		if(Ov_Fail(*pparresult)) {
			return OV_ERR_BADINITPARAM;
		}
	}
	/*
	*	initialize the object's links
	*/
	for(i=0; i<lnklen; i++, plnkitem++, plnkresult++) {
		/*
		*	resolve link path and element path; the link path
		*	must be relative, the element path absolute
		*/
		if(*plnkitem->link_path == '/') {
			return OV_ERR_BADINITPARAM;
		}
		*plnkresult = ov_path_resolve(&linkpath, &relpath, plnkitem->link_path, 
			version);
		if(Ov_Fail(*plnkresult)) {
			continue;
		}
		*plnkresult = ov_path_resolve(&elementpath, NULL, plnkitem->element_path,
			version);
		if(Ov_Fail(*plnkresult)) {
			continue;
		}
		/*
		*	if relative placement, resolve place path
		*/
		if((plnkitem->place.hint == OV_PMH_BEFORE)
			|| (plnkitem->place.hint == OV_PMH_AFTER)
		) {
			*plnkresult = ov_path_resolve(&placepath, NULL,
				plnkitem->place.place_path, version);
			if(Ov_Fail(*plnkresult)) {
				continue;
			}
		}
		/*
		*	if relative placement, resolve opposite place path
		*/
		if((plnkitem->opposite_place.hint == OV_PMH_BEFORE)
			|| (plnkitem->opposite_place.hint == OV_PMH_AFTER)
		) {
			*plnkresult = ov_path_resolve(&oppositeplacepath, NULL,
				plnkitem->opposite_place.place_path, version);
			if(Ov_Fail(*plnkresult)) {
				continue;
			}
		}
		/*
		*	link the item
		*/
		*plnkresult = ov_ksserver_link_linkitem(pticket,
			&linkpath.elements[linkpath.size-1],
			&elementpath.elements[elementpath.size-1],
			&placepath.elements[placepath.size-1],
			&oppositeplacepath.elements[oppositeplacepath.size-1],
			plnkitem->place.hint, plnkitem->opposite_place.hint);
		if(Ov_Fail(*plnkresult)) {
			return OV_ERR_BADINITPARAM;
		}
	}
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/

