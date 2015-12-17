/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/src/xdrtcpcon.cpp,v 1.21 2007-04-25 12:57:21 martin Exp $ */
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
 * xdrtcpcon.cpp -- Implements an TCP connection with XDR data as its
 *                  data representation for the mess we'll transport.
 *
 * Written by Harald Albrecht <harald@plt.rwth-aachen.de>
 */

#if PLT_USE_DEPRECIATED_HEADER
#include <iostream.h>
#else
#include <iostream>
#endif

#if PLT_USE_BUFFERED_STREAMS

#include "ks/xdrtcpcon.h"
#include "ks/xdrmemstream.h"
#include "ks/connectionmgr.h"


#if PLT_SYSTEM_NT
#include <errno.h>

#define write(fd,p,l) send(fd,p,l,0)
#define read(fd,p,l) recv(fd,p,l,0)
#define ioctl(fd,r,p) ioctlsocket(fd,r,p)

#ifdef  EINVAL
#undef  EINVAL
#endif
#define EINVAL      WSAEINVAL
#ifdef  EINTR
#undef  EINTR
#endif
#define EINTR       WSAEINTR
#ifdef  EWOULDBLOCK
#undef  EWOULDBLOCK
#endif
#define EWOULDBLOCK WSAEWOULDBLOCK

#else
#include <unistd.h>
#include <errno.h>
#endif


// ---------------------------------------------------------------------------
// Constructs a TCP rendevouser connection. The sole purpose of such a con-
// nection object is to wait for incomming connection requests. Then it
// creates the real TCP connection objects for handling such connections. To
// avoid blocking situations we´re putting the socket into a non-blocking
// listen state.
//
KssListenTCPXDRConnection::KssListenTCPXDRConnection(int fd, 
                                                     unsigned long timeout)
    : KssXDRConnection(fd, false, timeout, CNX_TYPE_SERVER)
{
    _cleanup_xdr_stream = false; /* this one doesn´t have a XDR stream!! */
    if ( !makeNonblocking() ) {
	_state = CNX_STATE_DEAD;
	return;
    }
#if !PLT_USE_XTI
    if ( listen(_fd, 5) < 0 ) {
	_state = CNX_STATE_DEAD;
    }
#else
    // TODO: already be configured by server code
#endif
} // KssListenTCPXDRConnection::KssListenTCPXDRConnection
        

// ---------------------------------------------------------------------------
// A TCP rendevouser connection is always in the readable state so it can
// accept incomming connections. Otherwise it can only be dead.
//
KssConnection::ConnectionIoMode KssListenTCPXDRConnection::getIoMode() const
{
    return _state == CNX_STATE_DEAD ? CNX_IO_DEAD : CNX_IO_READABLE;
} // KssListenTCPXDRConnection::ConnectionIoMode


// ---------------------------------------------------------------------------
// A TCP rendevouser can´t get (RPC) pinged. So there´s nothing to do here...
//
void KssListenTCPXDRConnection::sendPingReply()
{
} // KssListenTCPXDRConnection::sendPingReply


void KssListenTCPXDRConnection::sendErrorReply(KsAvTicket &, KS_RESULT)
{ }
void KssListenTCPXDRConnection::sendReply(KsAvTicket &, KsResult &)
{ }
void KssListenTCPXDRConnection::personaNonGrata()
{ }
bool KssListenTCPXDRConnection::beginRequest(u_long,
					     u_long,
					     u_long,
					     u_long)
{ return false; }
void KssListenTCPXDRConnection::sendRequest()
{ }


