/**
 * @file setIntVec.c
 * FunctionBlock for setting a remote Integer Vector.
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
 * The IntVec, witch has to be transmitted
 * This Get shouldnt be used from outside!
 * Well - it could....
 */
OV_DLLFNCEXPORT OV_INT * fbcomlib_setIntVec_sendintvec_get(
	OV_INSTPTR_fbcomlib_setIntVec	pobj,
	OV_UINT													*pveclen
) {
	*pveclen = pobj->v_sendintvec.veclen;
	return pobj->v_sendintvec.value;
}

/**
 * The Integer, witch has to be transmitted
*/
OV_DLLFNCEXPORT OV_RESULT fbcomlib_setIntVec_sendintvec_set(
	OV_INSTPTR_fbcomlib_setIntVec	pobj,
	const OV_INT											*pvalue,
	const OV_UINT											veclen
) {
	if(!(pobj->v_doSend)){
		return Ov_SetDynamicVectorValue(&pobj->v_sendintvec,pvalue,veclen,INT);
	}
	else{
		return OV_ERR_GENERIC;
	}	
}

/**
 * If TRUE, does the reset FBComCommon and removes sendintvec additionally
 * @todo Could the GET be removed???
 */
OV_DLLFNCEXPORT OV_BOOL fbcomlib_setIntVec_doReset_get(
	    OV_INSTPTR_fbcomlib_setIntVec          pobj
) {
    return pobj->v_doResetAbstract;
}

/**
 * If TRUE, Does the reset FBComCommon and removes sendintvec additionally
 */
OV_DLLFNCEXPORT OV_RESULT fbcomlib_setIntVec_doReset_set(
	    OV_INSTPTR_fbcomlib_setIntVec          pobj,
	    const OV_BOOL  value
) {
	if(value){
		fbcomlib_FBComCommon_doResetAbstract_set((OV_INSTPTR_fbcomlib_FBComCommon)pobj, value);
		fbcomlib_setIntVec_sendintvec_set(pobj, 0, 1);
	}
	return OV_ERR_OK;
}

/**
 * Individual startup method for creating ksapi_setintvec object for later communication
 */
OV_DLLFNCEXPORT void fbcomlib_setIntVec_startup(
	OV_INSTPTR_ov_object 	pobj
) {
    //~ OV_INSTPTR_fbcomlib_setIntVec pinst = Ov_StaticPtrCast(fbcomlib_setIntVec, pobj);

    OV_INSTPTR_fbcomlib_setIntVec				setintvec = Ov_StaticPtrCast(fbcomlib_setIntVec, pobj);
	OV_VTBLPTR_fbcomlib_setIntVec					stable = NULL;

	OV_INSTPTR_ksapi_setIntVec						ksapisetintvec = NULL;
	OV_VTBLPTR_ksapi_setIntVec						mtable = NULL;
	OV_RESULT res = 0;

    // do what the base class does first
    fb_functionblock_startup(pobj);
	Ov_GetVTablePtr(fbcomlib_setIntVec, stable, setintvec);

	ksapisetintvec = (OV_INSTPTR_ksapi_setIntVec)Ov_GetFirstChild(fbcomlib_FBComCommonAssoc, setintvec);
	if(! ksapisetintvec){
	  res = Ov_CreateObject(ksapi_setIntVec, ksapisetintvec, setintvec, "setIntVec");
	  if (Ov_Fail(res)){
		ov_logfile_error("Error while creating the setIntVec");
		fbcomlib_FBComCommon_state_set((OV_INSTPTR_fbcomlib_FBComCommon)setintvec, STATE_OVERROR);
		return;
	  }
	  Ov_Link(fbcomlib_FBComCommonAssoc, setintvec, ksapisetintvec);
	}

	Ov_GetVTablePtr(ksapi_setIntVec, mtable, ksapisetintvec);
	//@todo Better a doReset here???
	setintvec->v_doSend = FALSE;

	//register the return method
	mtable->m_registerMethod((OV_INSTPTR_ksapi_KSCommon) ksapisetintvec, stable->m_retMethod);
	fbcomlib_FBComCommon_state_set((OV_INSTPTR_fbcomlib_FBComCommon)setintvec, STATE_OK);
    return;
}

/**
 * @todo Will be removed by martin since get* Commands could use a general FBComCommon/typemethod
 */
OV_DLLFNCEXPORT void fbcomlib_setIntVec_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
    OV_INSTPTR_fbcomlib_FBComCommon 				fbcommon = Ov_StaticPtrCast(fbcomlib_FBComCommon, pfb);
    OV_INSTPTR_fbcomlib_setIntVec 					setIntVec = Ov_StaticPtrCast(fbcomlib_setIntVec, pfb);
	OV_INSTPTR_ksapi_setIntVec						ksapisetintvec = NULL;
	OV_VTBLPTR_ksapi_setIntVec						mtable = NULL;
	//OV_INSTPTR_ksapi_KSCommon						kscommon = NULL;
	//OV_VTBLPTR_ksapi_KSCommon						mtable = NULL;

	//check if send was set to true
	if (fbcommon->v_doSend){
		fbcommon->v_doSend = FALSE;
		//check if all values are set
		if (!((fbcommon->v_server)&&(fbcommon->v_host)&&(fbcommon->v_path))){
			ov_logfile_error("Please set all values: server,host,path");
			fbcomlib_FBComCommon_state_set(fbcommon, STATE_WRONGINPUT);
			return;
		}
		ksapisetintvec = (OV_INSTPTR_ksapi_setIntVec)Ov_GetFirstChild(fbcomlib_FBComCommonAssoc, fbcommon);
		if(! ksapisetintvec) {
			ov_logfile_error("setInt typemethod hasnt got a ksapi setintvec obj");
			return;
		}
		//get variable pointer
		Ov_GetVTablePtr(ksapi_setIntVec, mtable, ksapisetintvec);
		//start sending
		mtable->m_setandsubmit(ksapisetintvec,
				fbcommon->v_host,
				fbcommon->v_server,
				fbcommon->v_path,
				setIntVec->v_sendintvec.value, setIntVec->v_sendintvec.veclen);
	}
	return;


}

