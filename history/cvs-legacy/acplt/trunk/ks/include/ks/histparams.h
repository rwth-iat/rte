/* -*-plt-c++-*- */

#ifndef KS_HISTPARAMS_INCLUDED
#define KS_HISTPARAMS_INCLUDED

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

#include <limits.h>

#include "ks/xdr.h"
#include "ks/value.h"
#include "ks/array.h"
#include "ks/result.h"
#include "ks/selector.h"

/////////////////////////////////////////////////////////////////////////////
// Constants for GetHist service
/////////////////////////////////////////////////////////////////////////////

const KsString KS_HISTORY_PROTOCOL_NAME("plt_histories");

const u_long KS_GETHIST_MINOR_OPCODE  = 0x00000001;

/////////////////////////////////////////////////////////////////////////////
// Classes for GetHist request
//   - KsGetHistItem
//   - KsGetHistParams
/////////////////////////////////////////////////////////////////////////////

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

/////////////////////////////////////////////////////////////////////////////

#endif // KS_HISTPARAMS_INCLUDED



