#ifndef OV_COMPILE_LIBRARY_MessageSys
#define OV_COMPILE_LIBRARY_MessageSys
#endif


#include "MessageSys.h"
#include "libov/ov_macros.h"
#include "libov/ov_logfile.h"
#include "libov/ov_path.h"
#include "libov/ov_time.h"
#include "libov/ov_result.h"
#include "MessageSys_helpers.h"


OV_DLLFNCEXPORT OV_RESULT MessageSys_MsgDelivery_constructor(
		OV_INSTPTR_ov_object 	pobj
) {
	/*
	 *   local variables
	 */
	OV_INSTPTR_MessageSys_MsgDelivery this = Ov_StaticPtrCast(MessageSys_MsgDelivery, pobj);
	OV_RESULT result = result = ksserv_ComTask_constructor(pobj);
	OV_INSTPTR_ksapi_setString setstring =  NULL;
	OV_VTBLPTR_ksapi_setString setstringVtable = NULL;
	OV_VTBLPTR_MessageSys_MsgDelivery thisVtable = NULL;

	//OV_INSTPTR_ov_domain 			domain = NULL;

	/* do what the base class does first */
	if(Ov_Fail(result))return result;

	this->v_actimode = TRUE;
	this->v_cycInterval = 1;

	/* do what */

	//initialize setString
	if(setstring){
		ksapi_KSCommon_deregisterMethod((OV_INSTPTR_ksapi_KSCommon)setstring);
	}

	if (Ov_Fail(Ov_CreateObject(ksapi_setString,setstring,this, "sendingInstance"))){
		ov_logfile_error("MessageDelivery/constructor: Error while creating the setString/sendingInstance!");
		return OV_ERR_GENERIC;
	}

	//set up return method -- TODO Check: Why is this required? Startup is NOT enough!!!
	Ov_GetVTablePtr(ksapi_setString, setstringVtable, setstring);
	Ov_GetVTablePtr(MessageSys_MsgDelivery, thisVtable, this);
	setstringVtable->m_registerMethod((OV_INSTPTR_ksapi_KSCommon)setstring,thisVtable->m_retMethod);

	return OV_ERR_OK;
}

OV_DLLFNCEXPORT void MessageSys_MsgDelivery_startup(

		OV_INSTPTR_ov_object 	pobj

) {

	OV_INSTPTR_ksapi_setString setstring =  NULL;
	OV_VTBLPTR_ksapi_setString setstringVtable = NULL;
	OV_VTBLPTR_MessageSys_MsgDelivery thisVtable = NULL;
	OV_INSTPTR_MessageSys_MsgDelivery this = Ov_StaticPtrCast(MessageSys_MsgDelivery, pobj);
	OV_STRING tmpPath = NULL;

	ov_string_print(&tmpPath, "%s",SENDINGINSTANCE);
	setstring = (OV_INSTPTR_ksapi_setString)ov_path_getobjectpointer(tmpPath,2);
	Ov_GetVTablePtr(ksapi_setString, setstringVtable, setstring);
	Ov_GetVTablePtr(MessageSys_MsgDelivery, thisVtable, this);
	setstringVtable->m_registerMethod((OV_INSTPTR_ksapi_KSCommon)setstring,thisVtable->m_retMethod);

	ov_string_setvalue(&tmpPath, NULL);


}

