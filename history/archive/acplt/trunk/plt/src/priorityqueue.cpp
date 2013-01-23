/* -*-plt-c++-*- */

// File plt/priorityqueue.cpp

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

#include "plt/priorityqueue.h"

#if !PLT_INSTANTIATE_TEMPLATES
#include "plt/priorityqueue_impl.h"
#endif


#if 0

//////////////////////////////////////////////////////////////////////

template <class T>
inline size_t
PltPriorityQueue<T>::leftChild(size_t i) const
{
    return 2 * i + 1;
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline size_t
PltPriorityQueue<T>::rightChild(size_t i) const
{
    return 2 * i + 2;
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline size_t
PltPriorityQueue<T>::parent(size_t i) const
{
    return (i-1)/2;
}

//////////////////////////////////////////////////////////////////////

template <class T>
PltPriorityQueue<T>::PltPriorityQueue(size_t growsize) 
: a_growsize(growsize),
  a_capacity(0), 
  a_size(0), 
  a_elems(0)
{
    PLT_PRECONDITION( growsize > 0 );
    grow(a_growsize);
    PLT_CHECK_INVARIANT();
}

//////////////////////////////////////////////////////////////////////

template <class T>
PltPriorityQueue<T>::~PltPriorityQueue() {
    if (a_elems) {
        delete [] a_elems;
    }
}
//////////////////////////////////////////////////////////////////////

#if PLT_DEBUG_INVARIANTS

template <class T>
bool 
PltPriorityQueue<T>::invariant() const
{
    // allocation
    if (a_capacity < a_size ) return false;
    if (a_capacity > 0 && !a_elems ) return false;

    // partial ordering
    for (size_t i = 0; i < (a_size+1)/2; ++i ) {
        const size_t lc = leftChild(i);
        const size_t rc = rightChild(i);
        if ( lc < a_size && a_elems[lc] < a_elems[i] ) {
            return false;
        } else if ( rc < a_size && a_elems[rc] < a_elems[i] ) {
            return false;
        }
    }
    return true;
}

#endif

//////////////////////////////////////////////////////////////////////

template <class T>
size_t
PltPriorityQueue<T>::locate(size_t node, T elem) const
{
    // root
    if (elem == a_elems[node]) {
        return node;
    }
    const size_t lc = leftChild(node);
    if ( lc < a_size ) {
        // left subtree exists
        if ( elem >= a_elems[lc] ) {
            // maybe in this subtree 
            size_t lfound = locate(lc, elem);
            if ( lfound < a_size ) {
                return lfound;
            }
        }
        const size_t rc = rightChild(node);
        // right subtree
        if ( rc < a_size ) {
            // right subtree exists
            if ( elem >= a_elems[rc] ) {
                // maybe in this subtree
                size_t rfound = locate(rc, elem);
                if (rfound) {
                    return rfound;
                }
            }
        } // rc < a_size
    } // lc < a_size
    // not found
    return a_size;
}

//////////////////////////////////////////////////////////////////////

template <class T>
bool
PltPriorityQueue<T>::grow(size_t cap)
{
    PLT_PRECONDITION( cap > 0 && cap >= a_size );
    
    
    T *p = new T[cap];
    if (!p) {
        return false;
    } else {
        if (a_elems) {
            // copy a_elems
            T *s = a_elems;
            T *d = p;
            while (d < p + a_size) {
                *d++ = *s++;
            }
            // clean up old array
            delete[] a_elems;
        }
        a_capacity = cap;
        a_elems = p;
        return true;
    }
    PLT_CHECK_INVARIANT();
}

//////////////////////////////////////////////////////////////////////

template <class T>
void
PltPriorityQueue<T>::downheap(size_t k, T v) {
//
// reestablish heap condition, starting at k, inserting v
//
    const size_t last_parent = parent(a_size-1);
    while(k <= last_parent) {
        size_t lc = leftChild(k);
        size_t rc = lc+1;
        size_t j =  
            ( rc < a_size && a_elems[rc] < a_elems[lc] ) 
                ? rc : lc;
        // j is smaller child of k
        if ( a_elems[j] >= v ) {
            break;  // <<<<
        } else {
            a_elems[k] = a_elems[j];
            PLT_ASSERT(j > k); // k always increasing
            k = j;
        }
    }
    a_elems[k] = v;
    PLT_CHECK_INVARIANT();
}


//////////////////////////////////////////////////////////////////////


template <class T>
bool
PltPriorityQueue<T>::add(T x)
{
    if (a_size == a_capacity) {
        if (!grow(a_capacity+a_growsize)) {
            return false;
        }
    }
    PLT_ASSERT(a_size < a_capacity);

    // upheap
    size_t i = a_size++;
    while (i>0) {
        size_t j = parent(i);
        if ( x < a_elems[j] ) {
            // partial exchange
            a_elems[i] = a_elems[j];
            PLT_ASSERT(j < i); // i always decreasing
            i = j;
        } else {
            // i is insertion point for x
            break;
        }
    }
    a_elems[i] = x;
    PLT_CHECK_INVARIANT();
    return true;
}

//////////////////////////////////////////////////////////////////////

template <class T>
T
PltPriorityQueue<T>::removeFirst()
{
    PLT_PRECONDITION( ! isEmpty() );
    T x(a_elems[0]);
    T v(a_elems[--a_size]);

    // TODO: may shrink here

    downheap(0,v);

    PLT_CHECK_INVARIANT();
    return x;
}

//////////////////////////////////////////////////////////////////////

template <class T>
bool
PltPriorityQueue<T>::remove(T elem)
{
    PLT_PRECONDITION( ! isEmpty() );
    bool result;
    size_t loc = locate(0, elem);
    if ( loc < size() ) {
        // found 
        if ( (--a_size > 0) && (loc < a_size) ) {
            T v(a_elems[a_size]);
            // Martin once thought about shrinking the vector
            // if possible, but we just pretend that we might need
            // the free entries in the future, so we don't return
            // what we had used ...
            downheap(loc,v);
	}
        result = true;
    } else {
        result = false;
    }
    PLT_CHECK_INVARIANT();
    return result;
}
#endif
//////////////////////////////////////////////////////////////////////
// EOF plt/priorityqueue.cpp
//////////////////////////////////////////////////////////////////////
