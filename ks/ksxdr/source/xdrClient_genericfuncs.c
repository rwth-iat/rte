/*
 * xdrClient_genericfuncs.c
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
 * 				static Variable
 *******************************************************************************************************************************************************************************/
static OV_UINT client_xid = 0xd639a74c;	/*	some beating on the keyboard for initialization	*/


/*******************************************************************************************************************************************************************************
 * 				Channel-handling
 *******************************************************************************************************************************************************************************/

OV_RESULT ksxdr_getChannelPointer(OV_INSTPTR_ksxdr_xdrClient this, OV_INSTPTR_ksbase_Channel* ppChannel, OV_VTBLPTR_ksbase_Channel* ppVtblChannel)
{
	OV_STRING OptValTemp = NULL;
	OV_INSTPTR_ov_class pClassChannel = NULL;
	OV_RESULT result;

	/*	check if there is a channel and get pointer to it	*/
	*ppChannel = Ov_GetParent(ksbase_AssocChannelDataHandler, this);
	if(!(*ppChannel))
	{/*	no Channel found. Create one	*/
		/*
		 * Checking KSXDR_USECHANNEL and KS_USECHANNEL options to determine which channel type to use. use TCPChannel per default.
		 */
		ov_memstack_lock();
		OptValTemp = ov_vendortree_getcmdlineoption_value("KSXDR_USECHANNEL");
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
				result = ov_class_createobject(pClassChannel, Ov_StaticPtrCast(ov_domain, this), "channel", OV_PMH_DEFAULT, NULL, NULL, NULL, (OV_INSTPTR_ov_object*) ppChannel);
				if(Ov_Fail(result))
				{
					KS_logfile_error(("%s getChannelPointer: could not create channel for manager communication. reason: %s", this->v_identifier, ov_result_getresulttext(result)));
					ov_memstack_unlock();
					return result;
				}
				else
				{
					result = Ov_Link(ksbase_AssocChannelDataHandler, *ppChannel, this);
					if(Ov_Fail(result))
					{
						KS_logfile_error(("%s getChannelPointer: could not link with channel. reason: %s", this->v_identifier, ov_result_getresulttext(result)));
						ov_memstack_unlock();
						return result;
					}
					(*ppChannel)->v_ClientHandlerAssociated = KSBASE_CH_NOTNEEDED;

				}
			}
		}
		else
		{
			ov_memstack_unlock();
			*ppChannel = NULL;
			*ppVtblChannel = NULL;
			return OV_ERR_OK;
		}
		ov_memstack_unlock();
	}


	Ov_GetVTablePtr(ksbase_Channel, *ppVtblChannel, *ppChannel);
	if(!(*ppVtblChannel))
	{
		KS_logfile_error(("%s: getChannelPointer: could not get VTable of channel object. Aborting.", this->v_identifier));
		return OV_ERR_BADFACTORY;
	}
	return OV_ERR_OK;
}

/*******************************************************************************************************************************************************************************
 * 				Connection handling and sending
 ******************************************************************************************************************************************************************************/

/*	if there is no connection (open or opening) open one and reset lasteventtime	*/
OV_RESULT ksxdr_initiateConnection(OV_INSTPTR_ksxdr_xdrClient this, OV_INSTPTR_ksbase_Channel pChannel, OV_VTBLPTR_ksbase_Channel pVtblChannel, OV_BOOL isLocal, OV_STRING host, OV_STRING port)
{
	OV_RESULT result;

	if((pChannel->v_ConnectionState != KSBASE_CONNSTATE_OPEN) && (pChannel->v_ConnectionState != KSBASE_CONNSTATE_OPENING))
	{
		if(!port)
			return OV_ERR_BADPARAM;

		if(isLocal)
			result = pVtblChannel->m_OpenLocalConn(pChannel, port);
		else
		{
			if(!host)
				return OV_ERR_BADPARAM;
			result = pVtblChannel->m_OpenConnection(pChannel, host, port);
		}

		if(Ov_Fail(result))
		{
			KS_logfile_error(("%s: initiateConnection: could not open connection", this->v_identifier));
			return result;
		}
		ov_time_gettime(&(this->v_timeLastEvent));
	}
	return OV_ERR_OK;
}

