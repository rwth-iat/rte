/* -*-plt-c++-*- */
#ifndef PLT_HASHTABLE_INCLUDED
#define PLT_HASHTABLE_INCLUDED
/* $Header: /home/david/cvs/acplt/plt/include/plt/hashtable.h,v 1.21 2007-04-25 12:57:21 martin Exp $ */
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

//////////////////////////////////////////////////////////////////////
// plt/hashtable.h provides a dictionary using hash table implementation
//////////////////////////////////////////////////////////////////////

#include "plt/dictionary.h"
#include "plt/handle.h"

#include <stdlib.h>

//////////////////////////////////////////////////////////////////////
// forward declarations
//////////////////////////////////////////////////////////////////////

class PltHashTable_base;           // "private" implementation class
template <class K, class V> class PltHashTable_; 
                                   // "private" implementation template
class PltHashIterator_base;        // "private" implementation class
template <class K, class V> class PltHashIterator;
//////////////////////////////////////////////////////////////////////
// class PltHashTable<K,V>
//////////////////////////////////////////////////////////////////////
//
// A PltHashTable<K,V> maps from keys of class K to values of 
// class V. It is a PltDictionary<K,V>.
//
// Keys must provide
// the hash function and a comparison function for K.
//
// K::hash and K::operator== must have the following property:
//    k1 == k2 => k1.hash() == k2.hash()
//
// K::hash should be 'almost injective'
//
// Constructor:
// ------------
// The constructor takes 3 optional arguments:
//     mincap: 
//         the minimum capacity of the table, it will never shrink
//         below this limit
//     highwater:
//         when there are more than highwater * capacity elements in
//         the table, the table will grow
//     lowwater:
//         when there are less then lowwater * capacity elements in
//         the table, the table will shrink
//
// Operations:
// -----------
// see PltDictionary<K,V>
//
//////////////////////////////////////////////////////////////////////

#if PLT_RETTYPE_OVERLOADABLE
#define PltHashIterator_THISTYPE(K,V) PltHashIterator<K,V>
#else
#define PltHashIterator_THISTYPE(K,V) PltIterator_< PltAssoc<K,V> >
#endif

//////////////////////////////////////////////////////////////////////
// (PltHashTable_base is a private class)
//////////////////////////////////////////////////////////////////////

class PltHashTable_base 
{
    friend class PltHashIterator_base;
protected:
    static PltAssoc_ * deletedAssoc;
    static bool usedSlot(const PltAssoc_ *p) 
        { return p && p != deletedAssoc; }

    PltHashTable_base(size_t mincap=11, 
                      float highwater=0.8, 
                      float lowwater=0.4);
    virtual ~PltHashTable_base();

#if PLT_DEBUG_INVARIANTS
    virtual bool invariant() const;
#endif
    // accessors
    size_t size() const; // number of nondeleted elements
    PltAssoc_ *lookupAssoc(const void * key) const;

    // modifiers
    bool addAssoc(PltAssoc_ *p);
    PltAssoc_ *removeAssoc(const void * key);

    virtual unsigned long keyHash(const void *) const = 0;
    virtual bool keyEqual(const void *, const void *) const = 0;

    PltAssoc_ **a_table;
    size_t a_capacity;       // current capacity of a_table
    size_t a_minCapacity;    // minimal capacity
    float a_lowwater;        // \  try to keep the capacity between
    float a_highwater;       // /  lowwater * size and highwater * size
    float a_medwater;        // median of high- and lowwater
    size_t a_used;           // current number of used entries;
    size_t a_deleted;        // number of deleted elements

    // accessors
    size_t locate(const void * key) const;
    size_t collidx(size_t i, size_t j) const;

    // modifiers
    bool reset(size_t mincap);
    bool insert(PltAssoc_ *);
    bool changeCapacity(size_t mincap);

    PltHashTable_base(const PltHashTable_base &); // forbidden
    PltHashTable_base & operator = ( const PltHashTable_base & ); // forbidden
};

//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// (PltHashTable_<K,V> are private classes)
//////////////////////////////////////////////////////////////////////

template <class K, class V>
class PltHashTable_
: public PltDictionary<K,V>,
  private PltHashTable_base
{
    friend class PltHashIterator_base;
    friend class PltHashIterator<K,V>;
public:
    PltHashTable_(size_t mincap=11, 
                 float highwater=0.8, 
                 float lowwater=0.4)
        : PltHashTable_base(mincap, highwater, lowwater) { }
    virtual ~PltHashTable_();

    // accessors
    virtual bool query(const K&, V&) const;
    
    // modifiers
    bool reset(size_t mincap = 11);
    virtual bool add(const K&, const V&);

    virtual bool update(const K& key, 
                        const V & newValue, 
                        V & oldValue,
                        bool & oldValueValid);

    virtual bool remove(const K&, V&);

    // container interface
    PltHashIterator_THISTYPE(K,V) * newIterator() const;
    size_t size() const;

protected:
    virtual unsigned long keyHash(const K &) const = 0;
    virtual bool keyEqual(const K & , const K &) const = 0;

private:
    virtual unsigned long keyHash(const void * ) const;
    virtual bool keyEqual(const void *, const void *) const;
    PltHashTable_(const PltHashTable_ &); // forbidden
    PltHashTable_<K,V> & operator = ( const PltHashTable_ & ); // forbidden
};

