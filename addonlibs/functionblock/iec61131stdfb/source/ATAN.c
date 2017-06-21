/*
*   Copyright (C) 2010
*   Chair of Process Control Engineering,
*   D-52056 Aachen, Germany.
*   All rights reserved.
*
*
*   This file is part of the ACPLT/OV Package 
*   Licensed under the Apache License, Version 2.0 (the "License");
*   you may not use this file except in compliance with the License.
*   You may obtain a copy of the License at
*
*       http://www.apache.org/licenses/LICENSE-2.0
*
*   Unless required by applicable law or agreed to in writing, software
*   distributed under the License is distributed on an "AS IS" BASIS,
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*   See the License for the specific language governing permissions and
*   limitations under the License.
*/
/******************************************************************************
*
*   FILE
*   ----
*   ATAN.c
*
*   History
*   -------
*   2010-11-03   File created
*
*******************************************************************************
*
*   This file is generated by the 'fb_builder' command
*
******************************************************************************/


#ifndef OV_COMPILE_LIBRARY_iec61131stdfb
#define OV_COMPILE_LIBRARY_iec61131stdfb
#endif

#define OV_DEBUG

#include "iec61131stdfb.h"
#include "stdfb_macros.h"
#include "libov/ov_macros.h"
#include "libov/ov_logfile.h"
#include "helper.h"

#include <math.h>


OV_DLLFNCEXPORT OV_RESULT iec61131stdfb_ATAN_IN_set(
    OV_INSTPTR_iec61131stdfb_ATAN          pobj,
    const OV_ANY*  value
) {
    if((pobj->v_IN.value.vartype & OV_VT_KSMASK) == (value->value.vartype & OV_VT_KSMASK))	
		return ov_variable_setanyvalue(&pobj->v_IN, value);
	else
	{
		if (iec61131stdfb_isConnected (Ov_PtrUpCast (fb_functionblock, pobj)))
			return OV_ERR_NOACCESS;
		else
		{
			iec61131stdfb_freeVec(&pobj->v_IN);
			iec61131stdfb_freeVec(&pobj->v_OUT);
			switch(value->value.vartype & OV_VT_KSMASK)
			{
				case OV_VT_INT:
				case OV_VT_UINT:
				case OV_VT_BYTE:
				case OV_VT_SINGLE:
					pobj->v_OUT.value.vartype = OV_VT_SINGLE;
				return ov_variable_setanyvalue(&pobj->v_IN, value);
				
				case OV_VT_DOUBLE:
					pobj->v_OUT.value.vartype = OV_VT_DOUBLE;
				return ov_variable_setanyvalue(&pobj->v_IN, value);
				
				case OV_VT_INT_VEC:
				case OV_VT_UINT_VEC:
				case OV_VT_BYTE_VEC:
				case OV_VT_SINGLE_VEC:
					pobj->v_OUT.value.vartype = OV_VT_SINGLE_VEC;
				return ov_variable_setanyvalue(&pobj->v_IN, value);
				
				case OV_VT_DOUBLE_VEC:
					pobj->v_OUT.value.vartype = OV_VT_DOUBLE_VEC;
				return ov_variable_setanyvalue(&pobj->v_IN, value);
				
				default:
					return OV_ERR_BADPARAM;
			}
			return OV_ERR_GENERIC;
		}
	}
}

OV_DLLFNCEXPORT OV_ANY* iec61131stdfb_ATAN_OUT_get(
    OV_INSTPTR_iec61131stdfb_ATAN          pobj
) {
    return &pobj->v_OUT;
}


OV_DLLFNCEXPORT void iec61131stdfb_ATAN_destructor(OV_INSTPTR_ov_object pobj) {

	OV_INSTPTR_iec61131stdfb_ATAN pinst = Ov_StaticPtrCast(iec61131stdfb_ATAN, pobj);
	
	iec61131stdfb_freeVec(&pinst->v_IN);
	iec61131stdfb_freeVec(&pinst->v_OUT);
	fb_functionblock_destructor(pobj);
}


