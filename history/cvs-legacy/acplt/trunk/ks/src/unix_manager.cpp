/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/src/unix_manager.cpp,v 1.8 1997-11-27 18:18:30 harald Exp $ */
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

#include "ks/manager.h"
#include "plt/log.h"
#include <signal.h>
#include <unistd.h>
#include <errno.h>

//////////////////////////////////////////////////////////////////////

const char PROG_NAME[] = "manager";
const KsString KS_MANAGER_VERSION("1.01");

//////////////////////////////////////////////////////////////////////

extern "C" void handler(int) 
{
    KsServerBase::getServerObject().downServer();
}

//////////////////////////////////////////////////////////////////////

class KsUnixManager
: public KsManager
{	
public:
	KsUnixManager(int port);
    virtual KsString getServerVersion() const;
};

//////////////////////////////////////////////////////////////////////

KsUnixManager::KsUnixManager(int port)
: KsServerBase(port), KsManager(port)
{
    if (_is_ok && initVendorTree()) {
        signal(SIGINT, handler);
        signal(SIGHUP, handler);
        signal(SIGTERM, handler);
        signal(SIGPIPE, SIG_IGN);
    }
}

//////////////////////////////////////////////////////////////////////

KsString
KsUnixManager::getServerVersion() const
{
    return KS_MANAGER_VERSION;
}

//////////////////////////////////////////////////////////////////////

int main(int argc, char **argv) {
    bool daemon = false;
    bool argsok = true;
    int  port   = KsServerBase::KS_ANYPORT;
    int  idx    = 0;
    PltLog * pLog = 0;

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
        } else if ( (strcmp(argv[idx], "-d") == 0) ||
                    (strcmp(argv[idx], "--detach") == 0) ) {
            daemon = true;
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
             << "Runs the ACPLT/KS Manager process for un*x operating systems" << endl
             << endl
             << "  -d, --detach    sends ACPLT/KS manager process into background" << endl
             << "  -p #, --port #  binds the ACPLT/KS manager to port number #" << endl
             << "  --help          display this help and exit" << endl
             << "  --version       output version information and exit" << endl;
        return EXIT_FAILURE;
    }

    if ( daemon ) {
        //
        // Daemon mode is requested, detach and report to syslog
        //
        switch (fork()) {
        case -1:
            // error
            cerr << "fork failed: " << strerror(errno) << endl;
            return EXIT_FAILURE;
            break;
        case 0:
            // child
            for (int i = 0; i<FD_SETSIZE; ++i) close(i);
            setpgrp();
            pLog = new PltSyslog(PROG_NAME);
            break;
        default:
            // parent
            return EXIT_SUCCESS;
        }
    } else {
        //
        // Attached mode, report to cerr.
        //
        pLog = new PltCerrLog(PROG_NAME);
        if (!pLog) {
            cerr << "Could not create log." << endl;
        }
    }

    //
    // Ok, let's go!
    //
	KsUnixManager m(port);
    if (m.isOk()) {
        m.startServer();
        PltLog::Info("started.");
        m.run();
        PltLog::Info("exiting...");
	m.stopServer();
	PltLog::Info("exited.");
    } else {
        PltLog::Error("could not initialize.");
    }
    if (pLog) delete pLog;
    return EXIT_SUCCESS;
}

//////////////////////////////////////////////////////////////////////
// Template instantiation: What a mess!
//////////////////////////////////////////////////////////////////////

#if PLT_COMPILER_GCC
#include "plt/priorityqueue_impl.h"
#include "plt/hashtable_impl.h"
#include "ks/array_impl.h"
#include "ks/handle_impl.h"
#include "ks/list_impl.h"
#endif

#if PLT_COMPILER_WATCOM
#include "plt/priorityqueue_impl.h"
#include "plt/hashtable_impl.h"
#include "ks/array_impl.h"
#include "ks/array_builtins.h"
#include "ks/handle_impl.h"
#include "ks/list_impl.h"
#endif

#if PLT_COMPILER_BORLAND
#include "plt/priorityqueue.h"
#include "plt/hashtable.h"
#include "ks/array.h"
#include "ks/array_builtins.h"
#include "ks/handle.h"
#include "ks/list.h"
#endif

#include "plt/comparable.h"

