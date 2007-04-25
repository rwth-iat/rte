/* -*-plt-c++-*- */
/*
 * $Header: /home/david/cvs/acplt/ks/src/objmgrparams.cpp,v 1.11 2007-04-25 12:57:21 martin Exp $
 *
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

/* Author: Harald Albrecht <harald@plt.rwth-aachen.de> */


#include "ks/objmgrparams.h"


// ---------------------------------------------------------------------------
// constructor stuff...
//
KsPlacementHint::KsPlacementHint()
    : hint(KS_PMH_DEFAULT)
{} // KsPlacementHint::KsPlacementHint

KsLinkItem::KsLinkItem()
{} // KsLinkItem::KsLinkItem

KsUnlinkItem::KsUnlinkItem()
{} // KsUnlinkItem::KsUnlinkItem

KsCreateObjItem::KsCreateObjItem()
{} // KsCreateObjItem::KsCreateObjItem

KsCreateObjParams::KsCreateObjParams()
{} // KsCreateObjParams::KsCreateObjParams

KsCreateObjResultItem::KsCreateObjResultItem()
{} // KsCreateObjResultItem::KsCreateObjResultItem

KsCreateObjResult::KsCreateObjResult()
{} // KsCreateObjResult::KsCreateObjResult

KsDeleteObjParams::KsDeleteObjParams()
{} // KsDeleteObjParams::KsDeleteObjParams

KsDeleteObjResult::KsDeleteObjResult()
{} // KsDeleteObjResult::KsDeleteObjResult

KsLinkParams::KsLinkParams()
{} // KsLinkParams::KsLinkParams

KsLinkResult::KsLinkResult()
{} // KsLinkResult::KsLinkResult

KsUnlinkParams::KsUnlinkParams()
{} // KsUnlinkParams::KsUnlinkParams

KsUnlinkResult::KsUnlinkResult()
{} // KsUnlinkResult::KsUnlinkResult

KsGetCanonicalPathResultItem::KsGetCanonicalPathResultItem()
{ } // KsGetCanonicalPathResultItem::KsGetCanonicalPathResultItem

KsGetCanonicalPathParams::KsGetCanonicalPathParams()
{ } // KsGetCanonicalPathParams::KsGetCanonicalPathParams

KsGetCanonicalPathResult::KsGetCanonicalPathResult()
{ } // KsGetCanonicalPathResult::KsGetCanonicalPathResult

KsRenameObjItem::KsRenameObjItem()
{ } // KsRenameObjItem::KsRenameObjItem

KsRenameObjParams::KsRenameObjParams()
{ } // KsRenameObjParams::KsRenameObjParams

KsRenameObjResult::KsRenameObjResult()
{ } // KsRenameObjResult::KsRenameObjResult


// ---------------------------------------------------------------------------
// Implement the constructor, factory and encoding/decoding stuff for the
// KsPlacementHint class.
//
KS_IMPL_XDRNEW(KsPlacementHint);
KS_IMPL_XDRCTOR(KsPlacementHint);


bool
KsPlacementHint::xdrEncode(XDR *xdr) const
{
    PLT_PRECONDITION(xdr->x_op == XDR_ENCODE);
    return ks_xdre_enum(xdr, &hint)
        && (((hint == KS_PMH_BEFORE) ||
             (hint == KS_PMH_AFTER)) ?  place_path.xdrEncode(xdr) : true);
} // KsPlacementHint::xdrEncode


bool
KsPlacementHint::xdrDecode(XDR *xdr)
{
    PLT_PRECONDITION(xdr->x_op == XDR_DECODE);
    return ks_xdrd_enum(xdr, &hint)
        && (((hint == KS_PMH_BEFORE) ||
             (hint == KS_PMH_AFTER)) ? place_path.xdrDecode(xdr) : true);
} // KsPlacementHint::xdrDecode


// ---------------------------------------------------------------------------
// Implement the constructor, factory and encoding/decoding stuff for the
// KsLinkItem class.
//
KS_IMPL_XDRNEW(KsLinkItem);
KS_IMPL_XDRCTOR(KsLinkItem);


bool
KsLinkItem::xdrEncode(XDR *xdr) const
{
    PLT_PRECONDITION(xdr->x_op == XDR_ENCODE);
    return link_path.xdrEncode(xdr)
        && element_path.xdrEncode(xdr)
        && place.xdrEncode(xdr)
	&& opposite_place.xdrEncode(xdr);
} // KsLinkItem::xdrEncode


bool
KsLinkItem::xdrDecode(XDR *xdr)
{
    PLT_PRECONDITION(xdr->x_op == XDR_DECODE);
    return link_path.xdrDecode(xdr)
        && element_path.xdrDecode(xdr)
        && place.xdrDecode(xdr)
	&& opposite_place.xdrDecode(xdr);
} // KsLinkItem::xdrDecode


