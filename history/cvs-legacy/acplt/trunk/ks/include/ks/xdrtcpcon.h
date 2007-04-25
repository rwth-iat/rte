/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/include/ks/xdrtcpcon.h,v 1.12 2007-04-25 12:57:20 martin Exp $ */
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

    virtual bool beginRequest(u_long xid, u_long prog_number,
			      u_long prog_version, u_long proc_number);
    virtual void sendRequest();

protected:
    virtual ConnectionIoMode receive();
    virtual ConnectionIoMode send();
    virtual ConnectionIoMode timedOut();
    virtual ConnectionIoMode reset();

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

    virtual bool beginRequest(u_long xid, u_long prog_number,
			      u_long prog_version, u_long proc_number);
    virtual void sendRequest();

protected:
    virtual ConnectionIoMode receive();
    virtual ConnectionIoMode send();
    virtual ConnectionIoMode timedOut();
    virtual ConnectionIoMode reset();

    virtual void freeStreamMemory();

    ConnectionIoMode enterSendingState();
    
    enum FragmentState { FRAGMENT_HEADER, FRAGMENT_BODY };
    
    bool              _last_fragment;
    FragmentState     _fragment_state;
    u_long            _remaining_len;
    char              _fragment_header[4];
    char             *_ptr;
    
private:
    KssTCPXDRConnection(KssTCPXDRConnection &); // forbidden
}; // class KssTCPXDRConnection


#endif /* PLT_USE_BUFFERED_STREAMS */
#endif

/* End of xdrtcpcon.h */
