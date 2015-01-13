
/******************************************************************************
*
*   FILE
*   ----
*   sscHeader.c
*
*   History
*   -------
*   2011-07-04   File created
*
*******************************************************************************
*
*   This file is generated by the 'fb_builder' command
*
******************************************************************************/


#ifndef OV_COMPILE_LIBRARY_ssc
#define OV_COMPILE_LIBRARY_ssc
#endif


#include "ssclib.h"

OV_DLLFNCEXPORT OV_RESULT ssc_sscHeader_initStepName_set(
    OV_INSTPTR_ssc_sscHeader          pobj,
    const OV_STRING  value
)
{
	OV_INSTPTR_ssc_sscHeader pinst = Ov_StaticPtrCast(ssc_sscHeader, pobj);

	OV_INSTPTR_ssc_step pstep;
	OV_INSTPTR_ssc_step foundStep = NULL;

	if(ov_string_compare(value, "") == OV_STRCMP_EQUAL){
		//allow old INITIAL_VALUE for loading an backup
		return OV_ERR_OK;
	}

	if (pinst->v_workingState == SSC_WOST_INIT)
	{
		foundStep = Ov_SearchChildEx(ov_containment, pobj, value, ssc_step);
		if(foundStep != NULL){
			Ov_ForEachChildEx(ov_containment, pobj, pstep, ssc_step){
				if(foundStep == pstep)
				{
					pstep->v_internalRole = SSC_STEPROLE_START;
					pstep->v_actimode = FB_AM_ON;
				}
				else //deactivate all non init steps and set the correct id
				{
					if (pstep->v_internalRole != SSC_STEPROLE_END) //not an end step
					{
						pstep->v_internalRole = SSC_STEPROLE_NORMAL;
					}
					pstep->v_actimode = FB_AM_OFF;
				}
			}
		}
		else
		{
			pinst->v_error = TRUE;
			ov_string_setvalue(&(pinst->v_errorDetail), "step not found");
			return OV_ERR_BADVALUE;
		}
	}
	else
	{
		pinst->v_error = TRUE;
		ov_string_setvalue(&(pinst->v_errorDetail),"init and end step can only be set when workingState = 0");
		return OV_ERR_BADVALUE;
	}
	return ov_string_setvalue(&pobj->v_initStepName, value);
}

OV_DLLFNCEXPORT OV_RESULT ssc_sscHeader_endStepName_set(
    OV_INSTPTR_ssc_sscHeader          pobj,
    const OV_STRING  value
)
{
	OV_INSTPTR_ssc_sscHeader pinst = Ov_StaticPtrCast(ssc_sscHeader, pobj);
	OV_UINT count = 0; // count of end step strings
	int i = 0; // loop variable
	int n = 1;
	OV_STRING *pEndStepList = NULL;
	OV_INSTPTR_ssc_step pstep = NULL;

	pEndStepList = ov_string_split(value,",",&count);
	if(ov_string_compare(value, "") == OV_STRCMP_EQUAL){
		//allow old INITIAL_VALUE for loading an backup
		return OV_ERR_OK;
	}

	Ov_ForEachChildEx(ov_containment, pobj, pstep, ssc_step){
		for(i = 0; i<count; i++){
			if(ov_string_compare(pstep->v_identifier, pEndStepList[i]) == OV_STRCMP_EQUAL)
			{
				pstep->v_internalRole = SSC_STEPROLE_END;
				n++;
			}
		}
	}
	ov_string_freelist(pEndStepList);
	if(n < count)
	{
		pinst->v_error = TRUE;
		ov_string_setvalue(&(pinst->v_errorDetail),"could not set all end steps");
		return OV_ERR_BADVALUE;
	}
    return ov_string_setvalue(&pobj->v_endStepName,value);
}
static OV_RESULT ssc_sscHeader_checkLocation(
		OV_INSTPTR_ssc_sscHeader	pSSC
) {
	OV_INSTPTR_fb_functionchart pSscContainer = Ov_DynamicPtrCast(fb_functionchart, Ov_GetParent(ov_containment, pSSC));
	OV_INSTPTR_ov_domain pSscContainerContainer = Ov_GetParent(ov_containment, Ov_GetParent(ov_containment, pSSC));

	if ( (pSscContainer == NULL) && (pSscContainerContainer != NULL) )
	{
		pSSC->v_error=TRUE;
		ov_string_setvalue(&pSSC->v_errorDetail, "ssc must be encapsulated in a functionchart.");
		ov_logfile_error("ssc_sscHeader_constructor: ssc must be encapsulated in a functionchart.");
		return OV_ERR_BADPLACEMENT;
	}
	return OV_ERR_OK;
}


