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
*	classEvent_OperatorEvent.c
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


static OV_RESULT cshmi_OperatorEvent_typeSetAndCheck(
		OV_INSTPTR_cshmi_OperatorEvent          pobj,
		const OV_STRING  value,
		const OV_BOOL calledFromConstructor
	) {
	OV_STRING newvalue = NULL;
	OV_RESULT result = OV_ERR_OK;
	if(!value){
		return OV_ERR_BADPARAM;
	}
	ov_memstack_lock();
	ov_string_setvalue(&newvalue, ov_string_tolower(value));
	ov_memstack_unlock();
	if (	ov_string_compare(newvalue, "click") == OV_STRCMP_EQUAL
			||	ov_string_compare(newvalue, "doubleclick") == OV_STRCMP_EQUAL
			||	ov_string_compare(newvalue, "rightclick") == OV_STRCMP_EQUAL
			||	ov_string_compare(newvalue, "mouseover") == OV_STRCMP_EQUAL
			||	ov_string_compare(newvalue, "mouseout") == OV_STRCMP_EQUAL
			){
		//newvalue is ok
	}else{
		//ov_string_match("*drag*") does not match "drag"... 8-/
		ov_string_print(&newvalue, "X%sX", newvalue);
		if(ov_string_match(newvalue, "*move*") == TRUE ||
				ov_string_match(newvalue, "*drag*") == TRUE ){
			ov_string_setvalue(&newvalue, "aftermove");

		}else if(ov_string_match(newvalue, "*over*") == TRUE ){
			ov_string_setvalue(&newvalue, "mouseover");
		}else if(ov_string_match(newvalue, "*enter*") == TRUE ){
			ov_string_setvalue(&newvalue, "mouseover");

		}else if(ov_string_match(newvalue, "*out*") == TRUE ){
			ov_string_setvalue(&newvalue, "mouseout");
		}else if(ov_string_match(newvalue, "*leave*") == TRUE ){
			ov_string_setvalue(&newvalue, "mouseout");

		}else if(ov_string_compare(value, "") == OV_STRCMP_EQUAL || calledFromConstructor == TRUE){
			//allow resetting and initial wrong EventType
			ov_string_setvalue(&newvalue, "unconfigured");
		}else{
			ov_string_setvalue(&newvalue, NULL);
			return OV_ERR_BADPARAM;
		}
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

OV_DLLFNCEXPORT OV_RESULT cshmi_OperatorEvent_type_set(
	OV_INSTPTR_cshmi_OperatorEvent          pobj,
	const OV_STRING  value
) {
	cshmi_Object_resetCache(Ov_PtrUpCast(cshmi_Object, pobj));
	return cshmi_OperatorEvent_typeSetAndCheck(pobj, value, FALSE);
}

OV_DLLFNCEXPORT OV_RESULT cshmi_OperatorEvent_constructor(
	OV_INSTPTR_ov_object 	pobj
) {
	/*    
	*   local variables
	*/
	OV_INSTPTR_cshmi_OperatorEvent pinst = Ov_StaticPtrCast(cshmi_OperatorEvent, pobj);
	OV_RESULT    result;
	
	/* do what the base class does first */
	result = cshmi_Event_constructor(pobj);
	if(Ov_Fail(result))
		return result;
	
	//try setting the type with the name, could result in an rename
	return cshmi_OperatorEvent_typeSetAndCheck(pinst, pinst->v_identifier, TRUE);
}

OV_DLLFNCEXPORT OV_STRING cshmi_OperatorEvent_type_get(
	OV_INSTPTR_cshmi_OperatorEvent          pobj
) {
	return pobj->v_identifier;
}
