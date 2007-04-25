/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/src/connection.cpp,v 1.14 2007-04-25 12:57:20 martin Exp $ */
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
 * connection.cpp -- Abstract base class for I/O connections. This module
 *                   also contains the abstraction of a XDR connection.
 *
 * Written by Harald Albrecht <harald@plt.rwth-aachen.de>
 */

#include "ks/connection.h"


#if !PLT_SYSTEM_NT

#include <unistd.h>
#include <errno.h>

#if !PLT_SYSTEM_OPENVMS

#include <fcntl.h>

#else

//
// OpenVMS idiosyncrasies. Well, OpenVMS 6.2 is really "open" at this point.
// Gosh. What a rubbish.
//
#include <ioctl.h>
extern int ioctl(int d, int request, char *argp);

#endif

#endif


// ---------------------------------------------------------------------------
// Construct a new connection from a given file descriptor. A connection is
// ...well... just a connection, not necessarily used by ACPLT/KS or even
// ONC/RPC. Instead a connection to the ACPLT/KS server is just an i/o pipe
// connected so some partner. All instances of connections have their own
// file descriptor (NT looses to some extend, because it has problems to mix
// i/o channels, which are often fake file descriptors), an associated default
// timeout, and most important a type. The type indicates, whether this side
// of a connection plays the rule of the server or client. For example, the
// ACPLT/KS transports receiving requests are playing on the server side.
// Note that a fresh connection doesn't has a client address (well, this is
// rather the address of the peer at the other end of the connection), as
// well as a attention method. If a connection needs attention, then you have
// to set an attention handler later.
//
KssConnection::KssConnection(int fd, bool autoDestroyable,
                             unsigned long timeout,
			     ConnectionType type)
    : _cnx_type(type), _timeout(timeout),
      _auto_destroyable(autoDestroyable),
      _fd(fd),
      _client_address_len(0),
      _manager(0), // this is set later when putting this connection under
                   // control of the connection manager.
      _attention_partner(0)
{
    //
    // In case of a connection playing the server side, it starts out in the
    // idle state, ready to receive requests without any timeout pending. The
    // timeout will only be set later after the first byte of a request has
    // been seen. In case of being on the client side, a connection starts
    // in the passive state, because there's no i/o do be done yet at all.
    //
    _state = _cnx_type == CNX_TYPE_SERVER ? CNX_STATE_IDLE : CNX_STATE_PASSIVE;
} // KssConnection::KssConnection


// ---------------------------------------------------------------------------
// Set the IP address of the communication partner at the other end of the
// connection. This is necessary, so UDP-based connections can send their
// data to the appropriate peer and receive data only from the right peer.
//
bool KssConnection::setPeerAddr(struct sockaddr_in *addr, int addrLen)
{
    if ( addrLen <= (int) sizeof(_client_address) ) {
	memcpy(&_client_address, addr, addrLen);
	_client_address_len = addrLen;
	return true;
    } else {
	return false;
    }
} // KssConnection::setPeerAddr


// ---------------------------------------------------------------------------
// For this given connection, find out to what port the connection has been
// bound. Of course, calling this method only makes sense, if it's using the
// IP protocols.
//
#if PLT_USE_XTI && PLT_SYSTEM_SOLARIS
extern int t_getname(int fd, struct netbuf *namep, int type);
#endif

