/*
*	Copyright (C) 2015
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
*	Text.c
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



OV_DLLFNCEXPORT OV_RESULT cshmi_Text_x_set(
		OV_INSTPTR_cshmi_Text          pobj,
		const OV_SINGLE  value
) {
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
	pobj->v_x = value;
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT cshmi_Text_y_set(
		OV_INSTPTR_cshmi_Text          pobj,
		const OV_SINGLE  value
) {
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
	pobj->v_y = value;
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT cshmi_Text_content_set(
		OV_INSTPTR_cshmi_Text          pobj,
		const OV_STRING  value
) {
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
	return ov_string_setvalue(&pobj->v_content,value);
}


OV_DLLFNCEXPORT OV_RESULT cshmi_Text_fontSize_set(
	OV_INSTPTR_cshmi_Text	pobj,
	const OV_STRING  value
) {
	OV_STRING p = NULL;
	OV_RESULT fr = OV_ERR_OK;
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
	if(ov_string_compare(value, "normal") == OV_STRCMP_EQUAL){
		//fixing common typo
		return ov_string_setvalue(&pobj->v_fontSize,"medium");
	}
	fr = ov_string_setvalue(&pobj->v_fontSize, value);
	if(Ov_OK(fr) && value != NULL){
		(void)strtod(value, &p);
		if(p != NULL && *p == '\0'){
			//we had a numeric input
			fr = ov_string_append(&pobj->v_fontSize, "px");
		}
	}
	return fr;
}

OV_DLLFNCEXPORT OV_RESULT cshmi_Text_fontStyle_set(
	OV_INSTPTR_cshmi_Text	pobj,
	const OV_STRING  value
) {
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
	//force our keywords
	if (ov_string_compare(value, "") == OV_STRCMP_EQUAL){
		//setting default
		return ov_string_setvalue(&pobj->v_fontStyle,"normal");
	}else if (	ov_string_compare(value, "normal") == OV_STRCMP_EQUAL
			||	ov_string_compare(value, "italic") == OV_STRCMP_EQUAL
			||	ov_string_compare(value, "oblique") == OV_STRCMP_EQUAL){
		return ov_string_setvalue(&pobj->v_fontStyle,value);
	}else{
		return OV_ERR_BADPARAM;
	}
}

OV_DLLFNCEXPORT OV_RESULT cshmi_Text_fontWeight_set(
	OV_INSTPTR_cshmi_Text	pobj,
	const OV_STRING  value
) {
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
	//force our keywords
	if (ov_string_compare(value, "") == OV_STRCMP_EQUAL){
		//setting default
		return ov_string_setvalue(&pobj->v_fontWeight,"normal");
	}else if (	ov_string_compare(value, "normal") == OV_STRCMP_EQUAL
			||	ov_string_compare(value, "bold") == OV_STRCMP_EQUAL
			||	ov_string_compare(value, "lighter") == OV_STRCMP_EQUAL
			||	ov_string_compare(value, "bolder") == OV_STRCMP_EQUAL){
		return ov_string_setvalue(&pobj->v_fontWeight,value);
	}else{
		return OV_ERR_BADPARAM;
	}
}

OV_DLLFNCEXPORT OV_RESULT cshmi_Text_fontFamily_set(
		OV_INSTPTR_cshmi_Text          pobj,
		const OV_STRING  value
) {
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
	return ov_string_setvalue(&pobj->v_fontFamily,value);
}

OV_DLLFNCEXPORT OV_RESULT cshmi_Text_horAlignment_set(
	OV_INSTPTR_cshmi_Text	pobj,
	const OV_STRING  value
) {
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
	//force our keywords
	if (ov_string_compare(value, "") == OV_STRCMP_EQUAL){
		//setting default
		return ov_string_setvalue(&pobj->v_horAlignment,"start");
	}else if (ov_string_compare(value, "center") == OV_STRCMP_EQUAL){
		//fixing common typo
		return ov_string_setvalue(&pobj->v_horAlignment,"middle");
	}else if (	ov_string_compare(value, "start") == OV_STRCMP_EQUAL
			||	ov_string_compare(value, "middle") == OV_STRCMP_EQUAL
			||	ov_string_compare(value, "end") == OV_STRCMP_EQUAL){
		return ov_string_setvalue(&pobj->v_horAlignment,value);
	}else{
		return OV_ERR_BADPARAM;
	}
}

OV_DLLFNCEXPORT OV_RESULT cshmi_Text_verAlignment_set(
	OV_INSTPTR_cshmi_Text	pobj,
	const OV_STRING  value
) {
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
	//force our keywords
	if (ov_string_compare(value, "") == OV_STRCMP_EQUAL){
		//setting default
		return ov_string_setvalue(&pobj->v_verAlignment,"auto");
	}else if (ov_string_compare(value, "center") == OV_STRCMP_EQUAL){
		//fixing common typo
		return ov_string_setvalue(&pobj->v_verAlignment,"middle");
	}else if (	ov_string_compare(value, "auto") == OV_STRCMP_EQUAL
			||	ov_string_compare(value, "middle") == OV_STRCMP_EQUAL
			||	ov_string_compare(value, "hanging") == OV_STRCMP_EQUAL){
		return ov_string_setvalue(&pobj->v_verAlignment,value);
	}else{
		return OV_ERR_BADPARAM;
	}
}


OV_DLLFNCEXPORT OV_RESULT cshmi_Text_trimToLength_set(
		OV_INSTPTR_cshmi_Text          pobj,
		const OV_INT  value
) {
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
	pobj->v_trimToLength = value;
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT cshmi_Text_constructor(
	OV_INSTPTR_ov_object 	pobj
) {
	/*    
	*   local variables
	*/
	OV_INSTPTR_cshmi_Text pinst = Ov_StaticPtrCast(cshmi_Text, pobj);
	OV_RESULT    result;
	OV_STRING strTempOldStroke = NULL;
	
	/* do what the base class does first */
	result = cshmi_Element_constructor(pobj);
	if(Ov_Fail(result)){
		return result;
	}
	
	// swap default fill and stroke
	ov_string_setvalue(&strTempOldStroke, pinst->v_stroke);
	ov_string_setvalue(&pinst->v_stroke, pinst->v_fill);
	ov_string_setvalue(&pinst->v_fill, strTempOldStroke);
	
	//cleanup
	ov_string_setvalue(&strTempOldStroke, NULL);
	
	return OV_ERR_OK;
}

