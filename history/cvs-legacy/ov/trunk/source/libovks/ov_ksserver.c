/*
*   $Id: ov_ksserver.c,v 1.21 2004-06-28 16:53:23 ansgar Exp $
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
*	02-Apr-2001 Ansgar Münnemann <ansgar@plt.rwth-aachen.de>: adjustings for KsGetHist-service.
*/

#define OV_COMPILE_LIBOVKS

#include "libovks/ov_ksserver.h"
#include "libovks/ov_ksserver_xdr.h"
#include "libov/ov_object.h"
#include "libov/ov_scheduler.h"
#include "libov/ov_database.h"
#include "libov/ov_vendortree.h"
#include "libov/ov_macros.h"

#if OV_SYSTEM_UNIX || OV_SYSTEM_NT || OV_SYSTEM_OPENVMS
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
#if PLT_COMPILER_CYGWIN || PLT_COMPILER_DECCXX || PLT_COMPILER_GCC
template class PltContainer<PltPtrComparable<KsTimerEvent> >;
#endif
#endif

#endif	/* __cplusplus */

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
static KsConnectionManager *OvKsConnectionManager = NULL;
#endif

/*	----------------------------------------------------------------------	*/

/*
*	internal default VTable and functions for a default ticket
*/
static OV_TICKET *ov_ksserver_ticket_defaultticket_createticket(XDR *xdr, OV_TICKET_TYPE type);
static void ov_ksserver_ticket_defaultticket_deleteticket(OV_TICKET *pticket);
static OV_BOOL ov_ksserver_ticket_defaultticket_encodereply(XDR *xdr, OV_TICKET *pticket);
static OV_ACCESS ov_ksserver_ticket_defaultticket_getaccess(const OV_TICKET *pticket);

static OV_ACCESS defaultaccess = OV_AC_READ;

static OV_TICKET_VTBL defaultticketvtbl = {
	ov_ksserver_ticket_defaultticket_createticket,
	ov_ksserver_ticket_defaultticket_deleteticket,
	ov_ksserver_ticket_defaultticket_encodereply,
	ov_ksserver_ticket_defaultticket_getaccess
};

static OV_TICKET *ov_ksserver_ticket_defaultticket_createticket(XDR *xdr, OV_TICKET_TYPE type) {
	static OV_TICKET ticket = { &defaultticketvtbl,  OV_TT_SIMPLE };
	switch(type) {
		case OV_TT_NONE:
			break;
		case OV_TT_SIMPLE:
			if(!ov_ksserver_xdr_string(xdr, &ticket.ticketunion.simpleticket.id,
				KS_SIMPLEID_MAXLEN)
			) {
				return NULL;
			}
			break;
		default:
			return NULL;
	}
	ticket.type = type;
	if (pdb->serverpassword) {
		if (type == OV_TT_SIMPLE) {
			/*
			*	only grant read access to anyone who does not have the serverpassword
			*/
			if(!strcmp(ticket.ticketunion.simpleticket.id, pdb->serverpassword)) {
				defaultaccess = OV_AC_READWRITE | OV_AC_INSTANTIABLE
					| OV_AC_DELETEABLE | OV_AC_RENAMEABLE | OV_AC_LINKABLE
					| OV_AC_UNLINKABLE; 
			}
			else defaultaccess = OV_AC_READ;
		}
		else defaultaccess = OV_AC_READ;
	}
	else defaultaccess = OV_AC_READWRITE | OV_AC_INSTANTIABLE
				| OV_AC_DELETEABLE | OV_AC_RENAMEABLE | OV_AC_LINKABLE
				| OV_AC_UNLINKABLE; 
	return &ticket;
}

static void ov_ksserver_ticket_defaultticket_deleteticket(OV_TICKET *pticket) {
	/*
	*	local variables
	*/
	static XDR xdr;
	/*
	*	just let XDR free the memory it allocated during construction of the id
	*/
	if (pticket->type == OV_TT_SIMPLE) {
		xdr.x_op = XDR_FREE;
		ov_ksserver_xdr_string(&xdr, &(pticket->ticketunion.simpleticket.id), KS_SIMPLEID_MAXLEN);
	}
}

static OV_BOOL ov_ksserver_ticket_defaultticket_encodereply(XDR *, OV_TICKET *) {
	return TRUE;
}

static OV_ACCESS ov_ksserver_ticket_defaultticket_getaccess(const OV_TICKET *) {
	return defaultaccess; 
}

/*	----------------------------------------------------------------------	*/

/*
*	List of registered tickets
*/
struct OV_TICKET_TABLE_ENTRY {
	struct OV_TICKET_TABLE_ENTRY	*pnext;
	OV_TICKET_TYPE					type;
	const OV_TICKET_VTBL			*vtbl;
};
typedef struct OV_TICKET_TABLE_ENTRY OV_TICKET_TABLE_ENTRY;