OV_DLLFNCEXPORT void MessageSys_MsgDelivery_typemethod(
		 OV_INSTPTR_ksserv_ComTask       pfb
) {
	/*
	 *   local variables
	 */
	OV_INSTPTR_MessageSys_MsgDelivery this = Ov_StaticPtrCast(MessageSys_MsgDelivery, pfb);
	OV_INSTPTR_ksapi_setString sendingInstance = NULL;
	OV_INSTPTR_MessageSys_Message msg = NULL;
	OV_STRING value = NULL;
	OV_STRING receiverAddress = NULL;
	OV_STRING receiverName = NULL;
	OV_STRING tmpPath = NULL;
	unsigned usedWithMessage = 0;

	msg = Ov_GetChild(MessageSys_MsgDelivery2CurrentMessage, this);
	if(msg){ //Currently we are processing a message, lets see how far we are...
		if(msg->v_msgStatus == MSGDONE){
			Ov_Unlink(MessageSys_MsgDelivery2CurrentMessage, this, msg);
			Ov_Unlink(MessageSys_MsgDelivery2Message, this, msg);
			usedWithMessage = ov_database_getused();
			Ov_DeleteObject(msg);
			//ov_logfile_debug("FREED BY DELETING ANSWER: %u", usedWithMessage - ov_database_getused());
		} else if(msg->v_msgStatus == MSGRECEIVERSERVICEERROR){
			ov_logfile_debug("MessageDelivery/typeMethod: ReceiverService isn't registered, CurrentMessage wasn't sent");			//Pop the message from Queue and push it to the end
			Ov_Unlink(MessageSys_MsgDelivery2CurrentMessage, this, msg);
			Ov_Unlink(MessageSys_MsgDelivery2Message, this, msg);
			Ov_DeleteObject(msg);
		} else if(msg->v_msgStatus == MSGFATALERROR){
			ov_logfile_debug("MessageDelivery/typeMethod: An error occured in the sendingprocess, CurrentMessage wasn't sent yet");
			//Pop the message from Queue and push it to the end
			Ov_Unlink(MessageSys_MsgDelivery2CurrentMessage, this, msg);
			Ov_Unlink(MessageSys_MsgDelivery2Message, this, msg);
			Ov_DeleteObject(msg);
			/*if(!Ov_OK(Ov_Link(MessageSys_MsgDelivery2Message, this, msg))){
				ov_logfile_error("ServiceProvider: sendMessage: Couldn't link MessageObject with MessageQueue");
			}*/
		} //else not yet ready
		//Collecting Garbage
		ov_string_setvalue(&tmpPath,NULL);
		ov_string_setvalue(&receiverAddress,NULL);
		ov_string_setvalue(&receiverName,NULL);
		ov_string_setvalue(&value,NULL);
		return;
	} else { // we are NOT currently handling a message - lets see if sth is needs to be done!
		msg = Ov_GetFirstChild(MessageSys_MsgDelivery2Message, this);
		if(msg){ // todo - do we need a loop here for ignoring sent messages?
			//or are they deleted from the assertion?
			////ov_logfile_debug("MessageDelivery/typeMethod: No CurrentMessage found, new Message from Queue will proceed");
			//msg = Ov_GetFirstChild(MessageSys_MsgDelivery2Message, this);
			//Build SendString
			//ov_logfile_debug("USED MEMORY BEFORE SERIALIZE == %u", ov_database_getused());
			MessageSys_Message_serializeMessage(msg);
			//ov_logfile_debug("USED MEMORY AFTER SERIALIZE == %u", ov_database_getused());
			//get Values which are important to send the Messages
			//ov_string_setvalue(&value,MessageSys_Message_sendString_get(msg));

			//determine if we could locally send message!
			if(ov_string_compare(msg->v_senderAddress, msg->v_receiverAddress) == 0 &&
					ov_string_compare(msg->v_senderName,msg->v_receiverName) == 0) {
				ov_logfile_error("MsgDelivery/typeMethod: local delivery of msg to %s/%s!", msg->v_senderAddress, msg->v_senderName);
				//ov_logfile_debug("USED MEMORY BEFORE RETRIEVEMSG == %u", ov_database_getused());
				MessageSys_MsgDelivery_retrieveMessage_set(this, msg->v_sendString); //todo we might need to check return value for determining success?
				//ov_logfile_debug("USED MEMORY AFTER RETRIEVEMSG == %u", ov_database_getused());
				msg->v_msgStatus = MSGDONE; //this assumes that locally all messages are delivered
				//Collecting Garbage
				ov_string_setvalue(&tmpPath,NULL);
				ov_string_setvalue(&receiverAddress,NULL);
				ov_string_setvalue(&receiverName,NULL);
				ov_string_setvalue(&value,NULL);
				//ov_logfile_debug("USED MEMORY AFTER MSGCOMPTYPEMETHOD == %u", ov_database_getused());
				return;
			} else { //send "normal"-remotly
				ov_string_setvalue(&value,msg->v_sendString);
				ov_string_setvalue(&receiverAddress,MessageSys_Message_receiverAddress_get(msg));
				ov_string_setvalue(&receiverName,MessageSys_Message_receiverName_get(msg));
			}

			////ov_logfile_debug("MessageDelivery/typeMethod: Link MessageObject with CurrentMessage");
			if(!Ov_OK(Ov_Link(MessageSys_MsgDelivery2CurrentMessage, this, msg))){
				ov_logfile_error("MessageDelivery/typeMethod: Couldn't link MessageObject with CurrentMessage");

				//Collecting Garbage
				ov_string_setvalue(&tmpPath,NULL);
				ov_string_setvalue(&receiverAddress,NULL);
				ov_string_setvalue(&receiverName,NULL);
				ov_string_setvalue(&value,NULL);
				//ov_logfile_debug("USED MEMORY AFTER MSGCOMPTYPEMETHOD == %u", ov_database_getused());
				return;
			}

			////ov_logfile_debug("MessageDelivery/typeMethod: Calling ksapi/setandsubmit");
			ov_string_print(&tmpPath, "%s", SENDINGINSTANCE);
			sendingInstance = (OV_INSTPTR_ksapi_setString)ov_path_getobjectpointer(tmpPath,2);
			if(sendingInstance)
			{
				//ov_logfile_debug("USED MEMORY BEFORE KSAPI == %u", ov_database_getused());
				ksapi_setString_setandsubmit(sendingInstance,receiverAddress,receiverName,"/communication/MessageSys.retrieveMessage",value);
				//ov_logfile_debug("USED MEMORY AFTER KSAPI == %u", ov_database_getused());
			}
			else
			{
				ov_logfile_error("MessageDelivery/typeMethod: Couldn't find sendingInstance, no further sending will proceed");
			}
		}
	}

	//Collecting Garbage
	ov_string_setvalue(&tmpPath,NULL);
	ov_string_setvalue(&receiverAddress,NULL);
	ov_string_setvalue(&receiverName,NULL);
	ov_string_setvalue(&value,NULL);

	//ov_logfile_debug("USED MEMORY AFTER MSGCOMPTYPEMETHOD == %u", ov_database_getused());
	return;
}

