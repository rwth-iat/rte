/* -*-plt-c++-*- */
#ifndef KS_SVRRPCCTX_INCLUDED
#define KS_SVRRPCCTX_INCLUDED
/* $Header: /home/david/cvs/acplt/ks/include/ks/svrrpcctx.h,v 1.2 1997-07-18 14:11:06 martin Exp $ */
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
