/* -*-plt-c++-*- */
/* 
 *  Copyright (c) 1996 PLT, RWTH-Aachen, Germany. See file copy_plt.txt!
 *  Author: Martin Kneissl <martin@plt.rwth-aachen.de>
 *
 */

#ifndef PLT_HASHTABLE_INCLUDED
#define PLT_HASHTABLE_INCLUDED

#include <plt/debug.h>
#include <plt/rtti.hpp>
#include <plt/key.hpp>
#include <plt/assoc.hpp>
#include <plt/dictionary.hpp>
#include <plt/iterator.hpp>
#include <stdlib.h>

//////////////////////////////////////////////////////////////////////
// forward declarations
//////////////////////////////////////////////////////////////////////

class PltHashTable_base;           // "private" implementation class
class PltHashIterator_base;        // "private" implementation class

//////////////////////////////////////////////////////////////////////
// class PltHashTable<K,V>
//////////////////////////////////////////////////////////////////////
//
// A PltHashTable<K,V> maps from keys of class K to values of 
// class V. It is a PltDictionary<K,V>.
//
// Keys must implement the PltKey interface which provides
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

template <class K, class V>
class PltHashTable
: public PltDictionary<K,V>,
  private PltHashTable_base
{
    friend PltHashIterator_base::PltHashIterator_base
        (const PltHashTable_base &);

public:
    PltHashTable(size_t mincap=11, 
                 float highwater=0.8, 
                 float lowwater=0.4);
    // accessors
    virtual bool query(const K&, V&) const;
    
    PltIterator< PltAssoc<K,V> > * newIterator() const;

    // modifiers
    virtual bool add(const K&, const V&);
    virtual bool remove(const K&, V&);
};
    

    
//////////////////////////////////////////////////////////////////////

template <class K, class V>
class PltHashIterator 
: public PltIterator< PltAssoc<K,V> >, 
  private PltHashIterator_base
{
public:
    PltHashIterator(const PltHashTable<K,V> & t);
    virtual operator const void * () const;                 // termination
    virtual const PltAssoc<K,V> * operator -> () const;     // current element

    virtual PltIterator< PltAssoc<K,V> > & operator ++ ();  // advance
    virtual void toStart();                                 // from beginning
};

//////////////////////////////////////////////////////////////////////
// IMPLEMENTATION PART -- clients should stop reading here...
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// (PltHashTable_base is a private class)
//////////////////////////////////////////////////////////////////////
// Implementation class for PltHashTable<K,V>
//////////////////////////////////////////////////////////////////////

class PltHashTable_base : virtual public PltDebuggable
{
    friend class PltHashIterator_base;
protected:
    PltHashTable_base(size_t mincap=11, 
                      float highwater=0.8, 
                      float lowwater=0.4);
    virtual ~PltHashTable_base();

#if PLT_DEBUG_INVARIANTS
    bool invariant() const;
#endif
    // accessors
    size_t size() const; // number of nondeleted elements
    PltAssoc_ *lookupAssoc(const PltKey & key) const;

    // modifiers
    bool addAssoc(PltAssoc_ *p);
    PltAssoc_ *removeAssoc(const PltKey & key);

private:
    PltAssoc_ **a_table;
    size_t a_capacity;       // current capacity of a_table
    size_t a_minCapacity;    // minimal capacity
    float a_lowwater;        // \  try to keep the capacity between
    float a_highwater;       // /  lowwater * size and highwater * size
    float a_medwater;        // median of high- and lowwater
    size_t a_used;           // current number of used entries;
    size_t a_deleted;        // number of deleted elements

    // accessors
    size_t locate(const PltKey & key) const;
    size_t collidx(size_t i, size_t j) const;
    static bool usedSlot(const PltAssoc_ *);

    // modifiers
    bool insert(PltAssoc_ *);
    bool changeCapacity(size_t mincap);
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
    size_t a_index;
#if PLT_DEBUG_INVARIANTS
public:
    bool invariant() const;
#endif
};
    

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

template <class K, class V>
inline
PltHashTable<K,V>::PltHashTable(size_t mincap, 
                                float highwater, 
                                float lowwater)
: PltHashTable_base(mincap, highwater, lowwater)
{
}

//////////////////////////////////////////////////////////////////////

template <class K, class V>
inline bool
PltHashTable<K,V>::query(const K& key, V& value) const
{
    // This cast is safe, only we can put assocs into the table
    PltAssoc<K,V> *p = 
        ( PltAssoc<K,V> *) lookupAssoc(key);
    if (p) {
        value = p->a_value;
        return true;
    } else {
        return false;
    }
}

//////////////////////////////////////////////////////////////////////

template <class K, class V>
inline bool
PltHashTable<K,V>::add(const K& key, const V& value)
{
    PltAssoc<K,V> *p = new PltAssoc<K,V>(key,value);
    if (p) {
        if ( addAssoc(p) ) {
            return true;
        } else {
            delete p;
            return false;
        }
    } else {
        return false;
    }
}
    
//////////////////////////////////////////////////////////////////////

template <class K, class V>
inline bool
PltHashTable<K,V>::remove(const K& key, V& value)
{
    // This cast is safe, only we can put assocs into the table
    PltAssoc<K,V> *p = 
        (PltAssoc<K,V> *) removeAssoc(key);
    if (p) {
        value = p->a_value;
        delete p;
        return true;
    } else {
        return false;
    }
}

//////////////////////////////////////////////////////////////////////

template <class K, class V>
inline PltIterator< PltAssoc<K,V> > * 
PltHashTable<K,V>::newIterator() const
{
    return new PltHashIterator<K,V>(*this);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

template <class K, class V>
inline
PltHashIterator<K,V>::PltHashIterator(const PltHashTable<K,V> &t)
: PltHashIterator_base( (const PltHashTable_base &) t)
{
}

//////////////////////////////////////////////////////////////////////

template <class K, class V>
inline
PltHashIterator<K,V>::operator const void * () const 
{
    return inRange() ? this : 0;
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
inline PltIterator< PltAssoc<K,V> > &
PltHashIterator<K,V>::operator ++ ()
{
    advance();
    return *this;
}

//////////////////////////////////////////////////////////////////////


#endif // header file
