
/******************************************************************************
*
*   FILE
*   ----
*   createObject.c
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


OV_DLLFNCEXPORT void ksapi_createObject_startup(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
    OV_INSTPTR_ksapi_createObject pinst = Ov_StaticPtrCast(ksapi_createObject, pobj);

    /* do what the base class does first */
    ksapi_KSApiCommon_startup(pobj);

    /* do what */


    return;
}

OV_DLLFNCEXPORT void ksapi_createObject_submit(
    OV_INSTPTR_ksapi_createObject          pobj
) {
    return;
}

OV_DLLFNCEXPORT void ksapi_createObject_setandsubmit(
    OV_INSTPTR_ksapi_createObject          pobj,
    OV_STRING 									serverHost,
    OV_STRING 									serverName,
    OV_STRING 									path,
    OV_STRING									factorypath,
    OV_INT										position,
    OV_STRING									place
) {
    return;
}

