/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/src/server.cpp,v 1.11 1997-12-02 18:08:50 harald Exp $ */
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

#include "ks/server.h"
#include "ks/register.h"
#include "ks/svrrpcctx.h"
#include "plt/log.h"
#if !PLT_SYSTEM_NT
#include <sys/socket.h>
#endif

//////////////////////////////////////////////////////////////////////

typedef KssRPCContext<KS_REGISTER, KsRegistrationParams, KsRegistrationResult>
    KssRegistrationContext;

typedef KssRPCContext<KS_UNREGISTER, KsUnregistrationParams, KsUnregistrationResult>
    KssUnregistrationContext;

//////////////////////////////////////////////////////////////////////

#if PLT_INSTANTIATE_TEMPLATES
template class KssRPCContext<KS_REGISTER, 
                             KsRegistrationParams, 
                             KsRegistrationResult>;
template class KssRPCContext<KS_UNREGISTER, 
                             KsUnregistrationParams, 
                             KsUnregistrationResult>;
#endif

//////////////////////////////////////////////////////////////////////

KsServer::KsServer(u_long ttl, int port)
: _ttl(ttl),
  _registered(false)
{
    _sock_port = port;
}

//////////////////////////////////////////////////////////////////////

KsServer::~KsServer()
{
    if ( _registered ) {
        unregister_server();
    }
}

//////////////////////////////////////////////////////////////////////

CLIENT *
KsServer::createLocalClient()
{
    struct sockaddr_in sin;
    memset(&sin, 0, sizeof sin);
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    KsTime wait(1,0);
    int sock = RPC_ANYSOCK;
    return clntudp_create(&sin,
                          KS_RPC_PROGRAM_NUMBER,
                          KS_PROTOCOL_VERSION,
                          (struct timeval)wait,
                          &sock);
}

//////////////////////////////////////////////////////////////////////


bool
KsServer::register_server()
{
    KsAvNoneTicket ticket;
    KssRegistrationContext regctx(ticket);
    regctx.params.server.name = getServerName();
    regctx.params.server.protocol_version = getProtocolVersion();
    regctx.params.port = _tcp_transport->xp_port;
    regctx.params.time_to_live = _ttl;
    
    CLIENT * clnt = createLocalClient();
    if (!clnt) return false;

    enum clnt_stat res = regctx.call(clnt, KsTime(2,0));
    clnt_destroy(clnt);
    return res == RPC_SUCCESS && regctx.result.result == KS_ERR_OK;
}

//////////////////////////////////////////////////////////////////////

bool
KsServer::unregister_server()
{
    KsAvNoneTicket ticket;
    KssUnregistrationContext unregctx(ticket);
    unregctx.params.server.name = getServerName();
    unregctx.params.server.protocol_version = getProtocolVersion();

    CLIENT * clnt = createLocalClient();
    if (!clnt) return false;

    enum clnt_stat res = unregctx.call(clnt, KsTime(2,0));
    clnt_destroy(clnt);
    return res == RPC_SUCCESS && unregctx.result.result == KS_ERR_OK;
}

//////////////////////////////////////////////////////////////////////

void
KsServer::startServer()
{
    KsServerBase::startServer();

    if ( _is_ok ) {
      // schedule registration
      KsReregisterServerEvent * pevent = 
          new KsReregisterServerEvent(*this, KsTime::now());
      if ( pevent ) {
          addTimerEvent(pevent);
      } else {
          _is_ok = false;
      }
    }
}

void
KsServer::stopServer() 
{
    // unregister
    if ( !unregister_server() ) {
        PltLog::Warning("KsServer::stopServer: could not unregister with manager.");
    }
    KsServerBase::stopServer();
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

const u_long
KsReregisterServerEvent::MIN_TTL = 10; // secs

//////////////////////////////////////////////////////////////////////

void
KsReregisterServerEvent::trigger()
{
    if (!_server.isGoingDown()) {
        //
        // The server is still active, try to register it.
        //
        if (_server.register_server()) {
            // 
            // It worked. Reschedule event.
            //
            PLT_DMSG_ADD("Registered with manager.");
            PLT_DMSG_END;
            u_long secs = _server._ttl / 4;
            secs *= 3;
            if (secs < MIN_TTL) secs = MIN_TTL;
            _trigger_at = KsTime::now(secs);
            _server.addTimerEvent(this);
        } else {
            //
            // Registration failed. Retry soon.
            //
            PltLog::Warning("Could not register with manager."
                            " Is an ACPLT/KS manager running?");
            _trigger_at = KsTime::now(MIN_TTL);
            _server.addTimerEvent(this);
        }
    } else {
        //
        // The server is going down. Ignore and delete the event
        //
        delete this;
    }
}

//////////////////////////////////////////////////////////////////////
// EOF ks/server.cpp
