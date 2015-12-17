/* -*-plt-c++-*- */
#ifndef KS_SELECTOR_INCLUDED
#define KS_SELECTOR_INCLUDED
/*
 * $Header: /home/david/cvs/acplt/ks/include/ks/selector.h,v 1.13 2007-04-25 12:57:20 martin Exp $
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

/* Author : Markus Juergens <markusj@plt.rwth-aachen.de> */

/////////////////////////////////////////////////////////////////////////////

#include <plt/debug.h>
#include <plt/rtti.h>

#include "ks/ks.h"
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
    bool isAbsolute() const { return xdrTypeCode() == KS_TT_ABSOLUTE; }
    bool isRelative() const { return xdrTypeCode() == KS_TT_RELATIVE; }

    operator PltTime () const;
    operator PltTimeSpan () const;

    KsAbsRelTime &operator = (const PltTime &t);
    KsAbsRelTime &operator = (const PltTimeSpan &ts);

    virtual bool xdrEncode(XDR *) const;
    virtual bool xdrDecode(XDR *) ;
    static KsAbsRelTime * xdrNew(XDR *);

#if PLT_DEBUG
    void print(ostream &os) const;
#endif

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

#if PLT_DEBUG
    virtual void print(ostream &) const = 0;
#endif

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

#if PLT_DEBUG
    virtual void print(ostream &) const;
#endif

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

#if PLT_DEBUG
    virtual void print(ostream &) const;
#endif

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

#if PLT_DEBUG
    virtual void print(ostream &) const;
#endif

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