u_short KssConnection::getPort() const
{
    struct sockaddr_in addr;
#if defined(PLT_RUNTIME_GLIBC) && PLT_RUNTIME_GLIBC >= 0x2001
    socklen_t          addr_len;
#elif !PLT_SYSTEM_OPENVMS
    int                addr_len;
#else
    unsigned           addr_len;
#endif
    
    addr_len = sizeof(addr);
#if !PLT_USE_XTI
    if ( (getsockname(_fd, (struct sockaddr *) &addr, &addr_len) < 0) ||
         (addr.sin_family != AF_INET) ) {
    	return 0;
    }
#else
#if PLT_SYSTEM_SOLARIS
    //
    // This hack is for Solaris with TLI instead of XTI (SunOS 2.4 etc)
    //
    // Credit where credit is due: Richard Stevens saved my day once more again.
    // I asked in comp.protocols.tcp-ip for this undocumented function prototype
    // and he helped me out within the time zone difference between Europe and
    // the United States...
    //
    struct netbuf local;
    
    local.maxlen = sizeof(addr);
    local.buf    = (char *) &addr;
    if ( (t_getname(_fd, &local, LOCALNAME) < 0) ||
         (addr.sin_family != AF_INET) ) {
    	return 0;
    }
#else
    struct t_bind local, peer;
    
    local.addr.maxlen = addr_len;
    local.addr.buf    = (char *) &addr;
    peer.addr.maxlen  = 0;
    
    if ( t_getprotaddr(_fd, &local, &peer) < 0 ) {
    	return 0;
    }
    if ( (local.addr.len != addr_len) ||
    	 (addr.sin_familiy != AF_INET) ) {
    	return 0;
    }
#endif
#endif
    return ntohs(addr.sin_port);
} //  KssConnection::getPort


// ---------------------------------------------------------------------------
// Control timeouts. These are just default implementations which can be
// reimplemented in derived classes. The only real important accessor here is
// getTimeout(), as it is needed by the connection manager to calculate
// timeouts.
//
long KssConnection::getTimeout() const
{
    return _timeout;
} // KssConnection::getTimeout


void KssConnection::setTimeout(unsigned long timeout)
{
    _timeout = timeout;
} // KssConnection::setTimeout


// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// And now for the second important type of connections which are based on the
// ONC/XDR data type representation.
//

// ---------------------------------------------------------------------------
// A XDR-based connection is like a KssConnection, but in addition it has an
// associated xdr stream. While the specific kind of xdr stream is up to a
// derived class, this class has a flag (_cleanup_xdr_stream) whether it
// should clean up the xdr stream using xdr_destroy() when it's time for this
// instance to bite the bullet.
//
KssXDRConnection::KssXDRConnection(int fd, bool autoDestroyable, 
	                           unsigned long timeout,
	                           ConnectionType type)
    : KssConnection(fd, autoDestroyable, timeout, type),
      _cleanup_xdr_stream(true)
{
} // KssXDRConnection::KssXDRConnection


// ---------------------------------------------------------------------------
// With XDR-based connections we make sure that the XDR stream is destroyed
// when the connection is shut down. In case the xdr stream can't be destroyed
// with the usual xdr_destroy(), this can be inhibited by setting the
// _cleanup_xdr_stream flag to false. You are then responsible to clean up the
// xdr mess you've made yourself. 
//
// NOTE: you need to shutdown() a connection first, before you can destroy the
// connection object. These seperate steps are necessary because we may need
// to reimplement how to close a connection in a derived class. But we can not
// do this in a destructor, because there then would be multiple attempts to
// close the file description: from the destructor of the derived class as well
// as from the destructor of the base class.
//
void KssXDRConnection::shutdown()
{
    if ( _cleanup_xdr_stream ) {
    	xdr_destroy(&_xdrs);
	_cleanup_xdr_stream = false;
    }
    //
    // ...and in the following lines you can see so-called "advanced" operating
    // systems to fail miserably in the field of integrating all kind of i/o
    // through file descriptors. Only Unix handles them all in a uniform manner.
    // Way to go, NT...
    //
#if PLT_SYSTEM_NT
    closesocket(_fd); // Ouch!!!
#elif PLT_USE_XTI
    t_close(_fd); // one more "Ouch!!!"
#else
    close(_fd);
#endif
    //
    // Make sure, the connection is in the dead state, from which it can't be
    // reactivated any more.
    //
    _state = CNX_STATE_DEAD;
} // KssXDRConnection::shutdown


