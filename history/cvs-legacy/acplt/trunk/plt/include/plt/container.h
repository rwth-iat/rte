/* -*-plt-c++-*- */
#ifndef PLT_CONTAINER_INCLUDED
#define PLT_CONTAINER_INCLUDED
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
// plt/container.h contains Container and Iterator interfaces
//////////////////////////////////////////////////////////////////////

#include "plt/debug.h"

#include <stdlib.h>

//////////////////////////////////////////////////////////////////////
// Iterator interfaces
//////////////////////////////////////////////////////////////////////
// PltIterators iterate in one "direction" over a container,
// PltBidirIterators can iterate forward and backward.
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// CAVEAT: at least one of operator-> and operator * must be
// overridden in a derived class to avoid infinite recursion!
//////////////////////////////////////////////////////////////////////

template<class T>
class PltIterator
{
public:
    virtual ~PltIterator() { }
    virtual operator const void * () const = 0;   // remaining element?
    virtual const T & operator * () const;        //  /current element
    virtual const T * operator -> () const;       //  \override at least one!
    virtual PltIterator & operator ++ () = 0;     // advance
    virtual void operator ++ (int);               // (postfix)
    virtual void toStart() = 0;                   // go to the beginning
};


//////////////////////////////////////////////////////////////////////

template <class T>
class PltBidirIterator : public PltIterator<T>
{
public:
    virtual PltBidirIterator & operator -- () = 0;  // step backwards
    virtual void operator -- (int);                 // (postfix)
    virtual void toEnd() = 0;                       // go to the end
};

//////////////////////////////////////////////////////////////////////
// Container interface
//////////////////////////////////////////////////////////////////////

template<class T>
class PltContainer
{
public:
    virtual bool isEmpty() const;
    virtual size_t size() const = 0;
    virtual PltIterator<T> * newIterator() const = 0;
};

//////////////////////////////////////////////////////////////////////
// INLINE IMPLEMENTATION
//////////////////////////////////////////////////////////////////////

template <class T>
inline const T *
PltIterator<T>::operator -> () const
{
    return & (operator * () );
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline const T &
PltIterator<T>::operator * () const
{
    return * (operator -> () );
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline void
PltIterator<T>::operator ++ (int)
{
    operator ++ ();
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline void
PltBidirIterator<T>::operator -- (int)
{
    operator -- ();
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

template <class T>
inline bool
PltContainer<T>::isEmpty() const
{
    return size() == 0;
}

//////////////////////////////////////////////////////////////////////

#endif // PLT_CONTAINER_INCLUDED
