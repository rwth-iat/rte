/* -*-plt-c++-*- */
#ifndef KS_SVRRPCCTX_INCLUDED
#define KS_SVRRPCCTX_INCLUDED
/* $Header: /home/david/cvs/acplt/ks/include/ks/svrrpcctx.h,v 1.5 2007-04-25 12:57:20 martin Exp $ */
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

#include "ks/avticket.h"
#include "ks/time.h"

//////////////////////////////////////////////////////////////////////

struct KssRPCContext_base
{
public: // Microsoft bug
    KssRPCContext_base(const KsAvTicket &t)
        : request_ticket(t), p_response_ticket(0) { }
    virtual ~KssRPCContext_base();
    virtual u_long procedureNumber() const = 0;
    virtual bool encodeParams(XDR *) const = 0;
    virtual bool decodeResult(XDR *) = 0;
    
    enum clnt_stat call(CLIENT *clnt, const KsTime & timeout);

    // public attributes
    const KsAvTicket & request_ticket;
    KsAvTicket * p_response_ticket;
};

//////////////////////////////////////////////////////////////////////

template <u_long NR, class PARAMS, class RESULT>
struct KssRPCContext
: public KssRPCContext_base
{
public: // Microsoft bug
    PARAMS params;
    RESULT result;
    KssRPCContext(const KsAvTicket & ticket) : KssRPCContext_base(ticket) { }
    u_long procedureNumber() const { return NR; }
    bool encodeParams(XDR *xdr) const { return params.xdrEncode(xdr); }
    bool decodeResult(XDR *xdr) { return result.xdrDecode(xdr); }
};

//////////////////////////////////////////////////////////////////////
#endif // KS_SVRRPCCTX_INCLUDED
