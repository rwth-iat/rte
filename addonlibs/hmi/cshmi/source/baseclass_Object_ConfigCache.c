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
*	baseclass_Object_ConfigCache.c
*
*	Editors:
*	--------
*	Je							Holger Jeromin <jeromin@plt.rwth-aachen.de>
*
*	SVN:
*	----
*	$Revision: 6767 $
*	$Date: 2012-09-06 17:24:45 +0200 (Do, 06 Sep 2012) $
*
*	History:
*	--------
*	06-June-2014			Je		V0.1.0
*		-	File created
*
***********************************************************************/

#ifndef OV_COMPILE_LIBRARY_cshmi
#define OV_COMPILE_LIBRARY_cshmi
#endif

#include "cshmilib.h"

/**
 * encodes the string in a percent encoding
 * call ov_memstack_allow around this!
 * @param strIn
 * @return
 */
static OV_STRING cshmi_Object_prepareURIencode(OV_STRING strIn){
	OV_STRING	pcIn;
	OV_STRING	pcOut = 0;
	OV_STRING	strOut;

	if(!strIn){
		strOut = (OV_STRING)ov_memstack_alloc(1);
		*strOut = '\0';
		return strOut;
	}
	//only " would be the worst case
	strOut = (OV_STRING) ov_memstack_alloc(ov_string_getlength(strIn)*6+1);

	pcIn = strIn;
	pcOut = strOut;
	while(*pcIn) {
		if(*pcIn == '\n'){
			*pcOut = ' ';
		}else if(*pcIn == '\r'){
			*pcOut = ' ';
		}else if(*pcIn == '%'){
			*pcOut = '%';
			pcOut++;
			*pcOut = '2';
			pcOut++;
			*pcOut = '5';
		}else if(*pcIn == '"'){
			// JSON needs " has to be encoded to \", %5C%22 is urlencoded json escaping
			*pcOut = '%';	//JSON encoding of a backslash!
			pcOut++;
			*pcOut = '5';
			pcOut++;
			*pcOut = 'C';	//JSON end
			pcOut++;
			*pcOut = '%';	//urlencoding of a quote
			pcOut++;
			*pcOut = '2';
			pcOut++;
			*pcOut = '2';
		}else if(*pcIn == '{'){
			*pcOut = '%';
			pcOut++;
			*pcOut = '7';
			pcOut++;
			*pcOut = 'B';
		}else if(*pcIn == '}'){
			*pcOut = '%';
			pcOut++;
			*pcOut = '7';
			pcOut++;
			*pcOut = 'D';
		}else if(*pcIn == '['){
			*pcOut = '%';
			pcOut++;
			*pcOut = '5';
			pcOut++;
			*pcOut = 'B';
		}else if(*pcIn == ']'){
			*pcOut = '%';
			pcOut++;
			*pcOut = '5';
			pcOut++;
			*pcOut = 'D';
		}else if(*pcIn == '\\'){
			*pcOut = '%';
			pcOut++;
			*pcOut = '5';
			pcOut++;
			*pcOut = 'C';
		}else{
			*pcOut = *pcIn;
		}
		pcIn++;
		pcOut++;
	}
	*pcOut = '\0';		/*append terminating '\0'*/
	return strOut;
}

/**
 * prints the basic types to the string (visible, stroke, fill, opacity, rotate)
 * @param strResult
 * @param pElement
 * @return
 */
static OV_RESULT cshmi_Object_buildBaseElementString(OV_STRING*strResult, OV_INSTPTR_cshmi_Element pElement){
	ov_string_print(strResult, "%s%%22visible%%22:%%22%s%%22,", *strResult, pElement->v_visible==TRUE?"TRUE":"FALSE");
	ov_string_print(strResult, "%s%%22stroke%%22:%%22%s%%22,", *strResult, pElement->v_stroke==NULL?"":cshmi_Object_prepareURIencode(pElement->v_stroke));
	ov_string_print(strResult, "%s%%22fill%%22:%%22%s%%22,", *strResult, pElement->v_fill==NULL?"":cshmi_Object_prepareURIencode(pElement->v_fill));
	ov_string_print(strResult, "%s%%22opacity%%22:%%22%f%%22,", *strResult, pElement->v_opacity);
	ov_string_print(strResult, "%s%%22rotate%%22:%%22%i%%22,", *strResult, pElement->v_rotate);
	return OV_ERR_OK;
}


