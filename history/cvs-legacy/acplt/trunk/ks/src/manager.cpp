/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/src/manager.cpp,v 1.7 1997-04-02 14:52:20 martin Exp $ */
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
#include "ks/path.h"
#include "plt/log.h"
#include <ctype.h>
#include <string.h>
#include <sys/socket.h>

static const KsString KS_MANAGER_NAME("MANAGER");
static const KsString KS_MANAGER_VERSION("1");
static const KsString KS_MANAGER_DESCRIPTION("ACPLT/KS Manager");


//////////////////////////////////////////////////////////////////////

class KsmServer;

/////////////////////////////////////////////////////////////////////////////

class KsmServerIterator
: public KssDomainIterator
{
public:
    KsmServerIterator(const KsmServer & d)
        : _d(d), _what(0) { }

    virtual operator const void * () const
        { return (_what < STOP) ? this : 0; }
    
    virtual KssCommObjectHandle operator * () const;

    virtual KsmServerIterator& operator ++ ()
        { ++_what; return *this; }

    virtual void toStart()
        { _what = 0; }

private:
    enum { EXPIRES_AT, LIVING, PORT, STOP }; 
    const KsmServer & _d;
    enum_t _what;
    static const char * _ids[4];
};

//////////////////////////////////////////////////////////////////////

struct KsmServer
: public KssDomain
{
public:
    //// KssCommObject ////
    //// accessors

    // projected properties
    virtual KsString getIdentifier() const;
    virtual KsTime   getCreationTime() const;
    virtual KsString getComment() const;

    //// KssDomain
    //   accessors
    virtual KsmServerIterator * newIterator() const;


//  virtual KssCommObjectHandle getChildByPath(const KsPath & path) const;

    //// KsmServer
    KsmServer(const KsServerDesc & d,
              u_short p,
              u_long ttl,
              KsTime exp);
    KsServerDesc desc;
    u_short port;
    KsTime expires_at;
    u_long time_to_live; // seconds
    bool living;
    KsmExpireServerEvent *pevent;

    PLT_DECL_RTTI;
};

//////////////////////////////////////////////////////////////////////

PLT_IMPL_RTTI1(KsmServer, KssDomain);

//////////////////////////////////////////////////////////////////////

KsString 
KsmServer::getIdentifier() const
{ 
    return KsString::fromInt(desc.protocol_version); 
}

//////////////////////////////////////////////////////////////////////

KsTime   
KsmServer::getCreationTime() const
{ 
    return KsTime::now(); 
}

//////////////////////////////////////////////////////////////////////

KsString 
KsmServer::getComment() const
{ 
    return KsString(); 
}

//////////////////////////////////////////////////////////////////////

KsmServerIterator *
KsmServer::newIterator() const
{
    return new KsmServerIterator(*this);
}

//////////////////////////////////////////////////////////////////////


const char * KsmServerIterator::_ids[4] = 
{
    "expires_at",
    "living",
    "port",
    "/BUG/"
};

//////////////////////////////////////////////////////////////////////

KssCommObjectHandle 
KsmServerIterator::operator * () const 
{
    KssSimpleVariable *pvar = new KssSimpleVariable(_ids[_what]);
    if (pvar) {
        switch (_what) {
        case EXPIRES_AT:
            pvar->setValue(new KsTimeValue(_d.expires_at));
            pvar->lock();
            break;
        case LIVING:
            pvar->setValue(new KsIntValue(_d.living ? 1 : 0));
            pvar->lock();
            break;
        case PORT:
            pvar->setValue(new KsIntValue(_d.port));
            pvar->lock();
            break;
        default:
            PLT_ASSERT(0==1);
        };
    }
    return KssCommObjectHandle(pvar, KsOsNew);
}


//////////////////////////////////////////////////////////////////////

KsmServer::KsmServer(const KsServerDesc & d,
                     u_short p,
                     u_long ttl,
                     KsTime exp)
