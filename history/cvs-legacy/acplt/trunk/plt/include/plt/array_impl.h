/* -*-plt-c++-*- */
#ifndef PLT_ARRAY_IMPL_INCLUDED
#define PLT_ARRAY_IMPL_INCLUDED
/* $Header: /home/david/cvs/acplt/plt/include/plt/array_impl.h,v 1.3 1997-04-10 14:09:18 martin Exp $ */
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
// PltArray template implementation
//////////////////////////////////////////////////////////////////////

#include "plt/array.h"

#if !PLT_SEE_ALL_TEMPLATES
#include "plt/handle_impl.h"
#include "plt/container_impl.h"
#endif

//////////////////////////////////////////////////////////////////////

#if PLT_DEBUG_INVARIANTS
template <class T>
bool 
PltArray<T>::invariant() const
{
    return (a_size == 0 && ! a_array) || (a_size > 0 && a_array);
}
#endif

//////////////////////////////////////////////////////////////////////

template <class T>
PltArray<T>
PltArray<T>::copy() const
{
    PltArray<T> res(a_size);
    
    // copy elements
    for (size_t i = 0; i < a_size; ++i) {
        res.a_array[i] = a_array[i];
    }
    return res;
}

//////////////////////////////////////////////////////////////////////

#endif
// EOF plt/array_impl.h
