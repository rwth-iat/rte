/* -*-plt-c++-*- */
/* 
 *  Copyright (c) 1996 PLT, RWTH-Aachen, Germany. See file copy_plt.txt!
 *  Author: Martin Kneissl <martin@plt.rwth-aachen.de>
 *
 *  03/06/97 Interfaces changed, xdrEncode/xdrDecode implemented
 */

#include "ks/value.h"
#if PLT_DEBUG && (PLT_COMPILER_MSVC || PLT_COMPILER_BORLAND)
#include <time.h>
#endif

//////////////////////////////////////////////////////////////////////
// RTTI Simulation
//////////////////////////////////////////////////////////////////////

PLT_IMPL_RTTI1(KsValue,KsXdrUnion);
PLT_IMPL_RTTI1(KsBoolValue,KsValue);
PLT_IMPL_RTTI1(KsIntValue,KsValue);
PLT_IMPL_RTTI1(KsUIntValue,KsValue);
PLT_IMPL_RTTI1(KsSingleValue,KsValue);
PLT_IMPL_RTTI1(KsDoubleValue,KsValue);
PLT_IMPL_RTTI2(KsStringValue,KsValue,KsString);
PLT_IMPL_RTTI2(KsTimeValue,KsValue,KsTime);
PLT_IMPL_RTTI2(KsTimeSpanValue,KsValue,KsTimeSpan);
PLT_IMPL_RTTI1(KsStateValue,KsValue);
PLT_IMPL_RTTI1(KsVoidValue,KsValue);

// RTTI skips KsVecValueBase!!!

PLT_IMPL_RTTI1(KsByteVecValue, KsValue);
PLT_IMPL_RTTI1(KsBoolVecValue, KsValue);
PLT_IMPL_RTTI1(KsIntVecValue, KsValue);
PLT_IMPL_RTTI1(KsUIntVecValue, KsValue);
PLT_IMPL_RTTI1(KsSingleVecValue, KsValue);
PLT_IMPL_RTTI1(KsDoubleVecValue, KsValue);
PLT_IMPL_RTTI1(KsStringVecValue, KsValue);
PLT_IMPL_RTTI1(KsTimeVecValue, KsValue);
PLT_IMPL_RTTI1(KsTimeSpanVecValue, KsValue);
PLT_IMPL_RTTI1(KsStateVecValue, KsValue);

//////////////////////////////////////////////////////////////////////

KS_BEGIN_IMPL_XDRUNION(KsValue);
KS_XDR_MAP(KS_VT_BOOL,KsBoolValue);
KS_XDR_MAP(KS_VT_INT,KsIntValue);
KS_XDR_MAP(KS_VT_UINT,KsUIntValue);
KS_XDR_MAP(KS_VT_SINGLE,KsSingleValue);
KS_XDR_MAP(KS_VT_DOUBLE,KsDoubleValue);
KS_XDR_MAP(KS_VT_STRING,KsStringValue);
KS_XDR_MAP(KS_VT_TIME,KsTimeValue);
KS_XDR_MAP(KS_VT_TIME_SPAN,KsTimeSpanValue);
KS_XDR_MAP(KS_VT_STATE,KsStateValue);
KS_XDR_MAP(KS_VT_VOID,KsVoidValue);
KS_XDR_MAP(KS_VT_BYTE_VEC, KsByteVecValue);
KS_XDR_MAP(KS_VT_BOOL_VEC, KsBoolVecValue);
KS_XDR_MAP(KS_VT_INT_VEC, KsIntVecValue);
KS_XDR_MAP(KS_VT_UINT_VEC, KsUIntVecValue);
KS_XDR_MAP(KS_VT_SINGLE_VEC, KsSingleVecValue);
KS_XDR_MAP(KS_VT_DOUBLE_VEC, KsDoubleVecValue);
KS_XDR_MAP(KS_VT_STRING_VEC, KsStringVecValue);
KS_XDR_MAP(KS_VT_TIME_VEC, KsTimeVecValue);
KS_XDR_MAP(KS_VT_TIME_SPAN_VEC, KsTimeSpanVecValue);
KS_XDR_MAP(KS_VT_STATE_VEC, KsStateVecValue);
KS_END_IMPL_XDRUNION;

//////////////////////////////////////////////////////////////////////
// Instantiate member functions (necessary)
//////////////////////////////////////////////////////////////////////

#include "ks/value_impl.h"

#if PLT_INSTANTIATE_TEMPLATES
template class KsVecValueBase<char>;
template class KsVecValueBase<bool>;
template class KsVecValueBase<long>;
template class KsVecValueBase<u_long>;
template class KsVecValueBase<float>;
template class KsVecValueBase<double>;
template class KsVecValueBase<KsTime>;
template class KsVecValueBase<KsTimeSpan>;
template class KsVecValueBase<KsString>;
template class KsVecValueBase<int>;
#endif

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
#if PLT_DEBUG

