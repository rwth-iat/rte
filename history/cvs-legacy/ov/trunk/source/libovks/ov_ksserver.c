/*
*   $Id: ov_ksserver.c,v 1.1 1999-07-19 15:02:16 dirk Exp $
*
*   Copyright (C) 1998-1999
*   Lehrstuhl fuer Prozessleittechnik,
*   RWTH Aachen, D-52056 Aachen, Germany.
*   All rights reserved.
*
*   Author: Dirk Meyer <dirk@plt.rwth-aachen.de>
*
*   This file is part of the ACPLT/OV Package which is licensed as open
*   source under the Artistic License; you can use, redistribute and/or
*   modify it under the terms of that license.
*
*   You should have received a copy of the Artistic License along with
*   this Package; see the file ARTISTIC-LICENSE. If not, write to the
*   Copyright Holder.
*
*   THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR IMPLIED
*   WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES
*   OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*/
/*
*	History:
*	--------
*	23-Jul-1998 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*	21-Apr-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: Major revision.
*/

#define OV_COMPILE_LIBOVKS

#include "libovks/ov_ksserver.h"
#include "libovks/ov_ksserver_xdr.h"
#include "libov/ov_object.h"
#include "libov/ov_scheduler.h"
#include "libov/ov_vendortree.h"

#if OV_SYSTEM_UNIX || OV_SYSTEM_NT
#ifndef __cplusplus
#error *** this file must be compiled using C++ on this platform ***
#endif
#endif

#ifdef __cplusplus

#if PLT_COMPILER_GCC || PLT_COMPILER_WATCOM || PLT_COMPILER_DECCXX
#include "plt/priorityqueue_impl.h"
#endif

#if PLT_COMPILER_BORLAND
#include "plt/priorityqueue.h"
#endif

#include "plt/comparable.h"

#if PLT_INSTANTIATE_TEMPLATES
template class PltPtrComparable<KsTimerEvent>;
template class PltPriorityQueue<PltPtrComparable<KsTimerEvent> >;
template class PltPQIterator<PltPtrComparable<KsTimerEvent> >;
template class PltIterator<PltPtrComparable<KsTimerEvent> >;
template class PltIterator_<PltPtrComparable<KsTimerEvent> >;
template class PltContainer_<PltPtrComparable<KsTimerEvent> >;
#if PLT_COMPILER_CYGWIN || PLT_COMPILER_DECCXX
template class PltContainer<PltPtrComparable<KsTimerEvent> >;
#endif
#endif

#endif	/* __cplusplus */

#if OV_SYSTEM_MC164
#include "mc164/Kachel.h"
#include "mc164/rpcdef.h"
#include <reg164.h>
#include "..\..\..\..\S7\base\inc\ct2types.hs"
#include "..\..\..\..\S7\Fmsdpg\inc\header.h"
#endif

#if OV_SYSTEM_RMOS
#include <rmapi.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*	----------------------------------------------------------------------	*/

/*
*	Global variables: the ACPLT/KS server for ACPLT/OV and the logger object
*/
#ifdef __cplusplus
static OvKsServer	*pserver = NULL;
static OvPltLog		*plog = NULL;
#endif

/*	----------------------------------------------------------------------	*/

/*
*	Terminate an ACPLT/KS server (RMOS only)
*/
#if OV_SYSTEM_RMOS
OV_RESULT OV_DLLFNCEXPORT ov_ksserver_terminate(
	OV_UINT			taskid
) {
	/*
	*	local variables
	*/
	int		result;
	/*
	*	create message queue
	*/
	if(RmCreateMessageQueue(NULL, RM_OWN_TASK) != RM_OK) {
		Ov_Warning("Could not create message queue");
		return OV_ERR_GENERIC;	/* FIXME: error code? */
	}
	/*
	*	send "terminate" message and wait until it is read
	*	with a timeout of 30 seconds and delete message queue
	*/
	result = RmSendMessage(RM_SECOND(60), RM_CURPRI, taskid, RM_MSG_USER, NULL);
	if(RmDeleteMessageQueue(RM_OWN_TASK) != RM_OK) {
		Ov_Warning("Could not delete message queue");
	}
	switch(result) {
		case RM_OK:
			return OV_ERR_OK;
		case RM_GOT_TIMEOUT:
			return OV_ERR_GENERIC;	/* FIXME: error code? */
		default:
			break;
	}
	return OV_ERR_BADPARAM;	/* maybe taskid was bad */
}
#endif

