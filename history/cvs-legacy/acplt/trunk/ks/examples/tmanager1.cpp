/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/examples/tmanager1.cpp,v 1.7 1997-12-02 18:08:47 harald Exp $ */
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

const char PROG_NAME[] = "tmanager";
const KsString KS_MANAGER_VERSION("1.01");

//////////////////////////////////////////////////////////////////////

extern "C" void handler(int sig) {
    PLT_DMSG("caught signal " << sig << endl);
    KsServerBase::getServerObject().downServer();
}

//////////////////////////////////////////////////////////////////////

class Manager 
: public KsManager
{	
public:
	Manager(int port = KS_ANYPORT);
    virtual KsString getServerVersion() const;
};

//////////////////////////////////////////////////////////////////////

Manager::Manager(int port)
  : KsManager(port)
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

int main(int argc, char **argv) {
    PltCerrLog log(PROG_NAME);

    bool argsok = true;
    int  port   = KsServerBase::KS_ANYPORT;
    int  idx    = 0;

    //
    // parse command line
    //
    while ( ++idx < argc ) {
        if ( strcmp(argv[idx], "--help") == 0 ) {
            argsok = false;
            break;
        } else if ( strcmp(argv[idx], "--version") == 0 ) {
            cerr << PROG_NAME << " version " << (const char *) KS_MANAGER_VERSION << endl;
            return EXIT_FAILURE;
        } else if ( (strcmp(argv[idx], "-p") == 0) ||
                    (strcmp(argv[idx], "--port") == 0) ) {
            if ( ++idx < argc ) {
                char *endptr;
                port = strtol(argv[idx], &endptr, 10);
                if ( (argv[idx][0] == '\0') || *endptr || (port <= 0) ) {
                    argsok = false;
                    break;
                }
            } else {
                argsok = false;
                break;
            }
        } else {
            argsok = false;
            break;
        }
    }

    if (!argsok) {
        cerr << "Usage: " << PROG_NAME << "[options]" << endl
             << "Runs the testing ACPLT/KS Manager process" << endl
             << endl
             << "  -p #, --port #  binds the testing ACPLT/KS manager to port number #" << endl
             << "  --help          display this help and exit" << endl
             << "  --version       output version information and exit" << endl;
        return EXIT_FAILURE;
    }

	Manager m(port);
    if (m.isOk()) {
        m.startServer();
	PLT_DMSG("entering service loop"<<endl);
        m.run();
	PLT_DMSG("left service loop"<<endl);
	m.stopServer();
    } else {
        cerr << "The manager could not be initialized." << endl;
    }
    return 0;
}


//////////////////////////////////////////////////////////////////////
// EOF tmanager.cpp
