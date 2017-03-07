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
*	GetValue.c
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
static void cshmi_getvalue_clearparameter(OV_INSTPTR_cshmi_GetValue this, const OV_ANY *newvalue){
	if(		 (newvalue->value.vartype & OV_VT_KSMASK) == OV_VT_VOID
		||	((newvalue->value.vartype & OV_VT_KSMASK) == OV_VT_STRING && ov_string_compare(newvalue->value.valueunion.val_string, NULL) == OV_STRCMP_EQUAL)){
		//do not clear the parameters if the new value is empty. A import sets all variables (even the empty ones).
		return;
	}
	//to not use the set accessors here to avoid a loop
	ov_string_setvalue(&this->v_ksVar, NULL);
	ov_string_setvalue(&this->v_elemVar, NULL);
	ov_string_setvalue(&this->v_globalVar, NULL);
	ov_string_setvalue(&this->v_persistentGlobalVar, NULL);
	ov_string_setvalue(&this->v_globalVar, NULL);
	ov_string_setvalue(&this->v_OperatorInput, NULL);
	ov_string_setvalue(&this->v_TemplateFBReferenceVariable, NULL);
	ov_string_setvalue(&this->v_TemplateFBVariableReferenceName, NULL);
	ov_string_setvalue(&this->v_TemplateConfigValues, NULL);
	//ov_string_setvalue(&this->v_value, NULL);
	Ov_SetAnyValue(&this->v_value, NULL);
	return;
}


OV_DLLFNCEXPORT OV_RESULT cshmi_GetValue_constructor(
	OV_INSTPTR_ov_object 	pobj
) {
	//	local variables
	//
	OV_INSTPTR_ov_object
		pParent = NULL;
	OV_RESULT    result;

	/* do what the base class does first */
	result = cshmi_Object_constructor(pobj);
	if(Ov_Fail(result))
		return result;

	pParent = Ov_StaticPtrCast(ov_object, Ov_GetParent(ov_containment, pobj));
	if (pParent != NULL){
		if(Ov_GetClassPtr(pParent) == pclass_cshmi_SetConcatValue){
			//whitelist any GetValue under a SetConcatValue
			return OV_ERR_OK;
		}else if (Ov_CanCastTo(cshmi_SetMathValue, pParent)){
			//check name of GetValue under a SetMathValue
			if(ov_string_match(pobj->v_identifier, "add*")){
				return OV_ERR_OK;
			}else if(ov_string_match(pobj->v_identifier, "sub*")){
				return OV_ERR_OK;
			}else if(ov_string_match(pobj->v_identifier, "mul*")){
				return OV_ERR_OK;
			}else if(ov_string_match(pobj->v_identifier, "div*")){
				return OV_ERR_OK;
			}else if(ov_string_match(pobj->v_identifier, "mod*")){
				//Returns the remainder of the modulo operator
				return OV_ERR_OK;
			}else if(ov_string_match(pobj->v_identifier, "abs*")){
				//Returns the absolute value (modulus) of a number.
				return OV_ERR_OK;
			}else if(ov_string_match(pobj->v_identifier, "acos*")){
				//Returns the arccosine of a number.
				return OV_ERR_OK;
			}else if(ov_string_match(pobj->v_identifier, "asin*")){
				//Returns the arcsine of a number.
				return OV_ERR_OK;
			}else if(ov_string_match(pobj->v_identifier, "atan*")){
				//Returns the arctangent of a number.
				return OV_ERR_OK;
			}else if(ov_string_match(pobj->v_identifier, "cos*")){
				//Returns the cosine of a number.
				return OV_ERR_OK;
			}else if(ov_string_match(pobj->v_identifier, "exp*")){
				//Returns E^number, where number is the argument, and E is Euler's constant (2.718...), the base of the natural logarithm.
				return OV_ERR_OK;
			}else if(ov_string_match(pobj->v_identifier, "log*")){
				//Returns the natural logarithm (loge, also ln) of a number.
				return OV_ERR_OK;
			}else if(ov_string_match(pobj->v_identifier, "pow*")){
				//Returns base to the exponent power, that is, base^exponent.
				return OV_ERR_OK;
			}else if(ov_string_match(pobj->v_identifier, "sin*")){
				//Returns the sine of a number.
				return OV_ERR_OK;
			}else if(ov_string_match(pobj->v_identifier, "sqrt*")){
				//Returns the positive square root of a number.
				return OV_ERR_OK;
			}else if(ov_string_match(pobj->v_identifier, "tan*")){
				//Returns the tangent of a number.
				return OV_ERR_OK;
			}else if(ov_string_match(pobj->v_identifier, "ran*")){
				//Returns a random number with the maximum of the value.
				return OV_ERR_OK;
			}
			ov_logfile_warning("Naming the GetValue %s was wrong. allowed is add*, sub*, mul*, div*, abs*, acos*, asin*, atan*, cos*, exp*, log*, sin*, sqrt*, tan*, pow* ", pobj->v_identifier);
			return OV_ERR_BADNAME;
		}
		ov_logfile_warning("An action is not allowed below this parent. Action: %s, parent: %s", pobj->v_identifier, pParent->v_identifier);
		return OV_ERR_BADPLACEMENT;
	}
	return OV_ERR_OK;
}



