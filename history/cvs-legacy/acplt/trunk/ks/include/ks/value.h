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

#include "plt/debug.h"
#include "plt/rtti.h"

#include "ks/xdr.h"
#include "ks/ks.h"
#include "ks/array.h"
#include "ks/string.h"
#include "ks/time.h"
#include "ks/handle.h"

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
    
protected:
    bool xdrEncodeCommon(XDR *) const;
    bool xdrDecodeCommon(XDR *);
    PLT_DECL_RTTI;

#if PLT_DEBUG
public:
    virtual void debugPrint(ostream & ostr) const;
#endif
};


//////////////////////////////////////////////////////////////////////
#if PLT_DEBUG
inline ostream &
operator << (ostream & ostr, const KsValue & v)
{
    v.debugPrint(ostr);
    return ostr;
}
#endif
//////////////////////////////////////////////////////////////////////

typedef KsPtrHandle<KsValue> KsValueHandle;

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

#if PLT_DEBUG
public:
    virtual void debugPrint(ostream & ostr) const;
#endif
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

#if PLT_DEBUG
public:
    virtual void debugPrint(ostream & ostr) const;
#endif
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

#if PLT_DEBUG
public:
    virtual void debugPrint(ostream & ostr) const;
#endif
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

#if PLT_DEBUG
public:
    virtual void debugPrint(ostream & ostr) const;
#endif
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

    PLT_DECL_RTTI;

#if PLT_DEBUG
public:
    virtual void debugPrint(ostream & ostr) const;
#endif
}; 

//////////////////////////////////////////////////////////////////////
// class KsTimeValue
//////////////////////////////////////////////////////////////////////

class KsTimeValue : public KsTime, public KsValue
{
public:
    KsTimeValue( long sec = 0L, long usec = 0L );
    KsTimeValue( const KsTime & );
    
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

    PLT_DECL_RTTI;

#if PLT_DEBUG
public:
    virtual void debugPrint(ostream & ostr) const;
#endif
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

    PLT_DECL_RTTI;

#if PLT_DEBUG
public:
    virtual void debugPrint(ostream & ostr) const;
#endif
};


//////////////////////////////////////////////////////////////////////
// class KsVecValueBase
//////////////////////////////////////////////////////////////////////

template <class T>
class KsVecValueBase
: public KsArray<T>, public KsValue
{
public:
    KsVecValueBase(size_t size = 0);
    KsVecValueBase(size_t size, T *p, PltOwnership os);

    // redefinition in order to resolve ambiguity between
    // functions of base classes
    virtual bool xdrDecode(XDR *);
    virtual bool xdrEncode(XDR *) const;

protected:
    virtual bool xdrEncodeVariant(XDR *) const;
    virtual bool xdrDecodeVariant(XDR *);
    // sorry : no RTTI
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

private:
    friend KsValue;
    KsByteVecValue(XDR *, bool &);

    PLT_DECL_RTTI;

};


//////////////////////////////////////////////////////////////////////
// class KsIntVecValue
//////////////////////////////////////////////////////////////////////

class KsIntVecValue 
: public KsVecValueBase<long>
{
public:
    KsIntVecValue(size_t size = 0);
    KsIntVecValue(size_t size, long *p, PltOwnership os);

    enum_t xdrTypeCode() const;

private:
    friend KsValue;
    KsIntVecValue(XDR *, bool &);

    PLT_DECL_RTTI;
};

//////////////////////////////////////////////////////////////////////
// class KsUIntVecValue
//////////////////////////////////////////////////////////////////////

class KsUIntVecValue 
: public KsVecValueBase<u_long>
{
public:
    KsUIntVecValue(size_t size = 0);
    KsUIntVecValue(size_t size, u_long *p, PltOwnership os);

    enum_t xdrTypeCode() const;

private:
    friend KsValue;
    KsUIntVecValue(XDR *, bool &);

    PLT_DECL_RTTI;
};

//////////////////////////////////////////////////////////////////////
// class KsSingleVecValue
//////////////////////////////////////////////////////////////////////

class KsSingleVecValue 
: public KsVecValueBase<float>
{
public:
    KsSingleVecValue(size_t size = 0);
    KsSingleVecValue(size_t size, float *p, PltOwnership os);

    enum_t xdrTypeCode() const;

private:
    friend KsValue;
    KsSingleVecValue(XDR *, bool &);

    PLT_DECL_RTTI;
};

//////////////////////////////////////////////////////////////////////
// class KsDoubleVecValue
//////////////////////////////////////////////////////////////////////

class KsDoubleVecValue 
: public KsVecValueBase<double>
{
public:
    KsDoubleVecValue(size_t size = 0);
    KsDoubleVecValue(size_t size, double *p, PltOwnership os);

    enum_t xdrTypeCode() const;

private:
    friend KsValue;
    KsDoubleVecValue(XDR *, bool &);

    PLT_DECL_RTTI;

#if PLT_DEBUG
public:
    virtual void debugPrint(ostream & ostr) const;
#endif
};

