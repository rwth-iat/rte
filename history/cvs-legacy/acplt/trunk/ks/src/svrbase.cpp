/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/src/svrbase.cpp,v 1.3 1997-03-17 19:58:17 martin Exp $ */
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

/////////////////////////////////////////////////////////////////////////////
//
// svrbase.cpp
//

#include <sys/errno.h>
#include <sys/time.h>

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "ks/svrbase.h"
#include "plt/log.h"


//////////////////////////////////////////////////////////////////////
// utilities

static size_t ks_dtsz;
static bool ks_dtsz_valid = false;
inline static size_t
ks_dtablesize()
{
    if (!ks_dtsz_valid) {
        ks_dtsz = FD_SETSIZE;
    }
    return ks_dtsz;
}

//////////////////////////////////////////////////////////////////////

inline void
KsTimerEvent::trigger()
{
    (_server.*method)(this);
}


/////////////////////////////////////////////////////////////////////////////
// There can be only one... KS server object. We need this pointer lateron
// when redirecting (de-)serializing requests comming from the RPC level to
// our server object.
//
KsServerBase *
KsServerBase::the_server = 0;


////////////////////////////////////////////////////////////////////////////
// Construction & destruction area. Watch for falling bits...
// Uhh, I've been hit.

KsServerBase::KsServerBase(const char *svr_name,
                           const u_long prot_version)
    : server_name(svr_name), 
      protocol_version(prot_version),
      shutdown_flag(false),
      _tcp_transport(0),
      send_buffer_size(16384),
      receive_buffer_size(16384)
{
    PLT_PRECONDITION( the_server == 0 );
    the_server    = this;
    
}

/////////////////////////////////////////////////////////////////////////////
// Now we're through. Clean up everything
//
KsServerBase::~KsServerBase()
{
    destroyTransports();
    destroyLurkingTransports();
    PLT_ASSERT(the_server == this);
    the_server = 0;
} 

/////////////////////////////////////////////////////////////////////////////
// Okay, okay. Now take a deep breath...and... fire up the whole mess! We
// can't do this in the constructor as derived classes need first to complete
// their initializations before we can create the transports, and everything
// else.
//
void KsServerBase::init()
{
    createTransports();
}


////////////////////////////////////////////////////////////////////////////
// Transport section. Watch for network cables strangling innocent
// workstations...
// A wise word of warning: when using transports, don't use svc_register
// again. Here is the ONLY place where it can be used safely. The reason
// for this is that someone at Sun once "shot a giant ram" -- both
// svc_register() as well as svc_unregister() aren't very useful as the
// latter one ALWAYS contacts the portmapper.
//

bool 
KsServerBase::createTransports()
{
    _tcp_transport = svctcp_create(RPC_ANYSOCK, 
                                   send_buffer_size, receive_buffer_size);
    if (! _tcp_transport) {
        PltLog::Error("KsServerBase: could not create transport");
        return false;
    }

    //
    // Now register the dispatcher that should be called whenever there
    // is a request for the KS program id and the correct version number.
    //
    if ( !svc_register(_tcp_transport, // nothing more than just a dummy
		                      //-- at least if you use the "0"
		                      // as the protocol parameter...
                       KS_RPC_PROGRAM_NUMBER,
                       KS_PROTOCOL_VERSION,
                       ks_c_dispatch,
                       0) ) {  // Do not contact the portmapper!
        svc_destroy(_tcp_transport);
        _tcp_transport = 0;
        PltLog::Error("KsServerBase: could not register");
        return false;
    }
    return true;
} 

////////////////////////////////////////////////////////////////////////////
// Clean up all transports used for the communication with clients.
//
void KsServerBase::destroyTransports()
{
    if ( _tcp_transport ) {
        svc_destroy(_tcp_transport); 
        _tcp_transport = 0;
    }
}

/////////////////////////////////////////////////////////////////////////////
// The RPC library is so brain-damaged, I wanna cry. If you want to close
// down cleanly, you have to destroy ALL transports currently in use. But
// you only know of the primary transport(s) you've once created. The other
// ones are the transports that are used for the communication with the
// particular clients and are hidden within the RPC layer. And they must be
// closed too.
//
void KsServerBase::destroyLurkingTransports()
{
    int  fd;
    size_t set_size = ks_dtablesize();
    bool lurking = false;

    for ( fd = 0; fd < set_size; fd++ ) {
        if ( FD_ISSET(fd, &svc_fdset) ) {
            //
            // First shoot the transport's socket...
            //
            close(fd);
            lurking = true;
        }
    }
    if ( lurking ) {
        //
        // Now kill the transports by trying to read from them...
        //
        svc_getreqset(&svc_fdset);
    }
}

