/* -*-plt-c++-*- */
/* 
 *  Copyright (c) 1996 PLT, RWTH-Aachen, Germany. See file copy_plt.txt!
 *  Author: Martin Kneissl <martin@plt.rwth-aachen.de>
 *
 */

#ifndef PLT_ASSOC_INCLUDED
#define PLT_ASSOC_INCLUDED

#include <plt/debug.h>
#include <plt/key.hpp>

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
struct PltAssoc : public PltAssoc_
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

#endif // end of header file
