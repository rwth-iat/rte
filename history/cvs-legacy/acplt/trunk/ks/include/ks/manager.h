/* -*-plt-c++-*- */
#ifndef KS_MANAGER_INCLUDED
#define KS_MANAGER_INCLUDED
/* $Header: /home/david/cvs/acplt/ks/include/ks/manager.h,v 1.14 1998-06-29 11:16:08 harald Exp $ */
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
#include "ks/rpc.h"
#include "ks/hostinaddrset.h"
#include "plt/hashtable.h"

#if PLT_USE_BUFFERED_STREAMS
#include "ks/xdrudpcon.h"
#endif

//////////////////////////////////////////////////////////////////////
// forward declarations
//////////////////////////////////////////////////////////////////////

class KsmServer;
class KsmExpireServerEvent;
class KsmExpireManagerEvent;

//////////////////////////////////////////////////////////////////////
// This is the MANAGER class, which represents an ACPLT/KS server
// which acts as the MANAGER.
//

class KsManager
: public KsSimpleServer
{
public:
    KsManager(int port = KS_ANYPORT);
    virtual ~KsManager();
    //// accessors
    // "virtual constants"
    virtual KsString getServerName() const;
    virtual KsString getServerVersion() const=0;
    virtual KsString getServerDescription() const;
    virtual KsString getVendorName () const;


    const PltHashTable<PltKeyPtr<KsServerDesc>, KsmServer *> &
        getServerTable() const
            { return _server_table; }

    virtual void startServer();
    virtual void stopServer();

protected:
    virtual void dispatch(u_long serviceId, 
                          KssTransport &transport,
                          XDR *incomingXdr,
                          KsAvTicket &ticket);

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

private:
    friend class KsmExpireServerEvent;
    friend class KsmExpireManagerEvent;

    void cleanup();

    void removeServer(KsmServer *p);
    bool isLocal(KssTransport &t);

#if !PLT_USE_BUFFERED_STREAMS
    SVCXPRT *_udp_transport;
#else
    KssXDRConnection *_udp_transport;
#endif
    bool _registered;
    PltHashTable<PltKeyPtr<KsServerDesc>, KsmServer *> _server_table;
    KssSimpleDomain _servers_domain;
    KsHostInAddrSet _localIpAddresses;

    KssSimpleVariable *_manager_port;
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
