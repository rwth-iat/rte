/*
*   $Id: ov_variable.c,v 1.2 1999-09-15 10:48:22 dirk Exp $
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
*	09-Apr-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*/

#define OV_COMPILE_LIBOV

#include "libov/ov_variable.h"
#include "libov/ov_structure.h"
#include "libov/ov_logfile.h"
#include "libov/ov_string.h"
#include "libov/ov_macros.h"

/*	----------------------------------------------------------------------	*/

/*
*	Load a variable into the database
*/
OV_RESULT ov_variable_load(
	OV_VARIABLE_DEF*		pvardef,
	OV_INSTPTR_ov_domain	pparent
) {
	/*
	*	local variables
	*/
	OV_RESULT				result;
	OV_INSTPTR_ov_variable	pvar;
	OV_INSTPTR_ov_structure	pstruct;
	/*
	*	if the variable is a structure, look for the definition
	*/
	/*
	*	FIXME! look for structure with name "libraryname/structurename"
	*/
	if(pvardef->structurename) {
		pstruct = ov_structure_search(pvardef->structurename);
		if(pstruct) {
			goto CONTINUE;
		}
		return OV_ERR_UNKNOWNSTRUCTDEF;
	} else {
		pstruct = NULL;
	}
	/*
	*	create variable object
	*/
CONTINUE:
	result = Ov_CreateObject(ov_variable, pvar, pparent, pvardef->identifier);
	if(Ov_Fail(result)) {
		return result;
	}
	/*
	*	set parameters
	*/
	pvar->v_veclen = pvardef->veclen;
	pvar->v_varprops = pvardef->varprops;
	pvar->v_vartype = pvardef->vartype;
	ov_string_setvalue(&pvar->v_ctypename, pvardef->ctypename);
	pvar->v_size = pvardef->size;
	pvar->v_offset = pvardef->offset;
	pvar->v_flags = pvardef->flags;
	ov_string_setvalue(&pvar->v_comment, pvardef->comment);
	ov_string_setvalue(&pvar->v_techunit, pvardef->techunit);
	pvar->v_getfnc = pvardef->getfnc;
	pvar->v_setfnc = pvardef->setfnc;
	/*
	*	link with structure object if it is a structure
	*/
	if(pstruct) {
		Ov_WarnIfNot(Ov_OK(Ov_Link(ov_construction, pstruct, pvar)));
	}
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
*	Compare a variable with its definition
*/
OV_RESULT ov_variable_compare(
	OV_INSTPTR_ov_variable	pvar,
	OV_VARIABLE_DEF*		pvardef
) {
	/*
	*	local variables
	*/
	OV_INSTPTR_ov_structure	pstruct;
	OV_STRING				id, libid;
	/*
	*	compare name of the structure
	*/
	pstruct = Ov_GetParent(ov_construction, pvar);
	if(pvardef->structurename) {
		if(pstruct) {
			libid = Ov_GetParent(ov_containment, pstruct)->v_identifier;
			id = (OV_STRING)Ov_HeapMalloc(strlen(libid)+1+strlen(pstruct->v_identifier)+1);
			if(!id) {
				return OV_ERR_HEAPOUTOFMEMORY;
			}
			sprintf(id, "%s/%s", libid, pstruct->v_identifier);
			if(strcmp(pvardef->structurename, id)) {
				Ov_HeapFree(id);
				goto ERRORMSG;
			}
			Ov_HeapFree(id);
		} else {
			goto ERRORMSG;
		}
	} else {
		if(pstruct) {
			goto ERRORMSG;
		}
	}
	/*
	*	compare variable attributes
	*/
	if(strcmp(pvar->v_identifier, pvardef->identifier)
		|| (pvar->v_veclen != pvardef->veclen)
		|| (pvar->v_vartype != pvardef->vartype)
		|| (pvar->v_varprops != pvardef->varprops)
		|| (pvar->v_size != pvardef->size)
		|| (pvar->v_offset != pvardef->offset)
	) {
		goto ERRORMSG;
	}
	if(pvar->v_ctypename) {
		if(!pvardef->ctypename) {
			goto ERRORMSG;
		}
		if(strcmp(pvar->v_ctypename, pvardef->ctypename)) {
			goto ERRORMSG;
		}
	}
	/*
	*	all is fine, load function pointers and stuff, then exit
	*/
	pvar->v_flags = pvardef->flags;
	ov_string_setvalue(&pvar->v_comment, pvardef->comment);
	ov_string_setvalue(&pvar->v_techunit, pvardef->techunit);
	pvar->v_getfnc = pvardef->getfnc;
	pvar->v_setfnc = pvardef->setfnc;
	return OV_ERR_OK;
	/*
	*	print error message and exit
	*/
ERRORMSG:
	ov_logfile_error("Error loading variable \"%s/%s/%s\".",
		Ov_GetParent(ov_containment, Ov_GetParent(ov_containment, pvar))->v_identifier,
		Ov_GetParent(ov_containment, pvar)->v_identifier,
		pvar->v_identifier);
	return OV_ERR_VARDEFMISMATCH;
}

/*	----------------------------------------------------------------------	*/

/*
*	Test if we can unload a variable from the database
*/
OV_BOOL ov_variable_canunload(
	OV_INSTPTR_ov_variable		pvar
) {
	return TRUE;
}

/*	----------------------------------------------------------------------	*/

/*
*	Accessor functions
*/
OV_DLLFNCEXPORT OV_INT ov_variable_vartype_get(
	OV_INSTPTR_ov_variable	pvar
) {
	return pvar->v_vartype;
}

OV_DLLFNCEXPORT OV_STRING ov_variable_ctypename_get(
	OV_INSTPTR_ov_variable	pvar
) {
	return pvar->v_ctypename;
}

OV_DLLFNCEXPORT OV_INT ov_variable_varprops_get(
	OV_INSTPTR_ov_variable	pvar
) {
	return pvar->v_varprops;
}

OV_DLLFNCEXPORT OV_UINT ov_variable_veclen_get(
	OV_INSTPTR_ov_variable	pvar
) {
	return pvar->v_veclen;
}

OV_DLLFNCEXPORT OV_STRING ov_variable_comment_get(
	OV_INSTPTR_ov_variable	pvar
) {
	return pvar->v_comment;
}

OV_DLLFNCEXPORT OV_UINT ov_variable_flags_get(
	OV_INSTPTR_ov_variable	pvar
) {
	return pvar->v_flags;
}

OV_DLLFNCEXPORT OV_STRING ov_variable_techunit_get(
	OV_INSTPTR_ov_variable	pvar
) {
	return pvar->v_techunit;
}

/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/

