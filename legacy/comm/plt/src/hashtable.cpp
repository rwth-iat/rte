/* -*-plt-c++-*- */
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

#include "plt/hashtable.h"

//////////////////////////////////////////////////////////////////////

class PltDeletedHashAssoc : public PltAssoc_
{
    virtual const void * key() const { return 0; }
};

//////////////////////////////////////////////////////////////////////
// sentinel

static PltDeletedHashAssoc deleted_obj;

PltAssoc_ *
PltHashTable_base::deletedAssoc = &deleted_obj;

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

static size_t primes[] = 
    // The hash table must have prime size. Here are some primes.
{ 
    7, 11, 13, 17, 19, 23, 29,
    31, 41, 47, 59, 67, 79, 97, 127, 137, 167, 197, 239, 293, 347, 419,
    499, 593, 709, 853, 1021, 1229, 1471, 1777, 2129, 2539, 3049, 3659,
    4391, 5273, 6323, 7583, 9103, 10937, 13109, 15727, 18899, 22651,
    27179, 32609, 39133, 46957, 56359, 67619, 81157, 97369, 116849,
    140221, 168253, 201907, 242309, 290761, 348889, 418667, 502409,
    602887, 723467, 868151, 1041779, 1250141 , 1500181, 1800191, 2160233,
    2592277, 3110741, 3732887, 4479463, 5375371 , 6450413, 7740517,
    9288589, 11146307, 13375573, 16050689, 19260817, 23112977, 27735583,
    33282701, 39939233, 47927081, 57512503, 69014987, 82818011, 99381577,
    119257883, 143109469, 171731387, 206077643, 247293161, 296751781,
    356102141, 427322587, 512787097, 615344489, 738413383, 886096061,
    1063315271, 1275978331, 1531174013, 1837408799
};

static inline size_t cprimes() 
    // safe 'macro' for the count of primes
{
    return sizeof primes / sizeof (primes[0]);
}

//////////////////////////////////////////////////////////////////////

inline size_t
PltHashTable_base::collidx(size_t i, size_t j) const
    // collision resolving algorithm
    // A few steps of quadratic probe followed by linear steps.
{
    const size_t k = (j < 12) ? i+j : i+1;
    return k % a_capacity;
}


//////////////////////////////////////////////////////////////////////

PltHashTable_base::PltHashTable_base(size_t mincap,
                                     float highwater, 
                                     float lowwater)
: a_table(0),
  a_capacity(0),
  a_minCapacity(mincap),
  a_lowwater(lowwater),
  a_highwater(highwater),
  a_medwater((highwater+lowwater)/2),
  a_used(0),
  a_deleted(0)
{
    PLT_PRECONDITION( mincap > 0
        && 0.0 < highwater && highwater < 1.0
        && 0.0 < lowwater  && lowwater  < 1.0
        && lowwater < highwater);
    changeCapacity(mincap);
    PLT_CHECK_INVARIANT();
}

//////////////////////////////////////////////////////////////////////

PltHashTable_base::~PltHashTable_base()
{
    if (a_table) {
        delete [] a_table;
    }
}

/////////////////////////////////////////////////////////////////////////////

bool
PltHashTable_base::reset(size_t mincap)
{
    PLT_PRECONDITION( mincap > 0 );

    if (a_table) {
        delete [] a_table;
        a_table = 0;
    }

    a_capacity = 0;
    a_minCapacity = mincap;
    a_used = 0;
    a_deleted = 0;

    bool ok = changeCapacity(mincap);

    PLT_CHECK_INVARIANT();

    return ok;
}

//////////////////////////////////////////////////////////////////////

