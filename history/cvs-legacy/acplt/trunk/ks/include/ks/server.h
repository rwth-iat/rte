/* -*-plt-c++-*- */
#ifndef KS_SERVER_INCLUDED
#define KS_SERVER_INCLUDED
/* $Header: /home/david/cvs/acplt/ks/include/ks/server.h,v 1.1 1997-04-02 14:52:13 martin Exp $ */
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

#include "ks/svrbase.h"


//////////////////////////////////////////////////////////////////////

class KsReregisterServerEvent;

//////////////////////////////////////////////////////////////////////

class KsServer
: virtual public KsServerBase
{
    friend KsReregisterServerEvent;
public:
    KsServer(const char * svr_name, 
             u_long prot_version,
             u_long ttl);
    virtual ~KsServer();
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
    
#endif // KS_SERVER_INCLUDED


