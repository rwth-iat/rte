/* -*-plt-c++-*- */
/* 
 *  Copyright (c) 1996 PLT, RWTH-Aachen, Germany. See file copy_plt.txt!
 *  Author: Martin Kneissl <martin@plt.rwth-aachen.de>
 *
 */

#include <plt/debug.h>
#include <plt/hashtable.hpp>


//////////////////////////////////////////////////////////////////////

PltHashAssoc_base
PltHashAssoc_base::deleted_obj;

//////////////////////////////////////////////////////////////////////

PltHashAssoc_base *
PltHashAssoc_base::deleted = &PltHashAssoc_base::deleted_obj;

//////////////////////////////////////////////////////////////////////

bool
PltHashAssoc_base::keyEqual(const PltHashKey_base &) const
{
    return false;
}

//////////////////////////////////////////////////////////////////////

bool
PltHashAssoc_base::keyEqual(const PltHashAssoc_base &) const
{
    return false;
}

//////////////////////////////////////////////////////////////////////

unsigned long
PltHashAssoc_base::keyHash() const
{
    return 0;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

static size_t primes[] = 
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
{
    return sizeof primes / sizeof (primes[0]);
}

//////////////////////////////////////////////////////////////////////

inline size_t
PltHashTable_base::collidx(size_t i, size_t j) const
{
    const size_t k = i+j;
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
        delete a_table;
    }
}

//////////////////////////////////////////////////////////////////////

#if PLT_DEBUG_INVARIANTS
bool
PltHashTable_base::invariant() const
{
    if ( a_capacity < a_minCapacity ) return false;
    if ( a_capacity <= a_used ) return false;
    if ( a_deleted > a_used ) return false;
#if PLT_DEBUG_SLOW
    size_t used = 0;
    size_t deld = 0;
    for (size_t j = 0; j < a_capacity; ++j) {
        if (a_table[j]) {
                ++used;
            if (a_table[j] == PltHashAssoc_base::deleted) {
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
            if (   a_table[k] 
                && a_table[k] != PltHashAssoc_base::deleted
                && a_table[l]
                && a_table[l] != PltHashAssoc_base::deleted
                && a_table[k]->keyEqual(*a_table[l])) {
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
    PltHashAssoc_base ** oldTable = a_table;
    size_t oldCapacity = a_capacity;
    bool result;

    PltHashAssoc_base ** newTable = new PltHashAssoc_base*[newCap];
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
                if (oldTable[i] && oldTable[i] != PltHashAssoc_base::deleted) {
                    bool b = insert(oldTable[i]);
                    PLT_ASSERT(b);
                }
            }
            delete[] oldTable; oldTable = 0;
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
PltHashTable_base::locate(const PltHashKey_base & key) const {
    PLT_PRECONDITION(a_used < a_capacity);
    size_t i = key.hash();
    size_t j = 0;
    for (i = collidx(i,0); ; i = collidx(i,++j)) {
        if ( !a_table[i] ) {
            // unused entry
            break;
        } else if ( a_table[i]->keyEqual(key) ) {
            // key found
            break;
        }
    }
    return i;
}
    
//////////////////////////////////////////////////////////////////////

bool
PltHashTable_base::insert(PltHashAssoc_base *p) 
    // insert without size checking
{
    PLT_PRECONDITION(p && a_used + 1 < a_capacity);

#if PLT_DEBUG_POSTCONDITIONS
    size_t oldSize = size();
#endif

    bool dupe   = false;         // duplicate found?
    size_t deleted = a_capacity; // first matching deleted if any
    size_t ins;                  // insertion point
    size_t i = p->keyHash();
    size_t j = 0;
    for (i = collidx(i,0); !dupe && a_table[i]; i = collidx(i,++j)) {
        if ( a_table[i] == PltHashAssoc_base::deleted ) {
            // deleted entry while inserting
            deleted = i;
            // continue and check for duplicate
        } else if ( a_table[i]->keyEqual(*p) ) {
            // found duplicate
            dupe = true;
        }
    }
    if (!dupe) {
        if (deleted < a_capacity) {
            // use this deleted entry
            ins = deleted;
            PLT_ASSERT(a_table[ins] == PltHashAssoc_base::deleted);
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
PltHashTable_base::addAssoc(PltHashAssoc_base *p) 
    // grow if needed and insert p
{
    PLT_PRECONDITION(p);
    size_t minCap = a_used + 1 + 1; // compensate for assoc to be added
    if (    a_capacity < minCap             // ensure functionality
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

PltHashAssoc_base *
PltHashTable_base::removeAssoc(const PltHashKey_base &key)
{
#if PLT_DEBUG_POSTCONDITIONS
    size_t oldSize = size();
#endif
    size_t i = locate(key);
    PltHashAssoc_base *result;
    
    if ( a_table[i] && a_table[i] != PltHashAssoc_base::deleted ) {
        result = a_table[i];
        a_table[i] = PltHashAssoc_base::deleted;
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

PltHashAssoc_base *
PltHashTable_base::lookupAssoc(const PltHashKey_base &key) const
{
    size_t i = locate(key);
    PltHashAssoc_base *result;

    if ( a_table[i] && a_table[i] != PltHashAssoc_base::deleted ) {
        result = a_table[i];
    } else {
        result = 0;
    }

    return result;
}
    

