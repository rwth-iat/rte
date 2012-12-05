#include <signal.h>

#if OV_SYSTEM_UNIX
#include <unistd.h>
#endif

#define OV_COMPILE_LIBOVKS


#include "ov_ksserver_stripped.h"
#include "ov_ksserver_xdr_stripped.h"
#include "ksserv_logfile.h"
#include "libov/ov_ov.h"
#include "libov/ov_object.h"
#include "libov/ov_scheduler.h"
#include "libov/ov_database.h"
#include "libov/ov_vendortree.h"
#include "libov/ov_macros.h"

#include "ov_ks_missing_symbols.h"




#if OV_SYSTEM_RMOS
#include <rmapi.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*	----------------------------------------------------------------------	*/

/*	----------------------------------------------------------------------	*/

/*
*	internal default VTable and functions for a default ticket
*/

static OV_TICKET *ov_ksserver_ticket_defaultticket_createticket2(XDR *xdr, OV_TICKET_TYPE type);
static void ov_ksserver_ticket_defaultticket_deleteticket(OV_TICKET *pticket);
static OV_BOOL ov_ksserver_ticket_defaultticket_encodereply(XDR *xdr, OV_TICKET *pticket);
static OV_ACCESS ov_ksserver_ticket_defaultticket_getaccess(const OV_TICKET *pticket);

static OV_ACCESS defaultaccess = OV_AC_READ;

OV_TICKET_VTBL defaultticketvtbl = {
	ov_ksserver_ticket_defaultticket_createticket2,
	ov_ksserver_ticket_defaultticket_deleteticket,
	ov_ksserver_ticket_defaultticket_encodereply,
	ov_ksserver_ticket_defaultticket_getaccess
};

OV_TICKET *ov_ksserver_ticket_defaultticket_createticket_wrap(XDR *xdr, OV_TICKET_TYPE type) {
    return ov_ksserver_ticket_defaultticket_createticket2(xdr, type);
}

