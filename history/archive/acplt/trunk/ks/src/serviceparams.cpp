/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/src/serviceparams.cpp,v 1.11 2007-04-25 12:57:21 martin Exp $ */
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


// ----------------------------------------------------------------------------
// class KsGetEPParams
//

KS_IMPL_XDRNEW(KsGetEPParams);
KS_IMPL_XDRCTOR(KsGetEPParams);


bool 
KsGetEPParams::xdrEncode(XDR *xdr) const
{
    PLT_PRECONDITION(xdr->x_op == XDR_ENCODE);
    return path.xdrEncode(xdr)
        && ks_xdre_enum(xdr, &type_mask)
        && name_mask.xdrEncode(xdr)
	&& ks_xdre_enum(xdr, &scope_flags);
} // KsGetEPParams::xdrEncode


bool 
KsGetEPParams::xdrDecode(XDR *xdr) 
{
    PLT_PRECONDITION(xdr->x_op == XDR_DECODE);
    return path.xdrDecode(xdr)
        && ks_xdrd_enum(xdr, &type_mask)
        && name_mask.xdrDecode(xdr)
	&& ks_xdrd_enum(xdr, &scope_flags);
} // KsGetEPParams::xdrDecode


// ----------------------------------------------------------------------------
// class KsGetEPResult
//

KS_IMPL_XDRNEW(KsGetEPResult);
KS_IMPL_XDRCTOR(KsGetEPResult);


bool
KsGetEPResult::xdrEncode(XDR *xdr) const
{
    //
    // First serialize the members of the base class.
    //
    if ( !KsResult::xdrEncode(xdr) ) return false;

    //
    // Check for valid information and only then encode the list's
    // contents, that is, the engineered properties.
    //
    if ( result == KS_ERR_OK ) {
        return items.xdrEncode(xdr);
    }
    
    return true;
} // KsGetEPResult::xdrEncode


bool
KsGetEPResult::xdrDecode(XDR *xdr)
{
    //
    // First deserialize the members of the base class.
    //
    if ( !KsResult::xdrDecode(xdr) ) return false;

    if ( result == KS_ERR_OK ) {
	// 
        // Decode additional information only if the ACPLT/KS service was
	// fullfilled.
        //
        return items.xdrDecode(xdr);
    }

    return true;
} // KsGetEPResult::xdrDecode



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

    
