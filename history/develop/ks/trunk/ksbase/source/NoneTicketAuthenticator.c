
/******************************************************************************
*
*   FILE
*   ----
*   NoneTicketAuthenticator.c
*
*   History
*   -------
*   2013-02-25   File created
*
*******************************************************************************
*
*   This file is generated by the 'acplt_builder' command
*
******************************************************************************/


#ifndef OV_COMPILE_LIBRARY_ksbase
#define OV_COMPILE_LIBRARY_ksbase
#endif


#include "ksbase.h"
#include "libov/ov_macros.h"

/*
 * global pointer to the NoneTicketAuthenticator (this implies no problem since the authenticator is a sigleton)
 * it is needed for the ticket functions to find necessary data
 */
OV_DLLVAREXPORT OV_INSTPTR_ksbase_NoneTicketAuthenticator ksbase_NoneAuth = NULL;

OV_DLLFNCEXPORT OV_RESULT ksbase_NoneTicketAuthenticator_constructor(
		OV_INSTPTR_ov_object 	pobj
) {
	OV_RESULT result;

	result = ksbase_TicketAuthenticator_constructor(pobj);
	if(Ov_Fail(result))
		return result;

	((OV_INSTPTR_ksbase_NoneTicketAuthenticator) pobj)->v_NoneTicketAccess = (OV_AC_READWRITE
																			| OV_AC_DELETEABLE
																			| OV_AC_INSTANTIABLE
																			| OV_AC_LINKABLE
																			| OV_AC_RENAMEABLE
																			| OV_AC_UNLINKABLE);
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT void ksbase_NoneTicketAuthenticator_startup(
		OV_INSTPTR_ov_object 	pobj
) {

	OV_INSTPTR_ksbase_NoneTicketAuthenticator this = Ov_StaticPtrCast(ksbase_NoneTicketAuthenticator, pobj);

	/*	call baseclass' startup	*/
	ksbase_TicketAuthenticator_startup(pobj);

	this->v_TicketType = OV_TT_NONE;
	/*
	 * ksbase_TicketAuthenticator_constructor verifies that no other NoneTicketAuthenticator exists
	 * Hence, the global variable means no problems.
	 */
	ksbase_NoneAuth = this;


	return;
}

OV_DLLFNCEXPORT OV_BOOL ksbase_NoneTicketAuthenticator_ACRead_get(
    OV_INSTPTR_ksbase_NoneTicketAuthenticator          pobj
) {
	if(pobj->v_NoneTicketAccess & OV_AC_READ)
		return TRUE;
	else
		return FALSE;
}

OV_DLLFNCEXPORT OV_RESULT ksbase_NoneTicketAuthenticator_ACRead_set(
    OV_INSTPTR_ksbase_NoneTicketAuthenticator          pobj,
    const OV_BOOL  value
) {
    if(value)
    	pobj->v_NoneTicketAccess |= OV_AC_READ;
    else
    	pobj->v_NoneTicketAccess &= ~(OV_AC_READ);

	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_BOOL ksbase_NoneTicketAuthenticator_ACWrite_get(
    OV_INSTPTR_ksbase_NoneTicketAuthenticator          pobj
) {
	if(pobj->v_NoneTicketAccess & OV_AC_WRITE)
		return TRUE;
	else
		return FALSE;
}

OV_DLLFNCEXPORT OV_RESULT ksbase_NoneTicketAuthenticator_ACWrite_set(
    OV_INSTPTR_ksbase_NoneTicketAuthenticator          pobj,
    const OV_BOOL  value
) {
	 if(value)
		 pobj->v_NoneTicketAccess |= OV_AC_WRITE;
	 else
		 pobj->v_NoneTicketAccess &= ~(OV_AC_WRITE);
	 return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_BOOL ksbase_NoneTicketAuthenticator_ACInstantiate_get(
    OV_INSTPTR_ksbase_NoneTicketAuthenticator          pobj
) {
	if(pobj->v_NoneTicketAccess & OV_AC_INSTANTIABLE)
		return TRUE;
	else
		return FALSE;
}

OV_DLLFNCEXPORT OV_RESULT ksbase_NoneTicketAuthenticator_ACInstantiate_set(
    OV_INSTPTR_ksbase_NoneTicketAuthenticator          pobj,
    const OV_BOOL  value
) {
	 if(value)
		 pobj->v_NoneTicketAccess |= OV_AC_INSTANTIABLE;
	 else
		 pobj->v_NoneTicketAccess &= ~(OV_AC_INSTANTIABLE);
	 return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_BOOL ksbase_NoneTicketAuthenticator_ACDelete_get(
    OV_INSTPTR_ksbase_NoneTicketAuthenticator          pobj
) {
	if(pobj->v_NoneTicketAccess & OV_AC_DELETEABLE)
		return TRUE;
	else
		return FALSE;
}

OV_DLLFNCEXPORT OV_RESULT ksbase_NoneTicketAuthenticator_ACDelete_set(
    OV_INSTPTR_ksbase_NoneTicketAuthenticator          pobj,
    const OV_BOOL  value
) {
	 if(value)
		 pobj->v_NoneTicketAccess |= OV_AC_DELETEABLE;
	 else
		 pobj->v_NoneTicketAccess &= ~(OV_AC_DELETEABLE);
	 return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_BOOL ksbase_NoneTicketAuthenticator_ACRename_get(
    OV_INSTPTR_ksbase_NoneTicketAuthenticator          pobj
) {
	if(pobj->v_NoneTicketAccess & OV_AC_RENAMEABLE)
		return TRUE;
	else
		return FALSE;
}

OV_DLLFNCEXPORT OV_RESULT ksbase_NoneTicketAuthenticator_ACRename_set(
    OV_INSTPTR_ksbase_NoneTicketAuthenticator          pobj,
    const OV_BOOL  value
) {
	 if(value)
		 pobj->v_NoneTicketAccess |= OV_AC_RENAMEABLE;
	 else
		 pobj->v_NoneTicketAccess &= ~(OV_AC_RENAMEABLE);
	 return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_BOOL ksbase_NoneTicketAuthenticator_ACLink_get(
    OV_INSTPTR_ksbase_NoneTicketAuthenticator          pobj
) {
	if(pobj->v_NoneTicketAccess & OV_AC_LINKABLE)
		return TRUE;
	else
		return FALSE;
}

OV_DLLFNCEXPORT OV_RESULT ksbase_NoneTicketAuthenticator_ACLink_set(
    OV_INSTPTR_ksbase_NoneTicketAuthenticator          pobj,
    const OV_BOOL  value
) {
	 if(value)
		 pobj->v_NoneTicketAccess |= OV_AC_LINKABLE;
	 else
		 pobj->v_NoneTicketAccess &= ~(OV_AC_LINKABLE);
	 return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_BOOL ksbase_NoneTicketAuthenticator_ACUnlink_get(
    OV_INSTPTR_ksbase_NoneTicketAuthenticator          pobj
) {
	if(pobj->v_NoneTicketAccess & OV_AC_UNLINKABLE)
		return TRUE;
	else
		return FALSE;
}

OV_DLLFNCEXPORT OV_RESULT ksbase_NoneTicketAuthenticator_ACUnlink_set(
    OV_INSTPTR_ksbase_NoneTicketAuthenticator          pobj,
    const OV_BOOL  value
) {
	 if(value)
		 pobj->v_NoneTicketAccess |= OV_AC_UNLINKABLE;
	 else
		 pobj->v_NoneTicketAccess &= ~(OV_AC_UNLINKABLE);
	 return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_TICKET* ksbase_NoneTicketAuthenticator_createticket(
	void *data,
	OV_TICKET_TYPE type
) {
    return &ksbase_NoneAuth->v_ticket;
}

OV_DLLFNCEXPORT void ksbase_NoneTicketAuthenticator_deleteticket(
    OV_TICKET* pticket
) {
    return;
}

OV_DLLFNCEXPORT OV_RESULT ksbase_NoneTicketAuthenticator_encodereply(
	void *data,
	OV_TICKET *pticket
) {
    return TRUE;
}

OV_DLLFNCEXPORT OV_RESULT ksbase_NoneTicketAuthenticator_TicketGetaccess(
	const OV_TICKET *pticket
) {
    return ksbase_NoneAuth->v_NoneTicketAccess;
}
