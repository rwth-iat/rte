/**
 * @file unlinkObject.c
 * FunctionBlock for creating object.
 */
#ifndef OV_COMPILE_LIBRARY_fbcomlib
#define OV_COMPILE_LIBRARY_fbcomlib
#endif

#include "fbcomlib.h"
#include "../../include/config.h"
#include "libov/ov_macros.h"
#include "libov/ov_logfile.h"

/**
 *	returns the actual linkedpath
 */
OV_DLLFNCEXPORT OV_STRING fbcomlib_unlinkObject_linkedpath_get(
	OV_INSTPTR_fbcomlib_unlinkObject	pobj
) {
	return pobj->v_linkedpath;
}

/**
 *	sets the actual linkedpath
 */
OV_DLLFNCEXPORT OV_RESULT fbcomlib_unlinkObject_linkedpath_set(
	OV_INSTPTR_fbcomlib_unlinkObject	pobj,
	const OV_STRING								value
) {
	if(!(pobj->v_doSend)){
		return ov_string_setvalue(&pobj->v_linkedpath,value);
	}
	else{
		return OV_ERR_GENERIC;
	}
}

/**
 * If TRUE, does the reset FBComCommon and removed identifierList additionally
 * @todo Could the GET be removed???
 */
OV_DLLFNCEXPORT OV_BOOL fbcomlib_unlinkObject_doReset_get(
	OV_INSTPTR_fbcomlib_unlinkObject	pobj
) {
	return pobj->v_doReset;
}

/**
 * If TRUE, does the reset FBComCommon and removed identifierList additionally
 */
OV_DLLFNCEXPORT OV_RESULT fbcomlib_unlinkObject_doReset_set(
	OV_INSTPTR_fbcomlib_unlinkObject	pobj,
	const OV_BOOL										value
) {
	if(value){
		fbcomlib_FBComCommon_doResetAbstract_set((OV_INSTPTR_fbcomlib_FBComCommon)pobj, value);
		fbcomlib_unlinkObject_linkedpath_set(pobj, "");
	}
	return OV_ERR_OK;
}

/**
 * Individual startup method for creating ksapi_unlinkObject object for later communication
 */
OV_DLLFNCEXPORT void fbcomlib_unlinkObject_startup(
	OV_INSTPTR_ov_object 	pobj
) {
	OV_INSTPTR_fbcomlib_unlinkObject 					unlinkobject = Ov_StaticPtrCast(fbcomlib_unlinkObject, pobj);
	OV_VTBLPTR_fbcomlib_unlinkObject						stable = NULL;

	OV_INSTPTR_ksapi_unlinkObject							ksapiunlinkobject = NULL;
	OV_VTBLPTR_ksapi_unlinkObject							mtable = NULL;
	OV_RESULT res = 0;

    // do what the base class does first
    fb_functionblock_startup(pobj);

	Ov_GetVTablePtr(fbcomlib_unlinkObject, stable, unlinkobject);

	ksapiunlinkobject = (OV_INSTPTR_ksapi_unlinkObject)Ov_GetFirstChild(fbcomlib_FBComCommonAssoc, unlinkobject);
	if(! ksapiunlinkobject){
	  res = Ov_CreateObject(ksapi_unlinkObject, ksapiunlinkobject, unlinkobject, "unlinkObject");
	  if (Ov_Fail(res)){
		ov_logfile_error("Error while creating the unlinkObject");
		fbcomlib_FBComCommon_state_set((OV_INSTPTR_fbcomlib_FBComCommon)unlinkobject, STATE_OVERROR);
		return;
	  }
	  Ov_Link(fbcomlib_FBComCommonAssoc, unlinkobject, ksapiunlinkobject);
	}
	Ov_GetVTablePtr(ksapi_unlinkObject, mtable, ksapiunlinkobject);
	unlinkobject->v_doSend = FALSE;

	//(re)register the return method
	mtable->m_registerMethod((OV_INSTPTR_ksapi_KSCommon) ksapiunlinkobject, stable->m_retMethod);
	fbcomlib_FBComCommon_state_set((OV_INSTPTR_fbcomlib_FBComCommon)unlinkobject, STATE_OK);
	return;
}

/**
 * @todo Will be removed by martin since get* Commands could use a general FBComCommon/typemethod
 */
OV_DLLFNCEXPORT void fbcomlib_unlinkObject_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
	OV_INSTPTR_fbcomlib_unlinkObject					this = Ov_StaticPtrCast(fbcomlib_unlinkObject, pfb);
	OV_INSTPTR_fbcomlib_FBComCommon 					fbcommon = Ov_StaticPtrCast(fbcomlib_FBComCommon, pfb);
	OV_INSTPTR_ksapi_unlinkObject						ksapiunlinkobject = NULL;
	OV_VTBLPTR_ksapi_unlinkObject						mtable = NULL;
	//OV_INSTPTR_ksapi_KSCommon						kscommon = NULL;
	//OV_VTBLPTR_ksapi_KSCommon						mtable = NULL;

	//check if read was set to true
	if (fbcommon->v_doSend){
		fbcommon->v_doSend = FALSE;
		//check if all values are set
		if (!((fbcommon->v_server)&&(fbcommon->v_host)&&(fbcommon->v_path)&&(this->v_linkedpath))){
			ov_logfile_error("Please set all values: server,host,path,linkedpath");
			fbcomlib_FBComCommon_state_set(fbcommon, STATE_WRONGINPUT);
			return;
		}
		ksapiunlinkobject = (OV_INSTPTR_ksapi_unlinkObject)Ov_GetFirstChild(fbcomlib_FBComCommonAssoc, fbcommon);
		if(! ksapiunlinkobject) {
			ov_logfile_error("unlinkObject typemethod hasnt got a ksapi unlinkobject obj");
			return;
		}
		//get variable pointer
		Ov_GetVTablePtr(ksapi_unlinkObject, mtable, ksapiunlinkobject);
		//start reading
		mtable->m_setandsubmit(ksapiunlinkobject,fbcommon->v_host,fbcommon->v_server,fbcommon->v_path, this->v_linkedpath);
	}
	return;
}
