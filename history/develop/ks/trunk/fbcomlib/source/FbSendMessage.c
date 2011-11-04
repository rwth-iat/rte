#ifndef OV_COMPILE_LIBRARY_fbcomlib
#define OV_COMPILE_LIBRARY_fbcomlib
#endif


#include "fbcomlib.h"
#include "msgconfig.h"
#include "libov/ov_macros.h"
#include "libov/ov_path.h"
#include "../../../ksapi/include/config.h"

OV_DLLFNCEXPORT OV_RESULT fbcomlib_FbSendMessage_constructor(
	OV_INSTPTR_ov_object 	pobj
) {
    /*
    *   local variables
    */
	OV_INSTPTR_fbcomlib_FbSendMessage this = Ov_StaticPtrCast(fbcomlib_FbSendMessage, pobj);
	OV_RESULT result = fb_functionblock_constructor(pobj);
	OV_INSTPTR_ksapi_setString setstring =  NULL;

    /* do what the base class does first */

    if(Ov_Fail(result))return result;

	this->v_iexreq = TRUE;
	this->v_cyctime = MSGCYCTIME;
	this->v_actimode = TRUE;

    /* do what */

	//initialize setString
	if(setstring){
		ksapi_KSCommon_deregisterMethod((OV_INSTPTR_ksapi_KSCommon)setstring);
	}

	if (Ov_Fail(Ov_CreateObject(ksapi_setString,setstring,this, "sendingInstance"))){
		ov_logfile_error("FbSendMsg/constructor: Error while creating the setString/sendingInstance!");
		return OV_ERR_GENERIC;
	}
	return OV_ERR_OK;
}
/**
 * fbcomlib_FbSendMessage_typemethod
 * Sends the messages existing below this object.
 * It just sends ONE message -- even locally -- per call to prevent a high load
 */
OV_DLLFNCEXPORT void fbcomlib_FbSendMessage_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
    /*
    *   local variables
    */
	OV_INSTPTR_fbcomlib_FbSendMessage this = Ov_StaticPtrCast(fbcomlib_FbSendMessage, pfb);
	OV_INSTPTR_fbcomlib_MsgComponent msgsys = NULL;
	OV_INSTPTR_ksapi_setString sendingInstance = NULL;
	OV_INSTPTR_ov_object child = NULL;
	OV_INSTPTR_fbcomlib_Message msg = NULL;
	OV_STRING value = NULL;
	OV_STRING receiverAddress = NULL;
	OV_STRING receiverName = NULL;


	sendingInstance = (OV_INSTPTR_ksapi_setString)Ov_SearchChild(ov_containment, pfb, "sendingInstance");
	if(! sendingInstance) { //TODO ClearText state bzw status!
		ov_logfile_error("FbSendMsg/typeMethod: Couldn't find sendingInstance.");
		return;
	}
	//check if we COULD send...
	if (ksapi_KSCommon_status_get((OV_INSTPTR_ksapi_KSCommon)sendingInstance) != STATUS_KSCOMMON_OK) {
		ov_logfile_error("FbSendMsg/typeMethod: Busy sendingInstance, waiting");
		return;
	}
	//we switch actimode off - its turned back on ONLY IF more messages are likely to exist
	this->v_actimode = 0;
	//check if we have sth to send...
	child = Ov_GetFirstChild(ov_containment, this);
	while(child) {
	  if(Ov_CanCastTo(fbcomlib_Message, child)){ //if this is a message and not the sendinginstance
		msg = Ov_StaticPtrCast(fbcomlib_Message, child);
		this->v_actimode = 1; //we might process this message now - and its likely that more msgs exist.
		if (msg->v_msgStatus == MSGNEW) { // take one
			fbcomlib_Message_serializeMessage(msg);
			//determine if we could locally send message!
			msgsys = (OV_INSTPTR_fbcomlib_MsgComponent)ov_path_getobjectpointer("/TechUnits/MsgSys", 2);
			if(msgsys &&
					ov_string_compare(msg->v_senderAddress, msg->v_receiverAddress) == 0&&
					ov_string_compare(msg->v_senderName,msg->v_receiverName) == 0) {
				ov_logfile_error("FbSendMsg/typeMethod: local delivery of msg to %s/%s!", msg->v_senderAddress, msg->v_senderName);
				fbcomlib_MsgComponent_retrieveMessage_set(msgsys, msg->v_sendString);
				Ov_DeleteObject(msg);
				return;
			}
			ov_string_setvalue(&value,msg->v_sendString);
			ov_string_setvalue(&receiverAddress,fbcomlib_Message_receiverAddress_get(msg));
			ov_string_setvalue(&receiverName,fbcomlib_Message_receiverName_get(msg));
			//ov_logfile_debug("FbSendMsg/typeMethod: Calling ksapi/setandsubmit");
			ov_logfile_debug("FbSendMsg/typeMethod: sending Msg to receiverAdress: %s ; receiverName: %s",receiverAddress,receiverName);
			ksapi_setString_setandsubmit(sendingInstance,receiverAddress,receiverName,"/TechUnits/MsgSys.retrieveMessage",value);
			Ov_DeleteObject(msg);
			return; //so no 2nd message is processed and overwrites data in sendingInstance.
		} else {
			ov_logfile_debug("FbSendMsg/typeMethod: Found Message with unknown nonNEW state %d - please manually remove!",msg->v_msgStatus);
			return;
		}
	  }
	  child = Ov_GetNextChild(ov_containment,child);
	}
	//Collecting Garbage
	ov_string_setvalue(&receiverAddress,NULL);
	ov_string_setvalue(&receiverName,NULL);
	ov_string_setvalue(&value,NULL);
	return;
}

