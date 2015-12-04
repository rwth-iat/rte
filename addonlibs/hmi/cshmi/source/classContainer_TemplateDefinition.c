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
