/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/src/svrrpcctx.cpp,v 1.4 2007-04-25 12:57:21 martin Exp $ */
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
