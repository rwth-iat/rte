#ifndef OV_COMPILE_LIBRARY_kshist
#define OV_COMPILE_LIBRARY_kshist
#endif

#include "kshist.h"
#include "libov/ov_macros.h"


/*
*	Operations
*/

OV_RESULT OV_DLLFNCEXPORT kshist_KsSimpleHistoryRO_getephist (
	OV_INSTPTR_ov_object 	pobj,
	OV_STRING*		type_identifier,
	OV_HIST_TYPE*		historytype,
	OV_INTERPOLATION_MODE*  default_interpolation,
	OV_INTERPOLATION_MODE*	supported_interpolation
)
{
	OV_INSTPTR_kshist_KsSimpleHistoryRO phist;
	OV_INSTPTR_kshist_KsSimpleHistoryEO phistEO;

	phist = Ov_StaticPtrCast(kshist_KsSimpleHistoryRO, pobj);
	phistEO = Ov_GetParent(kshist_representation, phist);
	if (phistEO) {
		*type_identifier = phistEO->v_TypeIdentifier;
		*historytype = (OV_HIST_TYPE) phistEO->v_HistoryType;
		*default_interpolation = (OV_INTERPOLATION_MODE) phistEO->v_defaultIPM;
		*supported_interpolation = (OV_INTERPOLATION_MODE) phistEO->v_supportedIPM;
		return OV_ERR_OK;
	}
	return OV_ERR_NOTIMPLEMENTED;
}

OV_RESULT OV_DLLFNCEXPORT kshist_KsSimpleHistoryRO_gethist(
  OV_INSTPTR_ov_object pobj,
  OV_UINT items_len,	
  OV_GETHIST_ITEM* pitem,
  OV_GETHISTRESULT_ITEM* presultitem,
  OV_UINT max_answers	
)
{
	OV_UINT			i;
	OV_INSTPTR_kshist_KsSimpleHistoryRO phist;
	OV_INSTPTR_kshist_KsSimpleHistoryTrack phisttrack;
	OV_VTBLPTR_kshist_KsSimpleHistoryTrack pvtable;
	OV_INSTPTR_kshist_KsSimpleHistoryEO phistEO;

	phist = Ov_StaticPtrCast(kshist_KsSimpleHistoryRO, pobj);
	phistEO = Ov_GetParent(kshist_representation, phist);
	if (phistEO) {
	   for(i=0; i<items_len; i++, presultitem++, pitem++) {
			
		phisttrack = Ov_GetFirstChild(kshist_historytrackcontainment, phistEO);
		while (phisttrack) {	
			if (strcmp(pitem->part, phisttrack->v_identifier)==0)
				break;
			phisttrack = Ov_GetNextChild(kshist_historytrackcontainment, phisttrack);
		}

		if (phisttrack) {
			Ov_GetVTablePtr(kshist_KsSimpleHistoryTrack, pvtable, phisttrack);
			presultitem->result = (pvtable->m_gettrackhist) (Ov_PtrUpCast(ov_object,phisttrack),&pitem->selector,&presultitem->value, max_answers);
		}
	   }
	}
	return OV_ERR_OK;
}


OV_RESULT OV_DLLFNCEXPORT kshist_KsSimpleHistoryRO_getephistelem(
	OV_INSTPTR_ov_object 	pobj,
	const OV_TICKET		*pticket,
	OV_GETEP_RES		*result,
	KS_EP_FLAGS		scope_flags,
	const OV_OBJ_TYPE	type_mask,
	const OV_STRING		name_mask
) 
{
	OV_OBJ_ENGINEERED_PROPS	*pprops;
	OV_INSTPTR_kshist_KsSimpleHistoryRO phist;
	OV_INSTPTR_kshist_KsSimpleHistoryTrack phisttrack;
	OV_VTBLPTR_kshist_KsSimpleHistoryTrack pvtable;
	OV_ACCESS		access;
	OV_ELEMENT		elem;
	OV_INSTPTR_kshist_KsSimpleHistoryEO phistEO;

	phist = Ov_StaticPtrCast(kshist_KsSimpleHistoryRO, pobj);
	phistEO = Ov_GetParent(kshist_representation, phist);
	if (phistEO) {
  	    if((scope_flags & KS_EPF_CHILDREN) && ((type_mask & (KS_OT_DOMAIN | KS_OT_VARIABLE | KS_OT_HISTORY)))) {

		phisttrack = Ov_GetFirstChild(kshist_historytrackcontainment, phistEO);
		while (phisttrack) {

			Ov_GetVTablePtr(kshist_KsSimpleHistoryTrack, pvtable, phisttrack);

			/*
			*	test if we have access to this object
			*/
			elem.elemtype = OV_ET_OBJECT;
			elem.pobj = Ov_PtrUpCast(ov_object,phisttrack);
			access = pvtable->m_getaccess(elem.pobj, &elem, pticket);
			if(!(access & OV_AC_READ)) {
				return OV_ERR_OK;
			}

			/*
			*	create new result item and insert into result item list
			*/

			pprops = Ov_MemStackAlloc(OV_OBJ_ENGINEERED_PROPS);
			if(!pprops) {
				return OV_ERR_TARGETGENERIC;	/* TODO! out of heap memory */
			}
			pprops->pnext = NULL;
			if(result->plast) {
				result->plast->pnext = pprops;
				result->plast = pprops;
			} else {
				result->pfirst = result->plast = pprops;
			}
			result->items_len++;

			/*
			*	set engineered properties
			*/

			pprops->objtype = KS_OT_VARIABLE;
			pprops->identifier = elem.pobj->v_identifier;
			pprops->creation_time = elem.pobj->v_creationtime;
			pprops->comment = (pvtable->m_getcomment)(elem.pobj, &elem);
			pprops->access = access;
			pprops->semantic_flags = (pvtable->m_getflags)(elem.pobj, &elem);

			pprops->OV_OBJ_ENGINEERED_PROPS_u.var_engineered_props.tech_unit
				= phisttrack->v_TechUnit;
			pprops->OV_OBJ_ENGINEERED_PROPS_u.var_engineered_props.vartype
				= phisttrack->v_Type;

			phisttrack = Ov_GetNextChild(kshist_historytrackcontainment, phisttrack);
		}
	    }
	}
	return OV_ERR_OK;
}
