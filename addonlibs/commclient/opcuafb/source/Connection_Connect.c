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

OV_DLLFNCEXPORT void opcuafb_Connect_startup(
		OV_INSTPTR_ov_object 	pobj
) {
	/*
	 *   local variables
	 */
	OV_INSTPTR_opcuafb_Connect pinst = Ov_StaticPtrCast(opcuafb_Connect, pobj);

	/* do what the base class does first */
	fb_functionblock_startup(pobj);

	if(fb_connection_getFirstConnectedObject(Ov_PtrUpCast(fb_object, pinst), FALSE, TRUE, "Execute") == NULL){
		//we have no connection on execute, so prepare for a new activation
		opcuafb_Connect_Execute_set(pinst, FALSE);
	}
	pinst->v_Done = FALSE;

	pinst->v_ConnectionHdl = 0;
	pinst->v_Client = NULL;

	return;
}

OV_DLLFNCEXPORT void opcuafb_Connect_shutdown(
		OV_INSTPTR_ov_object 	pobj
) {
	/*
	 *   local variables
	 */
	OV_INSTPTR_opcuafb_Connect pinst = Ov_StaticPtrCast(opcuafb_Connect, pobj);

	if(pinst->v_Client != NULL){
		UA_Client_disconnect(pinst->v_Client);
		UA_Client_delete(pinst->v_Client);
	}
	pinst->v_Client = NULL;

	/* set the object's state to "shut down" */
	fb_functionblock_shutdown(pobj);

	return;
}


OV_DLLFNCEXPORT OV_RESULT opcuafb_Connect_Execute_set(
		OV_INSTPTR_opcuafb_Connect          pinst,
		const OV_BOOL  value
) {
	UA_StatusCode retval = UA_STATUSCODE_GOOD;

	if(value == FALSE || pinst->v_Execute == TRUE){
		//only react on the rising edge
		pinst->v_Execute = value;
		return OV_ERR_OK;
	}

	pinst->v_Client = UA_Client_new(UA_ClientConfig_standard);
	//todo use TCPchannel which is a part of us
	retval = UA_Client_connect(pinst->v_Client, pinst->v_ServerEndpointUrl);
	if(retval != UA_STATUSCODE_GOOD) {
		if(pinst->v_Client != NULL){
			UA_Client_delete(pinst->v_Client);
		}
		pinst->v_ConnectionHdl = 0;
		pinst->v_Client = NULL;
		pinst->v_Error = TRUE;
		pinst->v_ErrorID = 1;	//todo
		pinst->v_Done = FALSE;
		return OV_ERR_BADVALUE;
	}
	pinst->v_ConnectionHdl = pinst->v_idL;
	pinst->v_Done = TRUE;
	pinst->v_Error = FALSE;
	pinst->v_ErrorID = 0;

	//keep session renew active
	pinst->v_actimode = FB_AM_ON;

	pinst->v_Execute = value;
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT opcuafb_Connect_ServerEndpointUrl_set(
		OV_INSTPTR_opcuafb_Connect          pinst,
		const OV_STRING  value
) {
	pinst->v_Done = FALSE;
	if(ov_string_compare(pinst->v_ServerEndpointUrl, value) != OV_STRCMP_EQUAL){
		opcuafb_Connect_Execute_set(pinst, FALSE);
		pinst->v_ConnectionHdl = 0;
	}
	return ov_string_setvalue(&pinst->v_ServerEndpointUrl,value);
}

OV_DLLFNCEXPORT void opcuafb_Connect_typemethod(
		OV_INSTPTR_fb_functionblock	pfb,
		OV_TIME						*pltc
) {
	/*
	 *   local variables
	 */
	OV_INSTPTR_opcuafb_Connect pinst = Ov_StaticPtrCast(opcuafb_Connect, pfb);
	if(pinst->v_ConnectionHdl == 0){
		//no active connection
		return;
	}
	if(pinst->v_Client == NULL){
		//internal error
		return;
	}
	UA_Client_manuallyRenewSecureChannel(pinst->v_Client);

	return;
}


