/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/src/xdrudpcon.cpp,v 1.16 2007-04-25 12:57:21 martin Exp $ */
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
 * xdrudpcon.cpp -- Implements an UDP connection with XDR data as its
 *                  data representation for the mess we'll transport.
 *
 * Written by Harald Albrecht <harald@plt.rwth-aachen.de>
 */

#if PLT_USE_BUFFERED_STREAMS

#include "ks/xdrudpcon.h"
#include "ks/connectionmgr.h"
#include "plt/time.h"

#if PLT_SYSTEM_NT

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
// Constructs an UDP connection with XDR for data representation. After con-
// structing such a connection, check the instance's state for CNX_STATE_DEAD,
// as this indicates a failure to initialize the object.
// Note that the retrytimeout parameter is used only for client connections.
//
KssUDPXDRConnection::KssUDPXDRConnection(int fd, 
					 unsigned long timeout,
					 unsigned long retrytimeout,
	                                 struct sockaddr_in *clientAddr,
                                         int clientAddrLen,
                                         ConnectionType type,
					 unsigned long buffsize)
    : KssXDRConnection(fd, false, 
		       (type == CNX_TYPE_CLIENT) ? retrytimeout : timeout, 
		       type),
      _total_timeout(timeout),
      _buffer_size(buffsize),
      _recvbuffer(0), _sendbuffer(0)
{
    if ( _buffer_size < 1024 ) {
    	_buffer_size = 1024;
    }
    if ( _buffer_size > 65500 ) {
    	_buffer_size = 65500;
    }

    if ( clientAddr && clientAddrLen ) {
        _client_address = *clientAddr;
	_client_address_len = clientAddrLen;
    }

    //
    // Now set up a XDR (static) memory stream which will be used for
    // receiving and sending UDP datagrams. Then, put the udp socket
    // into the non-blocking mode (this makes the whole thing more
    // robust against arriving potential trouble between select() and
    // read() or write().
    //
    _recvbuffer = new char[_buffer_size];
    _sendbuffer = new char[_buffer_size];
    if ( _recvbuffer && _sendbuffer ) {
    	xdrmem_create(&_xdrs, (caddr_t) _recvbuffer, _buffer_size, 
		      XDR_DECODE);
    	_cleanup_xdr_stream = true;
	
	if ( !makeNonblocking() ) {
	    _state = CNX_STATE_DEAD;
	} else {
	    if ( type == CNX_TYPE_CLIENT ) {
		PltTime jetzt = PltTime::now();
		_rpc_header._xid = getpid() ^ jetzt.tv_sec ^ jetzt.tv_usec;
		//
		// We immitate the connection behaviour found for TCP/IP
		// connection, where the connecting procedure can take quite
		// some time. With UDP/IP there's no connecting procedure,
		// but we switch into this mode and cause a attention request
		// nevertheless, so TCP and UDP connections behave similiar.
		//
		_state = CNX_STATE_CONNECTED;
	    }
	}
    } else {
    	_state = CNX_STATE_DEAD;
    }
} // KssUDPXDRConnection::KssUDPXDRConnection


// ---------------------------------------------------------------------------
// Clean up the mess we've once allocated...
//
KssUDPXDRConnection::~KssUDPXDRConnection()
{
    if ( _recvbuffer ) {
	delete [] _recvbuffer; _recvbuffer = 0;
    }
    if ( _sendbuffer ) {
	delete [] _sendbuffer; _sendbuffer = 0;
    }
} // KssUDPXDRConnection::~KssUDPXDRConnection


