/*
*	Copyright (C) 2014
*	Chair of Process Control Engineering,
*	Aachen University of Technology.
*	All rights reserved.
*
*	Redistribution and use in source and binary forms, with or without
*	modification, are permitted provided that the following conditions
*	are met:
*	1. Redistributions of source code must retain the above copyright
*	   notice, this list of conditions and the following disclaimer.
*	2. Redistributions in binary form must print or display the above
*	   copyright notice either during startup or must have a means for
*	   the user to view the copyright notice.
*	3. Redistributions in binary form must reproduce the above copyright
*	   notice, this list of conditions and the following disclaimer in
*		the documentation and/or other materials provided with the
*		distribution.
*	4. Neither the name of the Chair of Process Control Engineering nor
*		the name of the Aachen University of Technology may be used to
*		endorse or promote products derived from this software without
*		specific prior written permission.
*
*	THIS SOFTWARE IS PROVIDED BY THE CHAIR OF PROCESS CONTROL ENGINEERING
*	``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*	LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*	A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE CHAIR OF
*	PROCESS CONTROL ENGINEERING BE LIABLE FOR ANY DIRECT, INDIRECT,
*	INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
*	BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
*	OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
*	AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
*	LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
*	WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
*	POSSIBILITY OF SUCH DAMAGE.
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

OV_DLLFNCEXPORT OV_RESULT cshmi_GetValue_constructor(
	OV_INSTPTR_ov_object 	pobj
) {
	//	local variables
	//
	OV_INSTPTR_ov_object
		pParent = NULL;
	OV_RESULT    result;

	/* general placement check is done here */
	result = cshmi_Action_constructor(pobj);
	if(Ov_Fail(result))
		return result;

	pParent = Ov_StaticPtrCast(ov_object, Ov_GetParent(ov_containment, pobj));
	if (pParent != NULL){
		if(Ov_GetParent(ov_instantiation, (pParent)) == pclass_cshmi_SetConcatValue){
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
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
	return ov_string_setvalue(&pobj->v_ksVar,value);
}

OV_DLLFNCEXPORT OV_RESULT cshmi_GetValue_elemVar_set(
		OV_INSTPTR_cshmi_GetValue          pobj,
		const OV_STRING  value
) {
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
	return ov_string_setvalue(&pobj->v_elemVar,value);
}

OV_DLLFNCEXPORT OV_RESULT cshmi_GetValue_globalVar_set(
		OV_INSTPTR_cshmi_GetValue          pobj,
		const OV_STRING  value
) {
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
	return ov_string_setvalue(&pobj->v_globalVar,value);
}

OV_DLLFNCEXPORT OV_RESULT cshmi_GetValue_persistentGlobalVar_set(
		OV_INSTPTR_cshmi_GetValue          pobj,
		const OV_STRING  value
) {
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
	return ov_string_setvalue(&pobj->v_persistentGlobalVar,value);
}

OV_DLLFNCEXPORT OV_RESULT cshmi_GetValue_TemplateFBVariableReferenceName_set(
		OV_INSTPTR_cshmi_GetValue          pobj,
		const OV_STRING  value
) {
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
	return ov_string_setvalue(&pobj->v_TemplateFBVariableReferenceName,value);
}

OV_DLLFNCEXPORT OV_RESULT cshmi_GetValue_TemplateConfigValues_set(
		OV_INSTPTR_cshmi_GetValue          pobj,
		const OV_STRING  value
) {
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
	return ov_string_setvalue(&pobj->v_TemplateConfigValues,value);
}

OV_DLLFNCEXPORT OV_RESULT cshmi_GetValue_value_set(
		OV_INSTPTR_cshmi_GetValue          pobj,
		const OV_ANY*  value
) {
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
	return ov_variable_setanyvalue(&pobj->v_value, value);
}

OV_DLLFNCEXPORT OV_RESULT cshmi_GetValue_OperatorInput_set(
	OV_INSTPTR_cshmi_GetValue          pobj,
	const OV_STRING  value
) {
	if (	ov_string_compare(value, "") == OV_STRCMP_EQUAL
		||	ov_string_compare(value, "mousex") == OV_STRCMP_EQUAL
		||	ov_string_compare(value, "mousey") == OV_STRCMP_EQUAL
		||	ov_string_compare(value, "textinput") == OV_STRCMP_EQUAL
		||	ov_string_match(value, "textinput:*")){
		cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
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
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
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
