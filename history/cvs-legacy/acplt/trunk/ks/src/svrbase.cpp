/* -*-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/src/svrbase.cpp,v 1.46 2004-07-20 11:44:42 harald Exp $ */
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
 * svrbase.cpp -- The basic communication and dispatching stuff common to
 *                all ACPLT/KS servers (and managers). It's sometimes getting
 *                really dirty here as we've to hide the communication details
 *                from the server application programmer. Especially with
 *                the connection manager and other communication layers this
 *                leads to a bunch of #defines. But now -- enjoy =:)
 *
 * Written by Harald Albrecht & Martin Kneissl
 * <harald@plt.rwth-aachen.de> <martin@plt.rwth-aachen.de>
 */

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#if !PLT_SYSTEM_NT
#include <unistd.h>
#endif

#if PLT_SYSTEM_SOLARIS
#include <netconfig.h>
#endif

#include "ks/connection.h"
#include "ks/stdconnectionmgr.h"
#include "ks/xdrmemstream.h"

#include "ks/svrbase.h"
#include "plt/log.h"

#if PLT_DEBUG
#include <iostream.h>
#include <iomanip.h>
#endif


// ---------------------------------------------------------------------------
//
inline void
KsTimerEvent::trigger()
{
}


// ---------------------------------------------------------------------------
// This class implements the KssConnectionAttentionInterface and relays all
// calls for attention to the transport dispatcher of the current ACPLT/KS
// server object.
//
bool KsServerBase::KssAttentionXDRDispatcher::attention(KsConnection &conn)
{
    KsServerBase::getServerObject().dispatchTransport((KssXDRConnection &)conn);
    return true;
} // KsServerBase::KssAttentionXDRDispatcher::attention


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
      _reuse_addr(false), // per default don't reuse socket/port addresses
      _tcp_transport(0),
      _shutdown_flag(0),
      _send_buffer_size(16384),
      _receive_buffer_size(16384)
{
    PLT_PRECONDITION( the_server == 0 );
    the_server = this;
    _is_ok = true;
    //
    // Set some parameters of the dynamic XDR memory streams...
    //
    xdrmemstream_controlusage(
	4096, // 4k fragments
	PLT_POOL_BLOCK_COUNT, // default: max 2048 fragments
	0,    // 4k*0 = 0k minimum free pool size
	50);  // clean up 50% per "garbage collection"
} // KsServerBase::KsServerBase


// ---------------------------------------------------------------------------
// Now we're through. Clean up everything.
//
KsServerBase::~KsServerBase()
{
    cleanup();

    // TODO: when to destroy the Connection Manager? It might still be used
    // by a KS client object in the same address space...!

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
	KsConnectionManager::getConnectionManagerObject()->
	    removeConnection(*_tcp_transport);
	_tcp_transport->shutdown();
    	delete _tcp_transport;
        _tcp_transport = 0;
    }

    // TODO: tell the connection manager to cleanup its timer event queue

} // KsServerBase::cleanup


// ---------------------------------------------------------------------------
// Retrieve the A/V ticket from the XDR stream. In case this fails, it is
// okay to return a 0 pointer, because the dispatcher part will then use an
// emergency ticket to send back at least an error reply.
//
KsAvTicket*
KsServerBase::getTicket(XDR* xdr)
{
    return KsAvTicket::xdrNew(xdr);
} // KsServerBase::getTicket


// ---------------------------------------------------------------------------
// Now comes the dispatcher section. Here we redirect and bounce request
// forth and back until no-one knows where the request comes from and to
// whom it should be send, and who's responsible for all this mess. Hey,
// this is just like in real life...
//



