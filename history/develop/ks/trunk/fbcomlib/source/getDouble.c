/**
 * @file getDouble.c
 * FunctionBlock for retrieving a remote Double.
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
 * The received Double is stored here
 */
OV_DLLFNCEXPORT OV_DOUBLE fbcomlib_getDouble_receivedDouble_get(
    OV_INSTPTR_fbcomlib_getDouble          pobj
) {
    return pobj->v_receivedDouble;
}

/**
 * The received Double is stored here.
 * This Set shouldnt be used from outside!
 */
OV_DLLFNCEXPORT OV_RESULT fbcomlib_getDouble_receivedDouble_set(
    OV_INSTPTR_fbcomlib_getDouble          pobj,
    const OV_DOUBLE  value
) {

		pobj->v_receivedDouble = value;
		return OV_ERR_OK;
}

/**
 * If TRUE, does the reset FBComCommon and removed receivedDouble additionally
 * @todo Could the GET be removed???
 */
OV_DLLFNCEXPORT OV_BOOL fbcomlib_getDouble_doReset_get(
	    OV_INSTPTR_fbcomlib_getDouble          pobj
) {
    return pobj->v_doResetAbstract;
}

/**
 * If TRUE, does the reset FBComCommon and removed receivedDouble additionally
 */
OV_DLLFNCEXPORT OV_RESULT fbcomlib_getDouble_doReset_set(
	    OV_INSTPTR_fbcomlib_getDouble          pobj,
	    const OV_BOOL  value
) {
	if(value){
		fbcomlib_FBComCommon_doResetAbstract_set((OV_INSTPTR_fbcomlib_FBComCommon)pobj, value);
		fbcomlib_getDouble_receivedDouble_set(pobj, 0.0);
	}
	return OV_ERR_OK;
}

/**
 * Individual startup method for creating ksapi_getdouble object for later communication
 */
OV_DLLFNCEXPORT void fbcomlib_getDouble_startup(
	OV_INSTPTR_ov_object 	pobj
) {
    OV_INSTPTR_fbcomlib_getDouble 					getdouble = Ov_StaticPtrCast(fbcomlib_getDouble, pobj);
	OV_VTBLPTR_fbcomlib_getDouble					stable = NULL;

	OV_INSTPTR_ksapi_getDouble						ksapigetdouble = NULL;
	OV_VTBLPTR_ksapi_getDouble						mtable = NULL;
	OV_RESULT res = 0;

    // do what the base class does first
    fb_functionblock_startup(pobj);

	Ov_GetVTablePtr(fbcomlib_getDouble, stable, getdouble);

	ksapigetdouble = (OV_INSTPTR_ksapi_getDouble)Ov_GetFirstChild(fbcomlib_FBComCommonAssoc, getdouble);
	if(! ksapigetdouble){
	  res = Ov_CreateObject(ksapi_getDouble, ksapigetdouble, getdouble, "getDouble");
	  if (Ov_Fail(res)){
		ov_logfile_error("Error while creating the getDouble");
		fbcomlib_FBComCommon_state_set((OV_INSTPTR_fbcomlib_FBComCommon)getdouble, STATE_OVERROR);
		return;
	  }
	  Ov_Link(fbcomlib_FBComCommonAssoc, getdouble, ksapigetdouble);
	}
	Ov_GetVTablePtr(ksapi_getDouble, mtable, ksapigetdouble);
	getdouble->v_doSend = FALSE;

	//(re)register the return method
	mtable->m_registerMethod((OV_INSTPTR_ksapi_KSCommon) ksapigetdouble, stable->m_retMethod);
	fbcomlib_FBComCommon_state_set((OV_INSTPTR_fbcomlib_FBComCommon)getdouble, STATE_OK);
	return;

}

/**
 * Evaluates the answer of ksapi/getDouble.
 * Uses the FBComCommon/retMethod for status code and string
 * Additionally sets the receivedDouble.
 */
OV_DLLFNCEXPORT void fbcomlib_getDouble_retMethod(
		OV_INSTPTR_ov_object 		pobj,
		OV_STRING 						errorstring,
		OV_INT	 						errorcode
) {
	OV_INSTPTR_ksapi_getDouble ksapigetdouble = Ov_StaticPtrCast(ksapi_getDouble, pobj);
	OV_INSTPTR_fbcomlib_getDouble getdouble = (OV_INSTPTR_fbcomlib_getDouble)Ov_GetParent(fbcomlib_FBComCommonAssoc, ksapigetdouble);

	fbcomlib_FBComCommon_retMethod(pobj, errorstring, errorcode);
	//get the double
	fbcomlib_getDouble_receivedDouble_set(getdouble, ksapigetdouble->v_receivedouble);

}