static OV_RESULT cshmi_Object_updateConfigAsJSON(
	OV_INSTPTR_cshmi_Object          pObj
) {

	OV_INSTPTR_ov_class pClass = NULL;

	OV_INSTPTR_cshmi_InstantiateTemplate pInstantiateTemplate = NULL;
	OV_INSTPTR_cshmi_Group pGroup = NULL;
	OV_INSTPTR_cshmi_TemplateDefinition pTemplateDefinition = NULL;
	OV_INSTPTR_cshmi_Rectangle pRectangle = NULL;
	OV_INSTPTR_cshmi_Circle pCircle = NULL;
	OV_INSTPTR_cshmi_Text pText = NULL;
	OV_INSTPTR_cshmi_Line pLine = NULL;
	OV_INSTPTR_cshmi_Polyline pPolyline = NULL;
	OV_INSTPTR_cshmi_Polygon pPolygon = NULL;
	OV_INSTPTR_cshmi_Path pPath = NULL;
	OV_INSTPTR_cshmi_Ellipse pEllipse = NULL;

	OV_INSTPTR_cshmi_SetValue pSetValue = NULL;
	OV_INSTPTR_cshmi_SetConcatValue pSetConcatValue = NULL;
	OV_INSTPTR_cshmi_SetMathValue pSetMathValue = NULL;
	OV_INSTPTR_cshmi_GetValue pGetValue = NULL;
	OV_INSTPTR_cshmi_ChildrenIterator pChildrenIterator = NULL;
	OV_INSTPTR_cshmi_IfThenElse pIfThenElse = NULL;
	OV_INSTPTR_cshmi_Compare pCompare = NULL;
	OV_INSTPTR_cshmi_CompareIteratedChild pCompareIteratedChild = NULL;
	OV_INSTPTR_cshmi_RoutePolyline pRoutePolyline = NULL;
	OV_INSTPTR_cshmi_TimeEvent pTimeEvent = NULL;
	OV_INSTPTR_cshmi_TranslationSource pTranslationSource = NULL;
	OV_INSTPTR_cshmi_CreateObject pCreateObject = NULL;
	OV_INSTPTR_cshmi_Vibrate pVibrate = NULL;

	OV_STRING strIterate = NULL;
	OV_STRING ParameterName = NULL;
	OV_STRING ParameterValue = NULL;
	OV_STRING temp = NULL;
	OV_UINT i = 0;
	OV_UINT strLen = 0;


	/* builds something like this (without the whitespaces) for Elements:
	%22/TechUnits/sheet%22:%7B
		%22Parameters%22:%7B
			%22x%22:%220.000000%22,
			%22y%22:%220.000000%22,
			%22width%22:%221000.000000%22,
			%22height%22:%22900.000000%22,
			%22opacity%22:%221.000000%22,
			%22rotate%22:%220%22,
			%22hideable%22:%22FALSE%22,
			%22visible%22:%22TRUE%22,
			%22TemplateDefinition%22:%22%22,
			%22FBReference%22:%22%22,
			%22FBVariableReference%22:%22%22,
			%22ConfigValues%22:%22%22
		%7D
	%7D
	 */


	/* builds something like this (without the whitespaces) for Actions:
	%22/TechUnits/sheet/onload/set%22:%7B
		%22translationSource%22:%22%22,
		%22ParameterName%22:%22elemVar%22,
		%22ParameterValue%22:%22fill%22,
		%22translationSource%22:%22%22
	%7D,
	%22/TechUnits/sheet/onload/set.value%22:%7B
		%22ParameterName%22:%22value%22,
		%22ParameterValue%22:%22red%22%
	%7D
	%22/TechUnits/sheet/onload/if.if/conditionA%22:%7B
		%22Parameters%22:%7B
			%22comptype%22:%22==%22,
			%22ignoreError%22:%22FALSE%22
		%7D
	%7D
	*/

	pClass = Ov_GetClassPtr(pObj);

	ov_string_setvalue(&strIterate, "%22");
	ov_memstack_lock();
	ov_string_append(&strIterate, ov_path_getcanonicalpath(Ov_PtrUpCast(ov_object, pObj), 2));
	ov_memstack_unlock();
	ov_string_append(&strIterate, "%22:%7B");

	if(pClass == pclass_cshmi_Group){
		pGroup = Ov_StaticPtrCast(cshmi_Group, pObj);
		ov_string_append(&strIterate, "%22Parameters%22:%7B");
		ov_string_print(&strIterate, "%s%%22x%%22:%%22%f%%22,", strIterate, pGroup->v_x);
		ov_string_print(&strIterate, "%s%%22y%%22:%%22%f%%22,", strIterate, pGroup->v_y);
		ov_string_print(&strIterate, "%s%%22width%%22:%%22%f%%22,", strIterate, pGroup->v_width);
		ov_string_print(&strIterate, "%s%%22height%%22:%%22%f%%22,", strIterate, pGroup->v_height);
		ov_string_print(&strIterate, "%s%%22opacity%%22:%%22%f%%22,", strIterate, pGroup->v_opacity);
		ov_string_print(&strIterate, "%s%%22rotate%%22:%%22%i%%22,", strIterate, pGroup->v_rotate);
		ov_string_print(&strIterate, "%s%%22hideable%%22:%%22%s%%22,", strIterate, pGroup->v_hideable==TRUE?"TRUE":"FALSE");
		ov_string_print(&strIterate, "%s%%22visible%%22:%%22%s%%22,", strIterate, pGroup->v_visible==TRUE?"TRUE":"FALSE");
		ov_string_print(&strIterate, "%s%%22TemplateDefinition%%22:%%22%s%%22,", strIterate, pGroup->v_TemplateDefinition==NULL?"":cshmi_Object_prepareURIencode(pGroup->v_TemplateDefinition));
		ov_string_print(&strIterate, "%s%%22FBReference%%22:%%22%s%%22,", strIterate, (pGroup->v_FBReference.veclen!=1||pGroup->v_FBReference.value[0]==NULL)?"":cshmi_Object_prepareURIencode(pGroup->v_FBReference.value[0]));

		if(pGroup->v_FBVariableReference.veclen == 0){
			ov_string_setvalue(&temp, "%22%22");
		}else if(pGroup->v_FBVariableReference.veclen == 1){
			ov_string_print(&temp, "%%22%s%%22", cshmi_Object_prepareURIencode(pGroup->v_FBVariableReference.value[0]));
		}else{
			ov_string_print(&temp, "%%22%s", cshmi_Object_prepareURIencode(pGroup->v_FBVariableReference.value[0]));
			for(i = 1; i < pGroup->v_FBVariableReference.veclen;i++){
				ov_string_print(&temp, "%s %s", temp, cshmi_Object_prepareURIencode(pGroup->v_FBVariableReference.value[i]));
			}
			ov_string_append(&temp, "%22");
		}
		ov_string_print(&strIterate, "%s%%22FBVariableReference%%22:%s,", strIterate, temp);

		if(pGroup->v_ConfigValues.veclen == 0){
			ov_string_setvalue(&temp, "%22%22");
		}else if(pGroup->v_ConfigValues.veclen == 1){
			ov_memstack_lock();
			ov_string_print(&temp, "%%22%s%%22", cshmi_Object_prepareURIencode(pGroup->v_ConfigValues.value[0]));
			ov_memstack_unlock();
		}else{
			ov_memstack_lock();
			ov_string_print(&temp, "%%22%s", cshmi_Object_prepareURIencode(pGroup->v_ConfigValues.value[0]));
			ov_memstack_unlock();
			for(i = 1; i < pGroup->v_ConfigValues.veclen;i++){
				ov_memstack_lock();
				ov_string_print(&temp, "%s %s", temp, cshmi_Object_prepareURIencode(pGroup->v_ConfigValues.value[i]));
				ov_memstack_unlock();
			}
			ov_string_append(&temp, "%22");
		}
		ov_string_print(&strIterate, "%s%%22ConfigValues%%22:%s", strIterate, temp);

		ov_string_append(&strIterate, "%7D");
	}else if(pClass == pclass_cshmi_TemplateDefinition){
		pTemplateDefinition = Ov_StaticPtrCast(cshmi_TemplateDefinition, pObj);
		ov_string_append(&strIterate, "%22Parameters%22:%7B");
		ov_string_print(&strIterate, "%s%%22width%%22:%%22%f%%22,", strIterate, pTemplateDefinition->v_width);
		ov_string_print(&strIterate, "%s%%22height%%22:%%22%f%%22", strIterate, pTemplateDefinition->v_height);
		ov_string_append(&strIterate, "%7D");
	}else if(pClass == pclass_cshmi_InstantiateTemplate){
		pInstantiateTemplate = Ov_StaticPtrCast(cshmi_InstantiateTemplate, pObj);
		ov_string_append(&strIterate, "%22Parameters%22:%7B");
		ov_string_print(&strIterate, "%s%%22visible%%22:%%22%s%%22,", strIterate, pInstantiateTemplate->v_visible==TRUE?"TRUE":"FALSE");
		ov_string_print(&strIterate, "%s%%22stroke%%22:%%22%s%%22,", strIterate, pInstantiateTemplate->v_stroke==NULL?"":cshmi_Object_prepareURIencode(pInstantiateTemplate->v_stroke));
		ov_string_print(&strIterate, "%s%%22fill%%22:%%22%s%%22,", strIterate, pInstantiateTemplate->v_fill==NULL?"":cshmi_Object_prepareURIencode(pInstantiateTemplate->v_fill));
		ov_string_print(&strIterate, "%s%%22opacity%%22:%%22%f%%22,", strIterate, pInstantiateTemplate->v_opacity);
		ov_string_print(&strIterate, "%s%%22rotate%%22:%%22%i%%22,", strIterate, pInstantiateTemplate->v_rotate);
		ov_string_print(&strIterate, "%s%%22TemplateDefinition%%22:%%22%s%%22,", strIterate, pInstantiateTemplate->v_TemplateDefinition==NULL?"":cshmi_Object_prepareURIencode(pInstantiateTemplate->v_TemplateDefinition));
		ov_string_print(&strIterate, "%s%%22x%%22:%%22%f%%22,", strIterate, pInstantiateTemplate->v_x);
		ov_string_print(&strIterate, "%s%%22y%%22:%%22%f%%22,", strIterate, pInstantiateTemplate->v_y);
		ov_string_print(&strIterate, "%s%%22xOffset%%22:%%22%f%%22,", strIterate, pInstantiateTemplate->v_xOffset);
		ov_string_print(&strIterate, "%s%%22yOffset%%22:%%22%f%%22,", strIterate, pInstantiateTemplate->v_yOffset);
		ov_string_print(&strIterate, "%s%%22maxTemplatesPerDirection%%22:%%22%s%%22,", strIterate, pInstantiateTemplate->v_maxTemplatesPerDirection==NULL?"":cshmi_Object_prepareURIencode(pInstantiateTemplate->v_maxTemplatesPerDirection));
		ov_string_print(&strIterate, "%s%%22preventClone%%22:%%22%s%%22,", strIterate, pInstantiateTemplate->v_preventClone==TRUE?"TRUE":"FALSE");
		ov_string_print(&strIterate, "%s%%22FBReference%%22:%%22%s%%22,", strIterate, (pInstantiateTemplate->v_FBReference.veclen!=1||pInstantiateTemplate->v_FBReference.value[0]==NULL)?"":cshmi_Object_prepareURIencode(pInstantiateTemplate->v_FBReference.value[0]));

		if(pInstantiateTemplate->v_FBVariableReference.veclen == 0){
			ov_string_setvalue(&temp, "%22%22");
		}else if(pInstantiateTemplate->v_FBVariableReference.veclen == 1){
			ov_string_print(&temp, "%%22%s%%22", cshmi_Object_prepareURIencode(pInstantiateTemplate->v_FBVariableReference.value[0]));
		}else{
			ov_string_print(&temp, "%%22%s", cshmi_Object_prepareURIencode(pInstantiateTemplate->v_FBVariableReference.value[0]));
			for(i = 1; i < pInstantiateTemplate->v_FBVariableReference.veclen;i++){
				ov_string_print(&temp, "%s %s", temp, cshmi_Object_prepareURIencode(pInstantiateTemplate->v_FBVariableReference.value[i]));
			}
			ov_string_append(&temp, "%22");
		}
		ov_string_print(&strIterate, "%s%%22FBVariableReference%%22:%s,", strIterate, temp);

		if(pInstantiateTemplate->v_ConfigValues.veclen == 0){
			ov_string_setvalue(&temp, "%22%22");
		}else if(pInstantiateTemplate->v_ConfigValues.veclen == 1){
			ov_string_print(&temp, "%%22%s%%22", cshmi_Object_prepareURIencode(pInstantiateTemplate->v_ConfigValues.value[0]));
		}else{
			ov_string_print(&temp, "%%22%s", cshmi_Object_prepareURIencode(pInstantiateTemplate->v_ConfigValues.value[0]));
			for(i = 1; i < pInstantiateTemplate->v_ConfigValues.veclen;i++){
				ov_string_print(&temp, "%s %s", temp, cshmi_Object_prepareURIencode(pInstantiateTemplate->v_ConfigValues.value[i]));
			}
			ov_string_append(&temp, "%22");
		}
		ov_string_print(&strIterate, "%s%%22ConfigValues%%22:%s,", strIterate, temp);

		ov_string_print(&strIterate, "%s%%22hideable%%22:%%22%s%%22", strIterate, pInstantiateTemplate->v_hideable==TRUE?"TRUE":"FALSE");
		ov_string_append(&strIterate, "%7D");
	}else if(pClass == pclass_cshmi_Rectangle){
		pRectangle = Ov_StaticPtrCast(cshmi_Rectangle, pObj);
		ov_string_append(&strIterate, "%22Parameters%22:%7B");
		cshmi_Object_buildBaseElementString(&strIterate, Ov_PtrUpCast(cshmi_Element, pRectangle));
		ov_string_print(&strIterate, "%s%%22x%%22:%%22%f%%22,", strIterate, pRectangle->v_x);
		ov_string_print(&strIterate, "%s%%22y%%22:%%22%f%%22,", strIterate, pRectangle->v_y);
		ov_string_print(&strIterate, "%s%%22width%%22:%%22%f%%22,", strIterate, pRectangle->v_width);
		ov_string_print(&strIterate, "%s%%22height%%22:%%22%f%%22,", strIterate, pRectangle->v_height);
		ov_string_print(&strIterate, "%s%%22strokeWidth%%22:%%22%f%%22", strIterate, pRectangle->v_strokeWidth);
		ov_string_append(&strIterate, "%7D");
	}else if(pClass == pclass_cshmi_Circle){
		pCircle = Ov_StaticPtrCast(cshmi_Circle, pObj);
		ov_string_append(&strIterate, "%22Parameters%22:%7B");
		cshmi_Object_buildBaseElementString(&strIterate, Ov_PtrUpCast(cshmi_Element, pCircle));
		ov_string_print(&strIterate, "%s%%22cx%%22:%%22%f%%22,", strIterate, pCircle->v_cx);
		ov_string_print(&strIterate, "%s%%22cy%%22:%%22%f%%22,", strIterate, pCircle->v_cy);
		ov_string_print(&strIterate, "%s%%22r%%22:%%22%f%%22,", strIterate, pCircle->v_r);
		ov_string_print(&strIterate, "%s%%22strokeWidth%%22:%%22%f%%22", strIterate, pCircle->v_strokeWidth);
		ov_string_append(&strIterate, "%7D");
	}else if(pClass == pclass_cshmi_Text){
		pText = Ov_StaticPtrCast(cshmi_Text, pObj);
		ov_string_append(&strIterate, "%22Parameters%22:%7B");
		cshmi_Object_buildBaseElementString(&strIterate, Ov_PtrUpCast(cshmi_Element, pText));
		ov_string_print(&strIterate, "%s%%22x%%22:%%22%f%%22,", strIterate, pText->v_x);
		ov_string_print(&strIterate, "%s%%22y%%22:%%22%f%%22,", strIterate, pText->v_y);
		ov_memstack_lock();
		ov_string_print(&strIterate, "%s%%22content%%22:%%22%s%%22,", strIterate, pText->v_content==NULL?"":cshmi_Object_prepareURIencode(pText->v_content));
		ov_memstack_unlock();
		ov_string_print(&strIterate, "%s%%22fontSize%%22:%%22%s%%22,", strIterate, pText->v_fontSize==NULL?"":cshmi_Object_prepareURIencode(pText->v_fontSize));
		ov_string_print(&strIterate, "%s%%22fontFamily%%22:%%22%s%%22,", strIterate, pText->v_fontFamily==NULL?"":cshmi_Object_prepareURIencode(pText->v_fontFamily));
		ov_string_print(&strIterate, "%s%%22fontStyle%%22:%%22%s%%22,", strIterate, pText->v_fontStyle==NULL?"":pText->v_fontStyle);	//this is a white list of know values
		ov_string_print(&strIterate, "%s%%22fontWeight%%22:%%22%s%%22,", strIterate, pText->v_fontWeight==NULL?"":pText->v_fontWeight);	//this is a white list of know values
		ov_string_print(&strIterate, "%s%%22horAlignment%%22:%%22%s%%22,", strIterate, pText->v_horAlignment==NULL?"":pText->v_horAlignment);	//this is a white list of know values
		ov_string_print(&strIterate, "%s%%22verAlignment%%22:%%22%s%%22,", strIterate, pText->v_verAlignment==NULL?"":pText->v_verAlignment);	//this is a white list of know values
		ov_string_print(&strIterate, "%s%%22trimToLength%%22:%%22%i%%22", strIterate, pText->v_trimToLength);
		ov_string_append(&strIterate, "%7D");
	}else if(pClass == pclass_cshmi_Line){
		pLine = Ov_StaticPtrCast(cshmi_Line, pObj);
		ov_string_append(&strIterate, "%22Parameters%22:%7B");
		cshmi_Object_buildBaseElementString(&strIterate, Ov_PtrUpCast(cshmi_Element, pLine));
		ov_string_print(&strIterate, "%s%%22x1%%22:%%22%f%%22,", strIterate, pLine->v_x1);
		ov_string_print(&strIterate, "%s%%22y1%%22:%%22%f%%22,", strIterate, pLine->v_y1);
		ov_string_print(&strIterate, "%s%%22x2%%22:%%22%f%%22,", strIterate, pLine->v_x2);
		ov_string_print(&strIterate, "%s%%22y2%%22:%%22%f%%22,", strIterate, pLine->v_y2);
		ov_string_print(&strIterate, "%s%%22strokeWidth%%22:%%22%f%%22", strIterate, pLine->v_strokeWidth);
		ov_string_append(&strIterate, "%7D");
	}else if(pClass == pclass_cshmi_Polyline){
		pPolyline = Ov_StaticPtrCast(cshmi_Polyline, pObj);
		ov_string_append(&strIterate, "%22Parameters%22:%7B");
		cshmi_Object_buildBaseElementString(&strIterate, Ov_PtrUpCast(cshmi_Element, pPolyline));
		ov_memstack_lock();
		ov_string_print(&strIterate, "%s%%22points%%22:%%22%s%%22,", strIterate, pPolyline->v_points==NULL?"":cshmi_Object_prepareURIencode(pPolyline->v_points));
		ov_memstack_unlock();
		ov_string_print(&strIterate, "%s%%22strokeWidth%%22:%%22%f%%22", strIterate, pPolyline->v_strokeWidth);
		ov_string_append(&strIterate, "%7D");
	}else if(pClass == pclass_cshmi_Polygon){
		pPolygon = Ov_StaticPtrCast(cshmi_Polygon, pObj);
		ov_string_append(&strIterate, "%22Parameters%22:%7B");
		cshmi_Object_buildBaseElementString(&strIterate, Ov_PtrUpCast(cshmi_Element, pPolygon));
		ov_memstack_lock();
		ov_string_print(&strIterate, "%s%%22points%%22:%%22%s%%22,", strIterate, pPolygon->v_points==NULL?"":cshmi_Object_prepareURIencode(pPolygon->v_points));
		ov_memstack_unlock();
		ov_string_print(&strIterate, "%s%%22strokeWidth%%22:%%22%f%%22", strIterate, pPolygon->v_strokeWidth);
		ov_string_append(&strIterate, "%7D");
	}else if(pClass == pclass_cshmi_Path){
		pPath = Ov_StaticPtrCast(cshmi_Path, pObj);
		ov_string_append(&strIterate, "%22Parameters%22:%7B");
		cshmi_Object_buildBaseElementString(&strIterate, Ov_PtrUpCast(cshmi_Element, pPath));
		ov_memstack_lock();
		ov_string_print(&strIterate, "%s%%22d%%22:%%22%s%%22,", strIterate, pPath->v_d==NULL?"":cshmi_Object_prepareURIencode(pPath->v_d));
		ov_memstack_unlock();
		ov_string_print(&strIterate, "%s%%22strokeWidth%%22:%%22%f%%22", strIterate, pPath->v_strokeWidth);
		ov_string_append(&strIterate, "%7D");
	}else if(pClass == pclass_cshmi_Ellipse){
		pEllipse = Ov_StaticPtrCast(cshmi_Ellipse, pObj);
		ov_string_append(&strIterate, "%22Parameters%22:%7B");
		cshmi_Object_buildBaseElementString(&strIterate, Ov_PtrUpCast(cshmi_Element, pEllipse));
		ov_string_print(&strIterate, "%s%%22cx%%22:%%22%f%%22,", strIterate, pEllipse->v_cx);
		ov_string_print(&strIterate, "%s%%22cy%%22:%%22%f%%22,", strIterate, pEllipse->v_cy);
		ov_string_print(&strIterate, "%s%%22rx%%22:%%22%f%%22,", strIterate, pEllipse->v_rx);
		ov_string_print(&strIterate, "%s%%22ry%%22:%%22%f%%22,", strIterate, pEllipse->v_ry);
		ov_string_print(&strIterate, "%s%%22strokeWidth%%22:%%22%f%%22", strIterate, pEllipse->v_strokeWidth);
		ov_string_append(&strIterate, "%7D");
	}else

	//here comes the actions and conditions
	if(pClass == pclass_cshmi_SetValue){
		pSetValue = Ov_StaticPtrCast(cshmi_SetValue, pObj);
		ov_string_print(&strIterate, "%s%%22translationSource%%22:%%22%s%%22,", strIterate, pSetValue->v_translationSource==NULL?"":cshmi_Object_prepareURIencode(pSetValue->v_translationSource));

		if(ov_string_compare(pSetValue->v_ksVar, "") != OV_STRCMP_EQUAL){
			ov_string_setvalue(&ParameterName, "ksVar");
			ov_string_setvalue(&ParameterValue, pSetValue->v_ksVar);
		}else if(ov_string_compare(pSetValue->v_elemVar, "") != OV_STRCMP_EQUAL){
			ov_string_setvalue(&ParameterName, "elemVar");
			ov_string_setvalue(&ParameterValue, pSetValue->v_elemVar);
		}else if(ov_string_compare(pSetValue->v_globalVar, "") != OV_STRCMP_EQUAL){
			ov_string_setvalue(&ParameterName, "globalVar");
			ov_string_setvalue(&ParameterValue, pSetValue->v_globalVar);
		}else if(ov_string_compare(pSetValue->v_persistentGlobalVar, "") != OV_STRCMP_EQUAL){
			ov_string_setvalue(&ParameterName, "persistentGlobalVar");
			ov_string_setvalue(&ParameterValue, pSetValue->v_persistentGlobalVar);
		}else if(ov_string_compare(pSetValue->v_TemplateFBReferenceVariable, "") != OV_STRCMP_EQUAL){
			ov_string_setvalue(&ParameterName, "TemplateFBReferenceVariable");
			ov_string_setvalue(&ParameterValue, pSetValue->v_TemplateFBReferenceVariable);
		}else if(ov_string_compare(pSetValue->v_TemplateFBVariableReferenceName, "") != OV_STRCMP_EQUAL){
			ov_string_setvalue(&ParameterName, "TemplateFBVariableReferenceName");
			ov_string_setvalue(&ParameterValue, pSetValue->v_TemplateFBVariableReferenceName);
		}else if(ov_string_compare(pSetValue->v_TemplateConfigValues, "") != OV_STRCMP_EQUAL){
			ov_string_setvalue(&ParameterName, "TemplateConfigValues");
			ov_string_setvalue(&ParameterValue, pSetValue->v_TemplateConfigValues);
		}else{
			ov_string_setvalue(&ParameterName, NULL);
			ov_string_setvalue(&ParameterValue, NULL);
		}
		ov_string_print(&strIterate, "%s%%22ParameterName%%22:%%22%s%%22,", strIterate, ParameterName==NULL?"":ParameterName);
		if(ov_string_compare(ParameterValue, NULL) != OV_STRCMP_EQUAL){
			ov_memstack_lock();
			ov_string_print(&strIterate, "%s%%22ParameterValue%%22:%%22%s%%22,", strIterate, cshmi_Object_prepareURIencode(ParameterValue));
			ov_memstack_unlock();
		}else{
			ov_string_print(&strIterate, "%s%%22ParameterValue%%22:%%22%%22,", strIterate);
		}
		ov_string_print(&strIterate, "%s%%22translationSource%%22:%%22%s%%22", strIterate, pSetValue->v_translationSource==NULL?"":cshmi_Object_prepareURIencode(pSetValue->v_translationSource));
	}else if(pClass == pclass_cshmi_SetConcatValue){
		pSetConcatValue = Ov_StaticPtrCast(cshmi_SetConcatValue, pObj);
		if(ov_string_compare(pSetConcatValue->v_ksVar, "") != OV_STRCMP_EQUAL){
			ov_string_setvalue(&ParameterName, "ksVar");
			ov_string_setvalue(&ParameterValue, pSetConcatValue->v_ksVar);
		}else if(ov_string_compare(pSetConcatValue->v_elemVar, "") != OV_STRCMP_EQUAL){
			ov_string_setvalue(&ParameterName, "elemVar");
			ov_string_setvalue(&ParameterValue, pSetConcatValue->v_elemVar);
		}else if(ov_string_compare(pSetConcatValue->v_globalVar, "") != OV_STRCMP_EQUAL){
			ov_string_setvalue(&ParameterName, "globalVar");
			ov_string_setvalue(&ParameterValue, pSetConcatValue->v_globalVar);
		}else if(ov_string_compare(pSetConcatValue->v_persistentGlobalVar, "") != OV_STRCMP_EQUAL){
			ov_string_setvalue(&ParameterName, "persistentGlobalVar");
			ov_string_setvalue(&ParameterValue, pSetConcatValue->v_persistentGlobalVar);
		}else if(ov_string_compare(pSetConcatValue->v_TemplateFBReferenceVariable, "") != OV_STRCMP_EQUAL){
			ov_string_setvalue(&ParameterName, "TemplateFBReferenceVariable");
			ov_string_setvalue(&ParameterValue, pSetConcatValue->v_TemplateFBReferenceVariable);
		}else if(ov_string_compare(pSetConcatValue->v_TemplateFBVariableReferenceName, "") != OV_STRCMP_EQUAL){
			ov_string_setvalue(&ParameterName, "TemplateFBVariableReferenceName");
			ov_string_setvalue(&ParameterValue, pSetConcatValue->v_TemplateFBVariableReferenceName);
		}else if(ov_string_compare(pSetConcatValue->v_TemplateConfigValues, "") != OV_STRCMP_EQUAL){
			ov_string_setvalue(&ParameterName, "TemplateConfigValues");
			ov_string_setvalue(&ParameterValue, pSetConcatValue->v_TemplateConfigValues);
		}else{
			ov_string_setvalue(&ParameterName, NULL);
			ov_string_setvalue(&ParameterValue, NULL);
		}
		ov_string_print(&strIterate, "%s%%22ParameterName%%22:%%22%s%%22,", strIterate, ParameterName==NULL?"":ParameterName);
		if(ov_string_compare(ParameterValue, NULL) != OV_STRCMP_EQUAL){
			ov_memstack_lock();
			ov_string_print(&strIterate, "%s%%22ParameterValue%%22:%%22%s%%22", strIterate, cshmi_Object_prepareURIencode(ParameterValue));
			ov_memstack_unlock();
		}else{
			ov_string_print(&strIterate, "%s%%22ParameterValue%%22:%%22%%22", strIterate);
		}
	}else if(pClass == pclass_cshmi_SetMathValue){
		pSetMathValue = Ov_StaticPtrCast(cshmi_SetMathValue, pObj);
		if(ov_string_compare(pSetMathValue->v_ksVar, "") != OV_STRCMP_EQUAL){
			ov_string_setvalue(&ParameterName, "ksVar");
			ov_string_setvalue(&ParameterValue, pSetMathValue->v_ksVar);
		}else if(ov_string_compare(pSetMathValue->v_elemVar, "") != OV_STRCMP_EQUAL){
			ov_string_setvalue(&ParameterName, "elemVar");
			ov_string_setvalue(&ParameterValue, pSetMathValue->v_elemVar);
		}else if(ov_string_compare(pSetMathValue->v_globalVar, "") != OV_STRCMP_EQUAL){
			ov_string_setvalue(&ParameterName, "globalVar");
			ov_string_setvalue(&ParameterValue, pSetMathValue->v_globalVar);
		}else if(ov_string_compare(pSetMathValue->v_persistentGlobalVar, "") != OV_STRCMP_EQUAL){
			ov_string_setvalue(&ParameterName, "persistentGlobalVar");
			ov_string_setvalue(&ParameterValue, pSetMathValue->v_persistentGlobalVar);
		}else if(ov_string_compare(pSetMathValue->v_TemplateFBReferenceVariable, "") != OV_STRCMP_EQUAL){
			ov_string_setvalue(&ParameterName, "TemplateFBReferenceVariable");
			ov_string_setvalue(&ParameterValue, pSetMathValue->v_TemplateFBReferenceVariable);
		}else if(ov_string_compare(pSetMathValue->v_TemplateFBVariableReferenceName, "") != OV_STRCMP_EQUAL){
			ov_string_setvalue(&ParameterName, "TemplateFBVariableReferenceName");
			ov_string_setvalue(&ParameterValue, pSetMathValue->v_TemplateFBVariableReferenceName);
		}else if(ov_string_compare(pSetMathValue->v_TemplateConfigValues, "") != OV_STRCMP_EQUAL){
			ov_string_setvalue(&ParameterName, "TemplateConfigValues");
			ov_string_setvalue(&ParameterValue, pSetMathValue->v_TemplateConfigValues);
		}else{
			ov_string_setvalue(&ParameterName, NULL);
			ov_string_setvalue(&ParameterValue, NULL);
		}
		ov_string_print(&strIterate, "%s%%22ParameterName%%22:%%22%s%%22,", strIterate, ParameterName==NULL?"":ParameterName);
		if(ov_string_compare(ParameterValue, NULL) != OV_STRCMP_EQUAL){
			ov_memstack_lock();
			ov_string_print(&strIterate, "%s%%22ParameterValue%%22:%%22%s%%22", strIterate, cshmi_Object_prepareURIencode(ParameterValue));
			ov_memstack_unlock();
		}else{
			ov_string_print(&strIterate, "%s%%22ParameterValue%%22:%%22%%22", strIterate);
		}
	}else if(pClass == pclass_cshmi_GetValue){
		pGetValue = Ov_StaticPtrCast(cshmi_GetValue, pObj);
		if(ov_string_compare(pGetValue->v_ksVar, "") != OV_STRCMP_EQUAL){
			ov_string_setvalue(&ParameterName, "ksVar");
			ov_string_setvalue(&ParameterValue, pGetValue->v_ksVar);
		}else if(ov_string_compare(pGetValue->v_elemVar, "") != OV_STRCMP_EQUAL){
			ov_string_setvalue(&ParameterName, "elemVar");
			ov_string_setvalue(&ParameterValue, pGetValue->v_elemVar);
		}else if(ov_string_compare(pGetValue->v_globalVar, "") != OV_STRCMP_EQUAL){
			ov_string_setvalue(&ParameterName, "globalVar");
			ov_string_setvalue(&ParameterValue, pGetValue->v_globalVar);
		}else if(ov_string_compare(pGetValue->v_persistentGlobalVar, "") != OV_STRCMP_EQUAL){
			ov_string_setvalue(&ParameterName, "persistentGlobalVar");
			ov_string_setvalue(&ParameterValue, pGetValue->v_persistentGlobalVar);
		}else if(ov_string_compare(pGetValue->v_OperatorInput, "") != OV_STRCMP_EQUAL){
			ov_string_setvalue(&ParameterName, "OperatorInput");
			ov_string_setvalue(&ParameterValue, pGetValue->v_OperatorInput);
		}else if(ov_string_compare(pGetValue->v_TemplateFBReferenceVariable, "") != OV_STRCMP_EQUAL){
			ov_string_setvalue(&ParameterName, "TemplateFBReferenceVariable");
			ov_string_setvalue(&ParameterValue, pGetValue->v_TemplateFBReferenceVariable);
		}else if(ov_string_compare(pGetValue->v_TemplateFBVariableReferenceName, "") != OV_STRCMP_EQUAL){
			ov_string_setvalue(&ParameterName, "TemplateFBVariableReferenceName");
			ov_string_setvalue(&ParameterValue, pGetValue->v_TemplateFBVariableReferenceName);
		}else if(ov_string_compare(pGetValue->v_TemplateConfigValues, "") != OV_STRCMP_EQUAL){
			ov_string_setvalue(&ParameterName, "TemplateConfigValues");
			ov_string_setvalue(&ParameterValue, pGetValue->v_TemplateConfigValues);
		}else{
			ov_string_setvalue(&ParameterName, "value");
			switch (pGetValue->v_value.value.vartype) {
				case OV_VT_STRING_PV:
					ov_string_setvalue(&ParameterValue, pGetValue->v_value.value.valueunion.val_string);
					break;
				case OV_VT_BOOL_PV:
					ov_string_print(&ParameterValue, "%" OV_PRINT_STRING, (pGetValue->v_value.value.valueunion.val_bool==TRUE?"TRUE":"FALSE"));
					break;
				case OV_VT_INT_PV:
					ov_string_print(&ParameterValue, "%" OV_PRINT_INT, pGetValue->v_value.value.valueunion.val_int);
					break;
				case OV_VT_UINT_PV:
					ov_string_print(&ParameterValue, "%" OV_PRINT_UINT, pGetValue->v_value.value.valueunion.val_uint);
					break;
				case OV_VT_SINGLE_PV:
					ov_string_print(&ParameterValue, "%" OV_PRINT_SINGLE, pGetValue->v_value.value.valueunion.val_single);
					break;
				case OV_VT_DOUBLE_PV:
					ov_string_print(&ParameterValue, "%" OV_PRINT_DOUBLE, pGetValue->v_value.value.valueunion.val_double);
					break;
				case OV_VT_STRING_PV_VEC:
					if(pGetValue->v_value.value.valueunion.val_string_vec.veclen==0){
						ov_string_setvalue(&ParameterValue, NULL);
					}else if(pGetValue->v_value.value.valueunion.val_string_vec.veclen==1){
						ov_string_setvalue(&ParameterValue, pGetValue->v_value.value.valueunion.val_string_vec.value[0]);
					}else{
						ov_string_setvalue(&ParameterValue, pGetValue->v_value.value.valueunion.val_string_vec.value[0]);
						for ( i = 1; i < pGetValue->v_value.value.valueunion.val_string_vec.veclen;i++){
							ov_string_print(&ParameterValue, "%s %s", ParameterValue, pGetValue->v_value.value.valueunion.val_string_vec.value[i]);
						}
					}
					break;
				case OV_VT_VOID:
					ov_string_setvalue(&ParameterValue, NULL);
					break;
				default:
					//The format of the value is not supported, so skip precaching of the element
					Ov_HeapFree(pObj->v_ConfigCache.asJSON);
					pObj->v_ConfigCache.asJSON = NULL;
					pObj->v_ConfigCache.cacheDirty = FALSE;
					pObj->v_ConfigCache.parentObject = Ov_GetParent(ov_containment, pObj);
					ov_memstack_lock();
					ov_string_setvalue(&pObj->v_ConfigCache.identifier, ov_path_getcanonicalpath(Ov_PtrUpCast(ov_object, pObj),2));
					ov_memstack_unlock();

					ov_string_setvalue(&ParameterName, NULL);
					ov_string_setvalue(&ParameterValue, NULL);
					ov_string_setvalue(&temp, NULL);
					ov_string_setvalue(&strIterate, NULL);
					return OV_ERR_NOTIMPLEMENTED;
			}
		}
		ov_string_print(&strIterate, "%s%%22ParameterName%%22:%%22%s%%22,", strIterate, ParameterName==NULL?"":ParameterName);
		if(ov_string_compare(ParameterValue, NULL) != OV_STRCMP_EQUAL){
			ov_memstack_lock();
			ov_string_print(&strIterate, "%s%%22ParameterValue%%22:%%22%s%%22", strIterate, cshmi_Object_prepareURIencode(ParameterValue));
			ov_memstack_unlock();
		}else{
			ov_string_print(&strIterate, "%s%%22ParameterValue%%22:%%22%%22", strIterate);
		}
	}else if(pClass == pclass_cshmi_ChildrenIterator){
		pChildrenIterator = Ov_StaticPtrCast(cshmi_ChildrenIterator, pObj);
		ov_string_print(&strIterate, "%s%%22ChildrenIteratorParameterChildrenType%%22:%%22%s%%22", strIterate, pChildrenIterator->v_ChildrenType==NULL?"":cshmi_Object_prepareURIencode(pChildrenIterator->v_ChildrenType));
	}else if(pClass == pclass_cshmi_IfThenElse){
		pIfThenElse = Ov_StaticPtrCast(cshmi_IfThenElse, pObj);
		ov_string_print(&strIterate, "%s%%22IfThenElseParameterAnycond%%22:%%22%s%%22", strIterate, pIfThenElse->v_anycond==TRUE?"TRUE":"FALSE");
	}else if(pClass == pclass_cshmi_Compare){
		pCompare = Ov_StaticPtrCast(cshmi_Compare, pObj);
		ov_string_append(&strIterate, "%22Parameters%22:%7B");
		ov_string_print(&strIterate, "%s%%22comptype%%22:%%22%s%%22,", strIterate, pCompare->v_comptype==NULL?"":pCompare->v_comptype);	//this is a white list, no need to escape
		ov_string_print(&strIterate, "%s%%22ignoreCase%%22:%%22%s%%22,", strIterate, pCompare->v_ignoreCase==TRUE?"TRUE":"FALSE");
		ov_string_print(&strIterate, "%s%%22ignoreError%%22:%%22%s%%22", strIterate, pCompare->v_ignoreError==TRUE?"TRUE":"FALSE");
		ov_string_append(&strIterate, "%7D");
	}else if(pClass == pclass_cshmi_CompareIteratedChild){
		pCompareIteratedChild = Ov_StaticPtrCast(cshmi_CompareIteratedChild, pObj);
		ov_string_append(&strIterate, "%22Parameters%22:%7B");

		ov_string_print(&strIterate, "%s%%22comptype%%22:%%22%s%%22,", strIterate, pCompareIteratedChild->v_comptype==NULL?"":pCompareIteratedChild->v_comptype);
		ov_string_print(&strIterate, "%s%%22childValue%%22:%%22%s%%22,", strIterate, pCompareIteratedChild->v_childValue==NULL?"":pCompareIteratedChild->v_childValue);	//this is a white list
		ov_string_print(&strIterate, "%s%%22ignoreCase%%22:%%22%s%%22,", strIterate, pCompareIteratedChild->v_ignoreCase==TRUE?"TRUE":"FALSE");
		ov_string_print(&strIterate, "%s%%22ignoreError%%22:%%22%s%%22", strIterate, pCompareIteratedChild->v_ignoreError==TRUE?"TRUE":"FALSE");
		ov_string_append(&strIterate, "%7D");
	}else if(pClass == pclass_cshmi_TimeEvent){
		pTimeEvent = Ov_StaticPtrCast(cshmi_TimeEvent, pObj);
		ov_string_append(&strIterate, "%22Parameters%22:%7B");
		ov_string_print(&strIterate, "%s%%22cyctime%%22:%%22%g%%22", strIterate, pTimeEvent->v_cyctime);
		ov_string_append(&strIterate, "%7D");
	}else if(pClass == pclass_cshmi_RoutePolyline){
		pRoutePolyline = Ov_StaticPtrCast(cshmi_RoutePolyline, pObj);
		ov_string_append(&strIterate, "%22Parameters%22:%7B");
		ov_string_print(&strIterate, "%s%%22gridWidth%%22:%%22%f%%22,", strIterate, pRoutePolyline->v_gridWidth);
		ov_string_print(&strIterate, "%s%%22offset%%22:%%22%f%%22", strIterate, pRoutePolyline->v_offset);
		ov_string_append(&strIterate, "%7D");
	}else if(pClass == pclass_cshmi_TranslationSource){
		pTranslationSource = Ov_StaticPtrCast(cshmi_TranslationSource, pObj);
		ov_string_append(&strIterate, "%22Parameters%22:%7B");
		if(pTranslationSource->v_translationMapping.veclen == 0){
			ov_string_setvalue(&temp, " ");
		}else if(pTranslationSource->v_translationMapping.veclen == 1){
			ov_memstack_lock();
			ov_string_print(&temp, "%%22%s%%22", cshmi_Object_prepareURIencode(pTranslationSource->v_translationMapping.value[0]));
			ov_memstack_unlock();
		}else{
			ov_memstack_lock();
			ov_string_print(&temp, "%%22%s%%22", cshmi_Object_prepareURIencode(pTranslationSource->v_translationMapping.value[0]));
			ov_memstack_unlock();
			for(i = 1; i < pTranslationSource->v_translationMapping.veclen;i++){
				ov_memstack_lock();
				ov_string_print(&temp, "%s,%%22%s%%22", temp, cshmi_Object_prepareURIencode(pTranslationSource->v_translationMapping.value[i]));
				ov_memstack_unlock();
			}
		}
		ov_string_print(&strIterate, "%s%%22TranslationMappingRaw%%22:%%5B%s%%5D", strIterate, temp);

		ov_string_append(&strIterate, "%7D");
	}else if(pClass == pclass_cshmi_CreateObject){
		pCreateObject = Ov_StaticPtrCast(cshmi_CreateObject, pObj);
		ov_string_append(&strIterate, "%22Parameters%22:%7B");
		ov_string_print(&strIterate, "%s%%22autoRenameIfExists%%22:%%22%s%%22", strIterate, pCreateObject->v_autoRenameIfExists==TRUE?"TRUE":"FALSE");
		ov_string_append(&strIterate, "%7D");
	}else if(pClass == pclass_cshmi_Vibrate){
		pVibrate = Ov_StaticPtrCast(cshmi_Vibrate, pObj);
		ov_string_append(&strIterate, "%22Parameters%22:%7B");
		if(pVibrate->v_pattern.veclen == 0){
			ov_string_setvalue(&temp, " ");
		}else if(pVibrate->v_pattern.veclen == 1){
			ov_memstack_lock();
			ov_string_print(&temp, "%u", pVibrate->v_pattern.value[0]);
			ov_memstack_unlock();
		}else{
			ov_memstack_lock();
			ov_string_print(&temp, "%u", pVibrate->v_pattern.value[0]);
			ov_memstack_unlock();
			for(i = 1; i < pVibrate->v_pattern.veclen;i++){
				ov_memstack_lock();
				ov_string_print(&temp, "%s,%u", temp, pVibrate->v_pattern.value[i]);
				ov_memstack_unlock();
			}
		}
		ov_string_print(&strIterate, "%s%%22pattern%%22:%%5B%s%%5D", strIterate, cshmi_Object_prepareURIencode(temp));
		ov_string_append(&strIterate, "%7D");
	}else{
		//The object type is not supported, so skip the precaching of the element
		//Note: we do not precache blackbox or image! To risky with the encoding of the Strings.
		Ov_HeapFree(pObj->v_ConfigCache.asJSON);
		pObj->v_ConfigCache.asJSON = NULL;
		pObj->v_ConfigCache.cacheDirty = FALSE;
		pObj->v_ConfigCache.parentObject = Ov_GetParent(ov_containment, pObj);
		ov_memstack_lock();
		ov_string_setvalue(&pObj->v_ConfigCache.identifier, ov_path_getcanonicalpath(Ov_PtrUpCast(ov_object, pObj),2));
		ov_memstack_unlock();

		ov_string_setvalue(&ParameterName, NULL);
		ov_string_setvalue(&ParameterValue, NULL);
		ov_string_setvalue(&temp, NULL);
		ov_string_setvalue(&strIterate, NULL);
		return OV_ERR_NOTIMPLEMENTED;
	}
	ov_string_append(&strIterate, "%7D");

	strLen = ov_string_getlength(strIterate);
	//copy the string with its termination null
	if(strLen != 0){
		if(pObj->v_ConfigCache.asJSON != NULL){
			Ov_HeapFree(pObj->v_ConfigCache.asJSON);
		}
		pObj->v_ConfigCache.asJSON = Ov_HeapMalloc(strLen+1);
		memcpy(pObj->v_ConfigCache.asJSON, strIterate, strLen+1);
	}else{
		Ov_HeapFree(pObj->v_ConfigCache.asJSON);
		pObj->v_ConfigCache.asJSON = NULL;
	}
	pObj->v_ConfigCache.cacheDirty = FALSE;
	pObj->v_ConfigCache.parentObject = Ov_GetParent(ov_containment, pObj);
	ov_memstack_lock();
	ov_string_setvalue(&pObj->v_ConfigCache.identifier, ov_path_getcanonicalpath(Ov_PtrUpCast(ov_object, pObj),2));
	ov_memstack_unlock();
	ov_string_setvalue(&ParameterName, NULL);
	ov_string_setvalue(&ParameterValue, NULL);
	ov_string_setvalue(&temp, NULL);
	ov_string_setvalue(&strIterate, NULL);

	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_STRING cshmi_Object_getConfigAsJSON(
	OV_INSTPTR_cshmi_Object          pObj
) {
	OV_INSTPTR_cshmi_downloadApplication pDownloadApplication = NULL;
	OV_STRING fullpathname = NULL;

	ov_memstack_lock();
	ov_string_setvalue(&fullpathname, ov_path_getcanonicalpath(Ov_PtrUpCast(ov_object, pObj),2));
	ov_memstack_unlock();

	//update the cache if...
	if(		pObj->v_ConfigCache.parentObject != Ov_GetParent(ov_containment, pObj)
		||	ov_string_compare(pObj->v_ConfigCache.identifier, fullpathname) != OV_STRCMP_EQUAL
	){
		//...the parent is different (the object was moved)
		cshmi_Object_updateConfigAsJSON(pObj);

		// and clear childlist of turbo
		pDownloadApplication =  Ov_StaticPtrCast(cshmi_downloadApplication, Ov_GetFirstChild(ov_instantiation, pclass_cshmi_downloadApplication));
		if(pDownloadApplication != NULL){
			//throw away childlist from turbo
			CSHMI_EMPTYCLASSCACHEENTRY(Childlist);
		}
	}else if(pObj->v_ConfigCache.cacheDirty == TRUE){
		//... the cache is dirty (a parameter has changed)
		cshmi_Object_updateConfigAsJSON(pObj);
	}
	ov_string_setvalue(&fullpathname, NULL);
	return pObj->v_ConfigCache.asJSON;
}

OV_DLLFNCEXPORT OV_RESULT cshmi_Object_resetCache(
	OV_INSTPTR_cshmi_Object          pobj
) {
	OV_INSTPTR_cshmi_downloadApplication pDownloadApplication = Ov_StaticPtrCast(cshmi_downloadApplication, Ov_GetFirstChild(ov_instantiation, pclass_cshmi_downloadApplication));

	//mark our cache dirty
	pobj->v_ConfigCache.cacheDirty = TRUE;

	if(pDownloadApplication == NULL){
		//we have no turbo, so nothing to do
	}else if(Ov_CanCastTo(cshmi_Group, pobj)){
		CSHMI_EMPTYCLASSCACHEENTRY(Group);
	}else if(Ov_CanCastTo(cshmi_TemplateDefinition, pobj)){
		CSHMI_EMPTYCLASSCACHEENTRY(TemplateDefinition);
	}else if(Ov_CanCastTo(cshmi_InstantiateTemplate, pobj)){
		CSHMI_EMPTYCLASSCACHEENTRY(InstantiateTemplate);
	}else if(Ov_CanCastTo(cshmi_Rectangle, pobj)){
		CSHMI_EMPTYCLASSCACHEENTRY(Rectangle);
	}else if(Ov_CanCastTo(cshmi_Circle, pobj)){
		CSHMI_EMPTYCLASSCACHEENTRY(Circle);
	}else if(Ov_CanCastTo(cshmi_Text, pobj)){
		CSHMI_EMPTYCLASSCACHEENTRY(Text);
	}else if(Ov_CanCastTo(cshmi_Line, pobj)){
		CSHMI_EMPTYCLASSCACHEENTRY(Line);
	}else if(Ov_CanCastTo(cshmi_Polyline, pobj)){
		CSHMI_EMPTYCLASSCACHEENTRY(Polyline);
	}else if(Ov_CanCastTo(cshmi_Polygon, pobj)){
		CSHMI_EMPTYCLASSCACHEENTRY(Polygon);
	}else if(Ov_CanCastTo(cshmi_Path, pobj)){
		CSHMI_EMPTYCLASSCACHEENTRY(Path);
	}else if(Ov_CanCastTo(cshmi_Ellipse, pobj)){
		CSHMI_EMPTYCLASSCACHEENTRY(Ellipse);

	}else if(Ov_CanCastTo(cshmi_SetValue, pobj)){
		CSHMI_EMPTYCLASSCACHEENTRY(SetValue);
	}else if(Ov_CanCastTo(cshmi_SetConcatValue, pobj)){
		CSHMI_EMPTYCLASSCACHEENTRY(SetConcatValue);
	}else if(Ov_CanCastTo(cshmi_SetMathValue, pobj)){
		CSHMI_EMPTYCLASSCACHEENTRY(SetMathValue);
	}else if(Ov_CanCastTo(cshmi_GetValue, pobj)){
		CSHMI_EMPTYCLASSCACHEENTRY(GetValue);
	}else if(Ov_CanCastTo(cshmi_ChildrenIterator, pobj)){
		CSHMI_EMPTYCLASSCACHEENTRY(ChildrenIterator);
	}else if(Ov_CanCastTo(cshmi_IfThenElse, pobj)){
		CSHMI_EMPTYCLASSCACHEENTRY(IfThenElse);
	}else if(Ov_CanCastTo(cshmi_Compare, pobj)){
		CSHMI_EMPTYCLASSCACHEENTRY(Compare);
	}else if(Ov_CanCastTo(cshmi_CompareIteratedChild, pobj)){
		CSHMI_EMPTYCLASSCACHEENTRY(CompareIteratedChild);
	}else if(Ov_CanCastTo(cshmi_TimeEvent, pobj)){
		CSHMI_EMPTYCLASSCACHEENTRY(TimeEvent);
	}else if(Ov_CanCastTo(cshmi_RoutePolyline, pobj)){
		CSHMI_EMPTYCLASSCACHEENTRY(RoutePolyline);
	}else if(Ov_CanCastTo(cshmi_TranslationSource, pobj)){
		CSHMI_EMPTYCLASSCACHEENTRY(TranslationSource);
	}else if(Ov_CanCastTo(cshmi_CreateObject, pobj)){
		CSHMI_EMPTYCLASSCACHEENTRY(CreateObject);
	}else if(Ov_CanCastTo(cshmi_Vibrate, pobj)){
		CSHMI_EMPTYCLASSCACHEENTRY(Vibrate);
	}

	return OV_ERR_OK;
}
