
/******************************************************************************
*
*   FILE
*   ----
*   getVar.c
*
*   History
*   -------
*   2013-05-14   File created
*
*******************************************************************************
*
*   This file is generated by the 'acplt_builder' command
*
******************************************************************************/


#ifndef OV_COMPILE_LIBRARY_ksapi
#define OV_COMPILE_LIBRARY_ksapi
#endif


#include "ksapi.h"
#include "libov/ov_macros.h"


OV_DLLFNCEXPORT OV_RESULT ksapi_getVar_constructor(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
    OV_INSTPTR_ksapi_getVar pinst = Ov_StaticPtrCast(ksapi_getVar, pobj);
    OV_RESULT    result;

    /* do what the base class does first */
    result = ov_object_constructor(pobj);
    if(Ov_Fail(result))
         return result;

    /* do what */


    return OV_ERR_OK;
}

OV_DLLFNCEXPORT void ksapi_getVar_destructor(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
    OV_INSTPTR_ksapi_getVar pinst = Ov_StaticPtrCast(ksapi_getVar, pobj);

    /* do what */

    /* destroy object */
    ov_object_destructor(pobj);

    return;
}

OV_DLLFNCEXPORT void ksapi_getVar_startup(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
    OV_INSTPTR_ksapi_getVar pinst = Ov_StaticPtrCast(ksapi_getVar, pobj);

    /* do what the base class does first */
    ksapi_KSApiCommon_startup(pobj);

    /* do what */


    return;
}

OV_DLLFNCEXPORT void ksapi_getVar_shutdown(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
    OV_INSTPTR_ksapi_getVar pinst = Ov_StaticPtrCast(ksapi_getVar, pobj);

    /* do what */

    /* set the object's state to "shut down" */
    ksapi_KSApiCommon_shutdown(pobj);

    return;
}

OV_DLLFNCEXPORT void ksapi_getVar_submit(
    OV_INSTPTR_ksapi_getVar          pobj
) {
    return;
}

OV_DLLFNCEXPORT void ksapi_getVar_setandsubmit(
    OV_INSTPTR_ksapi_getVar          pobj,
	OV_STRING 									serverHost,
	OV_STRING 									serverName,
	OV_STRING 									path
) {
    return;
}

