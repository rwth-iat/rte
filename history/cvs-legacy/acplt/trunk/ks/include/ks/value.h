/* -*-plt-c++-*- */
/* 
 *  Copyright (c) 1996 PLT, RWTH-Aachen, Germany. See file copy_plt.txt!
 *  Author: Martin Kneissl <martin@plt.rwth-aachen.de>
 *
 *  03/06/97 Interfaces changed
 *  03/07/97 New streaming ability implemented
 *
 */

#ifndef KS_VALUE_INCLUDED
#define KS_VALUE_INCLUDED

#include <plt/debug.h>
#include <plt/rtti.h>
#include <plt/array.h>

#include <ks/xdr.h>
#include <ks/ks.h>
#include <ks/string.h>
#include <ks/time.h>

//////////////////////////////////////////////////////////////////////
// KsValue is the base class of value objects. It is closely related
// to struct KS_VAR_VALUE

class KsValue : 
public KsXdrUnion 
{
    
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
    
    KS_DECL_XDRUNION(KsValue);
    
    PLT_DECL_RTTI;

protected:
    bool xdrEncodeCommon(XDR *) const;
    bool xdrDecodeCommon(XDR *);

};

//////////////////////////////////////////////////////////////////////

class KsIntValue
: public KsValue 
{
public:
    KsIntValue(long l = 0L);

    void getInt(long& v) const;
    void setInt(long v);

    operator long () const;
    KsIntValue & operator = (long v);

    virtual enum_t xdrTypeCode() const { return KS_VT_INT; }

protected:
    virtual bool xdrEncodeVariant(XDR *xdr) const;
    virtual bool xdrDecodeVariant(XDR *);

private:
    long val;

    friend KsValue;
    KsIntValue(XDR *, bool &);
    
    PLT_DECL_RTTI;
};

//////////////////////////////////////////////////////////////////////

class KsUIntValue
: public KsValue 
{
public:
    KsUIntValue(unsigned long l = 0L);

    void getUInt(unsigned long& v) const;
    void setUInt(unsigned long v);

    virtual enum_t xdrTypeCode() const { return KS_VT_UINT; }

    operator unsigned long () const { return val; }

protected:
    virtual bool xdrEncodeVariant(XDR *) const;
    virtual bool xdrDecodeVariant(XDR *);

private:
    unsigned long val;

    friend KsValue;
    KsUIntValue(XDR *, bool &);
    
    PLT_DECL_RTTI;
};

//////////////////////////////////////////////////////////////////////

class KsSingleValue
: public KsValue 
{
public:
    KsSingleValue(float v = 0.0);

    void getSingle(float & v) const;
    void setSingle(float v);

    virtual enum_t xdrTypeCode() const { return KS_VT_SINGLE; }

    operator float () const { return val; }

protected:
    virtual bool xdrEncodeVariant(XDR *) const;
    virtual bool xdrDecodeVariant(XDR *);

private:
    float val;

    friend KsValue;
    KsSingleValue(XDR *, bool &);
    
    PLT_DECL_RTTI;
};


//////////////////////////////////////////////////////////////////////

class KsDoubleValue
: public KsValue 
{
public:
    KsDoubleValue(double d = 0.0);
    void getDouble(double &v) const;
    void setDouble(double v);
    virtual enum_t xdrTypeCode() const { return KS_VT_DOUBLE; }

    operator double () const { return val; }

protected:
    virtual bool xdrEncodeVariant(XDR *) const;
    virtual bool xdrDecodeVariant(XDR *);

private:
    double val;

    friend KsValue;
    KsDoubleValue(XDR *, bool &);

    PLT_DECL_RTTI;
};


//////////////////////////////////////////////////////////////////////
// class KsStringValue
//////////////////////////////////////////////////////////////////////

class KsStringValue : public KsString, public KsValue 
{
public:
    KsStringValue();
    KsStringValue(const char *);
    
    // redefinitions for resolving multiply function definitions 
    // in base classes
    virtual bool xdrEncode(XDR *) const;
    virtual bool xdrDecode(XDR *);

    virtual enum_t xdrTypeCode() const;

protected:
    virtual bool xdrEncodeVariant(XDR *) const;
    virtual bool xdrDecodeVariant(XDR *);

private:
    friend KsValue;
    KsStringValue(XDR *, bool &);
}; 

//////////////////////////////////////////////////////////////////////
// class KsTimeValue
//////////////////////////////////////////////////////////////////////

class KsTimeValue : public KsTime, public KsValue
{
public:
    KsTimeValue( long sec = 0L, long usec = 0L );
    
    // redefinitions for resolving multiply function definitions 
    // in base classes
    virtual bool xdrEncode(XDR *) const;
    virtual bool xdrDecode(XDR *);

    virtual enum_t xdrTypeCode() const;

protected:
    bool xdrEncodeVariant(XDR *) const;
    bool xdrDecodeVariant(XDR *);

private:
    friend KsValue;
    KsTimeValue(XDR *, bool &);
};


