/* -*-plt-c++-*- */

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

/* Author : Markus Juergens <plt@rwth-aachen.de> */

//////////////////////////////////////////////////////////////////////

#include "ks/histparams.h"

//////////////////////////////////////////////////////////////////////

KS_IMPL_XDRNEW(KsGHTimeSel);
KS_IMPL_XDRCTOR(KsGHTimeSel);

//////////////////////////////////////////////////////////////////////

KsGHTimeSel::KsGHTimeSel(const PltString &sel_name,
                         KS_TIME_SELECTOR_TYPE sel_type,
                         const PltTime &sel_from,
                         const PltTime &sel_to,
                         const PltTime &sel_delta)
    : sel(sel_name),
      type(sel_type),
      from(sel_from),
      to(sel_to),
      delta(sel_delta)
{}

/////////////////////////////////////////////////////////////////////////////

bool 
KsGHTimeSel::xdrEncode(XDR *xdr) const
{
    PLT_PRECONDITION(xdr->x_op == XDR_ENCODE);
    return sel.xdrEncode(xdr)
        && ks_xdre_enum(xdr, &type)
        && from.xdrEncode(xdr)
        && to.xdrEncode(xdr)
        && delta.xdrEncode(xdr);
}

//////////////////////////////////////////////////////////////////////

bool 
KsGHTimeSel::xdrDecode(XDR *xdr) 
{
    PLT_PRECONDITION(xdr->x_op == XDR_DECODE);
    return sel.xdrDecode(xdr)
        && ks_xdrd_enum(xdr, &type)
        && from.xdrDecode(xdr)
        && to.xdrDecode(xdr)
        && delta.xdrDecode(xdr);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

KS_IMPL_XDRNEW(KsGHStringSel);
KS_IMPL_XDRCTOR(KsGHStringSel);

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

KS_IMPL_XDRNEW(KsGetHistParams);
KS_IMPL_XDRCTOR(KsGetHistParams);

/////////////////////////////////////////////////////////////////////////////

KsGetHistParams::KsGetHistParams(const PltString &aName,
                                 KS_INTERPOLATION_MODE ipMode,
                                 u_long maxEntries,
                                 const KsGHTimeSel &timeSelector)
    : name(aName),
      ip_mode(ipMode),
      max_entries(maxEntries),
      time_selector(timeSelector)
{}

/////////////////////////////////////////////////////////////////////////////

bool 
KsGetHistParams::xdrEncode(XDR *xdr) const
{
    PLT_PRECONDITION(xdr->x_op == XDR_ENCODE);

    return name.xdrEncode(xdr)
        && ks_xdre_enum(xdr, &ip_mode)
        && ks_xdre_u_long(xdr, &max_entries)
        && time_selector.xdrEncode(xdr)
        && selectors.xdrEncode(xdr);
}

/////////////////////////////////////////////////////////////////////////////

bool 
KsGetHistParams::xdrDecode(XDR *xdr)
{
    PLT_PRECONDITION(xdr->x_op == XDR_DECODE);

    return name.xdrDecode(xdr)
        && ks_xdrd_enum(xdr, &ip_mode)
        && ks_xdrd_u_long(xdr, &max_entries)
        && time_selector.xdrDecode(xdr)
        && selectors.xdrDecode(xdr);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

KS_IMPL_XDRNEW(KsGHResultField);
KS_IMPL_XDRCTOR(KsGHResultField);

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

KS_IMPL_XDRNEW(KsGetHistResult);
KS_IMPL_XDRCTOR(KsGetHistResult);

/////////////////////////////////////////////////////////////////////////////

bool 
KsGetHistResult::xdrEncode(XDR *xdr) const
{
    PLT_PRECONDITION(xdr->x_op == XDR_ENCODE);

    if( result == KS_ERR_OK ) {
        return KsResult::xdrEncode(xdr)
            && fields.xdrEncode(xdr);
    } else {
        return KsResult::xdrEncode(xdr);
    }
}

/////////////////////////////////////////////////////////////////////////////

bool 
KsGetHistResult::xdrDecode(XDR *xdr)
{
    PLT_PRECONDITION(xdr->x_op == XDR_DECODE);
    
    if( KsResult::xdrDecode(xdr) ) {
        if( result == KS_ERR_OK ) {
            return fields.xdrDecode(xdr);
        } else {
            return true;
        }
    } else {
        return false;
    }
}

//////////////////////////////////////////////////////////////////////
// End of File histparams.cpp
//////////////////////////////////////////////////////////////////////

    
