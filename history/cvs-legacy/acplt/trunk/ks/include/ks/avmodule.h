/* -*-plt-c++-*- */

#ifndef KSC_AVMODULE_INCLUDED 
#define KSC_AVMODULE_INCLUDED

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

#include <ks/xdr.h>

//////////////////////////////////////////////////////////////////////
// class KscNegotiator
//
class KscNegotiator
// : public KsXdrAble // TODO:neccessary / useful ?
{
public:
    virtual bool xdrEncode(XDR *) = 0;
    virtual bool xdrDecode(XDR *) = 0;
};

//////////////////////////////////////////////////////////////////////
// class KscAvModule
// 
class KscAvModule
{
public:
    virtual ~KscAvModule() {}
    virtual KscNegotiator *getNegotiator() const = 0;
};

//////////////////////////////////////////////////////////////////////
// class KscNoneNegotiator
//   handles an AUTH_NONE authentication. 
//
class KscNoneNegotiator
: public KscNegotiator
{
public:
    bool xdrEncode(XDR *);
    bool xdrDecode(XDR *);

private:
    friend class KscAvNoneModule;
    KscNoneNegotiator() {}   // forbidden
};
    
//////////////////////////////////////////////////////////////////////
// class KscAvNoneModule
// This is a simple AvModule which give us a KscNegotiator object
// to handle an AUTH_NONE authentication.
//
class KscAvNoneModule
: public KscAvModule
{
public:
    KscNegotiator *getNegotiator() const;

    static KscNegotiator *getStaticNegotiator();

private:
    static KscNoneNegotiator the_negotiator;
};

//////////////////////////////////////////////////////////////////////
// Inline Implementation
//////////////////////////////////////////////////////////////////////

inline
KscNegotiator *
KscAvNoneModule::getStaticNegotiator()
{
    return &the_negotiator;
}

#endif





