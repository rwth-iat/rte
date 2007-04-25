/* -*-plt-c++-*- */
#ifndef KS_SERVER_INCLUDED
#define KS_SERVER_INCLUDED
/* $Header: /home/david/cvs/acplt/ks/include/ks/server.h,v 1.13 2007-04-25 12:57:20 martin Exp $ */
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
 * server.h: this module contains the knowledge how to register ACPLT/KS
 *           servers with the ACPLT/KS manager. There isn't really much
 *           more to it, folks.
 */

/*
 * Written by Harald Albrecht & Martin Kneissl
 * <harald@plt.rwth-aachen.de> <martin@plt.rwth-aachen.de>
 */

#include "ks/svrbase.h"


//////////////////////////////////////////////////////////////////////

class KsReregisterServerEvent;

//////////////////////////////////////////////////////////////////////

class KsServer
: virtual public KsServerBase
{
    friend class KsReregisterServerEvent;
public:
    KsServer(u_long ttl, int port = KS_ANYPORT);
    virtual ~KsServer();

    //// accessors
    virtual KsString getServerName() const=0;
    virtual KsString getServerVersion() const=0;
    virtual KsString getServerDescription() const=0;
    virtual KsString getVendorName() const=0;

    virtual void startServer();    // start answering requests
    virtual void stopServer();     // stop answering requests asap

protected:
    static CLIENT *createLocalClient();
    bool register_server();
    bool unregister_server();
private:
    u_long _ttl;
    bool _registered;
};

//////////////////////////////////////////////////////////////////////

class KsReregisterServerEvent
: public KsTimerEvent
{
public:
    KsReregisterServerEvent(KsServer & svr,
                            const KsTime at)
        : KsTimerEvent(at), _server(svr) { }

    virtual void trigger();
private:
    static const u_long MIN_TTL;
    KsServer & _server;
};

//////////////////////////////////////////////////////////////////////


//
// The purpose of the following declaration and the definition is to
// provide a link-time check making sure that both the ACPLT/KS server
// and the libkssvr were compiled with either PLT_USE_BUFFERED_STREAMS
// enabled or disabled, but not one with enabled buffering and the other
// one without.
//
extern void *This_libKssvr_Was_Compiled_Without_PLT_USE_BUFFERED_STREAMS;
extern void *This_libKssvr_Was_Compiled_With_PLT_USE_BUFFERED_STREAMS;
static void *Checking_PLT_USE_BUFFERED_STREAMS_Option = 
#if PLT_USE_BUFFERED_STREAMS
    &This_libKssvr_Was_Compiled_Without_PLT_USE_BUFFERED_STREAMS;
#else
    &This_libKssvr_Was_Compiled_With_PLT_USE_BUFFERED_STREAMS;
#endif

    
#endif // KS_SERVER_INCLUDED


