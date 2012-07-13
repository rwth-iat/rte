
/******************************************************************************
*
*   FILE
*   ----
*   fbComPkgVar.c
*
*   History
*   -------
*   2012-07-12   File created
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


OV_DLLFNCEXPORT OV_RESULT fbComPkg_fbComPkgVar_constructor(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
    OV_INSTPTR_fbComPkg_fbComPkgVar pinst = Ov_StaticPtrCast(fbComPkg_fbComPkgVar, pobj);
    OV_RESULT    result;

    /* do what the base class does first */
    result = fb_functionblock_constructor(pobj);
    if(Ov_Fail(result))
         return result;

    /* do what */


    return OV_ERR_OK;
}
