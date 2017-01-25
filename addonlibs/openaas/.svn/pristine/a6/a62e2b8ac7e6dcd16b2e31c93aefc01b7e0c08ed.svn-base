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

#ifndef OV_COMPILE_LIBRARY_opcuafb
#define OV_COMPILE_LIBRARY_opcuafb
#endif


#include "opcuafb.h"
#include "libov/ov_macros.h"
#include "fb_namedef.h"
#include "errno.h"


OV_DLLFNCEXPORT OV_RESULT opcuafb_Read_Execute_set(
		OV_INSTPTR_opcuafb_Read          pinst,
		const OV_BOOL  value
) {
	OV_INSTPTR_opcuafb_Connect pConnect = NULL;
	OV_INSTPTR_opcuafb_NodeGetHandle pNodeGetHandle = NULL;
	UA_ReadRequest ReadRequest;
	UA_ReadResponse ReadResponse;
	unsigned long int tempulong = 0;
	char *endPtr = NULL;

	if(value == FALSE || pinst->v_Execute == TRUE){
		//only react on the rising edge
		pinst->v_Execute = value;
		return OV_ERR_OK;
	}

	pConnect = Ov_DynamicPtrCast(opcuafb_Connect, fb_connection_getFirstConnectedObject(Ov_PtrUpCast(fb_object, pinst), FALSE, TRUE, "ConnectionHdl"));
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

	pNodeGetHandle = Ov_DynamicPtrCast(opcuafb_NodeGetHandle, fb_connection_getFirstConnectedObject(Ov_PtrUpCast(fb_object, pinst), FALSE, TRUE, "NodeHdl"));
	if(pNodeGetHandle == NULL){
		pinst->v_Error = TRUE;
		pinst->v_ErrorID = 1; //todo
		return OV_ERR_BADVALUE;
	}
	if(pNodeGetHandle->v_NodeHdl == 0){
		pinst->v_Error = TRUE;
		pinst->v_ErrorID = 1; //todo
		return OV_ERR_BADVALUE;
	}

	//free memory of old ANY variable
	if(pinst->v_Variable.value.vartype == OV_VT_STRING){
		ov_string_setvalue(&pinst->v_Variable.value.valueunion.val_string, NULL);
		pinst->v_Variable.value.vartype = OV_VT_VOID;
	}else if(pinst->v_Variable.value.vartype & OV_VT_ISVECTOR){
		Ov_SetAnyValue(&pinst->v_Variable, NULL);
	}

	UA_ReadRequest_init(&ReadRequest);
	ReadRequest.nodesToRead = UA_ReadValueId_new();
	ReadRequest.nodesToReadSize = 1;
	ReadRequest.nodesToRead[0].attributeId = UA_ATTRIBUTEID_VALUE;
	if(pNodeGetHandle->v_NodeID.IdentifierType == UA_IDTYPE_STRING){
		ReadRequest.nodesToRead[0].nodeId = UA_NODEID_STRING_ALLOC(pNodeGetHandle->v_NodeID.NamespaceIndex, pNodeGetHandle->v_NodeID.Identifier);
	}else if(pNodeGetHandle->v_NodeID.IdentifierType == UA_IDTYPE_NUMERIC){
		tempulong = strtoul(pNodeGetHandle->v_NodeID.Identifier, &endPtr, 10);
		if (ERANGE != errno &&
			tempulong < UINT16_MAX &&
			endPtr != pNodeGetHandle->v_NodeID.Identifier)
		{
			ReadRequest.nodesToRead[0].nodeId = UA_NODEID_NUMERIC(pNodeGetHandle->v_NodeID.NamespaceIndex, (UA_UInt16)tempulong);
		}
//	}else if(pNodeGetHandle->v_NodeID.IdentifierType == UA_IDTYPE_GUID){
		//todo
		//ReadRequest.nodesToRead[0].nodeId = UA_NODEID_GUID(pNodeGetHandle->v_NodeID.NamespaceIndex, pNodeGetHandle->v_NodeID.Identifier);
	}else{
		UA_ReadRequest_deleteMembers(&ReadRequest);
		return OV_ERR_BADVALUE;
	}

	ReadResponse = UA_Client_Service_read(pConnect->v_Client, ReadRequest);
	if(ReadResponse.responseHeader.serviceResult == UA_STATUSCODE_GOOD &&
			ReadResponse.resultsSize > 0 && ReadResponse.results[0].hasValue){
		pinst->v_Error = FALSE;
		pinst->v_ErrorID = 0;
		pinst->v_Done = TRUE;
		if(UA_Variant_isScalar(&ReadResponse.results[0].value)){
			if(ReadResponse.results[0].hasSourceTimestamp){
				pinst->v_TimeStamp = ov_1601nsTimeToOvTime(ReadResponse.results[0].sourceTimestamp);
//todo		}else if(ReadResponse.results[0].hasSourcePicoseconds){
			}else if(ReadResponse.results[0].hasServerTimestamp){
				pinst->v_TimeStamp = ov_1601nsTimeToOvTime(ReadResponse.results[0].sourceTimestamp);
//todo		}else if(ReadResponse.results[0].hasServerPicoseconds){
			}else{
				ov_time_gettime(&pinst->v_TimeStamp);
			}

			if(ReadResponse.results[0].value.type == &UA_TYPES[UA_TYPES_INT32]){
				pinst->v_Variable.value.vartype = OV_VT_INT;
				pinst->v_Variable.value.valueunion.val_int = (OV_INT)*(UA_Int32*)ReadResponse.results[0].value.data;
			}else if(ReadResponse.results[0].value.type == &UA_TYPES[UA_TYPES_UINT32]){
				pinst->v_Variable.value.vartype = OV_VT_UINT;
				pinst->v_Variable.value.valueunion.val_uint = (OV_UINT)*(UA_UInt32*)ReadResponse.results[0].value.data;
			}else if(ReadResponse.results[0].value.type == &UA_TYPES[UA_TYPES_FLOAT]){
				pinst->v_Variable.value.vartype = OV_VT_SINGLE;
				pinst->v_Variable.value.valueunion.val_single = (OV_SINGLE)*(UA_Float*)ReadResponse.results[0].value.data;
			}else if(ReadResponse.results[0].value.type == &UA_TYPES[UA_TYPES_DOUBLE]){
				pinst->v_Variable.value.vartype = OV_VT_DOUBLE;
				pinst->v_Variable.value.valueunion.val_double = (OV_DOUBLE)*(UA_Double*)ReadResponse.results[0].value.data;
			}else if(ReadResponse.results[0].value.type == &UA_TYPES[UA_TYPES_STRING]){
				pinst->v_Variable.value.vartype = OV_VT_STRING;
				pinst->v_Variable.value.valueunion.val_string = Ov_DbMalloc(((UA_String*)ReadResponse.results[0].value.data)->length);
				if(pinst->v_Variable.value.valueunion.val_string != NULL){
					memcpy(((UA_String*)ReadResponse.results[0].value.data)->data, pinst->v_Variable.value.valueunion.val_string, ((UA_String*)ReadResponse.results[0].value.data)->length);
				}
			}else if(ReadResponse.results[0].value.type == &UA_TYPES[UA_TYPES_DATETIME]){
				pinst->v_Variable.value.vartype = OV_VT_TIME;
				pinst->v_Variable.value.valueunion.val_time = ov_1601nsTimeToOvTime((OV_INT64)*(UA_DateTime*)ReadResponse.results[0].value.data);
			}else{
				if(ReadResponse.results[0].value.type == &UA_TYPES[UA_TYPES_STRING]){
					//free memory
					Ov_SetAnyValue(&pinst->v_Variable, NULL);
					//set length and type
					Ov_SetDynamicVectorLength(&pinst->v_Variable.value.valueunion.val_string_vec, ReadResponse.results[0].value.arrayLength, STRING);
					for(size_t iterator = 0; iterator < ReadResponse.results[0].value.arrayLength;iterator++){
						pinst->v_Variable.value.valueunion.val_string = Ov_DbMalloc(((UA_String*)ReadResponse.results[0].value.data)[iterator].length);
						if(pinst->v_Variable.value.valueunion.val_string != NULL){
							memcpy(((UA_String*)ReadResponse.results[0].value.data)[iterator].data, pinst->v_Variable.value.valueunion.val_string_vec.value[iterator], ((UA_String*)ReadResponse.results[0].value.data)[iterator].length);
						}
					}
				}else{
					//not implemented
					pinst->v_Done = TRUE;
					pinst->v_Error = TRUE;
					pinst->v_ErrorID = 1; //todo
				}
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


OV_DLLFNCEXPORT OV_RESULT opcuafb_Read_ConnectionHdl_set(
		OV_INSTPTR_opcuafb_Read          pinst,
		const OV_UINT  value
) {
	if(value == 0){
		pinst->v_Done = TRUE;
	}else{
		if(pinst->v_ConnectionHdl == 0 && fb_connection_getFirstConnectedObject(Ov_PtrUpCast(fb_object, pinst), FALSE, TRUE, "Execute") == NULL){
			//we have a new connection and no connection on execute, so prepare for a new activation
			opcuafb_Read_Execute_set(pinst, FALSE);
		}
		pinst->v_Done = FALSE;
	}
	pinst->v_ConnectionHdl = value;
	pinst->v_Busy = FALSE;
	pinst->v_Error = FALSE;
	pinst->v_ErrorID = 0;
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT opcuafb_Read_NodeHdl_set(
		OV_INSTPTR_opcuafb_Read          pinst,
		const OV_UINT  value
) {
	if(value == 0){
		pinst->v_Done = TRUE;
	}else{
		if(pinst->v_NodeHdl == 0 && fb_connection_getFirstConnectedObject(Ov_PtrUpCast(fb_object, pinst), FALSE, TRUE, "Execute") == NULL){
			//we have a new nodeID and no connection on execute, so prepare for a new activation
			opcuafb_Read_Execute_set(pinst, FALSE);
		}
		pinst->v_Done = FALSE;
	}
	pinst->v_Busy = FALSE;
	pinst->v_Error = FALSE;
	pinst->v_ErrorID = 0;
	pinst->v_NodeHdl = value;
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT void opcuafb_Read_startup(
		OV_INSTPTR_ov_object 	pobj
) {
	/*
	 *   local variables
	 */
	//OV_INSTPTR_opcuafb_Read pinst = Ov_StaticPtrCast(opcuafb_Read, pobj);

	/* do what the base class does first */
	fb_functionblock_startup(pobj);

	/*
	pinst->v_NodeAddInfo.IndexRangeCount = 0;
	pinst->v_NodeAddInfo.AttributeId = 0;
	*/

	return;
}
