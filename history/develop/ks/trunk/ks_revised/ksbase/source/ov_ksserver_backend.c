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

static OV_TICKET *ksxdr_ticket_noneticket_createticket(void *data, OV_TICKET_TYPE type);
static void ksxdr_ticket_noneticket_deleteticket(OV_TICKET *pticket);
static OV_BOOL ksxdr_ticket_noneticket_encodereply(void *data, OV_TICKET *pticket);
static OV_ACCESS ksxdr_ticket_noneticket_getaccess(const OV_TICKET *pticket);

static OV_ACCESS noneticketaccess = OV_AC_READ;

OV_DLLVAREXPORT OV_TICKET_VTBL noneticketvtbl = {
		ksxdr_ticket_noneticket_createticket,
		ksxdr_ticket_noneticket_deleteticket,
		ksxdr_ticket_noneticket_encodereply,
		ksxdr_ticket_noneticket_getaccess
};


static OV_TICKET *ksxdr_ticket_noneticket_createticket(void *data, OV_TICKET_TYPE type) {
	static OV_TICKET ticket = { &noneticketvtbl,  OV_TT_NONE };

	//ov_logfile_info("STRIPPED!!!: ov_ksserver_ticket_noneticket_createticket2 called !!!");
	switch(type) {
		case OV_TT_NONE:
			break;
		default:
			return NULL;
	}
	return &ticket;
}

static void ksxdr_ticket_noneticket_deleteticket(OV_TICKET *pticket) {
	/*
	*	local variables
	*/
	/*
	*	just let XDR free the memory it allocated during construction of the id
	*/
	return;
}

static OV_BOOL ksxdr_ticket_noneticket_encodereply(void* data, OV_TICKET *x) {
	return TRUE;
}

static OV_ACCESS ksxdr_ticket_noneticket_getaccess(const OV_TICKET *a) {
	return noneticketaccess;
}

/*	----------------------------------------------------------------------	*/

/*
*	Set access rights for none tickets
*/
OV_DLLFNCEXPORT void ov_ksserver_ticket_setnoneticketaccess(OV_ACCESS access) {
	noneticketaccess = access & (OV_AC_READWRITE | OV_AC_INSTANTIABLE
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


