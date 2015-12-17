/* -*-plt-c++-*- */
#ifndef KS_SVRBASE_INCLUDED
#define KS_SVRBASE_INCLUDED
/* $Header: /home/david/cvs/acplt/ks/include/ks/svrbase.h,v 1.33 2008-09-22 08:26:09 henning Exp $ */
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
    virtual void resetGoingDown() { _shutdown_flag = 0; }

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

    // address reuse
    bool getReuseAddr() { return _reuse_addr; }
    void setReuseAddr(bool reuse) { _reuse_addr = reuse; }

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
    bool _reuse_addr; // SO_REUSEADDR

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
        virtual ~KssAttentionXDRDispatcher() {}
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
    virtual ~KsServiceResult() {}

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

