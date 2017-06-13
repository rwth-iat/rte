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
*	classContainer_TemplateDefinition.c
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

OV_DLLFNCEXPORT OV_RESULT cshmi_TemplateDefinition_width_set(
	OV_INSTPTR_cshmi_TemplateDefinition          pobj,
	const OV_SINGLE  value
) {
	if (value <= 0){
		return OV_ERR_BADPARAM;
	}
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
	pobj->v_width = value;
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT cshmi_TemplateDefinition_height_set(
	OV_INSTPTR_cshmi_TemplateDefinition          pobj,
	const OV_SINGLE  value
) {
	if (value <= 0){
		return OV_ERR_BADPARAM;
	}
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
	pobj->v_height = value;
	return OV_ERR_OK;
}


OV_DLLFNCEXPORT OV_STRING cshmi_TemplateDefinition_getcomment(
	OV_INSTPTR_ov_object 	pobj,
	const OV_ELEMENT		*pelem
) {
	OV_INSTPTR_cshmi_TemplateDefinition pinst = Ov_StaticPtrCast(cshmi_TemplateDefinition, pobj);
	OV_INSTPTR_ov_class pClass = Ov_GetClassPtr(pobj);
	OV_STRING temp = NULL;
	OV_UINT classcommentlength = 0;
	OV_UINT length = 0;
	switch(pelem->elemtype) {
	case OV_ET_OBJECT:
		classcommentlength = ov_string_getlength(pClass->v_comment);
		length = classcommentlength + ov_string_getlength(pinst->v_comment)+3;
		temp = ov_memstack_alloc(length);
		//strcpy adds the terminator
		strcpy(temp, pClass->v_comment);
		if(pinst->v_comment != NULL){
			*(temp+classcommentlength) = ':';
			*(temp+classcommentlength+1) = ' ';
			strcpy((temp+classcommentlength+2), pinst->v_comment);
		}
		return temp;
	}
	return ov_object_getcomment(pobj, pelem);
}
