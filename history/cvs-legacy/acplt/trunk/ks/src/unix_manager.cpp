/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/src/unix_manager.cpp,v 1.1 1997-04-10 14:18:29 martin Exp $ */
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

const char PROG_NAME[] = "manager";


//////////////////////////////////////////////////////////////////////

extern "C" void handler(int) 
{
    KsServerBase::getServerObject().stopServer();
}

//////////////////////////////////////////////////////////////////////

class KsUnixManager 
: public KsManager
{	
public:
	KsUnixManager();
};

//////////////////////////////////////////////////////////////////////

KsUnixManager::KsUnixManager()
{
    if (_is_ok) {
        signal(SIGINT, handler);
        signal(SIGHUP, handler);
        signal(SIGTERM, handler);
    }
}

//////////////////////////////////////////////////////////////////////

int main(int argc, char **argv) {
    bool daemon = false;
    PltLog * pLog = 0;

    //
    // parse command line
    //
    if (argc == 2) {
        if (argv[1][0]== '-' && argv[1][1] = 'd') {
            daemon = true;
        } else {
            cerr << "Usage: " << PROG_NAME << "[-d]" << endl;
            return EXIT_FAILURE;
        }
    }

    if (daemon) {
        //
        // If daemon mode is requested, detach and report to syslog
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
	KsUnixManager m;
    if (m.isOk()) {
        m.startServer();
        PltLog::Info("started.");
        m.run();
        PltLog::Info("exiting.");
    } else {
        PltLog::Error("could not initialize.");
    }
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
template class PltAssoc<KsString, PltPtrHandle<KssCommObject> >;
template class PltAssoc<PltKeyPtr<KsServerDesc>, KsmServer *>;
template class PltContainer<PltAssoc<KsString, PltPtrHandle<KssCommObject> > >;
template class PltContainer<PltAssoc<PltKeyPtr<KsServerDesc>, KsmServer *> >;
template class PltContainer<PltPtrComparable<KsTimerEvent> >;
template class PltContainer_<KssCommObject>;
template class PltContainer_<PltAssoc<KsString, PltPtrHandle<KssCommObject> > >;
template class PltContainer_<PltAssoc<PltKeyPtr<KsServerDesc>, KsmServer *> >;
template class PltContainer_<PltPtrComparable<KsTimerEvent> >;
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




