/* -*-plt-c++-*- */
#ifndef KS_MANAGER_INCLUDED
#define KS_MANAGER_INCLUDED
/* $Header: /home/david/cvs/acplt/ks/include/ks/manager.h,v 1.8 1997-04-03 10:04:19 martin Exp $ */
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

#include "ks/simpleserver.h"
#include "ks/register.h"
#include "ks/serviceparams.h"
#include "ks/path.h"

extern "C" {
#include <rpc/pmap_clnt.h>
};

#include "plt/hashtable.h"

//////////////////////////////////////////////////////////////////////
// forward declarations
//////////////////////////////////////////////////////////////////////

class KsmServer;
class KsmExpireServerEvent;
class KsmExpireManagerEvent;

//////////////////////////////////////////////////////////////////////

class KsManager
: public KsSimpleServer
{
public:
    KsManager();
    virtual ~KsManager();
    //// accessors
    virtual KsString getServerName() const 
        { return KsString("MANAGER"); }

    virtual u_long   getProtocolVersion() const
        { return 1; }

    const PltHashTable<PltKeyPtr<KsServerDesc>, KsmServer *> &
        getServerTable() const
            { return _server_table; }

    virtual void startServer();
    virtual void stopServer();

protected:
    virtual void dispatch(u_long serviceId, 
                          SVCXPRT *transport,
                          XDR *incomingXdr,
                          KsAvTicket &ticket);

    virtual bool createTransports();
    virtual void destroyTransports();

    // service functions
    void registerServer(KsAvTicket & ticket,
                  KsRegistrationParams & params,
                  KsRegistrationResult & result);

    void unregisterServer(KsAvTicket & ticket,
                    KsUnregistrationParams & params,
                    KsUnregistrationResult & result);

    void getServer(KsAvTicket & ticket,
                    KsGetServerParams & params,
                    KsGetServerResult & result);

    virtual bool initObjectTree(); // initialize object tree with predefined values

private:
    friend KsmExpireServerEvent;
    friend KsmExpireManagerEvent;
    void removeServer(KsmServer *p);
    static bool isLocal(SVCXPRT *);
    SVCXPRT *_udp_transport;
    bool _registered;
    PltHashTable<PltKeyPtr<KsServerDesc>, KsmServer *> _server_table;
    KssSimpleDomain _servers_domain;
};

//////////////////////////////////////////////////////////////////////
// IMPLEMENTATION
//////////////////////////////////////////////////////////////////////

class KsmExpireServerEvent
: public KsTimerEvent
{
public:
    KsmExpireServerEvent(KsManager &m, const KsTime & at, KsmServer * p = 0)
        : KsTimerEvent(at), pserver(p), _manager(m) { }

    virtual void trigger();
    KsmServer * pserver;
private:
    KsManager & _manager;
    PLT_DECL_RTTI;
};


//////////////////////////////////////////////////////////////////////


class KsmExpireManagerEvent
: public KsTimerEvent
{
private:
    static const KsTime _check_delay;
public:
    KsmExpireManagerEvent(KsManager &m, 
                          const KsTime & at = KsTime::now(_check_delay)) 
        : KsTimerEvent(at), _manager(m) { }

    virtual void trigger();
private:
    KsManager &_manager;
    PLT_DECL_RTTI;
};



#endif // KS_MANAGER_INCLUDED
