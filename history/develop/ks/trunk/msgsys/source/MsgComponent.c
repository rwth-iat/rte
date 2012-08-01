#ifndef OV_COMPILE_LIBRARY_msgsys
#define OV_COMPILE_LIBRARY_msgsys
#endif


#include "msgsys.h"
#include "msgconfig.h"
#include "libov/ov_macros.h"

OV_DLLFNCEXPORT OV_RESULT msgsys_MsgComponent_constructor(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
	OV_INSTPTR_msgsys_MsgComponent this = Ov_StaticPtrCast(msgsys_MsgComponent, pobj);
	OV_RESULT result = fb_functionblock_constructor(pobj);
	OV_INSTPTR_ksapi_setString setstring =  NULL;
	OV_VTBLPTR_ksapi_setString setstringVtable = NULL;
	OV_VTBLPTR_msgsys_MsgComponent thisVtable = NULL;

    /* do what the base class does first */

    if(Ov_Fail(result))return result;
	
	this->v_iexreq = TRUE;
	this->v_cyctime = MSGCYCTIME;
	this->v_actimode = TRUE;

    /* do what */
	ov_logfile_error("MsgComp/constructor: do what");

	//initialize setString
	if(setstring){
		ksapi_KSCommon_deregisterMethod((OV_INSTPTR_ksapi_KSCommon)setstring);
	}
	
	ov_logfile_error("MsgComp/constructor: initialize setString");

	if (Ov_Fail(Ov_CreateObject(ksapi_setString,setstring,this, "sendingInstance"))){
		ov_logfile_error("MsgComp/constructor: Error while creating the setString/sendingInstance!");
		return OV_ERR_GENERIC;
	}
	ov_logfile_error("MsgComp/constructor: Ov_CreateObject(ksapi_setString");

	//set up return method -- TODO Check: Why is this required? Startup is NOT enough!!!
	Ov_GetVTablePtr(ksapi_setString, setstringVtable, setstring);
	Ov_GetVTablePtr(msgsys_MsgComponent, thisVtable, this);
	setstringVtable->m_registerMethod((OV_INSTPTR_ksapi_KSCommon)setstring,thisVtable->m_retMethod);

	ov_logfile_error("MsgComp/constructor: return OV_ERR_OK");
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT void msgsys_MsgComponent_startup(

	OV_INSTPTR_ov_object 	pobj

) {
	OV_INSTPTR_ksapi_setString setstring =  NULL;
	OV_VTBLPTR_ksapi_setString setstringVtable = NULL;
	OV_VTBLPTR_msgsys_MsgComponent thisVtable = NULL;
	OV_INSTPTR_msgsys_MsgComponent this = Ov_StaticPtrCast(msgsys_MsgComponent, pobj);

	setstring = (OV_INSTPTR_ksapi_setString)ov_path_getobjectpointer(SENDINGINSTANCEPATH,2);;
	Ov_GetVTablePtr(ksapi_setString, setstringVtable, setstring);
	Ov_GetVTablePtr(msgsys_MsgComponent, thisVtable, this);
	ov_logfile_error("MsgComp/constructor: setstringVtable");
	setstringVtable->m_registerMethod((OV_INSTPTR_ksapi_KSCommon)setstring,thisVtable->m_retMethod);
	ov_logfile_error("MsgComp/constructor: after setstringVtable");

}

OV_DLLFNCEXPORT void msgsys_MsgComponent_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
    /*    
    *   local variables
    */
	OV_INSTPTR_msgsys_MsgComponent this = Ov_StaticPtrCast(msgsys_MsgComponent, pfb);
	OV_INSTPTR_ksapi_setString sendingInstance = NULL;
	OV_INSTPTR_msgsys_Message msg = NULL;
	OV_STRING value = NULL;
	OV_STRING receiverAddress = NULL;
	OV_STRING receiverName = NULL;

	msg = Ov_GetFirstChild(msgsys_AssertionMsgSys2CurrentMessage, this);
	if(msg){ //Currently we are processing a message, lets see how far we are...
		if(msg->v_msgStatus == MSGDONE){
			Ov_Unlink(msgsys_AssertionMsgSys2CurrentMessage, this, msg);
			Ov_Unlink(msgsys_AssertionMsgSys2Message, this, msg);
			//ov_logfile_debug("MsgComp/typeMethod: CurrentMessage was sent and unlinked from Queue");
		} else if(msg->v_msgStatus == MSGRECEIVERSERVICEERROR){
			ov_logfile_debug("MsgComp/typeMethod: ReceiverService isn't registered, CurrentMessage wasn't sent");			//Pop the message from Queue and push it to the end
			Ov_Unlink(msgsys_AssertionMsgSys2CurrentMessage, this, msg);
			Ov_Unlink(msgsys_AssertionMsgSys2Message, this, msg);
		} else if(msg->v_msgStatus == MSGFATALERROR){
			ov_logfile_debug("MsgComp/typeMethod: An error occured in the sendingprocess, CurrentMessage wasn't sent yet");
			//Pop the message from Queue and push it to the end
			Ov_Unlink(msgsys_AssertionMsgSys2CurrentMessage, this, msg);
			Ov_Unlink(msgsys_AssertionMsgSys2Message, this, msg);
			/*if(!Ov_OK(Ov_Link(msgsys_AssertionMsgSys2Message, this, msg))){
				ov_logfile_error("MsgSys: sendMessage: Couldn't link MessageObject with MessageQueue");
			}*/
		} //else not yet ready
		return;
	} else { // we are NOT currently handling a message - lets see if sth is needs to be done!
		msg = Ov_GetFirstChild(msgsys_AssertionMsgSys2Message, this);
		if(msg){ // todo - do we need a loop here for ignoring sent messages?
					//or are they deleted from the assertion?
			//ov_logfile_debug("MsgComp/typeMethod: No CurrentMessage found, new Message from Queue will proceed");
			//msg = Ov_GetFirstChild(msgsys_AssertionMsgSys2Message, this);
			//Build SendString
			msgsys_Message_serializeMessage(msg);
			//get Values which are important to send the Messages
			//ov_string_setvalue(&value,msgsys_Message_sendString_get(msg));

			//determine if we could locally send message!
			if(ov_string_compare(msg->v_senderAddress, msg->v_receiverAddress) == 0 &&
					ov_string_compare(msg->v_senderName,msg->v_receiverName) == 0) {
				ov_logfile_error("MsgComponent/typeMethod: local delivery of msg to %s/%s!", msg->v_senderAddress, msg->v_senderName);
				msgsys_MsgComponent_retrieveMessage_set(this, msg->v_sendString); //todo we might need to check return value for determining success?
				msg->v_msgStatus = MSGDONE; //this assumes that locally all messages are delivered
				return;
			} else { //send "normal"-remotly
				ov_string_setvalue(&value,msg->v_sendString);
				ov_string_setvalue(&receiverAddress,msgsys_Message_receiverAddress_get(msg));
				ov_string_setvalue(&receiverName,msgsys_Message_receiverName_get(msg));
			}

			//ov_logfile_debug("MsgComp/typeMethod: Link MessageObject with CurrentMessage");
			if(!Ov_OK(Ov_Link(msgsys_AssertionMsgSys2CurrentMessage, this, msg))){
				ov_logfile_error("MsgComp/typeMethod: Couldn't link MessageObject with CurrentMessage");

				//collecting Garbage
				ov_string_setvalue(&value,NULL);
				ov_string_setvalue(&receiverAddress,NULL);
				ov_string_setvalue(&receiverName,NULL);
				return;
			}

			//ov_logfile_debug("MsgComp/typeMethod: Calling ksapi/setandsubmit");
			sendingInstance = (OV_INSTPTR_ksapi_setString)ov_path_getobjectpointer(SENDINGINSTANCEPATH,2);
			ov_logfile_debug("MsgComp/typeMethod: receiverAdress: %s ; receiverName: %s",receiverAddress,receiverName);
			if(sendingInstance)ksapi_setString_setandsubmit(sendingInstance,receiverAddress,receiverName,"/TechUnits/MsgSys.retrieveMessage",value);
			else ov_logfile_error("MsgComp/typeMethod: Couldn't find sendingInstance, no further sending will proceed");
		}
	}

	//Collecting Garbage
	ov_string_setvalue(&receiverAddress,NULL);
	ov_string_setvalue(&receiverName,NULL);
	ov_string_setvalue(&value,NULL);
	return;
}

