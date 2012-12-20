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

#include "config.h"

/*
static OV_TICKET *ov_ksservhttp_ticket_defaultticket_createticket(XDR *xdr, OV_TICKET_TYPE type);
static void ov_ksservhttp_ticket_defaultticket_deleteticket(OV_TICKET *pticket) {
	return;
}
static OV_BOOL ov_ksservhttp_ticket_defaultticket_encodereply(XDR *y, OV_TICKET *x) {
	return TRUE;
}
*/
static OV_ACCESS ov_ksservhttp_ticket_defaultticket_getaccess(const OV_TICKET *a) {
	return KS_AC_READ;
}

//we need ony a getaccess for the getVar service
OV_DLLVAREXPORT OV_TICKET_VTBL defaultticketvtblVar = {
	NULL,
	NULL,
	NULL,
	ov_ksservhttp_ticket_defaultticket_getaccess
};
/*
static OV_TICKET *ov_ksservhttp_ticket_defaultticket_createticket(XDR *xdr, OV_TICKET_TYPE type) {
	static OV_TICKET ticket = { &defaultticketvtbl,  OV_TT_SIMPLE };
	return &ticket;
	//test auf serverpasswort und zugriff
}
*/


#define EXEC_GETVAR_RETURN ov_string_freelist(pVarsList); \
		Ov_SetDynamicVectorLength(&match,0,STRING);\
		ov_string_setvalue(&temp, NULL);\
		ov_string_setvalue(&temp2, NULL);\
		ov_string_setvalue(&LoopEntryList, NULL);\
		return

/**
 * extracts the (multiple) commands for the getVar and let do ks_server_getVar the job
 * @param args arguments of the http get request
 * @param message pointer to the result string
 * @return resultcode of the operation
 */
