/* -*-plt-c++-*- */
/* 
 *  Copyright (c) 1996 PLT, RWTH-Aachen, Germany. See file copy_plt.txt!
 *  Author: Martin Kneissl <martin@plt.rwth-aachen.de>
 *
 */

#include <plt/debug.h>
#include <ks/value.hpp>

//////////////////////////////////////////////////////////////////////

KS_XDR_IF_BEGIN(KsValue)
    KS_XDR_IF_MAP(KS_VT_INT,       KsIntValue);
    KS_XDR_IF_MAP(KS_VT_DOUBLE,    KsDoubleValue);
KS_XDR_IF_END;


//////////////////////////////////////////////////////////////////////
// RTTI Simulation
//////////////////////////////////////////////////////////////////////

PLT_IMPL_RTTI0(KsValue);
PLT_IMPL_RTTI1(KsIntValue,KsValue);
PLT_IMPL_RTTI1(KsUIntValue,KsValue);
PLT_IMPL_RTTI1(KsSingleValue,KsValue);
PLT_IMPL_RTTI1(KsDoubleValue,KsValue);

//////////////////////////////////////////////////////////////////////

bool_t
KsIntValue::xdr(XDR *xdr) {
    return xdr_long(xdr, &val);
}

//////////////////////////////////////////////////////////////////////

bool_t
KsUIntValue::xdr(XDR *xdr) {
    return xdr_long(xdr, &val);
}

//////////////////////////////////////////////////////////////////////

bool_t
KsSingleValue::xdr(XDR *xdr) {
    return xdr_float(xdr, &val);
}

//////////////////////////////////////////////////////////////////////

bool_t
KsDoubleValue::xdr(XDR *xdr) {
    return xdr_double(xdr, &val);
}

//////////////////////////////////////////////////////////////////////


