/*
*   The Framework was generated by the ACPLT/OV Builder.
*
*   Builder version: 1.0
*   Date of file generation:  16-Jul-2001 (17:44:49)
*/
#ifndef OV_COMPILE_LIBRARY_kshist
#define OV_COMPILE_LIBRARY_kshist
#endif

#include "kshist.h"
#include "libov/ov_macros.h"

OV_DLLFNCEXPORT OV_STRING kshist_KsSimpleSingleTrack_varTechUnit_get(
             OV_INSTPTR_kshist_KsSimpleSingleTrack          pobj
) {
             return pobj->v_varTechUnit;
}

OV_DLLFNCEXPORT OV_RESULT kshist_KsSimpleSingleTrack_varTechUnit_set(
             OV_INSTPTR_kshist_KsSimpleSingleTrack          pobj,
             const OV_STRING           value
) {
	     OV_RESULT result;

	     result = ov_string_setvalue(&pobj->v_TechUnit,value);
	     if (result != OV_ERR_OK) return result;
             return ov_string_setvalue(&pobj->v_varTechUnit,value);
}

OV_DLLFNCEXPORT OV_SINGLE * kshist_KsSimpleSingleTrack_ValueVec_get(
             OV_INSTPTR_kshist_KsSimpleSingleTrack          pobj,
             OV_UINT *pveclen
) {
             *pveclen = pobj->v_ValueVec.veclen;
             return pobj->v_ValueVec.value;
}

OV_DLLFNCEXPORT OV_RESULT kshist_KsSimpleSingleTrack_ValueVec_set(
             OV_INSTPTR_kshist_KsSimpleSingleTrack          pobj,
             const OV_SINGLE *pvalue,
             const OV_UINT veclen
) {
             return Ov_SetDynamicVectorValue(&pobj->v_ValueVec,pvalue,veclen,SINGLE);
}

OV_DLLFNCEXPORT OV_RESULT kshist_KsSimpleSingleTrack_constructor(
	OV_INSTPTR_ov_object 	pobj
) {
	OV_INSTPTR_kshist_KsSimpleSingleTrack 	ptrack;

	ptrack = Ov_StaticPtrCast(kshist_KsSimpleSingleTrack, pobj);
	ptrack->v_Type = OV_VT_SINGLE;
	ov_string_setvalue(&ptrack->v_TechUnit, "Single");
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT kshist_KsSimpleSingleTrack_gettrackhist(
  OV_INSTPTR_ov_object pobj,
  OV_HISTSELECTOR* phistsel,
  OV_VAR_VALUE* pvarvalue,
  OV_UINT max_answers
) {
	OV_INSTPTR_kshist_KsSimpleTimeTrack 	pTimeTrack;
	OV_INSTPTR_kshist_KsSimpleSingleTrack 	pSingleTrack;
	OV_TIME	from,to;
	OV_SINGLE *psingle;
	OV_UINT answers;
	OV_UINT i;

	pSingleTrack = Ov_StaticPtrCast(kshist_KsSimpleSingleTrack, pobj);
	pvarvalue->vartype = OV_VT_SINGLE_VEC;
	pvarvalue->valueunion.val_single_vec.value = (OV_SINGLE*)ov_memstack_alloc(max_answers*sizeof(OV_SINGLE));		
	psingle = pvarvalue->valueunion.val_single_vec.value;
	switch(phistsel->hseltype) {
		case OV_HSELT_NONE:
			/*
			*	get all messages available
			*/
			from.secs = 0;
			from.usecs = 0;
			ov_time_gettime(&to);
			break;
		case OV_HSELT_TIME:
			/*
			*	only get selected messages
			*/
			if(phistsel->OV_HISTSELECTOR_u.ths.ip_mode != OV_IPM_NONE) {
				/* messages are change driven; no interpolation available */
				return OV_ERR_BADSELECTOR;
			}
			switch(phistsel->OV_HISTSELECTOR_u.ths.from.timetype) {
			case OV_TT_ABSOLUTE:
				from = phistsel->OV_HISTSELECTOR_u.ths.from.OV_ABSRELTIME_u.abstime;
				break;
			case OV_TT_RELATIVE:
				ov_time_gettime(&from);
				ov_time_add(&from, &from,
					&phistsel->OV_HISTSELECTOR_u.ths.from.OV_ABSRELTIME_u.reltime);
				break;
			default:
				return OV_ERR_BADSELECTOR;
			}
			switch(phistsel->OV_HISTSELECTOR_u.ths.to.timetype) {
			case OV_TT_ABSOLUTE:
				to = phistsel->OV_HISTSELECTOR_u.ths.to.OV_ABSRELTIME_u.abstime;
				break;
			case OV_TT_RELATIVE:
				ov_time_gettime(&to);
				ov_time_add(&to, &to,
					&phistsel->OV_HISTSELECTOR_u.ths.to.OV_ABSRELTIME_u.reltime);
				break;
			default:
				return OV_ERR_BADSELECTOR;
			}
			break;
		case OV_HSELT_STRING:
			/*
			*	not supported
			*/
			/* fall through... */
		default:
			return OV_ERR_BADSELECTOR;
	}	/* switch() */

	pTimeTrack = Ov_GetParent(kshist_isTimetrackOf, pSingleTrack);
	answers	= 0;
	if (pTimeTrack) {
		for (i=0; ((i< pSingleTrack->v_ValueVec.veclen) && (i< pTimeTrack->v_TimeVec.veclen) && (answers < max_answers)); i++) {
			if ((ov_time_compare((const OV_TIME*) &pTimeTrack->v_TimeVec.value[i], (const OV_TIME*) &from) >= 0 ) &&
			   (ov_time_compare((const OV_TIME*) &pTimeTrack->v_TimeVec.value[i], (const OV_TIME*) &to) <= 0 )) {
				*psingle = pSingleTrack->v_ValueVec.value[i];
				answers++;
				psingle++;
			}
		}
	}
	pvarvalue->valueunion.val_single_vec.veclen = answers;
	return OV_ERR_OK;
}

