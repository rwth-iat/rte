/**
 * @file setSingle.c
 * FunctionBlock for setting a remote Single.
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
 * The Single, witch has to be transmitted
 * This Get shouldnt be used from outside!
 * Well - it could....
 */
OV_DLLFNCEXPORT OV_SINGLE fbcomlib_setSingle_sendsingle_get(
    OV_INSTPTR_fbcomlib_setSingle          pobj
) {
    return pobj->v_sendsingle;
}

/**
 * The Single, witch has to be transmitted
 */
OV_DLLFNCEXPORT OV_RESULT fbcomlib_setSingle_sendsingle_set(
    OV_INSTPTR_fbcomlib_setSingle          pobj,
    const OV_SINGLE  value
) {
	if(!(pobj->v_doSend)){
		pobj->v_sendsingle = value;
		return OV_ERR_OK;
	}
	else{
		return OV_ERR_GENERIC;
	}
}
/**
 * If TRUE, does the reset FBComCommon and removes sendsingle additionally
 * @todo Could the GET be removed???
 */
OV_DLLFNCEXPORT OV_BOOL fbcomlib_setSingle_doReset_get(
	    OV_INSTPTR_fbcomlib_setSingle          pobj
) {
    return pobj->v_doResetAbstract;
}

/**
 * If TRUE, Does the reset FBComCommon and removes sendsingle additionally
 */
OV_DLLFNCEXPORT OV_RESULT fbcomlib_setSingle_doReset_set(
	    OV_INSTPTR_fbcomlib_setSingle          pobj,
	    const OV_BOOL  value
) {
	if(value){
		fbcomlib_FBComCommon_doResetAbstract_set((OV_INSTPTR_fbcomlib_FBComCommon)pobj, value);
		fbcomlib_setSingle_sendsingle_set(pobj, 0.0);
	}
	return OV_ERR_OK;
}

/**
 * Individual startup method for creating ksapi_setsingle object for later communication
 */
OV_DLLFNCEXPORT void fbcomlib_setSingle_startup(
	OV_INSTPTR_ov_object 	pobj
) {
    //~ OV_INSTPTR_fbcomlib_setSingle pinst = Ov_StaticPtrCast(fbcomlib_setSingle, pobj);

    OV_INSTPTR_fbcomlib_setSingle 					setsingle = Ov_StaticPtrCast(fbcomlib_setSingle, pobj);
	OV_VTBLPTR_fbcomlib_setSingle					stable = NULL;

	OV_INSTPTR_ksapi_setSingle						ksapisetsingle = NULL;
	OV_VTBLPTR_ksapi_setSingle						mtable = NULL;
	OV_RESULT res = 0;

    // do what the base class does first
    fb_functionblock_startup(pobj);
	Ov_GetVTablePtr(fbcomlib_setSingle, stable, setsingle);

	ksapisetsingle = (OV_INSTPTR_ksapi_setSingle)Ov_GetFirstChild(fbcomlib_FBComCommonAssoc, setsingle);
	if(! ksapisetsingle){
	  res = Ov_CreateObject(ksapi_setSingle, ksapisetsingle, setsingle, "setSingle");
	  if (Ov_Fail(res)){
		ov_logfile_error("Error while creating the setSingle");
		fbcomlib_FBComCommon_state_set((OV_INSTPTR_fbcomlib_FBComCommon)setsingle, STATE_OVERROR);
		return;
	  }
	  Ov_Link(fbcomlib_FBComCommonAssoc, setsingle, ksapisetsingle);
	}

	Ov_GetVTablePtr(ksapi_setSingle, mtable, ksapisetsingle);
	//@todo Better a doReset here???
	setsingle->v_doSend = FALSE;

	//register the return method
	mtable->m_registerMethod((OV_INSTPTR_ksapi_KSCommon) ksapisetsingle, stable->m_retMethod);
	fbcomlib_FBComCommon_state_set((OV_INSTPTR_fbcomlib_FBComCommon)setsingle, STATE_OK);
    return;
}


/**
 * @todo Will be removed by martin since get* Commands could use a general FBComCommon/typemethod
 */
OV_DLLFNCEXPORT void fbcomlib_setSingle_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
    OV_INSTPTR_fbcomlib_FBComCommon 				fbcommon = Ov_StaticPtrCast(fbcomlib_FBComCommon, pfb);
    OV_INSTPTR_fbcomlib_setSingle 					setSingle = Ov_StaticPtrCast(fbcomlib_setSingle, pfb);
	OV_INSTPTR_ksapi_setSingle						ksapisetsingle = NULL;
	OV_VTBLPTR_ksapi_setSingle						mtable = NULL;
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
		ksapisetsingle = (OV_INSTPTR_ksapi_setSingle)Ov_GetFirstChild(fbcomlib_FBComCommonAssoc, fbcommon);
		if(! ksapisetsingle) {
			ov_logfile_error("setSingle typemethod hasnt got a ksapi setsingle obj");
			return;
		}
		//get variable pointer
		Ov_GetVTablePtr(ksapi_setSingle, mtable, ksapisetsingle);

		//set time-stamp to current time
		ov_time_gettime(&ttemp);
		ksapi_setSingle_varTimeStamp_set(ksapisetsingle, &ttemp);
		//set qaulity state to not-supported
		ksapi_setSingle_varQState_set(ksapisetsingle, KS_ST_NOTSUPPORTED);
		//start sending
		mtable->m_setandsubmit(ksapisetsingle,
				fbcommon->v_host,
				fbcommon->v_server,
				fbcommon->v_path,
				setSingle->v_sendsingle);
	}
	return;


}

