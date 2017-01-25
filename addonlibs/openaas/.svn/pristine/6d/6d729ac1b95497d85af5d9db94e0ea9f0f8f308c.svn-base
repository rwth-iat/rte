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
#include "ksbase_helper.h"
#include "ks_logfile.h"

/*******************************************************************************************************************************************************************************
 * 				Connection handling and sending
 ******************************************************************************************************************************************************************************/

/*	if there is no connection (open or opening) open one	*/
static OV_RESULT MessageSys_initiateConnection(OV_INSTPTR_ksbase_Channel pChannel, OV_STRING host, OV_STRING port)
{
	OV_RESULT result;
	OV_VTBLPTR_ksbase_Channel pVtblChannel = NULL;
	if((pChannel->v_ConnectionState != KSBASE_CONNSTATE_OPEN) && (pChannel->v_ConnectionState != KSBASE_CONNSTATE_OPENING))
	{
		if(!port)
			return OV_ERR_BADPARAM;
		if(!host)
			return OV_ERR_BADPARAM;

		Ov_GetVTablePtr(ksbase_Channel, pVtblChannel, pChannel);
		result = pVtblChannel->m_OpenConnection(pChannel, host, port);
		return result;
	}
	return OV_ERR_OK;
}

/*	check if connection is open. if so, send and set message state to busy. if not set Message state to awaiting connection.	*/
static OV_RESULT MessageSys_trySend(OV_INSTPTR_MessageSys_Message pMsg, OV_INSTPTR_ksbase_Channel pChannel, OV_VTBLPTR_ksbase_Channel pVtblChannel)
{
	OV_RESULT result = OV_ERR_OK;

	if(pChannel->v_ConnectionState == KSBASE_CONNSTATE_OPEN)
	{
		result = pVtblChannel->m_SendData(pChannel);
		if(Ov_Fail(result))
		{
			ksbase_free_KSDATAPACKET(&(pChannel->v_outData));
			return result;
		}

		if((pChannel->v_outData.readPT - pChannel->v_outData.data) >= pChannel->v_outData.length)	/*	everything sent	*/
		{
			pMsg->v_msgStatus = MSGDONE;
			if(pChannel->v_CloseAfterSend == TRUE)
				Ov_DeleteObject(pChannel);
		}
	}
	return OV_ERR_OK;
}

static OV_RESULT MessageSys_createChannel(OV_INSTPTR_MessageSys_MsgDelivery pDelivery, OV_INSTPTR_ksbase_Channel* ppChannel)
{
	OV_STRING OptValTemp = NULL;
	OV_INSTPTR_ov_class pClassChannel = NULL;
	OV_RESULT result;
	/*
	 * Checking MSGSYS_USECHANNEL and KS_USECHANNEL options to determine which channel type to use. use TCPChannel per default.
	 */
	ov_memstack_lock();
	OptValTemp = ov_vendortree_getcmdlineoption_value("MSGSYS_USECHANNEL");
	if(!OptValTemp)
		OptValTemp = ov_vendortree_getcmdlineoption_value("KS_USECHANNEL");

	if(!OptValTemp)
	{/*	Neither option specified, use TCPChannel as default	*/
		OptValTemp = ov_memstack_alloc(sizeof("TCPChannel"));
		strcpy(OptValTemp, "TCPChannel");
	}

	if(*OptValTemp)
	{	/*	empty option means do not create channel	*/
		pClassChannel = Ov_StaticPtrCast(ov_class, Ov_GetFirstChild(ov_instantiation, pclass_ov_class));
		while(pClassChannel)
		{
			if(ov_string_compare(pClassChannel->v_identifier, OptValTemp) == OV_STRCMP_EQUAL)
				break;
			pClassChannel = Ov_StaticPtrCast(ov_class, Ov_GetNextChild(ov_instantiation, pClassChannel));
		}

		if(pClassChannel)
		{/*	channel class found create channel	*/
			result = MessageSys_createAnonymousObject(pClassChannel, &(pDelivery->p_Channels), "channel", (OV_INSTPTR_ov_object*) ppChannel);
			if(Ov_Fail(result))
			{
				KS_logfile_error(("%s createChannel: could not create channel. reason: %s", pDelivery->v_identifier, ov_result_getresulttext(result)));
				return result;
			}
			else
			{
				(*ppChannel)->v_ClientHandlerAssociated = KSBASE_CH_NOTNEEDED;
			}
		}
	}
	else
	{
		ov_memstack_unlock();
		*ppChannel = NULL;
		return OV_ERR_OK;
	}
	ov_memstack_unlock();
	return OV_ERR_OK;
}

