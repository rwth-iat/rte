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

enum KS_HSEL_TYPE_ENUM {
    KS_HSELT_NONE    = 0x0000,
    KS_HSELT_TIME    = 0x0001,
    KS_HSELT_STRING  = 0x0002

};
typedef enum_t KS_HSEL_TYPE;

/////////////////////////////////////////////////////////////////////////////

enum KS_TIME_TYPE_ENUM {
    KS_TT_ABSOLUTE = 0,
    KS_TT_RELATIVE = 1
};
typedef enum_t KS_TIME_TYPE; 

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
// A unified class for times and time spans
/////////////////////////////////////////////////////////////////////////////

class KsAbsRelTime
    : public KsXdrUnion
{
public:
    inline KsAbsRelTime();
    KsAbsRelTime(const PltTime &t);
    KsAbsRelTime(const PltTimeSpan &ts);
    KsAbsRelTime(XDR *, bool &);

    KS_TIME_TYPE xdrTypeCode() const { return type; }

    operator PltTime () const;
    operator PltTimeSpan () const;

    KsAbsRelTime &operator = (const PltTime &t);
    KsAbsRelTime &operator = (const PltTimeSpan &ts);

    virtual bool xdrEncode(XDR *) const;
    virtual bool xdrDecode(XDR *) ;
    static KsAbsRelTime * xdrNew(XDR *);

protected:
    virtual bool xdrEncodeCommon(XDR *) const
    { PLT_ASSERT(0); return false; }
    virtual bool xdrDecodeCommon(XDR *)
    { PLT_ASSERT(0); return false; }             
    virtual bool xdrEncodeVariant(XDR *) const
    { PLT_ASSERT(0); return false; }
    virtual bool xdrDecodeVariant(XDR *)
    { PLT_ASSERT(0); return false; }

    KS_TIME_TYPE type;
    union {
        struct {
            u_long sec;
            u_long usec;
        } abs;
        struct {
            long sec;
            long usec;
        } rel;
    } time;

    PLT_DECL_RTTI;
};

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

    enum_t xdrTypeCode() const { return KS_HSELT_NONE; }

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
    KsTimeSel() : ip_mode(KS_IPM_DEFAULT) {}

    enum_t xdrTypeCode() const { return KS_HSELT_TIME; }

    KS_INTERPOLATION_MODE  ip_mode;
    KsAbsRelTime           from;
    KsAbsRelTime           to;
    KsTimeSpan             delta;
    
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

    enum_t xdrTypeCode() const { return KS_HSELT_STRING; }

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
KsAbsRelTime::KsAbsRelTime()
    : type(KS_TT_ABSOLUTE)
{
    time.abs.sec = 0;
    time.abs.usec = 0;
}

/////////////////////////////////////////////////////////////////////////////

inline
KsAbsRelTime::KsAbsRelTime(const PltTime &t)
    : type(KS_TT_ABSOLUTE)
{
    time.abs.sec = t.tv_sec;
    time.abs.usec = t.tv_usec;
}

/////////////////////////////////////////////////////////////////////////////

inline
KsAbsRelTime::KsAbsRelTime(const PltTimeSpan &ts)
    : type(KS_TT_RELATIVE)
{
    time.rel.sec = ts.tv_sec;
    time.rel.usec = ts.tv_usec;
}

/////////////////////////////////////////////////////////////////////////////

inline
KsAbsRelTime::operator PltTime () const
{
    return type == KS_TT_ABSOLUTE ? 
        PltTime(time.abs.sec, time.abs.usec) : PltTime(0,0);
}

/////////////////////////////////////////////////////////////////////////////

inline
KsAbsRelTime::operator PltTimeSpan () const
{
    return type == KS_TT_RELATIVE ? 
        PltTimeSpan(time.rel.sec, time.rel.usec) : PltTimeSpan(0,0);
}

/////////////////////////////////////////////////////////////////////////////

inline
KsAbsRelTime &
KsAbsRelTime::operator = (const PltTime &t)
{
    type = KS_TT_ABSOLUTE;
    time.abs.sec = t.tv_sec;
    time.abs.usec = t.tv_usec;
    return *this;
}

/////////////////////////////////////////////////////////////////////////////

inline
KsAbsRelTime &
KsAbsRelTime::operator = (const PltTimeSpan &ts)
{
    type = KS_TT_RELATIVE;
    time.rel.sec = ts.tv_sec;
    time.rel.usec = ts.tv_usec;
    return *this;
}

/////////////////////////////////////////////////////////////////////////////
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



