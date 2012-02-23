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


// const int STOK; //0
// const int STPMSOCKCR; //2
// const int STPMRQSND; //3
// const int STPMRPLYRCV; //4
// const int STMNGRQSND; //5

//cyccount of managercom obj, thus intervall of REGISTER sending
#define MANAGER_REGISTER_INTERVALL 10000

#define STATUS_STOK          0
#define STATUS_STPMSOCKCR    2
#define STATUS_STPMRQSND     3
#define STATUS_STPMRPLYRCV   4
#define STATUS_STMNGRQSND    5

//tcpconnection status
#define STATUS_TCPCON_OK          0
#define STATUS_TCPCON_SOCKOPENFAILED 1
#define STATUS_TCPCON_SOCKNONBLOCKFAILED 2
#define STATUS_TCPCON_SOCKBINDFAILED 3
#define STATUS_TCPCON_SOCKLISTENFAILED 4
#define STATUS_TCPCON_SOCKACCEPTFAILED 5
#define STATUS_TCPCON_SOCKNONBLOCKCLIENTFAILED 6

#define HTTP_COMMON_HEADER "Access-Control-Allow-Origin:*\nConnection: close\n"
#define HTTP_404_ERROR "HTTP/1.1 404 Not Found\n" HTTP_COMMON_HEADER "\nerror 404: path not found\n"
#define HTTP_400_ERROR "HTTP/1.1 400 OK\n" HTTP_COMMON_HEADER "\nerror 400: parameter are not valid\n"
#define HTTP_418_ERROR "HTTP/1.1 418 OK\n" HTTP_COMMON_HEADER "\nI am a teapot (see RFC 2324)\n"
#define HTTP_500_ERROR "HTTP/1.1 500 OK\n" HTTP_COMMON_HEADER "\nerror 500: internal server error\n"
#define HTTP_503_ERROR "HTTP/1.1 503 OK\n" HTTP_COMMON_HEADER "\nerror 503: service not available\n"
#define HTTP_200_HEADER "HTTP/1.1 200 OK\n" HTTP_COMMON_HEADER //content has to follow

//manager status
#define STATUS_MANAGER_OK          0

//managersendrecv status
#define STATUS_MANAGERSENDRECV_UNUSED                             0
#define STATUS_MANAGERSENDRECV_WAITINGFORSENDING          1
#define STATUS_MANAGERSENDRECV_WAITINGFORANSWER           2


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
OV_RESULT parse_http_request(OV_STRING buffer, OV_STRING* cmd, OV_STRING_VEC* args);
OV_RESULT find_arguments(OV_STRING_VEC* args, const OV_STRING varname, OV_STRING_VEC* re);
