/* -*-plt-c++-*- */
#ifndef KS_SVROBJECTS_INCLUDED
#define KS_SVROBJECTS_INCLUDED
/* $Header: /home/david/cvs/acplt/ks/include/ks/svrobjects.h,v 1.1 1997-03-23 14:35:15 martin Exp $ */
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

#include "plt/rtti.h"
#include "plt/container.h"
#include "ks/handle.h"
#include "ks/ks.h"
#include "ks/string.h"
#include "ks/time.h"
#include "ks/value.h"

//////////////////////////////////////////////////////////////////////
// class KssCommObject
//////////////////////////////////////////////////////////////////////

class KssCommObject
{
public:
    // ctor/dtor
    virtual ~KssCommObject() { }
    virtual KS_OBJ_TYPE_ENUM typeCode() const = 0; 
    // ^ optimization in absence of true RTTI

    // projected properties
    virtual KsString getIdentifier() const = 0;
    virtual KsTime   getCreationTime() const = 0;
    virtual KsString getComment() const = 0;

    PLT_DECL_RTTI;
};

typedef PltPtrHandle<KssCommObject> KssCommObjectHandle;

//////////////////////////////////////////////////////////////////////
// class KssDomainIterator
//////////////////////////////////////////////////////////////////////

typedef PltIterator<KssCommObjectHandle> KssDomainIterator;

//////////////////////////////////////////////////////////////////////
// class KssDomain
//////////////////////////////////////////////////////////////////////

class KssDomain
: public KssCommObject
{
public:
    //// KssCommObject
    //   accessors
    virtual KS_OBJ_TYPE_ENUM typeCode() const { return KS_OT_DOMAIN; }

    // projected properties
    virtual KsString getIdentifier() const = 0;
    virtual KsTime   getCreationTime() const = 0;
    virtual KsString getComment() const = 0;

    //// KssDomain
    //   accessors
    virtual KssDomainIterator * newIterator(const KsString & name_mask,
                                            KS_OBJ_TYPE type_mask) 
        const = 0;

    virtual KssCommObjectHandle child(const KsString & id) const;
    // ^ defaults to linear search on iterator, 
    //   you should implement a more efficient version!!!
    
    PLT_DECL_RTTI;
};
typedef PltPtrHandle<KssDomain> KssDomainHandle;
//////////////////////////////////////////////////////////////////////
// class KssVariable
//////////////////////////////////////////////////////////////////////

class KssVariable
: public KssCommObject
{
public:
    //// KssCommObject
    ///  accessors
    virtual KS_OBJ_TYPE_ENUM typeCode() const { return KS_OT_VARIABLE; } 

    // projected properties
    virtual KsString getIdentifier() const = 0;
    virtual KsTime   getCreationTime() const = 0;
    virtual KsString getComment() const = 0;

    //// KssVariable ////
    //// accessors
    //   projected properties
    virtual KsString getTechUnit() const = 0;
    virtual KS_VAR_TYPE getType() const;
    
    //   current properties
    virtual KsValueHandle getValue() const = 0;
    virtual KsTime        getTime() const;
    virtual KS_STATE      getState() const;
    
    //// modifiers
    //   current properties
    virtual KS_RESULT     setValue(const KsValueHandle &) = 0;

    virtual KS_RESULT     setValue(KsValue *p);
    //  implicit handle creation, usual caveats apply

    virtual KS_RESULT     setTime(const KsTime &) = 0;

    virtual KS_RESULT     setState(KS_STATE) = 0;

    PLT_DECL_RTTI;
};
        
//////////////////////////////////////////////////////////////////////
#endif // KS_SVROBJECTS_INCLUDED
