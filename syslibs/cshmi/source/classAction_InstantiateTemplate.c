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
*	classAction_InstantiateTemplate.c
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
*	12-Januar-2012			Yannick Rocks		V0.1.0
*		-	File created
*
***********************************************************************/

#ifndef OV_COMPILE_LIBRARY_cshmi
#define OV_COMPILE_LIBRARY_cshmi
#endif


#include "cshmilib.h"


OV_DLLFNCEXPORT OV_RESULT cshmi_InstantiateTemplate_TemplateDefinition_set(
		OV_INSTPTR_cshmi_InstantiateTemplate          pobj,
		const OV_STRING  value
) {
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
	return ov_string_setvalue(&pobj->v_TemplateDefinition,value);
}

OV_DLLFNCEXPORT OV_RESULT cshmi_InstantiateTemplate_x_set(
		OV_INSTPTR_cshmi_InstantiateTemplate          pobj,
		const OV_SINGLE  value
) {
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
	pobj->v_x = value;
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT cshmi_InstantiateTemplate_y_set(
		OV_INSTPTR_cshmi_InstantiateTemplate          pobj,
		const OV_SINGLE  value
) {
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
	pobj->v_y = value;
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT cshmi_InstantiateTemplate_xOffset_set(
		OV_INSTPTR_cshmi_InstantiateTemplate          pobj,
		const OV_SINGLE  value
) {
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
	pobj->v_xOffset = value;
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT cshmi_InstantiateTemplate_yOffset_set(
		OV_INSTPTR_cshmi_InstantiateTemplate          pobj,
		const OV_SINGLE  value
) {
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
	pobj->v_yOffset = value;
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT cshmi_InstantiateTemplate_maxTemplatesPerDirection_set(
		OV_INSTPTR_cshmi_InstantiateTemplate          pobj,
		const OV_STRING  value
) {
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
	return ov_string_setvalue(&pobj->v_maxTemplatesPerDirection,value);
}

OV_DLLFNCEXPORT OV_RESULT cshmi_InstantiateTemplate_FBReference_set(
		OV_INSTPTR_cshmi_InstantiateTemplate          pobj,
		const OV_STRING*  value,
		const OV_UINT veclen
) {
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
	return Ov_SetDynamicVectorValue(&pobj->v_FBReference,value,veclen,STRING);
}

OV_DLLFNCEXPORT OV_RESULT cshmi_InstantiateTemplate_FBVariableReference_set(
		OV_INSTPTR_cshmi_InstantiateTemplate          pobj,
		const OV_STRING*  value,
		const OV_UINT veclen
) {
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
	return Ov_SetDynamicVectorValue(&pobj->v_FBVariableReference,value,veclen,STRING);
}

OV_DLLFNCEXPORT OV_RESULT cshmi_InstantiateTemplate_ConfigValues_set(
		OV_INSTPTR_cshmi_InstantiateTemplate          pobj,
		const OV_STRING*  value,
		const OV_UINT veclen
) {
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
	return Ov_SetDynamicVectorValue(&pobj->v_ConfigValues,value,veclen,STRING);
}

OV_DLLFNCEXPORT OV_RESULT cshmi_InstantiateTemplate_preventClone_set(
		OV_INSTPTR_cshmi_InstantiateTemplate          pobj,
		const OV_BOOL  value
) {
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
	pobj->v_preventClone = value;
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT cshmi_InstantiateTemplate_visible_set(
		OV_INSTPTR_cshmi_InstantiateTemplate          pobj,
		const OV_BOOL  value
) {
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
	pobj->v_visible = value;
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT cshmi_InstantiateTemplate_stroke_set(
		OV_INSTPTR_cshmi_InstantiateTemplate          pobj,
		const OV_STRING  value
) {
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
	return ov_string_setvalue(&pobj->v_stroke,value);
}

OV_DLLFNCEXPORT OV_RESULT cshmi_InstantiateTemplate_fill_set(
		OV_INSTPTR_cshmi_InstantiateTemplate          pobj,
		const OV_STRING  value
) {
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
	return ov_string_setvalue(&pobj->v_fill,value);
}



OV_DLLFNCEXPORT OV_RESULT cshmi_InstantiateTemplate_hideable_set(
		OV_INSTPTR_cshmi_InstantiateTemplate          pobj,
		const OV_BOOL  value
) {
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
	pobj->v_hideable = value;
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT cshmi_InstantiateTemplate_opacity_set(
	OV_INSTPTR_cshmi_InstantiateTemplate          pobj,
	const OV_SINGLE  value
) {
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
	if(value > 1.0){
		pobj->v_opacity = 1;
	}else if(value < 0.0){
		pobj->v_opacity = 0;
	}else{
		pobj->v_opacity = value;
	}
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT cshmi_InstantiateTemplate_rotate_set(
	OV_INSTPTR_cshmi_InstantiateTemplate          pobj,
	const OV_INT  value
) {
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
	pobj->v_rotate = value;
	while(pobj->v_rotate > 360.0) pobj->v_rotate -= 360.0;
	while(pobj->v_rotate < 0.0) pobj->v_rotate += 360.0;
	return OV_ERR_OK;
}
