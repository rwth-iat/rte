
/******************************************************************************
*
*   FILE
*   ----
*   getAny.c
*
*   History
*   -------
*   2012-06-28   File created
*
*******************************************************************************
*
*   This file is generated by the 'acplt_builder' command
*
******************************************************************************/


#ifndef OV_COMPILE_LIBRARY_fbcomlib
#define OV_COMPILE_LIBRARY_fbcomlib
#endif


#include "fbcomlib.h"
#include "libov/ov_macros.h"
#include "libov/ov_logfile.h"
#include "../../include/config.h"

OV_DLLFNCEXPORT OV_TIME* fbcomlib_getAny_varTimeStamp_get(
    OV_INSTPTR_fbcomlib_getAny          pobj
) {
    return &pobj->v_varTimeStamp;
}

OV_DLLFNCEXPORT OV_RESULT fbcomlib_getAny_varTimeStamp_set(
    OV_INSTPTR_fbcomlib_getAny          pobj,
    const OV_TIME*  value
) {
    pobj->v_varTimeStamp = *value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_UINT fbcomlib_getAny_varQState_get(
    OV_INSTPTR_fbcomlib_getAny          pobj
) {
    return pobj->v_varQState;
}

OV_DLLFNCEXPORT OV_RESULT fbcomlib_getAny_varQState_set(
    OV_INSTPTR_fbcomlib_getAny          pobj,
    const OV_UINT  value
) {
    pobj->v_varQState = value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_ANY* fbcomlib_getAny_receivedAny_get(
    OV_INSTPTR_fbcomlib_getAny          pobj
) {
    return &pobj->v_receivedAny;
}

OV_DLLFNCEXPORT OV_RESULT fbcomlib_getAny_receivedAny_set(
    OV_INSTPTR_fbcomlib_getAny          pobj,
    const OV_ANY*  value
) {
    return ov_variable_setanyvalue(&pobj->v_receivedAny, value);
}

OV_DLLFNCEXPORT OV_BOOL fbcomlib_getAny_doReset_get(
    OV_INSTPTR_fbcomlib_getAny          pobj
) {
    return pobj->v_doReset;
}

OV_DLLFNCEXPORT OV_RESULT fbcomlib_getAny_doReset_set(
    OV_INSTPTR_fbcomlib_getAny          pobj,
    const OV_BOOL  value
) {
	if(value){
		fbcomlib_FBComCommon_doResetAbstract_set((OV_INSTPTR_fbcomlib_FBComCommon)pobj, value);
		fbcomlib_getAny_receivedAny_set(pobj, NULL);
	}
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT void fbcomlib_getAny_startup(
	OV_INSTPTR_ov_object 	pobj
) {
	 OV_INSTPTR_fbcomlib_getAny 					getAny = Ov_StaticPtrCast(fbcomlib_getAny, pobj);
		OV_VTBLPTR_fbcomlib_getAny					stable = NULL;

		OV_INSTPTR_ksapi_getAny						ksapigetAny = NULL;
		OV_VTBLPTR_ksapi_getAny						mtable = NULL;
		OV_RESULT res = 0;

	    // do what the base class does first
	    fb_functionblock_startup(pobj);

		Ov_GetVTablePtr(fbcomlib_getAny, stable, getAny);

		ksapigetAny = (OV_INSTPTR_ksapi_getAny)Ov_GetFirstChild(fbcomlib_FBComCommonAssoc, getAny);
		if(! ksapigetAny){
		  res = Ov_CreateObject(ksapi_getAny, ksapigetAny, getAny, "getAny");
		  if (Ov_Fail(res)){
			ov_logfile_error("Error while creating the getAny");
			fbcomlib_FBComCommon_state_set((OV_INSTPTR_fbcomlib_FBComCommon)getAny, STATE_OVERROR);
			return;
		  }
		  Ov_Link(fbcomlib_FBComCommonAssoc, getAny, ksapigetAny);
		}
		Ov_GetVTablePtr(ksapi_getAny, mtable, ksapigetAny);
		getAny->v_doSend = FALSE;

		//(re)register the return method
		mtable->m_registerMethod((OV_INSTPTR_ksapi_KSCommon) ksapigetAny, stable->m_retMethod);
		fbcomlib_FBComCommon_state_set((OV_INSTPTR_fbcomlib_FBComCommon)getAny, STATE_OK);
		fbcomlib_getAny_receivedAny_set(getAny, NULL);
		return;

}

OV_DLLFNCEXPORT void fbcomlib_getAny_retMethod(
		OV_INSTPTR_ov_object 		pobj,
		OV_STRING 						errorstring,
		OV_INT	 						errorcode
) {
	OV_INSTPTR_ksapi_getAny ksapigetAny = Ov_StaticPtrCast(ksapi_getAny, pobj);
	OV_INSTPTR_fbcomlib_getAny getAny = (OV_INSTPTR_fbcomlib_getAny)Ov_GetParent(fbcomlib_FBComCommonAssoc, ksapigetAny);

	fbcomlib_FBComCommon_retMethod(pobj, errorstring, errorcode);
	//get the any
	fbcomlib_getAny_receivedAny_set(getAny, &(ksapigetAny->v_receiveany));
	getAny->v_varQState = ksapigetAny->v_varQState;
	getAny->v_varTimeStamp = ksapigetAny->v_varTimeStamp;
}

