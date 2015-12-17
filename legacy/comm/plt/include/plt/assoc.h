/* -*-plt-c++-*- */
#ifndef PLT_ASSOC_INCLUDED
#define PLT_ASSOC_INCLUDED
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


//////////////////////////////////////////////////////////////////////
// plt/assoc.h provides the PltAssoc structure
//////////////////////////////////////////////////////////////////////

#include "plt/key.h"

//////////////////////////////////////////////////////////////////////
// forward declaration
class PltAssoc_;

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
// INLINE IMPLEMENTATION
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
    virtual const void * key() const = 0;
};

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

#endif // PLT_ASSOC_INCLUDED
