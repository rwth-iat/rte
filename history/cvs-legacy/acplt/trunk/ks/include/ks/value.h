/* -*-plt-c++-*- */ 
/* $Header: /home/david/cvs/acplt/ks/include/ks/value.h,v 1.22 2008-09-22 08:26:09 henning Exp $ */
/*
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

/*
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

#if PLT_COMPILER_CYGWIN
#include "ks/array_builtins.h"
#endif

#if PLT_COMPILER_GCC && PLT_COMPILER_GCC >= 0x40000
#include "ks/array_builtins.h"
#endif


//////////////////////////////////////////////////////////////////////
// KsValue is the base class of value objects. It is closely related
// to struct KS_VAR_VALUE

class KsValue : 
public KsXdrUnion 
{
public:
    virtual ~KsValue() {}
    
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

class KsBoolValue
: public KsValue 
{
public:
    KsBoolValue(bool = false);

    operator bool () const;
    KsBoolValue & operator = (bool b);

    virtual enum_t xdrTypeCode() const { return KS_VT_BOOL; }

protected:
    virtual bool xdrEncodeVariant(XDR *xdr) const;
    virtual bool xdrDecodeVariant(XDR *);

private:
    bool val;

    friend class KsValue;
    KsBoolValue(XDR *, bool &);
    
    PLT_DECL_RTTI;

#if PLT_DEBUG
public:
    virtual void debugPrint(ostream & ostr) const;
#endif
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

    friend class KsValue;
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
    KsUIntValue &operator = (unsigned long); 

protected:
    virtual bool xdrEncodeVariant(XDR *) const;
    virtual bool xdrDecodeVariant(XDR *);

private:
    unsigned long val;

    friend class KsValue;
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
    KsSingleValue &operator = (float);

protected:
    virtual bool xdrEncodeVariant(XDR *) const;
    virtual bool xdrDecodeVariant(XDR *);

private:
    float val;

    friend class KsValue;
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
    KsDoubleValue & operator = (double);

protected:
    virtual bool xdrEncodeVariant(XDR *) const;
    virtual bool xdrDecodeVariant(XDR *);

private:
    double val;

    friend class KsValue;
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

    KsStringValue &operator = (const PltString &);

protected:
    virtual bool xdrEncodeVariant(XDR *) const;
    virtual bool xdrDecodeVariant(XDR *);

private:
    friend class KsValue;
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

    KsTimeValue &operator = (const PltTime &);

protected:
    bool xdrEncodeVariant(XDR *) const;
    bool xdrDecodeVariant(XDR *);

private:
    friend class KsValue;
    KsTimeValue(XDR *, bool &);

    PLT_DECL_RTTI;

#if PLT_DEBUG
public:
    virtual void debugPrint(ostream & ostr) const;
#endif
};

/////////////////////////////////////////////////////////////////////////////
// class KsTimeSpanValue
/////////////////////////////////////////////////////////////////////////////

class KsTimeSpanValue
    : public KsTimeSpan,
      public KsValue
{
public:
    KsTimeSpanValue(long sec = 0, long usec = 0);
    KsTimeSpanValue(const KsTimeSpan &ts);

    // redefinitions for resolving multiply function definitions 
    // in base classes
    virtual bool xdrEncode(XDR *) const;
    virtual bool xdrDecode(XDR *);

    virtual enum_t xdrTypeCode() const { return KS_VT_TIME_SPAN; }

protected:
    bool xdrEncodeVariant(XDR *) const;
    bool xdrDecodeVariant(XDR *);

private:
    friend class KsValue;
    KsTimeSpanValue(XDR *, bool &);

    PLT_DECL_RTTI;
    
#if PLT_DEBUG
public:
    virtual void debugPrint(ostream & ostr) const;
#endif
};

/////////////////////////////////////////////////////////////////////////////
// class KsStateValue
/////////////////////////////////////////////////////////////////////////////

class KsStateValue
    : public KsValue 
{
public:
    KsStateValue(KS_STATE state = KS_ST_NOTSUPPORTED)
        : val(state) {}

    void getState(KS_STATE &state) const { state = val; }
    void setState(KS_STATE state) { val = state; };

    operator KS_STATE () const { return val; }
    KsStateValue & operator = (KS_STATE state) { val = state; return *this; }

    virtual enum_t xdrTypeCode() const { return KS_VT_STATE; }

protected:
    virtual bool xdrEncodeVariant(XDR *xdr) const;
    virtual bool xdrDecodeVariant(XDR *);

private:
    KS_STATE val;

    friend class KsValue;
    KsStateValue(XDR *, bool &);
    
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
    friend class KsValue;
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
    friend class KsValue;
    KsByteVecValue(XDR *, bool &);

    PLT_DECL_RTTI;

};

//////////////////////////////////////////////////////////////////////
// class KsBoolVecValue
//////////////////////////////////////////////////////////////////////

class KsBoolVecValue 
: public KsVecValueBase<bool>
{
public:
    KsBoolVecValue(size_t size = 0);
    KsBoolVecValue(size_t size, bool *p, PltOwnership os);

    enum_t xdrTypeCode() const;

private:
    friend class KsValue;
    KsBoolVecValue(XDR *, bool &);

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
    friend class KsValue;
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
    friend class KsValue;
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
    friend class KsValue;
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
    friend class KsValue;
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
    friend class KsValue;
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
    friend class KsValue;
    KsTimeVecValue(XDR *, bool &);

    PLT_DECL_RTTI;
};

//////////////////////////////////////////////////////////////////////
// class KsTimeSpanVecValue
//////////////////////////////////////////////////////////////////////

class KsTimeSpanVecValue 
: public KsVecValueBase<KsTimeSpan>
{
public:
    KsTimeSpanVecValue(size_t size = 0);
    KsTimeSpanVecValue(size_t size, KsTimeSpan *p, PltOwnership os);

    enum_t xdrTypeCode() const;

private:
    friend class KsValue;
    KsTimeSpanVecValue(XDR *, bool &);

    PLT_DECL_RTTI;
};

//////////////////////////////////////////////////////////////////////
// class KsStateVecValue
//////////////////////////////////////////////////////////////////////

class KsStateVecValue 
: public KsVecValueBase<KS_STATE>
{
public:
    KsStateVecValue(size_t size = 0);
    KsStateVecValue(size_t size, KS_STATE *p, PltOwnership os);

    enum_t xdrTypeCode() const { return KS_VT_STATE_VEC; }

private:
    friend class KsValue;
    KsStateVecValue(XDR *, bool &);

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
//////////////////////////////////////////////////////////////////////

inline
KsBoolValue::KsBoolValue(bool b) 
: val(b)
{
}

//////////////////////////////////////////////////////////////////////

inline
KsBoolValue::KsBoolValue(XDR *xdr, bool &ok)
{
    ok = xdrDecodeVariant(xdr);
}

//////////////////////////////////////////////////////////////////////

inline 
KsBoolValue::operator bool () const
{
    return val;
}

//////////////////////////////////////////////////////////////////////

inline KsBoolValue &
KsBoolValue::operator = (bool b)
{
    val = b;
    return *this;
}

//////////////////////////////////////////////////////////////////////
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

inline
KsUIntValue &
KsUIntValue::operator = (unsigned long v)
{
    val = v;
    return *this;
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

inline
KsSingleValue &
KsSingleValue::operator = (float v)
{
    val = v;
    return *this;
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

inline
KsDoubleValue &
KsDoubleValue::operator = (double v)
{
    val = v;
    return *this;
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
{
    tv_sec = time.tv_sec;
    tv_usec = time.tv_usec;
}

//////////////////////////////////////////////////////////////////////

inline
KsTimeValue::KsTimeValue(XDR *xdr, bool &ok)
: KsTime(xdr, ok)
{}

//////////////////////////////////////////////////////////////////////

inline
KsTimeValue &
KsTimeValue::operator = (const PltTime &t)
{
    tv_sec = t.tv_sec;
    tv_usec = t.tv_usec;

    return *this;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

inline
KsTimeSpanValue::KsTimeSpanValue(long sec, long usec)
    : KsTimeSpan(sec, usec)
{}

/////////////////////////////////////////////////////////////////////////////

inline
KsTimeSpanValue::KsTimeSpanValue(const KsTimeSpan &ts)
    : KsTimeSpan(ts)
{}

/////////////////////////////////////////////////////////////////////////////

inline
KsTimeSpanValue::KsTimeSpanValue(XDR *xdr, bool &ok)
    : KsTimeSpan(xdr, ok)
{}

/////////////////////////////////////////////////////////////////////////////

inline
bool
KsTimeSpanValue::xdrEncode(XDR *xdr) const
{
    return KsValue::xdrEncode(xdr);
}

/////////////////////////////////////////////////////////////////////////////

inline
bool 
KsTimeSpanValue::xdrDecode(XDR *xdr)
{
    return KsValue::xdrDecode(xdr);
}

/////////////////////////////////////////////////////////////////////////////

inline
bool
KsTimeSpanValue::xdrEncodeVariant(XDR *xdr) const
{
    return KsTimeSpan::xdrEncode(xdr);
}

/////////////////////////////////////////////////////////////////////////////

inline
bool 
KsTimeSpanValue::xdrDecodeVariant(XDR *xdr)
{
    return KsTimeSpan::xdrDecode(xdr);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

inline
bool 
KsStateValue::xdrEncodeVariant(XDR *xdr) const
{
    return ks_xdre_enum(xdr, &val);
}

/////////////////////////////////////////////////////////////////////////////

inline
bool 
KsStateValue::xdrDecodeVariant(XDR *xdr)
{
    return ks_xdrd_enum(xdr, &val);
}

/////////////////////////////////////////////////////////////////////////////

inline
KsStateValue::KsStateValue(XDR *xdr, bool &ok)
{
    ok = xdrDecodeVariant(xdr);
}

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
KsBoolVecValue::KsBoolVecValue(size_t size)
: KsVecValueBase<bool>(size)
{}

//////////////////////////////////////////////////////////////////////

inline
KsBoolVecValue::KsBoolVecValue(size_t size, bool *p, PltOwnership os)
: KsVecValueBase<bool>(size, p, os)
{}

//////////////////////////////////////////////////////////////////////

inline
KsBoolVecValue::KsBoolVecValue(XDR *xdr, bool &ok)
{
    ok = xdrDecodeVariant(xdr);
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

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

inline
KsTimeSpanVecValue::KsTimeSpanVecValue(size_t size)
    : KsVecValueBase<KsTimeSpan>(size)
{}

/////////////////////////////////////////////////////////////////////////////

inline
KsTimeSpanVecValue::KsTimeSpanVecValue(
    size_t size, KsTimeSpan *p, PltOwnership os
)
    : KsVecValueBase<KsTimeSpan>(size, p, os)
{}

/////////////////////////////////////////////////////////////////////////////

inline
KsTimeSpanVecValue::KsTimeSpanVecValue(XDR *xdr, bool &ok)
{
    ok = xdrDecodeVariant(xdr);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

inline
KsStateVecValue::KsStateVecValue(size_t size)
    : KsVecValueBase<KS_STATE>(size)
{}

/////////////////////////////////////////////////////////////////////////////

inline
KsStateVecValue::KsStateVecValue(size_t size, KS_STATE *p, PltOwnership os)
    : KsVecValueBase<KS_STATE>(size, p, os)
{}

/////////////////////////////////////////////////////////////////////////////

inline
KsStateVecValue::KsStateVecValue(XDR *xdr, bool &ok)
{
    ok = xdrDecodeVariant(xdr);
}

//////////////////////////////////////////////////////////////////////

#if PLT_SEE_ALL_TEMPLATES
#include "ks/value_impl.h"
#endif

//////////////////////////////////////////////////////////////////////

#endif





