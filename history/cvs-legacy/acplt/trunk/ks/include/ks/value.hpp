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
#if 0
    // accessors
    virtual bool getAsInt(long &l) const;
    virtual bool getAsUInt(unsigned long &u) const;
    virtual bool getAsSingle(float &f) const;
    virtual bool getAsDouble(double &d) const;
    virtual bool getAsString(PltString &s) const;
    virtual bool getAsTime(PltTime &t) const;

    virtual bool getAsIntAt(size_t n,    long &l) const;
    virtual bool getAsUIntAt(size_t n,   unsigned long &u) const;
    virtual bool getAsSingleAt(size_t n, float &f) const;
    virtual bool getAsDoubleAt(size_t n, double &d) const;
    virtual bool getAsStringAt(size_t n, PltString &s) const;
    virtual bool getAsTimeAt(size_t n,   PltTime &t) const;

    // modifiers
    virtual bool setFromInt(long   l);
    virtual bool setFromUInt(unsigned long u);
    virtual bool setFromSingle(float f);
    virtual bool setFromDouble(double d);
    virtual bool setFromString(const PltString &s);
    virtual bool setFromTime(const PltTime &t);

    virtual bool setFromIntAt(size_t n,    long l);
    virtual bool setFromUIntAt(size_t n,   unsigned long u);
    virtual bool setFromSingleAt(size_t n, float f);
    virtual bool setFromDoubleAt(size_t n, double d);
    virtual bool setFromStringAt(size_t n, const PltString &s);
    virtual bool setFromTimeAt(size_t n,   const PltTime &t);
#endif
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

    void getInt(long& v) const;
    void setInt(long v);

    operator long () const;
    KsIntValue & operator = (long v);

protected:
    virtual enum_t xdrTypeCode() const { return KS_VT_INT; }
    virtual bool_t xdr(XDR *);
private:
    long val;
    
    PLT_DECL_RTTI;
};

//////////////////////////////////////////////////////////////////////

class KsUIntValue : public KsValue {
public:
    KsUIntValue(unsigned long l = 0L);

    void getUInt(unsigned long& v) const;
    void setUInt(unsigned long v);

protected:
    virtual enum_t xdrTypeCode() const { return KS_VT_UINT; }
    virtual bool_t xdr(XDR *);
private:
    unsigned long val;
    
    PLT_DECL_RTTI;
};

//////////////////////////////////////////////////////////////////////

class KsSingleValue : public KsValue {
public:
    KsSingleValue(float v = 0.0);

    void getSingle(float & v) const;
    void setSingle(float v);

protected:
    virtual enum_t xdrTypeCode() const { return KS_VT_SINGLE; }
    virtual bool_t xdr(XDR *);

private:
    float val;
    
    PLT_DECL_RTTI;
};


//////////////////////////////////////////////////////////////////////

class KsDoubleValue : public KsValue {
public:
    KsDoubleValue(double d = 0.0);
    void getDouble(double &v) const;
    void setDouble(double v);
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
KsIntValue::KsIntValue(long v) 
: val(v)
{
}

//////////////////////////////////////////////////////////////////////

inline void
KsIntValue::getInt(long & v) const
{
    v = val;
}

//////////////////////////////////////////////////////////////////////

inline void
KsIntValue::setInt(long v)
{
    val = v;
}

//////////////////////////////////////////////////////////////////////

inline 
KsIntValue::operator long () const
{
    return val;
}

//////////////////////////////////////////////////////////////////////

inline KsIntValue &
KsIntValue::operator = (long v)
{
    val = v;
    return *this;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

inline
KsUIntValue::KsUIntValue(unsigned long v)
: val(v)
{
}

//////////////////////////////////////////////////////////////////////

inline void
KsUIntValue::getUInt(unsigned long & v) const
{
    v = val;
}

//////////////////////////////////////////////////////////////////////

inline void
KsUIntValue::setUInt(unsigned long v)
{
    val = v;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

inline
KsSingleValue::KsSingleValue(float v)
: val(v)
{
}

//////////////////////////////////////////////////////////////////////

inline void
KsSingleValue::getSingle(float & v) const
{
    v = val;
}

//////////////////////////////////////////////////////////////////////

inline void
KsSingleValue::setSingle(float v)
{
    val = v;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

inline
KsDoubleValue::KsDoubleValue(double v)
: val(v)
{
}

//////////////////////////////////////////////////////////////////////

inline void
KsDoubleValue::getDouble(double & v) const
{
    v = val;
}

//////////////////////////////////////////////////////////////////////

inline void
KsDoubleValue::setDouble(double v)
{
    val = v;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

#endif

