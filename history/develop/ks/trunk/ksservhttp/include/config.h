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


//tcpconnection status
#define STATUS_TCPCON_OK          0
#define STATUS_TCPCON_SOCKOPENFAILED 1
#define STATUS_TCPCON_SOCKNONBLOCKFAILED 2
#define STATUS_TCPCON_SOCKBINDFAILED 3
#define STATUS_TCPCON_SOCKLISTENFAILED 4
#define STATUS_TCPCON_SOCKACCEPTFAILED 5
#define STATUS_TCPCON_SOCKNONBLOCKCLIENTFAILED 6

//maximal saved HTTP REQUEST
#define MAX_HTTP_REQUEST_SIZE 1024

//"HTTP/1.x " will be appended before header
#define HTTP_200_HEADER "200 OK\r\n"
#define HTTP_400_HEADER "400 Bad Request\r\n"
#define HTTP_400_BODY   "error 400: parameter are not valid\r\n"
#define HTTP_404_HEADER "404 Not Found\r\n"
#define HTTP_404_BODY   "error 404: path not found\r\n"
#define HTTP_406_HEADER "406 Not Acceptable\r\n"
#define HTTP_406_BODY   "error 406: Bad Name or Path requested\r\n"
#define HTTP_500_HEADER "500 Internal Server Error\r\n"
#define HTTP_500_BODY   "error 500: internal server error\r\n"
#define HTTP_503_HEADER "503 Service Unavailable\r\n"
#define HTTP_503_BODY 	"error 503: service not available\r\n"


#if PLT_SYSTEM_NT
 #define CLOSE_SOCKET closesocket
 #define IOCTL_SOCKET ioctlsocket
#else
 #define CLOSE_SOCKET close
 #define IOCTL_SOCKET ioctl
#endif

#ifndef EWOULDBLOCK
#define EWOULDBLOCK WSAEWOULDBLOCK
#endif

OV_RESULT getvar_to_string(OV_INSTPTR_ov_object pObj, OV_STRING* varname, OV_STRING* message);
OV_RESULT setvar_at_object(OV_INSTPTR_ov_object pObj, OV_STRING* varname, OV_STRING* newcontent, OV_STRING* message);
OV_RESULT parse_http_header(OV_STRING buffer, OV_STRING* cmd, OV_STRING_VEC* args, OV_STRING* http_version);
OV_RESULT find_arguments(OV_STRING_VEC* args, const OV_STRING varname, OV_STRING_VEC* re);
OV_STRING ov_path_topercentNoSlash (OV_STRING org);
