
/******************************************************************************
*
*   FILE
*   ----
*   sscStep.c
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
#define ssc_STEP_ERROR
#endif


#include "ssc.h"
#include "ssclib.h"


OV_DLLFNCEXPORT OV_RESULT ssc_step_constructor(
	OV_INSTPTR_ov_object 	pobj
) {
    /*
    *   local variables
    */
    OV_INSTPTR_ssc_step pinst = Ov_StaticPtrCast(ssc_step, pobj);
    OV_INSTPTR_fb_task    pEntry = &pinst->p_entry;
    OV_INSTPTR_fb_task    pDo 	 = &pinst->p_do;
    OV_INSTPTR_fb_task    pTrans = &pinst->p_trans;
    OV_INSTPTR_fb_task    pExit  = &pinst->p_exit;

    OV_INSTPTR_ssc_sscHeader pSSC = Ov_DynamicPtrCast(ssc_sscHeader, Ov_GetParent(ov_containment, pinst));
    //OV_INSTPTR_fb_task    pIntask=NULL;
    OV_RESULT    result;

    /* do what the base class does first */
    result = fb_functionblock_constructor(pobj);
    if(Ov_Fail(result))
         return result;

    // check location
    if (pSSC==NULL)
    {
    	ov_logfile_error("ssc_step_constructor: step must be encapsulated in a sscHeader.");
    	return OV_ERR_BADPLACEMENT;
    }

    // local tasklist
    //result=Ov_Link(fb_tasklist, pinst, pEntry);
    pEntry->v_actimode=1;
    //result=Ov_Link(fb_tasklist, pinst, pDo);
    pDo->v_actimode=1;
    //result=Ov_Link(fb_tasklist, pinst, pTrans);
    pTrans->v_actimode=1;
    //result=Ov_Link(fb_tasklist, pinst, pExit);
    pExit->v_actimode=1;

    //activate
    pinst->v_iexreq=1;

    return OV_ERR_OK;
}


OV_DLLFNCEXPORT void ssc_step_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
    /*    
    *   local variables
    */
    OV_INSTPTR_ssc_step pinst = Ov_StaticPtrCast(ssc_step, pfb);
    OV_INSTPTR_fb_task    pEntry = &pinst->p_entry;
    OV_INSTPTR_fb_task    pDo 	 = &pinst->p_do;
    OV_INSTPTR_fb_task    pExit  = &pinst->p_exit;
    OV_INSTPTR_fb_task    pTrans = &pinst->p_trans;
    OV_INSTPTR_ssc_sscHeader pSSC = Ov_DynamicPtrCast(ssc_sscHeader, Ov_GetParent(ov_containment, pinst));
    OV_INSTPTR_ssc_step pNextStep = NULL;
    OV_INSTPTR_ssc_sscHeader pSubSsc=NULL;
    OV_INSTPTR_ssc_executeSsc pExecuteSsc=NULL;
    OV_TIME elapsedTime;
    // helper vaiables
    OV_BOOL	  exitLoop=FALSE;

    // check location
    if (pSSC==NULL)
    {
      	ov_logfile_error("ssc_step_constructor: step must be encapsulated in a sscHeader.");
      	return;
    }

    // TODO:
    // check if sscHeader is the taskparent.

    // init variables
    pinst->v_cyctime.secs = 0;
    pinst->v_cyctime.usecs = 0;
    pinst->v_iexreq = TRUE;
    pinst->v_evTransTrigger=FALSE;


    // execute subtasks
    do{
    	exitLoop=TRUE;

    	switch (pinst->v_phase)
    	{
    	/* phase 1: entry, do */
    	case 1:
    		pinst->v_X=TRUE;
    		if(pinst->v_qualifier==1)
    		{
    			/* entry */
    			ov_time_gettime(&pinst->v_startTime);
    			//initialized with zero
    			ov_time_diff(&pinst->v_T, &pinst->v_startTime, &pinst->v_startTime);
    			//printf("%s/%s/entry\n", pSSC->v_identifier, pinst->v_identifier);
    			Ov_Call1 (fb_task, pEntry, execute, pltc);
    			pinst->v_qualifier=2;
    		}
    		/* do */
    		//printf("%s/%s/do\n", pSSC->v_identifier, pinst->v_identifier);
    		Ov_Call1 (fb_task, pDo, execute, pltc);

    		/* event: SSC terminates */
    		if (pinst->v_internalID==999)
    			pSSC->v_terminated=TRUE;
    		else
    			pSSC->v_terminated=FALSE;

    		pinst->v_phase = 2;
    		break;

    	/* phase 2: transitions, exit*/
    	case 2:

    		    /* subSSCs run to the end without interruption. */
    			if (pinst->v_hasSubSsc )
    			{
    				if (pinst->v_subSscTerminated)
    					pTrans->v_actimode=1;
    				else
    					pTrans->v_actimode=0;
    			}

    		// if stopping SSC, do not check transitions
    		if (pSSC->v_workingState==WOST_STOP)
    			pTrans->v_actimode=0;

    		/* transitions */
    		Ov_Call1 (fb_task, pTrans, execute, pltc);


    		// if Trigger, or cmd "STOP" for final step
    		if (pinst->v_evTransTrigger || ( pSSC->v_workingState==WOST_STOP) )
    		{
    			// stop/break subSSCs
				// find all action blocks calling subSSCs
				Ov_ForEachChildEx(ov_containment, pinst, pExecuteSsc, ssc_executeSsc)
				{
					// find all subSSCs for do
					if (pExecuteSsc->v_actionQualifier == ACT_DO)
					{
						pSubSsc = Ov_DynamicPtrCast(ssc_sscHeader, Ov_GetParent(ssc_actionBlocks, pExecuteSsc));

						if (pSubSsc !=NULL)
						{
							// stop subSSC
    						pSubSsc->v_EN=0;
    						Ov_Call1 (fb_task, Ov_DynamicPtrCast(fb_task, pSubSsc), execute, pltc);
						}
					}
				}

				if (pSSC->v_workingState==WOST_STOP)
					pExit->v_actimode=3;


    			/* exit */
    			//printf("%s/%s/exit\n", pSSC->v_identifier, pinst->v_identifier);
    			Ov_Call1 (fb_task, pExit, execute, pltc);
    			// unlink from sscHeader.intask
    			Ov_Unlink(fb_tasklist, Ov_GetParent(fb_tasklist, pinst), pinst);
    			pinst->v_X=FALSE;
    			pinst->v_qualifier=1;

    			// find next step and execute its entry & do
    			// Note: this job should be done by ov_ForEachChild(fb_tasklist, ...). But it is not possible to adapt the tasklist dynamically
    			pNextStep=Ov_DynamicPtrCast(ssc_step, Ov_GetLastChild(fb_tasklist, &pSSC->p_intask));
    			if (pNextStep != NULL)
    			{
    				// update activeStep
    				ov_string_setvalue(&pSSC->v_activeStep, pNextStep->v_identifier);
    				// execute nextStep entry & do
    				Ov_Call1 (fb_task, Ov_DynamicPtrCast(fb_task, pNextStep), execute, pltc);
    			}
    		} else exitLoop=FALSE;

    		pinst->v_phase = 1;
    		break;
    	}
		ov_time_gettime(&elapsedTime);
		ov_time_diff(&pinst->v_T, &elapsedTime, &pinst->v_startTime);
    } while (!exitLoop);

    // step flag will be reseted by triggered transition
    return;
}

