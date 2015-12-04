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
#include "base64_encode.h"
#include <errno.h>

/*******************************************************************************************************************************************************************************
 * 				Channel-handling
 *******************************************************************************************************************************************************************************/

OV_RESULT kshttp_getChannelPointer(OV_INSTPTR_kshttp_httpClientBase this, OV_INSTPTR_ksbase_Channel* ppChannel, OV_VTBLPTR_ksbase_Channel* ppVtblChannel)
{
	OV_STRING OptValTemp = NULL;
	OV_INSTPTR_ov_class pClassChannel = NULL;
	OV_RESULT result;

	/*	check if there is a channel and get pointer to it	*/
	*ppChannel = Ov_GetParent(ksbase_AssocChannelDataHandler, this);
	if(!(*ppChannel))
	{/*	no Channel found. Create one	*/
		/*
		 * Checking KSHTTP_USECHANNEL and KS_USECHANNEL options to determine which channel type to use. use TCPChannel per default.
		 */
		ov_memstack_lock();
		OptValTemp = ov_vendortree_getcmdlineoption_value("KSHTTP_USECHANNEL");
		if(!OptValTemp)
			OptValTemp = ov_vendortree_getcmdlineoption_value("KS_USECHANNEL");

		if(!OptValTemp)
		{/*	Neither option specified, use TCPChannel as default	*/
			OptValTemp = ov_memstack_alloc(sizeof("TCPChannel"));
			strcpy(OptValTemp, "TCPChannel");
		}

		if(*OptValTemp)
		{	/*	empty option means do not create channel	*/
			pClassChannel = Ov_StaticPtrCast(ov_class, Ov_GetFirstChild(ov_instantiation, pclass_ov_class));
			while(pClassChannel)
			{
				if(ov_string_compare(pClassChannel->v_identifier, OptValTemp) == OV_STRCMP_EQUAL)
					break;
				pClassChannel = Ov_StaticPtrCast(ov_class, Ov_GetNextChild(ov_instantiation, pClassChannel));
			}

			if(pClassChannel)
			{/*	channel class found create channel	*/
				result = ov_class_createobject(pClassChannel, Ov_PtrUpCast(ov_domain, this), "httpchannel", OV_PMH_DEFAULT, NULL, NULL, NULL, (OV_INSTPTR_ov_object*) ppChannel);
				if(Ov_Fail(result))
				{
					KS_logfile_error(("%s getChannelPointer: could not create channel for communication. reason: %s", this->v_identifier, ov_result_getresulttext(result)));
					ov_memstack_unlock();
					return result;
				}
				else
				{
					result = Ov_Link(ksbase_AssocChannelDataHandler, *ppChannel, this);
					if(Ov_Fail(result))
					{
						KS_logfile_error(("%s getChannelPointer: could not link with channel. reason: %s", this->v_identifier, ov_result_getresulttext(result)));
						ov_memstack_unlock();
						return result;
					}
					(*ppChannel)->v_ClientHandlerAssociated = KSBASE_CH_NOTNEEDED;

				}
			}
		}
		else
		{
			ov_memstack_unlock();
			*ppChannel = NULL;
			*ppVtblChannel = NULL;
			return OV_ERR_OK;
		}
		ov_memstack_unlock();
	}


	Ov_GetVTablePtr(ksbase_Channel, *ppVtblChannel, *ppChannel);
	if(!(*ppVtblChannel))
	{
		KS_logfile_error(("%s: getChannelPointer: could not get VTable of channel object. Aborting.", this->v_identifier));
		return OV_ERR_BADFACTORY;
	}
	return OV_ERR_OK;
}

/*******************************************************************************************************************************************************************************
 * 				Connection handling and sending
 ******************************************************************************************************************************************************************************/

