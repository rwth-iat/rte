/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/src/unix_manager.cpp,v 1.19 2007-04-25 12:57:21 martin Exp $ */
/*
 * Copyright (c) 1996, 1997, 1998, 1999
 * Lehrstuhl fuer Prozessleittechnik, RWTH Aachen
 * D-52064 Aachen, Germany.
 * All rights reserved.
 *
 * This file is part of the ACPLT/KS Package which is licensed as open
 * source under the Artistic License; you can use, redistribute and/or
 * modify it under the terms of that license.
 *
 * You should have received a copy of the Artistic License along with
 * this Package; see the file ARTISTIC-LICENSE. If not, write to the
 * Copyright Holder.
 *
 * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES
 * OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

/* Author: Martin Kneissl <martin@plt.rwth-aachen.de> */

#include "ks/manager.h"
#include "plt/log.h"
#include <signal.h>
#include <unistd.h>
#include <errno.h>

//////////////////////////////////////////////////////////////////////

const char PROG_NAME[] = "manager";
const KsString KS_MANAGER_VERSION(KS_VERSION_STRING);

//////////////////////////////////////////////////////////////////////////////
// First, we'll now entering a very dangerous terrain of unix land: the
// signals. Depending on the os vendor, we might be either running on top of
// BSD or SYS V R4. Whereas the first one has made the signal() mechanism
// reliable some time back in the good old days without changing the API,
// SYS V R4 has still the old unreliable signal() interface. Bottom line: to
// get reliable signals, we must use sigaction() (fortunately, all the
// contemporary systems support this -- and those which don't are (by
// conclusion) *not* contemporary.
//
typedef void (*reliableSignalHandler)(...);

static bool reliableSignal(int signo, reliableSignalHandler handler)
{
    struct sigaction act;

    act.sa_handler = handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
#ifdef SA_INTERRUPT
    act.sa_flags |= SA_INTERRUPT;
#endif
    return (sigaction(signo, &act, 0) >= 0) ? true : false;
} // reliableSignal


//////////////////////////////////////////////////////////////////////////////
// If we catch a signal, we will tell the server object to graceously shut
// down all services. Basically, the downServer() method just signals to end
// the services as soon as possible. Eventually, after returning from the
// signal handler, the kernel will terminate the current kernel call with an
// EINTR error code. This then allows us very soon to check for the shutdown
// flag.
//
extern "C" void shutDownSignalHandler() {
    KsServerBase::getServerObject().downServer();
} // shutDownSignalHandler


//////////////////////////////////////////////////////////////////////////////
// And now for that signal I like most: the SIGPIPE. We must catch it to
// prevent bad clients from killing us. Although we don't do anything in this
// signal handler, this will result in the affected communication getting
// aborted. And that is, what we want.
//
extern "C" void brokenPipeSignalHandler() {
    // Just do *nothing*
} // brokenPipeSignalHandler


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
: KsManager(port)
{
    if ( _is_ok && initVendorTree() ) {
        //
        // We're catching some typical signals here which usually tell a process
        // to "hit the road". In our case we'll eventually shut down the server
        // graceously before exiting.
        //
        if ( !reliableSignal(SIGINT, shutDownSignalHandler) ) {
            PltLog::Error("KsUnixManager::KsUnixManager(): can't install SIGINT handler. Continuing...");
        }
        if ( !reliableSignal(SIGHUP, shutDownSignalHandler) ) {
            PltLog::Error("KsUnixManager::KsUnixManager(): can't install SIGHUP handler. Continuing...");
        }
        if ( !reliableSignal(SIGTERM, shutDownSignalHandler) ) {
            PltLog::Error("KsUnixManager::KsUnixManager(): can't install SIGTERM handler. Continuing...");
        }
        //
        // Also catch that infamous SIGPIPE.
        //
        if ( !reliableSignal(SIGPIPE, brokenPipeSignalHandler) ) {
            PltLog::Error("KsUnixManager::KsUnixManager(): can't install SIGPIPE handler. "
                          "Expect trouble with broken clients. Continuing...");
        }
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
    bool daemon    = false;
    bool argsok    = true;
    int  port      = KsServerBase::KS_ANYPORT;
    int  reuseaddr = false;
    int  idx       = 0;
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
        } else if ( (strcmp(argv[idx], "-r") == 0) ||
                    (strcmp(argv[idx], "--reuseaddr") == 0) ) {
            reuseaddr = true;
        } else {
            argsok = false;
            break;
        }
    }

    if (!argsok) {
        cerr << "Usage: " << PROG_NAME << "[options]" << endl
             << "Runs the ACPLT/KS Manager process for un*x operating systems" << endl
             << endl
             << "  -d, --detach     sends ACPLT/KS manager process into background" << endl
             << "  -p #, --port #   binds the ACPLT/KS manager to port number #" << endl
             << "  -r, --reuseaddr  reuse socket address" << endl
             << "  --help           display this help and exit" << endl
             << "  --version        output version information and exit" << endl;
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
//cerr << "fork passed (in child)" << endl;
            for (int i = 0; i<FD_SETSIZE; ++i) {
                close(i);
            }
//cerr << "about to setpgrp()" << endl;
#if PLT_SYSTEM_FREEBSD
	    setpgid(0, 0);
#else
            setpgrp();
#endif
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
        m.setReuseAddr(reuseaddr);
        m.startServer();
        if ( m.isOk() ) {
            PltLog::Info("KsUnixManager started.");
            m.run();
            PltLog::Info("KsUnixManager exiting...");
        }
        m.stopServer();
        PltLog::Info("exited.");
    } else {
        PltLog::Error("KsUnixManager could not get initialized.");
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
