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

#include "config.h"

#define EXEC_CREATEOBJECT_RETURN	Ov_SetDynamicVectorLength(&match,0,STRING);\
		Ov_SetDynamicVectorLength(&factorymatch,0,STRING);\
		Ov_SetDynamicVectorLength(&pmhmatch,0,STRING);\
		Ov_SetDynamicVectorLength(&pmhpathmatch,0,STRING);\
		ov_string_setvalue(&temp, NULL);\
		return

/**
 * extracts the (multiple) commands for the create and let do ks_server_create the job
 * @param args arguments of the http get request
 * @param responseBody pointer to the result string
 * @return resultcode of the operation
 */
OV_RESULT kshttp_exec_createObject(OV_STRING_VEC* const args, OV_STRING* responseBody, KSHTTP_RESPONSEFORMAT const response_format){
	/*
	*	parameter and result objects
	*/
	OV_STRING_VEC match = {0,NULL};
	OV_STRING_VEC factorymatch = {0,NULL};
	OV_STRING_VEC pmhmatch = {0,NULL};
	OV_STRING_VEC pmhpathmatch = {0,NULL};
	OV_CREATEOBJ_ITEM *addrp = NULL;
	OV_UINT i = 0;
	OV_RESULT fr = OV_ERR_OK;
	OV_STRING temp = NULL;

	OV_CREATEOBJECT_PAR	params;
	OV_CREATEOBJECT_RES	result;

	OV_TICKET* pticket = NULL;

	//process path
	Ov_SetDynamicVectorLength(&match,0,STRING);
	kshttp_find_arguments(args, "path", &match);
	if(match.veclen<1){
		fr = OV_ERR_BADPARAM;
		kshttp_print_result_array(responseBody, response_format, &fr, 1, ": Variable path not found");
		EXEC_CREATEOBJECT_RETURN fr; //400
	}
	//process factory
	Ov_SetDynamicVectorLength(&factorymatch,0,STRING);
	kshttp_find_arguments(args, "factory", &factorymatch);
	if(factorymatch.veclen<1){
		fr = OV_ERR_BADPARAM;
		kshttp_print_result_array(responseBody, response_format, &fr, 1, ": Variable factory not found");
		EXEC_CREATEOBJECT_RETURN fr; //400
	}
	//process Placement hint
	Ov_SetDynamicVectorLength(&pmhmatch,0,STRING);
	kshttp_find_arguments(args, "placementHint", &pmhmatch);
	Ov_SetDynamicVectorLength(&pmhpathmatch,0,STRING);
	kshttp_find_arguments(args, "placePath", &pmhpathmatch);

	ov_memstack_lock();
	addrp = (OV_CREATEOBJ_ITEM*)ov_memstack_alloc(match.veclen*sizeof(OV_CREATEOBJ_ITEM));
	if(!addrp) {
		ov_memstack_unlock();
		fr = OV_ERR_TARGETGENERIC;
		kshttp_print_result_array(responseBody, response_format, &fr, 1, ": memory problem");
		EXEC_CREATEOBJECT_RETURN fr;
	}

	params.items_val = addrp;
	params.items_len = match.veclen;

	//process multiple path requests at once
	for(i=0;i<match.veclen;i++){
		addrp->new_path = match.value[i];
		//allow fewer factory parameters for multiple createObjects
		if(i<factorymatch.veclen){
			//set factory to matching parameter
			addrp->factory_path = factorymatch.value[i];
		}else{
			//set factory to the last factory given
			addrp->factory_path = factorymatch.value[factorymatch.veclen-1];
		}

		//placement hint
		if(i<pmhmatch.veclen){
			if(ov_string_compare(pmhmatch.value[i], "PMH_DEFAULT") == OV_STRCMP_EQUAL){
				addrp->place.hint = KS_PMH_DEFAULT;
			}else if(ov_string_compare(pmhmatch.value[i], "PMH_BEGIN") == OV_STRCMP_EQUAL){
				addrp->place.hint = KS_PMH_BEGIN;
			}else if(ov_string_compare(pmhmatch.value[i], "PMH_END") == OV_STRCMP_EQUAL){
				addrp->place.hint = KS_PMH_END;
			}else if(ov_string_compare(pmhmatch.value[i], "PMH_BEFORE") == OV_STRCMP_EQUAL && match.veclen<=pmhpathmatch.veclen){
				addrp->place.hint = KS_PMH_BEFORE;
				addrp->place.place_path = pmhpathmatch.value[i];
			}else if(ov_string_compare(pmhmatch.value[i], "PMH_AFTER") == OV_STRCMP_EQUAL && match.veclen<=pmhpathmatch.veclen){
				addrp->place.hint = KS_PMH_AFTER;
				addrp->place.place_path = pmhpathmatch.value[i];
			}else{
				addrp->place.hint = KS_PMH_DEFAULT;
			}
		}else{
			addrp->place.hint = KS_PMH_DEFAULT;
		}

		//todo, but a lot of work: do allow default value/links
		addrp->parameters_len = 0;
		addrp->parameters_val = NULL;
		addrp->links_len = 0;
		addrp->links_val = NULL;

		//add one size of a pointer
		addrp ++;
	}

	//create NONE-ticket
	pticket = ksbase_NoneAuth->v_ticket.vtbl->createticket(NULL, OV_TT_NONE);

	ov_ksserver_createobject(2, pticket, &params, &result);

	/*	delete Ticket	*/
	pticket->vtbl->deleteticket(pticket);

	/**
	 * Parse result from KS function
	 */
	if(Ov_Fail(result.result)){
		//general problem like memory problem or NOACCESS
		kshttp_print_result_array(responseBody, response_format, &result.result, 1, ": general problem");
		ov_memstack_unlock();
		EXEC_CREATEOBJECT_RETURN fr;
	}
	for (i=0; i< result.obj_results_len;i++){
		fr = (result.obj_results_val+i)->result;
		if(fr == OV_ERR_GENERIC){
			kshttp_print_result_array(&temp, response_format, &fr, 1, ": perhaps a base library is not loaded");
		}else{
			kshttp_print_result_array(&temp, response_format, &fr, 1, "");
		}
	}
	ov_memstack_unlock();
	ov_string_append(responseBody, temp);
	EXEC_CREATEOBJECT_RETURN fr;
}
