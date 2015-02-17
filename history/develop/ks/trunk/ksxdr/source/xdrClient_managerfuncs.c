/*
 * xdrClient_managerfuncs.c
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
 * 				register
 ******************************************************************************************************************************************************************************/

OV_DLLFNCEXPORT OV_RESULT ksxdr_xdrClient_requestRegister(
		const OV_INSTPTR_ksbase_ClientBase this,
		const OV_INSTPTR_ksbase_ClientTicketGenerator TicketGenerator,
		const OV_STRING servername,
		const OV_UINT	version,
		const OV_UINT port,
		const OV_UINT ttl,
		const OV_INSTPTR_ov_domain	callbackThat,
		void (*callback)(const OV_INSTPTR_ov_domain this, const OV_INSTPTR_ov_domain that)
) {
	/*
	 *   local variables
	 */
	OV_RESULT result;
	OV_UINT varToSet;
	OV_UINT msgindex = 0;
	OV_INSTPTR_ksxdr_xdrClient thisCl = Ov_StaticPtrCast(ksxdr_xdrClient, this);
	OV_INSTPTR_ksbase_Channel	pChannel = NULL;
	OV_VTBLPTR_ksbase_Channel	pVtblChannel = NULL;

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

	do{	/*	using a do...while(0) to run this block once and be able to use break-directives to jump out when an error occurs	*/

		/*	generate Header	*/
		result = ksxdr_generateClientMessageHeader(KS_REGISTER, pChannel->v_usesStreamProtocol, &(pChannel->v_outData), &(thisCl->v_sentXID), &msgindex);
		if(Ov_Fail(result))
			break;

		/*	insert Ticket, for register, we use always the none-ticket	*/
		result = ksxdr_insertTicket(&(pChannel->v_outData), TicketGenerator);
		if(Ov_Fail(result))
			break;

		/*	name	*/
		result = KS_DATAPACKET_write_xdr_string(&(pChannel->v_outData), &servername);
		if(Ov_Fail(result))
			break;

		/*	lowest needed version	*/
		result = KS_DATAPACKET_write_xdr_uint(&(pChannel->v_outData), &version);
		if(Ov_Fail(result))
			break;

		/*	port	*/
		result = KS_DATAPACKET_write_xdr_uint(&(pChannel->v_outData), &port);
		if(Ov_Fail(result))
			break;

		/*	time to live	*/
		varToSet = 30;	/*	30 seconds ttl	*/
		result = KS_DATAPACKET_write_xdr_uint(&(pChannel->v_outData), &varToSet);
		if(Ov_Fail(result))
			break;
		/*	set length (if necessary)	*/
		if(pChannel->v_usesStreamProtocol)
			ksxdr_prepend_length(&(pChannel->v_outData), msgindex);

	}while(0);
	if(Ov_Fail(result))
	{
		ksbase_free_KSDATAPACKET(&(pChannel->v_outData));
		return result;
	}

	/*	send created Message	*/
	result = ksxdr_initiateConnection(thisCl, pChannel, pVtblChannel, TRUE, NULL, thisCl->v_ManagerPort);
	if(Ov_Fail(result))
	{
		ksbase_free_KSDATAPACKET(&(pChannel->v_outData));
		return result;
	}

	result = ksxdr_trySend(thisCl, pChannel, pVtblChannel);
	if(Ov_Fail(result))
		return result;

	thisCl->v_actimode = 1;

	thisCl->v_sentProcID = KS_REGISTER;

	return OV_ERR_OK;

}

OV_DLLFNCEXPORT OV_RESULT ksxdr_xdrClient_processRegister(
		const OV_INSTPTR_ksbase_ClientBase this,
		const OV_INSTPTR_ksbase_ClientTicketGenerator TicketGenerator,
		OV_RESULT* result
) {
	OV_INSTPTR_ksxdr_xdrClient thisCl = Ov_StaticPtrCast(ksxdr_xdrClient, this);
	OV_RESULT fncresult;
	fncresult = KS_DATAPACKET_read_xdr_OV_RESULT(&(thisCl->v_dataReceived), result);
	if(Ov_Fail(fncresult) || (thisCl->v_dataReceived.readPT - thisCl->v_dataReceived.data >= (OV_INT) thisCl->v_dataReceived.length))
	{
		ksbase_free_KSDATAPACKET(&thisCl->v_dataReceived);
	}
	return fncresult;
}