// ---------------------------------------------------------------------------
// Ask an UDP XDR connection about its i/o mode: this way, a connection
// manager can find out in what fdsets it must put the connection.
//
KssConnection::ConnectionIoMode KssUDPXDRConnection::getIoMode() const
{
    switch ( _state ) {
    case CNX_STATE_DEAD:
	return CNX_IO_DEAD;
    case CNX_STATE_IDLE:
	return CNX_IO_READABLE;
    case CNX_STATE_WAITING:
	return (ConnectionIoMode)(CNX_IO_READABLE |
				  CNX_IO_NEED_TIMEOUT);
    case CNX_STATE_PASSIVE:
	return CNX_IO_DORMANT;
    case CNX_STATE_READY:
	return (ConnectionIoMode)(CNX_IO_DORMANT | CNX_IO_ATTENTION);
    case CNX_STATE_READY_FAILED:
	//
	// A reply didn't came in in time. So we're just asking for attention.
	//
	return (ConnectionIoMode)
	    (CNX_IO_DORMANT | CNX_IO_ATTENTION);
    case CNX_STATE_CONNECTED:
	//
	// Will happen as soon as this connection is created. It is just
	// a virtual connect, well: a fake.
	//
	return (ConnectionIoMode)(CNX_IO_DORMANT | CNX_IO_ATTENTION);
    case CNX_STATE_SENDING:
	//
	// This connection is actively sending a request or a reply (server
	// or client side).
	//
	return CNX_IO_WRITEABLE;
    default:
	return CNX_IO_READABLE;
    }
} // KssUDPXDRConnection::getIoMode