//////////////////////////////////////////////////////////////////////


template <class K, class V>
class PltHashTable
: public PltHashTable_<K,V>
{
public:
    PltHashTable(size_t mincap=11, 
                 float highwater=0.8, 
                 float lowwater=0.4)
        : PltHashTable_<K,V>(mincap, highwater, lowwater) { }
    virtual ~PltHashTable() {}

protected:
    virtual unsigned long keyHash(const K & ) const ;
    virtual bool keyEqual(const K &, const K &) const;
};
    
//////////////////////////////////////////////////////////////////////

template <class T>
class PltKeyHandle
: public PltPtrHandle<T>
{
public:
    PltKeyHandle();
    PltKeyHandle(T *p, enum PltOwnership);  // no default to avoid conversion!
    PltKeyHandle(const PltPtrHandle<T> &);
    PltKeyHandle(const PltKeyHandle &);

    unsigned long hash() const;
    bool operator == (const PltKeyHandle & h) const;
};
    
//////////////////////////////////////////////////////////////////////

template <class T>
class PltKeyPtr
{
public:
    PltKeyPtr(T *p = 0);
    unsigned long hash() const;
    bool operator == (const PltKeyPtr & p) const;
    T & operator * () const;
    T * operator ->() const;
private:
    T* _p;
};

//////////////////////////////////////////////////////////////////////

template <class T>
class PltKeyCPtr
{
public:
    PltKeyCPtr(const T *p = 0);
    unsigned long hash() const;
    bool operator == (const PltKeyCPtr & p) const;
    const T & operator * () const;
    const T * operator ->() const;
private:
    const T* _p;
};
    
//////////////////////////////////////////////////////////////////////

template <class T>
class PltKeyPlainConstPtr
{
public:
    PltKeyPlainConstPtr(const T *p = 0);
    unsigned long hash() const;
    bool operator == (const PltKeyPlainConstPtr & p) const;
    const T & operator * () const;
    const T * operator ->() const;
private:
    const T* _p;
};
    
//////////////////////////////////////////////////////////////////////

class PltHashIterator_base
{
protected:
    PltHashIterator_base(const PltHashTable_base &);
    bool inRange() const;
    const PltAssoc_ * pCurrent() const;
    void advance();
    void toStart();
private:
    const PltHashTable_base & a_container;
    PltHashIterator_base & operator = (const PltHashIterator_base &);
                         // forbidden
    size_t a_index;
#if PLT_DEBUG_INVARIANTS
public:
    virtual bool invariant() const;
    virtual ~PltHashIterator_base() { };
#endif
};
    

//////////////////////////////////////////////////////////////////////

template <class K, class V>
class PltHashIterator 
: public PltIterator< PltAssoc<K,V> >, 
  private PltHashIterator_base
{
public:
#if PLT_RETTYPE_OVERLOADABLE
    typedef PltHashIterator THISTYPE;
#endif
    PltHashIterator(const PltHashTable_<K,V> & t);
    virtual operator bool () const;                 // termination
    virtual const PltAssoc<K,V> * operator -> () const;     // current element

    virtual PltHashIterator_THISTYPE(K,V) & operator ++ ();  // advance
    virtual void toStart();                                 // from beginning
};

//////////////////////////////////////////////////////////////////////
// IMPLEMENTATION PART -- clients should stop reading here...
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// INLINE IMPLEMENTATION
//////////////////////////////////////////////////////////////////////