/*******************************************************************************************************************************************************************************
 * 				un-register
 ******************************************************************************************************************************************************************************/

OV_DLLFNCEXPORT OV_RESULT ksxdr_xdrClient_requestUnRegister(
		const OV_INSTPTR_ksbase_ClientBase this,
		const OV_INSTPTR_ksbase_ClientTicketGenerator TicketGenerator,
		const OV_STRING servername,
		const OV_UINT version,
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

	do{	/*	using a do...while(0) to run this block once and be able to use break-directives to jump out when an error occurs	*/

		/*	generate Header	*/
		result = ksxdr_generateClientMessageHeader(KS_UNREGISTER, pChannel->v_usesStreamProtocol, &(pChannel->v_outData), &(thisCl->v_sentXID), &msgindex);
		if(Ov_Fail(result))
			break;

		/*	insert Ticket, for register, we use always the none-ticket	*/
		result = ksxdr_insertTicket(&(pChannel->v_outData), TicketGenerator);
		if(Ov_Fail(result))
			break;

		/*	name	*/
		result = KS_DATAPACKET_write_xdr_string(&(pChannel->v_outData), &servername);
		if(Ov_Fail(result))
			break;

		/*	server version	*/
		result =  KS_DATAPACKET_write_xdr_uint(&(pChannel->v_outData), &version);
		if(Ov_Fail(result))
			break;

		/*	set length (if necessary)	*/
		if(pChannel->v_usesStreamProtocol)
			ksxdr_prepend_length(&(pChannel->v_outData), msgindex);

	}while(0);
	if(Ov_Fail(result))
	{
		ksbase_free_KSDATAPACKET(&(pChannel->v_outData));
		return result;
	}

	/*	send created Message	*/
	result = ksxdr_initiateConnection(thisCl, pChannel, pVtblChannel, TRUE, NULL, thisCl->v_ManagerPort);
	if(Ov_Fail(result))
	{
		ksbase_free_KSDATAPACKET(&(pChannel->v_outData));
		return result;
	}

	result = ksxdr_trySend(thisCl, pChannel, pVtblChannel);
	if(Ov_Fail(result))
		return result;

	thisCl->v_actimode = 1;

	/*	set ProcID	*/
	thisCl->v_sentProcID = KS_UNREGISTER;

	return OV_ERR_OK;

}

OV_DLLFNCEXPORT OV_RESULT ksxdr_xdrClient_processUnRegister(
		const OV_INSTPTR_ksbase_ClientBase this,
		const OV_INSTPTR_ksbase_ClientTicketGenerator TicketGenerator,
		OV_RESULT* result
) {
	/*
	 *   local variables
	 */

	OV_INSTPTR_ksxdr_xdrClient thisCl = Ov_StaticPtrCast(ksxdr_xdrClient, this);
	OV_RESULT fncresult;
	fncresult = KS_DATAPACKET_read_xdr_OV_RESULT(&(thisCl->v_dataReceived), result);
	if(Ov_Fail(fncresult) || (thisCl->v_dataReceived.readPT - thisCl->v_dataReceived.data >= (OV_INT) thisCl->v_dataReceived.length))
	{
		ksbase_free_KSDATAPACKET(&thisCl->v_dataReceived);
	}
	return fncresult;
}

/*******************************************************************************************************************************************************************************
 * 				getserver
 ******************************************************************************************************************************************************************************/

