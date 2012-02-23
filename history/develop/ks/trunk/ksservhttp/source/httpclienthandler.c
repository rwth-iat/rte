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

#include "libov/ov_macros.h"
#include "libov/ov_path.h"
#include "libov/ov_logfile.h"

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
	ov_logfile_error("tcpclient/shutdown: %s", pobj->v_identifier);
	if(!this->v_deleted) {// v_deleted cares that socket closing happens just once, not twiche while DeleteObj
		receivesocket = ksservhttp_httpclienthandler_receivesocket_get(this);
		if(receivesocket < 0) {
			ov_logfile_error("tcpclient/shutdown: %s has socket<0 - cant shutdown!?!", pobj->v_identifier);
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

void map_result_to_http(OV_RESULT* result, OV_STRING* re){
	switch (*result)
		{
			case OV_ERR_OK:
				ov_string_setvalue(re, HTTP_200_HEADER);
				break;
			case OV_ERR_GENERIC:
				ov_string_setvalue(re, HTTP_503_ERROR);
				break;
			case OV_ERR_BADNAME:
				ov_string_setvalue(re, HTTP_400_ERROR);
				break;
			case OV_ERR_BADPARAM:
				ov_string_setvalue(re, HTTP_400_ERROR);
				break;
			case OV_ERR_BADPATH:
				ov_string_setvalue(re, HTTP_404_ERROR);
				break;
			default:
				ov_string_setvalue(re, HTTP_503_ERROR);
				break;
    }
}

#define EXEC_GETVAR_RETURN ov_string_freelist(plist); \
						Ov_SetDynamicVectorLength(&match,0,STRING);\
						ov_string_setvalue(&message, NULL);\
						return

void exec_getvar(OV_STRING_VEC* args, OV_STRING* re){
	OV_STRING *plist = NULL;
	OV_UINT len;
	OV_STRING_VEC match = {0,NULL};
	OV_RESULT result;
	OV_STRING message = NULL;

	if(args->veclen != 2){
    	ov_string_setvalue(re, HTTP_400_ERROR); //not enough params - need a name!
    	ov_string_print(re, "%s\nNumber of parameter should be 1, but is %i", *re, args->veclen/2);
    	EXEC_GETVAR_RETURN;
	}

	find_arguments(args, "path", &match);
	if(match.veclen!=1){
    	ov_string_setvalue(re, HTTP_400_ERROR);
    	ov_string_append(re, "\nVariable path not found");
    	EXEC_GETVAR_RETURN;
	}
    plist = ov_string_split((match.value[0]), ".", &len);
    if(len!=2){
    	ov_string_setvalue(re, HTTP_400_ERROR); //no . found, thus no variable name given
    	ov_string_freelist(plist);
    	ov_string_append(re, "\nVariablename must contain a dot");
    	EXEC_GETVAR_RETURN;
    }
    result = getvar_to_string(ov_path_getobjectpointer(plist[0],2),&(plist[1]),&message);

    map_result_to_http(&result, re);
    ov_string_append(re, "\n");
   	ov_string_append(re, message);

    EXEC_GETVAR_RETURN;
}

#define EXEC_SETVAR_RETURN ov_string_freelist(plist); \
						Ov_SetDynamicVectorLength(&match,0,STRING);\
						ov_string_setvalue(&message, NULL);\
						return

void exec_setvar(OV_STRING_VEC* args, OV_STRING* re){
	OV_STRING *plist = NULL;
	OV_UINT len;
	OV_STRING_VEC match = {0,NULL};
	OV_RESULT result;
	OV_STRING message = NULL;

	if(args->veclen != 4){
    	ov_string_setvalue(re, HTTP_400_ERROR); //not enough params - need a name!
    	ov_string_print(re, "%s\nNumber of parameter should be 2, but is %i", *re, args->veclen/2);
    	EXEC_SETVAR_RETURN;
	}
	find_arguments(args, "path", &match);
	if(match.veclen!=1){
    	ov_string_setvalue(re, HTTP_400_ERROR);
    	ov_string_print(re, "%s\nVariable path not found", *re);
    	EXEC_SETVAR_RETURN;
	}
    plist = ov_string_split(match.value[0], ".", &len);
    if(len!=2){
    	ov_string_setvalue(re, HTTP_400_ERROR); //no . found, thus no variable name given
    	ov_string_freelist(plist);
    	ov_string_append(re, "\nVariablename must contain a dot");
    	EXEC_SETVAR_RETURN;
    }
	find_arguments(args, "newvalue", &match);
	if(match.veclen!=1){
    	ov_string_setvalue(re, HTTP_400_ERROR);
    	ov_string_print(re, "%s\nVariable newvalue not found", *re);
    	EXEC_SETVAR_RETURN;
	}
	result = setvar_at_object(ov_path_getobjectpointer(plist[0],2),&(plist[1]),&match.value[0], &message);

    map_result_to_http(&result, re);
    ov_string_append(re, "\n");
   	ov_string_append(re, message);

    EXEC_SETVAR_RETURN;
}

#define EXEC_GETEP_RETURN \
						Ov_SetDynamicVectorLength(&match,0,STRING);\
						ov_string_setvalue(&message, NULL);\
						return
void exec_getep(OV_STRING_VEC* args, OV_STRING* re){
	OV_INSTPTR_ov_object pObj = NULL;
	OV_INSTPTR_ov_object pChild = NULL;
	OV_STRING_VEC match = {0,NULL};
	OV_STRING message = NULL;

	//muss noch aufgebohrt werden:
	//path=/TechUnits
	//objectType=OT_DOMAINS|OT_VARIABLES|... (siehe tcl-tks doku)
	//TODO: outputinfos (anderer name muss her) ein bis sieben informatinen liefert tcl-tks
	if(args->veclen != 4){
    	ov_string_setvalue(re, HTTP_400_ERROR); //not enough params - need a name!
    	ov_string_print(re, "%s\nNumber of parameter should be 2, but is %i", *re, args->veclen/2);
    	EXEC_GETEP_RETURN;
	}

	find_arguments(args, "path", &match);
	if(match.veclen!=1){
    	ov_string_setvalue(re, HTTP_400_ERROR);
    	ov_string_append(re, "\nVariable path not found");
    	EXEC_GETEP_RETURN;
	}
	pObj = ov_path_getobjectpointer(match.value[0],0);
	if (pObj == NULL){
    	ov_string_setvalue(re, HTTP_404_ERROR);
    	ov_string_append(re, "\nVariable not found");
    	EXEC_GETEP_RETURN;
	}
	find_arguments(args, "objectType", &match);
	if(match.veclen!=1){
    	ov_string_setvalue(re, HTTP_400_ERROR);
    	ov_string_append(re, "\nobjectType not found");
    	EXEC_GETEP_RETURN;
	}
	if(ov_string_compare(match.value[0], "OT_DOMAINS") == OV_STRCMP_EQUAL
			&& Ov_CanCastTo(ov_domain, pObj)){
		Ov_ForEachChild(ov_containment, Ov_StaticPtrCast(ov_domain, pObj), pChild){
			if (message == NULL){
				ov_string_print(&message, "{%s}\n", pChild->v_identifier);
			}else{
				ov_string_print(&message, "%s{%s}", message, pChild->v_identifier);
			}
		}
	}else if(ov_string_compare(match.value[0], "OT_VARIABLES") == OV_STRCMP_EQUAL){
    	ov_string_setvalue(re, HTTP_503_ERROR);
    	ov_string_append(re, "\nobjectType VARIABLES not implemented");
    	EXEC_GETEP_RETURN;
	}else{
    	ov_string_setvalue(re, HTTP_503_ERROR);
    	ov_string_append(re, "\nobjectType not implemented");
    	EXEC_GETEP_RETURN;
	}
	ov_string_setvalue(re, HTTP_200_HEADER);
	ov_string_append(re, "\n");
   	ov_string_append(re, message);

	EXEC_GETEP_RETURN;
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
    char* placeInBuffer = NULL;
    OV_STRING reply, cmd, rawrequest;
    OV_RESULT fr;
    //vector of the variables, even elements are variable names, odds are values
    OV_STRING_VEC args = {0,NULL};

	int sentChunkSize = 0;
	int sentBytes = 0;
	int size_return = 0;

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
			//this is the important part - http request was read successfully
			//MAIN ROUTINE OF THE WEB SERVER

			ksserv_Client_setThisAsCurrent((OV_INSTPTR_ksserv_Client)this); //set this as current one

			ov_string_setvalue(&rawrequest, buffer);

			//buffer contains the raw request

			ov_logfile_error("tcpclient/typemethod: got http command w/ %d bytes",bytes);
			//ov_logfile_error("%s", rawrequest);

			reply = NULL; //raw reply to send via TCP
			cmd = NULL; //the get request without arguments

		    //parse request into get command and arguments request
			fr = parse_http_request(rawrequest, &cmd, &args);
			ov_string_setvalue(&rawrequest, NULL);
			if(Ov_Fail(fr)){
				reply = HTTP_400_ERROR;
			}

			if(ov_string_compare(cmd, "/getVar") == OV_STRCMP_EQUAL){
				exec_getvar(&args, &reply);
			}else if(ov_string_compare(cmd, "/setVar") == OV_STRCMP_EQUAL){
				exec_setvar(&args, &reply);
			}else if(ov_string_compare(cmd, "/getEP") == OV_STRCMP_EQUAL){
				exec_getep(&args, &reply);
			}else if(ov_string_compare(cmd, "/getHandle") == OV_STRCMP_EQUAL){
				ov_string_append(&reply, HTTP_200_HEADER);
				ov_string_append(&reply, "\n");
				ov_string_append(&reply, "Tks-NoHandleSupported");
				//only communication to this server allowed
			}else if(ov_string_compare(cmd, "/delHandle") == OV_STRCMP_EQUAL){
				ov_string_append(&reply, HTTP_200_HEADER);
				ov_string_append(&reply, "\n");
				ov_string_append(&reply, "We do not support Handles, so everything is ok.");
			}

			//no command matched yet... Is it a static file?
			if(reply == NULL){
				OV_INSTPTR_ov_domain pstaticfiles;
				OV_INSTPTR_ov_domain thisdomain = Ov_GetParent(ov_containment, Ov_GetParent(ov_containment, this));
				OV_STRING filename;
				//assume index.html as a root file
				if(ov_string_compare("/", cmd) == OV_STRCMP_EQUAL){
					filename = "index.html";
				}else{
					filename = cmd + 1;
				}
				pstaticfiles = (OV_INSTPTR_ov_domain) Ov_SearchChild(ov_containment, thisdomain, "staticfiles");
				temp = Ov_SearchChild(ov_containment, pstaticfiles, filename);
				if(temp != NULL && Ov_CanCastTo(ksservhttp_staticfile, temp)){
					staticfile = Ov_StaticPtrCast(ksservhttp_staticfile, temp);
					ov_string_append(&reply, HTTP_200_HEADER);
					ov_string_append(&reply, "Content-type: ");
					ov_string_append(&reply, staticfile->v_mimetype);
					ov_string_append(&reply, "\n\n");
					ov_string_append(&reply, staticfile->v_content);
				}
			}

			//no reply formed yet till now -- 404 error -- default behavior
			if(reply == NULL){
				reply = HTTP_404_ERROR;
			}

			//pushing answer back to client through the tcp socket
			size_return = ov_string_getlength(reply);
			placeInBuffer = reply;

			ov_logfile_debug("tcpclient: sending answer: %d bytes", (size_return));
			do
			{
				if((size_return - sentBytes) > 4096)
				{
					sentChunkSize = send(receivesocket, placeInBuffer, 4096, 0);
					if (sentChunkSize == -1)
					{
						ov_logfile_error("send() failed");
						break;
					}
				}
				else
				{
					sentChunkSize = send(receivesocket, placeInBuffer, (size_return - sentBytes), 0);
					if (sentChunkSize == -1)
					{
						ov_logfile_error("send() failed");
						break;
					}
				}
				sentBytes += sentChunkSize;
				ov_logfile_debug("tcpclient: answer sent, sentChunkSize: %d sentBytes: %d", sentChunkSize, sentBytes);
				//move pointer to next chunk
				placeInBuffer = &(reply[sentBytes]);

				if(sentBytes < size_return)
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
			}while(sentBytes < size_return);


			ksservhttp_httpclienthandler_shutdown((OV_INSTPTR_ov_object)cTask);
			ksserv_Client_unsetThisAsCurrent((OV_INSTPTR_ksserv_Client)this); //unset this as current one
	}
	//free up the memory
	ov_string_setvalue(&reply, NULL);
	ov_string_setvalue(&cmd, NULL);
	Ov_SetDynamicVectorLength(&args,0,STRING);
	free(buffer);
	return;
}
