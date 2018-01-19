
/******************************************************************************
*
*   FILE
*   ----
*   AASWriter.c
*
*   History
*   -------
*   2018-01-12   File created
*
*******************************************************************************
*
*   This file is generated by the 'acplt_builder' command
*
******************************************************************************/


#ifndef OV_COMPILE_LIBRARY_simpleExperimentDataArchiver
#define OV_COMPILE_LIBRARY_simpleExperimentDataArchiver
#endif


#include "simpleExperimentDataArchiver.h"
#include "libov/ov_macros.h"


OV_DLLFNCEXPORT OV_RESULT simpleExperimentDataArchiver_AASWriter_aasId_set(
    OV_INSTPTR_simpleExperimentDataArchiver_AASWriter          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_aasId,value);
}

OV_DLLFNCEXPORT OV_RESULT simpleExperimentDataArchiver_AASWriter_constructor(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
    OV_INSTPTR_simpleExperimentDataArchiver_AASWriter pinst = Ov_StaticPtrCast(simpleExperimentDataArchiver_AASWriter, pobj);
    OV_RESULT    result;

    /* do what the base class does first */
    result = simpleExperimentDataArchiver_writer_constructor(pobj);
    if(Ov_Fail(result))
         return result;

    /* do what */


    return OV_ERR_OK;
}

OV_DLLFNCEXPORT void simpleExperimentDataArchiver_AASWriter_destructor(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
    OV_INSTPTR_simpleExperimentDataArchiver_AASWriter pinst = Ov_StaticPtrCast(simpleExperimentDataArchiver_AASWriter, pobj);

    /* do what */

    /* destroy object */
    simpleExperimentDataArchiver_writer_destructor(pobj);

    return;
}

OV_DLLFNCEXPORT void simpleExperimentDataArchiver_AASWriter_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
    /*    
    *   local variables
    */
    OV_INSTPTR_simpleExperimentDataArchiver_AASWriter pinst = Ov_StaticPtrCast(simpleExperimentDataArchiver_AASWriter, pfb);

    return;
}
