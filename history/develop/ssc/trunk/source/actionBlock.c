
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


#include "ssc.h"
#include "ssclib.h"

OV_DLLFNCEXPORT OV_RESULT ssc_actionBlock_actionQualifier_set(
    OV_INSTPTR_ssc_actionBlock          pinst,
    const OV_UINT  value
) {
    OV_INSTPTR_fb_task pTaskParent = Ov_GetParent(fb_tasklist, pinst);
    OV_INSTPTR_ssc_step  pStep= Ov_DynamicPtrCast(ssc_step, Ov_GetParent(ov_containment, pinst));
    OV_RESULT    result;

    if (pStep==NULL)
    {
    	ov_logfile_error("ssc_actionBlock_actionQualifier_set: action block must be encapsulated in a step.");
    	return OV_ERR_BADPLACEMENT;
    }

	switch (value)
	{
	case ACT_ENTRY:
		//unlink from current task parent
		if (pTaskParent != NULL)	Ov_Unlink(fb_tasklist, pTaskParent, pinst);
		//link to entry task
		result=Ov_Link(fb_tasklist, &pStep->p_entry, pinst);
		if(Ov_Fail(result))
			return result;
		pinst->v_actionQualifier = value;
		break;
	case ACT_DO:
		//unlink from current task parent
		if (pTaskParent != NULL)	Ov_Unlink(fb_tasklist, pTaskParent, pinst);
		//link to do task
		result=Ov_Link(fb_tasklist, &pStep->p_do, pinst);
		if(Ov_Fail(result))
			return result;
		pinst->v_actionQualifier = value;
		break;
	case ACT_EXIT:
		//unlink from current task parent
		if (pTaskParent != NULL)	Ov_Unlink(fb_tasklist, pTaskParent, pinst);
		//link to entry task
		result=Ov_Link(fb_tasklist, &pStep->p_exit, pinst);
		if(Ov_Fail(result))
			return result;
		pinst->v_actionQualifier = value;
		break;
	}

    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT ssc_actionBlock_actionName_set(
    OV_INSTPTR_ssc_actionBlock          pinst,
    const OV_STRING  value
) {
	if(ov_string_compare(value, "") == OV_STRCMP_EQUAL){
		//allow INITIAL_VALUE for loading an backup
		return OV_ERR_OK;
	}

	return Ov_Call1 (ssc_actionBlock, pinst, setActionName, value);
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
    if ( pStep==NULL )
	{
		ov_logfile_error("ssc_actionBlock_constructor: action block must be encapsulated in a step.");
		return OV_ERR_BADPLACEMENT;
	}

    // link to subtask
    result=ssc_actionBlock_actionQualifier_set(pinst, pinst->v_actionQualifier);
    result = 0;
    /* do what */
    pinst->v_cyctime.secs = 0;
    pinst->v_cyctime.usecs = 0;
    pinst->v_iexreq = TRUE;




    // activate itself
    pinst->v_actimode = FB_AM_ON;

    return OV_ERR_OK;
}