OV_DLLFNCEXPORT OV_RESULT cshmi_GetValue_ksVar_set(
		OV_INSTPTR_cshmi_GetValue          pobj,
		const OV_STRING  value
) {
	OV_ANY newvalue;
	newvalue.value.vartype = OV_VT_STRING;
	newvalue.value.valueunion.val_string = value;
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
	cshmi_getvalue_clearparameter(pobj, &newvalue);
	return ov_string_setvalue(&pobj->v_ksVar,value);
}

OV_DLLFNCEXPORT OV_RESULT cshmi_GetValue_elemVar_set(
		OV_INSTPTR_cshmi_GetValue          pobj,
		const OV_STRING  value
) {
	OV_ANY newvalue;
	newvalue.value.vartype = OV_VT_STRING;
	newvalue.value.valueunion.val_string = value;
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
	cshmi_getvalue_clearparameter(pobj, &newvalue);
	return ov_string_setvalue(&pobj->v_elemVar,value);
}

OV_DLLFNCEXPORT OV_RESULT cshmi_GetValue_globalVar_set(
		OV_INSTPTR_cshmi_GetValue          pobj,
		const OV_STRING  value
) {
	OV_ANY newvalue;
	newvalue.value.vartype = OV_VT_STRING;
	newvalue.value.valueunion.val_string = value;
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
	cshmi_getvalue_clearparameter(pobj, &newvalue);
	return ov_string_setvalue(&pobj->v_globalVar,value);
}

OV_DLLFNCEXPORT OV_RESULT cshmi_GetValue_persistentGlobalVar_set(
		OV_INSTPTR_cshmi_GetValue          pobj,
		const OV_STRING  value
) {
	OV_ANY newvalue;
	newvalue.value.vartype = OV_VT_STRING;
	newvalue.value.valueunion.val_string = value;
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
	cshmi_getvalue_clearparameter(pobj, &newvalue);
	return ov_string_setvalue(&pobj->v_persistentGlobalVar,value);
}

OV_DLLFNCEXPORT OV_RESULT cshmi_GetValue_TemplateFBVariableReferenceName_set(
		OV_INSTPTR_cshmi_GetValue          pobj,
		const OV_STRING  value
) {
	OV_ANY newvalue;
	newvalue.value.vartype = OV_VT_STRING;
	newvalue.value.valueunion.val_string = value;
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
	cshmi_getvalue_clearparameter(pobj, &newvalue);
	return ov_string_setvalue(&pobj->v_TemplateFBVariableReferenceName,value);
}

