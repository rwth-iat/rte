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

#ifndef OV_COMPILE_LIBRARY_ksservhttp
#define OV_COMPILE_LIBRARY_ksservhttp
#endif


#include <time.h>

#include "config.h"
#include "ksservhttp.h"
#include "ksserv.h"
#ifndef KSSERVHTTP_DISABLE_GZIP
	#include "gzip.h"
#endif

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
OV_DLLFNCEXPORT OV_BOOL ksservhttp_httpclienthandler_stream_get(
		OV_INSTPTR_ksservhttp_httpclienthandler          pobj
) {
	return pobj->v_stream;
}

OV_DLLFNCEXPORT OV_RESULT ksservhttp_httpclienthandler_stream_set(
		OV_INSTPTR_ksservhttp_httpclienthandler          pobj,
		const OV_BOOL  value
) {
	pobj->v_stream = value;
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_STRING ksservhttp_httpclienthandler_streamrequestheader_get(
		OV_INSTPTR_ksservhttp_httpclienthandler          pobj
) {
	return pobj->v_streamrequestheader;
}

OV_DLLFNCEXPORT OV_RESULT ksservhttp_httpclienthandler_streamrequestheader_set(
		OV_INSTPTR_ksservhttp_httpclienthandler          pobj,
		const OV_STRING  value
) {
	return ov_string_setvalue(&pobj->v_streamrequestheader,value);
}

OV_DLLFNCEXPORT OV_STRING ksservhttp_httpclienthandler_streambuffer_get(
		OV_INSTPTR_ksservhttp_httpclienthandler          pobj
) {
	return pobj->v_streambuffer;
}

OV_DLLFNCEXPORT OV_RESULT ksservhttp_httpclienthandler_streambuffer_set(
		OV_INSTPTR_ksservhttp_httpclienthandler          pobj,
		const OV_STRING  value
) {
	return ov_string_setvalue(&pobj->v_streambuffer,value);
}
/** TEMP **/


/**
 * Returns the socket on which the object is receiving.
 */
OV_DLLFNCEXPORT OV_INT ksservhttp_httpclienthandler_receivesocket_get(
		OV_INSTPTR_ksservhttp_httpclienthandler          pobj
) {
	return pobj->v_receivesocket;
}

/**
 * Sets the socket on which the object is receiving.
 */
OV_DLLFNCEXPORT OV_RESULT ksservhttp_httpclienthandler_receivesocket_set(
		OV_INSTPTR_ksservhttp_httpclienthandler          pobj,
		const OV_INT           value
) {
	struct sockaddr_in m_addr;
#if OV_SYSTEM_NT
	int len;
#else
	socklen_t len;
#endif
	pobj->v_receivesocket = value;

	len = sizeof m_addr;
	if (getpeername(value, (struct sockaddr*)&m_addr, &len) == -1) {
		ksserv_logfile_error("Error while determining source IP of tcpclient -- getpeername() failed");
		return OV_ERR_OK;
	}
	ov_string_print(&pobj->v_sourceAdr, "%s:%d", inet_ntoa(m_addr.sin_addr), (int) ntohs(m_addr.sin_port));
	ksserv_logfile_debug("httpclienthandler/receivesocket/set: new TCPSocket %d from %s", value, pobj->v_sourceAdr);
	return OV_ERR_OK;
}

/**
 * This method is called on startup.
 */
OV_DLLFNCEXPORT void ksservhttp_httpclienthandler_startup(
		OV_INSTPTR_ov_object 	pobj
) {
	//ksserv_logfile_info("httpclienthandler/startup ###########");
	ov_object_startup(pobj);
	return;
}

/**
 * Procedure called on object shutdown.
 * It closes open socket.
 */
OV_DLLFNCEXPORT void ksservhttp_httpclienthandler_shutdown(
		OV_INSTPTR_ov_object 	pobj
) {
	OV_INSTPTR_ksservhttp_httpclienthandler this = Ov_StaticPtrCast(ksservhttp_httpclienthandler, pobj);

	int receivesocket;
	if(!this->v_deleted) {// v_deleted cares that socket closing happens just once, not twice while DeleteObj
		ksserv_logfile_error("httpclienthandler/shutdown socket %i", this->v_receivesocket);
		receivesocket = ksservhttp_httpclienthandler_receivesocket_get(this);
		if(receivesocket < 0) {
			ksserv_logfile_error("httpclienthandler/shutdown: instance %s has socket<0 - cant shutdown!?!", pobj->v_identifier);
			return;
		}
		//ksserv_logfile_debug("httpclienthandler/shutdown: %s closes socket %d", pobj->v_identifier, receivesocket);
		CLOSE_SOCKET(receivesocket);
		this->v_deleted = TRUE;
		Ov_DeleteObject(this); //calls shutdown again. Since it is deleted->TRUE, no recursion
		//ov_object_shutdown(pobj);
		return;
	}
	ov_object_shutdown(pobj);
	return;
}

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
	default:
		ov_string_print(header, "HTTP/%s %s%s", *http_version, HTTP_503_HEADER, tmp_header);
		if(response_format != RESPONSE_FORMAT_KSX){
			ov_string_print(body, "KS_ERR (unknown): %s%s", HTTP_503_BODY, tmp_body);
		}
		break;
	}
}