OV_DLLFNCEXPORT void MessageSys_MsgDelivery_retMethod(
		OV_INSTPTR_ov_object	pobj,
		OV_STRING	errorstring,
		OV_INT	errorcode
) {
	OV_INSTPTR_MessageSys_MsgDelivery component = NULL;
	OV_INSTPTR_MessageSys_Message msg = NULL;
	OV_INSTPTR_ov_object sobj = NULL;
	OV_STRING service = NULL;
	OV_STRING tmpPath = NULL;

	ov_string_print(&tmpPath, "%s",MSGDELIVERY);
	component = (OV_INSTPTR_MessageSys_MsgDelivery)ov_path_getobjectpointer(tmpPath, 2);
	if(component){
		msg = (OV_INSTPTR_MessageSys_Message)Ov_GetFirstChild(MessageSys_MsgDelivery2CurrentMessage,component);
		ov_memstack_lock();
		service = MessageSys_Message_getSenderComponent(msg->v_sendString);
		//new findService function called here
		if(Ov_Fail(MessageSys_MsgDelivery_findService(&sobj, service))){
			//Ignore this for now for that the message will be sent by the client, there is no need to check this here.
			//ov_logfile_error("MessageDelivery/retMethod: Couldn't find the ServiceRegistryEntry - %s!",service);
		} else {
			if(msg->v_returnmethodptr){
				msg->v_returnmethodptr(pobj,(OV_INSTPTR_ov_object)msg,errorstring,errorcode);
			}
		}
		ov_memstack_unlock();
	}

	if(errorcode == 1)msg->v_msgStatus = MSGDONE;
	else if(errorcode == 8)msg->v_msgStatus = MSGRECEIVERSERVICEERROR;
	else msg->v_msgStatus = MSGFATALERROR;

	//Ov_DeleteObject(msg); //uncommenting this frees memory faster
	//Collecting Garbage
	ov_string_setvalue(&tmpPath, NULL);
	return;
}

