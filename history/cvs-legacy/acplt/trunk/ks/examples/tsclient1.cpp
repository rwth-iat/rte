/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/examples/tsclient1.cpp,v 1.1 1997-04-14 16:52:06 harald Exp $ */
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
                const KsGetVarItemResult & item(getvar_ctx.result.items[j]);
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
// EOF tserver1.cpp
