/* -*-plt-c++-*- */
#ifndef KS_LIST_IMPL_INCLUDED
#define KS_LIST_IMPL_INCLUDED
/* $Header: /home/david/cvs/acplt/ks/include/ks/list_impl.h,v 1.9 2008-09-22 08:26:09 henning Exp $ */
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

//////////////////////////////////////////////////////////////////////

#if !PLT_SEE_ALL_TEMPLATES
#include "ks/list.h"
#include "plt/list_impl.h"
#endif

//////////////////////////////////////////////////////////////////////

template <class T>
bool
KsList<T>::xdrEncode(XDR * xdr) const
{
    PLT_PRECONDITION(xdr->x_op == XDR_ENCODE);
    // write size
    u_long sz = KsList<T>::size();
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
    while (! KsList<T>::isEmpty() ) {
        KsList<T>::removeFirst();
    }

    for ( u_long i=0; i < count; ++i) {
        bool ok=false;
        T elem(xdr, ok);
        if (! ok) return false;
        if (! this->addLast(elem) ) return false;
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
#endif // KS_LIST_IMPL_INCLUDED
// EOF ks/list_impl.h
