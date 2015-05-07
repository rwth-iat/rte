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
#define SSC_EXECUTE_ERROR
#endif

#ifdef SSC_DEBUG
#define SSC_EXECUTE_DEBUG
#endif

#include "ssclib.h"

/**
 * gets an object pointer from a execute object
 * @param pinst
 * @param nameToCheck
 * @param pTargetObj will be NULL if object is not found
 * @return
 */
OV_RESULT ssc_getObjectFromExecute(
		const OV_INSTPTR_ssc_execute pinst,
		const OV_STRING nameToCheck,
		OV_INSTPTR_fb_functionblock *pTargetObj
) {
	OV_STRING pathRelativeobject = NULL;
	OV_INSTPTR_ssc_step pStep = Ov_DynamicPtrCast(ssc_step, Ov_GetParent(ov_containment, pinst));
	OV_INSTPTR_ssc_SequentialControlChart activeHeader = Ov_DynamicPtrCast(ssc_SequentialControlChart, Ov_GetParent(ov_containment, pStep));
	OV_INSTPTR_ov_domain containerDomain = NULL;

	*pTargetObj = NULL;
	if(nameToCheck == NULL){
			return OV_ERR_BADPARAM;
	}else if(nameToCheck[0] == '/'){
		//we have a full path
		*pTargetObj = Ov_DynamicPtrCast(fb_functionblock, ov_path_getobjectpointer(nameToCheck, 2));
	}else{
		//we have a relative path
		ov_memstack_lock();
		//all path are relative to the activeHeader
		containerDomain = Ov_PtrUpCast(ov_domain, activeHeader);
		ov_string_setvalue(&pathRelativeobject, ov_path_getcanonicalpath(Ov_PtrUpCast(ov_object, containerDomain), 2));
		*pTargetObj = Ov_DynamicPtrCast(fb_functionblock, getrelativeobjectpointer(pathRelativeobject, nameToCheck));
		if(*pTargetObj == NULL){
			//perhaps the target is in the actions PART of the header
			containerDomain = Ov_GetPartPtr(actions, activeHeader);
			ov_string_setvalue(&pathRelativeobject, ov_path_getcanonicalpath(Ov_PtrUpCast(ov_object, containerDomain), 2));
			*pTargetObj = Ov_DynamicPtrCast(fb_functionblock, getrelativeobjectpointer(pathRelativeobject, nameToCheck));
		}
		ov_memstack_unlock();
	}
	ov_string_setvalue(&pathRelativeobject, NULL);

	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT ssc_execute_targetObject_set(
		OV_INSTPTR_ssc_execute          pinst,
		const OV_STRING  value
) {
	OV_RESULT result = OV_ERR_OK;
	result = ov_string_setvalue(&pinst->v_targetObject, value);
	if(Ov_OK(result)){
		//check this action. on loading of a FBD it is invalid till everything is loaded
		ssc_execute_checkAction(Ov_PtrUpCast(ssc_actionBlock, pinst));
	}
	return result;
}


OV_DLLFNCEXPORT void ssc_execute_typemethod(
		OV_INSTPTR_fb_functionblock	pfb,
		OV_TIME						*pltc
) {
	/*
	 *   local variables
	 */
	OV_INSTPTR_ssc_execute pinst = Ov_StaticPtrCast(ssc_execute, pfb);

	OV_INSTPTR_ssc_SequentialControlChart  	pTargetSequentialControlChart = NULL;
	OV_INSTPTR_fb_functionblock pTargetObj = NULL;
	OV_INT targetActimode = FB_AM_ON;
	OV_INT targetCyctimeSecs = 0;
	OV_INT targetCyctimeUsecs = 0;
	OV_BOOL targetIexreq = TRUE;

	// check config
	if (ssc_execute_checkAction(Ov_PtrUpCast(ssc_actionBlock, pinst)) == FALSE){
		return;
	}

	//FIXME remove double find
	ssc_getObjectFromExecute(pinst, pinst->v_targetObject, &pTargetObj);

	//save config and initialise targetObject
	targetActimode = pTargetObj->v_actimode;
	targetCyctimeSecs = pTargetObj->v_cyctime.secs;
	targetCyctimeUsecs = pTargetObj->v_cyctime.usecs;
	targetIexreq = pTargetObj->v_iexreq;
	pTargetObj->v_actimode = FB_AM_ONCE;
	pTargetObj->v_cyctime.secs = 0;
	pTargetObj->v_cyctime.usecs = 0;
	pTargetObj->v_iexreq = TRUE;

	if(Ov_CanCastTo(ssc_SequentialControlChart, pTargetObj)){
		pTargetSequentialControlChart = Ov_StaticPtrCast(ssc_SequentialControlChart, pTargetObj);
		// activate SSC
		pTargetSequentialControlChart->v_EN = SSC_CMD_START;
	}
	// execute action for once
	Ov_Call1 (fb_task, Ov_PtrUpCast(fb_task, pTargetObj), execute, pltc);

	//restore config of targetObject
	pTargetObj->v_actimode = targetActimode;
	pTargetObj->v_cyctime.secs = targetCyctimeSecs;
	pTargetObj->v_cyctime.usecs = targetCyctimeUsecs;
	pTargetObj->v_iexreq = targetIexreq;

	return;
}

OV_DLLFNCEXPORT void ssc_execute_destructor(
		OV_INSTPTR_ov_object 	pobj
) {
	/* destroy object */
	fb_functionblock_destructor(pobj);

	return;
}

OV_DLLFNCEXPORT OV_BOOL ssc_execute_checkAction(
		OV_INSTPTR_ssc_actionBlock	pActionBlock
) {
	/*
	 *   local variables
	 */
	OV_INSTPTR_fb_functionblock pTargetObj = NULL;
	OV_INSTPTR_ssc_execute pinst = Ov_StaticPtrCast(ssc_execute, pActionBlock);
	OV_INSTPTR_ssc_step pStep = Ov_DynamicPtrCast(ssc_step, Ov_GetParent(ov_containment, pinst));

	// check location
	if (pStep == NULL){
		pinst->v_error = TRUE;
		ov_string_setvalue(&pinst->v_errorDetail, "This ActionBlock is placed wrong.");
		return FALSE;
	}
	// check input
	if (ov_string_compare(pinst->v_targetObject, NULL) == OV_STRCMP_EQUAL){
		pinst->v_error=TRUE;
		ov_string_setvalue(&pinst->v_errorDetail, "Object to execute is not defined.");
		return FALSE;
	};

	ssc_getObjectFromExecute(pinst, pinst->v_targetObject, &pTargetObj);
	if(pTargetObj == NULL){
		pinst->v_error = TRUE;
		ov_string_setvalue(&pinst->v_errorDetail, "Configured Object not found.");
		return FALSE;
	}

	pinst->v_error = FALSE;
	ov_string_setvalue(&pinst->v_errorDetail, NULL);
	return TRUE;
}