/*	check if connection is open. if so, send and set Client state to busy and reset lasteventtime. if not set client state to awaiting connection. activate typemethod	*/
OV_RESULT ksxdr_trySend(OV_INSTPTR_ksxdr_xdrClient thisCl, OV_INSTPTR_ksbase_Channel pChannel, OV_VTBLPTR_ksbase_Channel pVtblChannel)
{
	OV_RESULT result = OV_ERR_OK;

	if(pChannel->v_ConnectionState == KSBASE_CONNSTATE_OPEN)
	{
		result = pVtblChannel->m_SendData(pChannel);
		if(Ov_Fail(result))
		{
			ksbase_free_KSDATAPACKET(&(thisCl->v_dataToSend));
			ksbase_free_KSDATAPACKET(&(pChannel->v_outData));
			return result;
		}
		ov_time_gettime(&(thisCl->v_timeLastEvent));
		thisCl->v_state = KSBASE_CLST_AWAITINGANSWER;	/*	set state to busy and activate typemethod	*/
		thisCl->v_actimode = 1;
	}
	else
	{
		thisCl->v_state = KSBASE_CLST_AWAITINGCONNECTION;
		thisCl->v_actimode = 1;
	}
	return OV_ERR_OK;
}
/*******************************************************************************************************************************************************************************
 * 				header
 ******************************************************************************************************************************************************************************/
/**
 * creation of rpc message header (call)
 * @param procedureNumber: procedure number to call
 * @param datapacket: datapacket to write into
 * @param xid: xid is returned (using a global counter)
 * @return	errorcode
 */
OV_RESULT ksxdr_generateClientMessageHeader(OV_UINT procedureNumber, OV_BOOL usesStreamProtocol, KS_DATAPACKET* datapacket, OV_UINT* xid, OV_UINT* pindex)
{
	OV_RESULT result;
	OV_UINT varToSet;

	if(!datapacket)
		return OV_ERR_BADPARAM;
	if(datapacket->data && datapacket->writePT)
		*pindex = (datapacket->writePT - datapacket->data);
	else
		*pindex = 0;

	if(usesStreamProtocol)
	{
		/*	reserve space for rpc-header	*/
		varToSet = 0x80000000;
		result = KS_DATAPACKET_write_xdr_uint(datapacket, &varToSet);
		if(Ov_Fail(result))
			return result;
	}

	varToSet = client_xid++;	/*	write xid (and increment variable)	*/
	result = KS_DATAPACKET_write_xdr_uint(datapacket, &varToSet);
	if(Ov_Fail(result))
		return result;
	*xid = varToSet;

	varToSet = KSXDR_MSGTYPE_CALL;
	result = KS_DATAPACKET_write_xdr_uint(datapacket, &varToSet);
	if(Ov_Fail(result))
		return result;

	varToSet = KSXDR_RPCVERSION;
	result = KS_DATAPACKET_write_xdr_uint(datapacket, &varToSet);
	if(Ov_Fail(result))
		return result;

	varToSet = KSXDR_PROGRAMNUMBER;
	result = KS_DATAPACKET_write_xdr_uint(datapacket, &varToSet);
	if(Ov_Fail(result))
		return result;

	varToSet = KSXDR_PROGRAMVERSION;
	result = KS_DATAPACKET_write_xdr_uint(datapacket, &varToSet);
	if(Ov_Fail(result))
		return result;

	result = KS_DATAPACKET_write_xdr_uint(datapacket, &procedureNumber);
	if(Ov_Fail(result))
		return result;

	/*	rpc auth part (not used)	*/
	varToSet = XDR_AUTH_NONE;
	result = KS_DATAPACKET_write_xdr_uint(datapacket, &varToSet);
	if(Ov_Fail(result))
		return result;

	result = KS_DATAPACKET_write_xdr_opaque(datapacket, NULL, 0);
	if(Ov_Fail(result))
		return result;

	varToSet = XDR_AUTH_NONE;
	result = KS_DATAPACKET_write_xdr_uint(datapacket, &varToSet);
	if(Ov_Fail(result))
		return result;

	result = KS_DATAPACKET_write_xdr_opaque(datapacket, NULL, 0);
	if(Ov_Fail(result))
		return result;

	return OV_ERR_OK;
}

/*****************************************************************************************************************************************************************************
 * 						Tickets
 *****************************************************************************************************************************************************************************/
