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

#if PLT_DEBUG

#include <iostream.h>


//////////////////////////////////////////////////////////////////////

void
KsIntValue::printOn(ostream & ostr) const 
{
    ostr << val;
}

//////////////////////////////////////////////////////////////////////

void
KsDoubleValue::printOn(ostream & ostr) const
{
    ostr << val;
}

#endif // PLT_DEBUG
