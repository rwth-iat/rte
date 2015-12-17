/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/plt/include/plt/list.h,v 1.14 2007-04-25 12:57:21 martin Exp $ */
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

#if PLT_RETTYPE_OVERLOADABLE
#define PltListIterator_THISTYPE(T) PltListIterator<T>
#define PltIListIterator_THISTYPE(T) PltIListIterator<T>
#else
#define PltListIterator_THISTYPE(T) PltIterator_<T>
#define PltIListIterator_THISTYPE(T) PltIterator_<T>
#endif

#if PLT_COMPILER_DECCXX
#define PltListIterator_CLASSTHISTYPE(T) PltListIterator<T>::THISTYPE
#define PltIListIterator_CLASSTHISTYPE(T) PltIListIterator<T>::THISTYPE
#else
#define PltListIterator_CLASSTHISTYPE(T) PltListIterator_THISTYPE(T) 
#define PltIListIterator_CLASSTHISTYPE(T) PltIListIterator_THISTYPE(T) 
#endif

////////////////////////////////////////////////////////////////////////
// Base class for intrusive lists without information on them.
// Not useful for direct use.

class PltList_base 
{
    friend class PltListIterator_base;
protected:
    PltListNode_base *first, *last;

public:
    PltList_base() : first(0), last(0) {}

#if PLT_DEBUG_INVARIANTS
    virtual bool invariant() const;
#endif
    virtual ~PltList_base();

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
    operator bool () const;

    PltListNode_base *getPtr() const;

    void stepForward();
    void stepBackward();

    void toStart();
    void toEnd();
private:
    PltListNode_base *curr_elem;
    const PltList_base  & list;
};

////////////////////////////////////////////////////////////////////////
// Non-intrusive double linked list node with information type T
template <class T>
class PltList 
: public PltContainer<T>,
  private PltList_base 
{
      friend class PltListIterator<T>;
public:
    PltList() { }
    ~PltList();

    // modifiers
    bool addFirst(const T & t);
    bool addLast(const T & t);
    T removeFirst();
    T removeLast();

    bool remove(const T & t);

    // container interface
    virtual bool isEmpty() const;
    virtual size_t size() const;
    virtual PltListIterator_THISTYPE(T) * newIterator() const;
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
    ~PltIList();
    // modifiers
    bool addFirst(T* p);
    bool addLast(T* p);
    T* remove(T* p);
    T* removeFirst();
    T* removeLast();

    // container interface
    virtual bool isEmpty() const;
    virtual size_t size() const;
    virtual PltIListIterator_THISTYPE(T) * newIterator() const;
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
#if PLT_RETTYPE_OVERLOADABLE
    typedef PltListIterator<T> THISTYPE;
#endif
    PltListIterator(const PltList<T> &list, bool startAtEnd=false);
    virtual operator bool () const;
    virtual const T & operator * () const;
    virtual THISTYPE & operator ++ ();
    virtual void toStart();
    virtual THISTYPE & operator -- ();
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
#if PLT_RETTYPE_OVERLOADABLE
    typedef PltIListIterator<T> THISTYPE;
#endif
    PltIListIterator(const PltIList<T> &list, bool startAtEnd=false );
    virtual operator bool () const;
    virtual const T* operator -> () const;
    virtual THISTYPE & operator ++ ();
    virtual void toStart();
    virtual THISTYPE & operator -- ();
    virtual void toEnd();
};


////////////////////////////////////////////////////////////////////////
// Link node with forward and backward chaining.
class PltListNode_base {
    friend class PltList_base;
    friend class PltListIterator_base;
    PltListNode_base *prev, *next;
protected:
    PltListNode_base *getPrev();
    PltListNode_base *getNext();
public:
    PltListNode_base() : prev(0),next(0) {}
};


//////////////////////////////////////////////////////////////////////
// INLINE IMPLEMENTATION
//////////////////////////////////////////////////////////////////////

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

inline
PltListNode_base *
PltListNode_base::getPrev() 
{
    return prev;
}

//////////////////////////////////////////////////////////////////////

inline
PltListNode_base *
PltListNode_base::getNext() 
{
    return next;
}

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
    PltListNode_base *res=remove(first);
    PLT_CHECK_INVARIANT();
    return res;
}

//////////////////////////////////////////////////////////////////////

inline PltListNode_base *
PltList_base::removeLast() 
{
    PLT_PRECONDITION( ! isEmpty() );
    PltListNode_base *res=remove(last);
    PLT_CHECK_INVARIANT();
    return res;
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

inline PltListIterator_base::operator bool () const 
{ 
    return curr_elem != 0; 
}

//////////////////////////////////////////////////////////////////////

inline PltListNode_base *
PltListIterator_base::getPtr() const 
{ 
    return curr_elem; 
}

//////////////////////////////////////////////////////////////////////

inline void
PltListIterator_base::stepForward() 
{ 
    PLT_PRECONDITION( curr_elem );
    curr_elem = curr_elem->next; 
}

//////////////////////////////////////////////////////////////////////

inline void
PltListIterator_base::stepBackward() 
{ 
    PLT_PRECONDITION( curr_elem );
    curr_elem = curr_elem->prev; 
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// Microsoft bug
#if 0
template<class T>
inline
PltList<T>::PltList()
{
}
#endif

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
inline PltListIterator_THISTYPE(T) *
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
PltListIterator<T>::operator bool() const
{
    return PltListIterator_base::operator bool ();
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline PltListIterator_CLASSTHISTYPE(T) &
PltListIterator<T>::operator ++ ()
{
    PltListIterator_base::stepForward();
    return *this;
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline PltListIterator_CLASSTHISTYPE(T) &
PltListIterator<T>::operator -- ()
{
    PltListIterator_base::stepBackward();
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
PltIListIterator<T>::operator bool() const
{
    return PltListIterator_base::operator bool ();
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline PltIListIterator_CLASSTHISTYPE(T) &
PltIListIterator<T>::operator ++ ()
{
    PltListIterator_base::stepForward();
    return *this;
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline PltIListIterator_CLASSTHISTYPE(T) &
PltIListIterator<T>::operator -- ()
{
    PltListIterator_base::stepBackward();
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

#if PLT_SEE_ALL_TEMPLATES
#include "plt/list_impl.h"
#endif

//////////////////////////////////////////////////////////////////////

#endif  // PLT_LIST_INCLUDED











