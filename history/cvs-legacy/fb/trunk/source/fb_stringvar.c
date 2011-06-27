
/******************************************************************************
*
*   FILE
*   ----
*   stringvar.c
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


OV_DLLFNCEXPORT OV_STRING fb_stringvar_value_get(
    OV_INSTPTR_fb_stringvar          pobj
) {
    return pobj->v_value;
}

OV_DLLFNCEXPORT OV_RESULT fb_stringvar_value_set(
    OV_INSTPTR_fb_stringvar          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_value,value);
}

