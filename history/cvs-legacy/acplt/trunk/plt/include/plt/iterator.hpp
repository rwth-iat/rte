/* -*-plt-c++-*- */
/* 
 *  Copyright (c) 1996 PLT, RWTH-Aachen, Germany. See file copy_plt.txt!
 *  Author: Martin Kneissl <martin@plt.rwth-aachen.de>
 *
 */

#ifndef PLT_ITERATOR_INCLUDED
#define PLT_ITERATOR_INCLUDED

#include <plt/debug.h>


//////////////////////////////////////////////////////////////////////

template<class T>
class PltIterator
{
public:
    virtual operator const void * () const = 0;   // remaining element?
    virtual T operator * () const = 0;            // current element
    virtual PltIterator & operator ++ () = 0;     // advance
};

//////////////////////////////////////////////////////////////////////

#endif // end of header file
