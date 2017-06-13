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
*	classContainer_Template.c
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
*	18-October-2011			Je		V0.1.0
*		-	File created
*
***********************************************************************/

#ifndef OV_COMPILE_LIBRARY_cshmi
#define OV_COMPILE_LIBRARY_cshmi
#endif


#include "cshmilib.h"

/**
 * change itself to an group on startup
 */
OV_DLLFNCEXPORT void cshmi_Template_startup(
	OV_INSTPTR_ov_object 	pobj
) {
	/*
	*   local variables
	*/
	OV_INSTPTR_cshmi_Template pinst = Ov_StaticPtrCast(cshmi_Template, pobj);
	OV_INSTPTR_cshmi_Group pGroup = NULL;
	OV_STRING desiredName = NULL;
	OV_STRING tempname = NULL;
	OV_RESULT    result;

	/* do what the base class does first */
	ov_object_startup(pobj);

	ov_string_setvalue(&desiredName, pobj->v_identifier);
	ov_string_print(&tempname, "temporaryrenamed_%s", pobj->v_identifier);

	result = ov_class_renameobject(pobj, Ov_GetParent(ov_containment, pobj), tempname, OV_PMH_DEFAULT, NULL);
	ov_string_setvalue(&tempname, NULL);
	if(Ov_Fail(result)){
		return;
	}
	result = Ov_CreateObject(cshmi_Group, pGroup, Ov_GetParent(ov_containment, pobj), desiredName);
	ov_string_setvalue(&desiredName, NULL);
	if(Ov_Fail(result)){
		return;
	}

	ov_string_setvalue(&pGroup->v_TemplateDefinition, pinst->v_TemplateDefinition);
	pGroup->v_x = pinst->v_x;
	pGroup->v_y = pinst->v_y;
	Ov_SetDynamicVectorValue(&pGroup->v_FBReference, pinst->v_FBReference.value, pinst->v_FBReference.veclen, STRING);
	Ov_SetDynamicVectorValue(&pGroup->v_FBVariableReference, pinst->v_FBVariableReference.value, pinst->v_FBVariableReference.veclen, STRING);
	Ov_SetDynamicVectorValue(&pGroup->v_ConfigValues, pinst->v_ConfigValues.value, pinst->v_ConfigValues.veclen, STRING);
	pGroup->v_hideable = pinst->v_hideable;
	pGroup->v_visible = pinst->v_visible;
	pGroup->v_opacity = pinst->v_opacity;
	pGroup->v_rotate = pinst->v_rotate;
	ov_string_setvalue(&pGroup->v_baseKsPath, pinst->v_baseKsPath);

	Ov_DeleteObject(pobj);

	return;
}

