/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/src/manager.cpp,v 1.1 1997-03-17 19:58:15 martin Exp $ */
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

//////////////////////////////////////////////////////////////////////

KsManager::KsManager()
: KsServerBase("Manager")
{
}

//////////////////////////////////////////////////////////////////////
 
KsManager::~KsManager()
{
    KsManager::destroyTransports();
}

//////////////////////////////////////////////////////////////////////
extern int from_local(struct sockaddr_in *);

bool 
KsManager::isLocal(SVCXPRT *xprt)
{
    struct sockaddr_in * sin = svc_getcaller(xprt);
    PLT_ASSERT(sin);
    if (from_local(sin)) {
        return true;
    } else {
        return false;
    }
}
         
//////////////////////////////////////////////////////////////////////

void
KsManager::dispatch(u_long serviceId, 
                    SVCXPRT *xprt,
                    XDR *xdrIn,
                    KsAvTicket &ticket)
{
    bool decodedOk;
    switch(serviceId) {
        
    case KS_REGISTER: 
        {
            KsRegistrationParams params(xdrIn, decodedOk);
            if (decodedOk) {
                if (isLocal(xprt)) {
                    KsRegistrationResult result;
                    registerServer(ticket, params, result);
                    sendReply(xprt, ticket, result);
                } else {
                    sendErrorReply(xprt, ticket, KS_ERR_NOREMOTE);
                }
            } else {
                // not properly decoded
                sendErrorReply(xprt, ticket, KS_ERR_GENERIC);
            }
        } 
        break;

    case KS_UNREGISTER:
        {
            KsUnregistrationParams params(xdrIn, decodedOk);
            if (decodedOk) {
                if (isLocal(xprt)) {
                    KsUnregistrationResult result;
                    unregisterServer(ticket, params, result);
                    sendReply(xprt, ticket, result);
                } else {
                    // not local
                    sendErrorReply(xprt, ticket, KS_ERR_NOREMOTE);
                }
            } else {
                // not properly decoded
                sendErrorReply(xprt, ticket, KS_ERR_GENERIC);
            }
        }
        break;

        // TODO: remaining services

    default:
        // fall back on base class
        KsServerBase::dispatch(serviceId, xprt, xdrIn, ticket);
    }
}

//////////////////////////////////////////////////////////////////////

bool
KsManager::createTransports()
{
    if (! KsServerBase::createTransports()) {
        return false;
    }
    _udp_transport = svcudp_create(RPC_ANYSOCK); // TODO: send/receive buff sz
    if (! _udp_transport) {
        PltLog::Error("Can't create UDP transport");
        return false;
    }
    if (pmap_unset(KS_RPC_PROGRAM_NUMBER, KS_PROTOCOL_VERSION)) {
        PltLog::Debug("Removed old pmap entry.");
    }
    if (! pmap_set(KS_RPC_PROGRAM_NUMBER, 
                   KS_PROTOCOL_VERSION,
                   IPPROTO_TCP,
                   _tcp_transport->xp_port)) {
        PltLog::Error("Can't register TCP transport.");
        return false;
    }
    if (! pmap_set(KS_RPC_PROGRAM_NUMBER, 
                   KS_PROTOCOL_VERSION,
                   IPPROTO_UDP,
                   _udp_transport->xp_port)) {
        PltLog::Error("Can't register UDP transport.");
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////

void 
KsManager::destroyTransports()
{
    if (! pmap_unset(KS_RPC_PROGRAM_NUMBER, KS_PROTOCOL_VERSION)) {
        PltLog::Warning("Can't unregister with portmapper.");
    }
    if ( _udp_transport ) {
        svc_destroy(_udp_transport); 
        _udp_transport = 0;
    }
    KsServerBase::destroyTransports();
}

//////////////////////////////////////////////////////////////////////

void
KsManager::registerServer(KsAvTicket & ticket,
                    KsRegistrationParams & params,
                    KsRegistrationResult & result)
{
    result.result = KS_ERR_BADNAME;
}


//////////////////////////////////////////////////////////////////////

void
KsManager::unregisterServer(KsAvTicket & ticket,
                    KsUnregistrationParams & params,
                    KsUnregistrationResult & result)
{
    result.result = KS_ERR_BADNAME;
}


//////////////////////////////////////////////////////////////////////

/* EOF ks/manager.cpp */
