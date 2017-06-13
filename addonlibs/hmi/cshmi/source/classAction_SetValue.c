/*
*   Copyright (C) 2015
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
*	classAction_SetValue.c
*
*	Editors:
*	--------
*	Je							Holger Jeromin <jeromin@plt.rwth-aachen.de>
*	GQ							Gustavo Quiros <g.quiros@plt.rwth-aachen.de>
*
*	SVN:
*	----
*	$Revision$
*	$Date$
*
*	History:
*	--------
*	18-August-2011			GQ		V0.1.0
*		-	File created
*
***********************************************************************/

#ifndef OV_COMPILE_LIBRARY_cshmi
#define OV_COMPILE_LIBRARY_cshmi
#endif

#include "cshmilib.h"

/**
 * clear all parameters to prepare the load of the new variable
 * @param this
 */
static void cshmi_setvalue_clearparameter(OV_INSTPTR_cshmi_SetValueBase this, const OV_STRING newvalue){
	if(ov_string_compare(newvalue, NULL) == OV_STRCMP_EQUAL){
		//do not clear the parameters if the new value is empty. A import sets all variables (even the empty ones).
		return;
	}
	//to not use the set accessors here to avoid a loop
	ov_string_setvalue(&this->v_ksVar, NULL);
	ov_string_setvalue(&this->v_elemVar, NULL);
	ov_string_setvalue(&this->v_globalVar, NULL);
	ov_string_setvalue(&this->v_persistentGlobalVar, NULL);
	ov_string_setvalue(&this->v_globalVar, NULL);
	ov_string_setvalue(&this->v_TemplateFBReferenceVariable, NULL);
	ov_string_setvalue(&this->v_TemplateFBVariableReferenceName, NULL);
	ov_string_setvalue(&this->v_TemplateConfigValues, NULL);
	return;
}

OV_DLLFNCEXPORT OV_RESULT cshmi_SetValueBase_ksVar_set(
		OV_INSTPTR_cshmi_SetValueBase          pobj,
		const OV_STRING  value
) {
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
	cshmi_setvalue_clearparameter(pobj, value);
	return ov_string_setvalue(&pobj->v_ksVar,value);
}

OV_DLLFNCEXPORT OV_RESULT cshmi_SetValueBase_elemVar_set(
		OV_INSTPTR_cshmi_SetValueBase          pobj,
		const OV_STRING  value
) {
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
	cshmi_setvalue_clearparameter(pobj, value);
	return ov_string_setvalue(&pobj->v_elemVar,value);
}

OV_DLLFNCEXPORT OV_RESULT cshmi_SetValueBase_elemVarPath_set(
		OV_INSTPTR_cshmi_SetValueBase          pobj,
		const OV_STRING  value
) {
	if (ov_string_compare(value, "") == OV_STRCMP_EQUAL){
		return ov_string_setvalue(&pobj->v_elemVarPath, NULL);
	}
	return OV_ERR_NOTIMPLEMENTED;
}

OV_DLLFNCEXPORT OV_RESULT cshmi_SetValueBase_globalVar_set(
		OV_INSTPTR_cshmi_SetValueBase          pobj,
		const OV_STRING  value
) {
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
	cshmi_setvalue_clearparameter(pobj, value);
	return ov_string_setvalue(&pobj->v_globalVar,value);
}

OV_DLLFNCEXPORT OV_RESULT cshmi_SetValueBase_persistentGlobalVar_set(
		OV_INSTPTR_cshmi_SetValueBase          pobj,
		const OV_STRING  value
) {
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
	cshmi_setvalue_clearparameter(pobj, value);
	return ov_string_setvalue(&pobj->v_persistentGlobalVar,value);
}

OV_DLLFNCEXPORT OV_RESULT cshmi_SetValueBase_TemplateFBReferenceVariable_set(
		OV_INSTPTR_cshmi_SetValueBase          pobj,
		const OV_STRING  value
) {
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
	cshmi_setvalue_clearparameter(pobj, value);
	return ov_string_setvalue(&pobj->v_TemplateFBReferenceVariable,value);
}

OV_DLLFNCEXPORT OV_RESULT cshmi_SetValueBase_TemplateFBVariableReferenceName_set(
		OV_INSTPTR_cshmi_SetValueBase          pobj,
		const OV_STRING  value
) {
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
	cshmi_setvalue_clearparameter(pobj, value);
	return ov_string_setvalue(&pobj->v_TemplateFBVariableReferenceName,value);
}

OV_DLLFNCEXPORT OV_RESULT cshmi_SetValueBase_TemplateConfigValues_set(
		OV_INSTPTR_cshmi_SetValueBase          pobj,
		const OV_STRING  value
) {
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
	cshmi_setvalue_clearparameter(pobj, value);
	return ov_string_setvalue(&pobj->v_TemplateConfigValues,value);
}



OV_DLLFNCEXPORT OV_RESULT cshmi_SetValue_translationSource_set(
		OV_INSTPTR_cshmi_SetValue          pobj,
		const OV_STRING  value
) {
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
	return ov_string_setvalue(&pobj->v_translationSource,value);
}


OV_DLLFNCEXPORT OV_RESULT cshmi_TranslationSource_translationMapping_set(
		OV_INSTPTR_cshmi_TranslationSource          pobj,
		const OV_STRING*  value,
		const OV_UINT veclen
) {
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
	return Ov_SetDynamicVectorValue(&pobj->v_translationMapping,value,veclen,STRING);
}

OV_DLLFNCEXPORT OV_ACCESS cshmi_SetValueBase_getaccess(
	OV_INSTPTR_ov_object	pobj,
	const OV_ELEMENT		*pelem,
	const OV_TICKET			*pticket
) {
	switch (pelem->elemtype){
		case OV_ET_VARIABLE :
			if (pelem->elemunion.pvar->v_offset >= offsetof(OV_INST_ov_object, __classinfo)){
				if (ov_string_compare(pelem->elemunion.pvar->v_identifier, "elemVarPath") == OV_STRCMP_EQUAL){
					//allow restoring an old backup, but prevent display and export of the old variable
					return OV_AC_WRITE;
				}
			}
			break;
		default:
			break;
	};
	return cshmi_Object_getaccess(pobj, pelem, pticket);
}
