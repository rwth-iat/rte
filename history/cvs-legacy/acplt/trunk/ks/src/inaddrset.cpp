/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/src/inaddrset.cpp,v 1.1 1997-09-02 15:08:41 martin Exp $ */
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

//////////////////////////////////////////////////////////////////////

#include "ks/inaddrset.h"

//////////////////////////////////////////////////////////////////////

bool
KsInAddrSet::addItem(in_addr addr, in_addr mask, bool incl)
{
    if ((addr.s_addr & ~mask.s_addr) != 0) return false;
    Item * pItem = new Item(addr, mask, incl);
    if (pItem) {
        return _list.addFirst(pItem);
    } else {
        return false;
    }
}

//////////////////////////////////////////////////////////////////////

bool
KsInAddrSet::isMember(in_addr addr) const
{
    PLT_PRECONDITION(addr.s_addr != INADDR_NONE && addr.s_addr != INADDR_ANY);
    
    // return first match
    for (PltIListIterator<Item> it=_list; it; ++it) {
        if ((addr.s_addr & it->_mask.s_addr) == it->_addr.s_addr) {
            // match
            return it->_incl;
        }
    }
    return false;
}

//////////////////////////////////////////////////////////////////////

istream & operator >> (istream & istr, KsInAddrSet & set)
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
        if (! (incl ? set.add(a,m) : set.remove(a,m))) {
            goto failure;
        }
    }
 failure:
    istr.fail();
 success:
    return istr;
}

//////////////////////////////////////////////////////////////////////

void
KsInAddrSet::removeAll()
{
    while(!_list.isEmpty()) {
        delete _list.removeFirst();
    }
}
        
        
        
/////////////////////////////////////////////////////////////////////////////
/* EOF ks/event.cpp */
