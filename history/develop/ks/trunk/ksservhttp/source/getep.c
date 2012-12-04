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

#define EXEC_GETEP_RETURN \
		Ov_SetDynamicVectorLength(&match,0,STRING);\
		ov_string_setvalue(&message, NULL);\
		Ov_SetDynamicVectorLength(&requestOutput,0,UINT);\
		ov_string_setvalue(&temp, NULL);\
		return
OV_RESULT exec_getep(OV_STRING_VEC* args, OV_STRING* re){
	OV_INSTPTR_ov_object pObj = NULL;
	OV_INSTPTR_ov_object pChild = NULL;
	OV_VTBLPTR_ov_object pChildVTable;
	OV_STRING_VEC match = {0,NULL};
	OV_STRING message = NULL;
	OV_UINT requestType = KS_OT_DOMAIN;
	OV_UINT_VEC requestOutput = {0,NULL};
	OV_STRING temp = NULL;
	OV_UINT output_format;
	OV_UINT requestOutputDefault[] = {OP_NAME, OP_TYPE, OP_COMMENT, OP_ACCESS, OP_SEMANTIC, OP_CREATIONTIME, OP_CLASS};
	OV_RESULT fr = OV_ERR_OK;
	int i = 0;

	//path=/TechUnits
	//requestType=OT_DOMAIN|OT_VARIABLE|... (siehe tcl-tks doku)
	//requestOutput or requestOutput[i] with OP_NAME, OP_TYPE, OP_COMMENT, OP_ACCESS, OP_SEMANTIC, OP_CREATIONTIME and OP_CLASS

	output_format = extract_output_format(args);

	find_arguments(args, "path", &match);
	if(match.veclen!=1){
		ov_string_append(re, "Variable path not found");
		EXEC_GETEP_RETURN OV_ERR_BADPARAM; //400
	}
	pObj = ov_path_getobjectpointer(match.value[0],0);
	if (pObj == NULL){
		ov_string_append(re, "getEP: Variable not found");
		EXEC_GETEP_RETURN OV_ERR_BADPATH; //404
	}
	find_arguments(args, "requestType", &match);
	if(match.veclen == 1){
		if(ov_string_compare(match.value[0], "OT_DOMAIN") == OV_STRCMP_EQUAL){
			requestType = KS_OT_DOMAIN;
		}else if(ov_string_compare(match.value[0], "OT_VARIABLE") == OV_STRCMP_EQUAL){
			requestType = KS_OT_VARIABLE;
		}else if(ov_string_compare(match.value[0], "OT_LINK") == OV_STRCMP_EQUAL){
			requestType = KS_OT_LINK;
		}else if(ov_string_compare(match.value[0], "OT_ANY") == OV_STRCMP_EQUAL){
			requestType = KS_OT_ANY;
		}else{
			ov_string_append(re, "Requesttype not supported");
			EXEC_GETEP_RETURN OV_ERR_BADPARAM; //400
		}
	}else{
		//default to OT_DOMAIN
		requestType = KS_OT_DOMAIN;
	}
	find_arguments(args, "requestOutput", &match);
	if(match.veclen == 0 || (match.veclen==1 && ov_string_compare(match.value[0], "OP_ANY") == OV_STRCMP_EQUAL )){
		//if nothing is specified or all is requested, give all
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
			}else{
				requestOutput.value[i] = OP_UNKNOWN;
			}
		}
	}

	//idea: reimplement via ov_element_getnextchild, see at ov_ksserver_getep.c
	if(requestType == KS_OT_DOMAIN && Ov_CanCastTo(ov_domain, pObj)){
		//OT_DOMAIN == ov_containment
		Ov_ForEachChild(ov_containment, Ov_StaticPtrCast(ov_domain, pObj), pChild){
			Ov_GetVTablePtr(ov_object, pChildVTable, pChild);
			//open Child item level
			if(Ov_GetPreviousChild(ov_containment, pChild) != NULL && output_format==GETVAR_FORMAT_TCL){
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
					ov_string_append(&temp, pChild->v_identifier);
					break;
				case OP_CREATIONTIME:
					ov_string_append(&temp, ov_time_timetoascii(&(pChild->v_creationtime)));
					break;
				case OP_CLASS:
					ov_memstack_lock();
					ov_string_append(&temp, ov_path_getcanonicalpath(Ov_StaticPtrCast(ov_object,Ov_GetClassPtr(pChild)),2));
					ov_memstack_unlock();
					break;
				case OP_TYPE:
				case OP_COMMENT:
					//pChildVTable->m_getcomment(pChild, pelem);
				case OP_ACCESS:
				case OP_SEMANTIC:
					/*	if(pChildVTable->m_getaccess(pChild, &pelem, NULL) & OV_AC_INSTANTIABLE) {

					}
				*/
				default:
					//nothing to do
					break;
				}
				if(requestOutput.veclen > 1){
					//close request item level, if we have more than one entry
					finalize_vector_output(&temp, output_format);
				}
			}
			//close Child item level
			finalize_vector_output(&temp, output_format);
		}
		//save our hard work
		ov_string_setvalue(&message, temp);
		ov_string_setvalue(&temp, NULL);
	}else if(requestType == KS_OT_VARIABLE){
		ov_string_append(re, "requestType VARIABLES not implemented");
		EXEC_GETEP_RETURN OV_ERR_NOTIMPLEMENTED; //501
	}else{
		ov_string_append(re, "requestType not implemented");
		EXEC_GETEP_RETURN OV_ERR_NOTIMPLEMENTED; //501
	}
	ov_string_append(re, message);

	EXEC_GETEP_RETURN OV_ERR_OK;
}