// ---------------------------------------------------------------------------
// Implement the constructor, factory and encoding/decoding stuff for the
// KsUnlinkItem class.
//
KS_IMPL_XDRNEW(KsUnlinkItem);
KS_IMPL_XDRCTOR(KsUnlinkItem);


bool
KsUnlinkItem::xdrEncode(XDR *xdr) const
{
    PLT_PRECONDITION(xdr->x_op == XDR_ENCODE);
    return link_path.xdrEncode(xdr)
        && element_path.xdrEncode(xdr);
} // KsUnlinkItem::xdrEncode


bool
KsUnlinkItem::xdrDecode(XDR *xdr)
{
    PLT_PRECONDITION(xdr->x_op == XDR_DECODE);
    return link_path.xdrDecode(xdr)
        && element_path.xdrDecode(xdr);
} // KsUnlinkItem::xdrDecode


// ---------------------------------------------------------------------------
//
KS_IMPL_XDRNEW(KsCreateObjItem);
KS_IMPL_XDRCTOR(KsCreateObjItem);


bool
KsCreateObjItem::xdrEncode(XDR *xdr) const
{
    PLT_PRECONDITION(xdr->x_op == XDR_ENCODE);
    return factory_path.xdrEncode(xdr)
        && new_path.xdrEncode(xdr)
        && place.xdrEncode(xdr)
        && parameters.xdrEncode(xdr)
        && links.xdrEncode(xdr);
} // KsCreateObjItem::xdrEncode


bool
KsCreateObjItem::xdrDecode(XDR *xdr)
{
    PLT_PRECONDITION(xdr->x_op == XDR_DECODE);
    return factory_path.xdrDecode(xdr)
        && new_path.xdrDecode(xdr)
        && place.xdrDecode(xdr)
        && parameters.xdrDecode(xdr)
        && links.xdrDecode(xdr);
} // KsCreateObjItem::xdrDecode


// ---------------------------------------------------------------------------
// Implement the constructor, factory and encoding/decoding stuff for the
// CreateObject service parameter class.
//
KS_IMPL_XDRNEW(KsCreateObjParams);
KS_IMPL_XDRCTOR(KsCreateObjParams);


bool
KsCreateObjParams::xdrEncode(XDR *xdr) const
{
    PLT_PRECONDITION(xdr->x_op == XDR_ENCODE);
    return items.xdrEncode(xdr);
} // KsCreateObjParams::xdrEncode


bool
KsCreateObjParams::xdrDecode(XDR *xdr)
{
    PLT_PRECONDITION(xdr->x_op == XDR_DECODE);
    return items.xdrDecode(xdr);
} // KsCreateObjParams::xdrDecode


//
KS_IMPL_XDRNEW(KsCreateObjResultItem);
KS_IMPL_XDRCTOR(KsCreateObjResultItem);


bool
KsCreateObjResultItem::xdrEncode(XDR *xdr) const
{
    PLT_PRECONDITION(xdr->x_op == XDR_ENCODE);
    return KsResult::xdrEncode(xdr)
        && (result == KS_ERR_BADINITPARAM ?
	        (   param_results.xdrEncode(xdr)
	         && link_results.xdrEncode(xdr))
	    : true);
} // KsCreateObjResultItem::xdrEncode


bool
KsCreateObjResultItem::xdrDecode(XDR *xdr)
{
    PLT_PRECONDITION(xdr->x_op == XDR_DECODE);
    return KsResult::xdrDecode(xdr)
        && (result == KS_ERR_BADINITPARAM ?
	        (   param_results.xdrDecode(xdr)
	         && link_results.xdrDecode(xdr))
	    : true);
} // KsCreateObjResultItem::xdrDecode


KS_IMPL_XDRNEW(KsCreateObjResult);
KS_IMPL_XDRCTOR(KsCreateObjResult);


bool
KsCreateObjResult::xdrEncode(XDR *xdr) const
{
    PLT_PRECONDITION(xdr->x_op == XDR_ENCODE);
    return KsResult::xdrEncode(xdr)
        && (result == KS_ERR_OK ? obj_results.xdrEncode(xdr)
	                        : true);
} // KsCreateObjResult::xdrEncode


bool
KsCreateObjResult::xdrDecode(XDR *xdr)
{
    PLT_PRECONDITION(xdr->x_op == XDR_DECODE);
    return KsResult::xdrDecode(xdr)
        && (result == KS_ERR_OK ? obj_results.xdrDecode(xdr)
	                        : true);
} // KsCreateObjResult::xdrDecode


// ---------------------------------------------------------------------------
//
KS_IMPL_XDRNEW(KsDeleteObjParams);
KS_IMPL_XDRCTOR(KsDeleteObjParams);


