/* -*-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/include/ks/connectionmgr.h,v 1.7 2003-10-13 11:22:25 harald Exp $ */
/*
 * Copyright (c) 1996, 2003
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
 * connectionmgr.h -- Abstract base class for connection managers which can
 *   handle connection objects, which in turn encapsulate XDR streams. The
 *   connection manager makes it possible to use so-called buffered XDR
 *   streams, which decouple RPC I/O from service handling and thus
 *   serialization and deserialization. In addition, the connection manager
 *   can also handle other kinds of connections.
 *
 * Written by Harald Albrecht <harald@plt.rwth-aachen.de>
 */

#ifndef KS_CONNECTIONMGR_H_INCLUDED
#define KS_CONNECTIONMGR_H_INCLUDED

#include "ks/time.h"
#include "ks/event.h"

class KsConnection;
class KsConnectionManager;

#ifndef KS_CONNECTION_H_INCLUDED
#include "ks/connection.h"
#endif


// ---------------------------------------------------------------------------
// The Connection Manager: it manages the whole mess of glory/gory connections
//
class KsConnectionManager {
public:
    KsConnectionManager();
    virtual ~KsConnectionManager();

    bool isOk() const { return _is_ok; }

    // Return connection manager singleton
    static KsConnectionManager * getConnectionManagerObject();
    
    // I/O and event management
    virtual bool hasPendingEvents() const = 0;
    virtual bool servePendingEvents(KsTime timeout = KsTime()) = 0;
    virtual bool servePendingEvents(KsTime *pTimeout) = 0;

    // Timeout event management
    virtual bool addTimerEvent(KsTimerEvent *event) = 0;
    virtual bool removeTimerEvent(KsTimerEvent *event) = 0;
    virtual KsTimerEvent *removeNextTimerEvent() = 0;
    virtual const KsTimerEvent *peekNextTimerEvent() const = 0;
    
    //
    virtual bool shutdownConnections(long secs) = 0;
    
    // connection management
    virtual bool addConnection(KsConnection &con) = 0;
    virtual bool removeConnection(KsConnection &con) = 0;
    virtual bool resetConnection(KsConnection &con) = 0;
    virtual bool trackConnection(KsConnection &con) = 0;

    // damned lies & statistics
    virtual unsigned int getConnectionCount() = 0;
    virtual unsigned int getIoErrorCount() = 0;
    virtual unsigned int getIoRxErrorCount() = 0;
    virtual unsigned int getIoTxErrorCount() = 0;
    
protected:
    friend class KsConnection;

    bool _is_ok;

    static KsConnectionManager *the_cnx_manager;

}; // class KsConnectionManager


// ---------------------------------------------------------------------------
//
inline KsConnectionManager *
KsConnectionManager::getConnectionManagerObject()
{
    return the_cnx_manager;
} // KsConnectionManager::getConnectionManagerObject


#endif
/* End of connectionmgr.h */
