/* -*-plt-c++-*- */
#ifndef PLT_ARRAY_IMPL_INCLUDED
#define PLT_ARRAY_IMPL_INCLUDED
/* $Header: /home/david/cvs/acplt/plt/include/plt/array_impl.h,v 1.7 2007-04-25 12:57:21 martin Exp $ */
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
// PltArray template implementation
//////////////////////////////////////////////////////////////////////

#include "plt/array.h"

#if !PLT_SEE_ALL_TEMPLATES
#include "plt/handle_impl.h"
#include "plt/container_impl.h"
#endif

//////////////////////////////////////////////////////////////////////

#if PLT_DEBUG_INVARIANTS
template <class T>
bool 
PltArray<T>::invariant() const
{
    return (a_size == 0 && ! a_array) || (a_size > 0 && a_array);
}
#endif

//////////////////////////////////////////////////////////////////////

template <class T>
PltArray<T>
PltArray<T>::copy() const
{
    PltArray<T> res(a_size);
    
    // copy elements
    for (size_t i = 0; i < a_size; ++i) {
        res.a_array[i] = a_array[i];
    }
    return res;
}

//////////////////////////////////////////////////////////////////////

#endif
// EOF plt/array_impl.h
