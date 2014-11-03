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
#include <ctype.h> //toupper

/**
 * Appends a text info to a string, if ksx is requested skipping a prefix and lowercased
 * a "_" is skipped if ksx
 * @param resultstr Pointer to a string to append
 * @param prefix String, for example KS_VT
 * @param value String, for example BOOL
 * @param response_format UINT format descriptor
 * @return
 */
OV_RESULT getEP_print_KSmakrovalue(OV_STRING *resultstr, OV_STRING prefix, OV_STRING value, KSHTTP_RESPONSEFORMAT response_format){
	OV_STRING changedValue = NULL;
	OV_STRING pointer = NULL;
	OV_UINT i = 0;
	if(response_format == KSX || response_format == JSON){
		//kill all underscores. TIME_SPAN_VEC for example has two...
		ov_memstack_lock();
		changedValue = ov_memstack_alloc(ov_string_getlength(value)+1);
		for (i = 0, pointer = value;*pointer != '\0';pointer++){
			if(*pointer != '_'){
				changedValue[i] = tolower(*pointer);
				i++;
			}
		}
		changedValue[i] = '\0'; /*append terminating '\0'*/
		pointer = NULL;
		ov_string_print(resultstr, "%s%s", *resultstr, changedValue);
		ov_memstack_unlock();
		return OV_ERR_OK;
	}else{
		ov_memstack_lock();
		ov_string_print(resultstr, "%s%s_%s", *resultstr, prefix, value);
		ov_memstack_unlock();
		return OV_ERR_OK;
	}
}

/**
 * begins the sub part of an requestOutput if needed (aka non tcl)
 * @param output pointer to the string to manipulate
 * @param format UINT for the type of response
 * @param entry_type string for naming the following content (xml node name in KSX)
 * @return return code always ov_err_ok
 */
OV_RESULT getEP_begin_RequestOutputPart(OV_STRING* output, KSHTTP_RESPONSEFORMAT response_format, OV_STRING entry_type){
	if(response_format == KSX){
		return kshttp_response_part_begin(output, response_format, entry_type);
	}else if(response_format == TCL){
		//nothing
	}else if(response_format == JSON){
		kshttp_response_part_begin(output, response_format, entry_type);
		return ov_string_append(output, "\"");
	}
	return OV_ERR_OK;
}
/**
 * finalize the sub part of an requestOutput if needed (aka non tcl)
 * @param output pointer to the string to manipulate
 * @param format UINT for the type of response
 * @param entry_type string for naming the following content (xml node name in KSX)
 * @return return code always ov_err_ok
 */
OV_RESULT getEP_finalize_RequestOutputPart(OV_STRING* output, KSHTTP_RESPONSEFORMAT response_format, OV_STRING entry_type){
	if(response_format == KSX){
		return kshttp_response_part_finalize(output, response_format, entry_type);
	}else if(response_format == TCL){
		//nothing
	}else if(response_format == JSON){
		ov_string_append(output, "\"");
		return kshttp_response_part_finalize(output, response_format, entry_type);
	}
	return OV_ERR_OK;
}

#define EXEC_GETEP_RETURN \
		Ov_SetDynamicVectorLength(&match,0,STRING);\
		ov_string_setvalue(&message, NULL);\
		Ov_SetDynamicVectorLength(&requestOutput,0,UINT);\
		ov_string_setvalue(&temp, NULL);\
		ov_string_setvalue(&params.path, NULL);\
		ov_string_setvalue(&params.name_mask, NULL);\
		return

