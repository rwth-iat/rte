/*
*   $Id: ov_ksserver_link.c,v 1.1.1.1 2011-01-31 07:47:43 henning Exp $
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
*	17-Dez-1998 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*	27-Apr-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: Major revision.
*/

#include "ov_ksserver_stripped.h"

#include "libov/ov_path.h"
#include "libov/ov_macros.h"

/*	----------------------------------------------------------------------	*/

/*
*	Execute the Link service (subroutine)
*/
OV_DLLFNCEXPORT void ov_ksserver_link(
	const OV_UINT		version,
	const OV_TICKET		*pticket,
	const OV_LINK_PAR	*params,
	OV_LINK_RES			*result
) {
	/*
	*	local variables
	*/
	OV_UINT			len = params->items_len;
	OV_RESULT		*presult;
	OV_LINK_ITEM	*pitem = params->items_val;
	OV_PATH			linkpath, elementpath, placepath, oppositeplacepath;
	OV_UINT			i;
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
	if(!(pticket->vtbl->getaccess(pticket) & OV_AC_LINKABLE) ) {
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
		*	resolve link path and element path (both absolute)
		*/
		*presult = ov_path_resolve(&linkpath, NULL, pitem->link_path, 
			version);
		if(Ov_Fail(*presult)) {
			continue;
		}
		*presult = ov_path_resolve(&elementpath, NULL, pitem->element_path,
			version);
		if(Ov_Fail(*presult)) {
			continue;
		}
		/*
		*	if relative placement, resolve place path
		*/
		if((pitem->place.hint == OV_PMH_BEFORE)
			|| (pitem->place.hint == OV_PMH_AFTER)
		) {
			*presult = ov_path_resolve(&placepath, NULL,
				pitem->place.place_path, version);
			if(Ov_Fail(*presult)) {
				continue;
			}
		}
		/*
		*	if relative placement, resolve opposite place path
		*/
		if((pitem->opposite_place.hint == OV_PMH_BEFORE)
			|| (pitem->opposite_place.hint == OV_PMH_AFTER)
		) {
			*presult = ov_path_resolve(&oppositeplacepath, NULL,
				pitem->opposite_place.place_path, version);
			if(Ov_Fail(*presult)) {
				continue;
			}
		}
		/*
		*	link the item
		*/
		*presult = ov_ksserver_link_linkitem(pticket,
			&linkpath.elements[linkpath.size-1],
			&elementpath.elements[elementpath.size-1],
			&placepath.elements[placepath.size-1],
			&oppositeplacepath.elements[oppositeplacepath.size-1],
			pitem->place.hint, pitem->opposite_place.hint);
	}
}

/*	----------------------------------------------------------------------	*/

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
) {
	/*
	*	local variables
	*/
	OV_INSTPTR_ov_object	pparent;
	OV_INSTPTR_ov_object	pchild;
	OV_INSTPTR_ov_object	prelparent;
	OV_INSTPTR_ov_object	prelchild;
	OV_PLACEMENT_HINT		parenthint;
	OV_PLACEMENT_HINT		childhint;
	/*
	*	test element types and initialize parent, child and relchild pointer
	*/
	if(pelementelem->elemtype != OV_ET_OBJECT) {
		return OV_ERR_BADOBJTYPE;
	}
	switch(plinkelem->elemtype) {
	case OV_ET_PARENTLINK:
		pparent = plinkelem->pobj;
		pchild = pelementelem->pobj;
		parenthint = oppositehint;
		childhint = hint;
		if((hint == OV_PMH_BEFORE) || (hint == OV_PMH_AFTER)) {
			if(pplaceelem->elemtype != OV_ET_OBJECT) {
				return OV_ERR_BADOBJTYPE;
			}
			prelchild = pplaceelem->pobj;
		} else {
			prelchild = NULL;
		}
		if((oppositehint == OV_PMH_BEFORE) || (oppositehint == OV_PMH_AFTER)) {
			if(poppositeplaceelem->elemtype != OV_ET_OBJECT) {
				return OV_ERR_BADOBJTYPE;
			}
			prelparent = poppositeplaceelem->pobj;
		} else {
			prelparent = NULL;
		}
		/*
		*	test if child can participate in the association
		*/
		if(!ov_class_cancastto(Ov_GetParent(ov_instantiation, pchild),
			Ov_GetParent(ov_childrelationship, plinkelem->elemunion.passoc))
		) {
			return OV_ERR_BADOBJTYPE;
		}
		break;
	case OV_ET_CHILDLINK:
		pparent = pelementelem->pobj;
		pchild = plinkelem->pobj;
		parenthint = hint;
		childhint = oppositehint;
		if((hint == OV_PMH_BEFORE) || (hint == OV_PMH_AFTER)) {
			if(pplaceelem->elemtype != OV_ET_OBJECT) {
				return OV_ERR_BADOBJTYPE;
			}
			prelparent = pplaceelem->pobj;
		} else {
			prelparent = NULL;
		}
		if((oppositehint == OV_PMH_BEFORE) || (oppositehint == OV_PMH_AFTER)) {
			if(poppositeplaceelem->elemtype != OV_ET_OBJECT) {
				return OV_ERR_BADOBJTYPE;
			}
			prelchild = poppositeplaceelem->pobj;
		} else {
			prelchild = NULL;
		}
		/*
		*	test if parent can participate in the association
		*/
		if(!ov_class_cancastto(Ov_GetParent(ov_instantiation, pparent),
			Ov_GetParent(ov_parentrelationship, plinkelem->elemunion.passoc))
		) {
			return OV_ERR_BADOBJTYPE;
		}
		break;
	default:			
		return OV_ERR_BADOBJTYPE;
	}
	/*
	*	test if relparent and relchild can participate in the association
	*/
	if(prelparent) {
		if(!ov_class_cancastto(Ov_GetParent(ov_instantiation, prelparent),
			Ov_GetParent(ov_parentrelationship, plinkelem->elemunion.passoc))
		) {
			return OV_ERR_BADPLACEMENT;
		}
	}
	if(prelchild) {
		if(!ov_class_cancastto(Ov_GetParent(ov_instantiation, prelchild),
			Ov_GetParent(ov_childrelationship, plinkelem->elemunion.passoc))
		) {
			return OV_ERR_BADPLACEMENT;
		}
	}
	/*
	*	check for access rights
	*/
	if((plinkelem->elemunion.passoc->v_getaccessfnc) && (!ov_activitylock)) {
		if(!(plinkelem->elemunion.passoc->v_getaccessfnc(pparent, pchild, pticket)
			& OV_AC_LINKABLE)
		) {
			return OV_ERR_NOACCESS;
		}
	}
	/*
	*	link child with parent relative to relchild
	*/
	if(plinkelem->elemunion.passoc->v_linkfnc) {
		if (ov_activitylock) {
			return ov_association_link(plinkelem->elemunion.passoc, pparent, pchild, parenthint,	prelparent, childhint, prelchild);
		}
		return plinkelem->elemunion.passoc->v_linkfnc(pparent, pchild, parenthint,
			prelparent, childhint, prelchild);
	} else	return ov_association_link(plinkelem->elemunion.passoc, pparent, pchild, parenthint,	prelparent, childhint, prelchild);
}

/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/