OV_DLLFNCEXPORT OV_RESULT iec61131stdfb_ATAN_constructor(OV_INSTPTR_ov_object pobj) {

	OV_RESULT res;
	OV_INSTPTR_iec61131stdfb_ATAN pinst = Ov_StaticPtrCast(iec61131stdfb_ATAN, pobj);
	
	res = fb_functionblock_constructor(pobj);
	
	if(Ov_OK(res))
	{
		pinst->v_IN.value.valueunion.val_double = 0;
		pinst->v_OUT.value.valueunion.val_double = 0;
		pinst->v_IN.value.vartype = OV_VT_SINGLE;
		pinst->v_OUT.value.vartype = OV_VT_SINGLE;
		return OV_ERR_OK;
	}
	else
		return res;
}

OV_DLLFNCEXPORT void iec61131stdfb_ATAN_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
    /*    
    *   local variables
    */
	
	unsigned int i;
	
    OV_INSTPTR_iec61131stdfb_ATAN pinst = Ov_StaticPtrCast(iec61131stdfb_ATAN, pfb);
	
	iec61131stdfb_freeVec(&pinst->v_OUT);
	if(!(pinst->v_IN.value.vartype & OV_VT_ISVECTOR))
		{
			switch(pinst->v_IN.value.vartype & OV_VT_KSMASK)
			{
				
				case OV_VT_INT:
					pinst->v_OUT.value.vartype = OV_VT_SINGLE;
					ov_logfile_warning("%s: tan of integer value, setting out to single to prevent data loss", pinst->v_identifier);
					pinst->v_OUT.value.valueunion.val_single = (OV_SINGLE) atan(pinst->v_IN.value.valueunion.val_int);
				break;
				
				case OV_VT_UINT:
					pinst->v_OUT.value.vartype = OV_VT_SINGLE;
					ov_logfile_warning("%s: tan of integer value, setting out to single to prevent data loss", pinst->v_identifier);
					pinst->v_OUT.value.valueunion.val_single = (OV_SINGLE) atan(pinst->v_IN.value.valueunion.val_uint); 
				break;
				
				case OV_VT_SINGLE:
					pinst->v_OUT.value.vartype = OV_VT_SINGLE;
					pinst->v_OUT.value.valueunion.val_single = (OV_SINGLE) atan(pinst->v_IN.value.valueunion.val_single);
				break;
				
				case OV_VT_DOUBLE:
					pinst->v_OUT.value.vartype = OV_VT_DOUBLE;
					pinst->v_OUT.value.valueunion.val_double = atan(pinst->v_IN.value.valueunion.val_double);
				
				break;

				case OV_VT_BYTE:
					pinst->v_OUT.value.vartype = OV_VT_SINGLE;
					pinst->v_OUT.value.valueunion.val_single = (OV_SINGLE) atan(pinst->v_IN.value.valueunion.val_byte);
					ov_logfile_warning("%s: bitstring given, treating as unsigned integer, setting out to single to prevent data loss", pinst->v_identifier);
				break;
				
				
					
				default:
					pinst->v_OUT.value.vartype = OV_VT_BOOL;
					pinst->v_OUT.value.valueunion.val_bool = FALSE;
					ov_logfile_alert("%s: operation cannot be done on given datatype", pinst->v_identifier);
				break;
			}
		}
		else
		{
				switch(pinst->v_IN.value.vartype & OV_VT_KSMASK)
			{
			
				case OV_VT_INT_VEC:
					STDFB_VEC_ATAN_I(INT, int);
				break;
		
				case OV_VT_UINT_VEC:
					STDFB_VEC_ATAN_I(UINT, uint);
				break;
			
				case OV_VT_SINGLE_VEC:
					STDFB_VEC_ATAN_R(SINGLE, single);
				break;
		
				case OV_VT_DOUBLE_VEC:
					STDFB_VEC_ATAN_R(DOUBLE, double);
				break;

				case OV_VT_BYTE_VEC:
					ov_logfile_warning("%s: bitstring given, treating as unsigned integer", pinst->v_identifier);
					STDFB_VEC_ATAN_I(BYTE, byte);
				break;
			
				default:
					pinst->v_OUT.value.vartype = OV_VT_BOOL;
					pinst->v_OUT.value.valueunion.val_bool = FALSE;
					ov_logfile_alert("%s: arctan of given datatypes senseless", pinst->v_identifier);
				break;
			}
		}
		
		/************** handling states ********************************/
		
	STDFB_STATE(IN);
	
	/***************** handling timestamps ************************/
	STDFB_TIMESTAMP(IN);
		
    return;
}