// ---------------------------------------------------------------------------
// Try to receive an UDP datagram and make sure it has at least some bytes in
// it. If this was successful, then this connection will go into the state
// CNX_STATE_READY and signal for CNX_STATE_ATTENTION. Otherwise you will
// probably see a CNX_STATE_HAD_ERROR. If there was some trouble and the
// socket signalled a possible blocking condition, then the connection will
// remain in the CNX_STATE_IDLE state.
//
KssConnection::ConnectionIoMode KssUDPXDRConnection::receive()
{
    int received;
#if PLT_USE_XTI
    int  flags;
    bool flush = false;
#endif

    //
    // First, we don't want to deal with dead connections -- it just makes
    // no fun. And we can work only on idle connections...
    //
    if ( _state == CNX_STATE_DEAD ) {
    	return CNX_IO_DEAD;
    }
    if ( (_state != CNX_STATE_IDLE) && (_state != CNX_STATE_WAITING) ) {
    	return reset();
    }
    //
    // reset the read pointer of the underlaying XDR stream back to the
    // start of the buffer.
    //
    _xdrs.x_op = XDR_DECODE;
    XDR_SETPOS(&_xdrs, 0);

    for ( ;; ) {
	if ( _cnx_type == CNX_TYPE_SERVER ) {
#if !PLT_USE_XTI
    	    _client_address_len = sizeof(_client_address);
	    received = recvfrom(_fd, 
#if PLT_SYSTEM_NT
                                (char *)
#endif
                                _recvbuffer, 
                                _buffer_size,
		        	0, // no special flags
	                	(struct sockaddr *) &_client_address,
		        	&_client_address_len);
#else
    	    struct t_unitdata udta;
    	    _client_address_len = sizeof(_client_address);
	    udta.addr.maxlen  = _client_address_len;
	    udta.addr.buf     = (char *) &_client_address;
	    udta.opt.maxlen   = 0;
	    udta.udata.maxlen = _buffer_size;
	    udta.udata.buf    = (char *) _recvbuffer;
	    received = t_rcvudata(_fd, &udta, &flags);
	    if ( received == 0 ) {
	    	//
		// Make this piece of code behave like the socket functions:
		// return number of bytes read or -1 as an error indication.
		//
	    	received = udta.udata.len;
	    }
#endif
	} else {
	    //
	    // In case of a client-side connection, don´t overwrite the
	    // _client_address as it contains the address of the server.
	    // To be compatible with Sun´s ONC/RPC we´re just ignoring
	    // the sender´s IP address -- well, I don´t like this idea
	    // very much...
	    //
	    sockaddr dummy_addr;
#if defined(PLT_RUNTIME_GLIBC) && (PLT_RUNTIME_GLIBC >= 0x20001)
            socklen_t    dummy_addr_len;
#elif PLT_SYSTEM_OPENVMS
            unsigned int dummy_addr_len;
#else
            int          dummy_addr_len;
#endif
	    dummy_addr_len = sizeof(dummy_addr);

#if !PLT_USE_XTI
	    received = recvfrom(_fd, 
#if PLT_SYSTEM_NT
                                (char *)
#endif
                                _recvbuffer, 
                                _buffer_size,
		        	0, // no special flags
		    	        &dummy_addr,
				&dummy_addr_len);
#else
    	    struct t_unitdata udta;
	    udta.addr.maxlen  = 0; // Not interested in the sender's
	    udta.opt.maxlen   = 0; // address...
	    udta.udata.maxlen = _buffer_size;
	    udta.udata.len    = 0;
	    udta.udata.buf    = (char *) _recvbuffer;
	    received = t_rcvudata(_fd, &udta, &flags);
	    if ( received == 0 ) {
	    	// Same as above...
	    	received = udta.udata.len;
	    }
#endif
	}
	if ( received < 0 ) {
	    //
	    // An error occured, so find out what has happened. In case of
	    // an interrupted call, let's try again... For all other errors
	    // ignore them silently and remain in the idle state.
	    //
#if PLT_SYSTEM_NT
    	    int myerrno;
	    myerrno = WSAGetLastError(); /* MSVC++ 4.2 */
#else
            int myerrno = errno;
#endif
#if PLT_USE_XTI
    	    //
	    // Make sure that a pending event on the XTI endpoint is read.
	    //
    	    switch ( t_errno ) {
	    case TLOOK: {
	    	int events = t_look(_fd);
		if ( events & T_UDERR ) {
		    t_rcvuderr(_fd, 0); // clear error for previous datagram
		}
	    	if ( events & T_DATA ) {
		    continue; // next try...
		}
		break; // fall through and return an io error indication
		}
	    case TNODATA:
	    	return getIoMode();
	    case TSYSERR:
	    	break; // leave the XTI stuff and let the code below handle it
	    default:
		return getIoMode();
	    }
#endif
	    switch ( myerrno ) {
	    case EINTR:
		continue;
	    case EWOULDBLOCK:
	    	return getIoMode();
	    }
#if PLT_USE_XTI
	} else if ( flags & T_MORE ) {
	    //
	    // When flushing and there is more data to read, then just
	    // take the next loop slurping in the data...
	    //
	    flush = true;
	    continue;
    	} else if ( flush ) {
	    //
	    // Silently ignore this oversized packet and just remember one
	    // more io error by falling out of the loop.
	    //
	    break;
#endif
	} else if ( received >= 4 ) {
	    //
	    // There seems to be enough data to process at least the first
	    // entry of the header, so report a ready request. Remember that
	    // the stream already has been reset before entering the for loop.
	    // In case of a client-side connection, make sure that this is a
	    // valid answer.
	    //
	    if ( _cnx_type == CNX_TYPE_CLIENT ) {
		long *ppp = (long *) _recvbuffer;
		if ( ((u_long) IXDR_GET_LONG(ppp)) != 
		         _rpc_header._xid ) {
		    //
		    // try once again next time as this reply had the wrong
		    // transaction id.
		    //
		    return getIoMode();
		}
		if ( !_rpc_header.xdrDecode(&_xdrs) ) {
	    	    return (ConnectionIoMode)(getIoMode() | CNX_IO_HAD_ERROR);
		}
	    } else {
		if ( !_rpc_header.xdrDecode(&_xdrs) ) {
		    _rpc_header.setDecodeError();
		    _state = CNX_STATE_SENDING;
	    	    return (ConnectionIoMode)(getIoMode() | CNX_IO_HAD_ERROR);
		}
	    }
    	    _state = CNX_STATE_READY;
	    return getIoMode();
	}
	break; // some error...
    }
    return (ConnectionIoMode)(getIoMode() | CNX_IO_HAD_RX_ERROR);
} // KssUDPXDRConnection::receive