OV_DLLFNCEXPORT void msgsys_MsgComponent_retMethod(
	OV_INSTPTR_ov_object	pobj,
	OV_STRING	errorstring,
	OV_INT	errorcode
) {
	OV_INSTPTR_msgsys_MsgComponent component = NULL;
	OV_INSTPTR_msgsys_Message msg = NULL;
	OV_INSTPTR_msgsys_ServiceRegistryEntry sobj = NULL;
	OV_STRING service = NULL;

	ov_logfile_debug("MsgComp/retMethod: got answer for %s: %s!",pobj->v_identifier, errorstring);

	component = (OV_INSTPTR_msgsys_MsgComponent)ov_path_getobjectpointer(MSGCOMPONENTROOT, 2);
	if(component){
		msg = (OV_INSTPTR_msgsys_Message)Ov_GetFirstChild(msgsys_AssertionMsgSys2CurrentMessage,component);
		ov_string_setvalue(&service,msgsys_Message_getSenderService(msg->v_sendString));
		if(Ov_Fail(msgsys_MsgComponent_findService((OV_INSTPTR_msgsys_MsgComponent)pobj,&sobj,service))){
			ov_logfile_error("MsgComp/retMethod: Couldn't find the ServiceRegistryEntry - %s!",service);
		} else {
			if(msg->v_returnmethodptr){
				ov_logfile_debug("MsgComp/retMethod: Called with %d/%s for Message: %s with ID: %lu !",errorcode,errorstring,msg->v_identifier,msg->v_msgID);
				ov_logfile_debug("MsgComp/retMethod: Calling now the registered retMethod");
				msg->v_returnmethodptr((OV_INSTPTR_ov_object)pobj,(OV_INSTPTR_ov_object)msg,errorstring,errorcode);
			} else {
				ov_logfile_debug("MsgComp/retMethod: Called with %d/%s for Message: %s with ID: %lu !",errorcode,errorstring,msg->v_identifier,msg->v_msgID);
			}
		}
	}

	if(errorcode == 1)msg->v_msgStatus = MSGDONE;
	else if(errorcode == 8)msg->v_msgStatus = MSGRECEIVERSERVICEERROR;
	else msg->v_msgStatus = MSGFATALERROR;

	//Ov_DelteObject(message);
	//Collecting Garbage
	ov_string_setvalue(&service,NULL);
	return;
}

