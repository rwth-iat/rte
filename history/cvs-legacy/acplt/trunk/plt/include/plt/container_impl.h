/* -*-plt-c++-*- */
#ifndef PLT_CONTAINER_IMPL_INCLUDED
#define PLT_CONTAINER_IMPL_INCLUDED
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


//////////////////////////////////////////////////////////////////////
// plt/container.h contains Container and Iterator interfaces
//////////////////////////////////////////////////////////////////////

#if !PLT_SEE_ALL_TEMPLATES
#include "plt/container.h"
#endif

//////////////////////////////////////////////////////////////////////

template <class T>
size_t 
PltContainer<T>::size() const
{
    PltIterator<T> *pit = 
        PLT_RETTYPE_CAST((PltIterator<T>*)) newIterator();
    size_t count = 0;
    if (pit) {
        for (PltIterator<T> &it = *pit; it; ++it) {
            ++count;
        }
    }
    return count;
}

//////////////////////////////////////////////////////////////////////

template <class T>
size_t 
PltHandleContainer<T>::size() const
{
    PltHandleIterator<T> *pit = 
        PLT_RETTYPE_CAST((PltHandleIterator<T> *)) newIterator();
    size_t count = 0;
    if (pit) {
        for (PltHandleIterator<T> &it = *pit; it; ++it) {
            ++count;
        }
    }
    return count;
}

//////////////////////////////////////////////////////////////////////
#if 0

template <class T>
void 
PltContainer<T>::debugPrint(ostream & ostr)
{
    PltIterator<T> *pit = newIterator();
    if (pit) {
        ostr << "(";
        for (PltIterator<T> &it(*pit); it; ++it) {
            it->debugPrint();
        }
    }
}
    
#endif
//////////////////////////////////////////////////////////////////////

#endif // PLT_CONTAINER_IMPL_INCLUDED
