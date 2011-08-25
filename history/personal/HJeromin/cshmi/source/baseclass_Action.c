
/******************************************************************************
*
*   FILE
*   ----
*   Action.c
*
*   History
*   -------
*   2011-08-18   File created
*
*******************************************************************************
*
*   This file is generated by the 'acplt_builder' command
*
******************************************************************************/


#ifndef OV_COMPILE_LIBRARY_cshmi
#define OV_COMPILE_LIBRARY_cshmi
#endif


#include "cshmi.h"
#include "libov/ov_macros.h"


OV_DLLFNCEXPORT OV_BOOL cshmi_Action_cyclic_get(
    OV_INSTPTR_cshmi_Action          pobj
) {
    return pobj->v_cyclic;
}

OV_DLLFNCEXPORT OV_RESULT cshmi_Action_cyclic_set(
    OV_INSTPTR_cshmi_Action          pobj,
    const OV_BOOL  value
) {
    pobj->v_cyclic = value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_TIME_SPAN* cshmi_Action_cyctime_get(
    OV_INSTPTR_cshmi_Action          pobj
) {
    return &pobj->v_cyctime;
}

OV_DLLFNCEXPORT OV_RESULT cshmi_Action_cyctime_set(
    OV_INSTPTR_cshmi_Action          pobj,
    const OV_TIME_SPAN*  value
) {
    pobj->v_cyctime = *value;
    return OV_ERR_OK;
}

