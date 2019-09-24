
/******************************************************************************
*
*   FILE
*   ----
*   library_open.c
*
*   History
*   -------
*   2013-01-15   File created
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
#include "ov_result.h"
#include "ov_logfile.h"
#include "ov_memstack.h"
#include "ov_result.h"
#include "ks_logfile.h"
#include "ksbase_helper.h"

#ifdef ov_library_open_ksxdr
#undef ov_library_open_ksxdr
#endif

/*
* This function will be called, when the library is loaded.
* It could generate components and initializes the startup procedure
*/
OV_RESULT ov_library_setglobalvars_ksxdr_new(void) {
	OV_RESULT result;
	OV_INSTPTR_ov_domain pcommunication = NULL;
	OV_INSTPTR_ov_domain pDomksxdr = NULL;
	OV_INSTPTR_ov_domain pDomTicketAuths = NULL;
	OV_INSTPTR_ksxdr_xdrIdentificator pIdentificator = NULL;
	OV_INSTPTR_ksxdr_xdrSimpleTicketAuthenticator pSimpleAuthenticator = NULL;
	OV_INSTPTR_ksxdr_xdrManagerCom xdrMngCom = NULL;
	/*
	 *    set the global variables of the original version
	 *    and if successful, load other libraries
	 *    and create some objects
	 */
	result = ov_library_setglobalvars_ksxdr();
	if(Ov_Fail(result))
		return result;

	KS_logfile_debug(("entering ksxdr_open"));

	//get the communication domain for putting in the ksxdr-objects
	pcommunication = Ov_StaticPtrCast(ov_domain, Ov_SearchChild(ov_containment, &(pdb->root), "communication"));
	if(!pcommunication) {
		result = Ov_CreateObject(ov_domain, pcommunication, &(pdb->root), "communication");
		if(Ov_Fail(result)) {
			ov_logfile_error("Fatal: Could not create Object 'communication'");
			return result;
		}
	}
	else if(!Ov_CanCastTo(ov_domain, (OV_INSTPTR_ov_object) pcommunication))
	{
		ov_logfile_error("Fatal: communication object found but not domain (or derived)");
		return OV_ERR_GENERIC;
	}

	pDomksxdr = Ov_StaticPtrCast(ov_domain, Ov_SearchChild(ov_containment, pcommunication, "ksxdr"));
	if(!pDomksxdr) {
		result = Ov_CreateObject(ov_domain, pDomksxdr, pcommunication, "ksxdr");
		if(Ov_Fail(result))
		{
			ov_logfile_error("Fatal: could not create ksxdr domain");
			return result;
		}
	}
	else if(!Ov_CanCastTo(ov_domain, (OV_INSTPTR_ov_object) pDomksxdr))
	{
		ov_logfile_error("Fatal: ksxdr object found but not domain (or derived)");
		return OV_ERR_GENERIC;
	}

		/*	create protocol identificator for ksxdr	*/
	pIdentificator = Ov_StaticPtrCast(ksxdr_xdrIdentificator, Ov_SearchChild(ov_containment, pDomksxdr, "Identificator"));
	if(pIdentificator)
		Ov_DeleteObject(pIdentificator);
	pIdentificator = NULL;

	result = Ov_CreateObject(ksxdr_xdrIdentificator, pIdentificator, pDomksxdr, "Identificator");
	if(Ov_Fail(result))
	{
		ov_logfile_error("Fatal: could not create Identificator object");
		return result;
	}

	/*	create ticket authenticators for ksxdr	*/
	pDomTicketAuths = Ov_StaticPtrCast(ov_domain, Ov_SearchChild(ov_containment, pDomksxdr, "TicketAuthenticators"));
	if(!pDomTicketAuths) {
		result = Ov_CreateObject(ov_domain, pDomTicketAuths, pDomksxdr, "TicketAuthenticators");
		if(Ov_Fail(result))
		{
			ov_logfile_error("Fatal: could not create TicketAuthenticators domain");
			return result;
		}
	}
	else if(!Ov_CanCastTo(ov_domain, (OV_INSTPTR_ov_object) pDomTicketAuths))
	{
		ov_logfile_error("Fatal: TicketAuthenticators object found but not domain (or derived)");
		return OV_ERR_GENERIC;
	}

	pSimpleAuthenticator = Ov_StaticPtrCast(ksxdr_xdrSimpleTicketAuthenticator, Ov_SearchChild(ov_containment, pDomTicketAuths, "xdrSimpleTicket"));
	if(!pSimpleAuthenticator)
	{
		result = Ov_CreateObject(ksxdr_xdrSimpleTicketAuthenticator, pSimpleAuthenticator, pDomTicketAuths, "xdrSimpleTicket");
		if(Ov_Fail(result))
		{
			ov_logfile_error("Fatal: could not create SimpleTicketAuthenticator object");
			return result;
		}
	}
	KS_logfile_debug(("ksxdr_open: creating ManagerCom"));
	/*	create xdrManagerCom object to register Server at the Manager (if wanted)	*/
	xdrMngCom = Ov_SearchChildEx(ov_containment, pDomksxdr, "ManagerCom", ksxdr_xdrManagerCom);
	if(!xdrMngCom)
	{
		result = Ov_CreateObject(ksxdr_xdrManagerCom, xdrMngCom, pDomksxdr, "ManagerCom");
		if(Ov_Fail(result))
		{
			KS_logfile_error(("ksxdr_open: ManagerCom could not be created"));
			return result;
		}
	}
	/*	if we are manager, we don't need a channel	*/
	if(Ov_GetFirstChild(ov_instantiation, pclass_ksbase_Manager)){
		xdrMngCom->v_UseShortCut = TRUE;
	}else{
		xdrMngCom->v_UseShortCut = FALSE;
	}

	KS_logfile_debug(("leaving ksxdr_open"));
	return OV_ERR_OK;

}


/*
 *       Replace the 'setglobalvars' function of a library with this
 *       previous one, which additionally creates instances.
 * 	This is called by the OV system upon library load.
 */
OV_DLLFNCEXPORT OV_LIBRARY_DEF *ov_library_open_ksxdr(void) {
	/* local variables */
	static OV_LIBRARY_DEF *OV_LIBRARY_DEF_ksxdr_new;
	/*
	*       replace the 'setglobalvars' function created by the code generator
	*       with a new one.
	*/
	OV_LIBRARY_DEF_ksxdr_new = ov_library_open_ksxdr_old();
	OV_LIBRARY_DEF_ksxdr_new->setglobalvarsfnc = ov_library_setglobalvars_ksxdr_new;
	return OV_LIBRARY_DEF_ksxdr_new;
}
