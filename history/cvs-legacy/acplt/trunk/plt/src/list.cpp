/* -*-plt-c++-*- 
/* 
 *  Copyright (c) 1996 PLT, RWTH-Aachen, Germany. See file copy_plt.txt!
 *  Author: Martin Kneissl <martin@plt.rwth-aachen.de>
 *
 */

#include <plt/debug.h>
#include <plt/list.hpp>



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

