/* -*-plt-c++-*- */
/* 
 *  Copyright (c) 1996 PLT, RWTH-Aachen, Germany. See file copy_plt.txt!
 *  Author: Martin Kneissl <martin@plt.rwth-aachen.de>
 *
 *  03/06/97 Interfaces changed, xdrEncode/xdrDecode implemented
 */

#include <plt/debug.h>
#include <ks/value.h>


//////////////////////////////////////////////////////////////////////
// RTTI Simulation
//////////////////////////////////////////////////////////////////////

PLT_IMPL_RTTI0(KsValue);
PLT_IMPL_RTTI1(KsIntValue,KsValue);
PLT_IMPL_RTTI1(KsUIntValue,KsValue);
PLT_IMPL_RTTI1(KsSingleValue,KsValue);
PLT_IMPL_RTTI1(KsDoubleValue,KsValue);
// PLT_IMPL_RTTI2(KsStringValue,KsValue,KsString);
// PLT_IMPL_RTTI2(KsTimeValue,KsValue,KsTime);

//////////////////////////////////////////////////////////////////////

KS_BEGIN_IMPL_XDRUNION(KsValue);
KS_XDR_MAP(KS_VT_INT,KsIntValue);
KS_XDR_MAP(KS_VT_UINT,KsUIntValue);
KS_XDR_MAP(KS_VT_SINGLE,KsSingleValue);
KS_XDR_MAP(KS_VT_DOUBLE,KsDoubleValue);
KS_XDR_MAP(KS_VT_STRING,KsStringValue);
KS_XDR_MAP(KS_VT_TIME,KsTimeValue);
KS_END_IMPL_XDRUNION;

// KS_IMPL_XDRCTOR(KsIntValue);
// KS_IMPL_XDRNEW(KsIntValue);

bool
KsIntValue::xdrEncodeVariant(XDR *xdr) const 
{
    return xdr_long(xdr,&val);
}

bool
KsIntValue::xdrDecodeVariant(XDR *xdr)
{
    return xdr_long(xdr,&val);
}

//////////////////////////////////////////////////////////////////////

// KS_IMPL_XDRCTOR(KsUIntValue);
// KS_IMPL_XDRNEW(KsUIntValue);

bool
KsUIntValue::xdrEncodeVariant(XDR *xdr) const
{
    return xdr_u_long(xdr,&val);
}

bool
KsUIntValue::xdrDecodeVariant(XDR *xdr) 
{
    return xdr_u_long(xdr,&val);
}

//////////////////////////////////////////////////////////////////////

// KS_IMPL_XDRCTOR(KsSingleValue);
// KS_IMPL_XDRNEW(KsSingleValue);

bool
KsSingleValue::xdrEncodeVariant(XDR *xdr) const
{
    return xdr_float(xdr,&val);
}

bool
KsSingleValue::xdrDecodeVariant(XDR *xdr)
{
    return xdr_float(xdr,&val);
}


//////////////////////////////////////////////////////////////////////

// KS_IMPL_XDRCTOR(KsDoubleValue);
// KS_IMPL_XDRNEW(KsDoubleValue);

bool
KsDoubleValue::xdrEncodeVariant(XDR *xdr) const 
{
    return xdr_double(xdr,&val);
}

bool
KsDoubleValue::xdrDecodeVariant(XDR *xdr) 
{
    return xdr_double(xdr,&val);
}

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





