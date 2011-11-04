/**
 * @file deleteObject.c
 * FunctionBlock for deleting object.
 */
#ifndef OV_COMPILE_LIBRARY_fbcomlib
#define OV_COMPILE_LIBRARY_fbcomlib
#endif

#include "fbcomlib.h"
#include "../../include/config.h"
#include "libov/ov_macros.h"
#include "libov/ov_logfile.h"

/**
 * If TRUE, does the reset FBComCommon and removed identifierList additionally
 * @todo Could the GET be removed???
 */
OV_DLLFNCEXPORT OV_BOOL fbcomlib_deleteObject_doReset_get(
	OV_INSTPTR_fbcomlib_deleteObject	pobj
) {
	return pobj->v_doReset;
}

/**
 * If TRUE, does the reset FBComCommon and removed identifierList additionally
 */
OV_DLLFNCEXPORT OV_RESULT fbcomlib_deleteObject_doReset_set(
	OV_INSTPTR_fbcomlib_deleteObject	pobj,
	const OV_BOOL										value
) {
	if(value){
		fbcomlib_FBComCommon_doResetAbstract_set((OV_INSTPTR_fbcomlib_FBComCommon)pobj, value);
	}
	return OV_ERR_OK;
}

/**
 * Individual startup method for creating ksapi_deleteObject object for later communication
 */
OV_DLLFNCEXPORT void fbcomlib_deleteObject_startup(
	OV_INSTPTR_ov_object 	pobj
) {
	OV_INSTPTR_fbcomlib_deleteObject 					deleteobject = Ov_StaticPtrCast(fbcomlib_deleteObject, pobj);
	OV_VTBLPTR_fbcomlib_deleteObject						stable = NULL;

	OV_INSTPTR_ksapi_deleteObject							ksapideleteobject = NULL;
	OV_VTBLPTR_ksapi_deleteObject							mtable = NULL;
	OV_RESULT res = 0;

    // do what the base class does first
    fb_functionblock_startup(pobj);

	Ov_GetVTablePtr(fbcomlib_deleteObject, stable, deleteobject);

	ksapideleteobject = (OV_INSTPTR_ksapi_deleteObject)Ov_GetFirstChild(fbcomlib_FBComCommonAssoc, deleteobject);
	if(! ksapideleteobject){
	  res = Ov_CreateObject(ksapi_deleteObject, ksapideleteobject, deleteobject, "deleteObject");
	  if (Ov_Fail(res)){
		ov_logfile_error("Error while creating the deleteObject");
		fbcomlib_FBComCommon_state_set((OV_INSTPTR_fbcomlib_FBComCommon)deleteobject, STATE_OVERROR);
		return;
	  }
	  Ov_Link(fbcomlib_FBComCommonAssoc, deleteobject, ksapideleteobject);
	}
	Ov_GetVTablePtr(ksapi_deleteObject, mtable, ksapideleteobject);
	deleteobject->v_doSend = FALSE;

	//(re)register the return method
	mtable->m_registerMethod((OV_INSTPTR_ksapi_KSCommon) ksapideleteobject, stable->m_retMethod);
	fbcomlib_FBComCommon_state_set((OV_INSTPTR_fbcomlib_FBComCommon)deleteobject, STATE_OK);
	return;
}

/**
 * @todo Will be removed by martin since get* Commands could use a general FBComCommon/typemethod
 */
OV_DLLFNCEXPORT void fbcomlib_deleteObject_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
	OV_INSTPTR_fbcomlib_FBComCommon 					fbcommon = Ov_StaticPtrCast(fbcomlib_FBComCommon, pfb);
	OV_INSTPTR_ksapi_deleteObject						ksapideleteobject = NULL;
	OV_VTBLPTR_ksapi_deleteObject						mtable = NULL;
	//OV_INSTPTR_ksapi_KSCommon						kscommon = NULL;
	//OV_VTBLPTR_ksapi_KSCommon						mtable = NULL;

	//check if read was set to true
	if (fbcommon->v_doSend){
		fbcommon->v_doSend = FALSE;
		//check if all values are set
		if (!((fbcommon->v_server)&&(fbcommon->v_host)&&(fbcommon->v_path))){
			ov_logfile_error("Please set all values: server,host,path,element,librarypath,position");
			fbcomlib_FBComCommon_state_set(fbcommon, STATE_WRONGINPUT);
			return;
		}
		ksapideleteobject = (OV_INSTPTR_ksapi_deleteObject)Ov_GetFirstChild(fbcomlib_FBComCommonAssoc, fbcommon);
		if(! ksapideleteobject) {
			ov_logfile_error("deleteObject typemethod hasnt got a ksapi deleteobject obj");
			return;
		}
		//get variable pointer
		Ov_GetVTablePtr(ksapi_deleteObject, mtable, ksapideleteobject);
		//start reading
		mtable->m_setandsubmit(ksapideleteobject,fbcommon->v_host,fbcommon->v_server,fbcommon->v_path);
	}
	return;
}
