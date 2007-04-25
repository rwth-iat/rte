/* -*-plt-c++-*- */
// File: ks/string.cpp

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
/* Author: Martin Kneissl <martin@plt.rwth-aachen.de> */


//////////////////////////////////////////////////////////////////////

#include "ks/string.h"
#include <string.h>

//////////////////////////////////////////////////////////////////////

// PLT_IMPL_RTTI1(KsString, PltString);

//////////////////////////////////////////////////////////////////////

KsString::KsString(XDR *xdr, bool &success)
{
    PLT_PRECONDITION(xdr->x_op == XDR_DECODE);

    // read length
    //
    u_long sz;
    if (ks_xdrd_u_long(xdr, &sz) ) {

        // allocate memory
        //
        char * n = new char[sz+1];
        if (! n) goto fail;
        
        // read characters
        //
        if (! xdr_opaque(xdr, n, sz) ) {
            delete[] n;
            goto fail;
        }

        // success: 
        // assign to *this and return
        //
        *this = KsString(sz, n);
        success = ok();
        return;
    }
 fail:
    success = false;
    return;
}

//////////////////////////////////////////////////////////////////////

bool
KsString::xdrDecode(XDR *xdr)
{
    PLT_PRECONDITION(xdr->x_op == XDR_DECODE);
    bool success;
    KsString n(xdr, success);
    if (success) {
        *this = n;
    } 
    return success;
}
    
//////////////////////////////////////////////////////////////////////

bool
KsString::xdrEncode(XDR *xdr) const
{
    PLT_PRECONDITION(xdr->x_op == XDR_ENCODE && ok());
    u_long sz = len();
    return ks_xdre_u_long(xdr, &sz) 
        && xdr_opaque(xdr, p->s, sz);
}

//////////////////////////////////////////////////////////////////////

KsString *
KsString::xdrNew(XDR *xdr)
{
    PLT_PRECONDITION(xdr->x_op == XDR_DECODE);
    bool ok;                                         
    KsString * p = new KsString(xdr, ok);     
    if ( !ok && p) {                             
        delete p;                                
        p = 0;                                   
    }                                            
    return p; 
}                                   

//////////////////////////////////////////////////////////////////////
// EOF ks/string.cpp
//////////////////////////////////////////////////////////////////////
