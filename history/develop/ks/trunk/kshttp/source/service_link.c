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

#define EXEC_LINK_RETURN	Ov_SetDynamicVectorLength(&match,0,STRING);\
		Ov_SetDynamicVectorLength(&elementmatch,0,STRING);\
		Ov_SetDynamicVectorLength(&placehintmatch,0,STRING);\
		Ov_SetDynamicVectorLength(&placepathmatch,0,STRING);\
		Ov_SetDynamicVectorLength(&oppositeplacehintmatch,0,STRING);\
		Ov_SetDynamicVectorLength(&oppositeplacepathmatch,0,STRING);\
		ov_string_setvalue(&temp, NULL);\
		return

/**
 * Syntax: /link?path=/TechUnits/con.sourcefb&element=/TechUnits/testFB
 * placehint=BEGIN
 * placehint=BEFORE&placepath=/TechUnits/add
 * oppositeplacehint=AFTER&oppositeplacepath=/TechUnits/add2
 *
 *
 * extracts the command for the linking and let do ks_server_link the job
 * @param args arguments of the http get request
 * @param message pointer to the result string
 * @return resultcode of the operation
 */
OV_RESULT kshttp_exec_link(OV_STRING_VEC* const args, OV_STRING* message, KSHTTP_RESPONSEFORMAT response_format){
	/*
	*	parameter and result objects
	*/
	OV_STRING_VEC match = {0,NULL};
	OV_STRING_VEC elementmatch = {0,NULL};
	OV_STRING_VEC placehintmatch = {0,NULL};
	OV_STRING_VEC placepathmatch = {0,NULL};
	OV_STRING_VEC oppositeplacehintmatch = {0,NULL};
	OV_STRING_VEC oppositeplacepathmatch = {0,NULL};

	OV_LINK_ITEM *addrp = NULL;
	OV_UINT i = 0;
	OV_RESULT fr = OV_ERR_OK;
	OV_STRING temp = NULL;

	OV_LINK_PAR	params;
	OV_LINK_RES	result;
	OV_TICKET* pticket = NULL;

	//process path
	Ov_SetDynamicVectorLength(&match,0,STRING);
	kshttp_find_arguments(args, "path", &match);
	if(match.veclen<1){
		fr = OV_ERR_BADPARAM;
		kshttp_print_result_array(message, response_format, &fr, 1, ": Variable path not found");
		EXEC_LINK_RETURN fr; //400
	}
	//process element
	Ov_SetDynamicVectorLength(&elementmatch,0,STRING);
	kshttp_find_arguments(args, "element", &elementmatch);
	if(elementmatch.veclen < match.veclen){
		fr = OV_ERR_BADPARAM;
		kshttp_print_result_array(message, response_format, &fr, 1, ": not enough Variables element found: ");
		EXEC_LINK_RETURN fr; //400
	}


	ov_memstack_lock();
	addrp = (OV_LINK_ITEM*)ov_memstack_alloc(match.veclen*sizeof(OV_LINK_ITEM));
	if(!addrp) {
		ov_memstack_unlock();
		fr = OV_ERR_TARGETGENERIC;
		kshttp_print_result_array(message, response_format, &fr, 1, ": memory problem");
		EXEC_LINK_RETURN fr;

	}

	params.items_val = addrp;
	params.items_len = match.veclen;

	kshttp_find_arguments(args, "placehint", &placehintmatch);
	kshttp_find_arguments(args, "placepath", &placepathmatch);
	kshttp_find_arguments(args, "oppositeplacehint", &oppositeplacehintmatch);
	kshttp_find_arguments(args, "oppositeplacepath", &oppositeplacepathmatch);

	//process multiple path requests at once
	for(i=0;i<match.veclen;i++){
		addrp->link_path = match.value[i];
		addrp->element_path = elementmatch.value[i];

		//handle placement hint if available, start with the default
		addrp->place.hint = KS_PMH_DEFAULT;
		addrp->place.place_path = NULL;
		addrp->opposite_place.hint = KS_PMH_DEFAULT;
		addrp->opposite_place.place_path = NULL;
		if(placehintmatch.veclen >= match.veclen){
			ov_memstack_lock();
			ov_string_setvalue(&temp, ov_string_toupper(placehintmatch.value[i]));
			ov_memstack_unlock();
			if(ov_string_compare(temp, "DEFAULT") == OV_STRCMP_EQUAL){
				//change nothing
			}else if(ov_string_compare(temp, "BEGIN") == OV_STRCMP_EQUAL){
				addrp->place.hint = KS_PMH_BEGIN;
			}else if(ov_string_compare(temp, "END") == OV_STRCMP_EQUAL){
				addrp->place.hint = KS_PMH_END;
			}else if(ov_string_compare(temp, "BEFORE") == OV_STRCMP_EQUAL && ov_string_compare(placepathmatch.value[i], NULL) != OV_STRCMP_EQUAL){
				//BEFORE needs an relative placement partner, too
				addrp->place.hint = KS_PMH_BEFORE;
				addrp->place.place_path = placepathmatch.value[i]; //save to set direct, as the StrVec will be cleared right before ending this function
			}else if(ov_string_compare(temp, "AFTER") == OV_STRCMP_EQUAL && ov_string_compare(placepathmatch.value[i], NULL) != OV_STRCMP_EQUAL){
				//AFTER needs an relative placement partner, too
				addrp->place.hint = KS_PMH_AFTER;
				addrp->place.place_path = placepathmatch.value[i]; //save to set direct, as the StrVec will be cleared right before ending this function
			}
			if(oppositeplacehintmatch.veclen >= match.veclen){
				ov_memstack_lock();
				ov_string_setvalue(&temp, ov_string_toupper(oppositeplacehintmatch.value[i]));
				ov_memstack_unlock();

				if(ov_string_compare(temp, "DEFAULT") == OV_STRCMP_EQUAL){
					//change nothing
				}else if(ov_string_compare(temp, "BEGIN") == OV_STRCMP_EQUAL){
					addrp->opposite_place.hint = KS_PMH_BEGIN;
				}else if(ov_string_compare(temp, "END") == OV_STRCMP_EQUAL){
					addrp->opposite_place.hint = KS_PMH_END;
				}else if(ov_string_compare(temp, "BEFORE") == OV_STRCMP_EQUAL && ov_string_compare(oppositeplacepathmatch.value[i], NULL) != OV_STRCMP_EQUAL){
					//BEFORE needs an relative placement partner, too
					addrp->opposite_place.hint = KS_PMH_BEFORE;
					addrp->opposite_place.place_path = oppositeplacepathmatch.value[i]; //save to set direct, as the StrVec will be cleared right before ending this function
				}else if(ov_string_compare(temp, "AFTER") == OV_STRCMP_EQUAL && ov_string_compare(oppositeplacepathmatch.value[i], NULL) != OV_STRCMP_EQUAL){
					//AFTER needs an relative placement partner, too
					addrp->opposite_place.hint = KS_PMH_AFTER;
					addrp->opposite_place.place_path = oppositeplacepathmatch.value[i]; //save to set direct, as the StrVec will be cleared right before ending this function
				}
			}
		}
		//add one size of a pointer
		addrp ++;
	}

	//create NONE-ticket
	pticket = ksbase_NoneAuth->v_ticket.vtbl->createticket(NULL, OV_TT_NONE);

	ov_ksserver_link(2, pticket, &params, &result);

	/*	delete Ticket	*/
	pticket->vtbl->deleteticket(pticket);

	/**
	 * Parse result from KS function
	 */
	if(Ov_Fail(result.result)){
		//general problem like memory problem or NOACCESS
		kshttp_print_result_array(message, response_format, &result.result, 1, ": general problem");
		ov_memstack_unlock();
		EXEC_LINK_RETURN result.result;
	}

	fr = kshttp_print_result_array(message, response_format, result.results_val, result.results_len, "");
	ov_memstack_unlock();
	EXEC_LINK_RETURN fr;
}
