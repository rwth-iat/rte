/**
 * @file renameObject.c
 * FunctionBlock for renaming object.
 */
#ifndef OV_COMPILE_LIBRARY_fbcomlib
#define OV_COMPILE_LIBRARY_fbcomlib
#endif

#include "fbcomlib.h"
#include "../../include/config.h"
#include "libov/ov_macros.h"
#include "libov/ov_logfile.h"

/**
 *	returns the actual newpath
 */
OV_DLLFNCEXPORT OV_STRING fbcomlib_renameObject_newpath_get(
	OV_INSTPTR_fbcomlib_renameObject	pobj
) {
	return pobj->v_newpath;
}

/**
 *	sets the actual newpath
 */
OV_DLLFNCEXPORT OV_RESULT fbcomlib_renameObject_newpath_set(
	OV_INSTPTR_fbcomlib_renameObject	pobj,
	const OV_STRING								value
) {
	if(!(pobj->v_doSend)){
		return ov_string_setvalue(&pobj->v_newpath,value);
	}
	else{
		return OV_ERR_GENERIC;
	}
}

/**
 * If TRUE, does the reset FBComCommon and removed identifierList additionally
 * @todo Could the GET be removed???
 */
OV_DLLFNCEXPORT OV_BOOL fbcomlib_renameObject_doReset_get(
	OV_INSTPTR_fbcomlib_renameObject	pobj
) {
	return pobj->v_doReset;
}

/**
 * If TRUE, does the reset FBComCommon and removed identifierList additionally
 */
OV_DLLFNCEXPORT OV_RESULT fbcomlib_renameObject_doReset_set(
	OV_INSTPTR_fbcomlib_renameObject	pobj,
	const OV_BOOL										value
) {
	if(value){
		fbcomlib_FBComCommon_doResetAbstract_set((OV_INSTPTR_fbcomlib_FBComCommon)pobj, value);
		fbcomlib_renameObject_newpath_set(pobj, "");
	}
	return OV_ERR_OK;
}

/**
 * Individual startup method for creating ksapi_renameObject object for later communication
 */
OV_DLLFNCEXPORT void fbcomlib_renameObject_startup(
	OV_INSTPTR_ov_object 	pobj
) {
	OV_INSTPTR_fbcomlib_renameObject 					renameobject = Ov_StaticPtrCast(fbcomlib_renameObject, pobj);
	OV_VTBLPTR_fbcomlib_renameObject						stable = NULL;

	OV_INSTPTR_ksapi_renameObject							ksapirenameobject = NULL;
	OV_VTBLPTR_ksapi_renameObject							mtable = NULL;
	OV_RESULT res = 0;

    // do what the base class does first
    fb_functionblock_startup(pobj);

	Ov_GetVTablePtr(fbcomlib_renameObject, stable, renameobject);

	ksapirenameobject = (OV_INSTPTR_ksapi_renameObject)Ov_GetFirstChild(fbcomlib_FBComCommonAssoc, renameobject);
	if(! ksapirenameobject){
	  res = Ov_CreateObject(ksapi_renameObject, ksapirenameobject, renameobject, "renameObject");
	  if (Ov_Fail(res)){
		ov_logfile_error("Error while creating the renameObject");
		fbcomlib_FBComCommon_state_set((OV_INSTPTR_fbcomlib_FBComCommon)renameobject, STATE_OVERROR);
		return;
	  }
	  Ov_Link(fbcomlib_FBComCommonAssoc, renameobject, ksapirenameobject);
	}
	Ov_GetVTablePtr(ksapi_renameObject, mtable, ksapirenameobject);
	renameobject->v_doSend = FALSE;

	//(re)register the return method
	mtable->m_registerMethod((OV_INSTPTR_ksapi_KSCommon) ksapirenameobject, stable->m_retMethod);
	fbcomlib_FBComCommon_state_set((OV_INSTPTR_fbcomlib_FBComCommon)renameobject, STATE_OK);
	return;
}

/**
 * @todo Will be removed by martin since get* Commands could use a general FBComCommon/typemethod
 */
OV_DLLFNCEXPORT void fbcomlib_renameObject_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
	OV_INSTPTR_fbcomlib_renameObject					this = Ov_StaticPtrCast(fbcomlib_renameObject, pfb);
	OV_INSTPTR_fbcomlib_FBComCommon 					fbcommon = Ov_StaticPtrCast(fbcomlib_FBComCommon, pfb);
	OV_INSTPTR_ksapi_renameObject						ksapirenameobject = NULL;
	OV_VTBLPTR_ksapi_renameObject						mtable = NULL;
	//OV_INSTPTR_ksapi_KSCommon						kscommon = NULL;
	//OV_VTBLPTR_ksapi_KSCommon						mtable = NULL;

	//check if read was set to true
	if (fbcommon->v_doSend){
		fbcommon->v_doSend = FALSE;
		//check if all values are set
		if (!((fbcommon->v_server)&&(fbcommon->v_host)&&(fbcommon->v_path)&&(this->v_newpath))){
			ov_logfile_error("Please set all values: server,host,path");
			fbcomlib_FBComCommon_state_set(fbcommon, STATE_WRONGINPUT);
			return;
		}
		ksapirenameobject = (OV_INSTPTR_ksapi_renameObject)Ov_GetFirstChild(fbcomlib_FBComCommonAssoc, fbcommon);
		if(! ksapirenameobject) {
			ov_logfile_error("renameObject typemethod hasnt got a ksapi renameobject obj");
			return;
		}
		//get variable pointer
		Ov_GetVTablePtr(ksapi_renameObject, mtable, ksapirenameobject);
		//start reading
		mtable->m_setandsubmit(ksapirenameobject,fbcommon->v_host,fbcommon->v_server,fbcommon->v_path, this->v_newpath);
	}
	return;
}
