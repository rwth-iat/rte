/**
 * @file setInt.c
 * FunctionBlock for setting a remote Integer.
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
 * The Integer, witch has to be transmitted
 * This Get shouldnt be used from outside!
 * Well - it could....
 */
OV_DLLFNCEXPORT OV_INT fbcomlib_setInt_sendint_get(
    OV_INSTPTR_fbcomlib_setInt          pobj
) {
    return pobj->v_sendint;
}

/**
 * The Integer, witch has to be transmitted
 */
OV_DLLFNCEXPORT OV_RESULT fbcomlib_setInt_sendint_set(
    OV_INSTPTR_fbcomlib_setInt          pobj,
    const OV_INT  value
) {
	if(!(pobj->v_doSend)){
		pobj->v_sendint = value;
		return OV_ERR_OK;
	}
	else{
		return OV_ERR_GENERIC;
	}
}
/**
 * If TRUE, does the reset FBComCommon and removes sendint additionally
 * @todo Could the GET be removed???
 */
OV_DLLFNCEXPORT OV_BOOL fbcomlib_setInt_doReset_get(
	    OV_INSTPTR_fbcomlib_setInt          pobj
) {
    return pobj->v_doResetAbstract;
}

/**
 * If TRUE, Does the reset FBComCommon and removes sendint additionally
 */
OV_DLLFNCEXPORT OV_RESULT fbcomlib_setInt_doReset_set(
	    OV_INSTPTR_fbcomlib_setInt          pobj,
	    const OV_BOOL  value
) {
	if(value){
		fbcomlib_FBComCommon_doResetAbstract_set((OV_INSTPTR_fbcomlib_FBComCommon)pobj, value);
		fbcomlib_setInt_sendint_set(pobj, 0);
	}
	return OV_ERR_OK;
}

/**
 * Individual startup method for creating ksapi_setint object for later communication
 */
OV_DLLFNCEXPORT void fbcomlib_setInt_startup(
	OV_INSTPTR_ov_object 	pobj
) {
    //~ OV_INSTPTR_fbcomlib_setInt pinst = Ov_StaticPtrCast(fbcomlib_setInt, pobj);

    OV_INSTPTR_fbcomlib_setInt 					setint = Ov_StaticPtrCast(fbcomlib_setInt, pobj);
	OV_VTBLPTR_fbcomlib_setInt					stable = NULL;

	OV_INSTPTR_ksapi_setInt						ksapisetint = NULL;
	OV_VTBLPTR_ksapi_setInt						mtable = NULL;
	OV_RESULT res = 0;

    // do what the base class does first
    fb_functionblock_startup(pobj);
	Ov_GetVTablePtr(fbcomlib_setInt, stable, setint);

	ksapisetint = (OV_INSTPTR_ksapi_setInt)Ov_GetFirstChild(fbcomlib_FBComCommonAssoc, setint);
	if(! ksapisetint){
	  res = Ov_CreateObject(ksapi_setInt, ksapisetint, setint, "setInt");
	  if (Ov_Fail(res)){
		ov_logfile_error("Error while creating the setInt");
		fbcomlib_FBComCommon_state_set((OV_INSTPTR_fbcomlib_FBComCommon)setint, STATE_OVERROR);
		return;
	  }
	  Ov_Link(fbcomlib_FBComCommonAssoc, setint, ksapisetint);
	}

	Ov_GetVTablePtr(ksapi_setInt, mtable, ksapisetint);
	//@todo Better a doReset here???
	setint->v_doSend = FALSE;

	//register the return method
	mtable->m_registerMethod((OV_INSTPTR_ksapi_KSCommon) ksapisetint, stable->m_retMethod);
	fbcomlib_FBComCommon_state_set((OV_INSTPTR_fbcomlib_FBComCommon)setint, STATE_OK);
    return;
}


/**
 * @todo Will be removed by martin since get* Commands could use a general FBComCommon/typemethod
 */
OV_DLLFNCEXPORT void fbcomlib_setInt_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
    OV_INSTPTR_fbcomlib_FBComCommon 				fbcommon = Ov_StaticPtrCast(fbcomlib_FBComCommon, pfb);
    OV_INSTPTR_fbcomlib_setInt 					setInt = Ov_StaticPtrCast(fbcomlib_setInt, pfb);
	OV_INSTPTR_ksapi_setInt						ksapisetint = NULL;
	OV_VTBLPTR_ksapi_setInt						mtable = NULL;
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
		ksapisetint = (OV_INSTPTR_ksapi_setInt)Ov_GetFirstChild(fbcomlib_FBComCommonAssoc, fbcommon);
		if(! ksapisetint) {
			ov_logfile_error("setInt typemethod hasnt got a ksapi setint obj");
			return;
		}
		//get variable pointer
		Ov_GetVTablePtr(ksapi_setInt, mtable, ksapisetint);

		//set time-stamp to current time
		ov_time_gettime(&ttemp);
		ksapi_setInt_varTimeStamp_set(ksapisetint, &ttemp);
		//set qaulity state to not-supported
		ksapi_setInt_varQState_set(ksapisetint, 0);
		//start sending
		mtable->m_setandsubmit(ksapisetint,
				fbcommon->v_host,
				fbcommon->v_server,
				fbcommon->v_path,
				setInt->v_sendint);
	}
	return;


}

