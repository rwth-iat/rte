/* -*-plt-c++-*- */
#ifndef PLT_PRIORITYQUEUE_INCLUDED
#define PLT_PRIORITYQUEUE_INCLUDED
/* $Header: /home/david/cvs/acplt/plt/include/plt/priorityqueue.h,v 1.3 1997-04-10 14:09:27 martin Exp $ */
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
#include <stdlib.h>
#include "plt/container.h"

//////////////////////////////////////////////////////////////////////

template <class T> class PltPQIterator;

//////////////////////////////////////////////////////////////////////
// T must be comparable
//////////////////////////////////////////////////////////////////////

template <class T>
class PltPriorityQueue 
: public PltContainer<T>
{
    friend class PltPQIterator<T>;
public:
    enum { defaultGrowSize = 16 };
    PltPriorityQueue(size_t growsize = defaultGrowSize);
    virtual ~PltPriorityQueue();

    // accessors
    bool isEmpty() const;
    T peek() const;

    // modifiers
    bool add(T elem);
    T removeFirst();
    bool remove(T elem); // remove one occurrence of elem from *this

    PltPQIterator<T> * newIterator() const;
    size_t size() const;

#if PLT_DEBUG_INVARIANTS
public:
    virtual bool invariant() const;
#endif

private:
    PltPriorityQueue(const PltPriorityQueue &);     // forbidden
    PltPriorityQueue &
        operator = (const PltPriorityQueue &);      // forbidden
    // attributes
    size_t a_growsize;           // grow this many elements
    size_t a_capacity;           // max num of elems 
    size_t a_size;               // num of elements in queue
    T* a_elems;                 // array of elements
    // helpers
    size_t leftChild(size_t) const;
    size_t rightChild(size_t) const;
    size_t parent(size_t) const;
    // accessors
    size_t locate(size_t node, T elem) const;
    // modifiers
    void downheap(size_t k, T v);
    bool grow(size_t);
};


//////////////////////////////////////////////////////////////////////

template <class T>
class PltPQIterator
: public PltIterator<T>
{
public:
    PltPQIterator(const PltPriorityQueue<T> &);

    virtual operator const void * () const;       // remaining element?
    virtual const T & operator * () const;        // current element
    virtual PltIterator<T> & operator ++ ();      // advance
    virtual void toStart();                       // go to the beginning
private:
    const PltPriorityQueue<T> & a_container;
    size_t a_index;
};

//////////////////////////////////////////////////////////////////////
// INLINE IMPLEMENTATION
//////////////////////////////////////////////////////////////////////

template <class T>
inline bool
PltPriorityQueue<T>::isEmpty() const 
{
    return a_size == 0;
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline size_t
PltPriorityQueue<T>::size() const
{
    return a_size;
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline T
PltPriorityQueue<T>::peek() const
{
    PLT_PRECONDITION( !isEmpty() );
    return a_elems[0];
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline PltPQIterator<T> *
PltPriorityQueue<T>::newIterator() const
{
    return new PltPQIterator<T>(*this);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

template <class T>
inline
PltPQIterator<T>::PltPQIterator(const PltPriorityQueue<T> & pq)
: a_container(pq), 
  a_index(0)
{
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline
PltPQIterator<T>::operator const void * () const
{
    return a_index < a_container.a_size ? this : 0;
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline const T & 
PltPQIterator<T>::operator * () const
{
    PLT_PRECONDITION(*this);
    return a_container.a_elems[a_index];
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline PltIterator<T> & 
PltPQIterator<T>::operator ++ ()
{
    PLT_PRECONDITION(*this);
    ++a_index;
    return *this;
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline void
PltPQIterator<T>::toStart()
{
    a_index=0;
}

//////////////////////////////////////////////////////////////////////

#if PLT_SEE_ALL_TEMPLATES
#include "plt/priorityqueue_impl.h"
#endif

//////////////////////////////////////////////////////////////////////

#endif // PLT_PRIORITYQUEUE_INCLUDED

