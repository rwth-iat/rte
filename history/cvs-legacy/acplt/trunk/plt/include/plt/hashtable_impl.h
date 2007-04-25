/* -*-plt-c++-*- */
#ifndef PLT_HASHTABLE_IMPL_INCLUDED
#define PLT_HASHTABLE_IMPL_INCLUDED

/* $Header: /home/david/cvs/acplt/plt/include/plt/hashtable_impl.h,v 1.13 2007-04-25 12:57:21 martin Exp $ */
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

#if !PLT_SEE_ALL_TEMPLATES
#include "plt/hashtable.h"
#include "plt/container_impl.h"
#endif

//////////////////////////////////////////////////////////////////////

template <class K, class V>
PltHashTable_<K,V>::~PltHashTable_()
{
    for (size_t i = 0; i<a_capacity; ++i) {
        if ( usedSlot(a_table[i]) ) {
                delete (PltAssoc<K,V> *) a_table[i];
        }
    }
}

/////////////////////////////////////////////////////////////////////////////

template <class K, class V>
bool
PltHashTable_<K,V>::reset(size_t mincap) 
{        
    for (size_t i = 0; i < a_capacity; ++i) {
        if ( usedSlot(a_table[i]) ) {
                delete (PltAssoc<K,V> *) a_table[i];
        }
    }

    return PltHashTable_base::reset(mincap);
}


/////////////////////////////////////////////////////////////////////////////

template <class K, class V>
bool
PltHashTable_<K,V>::query(const K& key, V& value) const
{
    // This cast is safe, only we can put assocs into the table
    PltAssoc<K,V> *p = 
        ( PltAssoc<K,V> *) lookupAssoc(&key);
    if (p) {
        value = p->a_value;
        return true;
    } else {
        return false;
    }
}

//////////////////////////////////////////////////////////////////////

template <class K, class V>
bool
PltHashTable_<K,V>::add(const K& key, const V& value)
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
bool
PltHashTable_<K,V>::update(const K& key, 
                           const V & newValue, 
                           V & oldValue,
                           bool & oldValueValid)
{
    // This cast is safe, only we can put assocs into the table
    PltAssoc<K,V> *p = 
        ( PltAssoc<K,V> *) lookupAssoc(&key);
    if (p) {
        oldValue = p->a_value;
        oldValueValid = true;
        p->a_value = newValue;
        return true;
    } else {
        oldValueValid = false;
        return add(key, newValue);
    }
}

    
//////////////////////////////////////////////////////////////////////

template <class K, class V>
bool
PltHashTable_<K,V>::remove(const K& key, V& value)
{
    // This cast is safe, only we can put assocs into the table
    PltAssoc<K,V> *p = 
        (PltAssoc<K,V> *) removeAssoc(&key);
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
PltHashIterator_THISTYPE(K,V) * 
PltHashTable_<K,V>::newIterator() const
{
    return new PltHashIterator<K,V>(*this);
}


//////////////////////////////////////////////////////////////////////
#endif //PLT_HASHTABLE_IMPL_INCLUDED

// EOF plt/hashtable_impl.h



