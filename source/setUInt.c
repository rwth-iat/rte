/**
 * @file setUInt.c
 * FunctionBlock for setting a remote Unsigned Integer.
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
 * The Unsigned Integer, witch has to be transmitted
 * This Get shouldnt be used from outside!
 * Well - it could....
 */
OV_DLLFNCEXPORT OV_UINT fbcomlib_setUInt_senduint_get(
    OV_INSTPTR_fbcomlib_setUInt          pobj
) {
    return pobj->v_senduint;
}

/**
 * The Unsigned Integer, witch has to be transmitted
 */
OV_DLLFNCEXPORT OV_RESULT fbcomlib_setUInt_senduint_set(
    OV_INSTPTR_fbcomlib_setUInt          pobj,
    const OV_UINT  value
) {
	if(!(pobj->v_doSend)){
		pobj->v_senduint = value;
		return OV_ERR_OK;
	}
	else{
		return OV_ERR_GENERIC;
	}
}
/**
 * If TRUE, does the reset FBComCommon and removes senduint additionally
 * @todo Could the GET be removed???
 */
OV_DLLFNCEXPORT OV_BOOL fbcomlib_setUInt_doReset_get(
	    OV_INSTPTR_fbcomlib_setUInt          pobj
) {
    return pobj->v_doResetAbstract;
}

/**
 * If TRUE, Does the reset FBComCommon and removes senduint additionally
 */
OV_DLLFNCEXPORT OV_RESULT fbcomlib_setUInt_doReset_set(
	    OV_INSTPTR_fbcomlib_setUInt          pobj,
	    const OV_BOOL  value
) {
	if(value){
		fbcomlib_FBComCommon_doResetAbstract_set((OV_INSTPTR_fbcomlib_FBComCommon)pobj, value);
		fbcomlib_setUInt_senduint_set(pobj, 0);
	}
	return OV_ERR_OK;
}

/**
 * Individual startup method for creating ksapi_setuint object for later communication
 */
OV_DLLFNCEXPORT void fbcomlib_setUInt_startup(
	OV_INSTPTR_ov_object 	pobj
) {
    //~ OV_INSTPTR_fbcomlib_setUInt pinst = Ov_StaticPtrCast(fbcomlib_setUInt, pobj);

    OV_INSTPTR_fbcomlib_setUInt 					setuint = Ov_StaticPtrCast(fbcomlib_setUInt, pobj);
	OV_VTBLPTR_fbcomlib_setUInt					stable = NULL;

	OV_INSTPTR_ksapi_setUInt						ksapisetuint = NULL;
	OV_VTBLPTR_ksapi_setUInt						mtable = NULL;
	OV_RESULT res = 0;

    // do what the base class does first
    fb_functionblock_startup(pobj);
	Ov_GetVTablePtr(fbcomlib_setUInt, stable, setuint);

	ksapisetuint = (OV_INSTPTR_ksapi_setUInt)Ov_GetFirstChild(fbcomlib_FBComCommonAssoc, setuint);
	if(! ksapisetuint){
	  res = Ov_CreateObject(ksapi_setUInt, ksapisetuint, setuint, "setUInt");
	  if (Ov_Fail(res)){
		ov_logfile_error("Error while creating the setUInt");
		fbcomlib_FBComCommon_state_set((OV_INSTPTR_fbcomlib_FBComCommon)setuint, STATE_OVERROR);
		return;
	  }
	  Ov_Link(fbcomlib_FBComCommonAssoc, setuint, ksapisetuint);
	}

	Ov_GetVTablePtr(ksapi_setUInt, mtable, ksapisetuint);
	//@todo Better a doReset here???
	setuint->v_doSend = FALSE;

	//register the return method
	mtable->m_registerMethod((OV_INSTPTR_ksapi_KSCommon) ksapisetuint, stable->m_retMethod);
	fbcomlib_FBComCommon_state_set((OV_INSTPTR_fbcomlib_FBComCommon)setuint, STATE_OK);
    return;
}


/**
 * @todo Will be removed by martin since get* Commands could use a general FBComCommon/typemethod
 */
OV_DLLFNCEXPORT void fbcomlib_setUInt_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
    OV_INSTPTR_fbcomlib_FBComCommon 				fbcommon = Ov_StaticPtrCast(fbcomlib_FBComCommon, pfb);
    OV_INSTPTR_fbcomlib_setUInt 					setUInt = Ov_StaticPtrCast(fbcomlib_setUInt, pfb);
	OV_INSTPTR_ksapi_setUInt						ksapisetuint = NULL;
	OV_VTBLPTR_ksapi_setUInt						mtable = NULL;
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
		ksapisetuint = (OV_INSTPTR_ksapi_setUInt)Ov_GetFirstChild(fbcomlib_FBComCommonAssoc, fbcommon);
		if(! ksapisetuint) {
			ov_logfile_error("setUInt typemethod hasnt got a ksapi setuint obj");
			return;
		}
		//get variable pointer
		Ov_GetVTablePtr(ksapi_setUInt, mtable, ksapisetuint);

		//set time-stamp to current time
		ov_time_gettime(&ttemp);
		ksapi_setUInt_varTimeStamp_set(ksapisetuint, &ttemp);
		//set qaulity state to not-supported
		ksapi_setUInt_varQState_set(ksapisetuint, KS_ST_NOTSUPPORTED);
		//start sending
		mtable->m_setandsubmit(ksapisetuint,
				fbcommon->v_host,
				fbcommon->v_server,
				fbcommon->v_path,
				setUInt->v_senduint);
	}
	return;


}

