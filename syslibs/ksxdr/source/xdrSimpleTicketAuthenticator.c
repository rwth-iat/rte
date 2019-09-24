
/******************************************************************************
*
*   FILE
*   ----
*   xdrSimpleTicketAuthenticator.c
*
*   History
*   -------
*   2013-01-31   File created
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
#include "ov_macros.h"
#include "KSDATAPACKET_xdrhandling.h"
#include "NoneTicketAuthenticator.h"
#include "ksbase_helper.h"

/*
 * global pointer (within this file) to the authenticator itself (this implies no problem since the authenticator is a sigleton)
 * it is needed for the ticket functions to find necessary data
 */
static OV_INSTPTR_ksxdr_xdrSimpleTicketAuthenticator SimpleAuth = NULL;



OV_DLLFNCEXPORT OV_RESULT ksxdr_xdrSimpleTicketAuthenticator_constructor(
	OV_INSTPTR_ov_object 	pobj
) {

	OV_RESULT result;
	OV_INSTPTR_ksxdr_xdrSimpleTicketAuthenticator this = Ov_StaticPtrCast(ksxdr_xdrSimpleTicketAuthenticator, pobj);

	/* do what the base class does first */
	result = ksbase_TicketAuthenticator_constructor(pobj);
	if(Ov_Fail(result))
		return result;


	/*
	 * ksbase_TicketAuthenticator_constructor verifies that no other xdrSimpleTicketAuthenticator exists
	 * Hence, the global variable means no problems.
	 */
	SimpleAuth = this;

	this->v_TicketType = OV_TT_SIMPLE;
	this->v_access = OV_AC_NONE;
	this->v_TicketAccess = (OV_AC_READWRITE
							| OV_AC_DELETEABLE
							| OV_AC_INSTANTIABLE
							| OV_AC_LINKABLE
							| OV_AC_RENAMEABLE
							| OV_AC_UNLINKABLE);
	return OV_ERR_OK;
}

/*
 * initialize ticket id with NULL
 */
OV_DLLFNCEXPORT void ksxdr_xdrSimpleTicketAuthenticator_startup(
		OV_INSTPTR_ov_object 	pobj
) {

	OV_INSTPTR_ksxdr_xdrSimpleTicketAuthenticator this = Ov_StaticPtrCast(ksxdr_xdrSimpleTicketAuthenticator, pobj);

	/*	call baseclass' startup	*/
	ksbase_TicketAuthenticator_startup(pobj);

	this->v_ticket.ticketunion.simpleticket.id = NULL;

	return;
}

OV_DLLFNCEXPORT void ksxdr_xdrSimpleTicketAuthenticator_shutdown(
		OV_INSTPTR_ov_object 	pobj
) {
	OV_INSTPTR_ksxdr_xdrSimpleTicketAuthenticator this = Ov_StaticPtrCast(ksxdr_xdrSimpleTicketAuthenticator, pobj);

	if(this->v_ticket.ticketunion.simpleticket.id)
	{
		ov_free(this->v_ticket.ticketunion.simpleticket.id);
		this->v_ticket.ticketunion.simpleticket.id = NULL;
	}

	ksbase_TicketAuthenticator_shutdown(pobj);
	return;
}

OV_DLLFNCEXPORT OV_TICKET* ksxdr_xdrSimpleTicketAuthenticator_createticket(
	void *data,
	OV_TICKET_TYPE type
) {
    /*    
    *   local variables
    */
	KS_DATAPACKET* datapacket = (KS_DATAPACKET*) data;

	if(!data
		|| !SimpleAuth)
		return NULL;

	if(type != SimpleAuth->v_TicketType)
		return NULL;
	SimpleAuth->v_ticket.type = type;
	/*	free old id	*/
	if(SimpleAuth->v_ticket.ticketunion.simpleticket.id)
	{
		ov_free(SimpleAuth->v_ticket.ticketunion.simpleticket.id);
		SimpleAuth->v_ticket.ticketunion.simpleticket.id = NULL;
	}

	/*	read password into simpleticket id	*/
	if(Ov_Fail(KS_DATAPACKET_read_xdr_string(datapacket, &(SimpleAuth->v_ticket.ticketunion.simpleticket.id), KS_SIMPLETICKETID_MAXLEN)))
		return NULL;

	return &(SimpleAuth->v_ticket);

}

OV_DLLFNCEXPORT void ksxdr_xdrSimpleTicketAuthenticator_deleteticket(
	OV_TICKET *pticket
) {
    /*    
    *   local variables
    */
	if(SimpleAuth->v_ticket.ticketunion.simpleticket.id)
	{
		ov_free(pticket->ticketunion.simpleticket.id);
		pticket->ticketunion.simpleticket.id = NULL;
	}
    return;
}

OV_DLLFNCEXPORT OV_BOOL ksxdr_xdrSimpleTicketAuthenticator_encodereply(
	void *data, 
	OV_TICKET *pticket
) {
    /*    
    *   local variables
    */

    return TRUE;
}

OV_DLLFNCEXPORT OV_ACCESS ksxdr_xdrSimpleTicketAuthenticator_TicketGetaccess(
	const OV_TICKET *pticket
) {
    /*    
    *   local variables
    */
	OV_ACCESS localaccess = OV_AC_READ;

	if (pdb->serverpassword) {
		if (pticket->type == OV_TT_SIMPLE) {
			/*
			 *	only grant read access to anyone who does not have the serverpassword
			 */
			if(ov_string_compare(pticket->ticketunion.simpleticket.id, pdb->serverpassword) == OV_STRCMP_EQUAL) {
				if(SimpleAuth)
					localaccess = SimpleAuth->v_TicketAccess;	/*	grant acces configured for simple ticket	*/
				else
					localaccess = OV_AC_READWRITE | OV_AC_INSTANTIABLE;	/*	grant enough access to ctreate a SimpleTicketAuthenticator (or mess with the rights)	*/
			}
		}
	}
	else localaccess = ksbase_NoneAuth->v_TicketAccess;
	if(SimpleAuth)
		SimpleAuth->v_access = localaccess;

	return localaccess;
}

