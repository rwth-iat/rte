/* -*-plt-c++-*- */

/*
 * Copyright (c) 1996, 1997, 1998
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

/* Author : Markus Juergens <markusj@plt.rwth-aachen.de> */

/////////////////////////////////////////////////////////////////////////////

#include "ks/selector.h"

/////////////////////////////////////////////////////////////////////////////

PLT_IMPL_RTTI0(KsSelector);
PLT_IMPL_RTTI1(KsNoneSel, KsSelector);
PLT_IMPL_RTTI1(KsTimeSel, KsSelector);
PLT_IMPL_RTTI1(KsStringSel, KsSelector);

/////////////////////////////////////////////////////////////////////////////

KS_BEGIN_IMPL_XDRUNION(KsSelector);
KS_XDR_MAP(KS_SEL_NONE, KsNoneSel);
KS_XDR_MAP(KS_SEL_TIME, KsTimeSel);
KS_XDR_MAP(KS_SEL_STRING, KsStringSel);
KS_END_IMPL_XDRUNION;

/////////////////////////////////////////////////////////////////////////////

KsTimeSel::KsTimeSel(KS_TIME_SELECTOR_TYPE atype,
                     KsTime afrom,
                     KsTime ato,
                     KsTime adelta,
                     KS_INTERPOLATION_MODE amode)
    : type(atype),
      ip_mode(amode),
      from(afrom),
      to(ato),
      delta(adelta)
{}

/////////////////////////////////////////////////////////////////////////////

bool 
KsTimeSel::xdrEncodeVariant(XDR *xdr) const
{
    PLT_PRECONDITION(xdr->x_op == XDR_ENCODE);
    
    return ks_xdre_enum(xdr, &type)
        && ks_xdre_enum(xdr, &ip_mode)
        && from.xdrEncode(xdr)
        && to.xdrEncode(xdr)
        && delta.xdrEncode(xdr);
}

//////////////////////////////////////////////////////////////////////

bool 
KsTimeSel::xdrDecodeVariant(XDR *xdr) 
{
    PLT_PRECONDITION(xdr->x_op == XDR_DECODE);

    return ks_xdrd_enum(xdr, &type)
        && ks_xdrd_enum(xdr, &ip_mode)
        && from.xdrDecode(xdr)
        && to.xdrDecode(xdr)
        && delta.xdrDecode(xdr);
}

/////////////////////////////////////////////////////////////////////////////
// EOF selector.cpp
/////////////////////////////////////////////////////////////////////////////






