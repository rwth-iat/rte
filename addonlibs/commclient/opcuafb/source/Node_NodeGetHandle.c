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
#include "fb_namedef.h"


OV_DLLFNCEXPORT OV_RESULT opcuafb_NodeGetHandle_Execute_set(
		OV_INSTPTR_opcuafb_NodeGetHandle          pinst,
		const OV_BOOL  value
) {
	if(value == FALSE || pinst->v_Execute == TRUE){
		//only react on the rising edge
		pinst->v_Execute = value;
		return OV_ERR_OK;
	}

	//todo register the node at the server

	pinst->v_Busy = FALSE;
	pinst->v_Error = FALSE;
	pinst->v_ErrorID = 0;

	pinst->v_NodeHdl = pinst->v_idL;
	pinst->v_Execute = value;
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT opcuafb_NodeGetHandle_ConnectionHdl_set(
		OV_INSTPTR_opcuafb_NodeGetHandle          pinst,
		const OV_UINT  value
) {
	if(value == 0){
		pinst->v_Done = TRUE;
	}else{
		if(pinst->v_ConnectionHdl == 0 && fb_connection_getFirstConnectedObject(Ov_PtrUpCast(fb_object, pinst), FALSE, TRUE, "Execute") == NULL){
			//we have a new connection and no connection on execute, so prepare for a new activation
			opcuafb_NodeGetHandle_Execute_set(pinst, FALSE);
			pinst->v_NodeHdl = 0;
		}
		pinst->v_Done = FALSE;
	}
	pinst->v_ConnectionHdl = value;
	pinst->v_Busy = FALSE;
	pinst->v_Error = FALSE;
	pinst->v_ErrorID = 0;
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT void opcuafb_NodeGetHandle_startup(
		OV_INSTPTR_ov_object 	pobj
) {
	/*
	 *   local variables
	 */
	OV_INSTPTR_opcuafb_NodeGetHandle pinst = Ov_StaticPtrCast(opcuafb_NodeGetHandle, pobj);

	/* do what the base class does first */
	fb_functionblock_startup(pobj);

	//fill the NodeID struct with the correct data.
	//the database pointers (for the string) are corrected on a database load, so we have to correct them in the struct
	pinst->v_NodeID.Identifier = pinst->v_NodeIDIdentifier;		//the pointer should be the same, no copy of the string
	pinst->v_NodeID.NamespaceIndex = pinst->v_NodeIDNSindex;	//this is probably the same even after a db reload, but overwrite it to be sure
	pinst->v_NodeID.IdentifierType = pinst->v_NodeIDIdType;		//this is probably the same even after a db reload, but overwrite it to be sure

	return;
}

OV_DLLFNCEXPORT void opcuafb_NodeGetHandle_shutdown(
		OV_INSTPTR_ov_object 	pobj
) {
	/*
	 *   local variables
	 */
	//OV_INSTPTR_opcuafb_NodeGetHandle pinst = Ov_StaticPtrCast(opcuafb_NodeGetHandle, pobj);

	/* set the object's state to "shut down" */
	fb_functionblock_shutdown(pobj);

	return;
}


OV_DLLFNCEXPORT OV_RESULT opcuafb_NodeGetHandle_NodeIDIdentifier_set(
		OV_INSTPTR_opcuafb_NodeGetHandle          pinst,
		const OV_STRING  value
) {
	OV_RESULT fr = OV_ERR_OK;
	pinst->v_Done = FALSE;
	fr = ov_string_setvalue(&pinst->v_NodeIDIdentifier, value);
	pinst->v_NodeID.Identifier = pinst->v_NodeIDIdentifier;	//the pointer should be the same, no copy of the string
	return fr;
}

OV_DLLFNCEXPORT OV_RESULT opcuafb_NodeGetHandle_NodeIDNSindex_set(
		OV_INSTPTR_opcuafb_NodeGetHandle          pinst,
		const OV_UINT  value
) {
	pinst->v_Done = FALSE;
	pinst->v_NodeIDNSindex = value;
	pinst->v_NodeID.NamespaceIndex = pinst->v_NodeIDNSindex;
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT opcuafb_NodeGetHandle_NodeIDIdType_set(
		OV_INSTPTR_opcuafb_NodeGetHandle          pinst,
		const OV_UINT  value
) {
	switch (value) {
		case UA_IDTYPE_STRING:
		case UA_IDTYPE_NUMERIC:
		case UA_IDTYPE_GUID:
		case UA_IDTYPE_OPAQUE:
			pinst->v_Done = FALSE;
			pinst->v_NodeIDIdType = value;
			pinst->v_NodeID.IdentifierType = pinst->v_NodeIDIdType;
			return OV_ERR_OK;
		default:
			return OV_ERR_NOTIMPLEMENTED;
	}
}
