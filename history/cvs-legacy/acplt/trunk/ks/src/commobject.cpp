/* -*-plt-c++-*- */

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

#include <ks/commobject.h>

//////////////////////////////////////////////////////////////////////
// Inline Implementation
// These functions should be inline, but there seems to be a compiler
// or linker error.
//////////////////////////////////////////////////////////////////////


KscCommObject::KscCommObject(const char *object_name)
: name(object_name),
  av_module(NULL)
{}

//////////////////////////////////////////////////////////////////////

KscCommObject::~KscCommObject()
{
    // nothing to do
}

//////////////////////////////////////////////////////////////////////

const char *
KscCommObject::getName() const
{
    return name;
}

//////////////////////////////////////////////////////////////////////

void
KscCommObject::setAvModule(const KscAvModule *avm) 
{
    // don't delete old AvModule since we are not owner
    // of it
    //
    av_module = avm;
}

//////////////////////////////////////////////////////////////////////


const KscAvModule *
KscCommObject::getAvModule() const
{
    return av_module;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////


KscDomain::KscDomain(const char *domain_name)
: KscCommObject(domain_name),
  proj_props(NULL)
{}

//////////////////////////////////////////////////////////////////////

KscDomain::~KscDomain()
{
    if(proj_props) {
        delete proj_props;
    }
}

//////////////////////////////////////////////////////////////////////


KS_OBJ_TYPE
KscDomain::typeCode() const
{
    return KS_OT_DOMAIN;
}

//////////////////////////////////////////////////////////////////////


const KsDomainProjProps *
KscDomain::getProjProps() const
{
    return proj_props;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////


KscVariable::KscVariable(const char *var_name)
: KscCommObject(var_name),
  proj_props(NULL),
  curr_props(NULL),
  fDirty(false)
{}

//////////////////////////////////////////////////////////////////////

KscVariable::~KscVariable()
{
    if(proj_props) {
        delete proj_props;
    }

    if(curr_props) {
        delete curr_props;
    }
}

//////////////////////////////////////////////////////////////////////


KS_OBJ_TYPE
KscVariable::typeCode() const
{
    return KS_OT_VARIABLE;
}

//////////////////////////////////////////////////////////////////////


const KsVarProjProps *
KscVariable::getProjProps() const
{
    return proj_props;
}

//////////////////////////////////////////////////////////////////////


const KsVarCurrProps *
KscVariable::getCurrProps() const
{
    return curr_props;
}
 
//////////////////////////////////////////////////////////////////////


bool
KscVariable::isDirty() const
{
    return fDirty;
}

//////////////////////////////////////////////////////////////////////
// End of  Implementation
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// class KscVariable
//////////////////////////////////////////////////////////////////////

KsValueHandle
KscVariable::getValue() const
{
    if( curr_props ) {
        return curr_props->value;
    }

    return KsValueHandle(); // return unbound handle
}

//////////////////////////////////////////////////////////////////////

bool
KscVariable::setCurrProps(const KsVarCurrProps &cp)
{
    if( curr_props ) {
        // object already exists, just copy new data
        //
        *curr_props = cp;
    }
    else {
        // allocate memory
        //
        curr_props = new KsVarCurrProps(cp);
        if( !curr_props ) {
            return false;
        }
    }

    fDirty = true;

    return true;
}

//////////////////////////////////////////////////////////////////////



        

