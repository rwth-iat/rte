/* -*-plt-c++-*- */
/* 
 *  Copyright (c) 1996 PLT, RWTH-Aachen, Germany. See file copy_plt.txt!
 *  Author: Martin Kneissl <martin@plt.rwth-aachen.de>
 *
 */

#ifndef PLT_DICTIONARY_INCLUDED
#define PLT_DICTIONARY_INCLUDED

#include <plt/debug.h>

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
// bool remove(const K& k, V& v)
//    Like query, but removes the association on success.
//
//////////////////////////////////////////////////////////////////////

template <class K, class V>
class PltDictionary : virtual public PltDebuggable {
public:
    // accessors
    virtual bool query(const K&, V&) const = 0;
    // modifiers
    virtual bool add(const K&, const V&) = 0;
    virtual bool remove(const K&, V&) = 0;
};

//////////////////////////////////////////////////////////////////////
    
#endif
