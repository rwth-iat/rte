
/******************************************************************************
*
*   FILE
*   ----
*   REPLACE.c
*
*   History
*   -------
*   2010-11-30   File created
*
*******************************************************************************
*
*   This file is generated by the 'fb_builder' command
*
******************************************************************************/


#ifndef OV_COMPILE_LIBRARY_iec61131stdfb
#define OV_COMPILE_LIBRARY_iec61131stdfb
#endif


#include "iec61131stdfb.h"
#include "libov/ov_macros.h"
#include "libov/ov_logfile.h"
#include "libov/ov_malloc.h"
#include "libov/ov_string.h"


OV_DLLFNCEXPORT OV_RESULT iec61131stdfb_REPLACE_IN1_set(
    OV_INSTPTR_iec61131stdfb_REPLACE          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_IN1,value);
}

OV_DLLFNCEXPORT OV_RESULT iec61131stdfb_REPLACE_IN2_set(
    OV_INSTPTR_iec61131stdfb_REPLACE          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_IN2,value);
}

OV_DLLFNCEXPORT OV_RESULT iec61131stdfb_REPLACE_L_set(
    OV_INSTPTR_iec61131stdfb_REPLACE          pobj,
    const OV_UINT  value
) {
    pobj->v_L = value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT iec61131stdfb_REPLACE_P_set(
    OV_INSTPTR_iec61131stdfb_REPLACE          pobj,
    const OV_UINT  value
) {
    pobj->v_P = value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_STRING iec61131stdfb_REPLACE_OUT_get(
    OV_INSTPTR_iec61131stdfb_REPLACE          pobj
) {
    return pobj->v_OUT;
}


OV_DLLFNCEXPORT void iec61131stdfb_REPLACE_shutdown(OV_INSTPTR_ov_object pobj) {

	OV_INSTPTR_iec61131stdfb_REPLACE pinst = Ov_StaticPtrCast(iec61131stdfb_REPLACE, pobj);
	
	ov_string_setvalue(&pinst->v_IN1, "");
	ov_string_setvalue(&pinst->v_IN2, "");
	ov_string_setvalue(&pinst->v_OUT, "");
	ov_object_shutdown(pobj);
}


OV_DLLFNCEXPORT void iec61131stdfb_REPLACE_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
    /*    
    *   local variables
    */
	
	unsigned int i;
	unsigned int in_length, end_length;
	
	OV_STRING p_begin;
	OV_STRING p_end;
	
    OV_INSTPTR_iec61131stdfb_REPLACE pinst = Ov_StaticPtrCast(iec61131stdfb_REPLACE, pfb);
	
	ov_string_setvalue(&pinst->v_OUT, "");
	in_length = ov_string_getlength(pinst->v_IN1);
	
	if(((pinst->v_P <= in_length) && (pinst->v_P > 0)))
	{
		if(pinst->v_L > 0)
		{
			if((pinst->v_P + pinst->v_L - 1) < in_length)		//without this if, negative end_lengths would result in random values
				end_length = in_length - (pinst->v_P + pinst->v_L - 1); //overlap position / number 	
			else
				end_length = 0;
				
			if(pinst->v_P > 1)
			{
				p_begin = (OV_STRING) ov_malloc((pinst->v_P) * sizeof(char));
				if(p_begin == NULL)	//if allocation failed, free memory
				{
					ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);
					ov_free(p_begin);
					return;
				}
				for(i=0; i < pinst->v_P - 1; i++)
					p_begin[i] = pinst->v_IN1[i];
				p_begin[i] = 0;
				
				if(Ov_Fail(ov_string_setvalue(&pinst->v_OUT, p_begin)))
				{
					ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);
					ov_free(p_begin);
					return;
				}
				
				ov_free(p_begin);
			}
			
			
			if(Ov_Fail(ov_string_append(&pinst->v_OUT, pinst->v_IN2)))
			{
				if(ov_string_getlength(pinst->v_IN2) > 0)
				{		
					ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);
					return;
				}
			}
				
			if(end_length > 0)
			{
				p_end = (OV_STRING) ov_malloc((end_length + 1) * sizeof(char));
				if(p_end == NULL)	//if allocation failed, free memory
				{
					ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);
					ov_free(p_end);
					return;
				}
				
				for(i=0; i < end_length; i++)
					p_end[i] = pinst->v_IN1[i + pinst->v_P + pinst->v_L - 1];
				p_end[i] = 0;
				
				if(Ov_Fail(ov_string_append(&pinst->v_OUT, p_end)))
				{
					ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);
					ov_free(p_end);
					return;
				}
				ov_free(p_end);
			}
			else
				ov_logfile_warning("%s: requested deletion over the end of inpust string", pinst->v_identifier);
		}
		else
		{
			end_length = in_length - pinst->v_P + 1;  	
			
				
			if(pinst->v_P > 1)
			{
				p_begin = (OV_STRING) ov_malloc((pinst->v_P) * sizeof(char));
				if(p_begin == NULL)	//if allocation failed, free memory
				{
					ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);
					ov_free(p_begin);
					return;
				}
				for(i=0; i < pinst->v_P - 1; i++)
					p_begin[i] = pinst->v_IN1[i];
				p_begin[i] = 0;
				
				if(Ov_Fail(ov_string_setvalue(&pinst->v_OUT, p_begin)))
				{
					ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);
					ov_free(p_begin);
					return;
				}
				
				ov_free(p_begin);
			}
			
			if(Ov_Fail(ov_string_append(&pinst->v_OUT, pinst->v_IN2)))
			{
				if(ov_string_getlength(pinst->v_IN2) > 0)
				{
					ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);
					return;
				}
			}
			
			if(end_length > 0)
			{
				p_end = (OV_STRING) ov_malloc((end_length + 1) * sizeof(char));
				if(p_end == NULL)	//if allocation failed, free memory
				{
					ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);
					ov_free(p_end);
					return;
				}
				
				for(i=0; i < end_length; i++)
					p_end[i] = pinst->v_IN1[i + pinst->v_P - 1];
				p_end[i] = 0;
				
				if(Ov_Fail(ov_string_append(&pinst->v_OUT, p_end)))
				{
					ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);
					ov_free(p_end);
					return;
				}
				ov_free(p_end);
			}
		}
	}
	else
		ov_logfile_error("%s: specified position outside input string", pinst->v_identifier);

	
    return;
}

