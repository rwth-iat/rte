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
