/**
 * @file getBoolVec.c
 * FunctionBlock for retrieving a remote Bool Vector.
 * Inherits data fields from FBComCommon.
 */
#ifndef OV_COMPILE_LIBRARY_fbcomlib
#define OV_COMPILE_LIBRARY_fbcomlib
#endif


#include "fbcomlib.h"
#include "../../include/config.h"
#include "libov/ov_macros.h"
#include "libov/ov_logfile.h"

OV_DLLFNCEXPORT OV_TIME* fbcomlib_getBoolVec_varTimeStamp_get(
    OV_INSTPTR_fbcomlib_getBoolVec          pobj
) {
    return &pobj->v_varTimeStamp;
}

OV_DLLFNCEXPORT OV_RESULT fbcomlib_getBoolVec_varTimeStamp_set(
    OV_INSTPTR_fbcomlib_getBoolVec          pobj,
    const OV_TIME*  value
) {
    pobj->v_varTimeStamp = *value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_UINT fbcomlib_getBoolVec_varQState_get(
    OV_INSTPTR_fbcomlib_getBoolVec          pobj
) {
    return pobj->v_varQState;
}

OV_DLLFNCEXPORT OV_RESULT fbcomlib_getBoolVec_varQState_set(
    OV_INSTPTR_fbcomlib_getBoolVec          pobj,
    const OV_UINT  value
) {
    pobj->v_varQState = value;
    return OV_ERR_OK;
}

/**
 * The received BoolVec is stored here
 */
OV_DLLFNCEXPORT OV_BOOL * fbcomlib_getBoolVec_receivedBoolVec_get(
	OV_INSTPTR_fbcomlib_getBoolVec	pobj,
	OV_UINT													*pveclen
) {
	*pveclen = pobj->v_receivedBoolVec.veclen;
	return pobj->v_receivedBoolVec.value;
}

/**
 * The received BoolVec is stored here.
 * This Set shouldnt be used from outside!
 */
OV_DLLFNCEXPORT OV_RESULT fbcomlib_getBoolVec_receivedBoolVec_set(
	OV_INSTPTR_fbcomlib_getBoolVec	pobj,
	const OV_BOOL											*pvalue,
	const OV_UINT											veclen
) {

		return Ov_SetDynamicVectorValue(&pobj->v_receivedBoolVec,pvalue,veclen,BOOL);
}

/**
 * If TRUE, does the reset FBComCommon and removed receivedBoolVec additionally
 * @todo Could the GET be removed???
 */
OV_DLLFNCEXPORT OV_BOOL fbcomlib_getBoolVec_doReset_get(
	    OV_INSTPTR_fbcomlib_getBoolVec          pobj
) {
    return pobj->v_doResetAbstract;
}

/**
 * If TRUE, does the reset FBComCommon and removed receivedBoolVec additionally
 */
OV_DLLFNCEXPORT OV_RESULT fbcomlib_getBoolVec_doReset_set(
	    OV_INSTPTR_fbcomlib_getBoolVec          pobj,
	    const OV_BOOL  value
) {
	if(value){
		fbcomlib_FBComCommon_doResetAbstract_set((OV_INSTPTR_fbcomlib_FBComCommon)pobj, value);
		fbcomlib_getBoolVec_receivedBoolVec_set(pobj, 0, 1);
	}
	return OV_ERR_OK;
}



/**
 * Individual startup method for creating ksapi_getboolvec object for later communication
 */
OV_DLLFNCEXPORT void fbcomlib_getBoolVec_startup(
	OV_INSTPTR_ov_object 	pobj
) {
    OV_INSTPTR_fbcomlib_getBoolVec 					getboolvec = Ov_StaticPtrCast(fbcomlib_getBoolVec, pobj);
	OV_VTBLPTR_fbcomlib_getBoolVec					stable = NULL;

	OV_INSTPTR_ksapi_getBoolVec						ksapigetboolvec = NULL;
	OV_VTBLPTR_ksapi_getBoolVec						mtable = NULL;
	OV_RESULT res = 0;

    // do what the base class does first
    fb_functionblock_startup(pobj);

	Ov_GetVTablePtr(fbcomlib_getBoolVec, stable, getboolvec);

	ksapigetboolvec = (OV_INSTPTR_ksapi_getBoolVec)Ov_GetFirstChild(fbcomlib_FBComCommonAssoc, getboolvec);
	if(! ksapigetboolvec){
	  res = Ov_CreateObject(ksapi_getBoolVec, ksapigetboolvec, getboolvec, "getBoolVec");
	  if (Ov_Fail(res)){
		ov_logfile_error("Error while creating the getBoolVec");
		fbcomlib_FBComCommon_state_set((OV_INSTPTR_fbcomlib_FBComCommon)getboolvec, STATE_OVERROR);
		return;
	  }
	  Ov_Link(fbcomlib_FBComCommonAssoc, getboolvec, ksapigetboolvec);
	}
	Ov_GetVTablePtr(ksapi_getBoolVec, mtable, ksapigetboolvec);
	getboolvec->v_doSend = FALSE;

	//(re)register the return method
	mtable->m_registerMethod((OV_INSTPTR_ksapi_KSCommon) ksapigetboolvec, stable->m_retMethod);
	fbcomlib_FBComCommon_state_set((OV_INSTPTR_fbcomlib_FBComCommon)getboolvec, STATE_OK);
	return;

}

/**
 * Evaluates the answer of ksapi/getBoolVec.
 * Uses the FBComCommon/retMethod for status code and string
 * Additionally sets the receivedBoolVec.
 */
OV_DLLFNCEXPORT void fbcomlib_getBoolVec_retMethod(
		OV_INSTPTR_ov_object 		pobj,
		OV_STRING 						errorstring,
		OV_INT	 						errorcode
) {
	OV_INSTPTR_ksapi_getBoolVec ksapigetboolvec = Ov_StaticPtrCast(ksapi_getBoolVec, pobj);
	OV_INSTPTR_fbcomlib_getBoolVec getboolvec = (OV_INSTPTR_fbcomlib_getBoolVec)Ov_GetParent(fbcomlib_FBComCommonAssoc, ksapigetboolvec);

	fbcomlib_FBComCommon_retMethod(pobj, errorstring, errorcode);
	//get the boolvec
	fbcomlib_getBoolVec_receivedBoolVec_set(getboolvec, ksapigetboolvec->v_receiveboolvec.value, ksapigetboolvec->v_receiveboolvec.veclen);
	getboolvec->v_varQState = ksapigetboolvec->v_varQState;
	getboolvec->v_varTimeStamp = ksapigetboolvec->v_varTimeStamp;

}
