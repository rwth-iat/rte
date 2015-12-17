/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/examples/tsclient1.cpp,v 1.6 2007-04-25 12:57:20 martin Exp $ */
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


/*


   How !_NOT_! to program an ACPLT/KS client...



 */

#include "ks/ks.h"
#include "ks/register.h"
#include "ks/serviceparams.h"
#include "ks/avticket.h"
#include "ks/svrrpcctx.h"
#include "plt/log.h"
#include <string.h>
#if PLT_SYSTEM_NT
#include <winsock.h>
#else
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#endif

//////////////////////////////////////////////////////////////////////

#if PLT_INSTANTIATE_TEMPLATES
template class KssRPCContext<KS_GETVAR,
                             KsGetVarParams,
                             KsGetVarResult>;
template class KssRPCContext<KS_GETSERVER,
                             KsGetServerParams,
                             KsGetServerResult>;
#endif

//////////////////////////////////////////////////////////////////////

typedef KssRPCContext<KS_GETVAR,
                      KsGetVarParams,
                      KsGetVarResult> TestGetVarContext;

typedef KssRPCContext<KS_GETSERVER,
                      KsGetServerParams,
                      KsGetServerResult> TestGetServerContext;

//////////////////////////////////////////////////////////////////////
static KsAvNoneTicket ticket(KS_ERR_OK, KS_AC_READ|KS_AC_WRITE);

CLIENT *
createClient(const char * host, 
             const char * server_name, 
             u_long protocol_version = 1)
{
    // 
    // Ask manager for port
    //
    CLIENT * manager = clnt_create(host,
                                   KS_RPC_PROGRAM_NUMBER,
                                   KS_PROTOCOL_VERSION,
                                   "udp");
    if (!manager) return 0;

    TestGetServerContext getserver_ctx(ticket);
    getserver_ctx.params.server.name = server_name;
    getserver_ctx.params.server.protocol_version = protocol_version;
    getserver_ctx.call(manager, KsTime(10,0));
    clnt_destroy(manager);
    if (!getserver_ctx.result.result == KS_ERR_OK) return 0;

    //
    // set up socket address
    //
    struct sockaddr_in sin;
    struct hostent * phe = gethostbyname(host);
    if (! phe) return 0;

    memset(&sin, 0, sizeof sin);
    sin.sin_family = AF_INET;
    sin.sin_port = htons(getserver_ctx.result.port);
    sin.sin_addr.s_addr =
           ((struct in_addr *)(phe->h_addr))->s_addr;

    int sock = RPC_ANYSOCK;

    //    
    // create client handle
    //
    return clnttcp_create(&sin,
                          KS_RPC_PROGRAM_NUMBER,
                          protocol_version,
                          &sock,
                          0, 0 );
}        

int main (int argc, char **argv)
{
    int num = 1; // num of copies of each variable
    int calls = 1; // num of repetitions of call
    const char * host = "localhost";
    const char * server = "MANAGER";
    bool print = false;
    bool error = false;
    PltList<KsString> varlist;
    for (int s = 1; s < argc && ! error; ++s) {
        char *arg = argv[s];
        if (arg[0]=='-') {
            switch (arg[1]) {
            case 'p':
#if PLT_DEBUG
                print = true;
#else
                cerr << "Client ohne Debug-Informationen kann nicht drucken."
                    << endl;
#endif
                break;
            default:
                if (s+1 < argc) {
                    char * opt = argv[++s];
                    switch (arg[1]) {
                    case 'n':
                        num = atoi(opt);
                        break;
                    case 'c':
                        calls = atoi(opt);
                        break;
                    case 'h':
                        host = opt;
                        break;
                    case 's':
                        server = opt;
                        break;
                    default:
                        error = true;
                    }
                } else {
                    error = true;
                }
            }
        } else {
            varlist.addLast(arg);
        }
    }
    if (error|| argc < 2) {
        cerr << argv[0] << " [-h <host>] [-s <server>] [-c calls] [-n times] [-p] var ..." << endl;
        return 1;
    } else {
        KsTime timeout(30,0);
        //
        // Construct parameters
        //
        TestGetVarContext getvar_ctx(ticket);
        size_t sz = varlist.size()*num;
        KsArray<KsString> &ids = getvar_ctx.params.identifiers;
        ids = KsArray<KsString>(sz);
        for (size_t i = 0; i < sz; i+=num) {
            KsString str = varlist.removeFirst();
            for (int ii = 0; ii<num; ++ii) {
                ids[i+ii] = str;
            }
        }
        CLIENT *clnt = createClient(host, server);
        if (!clnt) {
            cout << "Can't connect to server" << endl;
            return 2;
        }
        KsTime started(KsTime::now());
        for (int k=0; k<calls; ++k) {
            //
            // execute request
            //
            getvar_ctx.call(clnt, timeout);
            
            if (getvar_ctx.result.result != KS_ERR_OK) {
                cout << "Request failed: " << getvar_ctx.result.result << endl;
                return 2;
            }
            for (size_t j = 0; j < sz; ++j) {
                const KsGetVarItemResult & item = getvar_ctx.result.items[j];
                if (item.result == KS_ERR_OK) {
                    if (print) {
                        KsVarCurrProps *pcp = 
                            PLT_DYNAMIC_PCAST(KsVarCurrProps,
                                              item.item.getPtr());
#if PLT_DEBUG
                        pcp->value->debugPrint(cout);
#endif
                        cout << endl;
                    }
                } else {
                    cout << ids[j]<< ": result= " << item.result  << endl;
                }
            }
        }
        KsTime stopped(KsTime::now());
        KsTime elapsed(stopped - started);
        cout << "Elapsed: " << elapsed.tv_sec << "," 
            << elapsed.tv_usec << endl;
        return 0;
    }
}
// EOF tsclient1.cpp
