/*
*   Copyright (C) 2015
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
***********************************************************************/

#ifndef OV_COMPILE_LIBRARY_opcuafb
#define OV_COMPILE_LIBRARY_opcuafb
#endif


#include "opcuafb.h"
#include "libov/ov_macros.h"

OV_DLLFNCEXPORT void opcuafb_object_startup(
		OV_INSTPTR_ov_object 	pobj
) {
	/*
	 *   local variables
	 */
	OV_INSTPTR_opcuafb_object pinst = Ov_StaticPtrCast(opcuafb_object, pobj);

	/* do what the base class does first */
	fb_functionblock_startup(pobj);

	pinst->v_Done = FALSE;
	pinst->v_Busy = FALSE;
	pinst->v_Error = FALSE;
	pinst->v_ErrorID = 0;

	return;
}

OV_DLLFNCEXPORT OV_ACCESS opcuafb_object_getaccess(
	OV_INSTPTR_ov_object	pobj,
	const OV_ELEMENT		*pelem,
	const OV_TICKET			*pticket
) {
	//we have nothing to hide, so all our normal variables can be read and written

	switch (pelem->elemtype){
		case OV_ET_VARIABLE :
			//prevent "special variables" from being read/write
			if (pelem->elemunion.pvar->v_offset >= offsetof(OV_INST_ov_object, __classinfo)){
				if (pelem->elemunion.pvar->v_vartype == OV_VT_CTYPE || pelem->elemunion.pvar->v_vartype == OV_VT_POINTER){
					return OV_AC_NONE;
				}else{
					return OV_AC_READWRITE;
				}
			}
			break;
		default:
			break;
	};
	return ov_object_getaccess(pobj, pelem, pticket);
}


OV_DLLFNCEXPORT void opcuafb_object_typemethod(
		OV_INSTPTR_fb_functionblock	pfb,
		OV_TIME						*pltc
) {
	//we need no cyclic operation
	return;
}
