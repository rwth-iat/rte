/* -*-plt-c++-*- */

#ifndef KSC_COMMOBJECT_INCLUDED 
#define KSC_COMMOBJECT_INCLUDED

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

#include <ks/props.h>

//////////////////////////////////////////////////////////////////////

class KscAvModule;

//////////////////////////////////////////////////////////////////////
// class KscCommObject
//////////////////////////////////////////////////////////////////////

class KscCommObject
{
public:
    KscCommObject(const char *object_name);
    virtual ~KscCommObject();
    virtual const char *getName() const;
    virtual KS_OBJ_TYPE typeCode() const = 0;

    //virtual bool getProjPropsUpdate();     // TODO

    virtual void setAvModule(const KscAvModule *avm);
    virtual const KscAvModule *getAvModule() const;

protected:
    PltString name;
    const KscAvModule *av_module;
};


//////////////////////////////////////////////////////////////////////
// class KscDomain
//////////////////////////////////////////////////////////////////////

class KscDomain
: public KscCommObject
{
public:
    KscDomain(const char *domain_name);
    ~KscDomain();

    KS_OBJ_TYPE typeCode() const;

    const KsDomainProjProps *getProjProps() const;
//    KscChildIterator *newChildIterator(KS_OBJ_TYPE mask); // TODO

protected:
    KsDomainProjProps *proj_props;
};

//////////////////////////////////////////////////////////////////////
// class KscVariable
//////////////////////////////////////////////////////////////////////

class KscVariable
: public KscCommObject
{
public:
    KscVariable(const char *var_name);
    ~KscVariable();

    KS_OBJ_TYPE typeCode() const;

    //virtual bool getUpdate();            // TODO
    //virtual bool setUpdate();            // TODO
    KsValueHandle getValue() const;      // TODO
    const KsVarProjProps *getProjProps() const;
    const KsVarCurrProps *getCurrProps() const;
    bool setCurrProps(const KsVarCurrProps &cp);
    bool isDirty() const;
   
protected:

    KsVarProjProps *proj_props;
    KsVarCurrProps *curr_props;

    bool fDirty;
};



#endif

//////////////////////////////////////////////////////////////////////
// EOF CommObject.h
//////////////////////////////////////////////////////////////////////





