
/******************************************************************************
*
*   FILE
*   ----
*   uaClientHandler.c
*
*   History
*   -------
*   2014-10-13   File created
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


OV_DLLFNCEXPORT OV_RESULT iec62541_uaClientHandler_constructor(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
    OV_INSTPTR_iec62541_uaClientHandler pinst = Ov_StaticPtrCast(iec62541_uaClientHandler, pobj);
    OV_RESULT    result;

    /* do what the base class does first */
    result = ov_object_constructor(pobj);
    if(Ov_Fail(result))
         return result;

    /* do what */


    return OV_ERR_OK;
}