// ---------------------------------------------------------------------------
// When a TCP rendevouser "receives" data, this means that there are new
// incomming connections waiting to be served.
//
KssConnection::ConnectionIoMode KssListenTCPXDRConnection::receive()
{
    struct sockaddr_in saddr;
#if defined(PLT_RUNTIME_GLIBC) && (PLT_RUNTIME_GLIBC >= 0x20001)
    socklen_t          saddr_len;
#elif !PLT_SYSTEM_OPENVMS
    int                saddr_len;
#else
    unsigned int       saddr_len;
#endif
    int                newfd;
    
    if ( _state == CNX_STATE_DEAD ) {
	return CNX_IO_DEAD;
    }
    saddr_len = sizeof(saddr);
    newfd = accept(_fd, (struct sockaddr *) &saddr, &saddr_len);
    if ( newfd < 0 ) {
	//
	// An incomming connection was probably reset before we had a chance
	// to accept it and there are no more other pending connections in
	// the backlog queue.
	//
	return CNX_IO_READABLE;
    } else {
	KssConnection *con = new KssTCPXDRConnection(newfd, _timeout, 
		                                     saddr, saddr_len,
		                                     _cnx_type);
        if ( !con ) {
#if PLT_SYSTEM_NT
	    closesocket(newfd);
#else
	    close(newfd);
#endif
	    return CNX_IO_READABLE;
	}
	con->setAttentionPartner(getAttentionPartner());
	ConnectionIoMode ioMode = con->getIoMode();
	if ( ioMode == CNX_IO_DEAD ) {
	    delete con;
	} else {
    	    if ( !_manager->addConnection(*con) ) {
		delete con;
	    }
	}
	return CNX_IO_READABLE;
    }
} // KssListenTCPXDRConnection::receive


// ---------------------------------------------------------------------------
// A TCP rendevouser can´t send anything...
//
KssConnection::ConnectionIoMode KssListenTCPXDRConnection::send()
{
    return getIoMode();
} // KssListenTCPXDRConnection::send


// ---------------------------------------------------------------------------
// And a TCP rendevouser can't get reset -- it's always "reset" in some
// respect. And we also don't care of time outs.
//
KssConnection::ConnectionIoMode KssListenTCPXDRConnection::reset()
{
    return getIoMode();
} // KssListenTCPXDRConnection::reset

KssConnection::ConnectionIoMode KssListenTCPXDRConnection::timedOut()
{
    return getIoMode();
} // KssListenTCPXDRConnection::timedOut


// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// And now for the real TCP-based XDR dynamic stream.
//

