/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/include/ks/xdrtcpcon.h,v 1.4 1999-01-29 12:42:21 harald Exp $ */
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

#ifndef XDRTCPCON_H_INCLUDED
#define XDRTCPCON_H_INCLUDED

#if PLT_USE_BUFFERED_STREAMS

#include "ks/connection.h"


// ---------------------------------------------------------------------------
// A TCP rendevouser connection which handles incomming connection requests.
//
class KssListenTCPXDRConnection : public KssXDRConnection {
public:
    KssListenTCPXDRConnection(int fd, unsigned long timeout);
        
    virtual ConnectionIoMode getIoMode() const;

    virtual void sendPingReply();
    virtual void sendErrorReply(KsAvTicket &avt, KS_RESULT error);
    virtual void sendReply(KsAvTicket &avt, KsResult &result);
    virtual void personaNonGrata();

    virtual bool beginRequest();
    virtual void sendRequest();

protected:
    virtual ConnectionIoMode receive();
    virtual ConnectionIoMode send();
    virtual ConnectionIoMode reset(bool hadTimeout);

private:
    KssListenTCPXDRConnection(KssListenTCPXDRConnection &); // forbidden
}; // class KssListenTCPXDRConnection


// ---------------------------------------------------------------------------
// A real TCP connection to communicate with clients.
//
class KssTCPXDRConnection : public KssXDRConnection {
public:
    KssTCPXDRConnection(int fd, unsigned long timeout,
	                struct sockaddr_in &clientAddr, int clientAddrLen,
	                ConnectionType type);
        
    virtual ConnectionIoMode getIoMode() const;

    virtual void sendPingReply();
    virtual void sendErrorReply(KsAvTicket &avt, KS_RESULT error);
    virtual void sendReply(KsAvTicket &avt, KsResult &result);
    virtual void personaNonGrata();

    virtual bool beginRequest() { return false; } // TODO sometimes in the
    virtual void sendRequest()  { }               // future...

protected:
    virtual ConnectionIoMode receive();
    virtual ConnectionIoMode send();
    virtual ConnectionIoMode reset(bool hadTimeout);

    virtual void freeStreamMemory();

    ConnectionIoMode enterSendingState();
    
    enum FragmentState { FRAGMENT_HEADER, FRAGMENT_BODY };
    
    bool              _last_fragment;
    FragmentState     _fragment_state;
    u_long            _remaining_len;
    char              _fragment_header[4];
#if !PLT_COMPILER_BORLAND && !PLT_COMPILER_DECCXX
    void             *_ptr;
#else
    char             *_ptr;
#endif
    
private:
    KssTCPXDRConnection(KssTCPXDRConnection &); // forbidden
}; // class KssTCPXDRConnection


#endif /* PLT_USE_BUFFERED_STREAMS */
#endif

/* End of xdrtcpcon.h */
