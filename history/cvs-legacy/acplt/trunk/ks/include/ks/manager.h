/* -*-plt-c++-*- */
#ifndef KS_MANAGER_INCLUDED
#define KS_MANAGER_INCLUDED
/* $Header: /home/david/cvs/acplt/ks/include/ks/manager.h,v 1.3 1997-03-24 18:40:14 martin Exp $ */
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
#include "ks/objecttree.h"
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

//////////////////////////////////////////////////////////////////////

class KsManager
: public KsSimpleServer
{
public:
    KsManager();
    virtual ~KsManager();
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
    void getVar(KsAvTicket &ticket,
                const KsPath & path,
                KsGetVarItemResult &result);

    friend KsmExpireServerEvent;
    //
    // object tree manipulation
    //
    KsObjectTree _object_tree;

    static bool isLocal(SVCXPRT *);
    SVCXPRT *_udp_transport;
    PltHashTable<PltKeyPtr<KsServerDesc>, KsmServer *> _server_table;
};
    

//////////////////////////////////////////////////////////////////////

    
struct KsmServer
{
public:
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
};


//////////////////////////////////////////////////////////////////////

class KsmExpireServerEvent
: public KsTimerEvent
{
public:
    KsmExpireServerEvent(KsManager *m, const KsTime & at, KsmServer * p = 0)
        : KsTimerEvent(at), pserver(p), _pmanager(m) { }

    virtual void trigger();
    KsmServer * pserver;
private:
    KsManager * _pmanager;
};

//////////////////////////////////////////////////////////////////////
// INLINE IMPLEMENTATION
//////////////////////////////////////////////////////////////////////
inline
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

#endif // KS_MANAGER_INCLUDED
