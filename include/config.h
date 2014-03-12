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


#ifdef __GNUC__
#define UNUSED __attribute__ ((unused))
#else
#define UNUSED
#endif

/*
 * KS_HTTP identifier
 */

#define KSHTTP_IDENTIFIER		"KSHTTP"

//State of registration: 0 not registered, 1 waiting for answer, 2 registered, 128 register error
#define HTTP_MNGRCOM_REGISTERSTATE_NOTREGISTERED 0
#define HTTP_MNGRCOM_REGISTERSTATE_WAITING 1
#define HTTP_MNGRCOM_REGISTERSTATE_REGISTERED 2
#define HTTP_MNGRCOM_REGISTERSTATE_ERROR 128

/*
 * KSHTTP specific client states
 */

#define HTTPCL_WAITINGFORSENDING	(1<<18)
#define HTTPCL_WAITINGFORPORT	(1<<19)
#define HTTPCL_SERVERNOTFOUND	(1<<24)
#define HTTPCL_MANAGERNOTFOUND	(1<<25)
#define HTTPCL_TIMEOUT			(1<<26)

/*
 * HTTP Acceptance codes
 */

#define HTTP_MSG_NEW				(0)
#define HTTP_MSG_INCOMPLETE			(100)
#define HTTP_MSG_HEADERACCEPTED		(206)
#define HTTP_MSG_ACCEPTED			(200)
#define HTTP_MSG_DENIED				(400)
#define HTTP_MSG_HEAPOUTOFMEMORY	(500)
#define HTTP_MSG_DBOUTOFMEMORY		(507)

//config for gzip compression
#define MINIMAL_LENGTH_FOR_GZIP  150

//output format for response
typedef enum KSHTTP_RESPONSEFORMAT {FORMATUNDEFINED=0,TCL=1,PLAIN=2,KSX=3,JSON=4,FORMATDEFAULT=TCL} KSHTTP_RESPONSEFORMAT;

//requestOutput for getVar and getEP, only internal used!
#define OP_UNKNOWN 0
#define OP_NAME 1
#define OP_TYPE 2
#define OP_COMMENT 3
#define OP_ACCESS 4
#define OP_SEMANTIC 5
#define OP_CREATIONTIME 6
#define OP_CLASS 7
#define OP_TECHUNIT 8
#define OP_ASSOCIDENT 9
#define OP_ROLEIDENT 10
#define OP_DEFAULTINTERP 11
#define OP_SUPPORTEDINTERP 12
#define OP_TYPEIDENT 13


typedef enum KSHTTP_REQUESTHANDLEDBY {NONE,GETVAR,GETVARSTREAM,SETVAR,GETEP,GETHANDLE,DELHANDLE,HTTPAUTH,CORSOPTION,STATICFILE,CREATEOBJECT,DELETEOBJECT,RENAMEOBJECT,LINK,UNLINK,GETSERVER,REGISTER,UNREGISTER,GETLOGFILE} KSHTTP_REQUESTHANDLEDBY;

//authorization.c
#define REALM "Top secret! User: simple Pass: pass"
#define SESSION_TTL 1200 //http session time to live in seconds
#define RECHECK_MD5 15   //recalculate md5 checksums ov the authenticaton every nth access
						 //low levels will produce more cpu load since md5 will be calculated more often

//"HTTP/1.x " will be appended before header
#define HTTP_200_HEADER "200 OK\r\n"
#define HTTP_400_HEADER "400 Bad Request\r\n"
#define HTTP_400_BODY   "error 400: Bad Name or Path requested\r\n"
#define HTTP_401_HEADER "401 Unauthorized\r\n"
#define HTTP_401_BODY   "error 401: unauthorized\r\n"
#define HTTP_403_HEADER "403 Forbidden\r\n"
#define HTTP_403_BODY   "error 403: Forbidden\r\n"
#define HTTP_404_HEADER "404 Not Found\r\n"
#define HTTP_404_BODY   "error 404: path not found\r\n"
#define HTTP_406_HEADER "406 Not Acceptable\r\n"
#define HTTP_406_BODY   "error 406: Bad Value requested\r\n"
#define HTTP_409_HEADER "406 Conflict\r\n"
#define HTTP_409_BODY   "error 409: Conflict\r\n"
#define HTTP_413_HEADER "413 Request Entity Too Large\r\n"
#define HTTP_413_BODY   "error 414: Request Entity Too Large\r\n"
#define HTTP_414_HEADER "414 Request-URL Too Long\r\n"
#define HTTP_414_BODY   "error 414: Request-URL Too Long\r\n"
#define HTTP_500_HEADER "500 Internal Server Error\r\n"
#define HTTP_500_BODY   "error 500: internal server error\r\n"
#define HTTP_501_HEADER "501 Not Implemented\r\n"
#define HTTP_501_BODY 	"error 501: service not implemented\r\n"
#define HTTP_503_HEADER "503 Service Unavailable\r\n"
#define HTTP_503_BODY 	"error 503: service not available\r\n"

