/* -*-plt-c++-*- */
// File: ks/string.cpp

/*
 * Copyright (c) 1996, 1997
 * Chair of Process Control Engineering,
 * Aachen University of Technology.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must print or display the above
 *    copyright notice either during startup or must have a means for
 *    the user to view the copyright notice.
 * 3. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 4. Neither the name of the Chair of Process Control Engineering nor the
 *    name of the Aachen University of Technology may be used to endorse or
 *    promote products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE CHAIR OF PROCESS CONTROL ENGINEERING
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE CHAIR OF PROCESS CONTROL
 * ENGINEERING BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/* Author: Martin Kneissl <martin@plt.rwth-aachen.de> */


//////////////////////////////////////////////////////////////////////

#include "ks/string.h"
#include <string.h>

//////////////////////////////////////////////////////////////////////

PLT_IMPL_RTTI1(KsString, PltString);

//////////////////////////////////////////////////////////////////////

KsString::KsString(XDR *xdr, bool &success)
{
    PLT_PRECONDITION(xdr->x_op == XDR_DECODE);

    // read length
    //
    u_long sz;
    if (xdr_u_long(xdr, &sz) ) {

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
    return xdr_u_long(xdr, &sz) && xdr_opaque(xdr, p->s, sz);
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
