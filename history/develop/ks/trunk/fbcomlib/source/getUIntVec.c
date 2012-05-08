/**
 * @file getUIntVec.c
 * FunctionBlock for retrieving a remote Unsigned Integer Vector.
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
 * The received UIntVec is stored here
 */
OV_DLLFNCEXPORT OV_UINT * fbcomlib_getUIntVec_receivedUIntVec_get(
	OV_INSTPTR_fbcomlib_getUIntVec	pobj,
	OV_UINT													*pveclen
) {
	*pveclen = pobj->v_receivedUIntVec.veclen;
	return pobj->v_receivedUIntVec.value;
}

/**
 * The received UIntVec is stored here.
 * This Set shouldnt be used from outside!
 */
OV_DLLFNCEXPORT OV_RESULT fbcomlib_getUIntVec_receivedUIntVec_set(
	OV_INSTPTR_fbcomlib_getUIntVec	pobj,
	const OV_UINT											*pvalue,
	const OV_UINT											veclen
) {

		return Ov_SetDynamicVectorValue(&pobj->v_receivedUIntVec,pvalue,veclen,UINT);

}

/**
 * If TRUE, does the reset FBComCommon and removed receivedUIntVec additionally
 * @todo Could the GET be removed???
 */
OV_DLLFNCEXPORT OV_BOOL fbcomlib_getUIntVec_doReset_get(
	    OV_INSTPTR_fbcomlib_getUIntVec          pobj
) {
    return pobj->v_doResetAbstract;
}

/**
 * If TRUE, does the reset FBComCommon and removed receivedUIntVec additionally
 */
OV_DLLFNCEXPORT OV_RESULT fbcomlib_getUIntVec_doReset_set(
	    OV_INSTPTR_fbcomlib_getUIntVec          pobj,
	    const OV_BOOL  value
) {
	if(value){
		fbcomlib_FBComCommon_doResetAbstract_set((OV_INSTPTR_fbcomlib_FBComCommon)pobj, value);
		fbcomlib_getUIntVec_receivedUIntVec_set(pobj, 0, 1);
	}
	return OV_ERR_OK;
}



/**
 * Individual startup method for creating ksapi_getuintvec object for later communication
 */
OV_DLLFNCEXPORT void fbcomlib_getUIntVec_startup(
	OV_INSTPTR_ov_object 	pobj
) {
    OV_INSTPTR_fbcomlib_getUIntVec 					getuintvec = Ov_StaticPtrCast(fbcomlib_getUIntVec, pobj);
	OV_VTBLPTR_fbcomlib_getUIntVec					stable = NULL;

	OV_INSTPTR_ksapi_getUIntVec						ksapigetuintvec = NULL;
	OV_VTBLPTR_ksapi_getUIntVec						mtable = NULL;
	OV_RESULT res = 0;

    // do what the base class does first
    fb_functionblock_startup(pobj);

	Ov_GetVTablePtr(fbcomlib_getUIntVec, stable, getuintvec);

	ksapigetuintvec = (OV_INSTPTR_ksapi_getUIntVec)Ov_GetFirstChild(fbcomlib_FBComCommonAssoc, getuintvec);
	if(! ksapigetuintvec){
	  res = Ov_CreateObject(ksapi_getUIntVec, ksapigetuintvec, getuintvec, "getUIntVec");
	  if (Ov_Fail(res)){
		ov_logfile_error("Error while creating the getUIntVec");
		fbcomlib_FBComCommon_state_set((OV_INSTPTR_fbcomlib_FBComCommon)getuintvec, STATE_OVERROR);
		return;
	  }
	  Ov_Link(fbcomlib_FBComCommonAssoc, getuintvec, ksapigetuintvec);
	}
	Ov_GetVTablePtr(ksapi_getUIntVec, mtable, ksapigetuintvec);
	getuintvec->v_doSend = FALSE;

	//(re)register the return method
	mtable->m_registerMethod((OV_INSTPTR_ksapi_KSCommon) ksapigetuintvec, stable->m_retMethod);
	fbcomlib_FBComCommon_state_set((OV_INSTPTR_fbcomlib_FBComCommon)getuintvec, STATE_OK);
	return;

}

/**
 * Evaluates the answer of ksapi/getUIntVec.
 * Uses the FBComCommon/retMethod for status code and string
 * Additionally sets the receivedUIntVec.
 */
OV_DLLFNCEXPORT void fbcomlib_getUIntVec_retMethod(
		OV_INSTPTR_ov_object 		pobj,
		OV_STRING 						errorstring,
		OV_INT	 						errorcode
) {
	OV_INSTPTR_ksapi_getUIntVec ksapigetuintvec = Ov_StaticPtrCast(ksapi_getUIntVec, pobj);
	OV_INSTPTR_fbcomlib_getUIntVec getuintvec = (OV_INSTPTR_fbcomlib_getUIntVec)Ov_GetParent(fbcomlib_FBComCommonAssoc, ksapigetuintvec);

	fbcomlib_FBComCommon_retMethod(pobj, errorstring, errorcode);
	//get the uint
	fbcomlib_getUIntVec_receivedUIntVec_set(getuintvec, ksapigetuintvec->v_receiveuintvec.value, ksapigetuintvec->v_receiveuintvec.veclen);

}
