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
#include <plt/container.hpp>

//////////////////////////////////////////////////////////////////////

template <class T> class PltPQIterator;

//////////////////////////////////////////////////////////////////////
// T must be comparable
//////////////////////////////////////////////////////////////////////

template <class T>
class PltPriorityQueue 
: public PltContainer<T>,
  public PltDebuggable 
{
    friend class PltPQIterator<T>;
public:
    enum { defaultGrowSize = 16 };
    PltPriorityQueue(size_t growsize = defaultGrowSize);
    virtual ~PltPriorityQueue();

    // accessors
    bool isEmpty() const;
    size_t size() const;
    T peek() const;
    PltIterator<T> * newIterator() const;

    // modifiers
    bool add(T elem);
    T removeFirst();
    bool remove(T elem); // remove one occurrence of elem from *this

#if PLT_DEBUG_INVARIANTS
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
inline PltIterator<T> *
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

#endif

