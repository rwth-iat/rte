
/******************************************************************************
*
*   FILE
*   ----
*   SUB.c
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
#include "libov/ov_string.h"

#include <math.h>

OV_DLLFNCEXPORT OV_RESULT iec61131stdfb_SUB_IN1_set(
    OV_INSTPTR_iec61131stdfb_SUB          pobj,
    const OV_ANY*  value
) {
    return ov_variable_setanyvalue(&pobj->v_IN1, value);
}

OV_DLLFNCEXPORT OV_RESULT iec61131stdfb_SUB_IN2_set(
    OV_INSTPTR_iec61131stdfb_SUB          pobj,
    const OV_ANY*  value
) {
    return ov_variable_setanyvalue(&pobj->v_IN2, value);
}

OV_DLLFNCEXPORT OV_ANY* iec61131stdfb_SUB_OUT_get(
    OV_INSTPTR_iec61131stdfb_SUB          pobj
) {
    return &pobj->v_OUT;
}


OV_DLLFNCEXPORT void iec61131stdfb_SUB_shutdown(OV_INSTPTR_ov_object pobj) {

	unsigned int i;
	
	OV_INSTPTR_iec61131stdfb_SUB pinst = Ov_StaticPtrCast(iec61131stdfb_SUB, pobj);
	
	STDFB_FREE_VEC(pinst->v_IN1);
	STDFB_FREE_VEC(pinst->v_IN2);
	STDFB_FREE_VEC(pinst->v_OUT);
	ov_object_shutdown(pobj);
}


OV_DLLFNCEXPORT void iec61131stdfb_SUB_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
    /*    
    *   local variables
    */
	unsigned int i;
	
    OV_INSTPTR_iec61131stdfb_SUB pinst = Ov_StaticPtrCast(iec61131stdfb_SUB, pfb);
	if((pinst->v_IN1.value.vartype & OV_VT_KSMASK) == (pinst->v_IN2.value.vartype & OV_VT_KSMASK))
	{
		
		STDFB_FREE_VEC(pinst->v_OUT);		//free memory of preexisting out-vector
		if(!(pinst->v_IN1.value.vartype & OV_VT_ISVECTOR))
		{
			switch(pinst->v_IN1.value.vartype & OV_VT_KSMASK)
			{
				
				case OV_VT_INT:
					pinst->v_OUT.value.vartype = OV_VT_INT;
					pinst->v_OUT.value.valueunion.val_int = pinst->v_IN1.value.valueunion.val_int - pinst->v_IN2.value.valueunion.val_int;
				break;
				
				case OV_VT_UINT:
					pinst->v_OUT.value.vartype = OV_VT_UINT;
					pinst->v_OUT.value.valueunion.val_uint = pinst->v_IN1.value.valueunion.val_uint - pinst->v_IN2.value.valueunion.val_uint;
				break;
				
				case OV_VT_SINGLE:
					pinst->v_OUT.value.vartype = OV_VT_SINGLE;
					pinst->v_OUT.value.valueunion.val_single = pinst->v_IN1.value.valueunion.val_single - pinst->v_IN2.value.valueunion.val_single;
					if((pinst->v_OUT.value.valueunion.val_single == HUGE_VAL) || (pinst->v_OUT.value.valueunion.val_single == -HUGE_VAL))
					{
						ov_logfile_error("%s: result exceeds range of single", pinst->v_identifier);
						pinst->v_OUT.value.valueunion.val_single = 0;
					}
				break;
				
				case OV_VT_DOUBLE:
					pinst->v_OUT.value.vartype = OV_VT_DOUBLE;
					pinst->v_OUT.value.valueunion.val_double = pinst->v_IN1.value.valueunion.val_double - pinst->v_IN2.value.valueunion.val_double;
					if((pinst->v_OUT.value.valueunion.val_double == HUGE_VAL) || (pinst->v_OUT.value.valueunion.val_double == -HUGE_VAL))
					{
						ov_logfile_error("%s: result exceeds range of double", pinst->v_identifier);
						pinst->v_OUT.value.valueunion.val_double = 0;
					}
				break;

				case OV_VT_BYTE:
					pinst->v_OUT.value.vartype = OV_VT_BYTE;
					pinst->v_OUT.value.valueunion.val_byte = pinst->v_IN1.value.valueunion.val_byte - pinst->v_IN2.value.valueunion.val_byte;
					ov_logfile_warning("%s: subtraction of bitstring", pinst->v_identifier);
				break;
				
				case OV_VT_BOOL:
					pinst->v_OUT.value.vartype = OV_VT_BOOL;
					pinst->v_OUT.value.valueunion.val_bool = pinst->v_IN1.value.valueunion.val_bool ^ pinst->v_IN2.value.valueunion.val_bool;
					ov_logfile_warning("%s: subtraction of boolean, applying logical XOR", pinst->v_identifier);
				break;
						
				
				default:
					pinst->v_OUT.value.vartype = OV_VT_BOOL;
					pinst->v_OUT.value.valueunion.val_bool = FALSE;
					ov_logfile_alert("%s: subtraction of given datatypes senseless", pinst->v_identifier);
				break;
			}
		}
		else
		{
			
			switch(pinst->v_IN1.value.vartype & OV_VT_KSMASK)
			{
				
				case OV_VT_INT_VEC:
					STDFB_VEC_SUB(INT, int);
				break;
				
				case OV_VT_UINT_VEC:
					STDFB_VEC_SUB(UINT, uint);
				break;
				
				case OV_VT_SINGLE_VEC:
					STDFB_VEC_SUB_F(SINGLE, single);
				break;
				
				case OV_VT_DOUBLE_VEC:
					STDFB_VEC_SUB_F(DOUBLE, double);
				break;

				case OV_VT_BYTE_VEC:
					ov_logfile_warning("%s: subtraction of bitstrings requested", pinst->v_identifier);
					STDFB_VEC_SUB(BYTE, byte);
				break;
				
				case OV_VT_BOOL_VEC:
					pinst->v_OUT.value.vartype = OV_VT_BOOL_VEC;
					ov_logfile_warning("%s: subtraction of boolean, applying logical XOR", pinst->v_identifier);
					if(pinst->v_IN1.value.valueunion.val_bool_vec.veclen == pinst->v_IN2.value.valueunion.val_bool_vec.veclen)
					{
						if(Ov_OK(Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_bool_vec, pinst->v_IN1.value.valueunion.val_bool_vec.veclen, BOOL)))
						{
							for(i=0; i<pinst->v_IN1.value.valueunion.val_bool_vec.veclen; i++)
							{
								pinst->v_OUT.value.valueunion.val_bool_vec.value[i] = pinst->v_IN1.value.valueunion.val_bool_vec.value[i] ^ 
									pinst->v_IN2.value.valueunion.val_bool_vec.value[i];
							}
						}
						else
						{
							ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);
							return;
						}
					}
					else
					{
						ov_logfile_error("%s: vectors have different lengths, operation not possible", pinst->v_identifier);
						Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_bool_vec, 0, BOOL);
					}
					
				break;
						
				default:
					pinst->v_OUT.value.vartype = OV_VT_BOOL;
					pinst->v_OUT.value.valueunion.val_bool = FALSE;
					ov_logfile_alert("%s: subtraction of given datatypes senseless", pinst->v_identifier);
				break;
			}
		}
		
		/************** handling states and timestamps ********************************/
		
#include "state_2in.c"	
	
	}
	else
	{
		pinst->v_OUT.value.vartype = OV_VT_BOOL;
		pinst->v_OUT.value.valueunion.val_bool = FALSE;
		STDFB_FREE_VEC(pinst->v_OUT);
		ov_logfile_error("%s: trying to use inputs of different types for SUB-block", pinst->v_identifier); 
	}
	
    return;
}

