/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/include/ks/list_impl.h,v 1.2 1997-03-18 10:48:36 martin Exp $ */
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

#include "ks/list.h"
#include "plt/list_impl.h"

//////////////////////////////////////////////////////////////////////

template <class T>
bool
KsList<T>::xdrEncode(XDR * xdr) const
{
    PLT_PRECONDITION(xdr->x_op == XDR_ENCODE);
    // write size
    u_long sz = size();
    if (! ks_xdre_u_long(xdr, &sz) ) return false;

    // write elements
    for (PltListIterator<T> it(*this); it; ++it) {
        if (! it->xdrEncode(xdr) ) {
            // serialization of element failed
            return false;
        }
    }
    return true;
}

//////////////////////////////////////////////////////////////////////

template <class T>
bool
KsList<T>::xdrDecode(XDR * xdr)
{
    PLT_PRECONDITION(xdr->x_op == XDR_DECODE);
    u_long count;
    if ( ! ks_xdrd_u_long(xdr, &count) ) return false;
    
    // flush old elements
    while (! isEmpty() ) {
        removeFirst();
    }

    for ( u_long i=0; i < count; ++i) {
        bool ok=false;
        T elem(xdr, ok);
        if (! ok) return false;
        if (! addLast(elem) ) return false;
    } 
    return true;
}

//////////////////////////////////////////////////////////////////////

template <class T>
KsList<T> *
KsList<T>::xdrNew(XDR * xdr)
{
    bool ok=false;
    KsList<T> * p = new KsList<T>(xdr, ok);
    if ( !ok && p) {                                   
        delete p;                                  
        p = 0;                                     
    }                                              
    return p;                                      
}

//////////////////////////////////////////////////////////////////////
// EOF ks/list_impl.h
