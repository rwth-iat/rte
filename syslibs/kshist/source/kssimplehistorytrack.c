#ifndef OV_COMPILE_LIBRARY_kshist
#define OV_COMPILE_LIBRARY_kshist
#endif

#include "kshist.h"
#include "libov/ov_macros.h"


/*
*	Operations
*/


OV_RESULT OV_DLLFNCEXPORT kshist_KsSimpleHistoryTrack_constructor(
	OV_INSTPTR_ov_object pobj
) 
{
	OV_RESULT result;
	OV_INSTPTR_kshist_KsSimpleHistoryTrack phist;

	phist = Ov_StaticPtrCast(kshist_KsSimpleHistoryTrack, pobj);
	result = ov_object_constructor(pobj);
    	if(Ov_Fail(result))
        	return result;

	phist->v_Type = OV_VT_VOID;		
	phist->v_TechUnit = NULL;
	return OV_ERR_OK;
}

OV_RESULT OV_DLLFNCEXPORT kshist_KsSimpleHistoryTrack_gettrackhist(
  OV_INSTPTR_ov_object pobj,
  OV_HISTSELECTOR* phistsel,
  OV_VAR_VALUE* pvarvalue,
  OV_UINT max_answers	
)
{
	return OV_ERR_NOTIMPLEMENTED;
}