// ---------------------------------------------------------------------------
// The is the "middle level" part of dispatching requests. This method is
// called for instance from Sun's ONC/RPC dispatch mechanism through a
// C wrapper function. This method is responsible for answering RPC pings,
// deserializing the A/V ticket and finally dispatching to the ACPLT/KS
// high-level dispatchers.
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
        //KsAvTicket *pTicket = KsAvTicket::xdrNew(xdr);
	KsAvTicket *pTicket = getTicket(xdr);
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
#if !PLT_SERVER_TRUNC_ONLY

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
#if 1
        {
	    KsGetPPParams params(xdrIn, decodedOk);
	    transport.finishRequestDeserialization(ticket, decodedOk);
            if ( decodedOk ) {
                // execute service function
                KsGetPPResult result;
		KsGetEPParams getEPParams;
		KsGetEPResult getEPResult;

		getEPParams.path = params.path;
		getEPParams.type_mask = params.type_mask;
		getEPParams.name_mask = params.name_mask;
		getEPParams.scope_flags = KS_EPF_CHILDREN;

                getEP(ticket, getEPParams, getEPResult);

                result.result = getEPResult.result;
		if ( result.result == KS_ERR_OK ) {

		    KsEngPropsHandle hep;

		    while ( !getEPResult.items.isEmpty() ) {
			hep = getEPResult.items.removeFirst();
			if ( hep ) {
			    KsEngPropsV1 *epv1 = 0;
			    switch ( hep->xdrTypeCode() ) {
			    case KS_OT_DOMAIN:
				epv1 = new KsDomainEngPropsV1(
				    hep->identifier,
				    hep->creation_time,
				    hep->comment,
				    hep->access_mode
				    );
				break;
			    case KS_OT_VARIABLE:
				epv1 = new KsVarEngPropsV1(
				    hep->identifier,
				    hep->creation_time,
				    hep->comment,
				    hep->access_mode,
				    ((KsVarEngProps *)hep.getPtr())->tech_unit,
				    ((KsVarEngProps *)hep.getPtr())->type
				    );
				break;
#if PLT_SOURCE_V1PLUS_BC
			    case KS_OT_LINK:
				epv1 = new KsLinkEngPropsV1(
				    hep->identifier,
				    hep->creation_time,
				    hep->comment,
				    hep->access_mode,
				    ((KsLinkEngProps *)hep.getPtr())->type,
				    ((KsLinkEngProps *)hep.getPtr())->opposite_role_identifier,
				    ((KsLinkEngProps *)hep.getPtr())->association_identifier
				    );
				break;
			    case KS_OT_HISTORY:
				epv1 = new KsHistoryEngPropsV1(
				    hep->identifier,
				    hep->creation_time,
				    hep->comment,
				    hep->access_mode,
				    ((KsHistoryEngProps *)hep.getPtr())->type,
				    ((KsHistoryEngProps *)hep.getPtr())->default_interpolation,
				    ((KsHistoryEngProps *)hep.getPtr())->supported_interpolations,
				    ((KsHistoryEngProps *)hep.getPtr())->type_identifier
				    );
				break;
#endif
			    default:
				continue;
			    }
			    if ( !epv1 ||
				 !result.items.addLast(KsEngPropsV1Handle(epv1,
									  KsOsNew)) ) {
				result.result = KS_ERR_GENERIC;
				break;
			    }
			}
		    }

		}

                // send back result
                transport.sendReply(ticket, result);
            } else {
                // not properly decoded
                transport.sendErrorReply(ticket, KS_ERR_GENERIC);
            }
        }
#else
	transport.sendErrorReply(ticket, KS_ERR_NOTIMPLEMENTED);
#endif
	break;

    case KS_GETEP:
        {
            KsGetEPParams params(xdrIn, decodedOk);
	    transport.finishRequestDeserialization(ticket, decodedOk);
            if ( decodedOk ) {
                // execute service function
                KsGetEPResult result;
                getEP(ticket, params, result);
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

#endif
    default:
        // 
        // This is an unknown service.
        //
	transport.finishRequestDeserialization(ticket, false);
        transport.sendErrorReply(ticket, KS_ERR_NOTIMPLEMENTED);
    }
} // KsServerBase::dispatch


#if !PLT_SERVER_TRUNC_ONLY
// ---------------------------------------------------------------------------
// The following ACPLT/KS base services are not implemented by default. A
// derived class is responsible for providing suitable implementations. In
// case your server is written in C instead of C++, you can save quite some
// code space by compiling it using -DPLT_SERVER_TRUNC_ONLY=1. This prevents
// the linker from sucking in all the service parameters and all the stuff
// that depends on it.
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
KsServerBase::getEP(KsAvTicket &,
                    const KsGetEPParams &,
                    KsGetEPResult & result)
{
    result.result = KS_ERR_NOTIMPLEMENTED;
} // KsServerBase::getEP


void 
KsServerBase::exgData(KsAvTicket &,
                      const KsExgDataParams &,
                      KsExgDataResult & result) 
{
    result.result = KS_ERR_NOTIMPLEMENTED;
} // KsServerBase::exgData
#endif


// ---------------------------------------------------------------------------
// Check for pending events. This method is retained here for backwards
// compatibility, but now relies on the current connection manager object
// doing the real work.
//
bool
KsServerBase::hasPendingEvents() const
{
    return KsConnectionManager::getConnectionManagerObject()->
	hasPendingEvents();
} // KsServerBase::hasPendingEvents



