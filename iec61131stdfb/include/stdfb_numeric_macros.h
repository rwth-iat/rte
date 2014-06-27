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
*	stdfb_numeric_macros.h
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


/******arctan2 von vektorelementen******/
	

#define STDFB_VEC_ATAN2(idcap, idsmall) \
	pinst->v_OUT.value.vartype = OV_VT_SINGLE_VEC;	\
	if(pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen == pinst->v_IN2.value.valueunion.val_##idsmall##_vec.veclen)	\
	{	\
		if(Ov_OK(Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_single_vec, pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen, SINGLE)))	\
		{	\
			for(i=0; i<pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen; i++)	\
			{	\
				if(pinst->v_IN1.value.valueunion.val_##idsmall##_vec.value[i])	\
					pinst->v_OUT.value.valueunion.val_single_vec.value[i] = (OV_SINGLE) (atan(pinst->v_IN2.value.valueunion.val_##idsmall##_vec.value[i] / \
						pinst->v_IN1.value.valueunion.val_##idsmall##_vec.value[i]));	\
				else	\
					if(pinst->v_IN2.value.valueunion.val_##idsmall##_vec.value[i] < 0)	\
						pinst->v_OUT.value.valueunion.val_single_vec.value[i] = (OV_SINGLE) (-0.5 * Pi);	\
					else	\
						pinst->v_OUT.value.valueunion.val_single_vec.value[i] = (OV_SINGLE) (0.5 * Pi);	\
			}	\
		}	\
		else	\
		{	\
		ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);	\
		return;	\
		}	\
	}	\
	else	\
	{	\
		ov_logfile_error("%s: vectors have different lengths, operation not possible", pinst->v_identifier);	\
		Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_##idsmall##_vec, 0, idcap);	\
	}

	
	#define STDFB_VEC_ATAN2_D(idcap, idsmall) \
	pinst->v_OUT.value.vartype = OV_VT_DOUBLE_VEC;	\
	if(pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen == pinst->v_IN2.value.valueunion.val_##idsmall##_vec.veclen)	\
	{	\
		if(Ov_OK(Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_double_vec, pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen, DOUBLE)))	\
		{	\
			for(i=0; i<pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen; i++)	\
			{	\
				if(pinst->v_IN1.value.valueunion.val_##idsmall##_vec.value[i])	\
					pinst->v_OUT.value.valueunion.val_double_vec.value[i] = (atan(pinst->v_IN2.value.valueunion.val_##idsmall##_vec.value[i] / \
						pinst->v_IN1.value.valueunion.val_##idsmall##_vec.value[i]));	\
				else	\
					if(pinst->v_IN2.value.valueunion.val_##idsmall##_vec.value[i] < 0)	\
						pinst->v_OUT.value.valueunion.val_double_vec.value[i] = (-0.5 * Pi);	\
					else	\
						pinst->v_OUT.value.valueunion.val_double_vec.value[i] = (0.5 * Pi);	\
			}	\
		}	\
		else	\
		{	\
		ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);	\
		return;	\
		}	\
	}	\
	else	\
	{	\
		ov_logfile_error("%s: vectors have different lengths, operation not possible", pinst->v_identifier);	\
		Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_##idsmall##_vec, 0, idcap);	\
	}
	
/********Betrag von vektorlementen*********/
	
#define STDFB_VEC_ABS(idcap, idsmall) \
	pinst->v_OUT.value.vartype = OV_VT_##idcap##_VEC;	\
	if(Ov_OK(Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_##idsmall##_vec, pinst->v_IN.value.valueunion.val_##idsmall##_vec.veclen, idcap)))	\
	{	\
		for(i=0; i<pinst->v_IN.value.valueunion.val_##idsmall##_vec.veclen; i++)	\
		{	\
			if(pinst->v_IN.value.valueunion.val_##idsmall##_vec.value[i] < 0) \
				pinst->v_OUT.value.valueunion.val_##idsmall##_vec.value[i] = (OV_##idcap) (pinst->v_IN.value.valueunion.val_##idsmall##_vec.value[i] * - 1.0);	\
			else	\
				pinst->v_OUT.value.valueunion.val_##idsmall##_vec.value[i] = pinst->v_IN.value.valueunion.val_##idsmall##_vec.value[i];	\
		}	\
	}	\
	else	\
	{	\
		ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);	\
		return;	\
	}	
	
	
/********Wurzel von vektorlementen*********/
	
#define STDFB_VEC_SQRT_R(idcap, idsmall) \
	pinst->v_OUT.value.vartype = OV_VT_##idcap##_VEC;	\
	if(Ov_OK(Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_##idsmall##_vec, pinst->v_IN.value.valueunion.val_##idsmall##_vec.veclen, idcap)))	\
	{	\
		for(i=0; i<pinst->v_IN.value.valueunion.val_##idsmall##_vec.veclen; i++)	\
		{	\
			if(pinst->v_IN.value.valueunion.val_##idsmall##_vec.value[i] < 0) \
			{	\
				pinst->v_OUT.value.valueunion.val_##idsmall##_vec.value[i] = (OV_##idcap) sqrt(pinst->v_IN.value.valueunion.val_##idsmall##_vec.value[i]);	\
				ov_logfile_error("%s: trying to calculate the squareroot of a negative value", pinst->v_identifier);	\
				STDFB_bad_operation = TRUE;	\
			}	\
			else	\
				pinst->v_OUT.value.valueunion.val_##idsmall##_vec.value[i] = (OV_##idcap) sqrt(pinst->v_IN.value.valueunion.val_##idsmall##_vec.value[i]);	\
		}	\
	}	\
	else	\
	{	\
		ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);	\
		STDFB_bad_operation = TRUE;	\
		return;	\
	}

#define STDFB_VEC_SQRT_I(idcap, idsmall) \
	pinst->v_OUT.value.vartype = OV_VT_SINGLE_VEC;	\
	ov_logfile_warning("%s: input value is integer, output is set to single to prevent data loss", pinst->v_identifier);	\
	if(Ov_OK(Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_single_vec, pinst->v_IN.value.valueunion.val_##idsmall##_vec.veclen, SINGLE)))	\
	{	\
		for(i=0; i<pinst->v_IN.value.valueunion.val_##idsmall##_vec.veclen; i++)	\
		{	\
			if(pinst->v_IN.value.valueunion.val_##idsmall##_vec.value[i] < 0) \
			{	\
				pinst->v_OUT.value.valueunion.val_single_vec.value[i] = (OV_SINGLE) sqrt(pinst->v_IN.value.valueunion.val_##idsmall##_vec.value[i]);	\
				ov_logfile_error("%s: trying to calculate the squareroot of a negative value", pinst->v_identifier);	\
				STDFB_bad_operation = TRUE;	\
			}	\
			else	\
				pinst->v_OUT.value.valueunion.val_single_vec.value[i] = (OV_SINGLE) sqrt(pinst->v_IN.value.valueunion.val_##idsmall##_vec.value[i]);	\
		}	\
	}	\
	else	\
	{	\
		ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);	\
		STDFB_bad_operation = TRUE;	\
		return;	\
	}	

/********Natuerlicher Logarithmus von vektorlementen*********/
	
#define STDFB_VEC_LN_R(idcap, idsmall) \
	pinst->v_OUT.value.vartype = OV_VT_##idcap##_VEC;	\
	if(Ov_OK(Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_##idsmall##_vec, pinst->v_IN.value.valueunion.val_##idsmall##_vec.veclen, idcap)))	\
	{	\
		for(i=0; i<pinst->v_IN.value.valueunion.val_##idsmall##_vec.veclen; i++)	\
		{	\
			if(pinst->v_IN.value.valueunion.val_##idsmall##_vec.value[i] <= 0) \
			{	\
				pinst->v_OUT.value.valueunion.val_##idsmall##_vec.value[i] = (OV_##idcap) log(pinst->v_IN.value.valueunion.val_##idsmall##_vec.value[i]);	\
				STDFB_bad_operation = TRUE;	\
				ov_logfile_error("%s: trying to calculate the logarithm of 0 or a negative value", pinst->v_identifier);	\
			}	\
			else	\
				pinst->v_OUT.value.valueunion.val_##idsmall##_vec.value[i] = (OV_##idcap) log(pinst->v_IN.value.valueunion.val_##idsmall##_vec.value[i]);	\
		}	\
	}	\
	else	\
	{	\
		ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);	\
		STDFB_bad_operation = TRUE;	\
		return;	\
	}

#define STDFB_VEC_LN_I(idcap, idsmall) \
	pinst->v_OUT.value.vartype = OV_VT_SINGLE_VEC;	\
	ov_logfile_warning("%s: input value is integer, output is set to single to prevent data loss", pinst->v_identifier);	\
	if(Ov_OK(Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_single_vec, pinst->v_IN.value.valueunion.val_##idsmall##_vec.veclen, SINGLE)))	\
	{	\
		for(i=0; i<pinst->v_IN.value.valueunion.val_##idsmall##_vec.veclen; i++)	\
		{	\
			if(pinst->v_IN.value.valueunion.val_##idsmall##_vec.value[i] <= 0) \
			{	\
				pinst->v_OUT.value.valueunion.val_single_vec.value[i] = (OV_SINGLE) log(pinst->v_IN.value.valueunion.val_##idsmall##_vec.value[i]);	\
				STDFB_bad_operation = TRUE;	\
				ov_logfile_error("%s: trying to calculate the logarithm of 0 or a negative value", pinst->v_identifier);	\
			}	\
			else	\
				pinst->v_OUT.value.valueunion.val_single_vec.value[i] = (OV_SINGLE) log(pinst->v_IN.value.valueunion.val_##idsmall##_vec.value[i]);	\
		}	\
	}	\
	else	\
	{	\
		ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);	\
		STDFB_bad_operation = TRUE;	\
		return;	\
	}	
	
/********Logarithmus zur Basis 10 von vektorlementen*********/
	
#define STDFB_VEC_LOG_R(idcap, idsmall) \
	pinst->v_OUT.value.vartype = OV_VT_##idcap##_VEC;	\
	if(Ov_OK(Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_##idsmall##_vec, pinst->v_IN.value.valueunion.val_##idsmall##_vec.veclen, idcap)))	\
	{	\
		for(i=0; i<pinst->v_IN.value.valueunion.val_##idsmall##_vec.veclen; i++)	\
		{	\
			if(pinst->v_IN.value.valueunion.val_##idsmall##_vec.value[i] <= 0) \
			{	\
				pinst->v_OUT.value.valueunion.val_##idsmall##_vec.value[i] = (OV_##idcap) log10(pinst->v_IN.value.valueunion.val_##idsmall##_vec.value[i]);	\
				ov_logfile_error("%s: trying to calculate the logarithm of 0 or a negative value", pinst->v_identifier);	\
				STDFB_bad_operation = TRUE;	\
			}	\
			else	\
				pinst->v_OUT.value.valueunion.val_##idsmall##_vec.value[i] = (OV_##idcap) log10(pinst->v_IN.value.valueunion.val_##idsmall##_vec.value[i]);	\
		}	\
	}	\
	else	\
	{	\
		ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);	\
		STDFB_bad_operation = TRUE;	\
		return;	\
	}

#define STDFB_VEC_LOG_I(idcap, idsmall) \
	pinst->v_OUT.value.vartype = OV_VT_SINGLE_VEC;	\
	ov_logfile_warning("%s: input value is integer, output is set to single to prevent data loss", pinst->v_identifier);	\
	if(Ov_OK(Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_single_vec, pinst->v_IN.value.valueunion.val_##idsmall##_vec.veclen, SINGLE)))	\
	{	\
		for(i=0; i<pinst->v_IN.value.valueunion.val_##idsmall##_vec.veclen; i++)	\
		{	\
			if(pinst->v_IN.value.valueunion.val_##idsmall##_vec.value[i] <= 0) \
			{	\
				pinst->v_OUT.value.valueunion.val_single_vec.value[i] = (OV_SINGLE) log10(pinst->v_IN.value.valueunion.val_##idsmall##_vec.value[i]);	\
				ov_logfile_error("%s: trying to calculate the logarithm of 0 or a negative value", pinst->v_identifier);	\
				STDFB_bad_operation = TRUE;	\
			}	\
			else	\
				pinst->v_OUT.value.valueunion.val_single_vec.value[i] = (OV_SINGLE) log10(pinst->v_IN.value.valueunion.val_##idsmall##_vec.value[i]);	\
		}	\
	}	\
	else	\
	{	\
		ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);	\
		STDFB_bad_operation = TRUE;	\
		return;	\
	}	
	
/********Natürliche Exponentiation von vektorlementen*********/
	
#define STDFB_VEC_EXP_D \
	pinst->v_OUT.value.vartype = OV_VT_DOUBLE_VEC;	\
	if(Ov_OK(Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_double_vec, pinst->v_IN.value.valueunion.val_double_vec.veclen, DOUBLE)))	\
	{	\
		for(i=0; i<pinst->v_IN.value.valueunion.val_double_vec.veclen; i++)	\
		{	\
			dbl_temp = exp(pinst->v_IN.value.valueunion.val_double_vec.value[i]);	\
			if((dbl_temp == HUGE_VAL) || (dbl_temp == -HUGE_VAL))	\
			{	\
				ov_logfile_error("%s: result exceeds range of double", pinst->v_identifier);	\
				STDFB_bad_operation = TRUE;	\
			}	\
			pinst->v_OUT.value.valueunion.val_double_vec.value[i] = dbl_temp;	\
		}	\
	}	\
	else	\
	{	\
		ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);	\
		STDFB_bad_operation = TRUE;	\
		return;	\
	}

#define STDFB_VEC_EXP_I(idcap, idsmall) \
	pinst->v_OUT.value.vartype = OV_VT_SINGLE_VEC;	\
	ov_logfile_warning("%s: input value is integer, output is set to single to prevent data loss", pinst->v_identifier);	\
	if(Ov_OK(Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_single_vec, pinst->v_IN.value.valueunion.val_##idsmall##_vec.veclen, SINGLE)))	\
	{	\
		for(i=0; i<pinst->v_IN.value.valueunion.val_##idsmall##_vec.veclen; i++)	\
		{	\
			dbl_temp = exp(pinst->v_IN.value.valueunion.val_##idsmall##_vec.value[i]);	\
			STDFB_CONV_DBL_FLT(dbl_temp, pinst->v_OUT.value.valueunion.val_single_vec.value[i]);	\
			if((pinst->v_OUT.value.valueunion.val_single_vec.value[i] == HUGE_VAL) || (pinst->v_OUT.value.valueunion.val_single_vec.value[i] == -HUGE_VAL))	\
				STDFB_bad_operation = TRUE;	\
		}	\
	}	\
	else	\
	{	\
		ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);	\
		STDFB_bad_operation = TRUE;	\
		return;	\
	}	
	
#define STDFB_VEC_EXP_S \
	pinst->v_OUT.value.vartype = OV_VT_SINGLE_VEC;	\
	if(Ov_OK(Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_single_vec, pinst->v_IN.value.valueunion.val_single_vec.veclen, SINGLE)))	\
	{	\
		for(i=0; i<pinst->v_IN.value.valueunion.val_single_vec.veclen; i++)	\
		{	\
			dbl_temp = exp(pinst->v_IN.value.valueunion.val_single_vec.value[i]);	\
			STDFB_CONV_DBL_FLT(dbl_temp, pinst->v_OUT.value.valueunion.val_single_vec.value[i]);	\
			if((pinst->v_OUT.value.valueunion.val_single_vec.value[i] == HUGE_VAL) || (pinst->v_OUT.value.valueunion.val_single_vec.value[i] == -HUGE_VAL))	\
				STDFB_bad_operation = TRUE;	\
		}	\
	}	\
	else	\
	{	\
		ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);	\
		STDFB_bad_operation = TRUE;	\
		return;	\
	}	
	

	
/******************sinus von Vektorelementen***************/

#define STDFB_VEC_SIN_I(idcap, idsmall) \
	pinst->v_OUT.value.vartype = OV_VT_SINGLE_VEC;	\
	ov_logfile_warning("%s: input value is integer, output is set to single to prevent data loss", pinst->v_identifier);	\
	if(Ov_OK(Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_single_vec, pinst->v_IN.value.valueunion.val_##idsmall##_vec.veclen, SINGLE)))	\
	{	\
		for(i=0; i<pinst->v_IN.value.valueunion.val_##idsmall##_vec.veclen; i++)	\
		{	\
			pinst->v_OUT.value.valueunion.val_single_vec.value[i] = (OV_SINGLE) sin(pinst->v_IN.value.valueunion.val_##idsmall##_vec.value[i]);	\
		}	\
	}	\
	else	\
	{	\
		ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);	\
		return;	\
	}	
	
#define STDFB_VEC_SIN_R(idcap, idsmall) \
	pinst->v_OUT.value.vartype = OV_VT_##idcap##_VEC;	\
	if(Ov_OK(Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_##idsmall##_vec, pinst->v_IN.value.valueunion.val_##idsmall##_vec.veclen, idcap)))	\
	{	\
		for(i=0; i<pinst->v_IN.value.valueunion.val_##idsmall##_vec.veclen; i++)	\
		{	\
			pinst->v_OUT.value.valueunion.val_##idsmall##_vec.value[i] = (OV_##idcap) sin(pinst->v_IN.value.valueunion.val_##idsmall##_vec.value[i]);	\
		}	\
	}	\
	else	\
	{	\
		ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);	\
		return;	\
	}

	
/******************cosinus von Vektorelementen***************/

#define STDFB_VEC_COS_I(idcap, idsmall) \
	pinst->v_OUT.value.vartype = OV_VT_SINGLE_VEC;	\
	ov_logfile_warning("%s: input value is integer, output is set to single to prevent data loss", pinst->v_identifier);	\
	if(Ov_OK(Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_single_vec, pinst->v_IN.value.valueunion.val_##idsmall##_vec.veclen, SINGLE)))	\
	{	\
		for(i=0; i<pinst->v_IN.value.valueunion.val_##idsmall##_vec.veclen; i++)	\
		{	\
			pinst->v_OUT.value.valueunion.val_single_vec.value[i] = (OV_SINGLE) cos(pinst->v_IN.value.valueunion.val_##idsmall##_vec.value[i]);	\
		}	\
	}	\
	else	\
	{	\
		ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);	\
		return;	\
	}	
	
#define STDFB_VEC_COS_R(idcap, idsmall) \
	pinst->v_OUT.value.vartype = OV_VT_##idcap##_VEC;	\
	if(Ov_OK(Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_##idsmall##_vec, pinst->v_IN.value.valueunion.val_##idsmall##_vec.veclen, idcap)))	\
	{	\
		for(i=0; i<pinst->v_IN.value.valueunion.val_##idsmall##_vec.veclen; i++)	\
		{	\
			pinst->v_OUT.value.valueunion.val_##idsmall##_vec.value[i] = (OV_##idcap) cos(pinst->v_IN.value.valueunion.val_##idsmall##_vec.value[i]);	\
		}	\
	}	\
	else	\
	{	\
		ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);	\
		return;	\
	}
	
	
/******************tangens von Vektorelementen***************/

#define STDFB_VEC_TAN_I(idcap, idsmall) \
	pinst->v_OUT.value.vartype = OV_VT_SINGLE_VEC;	\
	ov_logfile_warning("%s: input value is integer, output is set to single to prevent data loss", pinst->v_identifier);	\
	if(Ov_OK(Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_single_vec, pinst->v_IN.value.valueunion.val_##idsmall##_vec.veclen, SINGLE)))	\
	{	\
		for(i=0; i<pinst->v_IN.value.valueunion.val_##idsmall##_vec.veclen; i++)	\
		{	\
			cosine = cos(pinst->v_IN.value.valueunion.val_##idsmall##_vec.value[i]);	\
			if(cosine)	\
				pinst->v_OUT.value.valueunion.val_single_vec.value[i] = (OV_SINGLE) (sin(pinst->v_IN.value.valueunion.val_##idsmall##_vec.value[i]) / cosine);	\
			else	\
			{	\
				ov_logfile_error("%s: tangens not steady for vector-element %d", pinst->v_identifier, i);	\
				pinst->v_OUT.value.valueunion.val_single_vec.value[i] = 0;	\
			}	\
		}	\
	}	\
	else	\
	{	\
		ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);	\
		return;	\
	}	
	
#define STDFB_VEC_TAN_R(idcap, idsmall) \
	pinst->v_OUT.value.vartype = OV_VT_##idcap##_VEC;	\
	if(Ov_OK(Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_##idsmall##_vec, pinst->v_IN.value.valueunion.val_##idsmall##_vec.veclen, idcap)))	\
	{	\
		for(i=0; i<pinst->v_IN.value.valueunion.val_##idsmall##_vec.veclen; i++)	\
		{	\
			cosine = cos(pinst->v_IN.value.valueunion.val_##idsmall##_vec.value[i]);	\
			if(cosine)	\
				pinst->v_OUT.value.valueunion.val_##idsmall##_vec.value[i] = (OV_SINGLE) (sin(pinst->v_IN.value.valueunion.val_##idsmall##_vec.value[i]) / cosine);	\
			else	\
			{	\
				ov_logfile_error("%s: tangens not steady for vector-element %d", pinst->v_identifier, i);	\
				pinst->v_OUT.value.valueunion.val_##idsmall##_vec.value[i] = 0;	\
			}	\
		}	\
	}	\
	else	\
	{	\
		ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);	\
		return;	\
	}
	
	
	#define STDFB_VEC_TAN_D(idcap, idsmall) \
	pinst->v_OUT.value.vartype = OV_VT_##idcap##_VEC;	\
	if(Ov_OK(Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_##idsmall##_vec, pinst->v_IN.value.valueunion.val_##idsmall##_vec.veclen, idcap)))	\
	{	\
		for(i=0; i<pinst->v_IN.value.valueunion.val_##idsmall##_vec.veclen; i++)	\
		{	\
			cosine = cos(pinst->v_IN.value.valueunion.val_##idsmall##_vec.value[i]);	\
			if(cosine)	\
				pinst->v_OUT.value.valueunion.val_##idsmall##_vec.value[i] = sin(pinst->v_IN.value.valueunion.val_##idsmall##_vec.value[i]) / cosine;	\
			else	\
			{	\
				ov_logfile_error("%s: tangens not steady for vector-element %d", pinst->v_identifier, i);	\
				pinst->v_OUT.value.valueunion.val_##idsmall##_vec.value[i] = 0;	\
			}	\
		}	\
	}	\
	else	\
	{	\
		ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);	\
		return;	\
	}
	
/******************arcsin von Vektorelementen***************/

#define STDFB_VEC_ASIN_I(idcap, idsmall) \
	pinst->v_OUT.value.vartype = OV_VT_SINGLE_VEC;	\
	ov_logfile_warning("%s: input value is integer, output is set to single to prevent data loss", pinst->v_identifier);	\
	if(Ov_OK(Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_single_vec, pinst->v_IN.value.valueunion.val_##idsmall##_vec.veclen, SINGLE)))	\
	{	\
		for(i=0; i<pinst->v_IN.value.valueunion.val_##idsmall##_vec.veclen; i++)	\
		{	\
			if((pinst->v_IN.value.valueunion.val_##idsmall##_vec.value[i] >= -1)	\
				&& (pinst->v_IN.value.valueunion.val_##idsmall##_vec.value[i] <= 1))	\
				pinst->v_OUT.value.valueunion.val_single_vec.value[i] = (OV_SINGLE)asin(pinst->v_IN.value.valueunion.val_##idsmall##_vec.value[i]);	\
			else	\
			{	\
				pinst->v_OUT.value.valueunion.val_single_vec.value[i] = (OV_SINGLE)asin(pinst->v_IN.value.valueunion.val_##idsmall##_vec.value[i]);	\
				ov_logfile_error("%s: trying to calculate arc sine of value larger than 1 or smaller than -1, vector-element %d", pinst->v_identifier, i);	\
				STDFB_bad_operation = TRUE;	\
			}	\
		}	\
	}	\
	else	\
	{	\
		ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);	\
		STDFB_bad_operation = TRUE;	\
		return;	\
	}	
	
	#define STDFB_VEC_ASIN_UI(idcap, idsmall) \
	pinst->v_OUT.value.vartype = OV_VT_SINGLE_VEC;	\
	ov_logfile_warning("%s: input value is integer, output is set to single to prevent data loss", pinst->v_identifier);	\
	if(Ov_OK(Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_single_vec, pinst->v_IN.value.valueunion.val_##idsmall##_vec.veclen, SINGLE)))	\
	{	\
		for(i=0; i<pinst->v_IN.value.valueunion.val_##idsmall##_vec.veclen; i++)	\
		{	\
			if(pinst->v_IN.value.valueunion.val_##idsmall##_vec.value[i] <= 1)	\
				pinst->v_OUT.value.valueunion.val_single_vec.value[i] = (OV_SINGLE) asin(pinst->v_IN.value.valueunion.val_##idsmall##_vec.value[i]);	\
			else	\
			{	\
				pinst->v_OUT.value.valueunion.val_single_vec.value[i] = (OV_SINGLE) asin(pinst->v_IN.value.valueunion.val_##idsmall##_vec.value[i]);	\
				ov_logfile_error("%s: trying to calculate arc sine of value larger than 1, vector-element %d", pinst->v_identifier, i);	\
				STDFB_bad_operation = TRUE;	\
			}	\
		}	\
	}	\
	else	\
	{	\
		ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);	\
		STDFB_bad_operation = TRUE;	\
		return;	\
	}	
	
#define STDFB_VEC_ASIN_R(idcap, idsmall) \
	pinst->v_OUT.value.vartype = OV_VT_##idcap##_VEC;	\
	if(Ov_OK(Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_##idsmall##_vec, pinst->v_IN.value.valueunion.val_##idsmall##_vec.veclen, idcap)))	\
	{	\
		for(i=0; i<pinst->v_IN.value.valueunion.val_##idsmall##_vec.veclen; i++)	\
		{	\
			if((pinst->v_IN.value.valueunion.val_##idsmall##_vec.value[i] >= -1)	\
				&& (pinst->v_IN.value.valueunion.val_##idsmall##_vec.value[i] <= 1))	\
				pinst->v_OUT.value.valueunion.val_##idsmall##_vec.value[i] = (OV_##idcap)asin(pinst->v_IN.value.valueunion.val_##idsmall##_vec.value[i]);	\
			else	\
			{	\
				pinst->v_OUT.value.valueunion.val_##idsmall##_vec.value[i] = (OV_##idcap)asin(pinst->v_IN.value.valueunion.val_##idsmall##_vec.value[i]);	\
				ov_logfile_error("%s: trying to calculate arc sine of value larger than 1 or smaller than -1, vector-element %d", pinst->v_identifier, i);	\
				STDFB_bad_operation = TRUE;	\
			}	\
		}	\
	}	\
	else	\
	{	\
		ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);	\
		STDFB_bad_operation = TRUE;	\
		return;	\
	}
	
	
/******************arccos von Vektorelementen***************/

#define STDFB_VEC_ACOS_I(idcap, idsmall) \
	pinst->v_OUT.value.vartype = OV_VT_SINGLE_VEC;	\
	ov_logfile_warning("%s: input value is integer, output is set to single to prevent data loss", pinst->v_identifier);	\
	if(Ov_OK(Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_single_vec, pinst->v_IN.value.valueunion.val_##idsmall##_vec.veclen, SINGLE)))	\
	{	\
		for(i=0; i<pinst->v_IN.value.valueunion.val_##idsmall##_vec.veclen; i++)	\
		{	\
			if((pinst->v_IN.value.valueunion.val_##idsmall##_vec.value[i] >= -1)	\
				&& (pinst->v_IN.value.valueunion.val_##idsmall##_vec.value[i] <= 1))	\
				pinst->v_OUT.value.valueunion.val_single_vec.value[i] = (OV_SINGLE) acos(pinst->v_IN.value.valueunion.val_##idsmall##_vec.value[i]);	\
			else	\
			{	\
				pinst->v_OUT.value.valueunion.val_single_vec.value[i] = (OV_SINGLE) acos(pinst->v_IN.value.valueunion.val_##idsmall##_vec.value[i]);	\
				ov_logfile_error("%s: trying to calculate arc cosine of value larger than 1 or smaller than -1, vector-element %d", pinst->v_identifier, i);	\
				STDFB_bad_operation = TRUE;	\
			}	\
		}	\
	}	\
	else	\
	{	\
		ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);	\
		STDFB_bad_operation = TRUE;	\
		return;	\
	}	
	
	#define STDFB_VEC_ACOS_UI(idcap, idsmall) \
	pinst->v_OUT.value.vartype = OV_VT_SINGLE_VEC;	\
	ov_logfile_warning("%s: input value is integer, output is set to single to prevent data loss", pinst->v_identifier);	\
	if(Ov_OK(Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_single_vec, pinst->v_IN.value.valueunion.val_##idsmall##_vec.veclen, SINGLE)))	\
	{	\
		for(i=0; i<pinst->v_IN.value.valueunion.val_##idsmall##_vec.veclen; i++)	\
		{	\
			if(pinst->v_IN.value.valueunion.val_##idsmall##_vec.value[i] <= 1)	\
				pinst->v_OUT.value.valueunion.val_single_vec.value[i] = (OV_SINGLE) acos(pinst->v_IN.value.valueunion.val_##idsmall##_vec.value[i]);	\
			else	\
			{	\
				pinst->v_OUT.value.valueunion.val_single_vec.value[i] = (OV_SINGLE) acos(pinst->v_IN.value.valueunion.val_##idsmall##_vec.value[i]);	\
				ov_logfile_error("%s: trying to calculate arc sine of value larger than 1, vector-element %d", pinst->v_identifier, i);	\
			STDFB_bad_operation = TRUE;	\
			}	\
		}	\
	}	\
	else	\
	{	\
		ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);	\
		STDFB_bad_operation = TRUE;	\
		return;	\
	}	
	
#define STDFB_VEC_ACOS_R(idcap, idsmall) \
	pinst->v_OUT.value.vartype = OV_VT_##idcap##_VEC;	\
	if(Ov_OK(Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_##idsmall##_vec, pinst->v_IN.value.valueunion.val_##idsmall##_vec.veclen, idcap)))	\
	{	\
		for(i=0; i<pinst->v_IN.value.valueunion.val_##idsmall##_vec.veclen; i++)	\
		{	\
			if((pinst->v_IN.value.valueunion.val_##idsmall##_vec.value[i] >= -1)	\
				&& (pinst->v_IN.value.valueunion.val_##idsmall##_vec.value[i] <= 1))	\
				pinst->v_OUT.value.valueunion.val_##idsmall##_vec.value[i] = (OV_##idcap) acos(pinst->v_IN.value.valueunion.val_##idsmall##_vec.value[i]);	\
			else	\
			{	\
				pinst->v_OUT.value.valueunion.val_##idsmall##_vec.value[i] = (OV_##idcap) acos(pinst->v_IN.value.valueunion.val_##idsmall##_vec.value[i]);	\
				ov_logfile_error("%s: trying to calculate arc sine of value larger than 1 or smaller than -1, vector-element %d", pinst->v_identifier, i);	\
			STDFB_bad_operation = TRUE;	\
			}	\
		}	\
	}	\
	else	\
	{	\
		ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);	\
		STDFB_bad_operation = TRUE;	\
		return;	\
	}
	
	
/******************arkustangens von Vektorelementen***************/

#define STDFB_VEC_ATAN_I(idcap, idsmall) \
	pinst->v_OUT.value.vartype = OV_VT_SINGLE_VEC;	\
	ov_logfile_warning("%s: input value is integer, output is set to single to prevent data loss", pinst->v_identifier);	\
	if(Ov_OK(Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_single_vec, pinst->v_IN.value.valueunion.val_##idsmall##_vec.veclen, SINGLE)))	\
	{	\
		for(i=0; i<pinst->v_IN.value.valueunion.val_##idsmall##_vec.veclen; i++)	\
		{	\
			pinst->v_OUT.value.valueunion.val_single_vec.value[i] = (OV_SINGLE) atan(pinst->v_IN.value.valueunion.val_##idsmall##_vec.value[i]);	\
		}	\
	}	\
	else	\
	{	\
		ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);	\
		return;	\
	}	
	
#define STDFB_VEC_ATAN_R(idcap, idsmall) \
	pinst->v_OUT.value.vartype = OV_VT_##idcap##_VEC;	\
	if(Ov_OK(Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_##idsmall##_vec, pinst->v_IN.value.valueunion.val_##idsmall##_vec.veclen, idcap)))	\
	{	\
		for(i=0; i<pinst->v_IN.value.valueunion.val_##idsmall##_vec.veclen; i++)	\
		{	\
			pinst->v_OUT.value.valueunion.val_##idsmall##_vec.value[i] = (OV_##idcap) atan(pinst->v_IN.value.valueunion.val_##idsmall##_vec.value[i]);	\
		}	\
	}	\
	else	\
	{	\
		ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);	\
		return;	\
	}

/********************** Addition von vektorelementen**************************/	
	
	#define STDFB_VEC_ADD(idcap, idsmall) \
	pinst->v_OUT.value.vartype = OV_VT_##idcap##_VEC;	\
	if(pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen == pinst->v_IN2.value.valueunion.val_##idsmall##_vec.veclen)	\
	{	\
		if(Ov_OK(Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_##idsmall##_vec, pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen, idcap)))	\
		{	\
			for(i=0; i<pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen; i++)	\
			{	\
				pinst->v_OUT.value.valueunion.val_##idsmall##_vec.value[i] = pinst->v_IN1.value.valueunion.val_##idsmall##_vec.value[i] + \
					pinst->v_IN2.value.valueunion.val_##idsmall##_vec.value[i];	\
			}	\
		}	\
		else	\
		{	\
		ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);	\
		STDFB_bad_operation = TRUE;	\
		return;	\
		}	\
	}	\
	else	\
	{	\
		ov_logfile_error("%s: vectors have different lengths, operation not possible", pinst->v_identifier);	\
		Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_##idsmall##_vec, 0, idcap);	\
		STDFB_bad_operation = TRUE;	\
	}

#define STDFB_VEC_ADD_F(idcap, idsmall) \
	pinst->v_OUT.value.vartype = OV_VT_##idcap##_VEC;	\
	if(pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen == pinst->v_IN2.value.valueunion.val_##idsmall##_vec.veclen)	\
	{	\
		if(Ov_OK(Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_##idsmall##_vec, pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen, idcap)))	\
		{	\
			for(i=0; i<pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen; i++)	\
			{	\
				pinst->v_OUT.value.valueunion.val_##idsmall##_vec.value[i] = pinst->v_IN1.value.valueunion.val_##idsmall##_vec.value[i] + \
					pinst->v_IN2.value.valueunion.val_##idsmall##_vec.value[i];	\
					if((pinst->v_OUT.value.valueunion.val_##idsmall##_vec.value[i] == HUGE_VAL) || (pinst->v_OUT.value.valueunion.val_##idsmall##_vec.value[i] == -HUGE_VAL))	\
					{	\
						ov_logfile_error("%s: result in vector-element %d exceeds range of " #idsmall, pinst->v_identifier, i);	\
						STDFB_bad_operation = TRUE;	\
					}	\
			}	\
		}	\
		else	\
		{	\
		ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);	\
		STDFB_bad_operation = TRUE;	\
		return;	\
		}	\
	}	\
	else	\
	{	\
		ov_logfile_error("%s: vectors have different lengths, operation not possible", pinst->v_identifier);	\
		Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_##idsmall##_vec, 0, idcap);	\
		STDFB_bad_operation = TRUE;	\
	}

	
/******Subtraktion von vektorelementen******/


#define STDFB_VEC_SUB(idcap, idsmall) \
	pinst->v_OUT.value.vartype = OV_VT_##idcap##_VEC;	\
	if(pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen == pinst->v_IN2.value.valueunion.val_##idsmall##_vec.veclen)	\
	{	\
		if(Ov_OK(Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_##idsmall##_vec, pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen, idcap)))	\
		{	\
			for(i=0; i<pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen; i++)	\
			{	\
				pinst->v_OUT.value.valueunion.val_##idsmall##_vec.value[i] = pinst->v_IN1.value.valueunion.val_##idsmall##_vec.value[i] - \
					pinst->v_IN2.value.valueunion.val_##idsmall##_vec.value[i];	\
			}	\
		}	\
		else	\
		{	\
		ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);	\
		STDFB_bad_operation = TRUE;	\
		return;	\
		}	\
	}	\
	else	\
	{	\
		ov_logfile_error("%s: vectors have different lengths, operation not possible", pinst->v_identifier);	\
		Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_##idsmall##_vec, 0, idcap);	\
		STDFB_bad_operation = TRUE;	\
	}

	
#define STDFB_VEC_SUB_F(idcap, idsmall) \
	pinst->v_OUT.value.vartype = OV_VT_##idcap##_VEC;	\
	if(pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen == pinst->v_IN2.value.valueunion.val_##idsmall##_vec.veclen)	\
	{	\
		if(Ov_OK(Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_##idsmall##_vec, pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen, idcap)))	\
		{	\
			for(i=0; i<pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen; i++)	\
			{	\
				pinst->v_OUT.value.valueunion.val_##idsmall##_vec.value[i] = pinst->v_IN1.value.valueunion.val_##idsmall##_vec.value[i] - \
					pinst->v_IN2.value.valueunion.val_##idsmall##_vec.value[i];	\
					if((pinst->v_OUT.value.valueunion.val_##idsmall##_vec.value[i] == HUGE_VAL) || (pinst->v_OUT.value.valueunion.val_##idsmall##_vec.value[i] == -HUGE_VAL))	\
					{	\
						ov_logfile_error("%s: result in vector-element %d exceeds range of " #idsmall, pinst->v_identifier, i);	\
						STDFB_bad_operation = TRUE;	\
					}	\
			}	\
		}	\
		else	\
		{	\
		ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);	\
		STDFB_bad_operation = TRUE;	\
		return;	\
		}	\
	}	\
	else	\
	{	\
		ov_logfile_error("%s: vectors have different lengths, operation not possible", pinst->v_identifier);	\
		Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_##idsmall##_vec, 0, idcap);	\
		STDFB_bad_operation = TRUE;	\
	}
	
/******Multiplikation von vektorelementen******/
	

#define STDFB_VEC_MUL(idcap, idsmall) \
	pinst->v_OUT.value.vartype = OV_VT_##idcap##_VEC;	\
	if(pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen == pinst->v_IN2.value.valueunion.val_##idsmall##_vec.veclen)	\
	{	\
		if(Ov_OK(Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_##idsmall##_vec, pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen, idcap)))	\
		{	\
			for(i=0; i<pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen; i++)	\
			{	\
				pinst->v_OUT.value.valueunion.val_##idsmall##_vec.value[i] = pinst->v_IN1.value.valueunion.val_##idsmall##_vec.value[i] * \
					pinst->v_IN2.value.valueunion.val_##idsmall##_vec.value[i];	\
			}	\
		}	\
		else	\
		{	\
		ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);	\
		STDFB_bad_operation = TRUE;	\
		return;	\
		}	\
	}	\
	else	\
	{	\
		ov_logfile_error("%s: vectors have different lengths, operation not possible", pinst->v_identifier);	\
		Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_##idsmall##_vec, 0, idcap);	\
		STDFB_bad_operation = TRUE;	\
	}
	

	
#define STDFB_VEC_MUL_F(idcap, idsmall) \
	pinst->v_OUT.value.vartype = OV_VT_##idcap##_VEC;	\
	if(pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen == pinst->v_IN2.value.valueunion.val_##idsmall##_vec.veclen)	\
	{	\
		if(Ov_OK(Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_##idsmall##_vec, pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen, idcap)))	\
		{	\
			for(i=0; i<pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen; i++)	\
			{	\
				pinst->v_OUT.value.valueunion.val_##idsmall##_vec.value[i] = pinst->v_IN1.value.valueunion.val_##idsmall##_vec.value[i] * \
					pinst->v_IN2.value.valueunion.val_##idsmall##_vec.value[i];	\
					if((pinst->v_OUT.value.valueunion.val_##idsmall##_vec.value[i] == HUGE_VAL) || (pinst->v_OUT.value.valueunion.val_##idsmall##_vec.value[i] == -HUGE_VAL))	\
					{	\
						ov_logfile_error("%s: result in vector-element %d exceeds range of " #idsmall, pinst->v_identifier, i);	\
						STDFB_bad_operation = TRUE;	\
					}	\
			}	\
		}	\
		else	\
		{	\
		ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);	\
		STDFB_bad_operation = TRUE;	\
		return;	\
		}	\
	}	\
	else	\
	{	\
		ov_logfile_error("%s: vectors have different lengths, operation not possible", pinst->v_identifier);	\
		Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_##idsmall##_vec, 0, idcap);	\
		STDFB_bad_operation = TRUE;	\
	}
	

/******Division von vektorelementen******/
	

#define STDFB_VEC_DIV(idcap, idsmall) \
	pinst->v_OUT.value.vartype = OV_VT_##idcap##_VEC;	\
	if(pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen == pinst->v_IN2.value.valueunion.val_##idsmall##_vec.veclen)	\
	{	\
		if(Ov_OK(Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_##idsmall##_vec, pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen, idcap)))	\
		{	\
			for(i=0; i<pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen; i++)	\
			{	\
				if(pinst->v_IN2.value.valueunion.val_##idsmall##_vec.value[i])	\
					pinst->v_OUT.value.valueunion.val_##idsmall##_vec.value[i] = pinst->v_IN1.value.valueunion.val_##idsmall##_vec.value[i] / \
						pinst->v_IN2.value.valueunion.val_##idsmall##_vec.value[i];	\
				else	\
				{	\
					if(pinst->v_IN1.value.valueunion.val_##idsmall##_vec.value[i] > 0)	\
						pinst->v_OUT.value.valueunion.val_##idsmall##_vec.value[i] = LONG_MAX;	\
					else if(pinst->v_IN1.value.valueunion.val_##idsmall##_vec.value[i] < 0)	\
						pinst->v_OUT.value.valueunion.val_##idsmall##_vec.value[i] = LONG_MIN;	\
					else	\
						pinst->v_OUT.value.valueunion.val_##idsmall##_vec.value[i] = 0;	\
					ov_logfile_error("%s: division by 0 in vector-element %d", pinst->v_identifier, i);	\
					STDFB_bad_operation = TRUE;	\
				}	\
			}	\
		}	\
		else	\
		{	\
		ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);	\
		STDFB_bad_operation = TRUE;	\
		return;	\
		}	\
	}	\
	else	\
	{	\
		ov_logfile_error("%s: vectors have different lengths, operation not possible", pinst->v_identifier);	\
		Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_##idsmall##_vec, 0, idcap);	\
		STDFB_bad_operation = TRUE;	\
	}
	
#define STDFB_VEC_DIVU(idcap, idsmall) \
	pinst->v_OUT.value.vartype = OV_VT_##idcap##_VEC;	\
	if(pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen == pinst->v_IN2.value.valueunion.val_##idsmall##_vec.veclen)	\
	{	\
		if(Ov_OK(Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_##idsmall##_vec, pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen, idcap)))	\
		{	\
			for(i=0; i<pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen; i++)	\
			{	\
				if(pinst->v_IN2.value.valueunion.val_##idsmall##_vec.value[i])	\
					pinst->v_OUT.value.valueunion.val_##idsmall##_vec.value[i] = pinst->v_IN1.value.valueunion.val_##idsmall##_vec.value[i] / \
						pinst->v_IN2.value.valueunion.val_##idsmall##_vec.value[i];	\
				else	\
				{	\
					if(pinst->v_IN1.value.valueunion.val_##idsmall##_vec.value[i])	\
						pinst->v_OUT.value.valueunion.val_##idsmall##_vec.value[i] = (OV_##idcap) ULONG_MAX;	\
					else	\
						pinst->v_OUT.value.valueunion.val_##idsmall##_vec.value[i] = 0;	\
					ov_logfile_error("%s: division by 0 in vector-element %d", pinst->v_identifier, i);	\
					STDFB_bad_operation = TRUE;	\
				}	\
			}	\
		}	\
		else	\
		{	\
		ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);	\
		STDFB_bad_operation = TRUE;	\
		return;	\
		}	\
	}	\
	else	\
	{	\
		ov_logfile_error("%s: vectors have different lengths, operation not possible", pinst->v_identifier);	\
		Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_##idsmall##_vec, 0, idcap);	\
		STDFB_bad_operation = TRUE;	\
	}
	
#define STDFB_VEC_DIV_F(idcap, idsmall) \
	pinst->v_OUT.value.vartype = OV_VT_##idcap##_VEC;	\
	if(pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen == pinst->v_IN2.value.valueunion.val_##idsmall##_vec.veclen)	\
	{	\
		if(Ov_OK(Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_##idsmall##_vec, pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen, idcap)))	\
		{	\
			for(i=0; i<pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen; i++)	\
			{	\
				if(pinst->v_IN2.value.valueunion.val_##idsmall##_vec.value[i])	\
				{	\
					pinst->v_OUT.value.valueunion.val_##idsmall##_vec.value[i] = pinst->v_IN1.value.valueunion.val_##idsmall##_vec.value[i] / \
						pinst->v_IN2.value.valueunion.val_##idsmall##_vec.value[i];	\
					if((pinst->v_OUT.value.valueunion.val_##idsmall##_vec.value[i] == HUGE_VAL) || (pinst->v_OUT.value.valueunion.val_##idsmall##_vec.value[i] == -HUGE_VAL))	\
					{	\
						ov_logfile_error("%s: result in vector-element %d exceeds range of " #idsmall, pinst->v_identifier, i);	\
						STDFB_bad_operation = TRUE;	\
					}	\
				}	\
				else	\
				{	\
					ov_logfile_error("%s: division by 0 in vector-element %d, setting result to 0", pinst->v_identifier, i);	\
					if(pinst->v_IN1.value.valueunion.val_##idsmall##_vec.value[i] > 0)	\
						pinst->v_OUT.value.valueunion.val_##idsmall##_vec.value[i] = (OV_##idcap) HUGE_VAL;	\
					else if(pinst->v_IN1.value.valueunion.val_##idsmall##_vec.value[i] < 0)	\
						pinst->v_OUT.value.valueunion.val_##idsmall##_vec.value[i] = (OV_##idcap) -HUGE_VAL;	\
					else	\
						pinst->v_OUT.value.valueunion.val_##idsmall##_vec.value[i] = 0;	\
					STDFB_bad_operation = TRUE;	\
				}	\
			}	\
		}	\
		else	\
		{	\
		ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);	\
		STDFB_bad_operation = TRUE;	\
		return;	\
		}	\
	}	\
	else	\
	{	\
		ov_logfile_error("%s: vectors have different lengths, operation not possible", pinst->v_identifier);	\
		Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_##idsmall##_vec, 0, idcap);	\
		STDFB_bad_operation = TRUE;	\
	}
	

/******Modulo von vektorelementen******/
	

#define STDFB_VEC_MOD(idcap, idsmall) \
	pinst->v_OUT.value.vartype = OV_VT_##idcap##_VEC;	\
	if(pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen == pinst->v_IN2.value.valueunion.val_##idsmall##_vec.veclen)	\
	{	\
		if(Ov_OK(Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_##idsmall##_vec, pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen, idcap)))	\
		{	\
			for(i=0; i<pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen; i++)	\
			{	\
				if(pinst->v_IN2.value.valueunion.val_##idsmall##_vec.value[i])	\
				pinst->v_OUT.value.valueunion.val_##idsmall##_vec.value[i] = pinst->v_IN1.value.valueunion.val_##idsmall##_vec.value[i] % \
					pinst->v_IN2.value.valueunion.val_##idsmall##_vec.value[i];	\
				else	\
				{	\
					pinst->v_OUT.value.valueunion.val_##idsmall##_vec.value[i] = (OV_##idcap) ULONG_MAX;	\
					STDFB_bad_operation = TRUE;	\
				}	\
			}	\
		}	\
		else	\
		{	\
		ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);	\
		STDFB_bad_operation = TRUE;	\
		return;	\
		}	\
	}	\
	else	\
	{	\
		ov_logfile_error("%s: vectors have different lengths, operation not possible", pinst->v_identifier);	\
		Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_##idsmall##_vec, 0, idcap);	\
		STDFB_bad_operation = TRUE;	\
	}
	
#define STDFB_VEC_MODS(idcap, idsmall) \
	pinst->v_OUT.value.vartype = OV_VT_##idcap##_VEC;	\
	if(pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen == pinst->v_IN2.value.valueunion.val_##idsmall##_vec.veclen)	\
	{	\
		if(Ov_OK(Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_##idsmall##_vec, pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen, idcap)))	\
		{	\
			for(i=0; i<pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen; i++)	\
			{	\
				if(pinst->v_IN2.value.valueunion.val_##idsmall##_vec.value[i])	\
				pinst->v_OUT.value.valueunion.val_##idsmall##_vec.value[i] = pinst->v_IN1.value.valueunion.val_##idsmall##_vec.value[i] % \
					pinst->v_IN2.value.valueunion.val_##idsmall##_vec.value[i];	\
				else	\
				{	\
					pinst->v_OUT.value.valueunion.val_##idsmall##_vec.value[i] = (OV_##idcap) LONG_MAX;	\
					STDFB_bad_operation = TRUE;	\
				}	\
			}	\
		}	\
		else	\
		{	\
		ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);	\
		STDFB_bad_operation = TRUE;	\
		return;	\
		}	\
	}	\
	else	\
	{	\
		ov_logfile_error("%s: vectors have different lengths, operation not possible", pinst->v_identifier);	\
		Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_##idsmall##_vec, 0, idcap);	\
		STDFB_bad_operation = TRUE;	\
	}
	
/******Exponenzieren von vektorelementen******/
	

#define STDFB_VEC_EXPT(idcap, idsmall) \
	pinst->v_OUT.value.vartype = OV_VT_##idcap##_VEC;	\
	if(pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen == pinst->v_IN2.value.valueunion.val_##idsmall##_vec.veclen)	\
	{	\
		if(Ov_OK(Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_##idsmall##_vec, pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen, idcap)))	\
		{	\
			for(i=0; i<pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen; i++)	\
			{	\
				pinst->v_OUT.value.valueunion.val_##idsmall##_vec.value[i] = (OV_##idcap) pow(pinst->v_IN1.value.valueunion.val_##idsmall##_vec.value[i], \
					pinst->v_IN2.value.valueunion.val_##idsmall##_vec.value[i]);	\
			}	\
		}	\
		else	\
		{	\
			ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);	\
			STDFB_bad_operation = TRUE;	\
			return;	\
		}	\
	}	\
	else	\
	{	\
		ov_logfile_error("%s: vectors have different lengths, operation not possible", pinst->v_identifier);	\
		Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_##idsmall##_vec, 0, idcap);	\
		STDFB_bad_operation = TRUE;	\
	}
	
#define STDFB_VEC_EXPT_S	\
	pinst->v_OUT.value.vartype = OV_VT_SINGLE_VEC;	\
	if(pinst->v_IN1.value.valueunion.val_single_vec.veclen == pinst->v_IN2.value.valueunion.val_single_vec.veclen)	\
	{	\
		if(Ov_OK(Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_single_vec, pinst->v_IN1.value.valueunion.val_single_vec.veclen, SINGLE)))	\
		{	\
			for(i=0; i<pinst->v_IN1.value.valueunion.val_single_vec.veclen; i++)	\
			{	\
				dbl_temp = pow(pinst->v_IN1.value.valueunion.val_single_vec.value[i], \
					pinst->v_IN2.value.valueunion.val_single_vec.value[i]);	\
				if(dbl_temp > FLT_MAX)	\
				{	\
					ov_logfile_error("%s: result in vector-element %d exceeds range of single", pinst->v_identifier, i);	\
					pinst->v_OUT.value.valueunion.val_single_vec.value[i] = (OV_SINGLE) HUGE_VAL;	\
					STDFB_bad_operation = TRUE;	\
				}	\
				else if(dbl_temp < FLT_MIN)	\
				{	\
					ov_logfile_error("%s: result in vector-element %d exceeds range of single", pinst->v_identifier, i);	\
					pinst->v_OUT.value.valueunion.val_single_vec.value[i] = (OV_SINGLE) -HUGE_VAL;	\
					STDFB_bad_operation = TRUE;	\
				}	\
				else	\
					pinst->v_OUT.value.valueunion.val_single_vec.value[i] = (OV_SINGLE) dbl_temp;	\
			}	\
		}	\
		else	\
		{	\
		ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);	\
		STDFB_bad_operation = TRUE;	\
		return;	\
		}	\
	}	\
	else	\
	{	\
		ov_logfile_error("%s: vectors have different lengths, operation not possible", pinst->v_identifier);	\
		Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_single_vec, 0, SINGLE);	\
		STDFB_bad_operation = TRUE;	\
	}
	
	
#define STDFB_VEC_EXPT_D	\
	pinst->v_OUT.value.vartype = OV_VT_DOUBLE_VEC;	\
	if(pinst->v_IN1.value.valueunion.val_double_vec.veclen == pinst->v_IN2.value.valueunion.val_double_vec.veclen)	\
	{	\
		if(Ov_OK(Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_double_vec, pinst->v_IN1.value.valueunion.val_double_vec.veclen, DOUBLE)))	\
		{	\
			for(i=0; i<pinst->v_IN1.value.valueunion.val_double_vec.veclen; i++)	\
			{	\
				dbl_temp = pow(pinst->v_IN1.value.valueunion.val_double_vec.value[i], \
					pinst->v_IN2.value.valueunion.val_double_vec.value[i]);	\
				if((dbl_temp == HUGE_VAL) || (dbl_temp == -HUGE_VAL))	\
				{	\
					ov_logfile_error("%s: result in vector-element %d exceeds range of double", pinst->v_identifier, i);	\
					STDFB_bad_operation = TRUE;	\
				}	\
				else	\
				pinst->v_OUT.value.valueunion.val_double_vec.value[i] = dbl_temp;	\
			}	\
		}	\
		else	\
		{	\
		ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);	\
		STDFB_bad_operation = TRUE;	\
		return;	\
		}	\
	}	\
	else	\
	{	\
		ov_logfile_error("%s: vectors have different lengths, operation not possible", pinst->v_identifier);	\
		Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_double_vec, 0, DOUBLE);	\
		STDFB_bad_operation = TRUE;	\
	}
