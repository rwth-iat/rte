#ifndef OV_COMPILE_LIBRARY_kshist
#define OV_COMPILE_LIBRARY_kshist
#endif

#include "kshist.h"
#include "libov/ov_macros.h"


/*
*	Operations
*/

OV_DLLFNCEXPORT OV_INT kshist_KsSimpleHistoryEO_HistoryType_get(
             OV_INSTPTR_kshist_KsSimpleHistoryEO          pobj
) {
             return pobj->v_HistoryType;
}

OV_DLLFNCEXPORT OV_RESULT kshist_KsSimpleHistoryEO_HistoryType_set(
             OV_INSTPTR_kshist_KsSimpleHistoryEO          pobj,
             const OV_INT           value
) {
             pobj->v_HistoryType = value;
             return OV_ERR_OK;
}


OV_RESULT OV_DLLFNCEXPORT kshist_KsSimpleHistoryEO_constructor(
	OV_INSTPTR_ov_object pobj
) 
{
	OV_RESULT result;
	OV_INSTPTR_kshist_KsSimpleHistoryEO phist;
	phist = Ov_StaticPtrCast(kshist_KsSimpleHistoryEO, pobj);
	result = ov_object_constructor(pobj);
	  
  	if(Ov_Fail(result))
        	return result;

	ov_string_setvalue(&phist->v_TypeIdentifier, "KsSimpleHistory");		
	phist->v_HistoryType = 0;		
	phist->v_defaultIPM = 0;		
	phist->v_supportedIPM = 0;		
	return OV_ERR_OK;
}


