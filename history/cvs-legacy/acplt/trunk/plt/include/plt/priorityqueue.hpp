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
    PltPriorityQueue
        (bool (*lessThanFunction)(const T, const T),
        size_t growsize=16);
    virtual ~PltPriorityQueue();

    // accessors
    bool isEmpty() const;
    size_t size() const;
    const T peek() const;

    // modifiers
    bool add(T elem);
    T removeFirst();
    bool remove(const T elem); // remove one occurrence of elem from *this

#if PLT_DEBUG_INVARIANTS
    virtual bool invariant() const;
#endif

protected:

private:
    PltPriorityQueue(const PltPriorityQueue &);     // forbidden
    PltPriorityQueue &
        operator = (const PltPriorityQueue &);      // forbidden
    // attributes
    size_t a_growsize;           // grow this many elements
    size_t a_capacity;           // max num of elems 
    size_t a_size;               // num of elements in queue
    T* a_elems;                 // array of elements
    bool (*lessThan)(const T , const T);

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
inline const T
PltPriorityQueue<T>::peek() const
{
    PLT_PRECONDITION( !isEmpty() );
    return a_elems[0];
}
//////////////////////////////////////////////////////////////////////

#endif
