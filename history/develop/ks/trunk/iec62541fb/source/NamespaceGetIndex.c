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


OV_DLLFNCEXPORT OV_RESULT iec62541fb_NamespaceGetIndex_Execute_set(
		OV_INSTPTR_iec62541fb_NamespaceGetIndex          pinst,
		const OV_BOOL  value
) {
	OV_INSTPTR_iec62541fb_Connect pConnect = NULL;
	UA_ReadRequest ReadRequest;
	UA_ReadResponse ReadResponse;

	if(value == FALSE || pinst->v_Execute == TRUE){
		//only react on the rising edge
		pinst->v_Execute = value;
		return OV_ERR_OK;
	}

	pConnect = Ov_DynamicPtrCast(iec62541fb_Connect, fb_connection_getFirstConnectedObject(Ov_PtrUpCast(fb_object, pinst), FALSE, TRUE, "ConnectionHdl"));
	if(pConnect == NULL){
		pinst->v_Error = TRUE;
		pinst->v_ErrorID = 1; //todo
		return OV_ERR_BADVALUE;
	}
	if(pConnect->v_ConnectionHdl == 0){
		pinst->v_Error = TRUE;
		pinst->v_ErrorID = 1; //todo
		return OV_ERR_BADVALUE;
	}
	if(pConnect->v_Client == NULL){
		//internal error
		pinst->v_Error = TRUE;
		pinst->v_ErrorID = 1; //todo
		return OV_ERR_BADVALUE;
	}

	UA_ReadRequest_init(&ReadRequest);
	ReadRequest.nodesToRead = UA_ReadValueId_new();
	ReadRequest.nodesToReadSize = 1;
	ReadRequest.nodesToRead[0].attributeId = UA_ATTRIBUTEID_VALUE;
	ReadRequest.nodesToRead[0].nodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_NAMESPACEARRAY);

	ReadResponse = UA_Client_read(pConnect->v_Client, &ReadRequest);
	if(ReadResponse.responseHeader.serviceResult == UA_STATUSCODE_GOOD &&
			ReadResponse.resultsSize > 0 && ReadResponse.results[0].hasValue){
		pinst->v_Error = TRUE;
		pinst->v_ErrorID = 0;
		pinst->v_Done = TRUE;
		if(!UA_Variant_isScalar(&ReadResponse.results[0].value)){
			if(ReadResponse.results[0].value.type == &UA_TYPES[UA_TYPES_STRING]){
				UA_String tempstring = UA_String_fromChars(pinst->v_NamespaceUri);
				for(UA_Int32 iterator = 0; iterator < ReadResponse.results[0].value.arrayLength;iterator++){
					if(UA_String_equal(&tempstring, &((UA_String*)ReadResponse.results[0].value.data)[iterator] )){
						pinst->v_NamespaceIndex = (OV_UINT)iterator;
						pinst->v_Error = FALSE;
					}
				}
				UA_String_delete(&tempstring);
			}
		}else{
			//not implemented
			pinst->v_Done = TRUE;
			pinst->v_Error = TRUE;
			pinst->v_ErrorID = 1; //todo
		}
	}else{
		pinst->v_Done = FALSE;
		pinst->v_Error = TRUE;
		pinst->v_ErrorID = 1; //todo
	}

	UA_ReadResponse_deleteMembers(&ReadResponse);
	UA_ReadRequest_deleteMembers(&ReadRequest);


	pinst->v_Execute = value;
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT iec62541fb_NamespaceGetIndex_ConnectionHdl_set(
		OV_INSTPTR_iec62541fb_NamespaceGetIndex          pinst,
		const OV_UINT  value
) {
	if(value == 0){
		pinst->v_Done = TRUE;
	}else{
		if(pinst->v_ConnectionHdl == 0 && fb_connection_getFirstConnectedObject(Ov_PtrUpCast(fb_object, pinst), FALSE, TRUE, "Execute") == NULL){
			//we have a new connection and no connection on execute, so prepare for a new activation
			iec62541fb_NamespaceGetIndex_Execute_set(pinst, FALSE);
		}
		pinst->v_Done = FALSE;
	}
	pinst->v_ConnectionHdl = value;
	pinst->v_Busy = FALSE;
	pinst->v_Error = FALSE;
	pinst->v_ErrorID = 0;
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT iec62541fb_NamespaceGetIndex_NamespaceUri_set(
		OV_INSTPTR_iec62541fb_NamespaceGetIndex          pinst,
		const OV_STRING  value
) {
	pinst->v_Done = FALSE;
	return ov_string_setvalue(&pinst->v_NamespaceUri,value);
}

OV_DLLFNCEXPORT OV_RESULT iec62541fb_NamespaceGetIndex_constructor(
		OV_INSTPTR_ov_object 	pobj
) {
	/*
	 *   local variables
	 */
	OV_INSTPTR_iec62541fb_NamespaceGetIndex pinst = Ov_StaticPtrCast(iec62541fb_NamespaceGetIndex, pobj);
	OV_RESULT    result;

	/* do what the base class does first */
	result = fb_functionblock_constructor(pobj);
	if(Ov_Fail(result))
		return result;

	pinst->v_Done = FALSE;
	return ov_string_setvalue(&pinst->v_NamespaceUri, OV_UA_NAMESPACEURI);
}