////////////////////////////////////////////////////////////////////////////
// Now comes the dispatcher section. Here we redirect and bounce request
// forth and back until no-one knows where the request comes from and to
// whom it should be send, and who's responsible for all this mess. Hey,
// this is just like in real life...
//
////////////////////////////////////////////////////////////////////////////
// This is the wrapper that redirects incomming requests issued by the RPC
// layer up to the responsible server object.
//
extern "C" void 
ks_c_dispatch(struct svc_req * request, SVCXPRT *transport)
{
    PLT_PRECONDITION(KsServerBase::the_server);
    if ( request->rq_proc == 0 ) {
        //
        // This is just here for the compliance with ONC/RPC rules. If
        // someone pings us, we send back a void reply.
        //
        svc_sendreply(transport, (xdrproc_t) xdr_void, 0); // FIXME ??
    } else {
        XDR *xdr = KsServerBase::the_server->getXdrForTransport(transport);

        // get a ticket
        KsAvTicket *pTicket = KsAvTicket::xdrNew(xdr);
        if (!pTicket) {
            // no ticket ?? Use emergency ticket!
            pTicket = KsAvTicket::emergencyTicket();
        }
        PLT_ASSERT(pTicket); // we have at least an error ticket

        if ( pTicket->result()!= KS_ERR_OK ) {
            //
            // Ups. Something went wrong when we created the A/V ticket. So
            // just send the error reply...
            //
            KsServerBase::sendErrorReply(transport, 
                                         *pTicket, 
                                         pTicket->result());
        } else {
            //
            // We're now ready to serve the service...
            //
            KsServerBase::the_server->dispatch(request->rq_proc, 
                                               transport, 
                                               xdr,
                                               *pTicket);
        }
        // TODO destroy ticket????
    }
}

/////////////////////////////////////////////////////////////////////////////
// Here we can find the real dispatcher. It must be overwritten in subclasses
// of the abstract server base class in order to hook in the (abstract)
// service handling.
//
void KsServerBase::dispatch(u_long serviceId, 
                            SVCXPRT *transport,
                            XDR * xdrIn,
                            KsAvTicket &ticket)
{
    //
    // This is a fall back for the case when we step onto a serviceId
    // we've never heard of before. We then simply send back a "not
    // implemented" error reply.
    //
    sendErrorReply(transport, ticket, KS_ERR_NOTIMPLEMENTED);
}


/////////////////////////////////////////////////////////////////////////////
// Because objects can serialize and deserialize themselves into/from xdr
// streams, we often need to know the xdr stream identifier when
//   **DESERIALIZING**
// requests. Never, NEVER, NEVER use getXdrForTransport() when SERIALIZING
// replys or you're in deep trouble, pals.
//
// C part
extern "C" bool_t 
ks_c_get_xdr_for_transport(XDR *xdr, void *obj_ptr)
{
    *((XDR **) obj_ptr) = xdr;
    return 1;
} 

//////////////////////////////////////////////////////////////////////
// C++ part
XDR *
KsServerBase::getXdrForTransport(SVCXPRT *transport)
{
    XDR *xdr;

    svc_getargs(transport, ks_c_get_xdr_for_transport, &xdr);

    PLT_ASSERT(xdr->x_op == XDR_DECODE);

    return xdr;
}


/////////////////////////////////////////////////////////////////////////////
// The next function is a helper that sends back a complete KS reply to some
// KS client. In every case, the reply consists of the A/V information as
// well as of an error code. If there is no error, then the service reply
// data will follow.
//
typedef struct {
    KsAvTicket     *ticket;
    KsResult *result;
    KS_RESULT       result_state;
} KsXdrReplyInfo;


//////////////////////////////////////////////////////////////////////
// C part
//
extern "C" bool_t 
ks_c_send_reply(XDR *xdr, void *reply_info)
{
    KsXdrReplyInfo *info = (KsXdrReplyInfo *) reply_info;

    if ( !info->ticket->xdrEncode(xdr) ) {
        return FALSE;
    }
    if ( info->result == 0 ) {
        //
        // We've got stuck with an error result very early in the
        // decoding process. Thus there's no full result available and
        // we just send back the error code...
        //
        return xdr_enum(xdr, &(info->result_state));
    } else {
        //
        // We have a full blown result to send back...
        //
        return info->result->xdrEncode(xdr);
    }
} 

/////////////////////////////////////////////////////////////////////////////
// Send back an full blown service reply.
//
void 
KsServerBase::sendReply(SVCXPRT *transport, 
                        KsAvTicket &ticket,
                        KsResult &result)
{
    KsXdrReplyInfo answer;

    answer.ticket = &ticket;
    answer.result = &result;
    answer.result_state = KS_ERR_GENERIC; // should never be used
    svc_sendreply(transport, (xdrproc_t) ks_c_send_reply, (char*) &answer);
}

/////////////////////////////////////////////////////////////////////////////
// Send back an error reply to the KS client.
//
void KsServerBase::sendErrorReply(SVCXPRT *transport, KsAvTicket &ticket,
				  KS_RESULT result)
{
    KsXdrReplyInfo answer;

    answer.ticket = &ticket;
    answer.result = 0;
    answer.result_state = result;
    svc_sendreply(transport, (xdrproc_t) ks_c_send_reply, (char*) &answer);
}


