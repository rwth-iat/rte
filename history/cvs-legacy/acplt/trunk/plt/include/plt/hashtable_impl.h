/* -*-plt-c++-*- */
#ifndef PLT_HASHTABLE_IMPL_INCLUDED
#define PLT_HASHTABLE_IMPL_INCLUDED

/* $Header: /home/david/cvs/acplt/plt/include/plt/hashtable_impl.h,v 1.8 1998-03-06 13:22:32 markusj Exp $ */
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



