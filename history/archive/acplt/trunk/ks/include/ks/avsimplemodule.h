/* -*-plt-c++-*- */
#ifndef KSC_AVSIMPLEMODULE_INCLUDED 
#define KSC_AVSIMPLEMODULE_INCLUDED
/*
 * Copyright (c) 1996, 1997, 1998, 1999, 2000
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
    KS_AUTH_TYPE typeCode() const;
    bool xdrEncode(XDR *);
    bool xdrDecode(XDR *);

private:
    friend class KscAvSimpleModule;
    KscSimpleNegotiator(const PltString &);

    KsString _id;
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
    KscAvSimpleModule(const PltString &id);
    ~KscAvSimpleModule();

    virtual KS_AUTH_TYPE typeCode() const;


protected:
    KsString _id;
    KscNegotiatorHandle _negotiator;

    KscNegotiatorHandle _getNegotiator(const KscServer *) const;
};

//////////////////////////////////////////////////////////////////////
// Inline Implementation
//////////////////////////////////////////////////////////////////////

inline
KscSimpleNegotiator::KscSimpleNegotiator(const PltString &str)
: _id(str)
{}

//////////////////////////////////////////////////////////////////////

inline
KS_AUTH_TYPE
KscSimpleNegotiator::typeCode() const
{
    return KS_AUTH_SIMPLE;
}

//////////////////////////////////////////////////////////////////////

inline
KscAvSimpleModule::KscAvSimpleModule(const PltString &id)
: _id(id),
  _negotiator(new KscSimpleNegotiator(id), KsOsNew)
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





