/* -*-plt-c++-*- */
#ifndef PLT_PRIORITYQUEUE_INCLUDED
#define PLT_PRIORITYQUEUE_INCLUDED
/* $Header: /home/david/cvs/acplt/plt/include/plt/priorityqueue.h,v 1.9 2007-04-25 12:57:21 martin Exp $ */
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
#include <stdlib.h>
#include "plt/container.h"

//////////////////////////////////////////////////////////////////////

template <class T> class PltPQIterator;

#if PLT_RETTYPE_OVERLOADABLE
#define PltPQIterator_THISTYPE(T) PltPQIterator<T>
#else
#define PltPQIterator_THISTYPE(T) PltIterator_<T>
#endif

#if PLT_COMPILER_DECCXX
#define PltPQIterator_CLASSTHISTYPE(T) PltPQIterator<T>::THISTYPE
#else
#define PltPQIterator_CLASSTHISTYPE(T) PltPQIterator_THISTYPE(T) 
#endif

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

    PltPQIterator_THISTYPE(T) * newIterator() const;
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
#if PLT_RETTYPE_OVERLOADABLE
    typedef PltPQIterator THISTYPE;
#endif
    PltPQIterator(const PltPriorityQueue<T> &);

    virtual operator bool () const;       // remaining element?
    virtual const T & operator * () const;        // current element
    virtual THISTYPE & operator ++ ();      // advance
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
inline PltPQIterator_THISTYPE(T) *
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
PltPQIterator<T>::operator bool () const
{
    return a_index < a_container.a_size;
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
inline PltPQIterator_CLASSTHISTYPE(T) & 
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

