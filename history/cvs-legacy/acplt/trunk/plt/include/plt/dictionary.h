/* -*-plt-c++-*- */
#ifndef PLT_DICTIONARY_INCLUDED
#define PLT_DICTIONARY_INCLUDED
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
// plt/dictionary.h provides the dictionary interface template
//////////////////////////////////////////////////////////////////////

#include "plt/assoc.h"
#include "plt/container.h"

//////////////////////////////////////////////////////////////////////
// abstract class PltDictionary<K,V>
//////////////////////////////////////////////////////////////////////
//
// A PltDictionary<K,V> maps from keys of class K to values of 
// class V. 
//
// Operations:
// ===========
//
// Accessors:
// ----------
//
// bool query(const K& k, V& v) const
//    Lookup key k. If k is a key in the dictionary assign the 
//    associated value to v and return true. 
//    Otherwise return false.
//
// Modifiers:
// ----------
//
// bool add(const K& k, const V& v)
//    Try to insert the association (k->v). Return true on success, 
//    false on failure. Failure could be caused by
//    - lack of memory
//    - key k is already a key of the dictionary
//
// bool update(const K& k, const V& v, V& oValue, bool & oValid)
//    Try associate (k with v). Return true on success, 
//    false on failure. Failure could be caused by
//    lack of memory
//    If oValid is true on return, oValue contains the old value associated
//    with k before the update call.
//
// bool remove(const K& k, V& v)
//    Like query, but removes the association on success.
//
//////////////////////////////////////////////////////////////////////

template <class K, class V>
class PltDictionary 
: public PltContainer< PltAssoc<K,V> > 
{
public:
    // accessors
    virtual bool query(const K& key, V& value) const = 0;

    PltIterator< PltAssoc<K, V> > * newIterator() const = 0;

    // modifiers
    virtual bool add(const K& key, const V& value) = 0;

    virtual bool update(const K & key, 
                        const V & newValue, 
                        V & oldValue,
                        bool & oldValueValid) const;

    virtual bool remove(const K& key, V& oldValue) = 0;
};

//////////////////////////////////////////////////////////////////////
    
#endif // PLT_DICTIONARY_INCLUDED
