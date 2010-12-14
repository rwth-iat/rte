/*
*	Copyright (C) 2010
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
*/
/******************************************************************************
*
*   FILE
*   ----
*   COS.c
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

#include <math.h>


OV_DLLFNCEXPORT OV_RESULT iec61131stdfb_COS_IN_set(
    OV_INSTPTR_iec61131stdfb_COS          pobj,
    const OV_ANY*  value
) {
    return ov_variable_setanyvalue(&pobj->v_IN, value);
}

OV_DLLFNCEXPORT OV_ANY* iec61131stdfb_COS_OUT_get(
    OV_INSTPTR_iec61131stdfb_COS          pobj
) {
    return &pobj->v_OUT;
}


OV_DLLFNCEXPORT void iec61131stdfb_COS_shutdown(OV_INSTPTR_ov_object pobj) {

	unsigned int i;
	
	OV_INSTPTR_iec61131stdfb_COS pinst = Ov_StaticPtrCast(iec61131stdfb_COS, pobj);
	
	STDFB_FREE_VEC(pinst->v_IN);
	STDFB_FREE_VEC(pinst->v_OUT);
	ov_object_shutdown(pobj);
}


OV_DLLFNCEXPORT void iec61131stdfb_COS_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
    /*    
    *   local variables
    */
	
	unsigned int i;
	
    OV_INSTPTR_iec61131stdfb_COS pinst = Ov_StaticPtrCast(iec61131stdfb_COS, pfb);
	
	STDFB_FREE_VEC(pinst->v_OUT);
	if(!(pinst->v_IN.value.vartype & OV_VT_ISVECTOR))
		{
			switch(pinst->v_IN.value.vartype & OV_VT_KSMASK)
			{
				
				case OV_VT_INT:
					pinst->v_OUT.value.vartype = OV_VT_SINGLE;
					ov_logfile_warning("%s: sine of integer value, setting out to single to prevent data loss", pinst->v_identifier);
					pinst->v_OUT.value.valueunion.val_single = cos(pinst->v_IN.value.valueunion.val_int);
				break;
				
				case OV_VT_UINT:
					pinst->v_OUT.value.vartype = OV_VT_SINGLE;
					ov_logfile_warning("%s: sine of integer value, setting out to single to prevent data loss", pinst->v_identifier);
					pinst->v_OUT.value.valueunion.val_single = cos(pinst->v_IN.value.valueunion.val_uint); 
				break;
				
				case OV_VT_SINGLE:
					pinst->v_OUT.value.vartype = OV_VT_SINGLE;
					pinst->v_OUT.value.valueunion.val_single = cos(pinst->v_IN.value.valueunion.val_single);
				break;
				
				case OV_VT_DOUBLE:
					pinst->v_OUT.value.vartype = OV_VT_DOUBLE;
					pinst->v_OUT.value.valueunion.val_double = cos(pinst->v_IN.value.valueunion.val_double);
				
				break;

				case OV_VT_BYTE:
					pinst->v_OUT.value.vartype = OV_VT_SINGLE;
					pinst->v_OUT.value.valueunion.val_byte = cos(pinst->v_IN.value.valueunion.val_byte);
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
					STDFB_VEC_COS_I(INT, int);
				break;
		
				case OV_VT_UINT_VEC:
					STDFB_VEC_COS_I(UINT, uint);
				break;
			
				case OV_VT_SINGLE_VEC:
					STDFB_VEC_COS_R(SINGLE, single);
				break;
		
				case OV_VT_DOUBLE_VEC:
					STDFB_VEC_COS_R(DOUBLE, double);
				break;

				case OV_VT_BYTE_VEC:
					ov_logfile_warning("%s: bitstring given, treating as unsigned integer", pinst->v_identifier);
					STDFB_VEC_COS_I(BYTE, byte);
				break;
			
				default:
					pinst->v_OUT.value.vartype = OV_VT_BOOL;
					pinst->v_OUT.value.valueunion.val_bool = FALSE;
					ov_logfile_alert("%s: operation cannot be done on given datatype", pinst->v_identifier);
				break;
			}
		}
		
		/************** handling states ********************************/
		
	STDFB_STATE(IN);
	
	/***************** handling timestamps ************************/
	STDFB_TIMESTAMP(IN);	
	
		
    return;
}

