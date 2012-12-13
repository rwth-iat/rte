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
OV_RESULT exec_getep(OV_STRING_VEC* args, OV_STRING* re){
	OV_STRING_VEC match = {0,NULL};

	OV_GETEP_PAR	params;
	OV_GETEP_RES	result;
	OV_OBJ_ENGINEERED_PROPS	*one_result;

	static OV_TICKET ticket = { &defaultticketvtbl,  OV_TT_NONE };

	OV_STRING message = NULL;
	OV_UINT_VEC requestOutput = {0,NULL};
	OV_STRING temp = NULL;
	OV_BOOL EntryFound = FALSE;
	OV_UINT output_format;
	OV_UINT requestOutputDefault[] = {OP_NAME, OP_TYPE, OP_COMMENT, OP_ACCESS, OP_SEMANTIC, OP_CREATIONTIME, OP_CLASS};
	OV_RESULT fr = OV_ERR_OK;
	OV_STRING usableflags = "abcdefghijklmnopqrstuvwxyz";
	OV_UINT usableflags_length = ov_string_getlength(usableflags);
	int i = 0;
	int j = 0;

	//path=/TechUnits
	//requestType=OT_DOMAIN|OT_VARIABLE|... (siehe tcl-tks doku)
	//requestOutput or requestOutput[i] with OP_NAME, OP_TYPE, OP_COMMENT, OP_ACCESS, OP_SEMANTIC, OP_CREATIONTIME and OP_CLASS

	output_format = extract_output_format(args);

	/**
	 * Build Parameter for KS function
	 */
	//process path
	Ov_SetDynamicVectorLength(&match,0,STRING);
	find_arguments(args, "path", &match);
	if(match.veclen!=1){
		ov_string_append(re, "Path not found or multiple path given");
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
			ov_string_append(re, "Requesttype not supported");
			EXEC_GETEP_RETURN OV_ERR_BADPARAM; //400
		}
	}else{
		//default to OT_DOMAIN
		params.type_mask = KS_OT_DOMAIN;
	}

	find_arguments(args, "requestOutput", &match);
	if(match.veclen == 0 || (match.veclen==1 && ov_string_compare(match.value[0], "OP_ANY") == OV_STRCMP_EQUAL )){
		//if nothing is specified or all is requested, give all
		//fixme ANY muss an 8. Stelle bei Variablen TECHUNIT, bei Links ASSOCIDENT und an 9. Stelle ROLEIDENT ausgeben
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
		ov_string_setvalue(&temp, ov_result_getresulttext(result.result));
		ov_memstack_unlock();
		EXEC_GETEP_RETURN result.result;
	}

	one_result = result.pfirst;
	while(one_result != NULL){

		//open Child item level
		if(result.pfirst != one_result && output_format==GETVAR_FORMAT_TCL){
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
				ov_string_append(&temp, one_result->identifier);
				break;
			case OP_CREATIONTIME:
				ov_string_append(&temp, ov_time_timetoascii(&(one_result->creation_time)));
				break;
			case OP_CLASS:
				if(one_result->objtype & KS_OT_DOMAIN) {
					ov_string_append(&temp, one_result->OV_OBJ_ENGINEERED_PROPS_u.domain_engineered_props.class_identifier);
				}else if(one_result->objtype & KS_OT_VARIABLE){
					ov_string_append(&temp, "vartype");
					//fixme, klartext erstellen: KS_VT_BOOL
					ov_string_print(&temp, "%s%d", temp, one_result->OV_OBJ_ENGINEERED_PROPS_u.var_engineered_props.vartype);
				}else if(one_result->objtype & KS_OT_LINK){
					ov_string_append(&temp, "linktype");
					//fixme, klartext erstellen: KS_LT_GLOBAL_1_MANY
					ov_string_print(&temp, "%s%d", temp, one_result->OV_OBJ_ENGINEERED_PROPS_u.link_engineered_props.linktype);
					//ov_string_append(&temp, one_result->OV_OBJ_ENGINEERED_PROPS_u.link_engineered_props.association_identifier);
				}
				break;
			case OP_TYPE:
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
				break;
			case OP_COMMENT:
				ov_string_append(&temp, one_result->comment);
				break;
			case OP_ACCESS:
				EntryFound = FALSE;
				if(one_result->access & KS_AC_NONE){
					ov_string_append(&temp, "KS_AC_NONE");
					EntryFound = TRUE;
				}
				if(one_result->access & KS_AC_READ){
					if(EntryFound == TRUE){
						ov_string_append(&temp, " ");
					}
					ov_string_append(&temp, "KS_AC_READ");
					EntryFound = TRUE;
				}
				if(one_result->access & KS_AC_WRITE){
					if(EntryFound == TRUE){
						ov_string_append(&temp, " ");
					}
					ov_string_append(&temp, "KS_AC_WRITE");
					EntryFound = TRUE;
				}
				if(one_result->access & KS_AC_DELETEABLE){
					if(EntryFound == TRUE){
						ov_string_append(&temp, " ");
					}
					ov_string_append(&temp, "KS_AC_DELETEABLE");
					EntryFound = TRUE;
				}
				if(one_result->access & KS_AC_RENAMEABLE){
					if(EntryFound == TRUE){
						ov_string_append(&temp, " ");
					}
					ov_string_append(&temp, "KS_AC_RENAMEABLE");
					EntryFound = TRUE;
				}
				if(one_result->access & KS_AC_LINKABLE){
					if(EntryFound == TRUE){
						ov_string_append(&temp, " ");
					}
					ov_string_append(&temp, "KS_AC_LINKABLE");
					EntryFound = TRUE;
				}
				if(one_result->access & KS_AC_UNLINKABLE){
					if(EntryFound == TRUE){
						ov_string_append(&temp, " ");
					}
					ov_string_append(&temp, "KS_AC_UNLINKABLE");
					EntryFound = TRUE;
				}
				if(one_result->access & KS_AC_INSTANTIABLE){
					if(EntryFound == TRUE){
						ov_string_append(&temp, " ");
					}
					ov_string_append(&temp, "KS_AC_INSTANTIABLE");
					EntryFound = TRUE;
				}
				break;
			case OP_SEMANTIC:
				for (j = 0;j < usableflags_length;j++){
					if(IsFlagSet(one_result->semantic_flags, usableflags[j])){
						ov_string_print(&temp, "%s%c", temp, usableflags[j]);
					}
				}
				break;
			case OP_TECHUNIT:
				if(one_result->objtype & KS_OT_VARIABLE) {
					ov_string_append(&temp, one_result->OV_OBJ_ENGINEERED_PROPS_u.var_engineered_props.tech_unit);
				}
				break;
			case OP_ASSOCIDENT:
				if(one_result->objtype & KS_OT_LINK) {
					ov_string_append(&temp, one_result->OV_OBJ_ENGINEERED_PROPS_u.link_engineered_props.association_identifier);
				}
				break;
			case OP_ROLEIDENT:
				if(one_result->objtype & KS_OT_LINK) {
					ov_string_append(&temp, one_result->OV_OBJ_ENGINEERED_PROPS_u.link_engineered_props.opposite_role_identifier);
				}
				break;
			default:
				ov_string_append(&temp, "NOT IMPLEMENTED");
				fr = OV_ERR_NOTIMPLEMENTED;
				break;
			}
			if(requestOutput.veclen > 1){
				//close request item level, if we have more than one entry
				finalize_vector_output(&temp, output_format);
			}
		}
		//close Child item level
		finalize_vector_output(&temp, output_format);

		one_result = one_result->pnext;
	}
	ov_memstack_unlock();

	//save our hard work
	ov_string_setvalue(&message, temp);
	ov_string_setvalue(&temp, NULL);

	ov_string_append(re, message);

	EXEC_GETEP_RETURN fr;
}

