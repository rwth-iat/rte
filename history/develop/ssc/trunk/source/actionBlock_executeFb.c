/*
*   The Framework was generated by the ACPLT/OV Builder.
*
*   Builder version: 1.0.4
*   Date of file generation:  15-Jul-2011 (13:34:12)
*/

#ifndef OV_COMPILE_LIBRARY_ssc
#define OV_COMPILE_LIBRARY_ssc
#endif

/***********************************************************************
	Debug & Error
***********************************************************************/

#ifdef SSC_ERROR
#define SSC_EXECUTEFB_ERROR
#endif

#ifdef SSC_DEBUG
#define SSC_EXECUTEFB_DEBUG
#endif


#include "ssc.h"
#include "ssclib.h"

OV_DLLFNCEXPORT void ssc_executeFb_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
    /*
    *   local variables
    */
    OV_INSTPTR_ssc_executeFb pinst = Ov_StaticPtrCast(ssc_executeFb, pfb);

    OV_INSTPTR_ssc_step  		pStep= Ov_DynamicPtrCast(ssc_step, Ov_GetParent(ov_containment, pinst));
    OV_INSTPTR_ssc_sscHeader  	pSSC= Ov_DynamicPtrCast(ssc_sscHeader, Ov_GetParent(ov_containment, pStep));
    OV_INSTPTR_fb_functionblock pAction= Ov_DynamicPtrCast(fb_functionblock, Ov_GetParent(ssc_actionBlocks, pinst));
    OV_INSTPTR_ssc_sscHeader  	pSscAction=NULL;
    OV_INSTPTR_fb_task 			pTask=NULL;
    OV_RESULT    			 result;

    // init parameters
    pinst->v_cyctime.secs = 0;
    pinst->v_cyctime.usecs = 0;
    pinst->v_iexreq = TRUE;
    pinst->v_error=FALSE;
    ov_string_setvalue(&pinst->v_errorDetail, NULL);

    // check location
    if (pSSC==NULL)
    {
      	pinst->v_error=TRUE;
      	ov_string_setvalue(&pinst->v_errorDetail, "wrong placement");
    	ov_logfile_error("ssc_executeFb_typemethod: action block must be placed in a step.");
       	return;
    }

    // check action
    result=Ov_Call0 (ssc_executeFb, pinst, checkAction);
    if(Ov_Fail(result)) return;

	// execute action for once
	Ov_Call1 (fb_task, Ov_PtrUpCast(fb_task, pAction), execute, pltc);

	// if subSSC terminates, then activate trans-task, else not
	if (pSscAction != NULL)
	{
		pTask=&pStep->p_trans;
		if (pSscAction->v_workingState == WOST_TERMINATE)
		{
			pTask->v_actimode = FB_AM_ON;
		} else {
			pTask->v_actimode = FB_AM_OFF;
		}


	}

	return;
}

