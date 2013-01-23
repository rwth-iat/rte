/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/include/ks/svrtransport.h,v 1.4 2007-04-25 12:57:20 martin Exp $ */
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
 * svrtransport.h -- Generalization of a XDR transport for a RPC server.
 *                   This will either wrap the ONC/RPC server transports
 *                   (aka SVCXPRT) or a XDR connection object, depending on
 *                   whether dynamic XDR streams and the connection manager
 *                   have been enabled or not.
 *
 * Written by Harald Albrecht <harald@plt.rwth-aachen.de>
 */

#ifndef KS_SVRTRANSPORT_H_INCLUDED
#define KS_SVRTRANSPORT_H_INCLUDED

#include "plt/config.h"
#include "ks/rpc.h" // Make sure that esp. for OpenVMS we get the right headers

#include "ks/avticket.h"
#include "ks/result.h"

#if !PLT_USE_BUFFERED_STREAMS

// ---------------------------------------------------------------------------
// And here comes in the Spanish Inquisition... oops, wrong movie. Of course,
// here comes the generalization of a XDR server transport.
//
// NOTE: the following class definition is only used for the genuine ONC/RPC
// package and wraps its (rather inconvenient/strange) server API.
//
class KssTransport {
public:
    //
    // Implementation-specific interface. For the genuine ONC/RPC we will
    // use only one static KssTransport object within the KsServerBase class,
    // which will just get wrapped around the SVCXPRT transport for every
    // ONC/RPC call.
    //
    KssTransport() { }
    void setNewRequest(SVCXPRT *t, svc_req *r);
    //
    // All the following functions are always available, regardless of how
    // the KssTransport class is implemented.
    //
    // IMPORTANT NOTE: NEVER USE getDeserializingXdr() WHEN SERIALIZING
    // AN ANSWER! THIS IS WRONG!!
    XDR       *getDeserializingXdr() const { return _xdr; }
    KS_RESULT  finishRequestDeserialization(KsAvTicket &avt, bool ok);
    u_long     getServiceId() const { return _req->rq_proc; }
    u_long     getVersion() const { return _req->rq_vers; }
    //
    // Generalization of the ONC/RPC mechanism for sending replies back
    // to clients.
    //
    void       sendPingReply();
    void       sendErrorReply(KsAvTicket &avt, KS_RESULT error);
    void       sendReply(KsAvTicket &avt, KsResult &result);
    //
    // Get the peer's IP address.
    //
    sockaddr  *getPeerAddress(int &namelen);
    //
    // Depending on the underlaying transport, this might drop the
    // connection (for TCP/IP only). To be used when an AV ticket indicates
    // bad authentification.
    //
    void       personaNonGrata();

protected:
    SVCXPRT *_t;    // The encapsulated ONC/RPC server transport
    svc_req *_req;  // The request (call)
    XDR     *_xdr;  // XDR stream for receiving requests
}; // class KssTransport


#else /* !PLT_USE_BUFFERED_STREAMS */

// ---------------------------------------------------------------------------
// NOTE: when using the dynamic XDR buffered streams (from xdrmemstream.cpp)
// the KssTransport class is rather an alias for the KssXDRConnection class.
// The connection class handles XDR connections and also provides an interface
// to the transport abstraction layer.
//
class KssXDRConnection;
typedef KssXDRConnection KssTransport;

#endif /* !PLT_USE_BUFFERED_STREAMS */

#endif

/* End of svrtransport.h */
