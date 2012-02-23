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


	OV_ELEMENT	Element;
	OV_ANY		Variable;

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

	Element.elemtype	= OV_ET_OBJECT;
	Element.pobj		= pObj;

	//	search Variable-Element with corresponding name in OV-Object
	//
	fr = ov_element_searchpart(&Element, &Element, OV_ET_VARIABLE, *varname);
	if (Ov_Fail(fr))
	{
		ov_string_append(message, "Object does not exist\n");
		SETVAR_AT_OBJECT_RETURN OV_ERR_BADPATH;
	};

	//	fill Variable with active Content of OV-Object
	//
	ov_memstack_lock();
		fr = pOvVTable->m_getvar(pObj, &Element, &Variable);
	ov_memstack_unlock();
	if (Ov_Fail(fr))
	{
		ov_string_append(message, "Object exists, but variable does not\n");
		SETVAR_AT_OBJECT_RETURN OV_ERR_BADPATH;
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
	switch (Variable.value.vartype)
	{
		case OV_VT_STRING:
		case OV_VT_STRING_PV:
			//setting the content of the whole union to zero (double is the entry which fills the valueunion complete)
			//otherwise setvalue() crashes as it wants to free memory from a garbage pointer
			Variable.value.valueunion.val_double = 0;
			fr = ov_string_setvalue(&Variable.value.valueunion.val_string, *newcontent);
			if (Ov_Fail(fr))
			{
				ov_string_append(message, "Setting value failed\n");
				SETVAR_AT_OBJECT_RETURN OV_ERR_GENERIC;
			};
			break;

		case OV_VT_BOOL:
		case OV_VT_BOOL_PV:
			if (CHECK_BOOLTRUE(*newcontent)){
				Variable.value.valueunion.val_bool = TRUE;
			}else if (CHECK_BOOLFALSE(*newcontent)){
				Variable.value.valueunion.val_bool = FALSE;
			}else{
				ov_string_append(message, "Userinput not detected as bool\n");
				SETVAR_AT_OBJECT_RETURN OV_ERR_BADPARAM;
			}
			break;

		case OV_VT_BYTE:
		case (OV_VT_BYTE | OV_VT_HAS_STATE | OV_VT_HAS_TIMESTAMP):
			ov_string_setvalue(&Temp, *newcontent);
			Variable.value.valueunion.val_byte = Temp[ov_string_getlength(Temp)-1];
		break;

		case OV_VT_INT:
		case OV_VT_INT_PV:
			Variable.value.valueunion.val_int = (OV_INT) strtol(*newcontent,NULL,0);
		break;

		case OV_VT_UINT:
		case OV_VT_UINT_PV:
			Variable.value.valueunion.val_uint = (OV_UINT) strtol(*newcontent,NULL,0);
		break;

		case OV_VT_SINGLE:
		case OV_VT_SINGLE_PV:
			Variable.value.valueunion.val_single = (OV_SINGLE) atof(*newcontent);
		break;

		case OV_VT_DOUBLE:
		case OV_VT_DOUBLE_PV:
			Variable.value.valueunion.val_double = (OV_DOUBLE) atof(*newcontent);
		break;

		case OV_VT_VOID:
		case OV_VT_VOID | OV_VT_HAS_STATE | OV_VT_HAS_TIMESTAMP:
			ov_string_append(message, "OV_VOID has no variable content to change.\n");
			SETVAR_AT_OBJECT_RETURN OV_ERR_GENERIC;
			break;
/*	TODO	implement this
		case OV_VT_TIME:
			ov_logfile_debug("%s:%d OV_VT_TIME", __FILE__, __LINE__);
		break;

		case OV_VT_TIME_SPAN:
			ov_logfile_debug("%s:%d OV_VT_TIME_SPAN", __FILE__, __LINE__);
		break;

		case OV_VT_STATE:
			ov_logfile_debug("%s:%d OV_VT_STATE", __FILE__, __LINE__);
		break;

		case OV_VT_STRUCT:
			ov_logfile_debug("%s:%d OV_VT_STRUCT", __FILE__, __LINE__);
		break;

*/
		default:
/*				ov_logfile_error("%s:%d - GestureReaction - target: %s, Userinput (%s), DataType %u not implemented.", __FILE__, __LINE__,
				*varname,
				*newcontent,
				Variable.value.vartype);
*/
			SETVAR_AT_OBJECT_RETURN OV_ERR_GENERIC;
	}

	//	set Variable Value
	//
	fr = pOvVTable->m_setvar(pObj, &Element, &Variable);
	if (Ov_Fail(fr))
	{
	    ov_string_append(message, "Error in writing new value\n");
		SETVAR_AT_OBJECT_RETURN OV_ERR_GENERIC;
	}else{
	    ov_string_append(message, "Success");
	};

    SETVAR_AT_OBJECT_RETURN OV_ERR_OK;
}
