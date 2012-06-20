/**
 * @file setDoubleVec.c
 * FunctionBlock for setting a remote Double Vector.
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
 * The DoubleVec, witch has to be transmitted
 * This Get shouldnt be used from outside!
 * Well - it could....
 */
OV_DLLFNCEXPORT OV_DOUBLE * fbcomlib_setDoubleVec_senddoublevec_get(
		OV_INSTPTR_fbcomlib_setDoubleVec	pobj,
		OV_UINT													*pveclen
) {
	*pveclen = pobj->v_senddoublevec.veclen;
	return pobj->v_senddoublevec.value;
}

/**
 * The DoubleVec, witch has to be transmitted
 */
OV_DLLFNCEXPORT OV_RESULT fbcomlib_setDoubleVec_senddoublevec_set(
		OV_INSTPTR_fbcomlib_setDoubleVec	pobj,
		const OV_DOUBLE										*pvalue,
		const OV_UINT											veclen
) {
	if(!(pobj->v_doSend)){
		return Ov_SetDynamicVectorValue(&pobj->v_senddoublevec,pvalue,veclen,DOUBLE);
	}
	else{
		return OV_ERR_GENERIC;
	}	
}

/**
 * If TRUE, does the reset FBComCommon and removes senddoublevec additionally
 * @todo Could the GET be removed???
 */
OV_DLLFNCEXPORT OV_BOOL fbcomlib_setDoubleVec_doReset_get(
		OV_INSTPTR_fbcomlib_setDoubleVec          pobj
) {
	return pobj->v_doResetAbstract;
}

/**
 * If TRUE, Does the reset FBComCommon and removes senddoublevec additionally
 */
OV_DLLFNCEXPORT OV_RESULT fbcomlib_setDoubleVec_doReset_set(
		OV_INSTPTR_fbcomlib_setDoubleVec          pobj,
		const OV_BOOL  value
) {
	if(value){
		fbcomlib_FBComCommon_doResetAbstract_set((OV_INSTPTR_fbcomlib_FBComCommon)pobj, value);
		fbcomlib_setDoubleVec_senddoublevec_set(pobj, 0, 1);
	}
	return OV_ERR_OK;
}

/**
 * Individual startup method for creating ksapi_setdoublevec object for later communication
 */
OV_DLLFNCEXPORT void fbcomlib_setDoubleVec_startup(
		OV_INSTPTR_ov_object 	pobj
) {
	//~ OV_INSTPTR_fbcomlib_setDoubleVec pinst = Ov_StaticPtrCast(fbcomlib_setDoubleVec, pobj);

	OV_INSTPTR_fbcomlib_setDoubleVec				setdoublevec = Ov_StaticPtrCast(fbcomlib_setDoubleVec, pobj);
	OV_VTBLPTR_fbcomlib_setDoubleVec					stable = NULL;

	OV_INSTPTR_ksapi_setDoubleVec						ksapisetdoublevec = NULL;
	OV_VTBLPTR_ksapi_setDoubleVec						mtable = NULL;
	OV_RESULT res = 0;

	// do what the base class does first
	fb_functionblock_startup(pobj);
	Ov_GetVTablePtr(fbcomlib_setDoubleVec, stable, setdoublevec);

	ksapisetdoublevec = (OV_INSTPTR_ksapi_setDoubleVec)Ov_GetFirstChild(fbcomlib_FBComCommonAssoc, setdoublevec);
	if(! ksapisetdoublevec){
		res = Ov_CreateObject(ksapi_setDoubleVec, ksapisetdoublevec, setdoublevec, "setDoubleVec");
		if (Ov_Fail(res)){
			ov_logfile_error("Error while creating the setDoubleVec");
			fbcomlib_FBComCommon_state_set((OV_INSTPTR_fbcomlib_FBComCommon)setdoublevec, STATE_OVERROR);
			return;
		}
		Ov_Link(fbcomlib_FBComCommonAssoc, setdoublevec, ksapisetdoublevec);
	}

	Ov_GetVTablePtr(ksapi_setDoubleVec, mtable, ksapisetdoublevec);
	//@todo Better a doReset here???
	setdoublevec->v_doSend = FALSE;

	//register the return method
	mtable->m_registerMethod((OV_INSTPTR_ksapi_KSCommon) ksapisetdoublevec, stable->m_retMethod);
	fbcomlib_FBComCommon_state_set((OV_INSTPTR_fbcomlib_FBComCommon)setdoublevec, STATE_OK);
	return;
}

/**
 * @todo Will be removed by martin since get* Commands could use a general FBComCommon/typemethod
 */
OV_DLLFNCEXPORT void fbcomlib_setDoubleVec_typemethod(
		OV_INSTPTR_fb_functionblock	pfb,
		OV_TIME						*pltc
) {
	OV_INSTPTR_fbcomlib_FBComCommon 				fbcommon = Ov_StaticPtrCast(fbcomlib_FBComCommon, pfb);
	OV_INSTPTR_fbcomlib_setDoubleVec 					setDoubleVec = Ov_StaticPtrCast(fbcomlib_setDoubleVec, pfb);
	OV_INSTPTR_ksapi_setDoubleVec						ksapisetdoublevec = NULL;
	OV_VTBLPTR_ksapi_setDoubleVec						mtable = NULL;
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
		ksapisetdoublevec = (OV_INSTPTR_ksapi_setDoubleVec)Ov_GetFirstChild(fbcomlib_FBComCommonAssoc, fbcommon);
		if(! ksapisetdoublevec) {
			ov_logfile_error("setDouble typemethod hasnt got a ksapi setdoublevec obj");
			return;
		}
		//get variable pointer
		Ov_GetVTablePtr(ksapi_setDoubleVec, mtable, ksapisetdoublevec);
		//start sending
		mtable->m_setandsubmit(ksapisetdoublevec,
				fbcommon->v_host,
				fbcommon->v_server,
				fbcommon->v_path,
				setDoubleVec->v_senddoublevec.value, setDoubleVec->v_senddoublevec.veclen);
	}
	return;


}

