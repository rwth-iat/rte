/**
 * @file getBool.c
 * FunctionBlock for retrieving a remote Bool.
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
 * The received Bool is stored here
 */
OV_DLLFNCEXPORT OV_BOOL fbcomlib_getBool_receivedBool_get(
    OV_INSTPTR_fbcomlib_getBool          pobj
) {
    return pobj->v_receivedBool;
}

/**
 * The received Bool is stored here.
 * This Set shouldnt be used from outside!
 */
OV_DLLFNCEXPORT OV_RESULT fbcomlib_getBool_receivedBool_set(
    OV_INSTPTR_fbcomlib_getBool          pobj,
    const OV_BOOL  value
) {
		pobj->v_receivedBool = value;
		return OV_ERR_OK;
}

/**
 * If TRUE, does the reset FBComCommon and removed receivedBool additionally
 * @todo Could the GET be removed???
 */
OV_DLLFNCEXPORT OV_BOOL fbcomlib_getBool_doReset_get(
	    OV_INSTPTR_fbcomlib_getBool          pobj
) {
    return pobj->v_doResetAbstract;
}

/**
 * If TRUE, does the reset FBComCommon and removed receivedBool additionally
 */
OV_DLLFNCEXPORT OV_RESULT fbcomlib_getBool_doReset_set(
	    OV_INSTPTR_fbcomlib_getBool          pobj,
	    const OV_BOOL  value
) {
	if(value){
		fbcomlib_FBComCommon_doResetAbstract_set((OV_INSTPTR_fbcomlib_FBComCommon)pobj, value);
		fbcomlib_getBool_receivedBool_set(pobj, 0);
	}
	return OV_ERR_OK;
}

/**
 * Individual startup method for creating ksapi_getBool object for later communication
 */
OV_DLLFNCEXPORT void fbcomlib_getBool_startup(
	OV_INSTPTR_ov_object 	pobj
) {
    OV_INSTPTR_fbcomlib_getBool 					getBool = Ov_StaticPtrCast(fbcomlib_getBool, pobj);
	OV_VTBLPTR_fbcomlib_getBool					stable = NULL;

	OV_INSTPTR_ksapi_getBool						ksapigetBool = NULL;
	OV_VTBLPTR_ksapi_getBool						mtable = NULL;
	OV_RESULT res = 0;

    // do what the base class does first
    fb_functionblock_startup(pobj);

	Ov_GetVTablePtr(fbcomlib_getBool, stable, getBool);

	ksapigetBool = (OV_INSTPTR_ksapi_getBool)Ov_GetFirstChild(fbcomlib_FBComCommonAssoc, getBool);
	if(! ksapigetBool){
	  res = Ov_CreateObject(ksapi_getBool, ksapigetBool, getBool, "getBool");
	  if (Ov_Fail(res)){
		ov_logfile_error("Error while creating the getBool");
		fbcomlib_FBComCommon_state_set((OV_INSTPTR_fbcomlib_FBComCommon)getBool, STATE_OVERROR);
		return;
	  }
	  Ov_Link(fbcomlib_FBComCommonAssoc, getBool, ksapigetBool);
	}
	Ov_GetVTablePtr(ksapi_getBool, mtable, ksapigetBool);
	getBool->v_doSend = FALSE;

	//(re)register the return method
	mtable->m_registerMethod((OV_INSTPTR_ksapi_KSCommon) ksapigetBool, stable->m_retMethod);
	fbcomlib_FBComCommon_state_set((OV_INSTPTR_fbcomlib_FBComCommon)getBool, STATE_OK);
	return;

}

/**
 * Evaluates the answer of ksapi/getBool.
 * Uses the FBComCommon/retMethod for status code and string
 * Additionally sets the receivedBool.
 */
OV_DLLFNCEXPORT void fbcomlib_getBool_retMethod(
		OV_INSTPTR_ov_object 		pobj,
		OV_STRING 						errorstring,
		OV_INT	 						errorcode
) {
	OV_INSTPTR_ksapi_getBool ksapigetBool = Ov_StaticPtrCast(ksapi_getBool, pobj);
	OV_INSTPTR_fbcomlib_getBool getBool = (OV_INSTPTR_fbcomlib_getBool)Ov_GetParent(fbcomlib_FBComCommonAssoc, ksapigetBool);

	fbcomlib_FBComCommon_retMethod(pobj, errorstring, errorcode);
	//get the bool
	fbcomlib_getBool_receivedBool_set(getBool, ksapigetBool->v_receivebool);

}

