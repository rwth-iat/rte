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


OV_DLLFNCEXPORT OV_TIME * kshist_KsSimpleTimeTrack_TimeVec_get(
             OV_INSTPTR_kshist_KsSimpleTimeTrack          pobj,
             OV_UINT *pveclen
) {
             *pveclen = pobj->v_TimeVec.veclen;
             return pobj->v_TimeVec.value;
}

OV_DLLFNCEXPORT OV_RESULT kshist_KsSimpleTimeTrack_TimeVec_set(
             OV_INSTPTR_kshist_KsSimpleTimeTrack          pobj,
             const OV_TIME *pvalue,
             const OV_UINT veclen
) {
             return Ov_SetDynamicVectorValue(&pobj->v_TimeVec,pvalue,veclen,TIME);
}

OV_DLLFNCEXPORT OV_RESULT kshist_KsSimpleTimeTrack_constructor(
	OV_INSTPTR_ov_object 	pobj
) {
	OV_INSTPTR_kshist_KsSimpleTimeTrack 	ptrack;

	ptrack = Ov_StaticPtrCast(kshist_KsSimpleTimeTrack, pobj);
	ptrack->v_Type = OV_VT_TIME;		
	ov_string_setvalue(&ptrack->v_TechUnit, "Date");
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT kshist_KsSimpleTimeTrack_gettrackhist(
  OV_INSTPTR_ov_object pobj,
  OV_HISTSELECTOR* phistsel,
  OV_VAR_VALUE* pvarvalue,
  OV_UINT max_answers	
) {
	OV_INSTPTR_kshist_KsSimpleTimeTrack 	pTimeTrack;
	OV_TIME	from,to;
	OV_TIME	*ptime;
	OV_UINT i;
	OV_UINT answers;

	pTimeTrack = Ov_StaticPtrCast(kshist_KsSimpleTimeTrack, pobj);
	pvarvalue->vartype = OV_VT_TIME_VEC;
	pvarvalue->valueunion.val_time_vec.value = (OV_TIME*)ov_memstack_alloc(max_answers*sizeof(OV_TIME));		
	ptime = pvarvalue->valueunion.val_time_vec.value;
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
			if((phistsel->OV_HISTSELECTOR_u.ths.ip_mode != OV_IPM_NONE) && (phistsel->OV_HISTSELECTOR_u.ths.ip_mode != OV_IPM_DEFAULT)) {
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

	answers = 0;
	for (i=0; ((i< pTimeTrack->v_TimeVec.veclen) && (answers < max_answers)); i++) {
		if ((ov_time_compare((const OV_TIME*) &pTimeTrack->v_TimeVec.value[i], (const OV_TIME*) &from) >= 0 ) &&
		   (ov_time_compare((const OV_TIME*) &pTimeTrack->v_TimeVec.value[i], (const OV_TIME*) &to) <= 0 )) {
			*ptime=pTimeTrack->v_TimeVec.value[i]; 
			answers++;
			ptime++;
		}

	}
	pvarvalue->valueunion.val_time_vec.veclen = answers;
	return OV_ERR_OK;
}
