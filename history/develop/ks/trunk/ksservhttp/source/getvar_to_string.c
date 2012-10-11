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


#define GETVAR_TO_STRING_RETURN \
		ov_string_setvalue(&temp, NULL);\
		if(format==GETVAR_FORMAT_TCL)ov_string_append(message, "}");\
		return

/**
 * getvar_to_string returns the string result of the getvar call
 *
 * pObj is the pointer to the object
 * varname is the name of thq ueried variable
 * format is an it specifying the format: check constants GETVAR_FORMAT_TCL, GETVAR_FORMAT_PLAIN etc
 * message is the output string
 */
OV_RESULT getvar_to_string(OV_INSTPTR_ov_object pObj, OV_STRING* varname, OV_UINT format, OV_STRING* message){
	//	Local Variables
	//
	OV_VTBLPTR_ov_object pOvVTable = NULL;

	//	Local Variables
	//
	OV_ELEMENT	ParentElement;
	OV_ELEMENT	PartElement;
	OV_ANY		Variable;
	OV_RESULT	fr = OV_ERR_OK;
	OV_UINT i = 0;
	OV_STRING temp = NULL;
	OV_STRING temp2 = NULL;

	Ov_GetVTablePtr(ov_object, pOvVTable, pObj);
	ParentElement.elemtype	= OV_ET_OBJECT;
	ParentElement.pobj		= Ov_PtrUpCast(ov_object, pObj);

	//first separator
	if(format==GETVAR_FORMAT_TCL)ov_string_append(message, "{");

	if(pObj == NULL){
		ov_string_append(message, "Object does not exist");
		GETVAR_TO_STRING_RETURN OV_ERR_BADPATH; //404
	}

	if(ov_string_compare(pObj->v_identifier, "vendor") == OV_STRCMP_EQUAL && Ov_GetParent(ov_containment, pObj)==&pdb->root){
		//vendor is very special, so normal ov functions do not work here
		if(ov_string_compare(*varname, "classes") == OV_STRCMP_EQUAL){
			//TODO: tut nicht bei classes und libraries
			fr = ov_vendortree_getclasses(&Variable, NULL);
		}else{
			pObj = Ov_SearchChild(ov_containment, Ov_StaticPtrCast(ov_domain, pObj), *varname);
			fr = ov_vendortree_getvar(pObj, &Variable, NULL);
		}
		if (Ov_Fail(fr))
		{
			ov_string_append(message, "Vendor object exists, but variable does not (however vendor-elements that are vectors are not supported yet, so that it must not be your fault)");
			GETVAR_TO_STRING_RETURN OV_ERR_BADPATH; //404
		}
	}else{
		//	search Variable-Element with corresponding name in OV-Object
		//
		fr = ov_element_searchpart(&ParentElement, &PartElement, OV_ET_VARIABLE, *varname);
		if (Ov_Fail(fr))
		{
			GETVAR_TO_STRING_RETURN OV_ERR_GENERIC; //503
		};
		if(!(pOvVTable->m_getaccess(pObj, &PartElement, NULL) & OV_AC_READ)) {
			return OV_ERR_NOACCESS;
		}
		//	fill Variable with active Content of OV-Object
		//
		ov_memstack_lock();
		fr = pOvVTable->m_getvar(pObj, &PartElement, &Variable);
		ov_memstack_unlock();
		if (Ov_Fail(fr))
		{
			ov_string_append(message, "Object exists, but variable does not");
			GETVAR_TO_STRING_RETURN OV_ERR_BADPATH; //404
		};
	}

	//	We are checking actual content and not the definition of Variable, so we have no ANY, but VOID
	//	OV_VT_BOOL_PV is an ANY which is set to BOOL
	//
	switch (Variable.value.vartype)
	{
	case OV_VT_BOOL:
	case OV_VT_BOOL_PV:
		if (Variable.value.valueunion.val_bool == TRUE){
			ov_string_append(&temp, "TRUE");
		}else{
			ov_string_append(&temp, "FALSE");
		}
		break;

	case OV_VT_BYTE:
	case (OV_VT_BYTE | OV_VT_HAS_STATE | OV_VT_HAS_TIMESTAMP):
	ov_string_print(&temp, "%d", Variable.value.valueunion.val_byte);
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

	case OV_VT_VOID:															//unused ANY with explicit no content
	case (OV_VT_VOID | OV_VT_HAS_STATE | OV_VT_HAS_TIMESTAMP):	//  used ANY with explicit no content
	ov_string_print(&temp, "%s", "VOID");
	break;

	//****************** VEC: *******************
	case OV_VT_BOOL_VEC:
	case OV_VT_BOOL_PV_VEC:
		init_vector_output(&temp, format);
		for ( i = 0; i < Variable.value.valueunion.val_bool_vec.veclen;i++){
			if (i != 0){
				split_vector_output(&temp, format);
			}
			if (Variable.value.valueunion.val_bool_vec.value[i] == TRUE){
				ov_string_setvalue(&temp, "TRUE");
			}else{
				ov_string_setvalue(&temp, "FALSE");
			}
		}
		finalize_vector_output(&temp, format);
		break;

	case OV_VT_BYTE_VEC:
	case (OV_VT_BYTE_VEC | OV_VT_HAS_STATE | OV_VT_HAS_TIMESTAMP):
	init_vector_output(&temp, format);
	for ( i = 0; i < Variable.value.valueunion.val_byte_vec.veclen;i++){
		if (i != 0){
			split_vector_output(&temp, format);
		}
		//TODO: I know copy-pasting all appends around is mad, however i went even more mad by passing
		//variable number of arguments around :(
		ov_string_print(&temp2, "%d", Variable.value.valueunion.val_byte_vec.value[i]);
		ov_string_append(&temp, temp2);
		ov_string_setvalue(&temp2, NULL);
	}
	finalize_vector_output(&temp, format);
	break;

	case OV_VT_INT_VEC:
	case OV_VT_INT_PV_VEC:
		init_vector_output(&temp, format);
		for ( i = 0; i < Variable.value.valueunion.val_int_vec.veclen;i++){
			if (i != 0){
				split_vector_output(&temp, format);
			}
			ov_string_print(&temp2, "%i", Variable.value.valueunion.val_int_vec.value[i]);
			ov_string_append(&temp, temp2);
			ov_string_setvalue(&temp2, NULL);
		}
		finalize_vector_output(&temp, format);
		break;

	case OV_VT_UINT_VEC:
	case OV_VT_UINT_PV_VEC:
		init_vector_output(&temp, format);
		for ( i = 0; i < Variable.value.valueunion.val_uint_vec.veclen;i++){
			if (i != 0){
				split_vector_output(&temp, format);
			}
			ov_string_print(&temp2, "%i", Variable.value.valueunion.val_uint_vec.value[i]);
			ov_string_append(&temp, temp2);
			ov_string_setvalue(&temp2, NULL);
		}
		finalize_vector_output(&temp, format);
		break;

	case OV_VT_SINGLE_VEC:
	case OV_VT_SINGLE_PV_VEC:
		init_vector_output(&temp, format);
		for ( i = 0; i < Variable.value.valueunion.val_single_vec.veclen;i++){
			if (i != 0){
				split_vector_output(&temp, format);
			}
			ov_string_print(&temp2, "%g", Variable.value.valueunion.val_single_vec.value[i]);
			ov_string_append(&temp, temp2);
			ov_string_setvalue(&temp2, NULL);
		}
		finalize_vector_output(&temp, format);
		break;

	case OV_VT_DOUBLE_VEC:
	case OV_VT_DOUBLE_PV_VEC:
		init_vector_output(&temp, format);
		for ( i = 0; i < Variable.value.valueunion.val_double_vec.veclen;i++){
			if (i != 0){
				split_vector_output(&temp, format);
			}
			ov_string_print(&temp2, "%g", Variable.value.valueunion.val_double_vec.value[i]);
			ov_string_append(&temp, temp2);
			ov_string_setvalue(&temp2, NULL);
		}
		finalize_vector_output(&temp, format);
		break;

	case OV_VT_STRING_VEC:
	case OV_VT_STRING_PV_VEC:
		init_vector_output(&temp, format);
		for ( i = 0; i < Variable.value.valueunion.val_string_vec.veclen;i++){
			if (i != 0){
				split_vector_output(&temp, format);
			}
			if (ov_string_compare(Variable.value.valueunion.val_string_vec.value[i], NULL) == OV_STRCMP_EQUAL){
				ov_string_append(&temp, "");
			}else{
				ov_string_print(&temp2, "%s", Variable.value.valueunion.val_string_vec.value[i]);
				ov_string_append(&temp, temp2);
				ov_string_setvalue(&temp2, NULL);
			}
		}
		finalize_vector_output(&temp, format);
		break;

	case OV_VT_TIME_VEC:
	case OV_VT_TIME_PV_VEC:
		init_vector_output(&temp, format);
		for ( i = 0; i < Variable.value.valueunion.val_time_vec.veclen;i++){
			if (i != 0){
				split_vector_output(&temp, format);
			}
			ov_string_print(&temp2, "%s", ov_time_timetoascii(&Variable.value.valueunion.val_time_vec.value[i]));
			ov_string_append(&temp, temp2);
			ov_string_setvalue(&temp2, NULL);
		}
		finalize_vector_output(&temp, format);
		break;

	case OV_VT_TIME_SPAN_VEC:
	case OV_VT_TIME_SPAN_PV_VEC:
		ov_string_setvalue(&temp, "Vector has no entries");
		for ( i = 0; i < Variable.value.valueunion.val_time_span_vec.veclen;i++){
			if (i != 0){
				split_vector_output(&temp, format);
			}
			ov_string_print(&temp2, "%s", ov_time_timespantoascii(&Variable.value.valueunion.val_time_span_vec.value[i]));
			ov_string_append(&temp, temp2);
			ov_string_setvalue(&temp2, NULL);
		}
		finalize_vector_output(&temp, format);
		break;

	case OV_VT_STATE_VEC:
	case (OV_VT_STATE_VEC | OV_VT_HAS_STATE | OV_VT_HAS_TIMESTAMP):
	ov_string_print(&temp, "%s", "unknown");
	break;

	case OV_VT_STRUCT_VEC:
	case (OV_VT_STRUCT_VEC | OV_VT_HAS_STATE | OV_VT_HAS_TIMESTAMP):
	ov_string_print(&temp, "%s", "unknown");
	break;

	default:
		ov_string_print(&temp, "unknown value, VarType: %#X", Variable.value.vartype);
		break;
	}

	ov_string_setvalue(message, "");
	if(format==GETVAR_FORMAT_TCL)ov_string_append(message, "{");
	ov_string_append(message, temp);
	GETVAR_TO_STRING_RETURN OV_ERR_OK;
}


