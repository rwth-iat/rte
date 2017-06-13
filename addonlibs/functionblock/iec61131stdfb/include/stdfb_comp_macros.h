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
	
	
