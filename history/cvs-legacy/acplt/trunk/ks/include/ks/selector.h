/* -*-plt-c++-*- */

#ifndef KS_SELECTOR_INCLUDED
#define KS_SELECTOR_INCLUDED

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

#include <plt/rtti.h>

#include "ks/xdr.h"
#include "ks/handle.h"
#include "ks/string.h"
#include "ks/time.h"

/////////////////////////////////////////////////////////////////////////////

enum KS_SELECTOR_TYPE_ENUM {
    KS_SEL_NONE,
    KS_SEL_STRING,
    KS_SEL_TIME
};
typedef enum_t KS_SELECTOR_TYPE;

/////////////////////////////////////////////////////////////////////////////

enum KS_TIME_SELECTOR_TYPE_ENUM {
    KS_TST_ABSOLUTE = 0,
    KS_TST_RELATIVE = 1
};
typedef enum_t KS_TIME_SELECTOR_TYPE; 

/////////////////////////////////////////////////////////////////////////////

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
// Common Base class for selectors
// 
class KsSelector
    : public KsXdrUnion
{
public:
    KsSelector(XDR *, bool &);
    KsSelector() {}

protected:
    bool xdrEncodeCommon(XDR *) const { return true; }
    bool xdrDecodeCommon(XDR *) { return true; }

    PLT_DECL_RTTI;
    KS_DECL_XDRUNION(KsSelector);
};

typedef KsPtrHandle<KsSelector> KsSelectorHandle;

/////////////////////////////////////////////////////////////////////////////
// Dummy selector for no selection criteria
//
class KsNoneSel
    : public KsSelector
{
public:
    KsNoneSel() {}

    enum_t xdrTypeCode() const { return KS_SEL_NONE; }

protected:    
    bool xdrEncodeVariant(XDR *) const { return true; }
    bool xdrDecodeVariant(XDR  *) { return true; }

private:
    KsNoneSel(XDR *, bool &);
    friend class KsSelector;

    PLT_DECL_RTTI;
};

/////////////////////////////////////////////////////////////////////////////
// Selector for times
//
class KsTimeSel
    : public KsSelector
{
public:
    KsTimeSel() {}
    KsTimeSel(KS_TIME_SELECTOR_TYPE atype,
              KsTime afrom,
              KsTime ato,
              KsTime adelta,
              KS_INTERPOLATION_MODE amode = KS_IPM_DEFAULT);

    enum_t xdrTypeCode() const { return KS_SEL_TIME; }

    KS_TIME_SELECTOR_TYPE  type;
    KS_INTERPOLATION_MODE  ip_mode;
    KsTime                 from;
    KsTime                 to;
    KsTime                 delta;
    
protected:
    bool xdrEncodeVariant(XDR *) const;
    bool xdrDecodeVariant(XDR *);

private:
    KsTimeSel(XDR *, bool &);
    friend class KsSelector;

    PLT_DECL_RTTI;
};

/////////////////////////////////////////////////////////////////////////////
// Selector for strings
// Currently there is no specification how to form a mask and 
// therefore mask is not restricted to any special form.
//
class KsStringSel
    : public KsSelector
{
public:
    KsStringSel() {}
    KsStringSel(const PltString &sel_mask);

    enum_t xdrTypeCode() const { return KS_SEL_STRING; }

    KsString mask;

protected:
    bool xdrEncodeVariant(XDR *) const;
    bool xdrDecodeVariant(XDR *);

private:
    KsStringSel(XDR *, bool &);
    friend class KsSelector;
    
    PLT_DECL_RTTI;
};

/////////////////////////////////////////////////////////////////////////////
// INLINE IMPLEMENTATION
/////////////////////////////////////////////////////////////////////////////

inline
KsNoneSel::KsNoneSel(XDR *xdr, bool &ok)
{
    ok = xdrDecodeVariant(xdr);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

inline
KsTimeSel::KsTimeSel(XDR *xdr, bool &ok)
{
    ok = xdrDecodeVariant(xdr);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

inline
KsStringSel::KsStringSel(const PltString &sel_mask)
    : mask(sel_mask)
{}

/////////////////////////////////////////////////////////////////////////////

inline 
bool 
KsStringSel::xdrEncodeVariant(XDR *xdr) const
{
    PLT_PRECONDITION(xdr->x_op == XDR_ENCODE);

    return mask.xdrEncode(xdr);
}

/////////////////////////////////////////////////////////////////////////////

inline 
bool 
KsStringSel::xdrDecodeVariant(XDR *xdr)
{
    PLT_PRECONDITION(xdr->x_op == XDR_DECODE);

    return mask.xdrDecode(xdr);
}

/////////////////////////////////////////////////////////////////////////////

inline
KsStringSel::KsStringSel(XDR *xdr, bool &ok)
{
    ok = xdrDecodeVariant(xdr);
}

/////////////////////////////////////////////////////////////////////////////

#endif // KS_SELECTOR_INCLUDED



