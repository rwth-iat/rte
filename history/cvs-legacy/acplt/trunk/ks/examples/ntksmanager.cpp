/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/examples/ntksmanager.cpp,v 1.4 1997-12-02 18:41:55 harald Exp $ */
/*
 * Copyright (c) 1996, 1997
 * Chair of Process Control Engineering,
 * Aachen University of Technology.
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

/* Author: Harald Albrecht <harald@plt.rwth-aachen.de> */

// -------------------------------------------------------------------------
// Below you'll find a full-blown KS manager, which runs as a native NT
// service (or was that "naive"?!). Twist the registry, then reboot, and off
// you go with your new KS manager service. From the code it should be quite
// obvious how to turn an innocent console application (aka KS server) into
// a NT service...
//
// NOTE: remember to link against the multi-threaded run-time libraries!
//
// REGISTRY:
// add the following entries:
//   HKEY_LOGCAL_MACHINE\SYSTEM\CurrentControlSet\Services
//     KS_manager
//       DisplayName     (SZ)        "ACPLT/KS Manager"
//       ImagePath       (EXPAND_SZ) "%SystemRoot%\system32\ntksmanager.exe"
//       ObjectName      (SZ)        "LocalSystem"
//       ErrorControl    (DWORD)     0x00000001
//       Start           (DWORD)     0x00000002 (or 0x00000003 for "by hand")
//       Type            (DWORD)     0x00000010
//       DependOnService (MULTI_SZ)  "tcpip" "portmap"
//
//
// BTW: The ONC/RPC portmap then must have been entered into the REGISTRY too:
//   HKEY_LOGCAL_MACHINE\SYSTEM\CurrentControlSet\Services
//     Portmap
//       DisplayName     (SZ)        "ONC/RPC Portmap"
//       ImagePath       (EXPAND_SZ) "%SystemRoot%\system32\portmap.exe"
//       ObjectName      (SZ)        "LocalSystem"
//       ErrorControl    (DWORD)     0x00000001
//       Start           (DWORD)     0x00000002 (or 0x00000003 for "by hand")
//       Type            (DWORD)     0x00000010
//       DependOnService (MULTI_SZ)  "tcpip"
//

#ifdef PLT_W95SERVICE
#include "ks/w95service.h"
#else
#include "ks/ntservice.h"
#endif
#include "ks/manager.h"
#include "plt/log.h"

#include <string.h>

// -------------------------------------------------------------------------
// We're using here a global logger object, which gets initialized right
// after the executable was loaded and is shut down just before the exe-
// cutable fades away. This will make sure that there is a logger ready
// for us as soon as we (really) start.
//
#ifdef PLT_W95SERVICE
	// No default logger object for Windooze 95
#else
PltNtLog log("ACPLT/KS manager");
#endif


// -------------------------------------------------------------------------
// We simply store the version id string in a static KsString, and return
// a copy of it whenever someone asks us for it.
//
#ifdef PLT_W95SERVICE
static const KsString NtKsManagerVersionString("1.01(w95)");
#else
static const KsString NtKsManagerVersionString("1.01(nt)");
#endif

class NtManager : public KsManager {
public:
    NtManager(int port = KS_ANYPORT);
    virtual KsString getServerVersion() const
        { return NtKsManagerVersionString; }
}; // NtManager


NtManager::NtManager(int port)
  : KsManager(port)
{
    if ( _is_ok &&
         initVendorTree() ) {
        //
        // Okay. Now we've got a basic "/vendor" branch. Now we're ready to
        // do dirty things...
    	//
    }
} // NtManager::NtManager


// -------------------------------------------------------------------------
// Okay. Now you're almost through. So far, you now have made up with the
// KS manager (or maybe KS server) object class. Now you're ready to create
// your service object class which just extends the basic service class by
// the knowlodge how to instantiate a manager/server object.
//
class NtManagerService : 
#ifdef PLT_W95SERVICE
	public KsW95ServiceServer
#else
	public KsNtServiceServer
#endif
{
public:
#ifdef PLT_W95SERVICE
    NtManagerService(int argc, char **argv)
	: KsW95ServiceServer("KS_manager", argc, argv)
#else
    NtManagerService()
        : KsNtServiceServer("KS_manager", 7000, 7000, 7000, 7000)
#endif
		{ }

protected:
    virtual KsServerBase *createServer(int argc, char **argv);
}; // class NtManagerService

// -------------------------------------------------------------------------
// In addition to instanciating the server/manager object, you may want to
// parse the command line sent in from the service manager.
//
KsServerBase *NtManagerService::createServer(int argc, char **argv)
{
    int idx;
    int port = KsServerBase::KS_ANYPORT;

    for ( idx = 1; idx < argc; idx++ ) {
        if ( (stricmp(argv[idx], "-v") == 0) ||
             (stricmp(argv[idx], "--verbose") == 0) ) {
             _is_verbose = true;
        } else if ( (stricmp(argv[idx], "-v-") == 0) ||
             (stricmp(argv[idx], "--verbose-") == 0) ) {
             _is_verbose = false;
        } else if ( (stricmp(argv[idx], "-p") == 0) ||
                    (stricmp(argv[idx], "--port") == 0) ) {
            if ( ++idx < argc ) {
                char *endptr;
                port = strtol(argv[idx], &endptr, 10);
                if ( (argv[idx][0] == '\0') || *endptr || (port <= 0) ) {
                    PltLog::Error("invalid port number");
                    break;
                }
            } else {
                PltLog::Error("missing port number");
                break;
            }
        } else {
            PltLog::Error("invalid command line option");
            break;
        }
    }
    //
    // Okay. Now for the real things...
    //
#ifdef PLT_W95SERVICE
    verbose("Creating W95Manager object");
#else
    verbose("Creating NtManager object");
#endif
    return new NtManager(port);
} // NtManagerService::createServer


// -------------------------------------------------------------------------
// Okay. That's it, really! Now let us create the service object and do a
// run() on it. On return from run(), the server object has already been
// destroyed.
//
#ifdef PLT_W95SERVICE
int main(int argc, char **argv) {
    NtManagerService nt_service(argc, argv);
#else
int main(int, char **) {
    NtManagerService nt_service;
#endif

    if ( nt_service.isOk() ) {
        nt_service.run();
    } else {
        PltLog::Error("Failed to setup the KS Manager service object");
    }
    return nt_service.isOk() ? 0 : 42;
} // main

/* End of ntksmanager.cpp */