: desc(d), 
  port(p), 
  expires_at(exp), 
  time_to_live(ttl),
  living(true), 
  pevent(0) 
{ 
}

//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////

KsManager::KsManager()
:  KsServerBase(KS_MANAGER_NAME),
   KsSimpleServer(KS_MANAGER_NAME),
  _registered(false),
  _servers_domain("servers")
{
}

//////////////////////////////////////////////////////////////////////
 
KsManager::~KsManager()
{
    KsManager::destroyTransports();
    // server entries are owned (indirectly) by _root_domain
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
    //
    // Now register the dispatcher that should be called whenever there
    // is a request for the KS program id and the correct version number.
    //
    if ( !svc_register(_udp_transport,
                       KS_RPC_PROGRAM_NUMBER,
                       KS_PROTOCOL_VERSION,
                       ks_c_dispatch,
                       0) ) {  // Do not contact the portmapper!
        svc_destroy(_tcp_transport);
        _tcp_transport = 0;
        PltLog::Error("KsServerBase: could not register");
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////

void
KsManager::startServer()
{
    KsSimpleServer::startServer();
    //
    // Contact the
    // portmapper and tell him that we are ready to receive
    // requests on them.
    //
    if (pmap_unset(KS_RPC_PROGRAM_NUMBER, KS_PROTOCOL_VERSION)) {
        PltLog::Debug("Removed old pmap entry.");
    }
    _registered = false;
    if (pmap_set(KS_RPC_PROGRAM_NUMBER, 
                 KS_PROTOCOL_VERSION,
                 IPPROTO_TCP,
                 _tcp_transport->xp_port)) {
        _registered = true;
    } else {
        PltLog::Error("Can't register TCP transport.");
    }
    if (pmap_set(KS_RPC_PROGRAM_NUMBER, 
                 KS_PROTOCOL_VERSION,
                 IPPROTO_UDP,
                 _udp_transport->xp_port)) {
        _registered = true;
    } else {
        PltLog::Error("Can't register UDP transport.");
    }
    KsmExpireManagerEvent * pevent = new KsmExpireManagerEvent(*this);
    if (pevent) {
        addTimerEvent(pevent);
    }
}

//////////////////////////////////////////////////////////////////////

void
KsManager::stopServer()
{
    if (   _registered
        && ! pmap_unset(KS_RPC_PROGRAM_NUMBER, KS_PROTOCOL_VERSION)) {
        PltLog::Warning("Can't unregister with portmapper.");
    }
    KsSimpleServer::stopServer();
}

//////////////////////////////////////////////////////////////////////

void 
KsManager::destroyTransports()
{
    if ( _udp_transport ) {
        svc_destroy(_udp_transport); 
        _udp_transport = 0;
    }
    KsServerBase::destroyTransports();
}

//////////////////////////////////////////////////////////////////////

static inline bool
isValidManagerName(const PltString & name)
{
    // Allowed characters: [A-Za-z0-9_]
    for (size_t i; i<name.len(); ++i) {
        if (!isalnum(name[i]) && name[i] != '_' ) {
            return false;
        }
    }
    return true;
}

//////////////////////////////////////////////////////////////////////

void
KsManager::registerServer(KsAvTicket & /*ticket*/,
                    KsRegistrationParams & params,
                    KsRegistrationResult & result)
{
    // TODO use ticket, check params
    if (   !isValidManagerName(params.server.name)
        || params.server.protocol_version < 1) {
        result.result = KS_ERR_BADPARAM;
        return;
    }
    if (params.server.name == server_name) {
        // Do not overwrite entry for myself!
        result.result = KS_ERR_BADPARAM;
        return;
    }
            
    // calculate expiration time:
    KsTime expire_at = KsTime::now(params.time_to_live);
    
    // create expiration event:
    KsmExpireServerEvent * pevent =
        new KsmExpireServerEvent(*this,expire_at);
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
        pserver->expires_at = expire_at;
        pserver->pevent->pserver = 0; // inactivate old event
    } else {
        //
        // Unknown server:
        // Create server registration object...
        //
        pserver = new KsmServer(params.server, 
                                params.port,
                                params.time_to_live,
                                expire_at);
        if (!pserver) {
            result.result = KS_ERR_GENERIC; return;
        }
        //
        // ...add it to the table of all servers on this machine...
        //
        pdesc = &pserver->desc;
        if (! _server_table.add(pdesc, pserver)) {
            delete pserver;
            result.result = KS_ERR_GENERIC; return;
        }
        //
        // ... finally create /servers/... subdomains
        //
        KssCommObjectHandle hs(_servers_domain.getChildById(pdesc->name));
        KssSimpleDomain *ps;
        bool fresh;
        if (hs) {
            // This is another version of an existing server.
            ps = PLT_DYNAMIC_PCAST(KssSimpleDomain, hs.getPtr());
            PLT_ASSERT(ps);
            fresh = false;
        } else {
            ps = new KssSimpleDomain(pdesc->name);
            if (   ! hs.bindTo(ps, KsOsNew) 
                || ! _servers_domain.addChild(hs)) {
                result.result = KS_ERR_GENERIC;
                return;
            }
            fresh = true;
        }
        KssCommObjectHandle hv;
        if (   ! hv.bindTo(pserver, KsOsNew) 
            || ! ps->addChild(hv)) {
            if (fresh) {
                _servers_domain.removeChild(pdesc->name);
                result.result = KS_ERR_GENERIC;
                return;
            }
        }
    }
    //
    // Success: event created and queued, serverobj resides in the tables.
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
    // Lookup server entry.
    //
    PltKeyPtr<KsServerDesc> pdesc = &params.server;
    KsmServer *pserver;
    if (_server_table.query(pdesc, pserver)) {
        //
        // found it
        //
        removeServer(pserver);
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
        result.expires_at = KsTime(LONG_MAX);
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
    }
}

//////////////////////////////////////////////////////////////////////

PLT_IMPL_RTTI1(KsmExpireServerEvent, KsEvent);

//////////////////////////////////////////////////////////////////////

void 
KsmExpireServerEvent::trigger()
{
    PLT_DMSG("KsmExpireServerEvent: ");
    if (pserver) {
        PLT_DMSG("(" << pserver->desc.name  << " " 
                 << pserver->desc.protocol_version << ") ");
        PLT_ASSERT(pserver->pevent == this);
        //
        // Is the server living?
        //
        if (pserver->living) {
            //
            // Mark it dead...
            //
            pserver->living = false;
            PLT_DMSG("dying << endl");
            //
            // ...and reschedule event.
            // 
            _trigger_at = KsTime::now(pserver->time_to_live);
            _manager.addTimerEvent(this);
        } else {
            // 
            // Remove the zombie.
            //
            PLT_DMSG("being removed." << endl);
            _manager.removeServer(pserver);
            delete this;
        }
    } else {
        // 
        // This event is inactive. Do nothing
        //
        delete this;
        PLT_DMSG("(inactive)" << endl);
    }
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

PLT_IMPL_RTTI1(KsmExpireManagerEvent, KsTimerEvent);

//////////////////////////////////////////////////////////////////////

const KsTime
KsmExpireManagerEvent::_check_delay(30,0);

//////////////////////////////////////////////////////////////////////

void
KsmExpireManagerEvent::trigger()
{
    // Check if we are still registered.
    // Looking at the TCP port registration should be sufficient.
    struct sockaddr_in sin;
#if 1
    memset(&sin, 0, sizeof sin);
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
#else
    get_myaddress(&sin);
#endif
    u_short port = pmap_getport(&sin, 
                                KS_RPC_PROGRAM_NUMBER,
                                KS_PROTOCOL_VERSION,
                                IPPROTO_TCP);
    if (port == 0 && rpc_createerr.cf_stat != RPC_SUCCESS) {
        //
        // Portmapper failure. Report this and retry soon.
        //
        PltLog::Warning("Can't query portmapper. Check if it is running!");
        PLT_DMSG("Portmapper failure:" << rpc_createerr.cf_stat << endl);
        _trigger_at = KsTime::now(60);
        _manager.addTimerEvent(this);
    } else {
        // 
        // Ok. The port value is valid.
        //
        if (port == _manager._tcp_transport->xp_port) {
            //
            // We are still registered. 
            // Reschedule event.
            //
            PLT_DMSG("Still registered with portmapper" << endl);
            _trigger_at = KsTime::now(_check_delay);
            _manager.addTimerEvent(this);
        } else {
            //
            // Someone has unregistered us.
            // No client can reach us anymore, so exit.
            //
            PLT_DMSG("Not registered with portmapper. Stopping now." << endl);
            _manager._registered = false;
            _manager.stopServer();
            delete this; // suicide
        }
    }
}        
        
//////////////////////////////////////////////////////////////////////

void
KsManager::removeServer(KsmServer * pserver) 
{
    PLT_PRECONDITION(pserver);
    PltKeyPtr<KsServerDesc> pdesc = &pserver->desc;
    // 
    // Remove server from the table...
    //
    KsmServer *pdummy;
    bool removed = _server_table.remove(pdesc, pdummy);
    PLT_ASSERT(removed && pdummy == pserver);
    //
    // ... and inactivate the associated event.
    //
    pserver->pevent->pserver = 0;
    //
    // Then remove associated /servers/... entries
    // from the tree. This deletes the entry through
    // the handle mechanism.
    //
    KssCommObjectHandle hs(_servers_domain.getChildById(pdesc->name));
    PLT_ASSERT(hs);
    KssSimpleDomain *ps = PLT_DYNAMIC_PCAST(KssSimpleDomain,hs.getPtr());
    PLT_ASSERT(ps && ps->size() > 0);
    if (ps->size() == 1) {
        //
        // Server is single version, remove entire subtree
        //
        _servers_domain.removeChild(pdesc->name);
    } else {
        //
        // Remove only the domain for this version.
        //
        PltString version(PltString::fromInt(pdesc->protocol_version));
        ps->removeChild(version);
    }
}
//////////////////////////////////////////////////////////////////////
// Object tree
//////////////////////////////////////////////////////////////////////

bool
KsManager::initObjectTree()
{
    if (initVendorTree(KS_MANAGER_DESCRIPTION,
                       "Lehrstuhl fuer Prozessleittechnik, RWTH-Aachen")) {
        KssSimpleDomain *servers_manager =
            new KssSimpleDomain(KS_MANAGER_NAME);
        
        KssSimpleDomain *servers_manager_version =
            new KssSimpleDomain(KS_MANAGER_VERSION);
        
        KssSimpleVariable *manager_port =
            new KssSimpleVariable("port");
        
        KssSimpleVariable *manager_expires_at =
            new KssSimpleVariable("expires_at");
        
        KssSimpleVariable *manager_living =
            new KssSimpleVariable("living");
        
        manager_living->setValue(new KsIntValue(1));
        manager_port->setValue(new KsIntValue(_tcp_transport->xp_port));
        manager_expires_at->setValue(new KsTimeValue(LONG_MAX,0));
        
        servers_manager_version->addChild(manager_living);
        servers_manager_version->addChild(manager_port);
        servers_manager_version->addChild(manager_expires_at);
        
        servers_manager->addChild(servers_manager_version);
        _servers_domain.addChild(servers_manager);
        
        _root_domain.addChild(KssCommObjectHandle(&_servers_domain, 
                                              KsOsUnmanaged));
    }

    return true; // TODO: This is optimistic...
}
                
//////////////////////////////////////////////////////////////////////

/* EOF ks/manager.cpp */


