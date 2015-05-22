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

/********************************************
 * This file is double licensed!
 * LGPL + static linking exception is additionally possible
 *********************************************/

#ifdef HTTPSERVER_STANDALONE
	//we are compiling not in the OV infrastructure

	//we need the the basic types like OV_UINT, ...
	#include "ov_ov.h"
	//and the HTTP defines
	#include "kshttp.ovt"

	//you have to include something which provides ov_string_setvalue, ov_string_print, ov_string_
#else
	#ifndef OV_COMPILE_LIBRARY_kshttp
	#define OV_COMPILE_LIBRARY_kshttp
	#endif

	#include "config.h"
#endif

#ifndef HTTP_DISABLE_GZIP
	#include "gzip.h"
#endif

/**
 * strnstr implementation to find a needle but only till a length
 * @param haystack
 * @param needle
 * @param length
 * @return
 */
OV_BYTE *kshttp_strnstr(OV_BYTE *haystack, OV_STRING needle, OV_UINT length){
	OV_UINT needle_length = strlen(needle);
	OV_UINT i;

	for (i = 0; i < length; i++){
		if (i + needle_length > length){
			return NULL;
		}
		if (strncmp((char*)&haystack[i], needle, needle_length) == 0){
			return &haystack[i];
		}
	}
	return NULL;
}


/**
 * init structs, read header (if full available) and parse it into request
 */
DLLFNCEXPORT OV_RESULT kshttp_httpclienthandler_analyzeRequestHeader(
		HTTP_REQUEST *request,
		HTTP_RESPONSE *response,
		HTTP_COMMUNICATIONSTATUS *communicationStatus,
		OV_BYTE *indata,
		OV_UINT datalength
) {
	OV_RESULT result = OV_ERR_OK;
	OV_BYTE *endOfHeader = NULL;

	//initialise struct
	request->response_format = FORMATUNDEFINED;
	response->compressionGzip = FALSE;

	ov_string_setvalue(&response->contentString, NULL);
	Ov_DbFree(response->contentBinary);
	response->contentBinary = NULL;
	response->contentLength = 0;
	ov_string_setvalue(&response->contentType, NULL);
	ov_string_setvalue(&response->contentEncoding, NULL);
	response->requestHandledBy = KSHTTP_RGB_NONE;
	*communicationStatus = HTTP_CS_INITIAL;

	//wait for the end of the header
	//split header and footer of the http request
	endOfHeader = kshttp_strnstr(indata, "\r\n\r\n", datalength);
	if(!endOfHeader){
		//if no double line break detected yet - wait till next cycle
		return OV_ERR_OK;		/*	get called again to process the request next time (if it is complete then).
										Yes, this could block the ClientHandler for a longer time.	*/
	}

	//terminate after header for ov_string_split
	*endOfHeader = '\0';

	//header plus both line breaks
	request->headerLength = endOfHeader - indata + 4;

	ov_string_setvalue(&request->requestHeader, (OV_STRING)indata);

	//parse request header into get command and arguments request
	result = kshttp_parse_http_header_from_client(request, response);
	if(Ov_Fail(result)){
		*communicationStatus = HTTP_CS_SHUTDOWN;
		return OV_ERR_BADPARAM;
	}
	*communicationStatus = HTTP_CS_REQUESTHEADERPARSED;
	return OV_ERR_OK;
}

/**
 * move requestBody to Heap (if full available)
 */
DLLFNCEXPORT OV_RESULT kshttp_httpclienthandler_analyzeRequestBody(
		HTTP_REQUEST *request,
		HTTP_RESPONSE *response,
		HTTP_COMMUNICATIONSTATUS *communicationStatus,
		OV_BYTE *indata,
		OV_UINT datalength
) {
	if(request->contentLength != 0){
		//save message body (for POST?) in heap
		if(datalength - request->headerLength >= request->contentLength){
			request->messageBody = (OV_BYTE*)Ov_HeapMalloc(request->contentLength+1);
			if(!request->messageBody){
				*communicationStatus = HTTP_CS_SHUTDOWN;
				return OV_ERR_TARGETGENERIC;
			}

			memcpy(request->messageBody, indata + request->headerLength, request->contentLength);
			request->messageBody[request->contentLength] = '\0';
		}else{
			//we have the full header, but not the full body yet. Get called again next time
			return OV_ERR_OK;
		}
	}
	*communicationStatus = HTTP_CS_REQUESTPARSED;

	return OV_ERR_OK;
}

