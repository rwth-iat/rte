/**
 * @file getString.c
 * FunctionBlock for retrieving a remote String.
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
 * The received String is stored here
 */
OV_DLLFNCEXPORT OV_STRING fbcomlib_getString_receivedString_get(
    OV_INSTPTR_fbcomlib_getString          pobj
) {
    return pobj->v_receivedString;
}

/**
 * The received String is stored here.
 * This Set shouldnt be used from outside!
 */
OV_DLLFNCEXPORT OV_RESULT fbcomlib_getString_receivedString_set(
    OV_INSTPTR_fbcomlib_getString          pobj,
    const OV_STRING  value
) {
	if(!(pobj->v_doSend)){
	    return ov_string_setvalue(&pobj->v_receivedString,value);
	} else{
		return OV_ERR_GENERIC;
	}

}

/**
 * If TRUE, does the reset FBComCommon and removed receivedString additionally
 * @todo Could the GET be removed???
 */
OV_DLLFNCEXPORT OV_BOOL fbcomlib_getString_doReset_get(
	    OV_INSTPTR_fbcomlib_getString          pobj
) {
    return pobj->v_doResetAbstract;
}

/**
 * If TRUE, does the reset FBComCommon and removed receivedString additionally
 */
OV_DLLFNCEXPORT OV_RESULT fbcomlib_getString_doReset_set(
	    OV_INSTPTR_fbcomlib_getString          pobj,
	    const OV_BOOL  value
) {
	if(value){
		fbcomlib_FBComCommon_doResetAbstract_set((OV_INSTPTR_fbcomlib_FBComCommon)pobj, value);
		fbcomlib_getString_receivedString_set(pobj, "");
	}
	return OV_ERR_OK;
}

/**
 * Individual startup method for creating ksapi_getstring object for later communication
 */
OV_DLLFNCEXPORT void fbcomlib_getString_startup(
	OV_INSTPTR_ov_object 	pobj
) {
    OV_INSTPTR_fbcomlib_getString 					getstring = Ov_StaticPtrCast(fbcomlib_getString, pobj);
	OV_VTBLPTR_fbcomlib_getString					stable = NULL;

	OV_INSTPTR_ksapi_getString						ksapigetstring = NULL;
	OV_VTBLPTR_ksapi_getString						mtable = NULL;
	OV_RESULT res = 0;

    // do what the base class does first
    fb_functionblock_startup(pobj);

	Ov_GetVTablePtr(fbcomlib_getString, stable, getstring);

	ksapigetstring = (OV_INSTPTR_ksapi_getString)Ov_GetFirstChild(fbcomlib_FBComCommonAssoc, getstring);
	if(! ksapigetstring){
	  res = Ov_CreateObject(ksapi_getString, ksapigetstring, getstring, "getString");
	  if (Ov_Fail(res)){
		ov_logfile_error("Error while creating the getString");
		fbcomlib_FBComCommon_state_set((OV_INSTPTR_fbcomlib_FBComCommon)getstring, STATE_OVERROR);
		return;
	  }
	  Ov_Link(fbcomlib_FBComCommonAssoc, getstring, ksapigetstring);
	}
	Ov_GetVTablePtr(ksapi_getString, mtable, ksapigetstring);
	getstring->v_doSend = FALSE;

	//(re)register the return method
	mtable->m_registerMethod((OV_INSTPTR_ksapi_KSCommon) ksapigetstring, stable->m_retMethod);
	fbcomlib_FBComCommon_state_set((OV_INSTPTR_fbcomlib_FBComCommon)getstring, STATE_OK);
	return;

}

/**
 * Evaluates the answer of ksapi/getString.
 * Uses the FBComCommon/retMethod for status code and string
 * Additionally sets the receivedString.
 */
OV_DLLFNCEXPORT void fbcomlib_getString_retMethod(
		OV_INSTPTR_ov_object 		pobj,
		OV_STRING 						errorstring,
		OV_INT	 						errorcode
) {
	OV_INSTPTR_ksapi_getString ksapigetstring = Ov_StaticPtrCast(ksapi_getString, pobj);
	OV_INSTPTR_fbcomlib_getString getstring = (OV_INSTPTR_fbcomlib_getString)Ov_GetParent(fbcomlib_FBComCommonAssoc, ksapigetstring);

	fbcomlib_FBComCommon_retMethod(pobj, errorstring, errorcode);
	//get the string
	fbcomlib_getString_receivedString_set(getstring, ksapigetstring->v_receivestring);

}

