/* -*-plt-c++-*- */
/* 
 *  Copyright (c) 1996 PLT, RWTH-Aachen, Germany. See file copy_plt.txt!
 *  Author: Martin Kneissl <martin@plt.rwth-aachen.de>
 *
 */

#ifndef PLT_KEY_INCLUDED
#define PLT_KEY_INCLUDED

#include <plt/debug.h>
#include <plt/rtti.hpp>

//////////////////////////////////////////////////////////////////////

class PltKey 
{
public:
    virtual unsigned long hash() const = 0;
    virtual bool operator == (const PltKey &) const = 0;
    PLT_DECL_RTTI;
};

//////////////////////////////////////////////////////////////////////

#endif // header file