/**
 * Creates a Msg obj from current input
 * sets current input to L* variables and sets LTIME
 */
OV_DLLFNCEXPORT OV_RESULT fbcomlib_FbSendMessage_ACTIMODE_set(
    OV_INSTPTR_fbcomlib_FbSendMessage          pobj,
    const OV_BOOL  value
) {
	OV_RESULT result = 0;
	OV_INSTPTR_fbcomlib_Message	message = NULL;
	OV_STRING canonicalServicePath = NULL;

	if(! value) return OV_ERR_OK; //just send message if set to 1
	//create Mesage obj
	fbcomlib_CreateAnonymousMessage(fbcomlib_Message, message, pobj, "Message",&result);
	if(! result) {
		ov_logfile_debug("FbSendMsg/ACTIMODE_set: cant create Message!");
		return OV_ERR_GENERIC;
	}
	//leave default for: message->v_sende,
	//retrieve local service path as senderService
    ov_memstack_lock();
    ov_string_print(&canonicalServicePath,"%s",ov_path_getcanonicalpath((OV_INSTPTR_ov_object)pobj,2));
	fbcomlib_Message_senderService_set(message, canonicalServicePath);
    ov_string_setvalue(&canonicalServicePath, NULL);
    ov_memstack_unlock();

	fbcomlib_Message_receiverAddress_set(message, fbcomlib_FbSendMessage_HOST_get(pobj));
	fbcomlib_Message_receiverName_set(message, fbcomlib_FbSendMessage_SERVER_get(pobj));
	fbcomlib_Message_receiverService_set(message, fbcomlib_FbSendMessage_PATH_get(pobj));
	fbcomlib_Message_msgType_set(message, fbcomlib_FbSendMessage_TYPE_get(pobj));
	fbcomlib_Message_msgBody_set(message, fbcomlib_FbSendMessage_CONTENT_get(pobj));
	pobj->v_ID = pobj->v_ID + 1; //increase ID for this message
	fbcomlib_Message_msgID_set(message, fbcomlib_FbSendMessage_ID_get(pobj));

	//set L* variables
	pobj->v_LHOST   = fbcomlib_FbSendMessage_HOST_get(pobj);
	pobj->v_LSERVER = fbcomlib_FbSendMessage_SERVER_get(pobj);
	pobj->v_LPATH   = fbcomlib_FbSendMessage_PATH_get(pobj);
	pobj->v_LTYPE   = fbcomlib_FbSendMessage_TYPE_get(pobj);
	pobj->v_LCONTENT = fbcomlib_FbSendMessage_CONTENT_get(pobj);
	ov_time_gettime(&pobj->v_LTIM);

	//switch this fb on, thus the message could be sent.
	pobj->v_actimode = 1;

	return OV_ERR_OK;
}



