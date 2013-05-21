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

#ifndef OV_COMPILE_LIBRARY_kshttpd
#define OV_COMPILE_LIBRARY_kshttpd
#endif


#include <time.h>

#include "config.h"
#include "kshttpd.h"
#ifndef KSHTTPD_DISABLE_GZIP
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

#define BUFFER_CHUNK_SIZE 2048

/*** TEMP ***/
OV_DLLFNCEXPORT OV_BOOL kshttpd_httpclienthandler_stream_get(
		OV_INSTPTR_kshttpd_httpclienthandler          pobj
) {
	return pobj->v_stream;
}

OV_DLLFNCEXPORT OV_RESULT kshttpd_httpclienthandler_stream_set(
		OV_INSTPTR_kshttpd_httpclienthandler          pobj,
		const OV_BOOL  value
) {
	pobj->v_stream = value;
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_STRING kshttpd_httpclienthandler_streamrequestheader_get(
		OV_INSTPTR_kshttpd_httpclienthandler          pobj
) {
	return pobj->v_streamrequestheader;
}

OV_DLLFNCEXPORT OV_RESULT kshttpd_httpclienthandler_streamrequestheader_set(
		OV_INSTPTR_kshttpd_httpclienthandler          pobj,
		const OV_STRING  value
) {
	return ov_string_setvalue(&pobj->v_streamrequestheader,value);
}

OV_DLLFNCEXPORT OV_STRING kshttpd_httpclienthandler_streambuffer_get(
		OV_INSTPTR_kshttpd_httpclienthandler          pobj
) {
	return pobj->v_streambuffer;
}

OV_DLLFNCEXPORT OV_RESULT kshttpd_httpclienthandler_streambuffer_set(
		OV_INSTPTR_kshttpd_httpclienthandler          pobj,
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
void map_result_to_http(OV_RESULT* result, OV_STRING* http_version, OV_STRING* header, OV_STRING* body, OV_UINT response_format){
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

	//fixme i see HTTP status 0 after createObject...

	//no free needed since no memory allocated
	switch (*result)
	{
	//still some duplication, but good enough for now
	case OV_ERR_OK:
		ov_string_print(header, "HTTP/%s %s%s", *http_version, HTTP_200_HEADER, tmp_header);
		break;
	case OV_ERR_BADNAME:
		ov_string_print(header, "HTTP/%s %s%s", *http_version, HTTP_400_HEADER, tmp_header);
		if(response_format != RESPONSE_FORMAT_KSX){
			ov_string_print(body, "KS_ERR_BADNAME: %s%s", HTTP_400_BODY, tmp_body);
		}
		break;
	case OV_ERR_BADPARAM:
		ov_string_print(header, "HTTP/%s %s%s", *http_version, HTTP_400_HEADER, tmp_header);
		if(response_format != RESPONSE_FORMAT_KSX){
			ov_string_print(body, "KS_ERR_BADPARAM: %s%s", HTTP_400_BODY, tmp_body);
		}
		break;
	case OV_ERR_BADAUTH:
		ov_string_print(header, "HTTP/%s %s%s", *http_version, HTTP_401_HEADER, tmp_header);
		if(response_format != RESPONSE_FORMAT_KSX){
			ov_string_print(body, "KS_ERR_BADAUTH: %s%s", HTTP_401_BODY, tmp_body);
		}
		break;
	case OV_ERR_NOACCESS:
		ov_string_print(header, "HTTP/%s %s%s", *http_version, HTTP_403_HEADER, tmp_header);
		if(response_format != RESPONSE_FORMAT_KSX){
			ov_string_print(body, "KS_ERR_NOACCESS: %s%s", HTTP_403_BODY, tmp_body);
		}
		break;
	case OV_ERR_BADPATH:
		ov_string_print(header, "HTTP/%s %s%s", *http_version, HTTP_404_HEADER, tmp_header);
		if(response_format != RESPONSE_FORMAT_KSX){
			ov_string_print(body, "KS_ERR_BADPATH: %s%s", HTTP_404_BODY, tmp_body);
		}
		break;
	case OV_ERR_BADVALUE:
		ov_string_print(header, "HTTP/%s %s%s", *http_version, HTTP_406_HEADER, tmp_header);
		if(response_format != RESPONSE_FORMAT_KSX){
			ov_string_print(body, "KS_ERR_BADVALUE: %s%s", HTTP_406_BODY, tmp_body);
		}
		break;
	case OV_ERR_NOTIMPLEMENTED:
		ov_string_print(header, "HTTP/%s %s%s", *http_version, HTTP_501_HEADER, tmp_header);
		if(response_format != RESPONSE_FORMAT_KSX){
			ov_string_print(body, "KS_ERR_NOTIMPLEMENTED: %s%s", HTTP_501_BODY, tmp_body);
		}
		break;
	case OV_ERR_GENERIC:
		ov_string_print(header, "HTTP/%s %s%s", *http_version, HTTP_503_HEADER, tmp_header);
		if(response_format != RESPONSE_FORMAT_KSX){
			ov_string_print(body, "KS_ERR_GENERIC: %s%s", HTTP_503_BODY, tmp_body);
		}
		break;
	case OV_ERR_ALREADYEXISTS:
		ov_string_print(header, "HTTP/%s %s%s", *http_version, HTTP_409_HEADER, tmp_header);
		if(response_format != RESPONSE_FORMAT_KSX){
			ov_string_print(body, "KS_ERR_ALREADYEXISTS: %s%s", HTTP_409_BODY, tmp_body);
		}
		break;
	case KS_ERR_NOMANAGER:
		ov_string_print(header, "HTTP/%s %s%s", *http_version, HTTP_406_HEADER, tmp_header);
		if(response_format != RESPONSE_FORMAT_KSX){
			ov_string_print(body, "KS_ERR_NOMANAGER: %s%s", HTTP_406_BODY, tmp_body);
		}
		break;
	case KS_ERR_SERVERUNKNOWN:
		ov_string_print(header, "HTTP/%s %s%s", *http_version, HTTP_404_HEADER, tmp_header);
		if(response_format != RESPONSE_FORMAT_KSX){
			ov_string_print(body, "KS_ERR_SERVERUNKNOWN: %s%s", HTTP_404_BODY, tmp_body);
		}
		break;
	default:
		ov_string_print(header, "HTTP/%s %s%s", *http_version, HTTP_503_HEADER, tmp_header);
		if(response_format != RESPONSE_FORMAT_KSX){
			ov_string_print(body, "KS_ERR (unknown): %s%s", HTTP_503_BODY, tmp_body);
		}
		break;
	}
}

/**
 * This function handles requests received over a channel. It reads data as an http-stream and triggers the appropriate OV-functions
 */

OV_DLLFNCEXPORT OV_RESULT kshttpd_httpclienthandler_HandleRequest(
	OV_INSTPTR_ksbase_ClientHandler baseClientHandler,
	KS_DATAPACKET* dataReceived,
	KS_DATAPACKET* answer
) {
	OV_INSTPTR_kshttpd_httpclienthandler this = Ov_StaticPtrCast(kshttpd_httpclienthandler, baseClientHandler);
	OV_INSTPTR_ksbase_Channel pChannel = Ov_GetParent(ksbase_AssocChannelClientHandler, this);

	OV_STRING request_method = NULL; //GET, HEAD, etc.
	OV_STRING request_header = NULL;
	OV_STRING http_version = NULL; //HTTP version
	OV_BOOL keep_alive = TRUE; //default is to keep the connection open
	OV_STRING reply_header = NULL; //header of the reply
	OV_STRING reply_body = NULL; //reply *WITHOUT HEADER*
	int bodylength = 0; //length of the return body

	OV_STRING cmd = NULL; //the get request without arguments
	OV_UINT response_format = RESPONSE_FORMAT_NONE;
	OV_UINT request_handled_by = REQUEST_HANDLED_BY_NONE;

	OV_BOOL gzip_accepted = FALSE; //default on encoding
	OV_BOOL gzip_applicable = FALSE;

	OV_UINT len;
	OV_RESULT result = OV_ERR_OK;
	OV_STRING_VEC match = {0,NULL};
	OV_STRING reply_contenttype="text/plain", reply_encoding="Windows-1252";

	//vector of the variables, odd elements are variable names, even are values
	OV_STRING_VEC args = {0,NULL};

	//gzip compression business
	OV_STRING gzip_compressed_body = NULL;
	OV_INT gzip_compressed_body_length = 0;

	OV_STRING* http_request_array = NULL;

	OV_INSTPTR_kshttpd_staticfile pStaticfile;


	//TODO NOTE: this works only for GET and HEAD, for POST one needs to evaluate content-length
	if(dataReceived->length > MAX_HTTP_REQUEST_SIZE){
		result = OV_ERR_BADVALUE; //414
		keep_alive = FALSE; //close connection
		//append double line break and let server process the input
		ksbase_KSDATAPACKET_append(answer, (OV_BYTE*)"\r\n\r\n", 9);
	}

	//if no stream mode, wait for the end of the header
	if(this->v_stream == FALSE){
		//if no double line break detected yet - wait till next cycle
		if(!strstr((OV_STRING)dataReceived->data, "\r\n\r\n")){
			return OV_ERR_OK;		/*	get called again to process the request next time (if it is complete then).
											Yes, this could block the ClientHandler for a longer time.	*/
		}
	}
	ks_logfile_debug("httpclienthandler/HandleRequest: got http command w/ %d bytes",dataReceived->length);
	//END handling buffer


	if(this->v_stream == FALSE){
		//this->v_requestbuffer contains the raw request
		//split header and footer of the http request

		http_request_array = ov_string_split((OV_STRING)dataReceived->data, "\r\n\r\n", &len);
		//len is always > 0

		ov_string_setvalue(&request_header, http_request_array[0]);
		//last line of the header will not contain \r\n
		ov_string_append(&request_header,"\r\n");
		//TODO for POST: ov_string_setvalue(&http_request_body, http_request[1]);
		//http_request[1]..http_request[len-1] is the request body - will be used for POST requests (not implemented yet)

		ov_string_freelist(http_request_array);
	}else{
		ov_string_setvalue(&request_header, this->v_streamrequestheader);
	}

	//empty the buffers
	ksbase_KSDATAPACKET_set(dataReceived, NULL, 0);

	//debug - output header
	ks_logfile_debug("%s", request_header);

	//parse request header into get command and arguments request
	if(!Ov_Fail(result)){
		result = parse_http_header(request_header, &cmd, &args, &http_version, &request_method, &gzip_accepted, &keep_alive, &response_format);
	}

	//allow javascript connection from any source (CORS)
	ov_string_setvalue(&reply_header, "Access-Control-Allow-Origin:*\r\n");

	if(!Ov_Fail(result)){
		result = OV_ERR_NOTIMPLEMENTED;
		//check which kind of request is coming in
		if(	ov_string_compare(request_method, "GET") == OV_STRCMP_EQUAL ||
				ov_string_compare(request_method, "HEAD") == OV_STRCMP_EQUAL){
			result = OV_ERR_OK;
		}else if(ov_string_compare(request_method, "PUSH") == OV_STRCMP_EQUAL){
			result = OV_ERR_OK;
		}else if(ov_string_compare(request_method, "OPTIONS") == OV_STRCMP_EQUAL){
			//used for Cross-Origin Resource Sharing (CORS)
			//todo add if using http methods: Access-Control-Allow-Methods: POST, GET, LINK...

			//hmi uses this headers, which is no problem for us
			ov_string_append(&reply_header, "Access-Control-Allow-Headers: if-modified-since\r\nAccess-Control-Max-Age: 60\r\n");
			result = OV_ERR_OK;
			//only an 200 is required, so abort request handling
			request_handled_by = REQUEST_HANDLED_BY_CORS_OPTION;
		}
	}

	if(RESPONSE_FORMAT_KSX == response_format){
		ov_string_setvalue(&reply_contenttype, "text/xml");
	}else if(RESPONSE_FORMAT_JSON == response_format){
		//fixme wieder an
		//ov_string_setvalue(&content_type, "application/json");
	}
	//BEGIN command routine
	if(Ov_OK(result) && request_handled_by == REQUEST_HANDLED_BY_NONE){
		if(ov_string_compare(cmd, "/getServer") == OV_STRCMP_EQUAL){
			//http GET
			printresponseheader(&reply_body, response_format, "getserver");
			result = exec_getserver(&args, &reply_body, response_format);
			printresponsefooter(&reply_body, response_format, "getserver");
			request_handled_by = REQUEST_HANDLED_BY_GETSERVER;
		}else if(ov_string_compare(cmd, "/register") == OV_STRCMP_EQUAL){
			result = OV_ERR_NOTIMPLEMENTED;
		}else if(ov_string_compare(cmd, "/unregister") == OV_STRCMP_EQUAL){
			result = OV_ERR_NOTIMPLEMENTED;
		}else if(ov_string_compare(cmd, "/getVar") == OV_STRCMP_EQUAL){
			//http GET
			//FIXME: a server crashes if http://localhost:8080/getVar?path=/communication/httpservers/httpserver/staticfiles/index.html/.mimetype is called
			//it is caused by the second dot in the filename
			printresponseheader(&reply_body, response_format, "getvar");
			result = exec_getvar(&args, &reply_body, response_format);
			printresponsefooter(&reply_body, response_format, "getvar");
			//stream required?
			find_arguments(&args, "stream", &match);
			if(match.veclen>0){
				//yes
				ov_string_setvalue(&reply_contenttype, "text/event-stream");
				//first time?
				if(!this->v_stream){
					//backup the header
					ov_string_setvalue(&this->v_streamrequestheader, request_header);
				}
				//is cache updated?
				if(ov_string_compare(reply_body, this->v_streambuffer) != OV_STRCMP_EQUAL){
					//ov_logfile_debug("upd %s %s", body, this->v_streambuffer);
					//yes
					ov_string_setvalue(&this->v_streambuffer, reply_body);
					ov_string_print(&reply_body, "data: %s\r\n\r\n", this->v_streambuffer);
				}else{
					//no - set body to null
					ov_string_setvalue(&reply_body, NULL);
				}
				request_handled_by = REQUEST_HANDLED_BY_GETVARSTREAM;
			}else{
				//no
				request_handled_by = REQUEST_HANDLED_BY_GETVAR;
			}
		}else if(ov_string_compare(cmd, "/setVar") == OV_STRCMP_EQUAL){
			//http PUT, used in OData or PROPPATCH, used in WebDAV
			printresponseheader(&reply_body, response_format, "setvar");
			result = exec_setvar(&args, &reply_body, response_format);
			printresponsefooter(&reply_body, response_format, "setvar");
			request_handled_by = REQUEST_HANDLED_BY_SETVAR;
		}else if(ov_string_compare(cmd, "/getEP") == OV_STRCMP_EQUAL){
			//http PROPFIND, used in WebDAV
			printresponseheader(&reply_body, response_format, "getep");
			result = exec_getep(&args, &reply_body, response_format);
			printresponsefooter(&reply_body, response_format, "getep");
			request_handled_by = REQUEST_HANDLED_BY_GETEP;
		}else if(ov_string_compare(cmd, "/createObject") == OV_STRCMP_EQUAL){
			//http PUT, used in WebDAV
			printresponseheader(&reply_body, response_format, "createobject");
			result = exec_createObject(&args, &reply_body, response_format);
			printresponsefooter(&reply_body, response_format, "createobject");
			request_handled_by = REQUEST_HANDLED_BY_CREATEOBJECT;
		}else if(ov_string_compare(cmd, "/deleteObject") == OV_STRCMP_EQUAL){
			//http DELETE, used in WebDAV
			printresponseheader(&reply_body, response_format, "deleteobject");
			result = exec_deleteObject(&args, &reply_body, response_format);
			printresponsefooter(&reply_body, response_format, "deleteobject");
			request_handled_by = REQUEST_HANDLED_BY_DELETEOBJECT;
		}else if(ov_string_compare(cmd, "/renameObject") == OV_STRCMP_EQUAL){
			//http MOVE, used in WebDAV
			printresponseheader(&reply_body, response_format, "renameobject");
			result = exec_renameObject(&args, &reply_body, response_format);
			printresponsefooter(&reply_body, response_format, "renameobject");
			request_handled_by = REQUEST_HANDLED_BY_RENAMEOBJECT;
		}else if(ov_string_compare(cmd, "/link") == OV_STRCMP_EQUAL){
			//http LINK
			printresponseheader(&reply_body, response_format, "link");
			result = exec_link(&args, &reply_body, response_format);
			printresponsefooter(&reply_body, response_format, "link");
			request_handled_by = REQUEST_HANDLED_BY_LINK;
		}else if(ov_string_compare(cmd, "/unlink") == OV_STRCMP_EQUAL){
			//http UNLINK
			printresponseheader(&reply_body, response_format, "unlink");
			result = exec_unlink(&args, &reply_body, response_format);
			printresponsefooter(&reply_body, response_format, "unlink");
			request_handled_by = REQUEST_HANDLED_BY_UNLINK;
		}else if(ov_string_compare(cmd, "/auth") == OV_STRCMP_EQUAL){
			result = authorize(1, this, request_header, &reply_header, request_method, cmd);
			if(!Ov_Fail(result)){
				ov_string_append(&reply_body, "Secret area");
				result = OV_ERR_OK;
			}
			request_handled_by = REQUEST_HANDLED_BY_AUTH;
		}
	}
	//END command routine

	//raw request header not needed any longer
	ov_string_setvalue(&request_header, NULL);
	Ov_SetDynamicVectorLength(&args,0,STRING);
	Ov_SetDynamicVectorLength(&match,0,STRING);

	//BEGIN static file routine
	//no command matched yet... Is it a static file?
	if(!Ov_Fail(result) && request_handled_by == REQUEST_HANDLED_BY_NONE){
		OV_STRING filename = NULL;
		OV_STRING filepath = NULL;
		OV_STRING basepath = NULL;
		//assume index.html as a root file
		if(ov_string_compare("/", cmd) == OV_STRCMP_EQUAL){
			filename = "index.html";
		}else if(cmd[ov_string_getlength(cmd)-1] == '/'){
			ov_string_append(&cmd, "index.html");
			//remove leading /
			filename = cmd + 1;
		}else{
			//remove leading /
			filename = cmd + 1;
		}
		ov_memstack_lock();
		//basepath is something like /communication/httpservers/httpserver
		basepath = ov_path_getcanonicalpath((OV_INSTPTR_ov_object)Ov_GetParent(ov_containment,Ov_GetParent(ov_containment,this)), 2); //path to the current client instance
		//a dot in a filename is represented via a percent notation in an identifier, so we need
		//to change the parameter. A directory should be possible, so we need to skip / in conversion
		ov_string_print(&filepath, "%s/staticfiles/%s", basepath, ov_path_topercent_noslash(filename));
		ov_memstack_unlock();
		pStaticfile = Ov_DynamicPtrCast(kshttpd_staticfile, ov_path_getobjectpointer(filepath, 2));
		ov_string_setvalue(&filepath, NULL);
		filename = NULL;

		if(pStaticfile != NULL){
			ov_string_setvalue(&reply_contenttype, pStaticfile->v_mimetype);
			ov_string_setvalue(&reply_encoding, pStaticfile->v_encoding);
			result = OV_ERR_OK;
			//reply_body is NULL right now
			reply_body = pStaticfile->v_content;
			request_handled_by = REQUEST_HANDLED_BY_STATICFILE;
		}
	}
	//END static file routine

	//no method has found a hit
	if (request_handled_by == REQUEST_HANDLED_BY_NONE){
		ov_string_append(&reply_body, "Resource not found");
		result = OV_ERR_BADPATH; //404
	}

	//BEGIN forming and sending the answer

	//adding encoding and content-type to the header
	if (ov_string_compare(reply_encoding, "") == OV_STRCMP_EQUAL){
			ov_string_print(&reply_header, "%sContent-Type: %s\r\n", reply_header, reply_contenttype);
		}else{
			ov_string_print(&reply_header, "%sContent-Type: %s; charset=%s\r\n", reply_header, reply_contenttype, reply_encoding);
	}

	//now we have to format the raw http answer
	map_result_to_http(&result, &http_version, &reply_header, &reply_body, response_format);

	//Append common data to header:
	ov_string_print(&reply_header, "%sServer: ACPLT/OV HTTP Server %s (compiled %s %s)\r\n", reply_header, OV_LIBRARY_DEF_kshttpd.version, __TIME__, __DATE__);
	//no-cache
	if(request_handled_by != REQUEST_HANDLED_BY_STATICFILE){
		if(ov_string_compare(http_version, "1.0") == OV_STRCMP_EQUAL){
			//Cache-Control is not defined in 1.0, so we misuse the Pragma header (as everyone)
			ov_string_print(&reply_header, "%sPragma: no-cache\r\n", reply_header);
		}else{
			ov_string_print(&reply_header, "%sExpires: 0\r\n", reply_header);
		}
	}
	//HTTP1.1 says, we MUST send a Date: header if we have a clock. Do we have one? :)

	//handle keep_alives
	if (keep_alive == TRUE) {
		ov_string_print(&reply_header, "%sConnection: keep-alive\r\n", reply_header);
	}else{
		ov_string_print(&reply_header, "%sConnection: close\r\n", reply_header);
	}
	//in case of a HEAD request there is no need to send the body
	if(ov_string_compare(request_method, "HEAD") == OV_STRCMP_EQUAL){
		bodylength = 0;
	}else{
		bodylength = (int)ov_string_getlength(reply_body);
	}

#ifndef KSHTTPD_DISABLE_GZIP
	// check if the body length corresponds for compression
	if (bodylength >= MINIMAL_LENGTH_FOR_GZIP && gzip_accepted == TRUE &&
												  (ov_string_compare(reply_contenttype, "text/plain") == OV_STRCMP_EQUAL
												|| ov_string_compare(reply_contenttype, "text/html") == OV_STRCMP_EQUAL
												|| ov_string_compare(reply_contenttype, "text/xml") == OV_STRCMP_EQUAL
												|| ov_string_compare(reply_contenttype, "text/javascript") == OV_STRCMP_EQUAL
												|| ov_string_compare(reply_contenttype, "text/css") == OV_STRCMP_EQUAL
												|| ov_string_compare(reply_contenttype, "application/xml") == OV_STRCMP_EQUAL
												|| ov_string_compare(reply_contenttype, "application/xhtml+xml") == OV_STRCMP_EQUAL
												|| ov_string_compare(reply_contenttype, "application/javascript") == OV_STRCMP_EQUAL
												|| ov_string_compare(reply_contenttype, "application/x-javascript") == OV_STRCMP_EQUAL))
	{
		gzip_applicable = TRUE;
	}

	if(gzip_applicable){
		// The body is compressed by using gzip function in gzip.h
		gzip(reply_body, &gzip_compressed_body, &gzip_compressed_body_length);
	}
#endif

	if(request_handled_by != REQUEST_HANDLED_BY_GETVARSTREAM){
		//append content length
		if(gzip_applicable){
			ov_string_print(&reply_header, "%sContent-Length: %i\r\n", reply_header, gzip_compressed_body_length);
			ks_logfile_debug("Compression ratio: %f", (float)((float)gzip_compressed_body_length+ov_string_getlength(reply_header))/((float)ov_string_getlength(reply_header)-24+bodylength));
		}else{
			ov_string_print(&reply_header, "%sContent-Length: %i\r\n", reply_header, bodylength);
		}
	}

	//handle gzip encoding by attaching a line to the header if accepted
	if(gzip_applicable)
	{
		ov_string_append(&reply_header, "Content-Encoding: gzip\r\n");
	}

	// and finalize the header
	ov_string_append(&reply_header, "\r\n");

	//send header only if not in stream mode
	if(this->v_stream == FALSE){
		ks_logfile_debug("httpclienthandler: sending header: %d bytes", (int)ov_string_getlength(reply_header));
		ksbase_KSDATAPACKET_append(answer, (OV_BYTE*)reply_header, ov_string_getlength(reply_header));
		//todo this does not send the request
	}

	//are we starting a stream?
	if(this->v_stream == FALSE && request_handled_by == REQUEST_HANDLED_BY_GETVARSTREAM){
		this->v_stream = TRUE;
		//speed the processing time down to 5ms
		this->v_cycInterval = 5;
	}

	//in case of a HEAD request there is no need to send the body
	if(ov_string_compare(request_method, "HEAD") != OV_STRCMP_EQUAL && reply_body != NULL){
		ks_logfile_debug("httpclienthandler: sending body: %d bytes", (int)ov_string_getlength(reply_body));

		if(gzip_applicable){
			//todo this does not send the request
			ksbase_KSDATAPACKET_append(answer, (OV_BYTE*)gzip_compressed_body, gzip_compressed_body_length);
		}else{
			//todo this does not send the request
			ksbase_KSDATAPACKET_append(answer, (OV_BYTE*)reply_body, bodylength);
		}
	}

	//free resources

#ifndef KSHTTPD_DISABLE_GZIP
	ov_database_free(gzip_compressed_body);
#endif

	ov_string_setvalue(&request_method, NULL);
	//request_header is freed already
	ov_string_setvalue(&http_version, NULL);
	ov_string_setvalue(&reply_header, NULL);
	//if a static file is returned body is pointing inside the database
	if(request_handled_by != REQUEST_HANDLED_BY_STATICFILE){
		ov_string_setvalue(&reply_body, NULL);
	}

	ov_string_setvalue(&reply_encoding, NULL);
	ov_string_setvalue(&reply_contenttype, NULL);

	ov_string_setvalue(&cmd, NULL);

	//shutdown tcp connection if no keep_alive was set
	if (keep_alive != TRUE || Ov_Fail(result)) {
		ksbase_Channel_CloseConnection(pChannel);
	}
	return OV_ERR_OK;
}

