/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/src/server.cpp,v 1.1 1997-04-02 14:53:08 martin Exp $ */
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
#include "plt/log.h"
#include <sys/socket.h>

//////////////////////////////////////////////////////////////////////

struct KssRPCContext_base
{
    KssRPCContext_base(const KsAvTicket &t)
        : request_ticket(t), p_response_ticket(0) { }
    virtual ~KssRPCContext_base();
    virtual u_long procedureNumber() const = 0;
    virtual bool encodeParams(XDR *) const = 0;
    virtual bool decodeResult(XDR *) = 0;
    
    enum clnt_stat call(CLIENT *clnt, const KsTime & timeout);

    // public attributes
    const KsAvTicket & request_ticket;
    KsAvTicket * p_response_ticket;
};

//////////////////////////////////////////////////////////////////////

extern "C" bool_t ks_c_xdr_request(XDR *xdr, caddr_t * p)
{
    PLT_PRECONDITION(xdr && xdr->x_op == XDR_ENCODE && p);
    KssRPCContext_base *pcall = (KssRPCContext_base*)p;
    return pcall->request_ticket.xdrEncode(xdr) 
        && pcall->encodeParams(xdr);
}

//////////////////////////////////////////////////////////////////////

extern "C" bool_t ks_c_xdr_response(XDR *xdr, caddr_t *p)
{
    PLT_PRECONDITION(xdr && xdr->x_op == XDR_DECODE && p);
    KssRPCContext_base * pcall = (KssRPCContext_base*)p;
    //
    // The ticket must always be newly created:
    // First, delete the old one (if present)
    //
    if (pcall->p_response_ticket) {
        delete pcall->p_response_ticket;
    }
    //
    // Then create a new ticket from the XDR stream.
    //
    pcall->p_response_ticket = KsAvTicket::xdrNew(xdr);
    if (pcall->p_response_ticket) {
        //
        // We got a ticket, now decode the result. This depends
        // on the specific call, so ask our child class to do it.
        //
        return pcall->decodeResult(xdr);
    } else {
        // We could not decode the ticket. So sorry.
        return FALSE;
    }
}

//////////////////////////////////////////////////////////////////////

KssRPCContext_base::~KssRPCContext_base()
{
    if (p_response_ticket) {
        delete p_response_ticket;
    }
}

//////////////////////////////////////////////////////////////////////

enum clnt_stat
KssRPCContext_base::call(CLIENT *clnt, const KsTime & timeout)
{
    u_long procnr = procedureNumber();
    enum clnt_stat res = clnt_call(clnt,
                                   procnr,
                                   (xdrproc_t)ks_c_xdr_request,
                                   (caddr_t)this,
                                   (xdrproc_t)ks_c_xdr_response,
                                   (caddr_t)this,
                                   timeout);
    return res;
}

//////////////////////////////////////////////////////////////////////

template <u_long NR, class PARAMS, class RESULT>
struct KssRPCContext
: public KssRPCContext_base
{
    PARAMS params;
    RESULT result;
    KssRPCContext(const KsAvTicket & ticket) : KssRPCContext_base(ticket) { }
    u_long procedureNumber() const { return NR; }
    bool encodeParams(XDR *xdr) const { return params.xdrEncode(xdr); }
    bool decodeResult(XDR *xdr) { return result.xdrDecode(xdr); }
};

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

KsServer::KsServer(const char * svr_name, 
                   u_long prot_version,
                   u_long ttl)
: KsServerBase(svr_name, prot_version),
  _ttl(ttl),
  _registered(false)
{
}

//////////////////////////////////////////////////////////////////////

KsServer::~KsServer()
{
    if (_registered) {
        unregister_server();
    }
}

//////////////////////////////////////////////////////////////////////

CLIENT *
KsServer::createLocalClient()
{
#if 0
    struct sockaddr_in sin;
    memset(&sin, 0, sizeof sin);
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    KsTime wait(1,0);
    return clntudp_create(&sin,
                          KS_RPC_PROGRAM_NUMBER,
                          KS_PROTOCOL_VERSION,
                          wait,
                          0);
#else
    return clnt_create("localhost",
                       KS_RPC_PROGRAM_NUMBER,
                       KS_PROTOCOL_VERSION,
                       "udp");
#endif
}

//////////////////////////////////////////////////////////////////////


bool
KsServer::register_server()
{
    KsAvNoneTicket ticket(KS_ERR_OK); // TODO use a better ticket?
    KssRegistrationContext regctx(ticket);
    regctx.params.server.name = server_name;
    regctx.params.server.protocol_version = protocol_version;
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
    KsAvNoneTicket ticket(KS_ERR_OK); // TODO: better ticket?
    KssUnregistrationContext unregctx(ticket);
    unregctx.params.server.name = server_name;
    unregctx.params.server.protocol_version = protocol_version;

    CLIENT * clnt = createLocalClient();
    if (!clnt) return false;

    enum clnt_stat res = unregctx.call(clnt, KsTime(2,0));
    return res == RPC_SUCCESS && unregctx.result.result == KS_ERR_OK;
}

//////////////////////////////////////////////////////////////////////

void
KsServer::startServer()
{
    // schedule registration
    KsReregisterServerEvent * pevent = 
        new KsReregisterServerEvent(*this, KsTime::now());
    if (!pevent) return;

    addTimerEvent(pevent);
    KsServerBase::startServer();
}

void
KsServer::stopServer() 
{
    // unregister
    if (!unregister_server()) {
        PltLog::Warning("Could not unregister with manager.");
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
    if (!_server.shutdown_flag) {
        //
        // The server is still active, try to register it.
        //
        if (_server.register_server()) {
            // 
            // It worked. Reschedule event.
            //
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
