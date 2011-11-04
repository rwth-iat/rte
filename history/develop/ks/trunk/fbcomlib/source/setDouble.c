/**
 * @file setDouble.c
 * FunctionBlock for setting a remote Double.
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
 * The Double, witch has to be transmitted
 * This Get shouldnt be used from outside!
 * Well - it could....
 */
OV_DLLFNCEXPORT OV_DOUBLE fbcomlib_setDouble_senddouble_get(
    OV_INSTPTR_fbcomlib_setDouble          pobj
) {
    return pobj->v_senddouble;
}

/**
 * The Double, witch has to be transmitted
 */
OV_DLLFNCEXPORT OV_RESULT fbcomlib_setDouble_senddouble_set(
    OV_INSTPTR_fbcomlib_setDouble          pobj,
    const OV_DOUBLE  value
) {
	if(!(pobj->v_doSend)){
		pobj->v_senddouble = value;
		return OV_ERR_OK;
	}
	else{
		return OV_ERR_GENERIC;
	}
}
/**
 * If TRUE, does the reset FBComCommon and removes senddouble additionally
 * @todo Could the GET be removed???
 */
OV_DLLFNCEXPORT OV_BOOL fbcomlib_setDouble_doReset_get(
	    OV_INSTPTR_fbcomlib_setDouble          pobj
) {
    return pobj->v_doResetAbstract;
}

/**
 * If TRUE, Does the reset FBComCommon and removes senddouble additionally
 */
OV_DLLFNCEXPORT OV_RESULT fbcomlib_setDouble_doReset_set(
	    OV_INSTPTR_fbcomlib_setDouble          pobj,
	    const OV_BOOL  value
) {
	if(value){
		fbcomlib_FBComCommon_doResetAbstract_set((OV_INSTPTR_fbcomlib_FBComCommon)pobj, value);
		fbcomlib_setDouble_senddouble_set(pobj, 0.0);
	}
	return OV_ERR_OK;
}

/**
 * Individual startup method for creating ksapi_setdouble object for later communication
 */
OV_DLLFNCEXPORT void fbcomlib_setDouble_startup(
	OV_INSTPTR_ov_object 	pobj
) {
    //~ OV_INSTPTR_fbcomlib_setDouble pinst = Ov_StaticPtrCast(fbcomlib_setDouble, pobj);

    OV_INSTPTR_fbcomlib_setDouble 					setdouble = Ov_StaticPtrCast(fbcomlib_setDouble, pobj);
	OV_VTBLPTR_fbcomlib_setDouble					stable = NULL;

	OV_INSTPTR_ksapi_setDouble						ksapisetdouble = NULL;
	OV_VTBLPTR_ksapi_setDouble						mtable = NULL;
	OV_RESULT res = 0;

    // do what the base class does first
    fb_functionblock_startup(pobj);
	Ov_GetVTablePtr(fbcomlib_setDouble, stable, setdouble);

	ksapisetdouble = (OV_INSTPTR_ksapi_setDouble)Ov_GetFirstChild(fbcomlib_FBComCommonAssoc, setdouble);
	if(! ksapisetdouble){
	  res = Ov_CreateObject(ksapi_setDouble, ksapisetdouble, setdouble, "setDouble");
	  if (Ov_Fail(res)){
		ov_logfile_error("Error while creating the setDouble");
		fbcomlib_FBComCommon_state_set((OV_INSTPTR_fbcomlib_FBComCommon)setdouble, STATE_OVERROR);
		return;
	  }
	  Ov_Link(fbcomlib_FBComCommonAssoc, setdouble, ksapisetdouble);
	}

	Ov_GetVTablePtr(ksapi_setDouble, mtable, ksapisetdouble);
	//@todo Better a doReset here???
	setdouble->v_doSend = FALSE;

	//register the return method
	mtable->m_registerMethod((OV_INSTPTR_ksapi_KSCommon) ksapisetdouble, stable->m_retMethod);
	fbcomlib_FBComCommon_state_set((OV_INSTPTR_fbcomlib_FBComCommon)setdouble, STATE_OK);
    return;
}


/**
 * @todo Will be removed by martin since get* Commands could use a general FBComCommon/typemethod
 */
OV_DLLFNCEXPORT void fbcomlib_setDouble_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
    OV_INSTPTR_fbcomlib_FBComCommon 				fbcommon = Ov_StaticPtrCast(fbcomlib_FBComCommon, pfb);
    OV_INSTPTR_fbcomlib_setDouble 					setDouble = Ov_StaticPtrCast(fbcomlib_setDouble, pfb);
	OV_INSTPTR_ksapi_setDouble						ksapisetdouble = NULL;
	OV_VTBLPTR_ksapi_setDouble						mtable = NULL;
	//OV_INSTPTR_ksapi_KSCommon						kscommon = NULL;
	//OV_VTBLPTR_ksapi_KSCommon						mtable = NULL;

	//check if read was set to true
	if (fbcommon->v_doSend){
		fbcommon->v_doSend = FALSE;
		//check if all values are set
		if (!((fbcommon->v_server)&&(fbcommon->v_host)&&(fbcommon->v_path))){
			ov_logfile_error("Please set all values: server,host,path");
			fbcomlib_FBComCommon_state_set(fbcommon, STATE_WRONGINPUT);
			return;
		}
		ksapisetdouble = (OV_INSTPTR_ksapi_setDouble)Ov_GetFirstChild(fbcomlib_FBComCommonAssoc, fbcommon);
		if(! ksapisetdouble) {
			ov_logfile_error("setDouble typemethod hasnt got a ksapi setdouble obj");
			return;
		}
		//get variable pointer
		Ov_GetVTablePtr(ksapi_setDouble, mtable, ksapisetdouble);
		//start sending
		mtable->m_setandsubmit(ksapisetdouble,
				fbcommon->v_host,
				fbcommon->v_server,
				fbcommon->v_path,
				setDouble->v_senddouble);
	}
	return;


}

