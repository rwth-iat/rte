/* -*-plt-c++-*- */
#ifndef KS_SERVER_INCLUDED
#define KS_SERVER_INCLUDED
/* $Header: /home/david/cvs/acplt/ks/include/ks/server.h,v 1.8 1999-09-06 06:57:00 harald Exp $ */
/*
 * Copyright (c) 1996, 1997, 1998, 1999
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


