/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/examples/tmanager1.cpp,v 1.1 1997-04-14 16:52:04 harald Exp $ */
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

/* Author: Martin Kneissl <martin@plt.rwth-aachen.de> */

//////////////////////////////////////////////////////////////////////

#include "ks/manager.h"
#include "plt/log.h"
#include <signal.h>

//////////////////////////////////////////////////////////////////////

const KsString KS_MANAGER_VERSION("0.5.1");

//////////////////////////////////////////////////////////////////////

extern "C" void handler(int) {
    KsServerBase::getServerObject().stopServer();
}

//////////////////////////////////////////////////////////////////////

class Manager 
: public KsManager
{	
public:
	Manager();
    virtual KsString getServerVersion() const;
};

//////////////////////////////////////////////////////////////////////

Manager::Manager()
{
    if (_is_ok && initVendorTree()) {
        signal(SIGINT, handler);
        signal(SIGTERM, handler);
#if !PLT_SYSTEM_NT
        signal(SIGHUP, handler);
        signal(SIGPIPE, SIG_IGN);
#endif
    }
}

//////////////////////////////////////////////////////////////////////

KsString
Manager::getServerVersion() const
{
    return KS_MANAGER_VERSION;
}

//////////////////////////////////////////////////////////////////////

int main(int, char **) {
    PltCerrLog log("manager");
	Manager m;
    if (m.isOk()) {
        m.startServer();
        m.run();
    } else {
        cout << "The manager could not be initialized." << endl;
    }
    return 0;
}


//////////////////////////////////////////////////////////////////////
// EOF tmanager.cpp
