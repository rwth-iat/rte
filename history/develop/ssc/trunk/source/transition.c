
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


#ifndef OV_COMPILE_LIBRARY_ssc
#define OV_COMPILE_LIBRARY_ssc
#endif

#ifdef ssc_ERROR
#define ssc_TRANS_ERROR
#endif


#include "ssc.h"
#include "ssclib.h"

OV_DLLFNCEXPORT OV_RESULT ssc_transition_constructor(
	OV_INSTPTR_ov_object 	pobj
)
{
    /*
    *   local variables
    */
    OV_INSTPTR_ssc_transition pinst = Ov_StaticPtrCast(ssc_transition, pobj);

    OV_INSTPTR_ov_domain  pContainer = Ov_GetParent(ov_containment, pinst);
    OV_INSTPTR_ssc_sscHeader pSSC = NULL;
    OV_RESULT    result;

    /* do what the base class does first */
    result = fb_functionblock_constructor(pobj);
    if(Ov_Fail(result))
         return result;

    // check location
    if (Ov_CanCastTo(ssc_sscHeader, pContainer))
	{
    	pSSC=Ov_DynamicPtrCast(ssc_sscHeader, pContainer);
	} else {
		if (pContainer!=NULL)
		{
			ov_logfile_error("ssc_transition_constructor: transition must be encapsulated in a sscHeader.");
			return OV_ERR_BADPLACEMENT;
		}
	}

    return OV_ERR_OK;
}


OV_DLLFNCEXPORT void ssc_transition_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
    /*    
    *   local variables
    */
    OV_INSTPTR_ssc_transition 	pinst = Ov_StaticPtrCast(ssc_transition, pfb);
    //OV_INSTPTR_fb_functionblock pTransCond = Ov_StaticPtrCast(fb_functionblock, Ov_GetParent( fb_outputconnections, Ov_GetFirstChild(fb_inputconnections, pinst)));

    //find connection object with is connected to the result port
    OV_INSTPTR_ov_object pObj =  Ov_GetFirstChild(fb_inputconnections, pinst);
    OV_INSTPTR_fb_connection    pResultConnection = Ov_StaticPtrCast(fb_connection, pObj);

    // OV_INSTPTR_fb_connection    pResultConnection = Ov_StaticPtrCast(fb_connection, Ov_GetFirstChild(fb_inputconnections, pinst));

    //find source object which sends the data to transition object
    OV_INSTPTR_fb_object pSrcObj = Ov_GetParent( fb_outputconnections, pResultConnection);

    //OV_INSTPTR_fb_functionblock pTransCond = (OV_INSTPTR_fb_functionblock)(Ov_GetParent(ov_containment, pParentObj));// Ov_DynamicPtrCast(fb_functionblock, pParentObj);

    OV_INSTPTR_ov_domain        pTransCondsDomain = NULL;
    OV_INSTPTR_ov_domain        pTransCondsContainer = NULL;
    OV_INSTPTR_fb_task          pTransCondsTaskParent = NULL;

    OV_RESULT    result;
    OV_INSTPTR_fb_port port = NULL;

    OV_INSTPTR_fb_functionblock pTransCond = NULL;
    //Ov_DynamicPtrCast(fb_functionblock, Ov_GetParent( fb_outputconnections, pResultConnection));
    OV_INSTPTR_ssc_step  		pPreStep = Ov_GetParent(ssc_nextTransitions, pinst);
    OV_INSTPTR_ssc_step  		pNextStep = Ov_GetParent(ssc_previousTransitions, pinst);

    OV_INSTPTR_ssc_sscHeader	pSSC = Ov_StaticPtrCast(ssc_sscHeader, Ov_GetParent(ov_containment, pinst));
    OV_INSTPTR_fb_functionchart pFC = Ov_StaticPtrCast(fb_functionchart, Ov_GetParent(ov_containment, pSSC));

    //OV_INSTPTR_fb_task          pPreStepExit = &pPreStep->p_exit;
    //OV_INSTPTR_fb_task          pPreStepEntry = &pPreStep->p_entry;

    //check if the transCond is a functionchart or a functionblock
    if(Ov_CanCastTo(fb_port, pSrcObj))
    {
    	//transCond is a functionchart
    	pTransCond = (OV_INSTPTR_fb_functionblock)(Ov_GetParent(fb_variables,(OV_INSTPTR_fb_port) pSrcObj));
    }
    else
    {
    	//transCond is a functionblock
    	pTransCond = (OV_INSTPTR_fb_functionblock)(pSrcObj);
    }

    pTransCondsDomain = &pSSC->p_transConds;
    pTransCondsContainer = Ov_StaticPtrCast(ov_domain, Ov_GetParent(ov_containment, pTransCond));
    pTransCondsTaskParent = Ov_GetParent(fb_tasklist, pTransCond);






    // check location and links
    if ( !pPreStep || !pNextStep || !pSSC)
    {
    	pinst->v_error=TRUE;
    	ov_string_setvalue(&pinst->v_errorDetail, "wrong link");
    	return;
    }
    pinst->v_error=FALSE;
    ov_string_setvalue(&pinst->v_errorDetail, NULL);

    printf("%s/%s\n", pSSC->v_identifier, pinst->v_identifier);

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

        	// execute exit-actions of subSSCs


        	// link next-step to ssc-tasklist
        	//result=Ov_LinkPlaced(fb_tasklist, &pSSC->p_intask, pNextStep, OV_PMH_END);
        	result=Ov_Link(fb_tasklist, &pSSC->p_intask, pNextStep);
        	pNextStep->v_actimode=1;
        	pNextStep->v_phase=1;
        	pNextStep->v_qualifier=1;

        	// deactivate transition condition
        	if (pTransCond != NULL) pTransCond->v_actimode = 0;
        }
    }

    return;
}