/**
 * builds up the response->header with the information from the ServerResponse struct
 * maps responseCreationResult to http codes
 * additionalHeaders will be added (must end with \r\n)
 * convenience contentString will be moved to contentBinary
 * contentBinary will be gzipped on demand
 */
DLLFNCEXPORT OV_RESULT kshttp_httpclienthandler_generateHttpHeader(
	const HTTP_REQUEST request,
	HTTP_RESPONSE *response,
	HTTP_COMMUNICATIONSTATUS *communicationStatus,
	OV_RESULT responseCreationResult,
	OV_STRING additionalHeaders
){
	//gzip compression business
	OV_BYTE* gzip_compressed_body = NULL;
	OV_INT gzip_compressed_body_length = 0;

	OV_RESULT result = OV_ERR_OK;

	//header already sent?
	if(response->header == NULL){
		return OV_ERR_OK;
	}

	//adding encoding and content-type to the header
	if (ov_string_compare(response->contentEncoding, "") == OV_STRCMP_EQUAL){
			ov_string_print(&response->header, "%sContent-Type: %s\r\n", response->header, response->contentType);
		}else{
			ov_string_print(&response->header, "%sContent-Type: %s; charset=%s\r\n", response->header, response->contentType, response->contentEncoding);
	}

	//now we have to format the raw http answer
	kshttp_httpclienthandler_mapresult2http(request, response, responseCreationResult);

	//Append common data to header:
	ov_string_print(&response->header, "%sServer: ACPLT/OV HTTP Server %s (compiled %s %s)\r\n", response->header, OV_LIBRARY_DEF_kshttp.version, __TIME__, __DATE__);
	//no-cache
	if(ov_string_compare(response->httpVersion, "1.0") == OV_STRCMP_EQUAL){
		//Cache-Control is not defined in 1.0, so we misuse the Pragma header (as everyone)
		ov_string_print(&response->header, "%sPragma: no-cache\r\n", response->header);
	}else{
		ov_string_print(&response->header, "%sExpires: 0\r\n", response->header);
	}
	//HTTP1.1 says, we MUST send a Date: header if we have a clock. Do we have one? :)

	//handle keep_alives
	if (response->keepAlive == TRUE) {
		ov_string_print(&response->header, "%sConnection: keep-alive\r\n", response->header);
	}else{
		ov_string_print(&response->header, "%sConnection: close\r\n", response->header);
	}

	//move convenience contentString to contentBinary
	if(ov_string_compare(request.requestMethod, "HEAD") == OV_STRCMP_EQUAL){
		//in case of a HEAD request there is no need to send the body
		ov_string_setvalue(&response->contentString, NULL);
		Ov_DbFree(response->contentBinary);
		response->contentLength = 0;
	}else if(response->contentString != NULL){
		//we have a content as string, so check its length and move content to binary pointer
		response->contentLength = ov_string_getlength(response->contentString);
		response->contentBinary = (OV_BYTE*)response->contentString;
		response->contentString = NULL;
	}else if(response->contentBinary != NULL){
		//contentLength is already set
	}else{
		//we have nothing to send available...
		response->contentLength = 0;
	}

#ifndef HTTP_DISABLE_GZIP
	//set default minimal length
	#ifndef HTTP_MINIMAL_LENGTH_FOR_GZIP
	#define HTTP_MINIMAL_LENGTH_FOR_GZIP 150
	#endif
	// check if the body length corresponds for compression
	if (response->contentLength >= HTTP_MINIMAL_LENGTH_FOR_GZIP && response->compressionGzip == TRUE &&
												  (ov_string_match(response->contentType, "text/*") == TRUE
												|| ov_string_compare(response->contentType, "application/javascript") == OV_STRCMP_EQUAL
												|| ov_string_match(response->contentType, "application/xml") == TRUE
												|| ov_string_match(response->contentType, "application/xml-dtd") == TRUE
												|| ov_string_match(response->contentType, "*+xml") == TRUE 	//includes image/svg+xml
												|| ov_string_compare(response->contentType, "application/json") == OV_STRCMP_EQUAL))
	{
		// The body is compressed by using gzip function in gzip.h
		result = gzip(response->contentBinary, response->contentLength, &gzip_compressed_body, &gzip_compressed_body_length);
		if(Ov_OK(result)){
			KS_logfile_debug(("Compression ratio (content only): %" OV_PRINT_SINGLE, (OV_SINGLE)(gzip_compressed_body_length/response->contentLength)));
			//free old content
			Ov_DbFree(response->contentBinary);
			//replace by gzipped content
			response->contentBinary = gzip_compressed_body;
			response->contentLength = gzip_compressed_body_length;
			gzip_compressed_body = NULL;

			ov_string_append(&response->header, "Content-Encoding: gzip\r\n");
		}else{
			//prevent sending Content-Encoding Header
			response->compressionGzip = FALSE;
			Ov_DbFree(gzip_compressed_body);
			gzip_compressed_body = NULL;
		}
	}else{
		//prevent sending Content-Encoding Header
		response->compressionGzip = FALSE;
	}
#endif

	//append content length
	ov_string_print(&response->header, "%sContent-Length: %i\r\n", response->header, response->contentLength);

	if(ov_string_compare(additionalHeaders, "") != OV_STRCMP_EQUAL){
		ov_string_append(&response->header, additionalHeaders);
	}

	// and finalize the header
	ov_string_append(&response->header, "\r\n");

	ov_string_setvalue(&response->contentEncoding, NULL);
	ov_string_setvalue(&response->contentType, NULL);

	*communicationStatus = HTTP_CS_RESPONSEHEADERGENERATED;

	return OV_ERR_OK;
}


