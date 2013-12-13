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

#include "config.h"

#define EXEC_GETSERVER_RETURN	Ov_SetDynamicVectorLength(&match,0,STRING);\
		ov_string_setvalue(&servername, NULL);\
		ov_string_setvalue(&http_port, NULL);\
		Ov_SetDynamicVectorLength(&protocols, 0, STRING);\
		Ov_SetDynamicVectorLength(&ports, 0, STRING);\
		return

/**
 * Syntax: /getServer?servername=hallowelt&serverversion=2
 *
 * extracts the command for the getserver and let do ks_server_getserver the job
 * @param args arguments of the http get request
 * @param message pointer to the result string
 * @return resultcode of the operation
 */
OV_RESULT kshttp_exec_getserver(OV_STRING_VEC* const args, OV_STRING* message, OV_UINT response_format){
	/*
	*	parameter and result objects
	*/
	OV_STRING servername = NULL;
	OV_UINT serverversion;
	OV_INSTPTR_ksbase_Manager pManager = NULL;
	OV_STRING_VEC protocols = {0, NULL};
	OV_STRING_VEC ports = {0, NULL};
	OV_UINT	regTTl;
	OV_TIME ExpTime;
	OV_INT registeredVersion;
	OV_UINT i = 0;
	OV_STRING http_port = NULL;

	OV_STRING_VEC match = {0,NULL};
	OV_RESULT fr = OV_ERR_OK;

	//process path
	Ov_SetDynamicVectorLength(&match,0,STRING);
	kshttp_find_arguments(args, "servername", &match);
	if(match.veclen<1){
		fr = KS_ERR_BADPARAM;
		kshttp_print_result_array(message, response_format, &fr, 1, ": Variable servername not found");
		EXEC_GETSERVER_RETURN fr; //400
	}else if(ov_string_compare(match.value[0], NULL) == OV_STRCMP_EQUAL){
		fr = KS_ERR_BADPARAM;
		kshttp_print_result_array(message, response_format, &fr, 1, ": requested servername empty");
		EXEC_GETSERVER_RETURN fr;
	}
	ov_string_setvalue(&servername, match.value[0]);

	Ov_SetDynamicVectorLength(&match,0,STRING);
	kshttp_find_arguments(args, "serverversion", &match);
	if(match.veclen<1){
		serverversion = 2;
	}else{
		serverversion = atoi(match.value[0]);
	}

	pManager = Ov_StaticPtrCast(ksbase_Manager, Ov_GetFirstChild(ov_instantiation, pclass_ksbase_Manager));
	if(!pManager)
	{
		fr = KS_ERR_NOMANAGER;
		kshttp_print_result_array(message, response_format, &fr, 1, ": received Manager command but no Manager here");
		EXEC_GETSERVER_RETURN fr; //400
	}

	/*
	 * check if there is a server of the given name and version of the kshttp protocol (or the whole server if it is the only supported protocol)
	 */
	KS_logfile_debug(("kshttp_getserver: getserver: getting server data."));
	fr = ksbase_Manager_getserverdata(servername, serverversion, &protocols, &ports, &regTTl, &ExpTime, &registeredVersion);
	if(Ov_OK(fr))
	{	//server exists, check if ks protocol is supported
		KS_logfile_debug(("kshttp_getserver: getserver: server exists."));
		for(i=0; i<protocols.veclen; i++){
			if(ov_string_compare(protocols.value[i], KSHTTP_IDENTIFIER) == OV_STRCMP_EQUAL){
				break;
			}
		}

		if(i<protocols.veclen)
		{	/*	kshttp supported	*/
			KS_logfile_debug(("kshttp_getserver: getserver: kshttp supported."));
			ov_string_setvalue(&http_port, ports.value[i]);
		}
		else
		{	/*	kshttp not supported	*/
			//ksbase is happy, but we are not
			fr = KS_ERR_SERVERUNKNOWN;
			KS_logfile_debug(("kshttp_getserver: getserver: kshttp not supported."));
			kshttp_print_result_array(message, response_format, &fr, 1, ": Server unknown via kshttp protocol)");
			EXEC_GETSERVER_RETURN fr; //400
		}
	}
	else
	{
		if(fr == KS_ERR_SERVERUNKNOWN)
		{
			kshttp_print_result_array(message, response_format, &fr, 1, ": Server unknown");
			EXEC_GETSERVER_RETURN fr; //400
		}
		else
		{/*	not all values set...how can this happen?	*/
			kshttp_print_result_array(message, response_format, &fr, 1, ": weird: getserverdata returned unknown error. i don't know how this can happen");
			EXEC_GETSERVER_RETURN KS_ERR_TARGETGENERIC; //400
		}
	}


	kshttp_response_part_begin(message, response_format, "port");
	if(*message == NULL){
		//could be the case at format=plain
		ov_string_setvalue(message, http_port);
	}else{
		ov_string_append(message, http_port);
	}
	kshttp_response_part_finalize(message, response_format, "port");

	kshttp_response_part_begin(message, response_format, "servername");
	ov_string_append(message, servername);
	kshttp_response_part_finalize(message, response_format, "servername");

	kshttp_response_part_begin(message, response_format, "serverversion");
	ov_string_print(message, "%s%u", *message, registeredVersion);
	kshttp_response_part_finalize(message, response_format, "serverversion");

	EXEC_GETSERVER_RETURN fr;
}
