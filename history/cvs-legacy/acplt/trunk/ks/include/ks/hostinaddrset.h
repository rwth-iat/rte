/* -*-plt-c++-*- */
#ifndef KS_HOSTINADDRSET_INCLUDED
#define KS_HOSTINADDRSET_INCLUDED
/* $Header: /home/david/cvs/acplt/ks/include/ks/hostinaddrset.h,v 1.2 1997-12-02 10:14:53 harald Exp $ */
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