OV_DLLFNCEXPORT OV_RESULT ssc_sscHeader_constructor(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
    OV_INSTPTR_ssc_sscHeader pinst = Ov_StaticPtrCast(ssc_sscHeader, pobj);
    OV_INSTPTR_ssc_step pInitStep = NULL;
    OV_INSTPTR_ssc_step pEndStep = NULL;
    OV_RESULT    result;

    /* do what the base class does first */
    result = fb_functionblock_constructor(pobj);
    if(Ov_Fail(result))
         return result;

    // check location
    result = ssc_sscHeader_checkLocation(pinst);
    if(Ov_Fail(result)) return result;

    // create INIT- & END-step
    result = Ov_CreateObject(ssc_step, pInitStep, pinst, "INIT");
    pInitStep->v_internalRole = SSC_STEPROLE_START;
    pInitStep->v_actimode = FB_AM_ON;
    result = Ov_CreateObject(ssc_step, pEndStep, pinst, "END");
    pEndStep->v_internalRole = SSC_STEPROLE_END;

    //init variables
    pinst->v_workingState = SSC_WOST_INIT;
    pinst->v_actimode = FB_AM_ON;
    pinst->v_iexreq = TRUE;

    return OV_ERR_OK;
}

OV_DLLFNCEXPORT void ssc_sscHeader_typemethod(
		OV_INSTPTR_fb_functionblock	pfb,
		OV_TIME						*pltc
) {
    /*    
    *   local variables
    */
    OV_INSTPTR_ssc_sscHeader pinst = Ov_StaticPtrCast(ssc_sscHeader, pfb);
    //OV_INSTPTR_fb_functionchart pSscContainer = Ov_DynamicPtrCast(fb_functionchart, Ov_GetParent(ov_containment, pinst));
    //OV_INSTPTR_ssc_step      pInitStep = NULL;
    OV_INSTPTR_ssc_step      pActiveStep = NULL;
    //OV_INSTPTR_fb_task       pTask = NULL;
    OV_INSTPTR_fb_task 		 intask = &pinst->p_intask;
    //OV_INSTPTR_fb_task 		 pActiveStepExit = NULL;
    // helper vaiables
    OV_RESULT    			 result;
    OV_BOOL	                 exitLoop;

    // init variables
    pinst->v_error=FALSE;
    ov_string_setvalue(&pinst->v_errorDetail, NULL);
    intask->v_actimode = FB_AM_ON;
    intask->v_cyctime.secs = 0;
    intask->v_cyctime.usecs = 0;

    // check location
    result = ssc_sscHeader_checkLocation(pinst);
    if(Ov_Fail(result)){
    	return;
    }


    // find active step
    Ov_GetFirstChildEx(fb_tasklist, intask, pActiveStep, ssc_step);
    if (pActiveStep != NULL){
    	pActiveStep->v_actimode = FB_AM_OFF;
   		ov_string_setvalue(&pinst->v_activeStep, pActiveStep->v_identifier);
    }

    /*################################
    * state machine: working state
    #################################*/
    do{
    	exitLoop=TRUE;
    	switch (pinst->v_workingState)
    	{
    	/* step 0 init */
    	case SSC_WOST_INIT:
    		switch (pinst->v_woStPhase)
    		{
    		case SSC_PHASE_ENTRYDO:
    			/* entry: */
    			if (pinst->v_woStQualifier == SSC_QUALIFIER_ENTRY){
    				ov_string_setvalue(&pinst->v_woStText, "INIT");

    				// generic part
    				pinst->v_woStQualifier = SSC_QUALIFIER_DO;
    				//printf("init, entry\n");
    			}

    			/* do: */
    			//printf("init, do\n");
    			// reset ssc
    			result=Ov_Call0 (ssc_sscHeader, pinst, resetSsc);
    			if(Ov_Fail(result)) return;

    			// generic part
    			pinst->v_woStPhase = SSC_PHASE_EXITTRANS;
    		break;
    		case SSC_PHASE_EXITTRANS:
    			/* transitions */
    			//printf("init, transitions\n");
    			// INIT to START
    			if (pinst->v_EN == SSC_CMD_START){
    				pinst->v_workingState= SSC_WOST_START;
    			}

    			/* exit */
    			if (pinst->v_workingState != SSC_WOST_INIT){
    				// generic part
    				pinst->v_woStQualifier = SSC_QUALIFIER_ENTRY;
					//printf("init, exit\n");
				}

    			// generic part
    			pinst->v_woStPhase = SSC_PHASE_ENTRYDO;
    			exitLoop=FALSE;
    		break;
    		} // END switch woStPhase
    	break;
    	/* END step 0 init */

    	/* step 1 START */
        case SSC_WOST_START:
        	switch (pinst->v_woStPhase)
        	{
        	case SSC_PHASE_ENTRYDO:
        		/* entry: */
        		if (pinst->v_woStQualifier == SSC_QUALIFIER_ENTRY){
       				ov_string_setvalue(&pinst->v_woStText, "START");

       				// generic part
       				pinst->v_woStQualifier = SSC_QUALIFIER_DO;
       				//printf("start, entry\n");
       			}

       			/* do: */
       			//printf("start, do\n");
       			// activate active step
       			if (pActiveStep != NULL){
       				pActiveStep->v_actimode = FB_AM_ON;
       			}

       			// generic part
       			pinst->v_woStPhase = SSC_PHASE_EXITTRANS;
       		break;
       		case SSC_PHASE_EXITTRANS:
       			/* transitions */
       			//printf("start, transitions\n");

       			// START to BREAK
    			if (pinst->v_EN == SSC_CMD_BREAK){
    				pinst->v_workingState= SSC_WOST_BREAK;
    			}else
    			// START to STOP
    			if (pinst->v_EN == SSC_CMD_STOP){
    				pinst->v_workingState= SSC_WOST_STOP;
    			}

       			/* exit */
       			if (pinst->v_workingState != SSC_WOST_START){
      				// generic part
       				pinst->v_woStQualifier = SSC_QUALIFIER_ENTRY;
   					//printf("start, exit\n");
   				}

       			// generic part
               	pinst->v_woStPhase = SSC_PHASE_ENTRYDO;
               	exitLoop=FALSE;
       		break;
        	} // END switch woStPhase
        break;
        /* END step 1 START */

        /* step 2 BREAK */
        case SSC_WOST_BREAK:
        switch (pinst->v_woStPhase)
        {
           	case SSC_PHASE_ENTRYDO:
           		/* entry: */
           		if (pinst->v_woStQualifier == SSC_QUALIFIER_ENTRY){
           			// activate active step
           			if (pActiveStep != NULL){
           				pActiveStep->v_actimode = FB_AM_ON;
           			}

           			ov_string_setvalue(&pinst->v_woStText, "BREAK");

           			// generic part
           			pinst->v_woStQualifier = SSC_QUALIFIER_DO;
           			//printf("break, entry\n");
           		}

           		/* do: */
           		//printf("break, do\n");
           		// activate active step
           		if (pActiveStep != NULL){
           			pActiveStep->v_actimode = FB_AM_OFF;
           			pActiveStep->v_phase = SSC_PHASE_ENTRYDO;
           			pActiveStep->v_X=FALSE;
           		}

           		// generic part
           		pinst->v_woStPhase = SSC_PHASE_EXITTRANS;
           		break;
           	case SSC_PHASE_EXITTRANS:
           		/* transitions */
           		//printf("break, transitions\n");

           		// BREAK to STOP
           		if (pinst->v_EN == SSC_CMD_STOP){
           			pinst->v_workingState= SSC_WOST_STOP;
           		}else
           		// BREAK to START
           		if (pinst->v_EN == SSC_CMD_START){
           			pinst->v_workingState= SSC_WOST_START;
           		}

           		// generic part
           		if (pinst->v_workingState != SSC_WOST_BREAK){
           			pinst->v_woStQualifier = SSC_QUALIFIER_EXIT;
           		}

           		/* exit */
           		if (pinst->v_woStQualifier == SSC_QUALIFIER_EXIT){
           			// generic part
           			pinst->v_woStQualifier = SSC_QUALIFIER_ENTRY;
           			//printf("break, exit\n");
           		}

           	// generic part
           	pinst->v_woStPhase = SSC_PHASE_ENTRYDO;
           	exitLoop=FALSE;
           	break;
            } // END switch woStPhase
        break;
        /* END step 2 BREAK */

        /* step 3 STOP */
        case SSC_WOST_STOP:
        switch (pinst->v_woStPhase)
        {
           	case SSC_PHASE_ENTRYDO:
        	/* entry: */
        	if (pinst->v_woStQualifier == SSC_QUALIFIER_ENTRY){
        		// execute exit-actions of the end step
        		if (pActiveStep == NULL)					// step specific
        		{
        			// TODO: ERROR
        			return;
        		}
      		    //Ov_Call1 (fb_task, Ov_PtrUpCast(fb_task, pActiveStep), execute, pltc);
        		//Frage: Warum funktionier hier der Aufruf von execute nicht?
        		//Ov_Call1 (ssc_step, pActiveStep, typemethod, pltc);
        		Ov_Call1 (fb_functionblock, pActiveStep, typemethod, pltc);

        		ov_string_setvalue(&pinst->v_woStText, "STOP");

        		// generic part
        		pinst->v_woStQualifier = SSC_QUALIFIER_DO;
        		//printf("stop, entry\n");
        	}

        	/* do: */
        	//printf("stop, do\n");
        	// reset ssc
        	result=Ov_Call0 (ssc_sscHeader, pinst, resetSsc);
        	if(Ov_Fail(result)) return;

        	// generic part
        	pinst->v_woStPhase = SSC_PHASE_EXITTRANS;
        	break;
       	case SSC_PHASE_EXITTRANS:
        	/* transitions */
           	//printf("stop, transitions\n");

           	// STOP to START
        	if (pinst->v_EN == SSC_CMD_START){
        		pinst->v_workingState= SSC_WOST_START;
        	}

        	// generic part
           	if (pinst->v_workingState != SSC_WOST_STOP){
           		pinst->v_woStQualifier = SSC_QUALIFIER_EXIT;
           	}

           	/* exit */
           	if (pinst->v_woStQualifier == SSC_QUALIFIER_EXIT){
           		// generic part
           		pinst->v_woStQualifier = SSC_QUALIFIER_ENTRY;
       		}

           	// generic part
           	pinst->v_woStPhase = SSC_PHASE_ENTRYDO;
           	exitLoop = FALSE;
           	break;
            } // END switch woStPhase
        break;
        /* END step 3 STOP */


    	}
    } while (!exitLoop);
    /* END: state machine: working state
    #################################*/



    /* Execute internal task */
    Ov_Call1 (fb_task, intask, execute, pltc);

    return;
}