/*	if there is no connection (open or opening) open one and reset lasteventtime	*/
static OV_RESULT initiateConnection(OV_INSTPTR_kshttp_httpClientBase this, OV_INSTPTR_ksbase_Channel pChannel, OV_VTBLPTR_ksbase_Channel pVtblChannel, OV_BOOL isLocal, OV_STRING host, OV_STRING port)
{
	OV_RESULT result;

	if((pChannel->v_ConnectionState != KSBASE_CONNSTATE_OPEN) && (pChannel->v_ConnectionState != KSBASE_CONNSTATE_OPENING))
	{
		if(!port)
			return OV_ERR_BADPARAM;

		if(isLocal)
			result = pVtblChannel->m_OpenLocalConn(pChannel, port);
		else
		{
			if(!host)
				return OV_ERR_BADPARAM;
			result = pVtblChannel->m_OpenConnection(pChannel, host, port);
		}

		if(Ov_Fail(result))
		{
			KS_logfile_error(("%s: initiateConnection: could not open connection to %s:%s", this->v_identifier, host, port));
			return result;
		}
	}
	return OV_ERR_OK;
}

/*	check if connection is open. if so, send and set Client state to busy and reset lasteventtime. if not set client state to awaiting connection. activate typemethod	*/
static OV_RESULT trySend(OV_INSTPTR_kshttp_httpClientBase thisCl, OV_INSTPTR_ksbase_Channel pChannel, OV_VTBLPTR_ksbase_Channel pVtblChannel)
{
	OV_RESULT result = OV_ERR_OK;

	if(pChannel->v_ConnectionState == KSBASE_CONNSTATE_OPEN)
	{
		result = pVtblChannel->m_SendData(pChannel);
		if(Ov_Fail(result))
		{
			ksbase_free_KSDATAPACKET(&(pChannel->v_outData));
			return result;
		}
		thisCl->v_state = KSBASE_CLST_AWAITINGANSWER;	/*	set state to busy and activate typemethod	*/
		thisCl->v_actimode = 1;
	}
	else
	{
		thisCl->v_state = KSBASE_CLST_AWAITINGCONNECTION;
		thisCl->v_actimode = 1;
	}
	return OV_ERR_OK;
}
/**
 *
 * @param method GET, POST, ... (defaults to GET)
 * @param host Hostname or IP to communicate
 * @param port TCP Portnumber (defaults to 80)
 * @param username
 * @param password
 * @param usernameProvided force sending an auth header even if username and/or password is empty
 * @param requestUri URI to
 * @param messageBodyLength The length of the message-body (for POST)
 * @param messageBody the message-body (for POST)
 * @param thisCl client object which does the communication
 * @param callbackThat callback Object (ManagerObject with the register)
 * @param callback
 * @return
 */
