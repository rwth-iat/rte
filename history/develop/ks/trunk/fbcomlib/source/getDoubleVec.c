/**
 * @file getDoubleVec.c
 * FunctionBlock for retrieving a remote Double Vector.
 * Inherits data fields from FBComCommon.
 */
#ifndef OV_COMPILE_LIBRARY_fbcomlib
#define OV_COMPILE_LIBRARY_fbcomlib
#endif


#include "fbcomlib.h"
#include "../../include/config.h"
#include "libov/ov_macros.h"
#include "libov/ov_logfile.h"

/**
 * The received DoubleVec is stored here
 */
OV_DLLFNCEXPORT OV_DOUBLE * fbcomlib_getDoubleVec_receivedDoubleVec_get(
	OV_INSTPTR_fbcomlib_getDoubleVec	pobj,
	OV_UINT													*pveclen
) {
	*pveclen = pobj->v_receivedDoubleVec.veclen;
	return pobj->v_receivedDoubleVec.value;
}

/**
 * The received DoubleVec is stored here.
 * This Set shouldnt be used from outside!
 */
OV_DLLFNCEXPORT OV_RESULT fbcomlib_getDoubleVec_receivedDoubleVec_set(
	OV_INSTPTR_fbcomlib_getDoubleVec	pobj,
	const OV_DOUBLE										*pvalue,
	const OV_UINT											veclen
) {

		return Ov_SetDynamicVectorValue(&pobj->v_receivedDoubleVec,pvalue,veclen,DOUBLE);
}

/**
 * If TRUE, does the reset FBComCommon and removed receivedDoubleVec additionally
 * @todo Could the GET be removed???
 */
OV_DLLFNCEXPORT OV_BOOL fbcomlib_getDoubleVec_doReset_get(
	    OV_INSTPTR_fbcomlib_getDoubleVec          pobj
) {
    return pobj->v_doResetAbstract;
}

/**
 * If TRUE, does the reset FBComCommon and removed receivedDoubleVec additionally
 */
OV_DLLFNCEXPORT OV_RESULT fbcomlib_getDoubleVec_doReset_set(
	    OV_INSTPTR_fbcomlib_getDoubleVec          pobj,
	    const OV_BOOL  value
) {
	if(value){
		fbcomlib_FBComCommon_doResetAbstract_set((OV_INSTPTR_fbcomlib_FBComCommon)pobj, value);
		fbcomlib_getDoubleVec_receivedDoubleVec_set(pobj, 0, 1);
	}
	return OV_ERR_OK;
}



/**
 * Individual startup method for creating ksapi_getdoublevec object for later communication
 */
OV_DLLFNCEXPORT void fbcomlib_getDoubleVec_startup(
	OV_INSTPTR_ov_object 	pobj
) {
    OV_INSTPTR_fbcomlib_getDoubleVec 					getdoublevec = Ov_StaticPtrCast(fbcomlib_getDoubleVec, pobj);
	OV_VTBLPTR_fbcomlib_getDoubleVec					stable = NULL;

	OV_INSTPTR_ksapi_getDoubleVec						ksapigetdoublevec = NULL;
	OV_VTBLPTR_ksapi_getDoubleVec						mtable = NULL;
	OV_RESULT res = 0;

    // do what the base class does first
    fb_functionblock_startup(pobj);

	Ov_GetVTablePtr(fbcomlib_getDoubleVec, stable, getdoublevec);

	ksapigetdoublevec = (OV_INSTPTR_ksapi_getDoubleVec)Ov_GetFirstChild(fbcomlib_FBComCommonAssoc, getdoublevec);
	if(! ksapigetdoublevec){
	  res = Ov_CreateObject(ksapi_getDoubleVec, ksapigetdoublevec, getdoublevec, "getDoubleVec");
	  if (Ov_Fail(res)){
		ov_logfile_error("Error while creating the getDoubleVec");
		fbcomlib_FBComCommon_state_set((OV_INSTPTR_fbcomlib_FBComCommon)getdoublevec, STATE_OVERROR);
		return;
	  }
	  Ov_Link(fbcomlib_FBComCommonAssoc, getdoublevec, ksapigetdoublevec);
	}
	Ov_GetVTablePtr(ksapi_getDoubleVec, mtable, ksapigetdoublevec);
	getdoublevec->v_doSend = FALSE;

	//(re)register the return method
	mtable->m_registerMethod((OV_INSTPTR_ksapi_KSCommon) ksapigetdoublevec, stable->m_retMethod);
	fbcomlib_FBComCommon_state_set((OV_INSTPTR_fbcomlib_FBComCommon)getdoublevec, STATE_OK);
	return;

}

/**
 * Evaluates the answer of ksapi/getDoubleVec.
 * Uses the FBComCommon/retMethod for status code and string
 * Additionally sets the receivedDoubleVec.
 */
OV_DLLFNCEXPORT void fbcomlib_getDoubleVec_retMethod(
		OV_INSTPTR_ov_object 		pobj,
		OV_STRING 						errorstring,
		OV_INT	 						errorcode
) {
	OV_INSTPTR_ksapi_getDoubleVec ksapigetdoublevec = Ov_StaticPtrCast(ksapi_getDoubleVec, pobj);
	OV_INSTPTR_fbcomlib_getDoubleVec getdoublevec = (OV_INSTPTR_fbcomlib_getDoubleVec)Ov_GetParent(fbcomlib_FBComCommonAssoc, ksapigetdoublevec);

	fbcomlib_FBComCommon_retMethod(pobj, errorstring, errorcode);
	//get the doublevec
	fbcomlib_getDoubleVec_receivedDoubleVec_set(getdoublevec, ksapigetdoublevec->v_receivedoublevec.value, ksapigetdoublevec->v_receivedoublevec.veclen);

}
