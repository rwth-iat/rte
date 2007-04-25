/* -*-plt-c++-*- */
#ifndef PLT_DICTIONARY_INCLUDED
#define PLT_DICTIONARY_INCLUDED
/* $Header: /home/david/cvs/acplt/plt/include/plt/dictionary.h,v 1.12 2007-04-25 12:57:21 martin Exp $ */
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

#include "plt/container.h"
    
//////////////////////////////////////////////////////////////////////
// plt/dictionary.h provides the dictionary interface template
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// abstract class PltAssoc_
//////////////////////////////////////////////////////////////////////
// 
// Abstract base class for associations
//
// Operations:
// ===========
//
// Accessors:
// ----------
//
// key() returns a const reference to the key
//
// Modifiers:
// ----------
//
//
//
//////////////////////////////////////////////////////////////////////

class PltAssoc_
{
public:
    virtual ~PltAssoc_() { }
    virtual const void * key() const = 0;
};

//////////////////////////////////////////////////////////////////////
// struct PltAssoc<K,V>
//////////////////////////////////////////////////////////////////////
//
// A PltAssoc<K,V> associates keys of class K with values of 
// class V. 
//
// K must be kind of PltKey
//
// Base classes:
// =============
// PltAssoc_
//
//////////////////////////////////////////////////////////////////////

template <class K, class V>
struct PltAssoc 
: public PltAssoc_
{
public:
    // public attributes
    K a_key;
    V a_value;

    // ctors/dtors
    PltAssoc(K k, V v);

    // accessors
    virtual const void * key() const;

    // modifiers
};


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

//    PltIterator< PltAssoc<K, V> >::THISTYPE * newIterator() const = 0;
    typedef PltAssoc<K,V> PltTemplateAssoc;	// avoid errors in expanding the
    			                                 // following macro
    virtual PltIterator_THISTYPE(PltTemplateAssoc) * newIterator() const = 0;

    // modifiers
    virtual bool add(const K& key, const V& value) = 0;

    virtual bool update(const K & key,
                        const V & newValue,
                        V & oldValue,
                        bool & oldValueValid) = 0;

    virtual bool remove(const K& key, V& oldValue) = 0;
};

//////////////////////////////////////////////////////////////////////
// INLINE IMPLEMENTATION
//////////////////////////////////////////////////////////////////////

template <class K, class V>
inline
PltAssoc<K,V>::PltAssoc(K k, V v)
: a_key(k), a_value(v)
{
}

//////////////////////////////////////////////////////////////////////

template <class K, class V>
inline
const void *
PltAssoc<K,V>::key() const
{
    return &a_key;
}

//////////////////////////////////////////////////////////////////////

#endif // PLT_DICTIONARY_INCLUDED

