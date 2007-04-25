/* -*-plt-c++-*- */
#ifndef KS_OBJMGRPARAMS_INCLUDED
#define KS_OBJMGRPARAMS_INCLUDED
/*
 * $Header: /home/david/cvs/acplt/ks/include/ks/objmgrparams.h,v 1.11 2007-04-25 12:57:20 martin Exp $
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


#include "ks/ks.h"
#include "ks/xdr.h"
#include "ks/result.h"
#include "ks/array.h"
#include "ks/string.h"
#include "ks/serviceparams.h" // needed due to KsSetVarItem


// ---------------------------------------------------------------------------
// More or less generic classes for several object management services.
// We first start with the omnipresent placement hint, which is useful when
// the underlying object system supports ordered sets of children/parts.
//
class KsPlacementHint : public KsXdrAble {
public:
    KsPlacementHint(XDR *, bool &);
    KsPlacementHint();

    bool xdrEncode(XDR *) const;
    bool xdrDecode(XDR *);
    static KsPlacementHint *xdrNew(XDR *);

    KS_PLACEMENT_HINT hint;
    KsString          place_path;
}; // class KsPlacementHint


// ---------------------------------------------------------------------------
// Next comes the description of a link item, that is, what object is linked
// into what set and at which place.
//
class KsLinkItem : public KsXdrAble {
public:
    KsLinkItem(XDR *, bool &);
    KsLinkItem();

    bool xdrEncode(XDR *) const;
    bool xdrDecode(XDR *);
    static KsLinkItem *xdrNew(XDR *);

    KsString        link_path;
    KsString        element_path;
    KsPlacementHint place;
    KsPlacementHint opposite_place;
}; // class KsLinkItem


// ---------------------------------------------------------------------------
// Just for reasons for orthogonality, now comes the definition of an item to
// be unlinked from a set of objects. The appropriate service will be defined
// in some distance from here...
//
class KsUnlinkItem : public KsXdrAble {
public:
    KsUnlinkItem(XDR *, bool &);
    KsUnlinkItem();

    bool xdrEncode(XDR *) const;
    bool xdrDecode(XDR *);
    static KsUnlinkItem *xdrNew(XDR *);

    KsString link_path;
    KsString element_path;
}; // class KsUnlinkItem


// ---------------------------------------------------------------------------
// Classes for the CreateObject service. First, the information needed to
// create one single object: name, factory, place (because it'll be a child
// part of some other object, so it may be in an ordered set), as well as
// the parameters and additional links.
//
class KsCreateObjItem : public KsXdrAble {
public:
    KsCreateObjItem(XDR *, bool &);
    KsCreateObjItem();

    bool xdrEncode(XDR *) const;
    bool xdrDecode(XDR *);
    static KsCreateObjItem *xdrNew(XDR *);

    KsString              factory_path;
    KsString              new_path;
    KsPlacementHint       place;
    KsArray<KsSetVarItem> parameters;
    KsArray<KsLinkItem>   links;
}; // class KsCreateObjItem


// ---------------------------------------------------------------------------
// The full CreateObject service just embraces creating multiple objects with
// one service request.
//
class KsCreateObjParams : public KsXdrAble {
public:
    KsCreateObjParams(XDR *, bool &);
    KsCreateObjParams();

    bool xdrEncode(XDR *) const;
    bool xdrDecode(XDR *);
    static KsCreateObjParams *xdrNew(XDR *);

    KsArray<KsCreateObjItem> items;
}; // class KsCreateObjParams


// ---------------------------------------------------------------------------
// The answer of the CreateObject service.
//
class KsCreateObjResultItem : public KsResult {
public:
    KsCreateObjResultItem(XDR *, bool &);
    KsCreateObjResultItem();

    bool xdrEncode(XDR *) const;
    bool xdrDecode(XDR *);
    static KsCreateObjResultItem *xdrNew(XDR *);

    KsArray<KS_RESULT> param_results;
    KsArray<KS_RESULT> link_results;
}; // class KsCreateObjResultItem


class KsCreateObjResult : public KsResult {
public:
    KsCreateObjResult(XDR *, bool &);
    KsCreateObjResult();

    bool xdrEncode(XDR *) const;
    bool xdrDecode(XDR *);
    static KsCreateObjResult *xdrNew(XDR *);

    KsArray<KsCreateObjResultItem> obj_results;
}; // class KsCreateObjResult


// ---------------------------------------------------------------------------
// The request and reply classes of the DeleteObject service.
//
class KsDeleteObjParams : public KsXdrAble {
public:
    KsDeleteObjParams(XDR *, bool &);
    KsDeleteObjParams();

    bool xdrEncode(XDR *) const;
    bool xdrDecode(XDR *);
    static KsDeleteObjParams *xdrNew(XDR *);

    KsArray<KsString> paths;
}; // class KsDeleteObjParams


class KsDeleteObjResult : public KsResult {
public:
    KsDeleteObjResult(XDR *, bool &);
    KsDeleteObjResult();

    bool xdrEncode(XDR *) const;
    bool xdrDecode(XDR *);
    static KsDeleteObjResult *xdrNew(XDR *);

    KsArray<KS_RESULT> results;
}; // class KsDeleteObjResult


// ---------------------------------------------------------------------------
// The request and reply classes of the Link service.
//
class KsLinkParams : public KsXdrAble {
public:
    KsLinkParams(XDR *, bool &);
    KsLinkParams();

    bool xdrEncode(XDR *) const;
    bool xdrDecode(XDR *);
    static KsLinkParams *xdrNew(XDR *);

    KsArray<KsLinkItem> items;
}; // class KsLinkParams


class KsLinkResult : public KsResult {
public:
    KsLinkResult(XDR *, bool &);
    KsLinkResult();

    bool xdrEncode(XDR *) const;
    bool xdrDecode(XDR *);
    static KsLinkResult *xdrNew(XDR *);

    KsArray<KS_RESULT> results;
}; // class KsLinkResult


// ---------------------------------------------------------------------------
// The request and reply classes of the Unlink service.
//
class KsUnlinkParams : public KsXdrAble {
public:
    KsUnlinkParams(XDR *, bool &);
    KsUnlinkParams();

    bool xdrEncode(XDR *) const;
    bool xdrDecode(XDR *);
    static KsUnlinkParams *xdrNew(XDR *);

    KsArray<KsUnlinkItem> items;
}; // class KsUnlinkParams


class KsUnlinkResult : public KsResult {
public:
    KsUnlinkResult(XDR *, bool &);
    KsUnlinkResult();

    bool xdrEncode(XDR *) const;
    bool xdrDecode(XDR *);
    static KsUnlinkResult *xdrNew(XDR *);

    KsArray<KS_RESULT> results;
}; // class KsUnlinkResult


// ---------------------------------------------------------------------------
// The request and reply classes of the GetCanonicalPath service.
//
class KsGetCanonicalPathResultItem : public KsResult {
public:
    KsGetCanonicalPathResultItem(XDR *, bool &);
    KsGetCanonicalPathResultItem();

    bool xdrEncode(XDR *) const;
    bool xdrDecode(XDR *);
    static KsGetCanonicalPathResultItem *xdrNew(XDR *);

    KsString  canonical_path;
}; // class KsGetCanonicalPathResultItem


class KsGetCanonicalPathParams : public KsXdrAble {
public:
    KsGetCanonicalPathParams(XDR *, bool &);
    KsGetCanonicalPathParams();

    bool xdrEncode(XDR *) const;
    bool xdrDecode(XDR *);
    static KsGetCanonicalPathParams *xdrNew(XDR *);

    KsArray<KsString> paths;
}; // class KsGetCanonicalNameParams


class KsGetCanonicalPathResult : public KsResult {
public:
    KsGetCanonicalPathResult(XDR *, bool &);
    KsGetCanonicalPathResult();

    bool xdrEncode(XDR *) const;
    bool xdrDecode(XDR *);
    static KsGetCanonicalPathResult *xdrNew(XDR *);

    KsArray<KsGetCanonicalPathResultItem> results;
}; // class KsGetCanonicalPathResult


// ---------------------------------------------------------------------------
// The request and reply classes of the RenameObject service.
//
class KsRenameObjItem : public KsXdrAble {
public:
    KsRenameObjItem(XDR *, bool &);
    KsRenameObjItem();

    bool xdrEncode(XDR *) const;
    bool xdrDecode(XDR *);
    static KsRenameObjItem *xdrNew(XDR *);

    KsString        old_path;
    KsString        new_path;
    KsPlacementHint new_place;
}; // class KsRenameObjItem

class KsRenameObjParams : public KsXdrAble {
public:
    KsRenameObjParams(XDR *, bool &);
    KsRenameObjParams();

    bool xdrEncode(XDR *) const;
    bool xdrDecode(XDR *);
    static KsRenameObjParams *xdrNew(XDR *);

    KsArray<KsRenameObjItem> items;
}; // class KsRenameObjParams


class KsRenameObjResult : public KsResult {
public:
    KsRenameObjResult(XDR *, bool &);
    KsRenameObjResult();

    bool xdrEncode(XDR *) const;
    bool xdrDecode(XDR *);
    static KsRenameObjResult *xdrNew(XDR *);

    KsArray<KS_RESULT> results;
}; // class KsRenameObjResult


#endif
/* End of ks/objmgrparams.h */
