/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/plt/src/handle.cpp,v 1.5 1997-09-10 14:52:32 martin Exp $ */
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

#include "plt/handle.h"

//////////////////////////////////////////////////////////////////////

PltAllocator<Plt_AllocTracker>
Plt_AllocTracker::_allocator;

#if PLT_INSTANTIATE_TEMPLATES
template class PltAllocator<Plt_AllocTracker>;
#endif

//////////////////////////////////////////////////////////////////////

PltAllocator<PltHandle_base>
PltHandle_base::_allocator;

#if PLT_INSTANTIATE_TEMPLATES
template class PltAllocator<PltHandle_base>;
#endif

//////////////////////////////////////////////////////////////////////

#if PLT_DEBUG_INVARIANTS
bool 
PltHandle_base::invariant() const 
{
    return (prep==0 && palloc==0) 
        || (prep && (palloc ? palloc->count > 0 : true));
}
#endif

//////////////////////////////////////////////////////////////////////
// Add one reference to the handled object

void 
PltHandle_base::addRef() const
{
    PLT_PRECONDITION(palloc ? palloc->count<UINT_MAX : true);
    if (*this && palloc) {
        palloc->count++;
    }
    PLT_CHECK_INVARIANT();
}

//////////////////////////////////////////////////////////////////////
// Remove one reference to the handled object.

void
PltHandle_base::removeRef()
{
    if ( *this && palloc && --(palloc->count) == 0) {
        palloc->destroy(prep);
        prep = 0;
        delete palloc; palloc = 0;
    }
    PLT_CHECK_INVARIANT();
}


//////////////////////////////////////////////////////////////////////
// Bind to a new object.
// If binding succeeds p is no longer valid.
// Returns true on success.

void
PltHandle_base::bindTo(void * p, Plt_AllocTracker *a)
{
    // unbind old handled object
    removeRef();

    PLT_ASSERT( palloc == 0 );
    
    // bind new handled object
    prep = p;
    palloc = a;
    
    PLT_CHECK_INVARIANT();
}

//////////////////////////////////////////////////////////////////////
// Copy constructor. The handled object -- if any -- gets an 
// additional reference.

PltHandle_base::PltHandle_base(const PltHandle_base &h)
: prep(h.prep), palloc(h.palloc)
{
    if (h) {
        addRef();
    }
    PLT_CHECK_INVARIANT();
}

//////////////////////////////////////////////////////////////////////
//  Assignment

PltHandle_base & 
PltHandle_base::operator=(const PltHandle_base &rhs) 
{
    // order is important if called as 'handle = handle;'
    rhs.addRef();
    removeRef();
    
    palloc = rhs.palloc;
    prep = rhs.prep;

    PLT_CHECK_INVARIANT();
    return *this;
}

//////////////////////////////////////////////////////////////////////

unsigned
PltHandle_base::getRefCount() const
{
    if(palloc) {
        return palloc->count;
    } else {
        return 0;
    }
}

//////////////////////////////////////////////////////////////////////

#if !PLT_INSTANTIATE_TEMPLATES
#include "plt/handle_impl.h"
#endif

//////////////////////////////////////////////////////////////////////
// EOF plt/handle.cpp



