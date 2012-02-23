/*
*	Copyright (C) 2012
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

#ifndef OV_COMPILE_LIBRARY_ksservhttp
#define OV_COMPILE_LIBRARY_ksservhttp
#endif

#include "ksservhttp.h"

#ifdef ov_library_open_ksservhttp
#undef ov_library_open_ksservhttp
#endif


#include "libov/ov_macros.h"
#include "libov/ov_path.h"
#include "libov/ov_string.h"
#include "libov/ov_logfile.h"

/*
* This function will be called, when the library is loaded.
* It could generate components and initializes the startup procedure
*/
OV_RESULT ov_library_setglobalvars_ksservhttp_new(void) {
	OV_RESULT result;

	OV_INSTPTR_ov_domain			phttpservers  = NULL;
	OV_INSTPTR_ov_domain			pcommunication  = NULL;
	OV_INSTPTR_ksservhttp_httpserver	phttpserver  = NULL;
	OV_INSTPTR_ov_domain			pstaticfiles = NULL;
	OV_INSTPTR_ksservhttp_staticfile	pindexhtml  = NULL;

	/*
	 *    set the global variables of the original version
	 *    and if successful, load other libraries
	 *    and create some objects
	 */
	result = ov_library_setglobalvars_ksservhttp();

	if(Ov_OK(result))
	{
		/*
		*       create communication container
		*/
		//communication container should be provided by ksserv
		pcommunication = (OV_INSTPTR_ov_domain)Ov_SearchChild(ov_containment, &pdb->root, "communication");
		if(!pcommunication)
		{
			result = Ov_CreateObject(ov_domain, pcommunication, &pdb->root, "communication");
			if(Ov_Fail(result))
			{
				ov_logfile_error("Fatal: Could not create Object 'communication'");
				return result;
			}
		}

		/*
		*       create httpservers container
		*/
		phttpservers = Ov_SearchChildEx(ov_containment, pcommunication, "httpservers", ov_domain);
		if(!phttpservers)
		{
			result = Ov_CreateObject(ov_domain, phttpservers, pcommunication, "httpservers");

			if(Ov_Fail(result))
			{
				ov_logfile_error("Fatal: Could not create Object 'httpservers'");
				return result;
			}
		}

		/*
		*       create "httpserver" object
		*/
	   phttpserver = Ov_SearchChildEx(ov_containment, phttpservers, "httpserver", ksservhttp_httpserver);
	   if(!phttpserver)
	   {
    	   result = Ov_CreateObject(ksservhttp_httpserver, phttpserver, phttpservers, "httpserver");
    	   if(Ov_Fail(result))
    	   {
				ov_logfile_error("Fatal: Could not create Object 'httpserver'");
			   return result;
		   }
		}

	   /*
	    * 		create "staticfiles" container
	    */
		pstaticfiles = Ov_SearchChildEx(ov_containment, phttpserver, "staticfiles", ov_domain);
		if(!phttpservers)
		{
			result = Ov_CreateObject(ov_domain, pstaticfiles, phttpserver, "staticfiles");

			if(Ov_Fail(result))
			{
				ov_logfile_error("Fatal: Could not create Object 'staticfiles'");
				return result;
			}
		}
		/*
		 * 		create "index.html"
		 */
		pindexhtml = Ov_SearchChildEx(ov_containment, pstaticfiles, "index.html", ksservhttp_staticfile);
		if(!pindexhtml)
		{
			result = Ov_CreateObject(ksservhttp_staticfile, pindexhtml, pstaticfiles, "index.html");

			if(Ov_Fail(result))
			{
				ov_logfile_error("Fatal: Could not create Object 'index.html'");
				return result;
			}

			//set default value
			ov_string_setvalue(&(pindexhtml->v_content), "<!DOCTYPE html><html><head><title>OV-http Server</title></head><body><h1>It works!</h1>Powered by ACPLT/OV.</body></html>");

		}
	}

	return result;
}
/*
*       Replace the 'setglobalvars' function of a library with this
*       previous one, which additionally creates instances.
* 	This is called by the OV system upon library load.
*/
OV_DLLFNCEXPORT OV_LIBRARY_DEF *ov_library_open_ksservhttp(void) {
	/* local variables */
	static OV_LIBRARY_DEF *OV_LIBRARY_DEF_ksservhttp_new;
	/*
	*       replace the 'setglobalvars' function created by the code generator
	*       with a new one.
	*/
	OV_LIBRARY_DEF_ksservhttp_new = ov_library_open_ksservhttp_old();
	OV_LIBRARY_DEF_ksservhttp_new->setglobalvarsfnc = ov_library_setglobalvars_ksservhttp_new;
	return OV_LIBRARY_DEF_ksservhttp_new;
}