OV_DLLFNCEXPORT OV_RESULT ssc_executeFb_checkAction(
             OV_INSTPTR_ssc_executeFb          pinst
) {

	OV_INSTPTR_fb_functionblock pAction= Ov_DynamicPtrCast(fb_functionblock, Ov_GetParent(ssc_actionBlocks, pinst));
	OV_INSTPTR_fb_task 			pTask=NULL;


	// check action name
	if (ov_string_compare(pinst->v_actionName, NULL) == 0)
	    {
	    	pinst->v_error=TRUE;
	    	ov_string_setvalue(&pinst->v_errorDetail, "action instance is not defined");
	    	return OV_ERR_BADPARAM;
	    }

	// find action block
	if (pAction ==NULL)
	{
	    pinst->v_error=TRUE;
	    ov_string_setvalue(&pinst->v_errorDetail, "action instance can not be located");
	    return OV_ERR_NOACCESS;
	}

	// unlink, if action has task parent
	pTask = Ov_GetParent(fb_tasklist, pAction);
    if (pTask != NULL)	Ov_Unlink(fb_tasklist, pTask, pAction);

    // init parameters
	pAction->v_actimode = FB_AM_ON;
	pAction->v_cyctime.secs = 0;
	pAction->v_cyctime.usecs = 0;
	pAction->v_iexreq = TRUE;

	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT ssc_executeFb_setActionName(
             OV_INSTPTR_ssc_actionBlock                pobj,
             const OV_STRING  value
) {
    //  local variables
    OV_INSTPTR_ssc_executeSsc pinst = Ov_StaticPtrCast(ssc_executeSsc, pobj);
    OV_INSTPTR_ssc_step  		pStep= Ov_DynamicPtrCast(ssc_step, Ov_GetParent(ov_containment, pinst));
    OV_INSTPTR_ssc_sscHeader  	pSSC= Ov_DynamicPtrCast(ssc_sscHeader, Ov_GetParent(ov_containment, pStep));
    OV_INSTPTR_fb_functionblock pAction=NULL;
    OV_INSTPTR_fb_functionblock pCurrentAction=NULL;
    //OV_INSTPTR_ssc_sscHeader  	pSscAction=NULL;
    //OV_RESULT    result;

    // init parameters
    pinst->v_cyctime.secs = 0;
    pinst->v_cyctime.usecs = 0;
    pinst->v_iexreq = TRUE;
    pinst->v_error=FALSE;
    ov_string_setvalue(&pinst->v_errorDetail, NULL);

    // check location
    if (pSSC==NULL)
    {
      	pinst->v_error=TRUE;
      	ov_string_setvalue(&pinst->v_errorDetail, "wrong placement");
    	ov_logfile_error("ssc_executeFb_actionName_set: action block must be encapsulated in a step.");
       	return OV_ERR_BADPLACEMENT;
    }

    // check action name definition
    if (ov_string_compare(value, NULL) == OV_STRCMP_EQUAL)
    {
    	pinst->v_error=TRUE;
    	ov_string_setvalue(&pinst->v_errorDetail, "action is not defined");
    	return OV_ERR_BADPARAM;
    }

	//find current action
    pCurrentAction = Ov_GetParent(ssc_actionBlocks, pinst);

    // find new action instance
    pAction=Ov_SearchChildEx(ov_containment, &pSSC->p_actions, value, fb_functionblock);

    if (pAction == NULL)
    {
		//#ifdef SSC_EXECUTEFB_ERROR
		ov_logfile_error("%s : action instance can not be located.", pinst->v_identifier);
		//#endif

		pinst->v_error=TRUE;
		ov_string_setvalue(&pinst->v_errorDetail, "action can not be located");
    	if ( pCurrentAction == NULL )
    	{
    		//resetting actionName just to be sure
    		ov_string_setvalue(&pinst->v_actionName, NULL);
    	}
		return OV_ERR_BADPARAM;
	}
    else
	{
    	//activate new action

    	// unlink old action
    	if (pCurrentAction != NULL)
    	{
    		Ov_Unlink(ssc_actionBlocks, pCurrentAction, pinst);
    	}
    	// link new action
    	Ov_Link(ssc_actionBlocks, pAction, pinst);
    	return ov_string_setvalue(&pinst->v_actionName,value);
	}
	
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_ACCESS ssc_executeFb_getaccess(
	OV_INSTPTR_ov_object	pobj,
	const OV_ELEMENT		*pelem,
	const OV_TICKET			*pticket
) {
	/*
	*   local variables
	*/
	OV_INSTPTR_ssc_step pStep = Ov_DynamicPtrCast(ssc_step, Ov_GetParent(ov_containment, pobj));
	OV_INSTPTR_ssc_sscHeader activeHeader= Ov_DynamicPtrCast(ssc_sscHeader, Ov_GetParent(ov_containment, pStep));
	OV_ACCESS access_code = fb_functionblock_getaccess(pobj, pelem, pticket);

	/*
	*	switch based on the element's type
	*/
	switch(pelem->elemtype) {
		case OV_ET_OBJECT:
			if(!activeHeader){
				//skip handling
			}else if(	activeHeader->v_error == TRUE ||
						activeHeader->v_workingState == WOST_INIT ||
						activeHeader->v_workingState == WOST_STOP ||
						activeHeader->v_workingState == WOST_TERMINATE)
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
