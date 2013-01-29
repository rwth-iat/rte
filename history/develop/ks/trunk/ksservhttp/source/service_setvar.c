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

#define CHECK_BOOLTRUE(Value)	\
			ov_string_compare(Value, "1")		== OV_STRCMP_EQUAL	\
		||	ov_string_compare(Value, "T")	== OV_STRCMP_EQUAL	\
		||	ov_string_compare(Value, "t")	== OV_STRCMP_EQUAL	\
		||	ov_string_compare(Value, "TRUE")	== OV_STRCMP_EQUAL	\
		||	ov_string_compare(Value, "true")	== OV_STRCMP_EQUAL	\
		||	ov_string_compare(Value, "True")	== OV_STRCMP_EQUAL

#define CHECK_BOOLFALSE(Value)	\
			ov_string_compare(Value, "0")			== OV_STRCMP_EQUAL	\
		||	ov_string_compare(Value, "F")	== OV_STRCMP_EQUAL	\
		||	ov_string_compare(Value, "f")	== OV_STRCMP_EQUAL	\
		||	ov_string_compare(Value, "FALSE")	== OV_STRCMP_EQUAL	\
		||	ov_string_compare(Value, "false")	== OV_STRCMP_EQUAL	\
		||	ov_string_compare(Value, "False")	== OV_STRCMP_EQUAL


#define EXEC_SETVAR_RETURN ov_string_freelist(pPathList); \
		Ov_SetDynamicVectorLength(&match,0,STRING);\
		ov_string_setvalue(&message, NULL);\
		ov_string_setvalue(&prefix, NULL);\
		return

OV_RESULT exec_setvar(OV_STRING_VEC* args, OV_STRING* re, OV_UINT response_format){
	OV_STRING *pPathList = NULL;
	OV_UINT len;
	int i;
	OV_STRING_VEC match = {0,NULL};
	OV_RESULT result;
	OV_STRING message = NULL;
	OV_STRING prefix = NULL;

	find_arguments(args, "path", &match);
	if(match.veclen<1){
		begin_vector_output(re, response_format, "failure");
		if(response_format == RESPONSE_FORMAT_KSX){
			ov_string_print(&message, "%i", OV_ERR_BADPARAM);
		}else{
			ov_string_print(&message, "Variable path not found");
		}
		finalize_vector_output(&message, response_format, "failure");
		ov_string_append(re, message);
		EXEC_SETVAR_RETURN OV_ERR_BADPARAM; //400
	}
	pPathList = ov_string_split(match.value[0], ".", &len);
	if(len < 2){
		ov_string_freelist(pPathList);
		ov_string_append(re, "Variablename must contain a dot");
		EXEC_SETVAR_RETURN OV_ERR_BADPARAM; //400
	}else if(len == 2){
		ov_string_setvalue(&prefix, pPathList[0]);
	}else{
		ov_string_setvalue(&prefix, pPathList[0]);
		for(i=1;i < len-1;i++){
			ov_string_print(&prefix, "%s.%s", prefix, pPathList[i]);
		}
	}
	find_arguments(args, "newvalue", &match);
	if(match.veclen!=1){
		begin_vector_output(re, response_format, "failure");
		if(response_format == RESPONSE_FORMAT_KSX){
			ov_string_print(&message, "%i", OV_ERR_BADPARAM);
		}else{
			ov_string_print(&message, "Variable newvalue not found");
		}
		finalize_vector_output(&message, response_format, "failure");
		ov_string_append(re, message);
		EXEC_SETVAR_RETURN OV_ERR_BADPARAM; //400;
	}
	result = setvar_at_object(ov_path_getobjectpointer(prefix,2),&(pPathList[len-1]),&match.value[0], &message);

	ov_string_append(re, message);

	EXEC_SETVAR_RETURN result;
}

#define SETVAR_AT_OBJECT_RETURN	\
	ov_string_setvalue(&Temp, NULL);\
	return