void 
KsValue::debugPrint(ostream & ostr) const
{
    ostr << "(KsValue){...}";
}

#endif
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

bool
KsBoolValue::xdrEncodeVariant(XDR *xdr) const 
{
    return ks_xdre_bool(xdr,&val);
}

//////////////////////////////////////////////////////////////////////

bool
KsBoolValue::xdrDecodeVariant(XDR *xdr)
{
    return ks_xdrd_bool(xdr,&val);
}

//////////////////////////////////////////////////////////////////////
#if PLT_DEBUG
void
KsBoolValue::debugPrint(ostream & ostr) const
{
    ostr << "KsBoolValue{" << (val ? "true" : "false") << "}";
}
#endif

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

bool
KsIntValue::xdrEncodeVariant(XDR *xdr) const 
{
    return ks_xdre_long(xdr,&val);
}

//////////////////////////////////////////////////////////////////////

bool
KsIntValue::xdrDecodeVariant(XDR *xdr)
{
    return ks_xdrd_long(xdr,&val);
}

//////////////////////////////////////////////////////////////////////
#if PLT_DEBUG
void
KsIntValue::debugPrint(ostream & ostr) const
{
    ostr << "KsIntValue{" << val << "}";
}
#endif
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

bool
KsUIntValue::xdrEncodeVariant(XDR *xdr) const
{
    return ks_xdre_u_long(xdr,&val);
}

//////////////////////////////////////////////////////////////////////

bool
KsUIntValue::xdrDecodeVariant(XDR *xdr) 
{
    return ks_xdrd_u_long(xdr,&val);
}

//////////////////////////////////////////////////////////////////////
#if PLT_DEBUG
void
KsUIntValue::debugPrint(ostream & ostr) const
{
    ostr << "KsUIntValue{" << val << "}";
}
#endif
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

bool
KsSingleValue::xdrEncodeVariant(XDR *xdr) const
{
    return ks_xdre_float(xdr,&val);
}

//////////////////////////////////////////////////////////////////////

bool
KsSingleValue::xdrDecodeVariant(XDR *xdr)
{
    return ks_xdrd_float(xdr,&val);
}


//////////////////////////////////////////////////////////////////////
#if PLT_DEBUG
void
KsSingleValue::debugPrint(ostream & ostr) const
{
    ostr << "KsSingleValue{" << val << "}";
}
#endif
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

bool
KsDoubleValue::xdrEncodeVariant(XDR *xdr) const 
{
    return ks_xdre_double(xdr,&val);
}

//////////////////////////////////////////////////////////////////////

bool
KsDoubleValue::xdrDecodeVariant(XDR *xdr) 
{
    return ks_xdrd_double(xdr,&val);
}

//////////////////////////////////////////////////////////////////////
#if PLT_DEBUG
void
KsDoubleValue::debugPrint(ostream & ostr) const
{
    ostr << "KsDoubleValue{" << val << "}";
}
#endif
//////////////////////////////////////////////////////////////////////
// class KsStringValue
//////////////////////////////////////////////////////////////////////

bool
KsStringValue::xdrEncode(XDR *xdr) const
{
    return KsValue::xdrEncode(xdr);
}

//////////////////////////////////////////////////////////////////////

bool
KsStringValue::xdrDecode(XDR *xdr)
{
    return KsValue::xdrDecode(xdr);
}

//////////////////////////////////////////////////////////////////////

enum_t
KsStringValue::xdrTypeCode() const
{
    return KS_VT_STRING;
}

//////////////////////////////////////////////////////////////////////

bool
KsStringValue::xdrEncodeVariant(XDR *xdr) const
{
    return KsString::xdrEncode(xdr);
}

//////////////////////////////////////////////////////////////////////

bool
KsStringValue::xdrDecodeVariant(XDR *xdr)
{
    return KsString::xdrDecode(xdr);
}

//////////////////////////////////////////////////////////////////////

KsStringValue &
KsStringValue::operator = (const PltString &newstr)
{
    KsString::operator = (newstr);
    return *this;
}

//////////////////////////////////////////////////////////////////////
#if PLT_DEBUG
void
KsStringValue::debugPrint(ostream & ostr) const
{
    ostr << "KsStringValue{" << (const char *)*this << "}";
}
#endif
//////////////////////////////////////////////////////////////////////
// class KsTimeValue
//////////////////////////////////////////////////////////////////////

bool
KsTimeValue::xdrEncode(XDR *xdr) const
{
    return KsValue::xdrEncode(xdr);
}

//////////////////////////////////////////////////////////////////////

