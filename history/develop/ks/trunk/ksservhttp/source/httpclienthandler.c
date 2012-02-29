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

#ifndef OV_COMPILE_LIBRARY_ksservhttp
#define OV_COMPILE_LIBRARY_ksservhttp
#endif


#include <time.h>

#include "ksservhttp.h"
#include "ksserv.h"
#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

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
		ov_logfile_error("Error while determining source IP of tcpclient -- getpeername() failed");
		 return OV_ERR_OK;
	}
	ov_string_print(&pobj->v_sourceAdr, "%s:%d", inet_ntoa(m_addr.sin_addr), (int) ntohs(m_addr.sin_port));
	ov_logfile_debug("tcpclient/receivesocket/set: new TCPSocket %d from %s", value, pobj->v_sourceAdr);
	return OV_ERR_OK;
}


/**
 * This method is called on startup.
 */
OV_DLLFNCEXPORT void ksservhttp_httpclienthandler_startup(
	OV_INSTPTR_ov_object 	pobj
) {
	//ov_logfile_info("tcpclient/startup ###########");
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
	if(!this->v_deleted) {// v_deleted cares that socket closing happens just once, not twiche while DeleteObj
		ov_logfile_error("tcpclient/shutdown socket %i", this->v_receivesocket);
		receivesocket = ksservhttp_httpclienthandler_receivesocket_get(this);
		if(receivesocket < 0) {
			ov_logfile_error("tcpclient/shutdown: instance %s has socket<0 - cant shutdown!?!", pobj->v_identifier);
			return;
		}
		//ov_logfile_debug("tcpclient/shutdown: %s closes socket %d", pobj->v_identifier, receivesocket);
		CLOSE_SOCKET(receivesocket);
		this->v_deleted = TRUE;
		Ov_DeleteObject(this); //calls shutdown again. Since it is deleted->TRUE, no recursion
		//ov_object_shutdown(pobj);
		return;
	}
	ov_object_shutdown(pobj);
	return;
}

void map_result_to_http(OV_RESULT* result, OV_STRING* http_version, OV_STRING* header, OV_STRING* body){
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
			//still some duplication, but good enougth for now
			case OV_ERR_OK:
				ov_string_print(header, "HTTP/%s %s%s", *http_version, HTTP_200_HEADER, tmp_header);
				break;
			case OV_ERR_BADNAME:
				ov_string_print(header, "HTTP/%s %s%s", *http_version, HTTP_400_HEADER, tmp_header);
				ov_string_print(body, "%s%s", HTTP_400_BODY, tmp_body);
				break;
			case OV_ERR_BADPARAM:
				ov_string_print(header, "HTTP/%s %s%s", *http_version, HTTP_400_HEADER, tmp_header);
				ov_string_print(body, "%s%s", HTTP_400_BODY, tmp_body);
				break;
			case OV_ERR_BADPATH:
				ov_string_print(header, "HTTP/%s %s%s", *http_version, HTTP_404_HEADER, tmp_header);
				ov_string_print(body, "%s%s", HTTP_404_BODY, tmp_body);
				break;
			case OV_ERR_BADVALUE:
				ov_string_print(header, "HTTP/%s %s%s", *http_version, HTTP_414_HEADER, tmp_header);
				ov_string_print(body, "%s%s", HTTP_414_BODY, tmp_body);
				break;
			case OV_ERR_NOTIMPLEMENTED:
				ov_string_print(header, "HTTP/%s %s%s", *http_version, HTTP_501_HEADER, tmp_header);
				ov_string_print(body, "%s%s", HTTP_501_BODY, tmp_body);
				break;
			case OV_ERR_GENERIC:
				ov_string_print(header, "HTTP/%s %s%s", *http_version, HTTP_503_HEADER, tmp_header);
				ov_string_print(body, "%s%s", HTTP_503_BODY, tmp_body);
				break;
			default:
				ov_string_print(header, "HTTP/%s %s%s", *http_version, HTTP_503_HEADER, tmp_header);
				ov_string_print(body, "%s%s", HTTP_503_BODY, tmp_body);
				break;
	}

}

#define EXEC_GETVAR_RETURN ov_string_freelist(pPathList); \
						Ov_SetDynamicVectorLength(&match,0,STRING);\
						ov_string_setvalue(&message, NULL);\
						return

