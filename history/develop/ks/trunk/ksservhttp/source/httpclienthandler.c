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

#include "config.h"

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
	if(!this->v_deleted) {// v_deleted cares that socket closing happens just once, not twiche while DeleteObj
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
	//still some duplication, but good enough for now
	case OV_ERR_OK:
		ov_string_print(header, "HTTP/%s %s%s", *http_version, HTTP_200_HEADER, tmp_header);
		break;
	case OV_ERR_BADNAME:
		ov_string_print(header, "HTTP/%s %s%s", *http_version, HTTP_400_HEADER, tmp_header);
		ov_string_print(body, "KS_ERR_BADNAME: %s%s", HTTP_400_BODY, tmp_body);
		break;
	case OV_ERR_BADPARAM:
		ov_string_print(header, "HTTP/%s %s%s", *http_version, HTTP_400_HEADER, tmp_header);
		ov_string_print(body, "KS_ERR_BADPARAM: %s%s", HTTP_400_BODY, tmp_body);
		break;
	case OV_ERR_BADAUTH:
		ov_string_print(header, "HTTP/%s %s%s", *http_version, HTTP_401_HEADER, tmp_header);
		ov_string_print(body, "KS_ERR_BADAUTH: %s%s", HTTP_401_BODY, tmp_body);
		break;
	case OV_ERR_NOACCESS:
		ov_string_print(header, "HTTP/%s %s%s", *http_version, HTTP_403_HEADER, tmp_header);
		ov_string_print(body, "KS_ERR_NOACCESS: %s%s", HTTP_403_BODY, tmp_body);
		break;
	case OV_ERR_BADPATH:
		ov_string_print(header, "HTTP/%s %s%s", *http_version, HTTP_404_HEADER, tmp_header);
		ov_string_print(body, "KS_ERR_BADPATH: %s%s", HTTP_404_BODY, tmp_body);
		break;
	case OV_ERR_BADVALUE:
		ov_string_print(header, "HTTP/%s %s%s", *http_version, HTTP_414_HEADER, tmp_header);
		ov_string_print(body, "KS_ERR_BADVALUE: %s%s", HTTP_414_BODY, tmp_body);
		break;
	case OV_ERR_NOTIMPLEMENTED:
		ov_string_print(header, "HTTP/%s %s%s", *http_version, HTTP_501_HEADER, tmp_header);
		ov_string_print(body, "KS_ERR_NOTIMPLEMENTED: %s%s", HTTP_501_BODY, tmp_body);
		break;
	case OV_ERR_GENERIC:
		ov_string_print(header, "HTTP/%s %s%s", *http_version, HTTP_503_HEADER, tmp_header);
		ov_string_print(body, "KS_ERR_GENERIC: %s%s", HTTP_503_BODY, tmp_body);
		break;
	default:
		ov_string_print(header, "HTTP/%s %s%s", *http_version, HTTP_503_HEADER, tmp_header);
		ov_string_print(body, "KS_ERR_GENERIC: %s%s", HTTP_503_BODY, tmp_body);
		break;
	}

}

#define EXEC_GETVAR_RETURN ov_string_freelist(pPathList); \
		ov_string_freelist(pVarsList); \
		Ov_SetDynamicVectorLength(&match,0,STRING);\
		ov_string_setvalue(&message, NULL);\
		ov_string_setvalue(&prefix, NULL);\
		return

