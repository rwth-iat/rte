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

#include "ks/serviceparams.h"

//////////////////////////////////////////////////////////////////////

KS_IMPL_XDRNEW(KsGetPPParams);
KS_IMPL_XDRCTOR(KsGetPPParams);

//////////////////////////////////////////////////////////////////////

bool 
KsGetPPParams::xdrEncode(XDR *xdr) const
{
    PLT_PRECONDITION(xdr->x_op == XDR_ENCODE);
    return path.xdrEncode(xdr)
        && ks_xdre_enum(xdr, &type_mask)
        && name_mask.xdrEncode(xdr);
}

//////////////////////////////////////////////////////////////////////

bool 
KsGetPPParams::xdrDecode(XDR *xdr) 
{
    PLT_PRECONDITION(xdr->x_op == XDR_DECODE);
    return path.xdrDecode(xdr)
        && ks_xdrd_enum(xdr, &type_mask)
        && name_mask.xdrDecode(xdr);
}


//////////////////////////////////////////////////////////////////////
// class KsGetPPResult
//////////////////////////////////////////////////////////////////////

KS_IMPL_XDRNEW(KsGetPPResult);
KS_IMPL_XDRCTOR(KsGetPPResult);

//////////////////////////////////////////////////////////////////////

bool
KsGetPPResult::xdrEncode(XDR *xdr) const
{
    // serialize members of base class
    //
    if( !KsResult::xdrEncode(xdr) ) return false;

    // check for valid information and encode list contents
    //
    if( result == KS_ERR_OK ) {
        return items.xdrEncode(xdr);
    }
    
    return true;
}

//////////////////////////////////////////////////////////////////////

bool
KsGetPPResult::xdrDecode(XDR *xdr)
{
    // deserialize members of base class
    //
    if( !KsResult::xdrDecode(xdr) ) return false;

    if( result == KS_ERR_OK ) {
        // decode additional information
        //
        return items.xdrDecode(xdr);
    }

    return true;
}

//////////////////////////////////////////////////////////////////////
// class KsGetVarParams
//////////////////////////////////////////////////////////////////////

KS_IMPL_XDRCTOR(KsGetVarParams);
KS_IMPL_XDRNEW(KsGetVarParams);


//////////////////////////////////////////////////////////////////////
// class KsGetVarItemResult
//////////////////////////////////////////////////////////////////////

bool 
KsGetVarItemResult::xdrEncode(XDR *xdr) const
{
    if( !KsResult::xdrEncode(xdr) ) return false;

    if( result == KS_ERR_OK ) {
        return item.xdrEncode(xdr);
    }

    return true;
}

//////////////////////////////////////////////////////////////////////

bool
KsGetVarItemResult::xdrDecode(XDR *xdr)
{
    if( !KsResult::xdrDecode(xdr) ) return false;

    if( result == KS_ERR_OK ) {
        return item.xdrDecode(xdr);
    }

    return true;
}


//////////////////////////////////////////////////////////////////////
// class KsGetVarResult
//////////////////////////////////////////////////////////////////////

KS_IMPL_XDRNEW(KsGetVarResult);
KS_IMPL_XDRCTOR(KsGetVarResult);

//////////////////////////////////////////////////////////////////////

KsGetVarResult::KsGetVarResult()
{}

//////////////////////////////////////////////////////////////////////

bool
KsGetVarResult::xdrEncode(XDR *xdr) const
{
    if( !KsResult::xdrEncode(xdr) ) return false;

    if( result == KS_ERR_OK ) {
        return items.xdrEncode(xdr);
    }
    
    return true;
}

//////////////////////////////////////////////////////////////////////

bool
KsGetVarResult::xdrDecode(XDR *xdr)
{
    if( !KsResult::xdrDecode(xdr) ) return false;

    if( result == KS_ERR_OK ) {
        return items.xdrDecode(xdr);
    }
    
    return true;
}

//////////////////////////////////////////////////////////////////////
// classes for SetVar service
//////////////////////////////////////////////////////////////////////

KS_IMPL_XDRCTOR(KsSetVarItem);
KS_IMPL_XDRNEW(KsSetVarItem);

//////////////////////////////////////////////////////////////////////

bool
KsSetVarItem::xdrEncode(XDR *xdr) const
{
    return path_and_name.xdrEncode(xdr)
        && curr_props.xdrEncode(xdr);
}

//////////////////////////////////////////////////////////////////////

bool
KsSetVarItem::xdrDecode(XDR *xdr)
{
    return path_and_name.xdrDecode(xdr)
        && curr_props.xdrDecode(xdr);
}

//////////////////////////////////////////////////////////////////////

KS_IMPL_XDRCTOR(KsSetVarParams);
KS_IMPL_XDRNEW(KsSetVarParams);

//////////////////////////////////////////////////////////////////////

KS_IMPL_XDRCTOR(KsSetVarResult);
KS_IMPL_XDRNEW(KsSetVarResult);

//////////////////////////////////////////////////////////////////////

bool
KsSetVarResult::xdrEncode(XDR *xdr) const
{
    if( !KsResult::xdrEncode(xdr) ) return false;

    if( result == KS_ERR_OK ) {
        return results.xdrEncode(xdr);
    }

    return true;
}

//////////////////////////////////////////////////////////////////////

bool
KsSetVarResult::xdrDecode(XDR *xdr)
{
    if( !KsResult::xdrDecode(xdr) ) return false;

    if( result == KS_ERR_OK ) {
        return results.xdrDecode(xdr);
    }

    return true;
}

//////////////////////////////////////////////////////////////////////
// Classes for ExgData Service
//   - class KsSetVarItem(see SetVar service beyond)
//   - class KsGetVarItemResult(see GetVar service beyond)
//   - class KsExgDataParams
//   - class KsExgDataResult
//////////////////////////////////////////////////////////////////////

KS_IMPL_XDRCTOR(KsExgDataParams);
KS_IMPL_XDRNEW(KsExgDataParams);

//////////////////////////////////////////////////////////////////////

bool
KsExgDataParams::xdrEncode(XDR *xdr) const
{
    return set_vars.xdrEncode(xdr)
        && get_vars.xdrEncode(xdr);
}

//////////////////////////////////////////////////////////////////////

bool
KsExgDataParams::xdrDecode(XDR *xdr)
{
    return set_vars.xdrDecode(xdr)
        && get_vars.xdrDecode(xdr);
}

//////////////////////////////////////////////////////////////////////

KS_IMPL_XDRCTOR(KsExgDataResult);
KS_IMPL_XDRNEW(KsExgDataResult);

//////////////////////////////////////////////////////////////////////

bool
KsExgDataResult::xdrEncode(XDR *xdr) const 
{
    if( !KsResult::xdrEncode(xdr) ) return false;

    if( result == KS_ERR_OK ) {
        return results.xdrEncode(xdr)
            && items.xdrEncode(xdr);
    }

    return true;
}

//////////////////////////////////////////////////////////////////////

bool
KsExgDataResult::xdrDecode(XDR *xdr)
{
    if( !KsResult::xdrDecode(xdr) ) return false;

    if( result == KS_ERR_OK ) {
        return results.xdrDecode(xdr)
            && items.xdrDecode(xdr);
    }

    return true;
}

//////////////////////////////////////////////////////////////////////
// End of File serviceparams.cpp
//////////////////////////////////////////////////////////////////////

    
