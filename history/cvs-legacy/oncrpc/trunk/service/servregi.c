/*
 * servregi.c -- utility functions for self-registering or unregistering
 *               NT services. This can also be used with manual registration
 *               of a service from the command line. This is only the
 *               interface part to be used from service executables.
 *
 * (c) 1999 Harald Albrecht <harald@plt.rwth-aachen.de>
 * Chair of Process Control Engineering,
 * Aachen University of Technology, Germany.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must print or display the above
 *    copyright notice either during startup or must have a means for
 *    the user to view the copyright notice.
 * 3. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 4. Neither the name of the Chair of Process Control Engineering nor the
 *    name of the Aachen University of Technology may be used to endorse or
 *    promote products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE CHAIR OF PROCESS CONTROL ENGINEERING
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE CHAIR OF PROCESS CONTROL
 * ENGINEERING BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <windows.h>

#include "servregids.h"
#include "servregi.h"


/* --------------------------------------------------------------------------
 * Function pointers to the registration mumbo jumbo.
 */
typedef (*RegisterServiceType)(HMODULE,
			       char *, char *, char *,
			       NtServiceInstallationMode);
typedef (*UnregisterServiceType)(HMODULE,
				 char *,
				 NtServiceInstallationMode);

/* --------------------------------------------------------------------------
 *
 */
int
RegisterService(HMODULE hModule, NtServiceInstallationMode instMode)
{
    char                 moduleName[MAX_PATH];
    char                *pExt;
    HMODULE              hservRegModule;
    RegisterServiceType  regService;
    int                  result;
    char                 serviceId[64];
    char                 serviceDisplay[128];
    char                 serviceDeps[1024];

    /*
     * Determine the name of the service registration helper module from
     * this module's name. Just replace the module's filename extension
     * with ".dll".
     */
    if ( GetModuleFileName(hModule, moduleName, sizeof(moduleName) - 1) == 0 ) {
    	MessageBox(0,
		           "Can't determine service registration helper module.",
		           "NT Service Installation",
		           MB_OK | MB_ICONEXCLAMATION);
	    return 900;
    }
    pExt = strrchr(moduleName, '.');
    if ( pExt ) {
    	*pExt = 0;
    }
    strncat(moduleName, SELFREGISTRATIONDLLEXTENSION, sizeof(moduleName));
    /*
     * Now try to load the service registration helper module and get
     * your hands on the RegisterService() method.
     */
    hservRegModule = LoadLibrary(moduleName);
    if ( !hservRegModule ) {
    	MessageBox(0,
		           "Can't find service registration helper module.",
		           "NT Service Installation",
		           MB_OK | MB_ICONEXCLAMATION);
	    return 901;
    }
    regService = (RegisterServiceType)
        GetProcAddress(hservRegModule, "_RegisterService");
    if ( !regService ) {
	    regService = (RegisterServiceType)
            GetProcAddress(hservRegModule, "RegisterService");
    }
    if ( !regService ) {
	    MessageBox(0,
		           "Invalid service registration helper module.",
		           "NT Service Installation",
		           MB_OK | MB_ICONEXCLAMATION);
	    return 902;
    }

    if ( !LoadString(hservRegModule, IDS_SERVICEID, serviceId, sizeof(serviceId)) ||
         !LoadString(hservRegModule, IDS_SERVICEDISPLAY, serviceDisplay,
                     sizeof(serviceDisplay)) ||
         !LoadString(hservRegModule, IDS_SERVICEDEPS, serviceDeps, sizeof(serviceDeps)) ) {
	    MessageBox(0,
		           "Missing service registration information.",
		           "NT Service Installation",
		           MB_OK | MB_ICONEXCLAMATION);
	    return 903;
    }

    result = regService(hservRegModule,
			            serviceId, serviceDisplay, serviceDeps,
			            instMode);
    FreeLibrary(hservRegModule);
    return result;
} /* RegisterService */


/* --------------------------------------------------------------------------
 *
 */
int
UnregisterService(HMODULE hModule, NtServiceInstallationMode instMode)
{
    char                   moduleName[MAX_PATH];
    char                  *pExt;
    HMODULE                hservRegModule;
    UnregisterServiceType  unregService;
    int                    result;
    char                   serviceId[64];

    /*
     * Determine the name of the service registration helper module from
     * this module's name. Just replace the module's filename extension
     * with ".dll".
     */
    if ( GetModuleFileName(hModule, moduleName, sizeof(moduleName) - 1) == 0 ) {
	    MessageBox(0,
		           "Can't determine service registration helper module.",
		           "NT Service Installation",
		           MB_OK | MB_ICONEXCLAMATION);
	    return 900;
    }
    pExt = strrchr(moduleName, '.');
    if ( pExt ) {
	    *pExt = 0;
    }
    strncat(moduleName, SELFREGISTRATIONDLLEXTENSION, sizeof(moduleName));
    /*
     * Now try to load the service registration helper module and get
     * your hands on the UnregisterService() method.
     */
    hservRegModule = LoadLibrary(moduleName);
    if ( !hservRegModule ) {
    	MessageBox(0,
		           "Can't find service registration helper module.",
		           "NT Service Deinstallation",
		           MB_OK | MB_ICONEXCLAMATION);
	    return 901;
    }
    unregService = (UnregisterServiceType)
        GetProcAddress(hservRegModule, "_UnregisterService");
    if ( !unregService ) {
	    unregService = (UnregisterServiceType)
            GetProcAddress(hservRegModule, "UnregisterService");
    }
    if ( !unregService ) {
	    MessageBox(0,
		           "Invalid service registration helper module.",
		           "NT Service Deinstallation",
		           MB_OK | MB_ICONEXCLAMATION);
	    return 902;
    }

    if ( !LoadString(hservRegModule, IDS_SERVICEID, serviceId, sizeof(serviceId)) ) {
	    MessageBox(0,
		           "Missing service registration information.",
		           "NT Service Deinstallation",
		           MB_OK | MB_ICONEXCLAMATION);
	    return 903;
    }

    result = unregService(hservRegModule, serviceId, instMode);
    FreeLibrary(hservRegModule);
    return result;
} /* UnregisterService */


/* End of servregi.c */