OV_RESULT exec_getvar(OV_STRING_VEC* args, OV_STRING* re){
	OV_STRING *pPathList = NULL;
	OV_UINT len;
	OV_STRING_VEC match = {0,NULL};
	OV_RESULT result;
	OV_STRING message = NULL;

	find_arguments(args, "path", &match);
	if(match.veclen!=1){
		ov_string_append(re, "Variable path not found");
		EXEC_GETVAR_RETURN OV_ERR_BADPARAM; //400
	}
	pPathList = ov_string_split((match.value[0]), ".", &len);
	if(len!=2){
		ov_string_freelist(pPathList);
		ov_string_append(re, "Variablename must contain a dot");
		EXEC_GETVAR_RETURN OV_ERR_BADPARAM; //400
	}
	result = getvar_to_string(ov_path_getobjectpointer(pPathList[0],2),&(pPathList[1]),&message);

	ov_string_append(re, message);

	EXEC_GETVAR_RETURN result;
}

#define EXEC_SETVAR_RETURN ov_string_freelist(pPathList); \
						Ov_SetDynamicVectorLength(&match,0,STRING);\
						ov_string_setvalue(&message, NULL);\
						return

OV_RESULT exec_setvar(OV_STRING_VEC* args, OV_STRING* re){
	OV_STRING *pPathList = NULL;
	OV_UINT len;
	OV_STRING_VEC match = {0,NULL};
	OV_RESULT result;
	OV_STRING message = NULL;

	find_arguments(args, "path", &match);
	if(match.veclen<=1){
		ov_string_append(re, "Variable path not found");
		EXEC_SETVAR_RETURN OV_ERR_BADPARAM; //400
	}
	pPathList = ov_string_split(match.value[0], ".", &len);
	if(len!=2){
		ov_string_freelist(pPathList);
		ov_string_append(re, "Variablename must contain a dot");
		EXEC_SETVAR_RETURN OV_ERR_BADPARAM; //400
	}
	find_arguments(args, "newvalue", &match);
	if(match.veclen!=1){
		ov_string_print(re, "Variable newvalue not found");
		EXEC_SETVAR_RETURN OV_ERR_BADPARAM; //400;
	}
	result = setvar_at_object(ov_path_getobjectpointer(pPathList[0],2),&(pPathList[1]),&match.value[0], &message);

	ov_string_append(re, message);

	EXEC_SETVAR_RETURN result;
}

#define EXEC_GETEP_RETURN \
						Ov_SetDynamicVectorLength(&match,0,STRING);\
						ov_string_setvalue(&message, NULL);\
						ov_string_setvalue(&objectType, match.value[0]);\
						ov_string_setvalue(&outputInfos, match.value[0]);\
						return
OV_RESULT exec_getep(OV_STRING_VEC* args, OV_STRING* re){
	OV_INSTPTR_ov_object pObj = NULL;
	OV_INSTPTR_ov_object pChild = NULL;
	OV_STRING_VEC match = {0,NULL};
	OV_STRING message = NULL;
	OV_STRING objectType = NULL;
	OV_STRING outputInfos = NULL;

	//muss noch aufgebohrt werden:
	//path=/TechUnits
	//objectType=OT_DOMAINS|OT_VARIABLES|... (siehe tcl-tks doku)
	//TODO: outputInfos (anderer name muss her) ein bis sieben informatinen liefert tcl-tks

	find_arguments(args, "path", &match);
	if(match.veclen!=1){
		ov_string_append(re, "Variable path not found");
		EXEC_GETEP_RETURN OV_ERR_BADPARAM; //400
	}
	pObj = ov_path_getobjectpointer(match.value[0],0);
	if (pObj == NULL){
		ov_string_append(re, "Variable not found");
		EXEC_GETEP_RETURN OV_ERR_BADPATH; //404
	}
	find_arguments(args, "objectType", &match);
	if(match.veclen!=1){
		ov_string_append(re, "objectType not found");
		EXEC_GETEP_RETURN OV_ERR_BADPARAM; //400
	}
	ov_string_setvalue(&objectType, match.value[0]);
	find_arguments(args, "outputInfos", &match);
	if(match.veclen!=1){
		ov_string_setvalue(&outputInfos, "OP_ANY");
	}else{
		ov_string_setvalue(&outputInfos, match.value[0]);
	}
	if(ov_string_compare(objectType, "OT_DOMAINS") == OV_STRCMP_EQUAL
			&& Ov_CanCastTo(ov_domain, pObj)){
		Ov_ForEachChild(ov_containment, Ov_StaticPtrCast(ov_domain, pObj), pChild){
			//outputInfos=OP_NAME
			if (message == NULL){
				ov_string_print(&message, "{%s}\r\n", pChild->v_identifier);
			}else{
				ov_string_print(&message, "%s{%s}", message, pChild->v_identifier);
			}
		}
	}else if(ov_string_compare(objectType, "OT_VARIABLES") == OV_STRCMP_EQUAL){
		ov_string_append(re, "objectType VARIABLES not implemented");
		EXEC_GETEP_RETURN OV_ERR_NOTIMPLEMENTED; //501
	}else{
		ov_string_append(re, "objectType not implemented");
		EXEC_GETEP_RETURN OV_ERR_NOTIMPLEMENTED; //501
	}
	ov_string_append(re, message);

	EXEC_GETEP_RETURN OV_ERR_OK;
}

