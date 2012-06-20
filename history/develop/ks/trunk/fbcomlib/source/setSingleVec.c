/**
 * @file setSingleVec.c
 * FunctionBlock for setting a remote Single Vector.
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
 * The SingleVec, witch has to be transmitted
 * This Get shouldnt be used from outside!
 * Well - it could....
 */
OV_DLLFNCEXPORT OV_SINGLE * fbcomlib_setSingleVec_sendsinglevec_get(
	OV_INSTPTR_fbcomlib_setSingleVec	pobj,
	OV_UINT													*pveclen
) {
	*pveclen = pobj->v_sendsinglevec.veclen;
	return pobj->v_sendsinglevec.value;
}

/**
 * The SingleVec, witch has to be transmitted
*/
OV_DLLFNCEXPORT OV_RESULT fbcomlib_setSingleVec_sendsinglevec_set(
	OV_INSTPTR_fbcomlib_setSingleVec	pobj,
	const OV_SINGLE										*pvalue,
	const OV_UINT											veclen
) {
	if(!(pobj->v_doSend)){
		return Ov_SetDynamicVectorValue(&pobj->v_sendsinglevec,pvalue,veclen,SINGLE);
	}
	else{
		return OV_ERR_GENERIC;
	}	
}

/**
 * If TRUE, does the reset FBComCommon and removes sendsinglevec additionally
 * @todo Could the GET be removed???
 */
OV_DLLFNCEXPORT OV_BOOL fbcomlib_setSingleVec_doReset_get(
	    OV_INSTPTR_fbcomlib_setSingleVec          pobj
) {
    return pobj->v_doResetAbstract;
}

/**
 * If TRUE, Does the reset FBComCommon and removes sendsinglevec additionally
 */
OV_DLLFNCEXPORT OV_RESULT fbcomlib_setSingleVec_doReset_set(
	    OV_INSTPTR_fbcomlib_setSingleVec          pobj,
	    const OV_BOOL  value
) {
	if(value){
		fbcomlib_FBComCommon_doResetAbstract_set((OV_INSTPTR_fbcomlib_FBComCommon)pobj, value);
		fbcomlib_setSingleVec_sendsinglevec_set(pobj, 0, 1);
	}
	return OV_ERR_OK;
}

/**
 * Individual startup method for creating ksapi_setsinglevec object for later communication
 */
OV_DLLFNCEXPORT void fbcomlib_setSingleVec_startup(
	OV_INSTPTR_ov_object 	pobj
) {
    //~ OV_INSTPTR_fbcomlib_setSingleVec pinst = Ov_StaticPtrCast(fbcomlib_setSingleVec, pobj);

    OV_INSTPTR_fbcomlib_setSingleVec				setsinglevec = Ov_StaticPtrCast(fbcomlib_setSingleVec, pobj);
	OV_VTBLPTR_fbcomlib_setSingleVec					stable = NULL;

	OV_INSTPTR_ksapi_setSingleVec						ksapisetsinglevec = NULL;
	OV_VTBLPTR_ksapi_setSingleVec						mtable = NULL;
	OV_RESULT res = 0;

    // do what the base class does first
    fb_functionblock_startup(pobj);
	Ov_GetVTablePtr(fbcomlib_setSingleVec, stable, setsinglevec);

	ksapisetsinglevec = (OV_INSTPTR_ksapi_setSingleVec)Ov_GetFirstChild(fbcomlib_FBComCommonAssoc, setsinglevec);
	if(! ksapisetsinglevec){
	  res = Ov_CreateObject(ksapi_setSingleVec, ksapisetsinglevec, setsinglevec, "setSingleVec");
	  if (Ov_Fail(res)){
		ov_logfile_error("Error while creating the setSingleVec");
		fbcomlib_FBComCommon_state_set((OV_INSTPTR_fbcomlib_FBComCommon)setsinglevec, STATE_OVERROR);
		return;
	  }
	  Ov_Link(fbcomlib_FBComCommonAssoc, setsinglevec, ksapisetsinglevec);
	}

	Ov_GetVTablePtr(ksapi_setSingleVec, mtable, ksapisetsinglevec);
	//@todo Better a doReset here???
	setsinglevec->v_doSend = FALSE;

	//register the return method
	mtable->m_registerMethod((OV_INSTPTR_ksapi_KSCommon) ksapisetsinglevec, stable->m_retMethod);
	fbcomlib_FBComCommon_state_set((OV_INSTPTR_fbcomlib_FBComCommon)setsinglevec, STATE_OK);
    return;
}

/**
 * @todo Will be removed by martin since get* Commands could use a general FBComCommon/typemethod
 */
OV_DLLFNCEXPORT void fbcomlib_setSingleVec_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
    OV_INSTPTR_fbcomlib_FBComCommon 				fbcommon = Ov_StaticPtrCast(fbcomlib_FBComCommon, pfb);
    OV_INSTPTR_fbcomlib_setSingleVec 					setSingleVec = Ov_StaticPtrCast(fbcomlib_setSingleVec, pfb);
	OV_INSTPTR_ksapi_setSingleVec						ksapisetsinglevec = NULL;
	OV_VTBLPTR_ksapi_setSingleVec						mtable = NULL;
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
		ksapisetsinglevec = (OV_INSTPTR_ksapi_setSingleVec)Ov_GetFirstChild(fbcomlib_FBComCommonAssoc, fbcommon);
		if(! ksapisetsinglevec) {
			ov_logfile_error("setSingle typemethod hasnt got a ksapi setsinglevec obj");
			return;
		}
		//get variable pointer
		Ov_GetVTablePtr(ksapi_setSingleVec, mtable, ksapisetsinglevec);
		//start sending
		mtable->m_setandsubmit(ksapisetsinglevec,
				fbcommon->v_host,
				fbcommon->v_server,
				fbcommon->v_path,
				setSingleVec->v_sendsinglevec.value, setSingleVec->v_sendsinglevec.veclen);
	}
	return;


}

