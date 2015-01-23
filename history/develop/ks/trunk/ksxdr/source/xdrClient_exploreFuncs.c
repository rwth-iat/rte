/*
 * xdrClient_exploreFuncs.c
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
 * 				getPP
 ******************************************************************************************************************************************************************************/

OV_DLLFNCEXPORT OV_RESULT ksxdr_xdrClient_requestGetPP(
		const OV_INSTPTR_ksbase_ClientBase this,
		const OV_INSTPTR_ksbase_ClientTicketGenerator TicketGenerator,
		const OV_STRING path,
		const OV_OBJ_TYPE typeMask,
		const OV_STRING	nameMask,
		const OV_UINT dummy,
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
		result = ksxdr_generateClientMessageHeader(KS_GETPP, pChannel->v_usesStreamProtocol, &(tempDataPacket), &(thisCl->v_tempXID), &msgindex);
		if(Ov_Fail(result))
			break;

		/*	insert Ticket	*/
		result = ksxdr_insertTicket(&(tempDataPacket), TicketGenerator);
		if(Ov_Fail(result))
			break;

		/*	path	*/
		result =  KS_DATAPACKET_write_xdr_string(&(tempDataPacket), &path);
		if(Ov_Fail(result))
			break;

		/*	typemask	*/
		result = KS_DATAPACKET_write_xdr_OV_OBJ_TYPE(&(tempDataPacket), &typeMask);
		if(Ov_Fail(result))
			break;

		/*	namemask	*/
		result =  KS_DATAPACKET_write_xdr_string(&(tempDataPacket), &nameMask);
		if(Ov_Fail(result))
			break;

		/*	dummy	*/
		result = KS_DATAPACKET_write_xdr_uint(&(tempDataPacket), &dummy);
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
	thisCl->v_sentProcID = KS_GETPP;

	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT ksxdr_xdrClient_processGetPP(
		const OV_INSTPTR_ksbase_ClientBase this,
		const OV_INSTPTR_ksbase_ClientTicketGenerator TicketGenerator,
		OV_RESULT* result,
		OV_UINT* items_len,
		OV_OBJ_PROJECTED_PROPS** items_val
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
	{
		fncresult = KS_DATAPACKET_read_xdr_array_tomemstack(&(thisCl->v_dataReceived), (void**) items_val, sizeof(OV_OBJ_PROJECTED_PROPS), items_len,
				(xdr_readfncptr) &xdr_read_OV_OBJ_PROJECTED_PROPS);
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
 * 				getEP
 ******************************************************************************************************************************************************************************/

OV_DLLFNCEXPORT OV_RESULT ksxdr_xdrClient_requestGetEP(
		const OV_INSTPTR_ksbase_ClientBase this,
		const OV_INSTPTR_ksbase_ClientTicketGenerator TicketGenerator,
		const OV_STRING path,
		const OV_OBJ_TYPE typeMask,
		const OV_STRING	nameMask,
		const OV_EP_FLAGS scopeFlags,
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
		result = ksxdr_generateClientMessageHeader(KS_GETEP, pChannel->v_usesStreamProtocol, &(tempDataPacket), &(thisCl->v_tempXID), &msgindex);
		if(Ov_Fail(result))
			break;

		/*	insert Ticket	*/
		result = ksxdr_insertTicket(&(tempDataPacket), TicketGenerator);
		if(Ov_Fail(result))
			break;

		/*	path	*/
		result =  KS_DATAPACKET_write_xdr_string(&(tempDataPacket), &path);
		if(Ov_Fail(result))
			break;

		/*	typemask	*/
		result = KS_DATAPACKET_write_xdr_OV_OBJ_TYPE(&(tempDataPacket), &typeMask);
		if(Ov_Fail(result))
			break;

		/*	namemask	*/
		result =  KS_DATAPACKET_write_xdr_string(&(tempDataPacket), &nameMask);
		if(Ov_Fail(result))
			break;

		/*	scopeFlags	*/
		result = KS_DATAPACKET_write_xdr_OV_EP_FLAGS(&(tempDataPacket), &scopeFlags);
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
	thisCl->v_sentProcID = KS_GETEP;

	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT ksxdr_xdrClient_processGetEP(
		const OV_INSTPTR_ksbase_ClientBase this,
		const OV_INSTPTR_ksbase_ClientTicketGenerator TicketGenerator,
		OV_RESULT* result,
		OV_UINT* items_len,
		OV_OBJ_ENGINEERED_PROPS** items_val
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
	{
		fncresult = KS_DATAPACKET_read_xdr_array_tomemstack(&(thisCl->v_dataReceived), (void**) items_val, sizeof(OV_OBJ_ENGINEERED_PROPS), items_len,
				(xdr_readfncptr) &xdr_read_OV_OBJ_ENGINEERED_PROPS);
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
 * 				getCanonicalPath
 ******************************************************************************************************************************************************************************/

OV_DLLFNCEXPORT OV_RESULT ksxdr_xdrClient_requestGetCanonicalPath(
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
		result = ksxdr_generateClientMessageHeader(KS_GETCANONICALPATH, pChannel->v_usesStreamProtocol, &(tempDataPacket), &(thisCl->v_tempXID), &msgindex);
		if(Ov_Fail(result))
			break;

		/*	insert Ticket	*/
		result = ksxdr_insertTicket(&(tempDataPacket), TicketGenerator);
		if(Ov_Fail(result))
			break;

		/*	paths	*/
		result =  KS_DATAPACKET_write_xdr_array(&(tempDataPacket), (void**) paths, sizeof(OV_STRING), (OV_UINT*) &items_length,
				(xdr_writefncptr) &KS_DATAPACKET_write_xdr_string);
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
	thisCl->v_sentProcID = KS_GETEP;

	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT ksxdr_xdrClient_processGetCanonicalPath(
		const OV_INSTPTR_ksbase_ClientBase this,
		const OV_INSTPTR_ksbase_ClientTicketGenerator TicketGenerator,
		OV_RESULT* result,
		OV_UINT* items_length,
		OV_RESULT* items_results,
		OV_STRING** items_canonicalPaths
) {
	/*
	 *   local variables
	 */
	OV_INSTPTR_ksxdr_xdrClient	thisCl = Ov_StaticPtrCast(ksxdr_xdrClient, this);
	OV_RESULT fncresult;
	OV_UINT i;

	fncresult = KS_DATAPACKET_read_xdr_OV_RESULT(&(thisCl->v_dataReceived), result);
	if(Ov_Fail(fncresult))
		return fncresult;
	if(Ov_OK(*result))
	{
		fncresult = KS_DATAPACKET_read_xdr_uint(&(thisCl->v_dataReceived), items_length);
		if(Ov_Fail(fncresult))
		{
			ksbase_free_KSDATAPACKET(&thisCl->v_dataReceived);
			return fncresult;
		}

		items_results = ov_memstack_alloc(*items_length * sizeof(OV_RESULT));
		if(!items_results)
		{
			ksbase_free_KSDATAPACKET(&thisCl->v_dataReceived);
			return OV_ERR_HEAPOUTOFMEMORY;
		}
		*items_canonicalPaths = ov_memstack_alloc(*items_length * sizeof(OV_STRING));
		if(!(*items_canonicalPaths))
		{
			ksbase_free_KSDATAPACKET(&thisCl->v_dataReceived);
			return OV_ERR_HEAPOUTOFMEMORY;
		}
		for(i=0; i<*items_length; i++)
		{
			fncresult = KS_DATAPACKET_read_xdr_OV_RESULT(&(thisCl->v_dataReceived), &(items_results[i]));
			if(Ov_Fail(fncresult))
			{
				ksbase_free_KSDATAPACKET(&thisCl->v_dataReceived);
				return fncresult;
			}
			fncresult = KS_DATAPACKET_read_xdr_string_tomemstack_wolength(&(thisCl->v_dataReceived), (items_canonicalPaths[i]));
			if(Ov_Fail(fncresult))
			{
				ksbase_free_KSDATAPACKET(&thisCl->v_dataReceived);
				return fncresult;
			}
		}

	}
	if((thisCl->v_dataReceived.readPT - thisCl->v_dataReceived.data >= thisCl->v_dataReceived.length))
	{
		ksbase_free_KSDATAPACKET(&thisCl->v_dataReceived);
	}
	return OV_ERR_OK;
}

