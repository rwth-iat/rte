/* -*-plt-c++-*- */
#ifndef KS_ARRAY_IMPL_INCLUDED
#define KS_ARRAY_IMPL_INCLUDED
/* $Header: /home/david/cvs/acplt/ks/include/ks/array_impl.h,v 1.12 2008-09-22 08:26:09 henning Exp $ */
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
// Author: Martin Kneissl <martin@plt.rwth-aachen.de>
//////////////////////////////////////////////////////////////////////

#if !PLT_SEE_ALL_TEMPLATES
#include "ks/array.h"
#include "plt/array_impl.h"
#endif

#if PLT_COMPILER_BORLAND >= 0x0530
#include "ks/array_builtins.h"
#endif


//////////////////////////////////////////////////////////////////////

template <class T>
bool
KsArray<T>::xdrDecode(XDR *xdr)
{
    PLT_PRECONDITION(xdr->x_op == XDR_DECODE);
    // retrieve size
    //
    u_long sz;
    if (! ks_xdrd_u_long(xdr, &sz) ) return false; // failed

    // adjust array size (possibly losing contents)
    //
    if (KsArray<T>::size() != sz) {
        // allocate sz elements
        PltArrayHandle<T> ha(new T[sz], PltOsArrayNew);
        if (!ha) return false; // failed
        KsArray<T>::a_array = ha;
        KsArray<T>::a_size = sz;
    }
    PLT_ASSERT(size() == sz);

    // now deserialize elements
    //
    for (size_t i=0; i < KsArray<T>::a_size; ++i) {
        if (! (KsArray<T>::a_array[i]).xdrDecode(xdr) ) return false; // failed
    }

    // success
    //
    return true;
}

//////////////////////////////////////////////////////////////////////

template <class T>
bool
KsArray<T>::xdrEncode(XDR *xdrs) const
{
    PLT_PRECONDITION(xdrs->x_op == XDR_ENCODE);
    // serialize size
    //
    u_long sz = KsArray<T>::a_size;
    if (! ks_xdre_u_long(xdrs, &sz)) return false; // fail

    // serialize elements
    //
    for (size_t i = 0; i < KsArray<T>::a_size; ++i) {
        if (! (KsArray<T>::a_array[i]).xdrEncode(xdrs)) return false; // fail
    }

    // success
    //
    return true;
}

//////////////////////////////////////////////////////////////////////

template <class T>
KsArray<T> *
KsArray<T>::xdrNew(XDR *xdrs)
{
    PLT_PRECONDITION(xdrs->x_op == XDR_DECODE);
    bool ok;
    KsArray<T> * p = new KsArray<T>(xdrs, ok);
    if ( !ok && p) {
        delete p;
        p = 0;
    }
    return p;
}


//////////////////////////////////////////////////////////////////////
#endif // KS_ARRAY_IMPL_INCLUDED

// ks/array_impl.h

