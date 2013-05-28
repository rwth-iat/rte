/*
*	Copyright (C) 2013
*	Chair of Process Control Engineering,
*	Aachen University of Technology.
*	All rights reserved.
*
*	Redistribution and use in source and binary forms, with or without
*	modification, are permitted provided that the following conditions
*	are met:
*	1. Redistributions of source code must retain the above copyright
*	   notice, this list of conditions and the following disclaimer.
*	2. Redistributions in binary form must print or display the above
*	   copyright notice either during startup or must have a means for
*	   the user to view the copyright notice.
*	3. Redistributions in binary form must reproduce the above copyright
*	   notice, this list of conditions and the following disclaimer in
*		the documentation and/or other materials provided with the
*		distribution.
*	4. Neither the name of the Chair of Process Control Engineering nor
*		the name of the Aachen University of Technology may be used to
*		endorse or promote products derived from this software without
*		specific prior written permission.
*
*	THIS SOFTWARE IS PROVIDED BY THE CHAIR OF PROCESS CONTROL ENGINEERING
*	``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*	LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*	A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE CHAIR OF
*	PROCESS CONTROL ENGINEERING BE LIABLE FOR ANY DIRECT, INDIRECT,
*	INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
*	BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
*	OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
*	AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
*	LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
*	WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
*	POSSIBILITY OF SUCH DAMAGE.
*
***********************************************************************/

#ifndef OV_COMPILE_LIBRARY_kshttp
#define OV_COMPILE_LIBRARY_kshttp
#endif

#include "kshttp.h"

#ifdef ov_library_open_kshttp
#undef ov_library_open_kshttp
#endif


#include "config.h"

