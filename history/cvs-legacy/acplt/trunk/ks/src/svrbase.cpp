/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/src/svrbase.cpp,v 1.31 1998-09-17 12:02:24 harald Exp $ */
/*
 * Copyright (c) 1996, 1997, 1998
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
 * svrbase.cpp -- The basic communication and dispatching stuff common to
 *                all ACPLT/KS servers (and managers). It´s sometimes getting
 *                real dirty here as we´ve to hide the communication details
 *                from the server application programmer. Especially with
 *                the connection manager and other communication layers this
 *                leads to a bunch of #defines. But now -- enjoy =:)
 *
 * Written by Harald Albrecht & Martin Kneissl
 * <harald@plt.rwth-aachen.de> <martin@plt.rwth-aachen.de>
 */

#if PLT_SYSTEM_NT
//
// Take care of multi-shredded environments. See notes below in the
// function getReadyFds() for more information...
//
#include <errno.h>
#else
#include <errno.h>
#endif
#include <stdlib.h>
#include <string.h>

#if !PLT_SYSTEM_NT
#include <unistd.h>
#endif

#if PLT_SYSTEM_OPENVMS
#define bzero(x, y) memset(x, 0, y)
#endif

#if PLT_SYSTEM_SOLARIS
#include <netconfig.h>
#endif

#if PLT_USE_BUFFERED_STREAMS
#include "ks/xdrmemstream.h"
#endif

#include "ks/svrbase.h"
#include "plt/log.h"

#if PLT_DEBUG
#include <iostream.h>
#include <iomanip.h>
#endif


// ---------------------------------------------------------------------------
// As we don't know how expensive the call to FD_SETSIZE is, we're cashing the
// result here. On some systems, FD_SETSIZE boils down to a (fast!) kernel
// trap, on some others to a sloooow kernel trap. So we take our chances here.
// BTW -- with NT it boils down to a stupid constant: 64. Believe it or not:
// that is claimed to be "new technology" -- hah!
//
#if !PLT_USE_BUFFERED_STREAMS
static size_t ks_dtsz;
static bool ks_dtsz_valid = false;
inline static size_t
ks_dtablesize()
{
    if (!ks_dtsz_valid) {
        ks_dtsz = FD_SETSIZE;
    }
    return ks_dtsz;
} // ks_dtablesize
#endif

//////////////////////////////////////////////////////////////////////
//
// Return the next timer event that will trigger or 0, if there's none in
// the timer event queue.
//
inline const KsTimerEvent *
KsServerBase::peekNextTimerEvent() const
{
    if ( _timer_queue.isEmpty() ) {
        return 0;
    } else {
        return _timer_queue.peek();
    }
}



//////////////////////////////////////////////////////////////////////

inline void
KsTimerEvent::trigger()
{
}


#if PLT_USE_BUFFERED_STREAMS
// ---------------------------------------------------------------------------
// When working with buffered streams, we´re using a timer event to free up
// free fragments from time to time. This way, memory from large requests or
// replies can be reclaimed not only by the transports but also by the C++
// objects.
//
class KsGarbageTimerEvent : public KsTimerEvent {
public:
    KsGarbageTimerEvent(unsigned long interval);
    virtual void trigger();

protected:
    unsigned long _interval;
}; // class KsGarbageTimerEvent


KsGarbageTimerEvent::KsGarbageTimerEvent(unsigned long interval)
    : KsTimerEvent(KsTime::now(interval)),
      _interval(interval)
{
} // KsGarbageTimerEvent::KsGarbageTimerEvent


void KsGarbageTimerEvent::trigger()
{
    xdrmemstream_freegarbage();
    _trigger_at = KsTime::now(_interval);
    KsServerBase::getServerObject().addTimerEvent(this);
} // KsGarbageTimerEvent::trigger
#endif


// ---------------------------------------------------------------------------
// There can be only one... KS server object. We need this pointer lateron
// when redirecting (de-)serializing requests comming from the RPC level to
// our server object. And this pointer is also very handy in various places,
// so there´s even an accessor for it in the KsServerBase class.
//
KsServerBase *
KsServerBase::the_server = 0;


// ---------------------------------------------------------------------------
// Construction & destruction area. Watch for falling bits...
// Uhh, I've been hit.
// Me too, once again...
//
KsServerBase::KsServerBase()
    : _sock_port(KS_ANYPORT),
