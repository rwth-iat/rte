/*
*   $Id: ov_ksserver_gethist.c,v 1.2 2011-06-01 14:08:19 lars Exp $
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
*	20-Jul-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*	09-Apr-2001 Ansgar M�nnemann <ansgar@plt.rwth-aachen.de>: Gethistory-service-wrapper
*/


#include "ov_ksserver_backend.h"

#include "libov/ov_library.h"
#include "libov/ov_path.h"
#include "libov/ov_string.h"
#include "libov/ov_time.h"
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
*	Function prototype for method gethistory 
*	(compare ACPLT/KS)
*/
typedef OV_RESULT OV_FNC_GETHIST (
  OV_INSTPTR_ov_object pobj,
  OV_UINT items_len,	
  OV_GETHIST_ITEM* pitem,
  OV_GETHISTRESULT_ITEM* presultitem,
  OV_UINT max_answers	
);
#else
#include "kshist.h"
#endif


/*	----------------------------------------------------------------------	*/



/*	----------------------------------------------------------------------	*/

/*
*	Execute the GetHist service (subroutine)
*/
OV_DLLFNCEXPORT void ov_ksserver_gethist(
	const OV_UINT			version,
	const OV_TICKET			*pticket,
	const OV_GETHIST_PAR		*params,
	OV_GETHIST_RES			*result
) {
	/*
	*	local variables
	*/
	OV_GETHISTSINGLERESULT	*psingleresult;
	OV_GETHISTRESULT_ITEM	*presultitem;
	OV_UINT			j;
	OV_PATH			path;
	OV_INSTPTR_ov_class	pclass;
	OV_ELEMENT		child;
	OV_ELEMENT 		searchedelement;
	OV_FNC_GETHIST		*gethist = NULL;
#if OV_DYNAMIC_LIBRARIES
	OV_INSTPTR_ov_library	plib;
	char			tmpstring[256];
#endif
	/*
	*	check access rights
	*/
	if(!(pticket->vtbl->getaccess(pticket) & OV_AC_READ)) {
		result->result = OV_ERR_NOACCESS;
		return;
	}

	result->result = OV_ERR_BADPARAM;
	if((params->paths_len < 1) || (params->items_len < 1)) {
		return;
	}

	/*
	*	allocate memory for the paths_len number of single result items
	*/
	psingleresult = (OV_GETHISTSINGLERESULT*)ov_memstack_alloc(sizeof(OV_GETHISTSINGLERESULT) * params->paths_len);
	if(!psingleresult) {
		result->result = OV_ERR_TARGETGENERIC;
		return;
	}
	result->result = OV_ERR_OK;
	result->results_val = psingleresult;
	result->results_len = params->paths_len;
	/*
	*	further initialization
	*/
	psingleresult->items_len = 0;
	psingleresult->items_val = NULL;

	for(j=0; j<params->paths_len; j++, psingleresult++) {
		/*
		*	resolve path (must be absolute)
		*/
		psingleresult->result = ov_path_resolve(&path, NULL, params->paths_val[j], version);
		if(Ov_Fail(result->result)) {
			return;
		}
		psingleresult->result = OV_ERR_BADTYPE;
		if (path.elements[path.size-1].elemtype == OV_ET_OBJECT) {
			pclass=Ov_GetParent(ov_instantiation, path.elements[path.size-1].pobj);
			while (pclass && (ov_string_compare(pclass->v_identifier, "KsHistoryRO")!=0)) 
				pclass=Ov_GetParent(ov_inheritance, pclass);
			if (!pclass) {
				return;
			}

			pclass=Ov_GetParent(ov_instantiation, path.elements[path.size-1].pobj);
			child.elemtype = OV_ET_OBJECT;
			child.pobj = Ov_PtrUpCast(ov_object, pclass);
			while (pclass) {
				/*
				*	search for operation element "gethist"
				*/
				if (Ov_OK(ov_element_searchpart(&child, &searchedelement, OV_ET_OPERATION, "gethist"))) break;
				pclass=Ov_GetParent(ov_inheritance, pclass);
				child.pobj = Ov_PtrUpCast(ov_object, pclass);
			}
			
			if (!pclass) {
				return;
			}

		}
		else {
			return;
		}
		
		/*
		*	allocate memory for the result items
		*/
		presultitem = (OV_GETHISTRESULT_ITEM*)ov_memstack_alloc(params->items_len*sizeof(OV_GETHISTRESULT_ITEM));
		if(!presultitem) {
			psingleresult->result = OV_ERR_TARGETGENERIC;
			return;
		}
		psingleresult->items_val = presultitem;
		psingleresult->items_len = params->items_len;

#if OV_DYNAMIC_LIBRARIES
		plib=Ov_StaticPtrCast(ov_library,Ov_GetParent(ov_containment, Ov_PtrUpCast(ov_object,pclass)));
		sprintf(tmpstring, "%s_%s_%s", plib->v_identifier, pclass->v_identifier, "gethist");
		gethist = (OV_FNC_GETHIST*)Ov_Library_GetAddr((OV_DLLHANDLE)plib->v_handle, tmpstring);
#else
		gethist = (OV_FNC_GETHIST*) ((OV_VTBLPTR_kshist_KsHistoryRO) pclass->v_pvtable)->m_gethist;
#endif				
		psingleresult->result = gethist(path.elements[path.size-1].pobj,params->items_len, params->items_val,presultitem, params->max_answers);
	}

	return;
}

/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/