#if PLT_DEBUG_INVARIANTS
bool
PltHashTable_base::invariant() const
{
    if ( a_capacity < a_minCapacity ) return false;
    if ( a_capacity <= a_used ) return false;
    if ( a_deleted > a_used ) return false;

#if PLT_DEBUG_PEDANTIC
    size_t used = 0;
    size_t deld = 0;
    for (size_t j = 0; j < a_capacity; ++j) {
        if (a_table[j]) {
                ++used;
            if (a_table[j] == deletedAssoc) {
                ++deld;
            }
        }
    }
    if ( used != a_used ) return false;
    if ( deld != a_deleted ) return false;

    // prime size
    bool capIsPrime = false;
    for (size_t i = 0; ! capIsPrime && i < cprimes(); ++i) {
        if (primes[i] == a_capacity) {
            capIsPrime = true;
        }
    }
    if (! capIsPrime ) return false;
    
    // no duplicates
    bool dupes = false;
    for (size_t k = 0; !dupes && k < a_capacity; ++k) {
        for (size_t l = k + 1; !dupes && l < a_capacity; ++l) {
            if (   usedSlot(a_table[k])
                && usedSlot(a_table[l])
                && keyEqual(a_table[k]->key(), a_table[l]->key() )  ) {
                dupes = true;
            }
        }
    }
    if (dupes) return false;
#endif

    return true;
}
#endif

//////////////////////////////////////////////////////////////////////

bool PltHashTable_base::changeCapacity(size_t cap)
    // Growing and shrinking
{

#if PLT_DEBUG_POSTCONDITIONS
    size_t oldSize = size();
#endif

    // determine new capacity, which must be prime
    size_t minCap = (cap < a_minCapacity) ? a_minCapacity : cap;
    size_t k;
    for (k = 0; 
         k < cprimes() && primes[k] < minCap; 
         ++k) {
    }
    if (k>=cprimes()) {
        return false;
    }
    size_t newCap = primes[k];

    PLT_ASSERT(newCap >= cap 
           && newCap >= a_minCapacity 
           && newCap == primes[k]
           && newCap > size() );
    
    if (newCap == a_capacity) {
        // perhaps a shrink that doesn't really shrink
        return true;
    }

    // rehash
    PltAssoc_ ** oldTable = a_table;
    size_t oldCapacity = a_capacity;
    bool result;

    PltAssoc_ ** newTable = new PltAssoc_*[newCap];
    if (newTable) {
        a_table = newTable; 
        a_used = 0; 
        a_deleted = 0;
        a_capacity = newCap;
        
        size_t i;
        for (i = 0; i<a_capacity; ++i) {
            a_table[i] = 0;
        }
        if (oldTable) {
            // insert entries from old table into new table
            for (i = 0; i<oldCapacity; ++i) {
                if ( usedSlot(oldTable[i]) ) {
#if PLT_DEBUG
                    bool b = 
#endif
                        insert(oldTable[i]);
                    PLT_ASSERT(b);
                }
            }
            delete[] oldTable;
        }
        result = true;
    } else {
        result = false;
    }
    PLT_CHECK_INVARIANT();
    PLT_POSTCONDITION(size()==oldSize && (!result || (a_capacity >= minCap)));
    return result;
}

//////////////////////////////////////////////////////////////////////

size_t
PltHashTable_base::locate(const void * key) const 
    // find the index of a key, return a_capacity if not found
{
    PLT_PRECONDITION(a_used < a_capacity);
    size_t loc = a_capacity;
    size_t i = keyHash(key);
    size_t j = 1;

    for (i = collidx(i, 0); 
         loc >= a_capacity && a_table[i]; 
         i = collidx(i,j), j+=2) 
        {
            if (   a_table[i] != deletedAssoc
                && keyEqual( a_table[i]->key(), key ) ) {
                // key found
                loc = i;
            }
        }
    return loc;
}
    
//////////////////////////////////////////////////////////////////////

bool
PltHashTable_base::insert(PltAssoc_ * p) 
    // insert without size checking
{
    PLT_PRECONDITION(a_used + 1 < a_capacity);

#if PLT_DEBUG_POSTCONDITIONS
    size_t oldSize = size();
#endif

    bool dupe   = false;         // duplicate found?
    size_t deleted = a_capacity; // first matching deleted if any
    size_t ins;                  // insertion point
    size_t i = keyHash(p->key());
    size_t j = 1;

    for (i = collidx(i,0); 
         !dupe && a_table[i]; 
         i = collidx(i,j), j+=2) {
        if ( a_table[i] == deletedAssoc ) {
            // deleted entry while inserting
            deleted = i;
            // continue and check for duplicate
        } else if ( keyEqual(a_table[i]->key(), p->key()) ) {
            // found duplicate
            dupe = true;
        }
    }
    if (!dupe) {
        if (deleted < a_capacity) {
            // use this deleted entry
            ins = deleted;
            PLT_ASSERT(a_table[ins] == deletedAssoc);
            // not a deleted entry any more
            --a_deleted;
        } else {
            ins = i;
            PLT_ASSERT(a_table[ins] == 0);
            // one more used entry
            ++a_used;
        }
        a_table[ins] = p;
    }
    PLT_CHECK_INVARIANT();
    PLT_POSTCONDITION(dupe || (size() == oldSize + 1));
    return !dupe;
}

