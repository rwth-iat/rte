
/******************************************************************************
*
*   FILE
*   ----
*   remoteconnection.c
*
*   History
*   -------
*   2011-06-10   File created
*
*******************************************************************************
*
*   This file is generated by the 'fb_builder' command
*
******************************************************************************/


#ifndef OV_COMPILE_LIBRARY_fb
#define OV_COMPILE_LIBRARY_fb
#endif


#include "fb.h"
#include "libov/ov_macros.h"


OV_DLLFNCEXPORT OV_STRING fb_remoteconnection_sourceaddr_get(
    OV_INSTPTR_fb_remoteconnection          pobj
) {
    return pobj->v_sourceaddr;
}

OV_DLLFNCEXPORT OV_RESULT fb_remoteconnection_sourceaddr_set(
    OV_INSTPTR_fb_remoteconnection          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_sourceaddr,value);
}