static OV_TICKET_TABLE_ENTRY	*pfirstentry = NULL;

/*	----------------------------------------------------------------------	*/

/*
*	Create a ticket from an incoming XDR stream (internal)
*/
OV_TICKET *ov_ksserver_ticket_create(XDR *xdr) {
	/*
	*	local variables
	*/
	OV_TICKET_TYPE 			type;
	OV_TICKET_TABLE_ENTRY	*pentry;
	/*
	*	instructions
	*/
	if(!ov_ksserver_xdr_OV_TICKET_TYPE(xdr, &type)) {
		return NULL;
	}
	/* look up if a registered ticket */
	for(pentry=pfirstentry; pentry; pentry=pentry->pnext) {
		if(pentry->type == type) {
			return pentry->vtbl->createticket(xdr, type);
		}
	}
	/* use default ticket */
	if((type == OV_TT_NONE) || (type == OV_TT_SIMPLE)) {
		return ov_ksserver_ticket_defaultticket_createticket(xdr, type);
	}
	return NULL;
}

/*	----------------------------------------------------------------------	*/

/*
*	Delete a given ticket (internal)
*/
void ov_ksserver_ticket_delete(OV_TICKET *pticket) {
	if(pticket) {
		if(pticket->vtbl) {
			pticket->vtbl->deleteticket(pticket);
		}
	}
}

/*	----------------------------------------------------------------------	*/

/*
*	Register a new ticket
*/
OV_DLLFNCEXPORT OV_RESULT ov_ksserver_ticket_register(
	const OV_TICKET_TYPE	type,
	const OV_TICKET_VTBL	*vtbl
) {
	OV_TICKET_TABLE_ENTRY *pentry;
	if(!vtbl) {
		return OV_ERR_BADPARAM;
	}
	for(pentry=pfirstentry; pentry; pentry=pentry->pnext) {
		if(pentry->type == type) {
			/* ticket type already registered, modify vtable entry */
			pentry->vtbl = vtbl;
			return OV_ERR_OK;
		}
	}
	/* add new entry at the beginning of the table */
	pentry = Ov_HeapAlloc(OV_TICKET_TABLE_ENTRY);
	if(pentry) {
		pentry->type = type;
		pentry->vtbl = vtbl;
		pentry->pnext = pfirstentry;
		pfirstentry = pentry;
		return OV_ERR_OK;
	}
	return OV_ERR_HEAPOUTOFMEMORY;
}

/*	----------------------------------------------------------------------	*/

/*
*	Unregister a given ticket
*/
OV_DLLFNCEXPORT OV_RESULT ov_ksserver_ticket_unregister(
	const OV_TICKET_TYPE	type
) {
	OV_TICKET_TABLE_ENTRY *pentry, *plast;
	plast = NULL;
	for(pentry=pfirstentry; pentry; pentry=pentry->pnext) {
		if(pentry->type == type) {
			/* remove the entry from the list */
			if(plast) {
				plast->pnext = pentry->pnext;
			} else {
				pfirstentry = pentry->pnext;
			}
			/*
			*	delete the entry object
			*/
			Ov_HeapFree(pentry);
			return OV_ERR_OK;
		}
		plast = pentry;
	}
	/* no such ticket type registered */
	return OV_ERR_BADPARAM;
}

/*
*	Set access rights for default tickets
*/
OV_DLLFNCEXPORT void ov_ksserver_ticket_setdefaultaccess(OV_ACCESS access) {
	defaultaccess = access & (OV_AC_READWRITE | OV_AC_INSTANTIABLE
		| OV_AC_DELETEABLE | OV_AC_RENAMEABLE | OV_AC_LINKABLE
		| OV_AC_UNLINKABLE);
}

/*	----------------------------------------------------------------------	*/

/*
*	Terminate an ACPLT/KS server (RMOS only)
*/
#if OV_SYSTEM_RMOS
OV_DLLFNCEXPORT OV_RESULT ov_ksserver_terminate(
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
*	Create the ACPLT/KS server for ACPLT/OV
*/
#ifndef __cplusplus
/* function must be implemented separately */
#else
OV_DLLFNCEXPORT OV_RESULT ov_ksserver_create(
	OV_STRING			servername,
	int					port,
	OV_FNC_SIGHANDLER	*sighandler,
	OV_BOOL				reuse
) {
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
			Ov_Warning("Can not create message queue");
			return OV_ERR_GENERIC;
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
		if(sighandler == OV_SIGHANDLER_DEFAULT) {
			pserver = new OvKsServer(servername, port, ov_ksserver_sighandler);
		} else if(sighandler != OV_SIGHANDLER_NONE){
			pserver = new OvKsServer(servername, port, sighandler);
		} else {
			pserver = new OvKsServer(servername, port, 0);
		}
		if(pserver) {
			ov_vendortree_setservername(servername);
			if (reuse) pserver->setReuseAddr(reuse);
		        OvKsConnectionManager = KsConnectionManager::getConnectionManagerObject();
			if (!OvKsConnectionManager) {
			        OvKsConnectionManager = new KsStdConnectionManager();
			}
			return OV_ERR_OK;
		}
	}
	return OV_ERR_GENERIC;
}
#endif

