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

#include <plt/hashtable.h>

#include <ks/props.h>
#include <ks/abspath.h>

//////////////////////////////////////////////////////////////////////
// forward declarations
class KscAvModule;
class KscServer;

//////////////////////////////////////////////////////////////////////
// class KscCommObject
//////////////////////////////////////////////////////////////////////

class KscCommObject
{
public:
    KscCommObject(const KscAbsPath &object_path);
    virtual ~KscCommObject();

    PltString getName() const;
    KscAbsPath getHostAndServer() const;
    const KscAbsPath &getFullPath() const; 
 
    virtual KS_OBJ_TYPE typeCode() const = 0;

    KscServer *getServer() const;

    virtual const KsProjProps *getProjProps() const = 0;
    virtual bool getProjPropsUpdate() = 0;

    virtual void setAvModule(const KscAvModule *avm);
    virtual const KscAvModule *getAvModule() const;

protected:
    KscAbsPath path;
    const KscAvModule *av_module;

    virtual bool setProjProps(KsProjPropsHandle) = 0;
};


//////////////////////////////////////////////////////////////////////
// class KscDomain
//////////////////////////////////////////////////////////////////////

typedef PltIterator<KsProjProps> KscChildIterator;

class KscDomain
: public KscCommObject
{
public:
    KscDomain(const KscAbsPath &domain_path);

    KS_OBJ_TYPE typeCode() const;

    const KsDomainProjProps *getProjProps() const;

    // reread projected props 
    bool getProjPropsUpdate();
    // reread children from server, update properties
    bool updateChilds(KS_OBJ_TYPE typeMask);
    KscChildIterator *newChildIterator(KS_OBJ_TYPE typeMask,
                                       bool update = false);

protected:
    KsDomainProjProps proj_props;
    PltHashTable<KscAbsPath, KscCommObject *>child_table;

    // remove childs from table
    bool flushChilds(KS_OBJ_TYPE typeMask);

    bool setProjProps(KsProjPropsHandle);

#if 0
    // TODO : DEBUG
    //
    class ChildIterator
    : public KscChildIterator
    {
    public:
        ChildIterator(const KscDomain &, enum_t);
        operator const void * () const;   // remaining element?
        ChildIterator & operator ++ ();   // advance
        void operator ++ (int);           // (postfix)
        void toStart();                   // go to the beginning
        const KsProjProps * operator -> () const;
    private:
        enum_t type_mask;
        PltHashIterator<KscAbsPath, KscCommObject *> it;
    };
#endif
        
};

//////////////////////////////////////////////////////////////////////
// class KscVariable
//////////////////////////////////////////////////////////////////////

class KscVariable
: public KscCommObject
{
public:
    KscVariable(const KscAbsPath &var_name);

    KS_OBJ_TYPE typeCode() const;

    bool getProjPropsUpdate();
    virtual bool getUpdate();
    virtual bool setUpdate();
    KsValueHandle getValue() const;

    const KsVarProjProps *getProjProps() const;
    const KsVarCurrProps *getCurrProps() const;
    bool setCurrProps(KsVarCurrProps &cp);
    bool isDirty() const;
   
protected:

    KsVarProjProps proj_props;
    KsVarCurrProps curr_props;

    friend class KscPackage; // for access to fDirty 
    bool fDirty;

    bool setProjProps(KsProjPropsHandle);
};

#endif

//////////////////////////////////////////////////////////////////////
// EOF CommObject.h
//////////////////////////////////////////////////////////////////////










