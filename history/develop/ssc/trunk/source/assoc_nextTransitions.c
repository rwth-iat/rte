
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


#ifndef OV_COMPILE_LIBRARY_ssc
#define OV_COMPILE_LIBRARY_ssc
#endif



#include "ssclib.h"
#include "libov/ov_association.h"

/***********************************************************************
	Link
***********************************************************************/

OV_DLLFNCEXPORT OV_RESULT ssc_nextTransitions_link
(
	const OV_PPT_ssc_nextTransitions	pparent,
	const OV_CPT_ssc_nextTransitions	pchild,
	const OV_PLACEMENT_HINT				parenthint,
	const OV_CPT_ssc_nextTransitions	prelparent,
	const OV_PLACEMENT_HINT				childhint,
	const OV_CPT_ssc_nextTransitions	prelchild
) {
	//	local Pointers
	//

	OV_INSTPTR_ssc_transition pTrans = Ov_StaticPtrCast(ssc_transition, pchild);
	OV_INSTPTR_fb_task pTransTaskParent = Ov_GetParent(fb_tasklist, pchild);
	OV_INSTPTR_ssc_step pNextStep = Ov_GetParent(ssc_previousTransitions, pchild);
	OV_INSTPTR_ssc_SequentialControlChart pOwnSSC = Ov_DynamicPtrCast(ssc_SequentialControlChart, Ov_GetParent(ov_containment, pchild));

	// check if the previousStep and nextStep are the same
	if ( pNextStep == pparent){
		ov_logfile_error("ssc_nextTransitions_link: %s - the previous step and the next step cannot be the same.", pchild->v_identifier);
		return OV_ERR_BADPLACEMENT;
	}

	// unlink the transition from its taskparent
	if (pTransTaskParent != NULL){
		Ov_Unlink(fb_tasklist, pTransTaskParent, pchild);
	}
	// link to local tasklist of the parent ssc
	Ov_Link(fb_tasklist, Ov_GetPartPtr(trans, pOwnSSC), pchild);

	// activate transition
	pTrans->v_actimode = FB_AM_ON;
	pTrans->v_iexreq=TRUE;

	// link association
	return ov_association_link(passoc_ssc_nextTransitions,
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
OV_DLLFNCEXPORT void ssc_nextTransitions_unlink
(const OV_PPT_ssc_nextTransitions pparent,
 const OV_CPT_ssc_nextTransitions pchild)
{

	// Unlink tasklist
	Ov_Unlink(fb_tasklist, pparent, pchild);

  /* Unlink */
  ov_association_unlink (passoc_ssc_nextTransitions,
                         Ov_PtrUpCast (ov_object, pparent),
                         Ov_PtrUpCast (ov_object, pchild));
}

OV_IMPL_GETACCESS(ssc_nextTransitions)