/*	----------------------------------------------------------------------	*/

/*
*	Delete the ACPLT/KS server for ACPLT/OV
*/
#ifndef __cplusplus
/* function must be implemented separately */
#else
OV_DLLFNCEXPORT void ov_ksserver_delete(void) {
	if(pserver) {
		/*
		*	kill the KsConnectionManager object
		*/
		delete OvKsConnectionManager;
		OvKsConnectionManager = NULL;
		/*
		*	kill the server object
		*/
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
}
#endif

/*	----------------------------------------------------------------------	*/

/*
*	Start the ACPLT/KS server for ACPLT/OV
*/
#ifndef __cplusplus
/* function must be implemented separately */
#else
OV_DLLFNCEXPORT void ov_ksserver_start(void) {
	if(pserver) {
		ov_vendortree_setstartuptime(NULL);
		pserver->startServer();
	}
}
#endif

/*	----------------------------------------------------------------------	*/

/*
*	Run the ACPLT/KS server for ACPLKT/OV
*/
#ifndef __cplusplus
/* function must be implemented separately */
#else
OV_DLLFNCEXPORT void ov_ksserver_run(void) {
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
	pserver->setShutdownFlag(0);
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
			OvKsConnectionManager->servePendingEvents(KsTime(ptimeout->secs, ptimeout->usecs));
		}
	}
}
#endif

/*	----------------------------------------------------------------------	*/

/*
*	Stop the ACPLT/KS server for ACPLKT/OV
*/
#ifndef __cplusplus
/* function must be implemented separately */
#else
OV_DLLFNCEXPORT void ov_ksserver_stop(void) {
	if(pserver) {
		/*
		*	stop the server
		*/
		pserver->stopServer();
	}
}
#endif

/*	----------------------------------------------------------------------	*/

/*
*	Tell the server to shut down
*/
#ifndef __cplusplus
/* function must be implemented separately */
#else
OV_DLLFNCEXPORT void ov_ksserver_downserver(void) {
	if(pserver) {
		pserver->downServer();
	}
}
#endif

/*	----------------------------------------------------------------------	*/

/*
*	Test if the ACPLT/KS server for ACPLKT/OV is going down
*/
#ifndef __cplusplus
/* function must be implemented separately */
#else
OV_DLLFNCEXPORT OV_BOOL ov_ksserver_isgoingdown(void) {
	if(pserver) {
		return pserver->isGoingDown();
	}
	return TRUE;
}
#endif

/*	----------------------------------------------------------------------	*/

/*
*	Test the ACPLT/KS server for ACPLKT/OV has pending I/O events
*/
#ifndef __cplusplus
/* function must be implemented separately */
#else
OV_DLLFNCEXPORT OV_BOOL ov_ksserver_haspendingevents(void) {
	if(pserver) {
		return pserver->hasPendingEvents();
	}
	return FALSE;
}
#endif

/*	----------------------------------------------------------------------	*/

/*
*	Serve pending I/O events of the ACPLT/KS server for ACPLKT/OV
*/
#ifndef __cplusplus
/* function must be implemented separately */
#else
OV_DLLFNCEXPORT OV_BOOL ov_ksserver_servependingevents(
	OV_TIME_SPAN	*ptimeout
) {
	if(pserver) {
		if(ptimeout) {
			return OvKsConnectionManager->servePendingEvents(KsTime(ptimeout->secs,
				ptimeout->usecs));
		} else {
			return OvKsConnectionManager->servePendingEvents();
		}
	}
	return FALSE;
}
#endif

/*	----------------------------------------------------------------------	*/

/*
*	Default signal handler for server shutdown
*/
#ifndef __cplusplus
/* function must be implemented separately */
#else
OV_DLLFNCEXPORT void ov_ksserver_sighandler(int) {
	if(pserver) {
		pserver->downServer();
	}
}

#if PLT_SYSTEM_SOLARIS

/* Funktion zum reinstallieren des Signal-Handlers */
void ov_ksserver_sighandler_solaris_reinstall() {
    
  (void) signal(SIGPIPE, ov_ksserver_sighandler_solaris);
  
}

static void ov_ksserver_sighandler_solaris(
	int signal
) {
  /* Signal-Bearbeitung reinstallieren */
  (void) ov_ksserver_sighandler_solaris_reinstall();
  
}

#endif


