/* -*-plt-c++-*- */
/* 
 *  Copyright (c) 1996 PLT, RWTH-Aachen, Germany. See file copy_plt.txt!
 *  Author: Martin Kneissl <martin@plt.rwth-aachen.de>
 *
 */

#include "ks/xdr.h"

//////////////////////////////////////////////////////////////////////

bool
KsXdrUnion::xdrEncode(XDR *xdrs) const
{
    PLT_PRECONDITION(xdrs->x_op == XDR_ENCODE);                
    enum_t typecode = xdrTypeCode();                       
    return 
        xdr_enum(xdrs, &typecode) 
            && xdrEncodeVariant(xdrs)
                && xdrEncodeCommon(xdrs);
}                                                              
                                                                   
//////////////////////////////////////////////////////////////////////
                                                                   
bool KsXdrUnion::xdrDecode(XDR * xdrs)                               
{                                                              
    PLT_PRECONDITION(xdrs->x_op == XDR_DECODE);               
    enum_t typecode;                                       
    return 
        xdr_enum(xdrs, &typecode) 
            && typecode == xdrTypeCode()
                && xdrDecodeVariant(xdrs)
                    && xdrDecodeCommon(xdrs);
}

//////////////////////////////////////////////////////////////////////
