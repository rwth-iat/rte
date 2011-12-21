
/******************************************************************************
*
*   FILE
*   ----
*   nextTransitions.c
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
#include "libov/ov_association.h"

#include "libov/ov_macros.h"

/***********************************************************************
	Link
***********************************************************************/

OV_DLLFNCEXPORT OV_RESULT sfc_nextTransitions_link
(
	const OV_PPT_sfc_nextTransitions	pparent,
	const OV_CPT_sfc_nextTransitions	pchild,
	const OV_PLACEMENT_HINT				parenthint,
	const OV_CPT_sfc_nextTransitions	prelparent,
	const OV_PLACEMENT_HINT				childhint,
	const OV_CPT_sfc_nextTransitions	prelchild
) {
	//	local Pointers
	//

	OV_INSTPTR_sfc_transition pTrans = Ov_StaticPtrCast(sfc_transition, pchild);
	OV_INSTPTR_fb_task pTransTaskParent=Ov_GetParent(fb_tasklist, pchild);
	OV_INSTPTR_sfc_step pNextStep = Ov_GetParent(sfc_previousTransitions, pchild);
	OV_RESULT    result;

	// check if the previousStep and nextStep are the same
	if ( pNextStep == pparent)
	{
		ov_logfile_error("sfc_nextTransitions_link: %s - the previous step and the next step cannot be the same.", pchild->v_identifier);
		return OV_ERR_BADPLACEMENT;
	}

	// unlink the transition from its taskparent
	if (pTransTaskParent !=NULL)
		Ov_Unlink(fb_tasklist, pTransTaskParent, pchild);
	// link to local tasklist of the parent step
	result=Ov_Link(fb_tasklist, &pparent->p_trans, pchild);

	// activate transition
	pTrans->v_actimode=1;
	pTrans->v_iexreq=TRUE;

	// link association
	return ov_association_link(passoc_sfc_nextTransitions,
		Ov_PtrUpCast(ov_object, pparent),
		Ov_PtrUpCast(ov_object, pchild),
		parenthint,
		Ov_PtrUpCast(ov_object, prelparent),
		childhint,
		Ov_PtrUpCast(ov_object, prelchild));
}

/***********************************************************************
	Unlink
***********************************************************************/
OV_DLLFNCEXPORT void sfc_nextTransitions_unlink
(const OV_PPT_sfc_nextTransitions pparent,
 const OV_CPT_sfc_nextTransitions pchild)
{

	// Unlink tasklist
	Ov_Unlink(fb_tasklist, pparent, pchild);

  /* Unlink */
  ov_association_unlink (passoc_sfc_nextTransitions,
                         Ov_PtrUpCast (ov_object, pparent),
                         Ov_PtrUpCast (ov_object, pchild));
}

OV_IMPL_GETACCESS(sfc_nextTransitions)

