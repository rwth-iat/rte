/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/src/xdrtcpcon.cpp,v 1.5 1999-02-22 15:11:45 harald Exp $ */
/*
 * Copyright (c) 1998, 1999
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
 * xdrtcpcon.cpp -- Implements an TCP connection with XDR data as its
 *                  data representation for the mess we'll transport.
 *
 * Written by Harald Albrecht <harald@plt.rwth-aachen.de>
 */

//#define CNXDEBUG


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
bool KssListenTCPXDRConnection::beginRequest()
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
#if !PLT_SYSTEM_OPENVMS
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
        // FIXME: !con
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
// And a TCP rendevouser can´t get reset -- it´s always "reset" in some
// respect.
//
KssConnection::ConnectionIoMode KssListenTCPXDRConnection::reset(bool)
{
    return getIoMode();
} // KssListenTCPXDRConnection::reset


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
    if ( !xdrmemstream_create(&_xdrs) ) {
	_state = CNX_STATE_DEAD;
	return;
    }
    if ( !makeNonblocking() || !enableKeepAlive() ) {
	_state = CNX_STATE_DEAD;
	return;
    }
    _state          = CNX_STATE_IDLE;
    _fragment_state = FRAGMENT_HEADER;
    _remaining_len  = 4;
    _ptr            = _fragment_header;

    _client_address = clientAddr;
    _client_address_len = clientAddrLen;
} // KssTCPXDRConnection::KssTCPXDRConnection

      
// ---------------------------------------------------------------------------
// Indicate the i/o mode this TCP connection is currently in. This way the
// connection manager can make sure that this connection feels well.
//
KssConnection::ConnectionIoMode KssTCPXDRConnection::getIoMode() const
{
    switch ( _state ) {
    case CNX_STATE_DEAD:
	return CNX_IO_DEAD;
    case CNX_STATE_IDLE:
	return CNX_IO_READABLE;
    case CNX_STATE_READY:
	return (ConnectionIoMode)
	    (CNX_IO_DORMANT | CNX_IO_ATTENTION);
    case CNX_STATE_RECEIVING:
	return (ConnectionIoMode)
	    (CNX_IO_READABLE | CNX_IO_NEED_TIMEOUT);
    case CNX_STATE_SENDING:
	return (ConnectionIoMode)
	    (CNX_IO_WRITEABLE | CNX_IO_NEED_TIMEOUT);
    default:
	return CNX_IO_READABLE;
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
    long *ppp       = xdr_inline(&_xdrs, 4);
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
    	reset(false);
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
    	    reset(false);
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
	u_long dummy = 0x80000000ul;       // Make sure there´s room for the
	if ( xdr_u_long(&_xdrs, &dummy) && // fragment header we´ll fix later...
    	     _rpc_header.xdrEncode(&_xdrs) &&
	     avt.xdrEncode(&_xdrs) &&
	     result.xdrEncode(&_xdrs) &&
	     avt.xdrEncodeTrailer(&_xdrs) ) {
    	    enterSendingState();
	} else {
    	    reset(false);
	}
    }    
} // KssTCPXDRConnection::sendReply