OV_RESULT exec_getvar(OV_STRING_VEC* const args, OV_STRING* re){
	OV_STRING *pVarsList = NULL;
	OV_STRING *pPathList = NULL;
	OV_STRING prefix = NULL;
	OV_UINT len;
	int i;
	int j;
	OV_UINT innerlen;
	OV_STRING_VEC match = {0,NULL};
	OV_RESULT result;
	OV_STRING message = NULL;
	OV_UINT output_format;

	output_format = extract_output_format(args);

	//process path
	Ov_SetDynamicVectorLength(&match,0,STRING);
	find_arguments(args, "path", &match);
	if(match.veclen<1){
		ov_string_append(re, "Variable path not found");
		EXEC_GETVAR_RETURN OV_ERR_BADPARAM; //400
	}
	//process multiple path requests at once
	for(i=0;i<match.veclen;i++){
		//handle empty path requests like path="" to avoid passing NULL around
		if(match.value[i]==NULL)ov_string_setvalue(&match.value[i], " ");
		//separating spacer
		if(i>0)ov_string_append(re, " ");
		//handle the funny tcl syntax path=/vendor.database_name .database_free to get 2 variables at once
		//NOTE: space is %20 here
		pVarsList = ov_string_split((match.value[i]), "%20", &innerlen);
		//pVarsList[0] should be /vendor.database_name

		//process the first portion
		//e.g. /vendor.database_name
		//it is important to separate /vendor as prefix
		//FIXME! does not work with /vendor/database_name but with /vendor.database_name
		if(pPathList != NULL)ov_string_freelist(pPathList);
		pPathList = ov_string_split((pVarsList[0]), ".", &len);
		if(len < 2){
			ov_string_append(re, "First variable name must contain a dot");
			EXEC_GETVAR_RETURN OV_ERR_BADPARAM; //400
		}else if(len == 2){
			ov_string_setvalue(&prefix, pPathList[0]);
		}else{
			ov_string_setvalue(&prefix, pPathList[0]);
			for(i=1;i < len-1;i++){
				ov_string_print(&prefix, "%s.%s", prefix, pPathList[i]);
			}
		}
		result = getvar_to_string(ov_path_getobjectpointer(prefix,2),&(pPathList[len-1]),output_format,&message);
		ov_string_append(re, message);
		//process remaining queries like .database_free
		//no prefix expected here
		for(j=1;j<innerlen;j++){
			if(pPathList != NULL)ov_string_freelist(pPathList);
			pPathList = ov_string_split((pVarsList[j]), ".", &len);
			if(len!=2){
				ov_string_append(re, "Every variable name after a space must start with a dot");
				EXEC_GETVAR_RETURN OV_ERR_BADPARAM; //400
			}
			//this is the actual getvar call
			result = getvar_to_string(ov_path_getobjectpointer(prefix,2),&(pPathList[1]), output_format, &message);
			ov_string_append(re, " "); //one more spacer
			ov_string_append(re, message);
		}
	}

	EXEC_GETVAR_RETURN result;
}

#define EXEC_SETVAR_RETURN ov_string_freelist(pPathList); \
		Ov_SetDynamicVectorLength(&match,0,STRING);\
		ov_string_setvalue(&message, NULL);\
		ov_string_setvalue(&prefix, NULL);\
		return

OV_RESULT exec_setvar(OV_STRING_VEC* args, OV_STRING* re){
	OV_STRING *pPathList = NULL;
	OV_UINT len;
	int i;
	OV_STRING_VEC match = {0,NULL};
	OV_RESULT result;
	OV_STRING message = NULL;
	OV_STRING prefix = NULL;

	find_arguments(args, "path", &match);
	if(match.veclen<1){
		ov_string_append(re, "Variable path not found");
		EXEC_SETVAR_RETURN OV_ERR_BADPARAM; //400
	}
	pPathList = ov_string_split(match.value[0], ".", &len);
	if(len < 2){
		ov_string_freelist(pPathList);
		ov_string_append(re, "Variablename must contain a dot");
		EXEC_SETVAR_RETURN OV_ERR_BADPARAM; //400
	}else if(len == 2){
		ov_string_setvalue(&prefix, pPathList[0]);
	}else{
		ov_string_setvalue(&prefix, pPathList[0]);
		for(i=1;i < len-1;i++){
			ov_string_print(&prefix, "%s.%s", prefix, pPathList[i]);
		}
	}
	find_arguments(args, "newvalue", &match);
	if(match.veclen!=1){
		ov_string_print(re, "Variable newvalue not found");
		EXEC_SETVAR_RETURN OV_ERR_BADPARAM; //400;
	}
	result = setvar_at_object(ov_path_getobjectpointer(prefix,2),&(pPathList[len-1]),&match.value[0], &message);

	ov_string_append(re, message);

	EXEC_SETVAR_RETURN result;
}

