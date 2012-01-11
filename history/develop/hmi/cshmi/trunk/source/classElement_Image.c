/*
*	Copyright (C) 2012
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
*	classElement_Image.c
*
*	Editors:
*	--------
*	Je							Holger Jeromin <jeromin@plt.rwth-aachen.de>
*
*	SVN:
*	----
*	$Revision: $
*	$Date: $
*
*	History:
*	--------
*	2012-01-09				Jer
*		-	File created
*
***********************************************************************/

#ifndef OV_COMPILE_LIBRARY_cshmi
#define OV_COMPILE_LIBRARY_cshmi
#endif


#include "cshmilib.h"

OV_DLLFNCEXPORT OV_SINGLE cshmi_Image_x_get(
	OV_INSTPTR_cshmi_Image          pobj
) {
	return pobj->v_x;
}

OV_DLLFNCEXPORT OV_RESULT cshmi_Image_x_set(
	OV_INSTPTR_cshmi_Image          pobj,
	const OV_SINGLE  value
) {
	pobj->v_x = value;
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_SINGLE cshmi_Image_y_get(
	OV_INSTPTR_cshmi_Image          pobj
) {
	return pobj->v_y;
}

OV_DLLFNCEXPORT OV_RESULT cshmi_Image_y_set(
	OV_INSTPTR_cshmi_Image          pobj,
	const OV_SINGLE  value
) {
	pobj->v_y = value;
	return OV_ERR_OK;
}
OV_DLLFNCEXPORT OV_SINGLE cshmi_Image_width_get(
    OV_INSTPTR_cshmi_Image          pobj
) {
    return pobj->v_width;
}

OV_DLLFNCEXPORT OV_RESULT cshmi_Image_width_set(
	OV_INSTPTR_cshmi_Image          pobj,
	const OV_SINGLE  value
) {
	pobj->v_width = value;
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_SINGLE cshmi_Image_height_get(
	OV_INSTPTR_cshmi_Image          pobj
) {
	return pobj->v_height;
}

OV_DLLFNCEXPORT OV_RESULT cshmi_Image_height_set(
	OV_INSTPTR_cshmi_Image          pobj,
	const OV_SINGLE  value
) {
	pobj->v_height = value;
	return OV_ERR_OK;
}


OV_DLLFNCEXPORT OV_STRING cshmi_Image_SVGcontent_get(
	OV_INSTPTR_cshmi_Image          pobj
) {
	return pobj->v_SVGcontent;
}

OV_DLLFNCEXPORT OV_RESULT cshmi_Image_SVGcontent_set(
	OV_INSTPTR_cshmi_Image          pobj,
	const OV_STRING  value
) {
	return ov_string_setvalue(&pobj->v_SVGcontent,value);
}

OV_DLLFNCEXPORT OV_STRING cshmi_Image_Bitmapcontent_get(
	OV_INSTPTR_cshmi_Image          pobj
) {
	return pobj->v_Bitmapcontent;
}

OV_DLLFNCEXPORT OV_RESULT cshmi_Image_Bitmapcontent_set(
	OV_INSTPTR_cshmi_Image          pobj,
	const OV_STRING  value
) {
	return ov_string_setvalue(&pobj->v_Bitmapcontent,value);
}

OV_DLLFNCEXPORT OV_RESULT cshmi_Image_constructor(
	OV_INSTPTR_ov_object 	pobj
) {
	//	local variables
	//
	OV_INSTPTR_cshmi_Image pImg = Ov_StaticPtrCast(cshmi_Image, pobj);
	OV_RESULT    result;

	/* do what the base class does first */
	result = cshmi_Element_constructor(pobj);
	if(Ov_Fail(result))
		return result;

	//should not have a default stroke/fill
	ov_string_setvalue(&pImg->v_stroke, NULL);
	ov_string_setvalue(&pImg->v_fill, NULL);

	return OV_ERR_OK;
}

