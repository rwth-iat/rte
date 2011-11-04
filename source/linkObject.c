/**
 * @file linkObject.c
 * FunctionBlock for linking object.
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
OV_DLLFNCEXPORT OV_STRING fbcomlib_linkObject_linkedpath_get(
	OV_INSTPTR_fbcomlib_linkObject	pobj
) {
	return pobj->v_linkedpath;
}

/**
 *	sets the actual linkedpath
 */
OV_DLLFNCEXPORT OV_RESULT fbcomlib_linkObject_linkedpath_set(
	OV_INSTPTR_fbcomlib_linkObject	pobj,
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
 *	returns the actual position
 */
OV_DLLFNCEXPORT OV_INT fbcomlib_linkObject_position_get(
	OV_INSTPTR_fbcomlib_linkObject		pobj
) {
	return pobj->v_position;
}

/**
 *	sets the actual position
 */
OV_DLLFNCEXPORT OV_RESULT fbcomlib_linkObject_position_set(
	OV_INSTPTR_fbcomlib_linkObject		pobj,
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
OV_DLLFNCEXPORT OV_STRING fbcomlib_linkObject_element_get(
	OV_INSTPTR_fbcomlib_linkObject	pobj
) {
	return pobj->v_element;
}

/**
 *	sets the actual element
 */
OV_DLLFNCEXPORT OV_RESULT fbcomlib_linkObject_element_set(
	OV_INSTPTR_fbcomlib_linkObject	pobj,
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
OV_DLLFNCEXPORT OV_BOOL fbcomlib_linkObject_doReset_get(
	OV_INSTPTR_fbcomlib_linkObject	pobj
) {
	return pobj->v_doReset;
}

/**
 * If TRUE, does the reset FBComCommon and removed identifierList additionally
 */
OV_DLLFNCEXPORT OV_RESULT fbcomlib_linkObject_doReset_set(
	OV_INSTPTR_fbcomlib_linkObject	pobj,
	const OV_BOOL										value
) {
	if(value){
		fbcomlib_FBComCommon_doResetAbstract_set((OV_INSTPTR_fbcomlib_FBComCommon)pobj, value);
		fbcomlib_linkObject_element_set(pobj, "");
		fbcomlib_linkObject_linkedpath_set(pobj, "");
		fbcomlib_linkObject_position_set(pobj, 0);
	}
	return OV_ERR_OK;	
}

/**
 * Individual startup method for creating ksapi_linkObject object for later communication
 */
OV_DLLFNCEXPORT void fbcomlib_linkObject_startup(
	OV_INSTPTR_ov_object 	pobj
) {
	OV_INSTPTR_fbcomlib_linkObject 					linkobject = Ov_StaticPtrCast(fbcomlib_linkObject, pobj);
	OV_VTBLPTR_fbcomlib_linkObject						stable = NULL;

	OV_INSTPTR_ksapi_linkObject							ksapilinkobject = NULL;
	OV_VTBLPTR_ksapi_linkObject							mtable = NULL;
	OV_RESULT res = 0;

    // do what the base class does first
    fb_functionblock_startup(pobj);

	Ov_GetVTablePtr(fbcomlib_linkObject, stable, linkobject);

	ksapilinkobject = (OV_INSTPTR_ksapi_linkObject)Ov_GetFirstChild(fbcomlib_FBComCommonAssoc, linkobject);
	if(! ksapilinkobject){
	  res = Ov_CreateObject(ksapi_linkObject, ksapilinkobject, linkobject, "linkObject");
	  if (Ov_Fail(res)){
		ov_logfile_error("Error while creating the linkObject");
		fbcomlib_FBComCommon_state_set((OV_INSTPTR_fbcomlib_FBComCommon)linkobject, STATE_OVERROR);
		return;
	  }
	  Ov_Link(fbcomlib_FBComCommonAssoc, linkobject, ksapilinkobject);
	}
	Ov_GetVTablePtr(ksapi_linkObject, mtable, ksapilinkobject);
	linkobject->v_doSend = FALSE;

	//(re)register the return method
	mtable->m_registerMethod((OV_INSTPTR_ksapi_KSCommon) ksapilinkobject, stable->m_retMethod);
	fbcomlib_FBComCommon_state_set((OV_INSTPTR_fbcomlib_FBComCommon)linkobject, STATE_OK);
	return;
}

/**
 * @todo Will be removed by martin since get* Commands could use a general FBComCommon/typemethod
 */
OV_DLLFNCEXPORT void fbcomlib_linkObject_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
	OV_INSTPTR_fbcomlib_linkObject					this = Ov_StaticPtrCast(fbcomlib_linkObject, pfb);
	OV_INSTPTR_fbcomlib_FBComCommon 					fbcommon = Ov_StaticPtrCast(fbcomlib_FBComCommon, pfb);
	OV_INSTPTR_ksapi_linkObject						ksapilinkobject = NULL;
	OV_VTBLPTR_ksapi_linkObject						mtable = NULL;
	//OV_INSTPTR_ksapi_KSCommon						kscommon = NULL;
	//OV_VTBLPTR_ksapi_KSCommon						mtable = NULL;

	//check if read was set to true
	if (fbcommon->v_doSend){
		fbcommon->v_doSend = FALSE;
		//check if all values are set
		if (!((fbcommon->v_server)&&(fbcommon->v_host)&&(fbcommon->v_path)&&(this->v_linkedpath))){
			ov_logfile_error("Please set all values: server,host,path,element,librarypath,position");
			fbcomlib_FBComCommon_state_set(fbcommon, STATE_WRONGINPUT);
			return;
		}
		else if ((this->v_position != 0)&&(!(this->v_element))){
			ov_logfile_error("Please set all values: server,host,path,element,librarypath,position");
			fbcomlib_FBComCommon_state_set(fbcommon, STATE_WRONGINPUT);
			return;
		}
		ksapilinkobject = (OV_INSTPTR_ksapi_linkObject)Ov_GetFirstChild(fbcomlib_FBComCommonAssoc, fbcommon);
		if(! ksapilinkobject) {
			ov_logfile_error("linkObject typemethod hasnt got a ksapi linkobject obj");
			return;
		}
		//get variable pointer
		Ov_GetVTablePtr(ksapi_linkObject, mtable, ksapilinkobject);
		//start reading
		mtable->m_setandsubmit(ksapilinkobject,fbcommon->v_host,fbcommon->v_server,fbcommon->v_path, this->v_linkedpath, this->v_position, this->v_element);
	}
	return;
}