/*	----------------------------------------------------------------------	*/

/*
*	Start the ACPLT/KS server for ACPLT/OV
*/
OV_RESULT OV_DLLFNCEXPORT ov_ksserver_start(
	OV_STRING			servername,
	int					port,
	OV_FNC_SIGHANDLER	*sighandler
) {
#if OV_SYSTEM_MC164
	/*
	*	initialize CAN module
	*/
	/*
	*	TODO!!!
	*/
	/*
	*	initialize kachel memory
	*/
	array_mem_init();
	return OV_ERR_OK;
#else
	if(!pserver) {
		/*
		*	check server name
		*/
		if(!ov_object_identifierok(servername)) {
			return OV_ERR_BADNAME;
		}
#if OV_SYSTEM_RMOS
		/*
		*	create message queue
		*/
		if(RmCreateMessageQueue(NULL, RM_OWN_TASK) != RM_OK) {
			Ov_Warning("Could not create message queue");
			return OV_ERR_GENERIC;	/* FIXME: error code? */
		}
#endif
		/*
		*	install the logger object
		*/
		if(!plog) {
			plog = new OvPltLog();
		}
		/*
		*	install signal handler
		*/
		if(sighandler) {
			pserver = new OvKsServer(servername, port, sighandler);
		} else {
			pserver = new OvKsServer(servername, port, ov_ksserver_sighandler);
		}
		if(pserver) {
			pserver->startServer();
			ov_vendortree_setservername(servername);
			ov_vendortree_setstartuptime(NULL);
			return OV_ERR_OK;
		}
	}
	return OV_ERR_GENERIC;
#endif
}

/*	----------------------------------------------------------------------	*/

/*
*	Run the ACPLT/KS server for ACPLKT/OV
*/
void OV_DLLFNCEXPORT ov_ksserver_run(void) {
#if OV_SYSTEM_MC164
	/*
	*	local variables
	*/
	OV_TIME_SPAN	*ptimeout;
	/*
	*	run the server
	*/
	while(!ov_ksserver_isgoingdown()) {
		ptimeout = ov_scheduler_schedulenextevent();
		ov_ksserver_servependingevents(ptimeout);
		Begin_CopyKachelToCAN_Frame();
	}
#else
	/*
	*	local variables
	*/
	OV_TIME_SPAN	*ptimeout;
#if OV_SYSTEM_RMOS
	uint			msg;
	void			*param;
#endif
	/*
	*	run the server
	*/
	if(pserver) {
		while(!pserver->isGoingDown()) {
#if OV_SYSTEM_RMOS
			/*
			*	get next message from the message queue
			*/
			if(RmReadMessage(RM_CONTINUE, &msg, &param) == RM_OK) {
				if(msg == RM_MSG_USER) {
					raise(SIGTERM);
				}
			}
#endif
			ptimeout = ov_scheduler_schedulenextevent();
			pserver->servePendingEvents(KsTime(ptimeout->secs, ptimeout->usecs));
		}
	}
#endif
}

/*	----------------------------------------------------------------------	*/

/*
*	Stop the ACPLT/KS server for ACPLKT/OV
*/
void OV_DLLFNCEXPORT ov_ksserver_stop(void) {
#if OV_SYSTEM_MC164
	/*
	*   Nothing to do
	*/
	return;
#else
	if(pserver) {
		/*
		*	stop the server
		*/
		pserver->stopServer();
		delete pserver;
		pserver = NULL;
		/*
		*	uninstall the logger object
		*/
		if(plog) {
			delete plog;
		}
		/*
		*	remove the servername from the vendortree
		*/
		ov_vendortree_setservername(NULL);
#if OV_SYSTEM_RMOS
		/*
		*	delete message queue
		*/
		if(RmDeleteMessageQueue(RM_OWN_TASK) != RM_OK) {
			Ov_Warning("Could not delete message queue");
		}
#endif
	}
#endif
}

/*	----------------------------------------------------------------------	*/

/*
*	Test if the ACPLT/KS server for ACPLKT/OV is going down
*/
OV_BOOL OV_DLLFNCEXPORT ov_ksserver_isgoingdown(void) {
#if OV_SYSTEM_MC164
	/*
	*   for now, there's no shutdown signal
	*/
	return FALSE;
#else
	if(pserver) {
		return pserver->isGoingDown();
	}
	return TRUE;
#endif
}

