#ifndef OV_COMPILE_LIBRARY_fbcomlib
#define OV_COMPILE_LIBRARY_fbcomlib
#endif

#include "fbcomlib.h"
#include "../../include/config.h"
#include "libov/ov_macros.h"
#include "libov/ov_logfile.h"

/**
 * The Hostname/IPAdr. of the destination server
 */
OV_DLLFNCEXPORT OV_STRING fbcomlib_FBComCommon_host_get(
    OV_INSTPTR_fbcomlib_FBComCommon          pobj
) {
    return pobj->v_host;
}

/**
 * The Hostname/IPAdr. of the destination server
 */
OV_DLLFNCEXPORT OV_RESULT fbcomlib_FBComCommon_host_set(
    OV_INSTPTR_fbcomlib_FBComCommon          pobj,
    const OV_STRING  value
) {
	if(!(pobj->v_doSend)){
		return ov_string_setvalue(&pobj->v_host,value);
	}
	else {
		return OV_ERR_GENERIC;
	}
}

/**
 * The FB/OVServer of the destination server
 */
OV_DLLFNCEXPORT OV_STRING fbcomlib_FBComCommon_server_get(
    OV_INSTPTR_fbcomlib_FBComCommon          pobj
) {
    return pobj->v_server;
}

/**
 * The FB/OVServer of the destination server
 */
OV_DLLFNCEXPORT OV_RESULT fbcomlib_FBComCommon_server_set(
    OV_INSTPTR_fbcomlib_FBComCommon          pobj,
    const OV_STRING  value
) {
	if(!(pobj->v_doSend)){
	    return ov_string_setvalue(&pobj->v_server,value);
	}
	else {
		return OV_ERR_GENERIC;
	}
}

/**
 * The path of the destination variable
 */
OV_DLLFNCEXPORT OV_STRING fbcomlib_FBComCommon_path_get(
    OV_INSTPTR_fbcomlib_FBComCommon          pobj
) {
    return pobj->v_path;
}

/**
 * The path of the destination variable
 */
OV_DLLFNCEXPORT OV_RESULT fbcomlib_FBComCommon_path_set(
    OV_INSTPTR_fbcomlib_FBComCommon          pobj,
    const OV_STRING  value
) {
	if(!(pobj->v_doSend)){
	    return ov_string_setvalue(&pobj->v_path,value);
	}
	else {
		return OV_ERR_GENERIC;
	}
}

/**
 * do sending, now
 * requires actimode to be ON
 */
OV_DLLFNCEXPORT OV_BOOL fbcomlib_FBComCommon_doSend_get(
    OV_INSTPTR_fbcomlib_FBComCommon          pobj
) {
    return pobj->v_doSend;
}

/**
 * do sending, now
 * requires actimode to be ON
 */
OV_DLLFNCEXPORT OV_RESULT fbcomlib_FBComCommon_doSend_set(
    OV_INSTPTR_fbcomlib_FBComCommon          pobj,
    const OV_BOOL  value
) {
	if (pobj->v_doSend){
		return OV_ERR_GENERIC;
	}
	if(value){
		pobj->v_state=0;
		fbcomlib_FBComCommon_state_set(pobj, STATE_INIT);
	}
	pobj->v_doSend = value;
    return OV_ERR_OK;
}

/**
 * reset this instance.
 * is called by the inheriting objects so something like "abstract"
 * Shouldnt be used from outsid!
 * @todo Could this be realized as function?
 */
OV_DLLFNCEXPORT OV_BOOL fbcomlib_FBComCommon_doResetAbstract_get(
    OV_INSTPTR_fbcomlib_FBComCommon          pobj
) {
    return pobj->v_doResetAbstract;
}

/**
 * reset this instance.
 * is called by the inheriting objects so something like "abstract"
 * Shouldnt be used from outsid!
 * @todo Could this be realized as function?
 */
OV_DLLFNCEXPORT OV_RESULT fbcomlib_FBComCommon_doResetAbstract_set(
    OV_INSTPTR_fbcomlib_FBComCommon          pobj,
    const OV_BOOL  value
) {
	//OV_INSTPTR_ksapi_KSCommon			kscommon;
	//OV_VTBLPTR_ksapi_KSCommon			mtable;

	if(value){
		//Ov_GetVTablePtr(ksapi_KSCommon, mtable, kscommon);
		//Ov_GetVTablePtr(ksapi_getString, gtable, preader);
		pobj->v_doSend=FALSE;
		fbcomlib_FBComCommon_state_set(pobj, -1);

		fbcomlib_FBComCommon_server_set(pobj, "");
		fbcomlib_FBComCommon_path_set(pobj, "");
		fbcomlib_FBComCommon_host_set(pobj, "");

	}

	//@todo Why deregister in orig ksapiexample???
	//if (!(Ov_GetFirstChild(fbcomlib_FBComCommonAssoc, pobj) == NULL)){
	//	kscommon = Ov_GetFirstChild(fbcomlib_FBComCommonAssoc, pobj);
	//} else {
	//	return OV_ERR_GENERIC; //we should have a kscommon child!
	//}
	//mtable->m_deregisterMethod(kscommon);

	return OV_ERR_OK;
}

/**
 * do cyclic execution.
 * Indicates that a command sould be repeated if the previous one
 * is finied (ignores state of previous!!!)
 */
OV_DLLFNCEXPORT OV_BOOL fbcomlib_FBComCommon_doCyclic_get(
    OV_INSTPTR_fbcomlib_FBComCommon          pobj
) {
    return pobj->v_doCyclic;
}

/**
 * do cyclic execution.
 * Indicates that a command should be repeated if the previous one
 * is finished (ignores state of previous!!!)
 */
