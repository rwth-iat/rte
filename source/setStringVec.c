/**
 * @file setStringVec.c
 * FunctionBlock for setting a remote String Vector.
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
 * The StringVec, witch has to be transmitted
 * This Get shouldnt be used from outside!
 * Well - it could....
 */
OV_DLLFNCEXPORT OV_STRING * fbcomlib_setStringVec_sendstringvec_get(
	OV_INSTPTR_fbcomlib_setStringVec	pobj,
	OV_UINT													*pveclen
) {
	*pveclen = pobj->v_sendstringvec.veclen;
	return pobj->v_sendstringvec.value;
}

/**
 * The StringVec, witch has to be transmitted
*/
OV_DLLFNCEXPORT OV_RESULT fbcomlib_setStringVec_sendstringvec_set(
	OV_INSTPTR_fbcomlib_setStringVec	pobj,
	const OV_STRING										*pvalue,
	const OV_UINT											veclen
) {
	if(!(pobj->v_doSend)){
		return Ov_SetDynamicVectorValue(&pobj->v_sendstringvec,pvalue,veclen,STRING);
	}
	else{
		return OV_ERR_GENERIC;
	}	
}

/**
 * If TRUE, does the reset FBComCommon and removes sendstringvec additionally
 * @todo Could the GET be removed???
 */
OV_DLLFNCEXPORT OV_BOOL fbcomlib_setStringVec_doReset_get(
	    OV_INSTPTR_fbcomlib_setStringVec          pobj
) {
    return pobj->v_doResetAbstract;
}

/**
 * If TRUE, Does the reset FBComCommon and removes sendstringvec additionally
 */
OV_DLLFNCEXPORT OV_RESULT fbcomlib_setStringVec_doReset_set(
	    OV_INSTPTR_fbcomlib_setStringVec          pobj,
	    const OV_BOOL  value
) {
	if(value){
		fbcomlib_FBComCommon_doResetAbstract_set((OV_INSTPTR_fbcomlib_FBComCommon)pobj, value);
		fbcomlib_setStringVec_sendstringvec_set(pobj, (char* const*)"", 1);
	}
	return OV_ERR_OK;
}

/**
 * Individual startup method for creating ksapi_setstringvec object for later communication
 */
OV_DLLFNCEXPORT void fbcomlib_setStringVec_startup(
	OV_INSTPTR_ov_object 	pobj
) {
    //~ OV_INSTPTR_fbcomlib_setStringVec pinst = Ov_StaticPtrCast(fbcomlib_setStringVec, pobj);

    OV_INSTPTR_fbcomlib_setStringVec				setstringvec = Ov_StaticPtrCast(fbcomlib_setStringVec, pobj);
	OV_VTBLPTR_fbcomlib_setStringVec					stable = NULL;

	OV_INSTPTR_ksapi_setStringVec						ksapisetstringvec = NULL;
	OV_VTBLPTR_ksapi_setStringVec						mtable = NULL;
	OV_RESULT res = 0;

    // do what the base class does first
    fb_functionblock_startup(pobj);
	Ov_GetVTablePtr(fbcomlib_setStringVec, stable, setstringvec);

	ksapisetstringvec = (OV_INSTPTR_ksapi_setStringVec)Ov_GetFirstChild(fbcomlib_FBComCommonAssoc, setstringvec);
	if(! ksapisetstringvec){
	  res = Ov_CreateObject(ksapi_setStringVec, ksapisetstringvec, setstringvec, "setStringVec");
	  if (Ov_Fail(res)){
		ov_logfile_error("Error while creating the setStringVec");
		fbcomlib_FBComCommon_state_set((OV_INSTPTR_fbcomlib_FBComCommon)setstringvec, STATE_OVERROR);
		return;
	  }
	  Ov_Link(fbcomlib_FBComCommonAssoc, setstringvec, ksapisetstringvec);
	}

	Ov_GetVTablePtr(ksapi_setStringVec, mtable, ksapisetstringvec);
	//@todo Better a doReset here???
	setstringvec->v_doSend = FALSE;

	//register the return method
	mtable->m_registerMethod((OV_INSTPTR_ksapi_KSCommon) ksapisetstringvec, stable->m_retMethod);
	fbcomlib_FBComCommon_state_set((OV_INSTPTR_fbcomlib_FBComCommon)setstringvec, STATE_OK);
    return;
}

/**
 * @todo Will be removed by martin since get* Commands could use a general FBComCommon/typemethod
 */
OV_DLLFNCEXPORT void fbcomlib_setStringVec_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
    OV_INSTPTR_fbcomlib_FBComCommon 				fbcommon = Ov_StaticPtrCast(fbcomlib_FBComCommon, pfb);
    OV_INSTPTR_fbcomlib_setStringVec 					setStringVec = Ov_StaticPtrCast(fbcomlib_setStringVec, pfb);
	OV_INSTPTR_ksapi_setStringVec						ksapisetstringvec = NULL;
	OV_VTBLPTR_ksapi_setStringVec						mtable = NULL;
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
		ksapisetstringvec = (OV_INSTPTR_ksapi_setStringVec)Ov_GetFirstChild(fbcomlib_FBComCommonAssoc, fbcommon);
		if(! ksapisetstringvec) {
			ov_logfile_error("setString typemethod hasnt got a ksapi setstringvec obj");
			return;
		}
		//get variable pointer
		Ov_GetVTablePtr(ksapi_setStringVec, mtable, ksapisetstringvec);
		//start sending
		mtable->m_setandsubmit(ksapisetstringvec,
				fbcommon->v_host,
				fbcommon->v_server,
				fbcommon->v_path,
				setStringVec->v_sendstringvec.value, setStringVec->v_sendstringvec.veclen);
	}
	return;


}