// ---------------------------------------------------------------------------
// Try to get rid of an UDP datagram by sending it to someone out there on the
// Internet. If this was successful, then this connection will go into the
// CNX_STATE_IDLE. Otherwise you will probably see a CNX_STATE_HAD_ERROR. If
// there was some trouble and the socket signalled a possible blocking
// condition, then the connection will remain in the CNX_STATE_SENDING state.
//
KssConnection::ConnectionIoMode KssUDPXDRConnection::send()
{
    int sent;
    
    //
    // First, we don't want to deal with dead connections -- it just makes
    // no fun.
    //
    if ( _state == CNX_STATE_DEAD ) {
    	return CNX_IO_DEAD;
    }
    if ( _state != CNX_STATE_SENDING ) {
    	return reset();
    }
    
    for ( ;; ) {
#if !PLT_USE_XTI
	sent = sendto(_fd, 
#if PLT_SYSTEM_NT
                      (char *)
#endif
                      _sendbuffer, 
                      _tosend,
		      0,
		      (struct sockaddr *) &_client_address,
		      _client_address_len);
#else
    	    struct t_unitdata udta;
	    udta.addr.len     = _client_address_len;
	    udta.addr.buf     = (char *) &_client_address;
	    udta.opt.maxlen   = 0;
	    udta.udata.len    = tosend;
	    udta.udata.buf    = (char *) _sendbuffer;
	    sent = t_sndudata(_fd, &udta);
#endif
	if ( sent < 0 ) {
#if PLT_SYSTEM_NT
    	    int myerrno;
	    myerrno = WSAGetLastError(); /* MSVC++ 4.2 */
#else
            int myerrno = errno;
#endif
#if PLT_USE_XTI
    	    //
	    // Make sure that a pending event on the XTI endpoint is read.
	    //
    	    switch ( t_errno ) {
	    case TLOOK: {
	    	int events = t_look(_fd);
		if ( events & T_UDERR ) {
		    t_rcvuderr(_fd, 0); // clear error for previous datagram
		    continue;
		}
		break; // fall through and return an io error indication
		}
	    case TFLOW:
	    	continue;
	    case TSYSERR:
	    	break; // leave the XTI stuff and let the code below handle it
	    default:
		return getIoMode();
	    }
#endif
	    switch ( myerrno ) {
	    case EINTR:
		continue;
	    case EWOULDBLOCK:
	    	return getIoMode();
	    default:
	    	return (ConnectionIoMode)(reset() | CNX_IO_HAD_TX_ERROR);
	    }
	}
    	//
    	// Go back into the idle mode and don't care any more about this
    	// datagram.
    	//
    	_state = _cnx_type == CNX_TYPE_SERVER ? 
		                  CNX_STATE_IDLE : CNX_STATE_WAITING;
	break;
    }
    return getIoMode();
} // KssUDPXDRConnection::send


// ---------------------------------------------------------------------------
// Reset this connection to a well-known state, which is in our case the
// CNX_STATE_IDLE state where we'll wait for arriving datagrams.
//
KssConnection::ConnectionIoMode KssUDPXDRConnection::reset()
{
    if ( _state == CNX_STATE_DEAD ) {
    	//
	// Can't reset a dead connection.
	//
    	return CNX_IO_DEAD;
    } else {
    	_state = _cnx_type == CNX_TYPE_SERVER ? 
		                  CNX_STATE_IDLE : CNX_STATE_PASSIVE;
    	return getIoMode();
    }
} // KssUDPXDRConnection::reset


// ---------------------------------------------------------------------------
// Handle timeouts on this UDP "connection". Timeouts can only happen while
// we're playing the role of a client connection and we've sent a request and
// are now waiting for a reply.
//
KssConnection::ConnectionIoMode KssUDPXDRConnection::timedOut()
{
    if ( _cnx_type == CNX_TYPE_CLIENT ) {
	if ( (_time_passed += _timeout) >= _total_timeout ) {
	    //
	    // We've reached the total timeout. So we enter attention mode
	    // and signal a timeout.
	    //
	    _state = CNX_STATE_READY_FAILED;
	} else {
	    //
	    // The total timeout hasn't been reached yet. So take another
	    // round and try again. This involves sending the request again
	    // and waiting for an answer once more.
	    //
	    _state = CNX_STATE_SENDING;
	}
	return getIoMode();
    } else {
	//
	// In case it's a server connection, just reset and don't be afraid...
	//
	return reset();
    }
} // KssUDPXDRConnection::timedOut


