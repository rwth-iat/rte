/* -*-plt-c++-*- */
#ifndef KS_SVRBASE_INCLUDED
#define KS_SVRBASE_INCLUDED
/* $Header: /home/david/cvs/acplt/ks/include/ks/svrbase.h,v 1.24 1999-09-06 08:43:58 dirk Exp $ */
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

#include "ks/rpc.h"
#include "ks/avticket.h"
#include "ks/event.h"
#if !PLT_SERVER_TRUNC_ONLY
#include "ks/serviceparams.h"
#endif
#include "plt/comparable.h"
#include "plt/priorityqueue.h"
#include "ks/svrtransport.h"

#include <signal.h>

// This is currently missing in Cygnus' headers...
#if PLT_COMPILER_CYGWIN 
typedef int sig_atomic_t;
#endif

#if PLT_USE_BUFFERED_STREAMS
#include "ks/connectionmgr.h"
#include "ks/xdrtcpcon.h"
#endif


// ---------------------------------------------------------------------------
// PRIVATE! forward declaration
// This dispatcher is only used with the genuine ONC/RPC package.
//
#if !PLT_USE_BUFFERED_STREAMS
extern "C" void 
ks_c_dispatch(struct svc_req * request, SVCXPRT *transport);
#endif


// ---------------------------------------------------------------------------
// The root cause for all KS server stuff.
//
class KsServerBase {
public:
    KsServerBase();
    virtual ~KsServerBase(); // make sure the destructor is virtual...

    enum { KS_ANYPORT = 0 };

    //// accessors
    // "virtual constants"
    virtual KsString getServerName() const=0;
    virtual KsString getServerVersion() const=0;
    virtual KsString getServerDescription() const = 0;
    virtual KsString getVendorName () const = 0;

    virtual u_short  getProtocolVersion() const 
        { return KS_PROTOCOL_VERSION; }

    virtual bool isOk() const { return _is_ok; }
    virtual bool isGoingDown() const { return _shutdown_flag != 0; }

    static KsServerBase & getServerObject();

    bool hasPendingEvents() const;   // check for events that want to be served
    const KsTimerEvent *peekNextTimerEvent() const;

    //// modifiers
    virtual void startServer();    // start answering requests
    virtual void run();            // This is the main loop
    virtual void downServer();     // stop answering requests asap
    virtual void stopServer();     // shut down request handling finally
 
    // serve pending events
    bool servePendingEvents(KsTime timeout = KsTime()); 
    bool servePendingEvents(KsTime *pTimeout); 

    bool addTimerEvent(KsTimerEvent *event);
    bool removeTimerEvent(KsTimerEvent *event);
    KsTimerEvent *removeNextTimerEvent();

    // service functions
#if !PLT_SERVER_TRUNC_ONLY
    virtual void getVar(KsAvTicket &ticket,
                        const KsGetVarParams &params,
                        KsGetVarResult &result);

    virtual void getEP(KsAvTicket &ticket, 
                       const KsGetEPParams & params,
                       KsGetEPResult & result);

    virtual void setVar(KsAvTicket &ticket,
                        const KsSetVarParams &params,
                        KsSetVarResult &result);

    virtual void exgData(KsAvTicket &ticket,
                         const KsExgDataParams &params,
                         KsExgDataResult &result);
#endif

#if PLT_USE_BUFFERED_STREAMS
    KssConnectionManager *getConnectionManager() const
	{ return _cnx_manager; }
#endif

    inline void setBufferSizes(int rxBuffSize, int txBuffSize)
    	{ _receive_buffer_size = rxBuffSize;
          _send_buffer_size = txBuffSize; }


    void dispatchTransport(KssTransport &transp);

        
protected:
    virtual void dispatch(u_long serviceId, 
                          KssTransport &transport,
                          XDR *incomingXdr,
                          KsAvTicket &ticket);

    // protected attributes
    bool _is_ok;
    int _sock_port; // RPC socket port number

    // deserializes ticket or return 0 on failure. It can also return
    // an emergency ticket with a result() != 0.
    virtual KsAvTicket* getTicket(XDR* xdr);

#if !PLT_USE_BUFFERED_STREAMS
    SVCXPRT *_tcp_transport; // RPC transport used to receive requests
    KssTransport _transport; // SVCXPRT wrapper for ONC/RPC
#else
    class KssAttentionXDRDispatcher:
        public KssConnectionAttentionInterface {
    public:
        virtual bool attention(KssConnection &conn);
    };

    KssAttentionXDRDispatcher  _attention_dispatcher;
    KssConnectionManager      *_cnx_manager;
    KssXDRConnection          *_tcp_transport;
#endif

#if PLT_USE_XTI
    // ...and now for something completely different: Solaris & XTI!!!
    PltString getNetworkTransportDevice(const char *protocol);
#endif

    sig_atomic_t _shutdown_flag; // signal to the run() loop to quit

private:
#if !PLT_USE_BUFFERED_STREAMS
    friend void ks_c_dispatch(struct svc_req * request, SVCXPRT *transport);
#endif

    KsServerBase(const KsServerBase &); // forbidden
    KsServerBase & operator = (const KsServerBase &); // forbidden

    void cleanup();

    PltPriorityQueue< PltPtrComparable<KsTimerEvent> > _timer_queue;

    int _send_buffer_size;
    int _receive_buffer_size;

    int serveRequests(const KsTime *pTimeout);
    
    static KsServerBase *the_server;

}; // class KsServerBase


// ---------------------------------------------------------------------------
// The base class of all ACPLT/KS service results...
//
class KsServiceResult {
public:
    virtual bool_t xdrEncode(XDR *xdr) = 0;

    KS_RESULT result;
}; // class KsServiceResult



// ---------------------------------------------------------------------------
// INLINE IMPLEMENTATION
//

inline bool
KsServerBase::servePendingEvents(KsTime timeout) 
{
    return servePendingEvents(&timeout);
} // KsServerBase::servePendingEvents

// ---------------------------------------------------------------------------

inline KsServerBase &
KsServerBase::getServerObject()
{
    PLT_PRECONDITION(the_server);
    return *the_server;
} // KsServerBase::getServerObject


#endif // PLT_SVRBASE_INCLUDED
/* End of ks/svrbase.h */