OV_DLLFNCEXPORT OV_RESULT MessageSys_MsgDelivery_retrieveMessage_set(
		OV_INSTPTR_MessageSys_MsgDelivery	pobj,
		const OV_STRING	value
) {
	OV_INSTPTR_MessageSys_Message	message = NULL;
	OV_INSTPTR_ov_object sobj = NULL;
	OV_INSTPTR_ov_domain sDom = NULL;
	OV_INSTPTR_ov_domain inboxdomain = NULL;
	OV_STRING service = NULL;
	char inboxName[] = "INBOX";
	OV_UINT inboxNameLength = sizeof(inboxName);
	OV_UINT identifier_length = 0;
	OV_RESULT result = 0;
	//prepare registry findservice by extracting target service name

	ov_memstack_lock();
	service = MessageSys_Message_getReceiverComponent(value);

	//New findService called here
	result = MessageSys_MsgDelivery_findService(&sobj, service);

	if(result == OV_ERR_OK)
	{
		//Get pointer to service and then append "/Inbox" to find the inbox, which is a domain
		sDom = Ov_DynamicPtrCast(ov_domain, sobj);
		if(!sDom)
		{	/*	object is no Domain --> it cannot be an inbox	*/
			ov_logfile_error("%s: could not deliver message: receiving object is no domain (cannot have inbox)", pobj->v_identifier);
			ov_memstack_unlock();
			return OV_ERR_GENERIC;
		}
		Ov_ForEachChildEx(ov_containment, sDom, inboxdomain, ov_domain)
		{
			identifier_length = strlen(inboxdomain->v_identifier)+1;
			if(identifier_length != inboxNameLength)	/*	identifiers length differs from "INBOX" --> this must be the wrong one	*/
				continue;
			else
			{
				if(ov_string_compare(inboxName, ov_string_toupper(inboxdomain->v_identifier)) == OV_STRCMP_EQUAL)
				{	/*	inbox found	*/
					break;
				}
			}
		}
		if (!inboxdomain){
			ov_memstack_unlock();
			return OV_ERR_GENERIC;
		}
	}
	else
	{
		ov_logfile_error("MessageDelivery/retrieveMessage: Couldn't find Service! - Cant deliver Message - Fatal Error"); //,sobj->v_inboxPath
		ov_memstack_unlock();
		return OV_ERR_GENERIC;
	}
	result = MessageSys_createAnonymousMessage(inboxdomain, "Message", (OV_INSTPTR_ov_object*)(&message));
	if(Ov_Fail(result)){
		ov_logfile_error("MessageDelivery/retrieveMessage: Couldn't create Object 'Message'");
		ov_memstack_unlock();
		return OV_ERR_GENERIC;
	}
	ov_string_setvalue(&message->v_sendString,value);
	MessageSys_Message_deserializeMessage(message);
	message->v_msgStatus = MSGDONE;

	//Collecting Garbage
	ov_memstack_unlock();
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_BOOL MessageSys_MsgDelivery_sendMessage(
		OV_INSTPTR_MessageSys_MsgDelivery         component,
		OV_INSTPTR_MessageSys_Message          message,
		OV_FNCPTR_MSGSYS_RET_FNC	pfnc
) {
	if(message->v_msgStatus == MSGREADYFORSENDING){

		if (message && pfnc){
			message->v_returnmethodptr = pfnc;
		}

		else {
			// DK: Commented out the most useless error message I have seen so far. Simply too unique to deserve deletion.
			//ov_logfile_error("MessageDelivery/sendMessage: register returnMethod failed(this could be normal, so IGNORE this); missing object or function pointer");
		}

		if(!Ov_OK(Ov_Link(MessageSys_MsgDelivery2Message, component, message))){
			ov_logfile_error("MessageDelivery/sendMessage: Couldn't link MessageObject with MessageQueue");
			return FALSE;
		} else {
			message->v_msgStatus = MSGWAITING;
			return TRUE;
		}
	}
	return FALSE;
}

OV_DLLVAREXPORT OV_RESULT MessageSys_MsgDelivery_findService(
		OV_INSTPTR_ov_object *sobj,
		const OV_STRING service
) {
	OV_INSTPTR_ov_object pService = NULL;
	OV_INSTPTR_ov_domain domain = NULL;
	OV_ELEMENT serviceElem;
	OV_ELEMENT elemID;

	/*	check if service contains a path (begins with '/')	*/
	if(*service == '/')
	{
		*sobj = ov_path_getobjectpointer(service, 2);
		if(*sobj)
			return OV_ERR_OK;
		else
			return OV_ERR_GENERIC;
	}
	else
	{	/*	no path --> check IDs of registered services	*/
		//Get Path to regServices Folder
		domain = (OV_INSTPTR_ov_domain)ov_path_getobjectpointer(REGISTEREDPATH, 2);
		if(!domain)
		{/*	no registered services domain --> no services registered	*/
			return OV_ERR_GENERIC;
		}
		if(service)
		{
			//Find Service to which the Operation shall be registered
			Ov_ForEachChild(ov_containment, domain, pService)
			{
				/*	check for ID variable and if it exists read it out	*/
				serviceElem.elemtype = OV_ET_OBJECT;
				serviceElem.pobj = Ov_StaticPtrCast(ov_object, pService);

				if(Ov_OK(ov_element_searchpart(&serviceElem, &elemID, OV_ET_VARIABLE, "ID")))
				{
					if(elemID.elemtype == OV_ET_VARIABLE)
					{
						if(elemID.pvalue)
						{
							if(ov_string_compare(service, *((OV_STRING*) elemID.pvalue)) == OV_STRCMP_EQUAL)
							{
								*sobj = pService;
								return OV_ERR_OK;
							}
						}
					}
				}
			}
		}
		return OV_ERR_GENERIC;
	}
}



