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
*	Group.c
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



OV_DLLFNCEXPORT OV_RESULT cshmi_Group_x_set(
		OV_INSTPTR_cshmi_Group          pobj,
		const OV_SINGLE  value
) {
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
	pobj->v_x = value;
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT cshmi_Group_y_set(
		OV_INSTPTR_cshmi_Group          pobj,
		const OV_SINGLE  value
) {
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
	pobj->v_y = value;
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT cshmi_Group_hideable_set(
		OV_INSTPTR_cshmi_Group          pobj,
		const OV_BOOL  value
) {
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
	pobj->v_hideable = value;
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT cshmi_Group_visible_set(
		OV_INSTPTR_cshmi_Group          pobj,
		const OV_BOOL  value
) {
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
	pobj->v_visible = value;
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT cshmi_Group_FBReference_set(
		OV_INSTPTR_cshmi_Group          pobj,
		const OV_STRING*  value,
		const OV_UINT veclen
) {
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
	return Ov_SetDynamicVectorValue(&pobj->v_FBReference,value,veclen,STRING);
}

OV_DLLFNCEXPORT OV_RESULT cshmi_Group_FBVariableReference_set(
		OV_INSTPTR_cshmi_Group          pobj,
		const OV_STRING*  value,
		const OV_UINT veclen
) {
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
	return Ov_SetDynamicVectorValue(&pobj->v_FBVariableReference,value,veclen,STRING);
}

OV_DLLFNCEXPORT OV_RESULT cshmi_Group_ConfigValues_set(
		OV_INSTPTR_cshmi_Group          pobj,
		const OV_STRING*  value,
		const OV_UINT veclen
) {
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
	return Ov_SetDynamicVectorValue(&pobj->v_ConfigValues,value,veclen,STRING);
}

OV_DLLFNCEXPORT OV_RESULT cshmi_Group_width_set(
	OV_INSTPTR_cshmi_Group          pobj,
	const OV_SINGLE  value
) {
	if (value <= 0){
		return OV_ERR_BADPARAM;
	}
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
	pobj->v_width = value;
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT cshmi_Group_height_set(
	OV_INSTPTR_cshmi_Group          pobj,
	const OV_SINGLE  value
) {
	if (value <= 0){
		return OV_ERR_BADPARAM;
	}
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
	pobj->v_height = value;
	return OV_ERR_OK;
}


OV_DLLFNCEXPORT OV_RESULT cshmi_Group_opacity_set(
	OV_INSTPTR_cshmi_Group          pobj,
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

OV_DLLFNCEXPORT OV_RESULT cshmi_Group_rotate_set(
	OV_INSTPTR_cshmi_Group          pobj,
	const OV_INT  value
) {
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
	pobj->v_rotate = value;
	while(pobj->v_rotate > 360.0) pobj->v_rotate -= 360.0;
	while(pobj->v_rotate < 0.0) pobj->v_rotate += 360.0;
	return OV_ERR_OK;
}

/**
 * prevents recursive template usage
 */
OV_DLLFNCEXPORT OV_RESULT cshmi_Group_TemplateDefinition_set(
		OV_INSTPTR_cshmi_Group          pobj,
	const OV_STRING  value
) {
	OV_STRING fullpath = NULL;
	OV_STRING mask = NULL;

	ov_memstack_lock();
	ov_string_setvalue(&fullpath, ov_path_getcanonicalpath(Ov_PtrUpCast(ov_object, pobj), 2));
	ov_memstack_unlock();

	//the path to the templates is hardcoded in the visualisation system
	ov_string_print(&mask, "/TechUnits/cshmi/Templates/%s/*", value);
	if (ov_string_match(fullpath, mask)){
		//avoid recursive template usage
		ov_string_setvalue(&fullpath, NULL);
		ov_string_setvalue(&mask, NULL);
		return OV_ERR_BADPARAM;
	}
	ov_string_setvalue(&fullpath, NULL);
	ov_string_setvalue(&mask, NULL);

	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
	return ov_string_setvalue(&pobj->v_TemplateDefinition,value);
}