OV_DLLFNCEXPORT OV_RESULT msgsys_MsgComponent_retrieveMessage_set(
    OV_INSTPTR_msgsys_MsgComponent	pobj,
    const OV_STRING	value
) {
	OV_INSTPTR_msgsys_Message	message = NULL;
	//OV_INSTPTR_msgsys_MessageBox msgBox = NULL;
	OV_INSTPTR_msgsys_ServiceRegistryEntry sobj = NULL;
	OV_INSTPTR_ov_object generalInbox = NULL; //ov_object
	OV_INSTPTR_ov_domain inboxdomain = NULL;
	OV_STRING service = NULL;
	OV_STRING serviceINBOX = NULL;
	OV_RESULT result = 0;
	//prepare registry findservice by extracting target service name

	// Anmerkung Liyong 01.08.2012
	// hier oder in msgsys_Message_getReceiverServic fehlt eine Überprüfung von value. Wenn z.B. "" eingegeben wird, stürtz die Datenbasis ab!
	// Hiermit nur eine provisorische Lösung
	if (ov_string_compare(value, "") == 0)
	{
		ov_logfile_error("MsgComp/retrieveMessage: value invalid");
		return OV_ERR_BADVALUE;
	}
	// End der eine provisorische Lösung

	ov_string_setvalue(&service,msgsys_Message_getReceiverService(value));
	//prepare OVPath/INBOX-workaround
	ov_string_print(&serviceINBOX, "%s/INBOX",service);
	generalInbox = ov_path_getobjectpointer(serviceINBOX,2);

	//check if registry knows service

	result = msgsys_MsgComponent_findService(pobj,&sobj,service);


	if(Ov_OK(result)){
		inboxdomain = (OV_INSTPTR_ov_domain)ov_path_getobjectpointer(sobj->v_inboxPath, 2);

	} else if (generalInbox != NULL) { //lookup service as OVPath appending INBOX.
		inboxdomain = (OV_INSTPTR_ov_domain)generalInbox;
	} else {
		ov_logfile_error("MsgComp/retrieveMessage: Coudn't find Service nor OVPath/INBOX'%s'! - Cant deliver Message - Fatal Error"); //,sobj->v_inboxPath
		return OV_ERR_GENERIC;
	}
	ov_logfile_debug("MsgComp/retrieveMessage: Creating retrieved MessageObject!");
	msgsys_CreateAnonymousMessage(msgsys_Message,message,inboxdomain,"Message",&result);
	if(!result){
		ov_logfile_error("MsgComp/retrieveMessage: Couldn't create Object 'Message'");
		return OV_ERR_GENERIC;
	}
	ov_logfile_debug("MsgComp/retrieveMessage: Creating done.");
	ov_string_setvalue(&message->v_sendString,value);
	msgsys_Message_deserializeMessage(message);
	message->v_msgStatus = MSGDONE;

	//Collecting Garbage
	ov_string_setvalue(&service,NULL);
	ov_string_setvalue(&serviceINBOX,NULL);

	if(sobj && sobj->v_arrivalmethodptr){//registry match
		sobj->v_arrivalmethodptr((OV_INSTPTR_ov_object)pobj,(OV_INSTPTR_ov_object)message);
	} else if (Ov_CanCastTo(msgsys_MessageBox , generalInbox)) { //INBOX Match
		msgsys_MessageBox_arrivalMethod((OV_INSTPTR_msgsys_MessageBox)generalInbox, message);
	} else {
		ov_logfile_debug("MsgComp/retrieveMessage: No ArrivalMethode found!");
	}

	return OV_ERR_OK;
}