#if PLT_INSTANTIATE_TEMPLATES
template class PltAssoc<KsAuthType, KsAvTicket *(*)(XDR *)>;
template class PltAssoc<KsString, PltPtrHandle<KssCommObject> >;
template class PltAssoc<PltKeyPtr<KsServerDesc>, KsmServer *>;
template class PltContainer<PltAssoc<KsString, PltPtrHandle<KssCommObject> > >;
template class PltContainer<PltAssoc<PltKeyPtr<KsServerDesc>, KsmServer *> >;
template class PltContainer<PltPtrComparable<KsTimerEvent> >;
template class PltContainer_<KssCommObject>;
template class PltContainer<PltAssoc<KsAuthType, KsAvTicket *(*)(XDR *)> >;
template class PltContainer_<PltAssoc<KsString, PltPtrHandle<KssCommObject> > >;
template class PltContainer_<PltAssoc<PltKeyPtr<KsServerDesc>, KsmServer *> >;
template class PltContainer_<PltPtrComparable<KsTimerEvent> >;
template class PltContainer_<PltAssoc<KsAuthType, KsAvTicket *(*)(XDR *)> >;
template class PltDictionary<KsAuthType, KsAvTicket *(*)(XDR *)>;
template class PltDictionary<KsString, PltPtrHandle<KssCommObject> >;
template class PltDictionary<PltKeyPtr<KsServerDesc>, KsmServer *>;
template class PltHandle<KssCommObject>;
template class PltHandle<KssDomain>;
template class PltHandle<PltHandleIterator<KssCommObject> >;
template class PltHandleContainer<KssCommObject>;
template class PltHandleIterator<KssCommObject>;
template class PltHashIterator<KsString, PltPtrHandle<KssCommObject> >;
template class PltHashIterator<PltKeyPtr<KsServerDesc>, KsmServer *>;
template class PltHashTable<KsString, PltPtrHandle<KssCommObject> >;
template class PltHashTable<PltKeyPtr<KsServerDesc>, KsmServer *>;
template class PltHashTable_<KsString, PltPtrHandle<KssCommObject> >;
template class PltHashTable_<PltKeyPtr<KsServerDesc>, KsmServer *>;
template class PltIterator<PltAssoc<KsString, PltPtrHandle<KssCommObject> > >;
template class PltIterator<PltAssoc<PltKeyPtr<KsServerDesc>, KsmServer *> >;
template class PltIterator<PltPtrComparable<KsTimerEvent> >;
template class PltIterator_<KssCommObject>;
template class PltIterator_<PltAssoc<KsString, PltPtrHandle<KssCommObject> > >;
template class PltIterator_<PltAssoc<PltKeyPtr<KsServerDesc>, KsmServer *> >;
template class PltIterator_<PltPtrComparable<KsTimerEvent> >;
template class PltHashIterator<KsAuthType, KsAvTicket *(*)(XDR *)>;
template class PltHashTable<KsAuthType, KsAvTicket *(*)(XDR *)>;
template class PltIterator_<PltAssoc<KsAuthType, KsAvTicket *(*)(XDR *)> >;
template class PltIterator<PltAssoc<KsAuthType, KsAvTicket *(*)(XDR *)> >;
template class PltHashTable_<KsAuthType, KsAvTicket *(*)(XDR *)>;
template class PltKeyPtr<KsServerDesc>;
template class PltPQIterator<PltPtrComparable<KsTimerEvent> >;
template class PltPriorityQueue<PltPtrComparable<KsTimerEvent> >;
template class PltPtrComparable<KsTimerEvent>;
template class PltPtrComparable<KsmExpireServerEvent>;
template class PltPtrHandle<KssCommObject>;
template class PltPtrHandle<KssDomain>;
template class PltPtrHandle<PltHandleIterator<KssCommObject> >;
template class Plt_AtArrayNew<KssCommObject>;
template class Plt_AtArrayNew<KssDomain>;
template class Plt_AtArrayNew<PltHandleIterator<KssCommObject> >;
template class Plt_AtNew<KssCommObject>;
template class Plt_AtNew<KssDomain>;
template class Plt_AtNew<PltHandleIterator<KssCommObject> >;
#endif

// EOF unix_manager.cpp
