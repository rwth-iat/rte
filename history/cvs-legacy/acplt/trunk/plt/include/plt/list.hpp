/* -*-plt-c++-*- */
/* 
 *  Copyright (c) 1996 PLT, RWTH-Aachen, Germany. See file copy_plt.txt!
 *  Author: Martin Kneissl <martin@plt.rwth-aachen.de>
 *
 */

#ifndef PLT_LIST_INCLUDED
#define PLT_LIST_INCLUDED
    
#include <plt/debug.h>
#include <plt/iterator.hpp>

//////////////////////////////////////////////////////////////////////
// Overview
// --------
// We define two kinds of doubly linked lists here: 
// - intrusive lists (PltIList) and 
// - non-intrusive lists. (PltList).
// Intrusive lists require that their elements
// are derived from a special node class (PltListNode) but are
// more efficient than non-intrusive lists.
// Each list class has a corresponding iterator class
//
// Reducing code duplication in template instantiation we have special
// base classes (..._base) which carry the implementation and
// interfacing templates which are intended for users.
//
// Restrictions:
// -------------
// Do not remove an element from a list being iterated on.
//
// These classes are strongly inspired by [Stroustrup,2nd ed].
////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Forward declarations

template <class T> class PltIListIterator;
template <class T> class PltListIterator;

////////////////////////////////////////////////////////////////////////
// Link node with forward and backward chaining.
class PltListNode_base {
    friend class PltList_base;
    friend class PltListIterator_base;
    PltListNode_base *prev, *next;
public:
    PltListNode_base() : prev(0),next(0) {}
};

////////////////////////////////////////////////////////////////////////
// Base class for intrusive lists without information on them.
// Not useful for direct use.

class PltList_base : PltDebuggable {
    friend class PltListIterator_base;
    PltListNode_base *first, *last;

public:
    PltList_base() : first(0), last(0) {}

#if PLT_DEBUG_INVARIANTS
    virtual bool invariant() const;
#endif

    bool isEmpty() const;
    
    bool addFirst(PltListNode_base * p);
    bool addLast(PltListNode_base * p);
    
    PltListNode_base *remove(PltListNode_base *p );
    PltListNode_base *removeFirst();
    PltListNode_base *removeLast();
};
  
////////////////////////////////////////////////////////////////////////
// Class for iteration on PltList_base.
// Not useful for direct use.
class PltListIterator_base {
protected:
    PltListIterator_base(PltList_base &listarg, bool startAtEnd=false); 
    operator const void * () const;

    PltListNode_base *getPtr() const;

    PltListIterator_base & operator ++ ();
    PltListIterator_base & operator -- ();
    void operator ++(int);
    void operator --(int);

    void toStart();
    void toEnd();
private:
    PltListNode_base *curr_elem;
    PltList_base *list;
};

////////////////////////////////////////////////////////////////////////
// Non-intrusive double linked list node with information type T

template <class T>
class PltListNode : public PltListNode_base {
    friend class PltList<T>;
    friend class PltListIterator<T>;
    T info;
public:
    PltListNode(const T& t) : info(t) { }
};

////////////////////////////////////////////////////////////////////////
// Non-intrusive double linked list node with information type T
template <class T>
class PltList : private PltList_base {
    friend class PltListIterator<T>;
public:
    bool isEmpty() const;
    // modifiers
    bool addFirst(const T & t);
    bool addLast(const T & t);
    T removeFirst();
    T removeLast();
};

////////////////////////////////////////////////////////////////////////
// Intrusive double linked list of elements of type T which must be 
// derived from PltListNode_base.
template <class T>
class PltIList : private PltList_base {
    friend class PltIListIterator<T>;
public:
    bool isEmpty() const;
    // modifiers
    bool addFirst(T* p);
    bool addLast(T* p);
    T* remove(T* p);
    T* removeFirst();
    T* removeLast();
};

////////////////////////////////////////////////////////////////////////
// Iterator on PltList<T>
template <class T>
class PltListIterator 
: public PltIterator<T>, 
  private PltListIterator_base {
public:
    PltListIterator(PltList<T> &list, bool startAtEnd=false);
    virtual operator const void * () const;
    virtual const T & operator * () const;
    virtual PltListIterator<T> & operator ++ ();
    virtual void reset();

    T get() const;
#if 0
    T remove();
#endif
};



////////////////////////////////////////////////////////////////////////
// Iterator on PltIList<T>
template <class T>
class PltIListIterator : public PltListIterator_base {
public:
  PltIListIterator(PltIList<T> &list, bool startAtEnd=false );
  T* getPtr() const;
};


//////////////////////////////////////////////////////////////////////
// INLINE IMPLEMENTATION
//////////////////////////////////////////////////////////////////////

inline bool 
PltList_base::isEmpty() const 
{
    return ! first;
}

//////////////////////////////////////////////////////////////////////

inline PltListNode_base *
PltList_base::removeFirst() 
{
    PLT_PRECONDITION( ! isEmpty() );
    return remove(first);
    PLT_CHECK_INVARIANT();
}

//////////////////////////////////////////////////////////////////////

