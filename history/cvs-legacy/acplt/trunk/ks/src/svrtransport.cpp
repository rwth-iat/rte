/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/src/svrtransport.cpp,v 1.2 1998-06-30 11:29:08 harald Exp $ */
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
 * svrtransport.cpp -- Generalization of a XDR transport for a RPC server.
 *                     This will either wrap the ONC/RPC server transports
 *                     (aka SVCXPRT) or a XDR connection object, depending on
 *                     whether dynamic XDR streams and the connection manager
 *                     have been enabled or not.
 *
 * Written by Harald Albrecht <harald@plt.rwth-aachen.de>
 */

#include <ks/svrtransport.h>

#if !PLT_USE_BUFFERED_STREAMS

#if PLT_SYSTEM_OPENVMS
#include <unixio.h>
#endif

// ---------------------------------------------------------------------------
// Because objects can serialize and deserialize themselves into/from xdr
// streams, we often need to know the xdr stream identifier when
//   **DESERIALIZING**
// requests. Never, NEVER, NEVER use getDeserializingXdr() when SERIALIZING
// replies or you're in deep trouble, pals.
//
// C part...
extern "C" bool_t 
ks_c_get_deserializing_xdr_wrapper(XDR *xdr, void *obj_ptr)
{
    *((XDR **) obj_ptr) = xdr;
    return TRUE;
} // ks_c_get_deserializing_xdr_wrapper


// ---------------------------------------------------------------------------
// Associate (or better encapsulate) another ONC/RPC server transport with
// this transport object. This is not much work with the exception, that we
// need to find out what XDR stream is used for deserializing a request.
//
void KssTransport::setNewRequest(SVCXPRT *t, svc_req *r)
{
    _t = t;
    _req = r;
    svc_getargs(t, 
                (xdrproc_t) ks_c_get_deserializing_xdr_wrapper, 
                (caddr_t) &_xdr);
    PLT_ASSERT(xdr->x_op == XDR_DECODE);
} // KssTransport::setNewRequest


// ---------------------------------------------------------------------------
// Finish your job for the request encapsulated by this transport object. This
// involves deserializing the trailing part of the AV ticket. With other
// implementations, this might result in resetting the XDR stream into a
// state where it can now store the service reply. If you indicate an error,
// then now further deserializing will typically take place.
//
KS_RESULT KssTransport::finishRequestDeserialization(KsAvTicket &avt,
                                                     bool ok)
{
    //
    // Please note that an a/v ticket must be capable of recovering to
    // send a reply after the full incomming request couldn´t get decoded.
    // In case of an error, the trailing part of the a/v ticket information
    // is never read but instead when serializing the a/v ticket for the
    // answer the ticket must switch back to whatever mode it can (maybe
    // AV NONE).
    //
    if ( ok ) {
	if ( !avt.xdrDecodeTrailer(_xdr) ) {
    	    return KS_ERR_GENERIC;
	}
    }
    //
    // Always ask the ticket for permission...
    //
    return avt.result();
} // KssTransport::finishRequest


// ---------------------------------------------------------------------------
// Send back a ping reply which you usually want to do when you receive a
// call/request for procedure #0.
//
void KssTransport::sendPingReply()
{
    svc_sendreply(_t, (xdrproc_t) xdr_void, 0);
} // KssTransport::sendPingReply


// ---------------------------------------------------------------------------
// The next functions are helpers to send back a complete KS reply to some
// KS client. In every case, the reply consists of the A/V information as
// well as of an error code. If there is no error, then the service reply
// data will then follow the error code.
//
typedef struct {
    KsAvTicket     *ticket;
    KsResult *result;
    KS_RESULT       result_state;
} KsXdrReplyInfo;