bool
KsDeleteObjParams::xdrEncode(XDR *xdr) const
{
    PLT_PRECONDITION(xdr->x_op == XDR_ENCODE);
    return paths.xdrEncode(xdr);
} // KsDeleteObjParams::xdrEncode


bool
KsDeleteObjParams::xdrDecode(XDR *xdr)
{
    PLT_PRECONDITION(xdr->x_op == XDR_DECODE);
    return paths.xdrDecode(xdr);
} // KsDeleteObjParams::xdrDecode


KS_IMPL_XDRNEW(KsDeleteObjResult);
KS_IMPL_XDRCTOR(KsDeleteObjResult);


bool
KsDeleteObjResult::xdrEncode(XDR *xdr) const
{
    PLT_PRECONDITION(xdr->x_op == XDR_ENCODE);
    return KsResult::xdrEncode(xdr)
        && (result == KS_ERR_OK ? results.xdrEncode(xdr) 
                                : true);
} // KsDeleteObjResult::xdrEncode


bool
KsDeleteObjResult::xdrDecode(XDR *xdr)
{
    PLT_PRECONDITION(xdr->x_op == XDR_DECODE);
    return KsResult::xdrDecode(xdr)
        && (result == KS_ERR_OK ? results.xdrDecode(xdr) 
                                : true);
} // KsDeleteObjResult::xdrDecode


// ---------------------------------------------------------------------------
// Parameters and result stuff needed for the Link service.
//
KS_IMPL_XDRNEW(KsLinkParams);
KS_IMPL_XDRCTOR(KsLinkParams);


bool
KsLinkParams::xdrEncode(XDR *xdr) const
{
    PLT_PRECONDITION(xdr->x_op == XDR_ENCODE);
    return items.xdrEncode(xdr);
} // KsLinkParams::xdrEncode


bool
KsLinkParams::xdrDecode(XDR *xdr)
{
    PLT_PRECONDITION(xdr->x_op == XDR_DECODE);
    return items.xdrDecode(xdr);
} // KsLinkParams::xdrDecode


KS_IMPL_XDRNEW(KsLinkResult);
KS_IMPL_XDRCTOR(KsLinkResult);


bool
KsLinkResult::xdrEncode(XDR *xdr) const
{
    PLT_PRECONDITION(xdr->x_op == XDR_ENCODE);
    return KsResult::xdrEncode(xdr)
        && (result == KS_ERR_OK ? results.xdrEncode(xdr) 
                                : true);
} // KsLinkResult::xdrEncode


bool
KsLinkResult::xdrDecode(XDR *xdr)
{
    PLT_PRECONDITION(xdr->x_op == XDR_DECODE);
    return KsResult::xdrDecode(xdr)
        && (result == KS_ERR_OK ? results.xdrDecode(xdr) 
                                : true);
} // KsLinkResult::xdrDecode


// ---------------------------------------------------------------------------
// Parameters and result stuff needed for the Unlink service.
//
KS_IMPL_XDRNEW(KsUnlinkParams);
KS_IMPL_XDRCTOR(KsUnlinkParams);


bool
KsUnlinkParams::xdrEncode(XDR *xdr) const
{
    PLT_PRECONDITION(xdr->x_op == XDR_ENCODE);
    return items.xdrEncode(xdr);
} // KsUnlinkParams::xdrEncode


bool
KsUnlinkParams::xdrDecode(XDR *xdr)
{
    PLT_PRECONDITION(xdr->x_op == XDR_DECODE);
    return items.xdrDecode(xdr);
} // KsUnlinkParams::xdrDecode


KS_IMPL_XDRNEW(KsUnlinkResult);
KS_IMPL_XDRCTOR(KsUnlinkResult);


bool
KsUnlinkResult::xdrEncode(XDR *xdr) const
{
    PLT_PRECONDITION(xdr->x_op == XDR_ENCODE);
    return KsResult::xdrEncode(xdr)
        && (result == KS_ERR_OK ? results.xdrEncode(xdr) 
                                : true);
} // KsUnlinkResult::xdrEncode


bool
KsUnlinkResult::xdrDecode(XDR *xdr)
{
    PLT_PRECONDITION(xdr->x_op == XDR_DECODE);
    return KsResult::xdrDecode(xdr)
        && (result == KS_ERR_OK ? results.xdrDecode(xdr) 
                                : true);
} // KsUnlinkResult::xdrDecode


// ---------------------------------------------------------------------------
// Parameters and result stuff needed for the GetCanonicalPath service.
//
KS_IMPL_XDRNEW(KsGetCanonicalPathResultItem);
KS_IMPL_XDRCTOR(KsGetCanonicalPathResultItem);


