/**
 * @file getSingleVec.c
 * FunctionBlock for retrieving a remote Single Vector.
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
 * The received SingleVec is stored here
 */
OV_DLLFNCEXPORT OV_SINGLE * fbcomlib_getSingleVec_receivedSingleVec_get(
	OV_INSTPTR_fbcomlib_getSingleVec	pobj,
	OV_UINT													*pveclen
) {
	*pveclen = pobj->v_receivedSingleVec.veclen;
	return pobj->v_receivedSingleVec.value;
}

/**
 * The received SingleVec is stored here.
 * This Set shouldnt be used from outside!
 */
OV_DLLFNCEXPORT OV_RESULT fbcomlib_getSingleVec_receivedSingleVec_set(
	OV_INSTPTR_fbcomlib_getSingleVec	pobj,
	const OV_SINGLE										*pvalue,
	const OV_UINT											veclen
) {
	if(!(pobj->v_doSend)){
		return Ov_SetDynamicVectorValue(&pobj->v_receivedSingleVec,pvalue,veclen,SINGLE);
	}
	else{
		return OV_ERR_GENERIC;
	}	
}

/**
 * If TRUE, does the reset FBComCommon and removed receivedSingleVec additionally
 * @todo Could the GET be removed???
 */
OV_DLLFNCEXPORT OV_BOOL fbcomlib_getSingleVec_doReset_get(
	    OV_INSTPTR_fbcomlib_getSingleVec          pobj
) {
    return pobj->v_doResetAbstract;
}

/**
 * If TRUE, does the reset FBComCommon and removed receivedSingleVec additionally
 */
OV_DLLFNCEXPORT OV_RESULT fbcomlib_getSingleVec_doReset_set(
	    OV_INSTPTR_fbcomlib_getSingleVec          pobj,
	    const OV_BOOL  value
) {
	if(value){
		fbcomlib_FBComCommon_doResetAbstract_set((OV_INSTPTR_fbcomlib_FBComCommon)pobj, value);
		fbcomlib_getSingleVec_receivedSingleVec_set(pobj, 0, 1);
	}
	return OV_ERR_OK;
}



/**
 * Individual startup method for creating ksapi_getsinglevec object for later communication
 */
OV_DLLFNCEXPORT void fbcomlib_getSingleVec_startup(
	OV_INSTPTR_ov_object 	pobj
) {
    OV_INSTPTR_fbcomlib_getSingleVec 					getsinglevec = Ov_StaticPtrCast(fbcomlib_getSingleVec, pobj);
	OV_VTBLPTR_fbcomlib_getSingleVec					stable = NULL;

	OV_INSTPTR_ksapi_getSingleVec						ksapigetsinglevec = NULL;
	OV_VTBLPTR_ksapi_getSingleVec						mtable = NULL;
	OV_RESULT res = 0;

    // do what the base class does first
    fb_functionblock_startup(pobj);

	Ov_GetVTablePtr(fbcomlib_getSingleVec, stable, getsinglevec);

	ksapigetsinglevec = (OV_INSTPTR_ksapi_getSingleVec)Ov_GetFirstChild(fbcomlib_FBComCommonAssoc, getsinglevec);
	if(! ksapigetsinglevec){
	  res = Ov_CreateObject(ksapi_getSingleVec, ksapigetsinglevec, getsinglevec, "getSingleVec");
	  if (Ov_Fail(res)){
		ov_logfile_error("Error while creating the getSingleVec");
		fbcomlib_FBComCommon_state_set((OV_INSTPTR_fbcomlib_FBComCommon)getsinglevec, STATE_OVERROR);
		return;
	  }
	  Ov_Link(fbcomlib_FBComCommonAssoc, getsinglevec, ksapigetsinglevec);
	}
	Ov_GetVTablePtr(ksapi_getSingleVec, mtable, ksapigetsinglevec);
	getsinglevec->v_doSend = FALSE;

	//(re)register the return method
	mtable->m_registerMethod((OV_INSTPTR_ksapi_KSCommon) ksapigetsinglevec, stable->m_retMethod);
	fbcomlib_FBComCommon_state_set((OV_INSTPTR_fbcomlib_FBComCommon)getsinglevec, STATE_OK);
	return;

}

/**
 * Evaluates the answer of ksapi/getSingleVec.
 * Uses the FBComCommon/retMethod for status code and string
 * Additionally sets the receivedSingleVec.
 */
OV_DLLFNCEXPORT void fbcomlib_getSingleVec_retMethod(
		OV_INSTPTR_ov_object 		pobj,
		OV_STRING 						errorstring,
		OV_INT	 						errorcode
) {
	OV_INSTPTR_ksapi_getSingleVec ksapigetsinglevec = Ov_StaticPtrCast(ksapi_getSingleVec, pobj);
	OV_INSTPTR_fbcomlib_getSingleVec getsinglevec = (OV_INSTPTR_fbcomlib_getSingleVec)Ov_GetParent(fbcomlib_FBComCommonAssoc, ksapigetsinglevec);

	fbcomlib_FBComCommon_retMethod(pobj, errorstring, errorcode);
	//get the singlevec
	fbcomlib_getSingleVec_receivedSingleVec_set(getsinglevec, ksapigetsinglevec->v_receivesinglevec.value, ksapigetsinglevec->v_receivesinglevec.veclen);

}
