/* -*-plt-c++-*- */
#ifndef PLT_ASSOC_INCLUDED
#define PLT_ASSOC_INCLUDED
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
    virtual const PltKey & key() const;

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
    virtual const PltKey & key() const = 0;
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
const PltKey &
PltAssoc<K,V>::key() const
{
    return a_key;
}

//////////////////////////////////////////////////////////////////////

#endif // PLT_ASSOC_INCLUDED
