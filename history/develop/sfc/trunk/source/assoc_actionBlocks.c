
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


#ifndef OV_COMPILE_LIBRARY_sfc
#define OV_COMPILE_LIBRARY_sfc
#endif



#include "sfc.h"
#include "sfclib.h"
#include "libov/ov_association.h"


/***********************************************************************
	Link
***********************************************************************/

OV_DLLFNCEXPORT OV_RESULT sfc_actionBlocks_link
(	const OV_PPT_sfc_actionBlocks	pparent,
	const OV_CPT_sfc_actionBlocks	pchild,
	const OV_PLACEMENT_HINT			parenthint,
	const OV_CPT_sfc_actionBlocks	prelparent,
	const OV_PLACEMENT_HINT			childhint,
	const OV_CPT_sfc_actionBlocks	prelchild )
{

	//	local Pointers
	OV_INSTPTR_fb_functionblock pAction = Ov_StaticPtrCast(fb_functionblock, pparent);
	OV_INSTPTR_sfc_executeFb    pActionBlock = Ov_StaticPtrCast(sfc_executeFb, pchild);
	OV_INSTPTR_sfc_sfcHeader    pSfcAction  = Ov_DynamicPtrCast(sfc_sfcHeader, pAction);
	OV_INSTPTR_sfc_executeSfc   pExecuteSfc = Ov_DynamicPtrCast(sfc_executeSfc, pActionBlock);
	OV_INSTPTR_sfc_sfcHeader  	pSFC= Ov_DynamicPtrCast(sfc_sfcHeader, Ov_GetParent(ov_containment, Ov_GetParent(ov_containment, pActionBlock)));
	OV_INSTPTR_ov_domain  		pActionsContainer= Ov_GetParent(ov_containment, pAction);

	// check
	if ( (pActionBlock == NULL ) || (pAction == NULL) || (pSFC == NULL) || (pActionsContainer == NULL) || (pActionsContainer != &pSFC->p_actions) )
	{
		return OV_ERR_BADPATH;
	}
    // for SFC, the action block must be "executeSfc"

	if ((pSfcAction != NULL) && (pExecuteSfc == NULL))
	{
		pActionBlock->v_error=TRUE;
		ov_string_setvalue(&pActionBlock->v_errorDetail, "wrong action block class");
		return OV_ERR_BADOBJTYPE;
	}

	if ((pSfcAction == NULL) && (pExecuteSfc != NULL))
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

    return ov_association_link(passoc_sfc_actionBlocks, Ov_PtrUpCast
           (ov_object, pparent), Ov_PtrUpCast(ov_object, pchild),
           parenthint, Ov_PtrUpCast(ov_object, prelparent),
           childhint, Ov_PtrUpCast(ov_object, prelchild));
}

/***********************************************************************
	Unlink
***********************************************************************/
OV_DLLFNCEXPORT void sfc_actionBlocks_unlink
(const OV_PPT_sfc_actionBlocks pparent,
 const OV_CPT_sfc_actionBlocks pchild )
{
	//	local Pointers
	OV_INSTPTR_sfc_executeFb    pActionBlock = Ov_StaticPtrCast(sfc_executeFb, pchild);

	// reset parameter
	if (ov_string_compare(pActionBlock->v_actionName, pparent->v_identifier) == 0)
	{
		ov_string_setvalue(&pActionBlock->v_actionName, NULL);
		pActionBlock->v_error=TRUE;
		ov_string_setvalue(&pActionBlock->v_errorDetail, "action is not defined");

	}


	/* Unlink */
	ov_association_unlink (passoc_sfc_actionBlocks,
	                         Ov_PtrUpCast (ov_object, pparent),
	                         Ov_PtrUpCast (ov_object, pchild));


}

OV_DECL_GETACCESS(sfc_actionBlocks) {
        return OV_AC_READ | OV_AC_LINKABLE | OV_AC_UNLINKABLE;
}

