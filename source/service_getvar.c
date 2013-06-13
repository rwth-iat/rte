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

/*
static OV_TICKET *ov_kshttp_ticket_defaultticket_createticket(XDR *xdr, OV_TICKET_TYPE type);
static void ov_kshttp_ticket_defaultticket_deleteticket(OV_TICKET *pticket) {
	return;
}
static OV_BOOL ov_kshttp_ticket_defaultticket_encodereply(XDR *y, OV_TICKET *x) {
	return TRUE;
}
*/
static OV_ACCESS ov_kshttp_ticket_defaultticket_getaccess(const OV_TICKET *a) {
	return KS_AC_READ;
}

//we need ony a getaccess for the getVar service
OV_DLLVAREXPORT OV_TICKET_VTBL defaultticketvtblVar = {
	NULL,
	NULL,
	NULL,
	ov_kshttp_ticket_defaultticket_getaccess
};
/*
static OV_TICKET *ov_kshttp_ticket_defaultticket_createticket(XDR *xdr, OV_TICKET_TYPE type) {
	static OV_TICKET ticket = { &defaultticketvtbl,  OV_TT_SIMPLE };
	return &ticket;
	//test auf serverpasswort und zugriff
}
*/

/*
falsche ergebnisse bei: evtl behoben durch neuen code
http://localhost:8080/getVar?path[1]=/TechUnits/cshmi/Templates/Engineering/Domainv1/Trash.SVGcontent&path[9]=.Bitmapcontent
http://localhost:8080/setVar?path=/TechUnits/HMIManager.Command&newvalue={1}%20{010}%20{/TechUnits/HMIManager}%20{SHOWSHEETS}
*/



#define EXEC_GETVAR_RETURN \
		Ov_SetDynamicVectorLength(&match,0,STRING);\
		ov_string_setvalue(&LoopEntryValue, NULL);\
		ov_string_setvalue(&singleVecEntry, NULL);\
		ov_string_setvalue(&LoopEntryTypeString, NULL);\
		ov_string_setvalue(&LoopEntryList, NULL);\
		return

/**
 * extracts the (multiple) commands for the getVar and let do ks_server_getVar the job
 * @param args arguments of the http get request
 * @param message pointer to the result string
 * @return resultcode of the operation
 */
