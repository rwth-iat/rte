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

#ifndef KSHTTP_OVT_INCLUDED
	//we are compiling not in the OV infrastructure

	//we need the the basic types like OV_UINT, ...
	#include "ov_ov.h"
	//and the HTTP defines
	#include "kshttp.ovt"

#endif


/**
 * init structs, read header (if full available) and parse it into request
 */
DLLFNCEXPORT OV_RESULT kshttp_httpclienthandler_analyzeRequestHeader(
		HTTP_REQUEST *request,
		HTTP_RESPONSE *response,
		HTTP_COMMUNICATIONSTATUS *communicationStatus,
		OV_BYTE *indata,
		OV_UINT datalength
);

/**
 * move requestBody to Heap (if full available)
 */
DLLFNCEXPORT OV_RESULT kshttp_httpclienthandler_analyzeRequestBody(
		HTTP_REQUEST *request,
		HTTP_RESPONSE *response,
		HTTP_COMMUNICATIONSTATUS *communicationStatus,
		OV_BYTE *indata,
		OV_UINT datalength
);

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
);

/**
 * Builds header for http communication from OV_RESULT
 * @param result
 * @param http_version
 * @param header will be overwritten
 * @param body will be overwritten in an error state
 */
DLLFNCEXPORT void kshttp_httpclienthandler_mapresult2http(const HTTP_REQUEST request, HTTP_RESPONSE *response, OV_RESULT const result);