// Get Set methods

OV_DLLFNCEXPORT OV_BOOL fbcomlib_FbSendMessage_ACTIMODE_get(
    OV_INSTPTR_fbcomlib_FbSendMessage          pobj
) {
    return pobj->v_ACTIMODE;
}

OV_DLLFNCEXPORT OV_STRING fbcomlib_FbSendMessage_SERVER_get(
    OV_INSTPTR_fbcomlib_FbSendMessage          pobj
) {
    return pobj->v_SERVER;
}

OV_DLLFNCEXPORT OV_RESULT fbcomlib_FbSendMessage_SERVER_set(
    OV_INSTPTR_fbcomlib_FbSendMessage          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_SERVER,value);
}

OV_DLLFNCEXPORT OV_STRING fbcomlib_FbSendMessage_HOST_get(
    OV_INSTPTR_fbcomlib_FbSendMessage          pobj
) {
    return pobj->v_HOST;
}

OV_DLLFNCEXPORT OV_RESULT fbcomlib_FbSendMessage_HOST_set(
    OV_INSTPTR_fbcomlib_FbSendMessage          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_HOST,value);
}

OV_DLLFNCEXPORT OV_STRING fbcomlib_FbSendMessage_PATH_get(
    OV_INSTPTR_fbcomlib_FbSendMessage          pobj
) {
    return pobj->v_PATH;
}

OV_DLLFNCEXPORT OV_RESULT fbcomlib_FbSendMessage_PATH_set(
    OV_INSTPTR_fbcomlib_FbSendMessage          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_PATH,value);
}

OV_DLLFNCEXPORT OV_INT fbcomlib_FbSendMessage_TYPE_get(
    OV_INSTPTR_fbcomlib_FbSendMessage          pobj
) {
    return pobj->v_TYPE;
}

OV_DLLFNCEXPORT OV_RESULT fbcomlib_FbSendMessage_TYPE_set(
    OV_INSTPTR_fbcomlib_FbSendMessage          pobj,
    const OV_INT  value
) {
    pobj->v_TYPE = value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_STRING fbcomlib_FbSendMessage_CONTENT_get(
    OV_INSTPTR_fbcomlib_FbSendMessage          pobj
) {
    return pobj->v_CONTENT;
}

OV_DLLFNCEXPORT OV_RESULT fbcomlib_FbSendMessage_CONTENT_set(
    OV_INSTPTR_fbcomlib_FbSendMessage          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_CONTENT,value);
}

OV_DLLFNCEXPORT OV_STRING fbcomlib_FbSendMessage_LSERVER_get(
    OV_INSTPTR_fbcomlib_FbSendMessage          pobj
) {
    return pobj->v_LSERVER;
}

OV_DLLFNCEXPORT OV_STRING fbcomlib_FbSendMessage_LHOST_get(
    OV_INSTPTR_fbcomlib_FbSendMessage          pobj
) {
    return pobj->v_HOST;
}

OV_DLLFNCEXPORT OV_STRING fbcomlib_FbSendMessage_LPATH_get(
    OV_INSTPTR_fbcomlib_FbSendMessage          pobj
) {
    return pobj->v_LPATH;
}

OV_DLLFNCEXPORT OV_INT fbcomlib_FbSendMessage_LTYPE_get(
    OV_INSTPTR_fbcomlib_FbSendMessage          pobj
) {
    return pobj->v_LTYPE;
}

OV_DLLFNCEXPORT OV_STRING fbcomlib_FbSendMessage_LCONTENT_get(
    OV_INSTPTR_fbcomlib_FbSendMessage          pobj
) {
    return pobj->v_LCONTENT;
}

OV_DLLFNCEXPORT OV_TIME* fbcomlib_FbSendMessage_LTIM_get(
    OV_INSTPTR_fbcomlib_FbSendMessage          pobj
) {
    return &pobj->v_LTIM;
}

OV_DLLFNCEXPORT OV_INT fbcomlib_FbSendMessage_ID_get(
    OV_INSTPTR_fbcomlib_FbSendMessage          pobj
) {
    return pobj->v_ID;
}


