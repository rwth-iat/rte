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
 *
 ***********************************************************************/


#ifndef OV_COMPILE_LIBRARY_kshttp
#define OV_COMPILE_LIBRARY_kshttp
#endif


#include "config.h"

/**
 * we do not want to execute code if client is sending something. No content from the client should arrive.
 */
OV_DLLFNCEXPORT OV_RESULT kshttp_getvarpushhandler_HandleRequest(
		OV_INSTPTR_ksbase_ClientHandler this,
		OV_INSTPTR_ksbase_Channel pChannel,
		KS_DATAPACKET* dataReceived,
		KS_DATAPACKET* answer
) {
	/*
	 *   local variables
	 */

	return OV_ERR_OK;
}

/**
 * this is triggerd cyclic and pushes an update to the Channel if the result has changed
 */
OV_DLLFNCEXPORT void kshttp_getvarpushhandler_typemethod (
		OV_INSTPTR_ksbase_ComTask	pobj
) {
	/*
	 *   local variables
	 */
	OV_INSTPTR_kshttp_getvarpushhandler this = Ov_StaticPtrCast(kshttp_getvarpushhandler, pobj);
	OV_INSTPTR_ksbase_Channel pChannel = NULL;

	HTTP_REQUEST ClientRequest;
	HTTP_RESPONSE ServerResponse;

	//initialise the structs
	ServerResponse.contentString = NULL;
	ClientRequest.urlQuery.veclen = 0;
	ClientRequest.urlQuery.value = NULL;

	//fill with data
	ClientRequest.response_format = this->v_responseformat;
	Ov_SetDynamicVectorValue(&ClientRequest.urlQuery, this->v_urlQuery.value, this->v_urlQuery.veclen, STRING);

	ov_string_setvalue(&ServerResponse.contentString, "data:");
	kshttp_printresponseheader(&ServerResponse.contentString, ClientRequest.response_format, "getvar");
	(void)kshttp_exec_getvar(ClientRequest, &ServerResponse);
	kshttp_printresponsefooter(&ServerResponse.contentString, ClientRequest.response_format, "getvar");
	ov_string_append(&ServerResponse.contentString, "\r\n\r\n");

	//todo zeilenumbruch in vorhandenen Daten?


	if(ov_string_compare(this->v_responseSent, ServerResponse.contentString) != OV_STRCMP_EQUAL){
		//we have new data, so send
		pChannel = Ov_GetParent(ksbase_AssocChannelClientHandler, this);
		ksbase_KSDATAPACKET_append(&pChannel->v_outData, (OV_BYTE*)ServerResponse.contentString, ov_string_getlength(ServerResponse.contentString));

		//remember this response
		ov_string_setvalue(&this->v_responseSent, ServerResponse.contentString);

		this->v_receivedCalls++;
	}

	//cleanup
	ov_string_setvalue(&ServerResponse.contentString, NULL);
	Ov_SetDynamicVectorLength(&ClientRequest.urlQuery, 0, STRING);

	return;
}