OV_DLLFNCEXPORT OV_RESULT cshmi_GetValue_TemplateConfigValues_set(
		OV_INSTPTR_cshmi_GetValue          pobj,
		const OV_STRING  value
) {
	OV_ANY newvalue;
	newvalue.value.vartype = OV_VT_STRING;
	newvalue.value.valueunion.val_string = value;
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
	cshmi_getvalue_clearparameter(pobj, &newvalue);
	return ov_string_setvalue(&pobj->v_TemplateConfigValues,value);
}

OV_DLLFNCEXPORT OV_RESULT cshmi_GetValue_value_set(
		OV_INSTPTR_cshmi_GetValue          pobj,
		const OV_ANY*  value
) {
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
	cshmi_getvalue_clearparameter(pobj, value);
	return Ov_SetAnyValue(&pobj->v_value, value);
}

OV_DLLFNCEXPORT OV_RESULT cshmi_GetValue_OperatorInput_set(
	OV_INSTPTR_cshmi_GetValue          pobj,
	const OV_STRING  value
) {
	OV_ANY newvalue;
	if (	ov_string_compare(value, "") == OV_STRCMP_EQUAL
		||	ov_string_compare(value, "mousex") == OV_STRCMP_EQUAL
		||	ov_string_compare(value, "mousey") == OV_STRCMP_EQUAL
		||	ov_string_compare(value, "textinput") == OV_STRCMP_EQUAL
		||	ov_string_match(value, "textinput:*")){
		newvalue.value.vartype = OV_VT_STRING;
		newvalue.value.valueunion.val_string = value;
		cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
		cshmi_getvalue_clearparameter(pobj, &newvalue);
		return ov_string_setvalue(&pobj->v_OperatorInput,value);
	}
	ov_logfile_warning("Wrong OperatorInput set on: '%s', requested value was: '%s'. Only mousex, mousey and textinput* is allowed.", pobj->v_identifier, value);
	return OV_ERR_BADPARAM;
}

OV_DLLFNCEXPORT OV_RESULT cshmi_GetValue_elemVarPath_set(
	OV_INSTPTR_cshmi_GetValue          pobj,
	const OV_STRING  value
) {
	if (ov_string_compare(value, "") == OV_STRCMP_EQUAL){
		return ov_string_setvalue(&pobj->v_elemVarPath, NULL);
	}
	return OV_ERR_NOTIMPLEMENTED;
}

OV_DLLFNCEXPORT OV_RESULT cshmi_GetValue_TemplateFBReferenceVariable_set(
	OV_INSTPTR_cshmi_GetValue          pobj,
	const OV_STRING  value
) {
	OV_ANY newvalue;
	newvalue.value.vartype = OV_VT_STRING;
	newvalue.value.valueunion.val_string = value;
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
	cshmi_getvalue_clearparameter(pobj, &newvalue);
	if(ov_string_compare(value, "fullqualifiedparentname") == OV_STRCMP_EQUAL){
			return ov_string_setvalue(&pobj->v_TemplateFBReferenceVariable, "CSHMIfullqualifiedparentname");
	}else if(ov_string_compare(value, "fullqualifiedname") == OV_STRCMP_EQUAL){
		return ov_string_setvalue(&pobj->v_TemplateFBReferenceVariable, "CSHMIfullqualifiedname");
	}else if(ov_string_compare(value, "absoluteparentpathname") == OV_STRCMP_EQUAL){
		return ov_string_setvalue(&pobj->v_TemplateFBReferenceVariable, "CSHMIabsoluteparentpathname");
	}else if(ov_string_compare(value, "absolutepathname") == OV_STRCMP_EQUAL){
		return ov_string_setvalue(&pobj->v_TemplateFBReferenceVariable, "CSHMIabsolutepathname");
	}
	return ov_string_setvalue(&pobj->v_TemplateFBReferenceVariable,value);
}

OV_DLLFNCEXPORT OV_ACCESS cshmi_GetValue_getaccess(
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
