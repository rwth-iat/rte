
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

#ifdef sfc_ERROR
#define sfc_TRANS_ERROR
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
    OV_INSTPTR_sfc_sfcHeader pSFC = NULL;
    OV_RESULT    result;

    /* do what the base class does first */
    result = fb_functionblock_constructor(pobj);
    if(Ov_Fail(result))
         return result;

    // check location
    if (Ov_CanCastTo(sfc_sfcHeader, pContainer))
	{
    	pSFC=Ov_DynamicPtrCast(sfc_sfcHeader, pContainer);
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
    //OV_INSTPTR_fb_functionblock pTransCond = Ov_StaticPtrCast(fb_functionblock, Ov_GetParent( fb_outputconnections, Ov_GetFirstChild(fb_inputconnections, pinst)));
    OV_INSTPTR_fb_connection    pResultConnection = Ov_StaticPtrCast(fb_connection, Ov_GetFirstChild(fb_inputconnections, pinst));
    OV_INSTPTR_fb_functionblock pTransCond = Ov_StaticPtrCast(fb_functionblock, Ov_GetParent( fb_outputconnections, pResultConnection));
    OV_INSTPTR_sfc_step  		pPreStep = Ov_GetParent(sfc_nextTransitions, pinst);
    OV_INSTPTR_sfc_step  		pNextStep = Ov_GetParent(sfc_previousTransitions, pinst);
    OV_INSTPTR_sfc_sfcHeader	pSFC = Ov_StaticPtrCast(sfc_sfcHeader, Ov_GetParent(ov_containment, pinst));
    OV_INSTPTR_fb_functionchart pFC = Ov_StaticPtrCast(fb_functionchart, Ov_GetParent(ov_containment, pSFC));
    //OV_INSTPTR_fb_task          pPreStepExit = &pPreStep->p_exit;
    //OV_INSTPTR_fb_task          pPreStepEntry = &pPreStep->p_entry;
    OV_INSTPTR_ov_domain        pTransCondsDomain = &pSFC->p_transConds;
    OV_INSTPTR_ov_domain        pTransCondsContainer = Ov_StaticPtrCast(ov_domain, Ov_GetParent(ov_containment, pTransCond));
    OV_INSTPTR_fb_task          pTransCondsTaskParent = Ov_GetParent(fb_tasklist, pTransCond);
    OV_RESULT    result;


    // check location and links
    if ( !pPreStep || !pNextStep || !pSFC)
    {
    	pinst->v_error=TRUE;
    	ov_string_setvalue(&pinst->v_errorDetail, "wrong link");
    	return;
    }
    pinst->v_error=FALSE;
    ov_string_setvalue(&pinst->v_errorDetail, NULL);

    printf("%s/%s\n", pSFC->v_identifier, pinst->v_identifier);

    // execute transition condition
    if (pTransCond != NULL)
    {
        // check location
    	if (pTransCondsContainer != Ov_StaticPtrCast(ov_domain, pFC))
    	{
    		if ((pTransCondsContainer != pTransCondsDomain) && (pTransCondsContainer != Ov_StaticPtrCast(ov_domain, pFC)) )
    		{
    			pinst->v_error=TRUE;
    			ov_string_setvalue(&pinst->v_errorDetail, "transition condition must be placed in the transConds container ");
    			return;
    		}

    		// check tasklist
    		if (pTransCondsTaskParent != NULL)
    			Ov_Unlink(fb_tasklist, pTransCondsTaskParent, pTransCond);

    		// init transition condition
    		pTransCond->v_actimode = 1;
    		pTransCond->v_cyctime.secs = 0;
    		pTransCond->v_cyctime.usecs = 0;
    		pTransCond->v_iexreq = TRUE;
    		pResultConnection->v_on = 1;
    		pResultConnection->v_sourcetrig = 1;   // connection mode: source sends
    		// execute transtion condition
    		Ov_Call1(fb_task, pTransCond, execute, pltc);
    	}
    }

    // trigger
    if (pinst->v_result)
    {
       	if (!pPreStep->v_evTransTrigger)
       	{

       		// broadcast the trigger event
       		pPreStep->v_evTransTrigger=TRUE;

        	// deactivate transition condition
        	if (pTransCond !=NULL) pTransCond->v_actimode = 0;

        	// execute exit-actions of subSFCs


        	// link next-step to sfc-tasklist
        	//result=Ov_LinkPlaced(fb_tasklist, &pSFC->p_intask, pNextStep, OV_PMH_END);
        	result=Ov_Link(fb_tasklist, &pSFC->p_intask, pNextStep);
        	pNextStep->v_actimode=1;
        	pNextStep->v_phase=1;
        	pNextStep->v_qualifier=1;

        	// deactivate transition condition
        	if (pTransCond != NULL) pTransCond->v_actimode = 0;
        }
    }

    return;
}


