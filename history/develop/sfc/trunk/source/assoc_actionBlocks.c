
/******************************************************************************
*
*   FILE
*   ----
*   actionBlocks.c
*
*   History
*   -------
*   2011-03-08   File created
*
*******************************************************************************/


#ifndef OV_COMPILE_LIBRARY_ssc
#define OV_COMPILE_LIBRARY_ssc
#endif



#include "ssc.h"
#include "ssclib.h"
#include "libov/ov_association.h"


/***********************************************************************
	Link
***********************************************************************/

OV_DLLFNCEXPORT OV_RESULT ssc_actionBlocks_link
(	const OV_PPT_ssc_actionBlocks	pparent,
	const OV_CPT_ssc_actionBlocks	pchild,
	const OV_PLACEMENT_HINT			parenthint,
	const OV_CPT_ssc_actionBlocks	prelparent,
	const OV_PLACEMENT_HINT			childhint,
	const OV_CPT_ssc_actionBlocks	prelchild )
{

	//	local Pointers
	OV_INSTPTR_fb_functionblock pAction = Ov_StaticPtrCast(fb_functionblock, pparent);
	OV_INSTPTR_ssc_executeFb    pActionBlock = Ov_StaticPtrCast(ssc_executeFb, pchild);
	OV_INSTPTR_ssc_sscHeader    pSscAction  = Ov_DynamicPtrCast(ssc_sscHeader, pAction);
	OV_INSTPTR_ssc_executeSsc   pExecuteSsc = Ov_DynamicPtrCast(ssc_executeSsc, pActionBlock);
	OV_INSTPTR_ssc_sscHeader  	pSSC= Ov_DynamicPtrCast(ssc_sscHeader, Ov_GetParent(ov_containment, Ov_GetParent(ov_containment, pActionBlock)));
	OV_INSTPTR_ov_domain  		pActionsContainer= Ov_GetParent(ov_containment, pAction);

	// check
	if ( (pActionBlock == NULL ) || (pAction == NULL) || (pSSC == NULL) || (pActionsContainer == NULL) || (pActionsContainer != &pSSC->p_actions) )
	{
		return OV_ERR_BADPATH;
	}
    // for SSC, the action block must be "executeSsc"

	if ((pSscAction != NULL) && (pExecuteSsc == NULL))
	{
		pActionBlock->v_error=TRUE;
		ov_string_setvalue(&pActionBlock->v_errorDetail, "wrong action block class");
		return OV_ERR_BADOBJTYPE;
	}

	if ((pSscAction == NULL) && (pExecuteSsc != NULL))
	{
		pActionBlock->v_error=TRUE;
		ov_string_setvalue(&pActionBlock->v_errorDetail, "wrong action block class");
		return OV_ERR_BADOBJTYPE;
	}


	// send action instance identity
	ov_string_setvalue(&pActionBlock->v_actionName, pAction->v_identifier);
	// init action block
	pActionBlock->v_error=0;
	ov_string_setvalue(&pActionBlock->v_errorDetail, NULL);

    return ov_association_link(passoc_ssc_actionBlocks, Ov_PtrUpCast
           (ov_object, pparent), Ov_PtrUpCast(ov_object, pchild),
           parenthint, Ov_PtrUpCast(ov_object, prelparent),
           childhint, Ov_PtrUpCast(ov_object, prelchild));
}

/***********************************************************************
	Unlink
***********************************************************************/
OV_DLLFNCEXPORT void ssc_actionBlocks_unlink
(const OV_PPT_ssc_actionBlocks pparent,
 const OV_CPT_ssc_actionBlocks pchild )
{
	//	local Pointers
	OV_INSTPTR_ssc_executeFb    pActionBlock = Ov_StaticPtrCast(ssc_executeFb, pchild);

	// reset parameter
	if (ov_string_compare(pActionBlock->v_actionName, pparent->v_identifier) == 0)
	{
		ov_string_setvalue(&pActionBlock->v_actionName, NULL);
		pActionBlock->v_error=TRUE;
		ov_string_setvalue(&pActionBlock->v_errorDetail, "action is not defined");

	}


	/* Unlink */
	ov_association_unlink (passoc_ssc_actionBlocks,
	                         Ov_PtrUpCast (ov_object, pparent),
	                         Ov_PtrUpCast (ov_object, pchild));


}

OV_DECL_GETACCESS(ssc_actionBlocks) {
        return OV_AC_READ | OV_AC_LINKABLE | OV_AC_UNLINKABLE;
}

