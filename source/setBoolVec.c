/**
 * @file setBoolVec.c
 * FunctionBlock for setting a remote Bool Vector.
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
 * The BoolVec, witch has to be transmitted
 * This Get shouldnt be used from outside!
 * Well - it could....
 */
OV_DLLFNCEXPORT OV_BOOL * fbcomlib_setBoolVec_sendboolvec_get(
	OV_INSTPTR_fbcomlib_setBoolVec	pobj,
	OV_UINT													*pveclen
) {
	*pveclen = pobj->v_sendboolvec.veclen;
	return pobj->v_sendboolvec.value;
}

/**
 * The BoolVec, witch has to be transmitted
*/
OV_DLLFNCEXPORT OV_RESULT fbcomlib_setBoolVec_sendboolvec_set(
	OV_INSTPTR_fbcomlib_setBoolVec	pobj,
	const OV_BOOL											*pvalue,
	const OV_UINT											veclen
) {
	if(!(pobj->v_doSend)){
		return Ov_SetDynamicVectorValue(&pobj->v_sendboolvec,pvalue,veclen,BOOL);
	}
	else{
		return OV_ERR_GENERIC;
	}	
}

/**
 * If TRUE, does the reset FBComCommon and removes sendboolvec additionally
 * @todo Could the GET be removed???
 */
OV_DLLFNCEXPORT OV_BOOL fbcomlib_setBoolVec_doReset_get(
	    OV_INSTPTR_fbcomlib_setBoolVec          pobj
) {
    return pobj->v_doResetAbstract;
}

/**
 * If TRUE, Does the reset FBComCommon and removes sendboolvec additionally
 */
OV_DLLFNCEXPORT OV_RESULT fbcomlib_setBoolVec_doReset_set(
	    OV_INSTPTR_fbcomlib_setBoolVec          pobj,
	    const OV_BOOL  value
) {
	if(value){
		fbcomlib_FBComCommon_doResetAbstract_set((OV_INSTPTR_fbcomlib_FBComCommon)pobj, value);
		fbcomlib_setBoolVec_sendboolvec_set(pobj, 0, 1);
	}
	return OV_ERR_OK;
}

/**
 * Individual startup method for creating ksapi_setboolvec object for later communication
 */
OV_DLLFNCEXPORT void fbcomlib_setBoolVec_startup(
	OV_INSTPTR_ov_object 	pobj
) {
    //~ OV_INSTPTR_fbcomlib_setBoolVec pinst = Ov_StaticPtrCast(fbcomlib_setBoolVec, pobj);

    OV_INSTPTR_fbcomlib_setBoolVec				setboolvec = Ov_StaticPtrCast(fbcomlib_setBoolVec, pobj);
	OV_VTBLPTR_fbcomlib_setBoolVec					stable = NULL;

	OV_INSTPTR_ksapi_setBoolVec						ksapisetboolvec = NULL;
	OV_VTBLPTR_ksapi_setBoolVec						mtable = NULL;
	OV_RESULT res = 0;

    // do what the base class does first
    fb_functionblock_startup(pobj);
	Ov_GetVTablePtr(fbcomlib_setBoolVec, stable, setboolvec);

	ksapisetboolvec = (OV_INSTPTR_ksapi_setBoolVec)Ov_GetFirstChild(fbcomlib_FBComCommonAssoc, setboolvec);
	if(! ksapisetboolvec){
	  res = Ov_CreateObject(ksapi_setBoolVec, ksapisetboolvec, setboolvec, "setBoolVec");
	  if (Ov_Fail(res)){
		ov_logfile_error("Error while creating the setBoolVec");
		fbcomlib_FBComCommon_state_set((OV_INSTPTR_fbcomlib_FBComCommon)setboolvec, STATE_OVERROR);
		return;
	  }
	  Ov_Link(fbcomlib_FBComCommonAssoc, setboolvec, ksapisetboolvec);
	}

	Ov_GetVTablePtr(ksapi_setBoolVec, mtable, ksapisetboolvec);
	//@todo Better a doReset here???
	setboolvec->v_doSend = FALSE;

	//register the return method
	mtable->m_registerMethod((OV_INSTPTR_ksapi_KSCommon) ksapisetboolvec, stable->m_retMethod);
	fbcomlib_FBComCommon_state_set((OV_INSTPTR_fbcomlib_FBComCommon)setboolvec, STATE_OK);
    return;
}

/**
 * @todo Will be removed by martin since get* Commands could use a general FBComCommon/typemethod
 */
OV_DLLFNCEXPORT void fbcomlib_setBoolVec_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
    OV_INSTPTR_fbcomlib_FBComCommon 				fbcommon = Ov_StaticPtrCast(fbcomlib_FBComCommon, pfb);
    OV_INSTPTR_fbcomlib_setBoolVec 					setBoolVec = Ov_StaticPtrCast(fbcomlib_setBoolVec, pfb);
	OV_INSTPTR_ksapi_setBoolVec						ksapisetboolvec = NULL;
	OV_VTBLPTR_ksapi_setBoolVec						mtable = NULL;
	OV_TIME ttemp;
	//OV_INSTPTR_ksapi_KSCommon						kscommon = NULL;
	//OV_VTBLPTR_ksapi_KSCommon						mtable = NULL;

	//check if something should be sent and if we are in a final state
	if ((fbcommon->v_doSend || fbcommon->v_doCyclic)
			&& fbcommon->v_state != STATE_INIT
			&& fbcommon->v_state != STATE_CLIENTBUSY){
		fbcommon->v_doSend = TRUE;
		//check if all values are set
		if (!((fbcommon->v_server)&&(fbcommon->v_host)&&(fbcommon->v_path))){
			ov_logfile_error("Please set all values: server,host,path");
			fbcomlib_FBComCommon_state_set(fbcommon, STATE_WRONGINPUT);
			return;
		}
		ksapisetboolvec = (OV_INSTPTR_ksapi_setBoolVec)Ov_GetFirstChild(fbcomlib_FBComCommonAssoc, fbcommon);
		if(! ksapisetboolvec) {
			ov_logfile_error("setBool typemethod hasnt got a ksapi setboolvec obj");
			return;
		}
		//get variable pointer
		Ov_GetVTablePtr(ksapi_setBoolVec, mtable, ksapisetboolvec);

		//set time-stamp to current time
		ov_time_gettime(&ttemp);
		ksapi_setBoolVec_varTimeStamp_set(ksapisetboolvec, &ttemp);
		//set qaulity state to good
		ksapi_setBoolVec_varQState_set(ksapisetboolvec, 4);
		//start sending
		mtable->m_setandsubmit(ksapisetboolvec,
				fbcommon->v_host,
				fbcommon->v_server,
				fbcommon->v_path,
				setBoolVec->v_sendboolvec.value, setBoolVec->v_sendboolvec.veclen);
	}
	return;


}