//////////////////////////////////////////////////////////////////////
// class KsStringVecValue
//////////////////////////////////////////////////////////////////////

class KsStringVecValue 
: public KsVecValueBase<KsString>
{
public:
    KsStringVecValue(size_t size = 0);
    KsStringVecValue(size_t size, KsString *p, PltOwnership os);

    enum_t xdrTypeCode() const;

private:
    friend KsValue;
    KsStringVecValue(XDR *, bool &);

    PLT_DECL_RTTI;
};

//////////////////////////////////////////////////////////////////////
// class KsTimeVecValue
//////////////////////////////////////////////////////////////////////

class KsTimeVecValue 
: public KsVecValueBase<KsTime>
{
public:
    KsTimeVecValue(size_t size = 0);
    KsTimeVecValue(size_t size, KsTime *p, PltOwnership os);

    enum_t xdrTypeCode() const;

private:
    friend KsValue;
    KsTimeVecValue(XDR *, bool &);

    PLT_DECL_RTTI;
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
KsTimeValue::KsTimeValue(const KsTime &time)
: tv_sec(time.tv_sec),
  tv_usec(time.tv_usec)
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

inline
KsByteVecValue::KsByteVecValue(size_t size)
: KsVecValueBase<char>(size)
{}

//////////////////////////////////////////////////////////////////////

inline
KsByteVecValue::KsByteVecValue(size_t size, char *p, PltOwnership os)
: KsVecValueBase<char>(size, p, os)
{}

//////////////////////////////////////////////////////////////////////

inline
KsByteVecValue::KsByteVecValue(XDR *xdr, bool &ok)
{
    ok = KsArray<char>::xdrDecode(xdr);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

inline
KsIntVecValue::KsIntVecValue(size_t size)
: KsVecValueBase<long>(size)
{}

//////////////////////////////////////////////////////////////////////

inline
KsIntVecValue::KsIntVecValue(size_t size, long *p, PltOwnership os)
: KsVecValueBase<long>(size, p, os)
{}

//////////////////////////////////////////////////////////////////////

inline
KsIntVecValue::KsIntVecValue(XDR *xdr, bool &ok)
{
    ok = xdrDecodeVariant(xdr);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

inline
KsUIntVecValue::KsUIntVecValue(size_t size)
: KsVecValueBase<u_long>(size)
{}

//////////////////////////////////////////////////////////////////////

inline
KsUIntVecValue::KsUIntVecValue(size_t size, u_long *p, PltOwnership os)
: KsVecValueBase<u_long>(size, p, os)
{}

//////////////////////////////////////////////////////////////////////

inline
KsUIntVecValue::KsUIntVecValue(XDR *xdr, bool &ok)
{
    ok = xdrDecodeVariant(xdr);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

inline
KsSingleVecValue::KsSingleVecValue(size_t size)
: KsVecValueBase<float>(size)
{}

//////////////////////////////////////////////////////////////////////

inline
KsSingleVecValue::KsSingleVecValue(size_t size, float *p, PltOwnership os)
: KsVecValueBase<float>(size, p, os)
{}

//////////////////////////////////////////////////////////////////////

inline
KsSingleVecValue::KsSingleVecValue(XDR *xdr, bool &ok)
{
    ok = xdrDecodeVariant(xdr);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

inline
KsDoubleVecValue::KsDoubleVecValue(size_t size)
: KsVecValueBase<double>(size)
{}

//////////////////////////////////////////////////////////////////////

inline
KsDoubleVecValue::KsDoubleVecValue(size_t size, double *p, PltOwnership os)
: KsVecValueBase<double>(size, p, os)
{}

//////////////////////////////////////////////////////////////////////

inline
KsDoubleVecValue::KsDoubleVecValue(XDR *xdr, bool &ok)
{
    ok = xdrDecodeVariant(xdr);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

inline
KsStringVecValue::KsStringVecValue(size_t size)
: KsVecValueBase<KsString>(size)
{}

//////////////////////////////////////////////////////////////////////

inline
KsStringVecValue::KsStringVecValue(size_t size, KsString *p, PltOwnership os)
: KsVecValueBase<KsString>(size, p, os)
{}

//////////////////////////////////////////////////////////////////////

inline
KsStringVecValue::KsStringVecValue(XDR *xdr, bool &ok)
{
    ok = xdrDecodeVariant(xdr);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

inline
KsTimeVecValue::KsTimeVecValue(size_t size)
: KsVecValueBase<KsTime>(size)
{}

//////////////////////////////////////////////////////////////////////

inline
KsTimeVecValue::KsTimeVecValue(size_t size, KsTime *p, PltOwnership os)
: KsVecValueBase<KsTime>(size, p, os)
{}

//////////////////////////////////////////////////////////////////////

inline
KsTimeVecValue::KsTimeVecValue(XDR *xdr, bool &ok)
{
    ok = xdrDecodeVariant(xdr);
}


#endif





