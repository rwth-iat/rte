/**
 * @file getSingle.c
 * FunctionBlock for retrieving a remote Single.
 * Inherits data fields from FBComCommon.
 */
#ifndef OV_COMPILE_LIBRARY_fbcomlib
#define OV_COMPILE_LIBRARY_fbcomlib
#endif


#include "fbcomlib.h"
#include "../../include/config.h"
#include "libov/ov_macros.h"
#include "libov/ov_logfile.h"

OV_DLLFNCEXPORT OV_TIME* fbcomlib_getSingle_varTimeStamp_get(
    OV_INSTPTR_fbcomlib_getSingle          pobj
) {
    return &pobj->v_varTimeStamp;
}

OV_DLLFNCEXPORT OV_RESULT fbcomlib_getSingle_varTimeStamp_set(
    OV_INSTPTR_fbcomlib_getSingle          pobj,
    const OV_TIME*  value
) {
    pobj->v_varTimeStamp = *value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_UINT fbcomlib_getSingle_varQState_get(
    OV_INSTPTR_fbcomlib_getSingle          pobj
) {
    return pobj->v_varQState;
}

OV_DLLFNCEXPORT OV_RESULT fbcomlib_getSingle_varQState_set(
    OV_INSTPTR_fbcomlib_getSingle          pobj,
    const OV_UINT  value
) {
    pobj->v_varQState = value;
    return OV_ERR_OK;
}

/**
 * The received Single is stored here
 */
OV_DLLFNCEXPORT OV_SINGLE fbcomlib_getSingle_receivedSingle_get(
    OV_INSTPTR_fbcomlib_getSingle          pobj
) {
    return pobj->v_receivedSingle;
}

/**
 * The received Single is stored here.
 * This Set shouldnt be used from outside!
 */
OV_DLLFNCEXPORT OV_RESULT fbcomlib_getSingle_receivedSingle_set(
    OV_INSTPTR_fbcomlib_getSingle          pobj,
    const OV_SINGLE  value
) {

		pobj->v_receivedSingle = value;
		return OV_ERR_OK;


}

/**
 * If TRUE, does the reset FBComCommon and removed receivedSingle additionally
 * @todo Could the GET be removed???
 */
OV_DLLFNCEXPORT OV_BOOL fbcomlib_getSingle_doReset_get(
	    OV_INSTPTR_fbcomlib_getSingle          pobj
) {
    return pobj->v_doResetAbstract;
}

/**
 * If TRUE, does the reset FBComCommon and removed receivedSingle additionally
 */
OV_DLLFNCEXPORT OV_RESULT fbcomlib_getSingle_doReset_set(
	    OV_INSTPTR_fbcomlib_getSingle          pobj,
	    const OV_BOOL  value
) {
	if(value){
		fbcomlib_FBComCommon_doResetAbstract_set((OV_INSTPTR_fbcomlib_FBComCommon)pobj, value);
		fbcomlib_getSingle_receivedSingle_set(pobj, 0.0);
	}
	return OV_ERR_OK;
}

/**
 * Individual startup method for creating ksapi_getsingle object for later communication
 */
OV_DLLFNCEXPORT void fbcomlib_getSingle_startup(
	OV_INSTPTR_ov_object 	pobj
) {
    OV_INSTPTR_fbcomlib_getSingle 					getsingle = Ov_StaticPtrCast(fbcomlib_getSingle, pobj);
	OV_VTBLPTR_fbcomlib_getSingle					stable = NULL;

	OV_INSTPTR_ksapi_getSingle						ksapigetsingle = NULL;
	OV_VTBLPTR_ksapi_getSingle						mtable = NULL;
	OV_RESULT res = 0;

    // do what the base class does first
    fb_functionblock_startup(pobj);

	Ov_GetVTablePtr(fbcomlib_getSingle, stable, getsingle);

	ksapigetsingle = (OV_INSTPTR_ksapi_getSingle)Ov_GetFirstChild(fbcomlib_FBComCommonAssoc, getsingle);
	if(! ksapigetsingle){
	  res = Ov_CreateObject(ksapi_getSingle, ksapigetsingle, getsingle, "getSingle");
	  if (Ov_Fail(res)){
		ov_logfile_error("Error while creating the getSingle");
		fbcomlib_FBComCommon_state_set((OV_INSTPTR_fbcomlib_FBComCommon)getsingle, STATE_OVERROR);
		return;
	  }
	  Ov_Link(fbcomlib_FBComCommonAssoc, getsingle, ksapigetsingle);
	}
	Ov_GetVTablePtr(ksapi_getSingle, mtable, ksapigetsingle);
	getsingle->v_doSend = FALSE;

	//(re)register the return method
	mtable->m_registerMethod((OV_INSTPTR_ksapi_KSCommon) ksapigetsingle, stable->m_retMethod);
	fbcomlib_FBComCommon_state_set((OV_INSTPTR_fbcomlib_FBComCommon)getsingle, STATE_OK);
	return;

}

/**
 * Evaluates the answer of ksapi/getSingle.
 * Uses the FBComCommon/retMethod for status code and string
 * Additionally sets the receivedSingle.
 */
OV_DLLFNCEXPORT void fbcomlib_getSingle_retMethod(
		OV_INSTPTR_ov_object 		pobj,
		OV_STRING 						errorstring,
		OV_INT	 						errorcode
) {
	OV_INSTPTR_ksapi_getSingle ksapigetsingle = Ov_StaticPtrCast(ksapi_getSingle, pobj);
	OV_INSTPTR_fbcomlib_getSingle getsingle = (OV_INSTPTR_fbcomlib_getSingle)Ov_GetParent(fbcomlib_FBComCommonAssoc, ksapigetsingle);

	fbcomlib_FBComCommon_retMethod(pobj, errorstring, errorcode);
	//get the single
	fbcomlib_getSingle_receivedSingle_set(getsingle, ksapigetsingle->v_receivesingle);
	getsingle->v_varQState = ksapigetsingle->v_varQState;
	getsingle->v_varTimeStamp = ksapigetsingle->v_varTimeStamp;

}