OV_RESULT exec_getvar(OV_STRING_VEC* const args, OV_STRING* message, OV_UINT response_format){
	/*
	*	parameter and result objects
	*/
	OV_STRING_VEC match = {0,NULL};

	OV_STRING *addrp = NULL;

	OV_GETVAR_PAR	params;
	OV_GETVAR_RES	result;
	OV_GETVAR_ITEM	one_result;
	OV_ANY		Variable;
	OV_UINT i = 0;
	OV_UINT j = 0;
	OV_STRING LoopEntryValue = NULL;
	OV_STRING singleVecEntry = NULL;
	OV_STRING LoopEntryList = NULL;
	OV_STRING LoopEntryTypeString = NULL;
	OV_RESULT fr = OV_ERR_OK;
	OV_RESULT lasterror = OV_ERR_OK;

	static OV_TICKET ticket = { &defaultticketvtblVar,  OV_TT_NONE };

	/**
	 * Build Parameter for KS function
	 */
	//process path
	Ov_SetDynamicVectorLength(&match,0,STRING);
	find_arguments(args, "path", &match);
	if(match.veclen<1){
		fr = OV_ERR_BADPARAM;
		print_result_array(message, response_format, &fr, 1, ": Variable path not found");
		EXEC_GETVAR_RETURN fr; //400
	}

	ov_memstack_lock();
	addrp = Ov_MemStackAlloc(OV_STRING);

	*addrp = (OV_STRING)ov_memstack_alloc(match.veclen*sizeof(OV_STRING));
	if(!*addrp) {
		ov_memstack_unlock();
		fr = OV_ERR_TARGETGENERIC;
		print_result_array(message, response_format, &fr, 1, ": memory problem");
		EXEC_GETVAR_RETURN fr;
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


/*
<response xmlns="http://acplt.org/schemas/ksx/2.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="http://acplt.org/schemas/ksx/2.0 ksx.xsd">
	<getvar>
		<failure>17</failure>
		<var>
			<value>
				<single>421</single>
			</value>
			<timestamp>2013/05/28 14:13:46.351000</timestamp>
			<state>unknown</state>
		</var>
		<failure>17</failure>
		<var>
			<value>
				<single>421</single>
			</value>
			<timestamp>2013/05/28 14:13:46.351000</timestamp>
			<state>unknown</state>
		</var>
	</getvar>
</response>
*/
	/**
	 * Parse result from KS function
	 */

	if(Ov_Fail(result.result)){
		//general problem like memory problem or NOACCESS
		print_result_array(message, response_format, &result.result, 1, ": general problem");
		ov_memstack_unlock();
		EXEC_GETVAR_RETURN result.result;
	}
	for (j=0; j< result.items_len;j++){
		if(j>0){
			seperate_response_parts(&LoopEntryList, response_format);
		}
		one_result = *(result.items_val + j);
		fr = one_result.result;
		if(Ov_Fail(fr)){
			lasterror = fr;
			begin_response_part(&LoopEntryList, response_format, "failure");
			if(response_format == RESPONSE_FORMAT_KSX){
				if(LoopEntryList == NULL){
					ov_string_print(&LoopEntryList, "%i", fr);
				}else{
					ov_string_print(&LoopEntryList, "%s%i", LoopEntryList, fr);
				}
			}else{
				if(LoopEntryList == NULL){
					ov_string_print(&LoopEntryList, "KS_ERR: %s", ov_result_getresulttext(fr));
				}else{
					ov_string_print(&LoopEntryList, "%sKS_ERR: %s", LoopEntryList, ov_result_getresulttext(fr));
				}
			}
			finalize_response_part(&LoopEntryList, response_format, "failure");
		}else{
			Variable = one_result.var_current_props;

			if(Variable.value.vartype & OV_VT_ISVECTOR){
				//the nonVEC cases write into LoopEntryValue,
				//the VEC cases appends in a loop
				ov_string_setvalue(&LoopEntryValue, NULL);
			}
			switch (Variable.value.vartype){
				case OV_VT_VOID:															//unused ANY with explicit no content
				case (OV_VT_VOID | OV_VT_HAS_STATE | OV_VT_HAS_TIMESTAMP):	//  used ANY with explicit no content
					ov_string_setvalue(&LoopEntryTypeString, "void");
					ov_string_print(&LoopEntryValue, "%s", "");
					break;

				case OV_VT_BYTE:
				case (OV_VT_BYTE | OV_VT_HAS_STATE | OV_VT_HAS_TIMESTAMP):
					ov_string_setvalue(&LoopEntryTypeString, "byte");
					ov_string_print(&LoopEntryValue, "%d", Variable.value.valueunion.val_byte);
					break;

				case OV_VT_BOOL:
				case OV_VT_BOOL_PV:
					ov_string_setvalue(&LoopEntryTypeString, "bool");
					if (Variable.value.valueunion.val_bool == TRUE){
						ov_string_setvalue(&LoopEntryValue, "TRUE");
					}else{
						ov_string_setvalue(&LoopEntryValue, "FALSE");
					}
					break;

				case OV_VT_INT:
				case OV_VT_INT_PV:
					ov_string_setvalue(&LoopEntryTypeString, "int");
					ov_string_print(&LoopEntryValue, "%i", Variable.value.valueunion.val_int);
					break;

				case OV_VT_UINT:
				case OV_VT_UINT_PV:
					ov_string_setvalue(&LoopEntryTypeString, "uint");
					ov_string_print(&LoopEntryValue, "%u", Variable.value.valueunion.val_uint);
					break;

				case OV_VT_SINGLE:
				case OV_VT_SINGLE_PV:
					ov_string_setvalue(&LoopEntryTypeString, "single");
					ov_string_print(&LoopEntryValue, "%g", Variable.value.valueunion.val_single);
					break;

				case OV_VT_DOUBLE:
				case OV_VT_DOUBLE_PV:
					ov_string_setvalue(&LoopEntryTypeString, "double");
					ov_string_print(&LoopEntryValue, "%g", Variable.value.valueunion.val_double);
					break;

				case OV_VT_STRING:
				case OV_VT_STRING_PV:
					ov_string_setvalue(&LoopEntryTypeString, "string");
					if (ov_string_compare(Variable.value.valueunion.val_string, NULL) == OV_STRCMP_EQUAL){
						ov_string_setvalue(&LoopEntryValue, "");
					}else{
						kshttp_escapeString(&LoopEntryValue, &Variable.value.valueunion.val_string, response_format);
					}
					break;

				case OV_VT_TIME:
				case OV_VT_TIME_PV:
					ov_string_setvalue(&LoopEntryTypeString, "time");
					kshttp_timetoascii(&LoopEntryValue, &Variable.value.valueunion.val_time, response_format);
					break;

				case OV_VT_TIME_SPAN:
				case OV_VT_TIME_SPAN_PV:
					ov_string_setvalue(&LoopEntryTypeString, "timespan");
					kshttp_timespantoascii(&LoopEntryValue, &Variable.value.valueunion.val_time_span, response_format);
					break;

				case OV_VT_STATE:
				case (OV_VT_STATE | OV_VT_HAS_STATE | OV_VT_HAS_TIMESTAMP):
					ov_string_setvalue(&LoopEntryTypeString, "state");
					ov_string_print(&LoopEntryValue, "%s", "unknown");
					break;

				case OV_VT_STRUCT:
				case (OV_VT_STRUCT | OV_VT_HAS_STATE | OV_VT_HAS_TIMESTAMP):
					//fixme struct base64
					ov_string_setvalue(&LoopEntryTypeString, "struct");
					ov_string_print(&LoopEntryValue, "%s", "unknown");
					break;

				//****************** VEC: *******************
				case OV_VT_BYTE_VEC:
				case (OV_VT_BYTE_VEC | OV_VT_HAS_STATE | OV_VT_HAS_TIMESTAMP):
					ov_string_setvalue(&LoopEntryTypeString, "bytevec");
					fr = OV_ERR_NOTIMPLEMENTED;
					lasterror = fr;
					print_result_array(message, response_format, &fr, 1, ": bytevec");
					/* todo should be base64 encoded content
					for ( i = 0; i < Variable.value.valueunion.val_byte_vec.veclen;i++){
						ov_string_print(&singleVecEntry, "%d", Variable.value.valueunion.val_byte_vec.value[i]);
						ov_string_append(&LoopEntryValue, singleVecEntry);
						ov_string_setvalue(&singleVecEntry, NULL);
					}
					*/
					break;

				case OV_VT_BOOL_VEC:
				case OV_VT_BOOL_PV_VEC:
					ov_string_setvalue(&LoopEntryTypeString, "boolvec");
					for ( i = 0; i < Variable.value.valueunion.val_bool_vec.veclen;i++){
						if(i>0){
							seperate_response_parts(&LoopEntryValue, response_format);
						}
						begin_response_part(&LoopEntryValue, response_format, "bool");
						if (Variable.value.valueunion.val_bool_vec.value[i] == TRUE){
							ov_string_setvalue(&singleVecEntry, "TRUE");
						}else{
							ov_string_setvalue(&singleVecEntry, "FALSE");
						}
						ov_string_append(&LoopEntryValue, singleVecEntry);
						finalize_response_part(&LoopEntryValue, response_format, "bool");
					}
					break;

				case OV_VT_INT_VEC:
				case OV_VT_INT_PV_VEC:
					ov_string_setvalue(&LoopEntryTypeString, "intvec");
					for ( i = 0; i < Variable.value.valueunion.val_int_vec.veclen;i++){
						if(i>0){
							seperate_response_parts(&LoopEntryValue, response_format);
						}
						begin_response_part(&LoopEntryValue, response_format, "int");
						ov_string_print(&singleVecEntry, "%i", Variable.value.valueunion.val_int_vec.value[i]);
						ov_string_append(&LoopEntryValue, singleVecEntry);
						finalize_response_part(&LoopEntryValue, response_format, "int");
					}
					break;

				case OV_VT_UINT_VEC:
				case OV_VT_UINT_PV_VEC:
					ov_string_setvalue(&LoopEntryTypeString, "uintvec");
					for ( i = 0; i < Variable.value.valueunion.val_uint_vec.veclen;i++){
						if(i>0){
							seperate_response_parts(&LoopEntryValue, response_format);
						}
						begin_response_part(&LoopEntryValue, response_format, "uint");
						ov_string_print(&singleVecEntry, "%u", Variable.value.valueunion.val_uint_vec.value[i]);
						ov_string_append(&LoopEntryValue, singleVecEntry);
						finalize_response_part(&LoopEntryValue, response_format, "uint");
					}
					break;

				case OV_VT_SINGLE_VEC:
				case OV_VT_SINGLE_PV_VEC:
					ov_string_setvalue(&LoopEntryTypeString, "singlevec");
					for ( i = 0; i < Variable.value.valueunion.val_single_vec.veclen;i++){
						if(i>0){
							seperate_response_parts(&LoopEntryValue, response_format);
						}
						begin_response_part(&LoopEntryValue, response_format, "single");
						ov_string_print(&singleVecEntry, "%g", Variable.value.valueunion.val_single_vec.value[i]);
						ov_string_append(&LoopEntryValue, singleVecEntry);
						finalize_response_part(&LoopEntryValue, response_format, "single");
					}
					break;

				case OV_VT_DOUBLE_VEC:
				case OV_VT_DOUBLE_PV_VEC:
					ov_string_setvalue(&LoopEntryTypeString, "doublevec");
					for ( i = 0; i < Variable.value.valueunion.val_double_vec.veclen;i++){
						if(i>0){
							seperate_response_parts(&LoopEntryValue, response_format);
						}
						begin_response_part(&LoopEntryValue, response_format, "double");
						ov_string_print(&singleVecEntry, "%g", Variable.value.valueunion.val_double_vec.value[i]);
						ov_string_append(&LoopEntryValue, singleVecEntry);
						finalize_response_part(&LoopEntryValue, response_format, "double");
					}
					break;

				case OV_VT_STRING_VEC:
				case OV_VT_STRING_PV_VEC:
					ov_string_print(&LoopEntryTypeString, "stringvec length=\"%i\"", Variable.value.valueunion.val_string_vec.veclen);
					for ( i = 0; i < Variable.value.valueunion.val_string_vec.veclen;i++){
						if(i>0){
							seperate_response_parts(&LoopEntryValue, response_format);
						}
						begin_response_part(&LoopEntryValue, response_format, "string");
						if (ov_string_compare(Variable.value.valueunion.val_string_vec.value[i], NULL) == OV_STRCMP_EQUAL){
							//append an empty string and clear string
							ov_string_setvalue(&singleVecEntry, "");
						}else{
							kshttp_escapeString(&singleVecEntry, &Variable.value.valueunion.val_string_vec.value[i], response_format);
							ov_string_append(&LoopEntryValue, singleVecEntry);
						}
						finalize_response_part(&LoopEntryValue, response_format, "string");
					}
					break;

				case OV_VT_TIME_VEC:
				case OV_VT_TIME_PV_VEC:
					ov_string_setvalue(&LoopEntryTypeString, "timevec");
					for ( i = 0; i < Variable.value.valueunion.val_time_vec.veclen;i++){
						if(i>0){
							seperate_response_parts(&LoopEntryValue, response_format);
						}
						begin_response_part(&LoopEntryValue, response_format, "time");
						kshttp_timetoascii(&singleVecEntry, &Variable.value.valueunion.val_time_vec.value[i], response_format);
						ov_string_append(&LoopEntryValue, singleVecEntry);
						finalize_response_part(&LoopEntryValue, response_format, "time");
					}
					break;

				case OV_VT_TIME_SPAN_VEC:
				case OV_VT_TIME_SPAN_PV_VEC:
					ov_string_setvalue(&LoopEntryTypeString, "timespanvec");
					for ( i = 0; i < Variable.value.valueunion.val_time_span_vec.veclen;i++){
						if(i>0){
							seperate_response_parts(&LoopEntryValue, response_format);
						}
						begin_response_part(&LoopEntryValue, response_format, "timespan");
						kshttp_timespantoascii(&singleVecEntry, &Variable.value.valueunion.val_time_span_vec.value[i], response_format);
						ov_string_append(&LoopEntryValue, singleVecEntry);
						finalize_response_part(&LoopEntryValue, response_format, "timespan");
					}
					break;

				case OV_VT_STATE_VEC:
				case (OV_VT_STATE_VEC | OV_VT_HAS_STATE | OV_VT_HAS_TIMESTAMP):
					ov_string_setvalue(&LoopEntryTypeString, "statevec");
					ov_string_print(&LoopEntryValue, "%s", "unknown");
					fr = OV_ERR_NOTIMPLEMENTED;
					break;

				case OV_VT_STRUCT_VEC:
				case (OV_VT_STRUCT_VEC | OV_VT_HAS_STATE | OV_VT_HAS_TIMESTAMP):
					ov_string_setvalue(&LoopEntryTypeString, "structvec");
					ov_string_print(&LoopEntryValue, "%s", "unknown");
					fr = OV_ERR_NOTIMPLEMENTED;
					break;

				default:
					ov_string_print(&LoopEntryValue, "unknown value, VarType: %#X", Variable.value.vartype);
					fr = OV_ERR_NOTIMPLEMENTED;
					break;
			}//end switch vartype
			begin_response_part(&LoopEntryList, response_format, "var");
			if(ov_string_compare(LoopEntryTypeString, NULL) != OV_STRCMP_EQUAL && (response_format == RESPONSE_FORMAT_KSX || response_format == RESPONSE_FORMAT_JSON)){
				//get additional data if we serve ksx
				begin_response_part(&LoopEntryList, response_format, "value");
				begin_response_part(&LoopEntryList, response_format, LoopEntryTypeString);
				ov_string_append(&LoopEntryList, LoopEntryValue);
				if(Variable.value.vartype == OV_VT_STRING_VEC || Variable.value.vartype == OV_VT_STRING_PV_VEC){
					//fixme why a string has a length attribute??? thank harry for that mess
					ov_string_print(&LoopEntryTypeString, "stringvec");
				}
				finalize_response_part(&LoopEntryList, response_format, LoopEntryTypeString);
				finalize_response_part(&LoopEntryList, response_format, "value");

				begin_response_part(&LoopEntryList, response_format, "timestamp");
				kshttp_timetoascii(&singleVecEntry, &Variable.time, response_format);
				ov_string_append(&LoopEntryList, singleVecEntry);

				finalize_response_part(&LoopEntryList, response_format, "timestamp");
				begin_response_part(&LoopEntryList, response_format, "state");
				switch (Variable.state) {
					case OV_ST_NOTSUPPORTED:
						ov_string_append(&LoopEntryList, "notsupported");
						break;
					case OV_ST_UNKNOWN:
						ov_string_append(&LoopEntryList, "unknown");
						break;
					case OV_ST_BAD:
						ov_string_append(&LoopEntryList, "bad");
						break;
					case OV_ST_QUESTIONABLE:
						ov_string_append(&LoopEntryList, "questionable");
						break;
					case OV_ST_GOOD:
						ov_string_append(&LoopEntryList, "good");
						break;
					default:
						break;
				}
				finalize_response_part(&LoopEntryList, response_format, "state");
			}else{
				ov_string_append(&LoopEntryList, LoopEntryValue);
			}
			finalize_response_part(&LoopEntryList, response_format, "var");
		}//end if ov_fail
	}//end for entry

	ov_string_append(message, LoopEntryList);

	ov_memstack_unlock();
	EXEC_GETVAR_RETURN lasterror;
}
