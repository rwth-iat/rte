/* -*-plt-c++-*- */
/* 
 *  Copyright (c) 1996 PLT, RWTH-Aachen, Germany. See file copy_plt.txt!
 *  Author: Martin Kneissl <martin@plt.rwth-aachen.de>
 *
 */

#ifndef PLT_PRIORITYQUEUE_INCLUDED
#define PLT_PRIORITYQUEUE_INCLUDED

#include <plt/debug.h>
#include <stdlib.h>

template <class T>
class PltPriorityQueue : PltDebuggable {
public:
    PltPriorityQueue(size_t growsize=16);
    virtual ~PltPriorityQueue();

    // accessors
    bool isEmpty() const;
    size_t size() const;
    const T & peek() const;

    // modifiers
    bool add(const T & elem);
    T removeFirst();

#if PLT_DEBUG_INVARIANTS
    virtual bool invariant() const;
#endif

protected:
    virtual bool lessThan(const T& , const T&) const;

private:
    PltPriorityQueue(const PltPriorityQueue &);     // forbidden
    PltPriorityQueue &
        operator = (const PltPriorityQueue &);      // forbidden
    size_t a_growsize;           // grow this many elements
    size_t a_capacity;           // max num of elems 
    size_t a_size;               // num of elements in queue
    T* a_elems;                  // array of elements
    //
    bool grow(size_t);
};

//////////////////////////////////////////////////////////////////////
// INLINE IMPLEMENTATION
//////////////////////////////////////////////////////////////////////

template <class T>
bool
PltPriorityQueue<T>::isEmpty() const 
{
    return a_size == 0;
}

//////////////////////////////////////////////////////////////////////

template <class T>
size_t
PltPriorityQueue<T>::size() const
{
    return a_size;
}

//////////////////////////////////////////////////////////////////////

template <class T>
const T &
PltPriorityQueue<T>::peek() const
{
    PLT_PRECONDITION( !isEmpty() );
    return a_elems[0];
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

template <class T>
inline bool
PltPriorityQueue<T>::lessThan(const T & t1, const T & t2) const {
    return t1 < t2;
}


//////////////////////////////////////////////////////////////////////

#endif