////////////////////////////////////////////////////////////////////////////
// Event handling section.
// Here we deal with timer events and impatient KS clients requesting
// services from us...
//
////////////////////////////////////////////////////////////////////////////
// Try to stop the event loop as soon as possible. Due to the nature of
// ONC/RPC this can take some time as all incomming transports with waiting
// RPC requests are each served exactly once until we get back to the
// server main loop "run()".
//
void KsServerBase::stopServer()
{
    shutdown_flag = true;
}

//
// This is the main loop of a KS server. It waits for incomming RPC requests
// or timer events and dispatches them until the shutdown_flag indicates
// that we should do an easy escape.
//


//////////////////////////////////////////////////////////////////////
// Helper function. Get ready read fds with optional timeout
// Return number of ready fds or -1 on error
//
static int
getReadyFds(fd_set & fds, const KsTime * pto) 
{
    // copy timeout, bbsts
    KsTime timeout;
    KsTime *pTimeout;
    if (pto) {
        timeout = *pTimeout;
        pTimeout = &timeout;
    } else {
        pTimeout = 0;
    }

    size_t numfds = ks_dtablesize();
        
#if PLT_SYSTEM_HPUX
    int res = select(numfds, (int*) &fds,0,0, pTimeout); // TODO wasdas?
#else
    int res = select(numfds, &fds,0,0, pTimeout);
#endif

    if (res == -1) {
        // select reports an error
        if (errno == EINTR) {
            // interrupted by a signal
            // interpret as timeout
            FD_ZERO(&fds);
            res = 0;
        } else {
            // something unexpected happened
            PltLog::Error("Select failed unexpectedly");
        }
    }
    return res;
}        


//////////////////////////////////////////////////////////////////////
//
//  Check for pending events
//
bool
KsServerBase::hasPendingEvents() const
{
    fd_set read_fds = svc_fdset;
    KsTime zerotimeout(0,0);
    return 
        ( 
               !timer_queue.isEmpty 
            && timer_queue.peek()->remainingTime() == 0 
        )
        || getReadyFds(read_fds, &zerotimeout) > 0;
}




//////////////////////////////////////////////////////////////////////
//
// Serve RPC requests with timeout. Return -1 on error or the number
// of served fds
//
int
KsServerBase::serveRequests(const KsTime *pTimeout) {
    fd_set read_fds = svc_fdset;
    int res = getReadyFds(read_fds,pTimeout);
    if (res > 0) {
        //
        // Now as there are requests pending on some connections, try to
        // serve them. The RPC level is responsible for this. Eventually
        // the RPC level calls a wrapper which in turn redirects the
        // request handling back to our server object.
        // Each connection with waiting request(s) will be served exactly
        // once, so after a round through all input handles we'll come
        // back -- no matter how many requests are still waiting on the
        // connections just served.
        //
        svc_getreqset(&read_fds);
    } 
    return res;
}

//////////////////////////////////////////////////////////////////////

bool
KsServerBase::servePendingEvents(KsTime * pTimeout) {
    KsTimerEvent *pEvent = getNextTimerEvent();
    if (pEvent) {
        //
        // timerqueue nonempty
        //
        KsTime timeleft( pEvent->remainingTime() );
        if (timeleft) {
            // 
            // The next timer event is still not pending.
            //
            // Choose the shorter time from
            // external timeout and the time left until the
            // timer event occurs
            //
            if (pTimeout && *pTimeout < timeleft) {
                timeleft = *pTimeout;
            }
            int servedRequests = serveRequests(&timeleft);
            return servedRequests > 0;
        } else {
            //
            // pending timer event, execute it immediately
            // 
            (timer_queue.removeFirst())->trigger();
            return true;
        }
    } else {
        // 
        // timerqueue empty: use external timeout
        // 
        int servedRequests = serveRequests(pTimeout);
        return servedRequests > 0;
    }
}


//////////////////////////////////////////////////////////////////////

void
KsServerBase::run()
{    
    shutdown_flag = false;
    while (! shutdown_flag) {
        servePendingEvents(0); // no timeout -> wait forever
    }
}   

//////////////////////////////////////////////////////////////////////
//
// Add a timer event object to the timer event queue. If its time has come
// the timer event object will be activated by calling its trigger method.
//
bool KsServerBase::addTimerEvent(KsTimerEvent *event)
{
    return timer_queue.add(event);
} // KsServerBase::addTimerEvent

//
// Remove a timer event object -- at least if its in the timer event
// queue. In this case the method returns "true". Otherwise if the timer
// event isn't in the queue then you'll get a "false" result.
//
bool KsServerBase::removeTimerEvent(KsTimerEvent *event)
{
    return timer_queue.remove(event);
} // KsServerBase::removeTimerEvent