#if PLT_USE_BUFFERED_STREAMS
      _cnx_manager(0), // don't create a connection manager object yet as a
                       // server programmer might wish to subclass it and use
		       // this particular implementation.
#endif
      _tcp_transport(0),
      _shutdown_flag(0),
      _send_buffer_size(16384),
      _receive_buffer_size(16384)
{
    PLT_PRECONDITION( the_server == 0 );
    the_server = this;
    _is_ok = true;
#if PLT_USE_BUFFERED_STREAMS
    //
    // Set some parameters of the dynamic XDR memory streams...
    //
    xdrmemstream_controlusage(4096, // 4k fragments
	                      1024, // 4k*1024 = 4M maximum pool size
	                      0,    // 4k*0 = 0k minimum free pool size
	                      50);  // clean up 50% per "garbage collection"
#endif
} // KsServerBase::KsServerBase


// ---------------------------------------------------------------------------
// Now we're through. Clean up everything. Especially get rid of a possible
// connection manager.
//
KsServerBase::~KsServerBase()
{
    cleanup();

#if PLT_USE_BUFFERED_STREAMS
    if ( _cnx_manager ) {
	//
	// Get rid of the communication manager. We can´t do this in the
	// cleanup() method as this method is also called when stopping the
	// server. But a connection manager needs to be created only once
	// before starting the server and thus can only be destroyed when
	// the server object is destroyed too.
	//
#if !PLT_SYSTEM_OPENVMS
    	delete _cnx_manager; // TODO & FIXME!!!
#endif
	_cnx_manager = 0;
    }
#endif
    PLT_ASSERT(the_server == this);
    the_server = 0;
} // KsServerBase::~KsServerBase


// ---------------------------------------------------------------------------
// Cleanup such things that aren´t needed anymore after stopping the server.
// Don´t clean up things that might be needed the next time the server is
// started again (for instance, a connection manager).
//
void KsServerBase::cleanup()
{
    //
    // Destroy the TCP/IP transport, if there is one left...
    //
    if ( _tcp_transport ) {
#if !PLT_USE_BUFFERED_STREAMS
        svc_destroy(_tcp_transport);
#else
	_cnx_manager->removeConnection(*_tcp_transport);
	_tcp_transport->shutdown();
    	delete _tcp_transport;
#endif
        _tcp_transport = 0;
    }
    //
    // Remove remaining timer events, there's no need for them now.
    //
    while (!_timer_queue.isEmpty()) {
        KsTimerEvent *pevent = _timer_queue.removeFirst();
        delete pevent;
    }
} // KsServerBase::cleanup


#if PLT_USE_XTI
// ---------------------------------------------------------------------------
//
//
PltString KsServerBase::getNetworkTransportDevice(const char *protocol)
{
    void             *handle;
    struct netconfig *nc;
    PltString         device("/dev/", protocol);
    
    handle = setnetconfig();
    while ( (nc = getnetconfig(handle)) != 0 ) {
    	if ( strcmp(nc->nc_proto, protocol) == 0 ) {
	    device = PltString(nc->nc_device);
	    break;
	}
    }
    endnetconfig(handle);
    return device;
} // KsServerBase::getNetworkTransportDevice
#endif


// ---------------------------------------------------------------------------
// Now comes the dispatcher section. Here we redirect and bounce request
// forth and back until no-one knows where the request comes from and to
// whom it should be send, and who's responsible for all this mess. Hey,
// this is just like in real life...
//

// ---------------------------------------------------------------------------
// This is the wrapper that redirects incomming requests issued by the RPC
// layer up to the responsible server object. As mentioned in the header of
// this source file, this wrapper is communication layer-dependant. But thanks
// to the transport interface generalization this is rather painless to
// achieve.
//
#if !PLT_USE_BUFFERED_STREAMS
extern "C" void 
ks_c_dispatch(struct svc_req * request, SVCXPRT *transport)
{
    PLT_PRECONDITION(KsServerBase::the_server);
    
    KsServerBase::the_server->_transport.setNewRequest(transport, request);
    KsServerBase::the_server->dispatchTransport(KsServerBase::the_server->_transport);
} // ks_c_dispatch
#endif