static OV_TICKET *ov_ksserver_ticket_defaultticket_createticket2(XDR *xdr, OV_TICKET_TYPE type) {
	static OV_TICKET ticket = { &defaultticketvtbl,  OV_TT_SIMPLE };

	//ov_logfile_info("STRIPPED!!!: ov_ksserver_ticket_defaultticket_createticket2 called !!!");
	switch(type) {
		case OV_TT_NONE:
			break;
		case OV_TT_SIMPLE:
			if(!ov_ksserver_xdr_string(xdr, &ticket.ticketunion.simpleticket.id,
				KS_SIMPLEID_MAXLEN)
			) {
				return &ticket; //NULL;
			}
			break;
		default:
			return &ticket;//hmersch NULL
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

static OV_BOOL ov_ksserver_ticket_defaultticket_encodereply(XDR *y, OV_TICKET *x) {
	return TRUE;
}

static OV_ACCESS ov_ksserver_ticket_defaultticket_getaccess(const OV_TICKET *a) {
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
*	Service log
*/
struct OV_SVCLOG_TABLE_ENTRY {
	const OV_SVCLOG_VTBL			*vtbl;
};
typedef struct OV_SVCLOG_TABLE_ENTRY OV_SVCLOG_TABLE_ENTRY;

static OV_SVCLOG_TABLE_ENTRY	*plogentry = NULL;

/*
*	Register a new service logging
*/
OV_DLLFNCEXPORT OV_RESULT ov_ksserver_svclog_register(
	const OV_SVCLOG_VTBL	*vtbl
) {
	if(!vtbl) {
		return OV_ERR_BADPARAM;
	}
	if(plogentry) {
		/* Svclog already registered, modify vtable entry */
		plogentry->vtbl = vtbl;
		return OV_ERR_OK;
	}
	/* Create new entry */
	plogentry = Ov_HeapAlloc(OV_SVCLOG_TABLE_ENTRY);
	if(plogentry) {
		plogentry->vtbl = vtbl;
		return OV_ERR_OK;
	}
	return OV_ERR_HEAPOUTOFMEMORY;
}

/*
*	Unregister the service logging
*/
OV_DLLFNCEXPORT OV_RESULT ov_ksserver_svclog_unregister() {

	if(plogentry) {
		Ov_HeapFree(plogentry);
		plogentry = NULL;
		return OV_ERR_OK;
	}
	/* no service logging registered */
	return OV_ERR_BADPARAM;
}

/*
*	Log the service
*/
void ov_ksserver_svclog_log(
    OV_TICKET	*pticket,
    OV_SVC		service,
    void*		pparams,
    OV_RESULT	*presult,
    OV_RESULT   svcresult
) {
	/* Service logging registered? */
	if(!plogentry) {
		/* no service logging registered */
		return;
	}
	
	plogentry->vtbl->logsvc(pticket, service, pparams, presult, svcresult);
	return;
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
	//Ov_Warning("Entering dispatch with service %d", serivce);
	//ov_logfile_info("Entering dispatch with service %d, version %d", service, version);
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
     		Ov_Warning("ov_ksserver_stripepd / Entering default in switch");
		/*
		*	unknown service id, send error reply and unlock memory stack
		*/
        result = OV_ERR_NOTIMPLEMENTED;
		ov_ksserver_sendreply(xdrout, pticket, &result, NULL);
		return;
	}
	Ov_Warning("leaving switch");
	/*
	*	error decoding
	*/
	Ov_Warning("error decoding request");
	result = OV_ERR_GENERIC;
	ov_ksserver_sendreply(xdrout, pticket, &result, NULL);
}



/*	----------------------------------------------------------------------	*/

/*
*	Helper macro for implementing the service dispatcher
*/
#define Ov_KsServer_Dispatch_woanswer2(service, SERVICE)								\
	case KS_##SERVICE: {													\
			/*																\
			*	parameter and result objects								\
			*/																\
			OV_##SERVICE##_PAR	params;										\
			OV_##SERVICE##_RES	result;										\
			/*																\
			*	decode the parameters										\
			*/																\
			ov_memstack_lock(); \
			if(ov_ksserver_xdr_OV_##SERVICE##_PAR(xdrin, &params)) {		\
				/*															\
				*	properly decoded, call service function and send reply	\
				*/															\
				 ksserv_logfile_info("Executing Command."); \
				ov_ksserver_##service(version, povticket, &params, &result);\
				ov_ksserver_sendreply_woanswer22(xdrout, pticket, (OV_RESULT*)&result,	\
					(xdrproc_t)ov_ksserver_xdr_OV_##SERVICE##_RES);			\
				ov_memstack_unlock(); \
				return;														\
			}																\
		}																	\
		ov_memstack_unlock(); \
		break











/*
*	Dispatch a service (subroutine)
*/
void ov_ksserver_dispatch_wo_answer(
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
	OV_TICKET *povticket = pticket;

	//ov_logfile_info("Creating OV Ticket in ov_ksserver_dispatch_wo_answer");

	//printf("just C\n");
	/*
	*	dispatch the service
	*/
	//Ov_Warning("Entering dispatch with service %d", serivce);
	//ov_logfile_info("!!!!!!!!!!!!!!!!!!CALLED  ov_ksserver_dispatch_wo_answer from STRIPPED !!!");
	//ov_logfile_info("Entering dispatch_wo_answer with service %d, version %d", service, version);


	switch(service) {
	Ov_KsServer_Dispatch_woanswer2(getpp, GETPP);
	case (KS_GETVAR): {
				/*
				*	parameter and result objects
				*/
				OV_GETVAR_PAR	params;
				OV_GETVAR_RES	result;
				/*
				*	decode the parameters
				*/
				ov_memstack_lock();
				if(ov_ksserver_xdr_OV_GETVAR_PAR(xdrin, &params)) {
					/*
					*	properly decoded, call service function and send reply
					*/
					ksserv_logfile_info("Executing Command.");
					ov_ksserver_getvar(version, povticket, &params, &result);
					ov_ksserver_sendreply_woanswer22(xdrout, pticket, (OV_RESULT*)&result,
						(xdrproc_t)ov_ksserver_xdr_OV_GETVAR_RES);
					ov_memstack_unlock();
					return;
				}
			}
			ov_memstack_unlock();
			break;
	case (KS_SETVAR): {
				/*
				*	parameter and result objects
				*/
				OV_SETVAR_PAR	params;
				OV_SETVAR_RES	result;
				/*
				*	decode the parameters
				*/
				ov_memstack_lock();
				if(ov_ksserver_xdr_OV_SETVAR_PAR(xdrin, &params)) {
					/*
					*	properly decoded, call service function and send reply
					*/
					 ksserv_logfile_info("Executing Command.");
					ov_ksserver_setvar(version, povticket, &params, &result);
					ov_ksserver_sendreply_woanswer22(xdrout, pticket, (OV_RESULT*)&result,
						(xdrproc_t)ov_ksserver_xdr_OV_SETVAR_RES);
					ov_memstack_unlock();
					return;
				}
			}
			ov_memstack_unlock();
			break;
	Ov_KsServer_Dispatch_woanswer2(exgdata, EXGDATA);
	case (KS_GETEP): {
				/*
				*	parameter and result objects
				*/
				OV_GETEP_PAR	params;
				OV_GETEP_RES	result;
				/*
				*	decode the parameters
				*/
				ov_memstack_lock();
				if(ov_ksserver_xdr_OV_GETEP_PAR(xdrin, &params)) {
					/*
					*	properly decoded, call service function and send reply
					*/
					 ksserv_logfile_info("Executing Command.");
					ov_ksserver_getep(version, povticket, &params, &result);
					ov_ksserver_sendreply_woanswer22(xdrout, pticket, (OV_RESULT*)&result,
						(xdrproc_t)ov_ksserver_xdr_OV_GETEP_RES);
					ov_memstack_unlock();
					return;
				}
			}
			ov_memstack_unlock();
			break;
	case (KS_CREATEOBJECT): {
				/*
				*	parameter and result objects
				*/
				OV_CREATEOBJECT_PAR	params;
				OV_CREATEOBJECT_RES	result;
				/*
				*	decode the parameters
				*/
				ov_memstack_lock();
				if(ov_ksserver_xdr_OV_CREATEOBJECT_PAR(xdrin, &params)) {
					/*
					*	properly decoded, call service function and send reply
					*/
					 ksserv_logfile_info("Executing Command.");
					ov_ksserver_createobject(version, povticket, &params, &result);
					ov_ksserver_sendreply_woanswer22(xdrout, pticket, (OV_RESULT*)&result,
						(xdrproc_t)ov_ksserver_xdr_OV_CREATEOBJECT_RES);
					ov_memstack_unlock();
					return;
				}
			}
			ov_memstack_unlock();
			break;
	case (KS_DELETEOBJECT): {
				/*
				*	parameter and result objects
				*/
				OV_DELETEOBJECT_PAR	params;
				OV_DELETEOBJECT_RES	result;
				/*
				*	decode the parameters
				*/
				ov_memstack_lock();
				if(ov_ksserver_xdr_OV_DELETEOBJECT_PAR(xdrin, &params)) {
					/*
					*	properly decoded, call service function and send reply
					*/
					 ksserv_logfile_info("Executing Command.");
					ov_ksserver_deleteobject(version, povticket, &params, &result);
					ov_ksserver_sendreply_woanswer22(xdrout, pticket, (OV_RESULT*)&result,
						(xdrproc_t)ov_ksserver_xdr_OV_DELETEOBJECT_RES);
					ov_memstack_unlock();
					return;
				}
			}
			ov_memstack_unlock();
			break;
	case (KS_RENAMEOBJECT): {
				/*
				*	parameter and result objects
				*/
				OV_RENAMEOBJECT_PAR	params;
				OV_RENAMEOBJECT_RES	result;
				/*
				*	decode the parameters
				*/
				ov_memstack_lock();
				if(ov_ksserver_xdr_OV_RENAMEOBJECT_PAR(xdrin, &params)) {
					/*
					*	properly decoded, call service function and send reply
					*/
					 ksserv_logfile_info("Executing Command.");
					ov_ksserver_renameobject(version, povticket, &params, &result);
					ov_ksserver_sendreply_woanswer22(xdrout, pticket, (OV_RESULT*)&result,
						(xdrproc_t)ov_ksserver_xdr_OV_RENAMEOBJECT_RES);
					ov_memstack_unlock();
					return;
				}
			}
			ov_memstack_unlock();
			break;
	Ov_KsServer_Dispatch_woanswer2(getcanonicalpath, GETCANONICALPATH);
	case (KS_LINK): {
				/*
				*	parameter and result objects
				*/
				OV_LINK_PAR	params;
				OV_LINK_RES	result;
				/*
				*	decode the parameters
				*/
				ov_memstack_lock();
				if(ov_ksserver_xdr_OV_LINK_PAR(xdrin, &params)) {
					/*
					*	properly decoded, call service function and send reply
					*/
					 ksserv_logfile_info("Executing Command.");
					ov_ksserver_link(version, povticket, &params, &result);
					ov_ksserver_sendreply_woanswer22(xdrout, pticket, (OV_RESULT*)&result,
						(xdrproc_t)ov_ksserver_xdr_OV_LINK_RES);
					ov_memstack_unlock();
					return;
				}
			}
			ov_memstack_unlock();
			break;
	case (KS_UNLINK): {
				/*
				*	parameter and result objects
				*/
				OV_UNLINK_PAR	params;
				OV_UNLINK_RES	result;
				/*
				*	decode the parameters
				*/
				ov_memstack_lock();
				if(ov_ksserver_xdr_OV_UNLINK_PAR(xdrin, &params)) {
					/*
					*	properly decoded, call service function and send reply
					*/
					 ksserv_logfile_info("Executing Command.");
					ov_ksserver_unlink(version, povticket, &params, &result);
					ov_ksserver_sendreply_woanswer22(xdrout, pticket, (OV_RESULT*)&result,
						(xdrproc_t)ov_ksserver_xdr_OV_UNLINK_RES);
					ov_memstack_unlock();
					return;
				}
			}
			ov_memstack_unlock();
			break;
	Ov_KsServer_Dispatch_woanswer2(gethist, GETHIST);
	default:
     		Ov_Warning("Entering default in switch");
		/*
		*	unknown service id, send error reply and unlock memory stack
		*/
	        result = OV_ERR_NOTIMPLEMENTED;
		ov_ksserver_sendreply(xdrout, pticket, &result, NULL);
		return;
	}
	Ov_Warning("leaving switch");
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
/* function must be implemented separately; implementation could look like this: */
OV_DLLFNCEXPORT void ov_ksserver_sendreply(
	XDR			*xdrs,
	OV_TICKET	*pticket,
	OV_RESULT	*presult,
	xdrproc_t	encodefnc
) {
	return;
}

OV_DLLFNCEXPORT void ov_ksserver_sendreply_woanswer22 (
	XDR			*xdrs,
	OV_TICKET	*pticket,
	OV_RESULT	*presult,
	xdrproc_t	encodefnc
) {
	u_long dummy = 0x80000000ul;     // Make sure there�s room for the

	//ov_logfile_info("!!!!!!!!!!!!!!!!!!CALLED ov_ksserver_sendreply_woanswer222 from STRIPPED !!!");
	if(encodefnc) {
		/*
		*	send a normal reply
		*/
		//ov_logfile_info("with encodefnc 0 normal reply wo answer");
		//OvKsResult kssvrresult(presult, encodefnc);
		//ov_logfile_info("generated result for wo answer");
		//((KssTransport*)xdrs)->sendReply(*(OvKsAvTicket*)pticket, kssvrresult);
    		//xdrmemstream_clear(xdrs);
		//_rpc_header.acceptCall();
		if ( xdr_u_long(xdrs, &dummy)  // fragment header we�ll fix later...
	//    	     && _rpc_header.xdrEncode(&xdrs)
	//	     && avt.xdrEncode(xdrs)
	//     	     && kssvrresult.xdrEncode(xdrs)
		     && encodefnc(xdrs, presult)
	//	     && avt.xdrEncodeTrailer(xdrs) 
		) 	{
		//ov_logfile_info("###############generated XDR answer wo_answer...");
		} else {
			ov_logfile_error("###############cant generate XDR answer...");
		}

	} else {
		/*
		*	send an error reply
		*/
		ov_logfile_error("without encodefnc - error reply wo answer- will seq fault isnt implemtnted");
		//((KssTransport*)xdrs)->sendErrorReply(*(OvKsAvTicket*)pticket, *presult);
	}
   	//ov_memstack_unlock();
	return;
}


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

#ifdef __cplusplus
}
#endif
/*
*	End of file
*/