#define IsFlagSet(flags, name)	(flags & (1L << (name-'a')))

OV_RESULT kshttp_parse_http_header_from_client(KSHTTP_REQUEST *clientRequest, KSHTTP_RESPONSEFORMAT *response_format);
OV_RESULT kshttp_find_arguments(OV_STRING_VEC* args, const OV_STRING varname, OV_STRING_VEC* re);
OV_STRING kshttp_ov_path_topercent_noslash (OV_STRING org);
OV_RESULT kshttp_authorize(int level, OV_INSTPTR_kshttp_httpclienthandler this, OV_STRING request_header, OV_STRING* reply_header, OV_STRING request_method, OV_STRING cmd);
OV_RESULT kshttp_include_localfiles(OV_INSTPTR_ov_domain pstaticfiles);
OV_RESULT kshttp_printresponseheader(OV_STRING* output, KSHTTP_RESPONSEFORMAT response_format, OV_STRING entry_type);
OV_RESULT kshttp_printresponsefooter(OV_STRING* output, KSHTTP_RESPONSEFORMAT response_format, OV_STRING entry_type);
OV_RESULT kshttp_response_part_init(OV_STRING* output, KSHTTP_RESPONSEFORMAT response_format, OV_STRING entry_type);
OV_RESULT kshttp_response_part_begin(OV_STRING* output, KSHTTP_RESPONSEFORMAT response_format, OV_STRING entry_type);
OV_RESULT kshttp_response_parts_seperate(OV_STRING* output, KSHTTP_RESPONSEFORMAT response_format);
OV_RESULT kshttp_response_part_finalize(OV_STRING* output, KSHTTP_RESPONSEFORMAT response_format, OV_STRING entry_type);

OV_RESULT kshttp_print_result_array(OV_STRING *output, KSHTTP_RESPONSEFORMAT response_format, OV_RESULT *results, OV_UINT len, OV_STRING explain_text);

OV_RESULT kshttp_exec_getserver(OV_STRING_VEC* const args, OV_STRING* message, KSHTTP_RESPONSEFORMAT response_format);
OV_RESULT kshttp_exec_getep(OV_STRING_VEC* args, OV_STRING* re, KSHTTP_RESPONSEFORMAT response_format);
OV_RESULT kshttp_exec_getvar(OV_STRING_VEC* const args, OV_STRING* message, KSHTTP_RESPONSEFORMAT response_format);
OV_RESULT kshttp_exec_setvar(OV_STRING_VEC* args, OV_STRING* re, KSHTTP_RESPONSEFORMAT response_format);
OV_RESULT kshttp_exec_createObject(OV_STRING_VEC* const args, OV_STRING* message, KSHTTP_RESPONSEFORMAT response_format);
OV_RESULT kshttp_exec_deleteObject(OV_STRING_VEC* const args, OV_STRING* message, KSHTTP_RESPONSEFORMAT response_format);
OV_RESULT kshttp_exec_renameObject(OV_STRING_VEC* const args, OV_STRING* message, KSHTTP_RESPONSEFORMAT response_format);
OV_RESULT kshttp_exec_link(OV_STRING_VEC* const args, OV_STRING* message, KSHTTP_RESPONSEFORMAT response_format);
OV_RESULT kshttp_exec_unlink(OV_STRING_VEC* const args, OV_STRING* message, KSHTTP_RESPONSEFORMAT response_format);

OV_RESULT kshttp_exec_register(OV_STRING_VEC* const args, OV_STRING* message, KSHTTP_RESPONSEFORMAT response_format);
OV_RESULT kshttp_exec_unregister(OV_STRING_VEC* const args, OV_STRING* message, KSHTTP_RESPONSEFORMAT response_format);
OV_RESULT kshttp_exec_getlogfile(OV_STRING_VEC* const args, OV_STRING* message, KSHTTP_RESPONSEFORMAT response_format);

OV_RESULT kshttp_timetoascii(OV_STRING* timestring, OV_TIME* time, KSHTTP_RESPONSEFORMAT response_format);
OV_RESULT kshttp_timespantoascii(OV_STRING* timestring, OV_TIME_SPAN* ptime, KSHTTP_RESPONSEFORMAT response_format);
OV_RESULT kshttp_asciitotime(OV_TIME* time, OV_STRING timestring);
OV_RESULT kshttp_escapeString(OV_STRING* resultString, OV_STRING* strIn, KSHTTP_RESPONSEFORMAT response_format);

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