OV_RESULT setvar_at_object(OV_INSTPTR_ov_object pObj, OV_STRING* varname, OV_STRING* newcontent, OV_STRING* message){
	//	local Pointer
	//
	OV_VTBLPTR_ov_object
		pOvVTable = NULL;

	//	local variables
	//
	OV_RESULT	fr = OV_ERR_OK;

	OV_STRING	Temp = NULL;


	OV_ELEMENT	ParentElement;
	OV_ELEMENT	PartElement;
	OV_ANY		Variable;
	OV_STRING *pArgumentList = NULL;
	OV_UINT len = 0;
	OV_UINT i = 0;

	Variable.value.vartype = OV_VT_VOID;
	//	get VTable
	//
	Ov_GetVTablePtr(ov_object, pOvVTable, pObj);
	if (pOvVTable == NULL)
	{
//			ov_logfile_error("%s:%d - %s - Could not retrieve VTable-Pointer", __FILE__, __LINE__,
//				pObj->v_identifier);
		ov_string_append(message, "Object does not exist\n");
		SETVAR_AT_OBJECT_RETURN OV_ERR_BADPATH;
	};

	ParentElement.elemtype	= OV_ET_OBJECT;
	ParentElement.pobj		= pObj;

	//	search Variable-Element with corresponding name in OV-Object
	//
	fr = ov_element_searchpart(&ParentElement, &PartElement, OV_ET_VARIABLE, *varname);
	if (Ov_Fail(fr))
	{
		ov_string_append(message, "Object does not exist\n");
		SETVAR_AT_OBJECT_RETURN fr;
	};
	if(!(pOvVTable->m_getaccess(pObj, &PartElement, NULL) & OV_AC_WRITE)) {
		return OV_ERR_NOACCESS;
	}

	//	fill Variable with active Content of OV-Object
	//
	ov_memstack_lock();
		fr = pOvVTable->m_getvar(pObj, &PartElement, &Variable);
	ov_memstack_unlock();
	if (Ov_Fail(fr))
	{
		ov_string_append(message, "Object exists, but variable does not\n");
		SETVAR_AT_OBJECT_RETURN fr;
	};

	if (ov_string_compare(*varname, "identifier") == OV_STRCMP_EQUAL){
		//	new name is empty? TODO only via KS_RENAME?
		//
		if (ov_string_compare(*newcontent, NULL) == OV_STRCMP_EQUAL)
		{
			ov_string_append(message, "Submitted new name for object was empty\n");
			SETVAR_AT_OBJECT_RETURN OV_ERR_BADNAME; //503
		}

		//	rename Object
		//
		fr = ov_class_renameobject(Ov_StaticPtrCast(ov_object, pObj), Ov_GetParent(ov_containment, pObj), *newcontent, OV_PMH_END, NULL);
		if (Ov_Fail(fr)){
			ov_string_append(message, "Renaming object failed\n");
			SETVAR_AT_OBJECT_RETURN OV_ERR_GENERIC; //503
		}
		SETVAR_AT_OBJECT_RETURN OV_ERR_GENERIC;
	}


	//	OV_VT_x_PV is an ANY which is set to x
	//
	switch (Variable.value.vartype){
		case OV_VT_BYTE:
		case (OV_VT_BYTE | OV_VT_HAS_STATE | OV_VT_HAS_TIMESTAMP):
			fr = ov_string_setvalue(&Temp, *newcontent);
			//fixme klappt dies? Sehe da beim Speicher Probleme
			Variable.value.valueunion.val_byte = Temp[ov_string_getlength(Temp)-1];
			break;

		case OV_VT_BOOL:
		case OV_VT_BOOL_PV:
			if (CHECK_BOOLTRUE(*newcontent)){
				Variable.value.valueunion.val_bool = TRUE;
			}else if (CHECK_BOOLFALSE(*newcontent)){
				Variable.value.valueunion.val_bool = FALSE;
			}else{
				ov_string_append(message, "Input not detected as bool\n");
				SETVAR_AT_OBJECT_RETURN OV_ERR_BADPARAM;
			}
			break;

		case OV_VT_INT:
		case OV_VT_INT_PV:
			Variable.value.valueunion.val_int = (OV_INT) strtol(*newcontent,NULL,10);
		break;

		case OV_VT_UINT:
		case OV_VT_UINT_PV:
			Variable.value.valueunion.val_uint = (OV_UINT) strtoul(*newcontent,NULL,10);
		break;

		case OV_VT_SINGLE:
		case OV_VT_SINGLE_PV:
			Variable.value.valueunion.val_single = (OV_SINGLE) atof(*newcontent);
		break;

		case OV_VT_DOUBLE:
		case OV_VT_DOUBLE_PV:
			Variable.value.valueunion.val_double = (OV_DOUBLE) atof(*newcontent);
		break;

		case OV_VT_STRING:
		case OV_VT_STRING_PV:
			//setting the content of the pointer to null
			//otherwise setvalue() crashes as it wants to free memory from a garbage pointer
			Variable.value.valueunion.val_string = NULL;
			fr = ov_string_setvalue(&Variable.value.valueunion.val_string, *newcontent);
			if (Ov_Fail(fr)){
				ov_string_append(message, "Setting string value failed");
				SETVAR_AT_OBJECT_RETURN fr;
			};
			break;

		case OV_VT_VOID:
		case OV_VT_VOID | OV_VT_HAS_STATE | OV_VT_HAS_TIMESTAMP:
			ov_string_append(message, "");
			break;

		case OV_VT_TIME:
		case OV_VT_TIME | OV_VT_HAS_STATE | OV_VT_HAS_TIMESTAMP:
			fr = ov_time_asciitotime(&Variable.value.valueunion.val_time, *newcontent);
			if (Ov_Fail(fr)){
				ov_string_append(message, "Setting time value failed");
				SETVAR_AT_OBJECT_RETURN fr;
			};
			break;

			/*	TODO	implement this
		case OV_VT_TIME_SPAN:
		case OV_VT_TIME_SPAN | OV_VT_HAS_STATE | OV_VT_HAS_TIMESTAMP:
			//easy implementation by copying ov_time_asciitotime
			fr = ov_time_asciitotime(&Variable.value.valueunion.val_time_span, *newcontent);
			if (Ov_Fail(fr)){
				ov_string_append(message, "Setting time span value failed");
				SETVAR_AT_OBJECT_RETURN fr;
			};
			break;

		case OV_VT_STATE:
			ov_logfile_debug("%s:%d OV_VT_STATE", __FILE__, __LINE__);
		break;

		case OV_VT_STRUCT:
			ov_logfile_debug("%s:%d OV_VT_STRUCT", __FILE__, __LINE__);
		break;
*/
		//****************** VEC: *******************
		/* request could be "{1}%20{10}"
		 * split at "%20", discard the "{" via pointer arithmetic (+1)
		 * the strtol and atof commands ignores the last "}"
		 * with STRING_VEC we have to do some more
		 */

/*		case OV_VT_BYTE_VEC:
		case (OV_VT_BYTE_VEC | OV_VT_HAS_STATE | OV_VT_HAS_TIMESTAMP):
*/

		case OV_VT_BOOL_VEC:
		case OV_VT_BOOL_PV_VEC:
			pArgumentList = ov_string_split(*newcontent, "%20", &len);
			Ov_SetDynamicVectorLength(&Variable.value.valueunion.val_bool_vec, len, BOOL);
			for(i = 0; i < len; i++){
				//killing the first character
				ov_string_setvalue(&Temp, pArgumentList[i]+1);
				//kill the last character, now we have two null bytes at the end
				Temp[ov_string_getlength(Temp)-1] = '\0';

				if (CHECK_BOOLTRUE(Temp)){
					Variable.value.valueunion.val_bool_vec.value[i] = TRUE;
				}else if (CHECK_BOOLFALSE(Temp)){
					Variable.value.valueunion.val_bool_vec.value[i] = FALSE;
				}else{
					//default
					Variable.value.valueunion.val_bool_vec.value[i] = FALSE;
				}
			}
			ov_string_freelist(pArgumentList);
			break;

		case OV_VT_INT_VEC:
		case OV_VT_INT_PV_VEC:
			pArgumentList = ov_string_split(*newcontent, "%20", &len);
			Ov_SetDynamicVectorLength(&Variable.value.valueunion.val_int_vec, len, INT);
			for(i = 0; i < len; i++){
				Variable.value.valueunion.val_int_vec.value[i] = (OV_INT) strtol(pArgumentList[i]+1,NULL,10);
			}
			ov_string_freelist(pArgumentList);
			break;

		case OV_VT_UINT_VEC:
		case OV_VT_UINT_PV_VEC:
			pArgumentList = ov_string_split(*newcontent, "%20", &len);
			Ov_SetDynamicVectorLength(&Variable.value.valueunion.val_uint_vec, len, UINT);
			for(i = 0; i < len; i++){
				Variable.value.valueunion.val_uint_vec.value[i] = (OV_UINT) strtoul(pArgumentList[i]+1,NULL,10);
			}
			ov_string_freelist(pArgumentList);
			break;

		case OV_VT_SINGLE_VEC:
		case OV_VT_SINGLE_PV_VEC:
			pArgumentList = ov_string_split(*newcontent, "%20", &len);
			Ov_SetDynamicVectorLength(&Variable.value.valueunion.val_single_vec, len, SINGLE);
			for(i = 0; i < len; i++){
				Variable.value.valueunion.val_single_vec.value[i] = (OV_SINGLE) atof(pArgumentList[i]+1);
			}
			ov_string_freelist(pArgumentList);
			break;

		case OV_VT_DOUBLE_VEC:
		case OV_VT_DOUBLE_PV_VEC:
			pArgumentList = ov_string_split(*newcontent, "%20", &len);
			Ov_SetDynamicVectorLength(&Variable.value.valueunion.val_double_vec, len, DOUBLE);
			for(i = 0; i < len; i++){
				Variable.value.valueunion.val_double_vec.value[i] = (OV_SINGLE) atof(pArgumentList[i]+1);
			}
			ov_string_freelist(pArgumentList);
			break;

		case OV_VT_STRING_VEC:
		case OV_VT_STRING_PV_VEC:
			pArgumentList = ov_string_split(*newcontent, "%20", &len);
			Ov_SetDynamicVectorLength(&Variable.value.valueunion.val_string_vec, len, STRING);

			for(i = 0; i < len; i++){
				//killing the first character
				ov_string_setvalue(&Temp, pArgumentList[i]+1);
				//kill the last character, now we have two null bytes at the end
				Temp[ov_string_getlength(Temp)-1] = '\0';

				//setting the content of the pointers to null
				//otherwise setvalue() crashes as it wants to free memory from a garbage pointer
				Variable.value.valueunion.val_string_vec.value[i] = NULL;
				ov_string_setvalue(&Variable.value.valueunion.val_string_vec.value[i], Temp);

			}

			ov_string_freelist(pArgumentList);
			break;

/*
		case OV_VT_TIME_VEC:
		case OV_VT_TIME_PV_VEC:

		case OV_VT_TIME_SPAN_VEC:
		case OV_VT_TIME_SPAN_PV_VEC:

		case OV_VT_STATE_VEC:
		case (OV_VT_STATE_VEC | OV_VT_HAS_STATE | OV_VT_HAS_TIMESTAMP):

		case OV_VT_STRUCT_VEC:
		case (OV_VT_STRUCT_VEC | OV_VT_HAS_STATE | OV_VT_HAS_TIMESTAMP):
*/
		default:
/*				ov_logfile_error("%s:%d - GestureReaction - target: %s, Userinput (%s), DataType %u not implemented.", __FILE__, __LINE__,
				*varname,
				*newcontent,
				Variable.value.vartype);
*/
			SETVAR_AT_OBJECT_RETURN OV_ERR_NOTIMPLEMENTED;
	}

	//	set Variable Value
	//
	fr = pOvVTable->m_setvar(pObj, &PartElement, &Variable);
	if (Ov_Fail(fr))
	{
		ov_string_append(message, "Error in writing new value\n");
		SETVAR_AT_OBJECT_RETURN fr;
	}else{
		ov_string_append(message, "Success");
	};

	SETVAR_AT_OBJECT_RETURN OV_ERR_OK;
}
