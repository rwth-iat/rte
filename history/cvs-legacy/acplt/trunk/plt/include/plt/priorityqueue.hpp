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
    PltPriorityQueue(size_t cap);
    bool isEmpty() const;
    bool add(T elem);
    T removeFirst();

#if PLT_DEBUG_INVARIANTS
    virtual bool invariant() const;
#endif

protected:
    virtual bool lessThan(T, T) const = 0;

private:
    PltPriorityQueue(const PltPriorityQueue &);     // forbidden
    PltPriorityQueue &
        operator = (const PltPriorityQueue &);      // forbidden
    size_t capacity;           // max num of elems 
    size_t size;               // num of elements in queue
    T* elems;                  // array of elements
    bool grow(size_t);
};


//////////////////////////////////////////////////////////////////////

template <class T>
class PltSimplePQ : public PltPriorityQueue<T> {
public:
    PltSimplePQ(size_t cap);
protected:
    virtual bool lessThan(T,T) const;
};


//////////////////////////////////////////////////////////////////////
// INLINE IMPLEMENTATION
//////////////////////////////////////////////////////////////////////

template <class T>
inline
PltSimplePQ<T>::PltSimplePQ(size_t cap)
: PltPriorityQueue<T>(cap)
{
}
//////////////////////////////////////////////////////////////////////

template <class T>
bool
PltPriorityQueue<T>::isEmpty() const 
{
    return size == 0;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

template <class T>
inline bool
PltSimplePQ<T>::lessThan(T t1, T t2) const {
    return t1 < t2;
}


//////////////////////////////////////////////////////////////////////


#endif
