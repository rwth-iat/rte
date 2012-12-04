/*
*	Copyright (C) 2012
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

#include "ksservhttp.h"
#include "ksserv_logfile.h"
#include "ov_ksserver_stripped.h"

//macros for static file inclusion


//config for gzip compression
#define MINIMAL_LENGTH_FOR_GZIP  150

//output format of getvar
#define GETVAR_FORMAT_TCL 1
#define GETVAR_FORMAT_PLAIN 2
#define GETVAR_FORMAT_DEFAULT GETVAR_FORMAT_TCL

//requestOutput for getVar and getEP
#define OP_UNKNOWN 0
#define OP_NAME 1
#define OP_TYPE 2
#define OP_COMMENT 3
#define OP_ACCESS 4
#define OP_SEMANTIC 5
#define OP_CREATIONTIME 6
#define OP_CLASS 7

#define REQUEST_HANDLED_BY_NONE 0
#define REQUEST_HANDLED_BY_GETVAR 1
#define REQUEST_HANDLED_BY_GETVARSTREAM 2
#define REQUEST_HANDLED_BY_SETVAR 3
#define REQUEST_HANDLED_BY_GETEP 4
#define REQUEST_HANDLED_BY_GETHANDLE 5
#define REQUEST_HANDLED_BY_DELHANDLE 6
#define REQUEST_HANDLED_BY_AUTH 7
#define REQUEST_HANDLED_BY_CORS_OPTION 8
#define REQUEST_HANDLED_BY_STATICFILE 9

//authorization.c
#define REALM "Top secret! User: root Pass: pass"
#define SESSION_TTL 1200 //http session time to live in seconds
#define RECHECK_MD5 15   //recalculate md5 checksums ov the authenticaton every nth access
						 //low levels will produce more cpu load since md5 will be calculated more often

//tcpconnection status
#define STATUS_TCPCON_OK          0
#define STATUS_TCPCON_SOCKOPENFAILED 1
#define STATUS_TCPCON_SOCKNONBLOCKFAILED 2
#define STATUS_TCPCON_SOCKBINDFAILED 3
#define STATUS_TCPCON_SOCKLISTENFAILED 4
#define STATUS_TCPCON_SOCKACCEPTFAILED 5
#define STATUS_TCPCON_SOCKNONBLOCKCLIENTFAILED 6

//maximal saved HTTP REQUEST (full request header size)
#define MAX_HTTP_REQUEST_SIZE 8192

//"HTTP/1.x " will be appended before header
#define HTTP_200_HEADER "200 OK\r\n"
#define HTTP_400_HEADER "400 Bad Request\r\n"
#define HTTP_400_BODY   "error 400: parameter are not valid\r\n"
#define HTTP_401_HEADER "401 Unauthorized\r\n"
#define HTTP_401_BODY   "error 401: unauthorized\r\n"
#define HTTP_403_HEADER "403 Forbidden\r\n"
#define HTTP_403_BODY   "error 403: Forbidden\r\n"
#define HTTP_404_HEADER "404 Not Found\r\n"
#define HTTP_404_BODY   "error 404: path not found\r\n"
#define HTTP_406_HEADER "406 Not Acceptable\r\n"
#define HTTP_406_BODY   "error 406: Bad Name or Path requested\r\n"
#define HTTP_414_HEADER "414 Request Too Long\r\n"
#define HTTP_414_BODY   "error 414: request too long\r\n"
#define HTTP_500_HEADER "500 Internal Server Error\r\n"
#define HTTP_500_BODY   "error 500: internal server error\r\n"
#define HTTP_501_HEADER "501 Not Implemented\r\n"
#define HTTP_501_BODY 	"error 501: service not implemented\r\n"
#define HTTP_503_HEADER "503 Service Unavailable\r\n"
#define HTTP_503_BODY 	"error 503: service not available\r\n"


#if PLT_SYSTEM_NT
 #define CLOSE_SOCKET closesocket
 #define IOCTL_SOCKET ioctlsocket
 #define MSG_NOSIGNAL 0
#else
 #define CLOSE_SOCKET close
 #define IOCTL_SOCKET ioctl
#endif

#ifndef EWOULDBLOCK
#define EWOULDBLOCK WSAEWOULDBLOCK
#endif

OV_RESULT getvar_to_string(OV_INSTPTR_ov_object pObj, OV_STRING* varname, OV_UINT format, OV_STRING* message);
OV_RESULT setvar_at_object(OV_INSTPTR_ov_object pObj, OV_STRING* varname, OV_STRING* newcontent, OV_STRING* message);
OV_RESULT parse_http_header(OV_STRING buffer, OV_STRING* cmd, OV_STRING_VEC* args, OV_STRING* http_version, OV_STRING* http_request_type, OV_BOOL *gzip_accepted, OV_BOOL *keep_alive);
OV_RESULT find_arguments(OV_STRING_VEC* args, const OV_STRING varname, OV_STRING_VEC* re);
OV_STRING ov_path_topercent_noslash (OV_STRING org);
OV_RESULT authorize(int level, OV_INSTPTR_ksservhttp_httpclienthandler this, OV_STRING request_header, OV_STRING* reply_header, OV_STRING request_type, OV_STRING cmd);
OV_RESULT include_localfiles(OV_INSTPTR_ov_domain pstaticfiles);
OV_UINT extract_output_format(OV_STRING_VEC* args);
OV_RESULT init_vector_output(OV_STRING* output, OV_UINT format);
OV_RESULT split_vector_output(OV_STRING* output, OV_UINT format);
OV_RESULT begin_vector_output(OV_STRING* output, OV_UINT format);
OV_RESULT finalize_vector_output(OV_STRING* output, OV_UINT format);

OV_RESULT exec_getvar(OV_STRING_VEC* const args, OV_STRING* message);