//////////////////////////////////////////////////////////////////////
// class KsVoidValue
//////////////////////////////////////////////////////////////////////

class KsVoidValue : public KsValue
{
public:
    KsVoidValue() {}
    virtual enum_t xdrTypeCode() const;

protected:
    bool xdrEncodeVariant(XDR *) const;
    bool xdrDecodeVariant(XDR *);

private:
    friend KsValue;
    KsVoidValue(XDR *, bool &ok);
};


//////////////////////////////////////////////////////////////////////
// class KsVecValueBase
//////////////////////////////////////////////////////////////////////

template <class T>
class KsVecValueBase 
: public PltArray<T>, public KsValue
{
public:
    KsVecValueBase(size_t size = 0);
    KsVecValueBase(size_t size, char *p, PltOwnership os);

protected:
    virtual bool xdrEncodeElem(XDR *, size_t) const = 0;
    virtual bool xdrDecodeElem(XDR *, size_t) = 0;

    bool xdrEncodeVariant(XDR *) const;
    bool xdrDecodeVariant(XDR *);
};


//////////////////////////////////////////////////////////////////////
// class KsByteVecValue
//////////////////////////////////////////////////////////////////////

class KsByteVecValue 
: public KsVecValueBase<char>
{
public:
    KsByteVecValue(size_t size = 0);
    KsByteVecValue(size_t size, char *p, PltOwnership os);

    enum_t xdrTypeCode() const;

protected:
    bool xdrEncodeElem(XDR *, size_t) const;
    bool xdrDecodeElem(XDR *, size_t);

private:
    friend KsValue;
    KsVecValueBase(XDR *,bool &); 
};

//////////////////////////////////////////////////////////////////////
// Inline implementation
//////////////////////////////////////////////////////////////////////

inline bool
KsValue::xdrEncodeCommon(XDR *) const
{
    return true;
}

//////////////////////////////////////////////////////////////////////

inline bool
KsValue::xdrDecodeCommon(XDR *)
{
    return true;
}

//////////////////////////////////////////////////////////////////////

inline
KsIntValue::KsIntValue(long v) 
: val(v)
{
}

//////////////////////////////////////////////////////////////////////

inline
KsIntValue::KsIntValue(XDR *xdr, bool &ok)
{
    ok = xdrDecodeVariant(xdr);
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

inline
KsUIntValue::KsUIntValue(XDR *xdr, bool &ok)
{
    ok = xdrDecodeVariant(xdr);
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

inline
KsSingleValue::KsSingleValue(XDR *xdr, bool &ok)
{
    ok = xdrDecodeVariant(xdr);
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

inline
KsDoubleValue::KsDoubleValue(XDR *xdr, bool &ok)
{
    ok = xdrDecodeVariant(xdr);
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

inline 
KsStringValue::KsStringValue() 
: KsString()
{}

//////////////////////////////////////////////////////////////////////

inline 
KsStringValue::KsStringValue(const char *sz)
: KsString(sz)
{}

//////////////////////////////////////////////////////////////////////

inline
KsStringValue::KsStringValue(XDR *xdr, bool &ok)
: KsString(xdr, ok)
{}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

inline 
KsTimeValue::KsTimeValue(long sec, long usec)
: KsTime(sec,usec)
{}

//////////////////////////////////////////////////////////////////////

inline
KsTimeValue::KsTimeValue(XDR *xdr, bool &ok)
: KsTime(xdr, ok)
{}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

inline
KsVoidValue::KsVoidValue(XDR *, bool &ok)
{
    ok = true;
}


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

template <class T>
inline
KsVecValueBase<T>::KsVecValueBase(size_t size) : PltArray<T>(size)
{}

//////////////////////////////////////////////////////////////////////

template <class T>
inline
KsVecValueBase<T>::KsVecValueBase(size_t size, T *p, PltOwnership os)
: PltArray<T>(size, p, os)

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

inline
KsByteVecValue::KsByteVecValue(size_t size) 
: KsVecValueBase<char>(size)
{}

//////////////////////////////////////////////////////////////////////

inline
KsByteVecValue::KsByteVecValue(size_t size, char *p, PltOwnership os)
: KsVecValueBase<char>(size, p, os) {}

//////////////////////////////////////////////////////////////////////

inline enum_t
KsByteVecValue::xdrTypeCode() const
{
    return KS_VT_BYTE_VEC;
}

//////////////////////////////////////////////////////////////////////

inline bool
KsByteVecValue::xdrEncodeElem(XDR *xdr, size_t i) const
{
    return xdr_char(xdr, &(a_array[i]));
}

//////////////////////////////////////////////////////////////////////

inline bool
KsByteVecValue::xdrDecodeElem(XDR *xdr, size_t i)
{
    return xdr_char(xdr, &(a_array[i]));
}

#endif




