
/******************************************************************************
*
*   FILE
*   ----
*   transition.c
*
*   History
*   -------
*   2011-05-27   File created
*
*******************************************************************************
*
*   This file is generated by the 'fb_builder' command
*
******************************************************************************/


#ifndef OV_COMPILE_LIBRARY_sfc
#define OV_COMPILE_LIBRARY_sfc
#endif

#ifdef SFC_ERROR
#define SFC_TRANS_ERROR
#endif


#include "sfc.h"
#include "sfclib.h"

OV_DLLFNCEXPORT OV_RESULT sfc_transition_constructor(
	OV_INSTPTR_ov_object 	pobj
) {
    /*
    *   local variables
    */
    OV_INSTPTR_sfc_transition pinst = Ov_StaticPtrCast(sfc_transition, pobj);

    OV_INSTPTR_ov_domain  pContainer = Ov_GetParent(ov_containment, pinst);
    OV_INSTPTR_fc_sfcHeader pSfcHeader = NULL;
    OV_RESULT    result;

    /* do what the base class does first */
    result = fb_functionblock_constructor(pobj);
    if(Ov_Fail(result))
         return result;

    // check location
    if (Ov_CanCastTo(sfc_sfcHeader, pContainer))
	{
    	pSfcHeader=Ov_DynamicPtrCast(sfc_sfcHeader, pContainer);
	} else {
		if (pContainer!=NULL)
		{
			ov_logfile_error("sfc_transition_constructor: transition must be encapsulated in a sfcHeader.");
			return OV_ERR_BADPLACEMENT;
		}
	}

    return OV_ERR_OK;
}


OV_DLLFNCEXPORT void sfc_transition_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
    /*    
    *   local variables
    */
    OV_INSTPTR_sfc_transition 	pinst = Ov_StaticPtrCast(sfc_transition, pfb);
    //OV_INSTPTR_sfc_transitionCondition pTransCondition = Ov_StaticPtrCast(sfc_transitionCondition, Ov_GetParent( fb_outputconnections, Ov_GetFirstChild(fb_inputconnections, pinst)));

    OV_INSTPTR_sfc_step  		pPreStep = Ov_GetParent(sfc_nextTransitions, pinst);
    OV_INSTPTR_sfc_step  		pNextStep = Ov_GetParent(sfc_previousTransitions, pinst);
    OV_INSTPTR_sfc_sfcHeader	pSfcHeader = Ov_StaticPtrCast(sfc_sfcHeader, Ov_GetParent(ov_containment, pPreStep));
    OV_INSTPTR_fb_task          pPreStepExit = &pPreStep->p_exit;
    OV_INSTPTR_fb_task          pPreStepEntry = &pPreStep->p_entry;
    OV_RESULT    fr;

    // check location and links
    if ( !pPreStep || !pNextStep || !pSfcHeader)
    {
    	pinst->v_error=TRUE;
    	ov_string_setvalue(&pinst->v_errorDetail, "wrong link");
    	return;
    }
    pinst->v_error=FALSE;
    ov_string_setvalue(&pinst->v_errorDetail, NULL);

    // triggler sfcTransitionCondition if exists
    /*
    if (pTransCondition !=NULL)
    {
    	pTransCondition->v_actimode = 1;
    	Ov_Call1 (fb_functionblock, pTransCondition, execute, pltc);
    	// TODO: Warning
    	// TODO: triggler the connection again.
    	pTransCondition->v_actimode = 0;
    }
    */

    // trigger
    if (pinst->v_result)
    {
    	// broadcast the trigger event
       	if (!pPreStep->v_evTransTrigger)
       	{
       		pPreStep->v_evTransTrigger=TRUE;

       		// activate Exit-actions
        	pPreStepExit->v_actimode=3;

        	// reset step
        	pPreStep->v_actimode=0;
        	pPreStep->v_X=FALSE;
        	pPreStepEntry->v_actimode=3;

        	// link nextStep to the sfc-tasklist
        	fr=Ov_LinkPlaced(fb_tasklist, pSfcHeader, pNextStep, OV_PMH_BEGIN);
        	pNextStep->v_actimode=1;
        	// unlink previousStep
        	Ov_Unlink(fb_tasklist, pSfcHeader, pPreStep);
        }
    }
    return;
}


