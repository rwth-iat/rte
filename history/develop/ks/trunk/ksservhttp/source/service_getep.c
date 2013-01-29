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
OV_BOOL getEPprintprefixedvalue(OV_STRING *resultstr, OV_STRING prefix, OV_STRING value, OV_UINT response_format){
	OV_STRING changedValue = NULL;
	OV_STRING pointer = NULL;
	OV_UINT i = 0;
	if(response_format == RESPONSE_FORMAT_KSX){
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

static OV_ACCESS ov_ksservhttp_ticket_defaultticket_getaccess(const OV_TICKET *a) {
	return KS_AC_READ;
}
//we need ony a getaccess for the getEP service
OV_DLLVAREXPORT OV_TICKET_VTBL defaultticketvtbl = {
	NULL,
	NULL,
	NULL,
	ov_ksservhttp_ticket_defaultticket_getaccess
};

#define EXEC_GETEP_RETURN \
		Ov_SetDynamicVectorLength(&match,0,STRING);\
		ov_string_setvalue(&message, NULL);\
		Ov_SetDynamicVectorLength(&requestOutput,0,UINT);\
		ov_string_setvalue(&temp, NULL);\
		return
OV_RESULT exec_getep(OV_STRING_VEC* args, OV_STRING* re, OV_UINT response_format){
	OV_STRING_VEC match = {0,NULL};

	OV_GETEP_PAR	params;
	OV_GETEP_RES	result;
	OV_OBJ_ENGINEERED_PROPS	*one_result;

	static OV_TICKET ticket = { &defaultticketvtbl,  OV_TT_NONE };

	OV_STRING message = NULL;
	OV_UINT_VEC requestOutput = {0,NULL};
	OV_STRING temp = NULL;
	OV_BOOL EntryFound = FALSE;
	OV_BOOL anyRequested = FALSE;
	OV_UINT requestOutputDefaultDomain[] = {OP_NAME, OP_TYPE, OP_COMMENT, OP_ACCESS, OP_SEMANTIC, OP_CREATIONTIME, OP_CLASS};
	OV_UINT requestOutputDefaultVariable[] = {OP_NAME, OP_TYPE, OP_COMMENT, OP_ACCESS, OP_SEMANTIC, OP_CREATIONTIME, OP_CLASS, OP_TECHUNIT};
	OV_UINT requestOutputDefaultLink[] = {OP_NAME, OP_TYPE, OP_COMMENT, OP_ACCESS, OP_SEMANTIC, OP_CREATIONTIME, OP_CLASS, OP_ASSOCIDENT, OP_ROLEIDENT};
	OV_RESULT fr = OV_ERR_OK;
	unsigned char flagiterator = 'a';
	int i = 0;

	//path=/TechUnits
	//requestType=OT_DOMAIN|OT_VARIABLE|... (siehe tcl-tks doku)
	//requestOutput or requestOutput[i] with OP_NAME, OP_TYPE, OP_COMMENT, OP_ACCESS, OP_SEMANTIC, OP_CREATIONTIME and OP_CLASS

	/**
	 * Build Parameter for KS function
	 */
	//process path
	Ov_SetDynamicVectorLength(&match,0,STRING);
	find_arguments(args, "path", &match);
	if(match.veclen!=1){
		begin_vector_output(re, response_format, "failure");
		if(response_format == RESPONSE_FORMAT_KSX){
			ov_string_print(&temp, "%i", OV_ERR_BADPARAM);
		}else{
			ov_string_print(&temp, "Path not found or multiple path given");
		}
		finalize_vector_output(&temp, response_format, "failure");
		ov_string_append(re, temp);
		EXEC_GETEP_RETURN OV_ERR_BADPARAM; //400
	}

	//initialize ov_string
	params.path = NULL;
	params.name_mask = NULL;

	ov_string_setvalue(&params.path, match.value[0]);
	ov_string_setvalue(&params.name_mask, "*");
	params.scope_flags = KS_EPF_DEFAULT;

	find_arguments(args, "requestType", &match);
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
			begin_vector_output(re, response_format, "failure");
			if(response_format == RESPONSE_FORMAT_KSX){
				ov_string_print(&temp, "%i", OV_ERR_BADPARAM);
			}else{
				ov_string_print(&temp, "Requesttype not supported");
			}
			finalize_vector_output(&temp, response_format, "failure");
			ov_string_append(re, temp);
			EXEC_GETEP_RETURN OV_ERR_BADPARAM; //400
		}
	}else{
		//default to OT_DOMAIN
		params.type_mask = KS_OT_DOMAIN;
	}

	find_arguments(args, "requestOutput", &match);
	if(response_format == RESPONSE_FORMAT_KSX || match.veclen == 0 || (match.veclen==1 && ov_string_compare(match.value[0], "OP_ANY") == OV_STRCMP_EQUAL )){
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

	ov_memstack_lock(); //needed for ov_path_resolve and the class_identifier
	ov_ksserver_getep(2, &ticket, &params, &result);

	if(Ov_Fail(result.result)){
		//general problem like memory problem or NOACCESS
		ov_memstack_unlock();
		fr = result.result;
		begin_vector_output(re, response_format, "failure");
		if(response_format == RESPONSE_FORMAT_KSX){
			ov_string_print(&temp, "%i", fr);
		}else{
			ov_string_print(&temp, "problem: %s", ov_result_getresulttext(fr));
		}
		finalize_vector_output(&temp, response_format, "failure");
		ov_string_append(re, temp);
		EXEC_GETEP_RETURN result.result;
	}

	one_result = result.pfirst;
	while(one_result != NULL){
		//open Child item level
		if(result.pfirst != one_result && response_format==RESPONSE_FORMAT_TCL){
			//append here a space to maintain compatibility with tcl format handling
			ov_string_append(&temp, " ");
		}
		//change target output
		if(anyRequested && (one_result->objtype & KS_OT_DOMAIN)){
			begin_vector_output(&temp, response_format, "DomainEngProps");
			fr = Ov_SetDynamicVectorValue(&requestOutput, requestOutputDefaultDomain, 7, UINT);
		}else if(anyRequested && (one_result->objtype & KS_OT_VARIABLE)){
			begin_vector_output(&temp, response_format, "VariableEngProps");
			fr = Ov_SetDynamicVectorValue(&requestOutput, requestOutputDefaultVariable, 8, UINT);
		}else if(anyRequested && (one_result->objtype & KS_OT_LINK)){
			begin_vector_output(&temp, response_format, "LinkEngProps");
			fr = Ov_SetDynamicVectorValue(&requestOutput, requestOutputDefaultLink, 9, UINT);
		}else {
			begin_vector_output(&temp, response_format, "HistoryStructureUnsupported");
		}
		if(Ov_Fail(fr)) {
			//should not happen with an UINT
			ov_string_append(re, "internal memory problem");
			EXEC_GETEP_RETURN OV_ERR_GENERIC; //404
		}
		for (i=0;i < requestOutput.veclen;i++){
			if(i >= 1 && response_format==RESPONSE_FORMAT_TCL){
				//append here a space to maintain compatibility with tcl format
				ov_string_append(&temp, " ");
			}
			if(requestOutput.veclen > 1 && response_format==RESPONSE_FORMAT_TCL){
				//open request item level, if we have more than one entry
				begin_vector_output(&temp, response_format, "");
			}

			//######################### iterate over response ###############
			switch(requestOutput.value[i]){
			case OP_NAME:
				begin_vector_output(&temp, response_format, "identifier");
				ov_string_append(&temp, one_result->identifier);
				finalize_vector_output(&temp, response_format, "identifier");
				break;
			case OP_CREATIONTIME:
				//fixme ksx wants 2002-02-02T02:02:02.123
				begin_vector_output(&temp, response_format, "creationTimestamp");
				ov_string_append(&temp, ov_time_timetoascii(&(one_result->creation_time)));
				finalize_vector_output(&temp, response_format, "creationTimestamp");
				break;
			case OP_CLASS:
				if(one_result->objtype & KS_OT_DOMAIN) {
					begin_vector_output(&temp, response_format, "classIdentifier");
					ov_string_append(&temp, one_result->OV_OBJ_ENGINEERED_PROPS_u.domain_engineered_props.class_identifier);
					finalize_vector_output(&temp, response_format, "classIdentifier");
				}else if(one_result->objtype & KS_OT_VARIABLE){
					begin_vector_output(&temp, response_format, "type");
					switch (one_result->OV_OBJ_ENGINEERED_PROPS_u.var_engineered_props.vartype) {
						case KS_VT_VOID:
							getEPprintprefixedvalue(&temp, "KS_VT", "VOID", response_format);
							break;
						case KS_VT_BOOL:
							getEPprintprefixedvalue(&temp, "KS_VT", "BOOL", response_format);
							break;
						case KS_VT_INT:
							getEPprintprefixedvalue(&temp, "KS_VT", "INT", response_format);
							break;
						case KS_VT_UINT:
							getEPprintprefixedvalue(&temp, "KS_VT", "UINT", response_format);
							break;
						case KS_VT_SINGLE:
							getEPprintprefixedvalue(&temp, "KS_VT", "SINGLE", response_format);
							break;
						case KS_VT_DOUBLE:
							getEPprintprefixedvalue(&temp, "KS_VT", "DOUBLE", response_format);
							break;
						case KS_VT_STRING:
							getEPprintprefixedvalue(&temp, "KS_VT", "STRING", response_format);
							break;
						case KS_VT_TIME:
							getEPprintprefixedvalue(&temp, "KS_VT", "TIME", response_format);
							break;
						case KS_VT_TIME_SPAN:
							getEPprintprefixedvalue(&temp, "KS_VT", "TIME_SPAN", response_format);
							break;
						case KS_VT_STATE:
							getEPprintprefixedvalue(&temp, "KS_VT", "STATE", response_format);
							break;
						case KS_VT_STRUCT:
							getEPprintprefixedvalue(&temp, "KS_VT", "STRUCT", response_format);
							break;

						case KS_VT_BYTE_VEC:
							getEPprintprefixedvalue(&temp, "KS_VT", "BYTE_VEC", response_format);
							break;
						case KS_VT_BOOL_VEC:
							getEPprintprefixedvalue(&temp, "KS_VT", "BOOL_VEC", response_format);
							break;
						case KS_VT_INT_VEC:
							getEPprintprefixedvalue(&temp, "KS_VT", "INT_VEC", response_format);
							break;
						case KS_VT_UINT_VEC:
							getEPprintprefixedvalue(&temp, "KS_VT", "UINT_VEC", response_format);
							break;
						case KS_VT_SINGLE_VEC:
							getEPprintprefixedvalue(&temp, "KS_VT", "SINGLE_VEC", response_format);
							break;
						case KS_VT_DOUBLE_VEC:
							getEPprintprefixedvalue(&temp, "KS_VT", "DOUBLE_VEC", response_format);
							break;
						case KS_VT_STRING_VEC:
							getEPprintprefixedvalue(&temp, "KS_VT", "STRING_VEC", response_format);
							break;
						case KS_VT_TIME_VEC:
							getEPprintprefixedvalue(&temp, "KS_VT", "TIME_VEC", response_format);
							break;
						case KS_VT_TIME_SPAN_VEC:
							getEPprintprefixedvalue(&temp, "KS_VT", "TIME_SPAN_VEC", response_format);
							break;
						case KS_VT_STATE_VEC:
							getEPprintprefixedvalue(&temp, "KS_VT", "STATE_VEC", response_format);
							break;
						default:
							ov_string_append(&temp, "unknown");
							break;
					}
					finalize_vector_output(&temp, response_format, "type");
				}else if(one_result->objtype & KS_OT_LINK){
					begin_vector_output(&temp, response_format, "type");
					switch (one_result->OV_OBJ_ENGINEERED_PROPS_u.link_engineered_props.linktype) {
						case KS_LT_LOCAL_1_1:
							if(response_format == RESPONSE_FORMAT_KSX){
								ov_string_append(&temp, "local-1-1");
							}else{
								ov_string_append(&temp, "KS_LT_LOCAL_1_1");
							}
							break;
						case KS_LT_LOCAL_1_MANY:
							if(response_format == RESPONSE_FORMAT_KSX){
								ov_string_append(&temp, "local-1-m");
							}else{
								ov_string_append(&temp, "KS_LT_LOCAL_1_MANY");
							}
							break;
						case KS_LT_LOCAL_MANY_MANY:
							if(response_format == RESPONSE_FORMAT_KSX){
								ov_string_append(&temp, "local-m-m");
							}else{
								ov_string_append(&temp, "KS_LT_LOCAL_MANY_MANY");
							}
							break;
						case KS_LT_LOCAL_MANY_1:
							if(response_format == RESPONSE_FORMAT_KSX){
								ov_string_append(&temp, "local-m-1");
							}else{
								ov_string_append(&temp, "KS_LT_LOCAL_MANY_1");
							}
							break;
						case KS_LT_GLOBAL_1_1:
							if(response_format == RESPONSE_FORMAT_KSX){
								ov_string_append(&temp, "global-1-1");
							}else{
								ov_string_append(&temp, "KS_LT_GLOBAL_1_1");
							}
							break;
						case KS_LT_GLOBAL_1_MANY:
							if(response_format == RESPONSE_FORMAT_KSX){
								ov_string_append(&temp, "global-1-m");
							}else{
								ov_string_append(&temp, "KS_LT_GLOBAL_1_MANY");
							}
							break;
						case KS_LT_GLOBAL_MANY_MANY:
							if(response_format == RESPONSE_FORMAT_KSX){
								ov_string_append(&temp, "global-m-m");
							}else{
								ov_string_append(&temp, "KS_LT_GLOBAL_MANY_MANY");
							}
							break;
						case KS_LT_GLOBAL_MANY_1:
							if(response_format == RESPONSE_FORMAT_KSX){
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
					finalize_vector_output(&temp, response_format, "type");
				}
				break;
			case OP_TYPE:
				//ksx has this information in the surrounding XML element
				if(response_format != RESPONSE_FORMAT_KSX){
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
				begin_vector_output(&temp, response_format, "comment");
				ov_string_append(&temp, one_result->comment);
				finalize_vector_output(&temp, response_format, "comment");
				break;
			case OP_ACCESS:
				begin_vector_output(&temp, response_format, "access");
				EntryFound = FALSE;
				if(one_result->access & KS_AC_NONE){
					getEPprintprefixedvalue(&temp, "KS_AC", "NONE", response_format);
					EntryFound = TRUE;
				}
				if(one_result->access & KS_AC_READ){
					if(EntryFound == TRUE){
						ov_string_append(&temp, " ");
					}
					getEPprintprefixedvalue(&temp, "KS_AC", "READ", response_format);
					EntryFound = TRUE;
				}
				if(one_result->access & KS_AC_WRITE){
					if(EntryFound == TRUE){
						ov_string_append(&temp, " ");
					}
					getEPprintprefixedvalue(&temp, "KS_AC", "WRITE", response_format);
					EntryFound = TRUE;
				}
				if(one_result->access & KS_AC_INSTANTIABLE){
					if(EntryFound == TRUE){
						ov_string_append(&temp, " ");
					}
					getEPprintprefixedvalue(&temp, "KS_AC", "INSTANTIABLE", response_format);
					EntryFound = TRUE;
				}
				if(one_result->access & KS_AC_PART){
					if(EntryFound == TRUE){
						ov_string_append(&temp, " ");
					}
					getEPprintprefixedvalue(&temp, "KS_AC", "PART", response_format);
					EntryFound = TRUE;
				}
				if(one_result->access & KS_AC_DELETEABLE){
					if(EntryFound == TRUE){
						ov_string_append(&temp, " ");
					}
					getEPprintprefixedvalue(&temp, "KS_AC", "DELETEABLE", response_format);
					EntryFound = TRUE;
				}
				if(one_result->access & KS_AC_RENAMEABLE){
					if(EntryFound == TRUE){
						ov_string_append(&temp, " ");
					}
					getEPprintprefixedvalue(&temp, "KS_AC", "RENAMEABLE", response_format);
					EntryFound = TRUE;
				}
				if(one_result->access & KS_AC_LINKABLE){
					if(EntryFound == TRUE){
						ov_string_append(&temp, " ");
					}
					getEPprintprefixedvalue(&temp, "KS_AC", "LINKABLE", response_format);
					EntryFound = TRUE;
				}
				if(one_result->access & KS_AC_UNLINKABLE){
					if(EntryFound == TRUE){
						ov_string_append(&temp, " ");
					}
					getEPprintprefixedvalue(&temp, "KS_AC", "UNLINKABLE", response_format);
					EntryFound = TRUE;
				}
				finalize_vector_output(&temp, response_format, "access");
				break;
			case OP_SEMANTIC:
				begin_vector_output(&temp, response_format, "semantics");
				if(response_format == RESPONSE_FORMAT_KSX){
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
				finalize_vector_output(&temp, response_format, "semantics");
				break;
			case OP_TECHUNIT:
				if(one_result->objtype & KS_OT_VARIABLE) {
					begin_vector_output(&temp, response_format, "techunit");
					ov_string_append(&temp, one_result->OV_OBJ_ENGINEERED_PROPS_u.var_engineered_props.tech_unit);
					finalize_vector_output(&temp, response_format, "techunit");
				}
				break;
			case OP_ASSOCIDENT:
				// as a view at an connection: could be /acplt/fb/inputconnections
				if(one_result->objtype & KS_OT_LINK) {
					begin_vector_output(&temp, response_format, "associationIdentifier");
					ov_string_append(&temp, one_result->OV_OBJ_ENGINEERED_PROPS_u.link_engineered_props.association_identifier);
					finalize_vector_output(&temp, response_format, "associationIdentifier");
				}
				break;
			case OP_ROLEIDENT:
				// as a view at an connection: could be inputcon
				if(one_result->objtype & KS_OT_LINK) {
					begin_vector_output(&temp, response_format, "oppositeRoleIdentifier");
					ov_string_append(&temp, one_result->OV_OBJ_ENGINEERED_PROPS_u.link_engineered_props.opposite_role_identifier);
					finalize_vector_output(&temp, response_format, "oppositeRoleIdentifier");
				}
				break;
			default:
				begin_vector_output(&temp, response_format, "NOT_IMPLEMENTED");
				ov_string_append(&temp, "NOT IMPLEMENTED");
				fr = OV_ERR_NOTIMPLEMENTED;
				finalize_vector_output(&temp, response_format, "NOT_IMPLEMENTED");
				break;
			}
			if(requestOutput.veclen > 1 && response_format==RESPONSE_FORMAT_TCL){
				//close request item level, if we have more than one entry
				finalize_vector_output(&temp, response_format, "");
			}
		}
		//close Child item level
		if(anyRequested && (one_result->objtype & KS_OT_DOMAIN)){
			finalize_vector_output(&temp, response_format, "DomainEngProps");
		}else if(anyRequested && (one_result->objtype & KS_OT_VARIABLE)){
			finalize_vector_output(&temp, response_format, "VariableEngProps");
		}else if(anyRequested && (one_result->objtype & KS_OT_LINK)){
			finalize_vector_output(&temp, response_format, "LinkEngProps");
		}else {
			finalize_vector_output(&temp, response_format, "HistoryStructureUnsupported");
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

