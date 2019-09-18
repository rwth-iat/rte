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
*	Element.c
*
*	Editors:
*	--------
*	Je							Holger Jeromin <jeromin@plt.rwth-aachen.de>
*	GQ							Gustavo Quiros <g.quiros@plt.rwth-aachen.de>
*	St							Stefan Schmitz <StefanS@plt.rwth-aachen.de>
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

OV_DLLFNCEXPORT OV_RESULT cshmi_Element_constructor(
	OV_INSTPTR_ov_object 	pobj
) {
	//	local variables
	OV_RESULT    result;
	OV_INSTPTR_ov_domain
		pParent = NULL;

	/* do what the base class does first */
	result = cshmi_Object_constructor(pobj);
	if(Ov_Fail(result))
		return result;

	//force correct placement
	pParent = Ov_GetParent(ov_containment, pobj);
	if (pParent != NULL){
		//Element allowed only under ContainerElements, Templates and not under csContainers
		if (!( Ov_CanCastTo(cshmi_ContainerElement, pParent) || Ov_CanCastTo(cshmi_Template, pParent) ) ||
			Ov_CanCastTo(cshmi_csContainer, pParent)
		){
			ov_logfile_warning("An element is not allowed below this parent. Element: %s, parent: %s", pobj->v_identifier, pParent->v_identifier);
			return OV_ERR_BADPLACEMENT;
		}
	}

	return OV_ERR_OK;
}


OV_DLLFNCEXPORT OV_RESULT cshmi_Element_visible_set(
		OV_INSTPTR_cshmi_Element          pobj,
		const OV_BOOL  value
) {
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
	pobj->v_visible = value;
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT cshmi_Element_stroke_set(
		OV_INSTPTR_cshmi_Element          pobj,
		const OV_STRING  value
) {
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
	return ov_string_setvalue(&pobj->v_stroke,value);
}

OV_DLLFNCEXPORT OV_RESULT cshmi_Element_fill_set(
		OV_INSTPTR_cshmi_Element          pobj,
		const OV_STRING  value
) {
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
	return ov_string_setvalue(&pobj->v_fill,value);
}


OV_DLLFNCEXPORT OV_RESULT cshmi_Element_rotate_set(
	OV_INSTPTR_cshmi_Element          pobj,
	const OV_INT  value
) {
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
	pobj->v_rotate = value;
	while(pobj->v_rotate > 360.0) pobj->v_rotate -= 360.0;
	while(pobj->v_rotate < 0.0) pobj->v_rotate += 360.0;
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT cshmi_Element_opacity_set(
	OV_INSTPTR_cshmi_Element          pobj,
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

OV_DLLFNCEXPORT OV_RESULT cshmi_ShapeElement_strokeWidth_set(
	OV_INSTPTR_cshmi_ShapeElement          pobj,
	const OV_SINGLE  value
) {
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
	pobj->v_strokeWidth = value;
	return OV_ERR_OK;
}
