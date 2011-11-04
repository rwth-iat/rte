/**
 * @file getIntVec.c
 * FunctionBlock for retrieving a remote Integer Vector.
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
 * The received IntVec is stored here
 */
OV_DLLFNCEXPORT OV_INT * fbcomlib_getIntVec_receivedIntVec_get(
	OV_INSTPTR_fbcomlib_getIntVec	pobj,
	OV_UINT													*pveclen
) {
	*pveclen = pobj->v_receivedIntVec.veclen;
	return pobj->v_receivedIntVec.value;
}

/**
 * The received IntVec is stored here.
 * This Set shouldnt be used from outside!
 */
OV_DLLFNCEXPORT OV_RESULT fbcomlib_getIntVec_receivedIntVec_set(
	OV_INSTPTR_fbcomlib_getIntVec	pobj,
	const OV_INT											*pvalue,
	const OV_UINT											veclen
) {
	if(!(pobj->v_doSend)){
		return Ov_SetDynamicVectorValue(&pobj->v_receivedIntVec,pvalue,veclen,INT);
	}
	else{
		return OV_ERR_GENERIC;
	}	
}

/**
 * If TRUE, does the reset FBComCommon and removed receivedIntVec additionally
 * @todo Could the GET be removed???
 */
OV_DLLFNCEXPORT OV_BOOL fbcomlib_getIntVec_doReset_get(
	    OV_INSTPTR_fbcomlib_getIntVec          pobj
) {
    return pobj->v_doResetAbstract;
}

/**
 * If TRUE, does the reset FBComCommon and removed receivedInt additionally
 */
OV_DLLFNCEXPORT OV_RESULT fbcomlib_getIntVec_doReset_set(
	    OV_INSTPTR_fbcomlib_getIntVec          pobj,
	    const OV_BOOL  value
) {
	if(value){
		fbcomlib_FBComCommon_doResetAbstract_set((OV_INSTPTR_fbcomlib_FBComCommon)pobj, value);
		fbcomlib_getIntVec_receivedIntVec_set(pobj, 0, 1);
	}
	return OV_ERR_OK;
}



/**
 * Individual startup method for creating ksapi_getintvec object for later communication
 */
OV_DLLFNCEXPORT void fbcomlib_getIntVec_startup(
	OV_INSTPTR_ov_object 	pobj
) {
    OV_INSTPTR_fbcomlib_getIntVec 					getintvec = Ov_StaticPtrCast(fbcomlib_getIntVec, pobj);
	OV_VTBLPTR_fbcomlib_getIntVec					stable = NULL;

	OV_INSTPTR_ksapi_getIntVec						ksapigetintvec = NULL;
	OV_VTBLPTR_ksapi_getIntVec						mtable = NULL;
	OV_RESULT res = 0;

    // do what the base class does first
    fb_functionblock_startup(pobj);

	Ov_GetVTablePtr(fbcomlib_getIntVec, stable, getintvec);

	ksapigetintvec = (OV_INSTPTR_ksapi_getIntVec)Ov_GetFirstChild(fbcomlib_FBComCommonAssoc, getintvec);
	if(! ksapigetintvec){
	  res = Ov_CreateObject(ksapi_getIntVec, ksapigetintvec, getintvec, "getIntVec");
	  if (Ov_Fail(res)){
		ov_logfile_error("Error while creating the getIntVec");
		fbcomlib_FBComCommon_state_set((OV_INSTPTR_fbcomlib_FBComCommon)getintvec, STATE_OVERROR);
		return;
	  }
	  Ov_Link(fbcomlib_FBComCommonAssoc, getintvec, ksapigetintvec);
	}
	Ov_GetVTablePtr(ksapi_getIntVec, mtable, ksapigetintvec);
	getintvec->v_doSend = FALSE;

	//(re)register the return method
	mtable->m_registerMethod((OV_INSTPTR_ksapi_KSCommon) ksapigetintvec, stable->m_retMethod);
	fbcomlib_FBComCommon_state_set((OV_INSTPTR_fbcomlib_FBComCommon)getintvec, STATE_OK);
	return;

}

/**
 * Evaluates the answer of ksapi/getIntVec.
 * Uses the FBComCommon/retMethod for status code and string
 * Additionally sets the receivedIntVec.
 */
OV_DLLFNCEXPORT void fbcomlib_getIntVec_retMethod(
		OV_INSTPTR_ov_object 		pobj,
		OV_STRING 						errorstring,
		OV_INT	 						errorcode
) {
	OV_INSTPTR_ksapi_getIntVec ksapigetintvec = Ov_StaticPtrCast(ksapi_getIntVec, pobj);
	OV_INSTPTR_fbcomlib_getIntVec getintvec = (OV_INSTPTR_fbcomlib_getIntVec)Ov_GetParent(fbcomlib_FBComCommonAssoc, ksapigetintvec);

	fbcomlib_FBComCommon_retMethod(pobj, errorstring, errorcode);
	//get the intvec
	fbcomlib_getIntVec_receivedIntVec_set(getintvec, ksapigetintvec->v_receiveintvec.value, ksapigetintvec->v_receiveintvec.veclen);

}
