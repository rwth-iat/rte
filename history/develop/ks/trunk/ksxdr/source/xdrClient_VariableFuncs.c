/*
 * xdrClient_VariableFuncs.c
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
 * 				getVar
 ******************************************************************************************************************************************************************************/

OV_DLLFNCEXPORT OV_RESULT ksxdr_xdrClient_requestGetVar(
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
	result = ksxdr_getChannelPointer(Ov_PtrUpCast(ksbase_DataHandler, thisCl), &pChannel, &pVtblChannel);
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
		result = ksxdr_generateClientMessageHeader(KS_GETVAR, pChannel->v_usesStreamProtocol, &(tempDataPacket), &(thisCl->v_tempXID), &msgindex);
		if(Ov_Fail(result))
			break;

		/*	insert Ticket	*/
		result = ksxdr_insertTicket(&(tempDataPacket), TicketGenerator);
		if(Ov_Fail(result))
			break;

		/*	path array	*/
		result =  KS_DATAPACKET_write_xdr_array(&(tempDataPacket), (void**)&paths, sizeof(OV_STRING), (OV_UINT*) &items_length, (xdr_writefncptr) &(KS_DATAPACKET_write_xdr_string));
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
		result = ksxdr_initiateConnection(thisCl, pChannel, pVtblChannel, FALSE, thisCl->v_serverHost, thisCl->v_serverPort);
		if(Ov_Fail(result))
		{
			ksbase_free_KSDATAPACKET(&(pChannel->v_outData));
			return result;
		}

		result = ksxdr_trySend(thisCl, pChannel, pVtblChannel);
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
	thisCl->v_sentProcID = KS_GETVAR;

	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT ksxdr_xdrClient_processGetVar(
		const OV_INSTPTR_ksbase_ClientBase this,
		const OV_INSTPTR_ksbase_ClientTicketGenerator TicketGenerator,
		OV_RESULT* result,
		OV_UINT* items_len,
		OV_GETVAR_ITEM** items_val
) {
	/*
	 *   local variables
	 */
	OV_INSTPTR_ksxdr_xdrClient	thisCl = Ov_StaticPtrCast(ksxdr_xdrClient, this);
	OV_RESULT fncresult;

	fncresult = KS_DATAPACKET_read_xdr_OV_RESULT(&(thisCl->v_dataReceived), result);
	if(Ov_Fail(fncresult))
	{
		ksbase_free_KSDATAPACKET(&thisCl->v_dataReceived);
		return fncresult;
	}
	if(Ov_OK(*result))
	{
		fncresult = KS_DATAPACKET_read_xdr_array_tomemstack(&(thisCl->v_dataReceived), (void**) items_val, sizeof(OV_GETVAR_ITEM), items_len, (xdr_readfncptr) &xdr_read_OV_GETVAR_ITEM);
	}
	else
	{
		fncresult = OV_ERR_OK;
	}

	if(Ov_Fail(fncresult) || (thisCl->v_dataReceived.readPT - thisCl->v_dataReceived.data >= thisCl->v_dataReceived.length))
	{
		ksbase_free_KSDATAPACKET(&thisCl->v_dataReceived);
	}
	return fncresult;
}

/*******************************************************************************************************************************************************************************
 * 				setVar
 ******************************************************************************************************************************************************************************/

OV_DLLFNCEXPORT OV_RESULT ksxdr_xdrClient_requestSetVar(
		const OV_INSTPTR_ksbase_ClientBase this,
		const OV_INSTPTR_ksbase_ClientTicketGenerator TicketGenerator,
		const OV_UINT items_length,
		const OV_SETVAR_ITEM* items_val,
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
	result = ksxdr_getChannelPointer(Ov_PtrUpCast(ksbase_DataHandler, thisCl), &pChannel, &pVtblChannel);
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
		result = ksxdr_generateClientMessageHeader(KS_SETVAR, pChannel->v_usesStreamProtocol, &(tempDataPacket), &(thisCl->v_tempXID), &msgindex);
		if(Ov_Fail(result))
			break;

		/*	insert Ticket	*/
		result = ksxdr_insertTicket(&(tempDataPacket), TicketGenerator);
		if(Ov_Fail(result))
			break;

		/*	items array	*/
		result =  KS_DATAPACKET_write_xdr_array(&(tempDataPacket), (void**) &items_val, sizeof(OV_SETVAR_ITEM), (OV_UINT*) &items_length,
				(xdr_writefncptr) &xdr_write_OV_SETVAR_ITEM);
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
		pChannel->v_outData = tempDataPacket;	/*	channel will free it after sending	*/
		thisCl->v_sentXID = thisCl->v_tempXID;
		result = ksxdr_initiateConnection(thisCl, pChannel, pVtblChannel, FALSE, thisCl->v_serverHost, thisCl->v_serverPort);
		if(Ov_Fail(result))
		{
			ksbase_free_KSDATAPACKET(&(pChannel->v_outData));
			return result;
		}

		result = ksxdr_trySend(thisCl, pChannel, pVtblChannel);
		if(Ov_Fail(result))
			return result;
	}
	else
	{
		/*	port is not known --> copy message to temporary location, save callbackfunction and sendxid	*/
		ksbase_free_KSDATAPACKET(&(thisCl->v_dataToSend));
		thisCl->v_dataToSend = tempDataPacket;	/*	will be freed after sending	*/
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
	thisCl->v_sentProcID = KS_SETVAR;

	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT ksxdr_xdrClient_processSetVar(
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
	{
		ksbase_free_KSDATAPACKET(&thisCl->v_dataReceived);
		return fncresult;
	}
	if(Ov_OK(*result))
	{
		fncresult = KS_DATAPACKET_read_xdr_array_tomemstack(&(thisCl->v_dataReceived), (void**) items_results, sizeof(OV_RESULT), items_length,
			(xdr_readfncptr) &KS_DATAPACKET_read_xdr_int);
	}
	else
	{
		fncresult = OV_ERR_OK;
	}
	if(Ov_Fail(fncresult) || (thisCl->v_dataReceived.readPT - thisCl->v_dataReceived.data >= thisCl->v_dataReceived.length))
	{
		ksbase_free_KSDATAPACKET(&thisCl->v_dataReceived);
	}
	return fncresult;
}
