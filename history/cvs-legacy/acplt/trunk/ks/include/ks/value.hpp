/* -*-plt-c++-*- */
/* 
 *  Copyright (c) 1996 PLT, RWTH-Aachen, Germany. See file copy_plt.txt!
 *  Author: Martin Kneissl <martin@plt.rwth-aachen.de>
 *
 */

#ifndef KS_VALUE_INCLUDED
#define KS_VALUE_INCLUDED

#include <plt/debug.h>
#include <plt/rtti.hpp>
#include <ks/rpc.hpp>
#include <ks/xdr.hpp>

//////////////////////////////////////////////////////////////////////
// KsValue is the base class of value objects. It is closely related
// to struct KS_VAR_VALUE

class KsValue;
extern "C" bool_t xdr_KsValue(XDR *, KsValue **);

////

class KsValue : public KsXdrUnion {
    friend bool_t xdr_KsValue(XDR *, KsValue **);
protected:
    // XDR routines
    virtual enum_t xdrTypeCode() const = 0;
    virtual bool_t xdr(XDR *) = 0;

    PLT_DECL_RTTI;
};

//////////////////////////////////////////////////////////////////////

class KsIntValue : public KsValue {
public:
    KsIntValue(long l = 0L);
    void setInt(long l);
    long getInt() const;
protected:
    virtual enum_t xdrTypeCode() const { return KS_VT_INT; }
    virtual bool_t xdr(XDR *);
private:
    long val;
    
    PLT_DECL_RTTI;
};

//////////////////////////////////////////////////////////////////////

class KsDoubleValue : public KsValue {
public:
    KsDoubleValue(double d = 0.0);
    void setDouble(double d);
    double getDouble() const;
protected:
    virtual enum_t xdrTypeCode() const { return KS_VT_DOUBLE; }
    virtual bool_t xdr(XDR *);
private:
    double val;

    PLT_DECL_RTTI;
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

inline void
KsIntValue::setInt(long l) 
{
    val = l;
}

//////////////////////////////////////////////////////////////////////

inline long
KsIntValue::getInt() const
{
    return val;
}

//////////////////////////////////////////////////////////////////////

inline bool_t
KsIntValue::xdr(XDR * xdrs)
{
    return xdr_long(xdrs, &val);
}



//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

inline
KsDoubleValue::KsDoubleValue(double d)
: val(d)
{
}

//////////////////////////////////////////////////////////////////////

inline void
KsDoubleValue::setDouble(double d) 
{
    val = d;
}

//////////////////////////////////////////////////////////////////////

inline double
KsDoubleValue::getDouble() const 
{
    return val;
}

//////////////////////////////////////////////////////////////////////

inline bool_t
KsDoubleValue::xdr(XDR * xdrs)
{
    return xdr_double(xdrs, &val);
}


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
#endif