inline size_t
PltHashTable_base::size() const
{
    return a_used-a_deleted;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// Microsoft bug
#if 0
template <class K, class V>
inline
PltHashTable_<K,V>::PltHashTable_(size_t mincap, 
                                float highwater, 
                                float lowwater)
: PltHashTable_base(mincap, highwater, lowwater)
{
}
#endif
//////////////////////////////////////////////////////////////////////

template <class K, class V>
inline size_t
PltHashTable_<K,V>::size() const
{
    return PltHashTable_base::size();
}

//////////////////////////////////////////////////////////////////////

template <class K, class V>
inline unsigned long
PltHashTable_<K,V>::keyHash(const void * key) const
{
    return keyHash(* (const K *) key);
}

//////////////////////////////////////////////////////////////////////

template <class K, class V>
inline bool
PltHashTable_<K,V>::keyEqual(const void * k1, const void * k2) const
{
    return keyEqual(* (const K *) k1, * (const K *) k2);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

template <class K, class V>
inline unsigned long
PltHashTable<K,V>::keyHash(const K & key) const
{
    return key.hash();
}

//////////////////////////////////////////////////////////////////////

template <class K, class V>
inline bool
PltHashTable<K,V>::keyEqual(const K & k1, const K & k2) const
{
    return k1 == k2;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
#if 0
template <class K, class V>
inline unsigned long
PltHandleHashTable<K,V>::keyHash(const PltPtrHandle<K> & key) const
{
    return key->hash();
}

//////////////////////////////////////////////////////////////////////

template <class K, class V>
inline bool
PltHandleHashTable<K,V>::keyEqual(const PltPtrHandle<K> & k1, 
                                  const PltPtrHandle<K> & k2) const
{
    return (* k1) == (* k2);
}
#endif
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

template <class T>
inline
PltKeyHandle<T>::PltKeyHandle()
: PltPtrHandle<T>()
{
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline
PltKeyHandle<T>::PltKeyHandle(T *p, enum PltOwnership os)
: PltPtrHandle<T>(p,os)
{
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline
PltKeyHandle<T>::PltKeyHandle(const PltKeyHandle &p)
: PltPtrHandle<T>(p)
{
}


//////////////////////////////////////////////////////////////////////

template <class T>
inline
PltKeyHandle<T>::PltKeyHandle(const PltPtrHandle<T> &p)
: PltPtrHandle<T>(p)
{
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline unsigned long
PltKeyHandle<T>::hash() const
{
    return (*this)->hash();
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline bool
PltKeyHandle<T>::operator == (const PltKeyHandle & rhs) const
{
    return this->operator*() == *rhs;
}

//////////////////////////////////////////////////////////////////////
// (template class PltKeyPtr<T>)
//////////////////////////////////////////////////////////////////////

template <class T>
inline 
PltKeyPtr<T>::PltKeyPtr(T *p)
: _p(p)
{
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline unsigned long
PltKeyPtr<T>::hash() const
{
    return _p->hash();
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline bool 
PltKeyPtr<T>::operator == (const PltKeyPtr & rhs) const
{
    return *_p == *rhs._p;
}
    

//////////////////////////////////////////////////////////////////////

template <class T>
inline T *
PltKeyPtr<T>::operator -> () const
{
    return _p;
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline T &
PltKeyPtr<T>::operator * () const
{
    return *_p;
}

//////////////////////////////////////////////////////////////////////
// (template class PltKeyCPtr<T>)
//////////////////////////////////////////////////////////////////////

template <class T>
inline 
PltKeyCPtr<T>::PltKeyCPtr(const T *p)
: _p(p)
{
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline unsigned long
PltKeyCPtr<T>::hash() const
{
    return _p->hash();
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline bool 
PltKeyCPtr<T>::operator == (const PltKeyCPtr & rhs) const
{
    return *_p == *rhs._p;
}
    

//////////////////////////////////////////////////////////////////////

template <class T>
inline const T *
PltKeyCPtr<T>::operator -> () const
{
    return _p;
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline const T &
PltKeyCPtr<T>::operator * () const
{
    return *_p;
}

//////////////////////////////////////////////////////////////////////
// (template class PltKeyPlainConstPtr<T>)
//////////////////////////////////////////////////////////////////////

template <class T>
inline 
PltKeyPlainConstPtr<T>::PltKeyPlainConstPtr(const T *p)
: _p(p)
{
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline unsigned long
PltKeyPlainConstPtr<T>::hash() const
{
    unsigned long h = (unsigned long) _p;
    h = h ^ ((h >> 24) & 0xFF)
          ^ ((h >> 16) & 0xFF)
          ^ ((h >>  8) & 0xFF);
    return h;
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline bool 
PltKeyPlainConstPtr<T>::operator == (const PltKeyPlainConstPtr & rhs) const
{
    return _p == rhs._p;
}
    

//////////////////////////////////////////////////////////////////////

template <class T>
inline const T *
PltKeyPlainConstPtr<T>::operator -> () const
{
    return _p;
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline const T &
PltKeyPlainConstPtr<T>::operator * () const
{
    return *_p;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

template <class K, class V>
inline
PltHashIterator<K,V>::PltHashIterator(const PltHashTable_<K,V> &t)
: PltHashIterator_base( (const PltHashTable_base &) t)
{
}

//////////////////////////////////////////////////////////////////////

template <class K, class V>
inline
PltHashIterator<K,V>::operator bool () const 
{
    return inRange();
}

//////////////////////////////////////////////////////////////////////

template <class K, class V>
inline const PltAssoc<K,V> *
PltHashIterator<K,V>::operator -> () const
{
    return (const PltAssoc<K,V> *) pCurrent();
}

//////////////////////////////////////////////////////////////////////

template <class K, class V>
inline void
PltHashIterator<K,V>::toStart()
{
    PltHashIterator_base::toStart();
}

//////////////////////////////////////////////////////////////////////

template <class K, class V>
inline PltHashIterator_THISTYPE(K,V) &
PltHashIterator<K,V>::operator ++ ()
{
    advance();
    return *this;
}

//////////////////////////////////////////////////////////////////////

#if PLT_SEE_ALL_TEMPLATES
#include "plt/hashtable_impl.h"
#endif

//////////////////////////////////////////////////////////////////////

#endif // header file