// ---------------------------------------------------------------------------
// With unwanted clients we just put the connection into the dead mode, thus
// it will be cleaned up as soon as it is to be reactivated by the connection
// manager.
//
void KssTCPXDRConnection::personaNonGrata()
{
    reset(false);
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
		// the stage...
	    	//
	    	_state = CNX_STATE_DEAD;
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
    	    	int myerrno = WSAGetLastError();
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
			    _state = CNX_STATE_DEAD;
			    return getIoMode();
			}
		    }
		    break; // report rx error...
		case EWOULDBLOCK:
		    return getIoMode();
		}
	    	_state = CNX_STATE_DEAD;
	    	return (ConnectionIoMode)(getIoMode() | CNX_IO_HAD_RX_ERROR);
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
	    long *ppp = (long *) _fragment_header;
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
	    if ( err == ENOMEM ) {
	    	// TODO!!!
		return reset(false); // flush all incomming data...
	    } else {
		_state = CNX_STATE_DEAD;
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
	// send back an error reply. If -- for some reason -- we can´t
	// serialize the error reply header, then we´ll send NO reply
	// at all but flag at least an error.
	//
	xdrmemstream_rewind(&_xdrs, XDR_DECODE);
	if ( !_rpc_header.xdrDecode(&_xdrs) ) {
	    _rpc_header.setDecodeError();
	    _state = CNX_STATE_SENDING;
	    xdrmemstream_clear(&_xdrs);
	    if ( !_rpc_header.xdrEncode(&_xdrs) ) {
		return (ConnectionIoMode)(reset(false) | CNX_IO_HAD_ERROR);
	    }
	    return (ConnectionIoMode)(enterSendingState() | CNX_IO_HAD_ERROR);
	}
	_state = CNX_STATE_READY;
    } else {
	_state = CNX_STATE_RECEIVING;
    }
    return getIoMode();
} // KssTCPXDRConnection::receive


// ---------------------------------------------------------------------------
// Send back a RPC telegramme to a client. Please note a peculiarity here:
// we´re always sending back the answer as *one* telegramme fragment, which
// is just fine.
//
KssConnection::ConnectionIoMode KssTCPXDRConnection::send()
{
    //
    // Gotcha: that one took me one full day (much(!) more than eight hours)
    // to find it. It´s simply not good first to write the fragment header to
    // the socket and then the body. This will usually result in two packets
    // on the wire and therefore cause large delays due to waiting for ACK´s.
    // This effectively slowed down communication quite a lot (with lots of
    // free cpu time...!). The solution: write fragment header and body in
    // one call to write.
    //
    if ( _fragment_state == FRAGMENT_BODY ) {
	//
	// Try to send another part of the fragment´s body.
	//
	int rlen = _remaining_len;
	int err;
	if ( !xdrmemstream_write_to_fd(&_xdrs, _fd, &rlen, &err) ) {
	    _remaining_len = rlen;
	    //
	    // We´ve run into trouble and cought some i/o error. So let´s
	    // kill the connection.
	    //
	    _state = CNX_STATE_DEAD;
	    return (ConnectionIoMode)(getIoMode() | CNX_IO_HAD_TX_ERROR);
	}
	_remaining_len = rlen;
    }

    if ( _remaining_len == 0 ) {
	//
	// We´ve sent the whole telegramme and can now go back to sleep
	// in the select() call waiting for new data to arrive.
	//
	return reset(false);
    }
    return getIoMode();
} // KssTCPXDRConnection::send


// ---------------------------------------------------------------------------
// Reset a TCP connection. This usually just results in clearing the under-
// lying XDR dynamic memory stream and falling back into the idle mode. In
// case we were receiving data and caught a timeout then we´re first flushing
// the pipe and try to send back an error indication.
//
KssConnection::ConnectionIoMode KssTCPXDRConnection::reset(bool hadTimeout)
{
    if ( _state == CNX_STATE_DEAD ) {
    	//
	// Can't reset a dead connection.
	//
    	return CNX_IO_DEAD;
    } else if ( !hadTimeout && (_state == CNX_STATE_RECEIVING) ) {
	//
	// We had some trouble on an active connection receiving data from
	// a client. So we´re just closing the connection to prevent any
	// dead-locks.
	//
	_state = CNX_STATE_DEAD;
    } else {
	//
	// The stream either timed out so we´re just resetting the stream
	// to the idle state. Especially for a timeout when receiving a
	// request we do not try to reply as this might result in a deadlock
	// situation.
	//
	xdrmemstream_clear(&_xdrs);
    	_state          = CNX_STATE_IDLE;
	_fragment_state = FRAGMENT_HEADER;
	_remaining_len  = 4;
	_ptr            = _fragment_header;
    }
    return getIoMode();
} // KssTCPXDRConnection::reset


#endif /* PLT_USE_BUFFERED_STREAMS */

/* End of xdrtcpcon.cpp */
