/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/plt/include/plt/list.h,v 1.2 1997-03-12 16:19:18 martin Exp $ */
/*
 * Copyright (c) 1996, 1997
 * Chair of Process Control Engineering,
 * Aachen University of Technology.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must print or display the above
 *    copyright notice either during startup or must have a means for
 *    the user to view the copyright notice.
 * 3. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 4. Neither the name of the Chair of Process Control Engineering nor the
 *    name of the Aachen University of Technology may be used to endorse or
 *    promote products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE CHAIR OF PROCESS CONTROL ENGINEERING
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE CHAIR OF PROCESS CONTROL
 * ENGINEERING BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/* Author: Martin Kneissl <martin@plt.rwth-aachen.de> */
#ifndef PLT_LIST_INCLUDED
#define PLT_LIST_INCLUDED

#include "plt/container.h"

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
class PltListNode_base;
class PltList_base;
class PltListIterator_base;

template <class T> class PltListNode;
template <class T> class PltIListIterator;
template <class T> class PltListIterator;

////////////////////////////////////////////////////////////////////////
// Non-intrusive double linked list node with information type T
template <class T>
class PltList 
: public PltContainer<T>,
  private PltList_base 
{
      friend class PltListIterator<T>;
public:
    PltList();

    // modifiers
    bool addFirst(const T & t);
    bool addLast(const T & t);
    T removeFirst();
    T removeLast();

    // container interface
    virtual bool isEmpty() const;
    virtual size_t size() const;
    virtual PltListIterator<T> * newIterator() const;
private:
    PltList(const PltList &); // forbidden
    PltList & operator = (const PltList &); // forbidden
};

////////////////////////////////////////////////////////////////////////
// Intrusive double linked list of elements of type T which must be 
// derived from PltListNode_base.
template <class T>
class PltIList 
: public PltContainer<T>,
  private PltList_base 
{
    friend class PltIListIterator<T>;
public:
    PltIList();

    // modifiers
    bool addFirst(T* p);
    bool addLast(T* p);
    T* remove(T* p);
    T* removeFirst();
    T* removeLast();

    // container interface
    virtual bool isEmpty() const;
    virtual size_t size() const;
    virtual PltIListIterator<T> * newIterator() const;
private:
    PltIList(const PltIList &); // forbidden
    PltIList & operator = (const PltIList &); // forbidden
};

////////////////////////////////////////////////////////////////////////
// Iterator on PltList<T>
template <class T>
class PltListIterator 
: public PltBidirIterator<T>, 
  private PltListIterator_base {
public:
    PltListIterator(const PltList<T> &list, bool startAtEnd=false);
    virtual operator const void * () const;
    virtual const T & operator * () const;
    virtual PltListIterator<T> & operator ++ ();
    virtual void toStart();
    virtual PltListIterator<T> & operator -- ();
    virtual void toEnd();
#if 0
    T remove();
#endif
};

////////////////////////////////////////////////////////////////////////
// Iterator on PltIList<T>
template <class T>
class PltIListIterator 
: public PltBidirIterator<T>,
  private PltListIterator_base 
{
public:
    PltIListIterator(const PltIList<T> &list, bool startAtEnd=false );
    virtual operator const void * () const;
    virtual const T* operator -> () const;
    virtual PltIListIterator<T> & operator ++ ();
    virtual void toStart();
    virtual PltIListIterator<T> & operator -- ();
    virtual void toEnd();
};


////////////////////////////////////////////////////////////////////////
// Link node with forward and backward chaining.
class PltListNode_base {
    friend class PltList_base;
    friend class PltListIterator_base;
    PltListNode_base *prev, *next;
public:
    PltListNode_base() : prev(0),next(0) {}
};


//////////////////////////////////////////////////////////////////////
// INLINE IMPLEMENTATION
//////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
// Base class for intrusive lists without information on them.
// Not useful for direct use.