OV_DLLFNCEXPORT OV_RESULT ksxdr_xdrClient_requestGetServer(
		const OV_INSTPTR_ksbase_ClientBase this,
		const OV_INSTPTR_ksbase_ClientTicketGenerator TicketGenerator,
		const OV_STRING servername,
		const OV_UINT leastVersion,
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

	do{	/*	using a do...while(0) to run this block once and be able to use break-directives to jump out when an error occurs	*/

		/*	generate Header	*/
		result = ksxdr_generateClientMessageHeader(KS_GETSERVER, pChannel->v_usesStreamProtocol, &(pChannel->v_outData), &(thisCl->v_sentXID), &msgindex);
		if(Ov_Fail(result))
			break;

		/*	insert Ticket, for register, we use always the none-ticket	*/
		result = ksxdr_insertTicket(&(pChannel->v_outData), TicketGenerator);
		if(Ov_Fail(result))
			break;

		/*	name	*/
		result = KS_DATAPACKET_write_xdr_string(&(pChannel->v_outData), &servername);
		if(Ov_Fail(result))
			break;

		/*	least server version	*/
		result =  KS_DATAPACKET_write_xdr_uint(&(pChannel->v_outData), &leastVersion);
		if(Ov_Fail(result))
			break;

		/*	set length (if necessary)	*/
		if(pChannel->v_usesStreamProtocol)
			ksxdr_prepend_length(&(pChannel->v_outData), msgindex);

	}while(0);
	if(Ov_Fail(result))
	{
		ksbase_free_KSDATAPACKET(&(pChannel->v_outData));
		return result;
	}
	result = ksxdr_checkPMAskList( thisCl);
	if(Ov_Fail(result)){
		KS_logfile_error(("%s: could not determine MANAGER port in PortMapList", thisCl->v_identifier));
		ksbase_free_KSDATAPACKET(&(pChannel->v_outData));
		return result;
	}
	/*	send created Message	*/
	result = ksxdr_initiateConnection(thisCl, pChannel, pVtblChannel, FALSE, thisCl->v_serverHost, thisCl->v_ManagerPort);
	if(Ov_Fail(result))
	{
		ksbase_free_KSDATAPACKET(&(pChannel->v_outData));
		return result;
	}

	result = ksxdr_trySend(thisCl, pChannel, pVtblChannel);
	if(Ov_Fail(result))
		return result;

	thisCl->v_actimode = 1;

	/*	set ProcID	*/
	thisCl->v_sentProcID = KS_GETSERVER;

	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT ksxdr_xdrClient_processGetServer(
		const OV_INSTPTR_ksbase_ClientBase this,
		const OV_INSTPTR_ksbase_ClientTicketGenerator TicketGenerator,
		OV_RESULT* result,
		OV_STRING* servername,
		OV_UINT* regVersion,
		OV_STRING* port,
		OV_TIME* expirationTime,
		OV_UINT* serverState
) {
	/*
	 *   local variables
	 */
	OV_RESULT fncresult = 0;
	OV_UINT xdr_port = 0;
	OV_INSTPTR_ksxdr_xdrClient thisCl = Ov_StaticPtrCast(ksxdr_xdrClient, this);

	fncresult = KS_DATAPACKET_read_xdr_OV_RESULT(&(thisCl->v_dataReceived), result);
	if(Ov_Fail(fncresult))
		return fncresult;

	/*	servername	*/
	fncresult = KS_DATAPACKET_read_xdr_string_tomemstack_wolength(&(thisCl->v_dataReceived), servername);
	if(Ov_Fail(fncresult))
		return fncresult;
	/*	version	*/
	fncresult = KS_DATAPACKET_read_xdr_uint(&(thisCl->v_dataReceived), regVersion);
	if(Ov_Fail(fncresult))
		return fncresult;
	/*	port	*/
	fncresult = KS_DATAPACKET_read_xdr_uint(&(thisCl->v_dataReceived), &xdr_port);
	if(Ov_Fail(fncresult))
		return fncresult;
	*port = ov_memstack_alloc(12 * sizeof(char));
	if(!(*port))
		return OV_ERR_HEAPOUTOFMEMORY;
	sprintf(*port, "%" OV_PRINT_UINT, xdr_port);

	/*	Expiration Time	*/
	fncresult = KS_DATAPACKET_read_xdr_uint(&(thisCl->v_dataReceived), &(expirationTime->secs));
	if(Ov_Fail(fncresult))
		return fncresult;
	fncresult = KS_DATAPACKET_read_xdr_uint(&(thisCl->v_dataReceived), &(expirationTime->usecs));
	if(Ov_Fail(fncresult))
		return fncresult;

	/*	state	*/
	fncresult = KS_DATAPACKET_read_xdr_uint(&(thisCl->v_dataReceived), serverState);
	if(Ov_Fail(fncresult) || (thisCl->v_dataReceived.readPT - thisCl->v_dataReceived.data >= (OV_INT) thisCl->v_dataReceived.length))
	{
		ksbase_free_KSDATAPACKET(&thisCl->v_dataReceived);
	}
	return fncresult;
}