bool
KsGetCanonicalPathResultItem::xdrEncode(XDR *xdr) const
{
    PLT_PRECONDITION(xdr->x_op == XDR_ENCODE);
    return KsResult::xdrEncode(xdr)
        && (result == KS_ERR_OK ? canonical_path.xdrEncode(xdr) 
                                : true);
} // KsGetCanonicalPathResultItem::xdrEncode


bool
KsGetCanonicalPathResultItem::xdrDecode(XDR *xdr)
{
    PLT_PRECONDITION(xdr->x_op == XDR_DECODE);
    return KsResult::xdrDecode(xdr)
        && (result == KS_ERR_OK ? canonical_path.xdrDecode(xdr) 
                                : true);
} // KsGetCanonicalPathResultItem::xdrDecode


KS_IMPL_XDRNEW(KsGetCanonicalPathParams);
KS_IMPL_XDRCTOR(KsGetCanonicalPathParams);


bool
KsGetCanonicalPathParams::xdrEncode(XDR *xdr) const
{
    PLT_PRECONDITION(xdr->x_op == XDR_ENCODE);
    return paths.xdrEncode(xdr);
} // KsGetCanonicalPathParams::xdrEncode


bool
KsGetCanonicalPathParams::xdrDecode(XDR *xdr)
{
    PLT_PRECONDITION(xdr->x_op == XDR_DECODE);
    return paths.xdrDecode(xdr);
} // KsGetCanonicalPathParams::xdrDecode


KS_IMPL_XDRNEW(KsGetCanonicalPathResult);
KS_IMPL_XDRCTOR(KsGetCanonicalPathResult);


bool
KsGetCanonicalPathResult::xdrEncode(XDR *xdr) const
{
    PLT_PRECONDITION(xdr->x_op == XDR_ENCODE);
    return KsResult::xdrEncode(xdr)
        && (result == KS_ERR_OK ? results.xdrEncode(xdr) 
                                : true);
} // KsGetCanonicalPathResult::xdrEncode


bool
KsGetCanonicalPathResult::xdrDecode(XDR *xdr)
{
    PLT_PRECONDITION(xdr->x_op == XDR_DECODE);
    return KsResult::xdrDecode(xdr)
        && (result == KS_ERR_OK ? results.xdrDecode(xdr) 
                                : true);
} // KsGetCanonicalPathResult::xdrDecode


// ---------------------------------------------------------------------------
// Parameters and result stuff needed for the RenameObject service.
//
KS_IMPL_XDRNEW(KsRenameObjItem);
KS_IMPL_XDRCTOR(KsRenameObjItem);


bool
KsRenameObjItem::xdrEncode(XDR *xdr) const
{
    PLT_PRECONDITION(xdr->x_op == XDR_ENCODE);
    return old_path.xdrEncode(xdr)
	&& new_path.xdrEncode(xdr)
	&& new_place.xdrEncode(xdr);
} // KsRenameObjItem::xdrEncode


bool
KsRenameObjItem::xdrDecode(XDR *xdr)
{
    PLT_PRECONDITION(xdr->x_op == XDR_DECODE);
    return old_path.xdrDecode(xdr)
	&& new_path.xdrDecode(xdr)
	&& new_place.xdrDecode(xdr);
} // KsRenameObjItem::xdrDecode


KS_IMPL_XDRNEW(KsRenameObjParams);
KS_IMPL_XDRCTOR(KsRenameObjParams);


bool
KsRenameObjParams::xdrEncode(XDR *xdr) const
{
    PLT_PRECONDITION(xdr->x_op == XDR_ENCODE);
    return items.xdrEncode(xdr);
} // KsRenameObjParams::xdrEncode


bool
KsRenameObjParams::xdrDecode(XDR *xdr)
{
    PLT_PRECONDITION(xdr->x_op == XDR_DECODE);
    return items.xdrDecode(xdr);
} // KsRenameObjParams::xdrDecode


KS_IMPL_XDRNEW(KsRenameObjResult);
KS_IMPL_XDRCTOR(KsRenameObjResult);


bool
KsRenameObjResult::xdrEncode(XDR *xdr) const
{
    PLT_PRECONDITION(xdr->x_op == XDR_ENCODE);
    return KsResult::xdrEncode(xdr)
        && (result == KS_ERR_OK ? results.xdrEncode(xdr) 
                                : true);
} // KsRenameObjResult::xdrEncode


bool
KsRenameObjResult::xdrDecode(XDR *xdr)
{
    PLT_PRECONDITION(xdr->x_op == XDR_DECODE);
    return KsResult::xdrDecode(xdr)
        && (result == KS_ERR_OK ? results.xdrDecode(xdr) 
                                : true);
} // KsRenameObjResult::xdrDecode


/* End of ks/objmgrparams.cpp */