// ---------------------------------------------------------------------------
//
void KsServerBase::dispatchTransport(KssTransport &transp)
{

    PLT_DMSG_ADD("Req: " << hex << transp.getServiceId() << dec);
    PLT_DMSG_END;
    if ( transp.getServiceId() == 0 ) {
        //
        // This is just here for the compliance with ONC/RPC rules. If
        // someone pings us, we send back a void reply.
        //
        PLT_DMSG_ADD("Pinged...");
        PLT_DMSG_END;
	transp.sendPingReply();
    } else {
        //
        // Let´s see: it´s a real request... at least it seems to be so.
        //
	XDR *xdr = transp.getDeserializingXdr();
    	//
        // Now get a ticket -- nope, not a parking ticket, but an A/V ticket
	// instead, which can hold authentification/verification information.
	// If this fails (for whatever reason, for it is memory shortage or
	// an invalid a/v header) take the emergency ticket, which has been
	// statically allocated and is therefore always available.
	//
        KsAvTicket *pTicket = KsAvTicket::xdrNew(xdr);
        if ( !pTicket ) {
            pTicket = KsAvTicket::emergencyTicket();
        }
        PLT_ASSERT(pTicket); // we have at least an error ticket

        if ( pTicket->result() != KS_ERR_OK ) {
            //
            // Ups. Something went wrong when we created the A/V ticket. So
            // just send back the error reply...
            //
	    transp.finishRequestDeserialization(*pTicket, false);
	    transp.sendErrorReply(*pTicket, pTicket->result());
        } else {
            //
            // Save the socket address of the sender into the ticket. The
	    // ticket then might decide to drop the connection...
            //
            struct sockaddr *sa; // TODO
            int namelen;
	    namelen = sizeof(sockaddr);
    	    sa = (struct sockaddr *) transp.getPeerAddress(namelen);
            bool accept = pTicket->setSenderAddress(sa, namelen);
            if ( !accept ) {
                transp.personaNonGrata();
		return;
            }

#if PLT_DEBUG
            char *from =  inet_ntoa(pTicket->getSenderInAddr());
            PLT_DMSG_ADD("from: " << from);
            PLT_DMSG_END;
#endif
            //
            // We're now ready to serve the service request...
            //
            KsServerBase::the_server->dispatch(
	    	transp.getServiceId(), 
                transp,
		xdr, *pTicket);
        }
        if ( pTicket && (pTicket != KsAvTicket::emergencyTicket()) ) {
            delete pTicket;
        }
    } // if (pinged) else
} // KsServerBase::dispatchTransport


// ---------------------------------------------------------------------------
// Here comes the real service request dispatcher. It may be extended in 
// derived classes (like KsManager). If you extend the dispatcher, then you
// have to do it according to the following pattern:
//   case YOUR_SERVICE_ID:
//       read in the service parameters
//       finish request deserialization
//       do whatever you need to do to server the request
//       send back the (error) reply
//       ...and that´s it!
//
void
KsServerBase::dispatch(u_long serviceId,
                       KssTransport &transport,
                       XDR *xdrIn,
                       KsAvTicket &ticket)
{
    bool decodedOk = true;
    switch ( serviceId ) {
        
    case KS_GETVAR:
        {
            KsGetVarParams params(xdrIn, decodedOk);
	    transport.finishRequestDeserialization(ticket, decodedOk);
            if ( decodedOk ) {
                // execute service function
                KsGetVarResult result(params.identifiers.size());
                getVar(ticket, params, result);
                // send back result
                transport.sendReply(ticket, result);
            } else {
                // not properly decoded
                transport.sendErrorReply(ticket, KS_ERR_GENERIC);
            }
        }
        break;

    case KS_SETVAR:
        {
            KsSetVarParams params(xdrIn, decodedOk);
	    transport.finishRequestDeserialization(ticket, decodedOk);
            if ( decodedOk ) {
                // execute service function
                KsSetVarResult result(params.items.size());
                setVar(ticket, params, result);
                // send back result
                transport.sendReply(ticket, result);
            } else {
                // not properly decoded
                transport.sendErrorReply(ticket, KS_ERR_GENERIC);
            }
        }
        break;

    case KS_GETPP:
        {
            KsGetPPParams params(xdrIn, decodedOk);
	    transport.finishRequestDeserialization(ticket, decodedOk);
            if ( decodedOk ) {
                // execute service function
                KsGetPPResult result;
                getPP(ticket, params, result);
                // send back result
                transport.sendReply(ticket, result);
            } else {
                // not properly decoded
                transport.sendErrorReply(ticket, KS_ERR_GENERIC);
            }
        }
        break;

    case KS_EXGDATA:
        {
            KsExgDataParams params(xdrIn, decodedOk);
	    transport.finishRequestDeserialization(ticket, decodedOk);
            if ( decodedOk ) {
                // execute service function
                KsExgDataResult result(params.set_vars.size(),
                                       params.get_vars.size());
                if(result.results.size() == params.set_vars.size()
                   && result.items.size() == params.get_vars.size()) 
                { // allocation ok
                    exgData(ticket, params, result);
                    // send back result
                    transport.sendReply(ticket, result);
                } else {
                    // allocation failed
                    transport.sendErrorReply(ticket, KS_ERR_GENERIC);
                }
            } else {
                // not properly decoded
                transport.sendErrorReply(ticket, KS_ERR_GENERIC);
            }
        }
        break;

    default:
        // 
        // This is an unknown service.
        //
	transport.finishRequestDeserialization(ticket, false);
        transport.sendErrorReply(ticket, KS_ERR_NOTIMPLEMENTED);
    }
} // KsServerBase::dispatch


