/*
*   $Id: ov_structure.c,v 1.1 1999-07-19 15:02:14 dirk Exp $
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
*	12-Apr-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*/

#define OV_COMPILE_LIBOV

#include "libov/ov_structure.h"
#include "libov/ov_variable.h"
#include "libov/ov_library.h"
#include "libov/ov_logfile.h"
#include "libov/ov_time.h"
#include "libov/ov_macros.h"

/*	----------------------------------------------------------------------	*/

/*
*	Search for a structure object with given identifier
*/
OV_INSTPTR_ov_structure ov_structure_search(
	OV_STRING	identifier
) {
	/*
	*	local variables
	*/
	OV_INSTPTR_ov_library	plib;
	OV_INSTPTR_ov_structure	pstruct;
	OV_STRING				libid;
	OV_STRING				structid;
	/*
	*	check parameters
	*/
	if(!identifier) {
		return NULL;
	}
	/*
	*	allocate memory for the library and the structure identifier
	*/
	libid = Ov_HeapStrdup(identifier);
	if(!libid) {
		return NULL;
	}
	/*
	*	search for '/'
	*/
	for(structid = libid; *structid; structid++) {
		if(*structid == '/') {
			*structid = 0;
			structid++;
			break;
		}
	}
	/*
	*	search for the library
	*/
	plib = ov_library_search(libid);
	if(!plib) {
		Ov_HeapFree(libid);
		return NULL;
	}
	/*
	*	search for the structure
	*/
	pstruct = Ov_SearchChildEx(ov_containment, plib, structid, ov_structure);
	Ov_HeapFree(libid);
	return pstruct;
}

/*	----------------------------------------------------------------------	*/

/*
*	Load a structure into the database
*/
OV_RESULT ov_structure_load(
	OV_STRUCTURE_DEF*		pstructdef,
	OV_INSTPTR_ov_domain	pparent
) {
	/*
	*	local variables
	*/
	OV_RESULT				result;
	OV_INSTPTR_ov_structure	pstruct;
	OV_VARIABLE_DEF			*pvardef;
	/*
	*	test whether the structure already exists
	*/
	Ov_ForEachChildEx(ov_containment, pparent, pstruct, ov_structure) {
		if(!strcmp(pstruct->v_identifier, pstructdef->identifier)) {
			/*
			*	structure already exists; if it was created provisorically,
			*	skip object creation, otherwise it is an error
			*/
			if(pstruct->v_creationtime.secs == OV_VL_MAXUINT) {
				goto CONTINUE;
			}
			return OV_ERR_ALREADYEXISTS;
		}
	}
	/*
	*	create structure object
	*/
	result = Ov_CreateObject(ov_structure, pstruct, pparent, pstructdef->identifier);
	if(Ov_Fail(result)) {
		return result;
	}
CONTINUE:
	/*
	*	set parameters
	*/
	ov_time_gettime(&pstruct->v_creationtime);
	pstruct->v_size = pstructdef->size;
	/*
	*	load members of the structure
	*/
	for(pvardef=pstructdef->members; pvardef; pvardef=pvardef->pnext) {
		result = ov_variable_load(pvardef, Ov_PtrUpCast(ov_domain, pstruct));
		if(Ov_Fail(result)) {
			return result;
		}
	}
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
*	Compare a structure with its definition
*/
OV_RESULT ov_structure_compare(
	OV_INSTPTR_ov_structure	pstruct,
	OV_STRUCTURE_DEF*		pstructdef
) {
	/*
	*	local variables
	*/
	OV_INSTPTR_ov_object	pobj;
	OV_INSTPTR_ov_variable	pvar;
	OV_VARIABLE_DEF			*pvardef;
	OV_RESULT				result;
	/*
	*	compare structure attributes
	*/
	if(pstruct->v_size != pstructdef->size) {
		goto ERRORMSG;
	}
	/*
	*	compare members of the structure
	*/
	pvardef = pstructdef->members;
	Ov_ForEachChild(ov_containment, pstruct, pobj) {
		pvar = Ov_DynamicPtrCast(ov_variable, pobj);
		if(!pvar) {
			goto ERRORMSG;
		}
		result = ov_variable_compare(pvar, pvardef);
		if(Ov_Fail(result)) {
			goto ERRORMSG;
		}
		pvardef = pvardef->pnext;
	}
	if(pvardef) {
		goto ERRORMSG;
	}
	/*
	*	all is fine, exit
	*/
	return OV_ERR_OK;
	/*
	*	print error message and exit
	*/
ERRORMSG:
	ov_logfile_error("Error loading structure \"%s/%s\".",
		Ov_GetParent(ov_containment, pstruct)->v_identifier,
		pstruct->v_identifier);
	return OV_ERR_STRUCTDEFMISMATCH;
}

/*	----------------------------------------------------------------------	*/

/*
*	Test if we can unload a structure from the database
*/
OV_BOOL ov_structure_canunload(
	OV_INSTPTR_ov_structure	pstruct
) {
	/*
	*	local variables
	*/
	OV_INSTPTR_ov_object	pobj;
	OV_INSTPTR_ov_variable	pvar;
	OV_INSTPTR_ov_domain	plib;
	/*
	*	test if this structure is used classes or structures of other libraries
	*/
	plib = Ov_GetParent(ov_containment, pstruct);
	Ov_WarnIfNot(plib);
	Ov_ForEachChild(ov_construction, pstruct, pvar) {
		if(plib != Ov_GetParent(ov_containment, Ov_GetParent(ov_containment, pvar))) {
			return FALSE;
		}
	}
	/*
	*	test if we can unload the members
	*/
	Ov_ForEachChild(ov_containment, pstruct, pobj) {
		pvar = Ov_DynamicPtrCast(ov_variable, pobj);
		if(pvar) {
			if(!ov_variable_canunload(pvar)) {
				return FALSE;
			}
		} else {
			Ov_Warning("internal error");
			return FALSE;
		}
	}
	return TRUE;
}

/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/

