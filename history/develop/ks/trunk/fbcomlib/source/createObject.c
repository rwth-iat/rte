/**
 * @file createObject.c
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
 *	returns the actual librarypath
 */
OV_DLLFNCEXPORT OV_STRING fbcomlib_createObject_librarypath_get(
	OV_INSTPTR_fbcomlib_createObject	pobj
) {
	return pobj->v_librarypath;
}

/**
 *	sets the actual librarypath
 */
OV_DLLFNCEXPORT OV_RESULT fbcomlib_createObject_librarypath_set(
	OV_INSTPTR_fbcomlib_createObject	pobj,
	const OV_STRING								value
) {
	if(!(pobj->v_doSend)){
		return ov_string_setvalue(&pobj->v_librarypath,value);
	}
	else{
		return OV_ERR_GENERIC;
	}
}

/**
 *	returns the actual position
 */
OV_DLLFNCEXPORT OV_INT fbcomlib_createObject_position_get(
	OV_INSTPTR_fbcomlib_createObject		pobj
) {
	return pobj->v_position;
}

/**
 *	sets the actual position
 */
OV_DLLFNCEXPORT OV_RESULT fbcomlib_createObject_position_set(
	OV_INSTPTR_fbcomlib_createObject		pobj,
	const OV_INT           		value
) {
	if(!(pobj->v_doSend)){
		pobj->v_position=value;
		return OV_ERR_OK;
	}
	else{
		return OV_ERR_GENERIC;
	}
}

/**
 *	returns the actual element
 */
OV_DLLFNCEXPORT OV_STRING fbcomlib_createObject_element_get(
	OV_INSTPTR_fbcomlib_createObject	pobj
) {
	return pobj->v_element;
}

/**
 *	sets the actual element
 */
OV_DLLFNCEXPORT OV_RESULT fbcomlib_createObject_element_set(
	OV_INSTPTR_fbcomlib_createObject	pobj,
	const OV_STRING								value
) {
	if(!(pobj->v_doSend)){
		return ov_string_setvalue(&pobj->v_element,value);
	}
	else{
		return OV_ERR_GENERIC;
	}
}

/**
 * If TRUE, does the reset FBComCommon and removed identifierList additionally
 * @todo Could the GET be removed???
 */
OV_DLLFNCEXPORT OV_BOOL fbcomlib_createObject_doReset_get(
	OV_INSTPTR_fbcomlib_createObject	pobj
) {
	return pobj->v_doReset;
}

/**
 * If TRUE, does the reset FBComCommon and removed identifierList additionally
 */
OV_DLLFNCEXPORT OV_RESULT fbcomlib_createObject_doReset_set(
	OV_INSTPTR_fbcomlib_createObject	pobj,
	const OV_BOOL										value
) {
	if(value){
		fbcomlib_FBComCommon_doResetAbstract_set((OV_INSTPTR_fbcomlib_FBComCommon)pobj, value);
		fbcomlib_createObject_element_set(pobj, "");
		fbcomlib_createObject_librarypath_set(pobj, "");
		fbcomlib_createObject_position_set(pobj, 0);
	}
	return OV_ERR_OK;	
}

/**
 * Individual startup method for creating ksapi_createObject object for later communication
 */
OV_DLLFNCEXPORT void fbcomlib_createObject_startup(
	OV_INSTPTR_ov_object 	pobj
) {
	OV_INSTPTR_fbcomlib_createObject 					createobject = Ov_StaticPtrCast(fbcomlib_createObject, pobj);
	OV_VTBLPTR_fbcomlib_createObject						stable = NULL;

	OV_INSTPTR_ksapi_createObject							ksapicreateobject = NULL;
	OV_VTBLPTR_ksapi_createObject							mtable = NULL;
	OV_RESULT res = 0;

    // do what the base class does first
    fb_functionblock_startup(pobj);

	Ov_GetVTablePtr(fbcomlib_createObject, stable, createobject);

	ksapicreateobject = (OV_INSTPTR_ksapi_createObject)Ov_GetFirstChild(fbcomlib_FBComCommonAssoc, createobject);
	if(! ksapicreateobject){
	  res = Ov_CreateObject(ksapi_createObject, ksapicreateobject, createobject, "createObject");
	  if (Ov_Fail(res)){
		ov_logfile_error("Error while creating the createObject");
		fbcomlib_FBComCommon_state_set((OV_INSTPTR_fbcomlib_FBComCommon)createobject, STATE_OVERROR);
		return;
	  }
	  Ov_Link(fbcomlib_FBComCommonAssoc, createobject, ksapicreateobject);
	}
	Ov_GetVTablePtr(ksapi_createObject, mtable, ksapicreateobject);
	createobject->v_doSend = FALSE;

	//(re)register the return method
	mtable->m_registerMethod((OV_INSTPTR_ksapi_KSCommon) ksapicreateobject, stable->m_retMethod);
	fbcomlib_FBComCommon_state_set((OV_INSTPTR_fbcomlib_FBComCommon)createobject, STATE_OK);
	return;
}

/**
 * @todo Will be removed by martin since get* Commands could use a general FBComCommon/typemethod
 */
OV_DLLFNCEXPORT void fbcomlib_createObject_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
	OV_INSTPTR_fbcomlib_createObject					this = Ov_StaticPtrCast(fbcomlib_createObject, pfb);
	OV_INSTPTR_fbcomlib_FBComCommon 					fbcommon = Ov_StaticPtrCast(fbcomlib_FBComCommon, pfb);
	OV_INSTPTR_ksapi_createObject						ksapicreateobject = NULL;
	OV_VTBLPTR_ksapi_createObject						mtable = NULL;
	//OV_INSTPTR_ksapi_KSCommon						kscommon = NULL;
	//OV_VTBLPTR_ksapi_KSCommon						mtable = NULL;

	//check if read was set to true
	if (fbcommon->v_doSend){
		fbcommon->v_doSend = FALSE;
		//check if all values are set
		if (!((fbcommon->v_server)&&(fbcommon->v_host)&&(fbcommon->v_path)&&(this->v_librarypath))){
			ov_logfile_error("Please set all values: server,host,path,element,librarypath,position");
			fbcomlib_FBComCommon_state_set(fbcommon, STATE_WRONGINPUT);
			return;
		}
		else if ((this->v_position != 0)&&(!(this->v_element))){
			ov_logfile_error("Please set all values: server,host,path,element,librarypath,position");
			fbcomlib_FBComCommon_state_set(fbcommon, STATE_WRONGINPUT);
			return;
		}
		ksapicreateobject = (OV_INSTPTR_ksapi_createObject)Ov_GetFirstChild(fbcomlib_FBComCommonAssoc, fbcommon);
		if(! ksapicreateobject) {
			ov_logfile_error("createObject typemethod hasnt got a ksapi createobject obj");
			return;
		}
		//get variable pointer
		Ov_GetVTablePtr(ksapi_createObject, mtable, ksapicreateobject);
		//start reading
		mtable->m_setandsubmit(ksapicreateobject,fbcommon->v_host,fbcommon->v_server,fbcommon->v_path, this->v_librarypath, this->v_position, this->v_element);
	}
	return;
}
