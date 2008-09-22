/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/examples/tmanager1.cpp,v 1.16 2008-09-22 08:26:09 henning Exp $ */
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

//////////////////////////////////////////////////////////////////////

#include "ks/manager.h"
#include "plt/log.h"
#include <signal.h>

//////////////////////////////////////////////////////////////////////

const char PROG_NAME[] = "tmanager";
const KsString KS_MANAGER_VERSION(KS_VERSION_STRING);

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
    bool reuseaddr = false;
    int  idx    = 0;

#if PLT_USE_DEPRECIATED_HEADER
    #define OUT_STREAM  cerr
    #define NEW_LINE    endl
#else
    #define OUT_STREAM  STDNS::cerr
    #define NEW_LINE    STDNS::endl
#endif

    //
    // parse command line
    //
    while ( ++idx < argc ) {
        if ( strcmp(argv[idx], "--help") == 0 ) {
            argsok = false;
            break;
        } else if ( strcmp(argv[idx], "--version") == 0 ) {
            OUT_STREAM << PROG_NAME << " version " << (const char *) KS_MANAGER_VERSION << NEW_LINE;
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
        } else if ( (strcmp(argv[idx], "-r") == 0) ||
                    (strcmp(argv[idx], "--reuseaddr") == 0) ) {
                reuseaddr = true;
        } else {
            argsok = false;
            break;
        }
    }

    if (!argsok) {
        OUT_STREAM  << "Usage: " << PROG_NAME << "[options]" << NEW_LINE
                    << "Runs the testing ACPLT/KS Manager process" << NEW_LINE
                    << NEW_LINE
                    << "  -p #, --port #  binds the testing ACPLT/KS manager to port number #" << NEW_LINE
                    << "  --help          display this help and exit" << NEW_LINE
                    << "  -r, --reuseaddr reuse socket address" << NEW_LINE
                    << "  --version       output version information and exit" << NEW_LINE;
        return EXIT_FAILURE;
    }

	Manager m(port);
    if ( m.isOk() ) {
        m.setReuseAddr(reuseaddr);
        m.startServer();
        if ( m.isOk() ) {
            PLT_DMSG("entering service loop"<<NEW_LINE);
            m.run();
            PLT_DMSG("left service loop"<<NEW_LINE);
        } else {
            OUT_STREAM << "The manager could not be initialized." << NEW_LINE;
        }
        m.stopServer();
    } else {
        OUT_STREAM << "The manager could not be initialized." << NEW_LINE;
    }
    return 0;
}


//////////////////////////////////////////////////////////////////////
// EOF tmanager.cpp
