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

/* Author: Markus Juergens <markusj@plt.rwth-aachen.de> */
 
/////////////////////////////////////////////////////////////////////////////

#include "ks/currprops.h"

////////////////////////////////////////////////////////////////////////////
// class KsCurrProps
////////////////////////////////////////////////////////////////////////////

KS_BEGIN_IMPL_XDRUNION(KsCurrProps);
KS_XDR_MAP(KS_OT_VARIABLE,KsVarCurrProps);
KS_XDR_MAP(KS_OT_DOMAIN,KsDomainCurrProps);
KS_END_IMPL_XDRUNION;

KS_IMPL_XDR_C(KsCurrProps);

////////////////////////////////////////////////////////////////////////////
// class KsVarCurrProps
////////////////////////////////////////////////////////////////////////////

// KS_IMPL_XDR_NEW(KsVarCurrProps);

KsVarCurrProps::KsVarCurrProps(XDR *xdr, bool &ok)
{
    value = KsValue::xdrNew(xdr);
    if( value ) {
        ok = time.xdrDecode(xdr) && xdr_enum(xdr, &state);
    } else {
        ok = false;
    }
}

//////////////////////////////////////////////////////////////////////

bool
KsVarCurrProps::xdrEncodeVariant(XDR *xdr) const 
{
    return value->xdrEncode(xdr)
        && time.xdrEncode(xdr)
        && xdr_enum(xdr, &state);
}

bool
KsVarCurrProps::xdrDecodeVariant(XDR *xdr) 
{
    return value->xdrDecode(xdr) 
        && time.xdrDecode(xdr)
        && xdr_enum(xdr, &state);
}    

enum_t
KsVarCurrProps::xdrTypeCode() const
{
    return KS_OT_VARIABLE;
}

///////////////////////////////////////////////////////////////////////////
// class KsDomainCurrProps
///////////////////////////////////////////////////////////////////////////

// KS_IMPL_XDR_C(KsDomainCurrProps);

enum_t
KsDomainCurrProps::xdrTypeCode() const 
{
    return KS_OT_DOMAIN;
}

//////////////////////////////////////////////////////////////////////

bool
KsDomainCurrProps::xdrEncodeVariant(XDR *) const 
{
    return true;
}

//////////////////////////////////////////////////////////////////////

bool
KsDomainCurrProps::xdrDecodeVariant(XDR *)
{
    return true;
}

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// End of File currprops.cpp
//////////////////////////////////////////////////////////////////////////