#define EXEC_GETEP_RETURN \
		Ov_SetDynamicVectorLength(&match,0,STRING);\
		ov_string_setvalue(&message, NULL);\
		Ov_SetDynamicVectorLength(&requestOutput,0,UINT);\
		ov_string_setvalue(&temp, NULL);\
		return
OV_RESULT exec_getep(OV_STRING_VEC* args, OV_STRING* re){
	OV_INSTPTR_ov_object pObj = NULL;
	OV_INSTPTR_ov_object pChild = NULL;
	OV_VTBLPTR_ov_object pChildVTable;
	OV_STRING_VEC match = {0,NULL};
	OV_STRING message = NULL;
	OV_UINT requestType = KS_OT_DOMAIN;
	OV_UINT_VEC requestOutput = {0,NULL};
	OV_STRING temp = NULL;
	OV_UINT output_format;
	OV_UINT requestOutputDefault[] = {OP_NAME, OP_TYPE, OP_COMMENT, OP_ACCESS, OP_SEMANTIC, OP_CREATIONTIME, OP_CLASS};
	OV_RESULT fr = OV_ERR_OK;
	int i = 0;

	//path=/TechUnits
	//requestType=OT_DOMAIN|OT_VARIABLE|... (siehe tcl-tks doku)
	//requestOutput or requestOutput[i] with OP_NAME, OP_TYPE, OP_COMMENT, OP_ACCESS, OP_SEMANTIC, OP_CREATIONTIME and OP_CLASS

	output_format = extract_output_format(args);

	find_arguments(args, "path", &match);
	if(match.veclen!=1){
		ov_string_append(re, "Variable path not found");
		EXEC_GETEP_RETURN OV_ERR_BADPARAM; //400
	}
	pObj = ov_path_getobjectpointer(match.value[0],0);
	if (pObj == NULL){
		ov_string_append(re, "getEP: Variable not found");
		EXEC_GETEP_RETURN OV_ERR_BADPATH; //404
	}
	find_arguments(args, "requestType", &match);
	if(match.veclen == 1){
		if(ov_string_compare(match.value[0], "OT_DOMAIN") == OV_STRCMP_EQUAL){
			requestType = KS_OT_DOMAIN;
		}else if(ov_string_compare(match.value[0], "OT_VARIABLE") == OV_STRCMP_EQUAL){
			requestType = KS_OT_VARIABLE;
		}else if(ov_string_compare(match.value[0], "OT_LINK") == OV_STRCMP_EQUAL){
			requestType = KS_OT_LINK;
		}else if(ov_string_compare(match.value[0], "OT_ANY") == OV_STRCMP_EQUAL){
			requestType = KS_OT_ANY;
		}else{
			ov_string_append(re, "Requesttype not supported");
			EXEC_GETEP_RETURN OV_ERR_BADPARAM; //400
		}
	}else{
		//default to OT_DOMAIN
		requestType = KS_OT_DOMAIN;
	}
	find_arguments(args, "requestOutput", &match);
	if(match.veclen == 0 || (match.veclen==1 && ov_string_compare(match.value[0], "OP_ANY") == OV_STRCMP_EQUAL )){
		//if nothing is specified or all is requested, give all
		fr = Ov_SetDynamicVectorValue(&requestOutput, requestOutputDefault, 7, UINT);
		if(Ov_Fail(fr)) {
			//should not happen with an UINT
			ov_string_append(re, "internal memory problem");
			EXEC_GETEP_RETURN OV_ERR_BADPATH; //404
		}
	}else{
		//append to requestOutput, to allow search for a match in one string
		for (i=0;i<match.veclen;i++){
			Ov_SetDynamicVectorLength(&requestOutput, i+1, UINT);
			if(ov_string_compare(match.value[i], "OP_NAME") == OV_STRCMP_EQUAL){
				requestOutput.value[i] = OP_NAME;
			}else if(ov_string_compare(match.value[i], "OP_TYPE") == OV_STRCMP_EQUAL){
				requestOutput.value[i] = OP_TYPE;
			}else if(ov_string_compare(match.value[i], "OP_COMMENT") == OV_STRCMP_EQUAL){
				requestOutput.value[i] = OP_COMMENT;
			}else if(ov_string_compare(match.value[i], "OP_ACCESS") == OV_STRCMP_EQUAL){
				requestOutput.value[i] = OP_ACCESS;
			}else if(ov_string_compare(match.value[i], "OP_SEMANTIC") == OV_STRCMP_EQUAL){
				requestOutput.value[i] = OP_SEMANTIC;
			}else if(ov_string_compare(match.value[i], "OP_CREATIONTIME") == OV_STRCMP_EQUAL){
				requestOutput.value[i] = OP_CREATIONTIME;
			}else if(ov_string_compare(match.value[i], "OP_CLASS") == OV_STRCMP_EQUAL){
				requestOutput.value[i] = OP_CLASS;
			}else{
				requestOutput.value[i] = OP_UNKNOWN;
			}
		}
	}

	//idea: reimplement via ov_element_getnextchild, see at ov_ksserver_getep.c
	if(requestType == KS_OT_DOMAIN && Ov_CanCastTo(ov_domain, pObj)){
		//OT_DOMAIN == ov_containment
		Ov_ForEachChild(ov_containment, Ov_StaticPtrCast(ov_domain, pObj), pChild){
			Ov_GetVTablePtr(ov_object, pChildVTable, pChild);
			//open Child item level
			if(Ov_GetPreviousChild(ov_containment, pChild) != NULL && output_format==GETVAR_FORMAT_TCL){
				//append here a space to maintain compatibility with tcl format handling
				ov_string_append(&temp, " ");
			}
			begin_vector_output(&temp, output_format);
			for (i=0;i < requestOutput.veclen;i++){
				if(i >= 1 && output_format==GETVAR_FORMAT_TCL){
					//append here a space to maintain compatibility with tcl format
					ov_string_append(&temp, " ");
				}
				if(requestOutput.veclen > 1){
					//open request item level, if we have more than one entry
					begin_vector_output(&temp, output_format);
				}
				switch(requestOutput.value[i]){
				case OP_NAME:
					ov_string_append(&temp, pChild->v_identifier);
					break;
				case OP_CREATIONTIME:
					ov_string_append(&temp, ov_time_timetoascii(&(pChild->v_creationtime)));
					break;
				case OP_CLASS:
					ov_memstack_lock();
					ov_string_append(&temp, ov_path_getcanonicalpath(Ov_StaticPtrCast(ov_object,Ov_GetClassPtr(pChild)),2));
					ov_memstack_unlock();
					break;
				case OP_TYPE:
				case OP_COMMENT:
					//pChildVTable->m_getcomment(pChild, pelem);
				case OP_ACCESS:
				case OP_SEMANTIC:
					/*	if(pChildVTable->m_getaccess(pChild, &pelem, NULL) & OV_AC_INSTANTIABLE) {

					}
				*/
				default:
					//nothing to do
					break;
				}
				if(requestOutput.veclen > 1){
					//close request item level, if we have more than one entry
					finalize_vector_output(&temp, output_format);
				}
			}
			//close Child item level
			finalize_vector_output(&temp, output_format);
		}
		//save our hard work
		ov_string_setvalue(&message, temp);
		ov_string_setvalue(&temp, NULL);
	}else if(requestType == KS_OT_VARIABLE){
		ov_string_append(re, "requestType VARIABLES not implemented");
		EXEC_GETEP_RETURN OV_ERR_NOTIMPLEMENTED; //501
	}else{
		ov_string_append(re, "requestType not implemented");
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
	OV_BOOL keep_alive = TRUE; //default is to keep the connection open
	OV_UINT request_handled_by = REQUEST_HANDLED_BY_NONE;
	OV_STRING http_version;
	OV_UINT len;
	OV_STRING_VEC match = {0,NULL};

	//vector of the variables, even elements are variable names, odds are values
	OV_STRING_VEC args = {0,NULL};

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

		//START default behaviour
		//scan header for Connection: close - the default behavior is keep-alive
		if(strstr(http_request_header, "Connection: close\r\n")){
			keep_alive = FALSE;
		}
		//default HTTP version
		ov_string_setvalue(&http_version, "1.1");
		//END default behaviour

		//parse request header into get command and arguments request
		if(!Ov_Fail(result)){
			result = parse_http_header(http_request_header, &cmd, &args, &http_version, &http_request_type);
		}

		//todo setvar should be http PUT, createObject und Link POST, UnLink und DeleteObject DELETE
		if(!Ov_Fail(result)){
			result = OV_ERR_NOTIMPLEMENTED;
			//check which kind of request is coming in
			if(	ov_string_compare(http_request_type, "GET") != OV_STRCMP_EQUAL ||
					ov_string_compare(http_request_type, "HEAD") != OV_STRCMP_EQUAL){
				result = OV_ERR_OK;
			}else if(ov_string_compare(http_request_type, "PUSH") != OV_STRCMP_EQUAL){
				result = OV_ERR_OK;
			}else if(ov_string_compare(http_request_type, "OPTIONS") != OV_STRCMP_EQUAL){
				//used for Cross-Origin Resource Sharing (CORS)
				result = OV_ERR_OK;
				//only an 200 is required
				request_handled_by = REQUEST_HANDLED_BY_CORS_OPTION;
			}
		}

		//BEGIN command routine
		if(Ov_OK(result) && request_handled_by == REQUEST_HANDLED_BY_NONE){
			if(ov_string_compare(cmd, "/getVar") == OV_STRCMP_EQUAL){
				//FIXME: a server crashes if http://localhost:8080/getVar?path=/communication/httpservers/httpserver/staticfiles/index.html/.mimetype is called
				//it is caused by the second dot in the filename
				result = exec_getvar(&args, &body);
				//stream required?
				find_arguments(&args, "stream", &match);
				if(match.veclen>0){
					//yes
					ov_string_setvalue(&header, "Content-Type: text/event-stream; charset=Windows-1252\r\n");
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
					ov_string_setvalue(&header, "Content-Type: text/plain; charset=Windows-1252\r\n");
					request_handled_by = REQUEST_HANDLED_BY_GETVAR;
				}
			}else if(ov_string_compare(cmd, "/setVar") == OV_STRCMP_EQUAL){
				ov_string_setvalue(&header, "Content-Type: text/plain; charset=Windows-1252\r\n");
				result = exec_setvar(&args, &body);
				request_handled_by = REQUEST_HANDLED_BY_SETVAR;
			}else if(ov_string_compare(cmd, "/getEP") == OV_STRCMP_EQUAL){
				ov_string_setvalue(&header, "Content-Type: text/plain; charset=Windows-1252\r\n");
				result = exec_getep(&args, &body);
				request_handled_by = REQUEST_HANDLED_BY_GETEP;
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
				if (ov_string_compare(staticfile->v_encoding, "") == OV_STRCMP_EQUAL){
					//adding to the end of the header
					ov_string_print(&header, "Content-Type: %s\r\n", staticfile->v_mimetype);
				}else{
					ov_string_print(&header, "Content-Type: %s; charset=%s\r\n", staticfile->v_mimetype, staticfile->v_encoding);
				}
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
		//now we have to format the raw http answer
		map_result_to_http(&result, &http_version, &header, &body);
		//Append common data to header:
		ov_string_print(&header, "%sAccess-Control-Allow-Origin:*\r\nServer: ACPLT/OV HTTP Server %s\r\n", header, OV_LIBRARY_DEF_ksservhttp.version);
		//no-cache
		if(request_handled_by != REQUEST_HANDLED_BY_STATICFILE){
			ov_string_print(&header, "%sPragma: no-cache\r\nCache-Control: no-cache\r\n", header);
		}
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

		if(request_handled_by != REQUEST_HANDLED_BY_GETVARSTREAM){
			//append content length
			ov_string_print(&header, "%sContent-Length: %i\r\n", header, bodylength);
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
			//speed the processing time down to 500ms
			this->v_cycInterval = 500;
		}

		//in case of a HEAD request there is no need to send the body
		if(ov_string_compare(http_request_type, "HEAD") != OV_STRCMP_EQUAL && body!=NULL){
			ksserv_logfile_debug("httpclienthandler: sending body: %d bytes", (int)ov_string_getlength(body));
			result = send_tcp(receivesocket, body, (int)ov_string_getlength(body));
		}

		//free resources
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