/******************************************************************************************************************************************************************************************/

OV_DLLFNCEXPORT OV_RESULT MessageSys_MsgDelivery_constructor(
		OV_INSTPTR_ov_object 	pobj
) {
	/*
	 *   local variables
	 */
	OV_INSTPTR_MessageSys_MsgDelivery this = Ov_StaticPtrCast(MessageSys_MsgDelivery, pobj);
	OV_RESULT result;
	OV_INSTPTR_ksapi_setVar setVar =  NULL;


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

	return OV_ERR_OK;
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
	OV_INSTPTR_ksbase_Channel pChannel = NULL;
	OV_VTBLPTR_ksbase_Channel pVtblChannel = NULL;
	OV_INSTPTR_MessageSys_msgHandler pMsgHandler = NULL;
	OV_INSTPTR_MessageSys_MsgSendExtension	pMsgSendExt	=	NULL;
	OV_VTBLPTR_MessageSys_MsgSendExtension	pVtblSendExt	=	NULL;
	OV_ANY value;
	OV_STRING msgString = NULL;
	OV_STRING headerString = NULL;
	OV_UINT hdrLength = 0;
	OV_UINT bdyLength = 0;
	ACPLT_MSGHEADER msgHeader;
	OV_TIME tTemp = {0, 0};
	OV_RESULT result;

	msg = Ov_GetChild(MessageSys_MsgDelivery2CurrentMessage, this);
	if(msg){ //Currently we are processing a message, lets see how far we are...
		if((msg->v_msgStatus != MSGDONE) && (msg->v_msgStatus != MSGRECEIVERERROR)
				&& (msg->v_msgStatus != MSGFATALERROR)){
			/*	still sending / waiting for answer of ks-system	*/
			switch(msg->v_sendBy){
			case MSG_SEND_DIRECTLY:	/*	send directly	*/
				pChannel = Ov_GetChild(MessageSys_Message2Channel, msg);
				if(!pChannel || !pChannel->v_outData.length || (pChannel->v_outData.readPT - pChannel->v_outData.data >= pChannel->v_outData.length) || !pChannel->v_ConnectionState) {
				/*	everything is sent or Channel disappeared or connection is closed	*/
					if(!pChannel->v_ConnectionState)
						Ov_DeleteObject(pChannel);
					Ov_DeleteObject(msg);
				} else if(pChannel->v_ConnectionState > 128) {	/*	connection error	*/
					ov_logfile_info("MessageDelivery/typeMethod: Channel indicates error in connection, CurrentMessage wasn't sent");
					Ov_DeleteObject(msg);
				} else {	/*	check timeouts	*/
					ov_time_gettime(&tTemp);
					if(tTemp.secs > (this->v_sendTime.secs + this->v_sendTimeOut)) {
						ov_logfile_info("MessageDelivery/typeMethod: Timeout after sending Message, CurrentMessage probably wasn't delivered");
						Ov_DeleteObject(msg);
					}
				}
				break;
			case MSG_SEND_KSSETVAR:	/*	send via ks-setvar	*/
				sendingInstance = (OV_INSTPTR_ksapi_setVar)ov_path_getobjectpointer(SENDINGINSTANCE,2);
				if(sendingInstance->v_status == KSAPI_COMMON_REQUESTCOMPLETED) {
					Ov_DeleteObject(msg);
				} else if(sendingInstance->v_status == KSAPI_COMMON_INTERNALERROR) {
					ov_logfile_info("MessageDelivery/typeMethod: An error occured in the sendingprocess, CurrentMessage wasn't sent");
					Ov_DeleteObject(msg);
					ksapi_KSApiCommon_Reset_set(Ov_PtrUpCast(ksapi_KSApiCommon, sendingInstance), TRUE);
					ksapi_KSApiCommon_Reset_set(Ov_PtrUpCast(ksapi_KSApiCommon, sendingInstance), FALSE);
				} else if(sendingInstance->v_status ==KSAPI_COMMON_EXTERNALERROR) {
					ov_logfile_info("MessageDelivery/typeMethod: Error between ksClient-Object and receiver-side, CurrentMessage wasn't delivered");
					Ov_DeleteObject(msg);
					ksapi_KSApiCommon_Reset_set(Ov_PtrUpCast(ksapi_KSApiCommon, sendingInstance), TRUE);
					ksapi_KSApiCommon_Reset_set(Ov_PtrUpCast(ksapi_KSApiCommon, sendingInstance), FALSE);
				} else if(sendingInstance->v_status == KSAPI_COMMON_WAITINGFORANSWER) {
					ov_time_gettime(&tTemp);
					if(tTemp.secs > (this->v_sendTime.secs + this->v_sendTimeOut)) {
						ov_logfile_info("MessageDelivery/typeMethod: Timeout after sending Message, CurrentMessage probably wasn't delivered");
						Ov_DeleteObject(msg);
					}
				}
				break;
			case MSG_SEND_EXTENSION:
				/*	someone else is handling this one --> detach	*/
					Ov_Unlink(MessageSys_MsgDelivery2CurrentMessage, this, msg);
					Ov_Unlink(MessageSys_MsgDelivery2Message, this, msg);
				break;
			default:
				ov_logfile_info("MessageDelivery/typeMethod: invalid protocol for Message: %s", msg->v_identifier);
				Ov_DeleteObject(msg);
				break;
			}
		} else {	/*	message is sent or an error occurred -> delete message	*/
			Ov_DeleteObject(msg);
		}
		return;
	} else { // we are NOT currently handling a message - lets see if sth is needs to be done!
		value.value.vartype = OV_VT_VOID;	/*	initialize value	*/
		value.value.valueunion.val_string = NULL;
		msg = Ov_GetFirstChild(MessageSys_MsgDelivery2Message, this);
		if(msg) {
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
			if(!msg->v_receiverAddress || !msg->v_receiverName) {
				ov_logfile_error("MessageDelivery/typeMethod: receiverAddress or ReceiverName of message %s not set", msg->v_identifier);
				msg->v_msgStatus = MSGFATALERROR;
				ov_memstack_unlock();
				return;
			}
			result = ov_string_print(&(msgHeader.rcvSysAdr), "%s:%s", msg->v_receiverAddress, msg->v_receiverName);
			if(Ov_Fail(result)) {
				ov_logfile_error("MessageDelivery/typeMethod: Couldn't build rcvSysAdr for message %s", msg->v_identifier);
				msg->v_msgStatus = MSGFATALERROR;
				ov_memstack_unlock();
				return;
			}
			msgHeader.rcvLocAdr = msg->v_receiverComponent;
			if(msg->v_senderAddress && msg->v_senderName) {
				result = ov_string_print(&(msgHeader.sndSysAdr), "%s:%s", msg->v_senderAddress, msg->v_senderName);
				if(Ov_Fail(result)) {
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
			if(!headerString) {
				ov_logfile_error("MessageDelivery/typeMethod: Couldn't build header for message %s", msg->v_identifier);
				msg->v_msgStatus = MSGFATALERROR;
				ov_memstack_unlock();
				return;
			}
			if(!msg->v_msgBody) {
				ov_logfile_error("MessageDelivery/typeMethod: No body for message %s", msg->v_identifier);
				msg->v_msgStatus = MSGFATALERROR;
				ov_memstack_unlock();
				return;
			}
			/*	build message string (concatenate header and body and set <msg> </msg> tags around it and send it via specified protocol	*/
			switch(msg->v_sendBy) {
			case MSG_SEND_DIRECTLY:
				/*	send directly	*/
				hdrLength = strlen(headerString);
				bdyLength = strlen(msg->v_msgBody);
				result = ov_string_print(&msgString, "<msg hdrL=%u bdyL=%u>%s%s</msg>", hdrLength, bdyLength, headerString, msg->v_msgBody);
				if(Ov_Fail(result)) {
					ov_logfile_error("MessageDelivery/typeMethod: Couldn't build message %s", msg->v_identifier);
					msg->v_msgStatus = MSGFATALERROR;
					ov_memstack_unlock();
					return;
				}
				/*	unlock memstack, we don't need it anymore	*/
				ov_memstack_unlock();
				pChannel = Ov_GetChild(MessageSys_Message2Channel, msg);
				if(!pChannel) {	/*	no channel found, create new one	*/
					KS_logfile_debug(("msgDelivers: no Channel associated with message. creating new one"));
					result = MessageSys_createChannel(this, &pChannel);
					if(Ov_Fail(result)) {
						ov_memstack_lock();
						ov_logfile_error("MessageDelivery/typeMethod: Couldn't create Channel. reason: %s", ov_result_getresulttext(result));
						msg->v_msgStatus = MSGFATALERROR;
						ov_memstack_unlock();
						return;
					}
					result = Ov_Link(MessageSys_Message2Channel, msg, pChannel);
					if(Ov_Fail(result)) {
						ov_memstack_lock();
						ov_logfile_error("MessageDelivery/typeMethod: Couldn't link Message and Channel. reason: %s", ov_result_getresulttext(result));
						msg->v_msgStatus = MSGFATALERROR;
						Ov_DeleteObject(pChannel);
						ov_memstack_unlock();
						return;
					}
				}
				/*	copy data to channel	*/
				result = ksbase_KSDATAPACKET_append(&(pChannel->v_outData), (OV_BYTE*) msgString, strlen(msgString));
				if(Ov_Fail(result)) {
					ov_memstack_lock();
					ov_logfile_error("MessageDelivery/typeMethod: Couldn't copy data. reason: %s", ov_result_getresulttext(result));
					msg->v_msgStatus = MSGFATALERROR;
					ov_memstack_unlock();
					return;
				}
				/*	set flags at channel	*/
				if(msg->v_expectAnswer == TRUE) {
					/*	check if MsgHandler for answers is there. If not, create one and link it to the channel	*/
					pMsgHandler = Ov_StaticPtrCast(MessageSys_msgHandler, Ov_GetChild(ksbase_AssocChannelClientHandler, pChannel));
					if(!pMsgHandler) {
						pChannel->v_CloseAfterSend = FALSE;
						result = Ov_CreateObject(MessageSys_msgHandler, pMsgHandler, pChannel, "MsgHandler");
						if(Ov_Fail(result)) {
							ov_memstack_lock();
							ov_logfile_error("MessageDelivery/typeMethod: Couldn't create MsgHandler for AnswerMessages. reason: %s", ov_result_getresulttext(result));
							ov_memstack_unlock();
							pChannel->v_CloseAfterSend = TRUE;
						} else 	{
							result = Ov_Link(ksbase_AssocChannelClientHandler, pChannel, pMsgHandler);
							if(Ov_Fail(result)) {
								ov_memstack_lock();
								ov_logfile_error("MessageDelivery/typeMethod: Couldn't link MsgHandler and channel. reason: %s", ov_result_getresulttext(result));
								ov_memstack_unlock();
								pChannel->v_CloseAfterSend = TRUE;
								Ov_DeleteObject(pMsgHandler);
							}
							pChannel->v_ClientHandlerAssociated = KSBASE_CH_ASSOCIATED;
						}
					}

				} else {
					pChannel->v_CloseAfterSend = TRUE;
				}
				KS_logfile_debug(("msgDelivery: pChannel->v_ConnectionState is: %u", pChannel->v_ConnectionState));
				result = MessageSys_initiateConnection(pChannel, msg->v_receiverAddress, msg->v_receiverName);
				if(Ov_Fail(result)) {
					ov_memstack_lock();
					ov_logfile_error("MessageDelivery/typeMethod: Couldn't initiate connection. reason: %s", ov_result_getresulttext(result));
					msg->v_msgStatus = MSGFATALERROR;
					ov_memstack_unlock();
					return;
				}
				Ov_GetVTablePtr(ksbase_Channel, pVtblChannel, pChannel);
				result = MessageSys_trySend(msg, pChannel, pVtblChannel);
				if(Ov_Fail(result)) {
					ov_memstack_lock();
					ov_logfile_error("MessageDelivery/typeMethod: Couldn't send message. reason: %s", ov_result_getresulttext(result));
					msg->v_msgStatus = MSGFATALERROR;
					ov_memstack_unlock();
					return;
				}
				pChannel->v_actimode = 1;
				break;
			case MSG_SEND_KSSETVAR:
				/*	send via ks-setvar	*/
				result = ov_string_print(&msgString, "<msg>%s%s</msg>", headerString, msg->v_msgBody);
				/*	unlock memstack, we don't need it anymore	*/
				ov_memstack_unlock();
				if(Ov_Fail(result)) {
					ov_logfile_error("MessageDelivery/typeMethod: Couldn't build message %s", msg->v_identifier);
					msg->v_msgStatus = MSGFATALERROR;
					ov_memstack_unlock();
					return;
				}
				//send
				value.value.vartype = OV_VT_STRING;
				value.value.valueunion.val_string = msgString;
				sendingInstance = (OV_INSTPTR_ksapi_setVar)ov_path_getobjectpointer(SENDINGINSTANCE,2);
				if(sendingInstance) {
					ksapi_setVar_setandsubmit(sendingInstance, MessageSys_Message_receiverAddress_get(msg),MessageSys_Message_receiverName_get(msg),"/communication/MessageSys.retrieveMessage", value);
					ov_time_gettime(&(this->v_sendTime));
				} else {
					ov_logfile_error("MessageDelivery/typeMethod: Couldn't find sendingInstance, no further sending will proceed");
				}
				break;
			case MSG_SEND_EXTENSION:
				pMsgSendExt = Ov_GetParent(MessageSys_SendExtension2Message, msg);
				if(!pMsgSendExt){
					/*	no send extension --> delete message	*/
					ov_logfile_error("MessageDelivery/typemethod: no sendextension linked to msg %s which shall be sent by extension --> deleting message", msg->v_identifier);
					Ov_DeleteObject(msg);
					ov_memstack_unlock();
					break;
				}
				Ov_GetVTablePtr(MessageSys_MsgSendExtension, pVtblSendExt, pMsgSendExt);
				if(!pVtblSendExt){
					ov_logfile_error("MessageDelivery/typemethod: could not get Vtable pointer of sendextension for %s --> deleting message", msg->v_identifier);
					Ov_DeleteObject(msg);
					ov_memstack_unlock();
					break;
				}
				result = pVtblSendExt->m_sendMessage(pMsgSendExt, msg, headerString);
				ov_memstack_unlock();
				if(Ov_Fail(result)){
					ov_memstack_lock();
					ov_logfile_error("MessageDelivery/typemethod: sending message %s returned error: %s", msg->v_identifier, ov_result_getresulttext(result));
					ov_memstack_unlock();
					Ov_DeleteObject(msg);
					break;
				}
				break;
			default:
				/*	invalid protocol	*/
				ov_logfile_error("MessageDelivery/typemethod: invalid protocol for Message: %s", msg->v_identifier);
				Ov_DeleteObject(msg);
				return;
			}
			ov_string_setvalue(&msgString, NULL);
		}
	}
	return;
}

OV_DLLFNCEXPORT OV_RESULT MessageSys_parseAndDeliverMsg(const OV_STRING	value, OV_INSTPTR_MessageSys_Message* createdMsg, OV_INSTPTR_ov_domain* msgCreatedIn)
{
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
	OV_UINT	iterator = 0;

	ov_memstack_lock();

	/*	determine start and end of msg	*/
	result = acplt_simpleMsg_xml_findElementBegin(value, "msg", &tempBody);
	if(Ov_Fail(result))
	{
		ov_logfile_info("MessageDelivery/retrieveMessage: parsing message failed. Reason: %s", ov_result_getresulttext(result));
		ov_memstack_unlock();
		return OV_ERR_OK;
	}
	if(!tempBody)
	{
		ov_logfile_info("MessageDelivery/retrieveMessage: no msg element");
		ov_memstack_unlock();
		return OV_ERR_OK;
	}

	/*	extract message header	*/
	acplt_simpleMsg_initHeader(&msgHeader);
	result = acplt_simpleMsg_parseMessageHeader(value, &msgHeader);
	if(Ov_Fail(result))
	{
		ov_logfile_info("MessageDelivery/retrieveMessage: parsing message header failed. Reason: %s", ov_result_getresulttext(result));
		ov_memstack_unlock();
		return OV_ERR_OK;
	}


	service = msgHeader.rcvLocAdr;

	//New findService called here
	result = MessageSys_MsgDelivery_findService(&sobj, service);

	if(Ov_OK(result))
	{
		sDom = Ov_DynamicPtrCast(ov_domain, sobj);
		if(sDom)
		{
			/*	iterate over all children in containment to find inbox	*/
			Ov_ForEachChildEx(ov_containment, sDom, inboxdomain, ov_domain)
			{
				if(ov_string_comparei(inboxName, inboxdomain->v_identifier) == OV_STRCMP_EQUAL)
				{	/*	inbox found	*/
					break;
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
				ov_logfile_info("MessageDelivery/retrieveMessage: Found service but it has no inbox - Can't deliver Message"); //,sobj->v_inboxPath
				ov_memstack_unlock();
				return OV_ERR_OK;
			}
		}
	}
	else
	{
		ov_logfile_info("MessageDelivery/retrieveMessage: Couldn't find Service! - Can't deliver Message"); //,sobj->v_inboxPath
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
		if(msgHeader.rcvSysAdr[0] == '/' && msgHeader.rcvSysAdr[1] == '/'){
			for(iterator = 2; msgHeader.rcvSysAdr[iterator] != '\0' && msgHeader.rcvSysAdr[iterator] != '/'; iterator++)
				;
			if(msgHeader.rcvSysAdr[iterator] == '/'){
				tempSrvName = &(msgHeader.rcvSysAdr[iterator]);
			}
		} else {
			Ov_DeleteObject((OV_INSTPTR_ov_object) message);
			ov_logfile_info("MessageDelivery/retrieveMessage: servername not encoded in rcvSysAdr");
			ov_memstack_unlock();
			return OV_ERR_OK;
		}
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
		ov_logfile_info("MessageDelivery/retrieveMessage: setting of header-values for message object failed. cumulated error-code is: %X", result);
		ov_memstack_unlock();
		return OV_ERR_OK;
	}

	/*	copy out body. don't do ANY parsing here	*/
	/*	find begin of bdy	*/
	result = acplt_simpleMsg_xml_findElementBegin(value, "bdy", &tempBody);
	if(Ov_Fail(result))
	{
		Ov_DeleteObject((OV_INSTPTR_ov_object) message);
		ov_logfile_info("MessageDelivery/retrieveMessage: parsing message failed. Reason: %s", ov_result_getresulttext(result));
		ov_memstack_unlock();
		return OV_ERR_OK;
	}
	if(!tempBody)
	{
		Ov_DeleteObject((OV_INSTPTR_ov_object) message);
		ov_logfile_info("MessageDelivery/retrieveMessage: no bdy element");
		ov_memstack_unlock();
		return OV_ERR_OK;
	}

	/*	find end of msg	*/
	result = acplt_simpleMsg_xml_findElementBegin(tempBody, "/msg", &msgEnd);
	if(Ov_Fail(result))
	{
		Ov_DeleteObject((OV_INSTPTR_ov_object) message);
		ov_logfile_info("MessageDelivery/retrieveMessage: parsing message failed. Reason: %s", ov_result_getresulttext(result));
		ov_memstack_unlock();
		return OV_ERR_OK;
	}
	if(!msgEnd)
	{
		Ov_DeleteObject((OV_INSTPTR_ov_object) message);
		ov_logfile_info("MessageDelivery/retrieveMessage: no end of msg element found");
		ov_memstack_unlock();
		return OV_ERR_OK;
	}
	/*	terminate string at end of message	*/
	*msgEnd = '\0';


	result = ov_string_setvalue(&(message->v_msgBody), tempBody);
	if(Ov_Fail(result))
	{
		Ov_DeleteObject((OV_INSTPTR_ov_object) message);
		ov_logfile_info("MessageDelivery/retrieveMessage: error copying body. Reason: %s", ov_result_getresulttext(result));
		ov_memstack_unlock();
		return OV_ERR_OK;
	}

	message->v_msgStatus = MSGDONE;

	/*	fill in return information	*/
	if(createdMsg)
		*createdMsg = message;
	if(msgCreatedIn)
		*msgCreatedIn = inboxdomain;

	//Collecting Garbage
	ov_memstack_unlock();
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT MessageSys_MsgDelivery_retrieveMessage_set(
		OV_INSTPTR_MessageSys_MsgDelivery	pobj,
		const OV_STRING	value
) {
	OV_INSTPTR_MessageSys_Message pMsg = NULL;
	MessageSys_parseAndDeliverMsg(value, &pMsg, NULL);
	if(pMsg){
		pMsg->v_sendBy = MSG_SEND_KSSETVAR;
	}
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

OV_DLLFNCEXPORT OV_RESULT MessageSys_MsgDelivery_findService(
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