OV_RESULT exec_getvar(OV_STRING_VEC* const args, OV_STRING* message){
	/*
	*	parameter and result objects
	*/
	OV_UINT output_format;
	OV_STRING_VEC match = {0,NULL};
	OV_STRING *pVarsList = NULL;

	OV_STRING *addrp = NULL;

	OV_GETVAR_PAR	params;
	OV_GETVAR_RES	result;
	OV_GETVAR_ITEM	one_result;
	OV_ANY		Variable;
	OV_UINT i = 0;
	OV_UINT j = 0;
	OV_STRING temp = NULL;
	OV_STRING temp2 = NULL;
	OV_STRING LoopEntryList = NULL;
	OV_RESULT fr = OV_ERR_OK;

	static OV_TICKET ticket = { &defaultticketvtblVar,  OV_TT_NONE };

	output_format = extract_output_format(args);

	/**
	 * Build Parameter for KS function
	 */
	//process path
	Ov_SetDynamicVectorLength(&match,0,STRING);
	find_arguments(args, "path", &match);
	if(match.veclen<1){
		ov_string_append(message, "Variable path not found");
		EXEC_GETVAR_RETURN OV_ERR_BADPARAM; //400
	}

	ov_memstack_lock();
	addrp = Ov_MemStackAlloc(OV_STRING);

	*addrp = (OV_STRING)ov_memstack_alloc(match.veclen*sizeof(OV_STRING));
	if(!*addrp) {
		ov_memstack_unlock();
		EXEC_GETVAR_RETURN OV_ERR_TARGETGENERIC;
	}


	params.identifiers_val = addrp;
	params.identifiers_len = match.veclen;
	//process multiple path requests at once
	for(i=0;i<match.veclen;i++){
		*addrp = match.value[i];
		//add one size of a pointer
		addrp ++;
	}

	ov_ksserver_getvar(2, &ticket, &params, &result);

	/**
	 * Parse result from KS function
	 */

	if(Ov_Fail(result.result)){
		//general problem like memory problem or NOACCESS
		ov_memstack_unlock();
		EXEC_GETVAR_RETURN result.result;
	}
	for (j=0; j< result.items_len;j++){
		one_result = *(result.items_val + j);
		if(Ov_Fail(one_result.result)){
			//todo better info which element had an error
			fr = one_result.result;
			ov_string_setvalue(&temp, ov_result_getresulttext(fr));
		}else{
			Variable = one_result.var_current_props;

			switch (Variable.value.vartype){
				case OV_VT_VOID:															//unused ANY with explicit no content
				case (OV_VT_VOID | OV_VT_HAS_STATE | OV_VT_HAS_TIMESTAMP):	//  used ANY with explicit no content
					ov_string_print(&temp, "%s", "");
					break;

				case OV_VT_BYTE:
				case (OV_VT_BYTE | OV_VT_HAS_STATE | OV_VT_HAS_TIMESTAMP):
				ov_string_print(&temp, "%d", Variable.value.valueunion.val_byte);
				break;

				case OV_VT_BOOL:
				case OV_VT_BOOL_PV:
					if (Variable.value.valueunion.val_bool == TRUE){
						ov_string_setvalue(&temp, "TRUE");
					}else{
						ov_string_setvalue(&temp, "FALSE");
					}
					break;

				case OV_VT_INT:
				case OV_VT_INT_PV:
					ov_string_print(&temp, "%i", Variable.value.valueunion.val_int);
					break;

				case OV_VT_UINT:
				case OV_VT_UINT_PV:
					ov_string_print(&temp, "%u", Variable.value.valueunion.val_uint);
					break;

				case OV_VT_SINGLE:
				case OV_VT_SINGLE_PV:
					ov_string_print(&temp, "%g", Variable.value.valueunion.val_single);
					break;

				case OV_VT_DOUBLE:
				case OV_VT_DOUBLE_PV:
					ov_string_print(&temp, "%g", Variable.value.valueunion.val_double);
					break;

				case OV_VT_STRING:
				case OV_VT_STRING_PV:
					if (ov_string_compare(Variable.value.valueunion.val_string, NULL) == OV_STRCMP_EQUAL){
						ov_string_setvalue(&temp, "");
					}else{
						//FIXME: temp can be empty if there is no memory in the database
						//check it with Ov_OK!
						ov_string_print(&temp, "%s", Variable.value.valueunion.val_string);
					}
					break;

				case OV_VT_TIME:
				case OV_VT_TIME_PV:
					ov_string_print(&temp, "%s", ov_time_timetoascii(&Variable.value.valueunion.val_time));
					break;

				case OV_VT_TIME_SPAN:
				case OV_VT_TIME_SPAN_PV:
					ov_string_print(&temp, "%s", ov_time_timespantoascii(&Variable.value.valueunion.val_time_span));
					break;

				case OV_VT_STATE:
				case (OV_VT_STATE | OV_VT_HAS_STATE | OV_VT_HAS_TIMESTAMP):
				ov_string_print(&temp, "%s", "unknown");
				break;

				case OV_VT_STRUCT:
				case (OV_VT_STRUCT | OV_VT_HAS_STATE | OV_VT_HAS_TIMESTAMP):
				ov_string_print(&temp, "%s", "unknown");
				break;

				//****************** VEC: *******************
				case OV_VT_BYTE_VEC:
				case (OV_VT_BYTE_VEC | OV_VT_HAS_STATE | OV_VT_HAS_TIMESTAMP):
				init_vector_output(&temp, output_format);
				for ( i = 0; i < Variable.value.valueunion.val_byte_vec.veclen;i++){
					if (i != 0){
						split_vector_output(&temp, output_format);
					}
					//TODO: I know copy-pasting all appends around is mad, however i went even more mad by passing
					//variable number of arguments around :(
					ov_string_print(&temp2, "%d", Variable.value.valueunion.val_byte_vec.value[i]);
					ov_string_append(&temp, temp2);
					ov_string_setvalue(&temp2, NULL);
				}
				finalize_vector_output(&temp, output_format);
				break;

				case OV_VT_BOOL_VEC:
				case OV_VT_BOOL_PV_VEC:
					init_vector_output(&temp, output_format);
					for ( i = 0; i < Variable.value.valueunion.val_bool_vec.veclen;i++){
						if (i != 0){
							split_vector_output(&temp, output_format);
						}
						if (Variable.value.valueunion.val_bool_vec.value[i] == TRUE){
							ov_string_setvalue(&temp2, "TRUE");
						}else{
							ov_string_setvalue(&temp2, "FALSE");
						}
						ov_string_append(&temp, temp2);
						ov_string_setvalue(&temp2, NULL);
					}
					finalize_vector_output(&temp, output_format);
					break;

				case OV_VT_INT_VEC:
				case OV_VT_INT_PV_VEC:
					init_vector_output(&temp, output_format);
					for ( i = 0; i < Variable.value.valueunion.val_int_vec.veclen;i++){
						if (i != 0){
							split_vector_output(&temp, output_format);
						}
						ov_string_print(&temp2, "%i", Variable.value.valueunion.val_int_vec.value[i]);
						ov_string_append(&temp, temp2);
						ov_string_setvalue(&temp2, NULL);
					}
					finalize_vector_output(&temp, output_format);
					break;

				case OV_VT_UINT_VEC:
				case OV_VT_UINT_PV_VEC:
					init_vector_output(&temp, output_format);
					for ( i = 0; i < Variable.value.valueunion.val_uint_vec.veclen;i++){
						if (i != 0){
							split_vector_output(&temp, output_format);
						}
						ov_string_print(&temp2, "%i", Variable.value.valueunion.val_uint_vec.value[i]);
						ov_string_append(&temp, temp2);
						ov_string_setvalue(&temp2, NULL);
					}
					finalize_vector_output(&temp, output_format);
					break;

				case OV_VT_SINGLE_VEC:
				case OV_VT_SINGLE_PV_VEC:
					init_vector_output(&temp, output_format);
					for ( i = 0; i < Variable.value.valueunion.val_single_vec.veclen;i++){
						if (i != 0){
							split_vector_output(&temp, output_format);
						}
						ov_string_print(&temp2, "%g", Variable.value.valueunion.val_single_vec.value[i]);
						ov_string_append(&temp, temp2);
						ov_string_setvalue(&temp2, NULL);
					}
					finalize_vector_output(&temp, output_format);
					break;

				case OV_VT_DOUBLE_VEC:
				case OV_VT_DOUBLE_PV_VEC:
					init_vector_output(&temp, output_format);
					for ( i = 0; i < Variable.value.valueunion.val_double_vec.veclen;i++){
						if (i != 0){
							split_vector_output(&temp, output_format);
						}
						ov_string_print(&temp2, "%g", Variable.value.valueunion.val_double_vec.value[i]);
						ov_string_append(&temp, temp2);
						ov_string_setvalue(&temp2, NULL);
					}
					finalize_vector_output(&temp, output_format);
					break;

				case OV_VT_STRING_VEC:
				case OV_VT_STRING_PV_VEC:
					init_vector_output(&temp, output_format);
					for ( i = 0; i < Variable.value.valueunion.val_string_vec.veclen;i++){
						if (i != 0){
							split_vector_output(&temp, output_format);
						}
						if (ov_string_compare(Variable.value.valueunion.val_string_vec.value[i], NULL) == OV_STRCMP_EQUAL){
							ov_string_append(&temp, "");
						}else{
							ov_string_print(&temp2, "%s", Variable.value.valueunion.val_string_vec.value[i]);
							ov_string_append(&temp, temp2);
							ov_string_setvalue(&temp2, NULL);
						}
					}
					finalize_vector_output(&temp, output_format);
					break;

				case OV_VT_TIME_VEC:
				case OV_VT_TIME_PV_VEC:
					init_vector_output(&temp, output_format);
					for ( i = 0; i < Variable.value.valueunion.val_time_vec.veclen;i++){
						if (i != 0){
							split_vector_output(&temp, output_format);
						}
						ov_string_print(&temp2, "%s", ov_time_timetoascii(&Variable.value.valueunion.val_time_vec.value[i]));
						ov_string_append(&temp, temp2);
						ov_string_setvalue(&temp2, NULL);
					}
					finalize_vector_output(&temp, output_format);
					break;

				case OV_VT_TIME_SPAN_VEC:
				case OV_VT_TIME_SPAN_PV_VEC:
					init_vector_output(&temp, output_format);
					for ( i = 0; i < Variable.value.valueunion.val_time_span_vec.veclen;i++){
						if (i != 0){
							split_vector_output(&temp, output_format);
						}
						ov_string_print(&temp2, "%s", ov_time_timespantoascii(&Variable.value.valueunion.val_time_span_vec.value[i]));
						ov_string_append(&temp, temp2);
						ov_string_setvalue(&temp2, NULL);
					}
					finalize_vector_output(&temp, output_format);
					break;

				case OV_VT_STATE_VEC:
				case (OV_VT_STATE_VEC | OV_VT_HAS_STATE | OV_VT_HAS_TIMESTAMP):
					ov_string_print(&temp, "%s", "unknown");
					fr = OV_ERR_NOTIMPLEMENTED;
					break;

				case OV_VT_STRUCT_VEC:
				case (OV_VT_STRUCT_VEC | OV_VT_HAS_STATE | OV_VT_HAS_TIMESTAMP):
					ov_string_print(&temp, "%s", "unknown");
					fr = OV_ERR_NOTIMPLEMENTED;
					break;

				default:
					ov_string_print(&temp, "unknown value, VarType: %#X", Variable.value.vartype);
					fr = OV_ERR_NOTIMPLEMENTED;
					break;
			}
		}
		if(j>0){
			ov_string_append(&LoopEntryList, " ");
		}
		begin_vector_output(&LoopEntryList, output_format);

		ov_string_append(&LoopEntryList, temp);


		finalize_vector_output(&LoopEntryList, output_format);

	}

	ov_string_append(message, LoopEntryList);

	ov_memstack_unlock();
	EXEC_GETVAR_RETURN fr;
}
