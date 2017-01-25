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
*	stdfb_comp_macros.h
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


/******************* größer als ******************/

#define STDFB_GT(idcap, idsmall)	\
	case OV_VT_##idcap:	\
		if(pinst->v_IN1.value.valueunion.val_##idsmall > pinst->v_IN2.value.valueunion.val_##idsmall)	\
			pinst->v_OUT = TRUE;	\
		else	\
			pinst->v_OUT = FALSE;	\
	break;
	
	
#define STDFB_GT_TIME(tim1, tim2)	\
	((tim1.secs > tim2.secs) || ((tim1.secs == tim2.secs) && (tim1.usecs > tim2.usecs)))
	
#define STDFB_GT_VEC(idcap, idsmall)	\
	case OV_VT_##idcap##_VEC:	\
		if(pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen > pinst->v_IN2.value.valueunion.val_##idsmall##_vec.veclen)	\
			pinst->v_OUT = TRUE;	\
		else	\
			pinst->v_OUT = FALSE;	\
	break;
	
	
/******************* größer / gleich ******************/

#define STDFB_GE(idcap, idsmall)	\
	case OV_VT_##idcap:	\
		if(pinst->v_IN1.value.valueunion.val_##idsmall >= pinst->v_IN2.value.valueunion.val_##idsmall)	\
			pinst->v_OUT = TRUE;	\
		else	\
			pinst->v_OUT = FALSE;	\
	break;
	
	
#define STDFB_GE_TIME(tim1, tim2)	\
	((tim1.secs > tim2.secs) || ((tim1.secs == tim2.secs) && (tim1.usecs >= tim2.usecs)))
	
#define STDFB_GE_VEC(idcap, idsmall)	\
	case OV_VT_##idcap##_VEC:	\
		if(pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen >= pinst->v_IN2.value.valueunion.val_##idsmall##_vec.veclen)	\
			pinst->v_OUT = TRUE;	\
		else	\
			pinst->v_OUT = FALSE;	\
	break;
	
	
/******************* gleich ******************/

#define STDFB_EQ(idcap, idsmall)	\
	case OV_VT_##idcap:	\
		if(pinst->v_IN1.value.valueunion.val_##idsmall == pinst->v_IN2.value.valueunion.val_##idsmall)	\
			pinst->v_OUT = TRUE;	\
		else	\
			pinst->v_OUT = FALSE;	\
	break;
	
	
#define STDFB_EQ_TIME(tim1, tim2)	\
	((tim1.secs == tim2.secs) && (tim1.usecs == tim2.usecs))
	
#define STDFB_EQ_VEC(idcap, idsmall)	\
	case OV_VT_##idcap##_VEC:	\
		if(pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen == pinst->v_IN2.value.valueunion.val_##idsmall##_vec.veclen)	\
			pinst->v_OUT = TRUE;	\
		else	\
			pinst->v_OUT = FALSE;	\
	break;
	
	
/******************* kleiner / gleich ******************/

#define STDFB_LE(idcap, idsmall)	\
	case OV_VT_##idcap:	\
		if(pinst->v_IN1.value.valueunion.val_##idsmall <= pinst->v_IN2.value.valueunion.val_##idsmall)	\
			pinst->v_OUT = TRUE;	\
		else	\
			pinst->v_OUT = FALSE;	\
	break;
	
	
#define STDFB_LE_TIME(tim1, tim2)	\
	((tim1.secs < tim2.secs) || ((tim1.secs == tim2.secs) && (tim1.usecs <= tim2.usecs)))
	
#define STDFB_LE_VEC(idcap, idsmall)	\
	case OV_VT_##idcap##_VEC:	\
		if(pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen <= pinst->v_IN2.value.valueunion.val_##idsmall##_vec.veclen)	\
			pinst->v_OUT = TRUE;	\
		else	\
			pinst->v_OUT = FALSE;	\
	break;

/******************* kleiner als ******************/

#define STDFB_LT(idcap, idsmall)	\
	case OV_VT_##idcap:	\
		if(pinst->v_IN1.value.valueunion.val_##idsmall < pinst->v_IN2.value.valueunion.val_##idsmall)	\
			pinst->v_OUT = TRUE;	\
		else	\
			pinst->v_OUT = FALSE;	\
	break;
	
	
#define STDFB_LT_TIME(tim1, tim2)	\
	((tim1.secs < tim2.secs) || ((tim1.secs == tim2.secs) && (tim1.usecs < tim2.usecs)))
	
#define STDFB_LT_VEC(idcap, idsmall)	\
	case OV_VT_##idcap##_VEC:	\
		if(pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen < pinst->v_IN2.value.valueunion.val_##idsmall##_vec.veclen)	\
			pinst->v_OUT = TRUE;	\
		else	\
			pinst->v_OUT = FALSE;	\
	break;
	
	
	
/******************* ungleich ******************/

#define STDFB_NE(idcap, idsmall)	\
	case OV_VT_##idcap:	\
		if(pinst->v_IN1.value.valueunion.val_##idsmall != pinst->v_IN2.value.valueunion.val_##idsmall)	\
			pinst->v_OUT = TRUE;	\
		else	\
			pinst->v_OUT = FALSE;	\
	break;
	
	
#define STDFB_NE_TIME(tim1, tim2)	\
	((tim1.secs != tim2.secs) || (tim1.usecs != tim2.usecs))
	
#define STDFB_NE_VEC(idcap, idsmall)	\
	case OV_VT_##idcap##_VEC:	\
		if(pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen != pinst->v_IN2.value.valueunion.val_##idsmall##_vec.veclen)	\
			pinst->v_OUT = TRUE;	\
		else	\
			pinst->v_OUT = FALSE;	\
	break;
	
	
