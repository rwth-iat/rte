/*
*	Copyright (C) 2013
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


#include "kshttp.h"
#include "ksbase_helper.h"
#include "config.h"
#include "libov/ov_macros.h"

OV_DLLFNCEXPORT void kshttp_httpClient_typemethod (
	OV_INSTPTR_ksbase_ComTask	this
) {
	/*
	*   local variables
	*/
	OV_INSTPTR_kshttp_httpClient				thisCl = Ov_StaticPtrCast(kshttp_httpClient, this);
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
			thisCl->v_callback.callbackFunction(thisCl->v_callback.instanceCalled, Ov_StaticPtrCast(ov_domain, this));

		if(!thisCl->v_holdConnection)	/*	close connection if not otherwise specified	*/
		{
			result = getChannelPointer(thisCl, &pChannel, &pVtblChannel);
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

OV_DLLFNCEXPORT OV_RESULT kshttp_httpClient_HandleData(
	OV_INSTPTR_ksbase_DataHandler this,
	KS_DATAPACKET* dataReceived,
	KS_DATAPACKET* answer
) {
	OV_INSTPTR_kshttp_httpClient				thisCl = Ov_StaticPtrCast(kshttp_httpClient, this);
	OV_INSTPTR_ksbase_Channel				pChannel = NULL;
	OV_INSTPTR_ksbase_ClientTicketGenerator	pTicketGenerator = NULL;
	OV_RESULT								result;

	/*	process header and generic stuff	*/
	pChannel = Ov_GetParent(ksbase_AssocChannelDataHandler, thisCl);
	if(!pChannel)
	{
		KS_logfile_error(("%s: HandleData: no Channel object associated. cannot process data.", this->v_identifier));
		return OV_ERR_GENERIC;
	}

	pTicketGenerator = Ov_GetChild(ksbase_AssocClientToTicketGenerator, thisCl);

	KS_logfile_debug(("length of http answer: %lu\ndata: %p", dataReceived->length, dataReceived->data, dataReceived->readPT));
	result = kshttp_processServerReplyHeader(dataReceived, &thisCl->v_ServerResponse);
	if(result == OV_ERR_TARGETGENERIC)
	{	/*	message incomplete --> keep waiting	*/
		thisCl->v_state = KSBASE_CLST_BUSY;
		thisCl->v_msgAccepted = HTTP_MSG_INCOMPLETE;
		thisCl->v_actimode = 1;
		return OV_ERR_OK;
	}else if (Ov_Fail(result)){
		thisCl->v_msgAccepted = HTTP_MSG_DENIED;
		return OV_ERR_OK;
	}
	thisCl->v_msgAccepted = HTTP_MSG_ACCEPTED;

	KS_logfile_debug(("decoding successful"));

	thisCl->v_state = KSBASE_CLST_COMPLETED;

	//make sure the typemethod calls the callback and do other stuff
	thisCl->v_actimode = 1;

	return OV_ERR_OK;
}

OV_DLLFNCEXPORT void kshttp_httpClient_startup(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
    OV_INSTPTR_kshttp_httpClient thisCl = Ov_StaticPtrCast(kshttp_httpClient, pobj);

    /* do what the base class does first */
    ov_object_startup(pobj);

    /* do what */
	thisCl->v_ServerResponse.contentLength = 0;
	thisCl->v_ServerResponse.version = NULL;
	thisCl->v_ServerResponse.host = NULL;
	thisCl->v_ServerResponse.content = NULL;

	return;
}

OV_DLLFNCEXPORT OV_RESULT kshttp_httpClient_reset(
	OV_INSTPTR_ksbase_ClientBase this
) {
	OV_INSTPTR_kshttp_httpClient	thisCl = Ov_StaticPtrCast(kshttp_httpClient, this);
	OV_RESULT					result;
	OV_INSTPTR_ksbase_Channel 	pChannel = NULL;
	OV_VTBLPTR_ksbase_Channel	pVtblChannel = NULL;

	thisCl->v_callback.instanceCalled = NULL;
	thisCl->v_callback.callbackFunction = NULL;

	thisCl->v_actimode = 0;
	thisCl->v_msgAccepted = 0;
	thisCl->v_runningKSservice = 0;
	thisCl->v_state = KSBASE_CLST_INITIAL;

	thisCl->v_ServerResponse.content = NULL;

	ov_string_setvalue(&(thisCl->v_serverPort), NULL);
	ov_string_setvalue(&(thisCl->v_ServerResponse.version), NULL);
	ov_string_setvalue(&(thisCl->v_ServerResponse.host), NULL);

	result = getChannelPointer(thisCl, &pChannel, &pVtblChannel);
	if(Ov_Fail(result))
	{
		KS_logfile_error(("%s: Could not get Channel pointers.", this->v_identifier));
		return result;
	}
	pVtblChannel->m_CloseConnection(pChannel);

	return OV_ERR_OK;
}


OV_DLLFNCEXPORT void kshttp_httpClient_shutdown(
	OV_INSTPTR_ov_object 	pobj
) {
	/*
	*   local variables
	*/
	OV_INSTPTR_kshttp_httpClient thisCl = Ov_StaticPtrCast(kshttp_httpClient, pobj);

	thisCl->v_callback.instanceCalled = NULL;
	thisCl->v_callback.callbackFunction = NULL;

	thisCl->v_ServerResponse.content = NULL;

	ov_string_setvalue(&(thisCl->v_ServerResponse.version), NULL);
	ov_string_setvalue(&(thisCl->v_ServerResponse.host), NULL);


    /* set the object's state to "shut down" */
    ov_object_shutdown(pobj);

    return;
}

