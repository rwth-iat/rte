/* -*-plt-c++-*- */

#ifndef KSC_AVSIMPLEMODULE_INCLUDED 
#define KSC_AVSIMPLEMODULE_INCLUDED

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

/* Author: Markus Juergens <markusj@plt.rwth-aachen.de> */

//////////////////////////////////////////////////////////////////////

#include <plt/handle.h>
#include <plt/hashtable.h>

#include "ks/ks.h"
#include "ks/xdr.h"
#include "ks/string.h"
#include "ks/client.h"
#include "ks/avmodule.h"

//////////////////////////////////////////////////////////////////////
// class KscSimpleNegotiator
//   handles an AUTH_SIMPLE authentication. 
//
class KscSimpleNegotiator
: public KscNegotiator
{
public:
    bool xdrEncode(XDR *);
    bool xdrDecode(XDR *);

private:
    friend class KscAvSimpleModule;
    KscSimpleNegotiator(const KsString &);

    KsString id;
};
    
//////////////////////////////////////////////////////////////////////
// class KscAvSimpleModule
//   Handles a set of servers and associates with each an 
//   identification string. For unknown servers the AV-None Module 
//   is used.
//
class KscAvSimpleModule
: public KscAvModule
{
public:
    KscAvSimpleModule();
    ~KscAvSimpleModule();

    bool addId(const PltString &server, 
               const PltString &identification);
    virtual KS_AUTH_TYPE typeCode() const;
    KscNegotiatorHandle getNegotiator(const KscServer *) const;

protected:
    PltHashTable<PltString, PltString> _id_table;
};

//////////////////////////////////////////////////////////////////////
// Inline Implementation
//////////////////////////////////////////////////////////////////////

inline
KscSimpleNegotiator::KscSimpleNegotiator(const KsString &str)
: id(str)
{}

//////////////////////////////////////////////////////////////////////

inline
KscAvSimpleModule::KscAvSimpleModule()
{}

//////////////////////////////////////////////////////////////////////

inline
KscAvSimpleModule::~KscAvSimpleModule()
{}

//////////////////////////////////////////////////////////////////////

inline
KS_AUTH_TYPE
KscAvSimpleModule::typeCode() const
{
    return KS_AUTH_SIMPLE;
}

//////////////////////////////////////////////////////////////////////

#endif