// ---------------------------------------------------------------------------
// C part (helper function). This one calls the ONC/RPC package to serialize
// the reply.
//
extern "C" bool_t 
ks_c_send_reply_wrapper(XDR *xdr, void *reply_info)
{
    KsXdrReplyInfo *info = (KsXdrReplyInfo *) reply_info;

    if ( !info->ticket->xdrEncode(xdr) ) {
        return FALSE;
    }
    if ( info->result == 0 ) {
        //
        // We've got stuck with an error result very early in the
        // decoding process. Thus there's no full result available and
        // we just send back the error code...
        //
        return ks_xdre_enum(xdr, &(info->result_state)) &&
	       info->ticket->xdrEncodeTrailer(xdr);
    } else {
        //
        // We have a full blown result to send back...
        //
        return info->result->xdrEncode(xdr) &&
	       info->ticket->xdrEncodeTrailer(xdr);
    }
} // ks_c_send_reply_wrapper


// ---------------------------------------------------------------------------
// Send back an error reply to the KS client, which just consists of the AV
// ticket information and the error code. There's no service result body to
// be sent back to the client.
//
void KssTransport::sendErrorReply(KsAvTicket &avt, KS_RESULT error)
{
    KsXdrReplyInfo answer;

    answer.ticket = &avt;
    answer.result = 0;
    answer.result_state = error;
    svc_sendreply(_t, (xdrproc_t) ks_c_send_reply_wrapper,
                  (char*) &answer);
} // KssTransport::sendErrorReply


// ---------------------------------------------------------------------------
// Send back an full blown service reply: the AV ticket information, the error
// code and the service result.
//
void KssTransport::sendReply(KsAvTicket &avt, KsResult &result)
{
    KsXdrReplyInfo answer;

    answer.ticket = &avt;
    answer.result = &result;
    answer.result_state = KS_ERR_GENERIC; // should never be used
    svc_sendreply(_t, (xdrproc_t) ks_c_send_reply_wrapper,
                  (char*) &answer);
} // KssTransport::sendReply


// ---------------------------------------------------------------------------
// Ask for the client's IP address. This then can be used to write it into the
// AV ticket which then might want to reject the connection.
//
sockaddr *KssTransport::getPeerAddress(int &namelen)
{
#if PLT_USE_XTI
    //
    // Solaris does not store a valid sockaddr_in in xp_raddr...
    // instead it seems to use a netbuf in xp_rtaddr...
    //
    namelen = _t->xp_rtaddr.len;
    return (sockaddr*) _t->xp_rtaddr.buf; 
#else
    //
    // xp_raddr contains a valid sockaddr_in
    //
    PLT_ASSERT(sa->sa_family == AF_INET);
    namelen = sizeof (sockaddr_in);
    return (sockaddr *) svc_getcaller(_t);
#endif
} // KssTransport::getPeerAddress


// ---------------------------------------------------------------------------
// Force a shutdown on a connection to a client with whom we don't want to
// talk.
//
void KssTransport::personaNonGrata()
{
#if !PLT_USE_XTI
    int so_type;
#if !PLT_SYSTEM_OPENVMS
    int so_type_len = sizeof(so_type);
#else
    unsigned so_type_len = sizeof(so_type);
#endif
    
    if ( getsockopt(_t->xp_sock, SOL_SOCKET, SO_TYPE, 
                    &so_type, &so_type_len) >= 0 ) {
    	//
	// Could retrieve the socket type. If this is a streaming socket, then
	// we'll close the connection. To bring the stream into the XPRT_DIED
	// state, we try to receive something on the closed connection. Yes,
	// this is a dirty hack. How true...
	//
	if ( so_type == SOCK_STREAM ) {
	    rpc_msg msg;
#if !PLT_SYSTEM_NT
	    close(_t->xp_sock); // ...und tschuess!
#else
    	    closesocket(_t->xp_sock);
#endif
    	    svc_recv(_t, &msg);
	}
    }
#else
    // TODO
    // t_close(_t->xp_sock);
#endif
} // KssTransport::personaNonGrata


#endif /* !PLT_USE_BUFFERED_STREAMS */

/* End of svrtransport.cpp */
