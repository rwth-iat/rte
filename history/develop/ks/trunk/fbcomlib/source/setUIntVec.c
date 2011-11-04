/**
 * @file setUIntVec.c
 * FunctionBlock for setting a remote Unsigne Integer Vector.
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
 * The UIntVec, witch has to be transmitted
 * This Get shouldnt be used from outside!
 * Well - it could....
 */
OV_DLLFNCEXPORT OV_UINT * fbcomlib_setUIntVec_senduintvec_get(
	OV_INSTPTR_fbcomlib_setUIntVec	pobj,
	OV_UINT													*pveclen
) {
	*pveclen = pobj->v_senduintvec.veclen;
	return pobj->v_senduintvec.value;
}

/**
 * The UIntVec, witch has to be transmitted
*/
OV_DLLFNCEXPORT OV_RESULT fbcomlib_setUIntVec_senduintvec_set(
	OV_INSTPTR_fbcomlib_setUIntVec	pobj,
	const OV_UINT											*pvalue,
	const OV_UINT											veclen
) {
	if(!(pobj->v_doSend)){
		return Ov_SetDynamicVectorValue(&pobj->v_senduintvec,pvalue,veclen,UINT);
	}
	else{
		return OV_ERR_GENERIC;
	}	
}

/**
 * If TRUE, does the reset FBComCommon and removes senduintvec additionally
 * @todo Could the GET be removed???
 */
OV_DLLFNCEXPORT OV_BOOL fbcomlib_setUIntVec_doReset_get(
	    OV_INSTPTR_fbcomlib_setUIntVec          pobj
) {
    return pobj->v_doResetAbstract;
}

/**
 * If TRUE, Does the reset FBComCommon and removes senduintvec additionally
 */
OV_DLLFNCEXPORT OV_RESULT fbcomlib_setUIntVec_doReset_set(
	    OV_INSTPTR_fbcomlib_setUIntVec          pobj,
	    const OV_BOOL  value
) {
	if(value){
		fbcomlib_FBComCommon_doResetAbstract_set((OV_INSTPTR_fbcomlib_FBComCommon)pobj, value);
		fbcomlib_setUIntVec_senduintvec_set(pobj, 0, 1);
	}
	return OV_ERR_OK;
}

/**
 * Individual startup method for creating ksapi_setuintvec object for later communication
 */
OV_DLLFNCEXPORT void fbcomlib_setUIntVec_startup(
	OV_INSTPTR_ov_object 	pobj
) {
    //~ OV_INSTPTR_fbcomlib_setUIntVec pinst = Ov_StaticPtrCast(fbcomlib_setUIntVec, pobj);

    OV_INSTPTR_fbcomlib_setUIntVec				setuintvec = Ov_StaticPtrCast(fbcomlib_setUIntVec, pobj);
	OV_VTBLPTR_fbcomlib_setUIntVec					stable = NULL;

	OV_INSTPTR_ksapi_setUIntVec						ksapisetuintvec = NULL;
	OV_VTBLPTR_ksapi_setUIntVec						mtable = NULL;
	OV_RESULT res = 0;

    // do what the base class does first
    fb_functionblock_startup(pobj);
	Ov_GetVTablePtr(fbcomlib_setUIntVec, stable, setuintvec);

	ksapisetuintvec = (OV_INSTPTR_ksapi_setUIntVec)Ov_GetFirstChild(fbcomlib_FBComCommonAssoc, setuintvec);
	if(! ksapisetuintvec){
	  res = Ov_CreateObject(ksapi_setUIntVec, ksapisetuintvec, setuintvec, "setUIntVec");
	  if (Ov_Fail(res)){
		ov_logfile_error("Error while creating the setUIntVec");
		fbcomlib_FBComCommon_state_set((OV_INSTPTR_fbcomlib_FBComCommon)setuintvec, STATE_OVERROR);
		return;
	  }
	  Ov_Link(fbcomlib_FBComCommonAssoc, setuintvec, ksapisetuintvec);
	}

	Ov_GetVTablePtr(ksapi_setUIntVec, mtable, ksapisetuintvec);
	//@todo Better a doReset here???
	setuintvec->v_doSend = FALSE;

	//register the return method
	mtable->m_registerMethod((OV_INSTPTR_ksapi_KSCommon) ksapisetuintvec, stable->m_retMethod);
	fbcomlib_FBComCommon_state_set((OV_INSTPTR_fbcomlib_FBComCommon)setuintvec, STATE_OK);
    return;
}

/**
 * @todo Will be removed by martin since get* Commands could use a general FBComCommon/typemethod
 */
OV_DLLFNCEXPORT void fbcomlib_setUIntVec_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
    OV_INSTPTR_fbcomlib_FBComCommon 				fbcommon = Ov_StaticPtrCast(fbcomlib_FBComCommon, pfb);
    OV_INSTPTR_fbcomlib_setUIntVec 					setUIntVec = Ov_StaticPtrCast(fbcomlib_setUIntVec, pfb);
	OV_INSTPTR_ksapi_setUIntVec						ksapisetuintvec = NULL;
	OV_VTBLPTR_ksapi_setUIntVec						mtable = NULL;
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
		ksapisetuintvec = (OV_INSTPTR_ksapi_setUIntVec)Ov_GetFirstChild(fbcomlib_FBComCommonAssoc, fbcommon);
		if(! ksapisetuintvec) {
			ov_logfile_error("setUInt typemethod hasnt got a ksapi setuintvec obj");
			return;
		}
		//get variable pointer
		Ov_GetVTablePtr(ksapi_setUIntVec, mtable, ksapisetuintvec);
		//start sending
		mtable->m_setandsubmit(ksapisetuintvec,
				fbcommon->v_host,
				fbcommon->v_server,
				fbcommon->v_path,
				setUIntVec->v_senduintvec.value, setUIntVec->v_senduintvec.veclen);
	}
	return;


}

