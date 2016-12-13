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
*	classEvent_ClientEvent.c
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
*	15-September-2011			Je		V0.1.0
*		-	File created
*
***********************************************************************/

#ifndef OV_COMPILE_LIBRARY_cshmi
#define OV_COMPILE_LIBRARY_cshmi
#endif

#include "cshmilib.h"


static OV_RESULT cshmi_ClientEvent_typeSetAndCheck(
	OV_INSTPTR_cshmi_ClientEvent          pobj,
	const OV_STRING  value,
	const OV_BOOL calledFromConstructor
) {
	OV_STRING newvalue = NULL;
	OV_RESULT result = OV_ERR_OK;
	if(!value){
		return OV_ERR_BADPARAM;
	}
	//ov_string_match("*onload*") does not match "onload"... 8-/
	ov_memstack_lock();
	ov_string_print(&newvalue, "X%sX", ov_string_tolower(value));
	ov_memstack_unlock();
	if(ov_string_match(newvalue, "*load*") == TRUE){
		ov_string_setvalue(&newvalue, "onload");
	}else if(ov_string_match(newvalue, "*globalvar*") == TRUE ){
		ov_string_setvalue(&newvalue, "globalvarchanged");
	}else if(ov_string_compare(value, "") == OV_STRCMP_EQUAL || calledFromConstructor == TRUE){
		//allow resetting and initial wrong EventType
		ov_string_setvalue(&newvalue, "unconfigured");
	}else{
		ov_string_setvalue(&newvalue, NULL);
		return OV_ERR_BADPARAM;
	}
	if(ov_string_compare(newvalue, pobj->v_identifier) != OV_STRCMP_EQUAL){
		result = ov_class_renameobject(Ov_PtrUpCast(ov_object, pobj), Ov_GetParent(ov_containment, pobj), newvalue, OV_PMH_DEFAULT, NULL);
	}
	if(result == OV_ERR_CANTMOVE){
		result = OV_ERR_ALREADYEXISTS;
	}
	ov_string_setvalue(&newvalue, NULL);
	return result;
}

OV_DLLFNCEXPORT OV_RESULT cshmi_ClientEvent_type_set(
	OV_INSTPTR_cshmi_ClientEvent          pobj,
	const OV_STRING  value
) {
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
	return cshmi_ClientEvent_typeSetAndCheck(pobj, value, FALSE);
}


OV_DLLFNCEXPORT OV_RESULT cshmi_ClientEvent_constructor(
	OV_INSTPTR_ov_object 	pobj
) {
	/*    
	*   local variables
	*/
	OV_INSTPTR_cshmi_ClientEvent pinst = Ov_StaticPtrCast(cshmi_ClientEvent, pobj);
	OV_RESULT    result;
	
	/* do what the base class does first */
	result = cshmi_Event_constructor(pobj);
	if(Ov_Fail(result))
		return result;
	
	//try setting the type with the name, could result in an rename
	return cshmi_ClientEvent_typeSetAndCheck(pinst, pinst->v_identifier, TRUE);

	//a failure is OK
//	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_STRING cshmi_ClientEvent_type_get(
    OV_INSTPTR_cshmi_ClientEvent          pobj
) {
	return pobj->v_identifier;
}
