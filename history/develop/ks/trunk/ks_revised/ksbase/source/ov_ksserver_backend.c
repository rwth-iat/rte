#include <signal.h>

#if OV_SYSTEM_UNIX
#include <unistd.h>
#endif

#define OV_COMPILE_LIBOVKS


#include "ov_ksserver_backend.h"
#include "ks_logfile.h"
#include "libov/ov_ov.h"
#include "libov/ov_object.h"
#include "libov/ov_scheduler.h"
#include "libov/ov_database.h"
#include "libov/ov_vendortree.h"
#include "libov/ov_macros.h"




#if OV_SYSTEM_RMOS
#include <rmapi.h>
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

OV_DLLVAREXPORT OV_TICKET_VTBL defaultticketvtbl = {
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
		/*	if(!ov_ksserver_xdr_string(xdr, &ticket.ticketunion.simpleticket.id,
				KS_SIMPLEID_MAXLEN)
			) {
				return &ticket; //NULL;
			}*/		//TODO change handling
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
		//ov_ksserver_xdr_string(&xdr, &(pticket->ticketunion.simpleticket.id), KS_SIMPLEID_MAXLEN); //TODO change handling
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
*	End of file
*/


