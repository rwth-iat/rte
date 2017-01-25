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
/***********************************************************************
*
*	File
*	-----
*	stdfb_convert_macros.h
*
*	Autoren:
*	--------
*	LE						Lars Evertz <l.evertz@plt.rwth-aachen.de>
*
*	
*	Historie:
*	--------
*	14-December-2010		LE
*		-	file created
*
***********************************************************************/

/***************BYTE to **************************************/

#define BYTE_TO(anycap, anysmall)	\
	pinst->v_OUT.value.vartype = OV_VT_##anycap;	\
	pinst->v_OUT.value.valueunion.val_##anysmall = pinst->v_IN.value.valueunion.val_byte;
	
/*******************BYTE to Vector *****************************/
	
#define BYTE_TO_VEC(anycap, anysmall)	\
	pinst->v_OUT.value.vartype = OV_VT_##anycap##_VEC;	\
	if(Ov_Fail(Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_##anysmall##_vec, 1, anycap)))	\
	{	\
		ov_logfile_error("%s: allocation of memory failen, no operation performed", pinst->v_identifier);	\
		return;	\
	}	\
	pinst->v_OUT.value.valueunion.val_##anysmall##_vec.value[0] = pinst->v_IN.value.valueunion.val_byte;

/***************BOOL to **************************************/

	
#define BOOL_TO(anycap, anysmall)	\
	pinst->v_OUT.value.vartype = OV_VT_##anycap;	\
	if(pinst->v_IN.value.valueunion.val_bool)	\
		pinst->v_OUT.value.valueunion.val_##anysmall = 1;	\
	else	\
		pinst->v_OUT.value.valueunion.val_##anysmall = 0;

/***************BOOL to Vector **************************************/
		
#define BOOL_TO_VEC(anycap, anysmall)	\
	pinst->v_OUT.value.vartype = OV_VT_##anycap##_VEC;	\
	if(Ov_Fail(Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_##anysmall##_vec, 1, anycap)))	\
	{	\
		ov_logfile_error("%s: allocation of memory failen, no operation performed", pinst->v_identifier);	\
		return;	\
	}	\
	if(pinst->v_IN.value.valueunion.val_bool)	\
		pinst->v_OUT.value.valueunion.val_##anysmall##_vec.value[0] = 1;	\
	else	\
		pinst->v_OUT.value.valueunion.val_##anysmall##_vec.value[0] = 0;	
		
/***************Vector to BOOL (length != 0)**************************************/
		
		
#define STDFB_VEC_TO_BOOL(anysmall)	\
	pinst->v_OUT.value.vartype = OV_VT_BOOL;	\
	if(pinst->v_IN.value.valueunion.val_##anysmall##_vec.veclen)	\
		pinst->v_OUT.value.valueunion.val_bool = 1;	\
	else	\
		pinst->v_OUT.value.valueunion.val_bool = 0;
		
/***************Vector to UINT (length) **************************************/	
	
#define STDFB_VEC_TO_UINT(anysmall)	\
	pinst->v_OUT.value.vartype = OV_VT_UINT;	\
	pinst->v_OUT.value.valueunion.val_uint = pinst->v_IN.value.valueunion.val_##anysmall##_vec.veclen;
	
/***************BYTE Vector to (bitstring operation) **************************************/
	
#define STDFB_BYTE_VEC_TO(anycap, anysmall)	\
	pinst->v_OUT.value.vartype = OV_VT_##anycap;	\
	if(pinst->v_IN.value.valueunion.val_byte_vec.veclen >= sizeof(OV_##anycap) / sizeof(OV_BYTE))	\
	{	\
		pinst->v_OUT.value.valueunion.val_##anysmall = *(OV_##anycap*) pinst->v_IN.value.valueunion.val_byte_vec.value;	\
	}	\
	else	\
	{	\
		ov_logfile_error("%s: vector shorter than result datatype", pinst->v_identifier);	\
	}
	

/***************Vector to BYTE Vector (bitstring operation) **************************************/
	
	
#define STDFB_VEC_TO_BYTE_VEC(anycap, anysmall)	\
	pinst->v_OUT.value.vartype = OV_VT_BYTE_VEC;	\
	length = sizeof(OV_##anycap) / sizeof(OV_BYTE);	\
	if(Ov_OK(Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_byte_vec, pinst->v_IN.value.valueunion.val_##anysmall##_vec.veclen * length, BYTE)))	\
	{	\
		for(i=0; i < pinst->v_IN.value.valueunion.val_##anysmall##_vec.veclen; i++)	\
			((OV_##anycap*)pinst->v_OUT.value.valueunion.val_byte_vec.value)[i] = pinst->v_IN.value.valueunion.val_##anysmall##_vec.value[i];	\
	}	\
	else	\
	{	\
		ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);	\
		return;	\
	}
	

/***************BYTE Vector to Vector (bitstring operation) **************************************/
	
#define STDFB_BYTE_VEC_TO_VEC(anycap, anysmall)	\
	pinst->v_OUT.value.vartype = OV_VT_##anycap##_VEC;	\
	length = sizeof(OV_##anycap) / sizeof(OV_BYTE);	\
	if(length)	\
	{	\
		if(Ov_OK(Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_##anysmall##_vec, pinst->v_IN.value.valueunion.val_byte_vec.veclen / length, anycap)))	\
		{	\
			for(i=0; i < pinst->v_OUT.value.valueunion.val_##anysmall##_vec.veclen; i++)	\
				pinst->v_OUT.value.valueunion.val_##anysmall##_vec.value[i] = ((OV_##anycap*)pinst->v_IN.value.valueunion.val_byte_vec.value)[i];	\
		}	\
		else	\
		{	\
			ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);	\
			return;	\
		}	\
	}	\
	else	\
	{	\
		ov_logfile_error("%s: vector shorter than result datatype", pinst->v_identifier);	\
	}
	
/***************Vector to BOOL Vector (Element != 0) **************************************/
	
#define STDFB_VEC_TO_BOOL_VEC(anycap, anysmall)	\
	pinst->v_OUT.value.vartype= OV_VT_BOOL_VEC;	\
	if(Ov_OK(Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_bool_vec, pinst->v_IN.value.valueunion.val_##anysmall##_vec.veclen, BOOL)))	\
	{	\
		for(i=0; i < pinst->v_IN.value.valueunion.val_##anysmall##_vec.veclen; i++)	\
			if(pinst->v_IN.value.valueunion.val_##anysmall##_vec.value[i])	\
				pinst->v_OUT.value.valueunion.val_bool_vec.value[i] = TRUE;	\
			else	\
				pinst->v_OUT.value.valueunion.val_bool_vec.value[i] = FALSE;	\
	}	\
	else	\
	{	\
		ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);	\
		return;	\
	}


	/***************interger Vector to UINT Vector**************************************/
	
#define STDFB_AINT_VEC_TO_UINT_VEC(anycap, anysmall)	\
	pinst->v_OUT.value.vartype = OV_VT_UINT_VEC;	\
	if(Ov_OK(Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_uint_vec, pinst->v_IN.value.valueunion.val_##anysmall##_vec.veclen, UINT)))	\
	{	\
		for(i = 0; i < pinst->v_IN.value.valueunion.val_##anysmall##_vec.veclen; i++)	\
		{	\
			if(pinst->v_IN.value.valueunion.val_##anysmall##_vec.value[i] >= 0)	\
				pinst->v_OUT.value.valueunion.val_uint_vec.value[i] = pinst->v_IN.value.valueunion.val_##anysmall##_vec.value[i];	\
			else	\
				ov_logfile_error("%s: value in element %d exceeds range of uint", pinst->v_identifier, i);	\
		}	\
	}	\
	else	\
	{	\
		ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);	\
		return;	\
	}
	
/***************integer Vector to INT Vector**************************************/
	
#define STDFB_AINT_VEC_TO_INT_VEC(anycap, anysmall)	\
	pinst->v_OUT.value.vartype = OV_VT_INT_VEC;	\
	if(Ov_OK(Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_int_vec, pinst->v_IN.value.valueunion.val_##anysmall##_vec.veclen, INT)))	\
	{	\
		for(i = 0; i < pinst->v_IN.value.valueunion.val_##anysmall##_vec.veclen; i++)	\
		{	\
			if(pinst->v_IN.value.valueunion.val_##anysmall##_vec.value[i] <= OV_VL_MAXINT)	\
				pinst->v_OUT.value.valueunion.val_int_vec.value[i] = pinst->v_IN.value.valueunion.val_##anysmall##_vec.value[i];	\
			else	\
				ov_logfile_error("%s: value in element %d exceeds range of int", pinst->v_identifier, i);	\
		}	\
	}	\
	else	\
	{	\
		ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);	\
		return;	\
	}
		
		
/*************** integer Vector to SINGLE Vector **************************************/
		
		
#define STDFB_AINT_VEC_TO_SINGLE_VEC(anycap, anysmall)	\
	pinst->v_OUT.value.vartype = OV_VT_SINGLE_VEC;	\
	if(Ov_OK(Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_single_vec, pinst->v_IN.value.valueunion.val_##anysmall##_vec.veclen, SINGLE)))	\
	{	\
		for(i = 0; i < pinst->v_IN.value.valueunion.val_##anysmall##_vec.veclen; i++)	\
		{	\
			pinst->v_OUT.value.valueunion.val_single_vec.value[i] = (OV_SINGLE) pinst->v_IN.value.valueunion.val_##anysmall##_vec.value[i];	\
			if(pinst->v_IN.value.valueunion.val_##anysmall##_vec.value[i] > 10000000)	\
				ov_logfile_warning("%s: converting large integer value in element %d to single, possible loss of precision", pinst->v_identifier, i);	\
		}	\
	}	\
	else	\
	{	\
		ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);	\
		return;	\
	}		


/***************numeric Vector to DOUBLE Vector**************************************/
	
#define STDFB_VEC_TO_DOUBLE_VEC(anycap, anysmall)	\
	pinst->v_OUT.value.vartype = OV_VT_DOUBLE_VEC;	\
	if(Ov_OK(Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_double_vec, pinst->v_IN.value.valueunion.val_##anysmall##_vec.veclen, DOUBLE)))	\
	{	\
		for(i = 0; i < pinst->v_IN.value.valueunion.val_##anysmall##_vec.veclen; i++)	\
		{	\
			pinst->v_OUT.value.valueunion.val_double_vec.value[i] = pinst->v_IN.value.valueunion.val_##anysmall##_vec.value[i];	\
		}	\
	}	\
	else	\
	{	\
		ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);	\
		return;	\
	}		
	