bool
KsTimeValue::xdrDecode(XDR *xdr)
{
    return KsValue::xdrDecode(xdr);
}

//////////////////////////////////////////////////////////////////////

enum_t
KsTimeValue::xdrTypeCode() const
{
    return KS_VT_TIME;
}

//////////////////////////////////////////////////////////////////////

bool
KsTimeValue::xdrEncodeVariant(XDR *xdr) const
{
    return KsTime::xdrEncode(xdr);
}

//////////////////////////////////////////////////////////////////////

bool
KsTimeValue::xdrDecodeVariant(XDR *xdr)
{
    return KsTime::xdrDecode(xdr);
}

//////////////////////////////////////////////////////////////////////
#if PLT_DEBUG
void
KsTimeValue::debugPrint(ostream & ostr) const
{
#if PLT_SYSTEM_OPENVMS
    unsigned long sec = tv_sec;
#elif PLT_SYSTEM_HPUX
    int sec = tv_sec;
#else
    long sec = tv_sec;
# endif
    ostr << "KsTimeValue{" << tv_sec << "," << tv_usec 
         << " = " << ctime(&sec) << "}";
}
#endif

/////////////////////////////////////////////////////////////////////////////
// class KsTimeSpanValue
/////////////////////////////////////////////////////////////////////////////

#if PLT_DEBUG
void
KsTimeSpanValue::debugPrint(ostream &os) const
{
    os << "KsTimeSpanValue{" << tv_sec << "," << tv_usec << "}";
}
#endif

/////////////////////////////////////////////////////////////////////////////
// class KsStateValue
/////////////////////////////////////////////////////////////////////////////
    
#if PLT_DEBUG
void 
KsStateValue::debugPrint(ostream & os) const
{
    static char states[][13] = {
        "NOTSUPPORTED",
        "UNKNOWN",
        "BAD",
        "QUESTIONABLE",
        "GOOD"
    };

    os << "KsStateValue{";
    if( 0 <= val && val <= 4 ) {
        os << states[val];
    } else {
        os << val;
    }
    os << "}";
}
 
#endif


//////////////////////////////////////////////////////////////////////
// class KsVoidValue
//////////////////////////////////////////////////////////////////////

enum_t
KsVoidValue::xdrTypeCode() const
{
    return KS_VT_VOID;
}

//////////////////////////////////////////////////////////////////////

bool
KsVoidValue::xdrEncodeVariant(XDR *) const
{
    return true;
}

//////////////////////////////////////////////////////////////////////

bool
KsVoidValue::xdrDecodeVariant(XDR *)
{
    return true;
}

//////////////////////////////////////////////////////////////////////
#if PLT_DEBUG
void
KsVoidValue::debugPrint(ostream & ostr) const
{
    ostr << "KsVoidValue{}";
}
#endif
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

enum_t
KsByteVecValue::xdrTypeCode() const
{
    return KS_VT_BYTE_VEC;
}

//////////////////////////////////////////////////////////////////////

enum_t
KsBoolVecValue::xdrTypeCode() const
{
    return KS_VT_BOOL_VEC;
}

//////////////////////////////////////////////////////////////////////

enum_t
KsIntVecValue::xdrTypeCode() const
{
    return KS_VT_INT_VEC;
}

//////////////////////////////////////////////////////////////////////

enum_t
KsUIntVecValue::xdrTypeCode() const
{
    return KS_VT_UINT_VEC;
}

//////////////////////////////////////////////////////////////////////

enum_t
KsSingleVecValue::xdrTypeCode() const
{
    return KS_VT_SINGLE_VEC;
}

//////////////////////////////////////////////////////////////////////

enum_t
KsDoubleVecValue::xdrTypeCode() const
{
    return KS_VT_DOUBLE_VEC;
}

//////////////////////////////////////////////////////////////////////

#if PLT_DEBUG
void
KsDoubleVecValue::debugPrint(ostream & ostr) const
{
    ostr << "KsDoubleVecValue{size=";
    ostr << size() << ", values={;";
    for (size_t s = 0; s<size(); ++s) {
        ostr << (*this)[s] << endl;
    }
    ostr << "}}";
}
#endif

//////////////////////////////////////////////////////////////////////

enum_t
KsStringVecValue::xdrTypeCode() const
{
    return KS_VT_STRING_VEC;
}

//////////////////////////////////////////////////////////////////////

enum_t
KsTimeVecValue::xdrTypeCode() const
{
    return KS_VT_TIME_VEC;
}

//////////////////////////////////////////////////////////////////////

enum_t
KsTimeSpanVecValue::xdrTypeCode() const
{
    return KS_VT_TIME_SPAN_VEC;
}

/////////////////////////////////////////////////////////////////////////////
// EOF value.cpp
/////////////////////////////////////////////////////////////////////////////


