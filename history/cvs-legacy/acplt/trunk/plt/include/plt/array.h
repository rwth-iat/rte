/* -*-plt-c++-*- */
#ifndef PLT_ARRAY_INCLUDED
#define PLT_ARRAY_INCLUDED
/* $Header: /home/david/cvs/acplt/plt/include/plt/array.h,v 1.8 1997-09-02 15:10:52 martin Exp $ */
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
