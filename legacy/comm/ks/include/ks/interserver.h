/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/include/ks/interserver.h,v 1.9 2007-04-25 12:57:20 martin Exp $ */
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
 * interserver.h -- implements an interserver connection object which can
 *                  send ACPLT/KS service requests to other ACPLT/KS
 *                  servers and receive the service replies. This i/o is
 *                  done in the background using the connection manager
 *                  magic. Therefore it's only available when the buffered
 *                  XDR streams have been enabled.
 *
 * Written by Harald Albrecht <harald@plt.rwth-aachen.de>
 */

#ifndef KSINTERSERVER_H_INCLUDED
#define KSINTERSERVER_H_INCLUDED

#if PLT_USE_BUFFERED_STREAMS

#include "ks/svrbase.h"
#include "ks/xdrtcpcon.h"
#include "ks/xdrudpcon.h"
#include "ks/register.h"

#if PLT_USE_DEPRECIATED_HEADER
#include <iostream.h>
#else
#include <iostream>
#endif


// ---------------------------------------------------------------------------
// The class KssInterKsServerConnection provides client functionality to
// ACPLT/KS server, so servers can send ACPLT/KS service requests to other
// ACPLT/KS servers. This is done asynchronous during the i/o or idle slot
// and thus appears in the background.
//
class KssInterKsServerConnection : protected KssConnectionAttentionInterface
{
public:
    KssInterKsServerConnection(KsString host, KsString server);
    virtual ~KssInterKsServerConnection();

    //
    // Macro state of the connection.
    //
    enum KssInterKsServerConnectionState {
	ISC_STATE_CLOSED, // c. is closed
	ISC_STATE_OPEN,   // c. is open
	ISC_STATE_BUSY    // c. may be open and is currently in use (e.g. a
	                  // connection request may be on its way...)
    };

    //
    // Available asynchronous operations. You'll need them to find out what
    // has happened when this inter-server connection needs attention.
    //
    enum KssInterKsServerConnectionOperations {
	ISC_OP_OPEN, // open operation has been completed/failed
	ISC_OP_CALL  // service request has been completed/failed
    };

    //
    // Asynchronously open the connection (that is, in the background).
    //
    bool open();

    //
    // Immediately close the connection
    //
    void close();

    //
    // Return protocol version spoken by ACPLT/KS server
    //
    u_short getProtocolVersion() const;

    //
    // Send an ACPLT/KS message (request) to the wire:
    //   send() is just a C++ convenience,
    //   C-lovers please take the beginSend()/.../endSend() route.
    //
    bool send(u_long serviceid, KsXdrAble &params);
    bool beginSend(u_long serviceid);
    bool endSend();

    //
    // Receive an ACPLT/KS reply from the wire. Note that the same rules
    // for C++ versus C applies as mentioned just before.
    //
    bool receive(KsResult &result);
    bool beginReceive();
    bool endReceive();

    //
    // This method needs to be implemented in derived classes.
    //
    virtual void async_attention(KssInterKsServerConnectionOperations op) = 0;

    KssInterKsServerConnectionState getState() const { return _state; }
    KS_RESULT getLastResult() const { return _result; }

    //
    // The underlying XDR stream.
    //
    XDR *getXdr() const;

    //
    // Control timeout spans for connection establishment and sending a
    // RPC request/receiving the corresponding reply
    //
    void setTimeouts(unsigned long connectto, unsigned long callto)
        { _connect_timeout = connectto; _call_timeout = callto; }
    void getTimeouts(unsigned long &connectto, unsigned long &callto) const
        { connectto = _connect_timeout; callto = _call_timeout; }

protected:
    //
    // Substate of the connection. This state indicates what kind of
    // underlying connection is currently in use.
    //
    enum ISCSubState {
	ISC_SUBSTATE_NONE,
	ISC_SUBSTATE_CONNECTING_PMAP,
	ISC_SUBSTATE_CONNECTING_MANAGER,
	ISC_SUBSTATE_CONNECTING_SERVER
    };

    //
    // handler which is called by the internal connection
    //
    virtual bool attention(KssConnection &con);

    u_long makeXid();

    void activateConnection();
    void closeConnection();

    bool openPortmapperConnection();
    bool openManagerConnection(u_short port, int protocol);
    bool openServerConnection(u_short port);

    KssXDRConnection                *_cln_con;
    bool                             _cln_con_once_closed;
    KssInterKsServerConnectionState  _state;
    ISCSubState                      _sub_state;

    KS_RESULT                        _result;

    KsString                         _host;
    u_short                          _host_port;
    struct sockaddr_in               _host_addr; 
    struct in_addr                   _old_ip;
    KsString                         _server;
    u_short                          _server_port;

    unsigned long                    _connect_timeout;
    unsigned long                    _call_timeout;

    unsigned short                   _protocol_version;
}; // class KssInterKsServerConnection


#endif /* PLT_USE_BUFFERED_STREAMS */

#endif /* KSINTERSERVER_H_INCLUDED */

// End of ks/interserver.h
