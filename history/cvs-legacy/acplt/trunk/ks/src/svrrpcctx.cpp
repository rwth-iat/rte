/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/src/svrrpcctx.cpp,v 1.1 1997-04-03 10:03:08 martin Exp $ */
/*
 * Copyright (c) 1996, 1997
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

#include "ks/svrrpcctx.h"

//////////////////////////////////////////////////////////////////////

extern "C" bool_t ks_c_xdr_request(XDR *xdr, caddr_t * p)
{
    PLT_PRECONDITION(xdr && xdr->x_op == XDR_ENCODE && p);
    KssRPCContext_base *pcall = (KssRPCContext_base*)p;
    return pcall->request_ticket.xdrEncode(xdr) 
        && pcall->encodeParams(xdr);
}

//////////////////////////////////////////////////////////////////////

extern "C" bool_t ks_c_xdr_response(XDR *xdr, caddr_t *p)
{
    PLT_PRECONDITION(xdr && xdr->x_op == XDR_DECODE && p);
    KssRPCContext_base * pcall = (KssRPCContext_base*)p;
    //
    // The ticket must always be newly created:
    // First, delete the old one (if present)
    //
    if (pcall->p_response_ticket) {
        delete pcall->p_response_ticket;
    }
    //
    // Then create a new ticket from the XDR stream.
    //
    pcall->p_response_ticket = KsAvTicket::xdrNew(xdr);
    if (pcall->p_response_ticket) {
        //
        // We got a ticket, now decode the result. This depends
        // on the specific call, so ask our child class to do it.
        //
        return pcall->decodeResult(xdr);
    } else {
        // We could not decode the ticket. So sorry.
        return FALSE;
    }
}

//////////////////////////////////////////////////////////////////////

KssRPCContext_base::~KssRPCContext_base()
{
    if (p_response_ticket) {
        delete p_response_ticket;
    }
}

//////////////////////////////////////////////////////////////////////

enum clnt_stat
KssRPCContext_base::call(CLIENT *clnt, const KsTime & timeout)
{
    u_long procnr = procedureNumber();
    enum clnt_stat res = clnt_call(clnt,
                                   procnr,
                                   (xdrproc_t)ks_c_xdr_request,
                                   (caddr_t)this,
                                   (xdrproc_t)ks_c_xdr_response,
                                   (caddr_t)this,
                                   (struct timeval)timeout);
    return res;
}

//////////////////////////////////////////////////////////////////////
// EOF ks/svrrpcctx.cpp
