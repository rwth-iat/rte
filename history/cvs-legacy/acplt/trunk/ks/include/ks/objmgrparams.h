/* -*-plt-c++-*- */
#ifndef KS_OBJMGRPARAMS_INCLUDED
#define KS_OBJMGRPARAMS_INCLUDED
/*
 * $Header: /home/david/cvs/acplt/ks/include/ks/objmgrparams.h,v 1.5 1998-12-16 17:48:28 harald Exp $
 *
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


#endif
/* End of ks/objmgrparams.h */
