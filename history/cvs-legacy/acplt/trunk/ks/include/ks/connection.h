/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/include/ks/connection.h,v 1.12 2008-09-22 08:26:09 henning Exp $ */
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
 * connection.h -- Abstract base class for I/O connections.
 *
 * Written by Harald Albrecht <harald@plt.rwth-aachen.de>
 */

#ifndef KS_CONNECTION_H_INCLUDED
#define KS_CONNECTION_H_INCLUDED


#include "ks/xdr.h"
#include "ks/rpcproto.h"
#include "ks/avticket.h"
#include "ks/result.h"


// ---------------------------------------------------------------------------
// The KssConnectionAttentionInterface has to be inherited by classes which
// need to handle I/O connections derived from KssConnection. Whenever these
// connections need attention (request or answer available), they will invoke
// the attention method of the KssConnectionAttentionInterface.
// If the attention() method returns true, the connection will the reactivated,
// otherwise it won't be touched anymore.
//
class KssConnection;
class KssConnectionManager;

class KssConnectionAttentionInterface {
public:
    virtual bool attention(KssConnection &conn) = 0;
    virtual ~KssConnectionAttentionInterface() {}
}; // class KssConnectionAttentionInterface


// ---------------------------------------------------------------------------
// Abstract base class for a connection: a connection has its own state (it's
// a finite automata), a socket to suck from, ... and other stuff I forget
// what it's good for. But what's important: if the connection needs attention
// then some other object will be notified through the
// KssConnectionAttentionInterface. Note that any object implementing this
// interface can handle the attention request of a connection, so there's no
// need for a connection class to implement this interface itself.
//
class KssConnection {
public:
    //
    // type of connection, either acting as a server's connection waiting
    // for requests, or as a client's connection.
    //
    enum ConnectionType {
    	CNX_TYPE_SERVER,    	// it�s on the server side
	CNX_TYPE_CLIENT     	// it�s on the client side
    };
	
    KssConnection(int fd, bool autoDestroyable, unsigned long timeout,
	          ConnectionType type);
    virtual ~KssConnection() { }

    //
    // Shuts down the real connection encapsulated by this object. This is
    // where the old "You can't do late binding in your desctructor" comes
    // in again. So before you delete a connection, shut it down first.
    //
    virtual void shutdown() = 0;

    //
    // The "attention partner" is an object implementing the
    // KssConnectionAttentionInterface interface and handling the attention
    // request of this connection.
    //
    KssConnectionAttentionInterface *getAttentionPartner() const
        { return _attention_partner; }
    void setAttentionPartner(KssConnectionAttentionInterface *partner)
        { _attention_partner = partner; }
        
    //
    // The states a connection can be in (a connection is regarded as a
    // finite state automata).
    // For servers: IDLE->RECEIVING->READY->SENDING->IDLE
    // For clients: CONNECTING->CONNECTED->SENDING->WAITING
    //              ->RECEIVING->READY->CONNECTED
    //
    enum ConnectionState {
    	CNX_STATE_IDLE,   	// connection is waiting for next incom. request
	CNX_STATE_WAITING,  	// c. is waiting for reply (w/ timeout)
	CNX_STATE_PASSIVE,  	// c. is not doing any io yet
	CNX_STATE_READY,  	// c. is waiting to be served (has request/reply)
	CNX_STATE_READY_FAILED, // c. waiting for reply timed out
	CNX_STATE_RECEIVING,	// c. is receiving (maybe a request)
	CNX_STATE_SENDING,	// c. is sending (maybe a reply)
        CNX_STATE_CONNECTING,   // c. is connecting to a server (w/ timeout)
	CNX_STATE_CONN_FAILED,  // c. establishment has failed
	CNX_STATE_CONNECTED,    // c. established
	CNX_STATE_DEAD	    	// c. is dead and should be destroyed
    }; // enum ConnectionState
    //
    // The I/O mode is a generalization of the connection state with
    // respect of information the connection manager is interested in.
    //
    enum ConnectionIoMode {
        CNX_IO_DORMANT = 0x0000,      // c. waits to be served
    	CNX_IO_READABLE = 0x0001,     // c. can receive input
	CNX_IO_WRITEABLE = 0x0002,    // c. can send output
	CNX_IO_ATTENTION = 0x0004,    // c. need to be served
	CNX_IO_DEAD = 0x0008, 	      // c. is dead
	CNX_IO_NEED_TIMEOUT = 0x8000, // c. needs a timeout control
	CNX_IO_NO_FASTWRITE = 0x0800, // do not immediately send data
	CNX_IO_HAD_ERROR = 0x4000,    // c. had an i/o error
	CNX_IO_HAD_TX_ERROR = 0x2000, // c. had i/o error during sending
	CNX_IO_HAD_RX_ERROR = 0x1000  // c. had i/o error during receiving
    }; // enum ConnectionIoMode

        
    inline bool isAutoDestroyable() const { return _auto_destroyable; }
    inline int getFd() const { return _fd; }
    inline ConnectionType getCnxType() const { return _cnx_type; }
    u_short getPort() const;

