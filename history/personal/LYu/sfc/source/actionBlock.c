
/******************************************************************************
*
*   FILE
*   ----
*   sfcActionBlock.c
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


#include "sfc.h"
#include "sfclib.h"

OV_DLLFNCEXPORT OV_RESULT sfc_actionBlock_actionQualifier_set(
    OV_INSTPTR_sfc_actionBlock          pinst,
    const OV_UINT  value
) {
    OV_INSTPTR_fb_task pTaskParent = Ov_GetParent(fb_tasklist, pinst);
    OV_INSTPTR_sfc_step  pStep= Ov_DynamicPtrCast(sfc_step, Ov_GetParent(ov_containment, pinst));
    OV_RESULT    result;

    if (pStep==NULL)
    {
    	ov_logfile_error("sfc_actionBlock_actionQualifier_set: action block must be encapsulated in a step.");
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

OV_DLLFNCEXPORT OV_RESULT sfc_actionBlock_actionName_set(
    OV_INSTPTR_sfc_actionBlock          pinst,
    const OV_STRING  value
) {
	return Ov_Call1 (sfc_actionBlock, pinst, setActionName, value);
}

OV_DLLFNCEXPORT OV_RESULT sfc_actionBlock_constructor(
	OV_INSTPTR_ov_object 	pobj
) {
    /*
    *   local variables
    */
    OV_INSTPTR_sfc_actionBlock pinst = Ov_StaticPtrCast(sfc_actionBlock, pobj);
    OV_INSTPTR_sfc_step  pStep= Ov_DynamicPtrCast(sfc_step, Ov_GetParent(ov_containment, pinst));
    OV_RESULT    result;

    /* do what the base class does first */
    result = fb_functionblock_constructor(pobj);
    if(Ov_Fail(result))
         return result;

    /* do what */
    pinst->v_cyctime.secs = 0;
    pinst->v_cyctime.usecs = 0;
    pinst->v_iexreq = 1;

    // check location
    if ( pStep==NULL )
	{
		ov_logfile_error("sfc_actionBlock_constructor: action block must be encapsulated in a step.");
		return OV_ERR_BADPLACEMENT;
	}

    // link to subtask
    result=sfc_actionBlock_actionQualifier_set(pinst, pinst->v_actionQualifier);
    // activate itself
    pinst->v_actimode = 1;

    return OV_ERR_OK;
}

/*
OV_DLLFNCEXPORT OV_RESULT sfc_actionBlock_checkPlausibility(
             OV_INSTPTR_sfc_actionBlock          pobj
) {
             return OV_ERR_OK;
}
*/
/*
OV_DLLFNCEXPORT OV_RESULT sfc_actionBlock_setActionName(
             OV_INSTPTR_sfc_actionBlock          pinst,
             const OV_STRING  value
) {
             return ov_string_setvalue(&pinst->v_actionName,value);
}
*/


