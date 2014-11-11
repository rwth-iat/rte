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
#ifndef KSHTTP_DISABLE_GZIP
	#include "gzip.h"
#endif

/*** TEMP ***/
OV_DLLFNCEXPORT OV_BOOL kshttp_httpclienthandler_stream_get(
		OV_INSTPTR_kshttp_httpclienthandler          pobj
) {
	return pobj->v_stream;
}

OV_DLLFNCEXPORT OV_RESULT kshttp_httpclienthandler_stream_set(
		OV_INSTPTR_kshttp_httpclienthandler          pobj,
		const OV_BOOL  value
) {
	pobj->v_stream = value;
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_STRING kshttp_httpclienthandler_streamrequestheader_get(
		OV_INSTPTR_kshttp_httpclienthandler          pobj
) {
	return pobj->v_streamrequestheader;
}

OV_DLLFNCEXPORT OV_RESULT kshttp_httpclienthandler_streamrequestheader_set(
		OV_INSTPTR_kshttp_httpclienthandler          pobj,
		const OV_STRING  value
) {
	return ov_string_setvalue(&pobj->v_streamrequestheader,value);
}

OV_DLLFNCEXPORT OV_STRING kshttp_httpclienthandler_streambuffer_get(
		OV_INSTPTR_kshttp_httpclienthandler          pobj
) {
	return pobj->v_streambuffer;
}

OV_DLLFNCEXPORT OV_RESULT kshttp_httpclienthandler_streambuffer_set(
		OV_INSTPTR_kshttp_httpclienthandler          pobj,
		const OV_STRING  value
) {
	return ov_string_setvalue(&pobj->v_streambuffer,value);
}

/**
 * Builds header for http communication from OV_RESULT
 * @param result
 * @param http_version
 * @param header will be overwritten
 * @param body will be overwritten in an error state
 */
DLLFNCEXPORT void kshttp_httpclienthandler_mapresult2http(const KSHTTP_REQUEST request, KSHTTP_RESPONSE *response, OV_RESULT const result){
	OV_STRING tmp_header = NULL;
	OV_STRING tmp_body = NULL;
	//this is very ugly, but better than code duplication
	//the problem is that ov_string_print does not support NULL pointers as argument
	if(response->header == NULL){
		tmp_header = "";
	}else{
		tmp_header = response->header;
	}
	if(response->contentString == NULL){
		tmp_body = "";
	}else{
		tmp_body = response->contentString;
	}

	//no free needed since no memory allocated
	switch (result)
	{
	//still some duplication, but good enough for now
	case OV_ERR_OK:
		ov_string_print(&response->header, "HTTP/%s %s%s", response->httpVersion, HTTP_200_HEADER, tmp_header);
		break;
	case OV_ERR_BADNAME:
		ov_string_print(&response->header, "HTTP/%s %s%s", response->httpVersion, HTTP_400_HEADER, tmp_header);
		if(request.response_format != KSX){
			ov_string_print(&response->contentString, "KS_ERR_BADNAME: %s%s", HTTP_400_BODY, tmp_body);
		}
		break;
	case OV_ERR_BADPARAM:
		ov_string_print(&response->header, "HTTP/%s %s%s", response->httpVersion, HTTP_400_HEADER, tmp_header);
		if(request.response_format != KSX){
			ov_string_print(&response->contentString, "KS_ERR_BADPARAM: %s%s", HTTP_400_BODY, tmp_body);
		}
		break;
	case OV_ERR_BADAUTH:
		ov_string_print(&response->header, "HTTP/%s %s%s", response->httpVersion, HTTP_401_HEADER, tmp_header);
		if(request.response_format != KSX){
			ov_string_print(&response->contentString, "KS_ERR_BADAUTH: %s%s", HTTP_401_BODY, tmp_body);
		}
		break;
	case OV_ERR_NOACCESS:
		ov_string_print(&response->header, "HTTP/%s %s%s", response->httpVersion, HTTP_403_HEADER, tmp_header);
		if(request.response_format != KSX){
			ov_string_print(&response->contentString, "KS_ERR_NOACCESS: %s%s", HTTP_403_BODY, tmp_body);
		}
		break;
	case OV_ERR_BADPATH:
		ov_string_print(&response->header, "HTTP/%s %s%s", response->httpVersion, HTTP_404_HEADER, tmp_header);
		if(request.response_format != KSX){
			ov_string_print(&response->contentString, "KS_ERR_BADPATH: %s%s", HTTP_404_BODY, tmp_body);
		}
		break;
	case OV_ERR_BADVALUE:
		ov_string_print(&response->header, "HTTP/%s %s%s", response->httpVersion, HTTP_406_HEADER, tmp_header);
		if(request.response_format != KSX){
			ov_string_print(&response->contentString, "KS_ERR_BADVALUE: %s%s", HTTP_406_BODY, tmp_body);
		}
		break;
	case OV_ERR_NOTIMPLEMENTED:
		ov_string_print(&response->header, "HTTP/%s %s%s", response->httpVersion, HTTP_501_HEADER, tmp_header);
		if(request.response_format != KSX){
			ov_string_print(&response->contentString, "KS_ERR_NOTIMPLEMENTED: %s%s", HTTP_501_BODY, tmp_body);
		}
		break;
	case OV_ERR_GENERIC:
		ov_string_print(&response->header, "HTTP/%s %s%s", response->httpVersion, HTTP_503_HEADER, tmp_header);
		if(request.response_format != KSX){
			ov_string_print(&response->contentString, "KS_ERR_GENERIC: %s%s", HTTP_503_BODY, tmp_body);
		}
		break;
	case OV_ERR_BADTYPE:
		ov_string_print(&response->header, "HTTP/%s %s%s", response->httpVersion, HTTP_412_HEADER, tmp_header);
		if(request.response_format != KSX){
			ov_string_print(&response->contentString, "KS_ERR_BADTYPE: %s%s", HTTP_412_BODY, tmp_body);
		}
		break;
	case OV_ERR_ALREADYEXISTS:
		ov_string_print(&response->header, "HTTP/%s %s%s", response->httpVersion, HTTP_409_HEADER, tmp_header);
		if(request.response_format != KSX){
			ov_string_print(&response->contentString, "KS_ERR_ALREADYEXISTS: %s%s", HTTP_409_BODY, tmp_body);
		}
		break;
	case KS_ERR_NOMANAGER:
		ov_string_print(&response->header, "HTTP/%s %s%s", response->httpVersion, HTTP_406_HEADER, tmp_header);
		if(request.response_format != KSX){
			ov_string_print(&response->contentString, "KS_ERR_NOMANAGER: %s%s", HTTP_406_BODY, tmp_body);
		}
		break;
	case KS_ERR_NOREMOTE:
		ov_string_print(&response->header, "HTTP/%s %s%s", response->httpVersion, HTTP_406_HEADER, tmp_header);
		if(request.response_format != KSX){
			ov_string_print(&response->contentString, "KS_ERR_NOREMOTE: %s%s", HTTP_406_BODY, tmp_body);
		}
		break;
	case KS_ERR_SERVERUNKNOWN:
		ov_string_print(&response->header, "HTTP/%s %s%s", response->httpVersion, HTTP_404_HEADER, tmp_header);
		if(request.response_format != KSX){
			ov_string_print(&response->contentString, "KS_ERR_SERVERUNKNOWN: %s%s", HTTP_404_BODY, tmp_body);
		}
		break;
	case KS_ERR_TARGETGENERIC:
		ov_string_print(&response->header, "HTTP/%s %s%s", response->httpVersion, HTTP_413_HEADER, tmp_header);
		if(request.response_format != KSX){
			ov_string_print(&response->contentString, "KS_ERR_TARGETGENERIC: %s%s", HTTP_413_BODY, tmp_body);
		}
		break;
	default:
		ov_string_print(&response->header, "HTTP/%s %s%s", response->httpVersion, HTTP_503_HEADER, tmp_header);
		if(request.response_format != KSX){
			ov_string_print(&response->contentString, "KS_ERR (unknown): %s%s", HTTP_503_BODY, tmp_body);
		}
		break;
	}
}

/**
 * This function handles requests received over a channel. It reads data as an http-stream and triggers the appropriate OV-functions
 */
OV_DLLFNCEXPORT OV_RESULT kshttp_httpclienthandler_HandleRequest(
	OV_INSTPTR_ksbase_ClientHandler baseClientHandler,
	KS_DATAPACKET* dataReceived,
	KS_DATAPACKET* answer
) {
	OV_INSTPTR_kshttp_httpclienthandler this = Ov_StaticPtrCast(kshttp_httpclienthandler, baseClientHandler);
	OV_RESULT result = OV_ERR_OK;

	switch (this->v_CommunicationStatus) {
		case KSHTTP_CS_INITIAL:
			result = kshttp_httpclienthandler_analyzeRequestHeader(this, dataReceived);
			if(this->v_CommunicationStatus != KSHTTP_CS_REQUESTHEADERPARSED){
				//wait for more data
				return OV_ERR_OK;
			}
			if(Ov_Fail(result)) return result;
			//no break wanted
		case KSHTTP_CS_REQUESTHEADERPARSED :
			result = kshttp_httpclienthandler_analyzeRequestBody(this, dataReceived);
			if(this->v_CommunicationStatus != KSHTTP_CS_REQUESTPARSED){
				//wait for more data
				return OV_ERR_OK;
			}
			if(Ov_Fail(result)) return result;
			//no break wanted
		case KSHTTP_CS_REQUESTPARSED :
			result = kshttp_httpclienthandler_generateHttpBody(this);
			if(this->v_CommunicationStatus == KSHTTP_CS_CHANNELRESPONSIBILITYRELEASED){
				//we reject every further work on this channel
				return OV_ERR_OK;
			}
			//no break wanted
		case KSHTTP_CS_RESPONSEBODYGENERATED :
			result = kshttp_httpclienthandler_generateHttpHeader(this, result, NULL);
			if(Ov_Fail(result)) return result;
			//no break wanted
		case KSHTTP_CS_RESPONSEHEADERGENERATED :
			result = kshttp_httpclienthandler_sendHttpHeader(this, answer);
			if(Ov_Fail(result)) return result;
			//no break wanted
		case KSHTTP_CS_RESPONSEHEADERSEND :
			result = kshttp_httpclienthandler_sendHttpBody(this, answer);
			return result;

		case KSHTTP_CS_CHANNELRESPONSIBILITYRELEASED :
			KS_logfile_error(("%s: HandleRequest: we have no responsibility for this channel, but got called! 8-(", this->v_identifier));
			return OV_ERR_NOTIMPLEMENTED;
		case KSHTTP_CS_SHUTDOWN :
			Ov_DeleteObject(this);
			return OV_ERR_OK;
		default:
			KS_logfile_error(("%s: HandleRequest: no matching communication status found: %d. aborting.", this->v_identifier, this->v_CommunicationStatus));
			break;
	}
	return result;
}
DLLFNCEXPORT OV_RESULT kshttp_httpclienthandler_analyzeRequestHeader(
	OV_INSTPTR_kshttp_httpclienthandler this,
	KS_DATAPACKET* dataReceived
) {
	OV_RESULT result = OV_ERR_OK;
	OV_BYTE *endOfHeader = NULL;

	//initialise struct
	this->v_ClientRequest.response_format = FORMATUNDEFINED;
	this->v_ClientRequest.compressionGzip = FALSE;

	ov_string_setvalue(&this->v_ServerResponse.contentString, NULL);
	Ov_HeapFree(this->v_ServerResponse.contentBinary);
	this->v_ServerResponse.contentBinary = NULL;
	this->v_ServerResponse.contentLength = 0;
	ov_string_setvalue(&this->v_ServerResponse.contentType, NULL);
	ov_string_setvalue(&this->v_ServerResponse.contentEncoding, NULL);
	this->v_ServerResponse.requestHandledBy = KSHTTP_RGB_NONE;
	this->v_CommunicationStatus = KSHTTP_CS_INITIAL;

	//if no stream mode, wait for the end of the header
	if(this->v_stream == FALSE){
		//split header and footer of the http request
		endOfHeader = kshttp_strnstr(dataReceived->readPT, "\r\n\r\n", dataReceived->length);
		if(!endOfHeader){
			//if no double line break detected yet - wait till next cycle
			return OV_ERR_OK;		/*	get called again to process the request next time (if it is complete then).
											Yes, this could block the ClientHandler for a longer time.	*/
		}

		//terminate after header for ov_string_split
		*endOfHeader = '\0';

		//header plus both line breaks
		this->v_ClientRequest.headerLength = endOfHeader - dataReceived->data + 4;

		ov_string_setvalue(&this->v_ClientRequest.requestHeader, (OV_STRING)dataReceived->data);
		//save pointer to the content
		dataReceived->readPT = dataReceived->readPT + this->v_ClientRequest.headerLength;
	}else{
		ov_string_setvalue(&this->v_ClientRequest.requestHeader, this->v_streamrequestheader);
	}

	//parse request header into get command and arguments request
	result = kshttp_parse_http_header_from_client(&this->v_ClientRequest, &this->v_ServerResponse);
	if(Ov_Fail(result)){
		this->v_CommunicationStatus = KSHTTP_CS_SHUTDOWN;
		return OV_ERR_BADPARAM;
	}
	this->v_CommunicationStatus = KSHTTP_CS_REQUESTHEADERPARSED;
	return OV_ERR_OK;
}
DLLFNCEXPORT OV_RESULT kshttp_httpclienthandler_analyzeRequestBody(
	OV_INSTPTR_kshttp_httpclienthandler this,
	KS_DATAPACKET* dataReceived
) {
	if(this->v_ClientRequest.contentLength != 0){
		//save message body (for POST?) in memory
		if(dataReceived->length - this->v_ClientRequest.headerLength >= this->v_ClientRequest.contentLength){
			this->v_ClientRequest.messageBody = (OV_BYTE*)Ov_HeapMalloc(this->v_ClientRequest.contentLength+1);
			if(!this->v_ClientRequest.messageBody){
				this->v_CommunicationStatus = KSHTTP_CS_SHUTDOWN;
				return OV_ERR_TARGETGENERIC;
			}

			memcpy(this->v_ClientRequest.messageBody, dataReceived->data + this->v_ClientRequest.headerLength, this->v_ClientRequest.contentLength);
			this->v_ClientRequest.messageBody[this->v_ClientRequest.contentLength] = '\0';
		}else{
			//we have the full header, but not the full body yet. Get called again next time
			return OV_ERR_OK;
		}
	}
	this->v_CommunicationStatus = KSHTTP_CS_REQUESTPARSED;

	//empty the buffers
	ksbase_free_KSDATAPACKET(dataReceived);

	return OV_ERR_OK;
}

DLLFNCEXPORT OV_RESULT kshttp_httpclienthandler_generateHttpBody(
	OV_INSTPTR_kshttp_httpclienthandler this
) {
	OV_INSTPTR_ksbase_Channel pChannel = Ov_GetParent(ksbase_AssocChannelClientHandler, this);
	OV_RESULT result = OV_ERR_OK;
	OV_INSTPTR_kshttp_httpClientHandlerExtension pExtension = NULL;
	OV_VTBLPTR_kshttp_httpClientHandlerExtension pVtblClientHandlerExtension = NULL;
	OV_INSTPTR_ov_class pExtensionClass = NULL;
	OV_BOOL extclientfound = FALSE;
	OV_UINT iterator;
	OV_STRING_VEC match = {0,NULL};

	OV_INSTPTR_kshttp_staticfile pStaticfile;
	//gzip compression business
	OV_BYTE* gzip_compressed_body = NULL;
	OV_INT gzip_compressed_body_length = 0;


	this->v_ServerResponse.requestHandledBy = KSHTTP_RGB_NONE;

	if(KSX == this->v_ClientRequest.response_format){
		ov_string_setvalue(&this->v_ServerResponse.contentType, "text/xml");
	}else if(JSON == this->v_ClientRequest.response_format){
		ov_string_setvalue(&this->v_ServerResponse.contentType, "application/json");
	}else{
		ov_string_setvalue(&this->v_ServerResponse.contentType, "text/plain");
	}
	ov_string_setvalue(&this->v_ServerResponse.contentEncoding, "windows-1252");

	//allow javascript connection from any source (CORS)
	ov_string_setvalue(&this->v_ServerResponse.header, "Access-Control-Allow-Origin:*\r\n");

	if(Ov_OK(result) && ov_string_compare(this->v_ClientRequest.requestMethod, "OPTIONS") == OV_STRCMP_EQUAL){
		//used for Cross-Origin Resource Sharing (CORS)

		//hmi uses this headers, which is no problem for us
		ov_string_append(&this->v_ServerResponse.header, "Access-Control-Allow-Headers: if-modified-since\r\nAccess-Control-Max-Age: 60\r\n");
		//only an 200 is required, so abort request handling
		this->v_ServerResponse.requestHandledBy = KSHTTP_RGB_CORSOPTION;
	}

	//BEGIN command routine
	if(Ov_OK(result) && this->v_ServerResponse.requestHandledBy == KSHTTP_RGB_NONE){
		if(ov_string_compare(this->v_ClientRequest.urlPath, "/getServer") == OV_STRCMP_EQUAL){
			//http GET
			kshttp_printresponseheader(&this->v_ServerResponse.contentString, this->v_ClientRequest.response_format, "getserver");

			//todo rebuild the other services to use the same function prototype
			result = kshttp_exec_getserver(this->v_ClientRequest, &this->v_ServerResponse);
			kshttp_printresponsefooter(&this->v_ServerResponse.contentString, this->v_ClientRequest.response_format, "getserver");
			this->v_ServerResponse.requestHandledBy = KSHTTP_RGB_GETSERVER;
		}else if(ov_string_compare(this->v_ClientRequest.urlPath, "/register") == OV_STRCMP_EQUAL){
			kshttp_printresponseheader(&this->v_ServerResponse.contentString, this->v_ClientRequest.response_format, "register");
			if(!pChannel->v_isLocal){
				result = KS_ERR_NOREMOTE;
				kshttp_print_result_array(&this->v_ServerResponse.contentString, this->v_ClientRequest.response_format, &result, 1, ": Registering remote is not allowed/useful");
			}else{
				//name, port, ksversion
				result = kshttp_exec_register(&this->v_ClientRequest.urlQuery, &this->v_ServerResponse.contentString, this->v_ClientRequest.response_format);
			}
			kshttp_printresponsefooter(&this->v_ServerResponse.contentString, this->v_ClientRequest.response_format, "register");
			this->v_ServerResponse.requestHandledBy = KSHTTP_RGB_REGISTER;
		}else if(ov_string_compare(this->v_ClientRequest.urlPath, "/unregister") == OV_STRCMP_EQUAL){
			//name, port, ksversion
			kshttp_printresponseheader(&this->v_ServerResponse.contentString, this->v_ClientRequest.response_format, "unregister");
			if(!pChannel->v_isLocal){
				result = KS_ERR_NOREMOTE;
				kshttp_print_result_array(&this->v_ServerResponse.contentString, this->v_ClientRequest.response_format, &result, 1, ": Registering remote is not allowed/useful");
			}else{
				result = kshttp_exec_unregister(&this->v_ClientRequest.urlQuery, &this->v_ServerResponse.contentString, this->v_ClientRequest.response_format);
			}
			kshttp_printresponsefooter(&this->v_ServerResponse.contentString, this->v_ClientRequest.response_format, "unregister");
			this->v_ServerResponse.requestHandledBy = KSHTTP_RGB_UNREGISTER;
		}else if(ov_string_compare(this->v_ClientRequest.urlPath, "/getVar") == OV_STRCMP_EQUAL){
			//http GET
			kshttp_printresponseheader(&this->v_ServerResponse.contentString, this->v_ClientRequest.response_format, "getvar");
			result = kshttp_exec_getvar(&this->v_ClientRequest.urlQuery, &this->v_ServerResponse.contentString, this->v_ClientRequest.response_format);
			kshttp_printresponsefooter(&this->v_ServerResponse.contentString, this->v_ClientRequest.response_format, "getvar");
			//stream required?
			kshttp_find_arguments(&this->v_ClientRequest.urlQuery, "stream", &match);
			if(match.veclen>0){
				result = OV_ERR_NOTIMPLEMENTED;
				this->v_ServerResponse.requestHandledBy = KSHTTP_RGB_GETVAR;
			}else if(FALSE){
				//disabled, because we are not called cyclic

				//yes
				ov_string_setvalue(&this->v_ServerResponse.contentType, "text/event-stream");
				//first time?
				if(!this->v_stream){
					//backup the header
					ov_string_setvalue(&this->v_streamrequestheader, this->v_ClientRequest.requestHeader);
				}
				//is cache updated?
				if(ov_string_compare(this->v_ServerResponse.contentString, this->v_streambuffer) != OV_STRCMP_EQUAL){
					//ov_logfile_debug("upd %s %s", body, this->v_streambuffer);
					//yes
					ov_string_setvalue(&this->v_streambuffer, this->v_ServerResponse.contentString);
					ov_string_print(&this->v_ServerResponse.contentString, "data: %s\r\n\r\n", this->v_streambuffer);
				}else{
					//no - set body to null
					ov_string_setvalue(&this->v_ServerResponse.contentString, NULL);
				}
				this->v_ServerResponse.requestHandledBy = KSHTTP_RGB_GETVARSTREAM;
				//deactivate compression if push is used
				this->v_ClientRequest.compressionGzip = FALSE;
			}else{
				//no
				this->v_ServerResponse.requestHandledBy = KSHTTP_RGB_GETVAR;
			}
		}else if(ov_string_compare(this->v_ClientRequest.urlPath, "/setVar") == OV_STRCMP_EQUAL){
			//http PUT, used in OData or PROPPATCH, used in WebDAV
			//add Access-Control-Allow-Methods to OPTIONS if implement other Methods
			kshttp_printresponseheader(&this->v_ServerResponse.contentString, this->v_ClientRequest.response_format, "setvar");
			result = kshttp_exec_setvar(&this->v_ClientRequest.urlQuery, &this->v_ServerResponse.contentString, this->v_ClientRequest.response_format);
			kshttp_printresponsefooter(&this->v_ServerResponse.contentString, this->v_ClientRequest.response_format, "setvar");
			this->v_ServerResponse.requestHandledBy = KSHTTP_RGB_SETVAR;
		}else if(ov_string_compare(this->v_ClientRequest.urlPath, "/getEP") == OV_STRCMP_EQUAL){
			//http PROPFIND, used in WebDAV
			kshttp_printresponseheader(&this->v_ServerResponse.contentString, this->v_ClientRequest.response_format, "getep");
			result = kshttp_exec_getep(&this->v_ClientRequest.urlQuery, &this->v_ServerResponse.contentString, this->v_ClientRequest.response_format);
			kshttp_printresponsefooter(&this->v_ServerResponse.contentString, this->v_ClientRequest.response_format, "getep");
			this->v_ServerResponse.requestHandledBy = KSHTTP_RGB_GETEP;
		}else if(ov_string_compare(this->v_ClientRequest.urlPath, "/createObject") == OV_STRCMP_EQUAL){
			//http PUT, used in WebDAV
			kshttp_printresponseheader(&this->v_ServerResponse.contentString, this->v_ClientRequest.response_format, "createobject");
			result = kshttp_exec_createObject(&this->v_ClientRequest.urlQuery, &this->v_ServerResponse.contentString, this->v_ClientRequest.response_format);
			kshttp_printresponsefooter(&this->v_ServerResponse.contentString, this->v_ClientRequest.response_format, "createobject");
			this->v_ServerResponse.requestHandledBy = KSHTTP_RGB_CREATEOBJECT;
		}else if(ov_string_compare(this->v_ClientRequest.urlPath, "/deleteObject") == OV_STRCMP_EQUAL){
			//http DELETE, used in WebDAV
			kshttp_printresponseheader(&this->v_ServerResponse.contentString, this->v_ClientRequest.response_format, "deleteobject");
			result = kshttp_exec_deleteObject(&this->v_ClientRequest.urlQuery, &this->v_ServerResponse.contentString, this->v_ClientRequest.response_format);
			kshttp_printresponsefooter(&this->v_ServerResponse.contentString, this->v_ClientRequest.response_format, "deleteobject");
			this->v_ServerResponse.requestHandledBy = KSHTTP_RGB_DELETEOBJECT;
		}else if(ov_string_compare(this->v_ClientRequest.urlPath, "/renameObject") == OV_STRCMP_EQUAL){
			//http MOVE, used in WebDAV
			kshttp_printresponseheader(&this->v_ServerResponse.contentString, this->v_ClientRequest.response_format, "renameobject");
			result = kshttp_exec_renameObject(&this->v_ClientRequest.urlQuery, &this->v_ServerResponse.contentString, this->v_ClientRequest.response_format);
			kshttp_printresponsefooter(&this->v_ServerResponse.contentString, this->v_ClientRequest.response_format, "renameobject");
			this->v_ServerResponse.requestHandledBy = KSHTTP_RGB_RENAMEOBJECT;
		}else if(ov_string_compare(this->v_ClientRequest.urlPath, "/link") == OV_STRCMP_EQUAL){
			//http LINK
			kshttp_printresponseheader(&this->v_ServerResponse.contentString, this->v_ClientRequest.response_format, "link");
			result = kshttp_exec_link(&this->v_ClientRequest.urlQuery, &this->v_ServerResponse.contentString, this->v_ClientRequest.response_format);
			kshttp_printresponsefooter(&this->v_ServerResponse.contentString, this->v_ClientRequest.response_format, "link");
			this->v_ServerResponse.requestHandledBy = KSHTTP_RGB_LINK;
		}else if(ov_string_compare(this->v_ClientRequest.urlPath, "/unlink") == OV_STRCMP_EQUAL){
			//http UNLINK
			kshttp_printresponseheader(&this->v_ServerResponse.contentString, this->v_ClientRequest.response_format, "unlink");
			result = kshttp_exec_unlink(&this->v_ClientRequest.urlQuery, &this->v_ServerResponse.contentString, this->v_ClientRequest.response_format);
			kshttp_printresponsefooter(&this->v_ServerResponse.contentString, this->v_ClientRequest.response_format, "unlink");
			this->v_ServerResponse.requestHandledBy = KSHTTP_RGB_UNLINK;
		}else if(ov_string_compare(this->v_ClientRequest.urlPath, "/getLogfile") == OV_STRCMP_EQUAL){
			kshttp_printresponseheader(&this->v_ServerResponse.contentString, this->v_ClientRequest.response_format, "logfile");
			result = kshttp_exec_getlogfile(&this->v_ClientRequest.urlQuery, &this->v_ServerResponse.contentString, this->v_ClientRequest.response_format);
			kshttp_printresponsefooter(&this->v_ServerResponse.contentString, this->v_ClientRequest.response_format, "logfile");
			this->v_ServerResponse.requestHandledBy = KSHTTP_RGB_GETLOGFILE;
		}else if(ov_string_compare(this->v_ClientRequest.urlPath, "/auth") == OV_STRCMP_EQUAL){
			result = kshttp_authorize(1, this, this->v_ClientRequest.requestHeader, &this->v_ServerResponse.header, this->v_ClientRequest.requestMethod, this->v_ClientRequest.urlPath);
			if(!Ov_Fail(result)){
				ov_string_append(&this->v_ServerResponse.contentString, "Secret area");
				result = OV_ERR_OK;
			}
			this->v_ServerResponse.requestHandledBy = KSHTTP_RGB_HTTPAUTH;
		}else{
			/*	iterate over all httpClientHandlerExtension	*/
			Ov_ForEachChildEx(ov_inheritance, pclass_kshttp_httpClientHandlerExtension, pExtensionClass, ov_class){
				Ov_ForEachChildEx(ov_instantiation, pExtensionClass, pExtension, kshttp_httpClientHandlerExtension){
					for(iterator = 0; iterator < pExtension->v_commandList.veclen; iterator++){
						if(ov_string_compare(this->v_ClientRequest.urlPath, pExtension->v_commandList.value[iterator]) == OV_STRCMP_EQUAL){
							extclientfound = TRUE;
							break;
						}
					}
					if(extclientfound == TRUE){
						break;
					}
				}
				if(extclientfound == TRUE){
					break;
				}
			}
			if(extclientfound == TRUE){
				KS_logfile_debug(("%s: HandleRequest: processing extension (%s) by protocolExtension %s", this->v_identifier, this->v_ClientRequest.urlPath, pExtension->v_identifier));
				Ov_GetVTablePtr(kshttp_httpClientHandlerExtension, pVtblClientHandlerExtension, pExtension);
				if(!pVtblClientHandlerExtension){
					KS_logfile_error(("%s: HandleRequest: vtable pointer of %s not found. aborting. %s", this->v_identifier, pExtension->v_identifier));
				}else{
					result = pVtblClientHandlerExtension->m_HandleExtendedRequest(
							pExtension,
							this,
							this->v_ClientRequest,
							&this->v_ServerResponse);
					if(Ov_Fail(result)){
						ov_memstack_lock();
						KS_logfile_error(("%s: HandleRequest: processing service %s by %s failed with error %s", this->v_identifier, this->v_ClientRequest.urlPath, pExtension->v_identifier, ov_result_getresulttext(result)));
						ov_memstack_unlock();
					}else{
						this->v_ServerResponse.requestHandledBy = KSHTTP_RGB_CLIENTHANDLEREXT;
						if(this->v_CommunicationStatus == KSHTTP_CS_CHANNELRESPONSIBILITYRELEASED){
							//we reject every further work on this channel
							return OV_ERR_OK;
						}
					}
				}
			} else {
				//perhaps we have more luck with an static file
			}
		}
	}
	//END command routine

	//BEGIN static file routine
	//no command matched yet... Is it a static file?
	if(!Ov_Fail(result) && this->v_ServerResponse.requestHandledBy == KSHTTP_RGB_NONE){
		OV_STRING filename = NULL;
		OV_STRING filepath = NULL;
		//assume index.html as a root file
		if(ov_string_compare("/", this->v_ClientRequest.urlPath) == OV_STRCMP_EQUAL){
			filename = "index.html"; //memory from the stack of this function
		}else if(this->v_ClientRequest.urlPath[ov_string_getlength(this->v_ClientRequest.urlPath)-1] == '/'){
			ov_string_append(&this->v_ClientRequest.urlPath, "index.html");
			//remove leading /
			filename = this->v_ClientRequest.urlPath + 1;
		}else{
			//remove leading /
			filename = this->v_ClientRequest.urlPath + 1;
		}

		ov_memstack_lock();
		//a dot in a filename is represented via a percent notation in an identifier, so we need
		//to change the parameter. A directory should be possible, so we need to skip / in conversion
		ov_string_print(&filepath, "/data/kshttp/%s", kshttp_ov_path_topercent_noslash(filename));
		ov_memstack_unlock();
		pStaticfile = Ov_DynamicPtrCast(kshttp_staticfile, ov_path_getobjectpointer(filepath, 2));
		ov_string_setvalue(&filepath, NULL);
		filename = NULL;	//usage is save here, as we had no memory in the database or heap

		if(pStaticfile != NULL){
			ov_string_setvalue(&this->v_ServerResponse.contentType, pStaticfile->v_mimetype);
			ov_string_setvalue(&this->v_ServerResponse.contentEncoding, pStaticfile->v_encoding);
			result = OV_ERR_OK;
			//we do not copy the value here! The string pointer is NULL right now
			this->v_ServerResponse.contentString = pStaticfile->v_content;
			this->v_ServerResponse.requestHandledBy = KSHTTP_RGB_STATICFILE;
		}
	}
	//END static file routine

	//raw request header not needed any longer
	ov_string_setvalue(&this->v_ClientRequest.requestHeader, NULL);
	Ov_SetDynamicVectorLength(&this->v_ClientRequest.urlQuery,0,STRING);
	Ov_SetDynamicVectorLength(&match,0,STRING);
	ov_string_setvalue(&this->v_ClientRequest.urlPath, NULL);

	//no method has found a hit
	if (this->v_ServerResponse.requestHandledBy == KSHTTP_RGB_NONE){
		result = OV_ERR_BADPATH; //404
		kshttp_printresponseheader(&this->v_ServerResponse.contentString, this->v_ClientRequest.response_format, "notimplemented");
		kshttp_print_result_array(&this->v_ServerResponse.contentString, this->v_ClientRequest.response_format, &result, 1, ": KS command not supported or static file not found");
		kshttp_printresponsefooter(&this->v_ServerResponse.contentString, this->v_ClientRequest.response_format, "notimplemented");
	}

	//in case of a HEAD request there is no need to send the body
	if(ov_string_compare(this->v_ClientRequest.requestMethod, "HEAD") == OV_STRCMP_EQUAL){
		this->v_ServerResponse.contentLength = 0;
	}else if(this->v_ServerResponse.contentString != NULL){
		//we have a content as string, so check its length and move content to binary pointer
		this->v_ServerResponse.contentLength = ov_string_getlength(this->v_ServerResponse.contentString);
		this->v_ServerResponse.contentBinary = (OV_BYTE*)this->v_ServerResponse.contentString;
		this->v_ServerResponse.contentString = NULL;
	}else if(this->v_ServerResponse.contentBinary != NULL){
		//contentLength is already set
	}else{
		//we have nothing to send available...
		this->v_ServerResponse.contentLength = 0;
	}


#ifndef KSHTTP_DISABLE_GZIP
	// check if the body length corresponds for compression
	if (Ov_OK(result) && this->v_ServerResponse.contentLength >= MINIMAL_LENGTH_FOR_GZIP && this->v_ClientRequest.compressionGzip == TRUE &&
												  (ov_string_match(this->v_ServerResponse.contentType, "text/*") == TRUE
												|| ov_string_compare(this->v_ServerResponse.contentType, "application/javascript") == OV_STRCMP_EQUAL
												|| ov_string_match(this->v_ServerResponse.contentType, "application/xml") == TRUE
												|| ov_string_match(this->v_ServerResponse.contentType, "application/xml-dtd") == TRUE
												|| ov_string_match(this->v_ServerResponse.contentType, "*+xml") == TRUE 	//includes image/svg+xml
												|| ov_string_compare(this->v_ServerResponse.contentType, "application/json") == OV_STRCMP_EQUAL))
	{
		// The body is compressed by using gzip function in gzip.h
		result = gzip(this->v_ServerResponse.contentBinary, this->v_ServerResponse.contentLength, &gzip_compressed_body, &gzip_compressed_body_length);
		if(Ov_OK(result)){
			KS_logfile_debug(("Compression ratio (content only): %" OV_PRINT_SINGLE, (OV_SINGLE)(gzip_compressed_body_length/this->v_ServerResponse.contentLength)));
			//free old content
			if(this->v_ServerResponse.requestHandledBy != KSHTTP_RGB_STATICFILE){
				if(this->v_ServerResponse.contentBinary > pdb->pstart && this->v_ServerResponse.contentBinary < pdb->pend){
					//this was this->v_ServerResponse.contentString
					Ov_DbFree(this->v_ServerResponse.contentBinary);
				}else{
					Ov_HeapFree(this->v_ServerResponse.contentBinary);
				}
			}else{
				//this points to a string variable of a static file, so using is the right thing
			}
			//replace by gzipped content
			this->v_ServerResponse.contentBinary = gzip_compressed_body;
			this->v_ServerResponse.contentLength = gzip_compressed_body_length;
			gzip_compressed_body = NULL;
		}else{
			//prevent sending Content-Encoding Header
			this->v_ClientRequest.compressionGzip = FALSE;
			Ov_DbFree(gzip_compressed_body);
			gzip_compressed_body = NULL;
		}
	}else{
		//prevent sending Content-Encoding Header
		this->v_ClientRequest.compressionGzip = FALSE;
	}
#endif
	this->v_CommunicationStatus = KSHTTP_CS_RESPONSEBODYGENERATED;

	return result;
}

//BEGIN forming and sending the answer
DLLFNCEXPORT OV_RESULT kshttp_httpclienthandler_generateHttpHeader(
	OV_INSTPTR_kshttp_httpclienthandler this,
	OV_RESULT responseCreationResult,
	OV_STRING additionalHeaders
){
	//header already sent?
	if(this->v_ServerResponse.header == NULL){
		return OV_ERR_OK;
	}

	//adding encoding and content-type to the header
	if (ov_string_compare(this->v_ServerResponse.contentEncoding, "") == OV_STRCMP_EQUAL){
			ov_string_print(&this->v_ServerResponse.header, "%sContent-Type: %s\r\n", this->v_ServerResponse.header, this->v_ServerResponse.contentType);
		}else{
			ov_string_print(&this->v_ServerResponse.header, "%sContent-Type: %s; charset=%s\r\n", this->v_ServerResponse.header, this->v_ServerResponse.contentType, this->v_ServerResponse.contentEncoding);
	}

	//now we have to format the raw http answer
	kshttp_httpclienthandler_mapresult2http(this->v_ClientRequest, &this->v_ServerResponse, responseCreationResult);

	ov_string_setvalue(&this->v_ClientRequest.httpVersion, NULL);
	ov_string_setvalue(&this->v_ClientRequest.host, NULL);
	ov_string_setvalue(&this->v_ServerResponse.httpVersion, NULL);

	//Append common data to header:
	ov_string_print(&this->v_ServerResponse.header, "%sServer: ACPLT/OV HTTP Server %s (compiled %s %s)\r\n", this->v_ServerResponse.header, OV_LIBRARY_DEF_kshttp.version, __TIME__, __DATE__);
	//no-cache
	if(ov_string_compare(this->v_ServerResponse.httpVersion, "1.0") == OV_STRCMP_EQUAL){
		//Cache-Control is not defined in 1.0, so we misuse the Pragma header (as everyone)
		ov_string_print(&this->v_ServerResponse.header, "%sPragma: no-cache\r\n", this->v_ServerResponse.header);
	}else{
		ov_string_print(&this->v_ServerResponse.header, "%sExpires: 0\r\n", this->v_ServerResponse.header);
	}
	//HTTP1.1 says, we MUST send a Date: header if we have a clock. Do we have one? :)

	//handle keep_alives
	if (this->v_ClientRequest.keepAlive == TRUE) {
		ov_string_print(&this->v_ServerResponse.header, "%sConnection: keep-alive\r\n", this->v_ServerResponse.header);
	}else{
		ov_string_print(&this->v_ServerResponse.header, "%sConnection: close\r\n", this->v_ServerResponse.header);
	}

	//append content length
	ov_string_print(&this->v_ServerResponse.header, "%sContent-Length: %i\r\n", this->v_ServerResponse.header, this->v_ServerResponse.contentLength);

	//handle gzip encoding by attaching a line to the header if accepted
	if(this->v_ClientRequest.compressionGzip){
		ov_string_append(&this->v_ServerResponse.header, "Content-Encoding: gzip\r\n");
	}
	if(ov_string_compare(additionalHeaders, "") != OV_STRCMP_EQUAL){
		ov_string_append(&this->v_ServerResponse.header, additionalHeaders);
	}

	// and finalize the header
	ov_string_append(&this->v_ServerResponse.header, "\r\n");

	ov_string_setvalue(&this->v_ServerResponse.contentEncoding, NULL);
	ov_string_setvalue(&this->v_ServerResponse.contentType, NULL);

	this->v_CommunicationStatus = KSHTTP_CS_RESPONSEHEADERGENERATED;

	return OV_ERR_OK;
}

DLLFNCEXPORT OV_RESULT kshttp_httpclienthandler_sendHttpHeader(
	OV_INSTPTR_kshttp_httpclienthandler this,
	KS_DATAPACKET* answer
){
	if(this->v_ServerResponse.header == NULL){
		return OV_ERR_OK;
	}

	//send header only if not in stream mode
	if(this->v_ServerResponse.requestHandledBy != KSHTTP_RGB_GETVARSTREAM){
		KS_logfile_debug(("httpclienthandler: sending header: %" OV_PRINT_INT " bytes", ov_string_getlength(this->v_ServerResponse.header)));
		ksbase_KSDATAPACKET_append(answer, (OV_BYTE*)this->v_ServerResponse.header, ov_string_getlength(this->v_ServerResponse.header));
		//note: this does not send the request
		ov_string_setvalue(&this->v_ServerResponse.header, NULL);
	}

	this->v_CommunicationStatus = KSHTTP_CS_RESPONSEHEADERSEND;
	return OV_ERR_OK;
}

DLLFNCEXPORT OV_RESULT kshttp_httpclienthandler_sendHttpBody(
	OV_INSTPTR_kshttp_httpclienthandler this,
	KS_DATAPACKET* answer
){
	OV_INSTPTR_ksbase_Channel pChannel = Ov_GetParent(ksbase_AssocChannelClientHandler, this);

	//are we starting a stream?
	if(this->v_stream == FALSE && this->v_ServerResponse.requestHandledBy == KSHTTP_RGB_GETVARSTREAM){
		this->v_stream = TRUE;
		//speed the processing time down to 5ms
		this->v_cycInterval = 5;
	}

	//in case of a HEAD request there is no need to send the body
	if(this->v_ServerResponse.contentLength > 0){
		KS_logfile_debug(("httpclienthandler: sending body: %" OV_PRINT_INT " bytes", this->v_ServerResponse.contentLength));
		//this does not send the request
		ksbase_KSDATAPACKET_append(answer, this->v_ServerResponse.contentBinary, this->v_ServerResponse.contentLength);
	}

	Ov_HeapFree(this->v_ClientRequest.messageBody);
	this->v_ClientRequest.messageBody = NULL;
	this->v_CommunicationStatus = KSHTTP_CS_RESPONSEBODYSEND;

	//if a static file is returned body is pointing inside the database
	if(this->v_ServerResponse.requestHandledBy != KSHTTP_RGB_STATICFILE || this->v_ClientRequest.compressionGzip){
		if(this->v_ServerResponse.contentBinary > pdb->pstart && this->v_ServerResponse.contentBinary < pdb->pend){
			//this was in the database as for example this->v_ServerResponse.contentString
			Ov_DbFree(this->v_ServerResponse.contentBinary);
		}else{
			Ov_HeapFree(this->v_ServerResponse.contentBinary);
		}
	}else{
		//this points to a string variable of a static file, so setting NULL is the right thing
	}
	this->v_ServerResponse.contentBinary = NULL;


	//shutdown tcp connection if no keep_alive was set
	if (this->v_ClientRequest.keepAlive == TRUE) {
		pChannel->v_CloseAfterSend = TRUE;
		this->v_CommunicationStatus = KSHTTP_CS_INITIAL;
	}else{
		pChannel->v_CloseAfterSend = FALSE;
		this->v_CommunicationStatus = KSHTTP_CS_SHUTDOWN;
	}
	return OV_ERR_OK;
}


OV_DLLFNCEXPORT void kshttp_httpclienthandler_startup(
	OV_INSTPTR_ov_object 	pobj
) {
	/*
	*   local variables
	*/
	OV_INSTPTR_kshttp_httpclienthandler thisCl = Ov_StaticPtrCast(kshttp_httpclienthandler, pobj);

	/* do what the base class does first */
	ksbase_ClientHandler_startup(pobj);

	thisCl->v_CommunicationStatus = KSHTTP_CS_INITIAL;

	// initial strings
	thisCl->v_ClientRequest.urlQuery.veclen = 0;
	thisCl->v_ClientRequest.urlQuery.value = NULL;
	thisCl->v_ClientRequest.urlPath = NULL;
	thisCl->v_ClientRequest.host = NULL;
	thisCl->v_ClientRequest.requestHeader = NULL;
	thisCl->v_ClientRequest.requestMethod = NULL;
	thisCl->v_ClientRequest.httpVersion = NULL;
	thisCl->v_ClientRequest.Accept = NULL;

	thisCl->v_ClientRequest.contentLength = 0;
	thisCl->v_ClientRequest.compressionGzip = FALSE;

	thisCl->v_ClientRequest.response_format = FORMATUNDEFINED;

	thisCl->v_ServerResponse.httpVersion = NULL;
	thisCl->v_ServerResponse.statusCode = 0;
	thisCl->v_ServerResponse.headerLength = 0;
	thisCl->v_ServerResponse.header = NULL;
	thisCl->v_ServerResponse.contentLength = 0;
	thisCl->v_ServerResponse.contentString = NULL;
	thisCl->v_ServerResponse.contentBinary = NULL;
	thisCl->v_ServerResponse.contentType = NULL;
	thisCl->v_ServerResponse.contentEncoding = NULL;
	thisCl->v_ServerResponse.transferEncodingChunked = FALSE;

	return;
}

OV_DLLFNCEXPORT void kshttp_httpclienthandler_shutdown(
	OV_INSTPTR_ov_object 	pobj
) {
	/*
	*   local variables
	*/
	OV_INSTPTR_kshttp_httpclienthandler thisCl = Ov_StaticPtrCast(kshttp_httpclienthandler, pobj);

	thisCl->v_CommunicationStatus = KSHTTP_CS_SHUTDOWN;

	//free memory
	Ov_SetDynamicVectorLength(&thisCl->v_ClientRequest.urlQuery, 0, STRING);
	ov_string_setvalue(&thisCl->v_ClientRequest.urlPath, NULL);
	ov_string_setvalue(&thisCl->v_ClientRequest.host, NULL);
	ov_string_setvalue(&thisCl->v_ClientRequest.requestHeader, NULL);
	ov_string_setvalue(&thisCl->v_ClientRequest.requestMethod, NULL);
	ov_string_setvalue(&thisCl->v_ClientRequest.httpVersion, NULL);
	ov_string_setvalue(&thisCl->v_ClientRequest.Accept, NULL);

	ov_string_setvalue(&thisCl->v_ServerResponse.header, NULL);
	ov_string_setvalue(&thisCl->v_ServerResponse.httpVersion, NULL);
	ov_string_setvalue(&thisCl->v_ServerResponse.contentString, NULL);
	Ov_HeapFree(thisCl->v_ServerResponse.contentBinary);
	thisCl->v_ServerResponse.contentLength = 0;
	ov_string_setvalue(&thisCl->v_ServerResponse.contentType, NULL);

	/* set the object's state to "shut down" */
	ksbase_ClientHandler_shutdown(pobj);

	return;
}