OV_RESULT kshttp_generateAndSendHttpMessage(
		OV_STRING method,
		OV_STRING host,
		OV_STRING port,
		OV_STRING username,
		OV_STRING password,
		OV_BOOL usernameProvided,
		OV_STRING requestUri,
		OV_UINT contentLength,
		OV_STRING messageBody,
		const OV_INSTPTR_kshttp_httpClientBase thisCl,
		const OV_INSTPTR_ov_domain	callbackThat,
		void (*callback)(const OV_INSTPTR_ov_domain this, const OV_INSTPTR_ov_domain that)
		)
{
	OV_RESULT result;
	OV_STRING RequestLine = NULL;
	OV_STRING RequestAdditionalHeaders = NULL;
	OV_STRING AuthorizationLine = NULL;
	OV_STRING strBase64string = NULL;
	OV_STRING RequestHeaders = NULL;
	OV_INSTPTR_ksbase_Channel	pChannel = NULL;
	OV_VTBLPTR_ksbase_Channel	pVtblChannel = NULL;

	if(!method){
		ov_string_setvalue(&method, "GET");
	}
	if(!host){
		return OV_ERR_BADPARAM;
	}
	if(!port){
		ov_string_setvalue(&thisCl->v_serverPort, "80");
		ov_string_setvalue(&port, "80");
	}
	if(!requestUri){
		return OV_ERR_BADPARAM;
	}

	if((thisCl->v_state != KSBASE_CLST_COMPLETED) && (thisCl->v_state != KSBASE_CLST_INITIAL))
		return OV_ERR_NOACCESS;

	/*	set callback function	*/
	thisCl->v_callback.instanceCalled = callbackThat;
	thisCl->v_callback.callbackFunction = callback;

	/*	get pointer to channel and to its Vtable	*/
	result = kshttp_getChannelPointer(thisCl, &pChannel, &pVtblChannel);
	if(Ov_Fail(result))
	{
		KS_logfile_error(("%s: Could not get Channel pointers.", thisCl->v_identifier));
		ksbase_free_KSDATAPACKET(&(pChannel->v_outData));
		return result;
	}
	if(!pChannel || !pVtblChannel)
	{
		KS_logfile_warning(("%s: No Channel found and none should be created. Will do nothing.", thisCl->v_identifier));
		ksbase_free_KSDATAPACKET(&(pChannel->v_outData));
		return OV_ERR_OK;
	}

	ov_string_print(&RequestLine, "%s %s HTTP/1.1", method, requestUri);
	if(ov_string_compare(port, "80") == OV_STRCMP_EQUAL){
		ov_string_print(&RequestAdditionalHeaders, "Host:%s\r\n", host);
	}else{
		ov_string_print(&RequestAdditionalHeaders, "Host:%s:%s\r\n", host, port);
	}
	ov_string_print(&RequestAdditionalHeaders, "%sConnection: close\r\nUser-Agent: ACPLT/OV HTTP Client %s (compiled %s %s)\r\n", RequestAdditionalHeaders, OV_LIBRARY_DEF_kshttp.version, __TIME__, __DATE__);
	if(contentLength != 0 && messageBody){
		ov_string_print(&RequestAdditionalHeaders, "%sContent-Length: %" OV_PRINT_UINT "\r\n", RequestAdditionalHeaders, contentLength);
	}
	if(ov_string_compare(username, NULL) != OV_STRCMP_EQUAL || ov_string_compare(password, NULL) != OV_STRCMP_EQUAL || usernameProvided){
		//concat username and password, both are valid to be NULL
		if(username == NULL){
			ov_string_setvalue(&AuthorizationLine, ":");
		}else{
			ov_string_print(&AuthorizationLine, "%s:", username);
		}
		ov_string_append(&AuthorizationLine, password);

		kshttp_encodebase64(&strBase64string, AuthorizationLine);

		ov_string_print(&AuthorizationLine, "Authorization: Basic %s\r\n", strBase64string);
		ov_string_append(&RequestAdditionalHeaders, AuthorizationLine);
		ov_string_setvalue(&strBase64string, NULL);
		ov_string_setvalue(&AuthorizationLine, NULL);
	}
	ov_string_print(&RequestHeaders, "%s\r\n%s\r\n", RequestLine, RequestAdditionalHeaders);

	result = ksbase_KSDATAPACKET_append(&(pChannel->v_outData), (OV_BYTE*) RequestHeaders, ov_string_getlength(RequestHeaders));
	ov_string_setvalue(&RequestAdditionalHeaders, NULL);
	ov_string_setvalue(&RequestLine, NULL);
	ov_string_setvalue(&RequestHeaders, NULL);
	if(Ov_Fail(result)){
		ksbase_free_KSDATAPACKET(&(pChannel->v_outData));
		return result;
	}

	if(contentLength != 0 && messageBody){
		result = ksbase_KSDATAPACKET_append(&(pChannel->v_outData), (OV_BYTE*) messageBody, contentLength);
		if(Ov_Fail(result)){
			ksbase_free_KSDATAPACKET(&(pChannel->v_outData));
			return result;
		}
	}

	/*	send created Message	*/
	result = initiateConnection(thisCl, pChannel, pVtblChannel, FALSE, host, port);

	if(Ov_Fail(result))
	{
		ksbase_free_KSDATAPACKET(&(pChannel->v_outData));
		return result;
	}

	result = trySend(thisCl, pChannel, pVtblChannel);
	if(Ov_Fail(result))
		return result;

	thisCl->v_actimode = 1;

	return OV_ERR_OK;
}

/**
 * decode header of replies (params are not checked for NULL-pointers)
 * @param dataReceived
 * @param responseStruct
 * @param httpParseStatus
 * @return
 */
