/* -*-plt-c++-*- */
/* 
 *  Copyright (c) 1997 PLT, RWTH-Aachen, Germany. See file copy_plt.txt!
 *  Author: Martin Kneissl <martin@plt.rwth-aachen.de>
 *
 */

#ifndef PLT_CONTAINER_INCLUDED
#define PLT_CONTAINER_INCLUDED

#include <plt/debug.h>
#include <plt/iterator.hpp>

//////////////////////////////////////////////////////////////////////
// Container interface
//////////////////////////////////////////////////////////////////////

template<class T>
class PltContainer
{
public:
    virtual PltIterator<T> * newIterator() const = 0;
};

//////////////////////////////////////////////////////////////////////

#endif // end of header file
