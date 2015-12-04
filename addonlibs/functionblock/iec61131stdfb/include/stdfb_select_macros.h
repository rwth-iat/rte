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
*	stdfb_select_macros.h
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



/************************auswahl (binär oder mux) macros NICHT für strings, time und time_span anwenden***************************/

#define STDFB_SEL(idcap, idsmall, sel_in)	\
	case OV_VT_##idcap:	\
		pinst->v_OUT.value.vartype = OV_VT_##idcap;	\
		pinst->v_OUT.value.valueunion.val_##idsmall = pinst->v_##sel_in.value.valueunion.val_##idsmall;	\
	break;	
		
		
#define STDFB_SEL_VEC(idcap, idsmall, sel_in)	\
	case OV_VT_##idcap##_VEC:	\
		pinst->v_OUT.value.vartype = OV_VT_##idcap##_VEC;	\
		if(Ov_OK(Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_##idsmall##_vec, pinst->v_##sel_in.value.valueunion.val_##idsmall##_vec.veclen, idcap)))	\
		{	\
			for(i=0; i<pinst->v_##sel_in.value.valueunion.val_##idsmall##_vec.veclen; i++)	\
			{	\
				pinst->v_OUT.value.valueunion.val_##idsmall##_vec.value[i] = pinst->v_##sel_in.value.valueunion.val_##idsmall##_vec.value[i];	\
			}	\
		}	\
		else	\
		{	\
		ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);	\
		return;	\
		}	\
	break;	
	
/***************************spezielle macros für strings und times********************************/
	
#define STDFB_SEL_STR(sel_in)	\
	case OV_VT_STRING:	\
				pinst->v_OUT.value.vartype = OV_VT_STRING;	\
				if(Ov_Fail(ov_string_setvalue(&pinst->v_OUT.value.valueunion.val_string, pinst->v_##sel_in.value.valueunion.val_string)))	\
				{	\
					ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);	\
					return;	\
				}	\
			break;	
			
#define STDFB_SEL_TIME(idcap, idsmall, sel_in)	\
	case OV_VT_##idcap:	\
					pinst->v_OUT.value.vartype = OV_VT_##idcap;	\
					pinst->v_OUT.value.valueunion.val_##idsmall.secs = pinst->v_##sel_in.value.valueunion.val_##idsmall.secs;	\
					pinst->v_OUT.value.valueunion.val_##idsmall.usecs = pinst->v_##sel_in.value.valueunion.val_##idsmall.usecs;	\
			break;
			
#define STDFB_SEL_STRING_VEC(sel_in)	\
	case OV_VT_STRING_VEC:	\
	pinst->v_OUT.value.vartype = OV_VT_STRING_VEC;	\
		if(Ov_OK(Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_string_vec, pinst->v_##sel_in.value.valueunion.val_string_vec.veclen, STRING)))	\
		{	\
			for(i=0; i<pinst->v_##sel_in.value.valueunion.val_string_vec.veclen; i++)	\
			{	\
				if(Ov_Fail(ov_string_setvalue(&pinst->v_OUT.value.valueunion.val_string_vec.value[i], pinst->v_##sel_in.value.valueunion.val_string_vec.value[i])))	\
				{	\
					ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);	\
					return;	\
				}	\
			}	\
		}	\
		else	\
		{	\
		ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);	\
		return;	\
		}	\
	break;	
	
#define STDFB_SEL_TIME_VEC(idcap, idsmall, sel_in)	\
	case OV_VT_##idcap##_VEC:	\
		pinst->v_OUT.value.vartype = OV_VT_##idcap##_VEC;	\
		if(Ov_OK(Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_##idsmall##_vec, pinst->v_##sel_in.value.valueunion.val_##idsmall##_vec.veclen, idcap)))	\
		{	\
			for(i=0; i<pinst->v_##sel_in.value.valueunion.val_##idsmall##_vec.veclen; i++)	\
			{	\
				pinst->v_OUT.value.valueunion.val_##idsmall##_vec.value[i].secs = pinst->v_##sel_in.value.valueunion.val_##idsmall##_vec.value[i].secs;	\
				pinst->v_OUT.value.valueunion.val_##idsmall##_vec.value[i].usecs = pinst->v_##sel_in.value.valueunion.val_##idsmall##_vec.value[i].usecs;	\
			}	\
		}	\
		else	\
		{	\
		ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);	\
		return;	\
		}	\
	break;	
	
	
/*****************************Demux**************************/

#define STDFB_DEMUX(num)	\
	switch(pinst->v_IN##num.value.vartype & OV_VT_KSMASK)	\
		{	\
			STDFB_SEL(BYTE, byte, IN##num);	\
			STDFB_SEL(BOOL, bool, IN##num);	\
			STDFB_SEL(INT, int, IN##num);	\
			STDFB_SEL(UINT, uint, IN##num);	\
			STDFB_SEL(SINGLE, single, IN##num);	\
			STDFB_SEL(DOUBLE, double, IN##num);	\
			STDFB_SEL_STR(IN##num);	\
			STDFB_SEL_TIME(TIME, time, IN##num);	\
			STDFB_SEL_TIME(TIME_SPAN, time_span, IN##num);	\
			STDFB_SEL_VEC(BYTE, byte, IN##num);	\
			STDFB_SEL_VEC(BOOL, bool, IN##num);	\
			STDFB_SEL_VEC(INT, int, IN##num);	\
			STDFB_SEL_VEC(UINT, uint, IN##num);	\
			STDFB_SEL_VEC(SINGLE, single, IN##num);	\
			STDFB_SEL_VEC(DOUBLE, double, IN##num);	\
			STDFB_SEL_STRING_VEC(IN##num);	\
			STDFB_SEL_TIME_VEC(TIME,time, IN##num);	\
			STDFB_SEL_TIME_VEC(TIME_SPAN, time_span, IN##num);	\
			default:	\
				ov_logfile_error("%s: datatype of IN" #num "is currently not supported within this block", pinst->v_identifier);	\
				pinst->v_OUT.value.vartype = OV_VT_BOOL;	\
				pinst->v_OUT.value.valueunion.val_bool = 0;	\
			break;	\
		}	
		
		
/******************** maximumsauswahl********************/

#define STDFB_SEL_MAX(idcap, idsmall)	\
	pinst->v_OUT.value.vartype = OV_VT_##idcap;	\
	if(pinst->v_IN1.value.valueunion.val_##idsmall >= pinst->v_IN2.value.valueunion.val_##idsmall)	\
	{	\
		pinst->v_OUT.value.valueunion.val_##idsmall = pinst->v_IN1.value.valueunion.val_##idsmall;	\
	}	\
	else	\
	{	\
		pinst->v_OUT.value.valueunion.val_##idsmall = pinst->v_IN2.value.valueunion.val_##idsmall;	\
	}
	
#define STDFB_SEL_MAX_VEC(idcap, idsmall)	\
	pinst->v_OUT.value.vartype = OV_VT_##idcap##_VEC;	\
	if(pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen >= pinst->v_IN2.value.valueunion.val_##idsmall##_vec.veclen)	\
	{	\
		if(Ov_Fail(Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_##idsmall##_vec, pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen, idcap)))	\
		{	\
			ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);	\
			return;	\
		}	\
		longer = 1;	\
		s_length = pinst->v_IN2.value.valueunion.val_##idsmall##_vec.veclen;	\
		l_length = pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen;	\
	}	\
	else	\
	{	\
		if(Ov_Fail(Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_##idsmall##_vec, pinst->v_IN2.value.valueunion.val_##idsmall##_vec.veclen, idcap)))	\
		{	\
			ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);	\
			return;	\
		}	\
		longer = 0;	\
		s_length = pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen;	\
		l_length = pinst->v_IN2.value.valueunion.val_##idsmall##_vec.veclen;	\
	}	\
	for(i=0; i < s_length; i++)	\
	{	\
		if(pinst->v_IN1.value.valueunion.val_##idsmall##_vec.value[i] >= pinst->v_IN2.value.valueunion.val_##idsmall##_vec.value[i])	\
			pinst->v_OUT.value.valueunion.val_##idsmall##_vec.value[i] = pinst->v_IN1.value.valueunion.val_##idsmall##_vec.value[i];	\
		else	\
			pinst->v_OUT.value.valueunion.val_##idsmall##_vec.value[i] = pinst->v_IN2.value.valueunion.val_##idsmall##_vec.value[i];	\
	}	\
	if(longer)	\
	{	\
		for(i=s_length; i < l_length; i++)	\
			pinst->v_OUT.value.valueunion.val_##idsmall##_vec.value[i] = pinst->v_IN1.value.valueunion.val_##idsmall##_vec.value[i];	\
	}	\
	else	\
	{	\
		for(i=s_length; i < l_length; i++)	\
			pinst->v_OUT.value.valueunion.val_##idsmall##_vec.value[i] = pinst->v_IN2.value.valueunion.val_##idsmall##_vec.value[i];	\
	}

/*****************Minimumsauswahl********************/

/******************** maximumsauswahl********************/

#define STDFB_SEL_MIN(idcap, idsmall)	\
	pinst->v_OUT.value.vartype = OV_VT_##idcap;	\
	if(pinst->v_IN1.value.valueunion.val_##idsmall <= pinst->v_IN2.value.valueunion.val_##idsmall)	\
	{	\
		pinst->v_OUT.value.valueunion.val_##idsmall = pinst->v_IN1.value.valueunion.val_##idsmall;	\
	}	\
	else	\
	{	\
		pinst->v_OUT.value.valueunion.val_##idsmall = pinst->v_IN2.value.valueunion.val_##idsmall;	\
	}
	
#define STDFB_SEL_MIN_VEC(idcap, idsmall)	\
	pinst->v_OUT.value.vartype = OV_VT_##idcap##_VEC;	\
	if(pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen >= pinst->v_IN2.value.valueunion.val_##idsmall##_vec.veclen)	\
	{	\
		if(Ov_Fail(Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_##idsmall##_vec, pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen, idcap)))	\
		{	\
			ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);	\
			return;	\
		}	\
		longer = 1;	\
		s_length = pinst->v_IN2.value.valueunion.val_##idsmall##_vec.veclen;	\
		l_length = pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen;	\
	}	\
	else	\
	{	\
		if(Ov_Fail(Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_##idsmall##_vec, pinst->v_IN2.value.valueunion.val_##idsmall##_vec.veclen, idcap)))	\
		{	\
			ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);	\
			return;	\
		}	\
		longer = 0;	\
		s_length = pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen;	\
		l_length = pinst->v_IN2.value.valueunion.val_##idsmall##_vec.veclen;	\
	}	\
	for(i=0; i < s_length; i++)	\
	{	\
		if(pinst->v_IN1.value.valueunion.val_##idsmall##_vec.value[i] <= pinst->v_IN2.value.valueunion.val_##idsmall##_vec.value[i])	\
			pinst->v_OUT.value.valueunion.val_##idsmall##_vec.value[i] = pinst->v_IN1.value.valueunion.val_##idsmall##_vec.value[i];	\
		else	\
			pinst->v_OUT.value.valueunion.val_##idsmall##_vec.value[i] = pinst->v_IN2.value.valueunion.val_##idsmall##_vec.value[i];	\
	}	\
	if(longer)	\
	{	\
		for(i=s_length; i < l_length; i++)	\
			pinst->v_OUT.value.valueunion.val_##idsmall##_vec.value[i] = pinst->v_IN1.value.valueunion.val_##idsmall##_vec.value[i];	\
	}	\
	else	\
	{	\
		for(i=s_length; i < l_length; i++)	\
			pinst->v_OUT.value.valueunion.val_##idsmall##_vec.value[i] = pinst->v_IN2.value.valueunion.val_##idsmall##_vec.value[i];	\
	}
	
/*************************************limiter***************************/

#define STDFB_LIMIT(idcap, idsmall)	\
	case OV_VT_##idcap:	\
		if(pinst->v_MN.value.valueunion.val_##idsmall <= pinst->v_MX.value.valueunion.val_##idsmall)	\
		{	\
			pinst->v_OUT.value.vartype = OV_VT_##idcap;	\
			if(pinst->v_IN.value.valueunion.val_##idsmall < pinst->v_MN.value.valueunion.val_##idsmall)	\
				pinst->v_OUT.value.valueunion.val_##idsmall = pinst->v_MN.value.valueunion.val_##idsmall;	\
			else	\
				if(pinst->v_IN.value.valueunion.val_##idsmall > pinst->v_MX.value.valueunion.val_##idsmall)	\
					pinst->v_OUT.value.valueunion.val_##idsmall = pinst->v_MX.value.valueunion.val_##idsmall;	\
				else	\
					pinst->v_OUT.value.valueunion.val_##idsmall = pinst->v_IN.value.valueunion.val_##idsmall;	\
		}	\
		else	\
		{	\
			ov_logfile_error("%s: minimum is larger than maximum", pinst->v_identifier);	\
			pinst->v_OUT.value.vartype = OV_VT_BOOL;	\
			pinst->v_OUT.value.valueunion.val_bool = 0;	\
			return;	\
		}	\
	break;

#define STDFB_LIMIT_VEC(idcap, idsmall)	\
	case OV_VT_##idcap##_VEC:	\
		pinst->v_OUT.value.vartype = OV_VT_##idcap##_VEC;	\
		if((pinst->v_MN.value.vartype & OV_VT_ISVECTOR))	\
		{	\
			if(Ov_Fail(Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_##idsmall##_vec, pinst->v_IN.value.valueunion.val_##idsmall##_vec.veclen, idcap)))	\
			{	\
				ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);	\
			return;	\
			}	\
			for(i=0; i < pinst->v_IN.value.valueunion.val_##idsmall##_vec.veclen; i++)	\
			{	\
				if(i < pinst->v_MN.value.valueunion.val_##idsmall##_vec.veclen)	\
				{	\
					if(pinst->v_IN.value.valueunion.val_##idsmall##_vec.value[i] < pinst->v_MN.value.valueunion.val_##idsmall##_vec.value[i])	\
					{	\
						pinst->v_OUT.value.valueunion.val_##idsmall##_vec.value[i] = pinst->v_MN.value.valueunion.val_##idsmall##_vec.value[i];	\
						continue;	\
					}	\
				}	\
				else	\
					if(pinst->v_IN.value.valueunion.val_##idsmall##_vec.value[i] < 0)	\
					{	\
						pinst->v_OUT.value.valueunion.val_##idsmall##_vec.value[i] = 0;	\
						continue;	\
					}	\
				if(i < pinst->v_MX.value.valueunion.val_##idsmall##_vec.veclen)	\
				{	\
					if(pinst->v_IN.value.valueunion.val_##idsmall##_vec.value[i] > pinst->v_MX.value.valueunion.val_##idsmall##_vec.value[i])	\
					{	\
						pinst->v_OUT.value.valueunion.val_##idsmall##_vec.value[i] = pinst->v_MX.value.valueunion.val_##idsmall##_vec.value[i];	\
						continue;	\
					}	\
				}	\
				else	\
					if(pinst->v_IN.value.valueunion.val_##idsmall##_vec.value[i] > 0)	\
					{	\
						pinst->v_OUT.value.valueunion.val_##idsmall##_vec.value[i] = 0;	\
						continue;	\
					}	\
				pinst->v_OUT.value.valueunion.val_##idsmall##_vec.value[i] = pinst->v_IN.value.valueunion.val_##idsmall##_vec.value[i];	\
			}	\
		}	\
		else	\
		{	\
			if(Ov_Fail(Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_##idsmall##_vec, pinst->v_IN.value.valueunion.val_##idsmall##_vec.veclen, idcap)))	\
			{	\
				ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);	\
			return;	\
			}	\
			for(i=0; i < pinst->v_IN.value.valueunion.val_##idsmall##_vec.veclen; i++)	\
			{	\
				if(pinst->v_IN.value.valueunion.val_##idsmall##_vec.value[i] < pinst->v_MN.value.valueunion.val_##idsmall)	\
					pinst->v_OUT.value.valueunion.val_##idsmall##_vec.value[i] = pinst->v_MN.value.valueunion.val_##idsmall;	\
				else	\
					if(pinst->v_IN.value.valueunion.val_##idsmall##_vec.value[i] > pinst->v_MX.value.valueunion.val_##idsmall)	\
						pinst->v_OUT.value.valueunion.val_##idsmall##_vec.value[i] = pinst->v_MX.value.valueunion.val_##idsmall;	\
					else	\
						pinst->v_OUT.value.valueunion.val_##idsmall##_vec.value[i] = pinst->v_IN.value.valueunion.val_##idsmall##_vec.value[i];	\
			}	\
		}	\
	break;

	