/**
 * Builds header for http communication from OV_RESULT
 * @param result
 * @param http_version
 * @param header will be overwritten
 * @param body will be overwritten in an error state
 */
DLLFNCEXPORT void kshttp_httpclienthandler_mapresult2http(const HTTP_REQUEST request, HTTP_RESPONSE *response, OV_RESULT const result){
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
		if(request.response_format == KSTCL || request.response_format == PLAIN){
			ov_string_print(&response->contentString, "KS_ERR_BADNAME: %s%s", HTTP_400_BODY, tmp_body);
		}
		break;
	case OV_ERR_BADPARAM:
		ov_string_print(&response->header, "HTTP/%s %s%s", response->httpVersion, HTTP_400_HEADER, tmp_header);
		if(request.response_format == KSTCL || request.response_format == PLAIN){
			ov_string_print(&response->contentString, "KS_ERR_BADPARAM: %s%s", HTTP_400_BODY, tmp_body);
		}
		break;
	case OV_ERR_BADAUTH:
		ov_string_print(&response->header, "HTTP/%s %s%s", response->httpVersion, HTTP_401_HEADER, tmp_header);
		if(request.response_format == KSTCL || request.response_format == PLAIN){
			ov_string_print(&response->contentString, "KS_ERR_BADAUTH: %s%s", HTTP_401_BODY, tmp_body);
		}
		break;
	case OV_ERR_NOACCESS:
		ov_string_print(&response->header, "HTTP/%s %s%s", response->httpVersion, HTTP_403_HEADER, tmp_header);
		if(request.response_format == KSTCL || request.response_format == PLAIN){
			ov_string_print(&response->contentString, "KS_ERR_NOACCESS: %s%s", HTTP_403_BODY, tmp_body);
		}
		break;
	case OV_ERR_BADPATH:
		ov_string_print(&response->header, "HTTP/%s %s%s", response->httpVersion, HTTP_404_HEADER, tmp_header);
		if(request.response_format == KSTCL || request.response_format == PLAIN){
			ov_string_print(&response->contentString, "KS_ERR_BADPATH: %s%s", HTTP_404_BODY, tmp_body);
		}
		break;
	case OV_ERR_BADVALUE:
		ov_string_print(&response->header, "HTTP/%s %s%s", response->httpVersion, HTTP_406_HEADER, tmp_header);
		if(request.response_format == KSTCL || request.response_format == PLAIN){
			ov_string_print(&response->contentString, "KS_ERR_BADVALUE: %s%s", HTTP_406_BODY, tmp_body);
		}
		break;
	case OV_ERR_NOTIMPLEMENTED:
		ov_string_print(&response->header, "HTTP/%s %s%s", response->httpVersion, HTTP_501_HEADER, tmp_header);
		if(request.response_format == KSTCL || request.response_format == PLAIN){
			ov_string_print(&response->contentString, "KS_ERR_NOTIMPLEMENTED: %s%s", HTTP_501_BODY, tmp_body);
		}
		break;
	case OV_ERR_GENERIC:
		ov_string_print(&response->header, "HTTP/%s %s%s", response->httpVersion, HTTP_503_HEADER, tmp_header);
		if(request.response_format == KSTCL || request.response_format == PLAIN){
			ov_string_print(&response->contentString, "KS_ERR_GENERIC: %s%s", HTTP_503_BODY, tmp_body);
		}
		break;
	case OV_ERR_BADTYPE:
		ov_string_print(&response->header, "HTTP/%s %s%s", response->httpVersion, HTTP_412_HEADER, tmp_header);
		if(request.response_format == KSTCL || request.response_format == PLAIN){
			ov_string_print(&response->contentString, "KS_ERR_BADTYPE: %s%s", HTTP_412_BODY, tmp_body);
		}
		break;
	case OV_ERR_ALREADYEXISTS:
		ov_string_print(&response->header, "HTTP/%s %s%s", response->httpVersion, HTTP_409_HEADER, tmp_header);
		if(request.response_format == KSTCL || request.response_format == PLAIN){
			ov_string_print(&response->contentString, "KS_ERR_ALREADYEXISTS: %s%s", HTTP_409_BODY, tmp_body);
		}
		break;
	case KS_ERR_NOMANAGER:
		ov_string_print(&response->header, "HTTP/%s %s%s", response->httpVersion, HTTP_406_HEADER, tmp_header);
		if(request.response_format == KSTCL || request.response_format == PLAIN){
			ov_string_print(&response->contentString, "KS_ERR_NOMANAGER: %s%s", HTTP_406_BODY, tmp_body);
		}
		break;
	case KS_ERR_NOREMOTE:
		ov_string_print(&response->header, "HTTP/%s %s%s", response->httpVersion, HTTP_406_HEADER, tmp_header);
		if(request.response_format == KSTCL || request.response_format == PLAIN){
			ov_string_print(&response->contentString, "KS_ERR_NOREMOTE: %s%s", HTTP_406_BODY, tmp_body);
		}
		break;
	case KS_ERR_SERVERUNKNOWN:
		ov_string_print(&response->header, "HTTP/%s %s%s", response->httpVersion, HTTP_404_HEADER, tmp_header);
		if(request.response_format == KSTCL || request.response_format == PLAIN){
			ov_string_print(&response->contentString, "KS_ERR_SERVERUNKNOWN: %s%s", HTTP_404_BODY, tmp_body);
		}
		break;
	case KS_ERR_TARGETGENERIC:
		ov_string_print(&response->header, "HTTP/%s %s%s", response->httpVersion, HTTP_413_HEADER, tmp_header);
		if(request.response_format == KSTCL || request.response_format == PLAIN){
			ov_string_print(&response->contentString, "KS_ERR_TARGETGENERIC: %s%s", HTTP_413_BODY, tmp_body);
		}
		break;
	default:
		ov_string_print(&response->header, "HTTP/%s %s%s", response->httpVersion, HTTP_503_HEADER, tmp_header);
		if(request.response_format == KSTCL || request.response_format == PLAIN){
			ov_string_print(&response->contentString, "KS_ERR (unknown): %s%s", HTTP_503_BODY, tmp_body);
		}
		break;
	}
}