/*
 * The function sends length chars starting from the given pointer
 *
 */
OV_RESULT send_tcp(int socket, char* pointer, int length){
	int sentBytes=0;
	int sentChunkSize = 0;
	fd_set write_flags;
	struct timeval waitd;
	int err;

	do
	{
		// Zero the flags ready for using
		FD_ZERO(&write_flags);
		FD_SET(socket, &write_flags); // get write flags
		waitd.tv_sec = 0;     // Set Timeout
		waitd.tv_usec = 1000;    //  1 millisecond
		err = select(socket + 1, (fd_set*) 0,&write_flags, (fd_set*)0, &waitd);

		if(err < 0)
		{
			perror("httpclienthandler: error waiting for sending answer:");
			break;
		}

		if((length - sentBytes) > 4096)
		{
			sentChunkSize = send(socket, pointer, 4096, MSG_NOSIGNAL);
			if (sentChunkSize == -1)
			{
				ksserv_logfile_error("send() failed");
				ksserv_logfile_debug("send failed");
				return OV_ERR_GENERIC;
			}
		}
		else
		{
			sentChunkSize = send(socket, pointer, (length - sentBytes), MSG_NOSIGNAL);
			if (sentChunkSize == -1)
			{
				ksserv_logfile_error("send() failed");
				ksserv_logfile_debug("send failed");
				return OV_ERR_GENERIC;
			}
		}
		sentBytes += sentChunkSize;
		pointer += sentChunkSize;
		ksserv_logfile_debug("httpclienthandler: answer sent, sentChunkSize: %d sentBytes: %d", sentChunkSize, sentBytes);
		//move pointer to next chunk


	}while(sentBytes < length);
	return OV_ERR_OK;
}

/**
 * function to output the size of the database - needed for debug
 */
void checkdb(OV_STRING msg){
	OV_ANY		Variable;
	OV_INSTPTR_ov_object pObj=ov_path_getobjectpointer("/vendor",2);
	pObj = Ov_SearchChild(ov_containment, Ov_StaticPtrCast(ov_domain, pObj), "database_free");
	ov_vendortree_getvar(pObj, &Variable, NULL);
	ov_logfile_debug("-->> %s: %i", msg, Variable.value.valueunion.val_int);
}

/**
 * Time profiling
 */
void start(struct timeval* start){
	gettimeofday(start, NULL);
}
void stop(struct timeval* start, OV_STRING comment){
	struct timeval end;
	long mtime, seconds, useconds;
	gettimeofday(&end, NULL);
	seconds  = end.tv_sec  - start->tv_sec;
	useconds = end.tv_usec - start->tv_usec;
	mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;
	ksserv_logfile_debug("%s: Elapsed time: %ld milliseconds\n", comment, mtime);
}


/**
 * Procedure periodically called by ComTask // ov_scheduler. *
 * It takes over the sending/receiving of data to/from the
 * connected client.
 *  * If the connection is closed by the client this object
 * deletes itself.
 */
