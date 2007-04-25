/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/src/histparams.cpp,v 1.8 2007-04-25 12:57:20 martin Exp $ */
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

#include "ks/xdr.h"
#include "ks/histparams.h"


#if PLT_COMPILER_DECCXX
#if PLT_SEE_ALL_TEMPLATES
#include "ks/array.h"
#include "ks/handle.h"
#else
#include "ks/array_impl.h"
#include "ks/handle_impl.h"
#endif
template class PltContainer<KsGetHistItem>;
template class PltContainer<KsGetHistResultItem>;
template class PltContainer<KsGetHistSingleResult>;
template class PltHandle<KsGetHistItem>;
template class PltHandle<KsGetHistResultItem>;
template class PltHandle<KsGetHistSingleResult>;
template class KsPtrHandle<KsSelector>;
template class KsArray<KsGetHistItem>;
template class KsArray<KsGetHistResultItem>;
template class KsArray<KsGetHistSingleResult>;
#endif


/////////////////////////////////////////////////////////////////////////////

KS_IMPL_XDRNEW(KsGetHistItem);
KS_IMPL_XDRCTOR(KsGetHistItem);

KS_IMPL_XDRNEW(KsGetHistParams);
KS_IMPL_XDRCTOR(KsGetHistParams);

KS_IMPL_XDRNEW(KsGetHistResultItem);
KS_IMPL_XDRCTOR(KsGetHistResultItem);

KS_IMPL_XDRNEW(KsGetHistSingleResult);
KS_IMPL_XDRCTOR(KsGetHistSingleResult);

KS_IMPL_XDRNEW(KsGetHistResult);
KS_IMPL_XDRCTOR(KsGetHistResult);

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

KsGetHistItem::KsGetHistItem(const PltString &partId,
                             KsSelectorHandle selector)
    : part_id(partId),
      sel(selector)
{}

/////////////////////////////////////////////////////////////////////////////

bool
KsGetHistItem::xdrEncode(XDR *xdr) const
{
    PLT_PRECONDITION(xdr->x_op == XDR_ENCODE);

    return part_id.xdrEncode(xdr)
        && sel.xdrEncode(xdr);
}

/////////////////////////////////////////////////////////////////////////////

bool
KsGetHistItem::xdrDecode(XDR *xdr)
{
    PLT_PRECONDITION(xdr->x_op == XDR_DECODE);
    
    return part_id.xdrDecode(xdr)
        && sel.xdrDecode(xdr);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

KsGetHistParams::KsGetHistParams(size_t npaths,
                                 size_t nitems,
                                 u_long max)
    : paths(npaths),
      max_entries(max),
      items(nitems)
{}

/////////////////////////////////////////////////////////////////////////////

bool 
KsGetHistParams::xdrEncode(XDR *xdr) const
{
    PLT_PRECONDITION(xdr->x_op == XDR_ENCODE);

    return paths.xdrEncode(xdr)
        && ks_xdre_u_long(xdr, &max_entries)
        && items.xdrEncode(xdr);
}

/////////////////////////////////////////////////////////////////////////////

bool 
KsGetHistParams::xdrDecode(XDR *xdr)
{
    PLT_PRECONDITION(xdr->x_op == XDR_DECODE);

    return paths.xdrDecode(xdr)
        && ks_xdrd_u_long(xdr, &max_entries)
        && items.xdrDecode(xdr);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
 
bool 
KsGetHistResultItem::xdrEncode(XDR *xdr) const
{
    PLT_PRECONDITION(xdr->x_op == XDR_ENCODE);
    
    if( result == KS_ERR_OK ) {
        return KsResult::xdrEncode(xdr)
            && value.xdrEncode(xdr);
    } else {
        return KsResult::xdrEncode(xdr);
    }
}

/////////////////////////////////////////////////////////////////////////////

bool 
KsGetHistResultItem::xdrDecode(XDR *xdr)
{
    PLT_PRECONDITION(xdr->x_op == XDR_DECODE);
    
    if( KsResult::xdrDecode(xdr) ) {
        if( result == KS_ERR_OK ) {
            return value.xdrDecode(xdr);
        } else {
            return true;
        }
    }

    return false;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

bool 
KsGetHistSingleResult::xdrEncode(XDR *xdr) const 
{
    if( result == KS_ERR_OK ) {
        return KsResult::xdrEncode(xdr)
            && items.xdrEncode(xdr);
    } else {
        return KsResult::xdrEncode(xdr);
    }
}

/////////////////////////////////////////////////////////////////////////////

bool 
KsGetHistSingleResult::xdrDecode(XDR *xdr)
{
    if( KsResult::xdrDecode(xdr) ) {
        if( result == KS_ERR_OK ) {
            return items.xdrDecode(xdr);
        } else {
            return true;
        }
    } else {
        return false;
    }
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

bool 
KsGetHistResult::xdrEncode(XDR *xdr) const
{
    PLT_PRECONDITION(xdr->x_op == XDR_ENCODE);

    if( result == KS_ERR_OK ) {
        return KsResult::xdrEncode(xdr)
            && replies.xdrEncode(xdr);
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
            return replies.xdrDecode(xdr);
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

    
