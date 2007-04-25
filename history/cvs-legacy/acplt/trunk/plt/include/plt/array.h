/* -*-plt-c++-*- */
#ifndef PLT_ARRAY_INCLUDED
#define PLT_ARRAY_INCLUDED
/* $Header: /home/david/cvs/acplt/plt/include/plt/array.h,v 1.12 2007-04-25 12:57:21 martin Exp $ */
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
// plt/array.h provides a simple fixed size array template
//////////////////////////////////////////////////////////////////////

#include "plt/container.h"
#include "plt/handle.h"

//////////////////////////////////////////////////////////////////////
// template class PltArray<T>
//////////////////////////////////////////////////////////////////////
//
// Array knowing its size.
//
// Array elements are SHARED when operator= or the copy constructor is
// used. Arrays are handled data types (see plt/handle.h).
// 
// Constructor:
// ------------
//
// PltArray(size_t size=0, T * p=0, enum PltOwnership os=PltOsArrayNew)
// 
// If size==0, p must be 0 and an empty Array is constructed.
// If size>0 and p == 0 the storage for elements is created on free store.
// If size>0 and p != 0 p must point to an array of at least size elements.
// os must specify how the objects p points to are allocated
//
// You can get a shallow copy using copy()
//
// Remarks:
// --------
// [1] May fail to allocate enough memory.
//     In this case they return an empty Array ( size() == 0 ).
// [2] Be careful with the pointer to the representation that you
//     get. Object lifetime is controlled by the PltArray object!
//
//////////////////////////////////////////////////////////////////////

template <class T>
class PltArray
: public PltArrayed<T>
{
public:
    PltArray(size_t size = 0); // array size          // [1]
    PltArray(size_t size,      // array size 
             T * p,           // address of first element
             enum PltOwnership);                      // [1]

    // accessors
    const T & operator[] (size_t i) const;
    PltArray copy() const;                            // [1]
    T * getPtr() const;                               // [2]

    // modifiers
    T & operator[] (size_t i);
    PltArray<T> & operator = ( const PltArray<T> &);

    // container interface
    virtual size_t size() const;

protected:
    PltArrayHandle<T> a_array;
    size_t a_size;

#if PLT_DEBUG_INVARIANTS
public:
    virtual bool invariant() const;
    virtual ~PltArray() { }
#endif
};


//////////////////////////////////////////////////////////////////////
// INLINE IMPLEMENTATION
//////////////////////////////////////////////////////////////////////

template <class T>
inline
PltArray<T>::PltArray(size_t sz, T * p, enum PltOwnership os)
: a_array(p, os)
{
    PLT_PRECONDITION( sz > 0 && p );
    a_size = a_array ? sz : 0;
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline
PltArray<T>::PltArray(size_t sz)
: a_array(sz>0 ? new T[sz] : 0 , PltOsArrayNew)
{
    a_size = a_array ? sz : 0;
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline PltArray<T> &
PltArray<T>::operator = ( const PltArray<T> & rhs)
{
    a_size = rhs.a_size;
    a_array = rhs.a_array;
    return *this;
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline size_t
PltArray<T>::size() const
{
    return a_size;
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline T &
PltArray<T>::operator [] (size_t i)
{
    PLT_PRECONDITION( i < a_size );
    return a_array[i];
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline const T &
PltArray<T>::operator [] (size_t i) const
{
    PLT_PRECONDITION( i < a_size );
    return a_array[i];
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline T*
PltArray<T>::getPtr() const
{
    return a_array.getPtr();
}

//////////////////////////////////////////////////////////////////////

#if PLT_SEE_ALL_TEMPLATES
#include "plt/array_impl.h"
#endif

//////////////////////////////////////////////////////////////////////

#endif // PLT_ARRAY_INCLUDED
