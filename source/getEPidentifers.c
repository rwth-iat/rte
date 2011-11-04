/**
 * @file getEPidentifiers.c
 * FunctionBlock for retrieving EPidentifiers.
 */
#ifndef OV_COMPILE_LIBRARY_fbcomlib
#define OV_COMPILE_LIBRARY_fbcomlib
#endif


#include "fbcomlib.h"
#include "../../include/config.h"
#include "libov/ov_macros.h"
#include "libov/ov_logfile.h"


/**
 *	returns the actual namemask
 */
OV_DLLFNCEXPORT OV_STRING fbcomlib_getEPidentifiers_namemask_get(
	OV_INSTPTR_fbcomlib_getEPidentifiers	pobj
) {
	return pobj->v_namemask;
}

/**
 *	sets the actual namemask
 */
OV_DLLFNCEXPORT OV_RESULT fbcomlib_getEPidentifiers_namemask_set(
	OV_INSTPTR_fbcomlib_getEPidentifiers	pobj,
	const OV_STRING											value
) {
	if(!(pobj->v_doSend)){
	    return ov_string_setvalue(&pobj->v_namemask,value);
	} else{
		return OV_ERR_GENERIC;
	}
}

/**
 * If TRUE, does the reset FBComCommon and removed identifierList additionally
 * @todo Could the GET be removed???
 */
OV_DLLFNCEXPORT OV_BOOL fbcomlib_getEPidentifiers_doReset_get(
	OV_INSTPTR_fbcomlib_getEPidentifiers	pobj
) {
	return pobj->v_doReset;
}

/**
 * If TRUE, does the reset FBComCommon and removed identifierList additionally
 */
OV_DLLFNCEXPORT OV_RESULT fbcomlib_getEPidentifiers_doReset_set(
	OV_INSTPTR_fbcomlib_getEPidentifiers		pobj,
	const OV_BOOL													value
) {
	if(value){
		fbcomlib_FBComCommon_doResetAbstract_set((OV_INSTPTR_fbcomlib_FBComCommon)pobj, value);
		fbcomlib_getEPidentifiers_namemask_set(pobj, "");
		Ov_SetDynamicVectorValue(&pobj->v_identifierList, NULL, 0, STRING);
	}
	return OV_ERR_OK;
}

/**
 * Individual startup method for creating ksapi_getEPidentifiers object for later communication
 */
OV_DLLFNCEXPORT void fbcomlib_getEPidentifiers_startup(
	OV_INSTPTR_ov_object 									pobj
) {
	OV_INSTPTR_fbcomlib_getEPidentifiers 					getepidentifiers = Ov_StaticPtrCast(fbcomlib_getEPidentifiers, pobj);
	OV_VTBLPTR_fbcomlib_getEPidentifiers					stable = NULL;

	OV_INSTPTR_ksapi_getEPidentifiers						ksapigetepidentifiers = NULL;
	OV_VTBLPTR_ksapi_getEPidentifiers						mtable = NULL;
	OV_RESULT res = 0;

    // do what the base class does first
    fb_functionblock_startup(pobj);

	Ov_GetVTablePtr(fbcomlib_getEPidentifiers, stable, getepidentifiers);

	ksapigetepidentifiers = (OV_INSTPTR_ksapi_getEPidentifiers)Ov_GetFirstChild(fbcomlib_FBComCommonAssoc, getepidentifiers);
	if(! ksapigetepidentifiers){
	  res = Ov_CreateObject(ksapi_getEPidentifiers, ksapigetepidentifiers, getepidentifiers, "getEPidentifiers");
	  if (Ov_Fail(res)){
		ov_logfile_error("Error while creating the getEPidentifiers");
		fbcomlib_FBComCommon_state_set((OV_INSTPTR_fbcomlib_FBComCommon)getepidentifiers, STATE_OVERROR);
		return;
	  }
	  Ov_Link(fbcomlib_FBComCommonAssoc, getepidentifiers, ksapigetepidentifiers);
	}
	Ov_GetVTablePtr(ksapi_getEPidentifiers, mtable, ksapigetepidentifiers);
	getepidentifiers->v_doSend = FALSE;

	//(re)register the return method
	mtable->m_registerMethod((OV_INSTPTR_ksapi_KSCommon) ksapigetepidentifiers, stable->m_retMethod);
	fbcomlib_FBComCommon_state_set((OV_INSTPTR_fbcomlib_FBComCommon)getepidentifiers, STATE_OK);
	return;
}

