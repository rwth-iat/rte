/*
*   $Id: ov_ksserver_renameobject.c,v 1.1.1.1 2011-01-31 07:47:43 henning Exp $
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
*	23-Jun-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*/

#include "ov_ksserver_stripped.h"

#include "libov/ov_path.h"
#include "libov/ov_macros.h"

/*	----------------------------------------------------------------------	*/

/*
*	Execute the RenameObject service (subroutine)
*/
OV_DLLFNCEXPORT void ov_ksserver_renameobject(
	const OV_UINT				version,
	const OV_TICKET				*pticket,
	const OV_RENAMEOBJECT_PAR	*params,
	OV_RENAMEOBJECT_RES			*result
) {
	/*
	*	local variables
	*/
	OV_UINT					len = params->items_len;
	OV_RESULT				*presult;
	OV_RENAMEOBJECT_ITEM	*pitem = params->items_val;
	OV_UINT					i;
	OV_PATH					path, newpath;
	OV_INSTPTR_ov_domain	pdom;
	OV_INSTPTR_ov_object	prelchild;
	OV_INSTPTR_ov_object	pobj;
	OV_STRING				newobjpath, identifier;
	OV_PLACEMENT_HINT		hint;
	OV_VTBLPTR_ov_object	pvtable;
	/*
	*	initialization
	*/
	result->result = OV_ERR_OK;
	result->results_len = 0;
	result->results_val = NULL;
	/*
	*	if there are no items, we are done
	*/
	if(!len) {
		return;
	}
	/*
	*   test (grand) access
	*/
	if(!(pticket->vtbl->getaccess(pticket) & OV_AC_RENAMEABLE) ) {
		result->result = OV_ERR_NOACCESS;
		return;
	}
	/*
	*	allocate memory for the results
	*/
	presult = (OV_RESULT*)ov_memstack_alloc(len*sizeof(OV_RESULT));
	if(!presult) {
		result->result = OV_ERR_TARGETGENERIC;	/* TODO! no heap memory */
		return;
	}
	result->results_val = presult;
	result->results_len = len;
	/*
	*	iterate over the items given
	*/
	for(i=0; i<len; i++, pitem++, presult++) {
		/*
		*	resolve the old object's path (which must be absolute)
		*/
		*presult = ov_path_resolve(&path, NULL, pitem->old_path, version);
		if(Ov_Fail(*presult)) {
			continue;
		}
		/*
		*	test, if it is a valid object
		*/
		if(path.elements[path.size-1].elemtype != OV_ET_OBJECT) {
			*presult = OV_ERR_CANTMOVE;
			continue;
		}
		pobj = path.elements[path.size-1].pobj;
		/*
		*	test if we have rename access on the object
		*/
		Ov_GetVTablePtr(ov_object, pvtable, pobj);
		if(!pvtable) {
			pvtable = (OV_VTBLPTR_ov_object)pclass_ov_object->v_pvtable;
		}
		if(!(pvtable->m_getaccess(pobj, &path.elements[path.size-1],
			pticket) & OV_AC_RENAMEABLE)
		) {
			*presult = OV_ERR_NOACCESS;
			continue;
		}
		/*
		*	test if we have write access on the domain containing the object
		*/
		pdom = Ov_GetParent(ov_containment, pobj);
		if(!pdom) {
			*presult = OV_ERR_CANTMOVE;
			continue;
		}
		Ov_GetVTablePtr(ov_object, pvtable, Ov_PtrUpCast(ov_object, pdom));
		if(!pvtable) {
			pvtable = (OV_VTBLPTR_ov_object)pclass_ov_object->v_pvtable;
		}
		if(!(pvtable->m_getaccess(Ov_PtrUpCast(ov_object, pdom),
			&path.elements[path.size-2], pticket) & OV_AC_WRITE)
		) {
			*presult = OV_ERR_NOACCESS;
			continue;
		}
		/*
		*	now take a look at the path of the new object. We got to separate
		*	the path to the domain object which shall contain the new object
		*	and the identifier of the new object.
		*/
		newobjpath = pitem->new_path;
		for(identifier=newobjpath+strlen(newobjpath)-1; identifier>=newobjpath; identifier--) {
			if(*identifier == '/') {
				*identifier = 0;
				identifier++;
				break;
			}
		}
		if(identifier<newobjpath) {
			*presult = OV_ERR_BADPATH;
			continue;
		}
		if(!*newobjpath) {
			/*
			*	new_path is "/xxxx" and we removed the '/';
			*/
			newobjpath = "/";
		}
		/*
		*	resolve the path to the domain containing the new object
		*/
		*presult = ov_path_resolve(&newpath, &path, newobjpath, version);
		if(Ov_Fail(*presult)) {
			continue;
		}
		/*
		*	test, if it is a domain
		*/
		if(newpath.elements[newpath.size-1].elemtype != OV_ET_OBJECT) {
			*presult = OV_ERR_BADPATH;
			continue;
		}
		pdom = Ov_DynamicPtrCast(ov_domain, newpath.elements[newpath.size-1].pobj);
		if(!pdom) {
			*presult = OV_ERR_BADPATH;
			continue;
		}
		/*
		*	test if we have write access on the domain
		*/
		Ov_GetVTablePtr(ov_object, pvtable, Ov_PtrUpCast(ov_object, pdom));
		if(!pvtable) {
			pvtable = (OV_VTBLPTR_ov_object)pclass_ov_object->v_pvtable;
		}
		if(!(pvtable->m_getaccess(Ov_PtrUpCast(ov_object, pdom),
			&newpath.elements[newpath.size-1], pticket) & OV_AC_WRITE)
		) {
			*presult = OV_ERR_NOACCESS;
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
				*presult = ov_path_resolve(&path, NULL,
					pitem->place.place_path, version);
				if(Ov_Fail(presult)) {
					continue;
				}
				if(path.elements[path.size-1].elemtype != OV_ET_OBJECT) {
					*presult = OV_ERR_BADPLACEMENT;
					continue;
				}
				prelchild = path.elements[path.size-1].pobj;
				if(Ov_GetParent(ov_containment, prelchild) != pdom) {
					*presult = OV_ERR_BADPLACEMENT;
					continue;
				}
			} else {
				/* relative path */
				prelchild = Ov_SearchChild(ov_containment, pdom,
					ov_path_frompercent(pitem->place.place_path));
				if(!prelchild) {
					*presult = OV_ERR_BADPLACEMENT;
					continue;
				}
			}
		}
		/*
		*	rename the object
		*/
		*presult = ov_class_renameobject(pobj, pdom, ov_path_frompercent(identifier), hint, prelchild);
	}	/* for */
	/*
	*	we are finished.
	*/
	return;
}

/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/

