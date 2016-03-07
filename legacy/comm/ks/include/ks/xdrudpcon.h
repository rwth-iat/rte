/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/include/ks/xdrudpcon.h,v 1.10 2007-04-25 12:57:20 martin Exp $ */
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
 * xdrudpcon.h -- Implements an UDP connection with XDR data as its
 *                data representation for the mess we'll transport.
 *
 * Written by Harald Albrecht <harald@plt.rwth-aachen.de>
 */

#ifndef XDRUDPCON_H_INCLUDED
#define XDRUDPCON_H_INCLUDED

#if PLT_USE_BUFFERED_STREAMS

#include "ks/connection.h"


class KssUDPXDRConnection : public KssXDRConnection {
public:
    KssUDPXDRConnection(int fd, 
			unsigned long timeout,
			unsigned long retrytimeout,
	                struct sockaddr_in *clientAddr = 0, 
			int clientAddrLen = 0,
	                ConnectionType type = CNX_TYPE_SERVER,
                        unsigned long buffsize = 16384);
    ~KssUDPXDRConnection();
        
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

    unsigned long  _total_timeout;
    unsigned long  _time_passed;

    unsigned long  _buffer_size;
    char          *_recvbuffer;
    char          *_sendbuffer;
    unsigned long  _tosend;

private:
    KssUDPXDRConnection(KssUDPXDRConnection &); // forbidden
}; // class KssUDPXDRConnection


#endif /* PLT_USE_BUFFERED_STREAMS */
#endif

/* End of xdrudpcon.h */
