/* -*-plt-c++-*- */
/* 
 *  Copyright (c) 1996 PLT, RWTH-Aachen, Germany. See file copy_plt.txt!
 *  Author: Martin Kneissl <martin@plt.rwth-aachen.de>
 *
 */

#ifndef KS_VALUE_INCLUDED
#define KS_VALUE_INCLUDED

#include <plt/debug.h>
#include <ks/rpc.hpp>
#include <ks/xdr.hpp>

#if PLT_DEBUG
#include <iostream.h>
#endif

//////////////////////////////////////////////////////////////////////
// KsValue is the base class of value objects. It is closely related
// to struct KS_VAR_VALUE

class KsValue;

extern "C" bool_t xdr_KsValue(XDR *, KsValue **);

class KsValue : public KsXdrUnion {
    friend bool_t xdr_KsValue(XDR *, KsValue **);
protected:
    // XDR routines
    virtual enum_t xdrTypeCode() const = 0;
    virtual bool_t xdr(XDR *) = 0;
#if PLT_DEBUG
public:
    friend ostream & operator<<(ostream &, const KsValue &);
    virtual void printOn(ostream &) const = 0;
#endif
};


//////////////////////////////////////////////////////////////////////

class KsIntValue : public KsValue {
public:
    KsIntValue(long l = 0L);
protected:
    virtual enum_t xdrTypeCode() const { return KS_VT_INT; }
    virtual bool_t xdr(XDR *);
private:
    long val;
#if PLT_DEBUG
public:
    virtual void printOn(ostream &) const;
#endif
};

//////////////////////////////////////////////////////////////////////

class KsDoubleValue : public KsValue {
public:
    KsDoubleValue(double d = 0.0);
protected:
    virtual enum_t xdrTypeCode() const { return KS_VT_DOUBLE; }
    virtual bool_t xdr(XDR *);
private:
    double val;
#if PLT_DEBUG
public:
    virtual void printOn(ostream &) const;
#endif
};


//////////////////////////////////////////////////////////////////////
// Inline implementation
//////////////////////////////////////////////////////////////////////

inline
KsIntValue::KsIntValue(long l) 
: val(l)
{
}


//////////////////////////////////////////////////////////////////////

#if PLT_DEBUG

inline ostream & 
operator << (ostream & ostr, const KsValue &v)
{
    v.printOn(ostr);
    return ostr;
}

#endif

//////////////////////////////////////////////////////////////////////

inline bool_t
KsIntValue::xdr(XDR * xdrs)
{
    return xdr_long(xdrs, &val);
}


//////////////////////////////////////////////////////////////////////

inline
KsDoubleValue::KsDoubleValue(double d)
: val(d)
{
}

//////////////////////////////////////////////////////////////////////

inline bool_t
KsDoubleValue::xdr(XDR * xdrs)
{
    return xdr_double(xdrs, &val);
}

#endif

