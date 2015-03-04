
/******************************************************************************
*
*   FILE
*   ----
*   sscActionBlock.c
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


#include "ssclib.h"

OV_DLLFNCEXPORT OV_RESULT ssc_actionBlock_actionQualifier_set(
		OV_INSTPTR_ssc_actionBlock          pinst,
		const OV_UINT  value
) {
	OV_INSTPTR_fb_task pTaskParent = Ov_GetParent(fb_tasklist, pinst);
	OV_INSTPTR_ssc_step  pStep= Ov_DynamicPtrCast(ssc_step, Ov_GetParent(ov_containment, pinst));
	OV_RESULT    result;

	if (pStep==NULL){
		ov_logfile_error("ssc_actionBlock_actionQualifier_set: action block must be encapsulated in a step.");
		return OV_ERR_BADPLACEMENT;
	}

	switch (value)
	{
	case SSC_QUALIFIER_ENTRY:
		//unlink from current task parent
		if (pTaskParent != NULL){
			Ov_Unlink(fb_tasklist, pTaskParent, pinst);
		}
		//link to entry task
		result=Ov_Link(fb_tasklist, Ov_GetPartPtr(entry, pStep), pinst);
		if(Ov_Fail(result))
			return result;
		pinst->v_actionQualifier = value;
		break;
	case SSC_QUALIFIER_DO:
		//unlink from current task parent
		if (pTaskParent != NULL){
			Ov_Unlink(fb_tasklist, pTaskParent, pinst);
		}
		//link to do task
		result=Ov_Link(fb_tasklist, Ov_GetPartPtr(do, pStep), pinst);
		if(Ov_Fail(result))
			return result;
		pinst->v_actionQualifier = value;
		break;
	case SSC_QUALIFIER_EXIT:
		//unlink from current task parent
		if (pTaskParent != NULL){
			Ov_Unlink(fb_tasklist, pTaskParent, pinst);
		}
		//link to entry task
		result=Ov_Link(fb_tasklist, Ov_GetPartPtr(exit, pStep), pinst);
		if(Ov_Fail(result))
			return result;
		pinst->v_actionQualifier = value;
		break;
	default:
		return OV_ERR_BADPARAM;
	}

	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT ssc_actionBlock_constructor(
		OV_INSTPTR_ov_object 	pobj
) {
	/*
	 *   local variables
	 */
	OV_INSTPTR_ssc_actionBlock pinst = Ov_StaticPtrCast(ssc_actionBlock, pobj);
	OV_INSTPTR_ssc_step  pStep= Ov_DynamicPtrCast(ssc_step, Ov_GetParent(ov_containment, pinst));
	OV_RESULT    result;

	/* do what the base class does first */
	result = fb_functionblock_constructor(pobj);
	if(Ov_Fail(result))
		return result;

	// check location
	if ( pStep==NULL ){
		ov_logfile_error("ssc_actionBlock_constructor: action block must be encapsulated in a step.");
		return OV_ERR_BADPLACEMENT;
	}

	// link to corresponding taskparent
	//todo bald haben wir nur noch einen task!
	ssc_actionBlock_actionQualifier_set(pinst, pinst->v_actionQualifier);

	pinst->v_cyctime.secs = 0;
	pinst->v_cyctime.usecs = 0;
	pinst->v_iexreq = TRUE;

	// activate itself
	pinst->v_actimode = FB_AM_ON;

	return OV_ERR_OK;
}


/**
 * disallow deletion in run state
 */
OV_DLLFNCEXPORT OV_ACCESS ssc_actionBlock_getaccess(
	OV_INSTPTR_ov_object	pobj,
	const OV_ELEMENT		*pelem,
	const OV_TICKET			*pticket
) {
	/*
	*   local variables
	*/
	OV_INSTPTR_ssc_step pStep = Ov_DynamicPtrCast(ssc_step, Ov_GetParent(ov_containment, pobj));
	OV_INSTPTR_ssc_SequentialControlChart activeHeader= Ov_DynamicPtrCast(ssc_SequentialControlChart, Ov_GetParent(ov_containment, pStep));
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
