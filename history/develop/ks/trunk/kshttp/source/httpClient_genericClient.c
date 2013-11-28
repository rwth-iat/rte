
/******************************************************************************
*
*   FILE
*   ----
*   genericHttpClient.c
*
*   History
*   -------
*   2013-08-07   File created
*
*******************************************************************************
*
*   This file is generated by the 'acplt_builder' command
*
******************************************************************************/


#ifndef OV_COMPILE_LIBRARY_kshttp
#define OV_COMPILE_LIBRARY_kshttp
#endif

#include "limits.h"
#include "kshttp.h"
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
 * @param URI
 * @param server
 * @param port
 * @param username
 * @param password
 * @param requestUri
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

	while(offset <= URIlength){
		//    http://user:pass@localhost:8015/path/to/resource.html?query#fragment
		// or http://localhost:8015/path/to/resource.html?query#fragment
		// or http://[2001:0db8:85a3:08d3:1319:8a2e:0370:7344]:8080/
		// or http://user:pass@[2001:0db8:85a3:08d3:1319:8a2e:0370:7344]:8080/
		switch ((*URI)[offset]) {
		case '[':
			//todo check state and complain
			old_state = state;
			state = DECODESTATE_IPV6HOST;
			lastoffset++;
			break;
		case ']':
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
			//founduserorserver = TRUE;
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
					//an ipv6 adress could include an ]
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
						//an ipv6 adress could include an ], which would be valid as a password
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
				//the requestUri includes the seperator /, but not the fragment
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
	//check the syntax
	result = kshttp_decodeURI(&value, &thisCl->v_serverHost, &thisCl->v_serverPort, NULL, NULL, NULL, NULL);

	if(Ov_Fail(result)){
		return result;
	}
	ov_string_setvalue(&thisCl->v_contentType, "");
	ov_string_setvalue(&thisCl->v_messageBody, "");
	thisCl->v_state = KSBASE_CLST_INITIAL;
	thisCl->v_httpStatusCode = 0;
	thisCl->v_httpParseStatus = HTTP_MSG_NEW;
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

	ov_string_setvalue(&thisCl->v_contentType, "");
	ov_string_setvalue(&thisCl->v_messageBody, "");
	thisCl->v_state = KSBASE_CLST_INITIAL;
	thisCl->v_httpStatusCode = 0;
	thisCl->v_httpParseStatus = HTTP_MSG_NEW;

	result = kshttp_decodeURI(&thisCl->v_URI, &thisCl->v_serverHost, &thisCl->v_serverPort, &username, &password, &requestUri, &usernameProvided);
	if(Ov_Fail(result)){
		return result;
	}

	return kshttp_generateAndSendHttpMessage("GET", thisCl->v_serverHost, thisCl->v_serverPort, username, password, usernameProvided, requestUri, 0, NULL, Ov_PtrUpCast(kshttp_httpClientBase, thisCl), Ov_PtrUpCast(ov_domain, thisCl), &kshttp_genericHttpClient_Callback);
}

void kshttp_genericHttpClient_Callback(OV_INSTPTR_ov_domain instanceCalled, OV_INSTPTR_ov_domain instanceCalling){
	OV_INSTPTR_kshttp_genericHttpClient	thisCl = Ov_DynamicPtrCast(kshttp_genericHttpClient, instanceCalling);
	OV_RESULT result = OV_ERR_OK;
	OV_INSTPTR_ksbase_Channel 	pChannel = NULL;
	OV_VTBLPTR_ksbase_Channel	pVtblChannel = NULL;

	result = kshttp_getChannelPointer(Ov_PtrUpCast(kshttp_httpClientBase, thisCl), &pChannel, &pVtblChannel);
	if(Ov_Fail(result))
	{
		KS_logfile_error(("%s: Could not get Channel pointers.", this->v_identifier));
		return;
	}
	ov_string_setvalue(&thisCl->v_contentType, thisCl->v_ServerResponse.contentType);

	if(	ov_string_match(thisCl->v_contentType, "text/*") == TRUE ||
			ov_string_match(thisCl->v_contentType, "*application/xml*") == TRUE ||	//includes application/xml-dtd
			ov_string_match(thisCl->v_contentType, "*+xml*") == TRUE ||	//includes image/svg+xml
			ov_string_match(thisCl->v_contentType, "application/json*") == TRUE ||
			ov_string_match(thisCl->v_contentType, "application/javascript*") == TRUE ){
		if(thisCl->v_ServerResponse.transferEncodingChunked == TRUE){
			kshttp_decodeTransferEncodingChunked((OV_STRING*)&thisCl->v_ServerResponse.messageBodyPtr, &thisCl->v_messageBody, &thisCl->v_ServerResponse.contentLength, OV_VL_MAXINT);
		}else if(thisCl->v_ServerResponse.contentLength != 0){
			thisCl->v_messageBody =  ov_database_malloc(thisCl->v_ServerResponse.contentLength+1);
			if(!thisCl->v_messageBody){
				//database too small
				thisCl->v_httpParseStatus = HTTP_MSG_DBOUTOFMEMORY;

				//this memory is from the ksdatapackage!
				thisCl->v_ServerResponse.messageBodyPtr = NULL;
				pVtblChannel->m_CloseConnection(pChannel);
				return;
			}
			memcpy(thisCl->v_messageBody, thisCl->v_ServerResponse.messageBodyPtr, thisCl->v_ServerResponse.contentLength);
			thisCl->v_messageBody[thisCl->v_ServerResponse.contentLength] = '\0';
		}
	}else{
		ov_string_setvalue(&thisCl->v_messageBody, "binary mimetype");
	}

	//this memory is from the ksdatapackage!
	thisCl->v_ServerResponse.messageBodyPtr = NULL;
	pVtblChannel->m_CloseConnection(pChannel);

	return;
}
