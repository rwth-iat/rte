/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/src/inaddrset.cpp,v 1.10 2007-04-25 12:57:20 martin Exp $ */
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
/* Written by Martin Kneissl <martin@plt.rwth-aachen.de> */
/* Devastated by Harald Albrecht <harald@plt.rwth-aachen.de> */

#include <stdlib.h>

#include "ks/inaddrset.h"

//////////////////////////////////////////////////////////////////////

KsInAddrSet_base::KsInAddrSet_base()
{
    _items = 0;
    _items_allocated = 0;
    _items_used = 0;
} // KsInAddrSet::KsInAddrSet

KsInAddrSet_base::~KsInAddrSet_base()
{
    removeAll();
} // KsInAddrSet_base::~KsInAddrSet_base

//////////////////////////////////////////////////////////////////////

bool KsInAddrSet_base::addItem(in_addr addr, in_addr mask, bool incl)
{
    Item *newItems;
    int   newItemsCount;

    PLT_PRECONDITION(_items_used >= 0 && _items_used <= _items_allocated);

    if ( (addr.s_addr & ~mask.s_addr) != 0 ) {
        return false;
    }

    if ( _items_used >= _items_allocated ) {
        if ( _items_allocated == 0 ) {
            newItemsCount = 1;
        } else {
            newItemsCount = _items_allocated * 2;
        }
        newItems = (Item *) realloc(_items, sizeof(Item) * newItemsCount);
        if ( newItems == 0 ) {
            return false;
        }
        _items = newItems;
        _items_allocated = newItemsCount;
    }
    //
    // Fill in next free entry
    //
    _items[_items_used]._addr = addr;
    _items[_items_used]._mask = mask;
    _items[_items_used]._incl = incl;
    ++_items_used;

    return true;
} // KsInAddrSet_base::addItem

//////////////////////////////////////////////////////////////////////

void
KsInAddrSet_base::removeAll()
{
    if ( _items ) {
        _items_used = 0;
        _items_allocated = 0;
        free(_items);
    }
} // KsInAddrSet_base::removeAll
        
//////////////////////////////////////////////////////////////////////

bool
KsInAddrSet_base::isMember(in_addr addr) const
{
    PLT_PRECONDITION(addr.s_addr != INADDR_NONE && addr.s_addr != INADDR_ANY);

    int count = _items_used;
    for ( int idx = 0; idx < count; ++idx ) {
        if ( (addr.s_addr & _items[idx]._mask.s_addr) == 
             _items[idx]._addr.s_addr ) {
            return _items[idx]._incl;
        }
    }
    return false;
} // KsInAddrSet_base::isMember

//////////////////////////////////////////////////////////////////////

KsSimpleInAddrSet::KsSimpleInAddrSet(bool defaultIsAccept)
    : KsInAddrSet_base(), _defaultIsAccept(defaultIsAccept)
{
} // KsSimpleInAddrSet::KsSimpleInAddrSet

//////////////////////////////////////////////////////////////////////

#if 0
istream & operator >> (istream & istr, KsSimpleInAddrSet & set)
{
    set.removeAll();
    while (istr) {
        bool incl;
        int ch = istr.get();
        switch (ch) {
        case '+':
            incl = true;
            break;
        case '-':
            incl = false;
            break;
        case EOF:
            goto success;
        default:
            goto failure;
        }
        char buf[20];

        // get address
        istr.getline(buf, sizeof buf, '/');
        if (!istr) goto failure;
        u_long addr = inet_addr(buf);
        if (addr == (u_long) -1) goto failure;
        
        // get mask
        istr.getline(buf, sizeof buf);
        if (!istr) goto failure;
        u_long mask;
        if (strcmp(buf,"255.255.255.255") != 0 ) {
            mask = inet_addr(buf);
            if (mask == (u_long) -1) goto failure;
        } else {
            mask = 0xffffffff;
        }

        // add to set
        in_addr a,m;
        a.s_addr = addr;
        m.s_addr = mask;
        if (! (incl ? set.accept(a,m) : set.reject(a,m))) {
            goto failure;
        }
    }
 failure:
    istr.fail();
 success:
    return istr;
}

#endif
        
/////////////////////////////////////////////////////////////////////////////
/* EOF ks/event.cpp */
