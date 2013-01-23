/*
 * servreg.h -- internal interface of the registration mumbo jumbo. Do
 *              not use this interface directly, only through the
 *              servregi.h interface.
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
#ifndef __SERVREG_H
#define __SERVREG_H


/* --------------------------------------------------------------------------
 * We use a global atom to signal that at least one NT service couldn't be
 * registered during the installation process because the service was still
 * running and thus its executable locked, so it couldn't be updated yet. As
 * system then needs to be rebooted for the renaming/updating to take effect,
 * we don't need to start new services.
 */
#define PENDINGREBOOTATOMNAME "OhNoThereIsYetAnotherNTRebootDue"


/* --------------------------------------------------------------------------
 * There are three modes of installation possible:
 * - NtServiceAutomaticInstallation: automatic installation by an
 *   installation program. No manual intervention possible.
 * - NtServiceRegistration: manual registration from the command line. Manual
 *   intervention possible.
 * - NtServiceForcedRegistration: like NtServiceRegistration, but without
 *   manual intervention.
 */
typedef enum {
    NtServiceAutomaticInstallation,
    NtServiceRegistration,
    NtServiceForcedRegistration
} NtServiceInstallationMode;


/* --------------------------------------------------------------------------
 * We assume that the service executable always end with the ".exe" file
 * extension. We need this information so we can deduce the service exe-
 * cutable's name from the DLL's module file name.
 */
#define SERVICEFILENAMEEXTENSION ".exe"


/* --------------------------------------------------------------------------
 * The registration magic can be triggered by these two functions...
 */
extern int
RegisterService(HMODULE hModule,
                char *serviceId, char *serviceDisplay, char *dependencies,
                NtServiceInstallationMode instMode);
extern int
UnregisterService(HMODULE hModule,
                  char *serviceId,
		          NtServiceInstallationMode instMode);


#endif
/* End of servregids.h */

