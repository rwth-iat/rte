#ifndef OV_COMPILE_LIBRARY_kshist
#define OV_COMPILE_LIBRARY_kshist
#endif

#include "kshist.h"
#include "libov/ov_macros.h"


/*
*	Operations
*/


OV_RESULT OV_DLLFNCEXPORT kshist_KsHistoryRO_gethist(
	OV_INSTPTR_ov_object pobj,
	OV_UINT items_len,
	OV_GETHIST_ITEM* pitem,
	OV_GETHISTRESULT_ITEM* presultitem,
	OV_UINT max_answers	
) 
{
	return OV_ERR_NOTIMPLEMENTED;
}

OV_RESULT OV_DLLFNCEXPORT kshist_KsHistoryRO_getephist (
	OV_INSTPTR_ov_object 	pobj,
	OV_STRING*		type_identifier,
	OV_HIST_TYPE*		historytype,
	OV_INTERPOLATION_MODE*  default_interpolation,
	OV_INTERPOLATION_MODE*	supported_interpolation
)
{
	return OV_ERR_NOTIMPLEMENTED;
}

OV_RESULT OV_DLLFNCEXPORT kshist_KsHistoryRO_getephistelem(
	OV_INSTPTR_ov_object 	pobj,
	const OV_TICKET		*pticket,
	OV_GETEP_RES		*result,
	KS_EP_FLAGS		scope_flags,
	const OV_OBJ_TYPE	type_mask,
	const OV_STRING		name_mask
) 
{
	return OV_ERR_NOTIMPLEMENTED;
}

