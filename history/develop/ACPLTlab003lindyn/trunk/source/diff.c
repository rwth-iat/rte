
/******************************************************************************
*
*   FILE
*   ----
*   diff.c
*
*   History
*   -------
*   2014-06-03   File created
*
*******************************************************************************
*
*   This file is generated by the 'acplt_builder' command
*
******************************************************************************/


#ifndef OV_COMPILE_LIBRARY_ACPLTlab003lindyn
#define OV_COMPILE_LIBRARY_ACPLTlab003lindyn
#endif


#include "ACPLTlab003lindyn.h"
#include "libov/ov_macros.h"
#include "libov/ov_time.h"


OV_DLLFNCEXPORT OV_RESULT ACPLTlab003lindyn_diff_constructor(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
    OV_INSTPTR_ACPLTlab003lindyn_diff pinst = Ov_StaticPtrCast(ACPLTlab003lindyn_diff, pobj);
    OV_RESULT    result;

    /* do what the base class does first */
    result = fb_functionblock_constructor(pobj);
    if(Ov_Fail(result))
         return result;

    /* do what */


    return OV_ERR_OK;
}

OV_DLLFNCEXPORT void ACPLTlab003lindyn_diff_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
    /*    
    *   local variables
    */
	OV_TIME_SPAN temp;
	double dbl=0;
    OV_INSTPTR_ACPLTlab003lindyn_diff pinst = Ov_StaticPtrCast(ACPLTlab003lindyn_diff, pfb);
    if(pinst->v_methcount>1){
    	ov_time_diff( &temp, pltc,&(pinst->v_timeold)) ;
    	Ov_TimeSpanToDouble(temp, dbl);
    	pinst->v_XOUT=(pinst->v_XIN-pinst->v_xold) /dbl;
    }

    pinst->v_ENO=pinst->v_EN;

    pinst->v_xold=pinst->v_XIN;
    pinst->v_timeold=*pltc;
    return;
}