class PltList_base 
{
    friend class PltListIterator_base;
    PltListNode_base *first, *last;

public:
    PltList_base() : first(0), last(0) {}

#if PLT_DEBUG_INVARIANTS
    virtual bool invariant() const;
    virtual ~PltList_base() { }
#endif

    bool isEmpty() const;
    size_t size() const;

    bool addFirst(PltListNode_base * p);
    bool addLast(PltListNode_base * p);
    
    PltListNode_base *remove(PltListNode_base *p );
    PltListNode_base *removeFirst();
    PltListNode_base *removeLast();
private:
    PltList_base(const PltList_base &); // forbidden
    PltList_base & operator = (const PltList_base &); // forbidden
};
  
////////////////////////////////////////////////////////////////////////
// Class for iteration on PltList_base.
// Not useful for direct use.
class PltListIterator_base {
protected:
    PltListIterator_base(const PltList_base &listarg, bool startAtEnd=false); 
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
    const PltList_base  & list;
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
    const PltList_base &listarg, 
    bool startAtEnd
    ) 
: list( listarg)
{ 
    curr_elem = startAtEnd?list.last:list.first; 
}

//////////////////////////////////////////////////////////////////////

inline void
PltListIterator_base::toStart()
{
    curr_elem = list.first;
}

//////////////////////////////////////////////////////////////////////

inline void
PltListIterator_base::toEnd()
{
    curr_elem = list.last;
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
inline
PltList<T>::PltList()
{
}

//////////////////////////////////////////////////////////////////////

template<class T>
inline bool 
PltList<T>::isEmpty() const 
{
    return PltList_base::isEmpty(); 
}

//////////////////////////////////////////////////////////////////////

template<class T>
inline size_t
PltList<T>::size() const
{
    return PltList_base::size();
}
    
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

template<class T>
inline
PltIList<T>::PltIList()
{
}

//////////////////////////////////////////////////////////////////////
template<class T>
inline bool
PltIList<T>::isEmpty() const
{ 
    return PltList_base::isEmpty(); 
}

//////////////////////////////////////////////////////////////////////

template<class T>
inline size_t
PltIList<T>::size() const
{
    return PltList_base::size();
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

template<class T>
inline PltListIterator<T> *
PltList<T>::newIterator() const
{
    return new PltListIterator<T>(*this);
}


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

template <class T>  
inline
PltListIterator<T>::PltListIterator(const PltList<T> &list, bool startAtEnd)
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
inline PltListIterator<T> &
PltListIterator<T>::operator ++ ()
{
    PltListIterator_base::operator ++();
    return *this;
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline PltListIterator<T> &
PltListIterator<T>::operator -- ()
{
    PltListIterator_base::operator --();
    return *this;
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline const T &
PltListIterator<T>::operator * () const
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

template <class T>
inline void
PltListIterator<T>::toEnd()
{
    PltListIterator_base::toEnd();
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
PltIListIterator<T>::PltIListIterator(const PltIList<T> &list, 
                                      bool startAtEnd) 
: PltListIterator_base(list, startAtEnd) 
{ 
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline
PltIListIterator<T>::operator const void *() const
{
    return PltListIterator_base::operator const void * ();
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline PltIListIterator<T> &
PltIListIterator<T>::operator ++ ()
{
    PltListIterator_base::operator ++();
    return *this;
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline PltIListIterator<T> &
PltIListIterator<T>::operator -- ()
{
    PltListIterator_base::operator --();
    return *this;
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline const T *
PltIListIterator<T>::operator -> () const
{
    return ( T *) ( PltListIterator_base::getPtr() );
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline void
PltIListIterator<T>::toStart()
{
    PltListIterator_base::toStart();
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline void
PltIListIterator<T>::toEnd()
{
    PltListIterator_base::toEnd();
}

//////////////////////////////////////////////////////////////////////

#endif  // PLT_LIST_INCLUDED











