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

#include <plt/debug.h>
#include <plt/rtti.h>
#include <plt/hashtable.h>

#include "ks/props.h"
#include "ks/abspath.h"
#include "ks/avmodule.h"

//////////////////////////////////////////////////////////////////////
// forward declarations
class KscServer;

//////////////////////////////////////////////////////////////////////
// class KscCommObject
//////////////////////////////////////////////////////////////////////

class KscCommObject
{
public:
    KscCommObject(const char *object_path);
//  KscCommObject(const KscAbsPath &object_path);

    virtual ~KscCommObject();

    // selectors
    //
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
    KscServer *findServer();

    // returns negotiator which is only approbiate for
    // single access to this object
    KscNegotiator *getNegotiator();

    friend class KscDomain;
    virtual bool setProjProps(KsProjPropsHandle) = 0;

    KscAbsPath path;
    KscServer *server;
    const KscAvModule *av_module;

    PLT_DECL_RTTI;

private:
    KscCommObject(const KscCommObject &other); // forbidden
    KscCommObject &operator = (const KscCommObject &other); // forbidden

#if PLT_DEBUG
public:
    virtual void debugPrint(ostream &os) const;
#endif
};


//////////////////////////////////////////////////////////////////////
// class KscDomain
//////////////////////////////////////////////////////////////////////

typedef PltIterator<KsProjProps> KscChildIterator;

class KscDomain
: public KscCommObject
{
public:
    KscDomain(const char *domain_path);
//  KscDomain(const KscAbsPath &domain_path);

    ~KscDomain();

    KS_OBJ_TYPE typeCode() const;

    const KsDomainProjProps *getProjProps() const;

    // reread projected props 
    bool getProjPropsUpdate();
    // reread children from server, update properties
    bool getChildPPUpdate();
    // TODO: currently two iterators of the same KscDomain
    //       will lead to problems
    KscChildIterator *newChildIterator(KS_OBJ_TYPE typeMask,
                                       bool update = false);

protected:
    // remove childs from table
    bool flushChilds(KS_OBJ_TYPE typeMask);

    bool setProjProps(KsProjPropsHandle);

    KsDomainProjProps proj_props;
    PltHashTable<KscAbsPath, KscCommObject *> child_table;
    bool fChildPPValid;   // indicates wether PP's already have been read  

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
        PltHashIterator<KscAbsPath, KscCommObject *> it;
        enum_t type_mask;
    };

    PLT_DECL_RTTI;

private:
    KscDomain(const KscDomain &other); // forbidden
    KscDomain &operator = (const KscDomain &other); // forbidden

#if PLT_DEBUG
public:
    virtual void debugPrint(ostream &os) const;
#endif
};

//////////////////////////////////////////////////////////////////////
// class KscVariable
//////////////////////////////////////////////////////////////////////

class KscVariable
: public KscCommObject
{
public:
    KscVariable(const char *var_name);
//  KscVariable(const KscAbsPath &var_name);

    KS_OBJ_TYPE typeCode() const;

    bool getProjPropsUpdate();
    virtual bool getUpdate();
    virtual bool setUpdate();
    KsValueHandle getValue() const;

    const KsVarProjProps *getProjProps() const;
    const KsVarCurrProps *getCurrProps() const;
    KsCurrPropsHandle getCurrPropsHandle();
    bool setCurrProps(KsVarCurrProps &cp);
    bool isDirty() const;
   
protected:

    KsVarProjProps proj_props;
    KsVarCurrProps curr_props;

    friend class KscPackage; // for access to fDirty 
    bool fDirty;

    bool setProjProps(KsProjPropsHandle);

    PLT_DECL_RTTI;

private:
    KscVariable(const KscVariable &other); // forbidden
    KscVariable &operator = (const KscVariable &other); // forbidden

#if PLT_DEBUG
public:
    virtual void debugPrint(ostream &os) const;
#endif
};


