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

//////////////////////////////////////////////////////////////////////

KS_IMPL_XDRCTOR(KsIntValue);
KS_IMPL_XDRNEW(KsIntValue);

bool
KsIntValue::xdrEncode(XDR *xdr) const {
    PLT_PRECONDITION(xdr->x_op == XDR_ENCODE);
    return xdr_long(xdr,&val);
}

bool
KsIntValue::xdrDecode(XDR *xdr) {
    PLT_PRECONDITION(xdr->x_op == XDR_DECODE);
    return xdr_long(xdr,&val);
}

//////////////////////////////////////////////////////////////////////

KS_IMPL_XDRCTOR(KsUIntValue);
KS_IMPL_XDRNEW(KsUIntValue);

bool
KsUIntValue::xdrEncode(XDR *xdr) const {
    PLT_PRECONDITION(xdr->x_op == XDR_ENCODE);
    return xdr_u_long(xdr,&val);
}

bool
KsUIntValue::xdrDecode(XDR *xdr) {
    PLT_PRECONDITION(xdr->x_op == XDR_DECODE);
    return xdr_u_long(xdr,&val);
}

//////////////////////////////////////////////////////////////////////

KS_IMPL_XDRCTOR(KsSingleValue);
KS_IMPL_XDRNEW(KsSingleValue);

bool
KsSingleValue::xdrEncode(XDR *xdr) const {
    PLT_PRECONDITION(xdr->x_op == XDR_ENCODE);
    return xdr_float(xdr,&val);
}

bool
KsSingleValue::xdrDecode(XDR *xdr) {
    PLT_PRECONDITION(xdr->x_op == XDR_DECODE);
    return xdr_float(xdr,&val);
}


//////////////////////////////////////////////////////////////////////

KS_IMPL_XDRCTOR(KsDoubleValue);
KS_IMPL_XDRNEW(KsDoubleValue);

bool
KsDoubleValue::xdrEncode(XDR *xdr) const {
    PLT_PRECONDITION(xdr->x_op == XDR_ENCODE);
    return xdr_double(xdr,&val);
}

bool
KsDoubleValue::xdrDecode(XDR *xdr) {
    PLT_PRECONDITION(xdr->x_op == XDR_DECODE);
    return xdr_double(xdr,&val);
}

//////////////////////////////////////////////////////////////////////