/*	----------------------------------------------------------------------	*/

/*
*	Test the ACPLT/KS server for ACPLKT/OV has pending I/O events
*/
OV_BOOL OV_DLLFNCEXPORT ov_ksserver_haspendingevents(void) {
#if OV_SYSTEM_MC164
	/*
	*   local variables
	*/
	XDR*	stream;
	/*
	*   instructions
	*/
	XP0IE = 0;  /* lock CAN interrupt */
	stream = search_ptr_of_state(ARRAY_MEMORY_BASE_ADDRESS, XDR_DECODE);				   
	XP0IE = 1;  /* unlock CAN-Interrupt */
	if((stream)) {
		return TRUE;
	} else {
		return FALSE;
	}
#else
	if(pserver) {
		return pserver->hasPendingEvents();
	}
	return FALSE;
#endif
}

/*	----------------------------------------------------------------------	*/

/*
*	Serve pending I/O events of the ACPLT/KS server for ACPLKT/OV
*/
OV_BOOL OV_DLLFNCEXPORT ov_ksserver_servependingevents(
	OV_TIME_SPAN	*ptimeout
) {
#if OV_SYSTEM_MC164
	/*
	*	local variables
	*/
	rpc_answer_t	decode_result;
	Answer			encode_result;
	XDR*			stream;
	OV_TICKET 		ticket;
	/*
	*	instructions
	*/
	XP0IE = 0;  /* lock CAN interrupt */
	stream = search_ptr_of_state(ARRAY_MEMORY_BASE_ADDRESS, XDR_DECODE);
	XP0IE = 1;  /* unlock CAN interrupt */				   
	if(!stream) {
		return FALSE; /* no ready XDR stream */
	}
 	/*
	*	decode RPC header
	*/
	decode_result = ov_ksserver_rpcheader_call_decode(stream);
	if(decode_result == rpc_success) {
		/* 
		*	decode ticket 
		*/
		if(!ov_ksserver_xdr_OV_TICKET_PAR(stream, &ticket)) {
			decode_result = rpc_garbage_args;
		} else {
			/*
			*	execute service
			*/
			ov_memstack_lock();
			ov_ksserver_dispatch(stream->service_id, stream->vers,
			   &ticket, stream, stream);
			ov_memstack_unlock();
			/*
			*	make reply ready for sending
			*/
			XP0IE = 0;  /* lock CAN interrupt */
			set_state(ARRAY_MEMORY_BASE_ADDRESS, stream, CAN_new);
			XP0IE = 1;  /* unlock CAN interrupt */
		}
	}
	switch (decode_result) {
		case rpc_success:
			/*
			*	no error decoding RPC header
			*/
			break;
		case rpc_error:
			/*
			*	error in XDR routine, delete stream
			*/
			XP0IE = 0;  /* lock CAN interrupt */
			delete_stream(ARRAY_MEMORY_BASE_ADDRESS, stream);
			XP0IE = 1;  /* unlock CAN interrupt */
			break;
		default:
			/*
			*	any other RPC error, write RPC reply to stream
			*/
			encode_result = ov_ksserver_rpcheader_reply_encode(stream, decode_result);
			if(encode_result == no) {
				/*
				*	delete stream
				*/
				XP0IE = 0;  /* lock CAN interrupt */
				delete_stream(ARRAY_MEMORY_BASE_ADDRESS, stream);
				XP0IE = 1;  /* unlock CAN interrupt */
			} else {
				/*
				*	make reply ready for sending
				*/
				XP0IE = 0;  /* lock CAN interrupt */
				set_state(ARRAY_MEMORY_BASE_ADDRESS, stream, CAN_new);
				XP0IE = 1;  /* unlock CAN interrupt */
			}
	}
	return TRUE;
#else
	if(pserver) {
		if(ptimeout) {
			return pserver->servePendingEvents(KsTime(ptimeout->secs,
				ptimeout->usecs));
		} else {
			return pserver->servePendingEvents();
		}
	}
	return FALSE;
#endif
}

/*	----------------------------------------------------------------------	*/

/*
*	Default signal handler for server shutdown
*/
#if !OV_SYSTEM_MC164
void OV_DLLFNCEXPORT ov_ksserver_sighandler(int) {
	if(pserver) {
		pserver->downServer();
	}
}
#endif

