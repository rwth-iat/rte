/* -*-plt-c++-*- */
/* File: plt/list.cpp */
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

#include "plt/list.h"

//////////////////////////////////////////////////////////////////////

#if PLT_DEBUG_INVARIANTS
bool
PltList_base::invariant() const
{
    // Checking is simple and inefficient:
    // - there must be as many forward links as backward links.
    // - by traversing the list we may catch bad links

    int fwdc, bwdc;
    PltListNode_base *p;
    
    // Count forward links
    for (fwdc=0, p=first; p; p = p->next) {
        ++fwdc;
    }

    // Count backward links
    for (bwdc=0, p= last; p; p = p->prev) {
        ++bwdc;
    }

    return bwdc==fwdc;
}
#endif // PLT_DEBUG_INVARIANTS

//////////////////////////////////////////////////////////////////////

PltList_base::~PltList_base()
{
    for (PltListNode_base *p = first; p; p=p->next) {
        delete p;
    }
}

//////////////////////////////////////////////////////////////////////

size_t
PltList_base::size() const 
{
    size_t res = 0;
    for (PltListNode_base * p = first; p ; p = p->next) {
        ++res;
    }
    return res;
}

//////////////////////////////////////////////////////////////////////

bool 
PltList_base::addFirst(PltListNode_base * p) 
{ 
    PLT_PRECONDITION(p);
    p->prev = 0;
    if (first) {
        first->prev = p;
    }
    p->next = first;
    first = p;
    if (!last) {
        last=first;
    }
    PLT_CHECK_INVARIANT();
    return true;
}

//////////////////////////////////////////////////////////////////////

bool 
PltList_base::addLast(PltListNode_base * p) 
{
    PLT_PRECONDITION(p);
    p->next = 0;
    if (last) {
        last->next = p;
    }
    p->prev = last;
    last = p;
    if (!first) {
        first=last;
    }
    PLT_CHECK_INVARIANT();
    return true;
}

//////////////////////////////////////////////////////////////////////

PltListNode_base *
PltList_base::remove(PltListNode_base *p ) 
{
    PLT_PRECONDITION(p);
    PltListNode_base *pred = p->prev;
    PltListNode_base *succ = p->next;
    if (pred) {
        pred->next = succ;
    } else {
        first = succ;
    }
    if (succ) {
        succ->prev = pred;
    } else {
        last = pred;
    }
    p->next = p->prev = 0;
    PLT_CHECK_INVARIANT();
    return p;
}

//////////////////////////////////////////////////////////////////////
// EOF plt/list.cpp
//////////////////////////////////////////////////////////////////////