// ---------------------------------------------------------------------------
// Start the Server. This method will be extended by derived classes. Here in
// this class we create the transports. Note that _sock_port should have
// already been initialized by now. Another note about the _is_ok flag. To
// allow the restart of a server without explicit reset, we don't check here
// the flag before starting. But _is_ok will indicate the outcome of the
// startServer() method.
// 
void
KsServerBase::startServer()
{
    //
    // See note above about the _is_ok flag: start with a clean state and
    // let others indicate problems...
    //
    _is_ok = true;
    //
    // If the user didn't create his very own connection manager object
    // then we'll supply a default one. In case this can't be done or
    // the user's manager isn't okay, then bail out with an error.
    //
    if ( !KsConnectionManager::getConnectionManagerObject() ) {
    	new KsStdConnectionManager();
    }
    if ( !KsConnectionManager::getConnectionManagerObject()
         || !KsConnectionManager::getConnectionManagerObject()->isOk() ) {
    	_is_ok = false;
	return;
    }

    //
    // Create transport: because the caller can specify the port to which we
    // should bind, this can be sometimes a lot of work... especially if
    // we're sitting on top of a TLI system. Urgs.
    // Note: if the programmer wants to use its very own transports when the
    // connection manager is in use, she/he can just create it and let the
    // _tcp_transport member variable point to it. In this case we won't
    // create a socket and a suitable transport here.
    //
    if ( !_tcp_transport ) {
	//
	// Okay, let's create a socket and the corresponding transport only
	// if the server writer hasn't done so.
	//
	int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if ( sock >= 0 ) {
            //
            // Allow for reuse of IP address to cure problems with Winblows-
            // based clients which aren't able to properly shut down their
            // TCP/IP connections...
            //
            if ( _reuse_addr ) {
                PltLog::Info("KsServerBase::startServer(): "
                             "enabling address reuse for TCP socket.");
                int flagOn = 1;
                if ( setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, 
#if PLT_SYSTEM_NT || PLT_SYSTEM_SOLARIS
                                 (char *) // signature wants generic char pointer...
#endif
                                 &flagOn,
                                 sizeof(flagOn)) ) {
                    PltLog::Warning("KsServerBase::startServer(): "
                                    "Can not enable IP address reuse for TCP socket.");
                }
            }

            //
            // Next, bind the socket...
            //
	    struct sockaddr_in my_addr;

	    memset(&my_addr, 0, sizeof(my_addr));
	    my_addr.sin_family      = AF_INET;
	    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	    my_addr.sin_port        = htons((u_short) _sock_port);

	    if ( bind(sock, (struct sockaddr *) &my_addr, 
		      sizeof(my_addr)) < 0 ) {
		//
		// Failed to bind the socket. So close the socket and
		// bail out with an error message.
		//
#if PLT_SYSTEM_NT
		closesocket(sock);
#else
		close(sock);
#endif
		PltLog::Error("KsServerBase::startServer(): "
			      "could not bind the TCP socket.");
	    } else {
		_tcp_transport = new KssListenTCPXDRConnection(
		    sock, 60/* secs */);
	    }
	}
    }

    if ( _tcp_transport ) {
	//
	// Make sure that the _tcp_transport is always automatically
	// dispatched into the ACPLT/KS server dispatcher. Then register
	// the transport with the connection manager.
	//
	_tcp_transport->setAttentionPartner(&_attention_dispatcher);
    	if ( KsConnectionManager::getConnectionManagerObject()->
	       addConnection(*_tcp_transport) ) {
	    _is_ok = true;
	} else {
	    _tcp_transport->shutdown();
	    delete _tcp_transport;
            _tcp_transport = 0;
            PltLog::Error("KsServerBase::startServer(): "
                          "could not add TCP transport.");
            _is_ok = false;
	}
    } else {
        PltLog::Error("KsServerBase::startServer(): "
                      "could not create TCP transport.");
        _is_ok = false;
    }
} // KsServerBase::startServer


// ---------------------------------------------------------------------------
// Try to stop the event loop as soon as possible. Due to the nature of
// ONC/RPC this can take some time as all incomming transports with waiting
// RPC requests are each served exactly once until we get back to the
// server main loop "run()".
//
void KsServerBase::downServer()
{
    _shutdown_flag = 1;
} // KsServerBase::downServer


// ---------------------------------------------------------------------------
// Finally shut down the server. Here this isn't of much woe, but
// derived classes needs this sucker, so they can deregister them, or
// do other strange things... Well, at least we shoot our transport
// and pending events, too.
//
void KsServerBase::stopServer()
{
    if ( KsConnectionManager::getConnectionManagerObject() ) {
	// TODO: can we shutdown here? What if a client is still running?
	if ( !KsConnectionManager::getConnectionManagerObject()->shutdownConnections(15) ) { // FIXME
	    PltLog::Info("KsServerBase::stopServer(): "
                         "could not successfully flush all (sending) streams.");
	}
    }
    cleanup();
} // KsServerBase::stopServer


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

    resetGoingDown();
    while (!isGoingDown()) {
#if ! (PLT_SYSTEM_NT || PLT_SYSTEM_OPENVMS)
        KsConnectionManager::getConnectionManagerObject()->
	    servePendingEvents(0); // no timeout -> wait forever
#else
        KsConnectionManager::getConnectionManagerObject()->
	    servePendingEvents(&aShortTime);
#endif
    }
}


// ---------------------------------------------------------------------------
// Compatibility stuff for backward compatibility with 1.0.x and 1.2.x source
// code series.
//
bool
KsServerBase::addTimerEvent(KsTimerEvent *event)
{
    return KsConnectionManager::getConnectionManagerObject()->
	addTimerEvent(event);
} // KsServerBase::addTimerEvent

bool
KsServerBase::removeTimerEvent(KsTimerEvent *event)
{
    return KsConnectionManager::getConnectionManagerObject()->
	removeTimerEvent(event);
} // KsServerBase::removeTimerEvent

KsTimerEvent *KsServerBase::removeNextTimerEvent()
{
    return KsConnectionManager::getConnectionManagerObject()->
	removeNextTimerEvent();
} // KsServerBase::removeNextTimerEvent


/* End of svrbase.cpp */
