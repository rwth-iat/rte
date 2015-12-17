/* -*-plt-c++-*- */
#ifndef PLT_COMPARABLE_INCLUDED
#define PLT_COMPARABLE_INCLUDED
/* $Header: /home/david/cvs/acplt/plt/include/plt/comparable.h,v 1.9 2007-04-25 12:57:21 martin Exp $ */
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

#include "plt/debug.h"

//////////////////////////////////////////////////////////////////////

template <class T>
class PltPtrComparable
{
public:
    bool operator <  ( PltPtrComparable<T> t2);
    bool operator >  ( PltPtrComparable<T> t2);
    bool operator != ( PltPtrComparable<T> t2);
    bool operator == ( PltPtrComparable<T> t2);
    bool operator <= ( PltPtrComparable<T> t2);
    bool operator >= ( PltPtrComparable<T> t2);

    operator T * ();
    T * operator ->() const;
    T& operator *() const;
    PltPtrComparable(T * = 0);

private:
    T *_p;
};

//////////////////////////////////////////////////////////////////////
// INLINE IMPLEMENTATION
//////////////////////////////////////////////////////////////////////

template <class T>
inline
PltPtrComparable<T>::PltPtrComparable(T *p)
: _p(p)
{
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline
PltPtrComparable<T>::operator T * ()
{
    return _p;
}

//////////////////////////////////////////////////////////////////////


template <class T>
inline T *
PltPtrComparable<T>::operator -> () const
{
    return _p;
}

//////////////////////////////////////////////////////////////////////


template <class T>
inline T&
PltPtrComparable<T>::operator * () const
{
    return *_p;
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline bool 
PltPtrComparable<T>::operator <  ( PltPtrComparable<T> t2) 
{
    return *_p < *t2._p;
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline bool 
PltPtrComparable<T>::operator >  ( PltPtrComparable<T> t2) 
{
    return *_p > *t2._p;
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline bool 
PltPtrComparable<T>::operator <= ( PltPtrComparable<T> t2) 
{
    return *_p <= *t2._p;
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline bool 
PltPtrComparable<T>::operator >=  ( PltPtrComparable<T> t2) 
{
    return *_p >= *t2._p;
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline bool 
PltPtrComparable<T>::operator ==  ( PltPtrComparable<T> t2) 
{
    return *_p == *t2._p;
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline bool 
PltPtrComparable<T>::operator !=  ( PltPtrComparable<T> t2) 
{
    return *_p != *t2._p;
}

//////////////////////////////////////////////////////////////////////


#endif // PLT_COMPARABLE_INCLUDED
