/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/src/manager.cpp,v 1.3 1997-03-20 09:43:53 martin Exp $ */
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
#if PLT_DEBUG
#include <iostream.h>
#include <iomanip.h>
#endif
//////////////////////////////////////////////////////////////////////

KsManager::KsManager()
: KsServerBase("MANAGER")
{
}

//////////////////////////////////////////////////////////////////////
 
KsManager::~KsManager()
{
    KsManager::destroyTransports();
    // Every registered server has exactly one corresponding
    // timer event in the queue. So we can delete the
    // servers in the table by iterating the queue.
    while (!timer_queue.isEmpty()) {
        KsmExpireServerEvent *pevent =
            (KsmExpireServerEvent *) (KsTimerEvent*)timer_queue.removeFirst();
        if (pevent->pserver) {
            delete pevent->pserver;
        }
        delete pevent;
    }
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
    bool decodedOk = true;
    switch(serviceId) {
        
    case KS_REGISTER: 
        {
            KsRegistrationParams params(xdrIn, decodedOk);
            if (decodedOk) {
#if PLT_DEBUG
                cerr << "REGISTER '";
                cerr << params.server.name << "' ";
                cerr << params.server.protocol_version << " ";
                cerr << params.time_to_live << " >> ";
#endif
                if (isLocal(xprt)) {
                    KsRegistrationResult result;
                    registerServer(ticket, params, result);
#if PLT_DEBUG
                    cerr << hex << result.result << dec << endl;
#endif
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
#if PLT_DEBUG 
                cerr << "UNREGISTER '";
                cerr << params.server.name << "' ";
                cerr << params.server.protocol_version << endl;
#endif
                if (isLocal(xprt)) {
                    KsUnregistrationResult result;
                    unregisterServer(ticket, params, result);
#if PLT_DEBUG
                    cerr << hex << result.result << dec << endl;
#endif
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

    case KS_GETSERVER:
        {
            KsGetServerParams params(xdrIn, decodedOk);
            if (decodedOk) {
#if PLT_DEBUG
                cerr << "GETSERVER '";
                cerr << params.server.name << "' ";
                cerr << params.server.protocol_version << endl;
#endif
                // properly decoded
                KsGetServerResult result;
                getServer(ticket, params, result);
#if PLT_DEBUG
                cerr << hex << result.result << dec << " ";
                cerr << result.server.name << "' ";
                cerr << result.server.protocol_version << " ";
                cerr << ( result.living ? " living" : "dead" )
                    << endl;
#endif
                sendReply(xprt, ticket, result);
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
KsManager::registerServer(KsAvTicket & /*ticket*/,
                    KsRegistrationParams & params,
                    KsRegistrationResult & result)
{
    // TODO use ticket, check params
    
    if (params.server.name == server_name) {
        // Do not overwrite entry for myself!
        result.result = KS_ERR_BADPARAM;
        return;
    }
            
    // calculate expiration time:
    KsTime expire_at = KsTime::now(params.time_to_live);
    
    // create expiration event:
    KsmExpireServerEvent * pevent =
        new KsmExpireServerEvent(this,expire_at);
    if (! pevent) {
        result.result = KS_ERR_GENERIC; return;
    }

    // queue it
    if (! addTimerEvent(PltPtrComparable<KsmExpireServerEvent>(pevent)) ) {
        delete pevent;
        result.result = KS_ERR_GENERIC; return;
    }

    // lookup server:
    KsmServer *pserver;
    PltKeyPtr<KsServerDesc> pdesc (&params.server);
    if (_server_table.query(pdesc,pserver)) {
        // 
        // found it
        //
        pserver->port = params.port;
        pserver->pevent->pserver = 0; // inactivate old event
    } else {

        //
        // unknown server:
        // create server registration object
        //
        pserver = new KsmServer(params.server, 
                                params.port,
                                params.time_to_live,
                                expire_at);
        if (!pserver) {
            result.result = KS_ERR_GENERIC; return;
        }

        // add it to the table of all servers on this machine
        pdesc = &pserver->desc;
        if (! _server_table.add(pdesc, pserver)) {
            delete pserver;
            result.result = KS_ERR_GENERIC; return;
        }
    }
    //
    // Success: event created and queued, serverobj resides in the table.
    // Now connect registration object and event
    //
    pserver->pevent = pevent;
    pevent->pserver = pserver;
    result.result = KS_ERR_OK;
    return;
}


//////////////////////////////////////////////////////////////////////

void
KsManager::unregisterServer(KsAvTicket & /*ticket*/,
                            KsUnregistrationParams & params,
                            KsUnregistrationResult & result)
{
    // TODO: check ticket and params
    
    if (params.server.name == server_name) {
        // Do not remove entry for myself!
        result.result = KS_ERR_BADPARAM;
        return;
    }
    //
    // try to remove server entry
    //
    PltKeyPtr<KsServerDesc> pdesc = &params.server;
    KsmServer *pserver;
    if (_server_table.remove(pdesc, pserver)) {
        //
        // found it
        //
        pserver->pevent->pserver = 0; // inactivate event
        delete pserver; // destroy entry
        result.result = KS_ERR_OK;
    } else {
        //
        // not found
        //
        result.result = KS_ERR_SERVERUNKNOWN;
    }
}

//////////////////////////////////////////////////////////////////////

void
KsManager::getServer(KsAvTicket & /*ticket*/,
                     KsGetServerParams & params,
                     KsGetServerResult & result)
{
    const KsServerDesc & reqdesc = params.server; // requested
    if (reqdesc.protocol_version < 1) {
        result.result = KS_ERR_BADPARAM;
        return;
    }
    // TODO: check ticket and params

    if (params.server.name == server_name) {
        // Hey, that's me!!!
        result.server.name = server_name;
        result.server.protocol_version = protocol_version;
        result.port = _tcp_transport->xp_port;
        result.expires_at = KsTime::now(82400);
        result.living = true;
        result.result = KS_ERR_OK;
        return;
    }
    //
    // find best match
    //
    KsmServer *pbest = 0;
    u_long best_version = 0;
    for (PltHashIterator<PltKeyPtr<KsServerDesc>, KsmServer *> 
             it(_server_table);
         it;
         ++it) {
        const KsServerDesc & desc = it->a_value->desc; // current
        if (desc.protocol_version >= reqdesc.protocol_version
            && desc.name == reqdesc.name ) {
                // this is a match
                if (best_version < desc.protocol_version) {
                    //  remember: this is the best match so far
                    best_version = desc.protocol_version;
                    pbest = it->a_value;
                } // best match
            } // match
    } // iteration
    
    if (pbest) {
        // success
        // fill result structure
        result.server      = pbest->desc;
        result.port        = pbest->port;
        result.expires_at  = pbest->expires_at;
        result.living      = pbest->living;
        result.result      = KS_ERR_OK;
    } else {
        result.result = KS_ERR_SERVERUNKNOWN;
        // fill remaining fields with defensive values
        result.server.name             = "/dead/";
        result.server.protocol_version = 0;
        result.port                    = 0;
        result.expires_at              = KsTime(0,0);
        result.living                  = false;
    }
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void 
KsmExpireServerEvent::trigger()
{
#if PLT_DEBUG
            cerr << "KsmExpireServerEvent: ";
#endif
    if (pserver) {
#if PLT_DEBUG
            cerr << "(" << pserver->desc.name  << " " 
                 << pserver->desc.protocol_version << ") ";
#endif
        PLT_ASSERT(pserver->pevent == this);
        if (pserver->living) {
            // mark server dead
            pserver->living = false;
#if PLT_DEBUG
            cerr << "dying (sp?)" << endl;
#endif
                     
            // reschedule event
            _trigger_at = KsTime::now(pserver->time_to_live);
            _pmanager->addTimerEvent(this);
        } else {
            // remove server
            PltKeyPtr<KsServerDesc> pdesc = &pserver->desc;
            KsmServer *pdummy;
            if (_pmanager->_server_table.remove(pdesc, pdummy)) {
                // ok
                PLT_ASSERT(pdummy == pserver);
#if PLT_DEBUG
                cerr << "removed." << endl;
#endif
                delete pdummy;
                delete this;
            } else {
                PltLog::Error("Could not expire server.");
                // don't delete this. this is a memory leak,
                // but this should not happen anyway.
            }
        }
    } else {
        // inactive
        delete this;
#if PLT_DEBUG
        cerr << "(inactive)" << endl;
#endif
    }
}
                
//////////////////////////////////////////////////////////////////////

/* EOF ks/manager.cpp */
