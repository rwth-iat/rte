/* -*-plt-c++-*- */
/* 
 *  Copyright (c) 1996 PLT, RWTH-Aachen, Germany. See file copy_plt.txt!
 *  Author: Martin Kneissl <martin@plt.rwth-aachen.de>
 *
 */

#include <plt/debug.h>
#include <plt/priorityqueue.hpp>

//////////////////////////////////////////////////////////////////////

template <class T>
PltPriorityQueue<T>::PltPriorityQueue(size_t cap) 
: capacity(0), 
  size(0), 
  elems(0)
{
    PLT_PRECONDITION( cap > 0 );
    grow(cap);
    PLT_CHECK_INVARIANT();
}

//////////////////////////////////////////////////////////////////////

#if PLT_DEBUG_INVARIANTS

template <class T>
bool 
PltPriorityQueue<T>::invariant() const
{
    // allocation
    if (capacity < size ) return false;
    if (capacity > 0 && !elems ) return false;

    // partial ordering
    for (size_t i = 0; i < (size+1)/2; ++i ) {
        // first child must not be greater than parent
        if ( lessThan(elems[2*i+1], elems[i]) ) {
            return false;
        }
        // second child -- if present -- must not be greater
        // than parent
        if ( 2*i+2 < size && lessThan(elems[2*i+2], elems[i]) ) {
            return false;
        }
    }
    return true;
}

#endif

//////////////////////////////////////////////////////////////////////

template <class T>
bool
PltPriorityQueue<T>::grow(size_t cap)
{
    PLT_PRECONDITION( cap > 0 && cap >= size );
    
    
    T *p = new T[cap];
    if (!p) {
        return false;
    } else {
        if (elems) {
            // copy elems
            T *s = elems;
            T *d = p;
            while (d < p+size) {
                *d++ = *s++;
            }
            // clean up old array
            delete[] elems;
        }
        capacity = cap;
        elems = p;
        return true;
    }
    PLT_CHECK_INVARIANT();
}

//////////////////////////////////////////////////////////////////////

template <class T>
bool
PltPriorityQueue<T>::add(T x)
{
    if (size == capacity) {
        if (!grow(2*capacity)) {
            return false;
        }
    }
    assert(size < capacity);

    // upheap
    size_t i;
    for (i = size++;
         i > 0 && lessThan(x, elems[i/2]); 
         i /= 2) {
        elems[i] = elems[i/2];
    }
    elems[i] = x;
    PLT_CHECK_INVARIANT();
    return true;
}

//////////////////////////////////////////////////////////////////////

template <class T>
T
PltPriorityQueue<T>::removeFirst()
{
    PLT_PRECONDITION( ! isEmpty() );

    T x(elems[0]);
    T v(elems[size--]);
    
    // TODO: may shrink here
    
    // downheap
    size_t j;
    size_t k;
    for ( k = 0; k <= (size+1)/2; k = j ) {
        j = 2 * k + 1;
        // j is first child of k
        if ( j + 1 < size && lessThan(elems[j+1] , elems[j] ) )  {
            ++j;
        }
        // j is smaller child of k
        if ( ! lessThan( elems[j], v ) ) {
            break;  // <<<<
        } else {
            elems[k] = elems[j];
        }
    }
    elems[k] = v;

    PLT_CHECK_INVARIANT();
    return x;
}


//////////////////////////////////////////////////////////////////////

