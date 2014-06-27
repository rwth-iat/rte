/* 
*   $Id: ov_variable.c,v 1.5 2002-01-23 13:44:14 ansgar Exp $
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
*	04-Nov-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: variable type ANY added.
*	06-Dez-2001 Ansgar Münnemann <ansgar@plt.rwth-aachen.de>: initialvalue of variable added.
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

OV_DLLFNCEXPORT OV_ANY* ov_variable_initialvalue_get(
	OV_INSTPTR_ov_variable	pvar
) {
	return &pvar->v_initialvalue;
}

/*	----------------------------------------------------------------------	*/

/*
*	Helper macros for implementing ov_setvariable_setanyvalue()
*/
#define Ov_Variable_SetAnyValueVec(type, TYPE)								\
	case OV_VT_##TYPE##_VEC:												\
		value.value.valueunion.val_generic_vec.veclen = 0;					\
		value.value.valueunion.val_generic_vec.value = NULL;				\
		result = Ov_SetDynamicVectorValue(									\
			&value.value.valueunion.val_##type##_vec,						\
			pvalue->value.valueunion.val_##type##_vec.value,				\
			pvalue->value.valueunion.val_##type##_vec.veclen, TYPE);		\
		break

#define Ov_Variable_FreeAnyValueVec(type, TYPE)								\
	case OV_VT_##TYPE##_VEC:												\
		Ov_WarnIfNot(Ov_OK(Ov_SetDynamicVectorValue(						\
			&pany->value.valueunion.val_##type##_vec, NULL, 0, TYPE)));		\
		break

/*	----------------------------------------------------------------------	*/

/*
*	Set the value of an ANY variable
*/
OV_DLLFNCEXPORT OV_RESULT ov_variable_setanyvalue(
	OV_ANY			*pany,
	const OV_ANY	*pvalue
) {
	/*
	*	internal variables
	*/
	OV_ANY		value;
	OV_RESULT	result = OV_ERR_OK;
	/*
	*	instructions
	*/
	if(!pany) {
		return OV_ERR_BADPARAM;
	}
	if(pvalue) {
		value = *pvalue;
	} else {
		value.value.vartype = OV_VT_VOID;
	}
	switch(value.value.vartype & OV_VT_KSMASK) {
	case OV_VT_VOID:
	case OV_VT_BOOL:
	case OV_VT_INT:
	case OV_VT_UINT:
	case OV_VT_SINGLE:
	case OV_VT_DOUBLE:
	case OV_VT_TIME:
	case OV_VT_TIME_SPAN:
		break;
	case OV_VT_STRING:
		value.value.valueunion.val_string = NULL;
		result = ov_string_setvalue(&value.value.valueunion.val_string,
			pvalue->value.valueunion.val_string);
		break;
	Ov_Variable_SetAnyValueVec(byte, BYTE);
	Ov_Variable_SetAnyValueVec(bool, BOOL);
	Ov_Variable_SetAnyValueVec(int, INT);
	Ov_Variable_SetAnyValueVec(uint, UINT);
	Ov_Variable_SetAnyValueVec(single, SINGLE);
	Ov_Variable_SetAnyValueVec(double, DOUBLE);
	Ov_Variable_SetAnyValueVec(time, TIME);
	Ov_Variable_SetAnyValueVec(time_span, TIME_SPAN);
	Ov_Variable_SetAnyValueVec(string, STRING);
	default:
		return OV_ERR_BADVALUE;
	}
	if(Ov_OK(result)) {
		/*
		*	free the memory used by the old ANY value
		*/
		switch(pany->value.vartype & OV_VT_KSMASK) {
		case OV_VT_STRING:
			ov_string_setvalue(&pany->value.valueunion.val_string, NULL);
			break;
		Ov_Variable_FreeAnyValueVec(byte, BYTE);
		Ov_Variable_FreeAnyValueVec(bool, BOOL);
		Ov_Variable_FreeAnyValueVec(int, INT);
		Ov_Variable_FreeAnyValueVec(uint, UINT);
		Ov_Variable_FreeAnyValueVec(single, SINGLE);
		Ov_Variable_FreeAnyValueVec(double, DOUBLE);
		Ov_Variable_FreeAnyValueVec(time, TIME);
		Ov_Variable_FreeAnyValueVec(time_span, TIME_SPAN);
		Ov_Variable_FreeAnyValueVec(string, STRING);
		default:
			break;
		}
		*pany = value;
	}
	return result;
}

/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/

