/**
 * @file setString.c
 * FunctionBlock for setting a remote String.
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
 * The String, witch has to be transmitted
 * This Get shouldnt be used from outside!
 * Well - it could....
 */
OV_DLLFNCEXPORT OV_STRING fbcomlib_setString_sendstring_get(
    OV_INSTPTR_fbcomlib_setString          pobj
) {
    return pobj->v_sendstring;
}

/**
 * The String, witch has to be transmitted
 */
OV_DLLFNCEXPORT OV_RESULT fbcomlib_setString_sendstring_set(
    OV_INSTPTR_fbcomlib_setString          pobj,
    const OV_STRING  value
) {
	if(!(pobj->v_doSend)){
		return ov_string_setvalue(&pobj->v_sendstring,value);
	}
	else{
		return OV_ERR_GENERIC;
	}
}
/**
 * If TRUE, does the reset FBComCommon and removes sendstring additionally
 * @todo Could the GET be removed???
 */
OV_DLLFNCEXPORT OV_BOOL fbcomlib_setString_doReset_get(
	    OV_INSTPTR_fbcomlib_setString          pobj
) {
    return pobj->v_doResetAbstract;
}

/**
 * If TRUE, Does the reset FBComCommon and removes sendstring additionally
 */
OV_DLLFNCEXPORT OV_RESULT fbcomlib_setString_doReset_set(
	    OV_INSTPTR_fbcomlib_setString          pobj,
	    const OV_BOOL  value
) {
	if(value){
		fbcomlib_FBComCommon_doResetAbstract_set((OV_INSTPTR_fbcomlib_FBComCommon)pobj, value);
		fbcomlib_setString_sendstring_set(pobj, "");
	}
	return OV_ERR_OK;
}

/**
 * Individual startup method for creating ksapi_setstring object for later communication
 */
OV_DLLFNCEXPORT void fbcomlib_setString_startup(
	OV_INSTPTR_ov_object 	pobj
) {
    //~ OV_INSTPTR_fbcomlib_setString pinst = Ov_StaticPtrCast(fbcomlib_setString, pobj);

    OV_INSTPTR_fbcomlib_setString 					setstring = Ov_StaticPtrCast(fbcomlib_setString, pobj);
	OV_VTBLPTR_fbcomlib_setString					stable = NULL;

	OV_INSTPTR_ksapi_setString						ksapisetstring = NULL;
	OV_VTBLPTR_ksapi_setString						mtable = NULL;
	OV_RESULT res = 0;

    // do what the base class does first
    fb_functionblock_startup(pobj);
	Ov_GetVTablePtr(fbcomlib_setString, stable, setstring);

	ksapisetstring = (OV_INSTPTR_ksapi_setString)Ov_GetFirstChild(fbcomlib_FBComCommonAssoc, setstring);
	if(! ksapisetstring){
	  res = Ov_CreateObject(ksapi_setString, ksapisetstring, setstring, "setString");
	  if (Ov_Fail(res)){
		ov_logfile_error("Error while creating the setString");
		fbcomlib_FBComCommon_state_set((OV_INSTPTR_fbcomlib_FBComCommon)setstring, STATE_OVERROR);
		return;
	  }
	  Ov_Link(fbcomlib_FBComCommonAssoc, setstring, ksapisetstring);
	}

	Ov_GetVTablePtr(ksapi_setString, mtable, ksapisetstring);
	//@todo Better a doReset here???
	setstring->v_doSend = FALSE;

	//register the return method
	mtable->m_registerMethod((OV_INSTPTR_ksapi_KSCommon) ksapisetstring, stable->m_retMethod);
	fbcomlib_FBComCommon_state_set((OV_INSTPTR_fbcomlib_FBComCommon)setstring, STATE_OK);
    return;
}


/**
 * @todo Will be removed by martin since get* Commands could use a general FBComCommon/typemethod
 */
OV_DLLFNCEXPORT void fbcomlib_setString_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
    OV_INSTPTR_fbcomlib_FBComCommon 				fbcommon = Ov_StaticPtrCast(fbcomlib_FBComCommon, pfb);
    OV_INSTPTR_fbcomlib_setString 					setString = Ov_StaticPtrCast(fbcomlib_setString, pfb);
	OV_INSTPTR_ksapi_setString						ksapisetstring = NULL;
	OV_VTBLPTR_ksapi_setString						mtable = NULL;
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
		ksapisetstring = (OV_INSTPTR_ksapi_setString)Ov_GetFirstChild(fbcomlib_FBComCommonAssoc, fbcommon);
		if(! ksapisetstring) {
			ov_logfile_error("setString typemethod hasnt got a ksapi setstring obj");
			return;
		}
		//get variable pointer
		Ov_GetVTablePtr(ksapi_setString, mtable, ksapisetstring);
		//start sending
		mtable->m_setandsubmit(ksapisetstring,
				fbcommon->v_host,
				fbcommon->v_server,
				fbcommon->v_path,
				setString->v_sendstring);
	}
	return;


}

