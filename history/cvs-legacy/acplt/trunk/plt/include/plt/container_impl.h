/* -*-plt-c++-*- */
#ifndef PLT_CONTAINER_IMPL_INCLUDED
#define PLT_CONTAINER_IMPL_INCLUDED
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
