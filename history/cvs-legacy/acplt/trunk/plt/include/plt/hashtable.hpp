/* -*-plt-c++-*- */
/* 
 *  Copyright (c) 1996 PLT, RWTH-Aachen, Germany. See file copy_plt.txt!
 *  Author: Martin Kneissl <martin@plt.rwth-aachen.de>
 *
 */

#ifndef PLT_HASHTABLE_INCLUDED
#define PLT_HASHTABLE_INCLUDED

#include <plt/debug.h>
#include <plt/dictionary.hpp>

#include <stdlib.h>

//////////////////////////////////////////////////////////////////////
// (PltHashAssoc_base is a private class)
//////////////////////////////////////////////////////////////////////

class PltHashAssoc_base 
{
    friend class PltHashTable_base;
private:
    static PltHashAssoc_base * deleted; // sentinel for deleted elements
    static PltHashAssoc_base   deleted_obj;
};

//////////////////////////////////////////////////////////////////////
// (PltHashTable_base is a private class)
//////////////////////////////////////////////////////////////////////
// Implementation class for PltHashTable<K,V,H>
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
    PltHashAssoc_base *lookupAssoc(const void *key) const;

    // modifiers
    bool addAssoc(PltHashAssoc_base *p);
    PltHashAssoc_base *removeAssoc(const void *key);

    virtual unsigned long keyHash(const void *key) const = 0;
    virtual unsigned long assocHash(const PltHashAssoc_base *) const = 0;
    virtual bool assocEqualAssoc(const PltHashAssoc_base *, 
                                 const PltHashAssoc_base *) const = 0;
    virtual bool assocEqualKey(const PltHashAssoc_base *, 
                               const void *key) const = 0;

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
    size_t locate(const void *key) const;
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
};

//////////////////////////////////////////////////////////////////////
// class PltHashTable<K,V,H>
//////////////////////////////////////////////////////////////////////
//
// A PltHashTable<K,V,H> maps from keys of class K to values of 
// class V. It is a PltDictionary<K,V>.
//
// Helper
// ------
// H is a helper class which encapsulates the hash function and a
// comparison function for K.
//
// H has to provide the following members:
//    static unsigned long hash(const K & k); 
//    static bool equal(const K & k1, const K & k2);
//
// H::hash and H::equal must have the following property:
//    H::equal(k1,k2) => H::hash(k1) == H::hash(k2)
//
// H::hash should be 'almost injective'
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

template <class K, class V, class H>
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

    // modifiers
    virtual bool add(const K&, const V&);
    virtual bool remove(const K&, V&);

protected:
    virtual unsigned long keyHash(const void *) const;
    virtual unsigned long assocHash(const PltHashAssoc_base *) const;
    virtual bool assocEqualAssoc(const PltHashAssoc_base *, 
                                 const PltHashAssoc_base *) const;
    virtual bool assocEqualKey(const PltHashAssoc_base *, 
                               const void *) const;
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
PltHashAssoc<K,V>::PltHashAssoc(const K& k, const V& v)
: a_key(k), 
  a_value(v)
{
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

template <class K, class V, class H>
inline
PltHashTable<K,V,H>::PltHashTable(size_t mincap, 
                                float highwater, 
                                float lowwater)
: PltHashTable_base(mincap, highwater, lowwater)
{
}

//////////////////////////////////////////////////////////////////////

template <class K, class V, class H>
inline bool
PltHashTable<K,V,H>::query(const K& key, V& value) const
{
    // This cast is safe, only we can put assocs into the table
    PltHashAssoc<K,V> *p = 
        ( PltHashAssoc<K,V> *) lookupAssoc((const void *) &key);
    if (p) {
        value = p->a_value;
        return true;
    } else {
        return false;
    }
}

//////////////////////////////////////////////////////////////////////

template <class K, class V, class H>
inline bool
PltHashTable<K,V,H>::add(const K& key, const V& value)
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

template <class K, class V, class H>
inline bool
PltHashTable<K,V,H>::remove(const K& key, V& value)
{
    // This cast is safe, only we can put assocs into the table
    PltHashAssoc<K,V> *p = 
        (PltHashAssoc<K,V> *) removeAssoc((const void *)&key);
    if (p) {
        value = p->a_value;
        delete p;
        return true;
    } else {
        return false;
    }
}

//////////////////////////////////////////////////////////////////////

template <class K, class V, class H>
inline unsigned long
PltHashTable<K,V,H>::keyHash(const void * p) const
{
    PLT_ASSERT(p);
    // This cast is safe, only we can put assocs into the table
    const K * pk = (const K *) p;
    return H::hash( *pk );
}

//////////////////////////////////////////////////////////////////////

template <class K, class V, class H>
inline unsigned long
PltHashTable<K,V,H>::assocHash(const PltHashAssoc_base * p) const
{
    PLT_ASSERT(p);
    // This cast is safe, only we can put assocs into the table
    const PltHashAssoc<K,V> * pa = (const PltHashAssoc<K,V> *) p;
    return H::hash( pa->a_key );
}

//////////////////////////////////////////////////////////////////////


template <class K, class V, class H>
inline bool
PltHashTable<K,V,H>::assocEqualAssoc(
    const PltHashAssoc_base * p1,
    const PltHashAssoc_base * p2) const
{
    PLT_ASSERT(p1 && p2);
    // These casts are safe, only we can put assocs into the table
    const PltHashAssoc<K,V> * pa1 = (const PltHashAssoc<K,V> *) p1;
    const PltHashAssoc<K,V> * pa2 = (const PltHashAssoc<K,V> *) p2;
    return H::equal( pa1->a_key, pa2->a_key );
}

//////////////////////////////////////////////////////////////////////


template <class K, class V, class H>
inline bool
PltHashTable<K,V,H>::assocEqualKey(
    const PltHashAssoc_base* p1, 
    const void * p2) const
{
    PLT_ASSERT(p1 && p2);
    // These casts are safe, only we can put assocs into the table
    const PltHashAssoc<K,V> * pa = (const PltHashAssoc<K,V> *) p1;
    const K * pk = (const K *) p2;
    return H::equal( pa->a_key, *pk );
}

//////////////////////////////////////////////////////////////////////

#endif // header file
