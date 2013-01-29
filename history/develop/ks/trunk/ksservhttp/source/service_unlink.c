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

static OV_ACCESS ov_ksservhttp_ticket_defaultticket_getaccess(const OV_TICKET *a) {
	return KS_AC_UNLINKABLE;
}

//we need ony a getaccess for the getVar service
OV_DLLVAREXPORT OV_TICKET_VTBL defaultticketvtblUnlink = {
	NULL,
	NULL,
	NULL,
	ov_ksservhttp_ticket_defaultticket_getaccess
};

#define EXEC_UNLINK_RETURN	Ov_SetDynamicVectorLength(&match,0,STRING);\
		Ov_SetDynamicVectorLength(&elementmatch,0,STRING);\
		ov_string_setvalue(&temp, NULL);\
		return

/**
 * Syntax: /unlink?path=/TechUnits/con.sourcefb&element=/TechUnits/testFB
 *
 * extracts the command for the linking and let do ks_server_link the job
 * @param args arguments of the http get request
 * @param message pointer to the result string
 * @return resultcode of the operation
 */
OV_RESULT exec_unlink(OV_STRING_VEC* const args, OV_STRING* message, OV_UINT response_format){
	/*
	*	parameter and result objects
	*/
	OV_STRING_VEC match = {0,NULL};
	OV_STRING_VEC elementmatch = {0,NULL};
	OV_UNLINK_ITEM *addrp = NULL;
	OV_UINT i = 0;
	OV_RESULT fr = OV_ERR_OK;
	OV_STRING temp = NULL;

	OV_UNLINK_PAR	params;
	OV_UNLINK_RES	result;

	static OV_TICKET ticket = { &defaultticketvtblUnlink,  OV_TT_NONE };

	//process path
	Ov_SetDynamicVectorLength(&match,0,STRING);
	find_arguments(args, "path", &match);
	if(match.veclen<1){
		begin_vector_output(message, response_format, "failure");
		if(response_format == RESPONSE_FORMAT_KSX){
			ov_string_print(&temp, "%i", OV_ERR_BADPARAM);
		}else{
			ov_string_print(&temp, "Variable path not found");
		}
		finalize_vector_output(&temp, response_format, "failure");
		ov_string_append(message, temp);
		EXEC_UNLINK_RETURN OV_ERR_BADPARAM; //400
	}
	//process element
	Ov_SetDynamicVectorLength(&elementmatch,0,STRING);
	find_arguments(args, "element", &elementmatch);
	if(elementmatch.veclen < match.veclen){
		begin_vector_output(message, response_format, "failure");
		if(response_format == RESPONSE_FORMAT_KSX){
			ov_string_print(&temp, "%i", OV_ERR_BADPARAM);
		}else{
			ov_string_print(&temp, "not enough Variables element found");
		}
		finalize_vector_output(&temp, response_format, "failure");
		ov_string_append(message, temp);
		EXEC_UNLINK_RETURN OV_ERR_BADPARAM; //400
	}

	ov_memstack_lock();
	addrp = (OV_UNLINK_ITEM*)ov_memstack_alloc(match.veclen*sizeof(OV_UNLINK_ITEM));
	if(!addrp) {
		ov_memstack_unlock();
		EXEC_UNLINK_RETURN OV_ERR_TARGETGENERIC;
	}

	params.items_val = addrp;
	params.items_len = match.veclen;

	//process multiple path requests at once
	for(i=0;i<match.veclen;i++){
		addrp->link_path = match.value[i];
		addrp->element_path = elementmatch.value[i];

		//add one size of a pointer
		addrp ++;
	}

	ov_ksserver_unlink(2, &ticket, &params, &result);

	/**
	 * Parse result from KS function
	 */
	if(Ov_Fail(result.result)){
		//general problem like memory problem or NOACCESS
		ov_memstack_unlock();
		fr = result.result;
		begin_vector_output(message, response_format, "failure");
		if(response_format == RESPONSE_FORMAT_KSX){
			ov_string_print(&temp, "%i", fr);
		}else{
			ov_string_print(&temp, "problem: %s", ov_result_getresulttext(fr));
		}
		finalize_vector_output(&temp, response_format, "failure");
		ov_string_append(message, temp);
		EXEC_UNLINK_RETURN fr;
	}

	for (i=0; i< result.results_len;i++){
		if(Ov_Fail(result.results_val[i])){
			fr = result.results_val[i];
			begin_vector_output(&temp, response_format, "failure");
			if(response_format == RESPONSE_FORMAT_KSX){
				ov_string_print(&temp, "%i", fr);
			}else{
				ov_string_print(&temp, "problem: %s", ov_result_getresulttext(fr));
			}
			finalize_vector_output(&temp, response_format, "failure");
		}else{
			begin_vector_output(&temp, response_format, "success");
			finalize_vector_output(&temp, response_format, "success");
		}
	}

	ov_memstack_unlock();
	ov_string_append(message, temp);
	EXEC_UNLINK_RETURN fr;
}
