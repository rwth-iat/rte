/**
 * @file getUInt.c TEST2
 * FunctionBlock for retrieving a remote UInt.
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
 * The received Uint is stored here
 */
OV_DLLFNCEXPORT OV_UINT fbcomlib_getUInt_receivedUInt_get(
    OV_INSTPTR_fbcomlib_getUInt          pobj
) {
    return pobj->v_receivedUInt;
}

/**
 * The received UInt is stored here.
 * This Set shouldnt be used from outside!
 */
OV_DLLFNCEXPORT OV_RESULT fbcomlib_getUInt_receivedUInt_set(
    OV_INSTPTR_fbcomlib_getUInt          pobj,
    const OV_UINT  value
) {
	if(!(pobj->v_doSend)){
		pobj->v_receivedUInt = value;
		return OV_ERR_OK;
	} else{
		return OV_ERR_GENERIC;
	}

}

/**
 * If TRUE, does the reset FBComCommon and removed receivedUInt additionally
 * @todo Could the GET be removed???
 */
OV_DLLFNCEXPORT OV_BOOL fbcomlib_getUInt_doReset_get(
	    OV_INSTPTR_fbcomlib_getUInt          pobj
) {
    return pobj->v_doResetAbstract;
}

/**
 * If TRUE, does the reset FBComCommon and removed receivedUInt additionally
 */
OV_DLLFNCEXPORT OV_RESULT fbcomlib_getUInt_doReset_set(
	    OV_INSTPTR_fbcomlib_getUInt          pobj,
	    const OV_BOOL  value
) {
	if(value){
		fbcomlib_FBComCommon_doResetAbstract_set((OV_INSTPTR_fbcomlib_FBComCommon)pobj, value);
		fbcomlib_getUInt_receivedUInt_set(pobj, 0);
	}
	return OV_ERR_OK;
}

/**
 * Individual startup method for creating ksapi_getuint object for later communication
 */
OV_DLLFNCEXPORT void fbcomlib_getUInt_startup(
	OV_INSTPTR_ov_object 	pobj
) {
    OV_INSTPTR_fbcomlib_getUInt 					getuint = Ov_StaticPtrCast(fbcomlib_getUInt, pobj);
	OV_VTBLPTR_fbcomlib_getUInt					stable = NULL;

	OV_INSTPTR_ksapi_getUInt						ksapigetuint = NULL;
	OV_VTBLPTR_ksapi_getUInt						mtable = NULL;
	OV_RESULT res = 0;

    // do what the base class does first
    fb_functionblock_startup(pobj);

	Ov_GetVTablePtr(fbcomlib_getUInt, stable, getuint);

	ksapigetuint = (OV_INSTPTR_ksapi_getUInt)Ov_GetFirstChild(fbcomlib_FBComCommonAssoc, getuint);
	if(! ksapigetuint){
	  res = Ov_CreateObject(ksapi_getUInt, ksapigetuint, getuint, "getUInt");
	  if (Ov_Fail(res)){
		ov_logfile_error("Error while creating the getUInt");
		fbcomlib_FBComCommon_state_set((OV_INSTPTR_fbcomlib_FBComCommon)getuint, STATE_OVERROR);
		return;
	  }
	  Ov_Link(fbcomlib_FBComCommonAssoc, getuint, ksapigetuint);
	}
	Ov_GetVTablePtr(ksapi_getUInt, mtable, ksapigetuint);
	getuint->v_doSend = FALSE;

	//(re)register the return method
	mtable->m_registerMethod((OV_INSTPTR_ksapi_KSCommon) ksapigetuint, stable->m_retMethod);
	fbcomlib_FBComCommon_state_set((OV_INSTPTR_fbcomlib_FBComCommon)getuint, STATE_OK);
	return;

}

/**
 * Evaluates the answer of ksapi/getUInt.
 * Uses the FBComCommon/retMethod for status code and string
 * Additionally sets the receivedUInt.
 */
OV_DLLFNCEXPORT void fbcomlib_getUInt_retMethod(
		OV_INSTPTR_ov_object 		pobj,
		OV_STRING 						errorstring,
		OV_INT	 						errorcode
) {
	OV_INSTPTR_ksapi_getUInt ksapigetuint = Ov_StaticPtrCast(ksapi_getUInt, pobj);
	OV_INSTPTR_fbcomlib_getUInt getuint = (OV_INSTPTR_fbcomlib_getUInt)Ov_GetParent(fbcomlib_FBComCommonAssoc, ksapigetuint);

	fbcomlib_FBComCommon_retMethod(pobj, errorstring, errorcode);
	//get the uint
	fbcomlib_getUInt_receivedUInt_set(getuint, ksapigetuint->v_receiveuint);

}

