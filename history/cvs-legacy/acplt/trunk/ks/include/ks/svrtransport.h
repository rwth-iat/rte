/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/include/ks/svrtransport.h,v 1.1 1998-06-29 11:19:17 harald Exp $ */
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