    virtual long getTimeout() const;
    virtual void setTimeout(unsigned long timeout);
    
    ConnectionState getState() const { return _state; }
    virtual ConnectionIoMode getIoMode() const = 0;
        
    inline struct sockaddr_in *getPeerAddress(int &addrLen)
    	{ addrLen = _client_address_len; return &_client_address; }
    bool setPeerAddr(struct sockaddr_in *addr, int addrLen);

protected:
    //
    // These methods are called by the connection manager whenever there
    // is i/o to carry out.
    //
    friend class KssConnectionManager;
    virtual ConnectionIoMode receive() = 0;
    virtual ConnectionIoMode send() = 0;
    //
    // Connection i/o has timed out.
    //
    virtual ConnectionIoMode timedOut() = 0;
    //
    // Bring connection back into its default state.
    //
    virtual ConnectionIoMode reset() = 0;
    
    void thisIsMyConnectionManager(KssConnectionManager *mgr)
    	{ _manager = mgr; }

    ConnectionType                   _cnx_type;    
    unsigned long                    _timeout;
    bool                             _auto_destroyable;
    int                              _fd;
    ConnectionState                  _state;
    struct sockaddr_in               _client_address;
#if defined(PLT_RUNTIME_GLIBC) && (PLT_RUNTIME_GLIBC >= 0x20001)
    socklen_t                        _client_address_len;
#elif PLT_SYSTEM_OPENVMS
    unsigned int                     _client_address_len;
#else
    int                              _client_address_len;
#endif
    KssConnectionManager            *_manager;
    KssConnectionAttentionInterface *_attention_partner;
}; // class KssConnection


// ---------------------------------------------------------------------------
// Still abstract class for XDR connections. This class only introduces the
// XDR stream encapsulated by instances of this class, the proper shutdown
// of such XDR streams, and how to get your hands on them.
//
class KssXDRConnection : public KssConnection {
public:
    KssXDRConnection(int fd, bool autoDestroyable, unsigned long timeout,
	             ConnectionType type);
    
    virtual void shutdown();
    
    XDR *getXdr() { return &_xdrs; }
    
    //
    // And now for the missing parts of the KssTransport pseudo-interface
    // which is used by the KsServerBase class for processing RPC calls.
    //
    XDR *getDeserializingXdr() { return &_xdrs; }
    KS_RESULT finishRequestDeserialization(KsAvTicket &avt, bool ok);
    u_long getProgramId() const { return _rpc_header._prog_number; }
    u_long getServiceId() const { return _rpc_header._procedure; }
    u_long getVersion() const { return _rpc_header._prog_version; }
    virtual void sendPingReply() = 0;
    virtual void sendErrorReply(KsAvTicket &avt, KS_RESULT error) = 0;
    virtual void sendReply(KsAvTicket &avt, KsResult &result) = 0;
    virtual void personaNonGrata() = 0;

    virtual bool beginRequest(u_long xid, u_long prog_number,
			      u_long prog_version, u_long proc_number) = 0;
    virtual void sendRequest() = 0;
    KsRpcHeader getRpcHeader() const { return _rpc_header; }
    
protected:
    virtual void freeStreamMemory() { }
    
    bool makeNonblocking();
    bool enableKeepAlive();

    XDR         _xdrs;
    bool        _cleanup_xdr_stream;
    KsRpcHeader _rpc_header;
}; // class KssXDRConnection


#endif

/* End of connection.h */
