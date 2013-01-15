
/******************************************************************************
*
*   FILE
*   ----
*   Manager.c
*
*   History
*   -------
*   2013-01-15   File created
*
*******************************************************************************
*
*   This file is generated by the 'acplt_builder' command
*
******************************************************************************/


#ifndef OV_COMPILE_LIBRARY_ksbase
#define OV_COMPILE_LIBRARY_ksbase
#endif


#include "ksbase.h"
#include "libov/ov_macros.h"


OV_DLLFNCEXPORT OV_RESULT ksbase_Manager_register(
	OV_STRING name,
	OV_INT version,
	OV_STRING_VEC protocols,
	OV_STRING_VEC ports,
	OV_UINT timetolive
) {
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT ksbase_Manager_unregister(
	OV_STRING name,
	OV_INT version
) {
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT ksbase_Manager_getserverdata(
	OV_STRING name,
	OV_INT version,
	OV_STRING_VEC* protocols,
	OV_STRING_VEC* ports,
	OV_UINT* TimeToLive,
	OV_TIME* ExpirationTime
) {
    return OV_ERR_OK;
}

