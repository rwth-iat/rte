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

void kshttp_genericHttpClient_Callback(OV_INSTPTR_ov_domain instanceCalled, OV_INSTPTR_ov_domain instanceCalling);


OV_DLLFNCEXPORT OV_RESULT kshttp_genericHttpClient_constructor(
	OV_INSTPTR_ov_object 	pobj
) {
	/*
	*   local variables
	*/
	OV_INSTPTR_kshttp_genericHttpClient thisCl = Ov_StaticPtrCast(kshttp_genericHttpClient, pobj);
	OV_RESULT    result;

	/* do what the base class does first */
	result = ksbase_ComTask_constructor(pobj);
	if(Ov_Fail(result)){
		return result;
	}

	ov_string_setvalue(&thisCl->v_serverPort, "80");
	ov_string_setvalue(&thisCl->v_serverName, "<unused>");

	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT kshttp_genericHttpClient_reset(
	OV_INSTPTR_ksbase_ClientBase this
) {
	/*
	*   local variables
	*/
	OV_BOOL fr = OV_ERR_OK;
	OV_INSTPTR_kshttp_genericHttpClient thisCl = Ov_StaticPtrCast(kshttp_genericHttpClient, this);

	fr = kshttp_httpClientBase_reset(this);
	if(Ov_Fail(fr)){
		return fr;
	}
	ov_string_setvalue(&thisCl->v_serverPort, "80");

	ov_string_setvalue(&thisCl->v_messageBody, NULL);
	thisCl->v_contentLength = 0;
	ov_string_setvalue(&thisCl->v_contentType, NULL);

	return OV_ERR_OK;
}




#define DECODESTATE_INIT 0
#define DECODESTATE_USER_OR_SERVER 1
#define DECODESTATE_PASSWORD_OK 2
#define DECODESTATE_SERVER_OK 3
#define DECODESTATE_PORT_OK 4
#define DECODESTATE_READY 5
#define DECODESTATE_IPV6HOST 6
#define DECODESTATE_ERROR 42

/**
 * helper function to decode an Uniform Resource Identifier
 * http://user:pass@localhost:8015/path/to/resource.html?query#fragment
 * parameters could be NULL, if only a check should be performed or this information is not needed
 * @param URI Input string to analyze
 * @param server pointer to the server string
 * @param port pointer to the port string
 * @param username pointer to the username string
 * @param password pointer to the password string
 * @param requestUri pointer to the requestURI string (without the server, port,...)
 * @param usernameProvided pointer to an bool to indicate a username was provided
 * @return
 */
OV_RESULT kshttp_decodeURI(const OV_STRING *URI, OV_STRING *server, OV_STRING *port, OV_STRING *username, OV_STRING *password, OV_STRING *requestUri, OV_BOOL *usernameProvided){
	OV_UINT offset = 0;
	OV_UINT lastoffset = 0;
	OV_UINT URIlength = 0;
	OV_UINT thislength = 0;
	OV_UINT serverlength = 0;
	OV_UINT state = DECODESTATE_INIT;
	OV_UINT old_state = DECODESTATE_INIT;
	OV_STRING strTemp = NULL;
	OV_STRING strUserOrServer = NULL;
	OV_STRING strPassword = NULL;
	OV_STRING strServer = NULL;
	OV_STRING strPort = NULL;
	OV_STRING *plist = NULL;
	OV_UINT len = 0;

	URIlength = ov_string_getlength(*URI);
	if(URIlength == 0){
		return OV_ERR_BADPARAM;
	}

	if(ov_string_match(*URI, "http://*") == TRUE){
		//http:// found
		lastoffset = 7;
		offset = 7;
	}else if(ov_string_match(*URI, "https://*") == TRUE){
		return OV_ERR_NOTIMPLEMENTED;
	}else if(ov_string_match(*URI, "ftp://*") == TRUE){
		return OV_ERR_NOTIMPLEMENTED;
	}else{
		//no scheme given, we guess we have http
		lastoffset = 0;
		offset = 0;
	}
	//setting default port to 80. Will be overwritten if set via url
	ov_string_setvalue(port, "80");

	while(offset <= URIlength){
		//    http://user:pass@localhost:8015/path/to/resource.html?query#fragment
		// or http://localhost:8015/path/to/resource.html?query#fragment
		// or http://[2001:0db8:85a3:08d3:1319:8a2e:0370:7344]:8080/
		// or http://user:pass@[2001:0db8:85a3:08d3:1319:8a2e:0370:7344]:8080/
		switch ((*URI)[offset]) {
		case '[':
			if(state != DECODESTATE_USER_OR_SERVER && state != DECODESTATE_INIT){
				state = DECODESTATE_ERROR;
				break;
			}
			old_state = state;
			state = DECODESTATE_IPV6HOST;
			lastoffset++;
			break;
		case ']':
			if(state != DECODESTATE_IPV6HOST){
				state = DECODESTATE_ERROR;
				break;
			}
			state = old_state;
			break;
		case ':':
			if(state == DECODESTATE_IPV6HOST){
				// collons in ipv6 ips are no seperator
				break;
			}else if(!(state == DECODESTATE_INIT || state == DECODESTATE_PASSWORD_OK)){
				state = DECODESTATE_ERROR;
				break;
			}
			thislength = offset-lastoffset;
			strUserOrServer = (OV_STRING)ov_database_malloc(thislength +1);
			memcpy(strUserOrServer, &(*URI)[lastoffset], thislength);
			strUserOrServer[thislength] = '\0';
			if(state == DECODESTATE_INIT){
				//not sure if this the server or user
				serverlength = thislength;
				state = DECODESTATE_USER_OR_SERVER;
			}else if(state == DECODESTATE_PASSWORD_OK){
				//this is the server, so remember
				if(strUserOrServer[thislength-1] == ']'){
					//an ipv6 adress could include an ]
					strUserOrServer[thislength-1] = '\0';
				}
				ov_string_setvalue(server, strUserOrServer);
				ov_database_free(strUserOrServer);
				strUserOrServer = NULL;
				state = DECODESTATE_SERVER_OK;
			}
			lastoffset = offset+1;
			break;
		case '@':
			if(state != DECODESTATE_USER_OR_SERVER && state != DECODESTATE_INIT){
				state = DECODESTATE_ERROR;
				break;
			}
			thislength = offset-lastoffset;
			if(state == DECODESTATE_INIT){
				strUserOrServer = (OV_STRING)ov_database_malloc(thislength +1);
				memcpy(strUserOrServer, &(*URI)[lastoffset], thislength);
				strUserOrServer[thislength] = '\0';
			}else{
				strPassword = (OV_STRING)ov_database_malloc(thislength +1);
				memcpy(strPassword, &(*URI)[lastoffset], thislength);
				strPassword[thislength] = '\0';
			}
			ov_string_setvalue(username, strUserOrServer);
			ov_string_setvalue(password, strPassword);
			ov_database_free(strUserOrServer);
			ov_database_free(strPassword);
			strUserOrServer = NULL;
			strPassword = NULL;
			lastoffset = offset+1;
			if(usernameProvided != NULL){
				*usernameProvided = TRUE;
			}
			state = DECODESTATE_PASSWORD_OK;
			break;
		case '/':
		case '?':
		case '#':
		case '\0':
			thislength = offset-lastoffset;
			if(state == DECODESTATE_IPV6HOST){
				state = DECODESTATE_ERROR;
				break;
			}else if(state == DECODESTATE_PASSWORD_OK || state == DECODESTATE_INIT){
				//save server info
				strServer = (OV_STRING)ov_database_malloc(thislength +1);
				memcpy(strServer, &(*URI)[lastoffset], thislength);
				if(strServer[thislength-1] == ']'){
					//an ipv6 address could include an ]
					strServer[thislength-1] = '\0';
				}else{
					strServer[thislength] = '\0';
				}
				ov_string_setvalue(server, strServer);
				ov_database_free(strServer);
				strServer = NULL;

				state = DECODESTATE_READY;
			}else if(state == DECODESTATE_SERVER_OK || state == DECODESTATE_USER_OR_SERVER){
				//save port info
				strPort = (OV_STRING)ov_database_malloc(thislength +1);
				memcpy(strPort, &(*URI)[lastoffset], thislength);
				strPort[thislength] = '\0';
				ov_string_setvalue(port, strPort);
				ov_database_free(strPort);
				strPort = NULL;
				if(state == DECODESTATE_USER_OR_SERVER){
					//save server info
					if(strUserOrServer[serverlength-1] == ']'){
						//an ipv6 address could include an ], which would be valid as a password
						strUserOrServer[serverlength-1] = '\0';
					}
					//server could be NULL if only a validation is requested, so do not check return value
					ov_string_setvalue(server, strUserOrServer);
					ov_database_free(strUserOrServer);
					strUserOrServer = NULL;
				}
				state = DECODESTATE_READY;
			}
			lastoffset = offset+1;
			break;
		default:
			if(state == DECODESTATE_IPV6HOST){
				//ipv6 address has to be hex or . (RFC 4291 sect 2.2)
				if(
						((*URI)[offset] >= '0'
							&&	(*URI)[offset] <= '9')
					||	((*URI)[offset] >= 'a'
							&&	(*URI)[offset] <= 'f')
					||	((*URI)[offset] >= 'A'
							&&	(*URI)[offset] <= 'F')
					||	(*URI)[offset] == '.'
				){
					//valid
				}else{
					state = DECODESTATE_ERROR;
				}
			}
			break;
		}
		if(state == DECODESTATE_READY){
			if((*URI)[lastoffset-1] == '\0' || (*URI)[lastoffset-1] == '#'){
				//set / if the termination of the uri is implicit
				ov_string_setvalue(requestUri, "/");
				break;
			}
			if((*URI)[lastoffset-1] == '?'){
				//prefix / if the termination is an query string
				ov_string_print(&strTemp, "/%s", &(*URI)[lastoffset-1]);
				plist = ov_string_split(strTemp, "#", &len);
				ov_string_setvalue(&strTemp, NULL);
			}else{
				//the requestUri includes the separator /, but not the fragment
				plist = ov_string_split(&(*URI)[lastoffset-1], "#", &len);
			}
			//plist[0] is available, since we do not have a NULL pointer
			ov_string_setvalue(requestUri, plist[0]);
			ov_string_freelist(plist);
			//we are ready, so break while loop
			break;
		}else if(state == DECODESTATE_ERROR){
			//we are ready, so break while loop
			break;
		}
		offset++;
	}
	//free does not crash if the pointer is NULL
	ov_database_free(strUserOrServer);
	strUserOrServer = NULL;
	if(state != DECODESTATE_READY){
		return OV_ERR_BADPARAM;
	}
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT kshttp_genericHttpClient_URI_set(
	OV_INSTPTR_kshttp_genericHttpClient          thisCl,
	const OV_STRING  value
) {
	OV_RESULT result = OV_ERR_OK;

	//reset client
	result = kshttp_genericHttpClient_reset(Ov_PtrUpCast(ksbase_ClientBase, thisCl));
	if(Ov_Fail(result)){
		return result;
	}

	//check the syntax
	result = kshttp_decodeURI(&value, &thisCl->v_serverHost, &thisCl->v_serverPort, NULL, NULL, NULL, NULL);

	if(Ov_Fail(result)){
		return result;
	}
	return ov_string_setvalue(&thisCl->v_URI, value);
}

OV_DLLFNCEXPORT OV_RESULT kshttp_genericHttpClient_beginCommunication_set(
	OV_INSTPTR_kshttp_genericHttpClient          thisCl,
	const OV_BOOL  value
) {
	OV_RESULT result = OV_ERR_OK;
	OV_STRING username = NULL;
	OV_STRING password = NULL;
	OV_STRING requestUri = NULL;
	OV_BOOL usernameProvided = FALSE;

	if(value == FALSE){
		return OV_ERR_OK;
	}
	kshttp_genericHttpClient_reset(Ov_PtrUpCast(ksbase_ClientBase, thisCl));

	result = kshttp_decodeURI(&thisCl->v_URI, &thisCl->v_serverHost, &thisCl->v_serverPort, &username, &password, &requestUri, &usernameProvided);
	if(Ov_Fail(result)){
		return result;
	}

	return kshttp_generateAndSendHttpMessage("GET", thisCl->v_serverHost, thisCl->v_serverPort, username, password, usernameProvided, requestUri, 0, NULL, Ov_PtrUpCast(kshttp_httpClientBase, thisCl), Ov_PtrUpCast(ov_domain, thisCl), &kshttp_genericHttpClient_Callback);
}

void kshttp_genericHttpClient_Callback(OV_INSTPTR_ov_domain UNUSED instanceCalled, OV_INSTPTR_ov_domain instanceCalling){
	OV_INSTPTR_kshttp_genericHttpClient	thisCl = Ov_DynamicPtrCast(kshttp_genericHttpClient, instanceCalling);
	OV_RESULT result = OV_ERR_OK;
	OV_INSTPTR_ksbase_Channel 	pChannel = NULL;
	OV_VTBLPTR_ksbase_Channel	pVtblChannel = NULL;

	result = kshttp_getChannelPointer(Ov_PtrUpCast(kshttp_httpClientBase, thisCl), &pChannel, &pVtblChannel);
	if(Ov_Fail(result))
	{
		KS_logfile_error(("%s: Could not get Channel pointers.", thisCl->v_identifier));
		return;
	}
	ov_string_setvalue(&thisCl->v_contentType, thisCl->v_ServerResponse.contentType);

	if(thisCl->v_ServerResponse.transferEncodingChunked == TRUE){
		//the http message could be transfered as chunked, so we have to decode it (to the heap)
		kshttp_decodeTransferEncodingChunked(pChannel->v_inData.readPT, &thisCl->v_ServerResponse.contentBinary, &thisCl->v_ServerResponse.contentLength, OV_VL_MAXINT, NULL);
	}else if(thisCl->v_ServerResponse.contentLength != 0){
		//the content is available unchunked. copy it to the heap
		thisCl->v_ServerResponse.contentBinary = Ov_HeapMalloc(thisCl->v_ServerResponse.contentLength);
		if(!thisCl->v_ServerResponse.contentBinary){
			//heap too small
			thisCl->v_httpParseStatus = HTTP_MSG_HEAPOUTOFMEMORY;
			pVtblChannel->m_CloseConnection(pChannel);
			ksbase_free_KSDATAPACKET(&pChannel->v_inData);
			return;
		}
		memcpy(thisCl->v_ServerResponse.contentBinary, pChannel->v_inData.readPT, thisCl->v_ServerResponse.contentLength);
	}
	//move the read pointer to the end, as we handled all data
	pChannel->v_inData.readPT = pChannel->v_inData.data + pChannel->v_inData.length;

	//if the content is a string, move it to the STRING output
	if(	ov_string_match(thisCl->v_contentType, "text/*") == TRUE ||
			ov_string_match(thisCl->v_contentType, "*application/xml*") == TRUE ||	//includes application/xml-dtd
			ov_string_match(thisCl->v_contentType, "*+xml*") == TRUE ||	//includes image/svg+xml
			ov_string_match(thisCl->v_contentType, "application/json*") == TRUE ||
			ov_string_match(thisCl->v_contentType, "application/javascript*") == TRUE ){
		thisCl->v_messageBody =  ov_database_malloc(thisCl->v_ServerResponse.contentLength+1);
		if(!thisCl->v_messageBody){
			//database too small
			thisCl->v_httpParseStatus = HTTP_MSG_DBOUTOFMEMORY;
			Ov_HeapFree(thisCl->v_ServerResponse.contentBinary);
			thisCl->v_ServerResponse.contentBinary = NULL;
			pVtblChannel->m_CloseConnection(pChannel);
			ksbase_free_KSDATAPACKET(&pChannel->v_inData);
			return;
		}
		memcpy(thisCl->v_messageBody, thisCl->v_ServerResponse.contentBinary, thisCl->v_ServerResponse.contentLength);
		//the contentBinary has no string termination till now
		thisCl->v_messageBody[thisCl->v_ServerResponse.contentLength] = '\0';

		Ov_HeapFree(thisCl->v_ServerResponse.contentBinary);
		thisCl->v_ServerResponse.contentBinary = NULL;
	}else{
		ov_string_setvalue(&thisCl->v_messageBody, "binary mimetype, content not exposable to this variable");
	}
	thisCl->v_contentLength = thisCl->v_ServerResponse.contentLength;

	pVtblChannel->m_CloseConnection(pChannel);
	ksbase_free_KSDATAPACKET(&pChannel->v_inData);

	return;
}
