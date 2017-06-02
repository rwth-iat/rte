
/******************************************************************************
*
*   FILE
*   ----
*   Diagnosis.c
*
*   History
*   -------
*   2017-06-01   File created
*
*******************************************************************************
*
*   This file is generated by the 'acplt_builder' command
*
******************************************************************************/


#ifndef OV_COMPILE_LIBRARY_openaasExample
#define OV_COMPILE_LIBRARY_openaasExample
#endif


#include "openaasExample.h"
#include "libov/ov_macros.h"


OV_DLLFNCEXPORT OV_RESULT openaasExample_Diagnosis_CallMethod(      
  OV_INSTPTR_openaas_Service pobj,       
  OV_UINT numberofInputArgs,       
  const void **packedInputArgList,       
  OV_UINT numberofOutputArgs,      
  void **packedOutputArgList,
  OV_UINT *typeArray       
) {
    /*    
    *   local variables
    */

	OV_INSTPTR_openaasExample_Diagnosis pinst = Ov_StaticPtrCast(openaasExample_Diagnosis, pobj);

	pinst->p_Timer.v_Released = TRUE;
	pinst->p_Timer.v_Counting = FALSE;

	packedOutputArgList[0] = ov_database_malloc(sizeof(OV_STRING));
	*(OV_STRING*)packedOutputArgList[5] = NULL;

	if (ov_string_compare(*(OV_STRING*)(packedInputArgList[0]), "FULL") == OV_STRCMP_EQUAL){
		pinst->p_GPIOOut.v_pin = 23;
		pinst->p_Timer.v_IN = TRUE;
		*(OV_STRING*)packedOutputArgList[0] = ov_database_malloc(ov_string_getlength("OK")+1);
		strcpy(*(OV_STRING*)packedOutputArgList[0], "OK");
		typeArray[0] = OV_VT_STRING;
	}else if (ov_string_compare(*(OV_STRING*)(packedInputArgList[0]), "FAST") == OV_STRCMP_EQUAL){
		pinst->p_GPIOOut.v_pin = 24;
		pinst->p_Timer.v_IN = TRUE;
		*(OV_STRING*)packedOutputArgList[0] = ov_database_malloc(ov_string_getlength("OK")+1);
		strcpy(*(OV_STRING*)packedOutputArgList[0], "OK");
		typeArray[0] = OV_VT_STRING;
	}else{
		pinst->p_Timer.v_IN = FALSE;
		*(OV_STRING*)packedOutputArgList[0] = ov_database_malloc(ov_string_getlength("FAILED")+1);
		strcpy(*(OV_STRING*)packedOutputArgList[0], "FAILED");
		typeArray[0] = OV_VT_STRING;
	}


    return OV_ERR_OK;
}


OV_DLLFNCEXPORT OV_RESULT openaasExample_Diagnosis_constructor(
	OV_INSTPTR_ov_object 	pobj
) {
    /*
    *   local variables
    */
    OV_INSTPTR_openaasExample_Diagnosis pinst = Ov_StaticPtrCast(openaasExample_Diagnosis, pobj);
    OV_RESULT    result;

    /* do what the base class does first */
    result = ov_object_constructor(pobj);
    if(Ov_Fail(result))
         return result;

    /* do what */


    return OV_ERR_OK;
}