// ---------------------------------------------------------------------------
// The following ACPLT/KS base services are not implemented by default. A
// derived class is responsible for providing suitable implementations.
//
void 
KsServerBase::getVar(KsAvTicket &,
                     const KsGetVarParams &,
                     KsGetVarResult & result) 
{
    result.result = KS_ERR_NOTIMPLEMENTED;
} // KsServerBase::getVar


void
KsServerBase::setVar(KsAvTicket &,
                     const KsSetVarParams &,
                     KsSetVarResult & result) 
{
    result.result = KS_ERR_NOTIMPLEMENTED;
} // KsServerBase::setVar


void 
KsServerBase::getPP(KsAvTicket &,
                    const KsGetPPParams &,
                    KsGetPPResult & result)
{
    result.result = KS_ERR_NOTIMPLEMENTED;
} // KsServerBase::getPP


void 
KsServerBase::exgData(KsAvTicket &,
                      const KsExgDataParams &,
                      KsExgDataResult & result) 
{
    result.result = KS_ERR_NOTIMPLEMENTED;
} // KsServerBase::exgData


// ---------------------------------------------------------------------------
// Event handling section (timeouts & i/o).
// Here we deal with timer events and impatient KS clients requesting
// services from us...
//


// ---------------------------------------------------------------------------
// Helper function. Get ready readable fds with optional timeout. When using
// a connection manager we´re watching for writeable fds too.
// Returns number of ready fds or -1 on error, and the fdsets are updated to
// reflect file descriptors willing to do i/o. Note that this helper function
// eventually boils down to a select() with an optional PltTime timeout.
//
static int
#if !PLT_USE_BUFFERED_STREAMS
getReadyFds(fd_set &read_fds, const KsTime *pto)
#else
getReadyFds(fd_set &read_fds, fd_set &write_fds, size_t numfds, 
            const KsTime *pto)
#endif
{
#if PLT_SYSTEM_NT
    //
    // Take care of a possible multi-shredded environment like NT...
    // Here we must take care of the fact that in MT environments, there
    // is no global errno, as it makes no sense. So we locally emulate
    // it ourselves.
    //
    int errno;
#endif

    // copy timeout, bbsts
    KsTime timeout;
    KsTime *pTimeout;
    if (pto) {
        timeout = *pto;
        pTimeout = &timeout;
    } else {
        pTimeout = 0;
    }

#if !PLT_USE_BUFFERED_STREAMS
#define WRITE_FDS 0
    size_t numfds = ks_dtablesize();
#else
#define WRITE_FDS &write_fds
#endif

#if PLT_SYSTEM_HPUX
    int res = select(numfds,
	             (int *) &read_fds, (int *) WRITE_FDS, 0,
	             pTimeout);
#else
    int res = select(numfds, &read_fds, WRITE_FDS, 0, pTimeout);
#endif

    if ( res == -1 ) {
        // select reports an error
#if PLT_SYSTEM_NT
        errno = WSAGetLastError();
#endif
        PLT_DMSG_ADD("select returned -1: " << strerror(errno));
        PLT_DMSG_END;
        if (errno == EINTR) {
            // interrupted by a signal
            // interpret as timeout
            FD_ZERO(&read_fds);
#if PLT_USE_BUFFERED_STREAMS
	    FD_ZERO(&write_fds);
#endif
            res = 0;
        } else {
            // something unexpected happened
            PltLog::Error("Select failed unexpectedly");
        }
    }
    return res;
} // getReadyFds


