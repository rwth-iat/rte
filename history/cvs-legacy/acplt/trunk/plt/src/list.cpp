/* -*-plt-c++-*- */
/* File: plt/list.cpp */
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

#if !PLT_INSTANTIATE_TEMPLATES
#include "plt/list_impl.h"
#endif

//////////////////////////////////////////////////////////////////////
// EOF plt/list.cpp
//////////////////////////////////////////////////////////////////////
