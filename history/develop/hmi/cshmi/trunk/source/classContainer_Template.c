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