/*
* This function will be called, when the library is loaded.
* It could generate components and initializes the startup procedure
*/
OV_RESULT ov_library_setglobalvars_kshttp_new(void) {
	OV_RESULT result;
	OV_INSTPTR_ov_domain pcommunication = NULL;
	OV_INSTPTR_ov_domain pDomkshttp = NULL;
	OV_INSTPTR_ov_domain pDomTicketAuths = NULL;
	OV_INSTPTR_kshttp_httpIdentificator pIdentificator = NULL;
	OV_INSTPTR_kshttp_httpSimpleTicketAuthenticator pSimpleAuthenticator = NULL;
	OV_INSTPTR_kshttp_httpManagerCom httpMngCom = NULL;
	OV_INSTPTR_ov_domain httpStaticfiles = NULL;
	OV_INSTPTR_ov_domain httpSessions = NULL;

	/*
	 *    set the global variables of the original version
	 *    and if successful, load other libraries
	 *    and create some objects
	 */
	result = ov_library_setglobalvars_kshttp();
	if(Ov_Fail(result))
		return result;

	KS_logfile_debug(("entering kshttp_open"));

	//get the communication domain for putting in the kshttp-objects
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

	pDomkshttp = Ov_StaticPtrCast(ov_domain, Ov_SearchChild(ov_containment, pcommunication, "kshttp"));
	if(!pDomkshttp) {
		result = Ov_CreateObject(ov_domain, pDomkshttp, pcommunication, "kshttp");
		if(Ov_Fail(result))
		{
			ov_logfile_error("Fatal: could not create kshttp domain");
			return result;
		}
	}
	else if(!Ov_CanCastTo(ov_domain, (OV_INSTPTR_ov_object) pDomkshttp))
	{
		ov_logfile_error("Fatal: kshttp object found but not domain (or derived)");
		return OV_ERR_GENERIC;
	}

		/*	create protocol identificator for kshttp	*/
	pIdentificator = Ov_StaticPtrCast(kshttp_httpIdentificator, Ov_SearchChild(ov_containment, pDomkshttp, "Identificator"));
	if(pIdentificator){
		Ov_DeleteObject(pIdentificator);
	}
	pIdentificator = NULL;

	result = Ov_CreateObject(kshttp_httpIdentificator, pIdentificator, pDomkshttp, "Identificator");
	if(Ov_Fail(result))
	{
		ov_logfile_error("Fatal: could not create Identificator object");
		return result;
	}

	/*	create ticket authenticators for kshttp	*/
	pDomTicketAuths = Ov_StaticPtrCast(ov_domain, Ov_SearchChild(ov_containment, pDomkshttp, "TicketAuthenticators"));
	if(!pDomTicketAuths) {
		result = Ov_CreateObject(ov_domain, pDomTicketAuths, pDomkshttp, "TicketAuthenticators");
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

	pSimpleAuthenticator = Ov_StaticPtrCast(kshttp_httpSimpleTicketAuthenticator, Ov_SearchChild(ov_containment, pDomTicketAuths, "httpSimpleTicket"));
	if(pSimpleAuthenticator)
		Ov_DeleteObject(pSimpleAuthenticator);
	pSimpleAuthenticator = NULL;

	result = Ov_CreateObject(kshttp_httpSimpleTicketAuthenticator, pSimpleAuthenticator, pDomTicketAuths, "httpSimpleTicket");
	if(Ov_Fail(result))
	{
		ov_logfile_error("Fatal: could not create SimpleTicketAuthenticator object");
		return result;
	}

	KS_logfile_debug(("kshttp_open: creating ManagerCom"));
	/*	create httpManagerCom object to register Server at the Manager (if wanted)	*/
	httpMngCom = Ov_SearchChildEx(ov_containment, pDomkshttp, "ManagerCom", kshttp_httpManagerCom);
	if(!httpMngCom)
	{
		result = Ov_CreateObject(kshttp_httpManagerCom, httpMngCom, pDomkshttp, "ManagerCom");
		if(Ov_Fail(result))
		{
			KS_logfile_error(("kshttp_open: ManagerCom could not be created"));
			return result;
		}

		/*	if we are manager, we don't need a channel	*/
		if(Ov_GetFirstChild(ov_instantiation, pclass_ksbase_Manager))
			httpMngCom->v_UseShortCut = TRUE;
	}


   /*
	* 		create "staticfiles" container
	*/
	/*	create ticket authenticators for kshttp	*/
	httpStaticfiles = Ov_StaticPtrCast(ov_domain, Ov_SearchChild(ov_containment, pDomkshttp, "staticfiles"));
	if(!httpStaticfiles) {
		result = Ov_CreateObject(ov_domain, httpStaticfiles, pDomkshttp, "staticfiles");
		if(Ov_Fail(result))
		{
			ov_logfile_error("Fatal: could not create staticfiles domain");
			return result;
		}
	}
	else if(!Ov_CanCastTo(ov_domain, (OV_INSTPTR_ov_object) httpStaticfiles))
	{
		ov_logfile_error("Fatal: staticfiles object found but not domain (or derived)");
		return OV_ERR_GENERIC;
	}


	/*
	 * 		create "sessions" container
	 */
	httpSessions = Ov_StaticPtrCast(ov_domain, Ov_SearchChild(ov_containment, pDomkshttp, "sessions"));
	if(!httpSessions) {
		result = Ov_CreateObject(ov_domain, httpSessions, pDomkshttp, "sessions");
		if(Ov_Fail(result))
		{
			ov_logfile_error("Fatal: could not create sessions domain");
			return result;
		}
	}
	else if(!Ov_CanCastTo(ov_domain, (OV_INSTPTR_ov_object) httpSessions))
	{
		ov_logfile_error("Fatal: sessions object found but not domain (or derived)");
		return OV_ERR_GENERIC;
	}


	/*
	 * calling a generated wrapper function to create all the static files from /staticfiles dir
	 * this function is generated in prebuild.tcl
	 */
	result = include_localfiles(httpStaticfiles);
	if(Ov_Fail(result))
	{
		return result;
	}


	KS_logfile_debug(("leaving kshttp_open"));
	return OV_ERR_OK;

}

/*
*       Replace the 'setglobalvars' function of a library with this
*       previous one, which additionally creates instances.
* 	This is called by the OV system upon library load.
*/
OV_DLLFNCEXPORT OV_LIBRARY_DEF *ov_library_open_kshttp(void) {
	/* local variables */
	static OV_LIBRARY_DEF *OV_LIBRARY_DEF_kshttp_new;
	/*
	*       replace the 'setglobalvars' function created by the code generator
	*       with a new one.
	*/

	OV_INSTPTR_ov_library pNewOvLib = NULL;
	OV_RESULT fr = OV_ERR_OK;
	//this is executed at database start, too

	fr = Ov_CreateObject(ov_library, pNewOvLib, &pdb->acplt, "ksbase");
	if (fr != OV_ERR_ALREADYEXISTS && Ov_Fail(fr))
	{
		ov_logfile_error("kshttp needs the library %s which is not available (%s).",
				"ksbase",
				ov_result_getresulttext(fr));
	}

	OV_LIBRARY_DEF_kshttp_new = ov_library_open_kshttp_old();
	OV_LIBRARY_DEF_kshttp_new->setglobalvarsfnc = ov_library_setglobalvars_kshttp_new;
	return OV_LIBRARY_DEF_kshttp_new;
}
