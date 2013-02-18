
/******************************************************************************
*
*   FILE
*   ----
*   xdrClientHandler.c
*
*   History
*   -------
*   2013-01-29   File created
*
*******************************************************************************
*
*   This file is generated by the 'acplt_builder' command
*
******************************************************************************/


#ifndef OV_COMPILE_LIBRARY_ksxdr
#define OV_COMPILE_LIBRARY_ksxdr
#endif


#include "ksxdr.h"
#include "ksxdr_config.h"
#include "libov/ov_macros.h"
#include "libov/ov_path.h"
#include "libov/ov_memstack.h"
#include "libov/ov_result.h"
#include "ks_logfile.h"
#include "KSDATAPACKET_xdrhandling.h"
#include "ov_ksserver_backend.h"
#include "ksbase_helper.h"
#include "ksxdr_services.h"


OV_DLLFNCEXPORT void ksxdr_xdrClientHandler_startup(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */

    /* do what the base class does first */
    ksbase_ClientHandler_startup(pobj);

    /* do what */


    return;
}

OV_DLLFNCEXPORT void ksxdr_xdrClientHandler_shutdown(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */

    /* do what */

    /* set the object's state to "shut down" */
    ksbase_ClientHandler_shutdown(pobj);

    return;
}

void ksxdr_prepend_length(KS_DATAPACKET* answer, OV_UINT begin)
{
	unsigned int i;
	unsigned int temp;
	if(answer && answer->data && answer->length)
	{
		temp = answer->length - 4;
		for(i=0; i<4; i++)
			answer->data[begin+3-i] = ((OV_BYTE*)&temp)[i];
		answer->data[begin] = 0x80;
	}
	return;
}

OV_RESULT ksxdr_create_global_answer(KS_DATAPACKET* answer, OV_UINT xid, OV_UINT msgAccepted, OV_UINT msgState, OV_UINT* beginAnswer)
{
	OV_RESULT result;
	OV_UINT dummy;

	/*	reserver space for length (first 4 bytes in xdr 0x80xxxxxx) length = xxxxxx	*/
	dummy = 0x80;
	result = KS_DATAPACKET_write_xdr_u_long(answer, &dummy);
	if(Ov_Fail(result))
			return result;
	/*	save index of first byte of answer in packet in beginAnswer	*/
	*beginAnswer = ((answer->writePT - answer->data) - 4);

	/*	xid is always here	*/
	result = KS_DATAPACKET_write_xdr_u_long(answer, &xid);
	if(Ov_Fail(result))
		return result;

		/*	with this function we always put together a reply	*/
	dummy = KSXDR_MSGTYPE_REPLY;
	result = KS_DATAPACKET_write_xdr_u_long(answer, &dummy);
	if(Ov_Fail(result))
		return result;

	/*	acceptance	*/
	result = KS_DATAPACKET_write_xdr_u_long(answer, &msgAccepted);
	if(Ov_Fail(result))
		return result;

	if(msgAccepted != XDR_MSG_ACCEPTED)
	{	/*	rejected add reason	*/
		/*	reason	*/
		result = KS_DATAPACKET_write_xdr_u_long(answer, &msgState);
		if(Ov_Fail(result))
			return result;

		if(msgState == XDR_DEN_AUTH_ERROR)
		{/*	rejection because of bad authentication --> generic reason	*/
			dummy = XDR_AUTH_FAILED;
			result = KS_DATAPACKET_write_xdr_u_long(answer, &dummy);
			if(Ov_Fail(result))
				return result;

		}
		else
		{/*	rejection because of rpc mismatch --> return highest and lowest supported version (here both 2)	*/
			dummy = 0x02;
			result = KS_DATAPACKET_write_xdr_u_long(answer, &dummy);
			if(Ov_Fail(result))
				return result;

			result = KS_DATAPACKET_write_xdr_u_long(answer, &dummy);
			if(Ov_Fail(result))
				return result;

		}
		/*	rejection message finished	*/
		return OV_ERR_OK;
	}

	/*	message accepted	*/
	/*	auth verifier	--> no auth*/
	dummy = XDR_AUTH_NONE;
	result = KS_DATAPACKET_write_xdr_u_long(answer, &dummy);
	if(Ov_Fail(result))
		return result;

	dummy = 0;
	result = KS_DATAPACKET_write_xdr_u_long(answer, &dummy);	/*	no auth data following (length 0)	*/
	if(Ov_Fail(result))
		return result;

	/*	message state	*/
	result = KS_DATAPACKET_write_xdr_u_long(answer, &msgState);
	if(Ov_Fail(result))
		return result;

	if(msgState != XDR_MSGST_SUCCESS)
	{	/*	processing failed	*/
		if(msgState == XDR_MSGST_PROG_MISMATCH)
		{/*rejection because of program verion mismatch -->return highest and lowest supported ks-version (here both 2)	*/
			dummy = 0x02;
			result = KS_DATAPACKET_write_xdr_u_long(answer, &dummy);
			if(Ov_Fail(result))
				return result;
			result = KS_DATAPACKET_write_xdr_u_long(answer, &dummy);
			if(Ov_Fail(result))
				return result;
		}
	}
		/*	global part finished	*/
	return OV_ERR_OK;
}

