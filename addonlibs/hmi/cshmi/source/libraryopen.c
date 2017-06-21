/*
*   Copyright (C) 2011
*   Chair of Process Control Engineering,
*   D-52056 Aachen, Germany.
*   All rights reserved.
*
*
*   This file is part of the ACPLT/OV Package 
*   Licensed under the Apache License, Version 2.0 (the "License");
*   you may not use this file except in compliance with the License.
*   You may obtain a copy of the License at
*
*       http://www.apache.org/licenses/LICENSE-2.0
*
*   Unless required by applicable law or agreed to in writing, software
*   distributed under the License is distributed on an "AS IS" BASIS,
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*   See the License for the specific language governing permissions and
*   limitations under the License.
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
