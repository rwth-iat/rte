/* -*-plt-c++-*- */
#ifndef PLT_COMPHANDLE_INCLUDED
#define PLT_COMPHANDLE_INCLUDED
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
/* Author: Markus Juergens <markusj@plt.rwth-aachen.de> */

//////////////////////////////////////////////////////////////////////

#include "plt/handle.h"

//////////////////////////////////////////////////////////////////////
// PltEqHandle<T> forms a handle which can be checked for equality.
// Class T must provide meaningful operators == and !=
//  
template <class T>
class PltEqHandle
: public PltPtrHandle<T>
{
public:
    PltEqHandle();
    PltEqHandle(T *p, enum PltOwnership);  // no default to avoid conversion!
    PltEqHandle(const PltEqHandle &);
       
    bool operator == (const PltEqHandle &rhs);
    bool operator != (const PltEqHandle &rhs);
};


//////////////////////////////////////////////////////////////////////
// PltCompHandle<T> is a handle which can be compared.
// Class T must provide the operators ==, <, <=, >, >=
//
template <class T>
class PltCompHandle
: public PltEqHandle<T>
{
public:
    PltCompHandle();
    PltCompHandle(T *p, enum PltOwnership);  // no default to avoid conversion!
    PltCompHandle(const PltCompHandle &);
       
    bool operator <  (const PltCompHandle &rhs);
    bool operator <= (const PltCompHandle &rhs);
    bool operator >  (const PltCompHandle &rhs);
    bool operator >= (const PltCompHandle &rhs);
};

//////////////////////////////////////////////////////////////////////
// Inline Implementation
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// class PltEqHandle<T>
//////////////////////////////////////////////////////////////////////

template <class T>
inline
PltEqHandle<T>::PltEqHandle()
{}

//////////////////////////////////////////////////////////////////////

template <class T>
inline
PltEqHandle<T>::PltEqHandle(T *p, enum PltOwnership os)
: PltPtrHandle<T>(p, os)
{}

//////////////////////////////////////////////////////////////////////

template <class T>
inline
PltEqHandle<T>::PltEqHandle(const PltEqHandle &rhs)
: PltPtrHandle<T>(rhs)
{}

//////////////////////////////////////////////////////////////////////

template <class T>
inline bool
PltEqHandle<T>::operator == (const PltEqHandle &rhs)
{
    PLT_PRECONDITION(*this && rhs);

    return **this == *rhs;
}

//////////////////////////////////////////////////////////////////////


template <class T>
inline bool
PltEqHandle<T>::operator != (const PltEqHandle &rhs)
{
    PLT_PRECONDITION(*this && rhs);

    return **this != *rhs;
}

//////////////////////////////////////////////////////////////////////
// class PltCompHandle<T>
//////////////////////////////////////////////////////////////////////


template <class T>
inline
PltCompHandle<T>::PltCompHandle()
{}

//////////////////////////////////////////////////////////////////////


template <class T>
inline
PltCompHandle<T>::PltCompHandle(T *p, enum PltOwnership os)
: PltEqHandle<T>(p, os)
{}

//////////////////////////////////////////////////////////////////////


template <class T>
inline
PltCompHandle<T>::PltCompHandle(const PltCompHandle &rhs)
: PltEqHandle<T>(rhs)
{}

//////////////////////////////////////////////////////////////////////


template <class T>
inline bool
PltCompHandle<T>::operator < (const PltCompHandle &rhs)
{
    PLT_PRECONDITION(*this && rhs);

    return **this < *rhs;
}

//////////////////////////////////////////////////////////////////////


template <class T>
inline bool
PltCompHandle<T>::operator <= (const PltCompHandle &rhs)
{
    PLT_PRECONDITION(*this && rhs);

    return **this <= *rhs;
}

//////////////////////////////////////////////////////////////////////


template <class T>
inline bool
PltCompHandle<T>::operator > (const PltCompHandle &rhs)
{
    PLT_PRECONDITION(*this && rhs);

    return **this > *rhs;
}

//////////////////////////////////////////////////////////////////////


template <class T>
inline bool
PltCompHandle<T>::operator >= (const PltCompHandle &rhs)
{
    PLT_PRECONDITION(*this && rhs);

    return **this >= *rhs;
}

#endif

//////////////////////////////////////////////////////////////////////
// EOF comphandle.h
//////////////////////////////////////////////////////////////////////


