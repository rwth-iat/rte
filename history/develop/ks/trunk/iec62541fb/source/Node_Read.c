/*
*	Copyright (C) 2015
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
***********************************************************************/

#ifndef OV_COMPILE_LIBRARY_iec62541fb
#define OV_COMPILE_LIBRARY_iec62541fb
#endif


#include "iec62541fb.h"
#include "libov/ov_macros.h"
#include "fb_namedef.h"


OV_DLLFNCEXPORT OV_RESULT iec62541fb_Read_Execute_set(
		OV_INSTPTR_iec62541fb_Read          pinst,
		const OV_BOOL  setvalue
) {
	OV_INSTPTR_iec62541fb_Connect pConnect = NULL;
	OV_INSTPTR_iec62541fb_NodeGetHandle pNodeGetHandle = NULL;
	UA_ReadResponse ReadResponse;

	pConnect = Ov_DynamicPtrCast(iec62541fb_Connect, fb_connection_getFirstConnectedObject(Ov_PtrUpCast(fb_object, pinst), FALSE, "ConnectionHdl"));
	pNodeGetHandle = Ov_DynamicPtrCast(iec62541fb_NodeGetHandle, fb_connection_getFirstConnectedObject(Ov_PtrUpCast(fb_object, pinst), FALSE, "NodeHdl"));

	//free memory of old ANY var
	if(pinst->v_Variable.value.vartype == OV_VT_STRING){
		ov_string_setvalue(&pinst->v_Variable.value.valueunion.val_string, NULL);
		pinst->v_Variable.value.vartype = OV_VT_VOID;
	}else if(pinst->v_Variable.value.vartype & OV_VT_ISVECTOR){
		Ov_SetAnyValue(&pinst->v_Variable, NULL);
	}

	ReadResponse = UA_Client_read(pConnect->v_Client, &pNodeGetHandle->v_ReadRequest);
	if(ReadResponse.responseHeader.serviceResult == UA_STATUSCODE_GOOD &&
			ReadResponse.resultsSize > 0 && ReadResponse.results[0].hasValue){
		if(UA_Variant_isScalar(&ReadResponse.results[0].value) &&
			ReadResponse.results[0].value.type == &UA_TYPES[UA_TYPES_INT32])
		{
			pinst->v_Variable.value.vartype = OV_VT_INT;
			pinst->v_Variable.value.valueunion.val_int = (OV_INT)*(UA_Int32*)ReadResponse.results[0].value.data;
		}else if(UA_Variant_isScalar(&ReadResponse.results[0].value) &&
				ReadResponse.results[0].value.type == &UA_TYPES[UA_TYPES_UINT32])
		{
			pinst->v_Variable.value.vartype = OV_VT_UINT;
			pinst->v_Variable.value.valueunion.val_uint = (OV_UINT)*(UA_UInt32*)ReadResponse.results[0].value.data;
		}else if(UA_Variant_isScalar(&ReadResponse.results[0].value) &&
				ReadResponse.results[0].value.type == &UA_TYPES[UA_TYPES_FLOAT])
		{
			pinst->v_Variable.value.vartype = OV_VT_SINGLE;
			pinst->v_Variable.value.valueunion.val_single = (OV_SINGLE)*(UA_Float*)ReadResponse.results[0].value.data;
		}else if(UA_Variant_isScalar(&ReadResponse.results[0].value) &&
				ReadResponse.results[0].value.type == &UA_TYPES[UA_TYPES_DOUBLE])
		{
			pinst->v_Variable.value.vartype = OV_VT_DOUBLE;
			pinst->v_Variable.value.valueunion.val_double = (OV_DOUBLE)*(UA_Double*)ReadResponse.results[0].value.data;
		}else if(UA_Variant_isScalar(&ReadResponse.results[0].value) &&
				ReadResponse.results[0].value.type == &UA_TYPES[UA_TYPES_STRING])
		{
			pinst->v_Variable.value.vartype = OV_VT_STRING;
			pinst->v_Variable.value.valueunion.val_string = Ov_DbMalloc(((UA_String*)ReadResponse.results[0].value.data)->length);
			memcpy(((UA_String*)ReadResponse.results[0].value.data)->data, pinst->v_Variable.value.valueunion.val_string, ((UA_String*)ReadResponse.results[0].value.data)->length);
		}else{
			pinst->v_Done = FALSE;
			pinst->v_Error = TRUE;
			pinst->v_ErrorID = 1; //fixme
		}
	}else{
		pinst->v_Done = FALSE;
		pinst->v_Error = TRUE;
		pinst->v_ErrorID = 1; //fixme
	}
	pinst->v_Error = FALSE;
	pinst->v_ErrorID = 0;

	UA_ReadResponse_deleteMembers(&ReadResponse);
	return OV_ERR_OK;
}


OV_DLLFNCEXPORT OV_RESULT iec62541fb_Read_ConnectionHdl_set(
		OV_INSTPTR_iec62541fb_Read          pinst,
		const OV_UINT  value
) {
	pinst->v_Done = FALSE;
	pinst->v_ConnectionHdl = value;
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT iec62541fb_Read_NodeHdl_set(
		OV_INSTPTR_iec62541fb_Read          pinst,
		const OV_UINT  value
) {
	pinst->v_Done = FALSE;
	pinst->v_NodeHdl = value;
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT void iec62541fb_Read_startup(
		OV_INSTPTR_ov_object 	pobj
) {
	/*
	 *   local variables
	 */
	//OV_INSTPTR_iec62541fb_Read pinst = Ov_StaticPtrCast(iec62541fb_Read, pobj);

	/* do what the base class does first */
	fb_functionblock_startup(pobj);

	/*
	pinst->v_NodeAddInfo.IndexRangeCount = 0;
	pinst->v_NodeAddInfo.AttributeId = 0;
	*/

	return;
}