/*	----------------------------------------------------------------------	*/

/*
*	Helper macro for implementing the service dispatcher
*/
#define Ov_KsServer_Dispatch(service, SERVICE)								\
	case KS_##SERVICE: {													\
			/*																\
			*	parameter and result objects								\
			*/																\
			OV_##SERVICE##_PAR	params;										\
			OV_##SERVICE##_RES	result;										\
			/*																\
			*	decode the parameters										\
			*/																\
			if(ov_ksserver_xdr_OV_##SERVICE##_PAR(xdrin, &params)) {		\
				/*															\
				*	properly decoded, call service function and send reply	\
				*/															\
				ov_ksserver_##service(version, povticket, &params, &result);\
				ov_ksserver_sendreply(xdrout, pticket, (OV_POINTER)&result,	\
					(xdrproc_t)ov_ksserver_xdr_OV_##SERVICE##_RES);			\
				return;														\
			}																\
		}																	\
		break

/*	----------------------------------------------------------------------	*/

/*
*	Dispatch a service (subroutine)
*/
void ov_ksserver_dispatch(
	OV_SVC		service,
	OV_UINT		version,
	OV_TICKET	*pticket,
	XDR			*xdrin,
	XDR			*xdrout
) {
	/*
	*	local variables
	*/
#ifdef __cplusplus
	OV_TICKET	*povticket = ((OvKsAvTicket*)pticket)->ov_getTicket();
#else
	OV_TICKET	*povticket = pticket;
#endif
	/*
	*	dispatch the service
	*/
	switch(service) {
		Ov_KsServer_Dispatch(getpp, GETPP);
		Ov_KsServer_Dispatch(getvar, GETVAR);
		Ov_KsServer_Dispatch(setvar, SETVAR);
		Ov_KsServer_Dispatch(exgdata, EXGDATA);
		Ov_KsServer_Dispatch(getep, GETEP);
		Ov_KsServer_Dispatch(createobject, CREATEOBJECT);
		Ov_KsServer_Dispatch(deleteobject, DELETEOBJECT);
		Ov_KsServer_Dispatch(renameobject, RENAMEOBJECT);
		Ov_KsServer_Dispatch(getcanonicalpath, GETCANONICALPATH);
		Ov_KsServer_Dispatch(link, LINK);
		Ov_KsServer_Dispatch(unlink, UNLINK);
		default:
			/*
			*	unknown service id, send error reply and unlock memory stack
			*/
			ov_ksserver_sendreply(xdrout, pticket, (OV_POINTER)
				OV_ERR_NOTIMPLEMENTED, NULL);
			return;
	}
	/*
	*	error decoding
	*/
	Ov_Warning("error decoding request");
	ov_ksserver_sendreply(xdrout, pticket, (OV_POINTER)
		OV_ERR_GENERIC, NULL);
}

/*	----------------------------------------------------------------------	*/

/*
*	Send a service reply (subroutine)
*/
void ov_ksserver_sendreply(
	XDR			*xdrs,
	OV_TICKET	*pticket,
	OV_POINTER	result,
	xdrproc_t	encodefnc
) {
#if OV_SYSTEM_MC164
	/*
	*	local variables
	*/
	OV_RESULT	ovresult = (OV_RESULT)result;
	/*
	*	write RPC reply header, ticket and result to XDR stream
	*/
   	ov_ksserver_rpcheader_reply_encode(xdrs, rpc_success);
	ov_ksserver_xdr_OV_TICKET_RES(xdrs, pticket);
	if(encodefnc) {
		/*
		*	send a normal reply
		*/
		encodefnc(xdrs, result);
	} else {
		/*
		*	send an error reply
		*/
		ov_ksserver_xdr_OV_RESULT(xdrs, &ovresult);
	}
#else
	if(encodefnc) {
		/*
		*	send a normal reply
		*/
		OvKsResult kssvrresult(result, encodefnc);
		((KssTransport*)xdrs)->sendReply(*(OvKsAvTicket*)pticket, kssvrresult);
	} else {
		/*
		*	send an error reply
		*/
		((KssTransport*)xdrs)->sendErrorReply(*(OvKsAvTicket*)pticket,
			(OV_RESULT)result);
	}
   	ov_memstack_unlock();
	return;
#endif
}

/*	----------------------------------------------------------------------	*/

#ifdef __cplusplus
}
#endif

/*
*	End of file
*/

