/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/src/server.cpp,v 1.23 2007-04-25 12:57:21 martin Exp $ */
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

/*
 * server.cpp: this module contains the knowledge how to register ACPLT/KS
 *             servers with the ACPLT/KS manager. There isn't really much
 *             more to it, folks.
 */

/*
 * Written by Harald Albrecht & Martin Kneissl
 * <harald@plt.rwth-aachen.de> <martin@plt.rwth-aachen.de>
 */


#include "ks/server.h"
#include "ks/register.h"
#include "ks/svrrpcctx.h"
#include "plt/log.h"
#if !PLT_SYSTEM_NT
#include <sys/socket.h>
#endif


// ---------------------------------------------------------------------------
// The purpose of the following definition is to provide a link-time check
// which makes sure that both the ACPLT/KS server and the libkssvr were
// compiled with either PLT_USE_BUFFERED_STREAMS enabled or disabled, but not
// one with buffering enabled and the other one without.
//
#if PLT_USE_BUFFERED_STREAMS
void *This_libKssvr_Was_Compiled_Without_PLT_USE_BUFFERED_STREAMS;
#else
void *This_libKssvr_Was_Compiled_With_PLT_USE_BUFFERED_STREAMS;
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


// ---------------------------------------------------------------------------
//
KsServer::KsServer(u_long ttl, int port)
: _ttl(ttl),
  _registered(false)
{
    if ( port != KS_ANYPORT ) {
        //
        // Some day I'll be after those who'd invented virtual base
        // classes without defining the order of constructors called
        // for derived classes. But on the other side, why did I ever
        // relied on that?! So shame on me...
        //
        _sock_port = port;
    }
} // KsServer::KsServer


// ---------------------------------------------------------------------------
//
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
    int sock = RPC_ANYSOCK;
    KsTime wait(1,0);

    return clntudp_create(&sin,
                          KS_RPC_PROGRAM_NUMBER,
                          KS_PROTOCOL_VERSION,
                          (struct timeval)wait,
                          &sock);
} // KsServer::createLocalClient

// ---------------------------------------------------------------------------
//
bool
KsServer::register_server()
{
    KsAvNoneTicket ticket;
    KssRegistrationContext regctx(ticket);
    regctx.params.server.name = getServerName();
    regctx.params.server.protocol_version = getProtocolVersion();
#if !PLT_USE_BUFFERED_STREAMS
    regctx.params.port = _tcp_transport->xp_port;
#else
    regctx.params.port = _tcp_transport->getPort();
#endif
    regctx.params.time_to_live = _ttl;
    
    CLIENT * clnt = createLocalClient();
    if (!clnt) return false;

    enum clnt_stat res = regctx.call(clnt, KsTime(2,0));
    clnt_destroy(clnt);
    return res == RPC_SUCCESS && regctx.result.result == KS_ERR_OK;
}


// ---------------------------------------------------------------------------
//
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


// ---------------------------------------------------------------------------
//
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
} // KsServer::startServer


// ---------------------------------------------------------------------------
//
void
KsServer::stopServer() 
{
    // unregister
    if ( !unregister_server() ) {
        PltLog::Warning("KsServer::stopServer(): "
			"could not unregister with manager.");
    }
    KsServerBase::stopServer();
} // KsServer::stopServer


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