//////////////////////////////////////////////////////////////////////

bool 
PltHashTable_base::addAssoc(PltAssoc_ *p) 
    // grow if needed and insert p
{
    PLT_PRECONDITION(p);
    size_t minCap = a_used + 1 + 1; // compensate for assoc to be added
    if (    a_capacity < minCap     // ensure functionality
        ||  size() + 1 > a_capacity * a_highwater) {
        // Grow.
        size_t tmp = size_t(float(size()) / a_medwater);
        size_t prefCap = tmp < minCap ? minCap : tmp;
        if ( !changeCapacity(prefCap) ) {
            // failed
            return false;
        }
    }
    PLT_CHECK_INVARIANT();
    return insert(p);
}

//////////////////////////////////////////////////////////////////////

PltAssoc_ *
PltHashTable_base::removeAssoc(const void * key)
    // remove an assoc with key key and return it. return 0 if not found
{
#if PLT_DEBUG_POSTCONDITIONS
    size_t oldSize = size();
#endif
    size_t i = locate(key);
    PltAssoc_ *result;
    
    if ( i < a_capacity ) {
        // found
        result = a_table[i];
        a_table[i] = deletedAssoc;
        ++a_deleted;
        PLT_CHECK_INVARIANT();
        if ( size() < a_capacity * a_lowwater ) {
            // shrink
            size_t minCap = size_t(float(size()) / a_medwater);
            changeCapacity(minCap); // failure not fatal
        }
    } else {
        result = 0;
    }
    PLT_POSTCONDITION(!result ||  size() == oldSize - 1  );
    PLT_CHECK_INVARIANT();
    return result;
}

//////////////////////////////////////////////////////////////////////

PltAssoc_ *
PltHashTable_base::lookupAssoc(const void * key) const
    // return an assoc with key key or 0 if not found
{
    size_t i = locate(key);
    PltAssoc_ *result;

    if (i < a_capacity) {
        // found
        result = a_table[i];
    } else {
        result = 0;
    }

    return result;
}


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

bool
PltHashIterator_base::inRange() const
{
    return a_index < a_container.a_capacity;
}
    
//////////////////////////////////////////////////////////////////////

PltHashIterator_base::PltHashIterator_base(const PltHashTable_base & t)
: a_container(t), a_index(0)
{
    toStart();
}


//////////////////////////////////////////////////////////////////////
#if PLT_DEBUG_INVARIANTS

bool
PltHashIterator_base::invariant() const
{
    return !inRange() 
        || PltHashTable_base::usedSlot(a_container.a_table[a_index]);
}

#endif
//////////////////////////////////////////////////////////////////////

void 
PltHashIterator_base::advance()
{
    PLT_PRECONDITION(inRange());

    do {
        ++ a_index;
    } while (inRange() && 
             !PltHashTable_base::usedSlot(a_container.a_table[a_index]) );

    PLT_CHECK_INVARIANT();
}

//////////////////////////////////////////////////////////////////////

void 
PltHashIterator_base::toStart()
{
    a_index=0;
    if (   inRange() 
        && !PltHashTable_base::usedSlot(a_container.a_table[a_index]) ) {
        advance();
    }
    PLT_CHECK_INVARIANT();
}

//////////////////////////////////////////////////////////////////////

const PltAssoc_ *
PltHashIterator_base::pCurrent() const
{
    PLT_PRECONDITION( inRange() );
    return a_container.a_table[a_index];
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

#if !PLT_INSTANTIATE_TEMPLATES
#include "plt/hashtable_impl.h"
#endif


//////////////////////////////////////////////////////////////////////
// EOF plt/hashtable.cpp
//////////////////////////////////////////////////////////////////////

