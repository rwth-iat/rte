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
        // first child must not be greater than parent
        if ( 2*i+1 < a_size && lessThan(a_elems[2*i+1], a_elems[i]) ) {
            return false;
        }
        // second child must not be greater than parent
        if ( 2*i+2 < a_size && lessThan(a_elems[2*i+2], a_elems[i]) ) {
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
bool
PltPriorityQueue<T>::add(const T & x)
{
    if (a_size == a_capacity) {
        if (!grow(a_capacity+a_growsize)) {
            return false;
        }
    }
    assert(a_size < a_capacity);

    // upheap
    size_t i = a_size++;
    while (i>0) {
        size_t j = (i-1)/2; // parent of i
        if ( lessThan(x, a_elems[j]) ) {
            // partial exchange
            a_elems[i] = a_elems[j];
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
    T v(a_elems[a_size--]);
    
    // TODO: may shrink here
    
    // downheap
    size_t j;
    size_t k;
    for ( k = 0; k <= (a_size+1)/2; k = j ) {
        j = 2 * k + 1;
        // j is first child of k
        if ( j + 1 < a_size && lessThan(a_elems[j+1] , a_elems[j] ) )  {
            ++j;
        }
        // j is smaller child of k
        if ( ! lessThan( a_elems[j], v ) ) {
            break;  // <<<<
        } else {
            a_elems[k] = a_elems[j];
        }
    }
    a_elems[k] = v;

    PLT_CHECK_INVARIANT();
    return x;
}


//////////////////////////////////////////////////////////////////////

