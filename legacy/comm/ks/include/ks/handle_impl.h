/* -*-plt-c++-*- */
#ifndef KS_HANDLE_IMPL_INCLUDED
#define KS_HANDLE_IMPL_INCLUDED
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
// ks/handle.h provides memory managing pointer replacements with xdr
//////////////////////////////////////////////////////////////////////

#if !PLT_SEE_ALL_TEMPLATES
#include "ks/handle.h"
#include "plt/handle_impl.h"
#endif

//////////////////////////////////////////////////////////////////////

#include "ks/xdr.h"

//////////////////////////////////////////////////////////////////////

template <class T>
bool
KsPtrHandle<T>::xdrDecode(XDR *xdr)
{
    T * p = T::xdrNew(xdr);
    if (!p) return false;
    if (!this->bindTo(p, KsOsNew) ) {
        delete p;
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////

template <class T>
KsPtrHandle<T>::KsPtrHandle(XDR * xdr, bool & success)
{
    T * p = T::xdrNew(xdr);
    if ( p ) {
        // Allocation ok, streaming succeeded.
        // Bind handle and object
        //
        success = this->bindTo(p, PltOsNew);
    } else {
        success = false;
    }
}

//////////////////////////////////////////////////////////////////////

template <class T>
KsPtrHandle<T> *
KsPtrHandle<T>::xdrNew(XDR *xdr)
{
    bool ok;                                         
    KsPtrHandle<T> * p = new KsPtrHandle<T>(xdr, ok);     
    if (!ok && p) {                             
        delete p;                                
        p = 0;                                   
    }                                            
    return p;                                    
}                                                

//////////////////////////////////////////////////////////////////////
#endif // PLT_HANDLE_IMPL_INCLUDED