// ---------------------------------------------------------------------------
// Create a new TCP connection on which we can receive requests from a RPC
// client. This connection object is automatically destroyed when the con-
// nection closes.
//
KssTCPXDRConnection::KssTCPXDRConnection(int fd, unsigned long timeout,
	                                 struct sockaddr_in &clientAddr,
                                         int clientAddrLen,
					 ConnectionType type)
    : KssXDRConnection(fd, true, timeout, type)
{
    //
    // First, create the necessary xdr dynamic memory stream. Then make the
    // socket non-blocking and enable keep alive "pings".
    //
    if ( !xdrmemstream_create(&_xdrs) ) {
	_state = CNX_STATE_DEAD;
	return;
    }
    if ( !makeNonblocking() || !enableKeepAlive() ) {
	_state = CNX_STATE_DEAD;
	return;
    }

    _client_address = clientAddr;
    _client_address_len = clientAddrLen;

    //
    // In case of a inter-server connection (where this connection is on the
    // client side) we initiate a non-blocking connect.
    // Please note that client connections are always *not* auto-destroyable.
    // Whenever something bad happens, client connections will mark themselves
    // as dead and ask for attention.
    //
    if ( type == CNX_TYPE_CLIENT ) {
	_auto_destroyable = false;
	if ( connect(_fd, (struct sockaddr *) &_client_address,
		     _client_address_len) == 0 ) {
	    //
	    // Whoops. That was fast. We've got an immediate connection.
	    // Well -- chances to get an immediate connection are very rare
	    // at best. It can probably only happen, if...
	    // 1) we're connecting to a local listening socket, and
	    // 2) the scheduler preempts us just after the request was sent
	    // to the listening socket and before the connect() call checks
	    // for an answer.
	    // So this situation will be pretty rare. But even if it should
	    // show up, we just remain in the connecting state. Because the
	    // socket will then get at least writeable (because there is now
	    // free space in the write buffer), the receive() method will
	    // be called the next time we run a select(), so we will be
	    // notified in every case that the connection has been
	    // established.
	    //
	    _state = CNX_STATE_CONNECTING;
	} else {
	    //
	    // Okay. The connection establishment is going on in the
	    // background.
	    //
#if PLT_SYSTEM_NT
	    int err;
	    err = WSAGetLastError(); /* MSVC 4.2++, once again */
	    if ( (err == WSAEINPROGRESS) || (err == WSAEWOULDBLOCK) ) {
#else
	    if ( (errno == EINPROGRESS) || (errno == EWOULDBLOCK) ) {
#endif
		_state = CNX_STATE_CONNECTING;
	    } else {
		_state = CNX_STATE_DEAD;
	    }
	}
	// FIXME: Init _ptr etc ??
    } else {
	//
	// Server side. Initialize to wait for fragment header...
	//
	_state          = CNX_STATE_IDLE;
	_fragment_state = FRAGMENT_HEADER;
	_remaining_len  = 4;
	_ptr            = _fragment_header;
    }
} // KssTCPXDRConnection::KssTCPXDRConnection


// ---------------------------------------------------------------------------
// Indicate the i/o mode this TCP connection is currently in. This way the
// connection manager can make sure that this connection feels well. The i/o
// basically tells the cnx manager whether a connection is currently read-
// able, writeable (or both) and whether it wants to time out after a certain
// time of inactivity.
//
KssConnection::ConnectionIoMode KssTCPXDRConnection::getIoMode() const
{
    switch ( _state ) {
    case CNX_STATE_DEAD:
	//
	// Dead connections are plain dead. That's all about them.
	//
	return CNX_IO_DEAD;
    case CNX_STATE_IDLE:
	//
	// Just idle, waiting for new request to arrive at the server side.
	// So we're readable, but have no timeout.
	//
	return CNX_IO_READABLE;
    case CNX_STATE_WAITING:
	//
	// We're waiting for new data to arrive. But in contrast to the idle
	// state we have a timeout set, so we won't wait forever.
	//
	return (ConnectionIoMode)
	    (CNX_IO_READABLE | CNX_IO_NEED_TIMEOUT);
    case CNX_STATE_READY:
	//
	// A request has arrived on the server side, or a reply has arrived
	// at the client side. In both cases the connections needs treatment.
	//
	return (ConnectionIoMode)
	    (CNX_IO_DORMANT | CNX_IO_ATTENTION);
    case CNX_STATE_READY_FAILED:
	//
	// A reply didn't came in in time. So we're just asking for attention.
	//
	return (ConnectionIoMode)
	    (CNX_IO_DORMANT | CNX_IO_ATTENTION);
    case CNX_STATE_RECEIVING:
	//
	// This connection is actively receiving a request or a reply (server
	// or client side) and thus in in the middle of transmission, so
	// there's a timeout necessary.
	//
	return (ConnectionIoMode)
	    (CNX_IO_READABLE | CNX_IO_NEED_TIMEOUT);
    case CNX_STATE_SENDING:
	//
	// Same as before, but the other way round.
	//
	return (ConnectionIoMode)
	    (CNX_IO_WRITEABLE | CNX_IO_NEED_TIMEOUT);
    case CNX_STATE_CONNECTING:
	//
	// Connection establishment is under way, so we need to wait for
	// the connection to become readeable and/or writeable. Because
	// we want to control the connection procedure, we use a timeout.
	//
	return (ConnectionIoMode)
	    (CNX_IO_READABLE | CNX_IO_WRITEABLE | CNX_IO_NEED_TIMEOUT |
	     CNX_IO_NO_FASTWRITE);
    case CNX_STATE_CONNECTED:
	//
	// Connection has been established. We just need some attention
	// now because there is probably now data to be sent.
	//
	return CNX_IO_DORMANT;
    case CNX_STATE_CONN_FAILED:
	//
	// Dead connections after a failed connection attempt just ask
	// for attention and get passive.
	//
	return (ConnectionIoMode)
	    (CNX_IO_DORMANT | CNX_IO_ATTENTION);
    case CNX_STATE_PASSIVE:
    default:
	return CNX_IO_DORMANT;
    }
} // KssTCPXDRConnection::getIoMode


// ---------------------------------------------------------------------------
// After a reply has been serialized into the underlaying XDR dynamic memory
// stream, this helper function puts the connection into the sending state.
//
KssConnection::ConnectionIoMode KssTCPXDRConnection::enterSendingState()
{
    int len;
    
    xdrmemstream_rewind(&_xdrs, XDR_DECODE);
    xdrmemstream_get_length(&_xdrs, &len);
    _state          = CNX_STATE_SENDING;
    //
    // When clearing the dynamic memory stream, it was the responsibility of
    // the replyXXX() methods to provide space for the fragment header. We´re
    // now fixing it, so we can later send out the whole mess as a complete
    // block (if the os can do this...). See below in the send() method for
    // more details.
    //
    _fragment_state = FRAGMENT_BODY;
    _remaining_len  = len;
    XDR_INLINE_PTR ppp = xdr_inline(&_xdrs, 4);
    IXDR_PUT_LONG(ppp, (len - 4) | 0x80000000ul);
    xdrmemstream_rewind(&_xdrs, XDR_DECODE);
    return getIoMode();
} // KssTCPXDRConnection::enterSendingState


// ---------------------------------------------------------------------------
// Answer the usual ping request. Just fill in the simple answer and send it
// down the pipe.
//
void KssTCPXDRConnection::sendPingReply()
{
    if ( _state == CNX_STATE_DEAD ) {
    	return;
    }
    xdrmemstream_clear(&_xdrs);
    _rpc_header.acceptCall();
    u_long dummy = 0x80000000ul;       // Make sure there´s room for the
    if ( xdr_u_long(&_xdrs, &dummy) && // fragment header we´ll fix later...
         _rpc_header.xdrEncode(&_xdrs) ) {
    	_state = CNX_STATE_SENDING;
    } else {
    	reset();
	return;
    }
    enterSendingState();
} // KssTCPXDRConnection::sendPingReply


// ---------------------------------------------------------------------------
//
void KssTCPXDRConnection::sendErrorReply(KsAvTicket &avt, KS_RESULT error)
{
    if ( _state != CNX_STATE_DEAD ) {
    	u_long e = error == KS_ERR_OK ? KS_ERR_GENERIC : error;
    	xdrmemstream_clear(&_xdrs);
	_rpc_header.acceptCall();
	u_long dummy = 0x80000000ul;       // Make sure there´s room for the
	if ( xdr_u_long(&_xdrs, &dummy) && // fragment header we´ll fix later...
	     _rpc_header.xdrEncode(&_xdrs) &&
	     avt.xdrEncode(&_xdrs) &&
	     xdr_u_long(&_xdrs, &e) &&
	     avt.xdrEncodeTrailer(&_xdrs) ) {
    	    enterSendingState();
	} else {
    	    reset();
	}
    }    
} // KssTCPXDRConnection::sendErrorReply


// ---------------------------------------------------------------------------
//
void KssTCPXDRConnection::sendReply(KsAvTicket &avt, KsResult &result)
{
    if ( _state != CNX_STATE_DEAD ) {
    	xdrmemstream_clear(&_xdrs);
	_rpc_header.acceptCall();
	u_long dummy = 0x80000000ul;     // Make sure there´s room for the
	if ( xdr_u_long(&_xdrs, &dummy)  // fragment header we´ll fix later...
    	     && _rpc_header.xdrEncode(&_xdrs)
	     && avt.xdrEncode(&_xdrs)
	     && result.xdrEncode(&_xdrs)
	     && avt.xdrEncodeTrailer(&_xdrs) ) {
    	    enterSendingState();
	} else {
    	    reset();
	}
    }    
} // KssTCPXDRConnection::sendReply



bool KssTCPXDRConnection::beginRequest(u_long xid,
				       u_long prog_number, u_long prog_version,
				       u_long proc_number)
{
    if ( _state == CNX_STATE_DEAD ) {
	return false;
    }
    
    xdrmemstream_clear(&_xdrs);
    _rpc_header.setRequest(xid, prog_number, prog_version, proc_number);

    u_long dummy = 0x80000000ul;     // Make sure there´s room for the
    if ( xdr_u_long(&_xdrs, &dummy)  // fragment header we´ll fix later...
	 && _rpc_header.xdrEncode(&_xdrs) ) {
	return true;
    } else {
	return false;
    } 
} // KssTCPXDRConnection::beginRequest


void KssTCPXDRConnection::sendRequest()
{
    enterSendingState();
} // KssTCPXDRConnection::sendRequest


// ---------------------------------------------------------------------------
// With unwanted clients we just put the connection into the dead mode, thus
// it will be cleaned up as soon as it is to be reactivated by the connection
// manager.
//
void KssTCPXDRConnection::personaNonGrata()
{
    reset();
    _state = CNX_STATE_DEAD;
} // KssTCPXDRConnection::personaNonGrata


void KssTCPXDRConnection::freeStreamMemory()
{
    xdrmemstream_clear(&_xdrs);
} // KssTCPXDRConnection::freeMemory


// ---------------------------------------------------------------------------
// We have data... so let´s see what to do with it and fight with RPC tele-
// gramme headers and bodies.
//
KssConnection::ConnectionIoMode KssTCPXDRConnection::receive()
{
    switch ( _state ) {
    case CNX_STATE_CONNECTING: {
	//
	// We're just completing a connection establishment. Now try to find
	// out whether it succeeded of failed. This is unfortunately an area
	// where platform issues can arise (portability...)
	//
	int error;
#if defined(PLT_RUNTIME_GLIBC) && (PLT_RUNTIME_GLIBC >= 0x20001)
        socklen_t size = sizeof(error);
#elif !PLT_SYSTEM_OPENVMS
	int size = sizeof(error);
#else
        unsigned int size = sizeof(error);
#endif

	if ( (getsockopt(_fd, SOL_SOCKET, SO_ERROR, 
                         (char *) &error, &size) < 0)
	     || error ) {
	    //
	    // Connection establishment failed. Put the connection into the
	    // attention state and mark it dead. Note that client connections
	    // are not auto-destroyable!
	    //
	    _state = CNX_STATE_CONN_FAILED;
	} else {
	    //
	    // Yesss! We've got it(tm)...
	    //
	    _state = CNX_STATE_CONNECTED;
	    return (ConnectionIoMode)
		(getIoMode() | CNX_IO_ATTENTION);
	}
	return getIoMode();
    }
    case CNX_STATE_CONNECTED:
    case CNX_STATE_CONN_FAILED: {
	//
	// We might be called from the cnx manager when a connection becomes
	// readable and writeable when connection establishment fails. Because
	// the send routine was already called earlier (depending on the cnx's
	// implementation) we just ignore this. Note that we don't indicate
	// attention again (it could otherwise cause problems).
	//
	return (ConnectionIoMode)(getIoMode() & ~CNX_IO_ATTENTION);
    }
    default:
	break;
    } // switch

    //
    // Okay. But now for the usual receiving stuff...
    //
    if ( _fragment_state == FRAGMENT_HEADER ) {
	//
	// We´re currently about to read in a RPC telegramme fragment
	// header, which indicates how many bytes will follow in this
	// fragment and whether this is the final fragment.
	//
	int len_read;
	for ( ;; ) {
	    len_read = read(_fd, _ptr, _remaining_len);
	    if ( len_read == 0 ) {
	    	//
	    	// This connection was closed by the other side, so we
	    	// know that it´s currently half-closed. The only thing
	    	// we have to do now is to flag that we´re ready to leave
		// the stage... In case we're the client side, then we are
		// not allowed to kill ourself but instead we only bark and
		// wait to get killed.
	    	//
		if ( _cnx_type == CNX_TYPE_CLIENT ) {
		    _state = CNX_STATE_READY_FAILED;
		} else {
		    _state = CNX_STATE_DEAD;
		}
		if ( _remaining_len < 4 ) {
		    //
		    // Hey! Looks like the connection broke *during*
		    // transmission. So bark. If the connection closes
		    // before a new telegramme is transmitted, then it's
		    // fine.
		    //
		    return (ConnectionIoMode)(getIoMode() | CNX_IO_HAD_RX_ERROR);
		}
	    	return getIoMode();
	    } else if ( len_read < 0 ) {
		//
		// An error occured, so find out what has happened. In case
		// of an interrupted call, let's try again... For all other
		// errors simply drop this connection.
		//
#if PLT_SYSTEM_NT
    	    	int myerrno;
		myerrno = WSAGetLastError(); /* MSVC 4.2++, once again */
#else
                int myerrno = errno;
#endif
		switch ( myerrno ) {
		case EINTR:
		    continue;
#if PLT_SYSTEM_NT
		// This is simply brain-damaged. Yes, Bill.
		case WSAECONNRESET:
#else
		case ECONNRESET:
#endif
		    //
		    // In case the connection was closed before any data
		    // was sent, we assume a gracious close...
		    //
		    if ( _remaining_len == 4 ) {
			int len;
			xdrmemstream_rewind(&_xdrs, XDR_DECODE);
			xdrmemstream_get_length(&_xdrs, &len);
			if ( len == 0 ) {
			    // ...it was a gracious close indeed...
			    if ( _cnx_type == CNX_TYPE_CLIENT ) {
				_state = CNX_STATE_READY_FAILED;
			    } else {
				_state = CNX_STATE_DEAD;
			    }
			    return getIoMode();
			}
		    }
		    break; // report rx error...
		case EWOULDBLOCK:
		    return getIoMode();
		}
		if ( _cnx_type == CNX_TYPE_CLIENT ) {
		    _state = CNX_STATE_READY_FAILED;
		} else {
		    _state = CNX_STATE_DEAD;
		}
                return getIoMode();
	    } else {
		break; // okay, we´ve got data...
	    }
	}
	//
	// We´ve got new data. So let´s see whether we´ve received
	// the full header now.
	//
	_remaining_len -= len_read;
	_ptr           += len_read;
	if ( _remaining_len == 0 ) {
	    //
	    // Yup. We´ve got it -- so decode it and see if it´s the
	    // final fragment(tm).
	    //
	    XDR_INLINE_PTR ppp = (XDR_INLINE_PTR) _fragment_header;
	    u_long l  = IXDR_GET_LONG(ppp);
	    _last_fragment = l & 0x80000000ul ? true : false;
	    _remaining_len = l & 0x7FFFFFFFul;
	    if ( _remaining_len ) {
		_fragment_state = FRAGMENT_BODY;
	    } else if ( !_last_fragment ) {
		//
		// This fragment´s body was empty so we´ll wait for
		// the next fragment header. In case the previous header
		// was for an empty last fragment body, we have to skip
		// this code here so we can detect below that the
		// connection should now enter the ready state.
		//
		_remaining_len = 4;
		_ptr           = _fragment_header;
	    }
	}
    }

    if ( _fragment_state == FRAGMENT_BODY ) {
	//
	// We´re in the process of receiving the data within a fragment´s
	// body. We´re relying here on a XDR dynamic memory stream to do
	// the i/o work. Note that I´ve put this if statement on purpose
	// below the check for the fragment´s head. Most probably a full
	// TCP segment has arrived carrying more than only the header, so
	// we don´t need to visit select() again...
	//
	int rlen = _remaining_len;
	int err;
	if ( xdrmemstream_read_from_fd(&_xdrs, _fd, &rlen, &err) ) {
	    _remaining_len = rlen;
	    if ( (_remaining_len == 0) && !_last_fragment ) {
		_fragment_state = FRAGMENT_HEADER;
		_remaining_len  = 4;
		_ptr            = _fragment_header;
	    }
	} else {
	    //
	    // We´ve run into trouble: maybe we´ve run out of buffers or
	    // the like, so we´re just flushing now and try to send back
	    // an error. If we had an i/o error instead, we´ll just shut
	    // down this connection. Basta.
	    //
	    _remaining_len = rlen;
	    if ( (err == ENOMEM) && (_cnx_type == CNX_TYPE_SERVER) ) {
	    	// TODO!!!
		return reset(); // flush all incomming data...
	    } else {
		if ( _cnx_type == CNX_TYPE_CLIENT ) {
		    _state = CNX_STATE_READY_FAILED;
		} else {
		    _state = CNX_STATE_DEAD;
		}
		return (ConnectionIoMode)(getIoMode() | CNX_IO_HAD_RX_ERROR);
	    }
	}
    }

    //
    // If we´ve slurped in the last fragment of a RPC telegramme then we´ll
    // enter the ready state, so the connection manager picks us up and
    // presents us to some other objects.
    //
    if ( _last_fragment && (_remaining_len == 0) ) {
	//
	// Try to deserialize the RPC call header. If this fails, then
	// send back an error reply. If -- for some reason -- we can't
	// serialize the error reply header, then we'll send NO reply
	// at all but flag at least an error (in case we're on the server
	// side). If we're on the client side then an answer for a previous
	// request has just come in. In this case make sure that the answer
	// matches the request (check the transaction id in xid). We don't
	// check whether it was a RPC error reply, this is up to the user of
	// this connection.
	//
	xdrmemstream_rewind(&_xdrs, XDR_DECODE);
	if ( _cnx_type == CNX_TYPE_SERVER ) {
	    if ( !_rpc_header.xdrDecode(&_xdrs) ) {
		_rpc_header.setDecodeError();
		_state = CNX_STATE_SENDING;
		xdrmemstream_clear(&_xdrs);
		if ( !_rpc_header.xdrEncode(&_xdrs) ) {
		    return (ConnectionIoMode)
			(reset() | CNX_IO_HAD_ERROR);
		}
		return (ConnectionIoMode)
		    (enterSendingState() | CNX_IO_HAD_ERROR);
	    }
	    _state = CNX_STATE_READY;
	} else {
	    u_long xid = _rpc_header._xid; // save request xid
	    if ( !_rpc_header.xdrDecode(&_xdrs) ||
		 (xid != _rpc_header._xid) ) {
		_state = CNX_STATE_READY_FAILED;
	    } else {
		_state = CNX_STATE_READY;
	    }
	}
    } else {
	//
	// In all other cases we're now actively receiving data, so we
	// may leave some other state and always enter receiving state.
	//
	_state = CNX_STATE_RECEIVING;
    }
    return getIoMode();
} // KssTCPXDRConnection::receive


// ---------------------------------------------------------------------------
// Send back a RPC telegramme to a client. Please note a peculiarity here:
// we're always sending back the answer as *one* telegramme fragment, which
// is just fine. Another note: we can also send a request to another server,
// so we need to make sure that the automata does work the right way(tm).
//
KssConnection::ConnectionIoMode KssTCPXDRConnection::send()
{
    switch ( _state ) {
    case CNX_STATE_CONNECTING: {
	//
	// We're just completing a connection establishment. Now try to find
	// out whether it succeeded of failed. This is unfortunately an area
	// where platform issues can arise (portability...)
	//
	int error;
#if defined(PLT_RUNTIME_GLIBC) && (PLT_RUNTIME_GLIBC >= 0x20001)
        socklen_t size = sizeof(error);
#elif !PLT_SYSTEM_OPENVMS
	int size = sizeof(error);
#else
	unsigned int size = sizeof(error);
#endif

	if ( (getsockopt(_fd, SOL_SOCKET, SO_ERROR, 
                         (char *) &error, &size) < 0)
	     || error ) {
	    //
	    // Connection establishment failed. Put the connection into the
	    // attention state and mark it dead. Note that client connections
	    // are not auto-destroyable!
	    //
	    _state = CNX_STATE_CONN_FAILED;
	} else {
	    //
	    // Yesss! We've got it(tm)...
	    //
	    _state = CNX_STATE_CONNECTED;
	    return (ConnectionIoMode)
		(getIoMode() | CNX_IO_ATTENTION);
	}
	return getIoMode();
    }
    case CNX_STATE_CONNECTED:
    case CNX_STATE_CONN_FAILED: {
	//
	// We might be called from the cnx manager when a connection becomes
	// readable and writeable when connection establishment fails. Because
	// the receive routine was already called earlier (depending on the
	// cnx's implementation) we just ignore this. Note that we don't
	// indicate attention again (it could otherwise cause problems).
	//
	return (ConnectionIoMode)(getIoMode() & ~CNX_IO_ATTENTION);
    }
    default:
	break;
    } // switch

    //
    // Gotcha: that one took me one full day (much(!) more than eight hours)
    // to find it. It's simply not good first to write the fragment header to
    // the socket and then the body. This will usually result in two packets
    // on the wire and therefore cause large delays due to waiting for ACK's.
    // This effectively slowed down communication quite a lot (with lots of
    // free cpu time...!). The solution: write fragment header and body in
    // one call to write.
    //
    if ( _fragment_state == FRAGMENT_BODY ) {
	//
	// Try to send another part of the fragment's body.
	//
	int rlen = _remaining_len;
	int err;
	if ( !xdrmemstream_write_to_fd(&_xdrs, _fd, &rlen, &err) ) {
	    _remaining_len = rlen;
	    //
	    // We've run into trouble and cought some i/o error. So let's
	    // kill the connection. When playing client we're not allowed
	    // to kill the connection immediately but leave that up to
	    // the user of this connection. We just enter the ready failed
	    // state.
	    //
	    if ( _cnx_type == CNX_TYPE_CLIENT ) {
		_state = CNX_STATE_READY_FAILED;
	    } else {
		_state = CNX_STATE_DEAD;
	    }
	    return (ConnectionIoMode)(getIoMode() | CNX_IO_HAD_TX_ERROR);
	}
	_remaining_len = rlen;
    }

    if ( _remaining_len == 0 ) {
	//
	// We've sent the whole telegramme and can now go back to sleep
	// in the select() call waiting for new data to arrive. In case
	// we are the client, then we need to wait for data to arrive.
	//
	if ( _cnx_type == CNX_TYPE_CLIENT ) {
	    _state          = CNX_STATE_WAITING;
	    _fragment_state = FRAGMENT_HEADER;
	    _remaining_len  = 4;
	    _ptr            = _fragment_header;
	    xdrmemstream_clear(&_xdrs);
	} else {
	    //
	    // Enter the IDLE state for a server side connection.
	    //
	    return reset();
	}
    }
    return getIoMode();
} // KssTCPXDRConnection::send


// ---------------------------------------------------------------------------
// A TCP connection timed out. In case we were receiving data then we're
// flushing the pipe and try to send back an error indication in case we're a
// server connection. Otherwise we'll just beg for attention.
//
KssConnection::ConnectionIoMode KssTCPXDRConnection::timedOut()
{
    if ( _state == CNX_STATE_DEAD ) {
    	//
	// We are not alive anymore...
	//
    	return CNX_IO_DEAD;
    } 

    //
    // The stream just timed out. In case of a connection acting as a
    // client we just ask for attention. We do this by entered the
    // READY_FAILED or CONN_FAILED state. This way, the caller can
    // determine whether the failure happened during the process of
    // opening a connection or during normal i/o (READY_FAILED).
    //
    // In case of a server connection we're just resetting the stream
    // to the idle state. Note that we do not try to reply with a RPC
    // error telegramme in this situation as this could result in a
    // deadlock.
    //
    if ( _cnx_type == CNX_TYPE_CLIENT ) {
	if ( _state == CNX_STATE_CONNECTING ) {
	    _state = CNX_STATE_CONN_FAILED;
	} else {
	    _state = CNX_STATE_READY_FAILED;
	}
    } else {
	_state = CNX_STATE_IDLE;
    }

    //
    // Always reset the stream. At this point the connection can be either
    // idle (thus ready to receive data without timeout) or in some other
    // state (probably CONNECTING or READY) where it doesn't need any i/o.
    // So it's perfectly okay to reset the underlying XDR stream and the
    // fragment automata to a state where they can receive new data.
    //
    xdrmemstream_clear(&_xdrs);
    _fragment_state = FRAGMENT_HEADER;
    _remaining_len  = 4;
    _ptr            = _fragment_header;
    return getIoMode();
} // KssTCPXDRConnection::timedOut


// ---------------------------------------------------------------------------
// Reset a TCP connection. This usually just results in clearing the under-
// lying XDR dynamic memory stream and falling back into the idle mode.
//
KssConnection::ConnectionIoMode KssTCPXDRConnection::reset()
{
    if ( _state == CNX_STATE_DEAD ) {
    	//
	// Can't reset a dead connection.
	//
    	return CNX_IO_DEAD;
    }
 
    switch ( _state ) {
    case CNX_STATE_SENDING:
    case CNX_STATE_WAITING:
    case CNX_STATE_RECEIVING:
	//
	// Enter failure state. Typically, the connection will then be
	// killed under the control of another object. In case of a
	// connection where we are the server side, the active connection
	// had trouble receiving data from a client. In this case we
	// only reset it without making any trouble. In case of sending
	// from the server, we *do* make trouble.
	//
	if ( _cnx_type == CNX_TYPE_CLIENT ) {
	    _state = CNX_STATE_READY_FAILED;
	} else {
	    //
	    // A server connection timing out while in sending state will
	    // return to idle state. When the server connection is in any
	    // other state, it will just enter zombie state, so it can be
	    // either killed completely manually or automatically.
	    //
	    if ( _state == CNX_STATE_SENDING ) {
		_state = CNX_STATE_IDLE;
	    } else {
		_state = CNX_STATE_DEAD;
	    }
	}
	break;
    default:
	//
	// Don't touch the state for a server connection. If we're on
	// the client side, then switch back into the connected state
	// so we're becomming inactive. It is not garantueed that there
	// is really a socket connection still living.
	//
	if ( _cnx_type == CNX_TYPE_CLIENT ) {
	    _state = CNX_STATE_CONNECTED;
	}
    }

    //
    // Always reset the stream. At this point the connection can be either
    // idle (thus ready to receive data without timeout) or in some other
    // state (probably CONNECTING or READY) where it doesn't need any i/o.
    // So it's perfectly okay to reset the underlying XDR stream and the
    // fragment automata to a state where they can receive new data.
    //
    xdrmemstream_clear(&_xdrs);
    _fragment_state = FRAGMENT_HEADER;
    _remaining_len  = 4;
    _ptr            = _fragment_header;
    return getIoMode();
} // KssTCPXDRConnection::reset


#endif /* PLT_USE_BUFFERED_STREAMS */

/* End of xdrtcpcon.cpp */
