/*
*	Copyright (C) 2011
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
*/
/***********************************************************************
*
*	File:
*	------
*	libraryopen.c
*
*	Editors:
*	--------
*	Je							Holger Jeromin <jeromin@plt.rwth-aachen.de>
*
*	CVS:
*	----
*	$Revision$
*	$Date$
*
*	History:
*	--------
*	20-Dez-2011		Je
*		-	File created
*
***********************************************************************/

#ifndef OV_COMPILE_LIBRARY_cshmi
#define OV_COMPILE_LIBRARY_cshmi
#endif

#include "cshmilib.h"


#ifdef ov_library_open_cshmi
#undef ov_library_open_cshmi
#endif

/*
* This function will be called, when the library is loaded.
* It creates the cshmi container objects
*/
OV_RESULT ov_library_setglobalvars_cshmi_new(void) {
	OV_RESULT result;
	OV_INSTPTR_ov_domain
		pTechunits = NULL;
	OV_INSTPTR_ov_domain
		pCsDomain = NULL;
	OV_INSTPTR_ov_domain
		pTemplateDomain = NULL;
	OV_INSTPTR_cshmi_downloadApplication
		pTurbo = NULL;

	/*
	 *    set the global variables of the original version
	 *    and if successful, load other libraries
	 *    and create some objects
	 */
	result = ov_library_setglobalvars_cshmi();

	if(Ov_Fail(result)){
		return result;
	}
	pTechunits = Ov_DynamicPtrCast(ov_domain, ov_path_getobjectpointer(FB_INSTANZ_CONTAINER_PATH, 2));
	if(pTechunits == NULL){
		result = Ov_CreateObject(ov_domain, pTechunits, &pdb->root, FB_INSTANZ_CONTAINER);
		if(Ov_Fail(result)){
			return result;
		}
	}
	pCsDomain = Ov_SearchChildEx(ov_containment, pTechunits, "cshmi", ov_domain);
	if(pCsDomain == NULL){
		result = Ov_CreateObject(ov_domain, pCsDomain, pTechunits, "cshmi");
		if(Ov_Fail(result)){
			return result;
		}
	}
	//optional instanciate an downloadApplication aka turbo object, do not abort on fail
	Ov_CreateObject(cshmi_downloadApplication, pTurbo, pCsDomain, "turbo");

	pTemplateDomain = Ov_SearchChildEx(ov_containment, pCsDomain, "Templates", ov_domain);
	if(pTemplateDomain == NULL){
		result = Ov_CreateObject(ov_domain, pTemplateDomain, pCsDomain, "Templates");
	}
	return result;
}
/*
*       Replace the 'setglobalvars' function of a library with this
*       previous one, which additionally creates instances.
* 	This is called by the OV system upon library load.
*/
OV_DLLFNCEXPORT OV_LIBRARY_DEF *ov_library_open_cshmi(void) {
	/* local variables */
	static OV_LIBRARY_DEF *OV_LIBRARY_DEF_cshmi_new;
	/*
	*       replace the 'setglobalvars' function created by the code generator
	*       with a new one.
	*/
	OV_LIBRARY_DEF_cshmi_new = ov_library_open_cshmi_old();
	OV_LIBRARY_DEF_cshmi_new->setglobalvarsfnc = ov_library_setglobalvars_cshmi_new;
	return OV_LIBRARY_DEF_cshmi_new;
}
