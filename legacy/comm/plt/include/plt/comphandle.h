/* -*-plt-c++-*- */
#ifndef PLT_COMPHANDLE_INCLUDED
#define PLT_COMPHANDLE_INCLUDED
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
       
    bool operator == (const PltEqHandle &rhs) const;
    bool operator != (const PltEqHandle &rhs) const;
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
       
    bool operator <  (const PltCompHandle &rhs) const;
    bool operator <= (const PltCompHandle &rhs) const;
    bool operator >  (const PltCompHandle &rhs) const;
    bool operator >= (const PltCompHandle &rhs) const;
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
PltEqHandle<T>::operator == (const PltEqHandle &rhs) const
{
    PLT_PRECONDITION(*this && rhs);

    return **this == *rhs;
}

//////////////////////////////////////////////////////////////////////


template <class T>
inline bool
PltEqHandle<T>::operator != (const PltEqHandle &rhs) const
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
PltCompHandle<T>::PltCompHandle(const PltCompHandle &rhs) const
: PltEqHandle<T>(rhs)
{}

//////////////////////////////////////////////////////////////////////


template <class T>
inline bool
PltCompHandle<T>::operator < (const PltCompHandle &rhs) const
{
    PLT_PRECONDITION(*this && rhs);

    return **this < *rhs;
}

//////////////////////////////////////////////////////////////////////


template <class T>
inline bool
PltCompHandle<T>::operator <= (const PltCompHandle &rhs) const
{
    PLT_PRECONDITION(*this && rhs);

    return **this <= *rhs;
}

//////////////////////////////////////////////////////////////////////


template <class T>
inline bool
PltCompHandle<T>::operator > (const PltCompHandle &rhs) const
{
    PLT_PRECONDITION(*this && rhs);

    return **this > *rhs;
}

//////////////////////////////////////////////////////////////////////


template <class T>
inline bool
PltCompHandle<T>::operator >= (const PltCompHandle &rhs) const
{
    PLT_PRECONDITION(*this && rhs);

    return **this >= *rhs;
}

#endif

//////////////////////////////////////////////////////////////////////
// EOF comphandle.h
//////////////////////////////////////////////////////////////////////