/*
 * The function sends length chars starting from the given pointer
 *
  */
OV_RESULT send_tcp(int socket, char* pointer, int length){
	int sentBytes=0;
	int sentChunkSize = 0;
	do
	{
		if((length - sentBytes) > 4096)
		{
			sentChunkSize = send(socket, pointer, 4096, 0);
			if (sentChunkSize == -1)
			{
				ov_logfile_error("send() failed");
				return OV_ERR_GENERIC;
			}
		}
		else
		{
			sentChunkSize = send(socket, pointer, (length - sentBytes), 0);
			if (sentChunkSize == -1)
			{
				ov_logfile_error("send() failed");
				return OV_ERR_GENERIC;
			}
		}
		sentBytes += sentChunkSize;
		pointer += sentChunkSize;
		ov_logfile_debug("tcpclient: answer sent, sentChunkSize: %d sentBytes: %d", sentChunkSize, sentBytes);
		//move pointer to next chunk

		if(sentBytes < length)
		{
			//sleep to give time for sending (sleep values are trial and error based for the development PC (Intel C600))
#if !OV_SYSTEM_NT
			//on linux usleep counts in usecs
			usleep(1000);
#else
			//on windows Sleep counts in msecs
			Sleep(1);
#endif
		}
	}while(sentBytes < length);
	return OV_ERR_OK;
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
	OV_STRING header, body, cmd;
	OV_RESULT result = OV_ERR_OK;
	OV_BOOL keep_alive = TRUE; //default is to keep the connection open
	OV_BOOL static_file = FALSE; //is true if we send a static file
	OV_STRING http_version;
	OV_UINT len;

	//vector of the variables, even elements are variable names, odds are values
	OV_STRING_VEC args = {0,NULL};

	OV_INSTPTR_ov_object temp;
	OV_INSTPTR_ksservhttp_staticfile staticfile;

	//ov_logfile_debug("tcpclient typemethod called ");
	if (receivesocket < 0) { // check if the socket might be OK.
		ov_logfile_error("%s/typemethod: no receive socket set (receivesocket==-1), thus deleting myself",cTask->v_identifier);
		ksservhttp_httpclienthandler_shutdown((OV_INSTPTR_ov_object)cTask);
		return;
	}
	do {//read HTTP request from socket in chunks until nothing more appears
		buffer_size++;
		buffer = (char*)realloc(buffer, BUFFER_CHUNK_SIZE*buffer_size);
		if(buffer == 0) {
			ov_logfile_error("tcpclient/typemethod: recv error, no memory for buffer");
			return;
		}
		buffer_location = buffer + BUFFER_CHUNK_SIZE * (buffer_size - 1);
		memset(buffer_location, 0, BUFFER_CHUNK_SIZE);
		recvBytes = recv(receivesocket, buffer_location, BUFFER_CHUNK_SIZE,0);
		if(recvBytes != -1 && bytes == -1) bytes = 0; //1st iteration and we have sth (prevents from missing 1 byte)
		if(recvBytes != -1) bytes += recvBytes;//if soth was received, calc overall size of received bytes
		//if(recvBytes > 0) ov_logfile_error("tcpclient/typemethod: ks cmd chunk no %d recv %d bytes, pBuffStart %p, pBuffInside %p",buffer_size, recvBytes, buffer, buffer_location);
	} while(recvBytes == BUFFER_CHUNK_SIZE); // stop if less than maximum bytes was read by recv

	//if(bytes != -1) ov_logfile_error("tcpclient/typemethod: ks cmd w/ size %d received ",bytes);
	

#if OV_SYSTEM_NT
	errno = WSAGetLastError();
#endif
	if(bytes == -1 && errno != EAGAIN && errno != EWOULDBLOCK ) {//ERROR during read - shutdown!
		ov_logfile_error("tcpclient/typemethod: recv error %i, shutdown TCPClient", errno);
		ksservhttp_httpclienthandler_shutdown((OV_INSTPTR_ov_object)cTask);
		//else: bytes = -1 && errno == EAGAIN || EWOULDBLOCK
	} else if(bytes == 0) {//normal shutdown by client
		ov_logfile_debug("tcpclient/typemethod: read 0 bytes - shutdown - %s", ((OV_INSTPTR_ksserv_Client)this)->v_sourceAdr);
		ksservhttp_httpclienthandler_shutdown((OV_INSTPTR_ov_object)cTask);
	//} else if (bytes == -1) {  // no current KS command this turn
	} else if(bytes > 0) {
			//this is the important part - somehting was read on tht tcp buffer

			ksserv_Client_setThisAsCurrent((OV_INSTPTR_ksserv_Client)this); //set this as current one

			cmd = NULL; //the get request without arguments
			body = NULL; //reply *WITHOUT HEADER*
			header = NULL; //header of the reply
			http_version = NULL; //HTTP version
			http_request = NULL;

			//MAIN ROUTINE OF THE WEB SERVER

			//NOTE: this works only for GET, for post one needs to evaluate content-length
			//appending the read chunk to the buffer that is saved between the cycles

			if(ov_string_getlength(this->v_requestbuffer) + ov_string_getlength(buffer) <= MAX_HTTP_REQUEST_SIZE){
				ov_string_append(&(this->v_requestbuffer),buffer);
			}else{
				result = OV_ERR_BADVALUE; //414
				keep_alive = FALSE; //close connection
				//append double line break and let server process the input
				ov_string_append(&(this->v_requestbuffer),"\r\n\r\n");
			}
			//if no double line break detected yet - wait till next cycle
			if(!strstr(this->v_requestbuffer, "\r\n\r\n")){
				free(buffer);
				return;
			}

			ov_string_setvalue(&http_version, "1.1"); //1.1 is default one

			ov_logfile_error("tcpclient/typemethod: got http command w/ %d bytes",bytes);
			//ov_logfile_error("%s", buffer);

			//this->v_requestbuffer contains the raw request
			//split header and footer of the http request
			http_request = ov_string_split(this->v_requestbuffer, "\r\n\r\n", &len);
			//empty the buffer
			ov_string_setvalue(&(this->v_requestbuffer),"");
			//len is always > 0
			//last line of the header will not contain \r\n
			ov_string_append(&(http_request[0]),"\r\n"); //no leak here, valgrind says it

			//http_request[0] is the request header
			//http_request[1]..http_request[len-1] is the request body - will be used for POST requests (not implemented yet)

			//scan header for Connection: close - the default behavior is keep-alive
			if(strstr(http_request[0], "Connection: close\r\n")){
				keep_alive = FALSE;
			}

			//parse request header into get command and arguments request
			if(!Ov_Fail(result)){
				result = parse_http_header(http_request[0], &cmd, &args, &http_version);
			}

			//raw request not needed any longer
			ov_string_freelist(http_request);

			if(!Ov_Fail(result)){
				if(ov_string_compare(cmd, "/getVar") == OV_STRCMP_EQUAL){
					ov_string_setvalue(&header, "Content-Type: text/plain; charset=Windows-1252\r\n");
					result = exec_getvar(&args, &body);
				}else if(ov_string_compare(cmd, "/setVar") == OV_STRCMP_EQUAL){
					ov_string_setvalue(&header, "Content-Type: text/plain; charset=Windows-1252\r\n");
					result = exec_setvar(&args, &body);
				}else if(ov_string_compare(cmd, "/getEP") == OV_STRCMP_EQUAL){
					ov_string_setvalue(&header, "Content-Type: text/plain; charset=Windows-1252\r\n");
					result = exec_getep(&args, &body);
				}else if(ov_string_compare(cmd, "/getHandle") == OV_STRCMP_EQUAL){
					ov_string_setvalue(&header, "Content-Type: text/plain; charset=Windows-1252\r\n");
					result = OV_ERR_BADPATH; //404
					ov_string_append(&body, "Tks-NoHandleSupported");
					//only communication to this server allowed
				}else if(ov_string_compare(cmd, "/delHandle") == OV_STRCMP_EQUAL){
					ov_string_setvalue(&header, "Content-Type: text/plain; charset=Windows-1252\r\n");
					result = OV_ERR_BADPATH; //404
					ov_string_append(&body, "We do not support Handles, so everything is ok.");
				}
			}

			Ov_SetDynamicVectorLength(&args,0,STRING);

			//no command matched yet... Is it a static file?
			if(!Ov_Fail(result) && body == NULL){
				OV_STRING filename = NULL;
				OV_STRING filepath = NULL;
				OV_STRING basepath = NULL;
				//assume index.html as a root file
				if(ov_string_compare("/", cmd) == OV_STRCMP_EQUAL){
					filename = "index.html";
				}else{
					//remove leading /
					filename = cmd + 1;
				}
				ov_memstack_lock();
				//basepath is something like /communication/httpservers/httpserver
				basepath = ov_path_getcanonicalpath((OV_INSTPTR_ov_object)Ov_GetParent(ov_containment,Ov_GetParent(ov_containment,this)), 2); //path to the current client instance
				//a dot in a filename is represented via a percent notation in an identifier,  so we need
				//to change the parameter. A directory should be possible, but so we need to skip / in conversion
				ov_string_print(&filepath, "%s/staticfiles/%s", basepath, ov_path_topercent_noslash(filename));
				ov_memstack_unlock();
				temp = ov_path_getobjectpointer(filepath,2);
				ov_string_setvalue(&filepath, NULL);
				filename = NULL;

				if(temp != NULL && Ov_CanCastTo(ksservhttp_staticfile, temp)){
					staticfile = Ov_StaticPtrCast(ksservhttp_staticfile, temp);
					//adding to the end of the header
					ov_string_print(&header, "Content-Type: %s\r\n", staticfile->v_mimetype);
					result = OV_ERR_OK;
					//body is NULL here
					body = staticfile->v_content;
					static_file = TRUE;
				}else{
					result = OV_ERR_BADPATH;
				}
			}

			//now we have to format the raw http answer
			map_result_to_http(&result, &http_version, &header, &body);
			//Append common data to header:
			ov_string_print(&header, "%sAccess-Control-Allow-Origin:*\r\nServer: ACPLT/OV HTTP Server %s\r\n", header, OV_LIBRARY_DEF_ksservhttp.version);
			//no-cache
			if(static_file == FALSE){
				ov_string_print(&header, "%sPragma: no-cache\r\nCache-Control: no-cache\r\n", header);
			}
			//handle keep_alives
			if (keep_alive == TRUE) {
				ov_string_print(&header, "%sConnection: keep-alive\r\n", header);
			}else{
				ov_string_print(&header, "%sConnection: close\r\n", header);
			}
			//append content length and finalize the header
			ov_string_print(&header, "%sContent-Length: %i\r\n\r\n", header, (int)ov_string_getlength(body));

			//free ressources
			ov_string_setvalue(&cmd, NULL);
			ov_string_setvalue(&http_version, NULL);


			ov_logfile_debug("tcpclient: sending header: %d bytes", (int)ov_string_getlength(header));
			send_tcp(receivesocket, header, (int)ov_string_getlength(header));
			ov_logfile_debug("tcpclient: sending body: %d bytes", (int)ov_string_getlength(body));
			send_tcp(receivesocket, body, (int)ov_string_getlength(body));

			ov_string_setvalue(&header, NULL);

			if(static_file == FALSE){
				ov_string_setvalue(&body, NULL);
			}

			if (keep_alive != TRUE) {
				ksservhttp_httpclienthandler_shutdown((OV_INSTPTR_ov_object)cTask);
			}
			ksserv_Client_unsetThisAsCurrent((OV_INSTPTR_ksserv_Client)this); //unset this as current one
	}
	//free up the memory
	free(buffer);
	return;
}
