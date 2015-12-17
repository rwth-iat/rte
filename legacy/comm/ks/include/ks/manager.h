/* -*-plt-c++-*- */
#ifndef KS_MANAGER_INCLUDED
#define KS_MANAGER_INCLUDED
/* $Header: /home/david/cvs/acplt/ks/include/ks/manager.h,v 1.19 2007-04-25 12:57:20 martin Exp $ */
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