OV_RESULT kshttp_exec_getep(OV_STRING_VEC* args, OV_STRING* re, KSHTTP_RESPONSEFORMAT response_format){
	OV_STRING_VEC match = {0,NULL};

	OV_GETEP_PAR	params;
	OV_GETEP_RES	result;
	OV_OBJ_ENGINEERED_PROPS	*one_result;

	OV_TICKET* pticket = NULL;

	OV_STRING message = NULL;
	OV_UINT_VEC requestOutput = {0,NULL};
	OV_STRING temp = NULL;
	OV_STRING temp2 = NULL;
	OV_BOOL EntryFound = FALSE;
	OV_BOOL anyRequested = FALSE;
	OV_UINT requestOutputDefaultDomain[] = {OP_NAME, OP_TYPE, OP_COMMENT, OP_ACCESS, OP_SEMANTIC, OP_CREATIONTIME, OP_CLASS};
	OV_UINT requestOutputDefaultVariable[] = {OP_NAME, OP_TYPE, OP_COMMENT, OP_ACCESS, OP_SEMANTIC, OP_CREATIONTIME, OP_CLASS, OP_TECHUNIT};
	OV_UINT requestOutputDefaultLink[] = {OP_NAME, OP_TYPE, OP_COMMENT, OP_ACCESS, OP_SEMANTIC, OP_CREATIONTIME, OP_CLASS, OP_ASSOCIDENT, OP_ROLEIDENT};
	OV_RESULT fr = OV_ERR_OK;
	unsigned char flagiterator = 'a';
	OV_UINT i = 0;

	//initialize ov_string
	params.path = NULL;
	params.name_mask = NULL;

	//path=/TechUnits
	//requestType=OT_DOMAIN|OT_VARIABLE|... (siehe tcl-tks doku)
	//requestOutput or requestOutput[i] with OP_NAME, OP_TYPE, OP_COMMENT, OP_ACCESS, OP_SEMANTIC, OP_CREATIONTIME and OP_CLASS
	/**
	 * Build Parameter for KS function
	 */
	//process path
	kshttp_find_arguments(args, "path", &match);
	if(match.veclen!=1){
		fr = OV_ERR_BADPARAM;
		kshttp_print_result_array(re, response_format, &fr, 1, ": Path not found or multiple path given");
		EXEC_GETEP_RETURN fr; //400
	}

	ov_string_setvalue(&params.path, match.value[0]);

	kshttp_find_arguments(args, "nameMask", &match);
	if(match.veclen > 0){
		ov_string_setvalue(&params.name_mask, match.value[0]);
	}else{
		ov_string_setvalue(&params.name_mask, "*");
	}
	kshttp_find_arguments(args, "scopeFlags", &match);
	if(match.veclen == 1){
		if(ov_string_compare(match.value[0], "parts") == OV_STRCMP_EQUAL){
			params.scope_flags = KS_EPF_PARTS;
		}else if(ov_string_compare(match.value[0], "children") == OV_STRCMP_EQUAL){
			params.scope_flags = KS_EPF_CHILDREN;
		}else if(ov_string_compare(match.value[0], "flatten") == OV_STRCMP_EQUAL){
			//was never implemented in libovks :-)
			params.scope_flags = KS_EPF_FLATTEN;
		}else{
			params.scope_flags = KS_EPF_DEFAULT;
		}
	}else{
		params.scope_flags = KS_EPF_DEFAULT;
	}

	kshttp_find_arguments(args, "requestType", &match);
	if(match.veclen == 1){
		if(ov_string_compare(match.value[0], "OT_DOMAIN") == OV_STRCMP_EQUAL){
			params.type_mask = KS_OT_DOMAIN;
		}else if(ov_string_compare(match.value[0], "OT_VARIABLE") == OV_STRCMP_EQUAL){
			params.type_mask = KS_OT_VARIABLE;
		}else if(ov_string_compare(match.value[0], "OT_LINK") == OV_STRCMP_EQUAL){
			params.type_mask = KS_OT_LINK;
		}else if(ov_string_compare(match.value[0], "OT_ANY") == OV_STRCMP_EQUAL){
			params.type_mask = KS_OT_ANY;
		}else{
			fr = OV_ERR_BADPARAM;
			kshttp_print_result_array(re, response_format, &fr, 1, ": Requesttype not supported");
			EXEC_GETEP_RETURN fr; //400
		}
	}else{
		//default to OT_DOMAIN
		params.type_mask = KS_OT_DOMAIN;
	}

	kshttp_find_arguments(args, "requestOutput", &match);
	if(response_format == KSX || response_format == JSON || match.veclen == 0 || (match.veclen==1 && ov_string_compare(match.value[0], "OP_ANY") == OV_STRCMP_EQUAL )){
		//if nothing is specified or all is requested, give all
		anyRequested = TRUE;
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
			}else if(ov_string_compare(match.value[i], "OP_TECHUNIT") == OV_STRCMP_EQUAL){
				requestOutput.value[i] = OP_TECHUNIT;
			}else if(ov_string_compare(match.value[i], "OP_ASSOCIDENT") == OV_STRCMP_EQUAL){
				requestOutput.value[i] = OP_ASSOCIDENT;
			}else if(ov_string_compare(match.value[i], "OP_ROLEIDENT") == OV_STRCMP_EQUAL){
				requestOutput.value[i] = OP_ROLEIDENT;
			}else{
				requestOutput.value[i] = OP_UNKNOWN;
			}
		}
	}

	//create NONE-ticket
	pticket = ksbase_NoneAuth->v_ticket.vtbl->createticket(NULL, OV_TT_NONE);

	ov_memstack_lock(); //needed for ov_path_resolve and the class_identifier
	ov_ksserver_getep(2, pticket, &params, &result);

	/*	delete Ticket	*/
	pticket->vtbl->deleteticket(pticket);

	if(Ov_Fail(result.result)){
		//general problem like memory problem or NOACCESS
		kshttp_print_result_array(re, response_format, &result.result, 1, ": general problem");
		ov_memstack_unlock();
		EXEC_GETEP_RETURN result.result;
	}

	one_result = result.pfirst;
	while(one_result != NULL){
		//open Child item level
		if(result.pfirst != one_result){
			kshttp_response_parts_seperate(&temp, response_format);
		}
		//change target output
		if(anyRequested && (one_result->objtype & KS_OT_DOMAIN)){
			kshttp_response_part_begin(&temp, response_format, "DomainEngProps");
			fr = Ov_SetDynamicVectorValue(&requestOutput, requestOutputDefaultDomain, 7, UINT);
		}else if(anyRequested && (one_result->objtype & KS_OT_VARIABLE)){
			kshttp_response_part_begin(&temp, response_format, "VariableEngProps");
			fr = Ov_SetDynamicVectorValue(&requestOutput, requestOutputDefaultVariable, 8, UINT);
		}else if(anyRequested && (one_result->objtype & KS_OT_LINK)){
			kshttp_response_part_begin(&temp, response_format, "LinkEngProps");
			fr = Ov_SetDynamicVectorValue(&requestOutput, requestOutputDefaultLink, 9, UINT);
		}else if(anyRequested) {
			kshttp_response_part_begin(&temp, response_format, "HistoryAndStructureUnsupported");
			fr = Ov_SetDynamicVectorValue(&requestOutput, NULL, 0, UINT);
		}else{
			kshttp_response_part_begin(&temp, response_format, "dummy");
		}
		if(Ov_Fail(fr)) {
			//should not happen with an UINT
			ov_string_append(re, "internal memory problem");
			fr = OV_ERR_GENERIC;
			kshttp_print_result_array(&message, response_format, &fr, 1, ": memory problem");
			EXEC_GETEP_RETURN fr; //404
		}
		for (i=0;i < requestOutput.veclen;i++){
			if(i >= 1 && !(requestOutput.value[i] == OP_TYPE &&
					(response_format == KSX || response_format == JSON))){
				//OP_TYPE is not displayed in ksx and json, so skip the seperator here
				kshttp_response_parts_seperate(&temp, response_format);
			}
			if(requestOutput.veclen > 1 && response_format==TCL){
				//open request item level, if we have more than one entry (OP_NAME and OP_CREATIONTIME for example)
				//ksx returns always everything
				//kshttp_response_part_begin(&temp, response_format, "dummy");
			}

			//######################### iterate over response ###############
			switch(requestOutput.value[i]){
			case OP_NAME:
				getEP_begin_RequestOutputPart(&temp, response_format, "identifier");
				ov_string_append(&temp, one_result->identifier);
				getEP_finalize_RequestOutputPart(&temp, response_format, "identifier");
				break;
			case OP_CREATIONTIME:
				if(response_format == TCL){
					if(temp == NULL){
						ov_string_setvalue(&temp, "{");
					}else{
						ov_string_append(&temp, "{");
					}
				}
				getEP_begin_RequestOutputPart(&temp, response_format, "creationtime");
				kshttp_timetoascii(&temp2, &(one_result->creation_time), response_format);
				ov_string_append(&temp, temp2);
				ov_string_setvalue(&temp2, NULL);

				getEP_finalize_RequestOutputPart(&temp, response_format, "creationtime");
				if(response_format == TCL){
					ov_string_append(&temp, "}");
				}
				break;
			case OP_CLASS:
				if(one_result->objtype & KS_OT_DOMAIN) {
					getEP_begin_RequestOutputPart(&temp, response_format, "classIdentifier");
					ov_string_append(&temp, one_result->OV_OBJ_ENGINEERED_PROPS_u.domain_engineered_props.class_identifier);
					getEP_finalize_RequestOutputPart(&temp, response_format, "classIdentifier");
				}else if(one_result->objtype & KS_OT_VARIABLE){
					getEP_begin_RequestOutputPart(&temp, response_format, "type");
					switch (one_result->OV_OBJ_ENGINEERED_PROPS_u.var_engineered_props.vartype) {
						case KS_VT_VOID:
							getEP_print_KSmakrovalue(&temp, "KS_VT", "VOID", response_format);
							break;
						case KS_VT_BOOL:
							getEP_print_KSmakrovalue(&temp, "KS_VT", "BOOL", response_format);
							break;
						case KS_VT_INT:
							getEP_print_KSmakrovalue(&temp, "KS_VT", "INT", response_format);
							break;
						case KS_VT_UINT:
							getEP_print_KSmakrovalue(&temp, "KS_VT", "UINT", response_format);
							break;
						case KS_VT_SINGLE:
							getEP_print_KSmakrovalue(&temp, "KS_VT", "SINGLE", response_format);
							break;
						case KS_VT_DOUBLE:
							getEP_print_KSmakrovalue(&temp, "KS_VT", "DOUBLE", response_format);
							break;
						case KS_VT_STRING:
							getEP_print_KSmakrovalue(&temp, "KS_VT", "STRING", response_format);
							break;
						case KS_VT_TIME:
							getEP_print_KSmakrovalue(&temp, "KS_VT", "TIME", response_format);
							break;
						case KS_VT_TIME_SPAN:
							getEP_print_KSmakrovalue(&temp, "KS_VT", "TIME_SPAN", response_format);
							break;
						case KS_VT_STATE:
							getEP_print_KSmakrovalue(&temp, "KS_VT", "STATE", response_format);
							break;
						case KS_VT_STRUCT:
							getEP_print_KSmakrovalue(&temp, "KS_VT", "STRUCT", response_format);
							break;

						case KS_VT_BYTE_VEC:
							getEP_print_KSmakrovalue(&temp, "KS_VT", "BYTE_VEC", response_format);
							break;
						case KS_VT_BOOL_VEC:
							getEP_print_KSmakrovalue(&temp, "KS_VT", "BOOL_VEC", response_format);
							break;
						case KS_VT_INT_VEC:
							getEP_print_KSmakrovalue(&temp, "KS_VT", "INT_VEC", response_format);
							break;
						case KS_VT_UINT_VEC:
							getEP_print_KSmakrovalue(&temp, "KS_VT", "UINT_VEC", response_format);
							break;
						case KS_VT_SINGLE_VEC:
							getEP_print_KSmakrovalue(&temp, "KS_VT", "SINGLE_VEC", response_format);
							break;
						case KS_VT_DOUBLE_VEC:
							getEP_print_KSmakrovalue(&temp, "KS_VT", "DOUBLE_VEC", response_format);
							break;
						case KS_VT_STRING_VEC:
							getEP_print_KSmakrovalue(&temp, "KS_VT", "STRING_VEC", response_format);
							break;
						case KS_VT_TIME_VEC:
							getEP_print_KSmakrovalue(&temp, "KS_VT", "TIME_VEC", response_format);
							break;
						case KS_VT_TIME_SPAN_VEC:
							getEP_print_KSmakrovalue(&temp, "KS_VT", "TIME_SPAN_VEC", response_format);
							break;
						case KS_VT_STATE_VEC:
							getEP_print_KSmakrovalue(&temp, "KS_VT", "STATE_VEC", response_format);
							break;
						default:
							ov_string_append(&temp, "unknown");
							break;
					}
					getEP_finalize_RequestOutputPart(&temp, response_format, "type");
				}else if(one_result->objtype & KS_OT_LINK){
					getEP_begin_RequestOutputPart(&temp, response_format, "type");
					switch (one_result->OV_OBJ_ENGINEERED_PROPS_u.link_engineered_props.linktype) {
						case KS_LT_LOCAL_1_1:
							if(response_format == KSX){
								ov_string_append(&temp, "local-1-1");
							}else{
								ov_string_append(&temp, "KS_LT_LOCAL_1_1");
							}
							break;
						case KS_LT_LOCAL_1_MANY:
							if(response_format == KSX){
								ov_string_append(&temp, "local-1-m");
							}else{
								ov_string_append(&temp, "KS_LT_LOCAL_1_MANY");
							}
							break;
						case KS_LT_LOCAL_MANY_MANY:
							if(response_format == KSX){
								ov_string_append(&temp, "local-m-m");
							}else{
								ov_string_append(&temp, "KS_LT_LOCAL_MANY_MANY");
							}
							break;
						case KS_LT_LOCAL_MANY_1:
							if(response_format == KSX){
								ov_string_append(&temp, "local-m-1");
							}else{
								ov_string_append(&temp, "KS_LT_LOCAL_MANY_1");
							}
							break;
						case KS_LT_GLOBAL_1_1:
							if(response_format == KSX){
								ov_string_append(&temp, "global-1-1");
							}else{
								ov_string_append(&temp, "KS_LT_GLOBAL_1_1");
							}
							break;
						case KS_LT_GLOBAL_1_MANY:
							if(response_format == KSX){
								ov_string_append(&temp, "global-1-m");
							}else{
								ov_string_append(&temp, "KS_LT_GLOBAL_1_MANY");
							}
							break;
						case KS_LT_GLOBAL_MANY_MANY:
							if(response_format == KSX){
								ov_string_append(&temp, "global-m-m");
							}else{
								ov_string_append(&temp, "KS_LT_GLOBAL_MANY_MANY");
							}
							break;
						case KS_LT_GLOBAL_MANY_1:
							if(response_format == KSX){
								ov_string_append(&temp, "global-m-1");
							}else{
								ov_string_append(&temp, "KS_LT_GLOBAL_MANY_1");
							}
							break;
							break;
						default:
							ov_string_append(&temp, "unknown");
							break;
					}
					getEP_finalize_RequestOutputPart(&temp, response_format, "type");
				}
				break;
			case OP_TYPE:
				//ksx has this information in the surrounding XML element
				if(response_format != KSX && response_format != JSON){
					if(one_result->objtype == KS_OT_DOMAIN){
						ov_string_append(&temp, "KS_OT_DOMAIN");
					}else if(one_result->objtype == KS_OT_VARIABLE){
						ov_string_append(&temp, "KS_OT_VARIABLE");
					}else if(one_result->objtype == KS_OT_LINK){
						ov_string_append(&temp, "KS_OT_LINK");
					}else if(one_result->objtype == KS_OT_STRUCTURE){
						ov_string_append(&temp, "KS_OT_STRUCTURE");
					}else if(one_result->objtype == KS_OT_HISTORY){
						ov_string_append(&temp, "KS_OT_HISTORY");
					}
				}
				break;
			case OP_COMMENT:
				if(response_format == TCL){
					if(temp == NULL){
						ov_string_setvalue(&temp, "{");
					}else{
						ov_string_append(&temp, "{");
					}
				}
				getEP_begin_RequestOutputPart(&temp, response_format, "comment");
				if(ov_string_getlength(one_result->comment) < KS_COMMENT_MAXLEN){
					kshttp_escapeString(&temp2, &(one_result->comment), response_format);
					ov_string_append(&temp, temp2);
					ov_string_setvalue(&temp2, NULL);
				}
				getEP_finalize_RequestOutputPart(&temp, response_format, "comment");
				if(response_format == TCL){
					ov_string_append(&temp, "}");
				}
				break;
			case OP_ACCESS:
				if(response_format == TCL){
					if(temp == NULL){
						ov_string_setvalue(&temp, "{");
					}else{
						ov_string_append(&temp, "{");
					}
				}
				getEP_begin_RequestOutputPart(&temp, response_format, "access");
				EntryFound = FALSE;
				if(one_result->access & KS_AC_NONE){
					getEP_print_KSmakrovalue(&temp, "KS_AC", "NONE", response_format);
					EntryFound = TRUE;
				}
				if(one_result->access & KS_AC_READ){	//would be R in Magellan
					if(EntryFound == TRUE){
						ov_string_append(&temp, " ");
					}
					getEP_print_KSmakrovalue(&temp, "KS_AC", "READ", response_format);
					EntryFound = TRUE;
				}
				if(one_result->access & KS_AC_WRITE){	//would be W in Magellan
					if(EntryFound == TRUE){
						ov_string_append(&temp, " ");
					}
					getEP_print_KSmakrovalue(&temp, "KS_AC", "WRITE", response_format);
					EntryFound = TRUE;
				}
				if(one_result->access & KS_AC_INSTANTIABLE){	//would be I in Magellan
					if(EntryFound == TRUE){
						ov_string_append(&temp, " ");
					}
					getEP_print_KSmakrovalue(&temp, "KS_AC", "INSTANTIABLE", response_format);
					EntryFound = TRUE;
				}
				if(one_result->access & KS_AC_PART){	//would be P in Magellan
					if(EntryFound == TRUE){
						ov_string_append(&temp, " ");
					}
					getEP_print_KSmakrovalue(&temp, "KS_AC", "PART", response_format);
					EntryFound = TRUE;
				}
				if(one_result->access & KS_AC_DELETEABLE){	//would be D in Magellan
					if(EntryFound == TRUE){
						ov_string_append(&temp, " ");
					}
					getEP_print_KSmakrovalue(&temp, "KS_AC", "DELETEABLE", response_format);
					EntryFound = TRUE;
				}
				if(one_result->access & KS_AC_RENAMEABLE){	//would be N in Magellan
					if(EntryFound == TRUE){
						ov_string_append(&temp, " ");
					}
					getEP_print_KSmakrovalue(&temp, "KS_AC", "RENAMEABLE", response_format);
					EntryFound = TRUE;
				}
				if(one_result->access & KS_AC_LINKABLE){	//would be L in Magellan
					if(EntryFound == TRUE){
						ov_string_append(&temp, " ");
					}
					getEP_print_KSmakrovalue(&temp, "KS_AC", "LINKABLE", response_format);
					EntryFound = TRUE;
				}
				if(one_result->access & KS_AC_UNLINKABLE){	//would be U in Magellan
					if(EntryFound == TRUE){
						ov_string_append(&temp, " ");
					}
					getEP_print_KSmakrovalue(&temp, "KS_AC", "UNLINKABLE", response_format);
					EntryFound = TRUE;
				}
				getEP_finalize_RequestOutputPart(&temp, response_format, "access");
				if(response_format == TCL){
					ov_string_append(&temp, "}");
				}
				break;
			case OP_SEMANTIC:
				getEP_begin_RequestOutputPart(&temp, response_format, "semantics");
				if(response_format == KSX){
					//could add a char info, as an additional xml element
					ov_string_print(&temp, "%s%i", temp, one_result->semantic_flags);
				}else{
					//semantic_flags is a u_long (32 bit) aka UINT, but a unsigned char for flagiterator is nicer in the debugger
					//and save enough, as we iterate only to 129
					for (flagiterator = 'a';flagiterator < 'a'+32;flagiterator++){
						if(IsFlagSet(one_result->semantic_flags, flagiterator)){
							ov_string_print(&temp, "%s%c", temp, flagiterator);
						}
					}
				}
				getEP_finalize_RequestOutputPart(&temp, response_format, "semantics");
				break;
			case OP_TECHUNIT:
				if(one_result->objtype & KS_OT_VARIABLE) {
					getEP_begin_RequestOutputPart(&temp, response_format, "techunit");
					if(ov_string_getlength(one_result->OV_OBJ_ENGINEERED_PROPS_u.var_engineered_props.tech_unit) < KS_TECHUNIT_MAXLEN){
						kshttp_escapeString(&temp2, &(one_result->OV_OBJ_ENGINEERED_PROPS_u.var_engineered_props.tech_unit), response_format);
						ov_string_append(&temp, temp2);
						ov_string_setvalue(&temp2, NULL);
					}
					getEP_finalize_RequestOutputPart(&temp, response_format, "techunit");
				}
				break;
			case OP_ASSOCIDENT:
				// as a view at an tasklist: could be /acplt/fb/tasklist
				if(one_result->objtype & KS_OT_LINK) {
					getEP_begin_RequestOutputPart(&temp, response_format, "associationIdentifier");
					ov_string_append(&temp, one_result->OV_OBJ_ENGINEERED_PROPS_u.link_engineered_props.association_identifier);
					getEP_finalize_RequestOutputPart(&temp, response_format, "associationIdentifier");
				}
				break;
			case OP_ROLEIDENT:
				// at a view at an tasklist on the LinkEngProps of the "taskparent": could be taskchild (not really needed on 1-m and m-1)
				if(one_result->objtype & KS_OT_LINK) {
					getEP_begin_RequestOutputPart(&temp, response_format, "oppositeRoleIdentifier");
					ov_string_append(&temp, one_result->OV_OBJ_ENGINEERED_PROPS_u.link_engineered_props.opposite_role_identifier);
					getEP_finalize_RequestOutputPart(&temp, response_format, "oppositeRoleIdentifier");
				}
				break;
			default:
				getEP_begin_RequestOutputPart(&temp, response_format, "NOT_IMPLEMENTED");
				ov_string_append(&temp, "NOT IMPLEMENTED");
				fr = OV_ERR_NOTIMPLEMENTED;
				getEP_finalize_RequestOutputPart(&temp, response_format, "NOT_IMPLEMENTED");
				break;
			}
			if(requestOutput.veclen > 1 && response_format==TCL){
				//close request item level, if we have more than one entry
				//kshttp_response_part_finalize(&temp, response_format, "dummy");
			}
		}
		//close Child item level
		if(anyRequested && (one_result->objtype & KS_OT_DOMAIN)){
			kshttp_response_part_finalize(&temp, response_format, "DomainEngProps");
		}else if(anyRequested && (one_result->objtype & KS_OT_VARIABLE)){
			kshttp_response_part_finalize(&temp, response_format, "VariableEngProps");
		}else if(anyRequested && (one_result->objtype & KS_OT_LINK)){
			kshttp_response_part_finalize(&temp, response_format, "LinkEngProps");
		}else if(anyRequested){
			kshttp_response_part_finalize(&temp, response_format, "HistoryAndStructureUnsupported");
		}else{
			kshttp_response_part_finalize(&temp, response_format, "dummy");
		}

		one_result = one_result->pnext;
	}
	ov_memstack_unlock();

	//save our hard work
	ov_string_setvalue(&message, temp);
	ov_string_setvalue(&temp, NULL);

	ov_string_append(re, message);

	EXEC_GETEP_RETURN fr;
}

