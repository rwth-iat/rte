/*
 * xdrClient_objectFuncs.c
 *
 *  Created on: 24.04.2013
 *      Author: lars
 */

#ifndef OV_COMPILE_LIBRARY_ksxdr
#define OV_COMPILE_LIBRARY_ksxdr
#endif


#include "ksxdr.h"
#include "libov/ov_macros.h"
#include "libov/ov_malloc.h"
#include "libov/ov_result.h"
#include "ov_ksserver_backend.h"
#include "KSDATAPACKET_xdrhandling.h"
#include "ksxdr_config.h"
#include "ks_logfile.h"
#include "ksbase_helper.h"
#include "ksxdr_services.h"



/*******************************************************************************************************************************************************************************
 * 				createObject
 ******************************************************************************************************************************************************************************/

OV_DLLFNCEXPORT OV_RESULT ksxdr_xdrClient_requestCreateObject(
		const OV_INSTPTR_ksbase_ClientBase this,
		const OV_INSTPTR_ksbase_ClientTicketGenerator TicketGenerator,
		const OV_UINT items_length,
		const OV_CREATEOBJ_ITEM* items_val,
		const OV_INSTPTR_ov_domain	callbackThat,
		void (*callback)(const OV_INSTPTR_ov_domain this, const OV_INSTPTR_ov_domain that)
) {
	/*
	 *   local variables
	 */
	OV_RESULT result;
	OV_UINT msgindex = 0;
	OV_INSTPTR_ksxdr_xdrClient thisCl = Ov_StaticPtrCast(ksxdr_xdrClient, this);
	OV_INSTPTR_ksbase_Channel	pChannel = NULL;
	OV_VTBLPTR_ksbase_Channel	pVtblChannel = NULL;
	KS_DATAPACKET tempDataPacket;


	if((thisCl->v_state != KSBASE_CLST_COMPLETED) && (thisCl->v_state != KSBASE_CLST_INITIAL))
		return OV_ERR_NOACCESS;

	/*	set callback function	*/
	thisCl->v_callback.instanceCalled = callbackThat;
	thisCl->v_callback.callbackFunction = callback;

	/*	get pointer to channel and to its Vtable	*/
	result = getChannelPointer(thisCl, &pChannel, &pVtblChannel);
	if(Ov_Fail(result))
	{
		KS_logfile_error(("%s: Could not get Channel pointers.", this->v_identifier));
		return result;
	}
	if(!pChannel || !pVtblChannel)
	{
		KS_logfile_warning(("%s: No Channel found and none should be created. Will do nothing.", this->v_identifier));
		return OV_ERR_OK;
	}

	/*	initialize temporary datapacket
	 *	message is written to temporary packet. if a port is set the temporary packet is put into the channels outData packet if not it is saved in the clients
	 *		dataToSend packet and a getserver is issued
	 */
	tempDataPacket.length = 0;
	tempDataPacket.data = NULL;
	tempDataPacket.readPT = NULL;
	tempDataPacket.writePT = NULL;

	do{	/*	using a do...while(0) to run this block once and be able to use break-directives to jump out when an error occurs	*/

		/*	generate Header	*/
		result = ksxdr_generateClientMessageHeader(KS_CREATEOBJECT, pChannel->v_usesStreamProtocol, &(tempDataPacket), &(thisCl->v_tempXID), &msgindex);
		if(Ov_Fail(result))
			break;

		/*	insert Ticket	*/
		result = ksxdr_insertTicket(&(tempDataPacket), TicketGenerator);
		if(Ov_Fail(result))
			break;

		result = KS_DATAPACKET_write_xdr_array(&(tempDataPacket), (void**) &items_val, sizeof(OV_CREATEOBJ_ITEM), (OV_UINT*) &items_length,
				(xdr_writefncptr) &xdr_write_OV_CREATEOBJ_ITEM);
		if(Ov_Fail(result))
			break;

		/*	set length (if necessary)	*/
		if(pChannel->v_usesStreamProtocol)
			ksxdr_prepend_length((&tempDataPacket), msgindex);

	}while(0);
	if(Ov_Fail(result))
	{
		ksbase_free_KSDATAPACKET(&(tempDataPacket));
		return result;
	}

	/*	here comes the magic: determine port if none is set. otherwise send message	*/
	if(thisCl->v_serverPort && (*thisCl->v_serverPort))
	{
		/*	port is known --> copy message to channel and send	*/
		ksbase_free_KSDATAPACKET(&(pChannel->v_outData));
		pChannel->v_outData = tempDataPacket;
		thisCl->v_sentXID = thisCl->v_tempXID;
		result = initiateConnection(thisCl, pChannel, pVtblChannel, FALSE, thisCl->v_serverHost, thisCl->v_serverPort);
		if(Ov_Fail(result))
		{
			ksbase_free_KSDATAPACKET(&(pChannel->v_outData));
			return result;
		}

		result = trySend(thisCl, pChannel, pVtblChannel);
		if(Ov_Fail(result))
			return result;
	}
	else
	{
		/*	port is not known --> copy message to temporary location, save callbackfunction and sendxid	*/
		ksbase_free_KSDATAPACKET(&(thisCl->v_dataToSend));
		thisCl->v_dataToSend = tempDataPacket;
		thisCl->v_tempCallback = thisCl->v_callback;
		result = ksxdr_xdrClient_requestGetServer(Ov_StaticPtrCast(ksbase_ClientBase, thisCl), NULL, thisCl->v_serverName, 2, NULL, NULL);
		if(Ov_Fail(result))
		{
			ksbase_free_KSDATAPACKET(&(thisCl->v_dataToSend));
			return result;
		}
	}

	thisCl->v_actimode = 1;

	/*	set ProcID	*/
	thisCl->v_sentProcID = KS_CREATEOBJECT;

	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT ksxdr_xdrClient_processCreateObject(
		const OV_INSTPTR_ksbase_ClientBase this,
		const OV_INSTPTR_ksbase_ClientTicketGenerator TicketGenerator,
		OV_RESULT* result,
		OV_UINT* items_length,
		OV_CREATEOBJECTITEM_RES** items_val
) {
	/*
	 *   local variables
	 */
	OV_INSTPTR_ksxdr_xdrClient	thisCl = Ov_StaticPtrCast(ksxdr_xdrClient, this);
	OV_RESULT fncresult;

	fncresult = KS_DATAPACKET_read_xdr_OV_RESULT(&(thisCl->v_dataReceived), result);
	if(Ov_Fail(fncresult))
		return fncresult;
	if(Ov_OK(*result))
		return KS_DATAPACKET_read_xdr_array_tomemstack(&(thisCl->v_dataReceived), (void**) items_val, sizeof(OV_CREATEOBJECTITEM_RES), items_length,
				(xdr_readfncptr) &xdr_read_OV_CREATEOBJECTITEM_RES);
	else
		return OV_ERR_OK;
}

/*******************************************************************************************************************************************************************************
 * 				deleteobject
 ******************************************************************************************************************************************************************************/

OV_DLLFNCEXPORT OV_RESULT ksxdr_xdrClient_requestDeleteObject(
		const OV_INSTPTR_ksbase_ClientBase this,
		const OV_INSTPTR_ksbase_ClientTicketGenerator TicketGenerator,
		const OV_UINT items_length,
		const OV_STRING* paths,
		const OV_INSTPTR_ov_domain	callbackThat,
		void (*callback)(const OV_INSTPTR_ov_domain this, const OV_INSTPTR_ov_domain that)
) {
	/*
	 *   local variables
	 */
	OV_RESULT result;
	OV_UINT msgindex = 0;
	OV_INSTPTR_ksxdr_xdrClient thisCl = Ov_StaticPtrCast(ksxdr_xdrClient, this);
	OV_INSTPTR_ksbase_Channel	pChannel = NULL;
	OV_VTBLPTR_ksbase_Channel	pVtblChannel = NULL;
	KS_DATAPACKET tempDataPacket;


	if((thisCl->v_state != KSBASE_CLST_COMPLETED) && (thisCl->v_state != KSBASE_CLST_INITIAL))
		return OV_ERR_NOACCESS;

	/*	set callback function	*/
	thisCl->v_callback.instanceCalled = callbackThat;
	thisCl->v_callback.callbackFunction = callback;

	/*	get pointer to channel and to its Vtable	*/
	result = getChannelPointer(thisCl, &pChannel, &pVtblChannel);
	if(Ov_Fail(result))
	{
		KS_logfile_error(("%s: Could not get Channel pointers.", this->v_identifier));
		return result;
	}
	if(!pChannel || !pVtblChannel)
	{
		KS_logfile_warning(("%s: No Channel found and none should be created. Will do nothing.", this->v_identifier));
		return OV_ERR_OK;
	}

	/*	initialize temporary datapacket
	 *	message is written to temporary packet. if a port is set the temporary packet is put into the channels outData packet if not it is saved in the clients
	 *		dataToSend packet and a getserver is issued
	 */
	tempDataPacket.length = 0;
	tempDataPacket.data = NULL;
	tempDataPacket.readPT = NULL;
	tempDataPacket.writePT = NULL;

	do{	/*	using a do...while(0) to run this block once and be able to use break-directives to jump out when an error occurs	*/

		/*	generate Header	*/
		result = ksxdr_generateClientMessageHeader(KS_DELETEOBJECT, pChannel->v_usesStreamProtocol, &(tempDataPacket), &(thisCl->v_tempXID), &msgindex);
		if(Ov_Fail(result))
			break;

		/*	insert Ticket	*/
		result = ksxdr_insertTicket(&(tempDataPacket), TicketGenerator);
		if(Ov_Fail(result))
			break;

		result = KS_DATAPACKET_write_xdr_array(&(tempDataPacket), (void**) &paths, sizeof(OV_STRING), (OV_UINT*) &items_length,
				(xdr_writefncptr) KS_DATAPACKET_write_xdr_string);
		if(Ov_Fail(result))
			break;

		/*	set length (if necessary)	*/
		if(pChannel->v_usesStreamProtocol)
			ksxdr_prepend_length((&tempDataPacket), msgindex);

	}while(0);
	if(Ov_Fail(result))
	{
		ksbase_free_KSDATAPACKET(&(tempDataPacket));
		return result;
	}

	/*	here comes the magic: determine port if none is set. otherwise send message	*/
	if(thisCl->v_serverPort && (*thisCl->v_serverPort))
	{
		/*	port is known --> copy message to channel and send	*/
		ksbase_free_KSDATAPACKET(&(pChannel->v_outData));
		pChannel->v_outData = tempDataPacket;
		thisCl->v_sentXID = thisCl->v_tempXID;
		result = initiateConnection(thisCl, pChannel, pVtblChannel, FALSE, thisCl->v_serverHost, thisCl->v_serverPort);
		if(Ov_Fail(result))
		{
			ksbase_free_KSDATAPACKET(&(pChannel->v_outData));
			return result;
		}

		result = trySend(thisCl, pChannel, pVtblChannel);
		if(Ov_Fail(result))
			return result;
	}
	else
	{
		/*	port is not known --> copy message to temporary location, save callbackfunction and sendxid	*/
		ksbase_free_KSDATAPACKET(&(thisCl->v_dataToSend));
		thisCl->v_dataToSend = tempDataPacket;
		thisCl->v_tempCallback = thisCl->v_callback;
		result = ksxdr_xdrClient_requestGetServer(Ov_StaticPtrCast(ksbase_ClientBase, thisCl), NULL, thisCl->v_serverName, 2, NULL, NULL);
		if(Ov_Fail(result))
		{
			ksbase_free_KSDATAPACKET(&(thisCl->v_dataToSend));
			return result;
		}
	}

	thisCl->v_actimode = 1;

	/*	set ProcID	*/
	thisCl->v_sentProcID = KS_DELETEOBJECT;

	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT ksxdr_xdrClient_processDeleteObject(
		const OV_INSTPTR_ksbase_ClientBase this,
		const OV_INSTPTR_ksbase_ClientTicketGenerator TicketGenerator,
		OV_RESULT* result,
		OV_UINT* items_length,
		OV_RESULT** items_results
) {
	/*
	 *   local variables
	 */
	OV_INSTPTR_ksxdr_xdrClient	thisCl = Ov_StaticPtrCast(ksxdr_xdrClient, this);
	OV_RESULT fncresult;

	fncresult = KS_DATAPACKET_read_xdr_OV_RESULT(&(thisCl->v_dataReceived), result);
	if(Ov_Fail(fncresult))
		return fncresult;
	if(Ov_OK(*result))
		return KS_DATAPACKET_read_xdr_array_tomemstack(&(thisCl->v_dataReceived), (void**) items_results, sizeof(OV_RESULT), items_length,
				(xdr_readfncptr) &KS_DATAPACKET_read_xdr_long);
	else
		return OV_ERR_OK;
}

/*******************************************************************************************************************************************************************************
 * 				renameObject
 ******************************************************************************************************************************************************************************/

OV_DLLFNCEXPORT OV_RESULT ksxdr_xdrClient_requestRenameObject(
		const OV_INSTPTR_ksbase_ClientBase this,
		const OV_INSTPTR_ksbase_ClientTicketGenerator TicketGenerator,
		const OV_UINT items_length,
		const OV_RENAMEOBJECT_ITEM* items_val,
		const OV_INSTPTR_ov_domain	callbackThat,
		void (*callback)(const OV_INSTPTR_ov_domain this, const OV_INSTPTR_ov_domain that)
) {
	/*
	 *   local variables
	 */
	OV_RESULT result;
	OV_UINT msgindex = 0;
	OV_INSTPTR_ksxdr_xdrClient thisCl = Ov_StaticPtrCast(ksxdr_xdrClient, this);
	OV_INSTPTR_ksbase_Channel	pChannel = NULL;
	OV_VTBLPTR_ksbase_Channel	pVtblChannel = NULL;
	KS_DATAPACKET tempDataPacket;


	if((thisCl->v_state != KSBASE_CLST_COMPLETED) && (thisCl->v_state != KSBASE_CLST_INITIAL))
		return OV_ERR_NOACCESS;

	/*	set callback function	*/
	thisCl->v_callback.instanceCalled = callbackThat;
	thisCl->v_callback.callbackFunction = callback;

	/*	get pointer to channel and to its Vtable	*/
	result = getChannelPointer(thisCl, &pChannel, &pVtblChannel);
	if(Ov_Fail(result))
	{
		KS_logfile_error(("%s: Could not get Channel pointers.", this->v_identifier));
		return result;
	}
	if(!pChannel || !pVtblChannel)
	{
		KS_logfile_warning(("%s: No Channel found and none should be created. Will do nothing.", this->v_identifier));
		return OV_ERR_OK;
	}

	/*	initialize temporary datapacket
	 *	message is written to temporary packet. if a port is set the temporary packet is put into the channels outData packet if not it is saved in the clients
	 *		dataToSend packet and a getserver is issued
	 */
	tempDataPacket.length = 0;
	tempDataPacket.data = NULL;
	tempDataPacket.readPT = NULL;
	tempDataPacket.writePT = NULL;

	do{	/*	using a do...while(0) to run this block once and be able to use break-directives to jump out when an error occurs	*/

		/*	generate Header	*/
		result = ksxdr_generateClientMessageHeader(KS_RENAMEOBJECT, pChannel->v_usesStreamProtocol, &(tempDataPacket), &(thisCl->v_tempXID), &msgindex);
		if(Ov_Fail(result))
			break;

		/*	insert Ticket	*/
		result = ksxdr_insertTicket(&(tempDataPacket), TicketGenerator);
		if(Ov_Fail(result))
			break;

		result = KS_DATAPACKET_write_xdr_array(&(tempDataPacket), (void**) &items_val, sizeof(OV_RENAMEOBJECT_ITEM), (OV_UINT*) &items_length,
				(xdr_writefncptr) &xdr_write_OV_RENAMEOBJECT_ITEM);
		if(Ov_Fail(result))
			break;

		/*	set length (if necessary)	*/
		if(pChannel->v_usesStreamProtocol)
			ksxdr_prepend_length((&tempDataPacket), msgindex);

	}while(0);
	if(Ov_Fail(result))
	{
		ksbase_free_KSDATAPACKET(&(tempDataPacket));
		return result;
	}

	/*	here comes the magic: determine port if none is set. otherwise send message	*/
	if(thisCl->v_serverPort && (*thisCl->v_serverPort))
	{
		/*	port is known --> copy message to channel and send	*/
		ksbase_free_KSDATAPACKET(&(pChannel->v_outData));
		pChannel->v_outData = tempDataPacket;
		thisCl->v_sentXID = thisCl->v_tempXID;
		result = initiateConnection(thisCl, pChannel, pVtblChannel, FALSE, thisCl->v_serverHost, thisCl->v_serverPort);
		if(Ov_Fail(result))
		{
			ksbase_free_KSDATAPACKET(&(pChannel->v_outData));
			return result;
		}

		result = trySend(thisCl, pChannel, pVtblChannel);
		if(Ov_Fail(result))
			return result;
	}
	else
	{
		/*	port is not known --> copy message to temporary location, save callbackfunction and sendxid	*/
		ksbase_free_KSDATAPACKET(&(thisCl->v_dataToSend));
		thisCl->v_dataToSend = tempDataPacket;
		thisCl->v_tempCallback = thisCl->v_callback;
		result = ksxdr_xdrClient_requestGetServer(Ov_StaticPtrCast(ksbase_ClientBase, thisCl), NULL, thisCl->v_serverName, 2, NULL, NULL);
		if(Ov_Fail(result))
		{
			ksbase_free_KSDATAPACKET(&(thisCl->v_dataToSend));
			return result;
		}
	}

	thisCl->v_actimode = 1;

	/*	set ProcID	*/
	thisCl->v_sentProcID = KS_RENAMEOBJECT;

	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT ksxdr_xdrClient_processRenameObject(
		const OV_INSTPTR_ksbase_ClientBase this,
		const OV_INSTPTR_ksbase_ClientTicketGenerator TicketGenerator,
		OV_RESULT* result,
		OV_UINT* items_length,
		OV_RESULT** items_results
) {
	/*
	 *   local variables
	 */
	OV_INSTPTR_ksxdr_xdrClient	thisCl = Ov_StaticPtrCast(ksxdr_xdrClient, this);
	OV_RESULT fncresult;

	fncresult = KS_DATAPACKET_read_xdr_OV_RESULT(&(thisCl->v_dataReceived), result);
	if(Ov_Fail(fncresult))
		return fncresult;
	if(Ov_OK(*result))
		return KS_DATAPACKET_read_xdr_array_tomemstack(&(thisCl->v_dataReceived), (void**) items_results, sizeof(OV_RESULT), items_length,
				(xdr_readfncptr) &KS_DATAPACKET_read_xdr_long);
	else
		return OV_ERR_OK;
}

/*******************************************************************************************************************************************************************************
 * 				link
 ******************************************************************************************************************************************************************************/

OV_DLLFNCEXPORT OV_RESULT ksxdr_xdrClient_requestLink(
		const OV_INSTPTR_ksbase_ClientBase this,
		const OV_INSTPTR_ksbase_ClientTicketGenerator TicketGenerator,
		const OV_UINT items_length,
		const OV_LINK_ITEM* items_val,
		const OV_INSTPTR_ov_domain	callbackThat,
		void (*callback)(const OV_INSTPTR_ov_domain this, const OV_INSTPTR_ov_domain that)
) {
	/*
	 *   local variables
	 */
	OV_RESULT result;
	OV_UINT msgindex = 0;
	OV_INSTPTR_ksxdr_xdrClient thisCl = Ov_StaticPtrCast(ksxdr_xdrClient, this);
	OV_INSTPTR_ksbase_Channel	pChannel = NULL;
	OV_VTBLPTR_ksbase_Channel	pVtblChannel = NULL;
	KS_DATAPACKET tempDataPacket;


	if((thisCl->v_state != KSBASE_CLST_COMPLETED) && (thisCl->v_state != KSBASE_CLST_INITIAL))
		return OV_ERR_NOACCESS;

	/*	set callback function	*/
	thisCl->v_callback.instanceCalled = callbackThat;
	thisCl->v_callback.callbackFunction = callback;

	/*	get pointer to channel and to its Vtable	*/
	result = getChannelPointer(thisCl, &pChannel, &pVtblChannel);
	if(Ov_Fail(result))
	{
		KS_logfile_error(("%s: Could not get Channel pointers.", this->v_identifier));
		return result;
	}
	if(!pChannel || !pVtblChannel)
	{
		KS_logfile_warning(("%s: No Channel found and none should be created. Will do nothing.", this->v_identifier));
		return OV_ERR_OK;
	}

	/*	initialize temporary datapacket
	 *	message is written to temporary packet. if a port is set the temporary packet is put into the channels outData packet if not it is saved in the clients
	 *		dataToSend packet and a getserver is issued
	 */
	tempDataPacket.length = 0;
	tempDataPacket.data = NULL;
	tempDataPacket.readPT = NULL;
	tempDataPacket.writePT = NULL;

	do{	/*	using a do...while(0) to run this block once and be able to use break-directives to jump out when an error occurs	*/

		/*	generate Header	*/
		result = ksxdr_generateClientMessageHeader(KS_LINK, pChannel->v_usesStreamProtocol, &(tempDataPacket), &(thisCl->v_tempXID), &msgindex);
		if(Ov_Fail(result))
			break;

		/*	insert Ticket	*/
		result = ksxdr_insertTicket(&(tempDataPacket), TicketGenerator);
		if(Ov_Fail(result))
			break;

		result = KS_DATAPACKET_write_xdr_array(&(tempDataPacket), (void**) &items_val, sizeof(OV_LINK_ITEM), (OV_UINT*) &items_length,
				(xdr_writefncptr) &xdr_write_OV_LINK_ITEM);
		if(Ov_Fail(result))
			break;

		/*	set length (if necessary)	*/
		if(pChannel->v_usesStreamProtocol)
			ksxdr_prepend_length((&tempDataPacket), msgindex);

	}while(0);
	if(Ov_Fail(result))
	{
		ksbase_free_KSDATAPACKET(&(tempDataPacket));
		return result;
	}

	/*	here comes the magic: determine port if none is set. otherwise send message	*/
	if(thisCl->v_serverPort && (*thisCl->v_serverPort))
	{
		/*	port is known --> copy message to channel and send	*/
		ksbase_free_KSDATAPACKET(&(pChannel->v_outData));
		pChannel->v_outData = tempDataPacket;
		thisCl->v_sentXID = thisCl->v_tempXID;
		result = initiateConnection(thisCl, pChannel, pVtblChannel, FALSE, thisCl->v_serverHost, thisCl->v_serverPort);
		if(Ov_Fail(result))
		{
			ksbase_free_KSDATAPACKET(&(pChannel->v_outData));
			return result;
		}

		result = trySend(thisCl, pChannel, pVtblChannel);
		if(Ov_Fail(result))
			return result;
	}
	else
	{
		/*	port is not known --> copy message to temporary location, save callbackfunction and sendxid	*/
		ksbase_free_KSDATAPACKET(&(thisCl->v_dataToSend));
		thisCl->v_dataToSend = tempDataPacket;
		thisCl->v_tempCallback = thisCl->v_callback;
		result = ksxdr_xdrClient_requestGetServer(Ov_StaticPtrCast(ksbase_ClientBase, thisCl), NULL, thisCl->v_serverName, 2, NULL, NULL);
		if(Ov_Fail(result))
		{
			ksbase_free_KSDATAPACKET(&(thisCl->v_dataToSend));
			return result;
		}
	}

	thisCl->v_actimode = 1;

	/*	set ProcID	*/
	thisCl->v_sentProcID = KS_LINK;

	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT ksxdr_xdrClient_processLink(
		const OV_INSTPTR_ksbase_ClientBase this,
		const OV_INSTPTR_ksbase_ClientTicketGenerator TicketGenerator,
		OV_RESULT* result,
		OV_UINT* items_length,
		OV_RESULT** items_results
) {
	/*
	 *   local variables
	 */
	OV_INSTPTR_ksxdr_xdrClient	thisCl = Ov_StaticPtrCast(ksxdr_xdrClient, this);
	OV_RESULT fncresult;

	fncresult = KS_DATAPACKET_read_xdr_OV_RESULT(&(thisCl->v_dataReceived), result);
	if(Ov_Fail(fncresult))
		return fncresult;
	if(Ov_OK(*result))
		return KS_DATAPACKET_read_xdr_array_tomemstack(&(thisCl->v_dataReceived), (void**) items_results, sizeof(OV_RESULT), items_length,
				(xdr_readfncptr) &KS_DATAPACKET_read_xdr_long);
	else
		return OV_ERR_OK;
}

/*******************************************************************************************************************************************************************************
 * 				unlink
 ******************************************************************************************************************************************************************************/

OV_DLLFNCEXPORT OV_RESULT ksxdr_xdrClient_requestUnLink(
		const OV_INSTPTR_ksbase_ClientBase this,
		const OV_INSTPTR_ksbase_ClientTicketGenerator TicketGenerator,
		const OV_UINT items_length,
		const OV_UNLINK_ITEM* items_val,
		const OV_INSTPTR_ov_domain	callbackThat,
		void (*callback)(const OV_INSTPTR_ov_domain this, const OV_INSTPTR_ov_domain that)
) {
	/*
	 *   local variables
	 */
	OV_RESULT result;
	OV_UINT msgindex = 0;
	OV_INSTPTR_ksxdr_xdrClient thisCl = Ov_StaticPtrCast(ksxdr_xdrClient, this);
	OV_INSTPTR_ksbase_Channel	pChannel = NULL;
	OV_VTBLPTR_ksbase_Channel	pVtblChannel = NULL;
	KS_DATAPACKET tempDataPacket;


	if((thisCl->v_state != KSBASE_CLST_COMPLETED) && (thisCl->v_state != KSBASE_CLST_INITIAL))
		return OV_ERR_NOACCESS;

	/*	set callback function	*/
	thisCl->v_callback.instanceCalled = callbackThat;
	thisCl->v_callback.callbackFunction = callback;

	/*	get pointer to channel and to its Vtable	*/
	result = getChannelPointer(thisCl, &pChannel, &pVtblChannel);
	if(Ov_Fail(result))
	{
		KS_logfile_error(("%s: Could not get Channel pointers.", this->v_identifier));
		return result;
	}
	if(!pChannel || !pVtblChannel)
	{
		KS_logfile_warning(("%s: No Channel found and none should be created. Will do nothing.", this->v_identifier));
		return OV_ERR_OK;
	}

	/*	initialize temporary datapacket
	 *	message is written to temporary packet. if a port is set the temporary packet is put into the channels outData packet if not it is saved in the clients
	 *		dataToSend packet and a getserver is issued
	 */
	tempDataPacket.length = 0;
	tempDataPacket.data = NULL;
	tempDataPacket.readPT = NULL;
	tempDataPacket.writePT = NULL;

	do{	/*	using a do...while(0) to run this block once and be able to use break-directives to jump out when an error occurs	*/

		/*	generate Header	*/
		result = ksxdr_generateClientMessageHeader(KS_UNLINK, pChannel->v_usesStreamProtocol, &(tempDataPacket), &(thisCl->v_tempXID), &msgindex);
		if(Ov_Fail(result))
			break;

		/*	insert Ticket	*/
		result = ksxdr_insertTicket(&(tempDataPacket), TicketGenerator);
		if(Ov_Fail(result))
			break;

		result = KS_DATAPACKET_write_xdr_array(&(tempDataPacket), (void**) &items_val, sizeof(OV_UNLINK_ITEM), (OV_UINT*) &items_length,
				(xdr_writefncptr) &xdr_write_OV_UNLINK_ITEM);
		if(Ov_Fail(result))
			break;

		/*	set length (if necessary)	*/
		if(pChannel->v_usesStreamProtocol)
			ksxdr_prepend_length((&tempDataPacket), msgindex);

	}while(0);
	if(Ov_Fail(result))
	{
		ksbase_free_KSDATAPACKET(&(tempDataPacket));
		return result;
	}

	/*	here comes the magic: determine port if none is set. otherwise send message	*/
	if(thisCl->v_serverPort && (*thisCl->v_serverPort))
	{
		/*	port is known --> copy message to channel and send	*/
		ksbase_free_KSDATAPACKET(&(pChannel->v_outData));
		pChannel->v_outData = tempDataPacket;
		thisCl->v_sentXID = thisCl->v_tempXID;
		result = initiateConnection(thisCl, pChannel, pVtblChannel, FALSE, thisCl->v_serverHost, thisCl->v_serverPort);
		if(Ov_Fail(result))
		{
			ksbase_free_KSDATAPACKET(&(pChannel->v_outData));
			return result;
		}

		result = trySend(thisCl, pChannel, pVtblChannel);
		if(Ov_Fail(result))
			return result;
	}
	else
	{
		/*	port is not known --> copy message to temporary location, save callbackfunction and sendxid	*/
		ksbase_free_KSDATAPACKET(&(thisCl->v_dataToSend));
		thisCl->v_dataToSend = tempDataPacket;
		thisCl->v_tempCallback = thisCl->v_callback;
		result = ksxdr_xdrClient_requestGetServer(Ov_StaticPtrCast(ksbase_ClientBase, thisCl), NULL, thisCl->v_serverName, 2, NULL, NULL);
		if(Ov_Fail(result))
		{
			ksbase_free_KSDATAPACKET(&(thisCl->v_dataToSend));
			return result;
		}
	}

	thisCl->v_actimode = 1;

	/*	set ProcID	*/
	thisCl->v_sentProcID = KS_UNLINK;

	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT ksxdr_xdrClient_processUnLink(
		const OV_INSTPTR_ksbase_ClientBase this,
		const OV_INSTPTR_ksbase_ClientTicketGenerator TicketGenerator,
		OV_RESULT* result,
		OV_UINT* items_length,
		OV_RESULT** items_results
) {
	/*
	 *   local variables
	 */
	OV_INSTPTR_ksxdr_xdrClient	thisCl = Ov_StaticPtrCast(ksxdr_xdrClient, this);
	OV_RESULT fncresult;

	fncresult = KS_DATAPACKET_read_xdr_OV_RESULT(&(thisCl->v_dataReceived), result);
	if(Ov_Fail(fncresult))
		return fncresult;
	if(Ov_OK(*result))
		return KS_DATAPACKET_read_xdr_array_tomemstack(&(thisCl->v_dataReceived), (void**) items_results, sizeof(OV_RESULT), items_length,
				(xdr_readfncptr) &KS_DATAPACKET_read_xdr_long);
	else
		return OV_ERR_OK;
}

