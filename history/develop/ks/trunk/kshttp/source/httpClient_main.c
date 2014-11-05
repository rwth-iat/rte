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
***********************************************************************/

#ifndef OV_COMPILE_LIBRARY_kshttp
#define OV_COMPILE_LIBRARY_kshttp
#endif


#include "config.h"

OV_DLLFNCEXPORT void kshttp_httpClientBase_typemethod (
	OV_INSTPTR_ksbase_ComTask	this
) {
	/*
	*   local variables
	*/
	OV_INSTPTR_kshttp_httpClientBase				thisCl = Ov_StaticPtrCast(kshttp_httpClientBase, this);
	OV_RESULT result = OV_ERR_OK;

	OV_INSTPTR_ksbase_Channel	pChannel = NULL;
	OV_VTBLPTR_ksbase_Channel	pVtblChannel = NULL;

	/*
	 * state machine
	 */
	switch(thisCl->v_state)
	{
	case KSBASE_CLST_BUSY:	/*	call HandleData again to process data	*/
		//i hope the channel will call us if we got fresh data fixme in xdr raus?
		break;
	case KSBASE_CLST_COMPLETED:	/*	issue the callback	*/
		thisCl->v_actimode = 0;
		KS_logfile_debug(("%s: completed. issuing callback", thisCl->v_identifier));
		if(thisCl->v_callback.callbackFunction)
			thisCl->v_callback.callbackFunction(thisCl->v_callback.instanceCalled, Ov_PtrUpCast(ov_domain, this));

		if(!thisCl->v_holdConnection)	/*	close connection if not otherwise specified	*/
		{
			result = kshttp_getChannelPointer(thisCl, &pChannel, &pVtblChannel);
			if(Ov_Fail(result))
			{
				KS_logfile_error(("%s: Could not get Channel pointers.", this->v_identifier));
				return;
			}

			pVtblChannel->m_CloseConnection(pChannel);
		}
		return;
	}

	return;
}

OV_DLLFNCEXPORT OV_RESULT kshttp_httpClientBase_HandleData(
	OV_INSTPTR_ksbase_DataHandler this,
	KS_DATAPACKET* dataReceived,
	UNUSED KS_DATAPACKET* answer
) {
	OV_INSTPTR_kshttp_httpClientBase				thisCl = Ov_StaticPtrCast(kshttp_httpClientBase, this);
	OV_INSTPTR_ksbase_Channel				pChannel = NULL;
	OV_VTBLPTR_ksbase_Channel	pVtblChannel = NULL;
	OV_RESULT								result;

	/*	process header and generic stuff	*/
	pChannel = Ov_GetParent(ksbase_AssocChannelDataHandler, thisCl);
	if(!pChannel)
	{
		KS_logfile_error(("%s: HandleData: no Channel object associated. cannot process data.", this->v_identifier));
		return OV_ERR_GENERIC;
	}

	KS_logfile_debug(("length of http answer: %lu\ndata: %p", dataReceived->length, dataReceived->data, dataReceived->readPT));
	result = kshttp_processServerReplyHeader(dataReceived, &thisCl->v_ServerResponse, &thisCl->v_httpParseStatus);

	if(thisCl->v_httpParseStatus == HTTP_MSG_HEADERACCEPTED || thisCl->v_httpParseStatus == HTTP_MSG_INCOMPLETE)
	{	/*	message incomplete --> keep waiting	*/
		thisCl->v_state = KSBASE_CLST_BUSY;
		thisCl->v_actimode = 1;
		return OV_ERR_OK;
	}else if (Ov_Fail(result)){
		//we had other failures...
		thisCl->v_state = KSBASE_CLST_ERROR;
		thisCl->v_httpParseStatus = HTTP_MSG_DENIED;
		ksbase_free_KSDATAPACKET(dataReceived);
		return result;
	}
	thisCl->v_httpParseStatus = HTTP_MSG_ACCEPTED;

	thisCl->v_httpStatusCode = thisCl->v_ServerResponse.statusCode;

	KS_logfile_debug(("decoding successful"));

	thisCl->v_state = KSBASE_CLST_COMPLETED;
	result = kshttp_getChannelPointer(thisCl, &pChannel, &pVtblChannel);
	if(Ov_Fail(result))
	{
		KS_logfile_error(("%s: Could not get Channel pointers.", this->v_identifier));
		return result;
	}
	pVtblChannel->m_CloseConnection(pChannel);

	//make sure the typemethod calls the callback and do other stuff
	thisCl->v_actimode = 1;

	return OV_ERR_OK;
}

OV_DLLFNCEXPORT void kshttp_httpClientBase_startup(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
    OV_INSTPTR_kshttp_httpClientBase thisCl = Ov_StaticPtrCast(kshttp_httpClientBase, pobj);

    /* do what the base class does first */
    ov_object_startup(pobj);

	thisCl->v_ServerResponse.contentType = NULL;
	thisCl->v_ServerResponse.contentLength = 0;
	thisCl->v_ServerResponse.contentBinary = NULL;
	thisCl->v_ServerResponse.version = NULL;
	return;
}

/**
 * resets all variables from the httpClientBase class and base class
 */
OV_DLLFNCEXPORT OV_RESULT kshttp_httpClientBase_reset(
	OV_INSTPTR_ksbase_ClientBase this
) {
	OV_RESULT fr = OV_ERR_OK;
	OV_INSTPTR_kshttp_httpClientBase	thisCl = Ov_StaticPtrCast(kshttp_httpClientBase, this);

	fr = ksbase_ClientBase_reset(this);
	if(Ov_Fail(fr)){
		return fr;
	}
	ov_string_setvalue(&thisCl->v_serverName, "<unused>");

	thisCl->v_httpParseStatus = HTTP_MSG_NEW;
	thisCl->v_httpStatusCode = 0;
	ov_string_setvalue(&(thisCl->v_ServerResponse.contentType), NULL);
	ov_string_setvalue(&(thisCl->v_ServerResponse.version), NULL);
	Ov_HeapFree(thisCl->v_ServerResponse.contentBinary);
	thisCl->v_ServerResponse.contentBinary = NULL;

	return OV_ERR_OK;
}


OV_DLLFNCEXPORT void kshttp_httpClientBase_shutdown(
	OV_INSTPTR_ov_object 	pobj
) {
	/*
	*   local variables
	*/
	OV_INSTPTR_kshttp_httpClientBase thisCl = Ov_StaticPtrCast(kshttp_httpClientBase, pobj);

	thisCl->v_callback.instanceCalled = NULL;
	thisCl->v_callback.callbackFunction = NULL;

	ov_string_setvalue(&(thisCl->v_ServerResponse.version), NULL);
	ov_string_setvalue(&(thisCl->v_ServerResponse.contentType), NULL);
	Ov_HeapFree(thisCl->v_ServerResponse.contentBinary);
	thisCl->v_ServerResponse.contentBinary = NULL;

    /* set the object's state to "shut down" */
    ov_object_shutdown(pobj);

    return;
}

