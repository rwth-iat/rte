/* -*-plt-c++-*- */
#ifndef PLT_COMPARABLE_INCLUDED
#define PLT_COMPARABLE_INCLUDED
/* $Header: /home/david/cvs/acplt/plt/include/plt/comparable.h,v 1.4 1997-03-17 10:58:31 martin Exp $ */
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
    T * operator ->();
    T& operator *();
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
PltPtrComparable<T>::operator -> ()
{
    return _p;
}

//////////////////////////////////////////////////////////////////////


template <class T>
inline T&
PltPtrComparable<T>::operator * ()
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