OV_DLLFNCEXPORT OV_RESULT ssc_step_resetStep(
	OV_INSTPTR_ssc_step	pinst
) {
    /*
    *   local variables
    */
    OV_INSTPTR_fb_task    pEntry = &pinst->p_entry;
    OV_INSTPTR_fb_task    pDo 	 = &pinst->p_do;
    OV_INSTPTR_fb_task    pTrans = &pinst->p_trans;
    OV_INSTPTR_fb_task    pExit  = &pinst->p_exit;
    OV_INSTPTR_ssc_actionBlock pActionBlock  = NULL;
    OV_INSTPTR_ssc_executeSsc pExecuteSsc=NULL;

    OV_INSTPTR_fb_task pTaskParent = Ov_GetParent(fb_tasklist, pinst);
    //OV_RESULT    result;

    //unlink from task parent
    if (pTaskParent != NULL)	Ov_Unlink(fb_tasklist, pTaskParent, pinst);


    //reset parameters
    pinst->v_actimode=0;
    pinst->v_cyctime.secs = 0;
    pinst->v_cyctime.usecs = 0;
    pinst->v_iexreq = TRUE;
    pinst->v_X = FALSE;
    //TODO: T, error, errorDetail
    pinst->v_phase = 1;
    pinst->v_qualifier = 1;

    // find subSSCs
    Ov_ForEachChildEx(ov_containment, pinst, pExecuteSsc, ssc_executeSsc)
	{
		if (pExecuteSsc->v_actionQualifier != ACT_EXIT)
			pinst->v_hasSubSsc=TRUE;
	}

    //reset subtasks
    pEntry->v_actimode=1;
    pDo->v_actimode=1;
    pTrans->v_actimode=1;
    pExit->v_actimode=0;

    // TODO: reset all action blocks

    //activate all action blocks
	  Ov_ForEachChildEx(ov_containment, pinst, pActionBlock, ssc_actionBlock)
	  {
		  pActionBlock->v_actimode=1;
		  pActionBlock->v_cyctime.secs = 0;
		  pActionBlock->v_cyctime.usecs = 0;
		  pActionBlock->v_iexreq = TRUE;
	  }


    return OV_ERR_OK;
}


OV_DLLFNCEXPORT OV_ACCESS ssc_step_getaccess(
	OV_INSTPTR_ov_object	pobj,
	const OV_ELEMENT		*pelem,
	const OV_TICKET			*pticket
) {
	/*
	*   local variables
	*/
	OV_INSTPTR_ssc_sscHeader activeHeader = Ov_DynamicPtrCast(ssc_sscHeader, Ov_GetParent(ov_containment, pobj));
	OV_ACCESS access_code = ov_object_getaccess(pobj, pelem, pticket);

	/*
	*	switch based on the element's type
	*/
	switch(pelem->elemtype) {
		case OV_ET_OBJECT:
			if(!activeHeader){
				//skip handling
			}else if(	activeHeader->v_workingState == WOST_INIT ||
						activeHeader->v_workingState == WOST_STOP ||
						activeHeader->v_workingState == WOST_TERMINATE)
			{
				//allow deletion
				access_code = (access_code | OV_AC_DELETEABLE);
			}else{
				//disallow deletion
				access_code = (access_code & OV_AC_DELETEABLE);
			}
			break;
		default:
			break;
	}
	return access_code;
}
