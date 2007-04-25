/* -*-plt-c++-*- */
#ifndef KS_HOSTINADDRSET_INCLUDED
#define KS_HOSTINADDRSET_INCLUDED
/* $Header: /home/david/cvs/acplt/ks/include/ks/hostinaddrset.h,v 1.7 2007-04-25 12:57:20 martin Exp $ */
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

#include "ks/inaddrset.h"

class KsHostInAddrSet:
    virtual public KsInAddrSet,
    private KsInAddrSet_base {
public:
    KsHostInAddrSet();
    ~KsHostInAddrSet() { }

    virtual bool isMember(in_addr addr) const;

    bool update();

private:
    bool findLocalAddresses();
#if PLT_SYSTEM_NT
    bool findLocalAddressesNT();
    bool findLocalAddressesW95();
#endif
}; // class KsHostInAddrSet

inline bool
KsHostInAddrSet::isMember(in_addr addr) const
{
    return KsInAddrSet_base::isMember(addr);
} // KsHostInAddrSet::isMember

#endif // KS_HOSTINADDRSET_INCLUDED

// End of hostinaddrset.h
