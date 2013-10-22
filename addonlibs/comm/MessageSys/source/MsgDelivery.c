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
#include "ksapi_commonFuncs.h"
#include "acplt_simpleMsgHandling.h"


OV_DLLFNCEXPORT OV_RESULT MessageSys_MsgDelivery_constructor(
		OV_INSTPTR_ov_object 	pobj
) {
	/*
	 *   local variables
	 */
	OV_INSTPTR_MessageSys_MsgDelivery this = Ov_StaticPtrCast(MessageSys_MsgDelivery, pobj);
	OV_RESULT result;
	OV_INSTPTR_ksapi_setVar setVar =  NULL;
	OV_VTBLPTR_ksapi_setVar setVarVtable = NULL;
	OV_VTBLPTR_MessageSys_MsgDelivery thisVtable = NULL;

	//OV_INSTPTR_ov_domain 			domain = NULL;

	/* do what the base class does first */
	result = ksbase_ComTask_constructor(pobj);
	if(Ov_Fail(result))
		return result;

	this->v_actimode = TRUE;
	this->v_cycInterval = 1;

	/* do what */


	if (Ov_Fail(Ov_CreateObject(ksapi_setVar,setVar,this, "sendingInstance"))){
		ov_logfile_error("MessageDelivery/constructor: Error while creating the setString/sendingInstance!");
		return OV_ERR_GENERIC;
	}

	//set up return method -- TODO Check: Why is this required? Startup is NOT enough!!!
	Ov_GetVTablePtr(ksapi_setVar, setVarVtable, setVar);
	Ov_GetVTablePtr(MessageSys_MsgDelivery, thisVtable, this);

	return OV_ERR_OK;
}

OV_DLLFNCEXPORT void MessageSys_MsgDelivery_startup(

		OV_INSTPTR_ov_object 	pobj

) {

	OV_INSTPTR_ksapi_setVar setVar =  NULL;
	OV_VTBLPTR_ksapi_setVar setVarVtable = NULL;
	OV_VTBLPTR_MessageSys_MsgDelivery thisVtable = NULL;
	OV_INSTPTR_MessageSys_MsgDelivery this = Ov_StaticPtrCast(MessageSys_MsgDelivery, pobj);
	OV_STRING tmpPath = NULL;

	ov_string_print(&tmpPath, "%s",SENDINGINSTANCE);
	setVar = (OV_INSTPTR_ksapi_setVar)ov_path_getobjectpointer(tmpPath,2);
	Ov_GetVTablePtr(ksapi_setVar, setVarVtable, setVar);
	Ov_GetVTablePtr(MessageSys_MsgDelivery, thisVtable, this);

	ov_string_setvalue(&tmpPath, NULL);


}

