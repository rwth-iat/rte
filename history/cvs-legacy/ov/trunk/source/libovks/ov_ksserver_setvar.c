/*
*   $Id: ov_ksserver_setvar.c,v 1.7 2002-01-29 15:36:07 ansgar Exp $
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
*	26-Apr-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: Major revision.
*/

#define OV_COMPILE_LIBOVKS

#include "libovks/ov_ksserver.h"
#include "libov/ov_path.h"
#include "libov/ov_macros.h"

/*	----------------------------------------------------------------------	*/

/*
*	Execute the SetVar service (subroutine)
*/
void ov_ksserver_setvar(
	const OV_UINT		version,
	const OV_TICKET		*pticket,
	const OV_SETVAR_PAR	*params,
	OV_SETVAR_RES		*result
) {
	/*
	*	local variables
	*/
	OV_UINT			len = params->items_len;
	OV_RESULT		*presult;
	OV_SETVAR_ITEM	*pitem = params->items_val;
	OV_PATH			lastpath, path;
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
	if(!(pticket->vtbl->getaccess(pticket) & OV_AC_WRITE) ) {
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
		*	resolve path to variable item, the first time the path is absolute
		*/
		if(i) {
			*presult = ov_path_resolve(&path, &lastpath, pitem->path_and_name, version);
		} else {
			*presult = ov_path_resolve(&path, NULL, pitem->path_and_name, version);
		}
		if(Ov_OK(*presult)) {
			/*
			*	path is ok, set variable item
			*/
			*presult = ov_ksserver_setvar_setitem(pticket, pitem,
				&path.elements[path.size-1], FALSE);
		} else {
			/*
			*	path is bad, skip all following relative paths
			*/
			while(i+1<len) {
				if(*((pitem+1)->path_and_name) == '/') {
					break;
				}
				i++;
				pitem++;
				presult++;
				*presult = OV_ERR_BADPATH;
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
*	Set a variable item of the SetVar service (subroutine)
*/
OV_RESULT ov_ksserver_setvar_setitem(
	const OV_TICKET		*pticket,
	OV_SETVAR_ITEM		*pitem,
	const OV_ELEMENT	*pelem,
	OV_BOOL				init
) {
	/*
	*	local variables
	*/
	OV_INSTPTR_ov_object	pobj = pelem->pobj;
	OV_VTBLPTR_ov_object	pvtable;
	/*
	*	get the vtable pointer of the object the variable belongs to
	*/
	Ov_GetVTablePtr(ov_object, pvtable, pobj);
	if((!pvtable) || (activitylock)){
		pvtable = pclass_ov_object->v_pvtable;
	}
	/*
	*	test if we have access to this variable
	*/
	if(init) {
		/*
		*	we are during initialization (instantiation) and need read OR write access
		*/
		if(!(pvtable->m_getaccess(pobj, pelem, pticket) & OV_AC_READWRITE)) {
			return OV_ERR_NOACCESS;
		}
	} else {
		/*
		*	we are at normal lifetime of the object
		*/
		if(!(pvtable->m_getaccess(pobj, pelem, pticket) & OV_AC_WRITE)) {
			return OV_ERR_NOACCESS;
		}
	}
	/*
	*	switch based on the element's type
	*/
	switch(pelem->elemtype) {
	case OV_ET_OBJECT:
		/*
		*	object may be a vendor object
		*/
		if((pelem->pobj < &pdb->vendorobj[OV_NUM_VENDOROBJECTS])
			&& (pelem->pobj >= &pdb->vendorobj[0])
		) {
			return ov_vendortree_setvar(pobj, &pitem->var_current_props, pticket);
		}
		break;
	case OV_ET_MEMBER:
	case OV_ET_VARIABLE:
		/*
		*	set the variable's value, timestamp and time
		*/
		pitem->var_current_props.value.vartype |= (OV_VT_HAS_STATE | OV_VT_HAS_TIMESTAMP);
		return (pvtable->m_setvar)(pobj, pelem, &pitem->var_current_props);
	default:
		break;
	}
	return OV_ERR_BADOBJTYPE;
}

/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/

