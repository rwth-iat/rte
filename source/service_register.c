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

#define EXEC_REGISTER_RETURN	Ov_SetDynamicVectorLength(&match,0,STRING);\
		ov_string_setvalue(&servername, NULL);\
		ov_string_setvalue(&portstr, NULL);\
		return

/**
 * Syntax: /register?name=dieter&port=7510&ksversion=2
 *
 * extracts the command for the register and process
 * @param args arguments of the http get request
 * @param message pointer to the result string
 * @return resultcode of the operation
 */
OV_RESULT kshttp_exec_register(OV_STRING_VEC* const args, OV_STRING* message, OV_UINT response_format){
	/*
	*	parameter and result objects
	*/
	OV_INSTPTR_ksbase_Manager pManager = NULL;
	OV_STRING_VEC match = {0,NULL};
	OV_RESULT fr = OV_ERR_OK;
	OV_STRING servername = NULL;
	OV_UINT serverversion;
	OV_UINT serverport;
	OV_STRING portstr = NULL;

	//process name
	Ov_SetDynamicVectorLength(&match,0,STRING);
	kshttp_find_arguments(args, "name", &match);
	if(match.veclen<1){
		fr = OV_ERR_BADPARAM;
		kshttp_print_result_array(message, response_format, &fr, 1, ": Variable name not found");
		EXEC_REGISTER_RETURN fr; //400
	}else if(ks_isvalidname(match.value[0])){
		fr = OV_ERR_BADPARAM;
		kshttp_print_result_array(message, response_format, &fr, 1, ": Server name not valid");
		EXEC_REGISTER_RETURN fr; //400
	}else{
		ov_string_setvalue(&servername, match.value[0]);
	}
	//process serverport
	Ov_SetDynamicVectorLength(&match,0,STRING);
	kshttp_find_arguments(args, "port", &match);
	if(match.veclen<1 || match.value[0] == NULL){
		fr = OV_ERR_BADPARAM;
		kshttp_print_result_array(message, response_format, &fr, 1, ": Variable port not found");
		EXEC_REGISTER_RETURN fr; //400
	}else{
		serverport = atoi(match.value[0]);
	}
	//process ksversion
	Ov_SetDynamicVectorLength(&match,0,STRING);
	kshttp_find_arguments(args, "version", &match);
	if(match.veclen<1 || match.value[0] == NULL){
		serverversion = 2;
	}else{
		serverversion = atoi(match.value[0]);
	}

	pManager = Ov_StaticPtrCast(ksbase_Manager, Ov_GetFirstChild(ov_instantiation, pclass_ksbase_Manager));
	if(!pManager){
		fr = KS_ERR_NOMANAGER;
		kshttp_print_result_array(message, response_format, &fr, 1, ": received Manager command but no Manager here");
		EXEC_REGISTER_RETURN fr;
	}

	ov_string_print(&portstr, "%lu", serverport);
	fr = ksbase_Manager_register(servername, serverversion, KSHTTP_IDENTIFIER, portstr, 30);
	if(Ov_Fail(fr)){
		kshttp_print_result_array(message, response_format, &fr, 1, ": could not register server at manager.");
		EXEC_REGISTER_RETURN fr;
	}
	EXEC_REGISTER_RETURN OV_ERR_OK;
}
