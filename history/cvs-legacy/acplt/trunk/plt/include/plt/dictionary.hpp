/* -*-plt-c++-*- */
/* 
 *  Copyright (c) 1996 PLT, RWTH-Aachen, Germany. See file copy_plt.txt!
 *  Author: Martin Kneissl <martin@plt.rwth-aachen.de>
 *
 */

#ifndef PLT_DICTIONARY_INCLUDED
#define PLT_DICTIONARY_INCLUDED

#include <plt/debug.h>

template <class K, class V>
class PltDictionary : virtual public PltDebuggable {
public:
    // accessors
    virtual bool query(const K&, V&) const = 0;
    // modifiers
    virtual bool add(const K&, const V&) = 0;
    virtual bool remove(const K&, V&) = 0;
};

    
#endif
