/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/src/inaddrset.cpp,v 1.3 1997-12-02 10:17:25 harald Exp $ */
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

        
/////////////////////////////////////////////////////////////////////////////
/* EOF ks/event.cpp */
