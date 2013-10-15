/*
*   The Framework was generated by the ACPLT/OV Builder.
*
*   Builder version: 1.0.4
*   Date of file generation:  15-Jul-2011 (13:34:12)
*/

#ifndef OV_COMPILE_LIBRARY_sfc
#define OV_COMPILE_LIBRARY_sfc
#endif

/***********************************************************************
	Debug & Error
***********************************************************************/

#ifdef SFC_ERROR
#define SFC_EXECUTEFB_ERROR
#endif

#ifdef SFC_DEBUG
#define SFC_EXECUTEFB_DEBUG
#endif


#include "sfc.h"
#include "sfclib.h"

OV_DLLFNCEXPORT void sfc_executeFb_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
    /*
    *   local variables
    */
    OV_INSTPTR_sfc_executeFb pinst = Ov_StaticPtrCast(sfc_executeFb, pfb);

    OV_INSTPTR_sfc_step  		pStep= Ov_DynamicPtrCast(sfc_step, Ov_GetParent(ov_containment, pinst));
    OV_INSTPTR_sfc_sfcHeader  	pSFC= Ov_DynamicPtrCast(sfc_sfcHeader, Ov_GetParent(ov_containment, pStep));
    OV_INSTPTR_fb_functionblock pAction= Ov_DynamicPtrCast(fb_functionblock, Ov_GetParent(sfc_actionBlocks, pinst));
    OV_INSTPTR_sfc_sfcHeader  	pSfcAction=NULL;
    OV_INSTPTR_fb_task 			pTask=NULL;
    OV_RESULT    			 result;

    // init parameters
    pinst->v_cyctime.secs = 0;
    pinst->v_cyctime.usecs = 0;
    pinst->v_iexreq = 1;
    pinst->v_error=0;
    ov_string_setvalue(&pinst->v_errorDetail, NULL);

    // check location
    if (pSFC==NULL)
    {
      	pinst->v_error=TRUE;
      	ov_string_setvalue(&pinst->v_errorDetail, "wrong placement");
    	ov_logfile_error("sfc_executeFb_typemethod: action block must be placed in a step.");
       	return;
    }

    // check action
    result=Ov_Call0 (sfc_executeFb, pinst, checkAction);
    if(Ov_Fail(result)) return;

	// execute action for once
	Ov_Call1 (fb_task, Ov_PtrUpCast(fb_task, pAction), execute, pltc);

	// if subSFC terminates, then activate trans-task, else not
	if (pSfcAction != NULL)
	{
		pTask=&pStep->p_trans;
		if (pSfcAction->v_workingState == WOST_TERMINATE)
		{
			pTask->v_actimode=1;
		} else {
			pTask->v_actimode=0;
		}


	}

	return;
}

OV_DLLFNCEXPORT OV_RESULT sfc_executeFb_checkAction(
             OV_INSTPTR_sfc_executeFb          pinst
) {

	OV_INSTPTR_fb_functionblock pAction= Ov_DynamicPtrCast(fb_functionblock, Ov_GetParent(sfc_actionBlocks, pinst));
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
	pAction->v_actimode = 1;
	pAction->v_cyctime.secs = 0;
	pAction->v_cyctime.usecs = 0;
	pAction->v_iexreq = 1;

	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT sfc_executeFb_setActionName(
             OV_INSTPTR_sfc_actionBlock                pobj,
             const OV_STRING  value
) {
    //  local variables
    OV_INSTPTR_sfc_executeSfc pinst = Ov_StaticPtrCast(sfc_executeSfc, pobj);
    OV_INSTPTR_sfc_step  		pStep= Ov_DynamicPtrCast(sfc_step, Ov_GetParent(ov_containment, pinst));
    OV_INSTPTR_sfc_sfcHeader  	pSFC= Ov_DynamicPtrCast(sfc_sfcHeader, Ov_GetParent(ov_containment, pStep));
    OV_INSTPTR_fb_functionblock pAction=NULL;
    OV_INSTPTR_fb_functionblock pCurrentAction=NULL;
    //OV_INSTPTR_sfc_sfcHeader  	pSfcAction=NULL;
    //OV_RESULT    result;

    // init parameters
    pinst->v_cyctime.secs = 0;
    pinst->v_cyctime.usecs = 0;
    pinst->v_iexreq = 1;
    pinst->v_error=0;
    ov_string_setvalue(&pinst->v_errorDetail, NULL);

    // check location
    if (pSFC==NULL)
    {
      	pinst->v_error=TRUE;
      	ov_string_setvalue(&pinst->v_errorDetail, "wrong placement");
    	ov_logfile_error("sfc_executeFb_actionName_set: action block must be encapsulated in a step.");
       	return OV_ERR_BADPLACEMENT;
    }

    // check action name definition
    if (ov_string_compare(value, NULL) == 0)
    {
    	pinst->v_error=TRUE;
    	ov_string_setvalue(&pinst->v_errorDetail, "action is not defined");
    	return OV_ERR_BADPARAM;
    }

	//find current action
    pCurrentAction = Ov_GetParent(sfc_actionBlocks, pinst);

    // find new action instance
    pAction=Ov_SearchChildEx(ov_containment, &pSFC->p_actions, value, fb_functionblock);

    if (pAction == NULL)
    {
		//#ifdef SFC_EXECUTEFB_ERROR
		ov_logfile_error("%s : action instance can not be located.", pinst->v_identifier);
		//#endif

    	if ( pCurrentAction == NULL )
    	{
    		pinst->v_error=TRUE;
    		ov_string_setvalue(&pinst->v_errorDetail, "action can not be located");
    		return ov_string_setvalue(&pinst->v_actionName,value);
    	}
	}

    if ( (pCurrentAction != NULL) && (pAction == NULL) )
	{
	    return OV_ERR_OK;
	}

    if (pAction != NULL)
	{
    	// unlink
    	if (pCurrentAction != NULL) Ov_Unlink(sfc_actionBlocks, pCurrentAction, pinst);
    	// link
    	Ov_Link(sfc_actionBlocks, pAction, pinst);
    	return ov_string_setvalue(&pinst->v_actionName,value);
	}
	
 return OV_ERR_OK;
}

