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

//////////////////////////////////////////////////////////////////////

template <class T>
class PltPriorityQueue_base : PltDebuggable {
public:
    const size_t defaultGrowSize = 16;
    PltPriorityQueue_base(size_t growsize = defaultGrowSize);
    virtual ~PltPriorityQueue_base();

    // accessors
    bool isEmpty() const;
    size_t size() const;
    T peek() const;

    // modifiers
    bool add(T elem);
    T removeFirst();
    bool remove(T elem); // remove one occurrence of elem from *this

#if PLT_DEBUG_INVARIANTS
    virtual bool invariant() const;
#endif

protected:
    virtual bool lessThan(T, T) const = 0;
private:
    PltPriorityQueue_base(const PltPriorityQueue_base &);     // forbidden
    PltPriorityQueue_base &
        operator = (const PltPriorityQueue_base &);      // forbidden
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
class PltPriorityQueue : public PltPriorityQueue_base<T> {
public:
    PltPriorityQueue(size_t growsize=defaultGrowSize);
protected:
    virtual bool lessThan(T,T) const;
};

//////////////////////////////////////////////////////////////////////

template <class P>
class PltPtrPriorityQueue : public PltPriorityQueue_base<void *> {
public:
    PltPtrPriorityQueue(size_t growsize=defaultGrowSize);
    // accessors
    P peek() const;
    // modifiers
    bool add(P elem);
    P removeFirst();
    bool remove(P elem); // remove one occurrence of elem from *this
protected:
    virtual bool lessThan(const P, const P) const; // new func
private:
    virtual bool lessThan(void*,void*) const;        // override old
};

//////////////////////////////////////////////////////////////////////
// INLINE IMPLEMENTATION
//////////////////////////////////////////////////////////////////////

template <class T>
inline bool
PltPriorityQueue_base<T>::isEmpty() const 
{
    return a_size == 0;
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline size_t
PltPriorityQueue_base<T>::size() const
{
    return a_size;
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline T
PltPriorityQueue_base<T>::peek() const
{
    PLT_PRECONDITION( !isEmpty() );
    return a_elems[0];
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

template <class T>
inline
PltPriorityQueue<T>::PltPriorityQueue(size_t growsize)
: PltPriorityQueue_base<T>(growsize)
{
}
  
//////////////////////////////////////////////////////////////////////

template <class T>
inline bool
PltPriorityQueue<T>::lessThan(T t1, T t2) const
{
    return t1<t2;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

template <class P>
inline
PltPtrPriorityQueue<P>::PltPtrPriorityQueue(size_t growsize)
: PltPriorityQueue_base<void*>(growsize)
{
}
  
//////////////////////////////////////////////////////////////////////

template <class P>
inline bool
PltPtrPriorityQueue<P>::lessThan(void *p1, void *p2) const
{
    return lessThan((P)p1,(P)p2);
}

//////////////////////////////////////////////////////////////////////

template <class P>
inline bool
PltPtrPriorityQueue<P>::lessThan(P p1, P p2) const
{
    return *p1 < *p2;
}

//////////////////////////////////////////////////////////////////////


template <class P>
inline P 
PltPtrPriorityQueue<P>::peek() const
{
    return (P) PltPriorityQueue_base<void*>::peek();
}

//////////////////////////////////////////////////////////////////////

template <class P>
inline bool
PltPtrPriorityQueue<P>::add(P elem)
{
    return PltPriorityQueue_base<void*>::add((void*)elem);
}

//////////////////////////////////////////////////////////////////////

template <class P>
P
PltPtrPriorityQueue<P>::removeFirst()
{
    return (P) PltPriorityQueue_base<void*>::removeFirst();
}

//////////////////////////////////////////////////////////////////////

template <class P>
bool
PltPtrPriorityQueue<P>::remove(P elem)
{
    return PltPriorityQueue_base<void*>::remove((void*) elem);
}


//////////////////////////////////////////////////////////////////////

#endif


