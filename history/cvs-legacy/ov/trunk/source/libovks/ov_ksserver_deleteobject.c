/*
*   $Id: ov_ksserver_deleteobject.c,v 1.1 1999-07-19 15:02:16 dirk Exp $
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
*	03-Dec-1998 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*	21-Apr-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: Major revision.
*/

#define OV_COMPILE_LIBOVKS

#include "libovks/ov_ksserver.h"
#include "libov/ov_path.h"
#include "libov/ov_macros.h"

/*	----------------------------------------------------------------------	*/

/*
*	Execute the DeleteObject service (subroutine)
*/
void ov_ksserver_deleteobject(
	const OV_UINT				version,
	const OV_TICKET				*pticket,
	const OV_DELETEOBJECT_PAR	*params,
	OV_DELETEOBJECT_RES			*result
) {
	/*
	*	local variables
	*/
	OV_UINT					len = params->paths_len;
	OV_RESULT				*presult;
	OV_STRING				*ppath = params->paths_val;
	OV_PATH					path;
	OV_UINT					i;
	OV_INSTPTR_ov_object	pobj;
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
	*	allocate memory for the results
	*/
	presult = (OV_RESULT*)ov_memstack_alloc(len*sizeof(OV_RESULT));
	if(!presult) {
		result->result = OV_ERR_TARGETGENERIC;	/* TODO! out of heap memory */
		return;
	}
	result->results_val = presult;
	result->results_len = len;
	/*
	*	iterate over the paths given
	*/
	for(i=0; i<len; i++, ppath++, presult++) {
		/*
		*	resolve path (absolute)
		*/
		*presult = ov_path_resolve(&path, NULL, *ppath, version);
		if(Ov_Fail(*presult)) {
			continue;
		}
		/*
		*	check if path is an object path
		*/
		if(path.elements[path.size-1].elemtype != OV_ET_OBJECT) {
			*presult = OV_ERR_BADOBJTYPE;
			continue;
		}
		pobj = path.elements[path.size-1].pobj;
		/*
		*	get the vtable pointer of the object the variable belongs to
		*/
		Ov_GetVTablePtr(ov_object, pvtable, pobj);
		if(!pvtable) {
			pvtable = pclass_ov_object->v_pvtable;
		}
		/*
		*	test if we have access to this variable
		*/
		if(!(((pvtable->m_getaccess)(pobj, &path.elements[path.size-1], pticket))
			& OV_AC_DELETEABLE)
		) {
			*presult = OV_ERR_NOACCESS;
			continue;
		}
		/*
		*	delete the object
		*/
		ov_class_deleteobject(pobj);
	}
}

/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/