OV_DLLFNCEXPORT OV_RESULT fbcomlib_FBComCommon_doCyclic_set(
    OV_INSTPTR_fbcomlib_FBComCommon          pobj,
    const OV_BOOL  value
) {
    pobj->v_doCyclic = value;
    return OV_ERR_OK;
}

/**
 * indicate state by string
 * A human readable description of the state variable
 */
OV_DLLFNCEXPORT OV_STRING fbcomlib_FBComCommon_stateString_get(
    OV_INSTPTR_fbcomlib_FBComCommon          pobj
) {
    return pobj->v_stateString;
}

/**
 * indicate state by string
 * A human readable description of the state variable
 */
OV_DLLFNCEXPORT OV_INT fbcomlib_FBComCommon_state_get(
    OV_INSTPTR_fbcomlib_FBComCommon          pobj
) {
    return pobj->v_state;
}

/**
 * indicate state of current/last execution
 * sets the stateString, too
 */
OV_DLLFNCEXPORT OV_RESULT fbcomlib_FBComCommon_state_set(
    OV_INSTPTR_fbcomlib_FBComCommon          pobj,
    const OV_INT  value
) {
    pobj->v_state = value;
    //set stateString as value is defined
    if(value == STATE_INIT) pobj->v_stateString = STATESTRING_INIT;
    else if(value == STATE_OK) pobj->v_stateString = STATESTRING_OK;
    else if(value == STATE_WRONGINPUT) pobj->v_stateString = STATESTRING_WRONGINPUT;
    else if(value == STATE_NORETMETHOD) pobj->v_stateString = STATESTRING_NORETMETHOD;
    else if(value == STATE_OVERROR) pobj->v_stateString = STATESTRING_OVERROR;
    else if(value == STATE_CONNECTIONERROR) pobj->v_stateString = STATESTRING_CONNECTIONERROR;
    else if(value == STATE_SERVICEERROR) pobj->v_stateString = STATESTRING_SERVICEERROR;
    else if(value == STATE_WRONGVARONDESTINATION) pobj->v_stateString = STATESTRING_WRONGVARONDESTINATION;
    else if(value == STATE_CLIENTBUSY) pobj->v_stateString = STATESTRING_CLIENTBUSY;
    else if(value == STATE_NOMEMORY) pobj->v_stateString = STATESTRING_NOMEMORY;
    else pobj->v_stateString = STATESTRING_UNKNOWNERROR;
    return OV_ERR_OK;
}


/**
 * the default retmethod, which is called by all inheriting classes
 * This evaluates just state and statestring.
 *
 * This is enough for all SET* Commands and needs to be overwritten and called by GET*
 */
OV_DLLFNCEXPORT void fbcomlib_FBComCommon_retMethod(
		OV_INSTPTR_ov_object 		pobj,
		OV_STRING 						errorstring,
		OV_INT	 						errorcode
) {
	OV_INSTPTR_ksapi_KSCommon kscommon = Ov_StaticPtrCast(ksapi_KSCommon, pobj);
	OV_INSTPTR_fbcomlib_FBComCommon	fbcomconnon = Ov_GetParent(fbcomlib_FBComCommonAssoc, kscommon);

	ov_logfile_debug("fbcomlib retmethod called with %i / %s",errorcode,errorstring);
	fbcomlib_FBComCommon_state_set(fbcomconnon, errorcode);
	//@todo: ?? wurde in der ksapiexample gemacht: fbcomlib_FBComCommon_stateString_set(fbcomconnon, errorstring);
	fbcomconnon->v_stateString = errorstring;

	if(fbcomconnon->v_doCyclic) fbcomlib_FBComCommon_doSend_set(fbcomconnon, TRUE); // cycle if requested
	return;
}

/**
 * Constructor just activates the function block by default
 * Everything else is done in the inheriting classes's startup methods, since
 * ksapi-command-specific objects need to be defined.
 */
OV_DLLFNCEXPORT OV_RESULT fbcomlib_FBComCommon_constructor(
	OV_INSTPTR_ov_object 							pobj
) {
	OV_INSTPTR_fb_functionblock fb = Ov_StaticPtrCast(fb_functionblock, pobj);
	fb_functionblock_constructor(pobj);

	fb->v_actimode = 1;

	return OV_ERR_OK;
}

/**
 * @todo Will be removed by martin since get* Commands could use a general FBComCommon/typemethod
 */
OV_DLLFNCEXPORT void fbcomlib_FBComCommon_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
    OV_INSTPTR_fbcomlib_FBComCommon 				fbcommon = Ov_StaticPtrCast(fbcomlib_FBComCommon, pfb);
	OV_INSTPTR_ksapi_KSCommon						kscommon = NULL;
	OV_VTBLPTR_ksapi_KSCommon						mtable = NULL;

	//check if read was set to true
	if (fbcommon->v_doSend){
		fbcommon->v_doSend = FALSE;
		//check if all values are set
		if (!((fbcommon->v_server)&&(fbcommon->v_host)&&(fbcommon->v_path))){
			ov_logfile_error("Please set all values: server,host,path");
			fbcomlib_FBComCommon_state_set(fbcommon, STATE_WRONGINPUT);
			return;
		}
		kscommon = (OV_INSTPTR_ksapi_KSCommon)Ov_GetFirstChild(fbcomlib_FBComCommonAssoc, fbcommon);
		if(! kscommon) {
			ov_logfile_error("get* typemethod hasnt got a ksapi obj");
			return;
		}
		//get variable pointer
		Ov_GetVTablePtr(ksapi_KSCommon, mtable, kscommon);
		//start reading
		mtable->m_setandsubmitget(kscommon,fbcommon->v_host,fbcommon->v_server,fbcommon->v_path);
	}
	return;
}