OV_DLLFNCEXPORT void MessageSys_MsgDelivery_typemethod(
		 OV_INSTPTR_ksbase_ComTask       pfb
) {
	/*
	 *   local variables
	 */
	OV_INSTPTR_MessageSys_MsgDelivery this = Ov_StaticPtrCast(MessageSys_MsgDelivery, pfb);
	OV_INSTPTR_ksapi_setVar sendingInstance = NULL;
	OV_INSTPTR_MessageSys_Message msg = NULL;
	OV_ANY value;
	OV_STRING msgString = NULL;
	OV_STRING headerString = NULL;
	ACPLT_MSGHEADER msgHeader;
	OV_TIME tTemp = {0, 0};
	OV_RESULT result;

	msg = Ov_GetChild(MessageSys_MsgDelivery2CurrentMessage, this);
	if(msg){ //Currently we are processing a message, lets see how far we are...
		if((msg->v_msgStatus != MSGDONE) && (msg->v_msgStatus != MSGRECEIVERERROR)
				&& (msg->v_msgStatus != MSGFATALERROR))
		{	/*	still sending / waiting for answer of ks-system	*/

			sendingInstance = (OV_INSTPTR_ksapi_setVar)ov_path_getobjectpointer(SENDINGINSTANCE,2);
			if(sendingInstance->v_status == KSAPI_COMMON_REQUESTCOMPLETED)
			{
				Ov_Unlink(MessageSys_MsgDelivery2CurrentMessage, this, msg);
				Ov_Unlink(MessageSys_MsgDelivery2Message, this, msg);
				Ov_DeleteObject(msg);
			}
			else if(sendingInstance->v_status == KSAPI_COMMON_INTERNALERROR)
			{
				ov_logfile_debug("MessageDelivery/typeMethod: An error occured in the sendingprocess, CurrentMessage wasn't sent");
				//Pop the message from Queue and push it to the end
				Ov_Unlink(MessageSys_MsgDelivery2CurrentMessage, this, msg);
				Ov_Unlink(MessageSys_MsgDelivery2Message, this, msg);
				Ov_DeleteObject(msg);
			}
			else if(sendingInstance->v_status ==KSAPI_COMMON_EXTERNALERROR)
			{
				ov_logfile_debug("MessageDelivery/typeMethod: Error between ksClient-Object and receiver-side, CurrentMessage wasn't delivered");
				Ov_Unlink(MessageSys_MsgDelivery2CurrentMessage, this, msg);
				Ov_Unlink(MessageSys_MsgDelivery2Message, this, msg);
				Ov_DeleteObject(msg);
			}
			else if(sendingInstance->v_status == KSAPI_COMMON_WAITINGFORANSWER)
			{
				ov_time_gettime(&tTemp);
				if(tTemp.secs > (this->v_sendTime.secs + this->v_sendTimeOut))
				{
					ov_logfile_debug("MessageDelivery/typeMethod: Timeout after sending Message, CurrentMessage probably wasn't delivered");
					Ov_Unlink(MessageSys_MsgDelivery2CurrentMessage, this, msg);
					Ov_Unlink(MessageSys_MsgDelivery2Message, this, msg);
					Ov_DeleteObject(msg);
				}
			}

		}
		return;
	}
	else
	{ // we are NOT currently handling a message - lets see if sth is needs to be done!
		value.value.vartype = OV_VT_VOID;	/*	initialize value	*/
		value.value.valueunion.val_string = NULL;
		msg = Ov_GetFirstChild(MessageSys_MsgDelivery2Message, this);
		if(msg)
		{
			result = Ov_Link(MessageSys_MsgDelivery2CurrentMessage, this, msg);
			if(Ov_Fail(result)){
				ov_memstack_lock();
				ov_logfile_error("MessageDelivery/typeMethod: Couldn't link MessageObject with CurrentMessage. Reason: %s", ov_result_getresulttext(result));
				ov_memstack_unlock();
				//Collecting Garbage
				return;
			}

			/*	build message string	*/
			/*	build header	*/
			ov_memstack_lock();
			acplt_simpleMsg_initHeader(&msgHeader);
			if(!msg->v_receiverAddress || !msg->v_receiverName)
			{
				ov_logfile_error("MessageDelivery/typeMethod: receiverAddress or ReceiverName of message %s not set", msg->v_identifier);
				msg->v_msgStatus = MSGFATALERROR;
				ov_memstack_unlock();
				return;
			}
			result = ov_string_print(&(msgHeader.rcvSysAdr), "%s:%s", msg->v_receiverAddress, msg->v_receiverName);
			if(Ov_Fail(result))
			{
				ov_logfile_error("MessageDelivery/typeMethod: Couldn't build rcvSysAdr for message %s", msg->v_identifier);
				msg->v_msgStatus = MSGFATALERROR;
				ov_memstack_unlock();
				return;
			}
			msgHeader.rcvLocAdr = msg->v_receiverComponent;
			if(msg->v_senderAddress && msg->v_senderName)
			{
				result = ov_string_print(&(msgHeader.sndSysAdr), "%s:%s", msg->v_senderAddress, msg->v_senderName);
				if(Ov_Fail(result))
				{
					ov_logfile_error("MessageDelivery/typeMethod: Couldn't build sndSysAdr for message %s", msg->v_identifier);
					msg->v_msgStatus = MSGFATALERROR;
					ov_string_setvalue(&(msgHeader.rcvSysAdr), NULL);
					ov_memstack_unlock();
					return;
				}
			}
			msgHeader.sndLocAdr = msg->v_senderComponent;
			msgHeader.msgId = msg->v_msgID;
			msgHeader.refMsgId = msg->v_refMsgID;
			msgHeader.auth = msg->v_auth;
			headerString = acplt_simpleMsg_generateMsgHeader(&msgHeader);
			ov_string_setvalue(&(msgHeader.rcvSysAdr), NULL);
			ov_string_setvalue(&(msgHeader.sndSysAdr), NULL);
			if(!headerString)
			{
				ov_logfile_error("MessageDelivery/typeMethod: Couldn't build header for message %s", msg->v_identifier);
				msg->v_msgStatus = MSGFATALERROR;
				ov_memstack_unlock();
				return;
			}

			/*	build message string (concatenate header and body and set <msg> </msg> tags around it	*/
			if(!msg->v_msgBody)
			{
				ov_logfile_error("MessageDelivery/typeMethod: Couldn't no body for message %s", msg->v_identifier);
				msg->v_msgStatus = MSGFATALERROR;
				ov_memstack_unlock();
				return;
			}
			result = ov_string_print(&msgString, "<msg>%s%s</msg>", headerString, msg->v_msgBody);
			/*	unlock memstack, we don't need it anymore	*/
			ov_memstack_unlock();
			if(Ov_Fail(result))
			{
				ov_logfile_error("MessageDelivery/typeMethod: Couldn't build message %s", msg->v_identifier);
				msg->v_msgStatus = MSGFATALERROR;
				ov_memstack_unlock();
				return;
			}

			//send
			value.value.vartype = OV_VT_STRING;
			value.value.valueunion.val_string = msgString;

			sendingInstance = (OV_INSTPTR_ksapi_setVar)ov_path_getobjectpointer(SENDINGINSTANCE,2);
			if(sendingInstance)
			{
				ksapi_setVar_setandsubmit(sendingInstance, MessageSys_Message_receiverAddress_get(msg),MessageSys_Message_receiverName_get(msg),"/communication/MessageSys.retrieveMessage", value);
				ov_time_gettime(&(this->v_sendTime));
			}
			else
				ov_logfile_error("MessageDelivery/typeMethod: Couldn't find sendingInstance, no further sending will proceed");
			ov_string_setvalue(&msgString, NULL);
		}
	}
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
	OV_ELEMENT parentObject, part;
	ACPLT_MSGHEADER msgHeader;
	OV_STRING tempSrvName = NULL;
	OV_STRING tempBody = NULL;
	OV_STRING msgEnd = NULL;

	ov_memstack_lock();

	/*	determine start and end of msg	*/
	result = acplt_simpleMsg_xml_findElementBegin(value, "msg", &tempBody);
	if(Ov_Fail(result))
	{
		ov_logfile_debug("MessageDelivery/retrieveMessage: parsing message failed. Reason: %s", ov_result_getresulttext(result));
		ov_memstack_unlock();
		return OV_ERR_OK;
	}
	if(!tempBody)
	{
		ov_logfile_debug("MessageDelivery/retrieveMessage: no msg element");
		ov_memstack_unlock();
		return OV_ERR_OK;
	}

	/*	extract message header	*/
	acplt_simpleMsg_initHeader(&msgHeader);
	result = acplt_simpleMsg_parseMessageHeader(value, &msgHeader);
	if(Ov_Fail(result))
	{
		ov_logfile_debug("MessageDelivery/retrieveMessage: parsing message header failed. Reason: %s", ov_result_getresulttext(result));
		ov_memstack_unlock();
		return OV_ERR_OK;
	}


	service = msgHeader.rcvLocAdr;

	//New findService called here
	result = MessageSys_MsgDelivery_findService(&sobj, service);

	if(Ov_OK(result))
	{
		//Get pointer to service and then append "/Inbox" to find the inbox, which is a domain
		sDom = Ov_DynamicPtrCast(ov_domain, sobj);
		if(sDom)
		{
			/*	iterate over all children in containment to find inbox	*/
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
		}
		if (!inboxdomain){
			/*	inbox not found, maybe it's a part...	*/
			parentObject.pobj = sobj;
			parentObject.elemtype = OV_ET_OBJECT;
			part.elemtype = OV_ET_NONE;

			/*	iterate over all parts	*/
			ov_element_getnextpart(&parentObject, &part, OV_ET_OBJECT);
			while(part.elemtype!=OV_ET_NONE){
				if(part.elemunion.ppart){
					inboxdomain = Ov_DynamicPtrCast(ov_domain, part.pobj);
					if(inboxdomain)
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
					inboxdomain = NULL;
				}
				ov_element_getnextpart(&parentObject, &part, OV_ET_OBJECT);
			}

			if (!inboxdomain){
				/*	not a part either	*/
				ov_logfile_debug("MessageDelivery/retrieveMessage: Found service but it has no inbox - Can't deliver Message"); //,sobj->v_inboxPath
				ov_memstack_unlock();
				return OV_ERR_OK;
			}
		}
	}
	else
	{
		ov_logfile_debug("MessageDelivery/retrieveMessage: Couldn't find Service! - Can't deliver Message"); //,sobj->v_inboxPath
		ov_memstack_unlock();
		return OV_ERR_OK;	/*	we could not find the service, but we don't care :-) (the specs...)	*/
	}
	result = MessageSys_createAnonymousMessage(inboxdomain, "Message", (OV_INSTPTR_ov_object*)(&message));
	if(Ov_Fail(result)){
		ov_logfile_error("MessageDelivery/retrieveMessage: Couldn't create Object 'Message' Reason: %s", ov_result_getresulttext(result));
		ov_memstack_unlock();
		return OV_ERR_OK;
	}

	/*	fill data into the message object	*/

	result = OV_ERR_OK;	/*	clear result. use binary or for errors in setting values to get a collective good/bad in the end	*/
	/*	find servername (representation for a port, so ':' is used as delimiter) and split field	*/
	tempSrvName = strchr(msgHeader.rcvSysAdr, ':');
	if(!tempSrvName)
	{
		Ov_DeleteObject((OV_INSTPTR_ov_object) message);
		ov_logfile_debug("MessageDelivery/retrieveMessage: servername not encoded in rcvSysAdr");
		ov_memstack_unlock();
		return OV_ERR_OK;
	}
	*tempSrvName = '\0';
	tempSrvName++;
	result |= MessageSys_Message_receiverName_set(message,tempSrvName);
	result |= MessageSys_Message_receiverAddress_set(message, msgHeader.rcvSysAdr);
	result |= MessageSys_Message_receiverComponent_set(message, msgHeader.rcvLocAdr);

	if(msgHeader.sndSysAdr)
	{
		/*	find servername (representation for a port, so ':' is used as delimiter) and split field	*/
		tempSrvName = strchr(msgHeader.sndSysAdr, ':');
		if(tempSrvName)
		{
			*tempSrvName = '\0';
			tempSrvName++;
			result |= MessageSys_Message_senderName_set(message, tempSrvName);
		}
		else
			result |= MessageSys_Message_senderName_set(message, "");
	}
	result |= MessageSys_Message_senderAddress_set(message, msgHeader.sndSysAdr);
	result |= MessageSys_Message_senderComponent_set(message, msgHeader.sndLocAdr);

	result |= MessageSys_Message_msgID_set(message, msgHeader.msgId);
	result |= ov_string_setvalue(&message->v_auth, msgHeader.auth);
	result |= ov_string_setvalue(&message->v_refMsgID, msgHeader.refMsgId);
	/*	header done, handle errors	*/
	if(Ov_Fail(result))
	{
		Ov_DeleteObject((OV_INSTPTR_ov_object) message);
		ov_logfile_debug("MessageDelivery/retrieveMessage: setting of header-values for message object failed. cumulated error-code is: %X", result);
		ov_memstack_unlock();
		return OV_ERR_OK;
	}

	/*	copy out body. don't do ANY parsing here	*/
	/*	find begin of bdy	*/
	result = acplt_simpleMsg_xml_findElementBegin(value, "bdy", &tempBody);
	if(Ov_Fail(result))
	{
		Ov_DeleteObject((OV_INSTPTR_ov_object) message);
		ov_logfile_debug("MessageDelivery/retrieveMessage: parsing message failed. Reason: %s", ov_result_getresulttext(result));
		ov_memstack_unlock();
		return OV_ERR_OK;
	}
	if(!tempBody)
	{
		Ov_DeleteObject((OV_INSTPTR_ov_object) message);
		ov_logfile_debug("MessageDelivery/retrieveMessage: no bdy element");
		ov_memstack_unlock();
		return OV_ERR_OK;
	}

	/*	find end of msg	*/
	result = acplt_simpleMsg_xml_findElementBegin(tempBody, "/msg", &msgEnd);
	if(Ov_Fail(result))
	{
		Ov_DeleteObject((OV_INSTPTR_ov_object) message);
		ov_logfile_debug("MessageDelivery/retrieveMessage: parsing message failed. Reason: %s", ov_result_getresulttext(result));
		ov_memstack_unlock();
		return OV_ERR_OK;
	}
	if(!msgEnd)
	{
		Ov_DeleteObject((OV_INSTPTR_ov_object) message);
		ov_logfile_debug("MessageDelivery/retrieveMessage: no end of msg element found");
		ov_memstack_unlock();
		return OV_ERR_OK;
	}
	/*	terminate string at end of message	*/
	*msgEnd = '\0';


	result = ov_string_setvalue(&(message->v_msgBody), tempBody);
	if(Ov_Fail(result))
	{
		Ov_DeleteObject((OV_INSTPTR_ov_object) message);
		ov_logfile_debug("MessageDelivery/retrieveMessage: error copying body. Reason: %s", ov_result_getresulttext(result));
		ov_memstack_unlock();
		return OV_ERR_OK;
	}

	message->v_msgStatus = MSGDONE;

	//Collecting Garbage
	ov_memstack_unlock();
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_BOOL MessageSys_MsgDelivery_sendMessage(
		OV_INSTPTR_MessageSys_MsgDelivery         component,
		OV_INSTPTR_MessageSys_Message          message
) {
	if(message->v_msgStatus == MSGREADYFORSENDING){

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



