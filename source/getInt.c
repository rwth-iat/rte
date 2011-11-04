/**
 * @file getInt.c
 * FunctionBlock for retrieving a remote Integer.
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
 * The received Integer is stored here
 */
OV_DLLFNCEXPORT OV_INT fbcomlib_getInt_receivedInt_get(
    OV_INSTPTR_fbcomlib_getInt          pobj
) {
    return pobj->v_receivedInt;
}

/**
 * The received Integer is stored here.
 * This Set shouldnt be used from outside!
 */
OV_DLLFNCEXPORT OV_RESULT fbcomlib_getInt_receivedInt_set(
    OV_INSTPTR_fbcomlib_getInt          pobj,
    const OV_INT  value
) {
	if(!(pobj->v_doSend)){
		pobj->v_receivedInt = value;
		return OV_ERR_OK;
	} else{
		return OV_ERR_GENERIC;
	}

}

/**
 * If TRUE, does the reset FBComCommon and removed receivedInt additionally
 * @todo Could the GET be removed???
 */
OV_DLLFNCEXPORT OV_BOOL fbcomlib_getInt_doReset_get(
	    OV_INSTPTR_fbcomlib_getInt          pobj
) {
    return pobj->v_doResetAbstract;
}

/**
 * If TRUE, does the reset FBComCommon and removed receivedInt additionally
 */
OV_DLLFNCEXPORT OV_RESULT fbcomlib_getInt_doReset_set(
	    OV_INSTPTR_fbcomlib_getInt          pobj,
	    const OV_BOOL  value
) {
	if(value){
		fbcomlib_FBComCommon_doResetAbstract_set((OV_INSTPTR_fbcomlib_FBComCommon)pobj, value);
		fbcomlib_getInt_receivedInt_set(pobj, 0);
	}
	return OV_ERR_OK;
}

/**
 * Individual startup method for creating ksapi_getint object for later communication
 */
OV_DLLFNCEXPORT void fbcomlib_getInt_startup(
	OV_INSTPTR_ov_object 	pobj
) {
    OV_INSTPTR_fbcomlib_getInt 					getint = Ov_StaticPtrCast(fbcomlib_getInt, pobj);
	OV_VTBLPTR_fbcomlib_getInt					stable = NULL;

	OV_INSTPTR_ksapi_getInt						ksapigetint = NULL;
	OV_VTBLPTR_ksapi_getInt						mtable = NULL;
	OV_RESULT res = 0;

    // do what the base class does first
    fb_functionblock_startup(pobj);

	Ov_GetVTablePtr(fbcomlib_getInt, stable, getint);

	ksapigetint = (OV_INSTPTR_ksapi_getInt)Ov_GetFirstChild(fbcomlib_FBComCommonAssoc, getint);
	if(! ksapigetint){
	  res = Ov_CreateObject(ksapi_getInt, ksapigetint, getint, "getInt");
	  if (Ov_Fail(res)){
		ov_logfile_error("Error while creating the getInt");
		fbcomlib_FBComCommon_state_set((OV_INSTPTR_fbcomlib_FBComCommon)getint, STATE_OVERROR);
		return;
	  }
	  Ov_Link(fbcomlib_FBComCommonAssoc, getint, ksapigetint);
	}
	Ov_GetVTablePtr(ksapi_getInt, mtable, ksapigetint);
	getint->v_doSend = FALSE;

	//(re)register the return method
	mtable->m_registerMethod((OV_INSTPTR_ksapi_KSCommon) ksapigetint, stable->m_retMethod);
	fbcomlib_FBComCommon_state_set((OV_INSTPTR_fbcomlib_FBComCommon)getint, STATE_OK);
	return;

}

/**
 * Evaluates the answer of ksapi/getInt.
 * Uses the FBComCommon/retMethod for status code and string
 * Additionally sets the receivedInt.
 */
OV_DLLFNCEXPORT void fbcomlib_getInt_retMethod(
		OV_INSTPTR_ov_object 		pobj,
		OV_STRING 						errorstring,
		OV_INT	 						errorcode
) {
	OV_INSTPTR_ksapi_getInt ksapigetint = Ov_StaticPtrCast(ksapi_getInt, pobj);
	OV_INSTPTR_fbcomlib_getInt getint = (OV_INSTPTR_fbcomlib_getInt)Ov_GetParent(fbcomlib_FBComCommonAssoc, ksapigetint);

	fbcomlib_FBComCommon_retMethod(pobj, errorstring, errorcode);
	//get the int
	fbcomlib_getInt_receivedInt_set(getint, ksapigetint->v_receiveint);

}

