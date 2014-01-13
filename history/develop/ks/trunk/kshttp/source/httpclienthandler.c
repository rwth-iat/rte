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


#include <time.h>

#include "config.h"
#include "kshttp.h"
#ifndef KSHTTP_DISABLE_GZIP
	#include "gzip.h"
#endif
#include "ksbase_helper.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

//TCP sockets
#if !OV_SYSTEM_NT
	#include <arpa/inet.h>
	#include <sys/socket.h>
	#include <sys/ioctl.h>
	#include <netinet/in.h>
	#include <unistd.h>
	#include <fcntl.h>
#else
	#include <winsock2.h>
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
/** TEMP **/


/**
 * Builds header for http communication from OV_RESULT
 * @param result
 * @param http_version
 * @param header will be overwritten
 * @param body will be overwritten in an error state
 */
static void map_result_to_http(OV_RESULT* result, OV_STRING* http_version, OV_STRING* header, OV_STRING* body, KSHTTP_RESPONSEFORMAT response_format){
	OV_STRING tmp_header = NULL;
	OV_STRING tmp_body = NULL;
	//this is very ugly, but better than code duplication
	//the problem is that ov_string_print does not support NULL pointers as argument
	if(*header == NULL){
		tmp_header = "";
	}else{
		tmp_header = *header;
	}
	if(*body == NULL){
		tmp_body = "";
	}else{
		tmp_body = *body;
	}

	//no free needed since no memory allocated
	switch (*result)
	{
	//still some duplication, but good enough for now
	case OV_ERR_OK:
		ov_string_print(header, "HTTP/%s %s%s", *http_version, HTTP_200_HEADER, tmp_header);
		break;
	case OV_ERR_BADNAME:
		ov_string_print(header, "HTTP/%s %s%s", *http_version, HTTP_400_HEADER, tmp_header);
		if(response_format != KSX){
			ov_string_print(body, "KS_ERR_BADNAME: %s%s", HTTP_400_BODY, tmp_body);
		}
		break;
	case OV_ERR_BADPARAM:
		ov_string_print(header, "HTTP/%s %s%s", *http_version, HTTP_400_HEADER, tmp_header);
		if(response_format != KSX){
			ov_string_print(body, "KS_ERR_BADPARAM: %s%s", HTTP_400_BODY, tmp_body);
		}
		break;
	case OV_ERR_BADAUTH:
		ov_string_print(header, "HTTP/%s %s%s", *http_version, HTTP_401_HEADER, tmp_header);
		if(response_format != KSX){
			ov_string_print(body, "KS_ERR_BADAUTH: %s%s", HTTP_401_BODY, tmp_body);
		}
		break;
	case OV_ERR_NOACCESS:
		ov_string_print(header, "HTTP/%s %s%s", *http_version, HTTP_403_HEADER, tmp_header);
		if(response_format != KSX){
			ov_string_print(body, "KS_ERR_NOACCESS: %s%s", HTTP_403_BODY, tmp_body);
		}
		break;
	case OV_ERR_BADPATH:
		ov_string_print(header, "HTTP/%s %s%s", *http_version, HTTP_404_HEADER, tmp_header);
		if(response_format != KSX){
			ov_string_print(body, "KS_ERR_BADPATH: %s%s", HTTP_404_BODY, tmp_body);
		}
		break;
	case OV_ERR_BADVALUE:
		ov_string_print(header, "HTTP/%s %s%s", *http_version, HTTP_406_HEADER, tmp_header);
		if(response_format != KSX){
			ov_string_print(body, "KS_ERR_BADVALUE: %s%s", HTTP_406_BODY, tmp_body);
		}
		break;
	case OV_ERR_NOTIMPLEMENTED:
		ov_string_print(header, "HTTP/%s %s%s", *http_version, HTTP_501_HEADER, tmp_header);
		if(response_format != KSX){
			ov_string_print(body, "KS_ERR_NOTIMPLEMENTED: %s%s", HTTP_501_BODY, tmp_body);
		}
		break;
	case OV_ERR_GENERIC:
		ov_string_print(header, "HTTP/%s %s%s", *http_version, HTTP_503_HEADER, tmp_header);
		if(response_format != KSX){
			ov_string_print(body, "KS_ERR_GENERIC: %s%s", HTTP_503_BODY, tmp_body);
		}
		break;
	case OV_ERR_ALREADYEXISTS:
		ov_string_print(header, "HTTP/%s %s%s", *http_version, HTTP_409_HEADER, tmp_header);
		if(response_format != KSX){
			ov_string_print(body, "KS_ERR_ALREADYEXISTS: %s%s", HTTP_409_BODY, tmp_body);
		}
		break;
	case KS_ERR_NOMANAGER:
		ov_string_print(header, "HTTP/%s %s%s", *http_version, HTTP_406_HEADER, tmp_header);
		if(response_format != KSX){
			ov_string_print(body, "KS_ERR_NOMANAGER: %s%s", HTTP_406_BODY, tmp_body);
		}
		break;
	case KS_ERR_NOREMOTE:
		ov_string_print(header, "HTTP/%s %s%s", *http_version, HTTP_406_HEADER, tmp_header);
		if(response_format != KSX){
			ov_string_print(body, "KS_ERR_NOREMOTE: %s%s", HTTP_406_BODY, tmp_body);
		}
		break;
	case KS_ERR_SERVERUNKNOWN:
		ov_string_print(header, "HTTP/%s %s%s", *http_version, HTTP_404_HEADER, tmp_header);
		if(response_format != KSX){
			ov_string_print(body, "KS_ERR_SERVERUNKNOWN: %s%s", HTTP_404_BODY, tmp_body);
		}
		break;
	case KS_ERR_TARGETGENERIC:
		ov_string_print(header, "HTTP/%s %s%s", *http_version, HTTP_413_HEADER, tmp_header);
		if(response_format != KSX){
			ov_string_print(body, "KS_ERR_TARGETGENERIC: %s%s", HTTP_413_BODY, tmp_body);
		}
		break;
	default:
		ov_string_print(header, "HTTP/%s %s%s", *http_version, HTTP_503_HEADER, tmp_header);
		if(response_format != KSX){
			ov_string_print(body, "KS_ERR (unknown): %s%s", HTTP_503_BODY, tmp_body);
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
	OV_INSTPTR_ksbase_Channel pChannel = Ov_GetParent(ksbase_AssocChannelClientHandler, this);

	OV_BYTE *endOfHeader = NULL;

	OV_STRING responseHeader = NULL; //header of the reply
	OV_STRING responseBody = NULL; //reply *WITHOUT HEADER*
	OV_INT responseBodylength = 0; //length of the return body

	KSHTTP_REQUESTHANDLEDBY request_handled_by = NONE;
	OV_BOOL gzip_applicable = FALSE;

	OV_RESULT result = OV_ERR_OK;
	OV_STRING_VEC match = {0,NULL};
	OV_STRING reply_contenttype = NULL;
	OV_STRING reply_encoding = NULL;

	//gzip compression business
	OV_STRING gzip_compressed_body = NULL;
	OV_INT gzip_compressed_body_length = 0;

	OV_INSTPTR_kshttp_staticfile pStaticfile;

	KSHTTP_RESPONSEFORMAT response_format = FORMATUNDEFINED;
	this->v_ClientRequest.gzipAccepted = FALSE;

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
	result = kshttp_parse_http_header_from_client(&this->v_ClientRequest, &response_format);
	if(this->v_ClientRequest.contentLength != 0){
		//save message body (for POST?) in memory
		if(dataReceived->length - this->v_ClientRequest.headerLength >= this->v_ClientRequest.contentLength){
			this->v_ClientRequest.messageBody = (OV_BYTE*)Ov_HeapMalloc(this->v_ClientRequest.contentLength+1);
			if(!this->v_ClientRequest.messageBody){
				return OV_ERR_TARGETGENERIC;
			}

			memcpy(this->v_ClientRequest.messageBody, dataReceived->data + this->v_ClientRequest.headerLength, this->v_ClientRequest.contentLength);
			this->v_ClientRequest.messageBody[this->v_ClientRequest.contentLength] = '\0';
		}else{
			//we have the full header, but not the full body yet. Get called again next time
			return OV_ERR_OK;
		}
	}

	//empty the buffers
	ksbase_free_KSDATAPACKET(dataReceived);

	if(KSX == response_format){
		ov_string_setvalue(&reply_contenttype, "text/xml");
	}else if(JSON == response_format){
		ov_string_setvalue(&reply_contenttype, "application/json");
	}else{
		ov_string_setvalue(&reply_contenttype, "text/plain");
	}
	ov_string_setvalue(&reply_encoding, "Windows-1252");

	//allow javascript connection from any source (CORS)
	ov_string_setvalue(&responseHeader, "Access-Control-Allow-Origin:*\r\n");

	if(Ov_OK(result) && ov_string_compare(this->v_ClientRequest.requestMethod, "OPTIONS") == OV_STRCMP_EQUAL){
		//used for Cross-Origin Resource Sharing (CORS)

		//hmi uses this headers, which is no problem for us
		ov_string_append(&responseHeader, "Access-Control-Allow-Headers: if-modified-since\r\nAccess-Control-Max-Age: 60\r\n");
		//only an 200 is required, so abort request handling
		request_handled_by = CORSOPTION;
	}

	//BEGIN command routine
	if(Ov_OK(result) && request_handled_by == NONE){
		if(ov_string_compare(this->v_ClientRequest.cmd, "/getServer") == OV_STRCMP_EQUAL){
			//http GET
			kshttp_printresponseheader(&responseBody, response_format, "getserver");
			result = kshttp_exec_getserver(&this->v_ClientRequest.args, &responseBody, response_format);
			kshttp_printresponsefooter(&responseBody, response_format, "getserver");
			request_handled_by = GETSERVER;
		}else if(ov_string_compare(this->v_ClientRequest.cmd, "/register") == OV_STRCMP_EQUAL){
			kshttp_printresponseheader(&responseBody, response_format, "register");
			if(!pChannel->v_isLocal){
				result = KS_ERR_NOREMOTE;
				kshttp_print_result_array(&responseBody, response_format, &result, 1, ": Registering remote is not allowed/useful");
			}else{
				//name, port, ksversion
				result = kshttp_exec_register(&this->v_ClientRequest.args, &responseBody, response_format);
			}
			kshttp_printresponsefooter(&responseBody, response_format, "register");
			request_handled_by = REGISTER;
		}else if(ov_string_compare(this->v_ClientRequest.cmd, "/unregister") == OV_STRCMP_EQUAL){
			//name, port, ksversion
			kshttp_printresponseheader(&responseBody, response_format, "unregister");
			if(!pChannel->v_isLocal){
				result = KS_ERR_NOREMOTE;
				kshttp_print_result_array(&responseBody, response_format, &result, 1, ": Registering remote is not allowed/useful");
			}else{
				result = kshttp_exec_unregister(&this->v_ClientRequest.args, &responseBody, response_format);
			}
			kshttp_printresponsefooter(&responseBody, response_format, "unregister");
			request_handled_by = UNREGISTER;
		}else if(ov_string_compare(this->v_ClientRequest.cmd, "/getVar") == OV_STRCMP_EQUAL){
			//http GET
			kshttp_printresponseheader(&responseBody, response_format, "getvar");
			result = kshttp_exec_getvar(&this->v_ClientRequest.args, &responseBody, response_format);
			kshttp_printresponsefooter(&responseBody, response_format, "getvar");
			//stream required?
			kshttp_find_arguments(&this->v_ClientRequest.args, "stream", &match);
			if(match.veclen>0){
				result = OV_ERR_NOTIMPLEMENTED;
				request_handled_by = GETVAR;
			}else if(FALSE){
				//disabled, because we are not called cyclic

				//yes
				ov_string_setvalue(&reply_contenttype, "text/event-stream");
				//first time?
				if(!this->v_stream){
					//backup the header
					ov_string_setvalue(&this->v_streamrequestheader, this->v_ClientRequest.requestHeader);
				}
				//is cache updated?
				if(ov_string_compare(responseBody, this->v_streambuffer) != OV_STRCMP_EQUAL){
					//ov_logfile_debug("upd %s %s", body, this->v_streambuffer);
					//yes
					ov_string_setvalue(&this->v_streambuffer, responseBody);
					ov_string_print(&responseBody, "data: %s\r\n\r\n", this->v_streambuffer);
				}else{
					//no - set body to null
					ov_string_setvalue(&responseBody, NULL);
				}
				request_handled_by = GETVARSTREAM;
			}else{
				//no
				request_handled_by = GETVAR;
			}
		}else if(ov_string_compare(this->v_ClientRequest.cmd, "/setVar") == OV_STRCMP_EQUAL){
			//http PUT, used in OData or PROPPATCH, used in WebDAV
			//add Access-Control-Allow-Methods to OPTIONS if implement other Methods
			kshttp_printresponseheader(&responseBody, response_format, "setvar");
			result = kshttp_exec_setvar(&this->v_ClientRequest.args, &responseBody, response_format);
			kshttp_printresponsefooter(&responseBody, response_format, "setvar");
			request_handled_by = SETVAR;
		}else if(ov_string_compare(this->v_ClientRequest.cmd, "/getEP") == OV_STRCMP_EQUAL){
			//http PROPFIND, used in WebDAV
			kshttp_printresponseheader(&responseBody, response_format, "getep");
			result = kshttp_exec_getep(&this->v_ClientRequest.args, &responseBody, response_format);
			kshttp_printresponsefooter(&responseBody, response_format, "getep");
			request_handled_by = GETEP;
		}else if(ov_string_compare(this->v_ClientRequest.cmd, "/createObject") == OV_STRCMP_EQUAL){
			//http PUT, used in WebDAV
			kshttp_printresponseheader(&responseBody, response_format, "createobject");
			result = kshttp_exec_createObject(&this->v_ClientRequest.args, &responseBody, response_format);
			kshttp_printresponsefooter(&responseBody, response_format, "createobject");
			request_handled_by = CREATEOBJECT;
		}else if(ov_string_compare(this->v_ClientRequest.cmd, "/deleteObject") == OV_STRCMP_EQUAL){
			//http DELETE, used in WebDAV
			kshttp_printresponseheader(&responseBody, response_format, "deleteobject");
			result = kshttp_exec_deleteObject(&this->v_ClientRequest.args, &responseBody, response_format);
			kshttp_printresponsefooter(&responseBody, response_format, "deleteobject");
			request_handled_by = DELETEOBJECT;
		}else if(ov_string_compare(this->v_ClientRequest.cmd, "/renameObject") == OV_STRCMP_EQUAL){
			//http MOVE, used in WebDAV
			kshttp_printresponseheader(&responseBody, response_format, "renameobject");
			result = kshttp_exec_renameObject(&this->v_ClientRequest.args, &responseBody, response_format);
			kshttp_printresponsefooter(&responseBody, response_format, "renameobject");
			request_handled_by = RENAMEOBJECT;
		}else if(ov_string_compare(this->v_ClientRequest.cmd, "/link") == OV_STRCMP_EQUAL){
			//http LINK
			kshttp_printresponseheader(&responseBody, response_format, "link");
			result = kshttp_exec_link(&this->v_ClientRequest.args, &responseBody, response_format);
			kshttp_printresponsefooter(&responseBody, response_format, "link");
			request_handled_by = LINK;
		}else if(ov_string_compare(this->v_ClientRequest.cmd, "/unlink") == OV_STRCMP_EQUAL){
			//http UNLINK
			kshttp_printresponseheader(&responseBody, response_format, "unlink");
			result = kshttp_exec_unlink(&this->v_ClientRequest.args, &responseBody, response_format);
			kshttp_printresponsefooter(&responseBody, response_format, "unlink");
			request_handled_by = UNLINK;
		}else if(ov_string_compare(this->v_ClientRequest.cmd, "/auth") == OV_STRCMP_EQUAL){
			result = kshttp_authorize(1, this, this->v_ClientRequest.requestHeader, &responseHeader, this->v_ClientRequest.requestMethod, this->v_ClientRequest.cmd);
			if(!Ov_Fail(result)){
				ov_string_append(&responseBody, "Secret area");
				result = OV_ERR_OK;
			}
			request_handled_by = HTTPAUTH;
		}
	}
	//END command routine

	//raw request header not needed any longer
	ov_string_setvalue(&this->v_ClientRequest.requestHeader, NULL);
	Ov_SetDynamicVectorLength(&this->v_ClientRequest.args,0,STRING);
	Ov_SetDynamicVectorLength(&match,0,STRING);

	//BEGIN static file routine
	//no command matched yet... Is it a static file?
	if(!Ov_Fail(result) && request_handled_by == NONE){
		OV_STRING filename = NULL;
		OV_STRING filepath = NULL;
		//assume index.html as a root file
		if(ov_string_compare("/", this->v_ClientRequest.cmd) == OV_STRCMP_EQUAL){
			filename = "index.html"; //memory given from the compiler
		}else if(this->v_ClientRequest.cmd[ov_string_getlength(this->v_ClientRequest.cmd)-1] == '/'){
			ov_string_append(&this->v_ClientRequest.cmd, "index.html");
			//remove leading /
			filename = this->v_ClientRequest.cmd + 1;
		}else{
			//remove leading /
			filename = this->v_ClientRequest.cmd + 1;
		}
		ov_memstack_lock();

		//a dot in a filename is represented via a percent notation in an identifier, so we need
		//to change the parameter. A directory should be possible, so we need to skip / in conversion
		ov_string_print(&filepath, "/data/kshttp/%s", kshttp_ov_path_topercent_noslash(filename));
		ov_memstack_unlock();
		pStaticfile = Ov_DynamicPtrCast(kshttp_staticfile, ov_path_getobjectpointer(filepath, 2));
		ov_string_setvalue(&filepath, NULL);
		filename = NULL;	//usage is save here, as we had no memory in the database

		if(pStaticfile != NULL){
			ov_string_setvalue(&reply_contenttype, pStaticfile->v_mimetype);
			ov_string_setvalue(&reply_encoding, pStaticfile->v_encoding);
			result = OV_ERR_OK;
			//reply_body is NULL right now
			responseBody = pStaticfile->v_content;
			request_handled_by = STATICFILE;
		}
	}
	//END static file routine

	//no method has found a hit
	if (request_handled_by == NONE){
		result = OV_ERR_BADPATH; //404
		kshttp_printresponseheader(&responseBody, response_format, "notimplemented");
		kshttp_print_result_array(&responseBody, response_format, &result, 1, ": KS command not supported or static file not found");
		kshttp_printresponsefooter(&responseBody, response_format, "notimplemented");
	}

	//BEGIN forming and sending the answer

	//adding encoding and content-type to the header
	if (ov_string_compare(reply_encoding, "") == OV_STRCMP_EQUAL){
			ov_string_print(&responseHeader, "%sContent-Type: %s\r\n", responseHeader, reply_contenttype);
		}else{
			ov_string_print(&responseHeader, "%sContent-Type: %s; charset=%s\r\n", responseHeader, reply_contenttype, reply_encoding);
	}

	//now we have to format the raw http answer
	map_result_to_http(&result, &this->v_ClientRequest.version, &responseHeader, &responseBody, response_format);

	//Append common data to header:
	ov_string_print(&responseHeader, "%sServer: ACPLT/OV HTTP Server %s (compiled %s %s)\r\n", responseHeader, OV_LIBRARY_DEF_kshttp.version, __TIME__, __DATE__);
	//no-cache
	if(request_handled_by != STATICFILE){
		if(ov_string_compare(this->v_ClientRequest.version, "1.0") == OV_STRCMP_EQUAL){
			//Cache-Control is not defined in 1.0, so we misuse the Pragma header (as everyone)
			ov_string_print(&responseHeader, "%sPragma: no-cache\r\n", responseHeader);
		}else{
			ov_string_print(&responseHeader, "%sExpires: 0\r\n", responseHeader);
		}
	}
	//HTTP1.1 says, we MUST send a Date: header if we have a clock. Do we have one? :)

	//handle keep_alives
	if (this->v_ClientRequest.keepAlive == TRUE) {
		ov_string_print(&responseHeader, "%sConnection: keep-alive\r\n", responseHeader);
	}else{
		ov_string_print(&responseHeader, "%sConnection: close\r\n", responseHeader);
	}
	//in case of a HEAD request there is no need to send the body
	if(ov_string_compare(this->v_ClientRequest.requestMethod, "HEAD") == OV_STRCMP_EQUAL){
		responseBodylength = 0;
	}else{
		responseBodylength = ov_string_getlength(responseBody);
	}

#ifndef KSHTTP_DISABLE_GZIP
	// check if the body length corresponds for compression
	if (responseBodylength >= MINIMAL_LENGTH_FOR_GZIP && this->v_ClientRequest.gzipAccepted == TRUE &&
												  (ov_string_match(reply_contenttype, "text/*") == TRUE
												|| ov_string_compare(reply_contenttype, "application/javascript") == OV_STRCMP_EQUAL
												|| ov_string_match(reply_contenttype, "application/xml") == TRUE
												|| ov_string_match(reply_contenttype, "application/xml-dtd") == TRUE
												|| ov_string_match(reply_contenttype, "*+xml") == TRUE 	//includes image/svg+xml
												|| ov_string_compare(reply_contenttype, "application/json") == OV_STRCMP_EQUAL))
	{
		gzip_applicable = TRUE;
	}

	if(gzip_applicable){
		// The body is compressed by using gzip function in gzip.h
		gzip(responseBody, &gzip_compressed_body, &gzip_compressed_body_length);
	}
#endif

	if(request_handled_by != GETVARSTREAM){
		//append content length
		if(gzip_applicable){
			ov_string_print(&responseHeader, "%sContent-Length: %i\r\n", responseHeader, gzip_compressed_body_length);
			KS_logfile_debug(("Compression ratio: %f", (float)((float)gzip_compressed_body_length+ov_string_getlength(responseHeader))/((float)ov_string_getlength(responseHeader)-24+responseBodylength)));
		}else{
			ov_string_print(&responseHeader, "%sContent-Length: %i\r\n", responseHeader, responseBodylength);
		}
	}

	//handle gzip encoding by attaching a line to the header if accepted
	if(gzip_applicable)
	{
		ov_string_append(&responseHeader, "Content-Encoding: gzip\r\n");
	}

	// and finalize the header
	ov_string_append(&responseHeader, "\r\n");

	//send header only if not in stream mode
	if(this->v_stream == FALSE){
		KS_logfile_debug(("httpclienthandler: sending header: %d bytes", (int)ov_string_getlength(responseHeader)));
		ksbase_KSDATAPACKET_append(answer, (OV_BYTE*)responseHeader, ov_string_getlength(responseHeader));
		//note this does not send the request
	}

	//are we starting a stream?
	if(this->v_stream == FALSE && request_handled_by == GETVARSTREAM){
		this->v_stream = TRUE;
		//speed the processing time down to 5ms
		this->v_cycInterval = 5;
	}

	//in case of a HEAD request there is no need to send the body
	if(ov_string_compare(this->v_ClientRequest.requestMethod, "HEAD") != OV_STRCMP_EQUAL && responseBody != NULL){
		KS_logfile_debug(("httpclienthandler: sending body: %d bytes", (int)ov_string_getlength(responseBody)));

		if(gzip_applicable){
			//this does not send the request
			ksbase_KSDATAPACKET_append(answer, (OV_BYTE*)gzip_compressed_body, gzip_compressed_body_length);
		}else{
			//this does not send the request
			ksbase_KSDATAPACKET_append(answer, (OV_BYTE*)responseBody, responseBodylength);
		}
	}

	//free resources

#ifndef KSHTTP_DISABLE_GZIP
	ov_database_free(gzip_compressed_body);
#endif

	//request_header is freed already
	ov_string_setvalue(&this->v_ClientRequest.version, NULL);
	ov_string_setvalue(&this->v_ClientRequest.host, NULL);
	ov_string_setvalue(&responseHeader, NULL);
	Ov_HeapFree(this->v_ClientRequest.messageBody);
	this->v_ClientRequest.messageBody = NULL;

	//if a static file is returned body is pointing inside the database
	if(request_handled_by != STATICFILE){
		ov_string_setvalue(&responseBody, NULL);
	}

	ov_string_setvalue(&reply_encoding, NULL);
	ov_string_setvalue(&reply_contenttype, NULL);

	ov_string_setvalue(&this->v_ClientRequest.cmd, NULL);

	//shutdown tcp connection if no keep_alive was set
	if (this->v_ClientRequest.keepAlive != TRUE || Ov_Fail(result)) {
		pChannel->v_CloseAfterSend = TRUE;
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

	// initial strings
	thisCl->v_ClientRequest.args.veclen = 0;
	thisCl->v_ClientRequest.args.value = NULL;
	thisCl->v_ClientRequest.cmd = NULL;
	thisCl->v_ClientRequest.host = NULL;
	thisCl->v_ClientRequest.requestHeader = NULL;
	thisCl->v_ClientRequest.requestMethod = NULL;
	thisCl->v_ClientRequest.version = NULL;

	thisCl->v_ClientRequest.contentLength = 0;
	thisCl->v_ClientRequest.gzipAccepted = FALSE;

	return;
}

OV_DLLFNCEXPORT void kshttp_httpclienthandler_shutdown(
	OV_INSTPTR_ov_object 	pobj
) {
	/*
	*   local variables
	*/
	OV_INSTPTR_kshttp_httpclienthandler thisCl = Ov_StaticPtrCast(kshttp_httpclienthandler, pobj);

	//free memory
	Ov_SetDynamicVectorLength(&thisCl->v_ClientRequest.args,0,STRING);
	ov_string_setvalue(&thisCl->v_ClientRequest.cmd, NULL);
	ov_string_setvalue(&thisCl->v_ClientRequest.host, NULL);
	ov_string_setvalue(&thisCl->v_ClientRequest.requestHeader, NULL);
	ov_string_setvalue(&thisCl->v_ClientRequest.requestMethod, NULL);
	ov_string_setvalue(&thisCl->v_ClientRequest.version, NULL);

	/* set the object's state to "shut down" */
	ksbase_ClientHandler_shutdown(pobj);

	return;
}