//////////////////////////////////////////////////////////////////////
// Inline Implementation
//

inline
PltString
KscCommObject::getName() const
{
    return path.getVarPath();
}

//////////////////////////////////////////////////////////////////////

inline
KscAbsPath
KscCommObject::getHostAndServer() const
{
    return path.getHostAndServer();
}

//////////////////////////////////////////////////////////////////////

inline
const KscAbsPath &
KscCommObject::getFullPath() const
{
    return path;
}

//////////////////////////////////////////////////////////////////////

inline
void
KscCommObject::setAvModule(const KscAvModule *avm) 
{
    // don't delete old AvModule since we are not owner
    // of it
    //
    av_module = avm;
}

//////////////////////////////////////////////////////////////////////

inline
const KscAvModule *
KscCommObject::getAvModule() const
{
    return av_module;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

inline
KscDomain::KscDomain(const char *domain_path)
: KscCommObject(domain_path),
  fChildPPValid(false)
{}

//////////////////////////////////////////////////////////////////////
#if 0

inline
KscDomain::KscDomain(const KscAbsPath &domain_path)
: KscCommObject(domain_path)
{}

//////////////////////////////////////////////////////////////////////

inline
KscDomain::KscDomain(const KscDomain &other)
: KscCommObject(other),
  proj_props(other.proj_props),
  child_table(other.child_table)
{}

//////////////////////////////////////////////////////////////////////

inline
KscDomain &
KscDomain::operator = (const KscDomain &other)
{
    KscCommObject::operator = (other);

    proj_props = other.proj_props;
    child_table = other.child_table;

    return *this;
}

#endif

//////////////////////////////////////////////////////////////////////

inline
KS_OBJ_TYPE
KscDomain::typeCode() const
{
    return KS_OT_DOMAIN;
}

//////////////////////////////////////////////////////////////////////

inline
const KsDomainProjProps *
KscDomain::getProjProps() const
{
    return &proj_props;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

inline
KscVariable::KscVariable(const char *var_path)
: KscCommObject(var_path),
  fDirty(false)
{}

//////////////////////////////////////////////////////////////////////
#if 0

inline
KscVariable::KscVariable(const KscAbsPath &var_path)
: KscCommObject(var_path),
  fDirty(false)
{}

//////////////////////////////////////////////////////////////////////

inline
KscVariable::KscVariable(const KscVariable &other)
: KscCommObject(other),
  proj_props(other.proj_props),
  curr_props(other.curr_props),
  fDirty(other.fDirty)
{}

//////////////////////////////////////////////////////////////////////

inline
KscVariable &
KscVariable::operator = (const KscVariable &other)
{
    KscCommObject::operator = (other);
    
    proj_props = other.proj_props;
    curr_props = other.curr_props;
    fDirty = other.fDirty;

    return *this;
}

#endif

//////////////////////////////////////////////////////////////////////

inline
KS_OBJ_TYPE
KscVariable::typeCode() const
{
    return KS_OT_VARIABLE;
}

//////////////////////////////////////////////////////////////////////

inline
const KsVarProjProps *
KscVariable::getProjProps() const
{
    return &proj_props;
}

//////////////////////////////////////////////////////////////////////

inline
const KsVarCurrProps *
KscVariable::getCurrProps() const
{
    return &curr_props;
}

//////////////////////////////////////////////////////////////////////

inline
KsCurrPropsHandle
KscVariable::getCurrPropsHandle()
{
    return KsCurrPropsHandle(&curr_props,
                             KsOsUnmanaged);
}

//////////////////////////////////////////////////////////////////////

inline
bool
KscVariable::isDirty() const
{
    return fDirty;
}

//////////////////////////////////////////////////////////////////////
// End of  Inline Implementation
//////////////////////////////////////////////////////////////////////

#endif

//////////////////////////////////////////////////////////////////////
// EOF CommObject.h
//////////////////////////////////////////////////////////////////////