OV_DLLFNCEXPORT OV_RESULT ssc_sscHeader_resetSsc(
	OV_INSTPTR_ssc_sscHeader    pinst
) {
	OV_INSTPTR_ssc_step      pStep = NULL;
	OV_INSTPTR_ssc_step      pInitStep = NULL;
	OV_INSTPTR_fb_task 		 intask = &pinst->p_intask;
	OV_INSTPTR_fb_functionblock pFbAction=NULL;
	OV_INSTPTR_fb_task       pTask = NULL;
	OV_INSTPTR_ssc_sscHeader pSscAction = NULL;
	OV_RESULT result = OV_ERR_OK;
    //OV_TIME *pTime;
    //ov_time_gettime(&pTime);

    //reset all steps; find and link INIT-step to intask
	  Ov_ForEachChildEx(ov_containment, pinst, pStep, ssc_step){
		  result=Ov_Call0(ssc_step, pStep, resetStep);

		  if(pStep->v_internalRole == SSC_STEPROLE_START){
			  pInitStep=pStep;
		  }
	  }
	  if (pInitStep == NULL){
		  pinst->v_error=TRUE;
		  ov_string_setvalue(&pinst->v_errorDetail, "no INIT-step is defined");
		  return OV_ERR_BADPATH;
	  }
	  result=Ov_Link(fb_tasklist, intask,  pInitStep);
	  if(Ov_Fail(result)){
		  return result;
	  }

	  // reset all actions (FB, CFC or SSC) under .actions folder
	  Ov_ForEachChildEx(ov_containment, &pinst->p_actions, pFbAction, fb_functionblock){
		  // unlink from task parent
		  pTask = Ov_GetParent(fb_tasklist, pFbAction);
		  if (pTask != NULL){
			  Ov_Unlink(fb_tasklist, pTask, pFbAction);
		  }

		  // reset parameters
		  pFbAction->v_actimode = FB_AM_OFF;
		  pFbAction->v_cyctime.secs = 0;
		  pFbAction->v_cyctime.usecs = 0;
		  pFbAction->v_iexreq = TRUE;

		  // reset SSC action
		  pSscAction=Ov_DynamicPtrCast(ssc_sscHeader,pFbAction);
		  if (pSscAction != NULL){
			  pSscAction->v_actimode = FB_AM_ON;
			  pSscAction->v_EN=SSC_CMD_STOP;
			  //Ov_Call1 (fb_task, Ov_PtrUpCast(fb_task, pSscAction), execute, &pTime);
			  result = Ov_Call0 (ssc_sscHeader, pSscAction, resetSsc);
			  if(Ov_Fail(result)){
				  return result;
			  }
			  pSscAction->v_actimode = FB_AM_OFF;
		  }
	  }

	  // TODO: reset all transition conditions under .transConds folder
	  Ov_ForEachChildEx(ov_containment, &pinst->p_transConds, pFbAction, fb_functionblock){
	  		  // unlink from task parent
	  		  OV_INSTPTR_fb_task pTask = Ov_GetParent(fb_tasklist, pFbAction);
	  		  if (pTask != NULL){
	  			  Ov_Unlink(fb_tasklist, pTask, pFbAction);
	  		  }

	  		  // reset parameters
	  		  pFbAction->v_actimode = FB_AM_OFF;
	  		  pFbAction->v_cyctime.secs = 0;
	  		  pFbAction->v_cyctime.usecs = 0;
	  		  pFbAction->v_iexreq = TRUE;
	  	  }
	  //printf("%s: reset all steps END\n", pinst->v_identifier);

	  return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_ACCESS ssc_sscHeader_getaccess(
	OV_INSTPTR_ov_object	pobj,
	const OV_ELEMENT		*pelem,
	const OV_TICKET			*pticket
) {
	/*
	*   local variables
	*/
	OV_INSTPTR_ssc_sscHeader activeHeader = Ov_StaticPtrCast(ssc_sscHeader, pobj);

	OV_ACCESS access_code = fb_functionblock_getaccess(pobj, pelem, pticket);
	/*
	*	switch based on the element's type
	*/
	switch(pelem->elemtype) {
		case OV_ET_OBJECT:
			if(!activeHeader){
				//skip handling
			}else if(	activeHeader->v_error == TRUE ||
						activeHeader->v_workingState == SSC_WOST_INIT ||
						activeHeader->v_workingState == SSC_WOST_STOP ||
						activeHeader->v_workingState == SSC_WOST_TERMINATE)
			{
				//allow deletion
				access_code = (access_code | OV_AC_DELETEABLE);
			}else{
				//disallow deletion
				access_code = (access_code &~ OV_AC_DELETEABLE);
			}
			break;
		default:
			break;
	}
	return access_code;
}