/**
 * This function handles requests received over a channel. It reads data as an xdr-stream and triggers the appropriate OV-functions
 */

OV_DLLFNCEXPORT OV_RESULT ksxdr_xdrClientHandler_HandleRequest(
	OV_INSTPTR_ksbase_ClientHandler this,
	KS_DATAPACKET* dataReceived,
	KS_DATAPACKET* answer
) {
    /*    
    *   local variables
    */
	OV_RESULT result;
	OV_UINT msgAccepted = XDR_MSG_ACCEPTED;	/* Acceptance state for message*/
	OV_UINT	msgState = XDR_MSGST_SUCCESS;	/* Error code for rpc layer*/
	OV_UINT ksErrCode = KS_ERR_OK;			/* Error code on ks layer*/
	OV_UINT header;
	OV_UINT xid;
	OV_UINT messageType;
	OV_UINT rpcVersion;
	OV_UINT progId;
	OV_UINT ProgVersion;
	OV_UINT procedure;
	OV_UINT authflavour = 0;					/*also used as ticketindicator*/
	OV_STRING authdata = NULL;
	OV_UINT decodeLength = 0;
	OV_TICKET noneticket = {&noneticketvtbl, OV_TT_NONE};
	OV_TICKET* pticket = NULL;
	OV_INSTPTR_ksbase_TicketAuthenticator pTicketAuth = NULL;
	OV_INSTPTR_ov_domain pDomAuthenticators	= NULL;
	KS_DATAPACKET serviceAnswer = {0, NULL, NULL, NULL};
	OV_UINT beginAnswer = 0;	/*	indicates the beginning of the answer in the packet, set by create_global_answer, used by prepend_length	*/

	/*********************************************************************************************************************************************************************
	 * decode header
	 ********************************************************************************************************************************************************************/

	if(Ov_Fail(KS_DATAPACKET_read_xdr_u_long(dataReceived, &header))				//not part of rpc call
			|| Ov_Fail(KS_DATAPACKET_read_xdr_u_long(dataReceived, &xid))			//offset 0 in rpc call
			|| Ov_Fail(KS_DATAPACKET_read_xdr_u_long(dataReceived, &messageType))	//offset 4
			|| Ov_Fail(KS_DATAPACKET_read_xdr_u_long(dataReceived, &rpcVersion))	//offset 8
			|| Ov_Fail(KS_DATAPACKET_read_xdr_u_long(dataReceived, &progId))		//offset 12
			|| Ov_Fail(KS_DATAPACKET_read_xdr_u_long(dataReceived, &ProgVersion))	//offset 16
			|| Ov_Fail(KS_DATAPACKET_read_xdr_u_long(dataReceived, &procedure)))	//offset 20
	{
		KS_logfile_error(("%s: HandleRequest: cold not decode header"));
		return OV_ERR_GENERIC;
	}

	KS_logfile_debug(("%s: Header values:\n\theader\t%0#8x\n\txid\t\t%0#8x\n\tmessageType\t%0#8x\n\trpcVersion\t%0#8x\n\tprogId\t\t%0#8x\n\tProgVersion\t%0#8x\n\tprocedure\t%0#8x\n\n",
				this->v_identifier, header, xid, messageType, rpcVersion, progId, ProgVersion, procedure));

	/****************************************************************************************************************************************************************************
	 * Check if data is valid
	 ***************************************************************************************************************************************************************************/
	if((rpcVersion > KSXDR_RPCVERSION))
	{
		msgAccepted = XDR_MSG_DENIED;
		msgState = XDR_DEN_RPC_MISMATCH;
		KS_logfile_error(("%s: HandleRequest: rpcVersion is not valid: rpcVersion\t%0#8x\tshould be %0#8x", this->v_identifier, rpcVersion, KSXDR_RPCVERSION));
	}
	else if(progId != KSXDR_PROGRAMNUMBER)
	{
		msgState = XDR_MSGST_PROG_UNAVAIL;
		KS_logfile_error(("%s: HandleRequest: ProgramID is not valid: ProgramID\t%0#8x\tshould be %0#8x", this->v_identifier, progId, KSXDR_PROGRAMNUMBER));
	}
	else if(ProgVersion > KSXDR_PROGRAMVERSION)
	{
		msgState = XDR_MSGST_PROG_MISMATCH;
		KS_logfile_error(("%s: HandleRequest: program version mismatch: program version\t%0#8x\tmust be less than be %0#8x", this->v_identifier, ProgVersion, KSXDR_PROGRAMVERSION));
	}
	else if(messageType != KSXDR_MSGTYPE_CALL)
	{
		KS_logfile_error(("%s: HandleRequest: wrong message type received: \t%0#8x\tshould be %0#8x", this->v_identifier, messageType, KSXDR_MSGTYPE_CALL));
		return OV_ERR_GENERIC;
	}

	/***************************************************************************************************************************************************************************
	 * Handle errors up to here
	 **************************************************************************************************************************************************************************/
	if((msgAccepted != XDR_MSG_ACCEPTED)
			|| (msgState != XDR_MSGST_SUCCESS))
	{
		KS_logfile_error(("%s: HandleRequest: Message denied or decoding not successfull. Sending Answer.", this->v_identifier));
		ksxdr_create_global_answer(answer, xid, msgAccepted, msgState, &beginAnswer);
		ksxdr_prepend_length(answer, beginAnswer);
		return OV_ERR_OK;		/*	this function worked properly, the error is encoded in the reply message	*/
	}

	/***************************************************************************************************************************************************************************
	 * first part of message decoded. Now the xdr auth-parts follow: we do not use them.
	 * there auth_credentials and auth_verification. Both contain up to 400 bytes opaque data (variable length)
	 **************************************************************************************************************************************************************************/
			/*credentials*/
	if(Ov_Fail(KS_DATAPACKET_read_xdr_u_long(dataReceived, &authflavour)))
	{
		KS_logfile_error(("%s: HandleRequest: reading auth_cred flavor failed", this->v_identifier));
		msgAccepted = XDR_MSG_DENIED;
		msgState = XDR_DEN_AUTH_ERROR;
	}
	else if(authflavour != XDR_AUTH_NONE)
	{
		KS_logfile_error(("%s: HandleRequest: auth_cred flavor NOT NONE", this->v_identifier));
		msgAccepted = XDR_MSG_DENIED;
		msgState = XDR_DEN_AUTH_ERROR;
	}
	if(Ov_Fail(KS_DATAPACKET_read_xdr_opaque(dataReceived, &authdata, &decodeLength, KS_SIMPLEID_MAXLEN)))
	{
		KS_logfile_error(("%s: HandleRequest: reading auth_cred data failed", this->v_identifier));
		msgAccepted = XDR_MSG_DENIED;
		msgState = XDR_DEN_AUTH_ERROR;
	}
			/*verifier*/
	if(Ov_Fail(KS_DATAPACKET_read_xdr_u_long(dataReceived, &authflavour)))
	{
		KS_logfile_error(("%s: HandleRequest: reading auth_ver flavor failed", this->v_identifier));
		msgAccepted = XDR_MSG_DENIED;
		msgState = XDR_DEN_AUTH_ERROR;
	}
	else if(authflavour != XDR_AUTH_NONE)
	{
		KS_logfile_error(("%s: HandleRequest: auth_ver flavor NOT NONE", this->v_identifier));
		msgAccepted = XDR_MSG_DENIED;
		msgState = XDR_DEN_AUTH_ERROR;
	}
	if(Ov_Fail(KS_DATAPACKET_read_xdr_opaque(dataReceived, &authdata, &decodeLength, KS_SIMPLEID_MAXLEN)))
	{
		KS_logfile_error(("%s: HandleRequest: reading auth_ver data failed", this->v_identifier));
		msgAccepted = XDR_MSG_DENIED;
		msgState = XDR_DEN_AUTH_ERROR;
	}

	/************************************************************************************************************************************************************************
	 * Handle errors up to here
	 ************************************************************************************************************************************************************************/
	if(msgAccepted != XDR_MSG_ACCEPTED)
	{
		KS_logfile_error(("%s: HandleRequest: Message denied. Sending Answer.", this->v_identifier));
		ksxdr_create_global_answer(answer, xid, msgAccepted, msgState, &beginAnswer);
		ksxdr_prepend_length(answer, beginAnswer);
		return OV_ERR_OK;		/*	this function worked properly, the error is encoded in the reply message	*/
	}

	/***************************************************************************************************************************************************************************
	 * handle ticket indicator; offset now should be 36 (since the auth blocks should be 8 bytes each (opaque data length 0))
	 **************************************************************************************************************************************************************************/
		/* read ticket indicator*/
	if(Ov_Fail(KS_DATAPACKET_read_xdr_u_long(dataReceived, &authflavour)))
	{
		KS_logfile_error(("%s: HandleRequest: reading ticketindicator failed", this->v_identifier));
		msgState = XDR_MSGST_SUCCESS;
		ksErrCode = KS_ERR_TARGETGENERIC;
	}

	/**********************************************************************************************************************************************************************
	 * The following code processes tickets
	 *********************************************************************************************************************************************************************/

	if(authflavour == OV_TT_NONE)
	{
		pticket = noneticket.vtbl->createticket(NULL, OV_TT_NONE);
	}
	else
	{
		pDomAuthenticators = Ov_StaticPtrCast(ov_domain, ov_path_getobjectpointer("/communication/ksxdr/TicketAuthenticators", 2));
		if(!pDomAuthenticators)
		{
			KS_logfile_error(("%s: HandleRequest: Fatal: No TicketAuthenticators installed", this->v_identifier));
			msgState = XDR_MSGST_SYSTEM_ERR;
			ksErrCode = KS_ERR_TARGETGENERIC;
		}
		else
		{
			Ov_ForEachChildEx(ov_containment, pDomAuthenticators, pTicketAuth, ksbase_TicketAuthenticator)
			{
					/*	check if authenticator fits for ticket type	*/
				if(pTicketAuth->v_TicketType == authflavour)
				{
					pticket = pTicketAuth->v_ticket.vtbl->createticket((void*) dataReceived, authflavour);
					if(!pticket)
					{
						KS_logfile_error(("%s: HandleRequest: Decoding ticket failed", this->v_identifier));
						msgState = XDR_MSGST_SYSTEM_ERR;
						ksErrCode = KS_ERR_TARGETGENERIC;
					}
				}
				else
				{
					KS_logfile_error(("%s: HandleRequest: No TicketAuthenticator for this TicketType", this->v_identifier));
					msgState = XDR_MSGST_SUCCESS;
					ksErrCode = KS_ERR_UNKNOWNAUTH;
				}
			}
		}
	}

	/***************************************************************************************************************************************************************************
	 * Handle errors up to here
	 **************************************************************************************************************************************************************************/
	if(msgState != XDR_MSGST_SUCCESS)
	{
		KS_logfile_error(("%s: HandleRequest: Error while authenticating. Sending Answer.", this->v_identifier));
		ksxdr_create_global_answer(answer, xid, msgAccepted, msgState, &beginAnswer);
		ksxdr_prepend_length(answer, beginAnswer);
		return OV_ERR_OK;		/*	this function worked properly, the error is encoded in the reply message	*/
	}


	/************************************************************************************************************************************************************************
	 * Dispatch ks-procedures / services
	 ***********************************************************************************************************************************************************************/

	switch(procedure)
	{
	case KS_REGISTER:
		KS_logfile_debug(("%s: HandleRequest: processing REGISTER (%0#8x)", this->v_identifier, procedure));

		//TODO handle remote requests here (return KS_ERR_NOREMOTE)
		result = ksxdr_register(ProgVersion, pticket, dataReceived, &serviceAnswer, &msgState, &ksErrCode);
		if(Ov_Fail(result))
		{
			ov_memstack_lock();
			KS_logfile_error(("%s: HandleRequest: processing service %0#8x failed with error %s", this->v_identifier, procedure, ov_result_getresulttext(result)));
			ov_memstack_unlock();
			ksbase_free_KSDATAPACKET(&serviceAnswer);
			ksbase_free_KSDATAPACKET(answer);
			return result;
		}
		break;
		/***********************************************************************************************************************************************************************/

	case KS_UNREGISTER:
		KS_logfile_debug(("%s: HandleRequest: processing UNREGISTER (%0#8x)", this->v_identifier, procedure));

		//TODO handle remote requests here (return KS_ERR_NOREMOTE)
		result = ksxdr_unregister(ProgVersion, pticket, dataReceived, &serviceAnswer, &msgState, &ksErrCode);
		if(Ov_Fail(result))
		{
			ov_memstack_lock();
			KS_logfile_error(("%s: HandleRequest: processing service %0#8x failed with error %s", this->v_identifier, procedure, ov_result_getresulttext(result)));
			ov_memstack_unlock();
			ksbase_free_KSDATAPACKET(&serviceAnswer);
			ksbase_free_KSDATAPACKET(answer);
			return result;
		}
		break;
		/***********************************************************************************************************************************************************************/

	case KS_GETSERVER:
		KS_logfile_debug(("%s: HandleRequest: processing GETSERVER (%0#8x)", this->v_identifier, procedure));
		result = ksxdr_getserver(ProgVersion, pticket, dataReceived, &serviceAnswer, &msgState, &ksErrCode);
		if(Ov_Fail(result))
		{
			ov_memstack_lock();
			KS_logfile_error(("%s: HandleRequest: processing service %0#8x failed with error %s", this->v_identifier, procedure, ov_result_getresulttext(result)));
			ov_memstack_unlock();
			ksbase_free_KSDATAPACKET(&serviceAnswer);
			ksbase_free_KSDATAPACKET(answer);
			return result;
		}
		break;
		/***********************************************************************************************************************************************************************/

	case KS_CREATEOBJECT:
		KS_logfile_debug(("%s: HandleRequest: processing CREATEOBJECT (%0#8x)", this->v_identifier, procedure));
		{
			KS_logfile_info(("%s: HandleRequest: procedure %0#8x not (yet) implemented", this->v_identifier, procedure));
			msgAccepted = XDR_MSG_ACCEPTED;
			msgState = XDR_MSGST_SUCCESS;
			ksErrCode = KS_ERR_NOTIMPLEMENTED;
			ksbase_free_KSDATAPACKET(dataReceived);
		}
		break;
		/***********************************************************************************************************************************************************************/

	case KS_DELETEOBJECT:
		KS_logfile_debug(("%s: HandleRequest: processing DELETEOBJECT (%0#8x)", this->v_identifier, procedure));

		{
			KS_logfile_info(("%s: HandleRequest: procedure %0#8x not (yet) implemented", this->v_identifier, procedure));
			msgAccepted = XDR_MSG_ACCEPTED;
			msgState = XDR_MSGST_SUCCESS;
			ksErrCode = KS_ERR_NOTIMPLEMENTED;
			ksbase_free_KSDATAPACKET(dataReceived);
		}
		break;
		/***********************************************************************************************************************************************************************/

	case KS_GETEP:
		KS_logfile_debug(("%s: HandleRequest: processing GETEP (%0#8x)", this->v_identifier, procedure));
		result = ksxdr_getEP(ProgVersion, pticket, dataReceived, &serviceAnswer, &msgState, &ksErrCode);
		if(Ov_Fail(result))
		{
			ov_memstack_lock();
			KS_logfile_error(("%s: HandleRequest: processing service %0#8x failed with error %s", this->v_identifier, procedure, ov_result_getresulttext(result)));
			ov_memstack_unlock();
			ksbase_free_KSDATAPACKET(&serviceAnswer);
			ksbase_free_KSDATAPACKET(answer);
			return result;
		}
		break;
		/***********************************************************************************************************************************************************************/

	case KS_GETPP:
		KS_logfile_debug(("%s: HandleRequest: processing GETPP (%0#8x)", this->v_identifier, procedure));
		result = ksxdr_getPP(ProgVersion, pticket, dataReceived, &serviceAnswer, &msgState, &ksErrCode);
		if(Ov_Fail(result))
		{
			ov_memstack_lock();
			KS_logfile_error(("%s: HandleRequest: processing service %0#8x failed with error %s", this->v_identifier, procedure, ov_result_getresulttext(result)));
			ov_memstack_unlock();
			ksbase_free_KSDATAPACKET(&serviceAnswer);
			ksbase_free_KSDATAPACKET(answer);
			return result;
		}
		break;
		/***********************************************************************************************************************************************************************/

	case KS_GETCANONICALPATH:
		KS_logfile_debug(("%s: HandleRequest: processing GETCANONICALPATH (%0#8x)", this->v_identifier, procedure));

		{
			KS_logfile_info(("%s: HandleRequest: procedure %0#8x not (yet) implemented", this->v_identifier, procedure));
			msgAccepted = XDR_MSG_ACCEPTED;
			msgState = XDR_MSGST_SUCCESS;
			ksErrCode = KS_ERR_NOTIMPLEMENTED;
			ksbase_free_KSDATAPACKET(dataReceived);
		}
		break;
		/***********************************************************************************************************************************************************************/

	case KS_GETVAR:
		KS_logfile_debug(("%s: HandleRequest: processing GETVAR (%0#8x)", this->v_identifier, procedure));
		result = ksxdr_getVar(ProgVersion, pticket, dataReceived, &serviceAnswer, &msgState, &ksErrCode);
		if(Ov_Fail(result))
		{
			ov_memstack_lock();
			KS_logfile_error(("%s: HandleRequest: processing service %0#8x failed with error %s", this->v_identifier, procedure, ov_result_getresulttext(result)));
			ov_memstack_unlock();
			ksbase_free_KSDATAPACKET(&serviceAnswer);
			ksbase_free_KSDATAPACKET(answer);
			return result;
		}
		break;
		/***********************************************************************************************************************************************************************/

	case KS_LINK:
		KS_logfile_debug(("%s: HandleRequest: processing LINK (%0#8x)", this->v_identifier, procedure));

		{
			KS_logfile_info(("%s: HandleRequest: procedure %0#8x not (yet) implemented", this->v_identifier, procedure));
			msgAccepted = XDR_MSG_ACCEPTED;
			msgState = XDR_MSGST_SUCCESS;
			ksErrCode = KS_ERR_NOTIMPLEMENTED;
			ksbase_free_KSDATAPACKET(dataReceived);
		}
		break;
		/***********************************************************************************************************************************************************************/

	case KS_RENAMEOBJECT:
		KS_logfile_debug(("%s: HandleRequest: processing RENAMEOBJECT (%0#8x)", this->v_identifier, procedure));

		{
			KS_logfile_info(("%s: HandleRequest: procedure %0#8x not (yet) implemented", this->v_identifier, procedure));
			msgAccepted = XDR_MSG_ACCEPTED;
			msgState = XDR_MSGST_SUCCESS;
			ksErrCode = KS_ERR_NOTIMPLEMENTED;
			ksbase_free_KSDATAPACKET(dataReceived);
		}
		break;
		/***********************************************************************************************************************************************************************/

	case KS_SETVAR:
		KS_logfile_debug(("%s: HandleRequest: processing SETVAR (%0#8x)", this->v_identifier, procedure));

		result = ksxdr_setVar(ProgVersion, pticket, dataReceived, &serviceAnswer, &msgState, &ksErrCode);
		if(Ov_Fail(result))
		{
			ov_memstack_lock();
			KS_logfile_error(("%s: HandleRequest: processing service %0#8x failed with error %s", this->v_identifier, procedure, ov_result_getresulttext(result)));
			ov_memstack_unlock();
			ksbase_free_KSDATAPACKET(&serviceAnswer);
			ksbase_free_KSDATAPACKET(answer);
			return result;
		}
		break;
		/***********************************************************************************************************************************************************************/

	case KS_UNLINK:
		KS_logfile_debug(("%s: HandleRequest: processing UNLINK (%0#8x)", this->v_identifier, procedure));

		{
			KS_logfile_info(("%s: HandleRequest: procedure %0#8x not (yet) implemented", this->v_identifier, procedure));
			msgAccepted = XDR_MSG_ACCEPTED;
			msgState = XDR_MSGST_SUCCESS;
			ksErrCode = KS_ERR_NOTIMPLEMENTED;
			ksbase_free_KSDATAPACKET(dataReceived);
		}
		break;
		/***********************************************************************************************************************************************************************/

	case KS_EXGDATA:
		KS_logfile_debug(("%s: HandleRequest: processing EXGDATA (%0#8x)", this->v_identifier, procedure));

		{
			KS_logfile_info(("%s: HandleRequest: procedure %0#8x not (yet) implemented", this->v_identifier, procedure));
			msgAccepted = XDR_MSG_ACCEPTED;
			msgState = XDR_MSGST_SUCCESS;
			ksErrCode = KS_ERR_NOTIMPLEMENTED;
			ksbase_free_KSDATAPACKET(dataReceived);
		}
		break;
		/***********************************************************************************************************************************************************************/

	case KS_GETHIST:
		KS_logfile_debug(("%s: HandleRequest: processing GETHIST (%0#8x)", this->v_identifier, procedure));

		{
			KS_logfile_info(("%s: HandleRequest: procedure %0#8x not (yet) implemented", this->v_identifier, procedure));
			msgAccepted = XDR_MSG_ACCEPTED;
			msgState = XDR_MSGST_SUCCESS;
			ksErrCode = KS_ERR_NOTIMPLEMENTED;
			ksbase_free_KSDATAPACKET(dataReceived);
		}
		break;
		/***********************************************************************************************************************************************************************/

	default:
		KS_logfile_error(("%s: HandleRequest: procedure %0#8x not implemented", this->v_identifier, procedure));
		msgAccepted = XDR_MSG_ACCEPTED;
		msgState = XDR_MSGST_SUCCESS;
		ksErrCode = KS_ERR_NOTIMPLEMENTED;
		ksbase_free_KSDATAPACKET(dataReceived);
		break;
	}
	/***********************************************************************************************************************************************************************
	 * Create Answer
	 ***********************************************************************************************************************************************************************/
	//DEBUG
	// KS_logfile_debug(("%s HandleData line %u: \n\tanswer.length:\t%u\n\tanswer.readPT\t%p\n\tanswer.data\t%p", this->v_identifier, __LINE__, answer->length, answer->readPT,answer->data));
	//DEBUG_END
	/*	create global part of answer, append service specific part (answerMessage) and send it	*/
	ksxdr_create_global_answer(answer, xid, msgAccepted, msgState, &beginAnswer);
	//DEBUG
	// KS_logfile_debug(("%s HandleData line %u: \n\tanswer.length:\t%u\n\tanswer.readPT\t%p\n\tanswer.data\t%p", this->v_identifier, __LINE__, answer->length, answer->readPT,answer->data));
	//DEBUG_END

	/*	set auth part in reply. set tickettype and use ticket->encodereply for specific data	*/
	/*	set ticketindicator	*/
	if(Ov_Fail(KS_DATAPACKET_write_xdr_u_long(answer, &authflavour)))
	{
		ksbase_free_KSDATAPACKET(&serviceAnswer);
		ksbase_free_KSDATAPACKET(answer);
		return OV_ERR_HEAPOUTOFMEMORY;
	}

	/*	set ticketreply	*/
	if(!(pticket->vtbl->encodereply(answer, pticket)))
	{
		KS_logfile_error(("%s: HandleRequest: ticket encodereply failed", this->v_identifier));
		return OV_ERR_GENERIC;
	}

	/*	set ksErrCode	*/
	if(Ov_Fail(KS_DATAPACKET_write_xdr_u_long(answer, &ksErrCode)))
	{
		ksbase_free_KSDATAPACKET(&serviceAnswer);
		ksbase_free_KSDATAPACKET(answer);
		return OV_ERR_HEAPOUTOFMEMORY;
	}
//DEBUG
		//KS_logfile_debug(("%s HandleData line %u: \n\tanswer.length:\t%u\n\tanswer.readPT\t%p\n\tanswer.data\t%p", this->v_identifier, __LINE__, answer->length, answer->readPT,answer->data));
//DEBUG_END
	/*	append service specific data (if service succeeded)	*/
	if(ksErrCode == KS_ERR_OK)
		if(serviceAnswer.length)
			if(Ov_Fail(ksbase_KSDATAPACKET_append(answer, serviceAnswer.data, serviceAnswer.length)))
			{
				ksbase_free_KSDATAPACKET(&serviceAnswer);
				ksbase_free_KSDATAPACKET(answer);
				return OV_ERR_HEAPOUTOFMEMORY;
			}
	//DEBUG
	 //KS_logfile_debug(("%s HandleData line %u: \n\tanswer.length:\t%u\n\tanswer.readPT\t%p\n\tanswer.data\t%p", this->v_identifier, __LINE__, answer->length, answer->readPT,answer->data));
	//DEBUG_END
	ksxdr_prepend_length(answer, beginAnswer);

	/*	if the whole buffer was decoded free it	*/
	if((dataReceived->readPT - dataReceived->data) >= dataReceived->length)
	{
		KS_logfile_debug(("%s: HandleRequest: dataReceived decoded completely, freeing it", this->v_identifier));
		ksbase_free_KSDATAPACKET(dataReceived);
	}


    return OV_ERR_OK;
}

