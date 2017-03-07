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
*	stdfb_bitwise_macros.h
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


/**********************logisches UND von vektorelementen *************/

#define STDFB_VEC_AND(idcap, idsmall) \
	pinst->v_OUT.value.vartype = OV_VT_##idcap##_VEC;	\
	if(pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen == pinst->v_IN2.value.valueunion.val_##idsmall##_vec.veclen)	\
	{	\
		if(Ov_OK(Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_##idsmall##_vec, pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen, idcap)))	\
		{	\
			for(i=0; i<pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen; i++)	\
			{	\
				pinst->v_OUT.value.valueunion.val_##idsmall##_vec.value[i] = 	\
					pinst->v_IN1.value.valueunion.val_##idsmall##_vec.value[i] & pinst->v_IN2.value.valueunion.val_##idsmall##_vec.value[i];	\
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
	
	
/**********************logisches ODER von vektorelementen *************/

#define STDFB_VEC_OR(idcap, idsmall) \
	pinst->v_OUT.value.vartype = OV_VT_##idcap##_VEC;	\
	if(pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen == pinst->v_IN2.value.valueunion.val_##idsmall##_vec.veclen)	\
	{	\
		if(Ov_OK(Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_##idsmall##_vec, pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen, idcap)))	\
		{	\
			for(i=0; i<pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen; i++)	\
			{	\
				pinst->v_OUT.value.valueunion.val_##idsmall##_vec.value[i] = 	\
					pinst->v_IN1.value.valueunion.val_##idsmall##_vec.value[i] | pinst->v_IN2.value.valueunion.val_##idsmall##_vec.value[i];	\
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
	
/**********************logische X-ODER von vektorelementen *************/

#define STDFB_VEC_XOR(idcap, idsmall) \
	pinst->v_OUT.value.vartype = OV_VT_##idcap##_VEC;	\
	if(pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen == pinst->v_IN2.value.valueunion.val_##idsmall##_vec.veclen)	\
	{	\
		if(Ov_OK(Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_##idsmall##_vec, pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen, idcap)))	\
		{	\
			for(i=0; i<pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen; i++)	\
			{	\
				pinst->v_OUT.value.valueunion.val_##idsmall##_vec.value[i] = 	\
					pinst->v_IN1.value.valueunion.val_##idsmall##_vec.value[i] ^ pinst->v_IN2.value.valueunion.val_##idsmall##_vec.value[i];	\
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
	