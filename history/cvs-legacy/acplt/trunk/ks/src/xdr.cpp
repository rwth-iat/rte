/* -*-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/src/xdr.cpp,v 1.5 2003-10-13 12:07:30 harald Exp $ */
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
 *
 *  Author: Martin Kneissl <martin@plt.rwth-aachen.de>
 */

#include "ks/xdr.h"

//////////////////////////////////////////////////////////////////////

PLT_IMPL_RTTI0(KsXdrAble);
PLT_IMPL_RTTI1(KsXdrUnion, KsXdrAble);

//////////////////////////////////////////////////////////////////////

bool
KsXdrUnion::xdrEncode(XDR *xdrs) const
{
    PLT_PRECONDITION(xdrs->x_op == XDR_ENCODE);                
    enum_t typecode = xdrTypeCode();                       
    return 
        ks_xdre_enum(xdrs, &typecode) 
            && xdrEncodeVariant(xdrs)
                && xdrEncodeCommon(xdrs);
}                                                              
                                                                   
//////////////////////////////////////////////////////////////////////
                                                                   
bool KsXdrUnion::xdrDecode(XDR * xdrs)                               
{                                                              
    PLT_PRECONDITION(xdrs->x_op == XDR_DECODE);               
    enum_t typecode;                                       
    return 
        ks_xdrd_enum(xdrs, &typecode) 
            && typecode == xdrTypeCode()
                && xdrDecodeVariant(xdrs)
                    && xdrDecodeCommon(xdrs);
}

//////////////////////////////////////////////////////////////////////