OV_DLLFNCEXPORT void msgsys_MsgComponent_sendMessage(
	OV_INSTPTR_msgsys_MsgComponent         component,
	OV_INSTPTR_msgsys_Message          message,
	OV_FNCPTR_MSGSYS_RET_FNC	pfnc 
) {
	if (message && pfnc){
		message->v_returnmethodptr = pfnc;
	} else {
		ov_logfile_error("MsgComp/sendMessage: register returnMethod failed(this could be normal, so IGNORE this); missing object or function pointer");
	}

	if(!Ov_OK(Ov_Link(msgsys_AssertionMsgSys2Message, component, message))){
		ov_logfile_error("MsgComp/sendMessage: Couldn't link MessageObject with MessageQueue");
	} else {
		message->v_msgStatus = MSGWAITING;
		ov_logfile_debug("MsgComp/sendMessage: MessageObject successfully linked with MessageQueue");
	}

    return;	
}

OV_DLLFNCEXPORT void msgsys_MsgComponent_registerService(
	OV_INSTPTR_msgsys_MsgComponent	pobj,
	const OV_STRING	serviceName,
	const OV_STRING	inboxPath,
	OV_FNCPTR_MSGSYS_ARRIVAL_FNC	pfnc 
) {
	OV_INSTPTR_msgsys_ServiceRegistryEntry	service = NULL;
	OV_INSTPTR_ov_domain servicedomain = NULL;

	servicedomain = (OV_INSTPTR_ov_domain)ov_path_getobjectpointer(SERVICEROOT, 2);
	if(!servicedomain){
		ov_logfile_error("MsgComp/registerService: Coudn't find Service-Domain '%s'! - Fatal Error",SERVICEROOT);
		return;
	}

	if(Ov_OK(msgsys_MsgComponent_findService(pobj,&service,serviceName))){
		ov_logfile_debug("MsgComp/registerService: No %s - Service found, go on with register!",serviceName);
		return;
	}
	else ov_logfile_debug("MsgComp/registerService: Service - %s already exists!",serviceName);

	ov_logfile_debug("MsgComp/registerService: Creating ServiceRegistryEntry '%s'",serviceName);
	if(!Ov_OK((Ov_CreateObject(msgsys_ServiceRegistryEntry, service, servicedomain, serviceName)))){
		ov_logfile_error("MsgComp/registerService: Couldn't create Object '%s'",serviceName);
	} else {
		ov_logfile_debug("MsgComp/registerService: Creating done.");
	}

	if (service && pfnc){
		service->v_arrivalmethodptr = pfnc;
	} else {
		ov_logfile_error("MsgComp/registerService: register arrivalMethod failed; missing object or function pointer");
	}

	ov_string_setvalue(&service->v_inboxPath,inboxPath);
	return;

}

