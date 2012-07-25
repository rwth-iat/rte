/**
 * @file setANY.c
 * FunctionBlock for setting a remote ANY.
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
 * The ANY, witch has to be transmitted
 * This Get shouldnt be used from outside!
 * Well - it could....
 */
OV_DLLFNCEXPORT OV_ANY* fbcomlib_setAny_sendany_get(
    OV_INSTPTR_fbcomlib_setAny          pobj
) {
    return &(pobj->v_sendany);
}

/**
 * The Any, witch has to be transmitted
 */
OV_DLLFNCEXPORT OV_RESULT fbcomlib_setAny_sendany_set(
    OV_INSTPTR_fbcomlib_setAny          pobj,
    const OV_ANY*  value
) {
	if(!(pobj->v_doSend)){
		return Ov_SetAnyValue(&(pobj->v_sendany), value);
	}
	else{
		return OV_ERR_GENERIC;
	}
}
/**
 * If TRUE, does the reset FBComCommon and removes sendany additionally
 * @todo Could the GET be removed???
 */
OV_DLLFNCEXPORT OV_BOOL fbcomlib_setAny_doReset_get(
	    OV_INSTPTR_fbcomlib_setAny          pobj
) {
    return pobj->v_doResetAbstract;
}

/**
 * If TRUE, Does the reset FBComCommon and removes sendany additionally
 */
OV_DLLFNCEXPORT OV_RESULT fbcomlib_setAny_doReset_set(
	    OV_INSTPTR_fbcomlib_setAny          pobj,
	    const OV_BOOL  value
) {
	if(value){
		fbcomlib_FBComCommon_doResetAbstract_set((OV_INSTPTR_fbcomlib_FBComCommon)pobj, value);
		fbcomlib_setAny_sendany_set(pobj, 0);
	}
	return OV_ERR_OK;
}

/**
 * Individual startup method for creating ksapi_setany object for later communication
 */
OV_DLLFNCEXPORT void fbcomlib_setAny_startup(
	OV_INSTPTR_ov_object 	pobj
) {
    //~ OV_INSTPTR_fbcomlib_setAny pinst = Ov_StaticPtrCast(fbcomlib_setAny, pobj);
	OV_INSTPTR_fbcomlib_setAny 					setany = Ov_StaticPtrCast(fbcomlib_setAny, pobj);
	OV_VTBLPTR_fbcomlib_setAny					stable = NULL;

	OV_INSTPTR_ksapi_setAny						ksapisetany = NULL;
	OV_VTBLPTR_ksapi_setAny						mtable = NULL;
	OV_RESULT res = 0;
	
    /* do what the base class does first */
	fb_functionblock_startup(pobj);

	Ov_GetVTablePtr(fbcomlib_setAny, stable, setany);

	ksapisetany = (OV_INSTPTR_ksapi_setAny)Ov_GetFirstChild(fbcomlib_FBComCommonAssoc, setany);
	if(! ksapisetany){
	  res = Ov_CreateObject(ksapi_setAny, ksapisetany, setany, "setAny");
	  if (Ov_Fail(res)){
		ov_logfile_error("Error while creating the setAny");
		fbcomlib_FBComCommon_state_set((OV_INSTPTR_fbcomlib_FBComCommon)setany, STATE_OVERROR);
		return;
	  }
	  Ov_Link(fbcomlib_FBComCommonAssoc, setany, ksapisetany);
	}

	Ov_GetVTablePtr(ksapi_setAny, mtable, ksapisetany);
	//@todo Better a doReset here???
	setany->v_doSend = FALSE;

	//register the return method
	mtable->m_registerMethod((OV_INSTPTR_ksapi_KSCommon) ksapisetany, stable->m_retMethod);
	fbcomlib_FBComCommon_state_set((OV_INSTPTR_fbcomlib_FBComCommon)setany, STATE_OK);
    return;
}


/**
 * @todo Will be removed by martin since get* Commands could use a general FBComCommon/typemethod
 */
OV_DLLFNCEXPORT void fbcomlib_setAny_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
    OV_INSTPTR_fbcomlib_FBComCommon 				fbcommon = Ov_StaticPtrCast(fbcomlib_FBComCommon, pfb);
    OV_INSTPTR_fbcomlib_setAny 					setAny = Ov_StaticPtrCast(fbcomlib_setAny, pfb);
	OV_INSTPTR_ksapi_setAny						ksapisetany = NULL;
	OV_VTBLPTR_ksapi_setAny						mtable = NULL;
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
		ksapisetany = (OV_INSTPTR_ksapi_setAny)Ov_GetFirstChild(fbcomlib_FBComCommonAssoc, fbcommon);
		if(! ksapisetany) {
			ov_logfile_error("setAny typemethod hasnt got a ksapi setany obj");
			return;
		}
		//get variable pointer
		Ov_GetVTablePtr(ksapi_setAny, mtable, ksapisetany);
		//set time-stamp to current time, will be overwritten inside ksapi-object, if ANY has a timestamp encoded
		ov_time_gettime(&ttemp);
		ksapi_setAny_varTimeStamp_set(ksapisetany, &ttemp);
		//set qaulity state to not-supported, will be overwritten inside ksapi-object, if ANY has a qstate encoded
		ksapi_setAny_varQState_set(ksapisetany, 0);
		//start sending
		mtable->m_setandsubmit(ksapisetany,
				fbcommon->v_host,
				fbcommon->v_server,
				fbcommon->v_path,
				setAny->v_sendany);
	}
	return;


}

