
/******************************************************************************
*
*   FILE
*   ----
*   staticfile.c
*
*   History
*   -------
*   2012-02-13   File created
*
*******************************************************************************
*
*   This file is generated by the 'acplt_builder' command
*
******************************************************************************/


#ifndef OV_COMPILE_LIBRARY_ksservhttp
#define OV_COMPILE_LIBRARY_ksservhttp
#endif


#include "ksservhttp.h"
#include "libov/ov_macros.h"


OV_DLLFNCEXPORT OV_STRING ksservhttp_staticfile_mimetype_get(
    OV_INSTPTR_ksservhttp_staticfile          pobj
) {
    return pobj->v_mimetype;
}

OV_DLLFNCEXPORT OV_RESULT ksservhttp_staticfile_mimetype_set(
    OV_INSTPTR_ksservhttp_staticfile          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_mimetype,value);
}

OV_DLLFNCEXPORT OV_STRING ksservhttp_staticfile_content_get(
    OV_INSTPTR_ksservhttp_staticfile          pobj
) {
    return pobj->v_content;
}

OV_DLLFNCEXPORT OV_RESULT ksservhttp_staticfile_content_set(
    OV_INSTPTR_ksservhttp_staticfile          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_content,value);
}