OV_DLLFNCEXPORT void msgsys_MsgComponent_deregisterService(
	OV_INSTPTR_msgsys_MsgComponent	pobj,
	const OV_STRING	serviceName
) {
	OV_INSTPTR_msgsys_ServiceRegistryEntry	sobj = NULL;
	OV_INSTPTR_ov_domain servicedomain = NULL;

	servicedomain = (OV_INSTPTR_ov_domain)ov_path_getobjectpointer(SERVICEROOT, 2);
	if(!servicedomain){
		ov_logfile_error("MsgComp/deregisterService: Coudnt find Service-Domain '%s'! - Fatal Error",SERVICEROOT);
		return;
	}

	if(Ov_OK(msgsys_MsgComponent_findService(pobj,&sobj,serviceName))){
		Ov_DeleteObject(sobj);
	}
	else{
		ov_logfile_error("MsgComp/deregisterService: Couldn't find the ServiceRegistryEntry - %s!",serviceName);
	}
	
	return;

}
/**
 * Locate a service within the registration
 */
OV_DLLVAREXPORT OV_RESULT msgsys_MsgComponent_findService(
		OV_INSTPTR_msgsys_MsgComponent pobj,
		OV_INSTPTR_msgsys_ServiceRegistryEntry *sobj,
		const OV_STRING service
) {
	OV_INSTPTR_msgsys_ServiceRegistry serviceparent = NULL;
	OV_INSTPTR_msgsys_ServiceRegistryEntry servicechild = NULL;


	//lookup registered services
	serviceparent = (OV_INSTPTR_msgsys_ServiceRegistry)ov_path_getobjectpointer(SERVICEROOT,2);
	if(serviceparent){
		servicechild = (OV_INSTPTR_msgsys_ServiceRegistryEntry)Ov_GetFirstChild(ov_containment,serviceparent);

		if(!servicechild){
			return OV_ERR_GENERIC;
		}

		while(servicechild)
		{

			if(!ov_string_compare(servicechild->v_identifier,service)){
				*sobj = servicechild;

				return OV_ERR_OK;
			}
			else servicechild = (OV_INSTPTR_msgsys_ServiceRegistryEntry)Ov_GetNextChild(ov_containment,servicechild);
		}
	}

	return OV_ERR_GENERIC;
}
