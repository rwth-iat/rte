/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/src/histparams.cpp,v 1.3 1999-05-12 10:02:12 harald Exp $ */
/*
 * Copyright (c) 1996, 1997, 1998, 1999
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

    
