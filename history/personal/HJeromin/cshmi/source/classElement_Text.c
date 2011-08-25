/*
*	Copyright (C) 2011
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
*	$Revision: 1.47 $
*	$Date: 2011-08-09 11:12:26 $
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


#include "cshmi.h"
#include "libov/ov_macros.h"


OV_DLLFNCEXPORT OV_SINGLE cshmi_Text_x_get(
    OV_INSTPTR_cshmi_Text          pobj
) {
    return pobj->v_x;
}

OV_DLLFNCEXPORT OV_RESULT cshmi_Text_x_set(
    OV_INSTPTR_cshmi_Text          pobj,
    const OV_SINGLE  value
) {
    pobj->v_x = value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_SINGLE cshmi_Text_y_get(
    OV_INSTPTR_cshmi_Text          pobj
) {
    return pobj->v_y;
}

OV_DLLFNCEXPORT OV_RESULT cshmi_Text_y_set(
    OV_INSTPTR_cshmi_Text          pobj,
    const OV_SINGLE  value
) {
    pobj->v_y = value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_STRING cshmi_Text_content_get(
    OV_INSTPTR_cshmi_Text          pobj
) {
    return pobj->v_content;
}

OV_DLLFNCEXPORT OV_RESULT cshmi_Text_content_set(
    OV_INSTPTR_cshmi_Text          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_content,value);
}

OV_DLLFNCEXPORT OV_STRING cshmi_Text_fontSize_get(
    OV_INSTPTR_cshmi_Text          pobj
) {
    return pobj->v_fontSize;
}

OV_DLLFNCEXPORT OV_RESULT cshmi_Text_fontSize_set(
    OV_INSTPTR_cshmi_Text          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_fontSize,value);
}

OV_DLLFNCEXPORT OV_STRING cshmi_Text_fontStyle_get(
    OV_INSTPTR_cshmi_Text          pobj
) {
    return pobj->v_fontStyle;
}

OV_DLLFNCEXPORT OV_RESULT cshmi_Text_fontStyle_set(
    OV_INSTPTR_cshmi_Text          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_fontStyle,value);
}

OV_DLLFNCEXPORT OV_STRING cshmi_Text_fontWeight_get(
    OV_INSTPTR_cshmi_Text          pobj
) {
    return pobj->v_fontWeight;
}

OV_DLLFNCEXPORT OV_RESULT cshmi_Text_fontWeight_set(
    OV_INSTPTR_cshmi_Text          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_fontWeight,value);
}

OV_DLLFNCEXPORT OV_STRING cshmi_Text_fontFamily_get(
    OV_INSTPTR_cshmi_Text          pobj
) {
    return pobj->v_fontFamily;
}

OV_DLLFNCEXPORT OV_RESULT cshmi_Text_fontFamily_set(
    OV_INSTPTR_cshmi_Text          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_fontFamily,value);
}

OV_DLLFNCEXPORT OV_STRING cshmi_Text_horAlignment_get(
    OV_INSTPTR_cshmi_Text          pobj
) {
    return pobj->v_horAlignment;
}

OV_DLLFNCEXPORT OV_RESULT cshmi_Text_horAlignment_set(
    OV_INSTPTR_cshmi_Text          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_horAlignment,value);
}

OV_DLLFNCEXPORT OV_STRING cshmi_Text_verAlignment_get(
    OV_INSTPTR_cshmi_Text          pobj
) {
    return pobj->v_verAlignment;
}

OV_DLLFNCEXPORT OV_RESULT cshmi_Text_verAlignment_set(
    OV_INSTPTR_cshmi_Text          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_verAlignment,value);
}

