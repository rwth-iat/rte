/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/plt/src/handle.cpp,v 1.9 2007-04-25 12:57:21 martin Exp $ */
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



