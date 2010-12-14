
/******************************************************************************
*
*   FILE
*   ----
*   LIMIT.c
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
#include "stdfb_macros.h"
#include "libov/ov_macros.h"
#include "libov/ov_logfile.h"

#define OV_DEBUG


OV_DLLFNCEXPORT OV_RESULT iec61131stdfb_LIMIT_MN_set(
    OV_INSTPTR_iec61131stdfb_LIMIT          pobj,
    const OV_ANY*  value
) {
    return ov_variable_setanyvalue(&pobj->v_MN, value);
}

OV_DLLFNCEXPORT OV_RESULT iec61131stdfb_LIMIT_IN_set(
    OV_INSTPTR_iec61131stdfb_LIMIT          pobj,
    const OV_ANY*  value
) {
    return ov_variable_setanyvalue(&pobj->v_IN, value);
}

OV_DLLFNCEXPORT OV_RESULT iec61131stdfb_LIMIT_MX_set(
    OV_INSTPTR_iec61131stdfb_LIMIT          pobj,
    const OV_ANY*  value
) {
    return ov_variable_setanyvalue(&pobj->v_MX, value);
}

OV_DLLFNCEXPORT OV_ANY* iec61131stdfb_LIMIT_OUT_get(
    OV_INSTPTR_iec61131stdfb_LIMIT          pobj
) {
    return &pobj->v_OUT;
}


OV_DLLFNCEXPORT void iec61131stdfb_LIMIT_shutdown(OV_INSTPTR_ov_object pobj) {

	unsigned int i;
	
	OV_INSTPTR_iec61131stdfb_LIMIT pinst = Ov_StaticPtrCast(iec61131stdfb_LIMIT, pobj);
	
	STDFB_FREE_VEC(pinst->v_IN);
	STDFB_FREE_VEC(pinst->v_MN);
	STDFB_FREE_VEC(pinst->v_MX);
	STDFB_FREE_VEC(pinst->v_OUT);
	ov_object_shutdown(pobj);
}


OV_DLLFNCEXPORT void iec61131stdfb_LIMIT_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
    /*    
    *   local variables
    */
	
	unsigned int i;
	
    OV_INSTPTR_iec61131stdfb_LIMIT pinst = Ov_StaticPtrCast(iec61131stdfb_LIMIT, pfb);
	
	STDFB_FREE_VEC(pinst->v_OUT);
	
	if(!(pinst->v_IN.value.vartype & OV_VT_ISVECTOR))	//handling scalars
	{
		if(((pinst->v_IN.value.vartype & OV_VT_KSMASK) == (pinst->v_MN.value.vartype & OV_VT_KSMASK))
			&& ((pinst->v_IN.value.vartype & OV_VT_KSMASK) == (pinst->v_MX.value.vartype & OV_VT_KSMASK)))
		{
			switch(pinst->v_IN.value.vartype & OV_VT_KSMASK)
			{
				STDFB_LIMIT(BOOL, bool);
				STDFB_LIMIT(BYTE, byte);
				STDFB_LIMIT(UINT, uint);
				STDFB_LIMIT(INT, int);
				STDFB_LIMIT(SINGLE, single);
				STDFB_LIMIT(DOUBLE, double);
				
				case OV_VT_TIME:
					if(ov_time_compare(&pinst->v_MN.value.valueunion.val_time, &pinst->v_MX.value.valueunion.val_time) <= 0)
					{
						pinst->v_OUT.value.vartype = OV_VT_TIME;
						
						if(ov_time_compare(&pinst->v_IN.value.valueunion.val_time, &pinst->v_MX.value.valueunion.val_time) > 0)
							pinst->v_OUT.value.valueunion.val_time = pinst->v_MX.value.valueunion.val_time;
						else
							if(ov_time_compare(&pinst->v_IN.value.valueunion.val_time, &pinst->v_MN.value.valueunion.val_time) < 0)
								pinst->v_OUT.value.valueunion.val_time = pinst->v_MN.value.valueunion.val_time;
							else
								pinst->v_OUT.value.valueunion.val_time = pinst->v_IN.value.valueunion.val_time;
					}
					else
					{
						ov_logfile_error("%s: minimum is larger than maximum", pinst->v_identifier);
						pinst->v_OUT.value.vartype = OV_VT_BOOL;
						pinst->v_OUT.value.valueunion.val_bool = 0;
						return;
					}
				break;
				
				case OV_VT_TIME_SPAN:
					if(STDFB_LE_TIME_SPAN(pinst->v_MN.value.valueunion.val_time_span, pinst->v_MX.value.valueunion.val_time_span))
					{
						pinst->v_OUT.value.vartype = OV_VT_TIME_SPAN;
						
						if(STDFB_LE_TIME_SPAN(pinst->v_IN.value.valueunion.val_time_span, pinst->v_MN.value.valueunion.val_time_span))
							pinst->v_OUT.value.valueunion.val_time_span = pinst->v_MN.value.valueunion.val_time_span;
						else
							if(STDFB_LE_TIME_SPAN(pinst->v_MX.value.valueunion.val_time_span, pinst->v_IN.value.valueunion.val_time_span))
								pinst->v_OUT.value.valueunion.val_time_span = pinst->v_MX.value.valueunion.val_time_span;
							else
								pinst->v_OUT.value.valueunion.val_time_span = pinst->v_IN.value.valueunion.val_time_span;
					}
					else
					{
						ov_logfile_error("%s: minimum is larger than maximum", pinst->v_identifier);
						pinst->v_OUT.value.vartype = OV_VT_BOOL;
						pinst->v_OUT.value.valueunion.val_bool = 0;
						return;
					}
				break;
				
				case OV_VT_STRING:
					if(ov_string_compare(pinst->v_MN.value.valueunion.val_string, pinst->v_MX.value.valueunion.val_string) <= 0)
					{
						pinst->v_OUT.value.vartype = OV_VT_STRING;
						if(ov_string_compare(pinst->v_IN.value.valueunion.val_string, pinst->v_MN.value.valueunion.val_string) < 0)
							if(Ov_Fail(ov_string_setvalue(&pinst->v_OUT.value.valueunion.val_string, pinst->v_MN.value.valueunion.val_string)))
							{
								ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);
								return;
							}
							else
								;
						else
							if(ov_string_compare(pinst->v_IN.value.valueunion.val_string, pinst->v_MX.value.valueunion.val_string) > 0)
								if(Ov_Fail(ov_string_setvalue(&pinst->v_OUT.value.valueunion.val_string, pinst->v_MX.value.valueunion.val_string)))
								{
									ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);
									return;
								}
								else
									;
							else
								if(Ov_Fail(ov_string_setvalue(&pinst->v_OUT.value.valueunion.val_string, pinst->v_IN.value.valueunion.val_string)))
								{
									ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);
									return;
								}
					}
					else
					{
						ov_logfile_error("%s: minimum is larger than maximum", pinst->v_identifier);
						pinst->v_OUT.value.vartype = OV_VT_BOOL;
						pinst->v_OUT.value.valueunion.val_bool = 0;
						return;
					}
				break;
				
				default:
					ov_logfile_error("%s: limiting of given type not implemented", pinst->v_identifier);
					pinst->v_OUT.value.vartype = OV_VT_BOOL;
					pinst->v_OUT.value.valueunion.val_bool = 0;
					return;
				
			}
		}
		else
		{
			ov_logfile_error("%s: trying to use inputs of different types on LIMIT-block", pinst->v_identifier);
			pinst->v_OUT.value.vartype = OV_VT_BOOL;
			pinst->v_OUT.value.valueunion.val_bool = 0;
			return;
		}
	}
	else			//handling vectors
	{
		if((pinst->v_MN.value.vartype & OV_VT_KSMASK) == (pinst->v_MX.value.vartype & OV_VT_KSMASK))
		{
			if((((pinst->v_IN.value.vartype & (~OV_VT_ISVECTOR)) & OV_VT_KSMASK) == (pinst->v_MN.value.vartype & OV_VT_KSMASK))	
				|| ((pinst->v_IN.value.vartype & OV_VT_KSMASK) == (pinst->v_MN.value.vartype & OV_VT_KSMASK)))	//scalars of same type also allowed as MN / MX
			{
				switch(pinst->v_IN.value.vartype & OV_VT_KSMASK)	//no test if min is smaller than max!!
				{
					STDFB_LIMIT_VEC(BOOL, bool);
					STDFB_LIMIT_VEC(BYTE, byte);
					STDFB_LIMIT_VEC(UINT, uint);
					STDFB_LIMIT_VEC(INT, int);
					STDFB_LIMIT_VEC(SINGLE, single);
					STDFB_LIMIT_VEC(DOUBLE, double);
					
					case OV_VT_STRING_VEC:
						pinst->v_OUT.value.vartype = OV_VT_STRING_VEC;
						if((pinst->v_MN.value.vartype & OV_VT_ISVECTOR))
						{	
							if(Ov_Fail(Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_string_vec, pinst->v_IN.value.valueunion.val_string_vec.veclen, STRING)))
							{
								ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);
								return;
							}
							for(i=0; i < pinst->v_IN.value.valueunion.val_string_vec.veclen; i++)
							{
								if(i < pinst->v_MN.value.valueunion.val_string_vec.veclen)
								{
									if(ov_string_compare(pinst->v_IN.value.valueunion.val_string_vec.value[i], pinst->v_MN.value.valueunion.val_string_vec.value[i]) < 0)
									{
										if(Ov_Fail(ov_string_setvalue(&pinst->v_OUT.value.valueunion.val_string_vec.value[i], pinst->v_MN.value.valueunion.val_string_vec.value[i])))
										{
											ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);
											return;
										}
										continue;
									}
								}
								else
									if(ov_string_compare(pinst->v_IN.value.valueunion.val_string_vec.value[i], "") < 0)
									{
										ov_string_setvalue(&pinst->v_OUT.value.valueunion.val_string_vec.value[i], "");
										continue;
									}
								if(i < pinst->v_MX.value.valueunion.val_string_vec.veclen)
								{
									if(ov_string_compare(pinst->v_IN.value.valueunion.val_string_vec.value[i], pinst->v_MX.value.valueunion.val_string_vec.value[i]) > 0)
									{
										if(Ov_Fail(ov_string_setvalue(&pinst->v_OUT.value.valueunion.val_string_vec.value[i], pinst->v_MX.value.valueunion.val_string_vec.value[i])))
										{
											ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);
											return;
										}
										continue;
									}
								}
								else
									if(ov_string_compare(pinst->v_IN.value.valueunion.val_string_vec.value[i], "") > 0)
									{
										ov_string_setvalue(&pinst->v_OUT.value.valueunion.val_string_vec.value[i], "");
										continue;
									}
								if(Ov_Fail(ov_string_setvalue(&pinst->v_OUT.value.valueunion.val_string_vec.value[i], pinst->v_IN.value.valueunion.val_string_vec.value[i])))
								{
									ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);
									return;
								}
							}
						}
						else
						{
							if(Ov_Fail(Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_string_vec, pinst->v_IN.value.valueunion.val_string_vec.veclen, STRING)))
							{
								ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);
							return;
							}
							for(i=0; i < pinst->v_IN.value.valueunion.val_string_vec.veclen; i++)
							{
								if(ov_string_compare(pinst->v_IN.value.valueunion.val_string_vec.value[i], pinst->v_MN.value.valueunion.val_string) < 0)
									if(Ov_Fail(ov_string_setvalue(&pinst->v_OUT.value.valueunion.val_string_vec.value[i], pinst->v_MN.value.valueunion.val_string)))
									{
										ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);
										return;
									}
								else
									if(ov_string_compare(pinst->v_IN.value.valueunion.val_string_vec.value[i], pinst->v_MX.value.valueunion.val_string) > 0)
										if(Ov_Fail(ov_string_setvalue(&pinst->v_OUT.value.valueunion.val_string_vec.value[i], pinst->v_MX.value.valueunion.val_string)))
										{
											ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);
											return;
										}
									else
										if(Ov_Fail(ov_string_setvalue(&pinst->v_OUT.value.valueunion.val_string_vec.value[i], pinst->v_IN.value.valueunion.val_string_vec.value[i])))
										{
											ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);
											return;
										}
							}
						}
					break;
					
					case OV_VT_TIME_VEC:
						pinst->v_OUT.value.vartype = OV_VT_TIME_VEC;
						if((pinst->v_MN.value.vartype & OV_VT_ISVECTOR))
						{
							if(Ov_Fail(Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_time_vec, pinst->v_IN.value.valueunion.val_time_vec.veclen, TIME)))
							{
								ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);
							return;
							}
							for(i=0; i < pinst->v_IN.value.valueunion.val_time_vec.veclen; i++)
							{
								if(i < pinst->v_MN.value.valueunion.val_time_vec.veclen)
								{
									if(ov_time_compare(&pinst->v_IN.value.valueunion.val_time_vec.value[i], &pinst->v_MN.value.valueunion.val_time_vec.value[i]) < 0)
									{
										pinst->v_OUT.value.valueunion.val_time_vec.value[i] = pinst->v_MN.value.valueunion.val_time_vec.value[i];
										continue;
									}
								}
								else
									if((pinst->v_IN.value.valueunion.val_time_vec.value[i].secs < 0) || (pinst->v_IN.value.valueunion.val_time_vec.value[i].usecs < 0))
									{
										pinst->v_OUT.value.valueunion.val_time_vec.value[i].secs = 0;
										pinst->v_OUT.value.valueunion.val_time_vec.value[i].usecs = 0;
										continue;
									}
								if(i < pinst->v_MX.value.valueunion.val_time_vec.veclen)
								{
									if(ov_time_compare(&pinst->v_IN.value.valueunion.val_time_vec.value[i], &pinst->v_MX.value.valueunion.val_time_vec.value[i]) > 0)
									{
										pinst->v_OUT.value.valueunion.val_time_vec.value[i] = pinst->v_MX.value.valueunion.val_time_vec.value[i];
										continue;
									}
								}
								else
									if((pinst->v_IN.value.valueunion.val_time_vec.value[i].secs > 0) || (pinst->v_IN.value.valueunion.val_time_vec.value[i].usecs > 0))
									{
										pinst->v_OUT.value.valueunion.val_time_vec.value[i].secs = 0;
										pinst->v_OUT.value.valueunion.val_time_vec.value[i].usecs = 0;
										continue;
									}
								pinst->v_OUT.value.valueunion.val_time_vec.value[i] = pinst->v_IN.value.valueunion.val_time_vec.value[i];
							}
						}
						else
						{
							if(Ov_Fail(Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_time_vec, pinst->v_IN.value.valueunion.val_time_vec.veclen, TIME)))
							{
								ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);
							return;
							}
							for(i=0; i < pinst->v_IN.value.valueunion.val_time_vec.veclen; i++)
							{
								if(ov_time_compare(&pinst->v_IN.value.valueunion.val_time_vec.value[i], &pinst->v_MN.value.valueunion.val_time) < 0)
									pinst->v_OUT.value.valueunion.val_time_vec.value[i] = pinst->v_MN.value.valueunion.val_time;
								else
									if(ov_time_compare(&pinst->v_IN.value.valueunion.val_time_vec.value[i], &pinst->v_MX.value.valueunion.val_time) > 0)
										pinst->v_OUT.value.valueunion.val_time_vec.value[i] = pinst->v_MX.value.valueunion.val_time;
									else
										pinst->v_OUT.value.valueunion.val_time_vec.value[i] = pinst->v_IN.value.valueunion.val_time_vec.value[i];
							}
						}
					break;
					
					case OV_VT_TIME_SPAN_VEC:
						pinst->v_OUT.value.vartype = OV_VT_TIME_SPAN_VEC;
						if((pinst->v_MN.value.vartype & OV_VT_ISVECTOR))
						{
							if(Ov_Fail(Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_time_span_vec, pinst->v_IN.value.valueunion.val_time_span_vec.veclen, TIME_SPAN)))
							{
								ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);
							return;
							}
							for(i=0; i < pinst->v_IN.value.valueunion.val_time_span_vec.veclen; i++)
							{
								if(i < pinst->v_MN.value.valueunion.val_time_span_vec.veclen)
								{
									if(STDFB_LE_TIME_SPAN(pinst->v_IN.value.valueunion.val_time_span_vec.value[i], pinst->v_MN.value.valueunion.val_time_span_vec.value[i]))
									{
										pinst->v_OUT.value.valueunion.val_time_span_vec.value[i] = pinst->v_MN.value.valueunion.val_time_span_vec.value[i];
										continue;
									}
								}
								else
									if((pinst->v_IN.value.valueunion.val_time_span_vec.value[i].secs < 0) 
										|| ((pinst->v_IN.value.valueunion.val_time_span_vec.value[i].usecs < 0) && (pinst->v_IN.value.valueunion.val_time_span_vec.value[i].secs == 0)))
									{
										pinst->v_OUT.value.valueunion.val_time_span_vec.value[i].secs = 0;
										pinst->v_OUT.value.valueunion.val_time_span_vec.value[i].usecs = 0;
										continue;
									}
								if(i < pinst->v_MX.value.valueunion.val_time_span_vec.veclen)
								{
									if(STDFB_LE_TIME_SPAN(pinst->v_MX.value.valueunion.val_time_span_vec.value[i], pinst->v_IN.value.valueunion.val_time_span_vec.value[i]))
									{
										pinst->v_OUT.value.valueunion.val_time_span_vec.value[i] = pinst->v_MX.value.valueunion.val_time_span_vec.value[i];
										continue;
									}
								}
								else
									if(((pinst->v_IN.value.valueunion.val_time_span_vec.value[i].secs > 0) 
										|| ((pinst->v_IN.value.valueunion.val_time_span_vec.value[i].usecs > 0) && (pinst->v_IN.value.valueunion.val_time_span_vec.value[i].secs == 0))))
									{
										pinst->v_OUT.value.valueunion.val_time_span_vec.value[i].secs = 0;
										pinst->v_OUT.value.valueunion.val_time_span_vec.value[i].usecs = 0;
										continue;
									}
								pinst->v_OUT.value.valueunion.val_time_span_vec.value[i] = pinst->v_IN.value.valueunion.val_time_span_vec.value[i];
							}
						}
						else
						{
							if(Ov_Fail(Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_time_span_vec, pinst->v_IN.value.valueunion.val_time_span_vec.veclen, TIME_SPAN)))
							{
								ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);
							return;
							}
							for(i=0; i < pinst->v_IN.value.valueunion.val_time_span_vec.veclen; i++)
							{
								if(STDFB_LE_TIME_SPAN(pinst->v_IN.value.valueunion.val_time_span_vec.value[i], pinst->v_MN.value.valueunion.val_time_span))
									pinst->v_OUT.value.valueunion.val_time_span_vec.value[i] = pinst->v_MN.value.valueunion.val_time_span;
								else
									if(STDFB_LE_TIME_SPAN(pinst->v_MX.value.valueunion.val_time_span, pinst->v_IN.value.valueunion.val_time_span_vec.value[i]))
										pinst->v_OUT.value.valueunion.val_time_span_vec.value[i] = pinst->v_MX.value.valueunion.val_time_span;
									else
										pinst->v_OUT.value.valueunion.val_time_span_vec.value[i] = pinst->v_IN.value.valueunion.val_time_span_vec.value[i];
							}
						}
					break;
					
					default:
						ov_logfile_error("%s: limiting of given type not implemented", pinst->v_identifier);
						pinst->v_OUT.value.vartype = OV_VT_BOOL;
						pinst->v_OUT.value.valueunion.val_bool = 0;
						return;
				}
			}
			else
			{
				ov_logfile_error("%s: trying to use inputs of different types on LIMIT-block", pinst->v_identifier);
				pinst->v_OUT.value.vartype = OV_VT_BOOL;
				pinst->v_OUT.value.valueunion.val_bool = 0;
				return;
			}
		
			/************** handling states ********************************/
		
	STDFB_STATE(IN);
	
	/***************** handling timestamps ************************/
	STDFB_TIMESTAMP(IN);
		
		}
		else
		{
			ov_logfile_error("%s: trying to use inputs of different types for MN and MX on LIMIT-block", pinst->v_identifier);
			pinst->v_OUT.value.vartype = OV_VT_BOOL;
			pinst->v_OUT.value.valueunion.val_bool = 0;
			return;
		}
	}
	
    return;
}

