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

class PltHashKey_base 
{	
public:
    virtual bool operator == (const PltHashKey_base &) const = 0;
    virtual unsigned long hash() const = 0;
};

//////////////////////////////////////////////////////////////////////

class PltHashAssoc_base 
{
    friend class PltHashTable_base;
public:
    virtual bool keyEqual (const PltHashKey_base &) const;
    virtual bool keyEqual (const PltHashAssoc_base &) const;
    virtual unsigned long keyHash() const;
private:
    static PltHashAssoc_base   deleted_obj;
    static PltHashAssoc_base * deleted;
};

//////////////////////////////////////////////////////////////////////

class PltHashTable_base : virtual public PltDebuggable
{
public:
    PltHashTable_base(size_t mincap=11, 
                      float highwater=0.8, 
                      float lowwater=0.4);
    virtual ~PltHashTable_base();

#if PLT_DEBUG_INVARIANTS
    bool invariant() const;
#endif
    // accessors
    size_t size() const; // number of nondeleted elements
    PltHashAssoc_base *lookupAssoc(const PltHashKey_base &) const;

    // modifiers
    bool addAssoc(PltHashAssoc_base *p);
    PltHashAssoc_base *removeAssoc(const PltHashKey_base &);

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
    size_t locate(const PltHashKey_base &key) const;
    size_t collidx(size_t i, size_t j) const;
 
    // modifiers
    bool insert(PltHashAssoc_base *);
    bool changeCapacity(size_t mincap);
};

//////////////////////////////////////////////////////////////////////

template <class K>
class PltHashKey : public PltHashKey_base 
{
public:
    PltHashKey(const K &);
    virtual unsigned long hash() const;
    virtual bool operator == (const PltHashKey_base &) const; 
protected:
    K a_key;
};


//////////////////////////////////////////////////////////////////////

template <class K, class V> class PltHashTable;

//////////////////////////////////////////////////////////////////////

template <class K, class V>
class PltHashAssoc : public PltHashAssoc_base
{
    friend class PltHashTable<K,V>;
private:
    PltHashAssoc(const K&, const V &);
    virtual bool keyEqual (const PltHashKey_base &) const;
    virtual bool keyEqual (const PltHashAssoc_base &) const;
    virtual unsigned long keyHash() const;
protected:
    K a_key;
    V a_value;
};

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

    // modifiers
    virtual bool add(const K&, const V&);
    virtual bool remove(const K&, V&);
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

template <class T>
inline
PltHashKey<T>::PltHashKey(const T & t)
: a_key(t)
{
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline unsigned long
PltHashKey<T>::hash() const
{
    return 0;
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline bool
PltHashKey<T>::operator==(const PltHashKey_base &k) const {
    T q = ((const PltHashKey<T> &)k).a_key; // TODO
    return q == a_key;
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

template <class K, class V>
inline bool
PltHashAssoc<K,V>::keyEqual (const PltHashKey_base & key) const
{
    return a_key == key;
}

//////////////////////////////////////////////////////////////////////

template <class K, class V>
inline bool
PltHashAssoc<K,V>::keyEqual (const PltHashAssoc_base & assoc) const
{
    return a_key == ((const PltHashAssoc<K,V> &) assoc).a_key; // TODO
}

//////////////////////////////////////////////////////////////////////

template <class K, class V>
inline unsigned long
PltHashAssoc<K,V>::keyHash() const
{
    return a_key.hash();
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
    PltHashAssoc<K,V> *p = ( PltHashAssoc<K,V> *) lookupAssoc(key);
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
    PltHashAssoc<K,V> *p = (PltHashAssoc<K,V> *) removeAssoc(key);
    if (p) {
        value = p->a_value;
        delete p;
        return true;
    } else {
        return false;
    }
}

//////////////////////////////////////////////////////////////////////

#endif
