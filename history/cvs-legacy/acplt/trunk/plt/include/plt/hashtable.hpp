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
#include <plt/dictionary.hpp>
#include <plt/hashkey.hpp>
#include <stdlib.h>

//////////////////////////////////////////////////////////////////////
// forward declarations
//////////////////////////////////////////////////////////////////////

class PltHashAssoc_base;           // "private" implementation class
class PltHashTable_base;           // "private" implementation class

template <class K> class PltIterator;

//////////////////////////////////////////////////////////////////////
// class PltHashTable<K,V>
//////////////////////////////////////////////////////////////////////
//
// A PltHashTable<K,V> maps from keys of class K to values of 
// class V. It is a PltDictionary<K,V>.
//
// Keys must implement the PltHashKey interface which provides
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
  protected PltHashTable_base
{
public:
    PltHashTable(size_t mincap=11, 
                 float highwater=0.8, 
                 float lowwater=0.4);
    // accessors
    virtual bool query(const K&, V&) const;
    virtual PltIterator<K> * newIterator() const;

    // modifiers
    virtual bool add(const K&, const V&);
    virtual bool remove(const K&, V&);
};
    

//////////////////////////////////////////////////////////////////////
// IMPLEMENTATION PART -- clients should stop reading here...
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// (PltHashAssoc_base is a private class)
//////////////////////////////////////////////////////////////////////

class PltHashAssoc_base
{
    friend class PltHashTable_base;
protected:
    virtual const PltHashKey & key() const = 0;
    bool operator == (const PltHashAssoc_base & ) const;
    bool operator == (const PltHashKey & ) const;
    unsigned long hash() const;
private:
    static PltHashAssoc_base * deleted; // sentinel for deleted elements
};

//////////////////////////////////////////////////////////////////////
// (PltHashTable_base is a private class)
//////////////////////////////////////////////////////////////////////
// Implementation class for PltHashTable<K,V>
//////////////////////////////////////////////////////////////////////

class PltHashTable_base : virtual public PltDebuggable
{
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
    PltHashAssoc_base *lookupAssoc(const PltHashKey & key) const;

    // modifiers
    bool addAssoc(PltHashAssoc_base *p);
    PltHashAssoc_base *removeAssoc(const PltHashKey & key);

private:
    PltHashAssoc_base **a_table;
    size_t a_capacity;       // current capacity of a_table
    size_t a_minCapacity;    // minimal capacity
    float a_lowwater;        // \  try to keep the capacity between
    float a_highwater;       // /  lowwater * size and highwater * size
    float a_medwater;        // median of high- and lowwater
    size_t a_used;           // current number of used entries;
    size_t a_deleted;        // number of deleted elements

    // accessors
    size_t locate(const PltHashKey & key) const;
    size_t collidx(size_t i, size_t j) const;
 
    // modifiers
    bool insert(PltHashAssoc_base *);
    bool changeCapacity(size_t mincap);
};

//////////////////////////////////////////////////////////////////////
// (PltHashAssoc<K,V> are private classes)
//////////////////////////////////////////////////////////////////////

template <class K, class V>
class PltHashAssoc : public PltHashAssoc_base
{
public:
    PltHashAssoc(const K&, const V &);
    K a_key;
    V a_value;
protected:
    virtual const PltHashKey & key() const;
};

//////////////////////////////////////////////////////////////////////
// INLINE IMPLEMENTATION
//////////////////////////////////////////////////////////////////////

template <class K, class V>
inline const PltHashKey &
PltHashAssoc<K,V>::key() const
{
    return a_key;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

inline unsigned long
PltHashAssoc_base::hash() const 
{
    return key().hash();
}

//////////////////////////////////////////////////////////////////////

inline bool
PltHashAssoc_base::operator == (const PltHashAssoc_base & arg) const
{
    return key() == arg.key();
}

//////////////////////////////////////////////////////////////////////

inline bool
PltHashAssoc_base::operator == (const PltHashKey & arg) const
{
    return key() == arg;
}


//////////////////////////////////////////////////////////////////////
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
PltHashAssoc<K,V>::PltHashAssoc(const K& k, const V& v)
: a_key(k), 
  a_value(v)
{
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
    PltHashAssoc<K,V> *p = 
        ( PltHashAssoc<K,V> *) lookupAssoc(key);
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
    PltHashAssoc<K,V> *p = new PltHashAssoc<K,V>(key,value);
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
    PltHashAssoc<K,V> *p = 
        (PltHashAssoc<K,V> *) removeAssoc(key);
    if (p) {
        value = p->a_value;
        delete p;
        return true;
    } else {
        return false;
    }
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

template <class K>
class PltHashTableIterator : public PltIterator<K>
{
    virtual operator void * () const ;
    virtual const K & operator *() const;
    virtual PltIterator<K> & operator ++;
private:
    PltHashTable_base & a_hashtable;
    size_t a_index;
};


//////////////////////////////////////////////////////////////////////

inline
PltHashTableIterator<K>::operator void *() const
{
    return a_index < a_hashtable.a_capacity;
}


//////////////////////////////////////////////////////////////////////

inline
PltHashTableIterator<K>::operator *


#endif // header file
