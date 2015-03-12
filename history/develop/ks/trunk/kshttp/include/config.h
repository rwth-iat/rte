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

#include "libov/ov_ov.h"
#include "libov/ov_macros.h"
#include "libov/ov_result.h"
#include "libov/ov_string.h"
#include "libov/ov_vendortree.h"
#include "libov/ov_path.h"
#include "libov/ov_logfile.h"
#include "libov/ov_time.h"

#include "kshttp.h"
#include "ks_logfile.h"
#include "ksbase_helper.h"
#include "ov_ksserver_backend.h"
#include "NoneTicketAuthenticator.h"

//config for gzip compression
#define HTTP_MINIMAL_LENGTH_FOR_GZIP  150

//authorization.c
#define KSHTTP_AUTH_REALM "Top secret! User: simple Pass: pass"
#define KSHTTP_AUTH_RECHECK_MD5 15   //recalculate md5 checksums ov the authenticaton every nth access
						 //low levels will produce more cpu load since md5 will be calculated more often


#define IsFlagSet(flags, name)	(flags & (1L << (name-'a')))

OV_RESULT kshttp_parse_http_header_from_client(KSHTTP_REQUEST *clientRequest, KSHTTP_RESPONSE *serverResponse);
OV_RESULT kshttp_find_arguments(const OV_STRING_VEC* urlQuery, const OV_STRING varname, OV_STRING_VEC* re);
OV_STRING kshttp_ov_path_topercent_noslash (OV_STRING org);
OV_RESULT kshttp_authorize(int level, OV_INSTPTR_kshttp_httpclienthandler this, OV_STRING request_header, OV_STRING* reply_header, OV_STRING request_method, OV_STRING cmd);
OV_RESULT kshttp_include_localfiles(OV_INSTPTR_ov_domain pstaticfiles);
OV_RESULT kshttp_printresponseheader(OV_STRING* output, const KSHTTP_RESPONSEFORMAT response_format, const OV_STRING entry_type);
OV_RESULT kshttp_printresponsefooter(OV_STRING* output, const KSHTTP_RESPONSEFORMAT response_format, const OV_STRING entry_type);
OV_RESULT kshttp_response_part_init(OV_STRING* output, const KSHTTP_RESPONSEFORMAT response_format, const OV_STRING entry_type);
OV_RESULT kshttp_response_part_begin(OV_STRING* output, const KSHTTP_RESPONSEFORMAT response_format, const OV_STRING entry_type);
OV_RESULT kshttp_response_parts_seperate(OV_STRING* output, const KSHTTP_RESPONSEFORMAT response_format);
OV_RESULT kshttp_response_part_finalize(OV_STRING* output, const KSHTTP_RESPONSEFORMAT response_format, const OV_STRING entry_type);

OV_RESULT kshttp_print_result_array(OV_STRING *output, const KSHTTP_RESPONSEFORMAT response_format, OV_RESULT *results, const OV_UINT len, const OV_STRING explain_text);

OV_RESULT kshttp_exec_getserver(const KSHTTP_REQUEST request, KSHTTP_RESPONSE *response);
OV_RESULT kshttp_exec_register(const KSHTTP_REQUEST request, KSHTTP_RESPONSE *response);
OV_RESULT kshttp_exec_unregister(const KSHTTP_REQUEST request, KSHTTP_RESPONSE *response);

OV_RESULT kshttp_exec_getvar(const KSHTTP_REQUEST request, KSHTTP_RESPONSE *response);
OV_RESULT kshttp_exec_setvar(const KSHTTP_REQUEST request, KSHTTP_RESPONSE *response);
OV_RESULT kshttp_exec_getep(const KSHTTP_REQUEST request, KSHTTP_RESPONSE *response);

OV_RESULT kshttp_exec_createObject(const KSHTTP_REQUEST request, KSHTTP_RESPONSE *response);
OV_RESULT kshttp_exec_deleteObject(const KSHTTP_REQUEST request, KSHTTP_RESPONSE *response);
OV_RESULT kshttp_exec_renameObject(const KSHTTP_REQUEST request, KSHTTP_RESPONSE *response);
OV_RESULT kshttp_exec_link(const KSHTTP_REQUEST request, KSHTTP_RESPONSE *response);
OV_RESULT kshttp_exec_unlink(const KSHTTP_REQUEST request, KSHTTP_RESPONSE *response);
OV_RESULT kshttp_exec_getlogfile(const KSHTTP_REQUEST request, KSHTTP_RESPONSE *response);

OV_RESULT kshttp_timetoascii(OV_STRING* timestring, const OV_TIME* time, const KSHTTP_RESPONSEFORMAT response_format);
OV_RESULT kshttp_timespantoascii(OV_STRING* timestring, const OV_TIME_SPAN* ptime, const KSHTTP_RESPONSEFORMAT response_format);
OV_RESULT kshttp_asciitotime(OV_TIME* time, const OV_STRING timestring);
OV_RESULT kshttp_escapeString(OV_STRING* resultString, OV_STRING* strIn, const KSHTTP_RESPONSEFORMAT response_format);

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
		);

OV_RESULT kshttp_getChannelPointer(OV_INSTPTR_kshttp_httpClientBase this, OV_INSTPTR_ksbase_Channel* ppChannel, OV_VTBLPTR_ksbase_Channel* ppVtblChannel);
OV_RESULT kshttp_processServerReplyHeader(KS_DATAPACKET* dataReceived, KSHTTP_RESPONSE *responseStruct, OV_UINT *httpParseStatus);
OV_RESULT kshttp_decodeTransferEncodingChunked(OV_BYTE *rawHTTPmessage, OV_BYTE **entityBody, OV_UINT *contentLength, OV_UINT receivedlength, OV_UINT *httpParseStatus);
OV_RESULT kshttp_encodebase64(OV_STRING * strBase64string, OV_STRING input);
OV_BYTE *kshttp_strnstr(OV_BYTE *haystack, OV_STRING needle, OV_UINT length);
