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

#include "ks/xdr.h"
#include "ks/value.h"
#include "ks/list.h"
#include "ks/result.h"

/////////////////////////////////////////////////////////////////////////////
// Constants for GetHist service
/////////////////////////////////////////////////////////////////////////////

const KsString KS_HISTORY_PROTOCOL_NAME("plt_histories");

const u_long KS_GETHIST_MINOR_OPCODE  = 0x00000001;

enum KS_TIME_SELECTOR_TYPE_ENUM {
    KS_TST_ABSOLUTE = 0,
    KS_TST_RELATIVE = 1
};
typedef enum_t KS_TIME_SELECTOR_TYPE; 

enum KS_INTERPOLATION_MODE_ENUM {
    KS_IPM_DEFAULT = 0,
    KS_IPM_NONE    = 1,
    KS_IPM_LINEAR  = 2,
    KS_IPM_MIN     = 3,
    KS_IPM_MAX     = 4,
    KS_IPM_HOLD    = 5
};
typedef enum_t KS_INTERPOLATION_MODE;

/////////////////////////////////////////////////////////////////////////////
// Classes for GetHist request
//   - KsGHTimeSel
//   - KsGHStringSel
//   - KsGetHistParams
/////////////////////////////////////////////////////////////////////////////

#if 0
//
// Keep things simple: 
// No common base class for KsGHTimeSel and KsGHStringSel
//
class KsGHSelector
    : public KsXdrAble
{
public:
    KsGHSelector(XDR *, bool &);
    KsGHSelector();

    bool xdrEncode(XDR *) const;
    bool xdrDecode(XDR *);
    static KsGHSelector *xdrNew(XDR *);

    KsString sel;
};

typedef KsPtrHandle<KsGHSelector> KsGHSelectorHandle;

#endif

/////////////////////////////////////////////////////////////////////////////

class KsGHTimeSel
    : public KsXdrAble
{
public:
    KsGHTimeSel(XDR *, bool &);
    KsGHTimeSel();
    KsGHTimeSel(const PltString &sel_name,
                KS_TIME_SELECTOR_TYPE sel_type,
                const PltTime &sel_from,
                const PltTime &sel_to,
                const PltTime &sel_delta);

    bool xdrEncode(XDR *) const;
    bool xdrDecode(XDR *);
    static KsGHTimeSel *xdrNew(XDR *);

    KsString               sel;
    KS_TIME_SELECTOR_TYPE  type;
    KsTime                 from;
    KsTime                 to;
    KsTime                 delta;
};

/////////////////////////////////////////////////////////////////////////////

class KsGHStringSel
    : public KsXdrAble
{
public:
    KsGHStringSel(XDR *, bool &);
    KsGHStringSel();
    KsGHStringSel(const PltString &sel_name,
                  const PltString &sel_mask);

    bool xdrEncode(XDR *) const;
    bool xdrDecode(XDR *);
    static KsGHStringSel *xdrNew(XDR *);

    // Operator is needed to instantiate PltList
    //
    bool operator == (const KsGHStringSel &) const
    { PLT_ASSERT(0); return false; }

    KsString sel;
    KsString mask;
};

/////////////////////////////////////////////////////////////////////////////

class KsGetHistParams
    : public KsXdrAble
{
public:
    KsGetHistParams(XDR *, bool &);
    KsGetHistParams();
    KsGetHistParams(const PltString &aName,
                    KS_INTERPOLATION_MODE ipMode,
                    u_long maxEntries,
                    const KsGHTimeSel &timeSelector);

    bool xdrEncode(XDR *) const;
    bool xdrDecode(XDR *);
    static KsGetHistParams *xdrNew(XDR *);

    KsString               name;
    KS_INTERPOLATION_MODE  ip_mode;
    u_long                 max_entries;

    KsGHTimeSel            time_selector;

    KsList<KsGHStringSel>  selectors;
};

/////////////////////////////////////////////////////////////////////////////
// Classes for GetHist reply:
//   - KsGHResultField
//   - KsGetHistResult
/////////////////////////////////////////////////////////////////////////////

class KsGHResultField
    : public KsXdrAble
{
public:
    KsGHResultField();
    KsGHResultField(XDR *, bool &);

    bool xdrEncode(XDR *) const;
    bool xdrDecode(XDR *);
    static KsGHResultField *xdrNew(XDR *);

    // Operator is necessary to instantiate PltList
    bool operator == (const KsGHResultField &) 
    { PLT_ASSERT(0); return false; }

    KsString      sel;
    KsValueHandle value;
};

/////////////////////////////////////////////////////////////////////////////

class KsGetHistResult
    : public KsResult
{
public:
    KsGetHistResult();
    KsGetHistResult(XDR *, bool &);

    bool xdrEncode(XDR *) const;
    bool xdrDecode(XDR *);
    static KsGetHistResult *xdrNew(XDR *);

    KsList<KsGHResultField> fields;
};

/////////////////////////////////////////////////////////////////////////////
// INLINE IMPLEMENTATION
/////////////////////////////////////////////////////////////////////////////

inline
KsGHTimeSel::KsGHTimeSel()
{}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

inline
KsGHStringSel::KsGHStringSel()
{}

/////////////////////////////////////////////////////////////////////////////

inline
KsGHStringSel::KsGHStringSel(const PltString &sel_name,
                             const PltString &sel_mask)
    : sel(sel_name),
      mask(sel_mask)
{}

/////////////////////////////////////////////////////////////////////////////

inline 
bool 
KsGHStringSel::xdrEncode(XDR *xdr) const
{
    PLT_PRECONDITION(xdr->x_op == XDR_ENCODE);

    return sel.xdrEncode(xdr)
        && mask.xdrEncode(xdr);
}

/////////////////////////////////////////////////////////////////////////////

inline 
bool 
KsGHStringSel::xdrDecode(XDR *xdr)
{
    PLT_PRECONDITION(xdr->x_op == XDR_DECODE);

    return sel.xdrDecode(xdr)
        && mask.xdrDecode(xdr);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

inline
KsGetHistParams::KsGetHistParams()
{}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

inline
KsGHResultField::KsGHResultField()
{}

/////////////////////////////////////////////////////////////////////////////

inline 
bool 
KsGHResultField::xdrEncode(XDR *xdr) const
{
    PLT_PRECONDITION(xdr->x_op == XDR_ENCODE);
    
    return sel.xdrEncode(xdr)
        && value.xdrEncode(xdr);
}

/////////////////////////////////////////////////////////////////////////////

inline 
bool 
KsGHResultField::xdrDecode(XDR *xdr)
{
    PLT_PRECONDITION(xdr->x_op == XDR_DECODE);
    
    return sel.xdrDecode(xdr)
        && value.xdrDecode(xdr);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

inline
KsGetHistResult::KsGetHistResult()
{}

/////////////////////////////////////////////////////////////////////////////

#endif // KS_HISTPARAMS_INCLUDED



