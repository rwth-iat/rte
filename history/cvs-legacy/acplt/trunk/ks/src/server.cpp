/* -*-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/src/server.cpp,v 1.19 2003-10-13 12:12:07 harald Exp $ */
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
 * server.cpp: this module contains the knowledge how to register ACPLT/KS
 *             servers with the ACPLT/KS manager. There isn't really much
 *             more to it, folks.
 */

/*
 * Written by Harald Albrecht & Martin Kneissl
 * <harald@plt.rwth-aachen.de> <martin@plt.rwth-aachen.de>
 */

#include "ks/server.h"
#include "ks/serverconnection.h"
#include "ks/register.h"
#include "ks/event.h"
#include "plt/log.h"
#if !PLT_SYSTEM_NT
#include <sys/socket.h>
#endif


#define DEBUG_REG 0


// ---------------------------------------------------------------------------
//
#define MIN_TTL 10 /* Minimum TimeToLive of a server registration */
#define MGR_TIMEOUT 2 /* Timeout when communicating with KS Manager */

// ---------------------------------------------------------------------------
// Non-blocking, buffered Manager registration stuff... This is specific to
// this class, so it's not externally visible outside of this source file.
//
class KsManagerRegistration : KsServerConnection, KsTimerEvent {
public:
    KsManagerRegistration(u_long ttl, int port);
    virtual ~KsManagerRegistration();

    void registerServer();
    void unregisterServer();

    virtual void async_attention(KsServerConnectionOperations op);
    virtual void trigger();

protected:
    bool sendRegistration();
    bool sendUnregistration();

    u_long _ttl;
    int _port;
    bool _pendingOperationIsRegistration;
};


// ---------------------------------------------------------------------------
// Create a Connection object for the registration of this server with the
// local KS Manager.
//
KsManagerRegistration::KsManagerRegistration(u_long ttl, int port)
    : KsServerConnection("127.0.0.1"),
      KsTimerEvent(KsTime::now()),
      _ttl(ttl),
      _port(port),
      _pendingOperationIsRegistration(false)
{
    setTimeouts(MGR_TIMEOUT, MGR_TIMEOUT); // connection timeout, call timeout
} // KsManagerRegistration::KsManagerRegistration


// ---------------------------------------------------------------------------
// Remove the registration if not done already.
//
KsManagerRegistration::~KsManagerRegistration()
{
    KsServerBase::getServerObject().removeTimerEvent(this);
} // KsManagerRegistration::~KsManagerRegistration


// ---------------------------------------------------------------------------
void
KsManagerRegistration::registerServer()
{
    //
    // only do or renew the registration, if the server is still running...
    //
    if ( !KsServerBase::getServerObject().isGoingDown() ) {
	switch ( getState() ) {
	    case ISC_STATE_OPEN:
		close();
	    case ISC_STATE_CLOSED:
		_pendingOperationIsRegistration = true;
		if ( !open() ) {
		    PltLog::Error("KsServer: can not open registration connection to ACPLT/KS Manager.");
		}
		break;
	    case ISC_STATE_BUSY:
		// do nothing here and do *not* queue the request
		break;
	}
    }
} // KsManagerRegistration::registerServer


// ---------------------------------------------------------------------------
// Unregister this KS server with the local ACPLT/KS Manager. In contrast to
// the registerServer() method, this method is *blocking*. This way we ensure
// that deregistration can successfull completed before shutting down the
// complete process.
//
void
KsManagerRegistration::unregisterServer()
{
#if DEBUG_REG
    PltLog::Debug("Unregistering...");
#endif
    //
    // remove any pending reregistration event (that is, us) from the
    // server's timer event queue.
    //
    KsServerBase::getServerObject().removeTimerEvent(this);
    //
    // If there is an outstanding call or connection opening, then abort
    // it immediately before we proceed to deregistration.
    //
    switch ( getState() ) {
	case ISC_STATE_OPEN:
	case ISC_STATE_BUSY:
	    close();
	case ISC_STATE_CLOSED:
	    _pendingOperationIsRegistration = false;
	    if ( !open() ) {
		PltLog::Error("KsServer: can not open unregistration connection to ACPLT/KS Manager.");
	    }
	    break;
    }
    //
    // Since the server is shutdown but the deregistration process is
    // continuing in the *background*, we need to give IO some spare cycles
    // so the communication with the KS Manager can happen. The
    // servePendingEvents() method of the connection manager returns not
    // later than the timeout given, but also immediately after some
    // connection has become active the one way or the other. We use this
    // behaviour to terminate the loop after the deregistration was
    // completed (if the timeout has not yet been reached).
    //
    KsTime slot(1);
    KsTime deadline(KsTime::now(2 * MGR_TIMEOUT));
    do {
	KsConnectionManager::getConnectionManagerObject()->
	    servePendingEvents(slot);
    } while ( (getState() != ISC_STATE_CLOSED)
	      && (KsTime::now() < deadline) );
} // KsManagerRegistration::unregisterServer


