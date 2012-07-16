
/******************************************************************************
*
*   FILE
*   ----
*   setPkgBool.c
*
*   History
*   -------
*   2012-07-13   File created
*
*******************************************************************************
*
*   This file is generated by the 'acplt_builder' command
*
******************************************************************************/


#ifndef OV_COMPILE_LIBRARY_fbComPkg
#define OV_COMPILE_LIBRARY_fbComPkg
#endif


#include "fbComPkg.h"
#include "libov/ov_macros.h"


OV_DLLFNCEXPORT OV_RESULT fbComPkg_setPkgBool_constructor(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
    OV_INSTPTR_fbComPkg_setPkgBool pinst = Ov_StaticPtrCast(fbComPkg_setPkgBool, pobj);
    OV_RESULT    result;

    /* do what the base class does first */
    result = fbComPkg_fbComPkgVar_constructor(pobj);
    if(Ov_Fail(result))
         return result;

    /* do what */


    return OV_ERR_OK;
}

OV_DLLFNCEXPORT void fbComPkg_setPkgBool_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
    /*    
    *   local variables
    */
    OV_INSTPTR_fbComPkg_setPkgBool pinst = Ov_StaticPtrCast(fbComPkg_setPkgBool, pfb);

    OV_INSTPTR_ksapi_setBool pKsSetBool = Ov_DynamicPtrCast(ksapi_setBool, Ov_GetParent(fbComPkg_fbVariable, pinst));

    if (!pKsSetBool)
    {
          ov_logfile_error("%s - fbComPkg_setPkgBool_typemethod: wrong ksapi object in pkg.", pinst->v_identifier);
          return;
    }

    //pinst->v_value=pKsSetBool->v_receivebool;
    ksapi_setBool_sendbool_set(pKsSetBool, pinst->v_value);

    return;

    return;
}