//////////////////////////////////////////////////////////////////////
//
//  Check for pending events
//
bool
KsServerBase::hasPendingEvents() const
{
    //TODO cnx mgr...
#if !PLT_USE_BUFFERED_STREAMS
    fd_set read_fds = svc_fdset;
#else
    fd_set read_fds, write_fds;
    size_t numfds = _cnx_manager->getFdSets(read_fds, write_fds);
#endif
    KsTime zerotimeout;
    return 
        (   // check for a timer event...
                !_timer_queue.isEmpty()
             && !_timer_queue.peek()->remainingTime().isZero()
        )
#if PLT_USE_BUFFERED_STREAMS
	||  // check to see whether there is a connection timeout
	(   // pending...
	         _cnx_manager->mayHaveTimeout()
	     && !_cnx_manager->getEarliestTimeoutSpan().isZero()
        )
#endif
#if !PLT_USE_BUFFERED_STREAMS
        || getReadyFds(read_fds, &zerotimeout) > 0;
#else
        || getReadyFds(read_fds, write_fds, numfds, &zerotimeout) > 0;
#endif
} // KsServerBase::hasPendingEvents


//////////////////////////////////////////////////////////////////////
// Serve RPC requests with timeout. Return -1 on error or the number
// of served fds.
//
int
KsServerBase::serveRequests(const KsTime *pTimeout)
{
#if !PLT_USE_BUFFERED_STREAMS
    fd_set read_fds = svc_fdset;
    int res = getReadyFds(read_fds, pTimeout);
#else
    KsTime cnx_timeout;
    bool   has_cnx_timeout = false;
    fd_set read_fds, write_fds;
    
    if ( _cnx_manager->mayHaveTimeout() ) {
	//
	// There is a timeout in the connection manager´s queue, so let´s
	// check when this timeout will happen. If it would happen earlier
	// than a possible other timeout, then we´ll take the timeout from
	// the connection manager.
	//
	cnx_timeout = _cnx_manager->getEarliestTimeoutSpan();
	if ( pTimeout ) {
	    if ( cnx_timeout < *pTimeout ) {
		pTimeout = &cnx_timeout;
		has_cnx_timeout = true;
	    }
	} else {
	    pTimeout = &cnx_timeout;
	    has_cnx_timeout = true;
	}
    }
    
    size_t numfds = _cnx_manager->getFdSets(read_fds, write_fds);
    int res = getReadyFds(read_fds, write_fds, numfds, pTimeout);
#endif
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

        // When you write to a pipe or a socket and there is no listener
        // any more, you will get a SIGPIPE, which by default terminates
        // the application. There are two problems here: The code of the
        // underlying system might depend on delivery of SIGPIPE and
        // there are multiple interfaces for setting signal handlers.

#ifdef KS_IGNORE_SIGPIPE
        // TODO: This is a temporary fix. We leave it
        // to the user what to do providing a hook.
        signal(SIGPIPE, SIG_IGN);
#endif

#if !PLT_USE_BUFFERED_STREAMS
        svc_getreqset(&read_fds);
#else
	int serviceables = _cnx_manager->processConnections(read_fds,
		                                            write_fds);
	while ( serviceables-- ) {
	    KssConnection *con = _cnx_manager->getNextServiceableConnection();
	    if ( !con ) {
		break;
	    }
	    dispatchTransport(* (KssXDRConnection *) con);
    	    _cnx_manager->reactivateConnection(*con);
	}
#endif
    } else {
#if PLT_USE_BUFFERED_STREAMS
	if ( has_cnx_timeout ) {
	    //
	    // We had a connection timeout. So let the connection manager do
	    // its dirty duty.
	    //
	    _cnx_manager->processTimeout();
	}
#endif
    }
    return res;
} // KsServerBase::serveRequests


