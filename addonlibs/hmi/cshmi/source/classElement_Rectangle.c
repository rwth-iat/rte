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
*	Rectangle.c
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



OV_DLLFNCEXPORT OV_RESULT cshmi_Rectangle_x_set(
		OV_INSTPTR_cshmi_Rectangle          pobj,
		const OV_SINGLE  value
) {
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
	pobj->v_x = value;
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT cshmi_Rectangle_y_set(
		OV_INSTPTR_cshmi_Rectangle          pobj,
		const OV_SINGLE  value
) {
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
	pobj->v_y = value;
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT cshmi_Rectangle_width_set(
		OV_INSTPTR_cshmi_Rectangle          pobj,
		const OV_SINGLE  value
) {
	if (value < 0){
		return OV_ERR_BADPARAM;
	}else{
		cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
		pobj->v_width = value;
	}
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT cshmi_Rectangle_height_set(
		OV_INSTPTR_cshmi_Rectangle          pobj,
		const OV_SINGLE  value
) {
	if (value < 0){
		return OV_ERR_BADPARAM;
	}else{
		cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
		pobj->v_height = value;
	}
	return OV_ERR_OK;
}