// ---------------------------------------------------------------------------
//
void KssUDPXDRConnection::sendPingReply()
{
    if ( _state != CNX_STATE_DEAD ) {
	_xdrs.x_op = XDR_ENCODE;
	XDR_SETPOS(&_xdrs, 0);
	_rpc_header.acceptCall();
	if ( _rpc_header.xdrEncode(&_xdrs) ) {
	    _tosend = (int) XDR_GETPOS(&_xdrs);
	    memcpy(_sendbuffer, _recvbuffer, _buffer_size);
	    _time_passed = 0;
    	    _state = CNX_STATE_SENDING;
	} else {
    	    reset();
	}
    }    
} // KssUDPXDRConnection::sendPingReply


// ---------------------------------------------------------------------------
//
void KssUDPXDRConnection::sendErrorReply(KsAvTicket &avt, KS_RESULT error)
{
    if ( _state != CNX_STATE_DEAD ) {
    	u_long e = error == KS_ERR_OK ? KS_ERR_GENERIC : error;
	_xdrs.x_op = XDR_ENCODE;
	XDR_SETPOS(&_xdrs, 0);
	_rpc_header.acceptCall();
	if ( _rpc_header.xdrEncode(&_xdrs) &&
	     avt.xdrEncode(&_xdrs) &&
	     xdr_u_long(&_xdrs, &e) &&
	     avt.xdrEncodeTrailer(&_xdrs) ) {
	    _tosend = (int) XDR_GETPOS(&_xdrs);
	    memcpy(_sendbuffer, _recvbuffer, _buffer_size);
	    _time_passed = 0;
    	    _state = CNX_STATE_SENDING;
	} else {
    	    reset();
	}
    }    
} // KssUDPXDRConnection::sendErrorReply


// ---------------------------------------------------------------------------
//
void KssUDPXDRConnection::sendReply(KsAvTicket &avt, KsResult &result)
{
    if ( _state != CNX_STATE_DEAD ) {
	_xdrs.x_op = XDR_ENCODE;
	XDR_SETPOS(&_xdrs, 0);
	_rpc_header.acceptCall();
	if ( _rpc_header.xdrEncode(&_xdrs) &&
	     avt.xdrEncode(&_xdrs) &&
	     result.xdrEncode(&_xdrs) &&
	     avt.xdrEncodeTrailer(&_xdrs) ) {
	    _tosend = (int) XDR_GETPOS(&_xdrs);
	    memcpy(_sendbuffer, _recvbuffer, _buffer_size);
	    _time_passed = 0;
    	    _state = CNX_STATE_SENDING;
	} else {
    	    reset();
	}
    }    
} // KssUDPXDRConnection::sendReply


// ---------------------------------------------------------------------------
//
void KssUDPXDRConnection::personaNonGrata()
{
    reset();
} // KssUDPXDRConnection::personaNonGrata


// ---------------------------------------------------------------------------
//
bool KssUDPXDRConnection::beginRequest(u_long xid,
				       u_long prog_number, u_long prog_version,
				       u_long proc_number)
{
    if ( _state == CNX_STATE_DEAD ) {
	return false;
    }
    //
    // reset the read pointer of the underlaying XDR stream back to the
    // start of the buffer and fill in request header.
    //
    _xdrs.x_op = XDR_ENCODE;
    XDR_SETPOS(&_xdrs, 0);
    _state = CNX_STATE_PASSIVE;
    _rpc_header.setRequest(xid, prog_number, prog_version, proc_number);
    return _rpc_header.xdrEncode(&_xdrs);
} // KssUDPXDRConnection::beginRequest


// ---------------------------------------------------------------------------
//
void KssUDPXDRConnection::sendRequest()
{
    if ( _state != CNX_STATE_DEAD ) {
	_tosend = (int) XDR_GETPOS(&_xdrs);
	memcpy(_sendbuffer, _recvbuffer, _buffer_size);
	_time_passed = 0;
    	_state = CNX_STATE_SENDING;
    }
} // KssUDPXDRConnection::sendRequest


#endif /* PLT_USE_BUFFERED_STREAMS */

/* End of xdrudpcon.cpp */
