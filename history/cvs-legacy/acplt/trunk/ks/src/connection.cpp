/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/src/connection.cpp,v 1.1 1998-06-29 11:22:51 harald Exp $ */
/*
 * Copyright (c) 1998
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
 * connection.cpp -- Abstract base class and basic classes for Internet-based
 *                   connections via UDP/IP and TCP/IP.
 *
 * Written by Harald Albrecht <harald@plt.rwth-aachen.de>
 */

#include "ks/connection.h"


#if PLT_SYSTEM_NT

#define ioctl(fd,r,p) ioctlsocket(fd,r,p)

#else

#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

#endif


// ---------------------------------------------------------------------------
// Construct a new connection from a given file descriptor.
//
KssConnection::KssConnection(int fd, bool autoDestroyable,
                             unsigned long timeout,
			     ConnectionType type)
    : _cnx_type(type), _timeout(timeout),
      _auto_destroyable(autoDestroyable),
      _fd(fd),
      _client_address_len(0)
{
    _state = _cnx_type == CNX_TYPE_SERVER ? CNX_STATE_IDLE : CNX_STATE_PASSIVE;
} // KssConnection::KssConnection


// ---------------------------------------------------------------------------
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
//
//
#if PLT_USE_XTI && PLT_SYSTEM_SOLARIS
extern int t_getname(int fd, struct netbuf *namep, int type);
#endif

u_short KssConnection::getPort() const
{
    struct sockaddr_in addr;
    int                addr_len;
    
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
// when the connection is shut down.
//
void KssXDRConnection::shutdown()
{
    if ( _cleanup_xdr_stream ) {
    	xdr_destroy(&_xdrs);
	_cleanup_xdr_stream = false;
    }
#if PLT_SYSTEM_NT
    closesocket(_fd); // Ouch!!!
#elif PLT_USE_XTI
    t_close(_fd); // one more "Ouch!!!"
#else
    close(_fd);
#endif
    _state = CNX_STATE_DEAD;
} // KssXDRConnection::shutdown


// ---------------------------------------------------------------------------
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
// various operating systems. Well, "NT" might be an operating system.
//
bool KssXDRConnection::makeNonblocking()
{
#if PLT_SYSTEM_NT
    int nbmode = 1;
    return ioctl(_fd, FIONBIO, &nbmode) != SOCKET_ERROR;
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
//
bool KssXDRConnection::enableKeepAlive()
{
#if !PLT_USE_XTI
    int mode = 1; // enable KEEP ALIVE
    return setsockopt(_fd, SOL_SOCKET, SO_KEEPALIVE, 
                      &mode, sizeof(mode))
	       != -1;
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