OV_DLLFNCEXPORT void ov_ksserver_sighandler_register()
{
			signal(SIGTERM, ov_ksserver_sighandler);
#if !PLT_SYSTEM_RMOS
			signal(SIGINT, ov_ksserver_sighandler);
#if !PLT_SYSTEM_NT
			signal(SIGHUP, ov_ksserver_sighandler);
#endif
#endif
#if PLT_SYSTEM_SOLARIS
			signal(SIGPIPE, ov_ksserver_sighandler_solaris);
#endif
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
				ov_ksserver_sendreply(xdrout, pticket, (OV_RESULT*)&result,	\
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
    OV_RESULT   result;
#ifdef __cplusplus
	OV_TICKET	*povticket = ((OvKsAvTicket*)pticket)->ov_getTicket();
#else
	OV_TICKET	*povticket = pticket;
	printf("just C\n");
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
	Ov_KsServer_Dispatch(gethist, GETHIST);
	default:
		/*
		*	unknown service id, send error reply and unlock memory stack
		*/
        result = OV_ERR_NOTIMPLEMENTED;
		ov_ksserver_sendreply(xdrout, pticket, &result, NULL);
		return;
	}
	/*
	*	error decoding
	*/
	Ov_Warning("error decoding request");
	result = OV_ERR_GENERIC;
	ov_ksserver_sendreply(xdrout, pticket, &result, NULL);
}

/*	----------------------------------------------------------------------	*/

/*
*	Send a service reply (subroutine)
*/
#ifndef __cplusplus
/* function must be implemented separately; implementation could look like this: */
#if 0
void ov_ksserver_sendreply(
	XDR			*xdrs,
	OV_TICKET	*pticket,
	OV_RESULT	*presult,
	xdrproc_t	encodefnc
) {
	/*
	*	local variables
	*/
	OV_RESULT	ovresult = (OV_RESULT)result;
	/*
	*	write RPC reply header, ticket and result to XDR stream
	*/
   	ov_ksserver_xdr_encode_rpcheader_reply(xdrs, rpc_success);
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
}
#endif
#else
void ov_ksserver_sendreply(
	XDR			*xdrs,
	OV_TICKET	*pticket,
	OV_RESULT	*presult,
	xdrproc_t	encodefnc
) {
	if(encodefnc) {
		/*
		*	send a normal reply
		*/
		OvKsResult kssvrresult(presult, encodefnc);
		((KssTransport*)xdrs)->sendReply(*(OvKsAvTicket*)pticket, kssvrresult);
	} else {
		/*
		*	send an error reply
		*/
		((KssTransport*)xdrs)->sendErrorReply(*(OvKsAvTicket*)pticket, *presult);
	}
   	ov_memstack_unlock();
	return;
}
#endif

/*	----------------------------------------------------------------------	*/
/*
*	maps database with supervised exceptions (subroutine)
*/

#ifdef OV_CATCH_EXCEPTIONS
#ifndef __cplusplus
/* function must be implemented separately */
#else
OV_RESULT ov_supervised_database_map(
	OV_STRING 	dbname
) {
	try {
		return ov_database_map(dbname);
	}
	catch(...) {
		return OV_ERR_GENERIC;
	}
}
#endif
#else
#ifndef __cplusplus
/* function must be implemented separately */
#else
OV_RESULT ov_supervised_database_map(
	OV_STRING 	dbname
) {
	return ov_database_map(dbname);
}
#endif
#endif
/*	----------------------------------------------------------------------	*/
/*
*	starts up database with supervised exceptions (subroutine)
*/

#ifdef OV_CATCH_EXCEPTIONS
#ifndef __cplusplus
/* function must be implemented separately */
#else
OV_RESULT ov_supervised_database_startup()
{
	try {
		return ov_database_startup();
	}
	catch(...) {
		return OV_ERR_GENERIC;
	}
}

#endif
#else
#ifndef __cplusplus
/* function must be implemented separately */
#else
OV_RESULT ov_supervised_database_startup()
{
	return ov_database_startup();
}
#endif
#endif
/*	----------------------------------------------------------------------	*/
/*
*	runs server with supervised exceptions (subroutine)
*/

#ifdef OV_CATCH_EXCEPTIONS
#ifndef __cplusplus
/* function must be implemented separately */
#else
OV_RESULT ov_supervised_server_run()
{
	try {
		ov_ksserver_run();
		return OV_ERR_OK;

	}
	catch(...) {
		return OV_ERR_GENERIC;
	}
}
#endif
#else
#ifndef __cplusplus
/* function must be implemented separately */
#else
OV_RESULT ov_supervised_server_run()
{
	ov_ksserver_run();
	return OV_ERR_OK;
}
#endif
#endif
/*	----------------------------------------------------------------------	*/

#ifdef __cplusplus
}
#endif

/*
*	End of file
*/

