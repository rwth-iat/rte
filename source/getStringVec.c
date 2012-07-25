/**
 * @file getStringVec.c
 * FunctionBlock for retrieving a remote String Vector.
 * Inherits data fields from FBComCommon.
 */
#ifndef OV_COMPILE_LIBRARY_fbcomlib
#define OV_COMPILE_LIBRARY_fbcomlib
#endif


#include "fbcomlib.h"
#include "../../include/config.h"
#include "libov/ov_macros.h"
#include "libov/ov_logfile.h"

OV_DLLFNCEXPORT OV_TIME* fbcomlib_getStringVec_varTimeStamp_get(
    OV_INSTPTR_fbcomlib_getStringVec          pobj
) {
    return &pobj->v_varTimeStamp;
}

OV_DLLFNCEXPORT OV_RESULT fbcomlib_getStringVec_varTimeStamp_set(
    OV_INSTPTR_fbcomlib_getStringVec          pobj,
    const OV_TIME*  value
) {
    pobj->v_varTimeStamp = *value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_UINT fbcomlib_getStringVec_varQState_get(
    OV_INSTPTR_fbcomlib_getStringVec          pobj
) {
    return pobj->v_varQState;
}

OV_DLLFNCEXPORT OV_RESULT fbcomlib_getStringVec_varQState_set(
    OV_INSTPTR_fbcomlib_getStringVec          pobj,
    const OV_UINT  value
) {
    pobj->v_varQState = value;
    return OV_ERR_OK;
}

/**
 * The received StringVec is stored here
 */
OV_DLLFNCEXPORT OV_STRING * fbcomlib_getStringVec_receivedStringVec_get(
	OV_INSTPTR_fbcomlib_getStringVec	pobj,
	OV_UINT													*pveclen
) {
	*pveclen = pobj->v_receivedStringVec.veclen;
	return pobj->v_receivedStringVec.value;
}

/**
 * The received StringVec is stored here.
 * This Set shouldnt be used from outside!
 */
OV_DLLFNCEXPORT OV_RESULT fbcomlib_getStringVec_receivedStringVec_set(
	OV_INSTPTR_fbcomlib_getStringVec	pobj,
	const OV_STRING										*pvalue,
	const OV_UINT											veclen
) {

		return Ov_SetDynamicVectorValue(&pobj->v_receivedStringVec,pvalue,veclen,STRING);

}

/**
 * If TRUE, does the reset FBComCommon and removed receivedStringVec additionally
 * @todo Could the GET be removed???
 */
OV_DLLFNCEXPORT OV_BOOL fbcomlib_getStringVec_doReset_get(
	    OV_INSTPTR_fbcomlib_getStringVec          pobj
) {
    return pobj->v_doResetAbstract;
}

/**
 * If TRUE, does the reset FBComCommon and removed receivedStringVec additionally
 */
OV_DLLFNCEXPORT OV_RESULT fbcomlib_getStringVec_doReset_set(
	    OV_INSTPTR_fbcomlib_getStringVec          pobj,
	    const OV_BOOL  value
) {
	if(value){
		fbcomlib_FBComCommon_doResetAbstract_set((OV_INSTPTR_fbcomlib_FBComCommon)pobj, value);
		fbcomlib_getStringVec_receivedStringVec_set(pobj, (char* const*)"", 1);
	}
	return OV_ERR_OK;
}



/**
 * Individual startup method for creating ksapi_getstringvec object for later communication
 */
OV_DLLFNCEXPORT void fbcomlib_getStringVec_startup(
	OV_INSTPTR_ov_object 	pobj
) {
    OV_INSTPTR_fbcomlib_getStringVec 					getstringvec = Ov_StaticPtrCast(fbcomlib_getStringVec, pobj);
	OV_VTBLPTR_fbcomlib_getStringVec					stable = NULL;

	OV_INSTPTR_ksapi_getStringVec						ksapigetstringvec = NULL;
	OV_VTBLPTR_ksapi_getStringVec						mtable = NULL;
	OV_RESULT res = 0;

    // do what the base class does first
    fb_functionblock_startup(pobj);

	Ov_GetVTablePtr(fbcomlib_getStringVec, stable, getstringvec);

	ksapigetstringvec = (OV_INSTPTR_ksapi_getStringVec)Ov_GetFirstChild(fbcomlib_FBComCommonAssoc, getstringvec);
	if(! ksapigetstringvec){
	  res = Ov_CreateObject(ksapi_getStringVec, ksapigetstringvec, getstringvec, "getStringVec");
	  if (Ov_Fail(res)){
		ov_logfile_error("Error while creating the getStringVec");
		fbcomlib_FBComCommon_state_set((OV_INSTPTR_fbcomlib_FBComCommon)getstringvec, STATE_OVERROR);
		return;
	  }
	  Ov_Link(fbcomlib_FBComCommonAssoc, getstringvec, ksapigetstringvec);
	}
	Ov_GetVTablePtr(ksapi_getStringVec, mtable, ksapigetstringvec);
	getstringvec->v_doSend = FALSE;

	//(re)register the return method
	mtable->m_registerMethod((OV_INSTPTR_ksapi_KSCommon) ksapigetstringvec, stable->m_retMethod);
	fbcomlib_FBComCommon_state_set((OV_INSTPTR_fbcomlib_FBComCommon)getstringvec, STATE_OK);
	return;

}

/**
 * Evaluates the answer of ksapi/getStringVec.
 * Uses the FBComCommon/retMethod for status code and string
 * Additionally sets the receivedStringVec.
 */
OV_DLLFNCEXPORT void fbcomlib_getStringVec_retMethod(
		OV_INSTPTR_ov_object 		pobj,
		OV_STRING 						errorstring,
		OV_INT	 						errorcode
) {
	OV_INSTPTR_ksapi_getStringVec ksapigetstringvec = Ov_StaticPtrCast(ksapi_getStringVec, pobj);
	OV_INSTPTR_fbcomlib_getStringVec getstringvec = (OV_INSTPTR_fbcomlib_getStringVec)Ov_GetParent(fbcomlib_FBComCommonAssoc, ksapigetstringvec);

	fbcomlib_FBComCommon_retMethod(pobj, errorstring, errorcode);
	//get the stringvec
	fbcomlib_getStringVec_receivedStringVec_set(getstringvec, ksapigetstringvec->v_receivestringvec.value, ksapigetstringvec->v_receivestringvec.veclen);
	getstringvec->v_varQState = ksapigetstringvec->v_varQState;
	getstringvec->v_varTimeStamp = ksapigetstringvec->v_varTimeStamp;

}
