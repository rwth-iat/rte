/*
*   Copyright (C) 2014
*   Chair of Process Control Engineering,
*   D-52056 Aachen, Germany.
*   All rights reserved.
*
*
*   This file is part of the ACPLT/OV Package 
*   Licensed under the Apache License, Version 2.0 (the "License");
*   you may not use this file except in compliance with the License.
*   You may obtain a copy of the License at
*
*       http://www.apache.org/licenses/LICENSE-2.0
*
*   Unless required by applicable law or agreed to in writing, software
*   distributed under the License is distributed on an "AS IS" BASIS,
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*   See the License for the specific language governing permissions and
*   limitations under the License.
*
***********************************************************************
*
* CSHMI - Client-Side Human-Machine Interface
*
*	File:
*	------
*	classCondition_CompareIteratedChild.c
*
*	Editors:
*	--------
*	Je							Holger Jeromin <jeromin@plt.rwth-aachen.de>
*
*	SVN:
*	----
*	$Revision$
*	$Date$
*
*	History:
*	--------
*	28-December-2011			Yannick Rocks		V0.1.0
*		-	File created
*
***********************************************************************/

#ifndef OV_COMPILE_LIBRARY_cshmi
#define OV_COMPILE_LIBRARY_cshmi
#endif


#include "cshmilib.h"

OV_DLLFNCEXPORT OV_RESULT cshmi_CompareIteratedChild_childValue_set(
	OV_INSTPTR_cshmi_CompareIteratedChild          pobj,
	const OV_STRING  value
) {
	OV_STRING erroroutput = NULL;
	//force our keywords
	if (	ov_string_compare(value, "OP_NAME") == OV_STRCMP_EQUAL
		||	ov_string_compare(value, "OP_TYPE") == OV_STRCMP_EQUAL
		||	ov_string_compare(value, "OP_COMMENT") == OV_STRCMP_EQUAL
		||	ov_string_compare(value, "OP_ACCESS") == OV_STRCMP_EQUAL
		||	ov_string_compare(value, "OP_SEMANTICS") == OV_STRCMP_EQUAL
		||	ov_string_compare(value, "OP_CREATIONTIME") == OV_STRCMP_EQUAL
		||	ov_string_compare(value, "OP_CLASS") == OV_STRCMP_EQUAL
		||	ov_string_compare(value, "OP_CREATIONTIME") == OV_STRCMP_EQUAL
		||	ov_string_compare(value, "OP_TECHUNIT") == OV_STRCMP_EQUAL
		||	ov_string_compare(value, "OP_ASSOCIDENT") == OV_STRCMP_EQUAL
		||	ov_string_compare(value, "OP_ROLEIDENT") == OV_STRCMP_EQUAL
		||	ov_string_compare(value, "OP_DEFAULTINTERP") == OV_STRCMP_EQUAL
		||	ov_string_compare(value, "OP_SUPPORTEDINTERP") == OV_STRCMP_EQUAL
		||	ov_string_compare(value, "OP_TYPEIDENT") == OV_STRCMP_EQUAL
		||	ov_string_compare(value, "OP_VALUE") == OV_STRCMP_EQUAL
	){
		cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
		return ov_string_setvalue(&pobj->v_childValue,value);
	} else if (	ov_string_compare(value, "") == OV_STRCMP_EQUAL){
		//unconfigured must be allowed
		cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
		return ov_string_setvalue(&pobj->v_childValue,value);
	}else{
		ov_memstack_lock();
		ov_string_print(&erroroutput, "object %s had wrong childValue '%s'. Rejecting Variable change.", ov_path_getcanonicalpath(Ov_PtrUpCast(ov_object, pobj), 2), value);
		ov_memstack_unlock();
		ov_logfile_warning(erroroutput);
		ov_string_setvalue(&erroroutput, NULL);
		return OV_ERR_BADPARAM;
	}
}

OV_DLLFNCEXPORT OV_RESULT cshmi_CompareIteratedChild_comptype_set(
	OV_INSTPTR_cshmi_CompareIteratedChild          pobj,
	const OV_STRING  value
) {
	OV_STRING erroroutput = NULL;
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
	//force our keywords
	if (	ov_string_compare(value, "<") == OV_STRCMP_EQUAL
		||	ov_string_compare(value, "<=") == OV_STRCMP_EQUAL
		||	ov_string_compare(value, "==") == OV_STRCMP_EQUAL
		||	ov_string_compare(value, "!=") == OV_STRCMP_EQUAL
		||	ov_string_compare(value, ">=") == OV_STRCMP_EQUAL
		||	ov_string_compare(value, ">") == OV_STRCMP_EQUAL){
		return ov_string_setvalue(&pobj->v_comptype,value);
	}else{
		ov_memstack_lock();
		ov_string_print(&erroroutput, "object %s had wrong comptype '%s'. Rejecting Variable change.", ov_path_getcanonicalpath(Ov_PtrUpCast(ov_object, pobj), 2), value);
		ov_memstack_unlock();
		ov_logfile_warning(erroroutput);
		ov_string_setvalue(&erroroutput, NULL);
		return OV_ERR_BADPARAM;
	}
}

OV_DLLFNCEXPORT OV_RESULT cshmi_CompareIteratedChild_ignoreCase_set(
	OV_INSTPTR_cshmi_CompareIteratedChild          pobj,
	const OV_BOOL  value
) {
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
	pobj->v_ignoreCase = value;
	return OV_ERR_OK;
}