inline PltListNode_base *
PltList_base::removeLast() 
{
    PLT_PRECONDITION( ! isEmpty() );
    return remove(last);
    PLT_CHECK_INVARIANT();
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

inline PltListIterator_base::PltListIterator_base (
    PltList_base &listarg, 
    bool startAtEnd
    ) 
: list( &listarg)
{ 
    curr_elem = startAtEnd?list->last:list->first; 
}

//////////////////////////////////////////////////////////////////////

inline void
PltListIterator_base::toStart()
{
    curr_elem = list->first;
}

//////////////////////////////////////////////////////////////////////

inline void
PltListIterator_base::toEnd()
{
    curr_elem = list->last;
}

//////////////////////////////////////////////////////////////////////

inline PltListIterator_base::operator const void * () const 
{ 
    return curr_elem; 
}

//////////////////////////////////////////////////////////////////////

inline PltListNode_base *
PltListIterator_base::getPtr() const 
{ 
    return curr_elem; 
}

//////////////////////////////////////////////////////////////////////

inline void
PltListIterator_base::operator ++ (int) 
{ 
    PLT_PRECONDITION( curr_elem );
    curr_elem = curr_elem->next; 
}

//////////////////////////////////////////////////////////////////////

inline void
PltListIterator_base::operator -- (int) 
{ 
    PLT_PRECONDITION( curr_elem );
    curr_elem = curr_elem->prev; 
}

//////////////////////////////////////////////////////////////////////

inline PltListIterator_base & 
PltListIterator_base::operator ++ () 
{ 
    PLT_PRECONDITION( curr_elem );
    curr_elem = curr_elem->next; 
    return *this; 
}

//////////////////////////////////////////////////////////////////////

inline PltListIterator_base & 
PltListIterator_base::operator -- () 
{ 
    PLT_PRECONDITION( curr_elem );
    curr_elem = curr_elem->prev; 
    return *this; 
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

template<class T>
inline bool 
PltList<T>::isEmpty() const 
{
    return PltList_base::isEmpty(); 
}

//////////////////////////////////////////////////////////////////////

template<class T>
inline bool 
PltList<T>::addFirst(const T& t) 
{
    PltListNode<T> *p = new PltListNode<T>(t);
    if (p) {
        return PltList_base::addFirst(p); 
    } else {
        return false;
    }
}

//////////////////////////////////////////////////////////////////////

template<class T>
inline bool 
PltList<T>::addLast(const T & t) 
{ 
    PltListNode<T> *p = new PltListNode<T>(t);
    if (p) {
        return PltList_base::addLast(p); 
    } else {
        return false;
    }
}

//////////////////////////////////////////////////////////////////////

template<class T>
inline T 
PltList<T>::removeFirst() 
{
  PltListNode<T> * p = (PltListNode<T> *) PltList_base::removeFirst();
  T t(p->info);
  delete p;
  return t;
}

//////////////////////////////////////////////////////////////////////

template<class T>
inline T 
PltList<T>::removeLast() 
{
  PltListNode<T> * p = (PltListNode<T> *) PltList_base::removeLast();
  T t(p->info);
  delete p;
  return t;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

template<class T>
inline bool
PltIList<T>::isEmpty() const
{ 
    return PltList_base::isEmpty(); 
}

//////////////////////////////////////////////////////////////////////

template<class T>
inline bool 
PltIList<T>::addFirst(T* p) 
{ 
    return PltList_base::addFirst(p); 
}

//////////////////////////////////////////////////////////////////////

template<class T>
inline bool 
PltIList<T>::addLast(T* p) 
{ 
    return PltList_base::addLast(p); 
}

//////////////////////////////////////////////////////////////////////

template<class T>
inline T * 
PltIList<T>::remove(T* p) 
{ 
    return (T*) PltList_base::remove(p); 
}

//////////////////////////////////////////////////////////////////////

template<class T>
inline T *
PltIList<T>::removeFirst() 
{ 
    return (T*) PltList_base::removeFirst(); 
}


//////////////////////////////////////////////////////////////////////
template <class T>
inline T* 
PltIList<T>::removeLast() 
{ 
    return (T*) PltList_base::removeLast(); 
}


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

template <class T>  
inline
PltListIterator<T>::PltListIterator(PltList<T> &list, bool startAtEnd)
: PltListIterator_base(list, startAtEnd) 
{ 
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline
PltListIterator<T>::operator const void *() const
{
    return PltListIterator_base::operator const void * ();
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline T 
PltListIterator<T>::get() const 
{
    return ( (PltListNode<T> *) PltListIterator_base::getPtr() ) -> info;
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline PltIterator<T> &
PltListIterator<T>::operator ++ ()
{
    PltListIterator_base::operator ++();
    return *this;
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline const T &
PltListIterator<T>::operator * ()
{
    return ( (PltListNode<T> *) PltListIterator_base::getPtr() ) -> info;
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline void
PltListIterator<T>::toStart()
{
    PltListIterator_base::toStart();
}

//////////////////////////////////////////////////////////////////////

#if 0
template <class T> 
T 
PltListIterator<T>::remove() 
{
    PltListNode<T> *p = 
        (PltListNode<T> *) list->remove(curr_elem);
    T result(p->info);
    delete p;
    return result;
}
#endif


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
template <class T>
inline
PltIListIterator<T>::PltIListIterator(PltIList<T> &list, bool startAtEnd) 
: PltListIterator_base(list, startAtEnd) 
{ 
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline T * 
PltIListIterator<T>::getPtr() const 
{ 
    return (T*) PltListIterator_base::getPtr(); 
}
#endif  