OV_RESULT ksxdr_insertTicket(KS_DATAPACKET* datapacket, const OV_INSTPTR_ksbase_ClientTicketGenerator TicketGenerator)
{
	OV_UINT varToSet;
	OV_VTBLPTR_ksbase_ClientTicketGenerator pVtblTicketGen = NULL;
	OV_RESULT result;

	if(!TicketGenerator)	/*	use none ticket if no Generator specified	*/
	{
		varToSet = OV_TT_NONE;
		result = KS_DATAPACKET_write_xdr_uint(datapacket, &varToSet);
		if(Ov_Fail(result))
			return result;
	}
	else
	{
		result = KS_DATAPACKET_write_xdr_OV_TICKET_TYPE(datapacket, &(TicketGenerator->v_TicketType));
		if(Ov_Fail(result))
			return result;
		Ov_GetVTablePtr(ksbase_ClientTicketGenerator, pVtblTicketGen, TicketGenerator);
		if(!pVtblTicketGen)
			return OV_ERR_GENERIC;

		return pVtblTicketGen->m_encodeTicket(TicketGenerator, datapacket);
	}
	return OV_ERR_OK;
}

OV_RESULT ksxdr_readBackTicket(KS_DATAPACKET* datapacket, const OV_INSTPTR_ksbase_ClientTicketGenerator TicketGenerator)
{
	OV_RESULT result;
	OV_INT TicketType;
	OV_VTBLPTR_ksbase_ClientTicketGenerator pVtblTicketGen = NULL;

	result = KS_DATAPACKET_read_xdr_int(datapacket, &TicketType);
	if(Ov_Fail(result))
		return result;

	if(!TicketGenerator)
		if( TicketType == OV_TT_NONE)
			return OV_ERR_OK;
		else
			return OV_ERR_GENERIC;
	else
	{
		if(TicketGenerator->v_TicketType != TicketType)
			return OV_ERR_BADAUTH;
		else
		{
			Ov_GetVTablePtr(ksbase_ClientTicketGenerator, pVtblTicketGen, TicketGenerator);
			if(!pVtblTicketGen)
				return OV_ERR_GENERIC;
			return pVtblTicketGen->m_decodeReply(TicketGenerator, datapacket);
		}
	}
}

/*************************************************************************************************************************************************************
 * 	decode header of replies (params are not checked for NULL-pointers)
 *************************************************************************************************************************************************************/

