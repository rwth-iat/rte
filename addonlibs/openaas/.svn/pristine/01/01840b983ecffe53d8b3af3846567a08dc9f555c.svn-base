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
