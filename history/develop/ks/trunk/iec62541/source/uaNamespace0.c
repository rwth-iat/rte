
/******************************************************************************
*
*   FILE
*   ----
*   uaNamespace0.c
*
*   History
*   -------
*   2014-10-24   File created
*
*******************************************************************************
*
*   This file is generated by the 'acplt_builder' command
*
******************************************************************************/


#ifndef OV_COMPILE_LIBRARY_iec62541
#define OV_COMPILE_LIBRARY_iec62541
#endif


#include "iec62541.h"
#include "libov/ov_macros.h"


OV_DLLFNCEXPORT OV_RESULT iec62541_uaNamespace0_constructor(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
    OV_INSTPTR_iec62541_uaNamespace0 pinst = Ov_StaticPtrCast(iec62541_uaNamespace0, pobj);
    OV_RESULT    result;

    /* do what the base class does first */
    result = ov_object_constructor(pobj);
    if(Ov_Fail(result))
         return result;

    /* do what */


    return OV_ERR_OK;
}

OV_DLLFNCEXPORT void iec62541_uaNamespace0_startup(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
    OV_INSTPTR_iec62541_uaNamespace0 pinst = Ov_StaticPtrCast(iec62541_uaNamespace0, pobj);

    /* do what the base class does first */
    ov_object_startup(pobj);

    /* do what */


    return;
}