//////////////////////////////////////////////////////////////////////
// Handles pending events or waits at most until timeout. In this
// case it returns false, otherwise -- when events have been served --
// it returns true.
//
bool
KsServerBase::servePendingEvents(KsTime * pTimeout)
{
    const KsTimerEvent *pEvent = peekNextTimerEvent();
    if ( pEvent ) {
        //
        // The timerqueue is not empty, so peek at the first entry
	// which is the event that will trigger soon...
        //
        KsTime timeleft( pEvent->remainingTime() );
        if ( !timeleft.isZero() ) {
            // 
            // The next timer event is still not pending.
            //
            // Choose the shorter time from
            // external timeout and the time left until the
            // timer event occurs
            //
            if ( pTimeout && *pTimeout < timeleft ) {
                timeleft = *pTimeout;
            }
            int servedRequests = serveRequests(&timeleft);
            return servedRequests > 0;
        } else {
            //
            // pending timer event, execute it immediately
            // 
            removeNextTimerEvent()->trigger();
            return true;
        }
    } else {
        // 
        // The timer queue is empty: use external timeout
        // 
        int servedRequests = serveRequests(pTimeout);
        return servedRequests > 0;
    }
} // KsServerBase::servePendingEvents


//////////////////////////////////////////////////////////////////////
// Start the Server. This method will be extended by derived classes.
// Here in this class we create the transports. Note that _sock_port
// should have already been initialized by now. Another note about
// the _is_ok flag. To allow the restart of a server without explicit
// reset, we don't check here the flag before starting. But _is_ok
// will indicate the outcome of the startServer() method.
// 
void
KsServerBase::startServer()
{
#if PLT_USE_BUFFERED_STREAMS
    //
    // If the user didn't create his very own connection manager object
    // then we'll supply a default one. In case this can't be done or
    // the user's manager isn't okay, then bail out with an error.
    //
    if ( !_cnx_manager ) {
    	_cnx_manager = new KssConnectionManager();
    }
    if ( !_cnx_manager && !_cnx_manager->isOk() ) {
    	_is_ok = false;
	return;
    }
#endif
    //
    // Create transport: because the caller can specify the port to which we
    // should bind, this can be sometimes a lot of work... especially if
    // we're sitting on top of a TLI system. Urgs.
    //
#if !PLT_USE_XTI
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
#else
    int sock = t_open((const char *) getNetworkTransportDevice("tcp"),
                      O_RDWR, (struct t_info *) 0);
#endif
    if ( sock >= 0 ) {
        struct sockaddr_in my_addr;

        memset(&my_addr, 0, sizeof(my_addr));
        my_addr.sin_family      = AF_INET;
        my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        my_addr.sin_port        = htons((u_short) _sock_port);

#if PLT_USE_XTI
        struct t_bind req;

        req.addr.maxlen = sizeof(my_addr);
        req.addr.len    = sizeof(my_addr);
        req.addr.buf    = (char *) &my_addr;
        req.qlen        = 5;

        if ( t_bind(sock, &req, (struct t_bind *) 0) < 0 ) {
#else
        if ( bind(sock, (struct sockaddr *) &my_addr, sizeof(my_addr)) < 0 ) {
#endif
            // Failed.
#if PLT_SYSTEM_NT
            closesocket(sock);
#elif PLT_USE_XTI
            t_close(sock);
#else
            close(sock);
#endif
            sock = -1;
            PltLog::Error("KsServerBase::startServer(): could not bind the TCP socket.");
        } else {
#if !PLT_USE_BUFFERED_STREAMS
            _tcp_transport = svctcp_create(sock,
                                           _send_buffer_size,
                                           _receive_buffer_size);
#else
    	    _tcp_transport = new KssListenTCPXDRConnection(sock, 60/* secs */);
#endif
        }
    }

    if ( _tcp_transport ) {
#if !PLT_USE_BUFFERED_STREAMS
        //
        // Now register the dispatcher that should be called whenever there
        // is a request for the KS program id and the correct version number.
        //
        // A wise word of warning: when using transports, be extremely care-
        // ful with what you do! You should never allow svc_register to contact
        // the portmapper -- better do it yourself. And never (NEVER!!!) use
        // svc_unregister() as this one would deregister EVERYTHING with the
        // same program and protocol version number, regardless of the transport
        // used (udp, tcp). With TI ONC/RPC you must register every transport
        // you create, but once again, never deregister it, if at all possible.
        //
        if ( svc_register(_tcp_transport, 
                          KS_RPC_PROGRAM_NUMBER,
                          KS_PROTOCOL_VERSION,
                          ks_c_dispatch,
                          0) ) {  // Do not contact the portmapper!
            _is_ok = true;
        } else {
            svc_destroy(_tcp_transport);
            _tcp_transport = 0;
            PltLog::Error("KsServerBase::startServer(): could not register TCP service.");
            _is_ok = false;
        }
#else
    	if ( _cnx_manager->addConnection(*_tcp_transport) ) {
	    _is_ok = true;
	} else {
	    _tcp_transport->shutdown();
	    delete _tcp_transport;
            _tcp_transport = 0;
            PltLog::Error("KsServerBase::startServer(): could not add TCP transport.");
            _is_ok = false;
	}
	//
	// Register a timer event which will fire from time to time and triggers
	// fragment garbage collection for the dynamic XDR memory streams.
	//
	KsGarbageTimerEvent *pGarbageEvent = new KsGarbageTimerEvent(10);
	if ( !pGarbageEvent ) {
            PltLog::Error("KsServerBase::startServer(): could not create garbage timer event.");
    	    _is_ok = false;
	}
	if ( !addTimerEvent(pGarbageEvent) ) {
            PltLog::Error("KsServerBase::startServer(): could not register garbage timer event.");
    	    _is_ok = false;
	}
#endif
    } else {
        PltLog::Error("KsServerBase::startServer(): could not create TCP transport.");
        _is_ok = false;
    }
} // KsServerBase::startServer

//////////////////////////////////////////////////////////////////////
// Try to stop the event loop as soon as possible. Due to the nature of
// ONC/RPC this can take some time as all incomming transports with waiting
// RPC requests are each served exactly once until we get back to the
// server main loop "run()".
//
void KsServerBase::downServer()
{
    _shutdown_flag = 1;
}

//////////////////////////////////////////////////////////////////////
// Finally shut down the server. Here this isn't of much woe, but
// derived classes needs this sucker, so they can deregister them, or
// do other strange things... Well, at least we shoot our transport
// and pending events, too.
//
void KsServerBase::stopServer()
{
#if PLT_USE_BUFFERED_STREAMS
    if ( _cnx_manager ) {
	if ( !_cnx_manager->shutdownConnections(15) ) { // FIXME
	    PltLog::Info("KsServerBase::stopServer(): could not flush all sending XDR streams.");
	}
    }
#endif
    cleanup();
}

//////////////////////////////////////////////////////////////////////
// This is the main loop of a KS server. It waits for incomming RPC
// requests or timer events and dispatches them until the shutdown
// flag indicates that we should do an easy escape.
// Unfortunately, NT looses (once again): as it has no signal concept,
// you can't wake up a sleeping thread after you've signalled it to
// shut down... so what? New Technology...? ridiculous!
// Here we can see that NT is based on VMS: same problem for VMS...
//
void
KsServerBase::run()
{
#if PLT_SYSTEM_NT || PLT_SYSTEM_OPENVMS
    KsTime aShortTime(2);
#endif

    _shutdown_flag = 0;
    while (!isGoingDown()) {
#if ! (PLT_SYSTEM_NT || PLT_SYSTEM_OPENVMS)
        servePendingEvents(0); // no timeout -> wait forever
#else
        servePendingEvents(&aShortTime);
#endif
    }
}

//////////////////////////////////////////////////////////////////////
//
// Add a timer event object to the timer event queue. If its time has come
// the timer event object will be activated by calling its trigger method.
//
bool 
KsServerBase::addTimerEvent(KsTimerEvent *event)
{
    PLT_PRECONDITION(event);
    return _timer_queue.add(event);
}

//
// Remove a timer event object -- at least if its in the timer event
// queue. In this case the method returns "true". Otherwise if the timer
// event isn't in the queue then you'll get a "false" result.
//
bool 
KsServerBase::removeTimerEvent(KsTimerEvent *event)
{
    return _timer_queue.remove(event);
} 

//
//  Remove the next timer event from the queue
//
KsTimerEvent *
KsServerBase::removeNextTimerEvent()
{
    return _timer_queue.removeFirst();
} 

/* End of svrbase.cpp */
