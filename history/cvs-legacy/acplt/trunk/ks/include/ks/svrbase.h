/* -*-plt-c++-*- */
#ifndef KS_SVRBASE_INCLUDED
#define KS_SVRBASE_INCLUDED
/* $Header: /home/david/cvs/acplt/ks/include/ks/svrbase.h,v 1.8 1997-04-02 14:52:15 martin Exp $ */
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

#include "ks/rpc.h"
#include "ks/avticket.h"
#include "ks/event.h"
#include "ks/serviceparams.h"
#include "plt/comparable.h"
#include "plt/priorityqueue.h"

//////////////////////////////////////////////////////////////////////
// PRIVATE! forward declaration

extern "C" void 
ks_c_dispatch(struct svc_req * request, SVCXPRT *transport);

/////////////////////////////////////////////////////////////////////////////
//
//
class KsServerBase {
public:
    KsServerBase(const char *svr_name,
                 u_long prot_version = KS_PROTOCOL_VERSION);
    virtual ~KsServerBase(); // make sure the destructor is virtual...
    
    virtual void startServer();    // start answering requests
    virtual void run();            // This is the main loop
    virtual void stopServer();     // stop answering requests asap
    bool hasPendingEvents() const;   // check for events that want to be served
 
    // serve pending events
    bool servePendingEvents(KsTime timeout = KsTime(0,0)); 
    bool servePendingEvents(KsTime *pTimeout); 

    bool addTimerEvent(KsTimerEvent *event);
    bool removeTimerEvent(KsTimerEvent *event);
    KsTimerEvent *getNextTimerEvent();
    
    static KsServerBase & getServerObject();

    // service functions
    virtual void getVar(KsAvTicket &ticket,
                        KsGetVarParams &params,
                        KsGetVarResult &result) = 0;

    virtual void getPP(KsAvTicket &ticket, 
                       const KsGetPPParams & params,
                       KsGetPPResult & result) = 0;

    virtual void setVar(KsAvTicket &ticket,
                        KsSetVarParams &params,
                        KsSetVarResult &result);

protected:
    virtual void dispatch(u_long serviceId, 
                          SVCXPRT *transport,
                          XDR *incomingXdr,
                          KsAvTicket &ticket);

    static void sendReply(SVCXPRT *transport, 
                          KsAvTicket &ticket,
                          KsResult &result);

    static void sendErrorReply(SVCXPRT *transport, 
                               KsAvTicket &ticket,
                               KS_RESULT result);

    virtual bool createTransports();
    virtual void destroyTransports();



    // KS_RESULT result; // TODO: wofuer???

    void init();

    PltString server_name;
    u_long protocol_version;
    PltPriorityQueue< PltPtrComparable<KsTimerEvent> > timer_queue;
    SVCXPRT *_tcp_transport; // RPC transport used to receive requests
    bool shutdown_flag; // signal to the run() loop to quit
private:
    friend void ks_c_dispatch(struct svc_req * request, SVCXPRT *transport);

    KsServerBase(const KsServerBase &); // forbidden
    KsServerBase & operator = (const KsServerBase &); // forbidden

    int send_buffer_size;
    int receive_buffer_size;

    static XDR *getXdrForTransport(SVCXPRT *transport);
    static void dispatcher(struct svc_req *request, SVCXPRT *transport);
    int serveRequests(const KsTime *pTimeout);
    void destroyLurkingTransports();
    static KsServerBase *the_server;
};



/////////////////////////////////////////////////////////////////////////////
//
class KsServiceResult {
public:
    virtual bool_t xdrEncode(XDR *xdr) = 0;

    KS_RESULT result;
};



//////////////////////////////////////////////////////////////////////
// INLINE IMPLEMENTATION
//////////////////////////////////////////////////////////////////////

inline bool
KsServerBase::servePendingEvents(KsTime timeout) 
{
    return servePendingEvents(&timeout);
}

//////////////////////////////////////////////////////////////////////

inline KsServerBase &
KsServerBase::getServerObject()
{
    PLT_PRECONDITION(the_server);
    return *the_server;
}
//////////////////////////////////////////////////////////////////////

#endif // PLT_SVRBASE_INCLUDED
// End of svrbase.h