// ---------------------------------------------------------------------------
bool
KsManagerRegistration::sendRegistration()
{
    KsRegistrationParams reg;

    reg.server.name = KsServerBase::getServerObject().getServerName();
    reg.server.protocol_version = 
	KsServerBase::getServerObject().getProtocolVersion();
    reg.port = _port;
    reg.time_to_live = _ttl;
    
    return send(KS_REGISTER, reg);
} // KsManagerRegistration::sendRegistration


// ---------------------------------------------------------------------------
bool
KsManagerRegistration::sendUnregistration()
{
    KsUnregistrationParams unreg;

    unreg.server.name = 
	KsServerBase::getServerObject().getServerName();
    unreg.server.protocol_version = 
	KsServerBase::getServerObject().getProtocolVersion();

    return send(KS_UNREGISTER, unreg);
} // KsManagerRegistration::sendUnregistration


// ---------------------------------------------------------------------------
// The reregistration timer fired off...
//
void
KsManagerRegistration::trigger()
{
    registerServer();
} // KsManagerRegistration::trigger


// ---------------------------------------------------------------------------
// Handle callbacks from the connection: they indicate that certain events,
// such as finished opening or receiving a call, are happening.
//
void
KsManagerRegistration::async_attention(KsServerConnectionOperations op)
{
    if ( getLastResult() != KS_ERR_OK ) {
	if ( _pendingOperationIsRegistration ) {
	    PltLog::Error("KsServer: could not register with ACPLT/KS Manager. Retrying...");
	} else {
	    PltLog::Error("KsServer: could not unregister with ACPLT/KS Manager.");
	}
    }

    switch ( op ) {
	//
	// The connection has been set up, so we can now proceed and issue
	// the registration or unregistration ACPLT/KS request to our local
	// KS Manager.
	//
	case ISC_OP_OPEN:
#if DEBUG_REG
	    PltLog::Debug("ISC_OP_OPEN");
#endif
	    if ( getLastResult() == KS_ERR_OK ) {
		_pendingOperationIsRegistration ?
		    sendRegistration() : sendUnregistration();
	    } else if ( _pendingOperationIsRegistration ) {
		//
		// If the connection establishment failed, then rearm the
		// timer event for reregistration.
		//
		PltLog::Debug("Rearming fast register timer event.");
		_trigger_at = KsTime::now(MIN_TTL);
		KsServerBase::getServerObject().addTimerEvent(this);
	    }
	    break;
	//
	// The result of a KS service call to the KS Manager has been
	// arrived.
	//
	case ISC_OP_CALL:
#if DEBUG_REG
	    PltLog::Debug("ISC_OP_CALL");
#endif
	    if ( getLastResult() == KS_ERR_OK ) {
		PltLog::Debug(_pendingOperationIsRegistration ?
			      "KsServer: registration successfull." :
		              "KsServer: unregistration successfull.");
	    }
	    //
	    // Don't forget to renew the registration automatically. If the
	    // registration was unsuccessfull, then we retry more often.
	    //
	    if ( _pendingOperationIsRegistration
		 && !KsServerBase::getServerObject().isGoingDown() ) {
#if DEBUG_REG
		PltLog::Debug("Rearming register timer event.");
#endif
		u_long secs = _ttl / 4;
		secs *= 3;
		if ( getLastResult() != KS_ERR_OK ) {
		    secs = MIN_TTL;
		} else if ( secs < MIN_TTL ) {
		    secs = MIN_TTL;
		}
		_trigger_at = KsTime::now(secs);
		KsServerBase::getServerObject().addTimerEvent(this);
	    }
	    //
	    // For the moment, we are immediately closing the connection
	    // after each registration/deregistration.
	    //
	    close();
	    break;
    }
} // KsManagerRegistration::async_attention


// ---------------------------------------------------------------------------
//
KsServer::KsServer(u_long ttl, int port)
: _ttl(ttl),
  _ks_mgr(0)
{
    if ( port != KS_ANYPORT ) {
        //
        // Some day I'll be after those who'd invented virtual base
        // classes without defining the order of constructors called
        // for derived classes. But on the other side, why did I ever
        // relied on that?! So shame on me...
        //
        _sock_port = port;
    }
} // KsServer::KsServer


// ---------------------------------------------------------------------------
//
KsServer::~KsServer()
{
    //
    // If this hasn't been done yet...
    //
    if ( _ks_mgr ) {
	delete _ks_mgr;
	_ks_mgr = 0;
    }
}


// ---------------------------------------------------------------------------
//
void
KsServer::startServer()
{
    KsServerBase::startServer();

    if ( _is_ok ) {
	_ks_mgr = new KsManagerRegistration(_ttl, _tcp_transport->getPort());
	if ( _ks_mgr ) {
	    _ks_mgr->registerServer();
	} else {
	    _is_ok = false;
	}
    }
} // KsServer::startServer


// ---------------------------------------------------------------------------
//
void
KsServer::stopServer() 
{
    //
    // Try to unregister first, before we shut down the transports.
    //
    if ( _ks_mgr ) {
	_ks_mgr->unregisterServer();
	delete _ks_mgr;
	_ks_mgr = 0;
    }
    KsServerBase::stopServer();
} // KsServer::stopServer


// End of file ks/server.cpp