void ksservhttp_httpclienthandler_typemethod(
		//COmTask OV_INSTPTR_ov_object		pfb
		OV_INSTPTR_ksserv_ComTask	cTask
) {

	OV_INSTPTR_ksservhttp_httpclienthandler this = Ov_StaticPtrCast(ksservhttp_httpclienthandler, cTask);

	int bytes = -1; //-1 means nothing received
	int receivesocket = ksservhttp_httpclienthandler_receivesocket_get(this);

	char *buffer = 0;
	int recvBytes = 0;
	int buffer_size = 0; //factor of buffer size * BUFFER_CHUNK_SIZE
	char *buffer_location = 0; //pointer into the buffer

	//http stuff
	OV_STRING *http_request;
	OV_STRING header, body, cmd, http_request_type, http_request_header;
	int bodylength = 0; //length of the return body
	OV_RESULT result = OV_ERR_OK;
	OV_UINT response_format = RESPONSE_FORMAT_NONE;
	OV_BOOL keep_alive = TRUE; //default is to keep the connection open
	OV_BOOL gzip_accepted = FALSE; //default on enconding
	OV_BOOL gzip_applicable = FALSE;
	OV_UINT request_handled_by = REQUEST_HANDLED_BY_NONE;
	OV_STRING http_version;
	OV_UINT len;
	OV_STRING_VEC match = {0,NULL};
	OV_STRING content_type="text/plain", encoding="Windows-1252";

	//vector of the variables, even elements are variable names, odds are values
	OV_STRING_VEC args = {0,NULL};

	//gzip compression business
	OV_STRING gzip_compressed_body = NULL;
	OV_INT gzip_compressed_body_length = 0;

	OV_INSTPTR_ov_object temp;
	OV_INSTPTR_ksservhttp_staticfile staticfile;
	//ksserv_logfile_debug("httpclienthandler typemethod called ");
	if (receivesocket < 0) { // check if the socket might be OK.
		ksserv_logfile_error("%s/typemethod: no receive socket set (receivesocket==-1), thus deleting myself",cTask->v_identifier);
		ksservhttp_httpclienthandler_shutdown((OV_INSTPTR_ov_object)cTask);
		return;
	}

	do {//read HTTP request from socket in chunks until nothing more appears
		buffer_size++;
		buffer = (char*)realloc(buffer, BUFFER_CHUNK_SIZE*buffer_size);
		if(buffer == 0) {
			ksserv_logfile_error("httpclienthandler/typemethod: recv error, no memory for buffer");
			return;
		}
		buffer_location = buffer + BUFFER_CHUNK_SIZE * (buffer_size - 1);
		memset(buffer_location, 0, BUFFER_CHUNK_SIZE);
		recvBytes = recv(receivesocket, buffer_location, BUFFER_CHUNK_SIZE,0);
		if(recvBytes != -1 && bytes == -1) bytes = 0; //1st iteration and we have sth (prevents from missing 1 byte)
		if(recvBytes != -1) bytes += recvBytes;//if soth was received, calc overall size of received bytes
		//if(recvBytes > 0) ksserv_logfile_error("httpclienthandler/typemethod: ks cmd chunk no %d recv %d bytes, pBuffStart %p, pBuffInside %p",buffer_size, recvBytes, buffer, buffer_location);
	} while(recvBytes == BUFFER_CHUNK_SIZE); // stop if less than maximum bytes was read by recv
	//if(bytes != -1) ksserv_logfile_error("httpclienthandler/typemethod: ks cmd w/ size %d received ",bytes);


#if OV_SYSTEM_NT
	errno = WSAGetLastError();
#endif
	if(bytes == -1 && (errno != EAGAIN && errno != EWOULDBLOCK) ) {//ERROR during read - shutdown!
		ksserv_logfile_error("httpclienthandler/typemethod: recv error %i, shutdown httpclienthandler", errno);
		ksservhttp_httpclienthandler_shutdown((OV_INSTPTR_ov_object)cTask);
		//else: bytes = -1 && errno == EAGAIN || EWOULDBLOCK
	} else if(this->v_stream == FALSE && bytes == 0) {//normal shutdown by client
		ksserv_logfile_debug("httpclienthandler/typemethod: read 0 bytes - shutdown - %s", ((OV_INSTPTR_ksserv_Client)this)->v_sourceAdr);
		ksservhttp_httpclienthandler_shutdown((OV_INSTPTR_ov_object)cTask);
		//} else if (bytes == -1) {  // no current KS command this turn
	} else if(this->v_stream == TRUE || (this->v_stream == FALSE && bytes > 0)) {
		//this is the important part - something was read on tht tcp buffer

		ksserv_Client_setThisAsCurrent((OV_INSTPTR_ksserv_Client)this); //set this as current one

		cmd = NULL; //the get request without arguments
		body = NULL; //reply *WITHOUT HEADER*
		header = NULL; //header of the reply
		http_version = NULL; //HTTP version
		http_request = NULL;
		http_request_header = NULL;
		http_request_type = NULL; //GET, HEAD, etc.

		//MAIN ROUTINE OF THE WEB SERVER

		//NOTE: this works only for GET and HEAD, for post one needs to evaluate content-length
		//START handling buffer: appending the read chunk to the buffer that is saved between the cycles
		if(bytes > 0){
			if(ov_string_getlength(this->v_requestbuffer) + ov_string_getlength(buffer) <= MAX_HTTP_REQUEST_SIZE){
				ov_string_append(&(this->v_requestbuffer),buffer);
			}else{
				result = OV_ERR_BADVALUE; //414
				keep_alive = FALSE; //close connection
				//append double line break and let server process the input
				ov_string_append(&(this->v_requestbuffer),"\r\n\r\n");
			}
		}

		//if no stream mode, wait for the end of the header
		if(this->v_stream == FALSE){
			//if no double line break detected yet - wait till next cycle
			if(!strstr(this->v_requestbuffer, "\r\n\r\n")){
				free(buffer);
				return;
			}
		}
		ksserv_logfile_error("httpclienthandler/typemethod: got http command w/ %d bytes",bytes);

		//END handling buffer

		if(this->v_stream == FALSE){
			//this->v_requestbuffer contains the raw request
			//split header and footer of the http request
			http_request = ov_string_split(this->v_requestbuffer, "\r\n\r\n", &len);
			//len is always > 0
			ov_string_setvalue(&http_request_header, http_request[0]);
			//last line of the header will not contain \r\n
			ov_string_append(&http_request_header,"\r\n");
			//in future: ov_string_setvalue(&http_request_body, http_request[1]);

			ov_string_freelist(http_request);
		}else{
			ov_string_setvalue(&http_request_header, this->v_streamrequestheader);
		}

		//empty the buffers
		ov_string_setvalue(&(this->v_requestbuffer),"");

		//debug - output header
		ksserv_logfile_error("%s", http_request_header);

		//http_request_header is the request header
		//http_request[1]..http_request[len-1] is the request body - will be used for POST requests (not implemented yet)

		//parse request header into get command and arguments request
		if(!Ov_Fail(result)){
			result = parse_http_header(http_request_header, &cmd, &args, &http_version, &http_request_type, &gzip_accepted, &keep_alive, &response_format);
		}

		//allow javascript connection from any source
		ov_string_setvalue(&header, "Access-Control-Allow-Origin:*\r\n");

		if(!Ov_Fail(result)){
			result = OV_ERR_NOTIMPLEMENTED;
			//check which kind of request is coming in
			if(	ov_string_compare(http_request_type, "GET") == OV_STRCMP_EQUAL ||
					ov_string_compare(http_request_type, "HEAD") == OV_STRCMP_EQUAL){
				result = OV_ERR_OK;
			}else if(ov_string_compare(http_request_type, "PUSH") == OV_STRCMP_EQUAL){
				result = OV_ERR_OK;
			}else if(ov_string_compare(http_request_type, "OPTIONS") == OV_STRCMP_EQUAL){
				//used for Cross-Origin Resource Sharing (CORS)
				//todo add if using http methods: Access-Control-Allow-Methods: POST, GET, LINK...

				//hmi uses this headers, which is no problem for us
				ov_string_append(&header, "Access-Control-Allow-Headers: if-modified-since\r\nAccess-Control-Max-Age: 60\r\n");
				result = OV_ERR_OK;
				//only an 200 is required
				request_handled_by = REQUEST_HANDLED_BY_CORS_OPTION;
			}
		}

		if(RESPONSE_FORMAT_KSX == response_format){
			ov_string_setvalue(&content_type, "text/xml");
		}else if(RESPONSE_FORMAT_JSON == response_format){
			//fixme wieder an
			//ov_string_setvalue(&content_type, "application/json");
		}
		//BEGIN command routine
		if(Ov_OK(result) && request_handled_by == REQUEST_HANDLED_BY_NONE){
			if(ov_string_compare(cmd, "/getServer") == OV_STRCMP_EQUAL){
				//http GET
				printresponseheader(&body, response_format, "getserver");
				result = exec_getserver(&args, &body, response_format);
				printresponsefooter(&body, response_format, "getserver");
				request_handled_by = REQUEST_HANDLED_BY_GETSERVER;
			}else if(ov_string_compare(cmd, "/getVar") == OV_STRCMP_EQUAL){
				//http GET
				//FIXME: a server crashes if http://localhost:8080/getVar?path=/communication/httpservers/httpserver/staticfiles/index.html/.mimetype is called
				//it is caused by the second dot in the filename
				printresponseheader(&body, response_format, "getvar");
				result = exec_getvar(&args, &body, response_format);
				printresponsefooter(&body, response_format, "getvar");
				//stream required?
				find_arguments(&args, "stream", &match);
				if(match.veclen>0){
					//yes
					ov_string_setvalue(&content_type, "text/event-stream");
					//first time?
					if(!this->v_stream){
						//backup the header
						ov_string_setvalue(&this->v_streamrequestheader, http_request_header);
					}
					//is cache updated?
					if(ov_string_compare(body, this->v_streambuffer) != OV_STRCMP_EQUAL){
						//ov_logfile_debug("upd %s %s", body, this->v_streambuffer);
						//yes
						ov_string_setvalue(&this->v_streambuffer, body);
						ov_string_print(&body, "data: %s\r\n\r\n", this->v_streambuffer);
					}else{
						//no - set body to null
						ov_string_setvalue(&body, NULL);
					}
					request_handled_by = REQUEST_HANDLED_BY_GETVARSTREAM;
				}else{
					//no
					request_handled_by = REQUEST_HANDLED_BY_GETVAR;
				}
			}else if(ov_string_compare(cmd, "/setVar") == OV_STRCMP_EQUAL){
				//http PUT, used in OData or PROPPATCH, used in WebDAV
				printresponseheader(&body, response_format, "setvar");
				result = exec_setvar(&args, &body, response_format);
				printresponsefooter(&body, response_format, "setvar");
				request_handled_by = REQUEST_HANDLED_BY_SETVAR;
			}else if(ov_string_compare(cmd, "/getEP") == OV_STRCMP_EQUAL){
				//http PROPFIND, used in WebDAV
				printresponseheader(&body, response_format, "getep");
				result = exec_getep(&args, &body, response_format);
				printresponsefooter(&body, response_format, "getep");
				request_handled_by = REQUEST_HANDLED_BY_GETEP;
			}else if(ov_string_compare(cmd, "/createObject") == OV_STRCMP_EQUAL){
				//http PUT, used in WebDAV
				printresponseheader(&body, response_format, "createobject");
				result = exec_createObject(&args, &body, response_format);
				printresponsefooter(&body, response_format, "createobject");
				request_handled_by = REQUEST_HANDLED_BY_CREATEOBJECT;
			}else if(ov_string_compare(cmd, "/deleteObject") == OV_STRCMP_EQUAL){
				//http DELETE, used in WebDAV
				printresponseheader(&body, response_format, "deleteobject");
				result = exec_deleteObject(&args, &body, response_format);
				printresponsefooter(&body, response_format, "deleteobject");
				request_handled_by = REQUEST_HANDLED_BY_DELETEOBJECT;
			}else if(ov_string_compare(cmd, "/renameObject") == OV_STRCMP_EQUAL){
				//http MOVE, used in WebDAV
				printresponseheader(&body, response_format, "renameobject");
				result = exec_renameObject(&args, &body, response_format);
				printresponsefooter(&body, response_format, "renameobject");
				request_handled_by = REQUEST_HANDLED_BY_RENAMEOBJECT;
			}else if(ov_string_compare(cmd, "/link") == OV_STRCMP_EQUAL){
				//http LINK
				printresponseheader(&body, response_format, "link");
				result = exec_link(&args, &body, response_format);
				printresponsefooter(&body, response_format, "link");
				request_handled_by = REQUEST_HANDLED_BY_LINK;
			}else if(ov_string_compare(cmd, "/unlink") == OV_STRCMP_EQUAL){
				//http UNLINK
				printresponseheader(&body, response_format, "unlink");
				result = exec_unlink(&args, &body, response_format);
				printresponsefooter(&body, response_format, "unlink");
				request_handled_by = REQUEST_HANDLED_BY_UNLINK;
			}else if(ov_string_compare(cmd, "/auth") == OV_STRCMP_EQUAL){
				result = authorize(1, this, http_request_header, &header, http_request_type, cmd);
				if(!Ov_Fail(result)){
					ov_string_append(&body, "Secret area");
					result = OV_ERR_OK;
				}
				request_handled_by = REQUEST_HANDLED_BY_AUTH;
			}
		}
		//END command routine

		//raw request header not needed any longer
		ov_string_setvalue(&http_request_header, NULL);
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
			temp = ov_path_getobjectpointer(filepath, 2);
			ov_string_setvalue(&filepath, NULL);
			filename = NULL;

			if(temp != NULL && Ov_CanCastTo(ksservhttp_staticfile, temp)){
				staticfile = Ov_StaticPtrCast(ksservhttp_staticfile, temp);
				ov_string_setvalue(&content_type, staticfile->v_mimetype);
				ov_string_setvalue(&encoding, staticfile->v_encoding);
				result = OV_ERR_OK;
				//body is NULL here
				body = staticfile->v_content;
				request_handled_by = REQUEST_HANDLED_BY_STATICFILE;
			}
		}
		//END static file routine

		//no method has found a hit
		if (request_handled_by == REQUEST_HANDLED_BY_NONE){
			ov_string_append(&body, "Resource not found");
			result = OV_ERR_BADPATH; //404
		}

		//BEGIN forming and sending the answer

		//adding encoding and content-type to the header
		if (ov_string_compare(encoding, "") == OV_STRCMP_EQUAL){
				ov_string_print(&header, "%sContent-Type: %s\r\n", header, content_type);
			}else{
				ov_string_print(&header, "%sContent-Type: %s; charset=%s\r\n", header, content_type, encoding);
		}

		//now we have to format the raw http answer
		map_result_to_http(&result, &http_version, &header, &body, response_format);

		//Append common data to header:
		ov_string_print(&header, "%sServer: ACPLT/OV HTTP Server %s (compiled %s %s)\r\n", header, OV_LIBRARY_DEF_ksservhttp.version, __TIME__, __DATE__);
		//no-cache
		if(request_handled_by != REQUEST_HANDLED_BY_STATICFILE){
			if(ov_string_compare(http_version, "1.0") == OV_STRCMP_EQUAL){
				//Cache-Control is not defined in 1.0, so we misuse the Pragma header (as everyone)
				ov_string_print(&header, "%sPragma: no-cache\r\n", header);
			}else{
				//todo mal testen ob hier ein caching im cshmi hilft
				//Cache-Control: max-age=2
				ov_string_print(&header, "%sExpires: 0\r\n", header);
			}
		}
		//HTTP1.1 says, we MUST send a Date: header if we have a clock. Do we have one? :)

		//handle keep_alives
		if (keep_alive == TRUE) {
			ov_string_print(&header, "%sConnection: keep-alive\r\n", header);
		}else{
			ov_string_print(&header, "%sConnection: close\r\n", header);
		}
		//in case of a HEAD request there is no need to send the body
		if(ov_string_compare(http_request_type, "HEAD") == OV_STRCMP_EQUAL){
			bodylength = 0;
		}else{
			bodylength = (int)ov_string_getlength(body);
		}

#ifndef KSSERVHTTP_DISABLE_GZIP
		// check if the body length corresponds for compression
		if (bodylength >= MINIMAL_LENGTH_FOR_GZIP && gzip_accepted == TRUE &&
													  (ov_string_compare(content_type, "text/plain") == OV_STRCMP_EQUAL
													|| ov_string_compare(content_type, "text/html") == OV_STRCMP_EQUAL
													|| ov_string_compare(content_type, "text/xml") == OV_STRCMP_EQUAL
													|| ov_string_compare(content_type, "text/javascript") == OV_STRCMP_EQUAL
													|| ov_string_compare(content_type, "text/css") == OV_STRCMP_EQUAL
													|| ov_string_compare(content_type, "application/xml") == OV_STRCMP_EQUAL
													|| ov_string_compare(content_type, "application/xhtml+xml") == OV_STRCMP_EQUAL
													|| ov_string_compare(content_type, "application/javascript") == OV_STRCMP_EQUAL
													|| ov_string_compare(content_type, "application/x-javascript") == OV_STRCMP_EQUAL))
		{
			gzip_applicable = TRUE;
		}

		if(gzip_applicable){
			// The body is compressed by using gzip function in gzip.h
			gzip(body, &gzip_compressed_body, &gzip_compressed_body_length);
		}
#endif

		if(request_handled_by != REQUEST_HANDLED_BY_GETVARSTREAM){
			//append content length
			if(gzip_applicable){
				ov_string_print(&header, "%sContent-Length: %i\r\n", header, gzip_compressed_body_length);
				ksserv_logfile_debug("Compression ratio: %f", (float)((float)gzip_compressed_body_length+ov_string_getlength(header))/((float)ov_string_getlength(header)-24+bodylength));
			}else{
				ov_string_print(&header, "%sContent-Length: %i\r\n", header, bodylength);
			}
		}

		//handle gzip encoding by attaching a line to the header if accepted
		if(gzip_applicable)
		{
			ov_string_append(&header, "Content-Encoding: gzip\r\n");
		}

		// and finalize the header
		ov_string_append(&header, "\r\n");

		//send header only if not in stream mode
		if(this->v_stream == FALSE){
			ksserv_logfile_debug("httpclienthandler: sending header: %d bytes", (int)ov_string_getlength(header));
			send_tcp(receivesocket, header, (int)ov_string_getlength(header));
		}

		//are we starting a stream?
		if(this->v_stream == FALSE && request_handled_by == REQUEST_HANDLED_BY_GETVARSTREAM){
			this->v_stream = TRUE;
			//speed the processing time down to 5ms
			this->v_cycInterval = 5;
		}

		//in case of a HEAD request there is no need to send the body

		if(ov_string_compare(http_request_type, "HEAD") != OV_STRCMP_EQUAL && body!=NULL){
			ksserv_logfile_debug("httpclienthandler: sending body: %d bytes", (int)ov_string_getlength(body));

			if(gzip_applicable){
				result = send_tcp(receivesocket, gzip_compressed_body, gzip_compressed_body_length);
			}else{
				result = send_tcp(receivesocket, body, bodylength);
			}

		}

		//free resources
		ov_string_setvalue(&encoding, NULL);
		ov_string_setvalue(&content_type, NULL);

#ifndef KSSERVHTTP_DISABLE_GZIP
		ov_database_free(gzip_compressed_body);
#endif

		ov_string_setvalue(&cmd, NULL);
		ov_string_setvalue(&http_version, NULL);
		ov_string_setvalue(&http_request_type, NULL);
		ov_string_setvalue(&header, NULL);
		//if a static file is returned body is pointing inside of the database
		if(request_handled_by != REQUEST_HANDLED_BY_STATICFILE){
			ov_string_setvalue(&body, NULL);
		}

		//shutdown tcp connection if no keep_alive was set
		if (keep_alive != TRUE || Ov_Fail(result)) {
			ksservhttp_httpclienthandler_shutdown((OV_INSTPTR_ov_object)cTask);
		}

		ksserv_Client_unsetThisAsCurrent((OV_INSTPTR_ksserv_Client)this); //unset this as current one
	}
	//free up the buffer
	free(buffer);

	return;
}
