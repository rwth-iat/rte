/**
 * @file setBool.c
 * FunctionBlock for setting a remote Bool.
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
 * The Bool, witch has to be transmitted
 * This Get shouldnt be used from outside!
 * Well - it could....
 */
OV_DLLFNCEXPORT OV_BOOL fbcomlib_setBool_sendbool_get(
    OV_INSTPTR_fbcomlib_setBool          pobj
) {
    return pobj->v_sendbool;
}

/**
 * The Bool, witch has to be transmitted
 */
OV_DLLFNCEXPORT OV_RESULT fbcomlib_setBool_sendbool_set(
    OV_INSTPTR_fbcomlib_setBool          pobj,
    const OV_BOOL  value
) {
	if(!(pobj->v_doSend)){
		pobj->v_sendbool = value;
		return OV_ERR_OK;
	}
	else{
		return OV_ERR_GENERIC;
	}
}
/**
 * If TRUE, does the reset FBComCommon and removes sendbool additionally
 * @todo Could the GET be removed???
 */
OV_DLLFNCEXPORT OV_BOOL fbcomlib_setBool_doReset_get(
	    OV_INSTPTR_fbcomlib_setBool          pobj
) {
    return pobj->v_doResetAbstract;
}

/**
 * If TRUE, Does the reset FBComCommon and removes sendbool additionally
 */
OV_DLLFNCEXPORT OV_RESULT fbcomlib_setBool_doReset_set(
	    OV_INSTPTR_fbcomlib_setBool          pobj,
	    const OV_BOOL  value
) {
	if(value){
		fbcomlib_FBComCommon_doResetAbstract_set((OV_INSTPTR_fbcomlib_FBComCommon)pobj, value);
		fbcomlib_setBool_sendbool_set(pobj, 0);
	}
	return OV_ERR_OK;
}

/**
 * Individual startup method for creating ksapi_setbool object for later communication
 */
OV_DLLFNCEXPORT void fbcomlib_setBool_startup(
	OV_INSTPTR_ov_object 	pobj
) {
    //~ OV_INSTPTR_fbcomlib_setBool pinst = Ov_StaticPtrCast(fbcomlib_setBool, pobj);
	OV_INSTPTR_fbcomlib_setBool 					setbool = Ov_StaticPtrCast(fbcomlib_setBool, pobj);
	OV_VTBLPTR_fbcomlib_setBool					stable = NULL;

	OV_INSTPTR_ksapi_setBool						ksapisetbool = NULL;
	OV_VTBLPTR_ksapi_setBool						mtable = NULL;
	OV_RESULT res = 0;
	
    /* do what the base class does first */
	fb_functionblock_startup(pobj);

	Ov_GetVTablePtr(fbcomlib_setBool, stable, setbool);

	ksapisetbool = (OV_INSTPTR_ksapi_setBool)Ov_GetFirstChild(fbcomlib_FBComCommonAssoc, setbool);
	if(! ksapisetbool){
	  res = Ov_CreateObject(ksapi_setBool, ksapisetbool, setbool, "setBool");
	  if (Ov_Fail(res)){
		ov_logfile_error("Error while creating the setBool");
		fbcomlib_FBComCommon_state_set((OV_INSTPTR_fbcomlib_FBComCommon)setbool, STATE_OVERROR);
		return;
	  }
	  Ov_Link(fbcomlib_FBComCommonAssoc, setbool, ksapisetbool);
	}

	Ov_GetVTablePtr(ksapi_setBool, mtable, ksapisetbool);
	//@todo Better a doReset here???
	setbool->v_doSend = FALSE;

	//register the return method
	mtable->m_registerMethod((OV_INSTPTR_ksapi_KSCommon) ksapisetbool, stable->m_retMethod);
	fbcomlib_FBComCommon_state_set((OV_INSTPTR_fbcomlib_FBComCommon)setbool, STATE_OK);
    return;
}


/**
 * @todo Will be removed by martin since get* Commands could use a general FBComCommon/typemethod
 */
OV_DLLFNCEXPORT void fbcomlib_setBool_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
    OV_INSTPTR_fbcomlib_FBComCommon 				fbcommon = Ov_StaticPtrCast(fbcomlib_FBComCommon, pfb);
    OV_INSTPTR_fbcomlib_setBool 					setBool = Ov_StaticPtrCast(fbcomlib_setBool, pfb);
	OV_INSTPTR_ksapi_setBool						ksapisetbool = NULL;
	OV_VTBLPTR_ksapi_setBool						mtable = NULL;
	//OV_INSTPTR_ksapi_KSCommon						kscommon = NULL;
	//OV_VTBLPTR_ksapi_KSCommon						mtable = NULL;

	//check if something should be sent and if we are in a final state
	if ((fbcommon->v_doSend || fbcommon->v_CyclicReactivation)
			&& fbcommon->v_state != STATE_INIT
			&& fbcommon->v_state != STATE_CLIENTBUSY){
		fbcommon->v_doSend = TRUE;

		//check if all values are set
		if (!((fbcommon->v_server)&&(fbcommon->v_host)&&(fbcommon->v_path))){
			ov_logfile_error("Please set all values: server,host,path");
			fbcomlib_FBComCommon_state_set(fbcommon, STATE_WRONGINPUT);
			return;
		}
		ksapisetbool = (OV_INSTPTR_ksapi_setBool)Ov_GetFirstChild(fbcomlib_FBComCommonAssoc, fbcommon);
		if(! ksapisetbool) {
			ov_logfile_error("setBool typemethod hasnt got a ksapi setbool obj");
			return;
		}
		//get variable pointer
		Ov_GetVTablePtr(ksapi_setBool, mtable, ksapisetbool);
		//start sending
		mtable->m_setandsubmit(ksapisetbool,
				fbcommon->v_host,
				fbcommon->v_server,
				fbcommon->v_path,
				setBool->v_sendbool);
	}
	return;


}

