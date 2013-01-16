/*
*   $Id: ov_ksserver_unlink.c,v 1.1.1.1 2011-01-31 07:47:43 henning Exp $
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
*	08-Jan-1998 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*	27-Apr-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: Major revision.
*/


#include "ov_ksserver_stripped.h"
#include "libov/ov_path.h"
#include "libov/ov_macros.h"

/*	----------------------------------------------------------------------	*/

/*
*	Execute the Unlink service (subroutine)
*/
void ov_ksserver_unlink(
	const OV_UINT		version,
	const OV_TICKET		*pticket,
	const OV_UNLINK_PAR	*params,
	OV_UNLINK_RES		*result
) {
	/*
	*	local variables
	*/
	OV_UINT			len = params->items_len;
	OV_RESULT		*presult;
	OV_UNLINK_ITEM	*pitem = params->items_val;
	OV_PATH			linkpath, elementpath;
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
	if(!(pticket->vtbl->getaccess(pticket) & OV_AC_UNLINKABLE) ) {
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
		*	unlink the item
		*/
		*presult = ov_ksserver_unlink_unlinkitem(pticket,
			&linkpath.elements[linkpath.size-1],
			&elementpath.elements[elementpath.size-1]);
	}
}

/*	----------------------------------------------------------------------	*/

/*
*	Unlink an unlink item of the Unlink service (subroutine)
*/
OV_RESULT ov_ksserver_unlink_unlinkitem(
	const OV_TICKET		*pticket,
	OV_ELEMENT			*plinkelem,
	OV_ELEMENT			*pelementelem
) {
	/*
	*	local variables
	*/
	OV_INSTPTR_ov_object	pparent;
	OV_INSTPTR_ov_object	pchild;
	OV_INSTPTR_ov_object	pcurrchild;
	Ov_Association_DefineIteratorNM(pit);
	/*
	*	test element types and initialize parent and child pointer
	*/
	if(pelementelem->elemtype != OV_ET_OBJECT) {
		return OV_ERR_BADOBJTYPE;
	}
	switch(plinkelem->elemtype) {
	case OV_ET_PARENTLINK:
		pparent = plinkelem->pobj;
		pchild = pelementelem->pobj;
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
		pchild = plinkelem->pobj;
		pparent = pelementelem->pobj;
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
	*	check for access rights
	*/
	if(plinkelem->elemunion.passoc->v_getaccessfnc) {
		if(!(plinkelem->elemunion.passoc->v_getaccessfnc(pparent, pchild, pticket)
			& OV_AC_UNLINKABLE)
		) {
			return OV_ERR_NOACCESS;
		}
	}
	/*
	*	check, if there is a link between that parent and child
	*/
	switch(plinkelem->elemunion.passoc->v_assoctype) {
	case OV_AT_ONE_TO_ONE:
		if(Ov_Association_GetParent(plinkelem->elemunion.passoc, pchild) != pparent) {
			return OV_ERR_BADPATH;	/* FIXME! error code? */
		}
		break;
	case OV_AT_ONE_TO_MANY:
		if(Ov_Association_GetParent(plinkelem->elemunion.passoc, pchild) != pparent) {
			return OV_ERR_BADPATH;	/* FIXME! error code? */
		}
		break;
	case OV_AT_MANY_TO_MANY:
		Ov_Association_ForEachChildNM(plinkelem->elemunion.passoc, pit, pparent, pcurrchild) {
			if(pcurrchild == pchild) {
				break;
			}
		}
		if(!pit) {
			return OV_ERR_BADPATH;	/* FIXME! error code? */
		}
		break;
	default:
		Ov_Warning("no such association type");
		return OV_ERR_GENERIC;
	}
	/*
	*	unlink child from parent
	*/
	if(plinkelem->elemunion.passoc->v_unlinkfnc) {
		if (ov_activitylock) ov_association_unlink(plinkelem->elemunion.passoc, pparent, pchild);
		else plinkelem->elemunion.passoc->v_unlinkfnc(pparent, pchild);
	}
	else ov_association_unlink(plinkelem->elemunion.passoc, pparent, pchild);
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/

