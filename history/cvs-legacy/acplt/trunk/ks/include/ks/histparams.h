/* -*-plt-c++-*- */
#ifndef KS_HISTPARAMS_INCLUDED
#define KS_HISTPARAMS_INCLUDED
/* $Header: /home/david/cvs/acplt/ks/include/ks/histparams.h,v 1.10 2007-04-25 12:57:20 martin Exp $ */
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

/* Author: Markus Juergens <markusj@plt.rwth-aachen.de> */
/* v2 compliance by: Harald Albrecht <harald@plt.rwth-aachen.de> */


#include <limits.h>

#include "ks/xdr.h"
#include "ks/value.h"
#include "ks/array.h"
#include "ks/result.h"
#include "ks/selector.h"


// ----------------------------------------------------------------------------
// Classes for GetHist request
//   - KsGetHistItem
//   - KsGetHistParams

class KsGetHistItem
    : public KsXdrAble
{
public:
    KsGetHistItem() {}
    KsGetHistItem(XDR *, bool &);
    KsGetHistItem(const PltString &partId,
                  KsSelectorHandle selector);

    bool xdrEncode(XDR *) const;
    bool xdrDecode(XDR *);
    static KsGetHistItem *xdrNew(XDR *);

    // Just compares part_id.
    bool operator == (const KsGetHistItem &) const;

    KsString         part_id;
    KsSelectorHandle sel;
};

/////////////////////////////////////////////////////////////////////////////

class KsGetHistParams
    : public KsXdrAble
{
public:
    KsGetHistParams(XDR *, bool &);
    KsGetHistParams() {}
    KsGetHistParams(size_t npaths,
                    size_t nitems,
                    u_long max = ULONG_MAX);

    bool xdrEncode(XDR *) const;
    bool xdrDecode(XDR *);
    static KsGetHistParams *xdrNew(XDR *);

    KsArray<KsString>      paths;
    u_long                 max_entries;
    KsArray<KsGetHistItem> items;
};

/////////////////////////////////////////////////////////////////////////////
// Classes for GetHist reply:
//   - KsGetHistResultItem
//   - KsGetHistSingleResult
//   - KsGetHistResult
/////////////////////////////////////////////////////////////////////////////

class KsGetHistResultItem
    : public KsResult
{
public:
    KsGetHistResultItem() {}
    KsGetHistResultItem(XDR *, bool &);

    bool xdrEncode(XDR *) const;
    bool xdrDecode(XDR *);
    static KsGetHistResultItem *xdrNew(XDR *);

    KsValueHandle value;
};

/////////////////////////////////////////////////////////////////////////////

class KsGetHistSingleResult
    : public KsResult
{
public:
    KsGetHistSingleResult() {}
    KsGetHistSingleResult(XDR *, bool &);

    bool xdrEncode(XDR *) const;
    bool xdrDecode(XDR *);
    static KsGetHistSingleResult *xdrNew(XDR *);

    KsArray<KsGetHistResultItem> items;
};

/////////////////////////////////////////////////////////////////////////////

class KsGetHistResult
    : public KsResult
{
public:
    KsGetHistResult() {}
    KsGetHistResult(XDR *, bool &);
    KsGetHistResult(size_t nitems);

    bool xdrEncode(XDR *) const;
    bool xdrDecode(XDR *);
    static KsGetHistResult *xdrNew(XDR *);

    KsArray<KsGetHistSingleResult> replies;
};

/////////////////////////////////////////////////////////////////////////////
// INLINE IMPLEMENTATION
/////////////////////////////////////////////////////////////////////////////

inline
bool 
KsGetHistItem::operator == (const KsGetHistItem &other) const
{
    return other.part_id == part_id;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

inline
KsGetHistResult::KsGetHistResult(size_t nitems)
    : replies(nitems)
{}


#endif // KS_HISTPARAMS_INCLUDED
// End of histparams.h


