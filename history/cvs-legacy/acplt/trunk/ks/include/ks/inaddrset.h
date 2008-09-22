/* -*-plt-c++-*- */
#ifndef KS_INADDRSET_INCLUDED
#define KS_INADDRSET_INCLUDED
/* $Header: /home/david/cvs/acplt/ks/include/ks/inaddrset.h,v 1.11 2008-09-22 08:26:09 henning Exp $ */
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

//////////////////////////////////////////////////////////////////////

#if PLT_USE_DEPRECIATED_HEADER
#include <iostream.h>
#else
#include <iostream>
#endif

#include "ks/rpc.h"

//////////////////////////////////////////////////////////////////////
//
// Public interface for inet address sets...
//
class KsInAddrSet {
public:
    virtual bool isMember(in_addr addr) const = 0;
    virtual ~KsInAddrSet() {}
}; // class KsInAddrSet

//////////////////////////////////////////////////////////////////////
//
// Base class implementing the management of the individual items
// stored inside an KsInAddrSet. Not useful on its own...
//
class KsInAddrSet_base {
protected:
    KsInAddrSet_base();
    ~KsInAddrSet_base();

    void removeAll();
    bool addItem(in_addr addr, in_addr mask, bool incl);
    bool isMember(in_addr addr) const;
    
    struct Item { // The individual items which make up an addr set...
    public:
        in_addr _addr;
        in_addr _mask; 
        bool _incl;
    };

    Item *_items;
    int   _items_allocated;
    int   _items_used;
}; // class KsInAddrSet_base

//////////////////////////////////////////////////////////////////////

class KsSimpleInAddrSet:
    virtual public KsInAddrSet,
    private KsInAddrSet_base {
public:
    KsSimpleInAddrSet(bool defaultIsAccept = false);
    ~KsSimpleInAddrSet() { }

    virtual bool isMember(in_addr addr) const;

    // modifier
    bool accept(in_addr addr, in_addr mask);
    bool accept(in_addr addr);
    bool reject(in_addr addr, in_addr mask);
    bool reject(in_addr addr);

    void removeAll() { KsInAddrSet_base::removeAll(); }
    
protected:
    bool _defaultIsAccept;
}; // class KsSimpleInAddrSet

// FIXME: istream & operator >> (istream & istr, KsSimpleInAddrSet & set);

//////////////////////////////////////////////////////////////////////
// INLINE IMPLEMENTATION
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////

inline bool
KsSimpleInAddrSet::accept(in_addr addr, in_addr mask)
{
    return addItem(addr, mask, true);
}

//////////////////////////////////////////////////////////////////////

inline bool
KsSimpleInAddrSet::reject(in_addr addr, in_addr mask)
{
    return addItem(addr, mask, false);
}

//////////////////////////////////////////////////////////////////////

inline bool
KsSimpleInAddrSet::accept(in_addr addr)
{
    in_addr mask;
    mask.s_addr = 0xffffffff;
    return accept(addr, mask);
}

//////////////////////////////////////////////////////////////////////

inline bool
KsSimpleInAddrSet::reject(in_addr addr)
{
    in_addr mask;
    mask.s_addr = 0xffffffff;
    return reject(addr, mask);
}

//////////////////////////////////////////////////////////////////////

inline bool
KsSimpleInAddrSet::isMember(in_addr addr) const
{
    return KsInAddrSet_base::isMember(addr) ?
        true : _defaultIsAccept;
} // KsSimpleInAddrSet::isMember

//////////////////////////////////////////////////////////////////////
#endif // KS_INADDRSET_INCLUDED

// End of inaddrset.h