// ---------------------------------------------------------------------------
// This method allows a XDR connection to free allocated buffer memory after
// a request has been received completely. So the request will not eat up
// precious buffer memory when it has already been deserialzed and thus
// converted into C++ objects. This method also gives the A/V tickets a
// chance to read in their trailer part and check the permissions.
//
KS_RESULT KssXDRConnection::finishRequestDeserialization(KsAvTicket &avt, bool)
{
    if ( !avt.xdrDecodeTrailer(&_xdrs) ) {
    	return KS_ERR_GENERIC;
    }
    freeStreamMemory();
    return avt.result(); // ask ticket for permission...
} // KssXDRConnection::finishRequestDeserialization


// ---------------------------------------------------------------------------
// Put a socket into non-blocking mode. This encapsulates differences between
// various operating systems. Well, "NT" might be an operating system. But
// then, who knows (no Bill, you don't know what an operating system is, you
// only know how to operate your multi-Bill-ion dollar company on the expense
// of your users).
//
bool KssXDRConnection::makeNonblocking()
{
#if PLT_SYSTEM_NT
    u_long nbmode = 1;
    return ioctlsocket(_fd, FIONBIO, &nbmode) != SOCKET_ERROR;
#elif PLT_SYSTEM_OPENVMS
    int nbmode = 1;
    return ioctl(_fd, FIONBIO, (char *) &nbmode) != -1;
#else
    //
    // This works even with XTI...
    //
    int mode = fcntl(_fd, F_GETFL);
    if ( mode == -1 ) {
    	return false;
    }
    mode |= O_NONBLOCK;
    return fcntl(_fd, F_SETFL, mode) != -1;
#endif
} // KssXDRConnection::makeNonblocking


// ---------------------------------------------------------------------------
// For connections (esp. on the server side) you can enable the keep alive
// mode in order to detect dead connections and drop such suckers.
//
bool KssXDRConnection::enableKeepAlive()
{
#if !PLT_USE_XTI
    int mode = 1; // enable KEEP ALIVE
#if PLT_SYSTEM_NT
    return setsockopt(_fd, SOL_SOCKET, SO_KEEPALIVE, 
                      (char *) &mode, sizeof(mode))
	       != -1;
#else
    return setsockopt(_fd, SOL_SOCKET, SO_KEEPALIVE, 
                      (char *) &mode, sizeof(mode))
	       != -1;
#endif
#else
#if 0
    struct t_kpalive {
    	t_scalar_t kp_onoff;
	t_scalar_t kp_timeout;
    };
    struct t_opthdr

    struct t_optmgmt request, result;
    struct {
    	struct t_opthdr  header;
	struct t_kpalive keepalive;
    } reqopt, resopt;
    
    request.opt.len           = sizeof(reqopt);
    request.opt.maxlen        = request.len;
    request.opt.buf           = &reqopt;
    request.flags             = T_NEGOTIATE;
    
    reqopt.header.len         = sizeof(header) + sizeof(keepalive);
    reqopt.header.level       = T_INET_TCP;
    reqopt.header.name        = T_TCP_KEEPALIVE;
    reqopt.keepalive.l_onoff  = T_YES;
    reqopt.keepalive.l_linger = T_UNSPEC;
    
    result.opt.len            = sizeof(header) + sizeof(keepalive);
    result.opt.maxlen         = result.len;
    result.opt.buf            = &resopt;
    result.flags              = T_CURRENT;
    
    resopt.header.len         = sizeof(resopt);
    resopt.header.level       = T_INET_TCP;
    resopt.header.name        = T_TCP_KEEPALIVE;
    
    if ( t_optmgmt(_fd, &request, &result) < 0 ) {
    	return false;
    }
    return (result.status == T_SUCCESS) &&
           (resopt.header.status == T_SUCCESS) &&
	   (resopt.keepalive.l_onoff == T_YES);
#else
    return true;
#endif
#endif
} // KssXDRConnection::enableKeepAlive


/* End of connection.cpp */