OV_RESULT kshttp_processServerReplyHeader(KS_DATAPACKET* dataReceived, HTTP_RESPONSE *responseStruct, OV_UINT *httpParseStatus)
{
	OV_STRING *pallheaderslist=NULL;
	OV_STRING *plist=NULL;
	OV_UINT allheaderscount = 0;
	OV_UINT len = 0, i = 0;
	OV_BYTE *endOfHeader = NULL;
	char *endPtr = NULL;
	unsigned long int tempulong = 0;

	if(*httpParseStatus == HTTP_MSG_NEW){
		endOfHeader = kshttp_strnstr(dataReceived->data, "\r\n\r\n", dataReceived->length);
		if(!endOfHeader){
			//not everything from the header is here till now
			*httpParseStatus = HTTP_MSG_INCOMPLETE;
			return OV_ERR_OK;
		}

		//split http header from content
//		http_request_array = ov_string_split((OV_STRING)dataReceived->readPT, "\r\n\r\n", &len);

		//terminate after header for ov_string_split
		*endOfHeader = '\0';

		//header plus both line breaks
		responseStruct->headerLength = endOfHeader - dataReceived->data + 4;

		//Separate all headers
		pallheaderslist = ov_string_split((OV_STRING)dataReceived->data, "\r\n", &allheaderscount);
		if(allheaderscount<=0){
			ov_string_freelist(pallheaderslist);
			//not everything is here till now
			*httpParseStatus = HTTP_MSG_INCOMPLETE;
			return OV_ERR_OK;
		}

		//split out the actual result "HTTP/1.0 200 OK" or "HTTP/1.1 404 Not Found"
		plist = ov_string_split(pallheaderslist[0], " ", &len);
		if(len<3){
			*httpParseStatus = HTTP_MSG_DENIED;
			ov_string_freelist(pallheaderslist);
			ov_string_freelist(plist);
			return OV_ERR_BADPARAM; //400
		}
		ov_string_setvalue(&responseStruct->httpVersion, plist[0]);
		tempulong = strtoul(plist[1], &endPtr, 10);
		if (ERANGE != errno &&
			tempulong < OV_VL_MAXUINT &&
			endPtr != plist[1])
		{
			responseStruct->statusCode = (OV_UINT)tempulong;
		}else{
			*httpParseStatus = HTTP_MSG_DENIED;
			ov_string_freelist(pallheaderslist);
			ov_string_freelist(plist);
			return OV_ERR_BADPARAM; //400
		}

		//check all other headers
		responseStruct->transferEncodingChunked = FALSE;
		for (i=1; i<allheaderscount; i++){
			if(ov_string_match(pallheaderslist[i], "?ontent-?ength:*") == TRUE){
				ov_string_freelist(plist);
				plist = ov_string_split(pallheaderslist[i], "Content-Length: ", &len);
				if(len == 1){
					//we have no result? perhaps the header was lowercase... second try
					ov_string_freelist(plist);
					plist = ov_string_split(pallheaderslist[i], "content-length: ", &len);
				}
				if(len == 2){
					tempulong = strtoul(plist[1], &endPtr, 10);
					if (ERANGE != errno &&
						tempulong < OV_VL_MAXUINT &&
						endPtr != plist[1])
					{
						responseStruct->contentLength = (OV_UINT)tempulong;
					}
				}
			}else if(ov_string_match(pallheaderslist[i], "content-type:*") == TRUE){
				ov_string_freelist(plist);
				plist = ov_string_split(pallheaderslist[i], "Content-Type: ", &len);
				if(len == 1){
					//we have no result? perhaps the header was lowercase... second try
					ov_string_freelist(plist);
					plist = ov_string_split(pallheaderslist[i], "content-type: ", &len);
				}
				if(len == 2){
					ov_string_setvalue(&(responseStruct->contentType), plist[1]);
				}
			}else if(ov_string_match(pallheaderslist[i], "?ransfer-?ncoding:*") == TRUE){
				if(ov_string_comparei(pallheaderslist[i], "Transfer-Encoding: chunked") == OV_STRCMP_EQUAL){
					//the server does not know how much data he will send
					responseStruct->transferEncodingChunked = TRUE;
					responseStruct->contentLength = 0;
				}
			}
		}
		ov_string_freelist(pallheaderslist);
		ov_string_freelist(plist);

		//save pointer to the content
		dataReceived->readPT = dataReceived->data + responseStruct->headerLength;
		*httpParseStatus = HTTP_MSG_HEADERACCEPTED;
	}

	//check if we have all http content
	if(dataReceived->length >= responseStruct->headerLength + responseStruct->contentLength && responseStruct->transferEncodingChunked == FALSE){
		//everything was received
		*httpParseStatus = HTTP_MSG_ACCEPTED;
		return OV_ERR_OK;
	}else if(responseStruct->transferEncodingChunked == TRUE){
		//we have to check if this message is complete by looking into it 8-/
		//only check if all is received, so "save" into an NULL pointer
		return kshttp_decodeTransferEncodingChunked(dataReceived->readPT, NULL, &responseStruct->contentLength, dataReceived->length - responseStruct->headerLength, httpParseStatus);
	}else{
		//we do not have all data till now
		return OV_ERR_OK;
	}
}