OV_RESULT ksxdr_processServerReplyHeader(KS_DATAPACKET* datapacket, const OV_INSTPTR_ksbase_ClientTicketGenerator TicketGenerator,
		OV_UINT expectedXID, OV_BOOL usesStreamProtocol, OV_UINT* xid, OV_INT* msgAccepted, OV_INT* msgStatus, OV_UINT* functionStatus)
{
	OV_VTBLPTR_ksbase_ClientTicketGenerator	pVtblTicketGenerator = NULL;
	OV_RESULT								result;
	OV_UINT									dummy;
	OV_BYTE*								BeginOfMessage;
	OV_UINT									header;
	OV_UINT									ticketindicator;

	*functionStatus = XDRCL_PROCANS_OK;
	BeginOfMessage = datapacket->readPT;

	if(usesStreamProtocol)
	{
		/*	header	*/
		result = KS_DATAPACKET_read_xdr_uint(datapacket, &header);
		if(Ov_Fail(result))
			return result;
	}

	/*	xid	*/
	result = KS_DATAPACKET_read_xdr_uint(datapacket, xid);
	if(Ov_Fail(result))
		return result;

	if(*xid != expectedXID)
	{
		*functionStatus = XDRCL_PROCANS_XIDMISSMATCH;
		KS_logfile_error(("xdrClient: xid (%lu) of received xdr does not match xid (%lu) of last request.", *xid, expectedXID));
		return OV_ERR_BADVALUE;
	}
	/*	messageType	*/
	result = KS_DATAPACKET_read_xdr_uint(datapacket, &dummy);
	if(Ov_Fail(result))
		return result;
	if(dummy != KSXDR_MSGTYPE_REPLY)
	{
		KS_logfile_error(("xdrClient: received message is not a reply (type is: %0#4x)", dummy));
		return OV_ERR_BADVALUE;
	}
	/***************************************************************************************************************************************************************************
	 * checking if request is in the buffer completely, if not, return ERR_OK and wait until it is complete. While waiting set the receive timeout value to a few seconds
	 * 	(we received a fragmented package so there will come more data. if no data arrives within a second, we can assume the package is broken)
	 ***************************************************************************************************************************************************************************/
	if(usesStreamProtocol == TRUE)
	{
		if(!bufferHoldsCompleteRequest(datapacket, BeginOfMessage, &result))
		{
			if(Ov_Fail(result))
				return result;
			*functionStatus = XDRCL_PROCANS_INCOMPLETE;
			datapacket->readPT = BeginOfMessage; 	/*	reset the read pointer to the place where the message began (to be read again next time)	*/
			return OV_ERR_BADVALUE;		/*	get called again to process the request next time (if it is complete then).
													Yes, this could block the ClientHandler for a longer time.	*/
		}

		/*
		 * unfragment large xdrs
		 */
		if(!(header & (0x80000000)))	/*	a 0x80 in the first byte means this is the last (or only in this case) fragment. 	*/
		{
			result = unfragmentXDRmessage(datapacket, BeginOfMessage);
			if(Ov_Fail(result))
				return result;

		}
	}


	/*	acceptance	*/
	result = KS_DATAPACKET_read_xdr_int(datapacket, msgAccepted);
	if(Ov_Fail(result))
		return result;
	if(*msgAccepted != XDR_MSG_ACCEPTED)
	{
		/*	rejected decode reason	*/
		/*	reason	*/
		result = KS_DATAPACKET_read_xdr_int(datapacket, msgStatus);
		if(Ov_Fail(result))
			return result;

		if(*msgStatus == XDR_DEN_AUTH_ERROR)
		{/*	rejection because of bad authentication
		 	 throw away 4 bytes (reason) --> we only know generic reasons*/
			datapacket->readPT += 4;

		}
		else
		{/*	rejection because of rpc mismatch --> throw away 8 bytes (version codes)	*/
			datapacket->readPT += 8;
		}
	}

	/*	message accepted	*/
	/*	auth verifier has to be no auth*/
	result = KS_DATAPACKET_read_xdr_uint(datapacket, &dummy);
	if(Ov_Fail(result))
		return result;
	if(dummy)
	{
		KS_logfile_error(("xdrClient: received message with auth-type not NONE (type is: %0#4x)", dummy));
		return OV_ERR_BADVALUE;
	}

	result = KS_DATAPACKET_read_xdr_uint(datapacket, &dummy);	/*	no auth data following (length 0)	*/
	if(Ov_Fail(result))
		return result;
	if(dummy)
	{
		KS_logfile_error(("xdrClient: received message with authentication data of length %u", dummy));
		return OV_ERR_BADAUTH;
	}

	/*	message state	*/
	result = KS_DATAPACKET_read_xdr_int(datapacket, msgStatus);
	if(Ov_Fail(result))
		return result;

	if(*msgStatus != XDR_MSGST_SUCCESS)
	{	/*	processing failed	*/
		if(*msgStatus == XDR_MSGST_PROG_MISMATCH)
		{/*rejection because of program version mismatch --> throw away 8 bytes (version numbers)	*/
			datapacket->readPT += 8;
		}
		return OV_ERR_OK;
	}

	/*	handle tickets (call readBack)	*/
	result = KS_DATAPACKET_read_xdr_uint(datapacket, &ticketindicator);
	if(Ov_Fail(result))
		return result;
	if(ticketindicator)	/*	TicketType != none	*/
	{
		if(!TicketGenerator)
		{
			KS_logfile_error(("processing reply header: found a ticket but no TicketGenerator associated."));
			return OV_ERR_GENERIC;
		}
		Ov_GetVTablePtr(ksbase_ClientTicketGenerator, pVtblTicketGenerator, TicketGenerator);
		if(!pVtblTicketGenerator)
		{
			KS_logfile_error(("processing reply header. ticket part. could not get VtablePointer of %s", TicketGenerator->v_identifier));
			return OV_ERR_GENERIC;
		}
		return pVtblTicketGenerator->m_decodeReply(TicketGenerator, datapacket);
	}

	/*	global part finished	*/
	return OV_ERR_OK;
}

void GetServerCallback(const OV_INSTPTR_ov_domain this, const OV_INSTPTR_ov_domain that)
{
	//OV_INSTPTR_ksxdr_xdrClient callingCl = Ov_StaticPtrCast(ksxdr_xdrClient, this);
	//OV_INSTPTR_ksxdr_xdrClient callbackOwner = Ov_StaticPtrCast(ksxdr_xdrClient, that);

	//OV_RESULT result;


}