/**
 * @todo Will be removed by martin since get* Commands could use a general FBComCommon/typemethod
 */
OV_DLLFNCEXPORT void fbcomlib_getEPidentifiers_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
	OV_INSTPTR_fbcomlib_getEPidentifiers			this = Ov_StaticPtrCast(fbcomlib_getEPidentifiers, pfb);
	OV_INSTPTR_fbcomlib_FBComCommon 					fbcommon = Ov_StaticPtrCast(fbcomlib_FBComCommon, pfb);
	OV_INSTPTR_ksapi_getEPidentifiers				ksapigetepidentifiers = NULL;
	OV_VTBLPTR_ksapi_getEPidentifiers				mtable = NULL;
	//OV_INSTPTR_ksapi_KSCommon						kscommon = NULL;
	//OV_VTBLPTR_ksapi_KSCommon						mtable = NULL;

	//check if read was set to true
	if (fbcommon->v_doSend){
		fbcommon->v_doSend = FALSE;
		//check if all values are set
		if (!((fbcommon->v_server)&&(fbcommon->v_host)&&(fbcommon->v_path)&&(this->v_namemask))){
			ov_logfile_error("Please set all values: server,host,path,namemask");
			fbcomlib_FBComCommon_state_set(fbcommon, STATE_WRONGINPUT);
			return;
		}
		ksapigetepidentifiers = (OV_INSTPTR_ksapi_getEPidentifiers)Ov_GetFirstChild(fbcomlib_FBComCommonAssoc, fbcommon);
		if(! ksapigetepidentifiers) {
			ov_logfile_error("getEPidentifiers typemethod hasnt got a ksapi epidentifiers obj");
			return;
		}
		//get variable pointer
		Ov_GetVTablePtr(ksapi_getEPidentifiers, mtable, ksapigetepidentifiers);
		//start reading
		mtable->m_setandsubmit(ksapigetepidentifiers,fbcommon->v_host,fbcommon->v_server,fbcommon->v_path, this->v_namemask);
	}
	return;
}

/**
 * Evaluates the answer of ksapi/getEPidentifiers.
 * Uses the FBComCommon/retMethod for status code and string
 */
OV_DLLFNCEXPORT void fbcomlib_getEPidentifiers_retMethod(
		OV_INSTPTR_ov_object 								pobj,
		OV_STRING 												errorstring,
		OV_INT	 												errorcode
) {
	OV_INSTPTR_ksapi_getEPidentifiers ksapigetepidentifiers = Ov_StaticPtrCast(ksapi_getEPidentifiers, pobj);
	OV_INSTPTR_fbcomlib_getEPidentifiers getepidentifiers = (OV_INSTPTR_fbcomlib_getEPidentifiers)Ov_GetParent(fbcomlib_FBComCommonAssoc, ksapigetepidentifiers);
	OV_UINT	i=0;
	OV_RESULT result;

	fbcomlib_FBComCommon_retMethod(pobj, errorstring, errorcode);
	//get the string
	for (i = 0; i < ksapigetepidentifiers->v_identifierList.veclen; i++)
	{
		result = ov_string_setvalue(&(getepidentifiers->v_identifierList.value[i]), ksapigetepidentifiers->v_identifierList.value[i]);
		if (Ov_Fail(result))
		{
			ov_logfile_error("Error setting string");
			Ov_SetDynamicVectorValue(&getepidentifiers->v_identifierList, NULL, 0, STRING);
			return;
		}
	}
}