/**
 * decodes a chunked transfer to the heap, or tests if a decoding is possible with the data length
 * @param rawHTTPmessage http traffic to decode
 * @param entityBody is allowed to be NULL! Is in the heap
 * @param contentLength of binary data
 * @param receivedlength which is available at the rawHTTPmessage
 * @return
 */
OV_RESULT kshttp_decodeTransferEncodingChunked(OV_BYTE *rawHTTPmessage, OV_BYTE **entityBody, OV_UINT *contentLength, OV_UINT receivedlength, OV_UINT *httpParseStatus){
	OV_UINT readlength = 0;
	OV_UINT chunkSize = 0;
	OV_BYTE *oldBody;
	OV_BYTE *readPointer = NULL;
	OV_BYTE *oldReadPointer = rawHTTPmessage;
	OV_UINT writelength = 0;
	*contentLength = 0;

	if(!rawHTTPmessage){
		return OV_ERR_BADPARAM;
	}

	if(entityBody){
		//initial the pointer
		*entityBody = NULL;
	}

	//get the length of the first chunk and move the pointer after the length

	chunkSize = strtoul((char *)oldReadPointer, (char **)&readPointer, 16);

	while (chunkSize > 0){
		//strtoul does not tell us how many bytes the chunk length information was
		readlength += readPointer - oldReadPointer;
		readlength += 2 + chunkSize + 2; // we have \r\n, the chunk and \r\n

		writelength = *contentLength;
		*contentLength += chunkSize;
		//we need the next line
		readPointer += 2;

		if(readlength > receivedlength){
			//we do not have all data till now, abort graceful
			*contentLength = 0;
			return OV_ERR_OK;
		}

		if(entityBody){
			oldBody = *entityBody;
			*entityBody = Ov_HeapRealloc(oldBody, *contentLength);
			if(!*entityBody){
				Ov_HeapFree(oldBody);
				*contentLength = 0;
				if(httpParseStatus){
					*httpParseStatus = HTTP_MSG_DENIED;
				}
				return OV_ERR_GENERIC;
			}
			memcpy(&(*entityBody)[writelength], readPointer, chunkSize);
		}

		//move to the end of the chunk and the newline
		oldReadPointer = readPointer+chunkSize+2;

		//get the length of the new chunk and move the pointer after the length
		chunkSize = strtoul((char *)oldReadPointer, (char **)&readPointer, 16);
	}

	if(httpParseStatus){
		*httpParseStatus = HTTP_MSG_ACCEPTED;
	}
	return OV_ERR_OK;
}

OV_RESULT kshttp_encodebase64(OV_STRING * strBase64string, OV_STRING input){
	base64_encodestate state;
	OV_UINT n = 0, length;
	OV_STRING out = NULL;
	int rc = 0;

	n = ov_string_getlength(input);
	length = 8 + ((n * 5) / 3) + (n / 72); /* bytes * 4/3 + bytes/72 + 4 rounding errors + 2*trailing '=' + trailing \n + trailing \0 */
	out = Ov_HeapMalloc((length+1) * sizeof(char));

	base64_init_encodestate(&state);
	rc = base64_encode_block(input, n, out, &state);
	rc += base64_encode_blockend(out + rc, &state);
	out[rc-1] = '\0';
	ov_string_setvalue(strBase64string, out);
	Ov_HeapFree(out);

	return OV_ERR_OK;
}
