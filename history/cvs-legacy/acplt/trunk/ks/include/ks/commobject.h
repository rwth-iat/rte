/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/include/ks/commobject.h,v 1.25 1998-01-12 07:49:24 harald Exp $ */

#ifndef KSC_COMMOBJECT_INCLUDED 
#define KSC_COMMOBJECT_INCLUDED

/*
 * Copyright (c) 1996, 1997, 1998
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
#include <plt/handle.h>
#include <plt/list.h>

#include "ks/props.h"
#include "ks/clntpath.h"
#include "ks/avmodule.h"

//////////////////////////////////////////////////////////////////////
// forward declarations
class KscServerBase;

//////////////////////////////////////////////////////////////////////
// class KscCommObject
//////////////////////////////////////////////////////////////////////

class KscCommObject
{
public:
    KscCommObject(const char *object_path);
    virtual ~KscCommObject();

    // selectors
    //
    bool hasValidPath() const;
    KsString getName() const;
    KsString getPathOnly() const;
    const KscPath &getPathAndName() const;
    KsString getHostAndServer() const;
    KsString getFullPath() const;

    virtual KS_OBJ_TYPE typeCode() const = 0;

    KscServerBase *getServer() const;

    virtual const KsProjProps_THISTYPE *getProjProps() const = 0;
    virtual bool getProjPropsUpdate() = 0;

    virtual void setAvModule(const KscAvModule *avm);
    virtual const KscAvModule *getAvModule() const;

    KS_RESULT getLastResult() const;

    //
    // Communication objects are considered equal if they occupy
    // the same memory location. Other relational operators are
    // not supported and make in fact no sense here. Yes, we've
    // already learned that the hard way, believe us...
    //
    bool operator == (const KscCommObject &) const;
    bool operator != (const KscCommObject &) const;

protected:
    KscServerBase *findServer();

    // returns negotiator which is only apropriate for
    // single access to this object
    KscNegotiator *getNegotiator();

    friend class KscDomain;
    virtual bool setProjProps(KsProjPropsHandle) = 0;

    KscPathParser path;
    KscServerBase *server;
    const KscAvModule *av_module;
    KS_RESULT _last_result;

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
// sorry, inline comp ops need to be defined here to avoid
// compiler errors
//
inline bool
KscCommObject::operator == (const KscCommObject &other) const
{
    return this == &other;
}

//////////////////////////////////////////////////////////////////////

inline bool
KscCommObject::operator != (const KscCommObject &other) const
{
    return this != &other;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

typedef PltPtrHandle<KscCommObject> KscCommObjectHandle;

//////////////////////////////////////////////////////////////////////
// class KscDomain
//////////////////////////////////////////////////////////////////////

class KsGetPPResult;

typedef PltIterator<KsProjPropsHandle> KscChildIterator;

class KscDomain
: public KscCommObject
{
public:
    KscDomain(const char *domain_path);

    ~KscDomain();

    KS_OBJ_TYPE typeCode() const;

    const KsDomainProjProps_THISTYPE *getProjProps() const;

    // reread projected props
    bool getProjPropsUpdate();
    // get iterator, 
    // if update is set to true, it forces the PP's to be read and
    // all other iterators related to this object will become invalid
    KscChildIterator *newChildIterator(KS_OBJ_TYPE typeMask,
                                       KsString nameMask = KsString("*"));

protected:
    // read children from server, returns PP's in list
    bool getChildPPUpdate(KS_OBJ_TYPE typeMask,
                          KsString nameMask,
			  KsGetPPResult *&result);

    bool setProjProps(KsProjPropsHandle);

    KsDomainProjProps proj_props;

    //
    // This protected iterator class is responsible for iterating over the
    // child list of a domain communication object. The trick here is that
    // the iterator is directly connected with the result object from a
    // GetPP service call, so we don't need to copy the list. Instead, the
    // iterator destroys the result object when it gets destroyed itself.
    //
    class ChildIterator
    : public PltListIterator<KsProjPropsHandle>
    {
    public:
#if PLT_RETTYPE_OVERLOADABLE
# define KscDomain_ChildIterator_THISTYPE KscDomain::ChildIterator
        typedef ChildIterator THISTYPE;
#else
# define KscDomain_ChildIterator_THISTYPE PltListIterator_THISTYPE(KsProjPropsHandle)
#endif
        ChildIterator(KsGetPPResult &getPPResult);
        ~ChildIterator();
    private:
        KsGetPPResult &_getPP_result;
    };

    friend class ChildIterator;

    PLT_DECL_RTTI;

private:
    KscDomain(const KscDomain &other); // forbidden
    KscDomain &operator = (const KscDomain &other); // forbidden

#if PLT_DEBUG
public:
    virtual void debugPrint(ostream &os) const;
#endif
};

typedef PltPtrHandle<KscDomain> KscDomainHandle;

//////////////////////////////////////////////////////////////////////
// class KscVariable
//////////////////////////////////////////////////////////////////////

class KscVariable
: public KscCommObject
{
public:
    KscVariable(const char *var_name);

    KS_OBJ_TYPE typeCode() const;

    bool getProjPropsUpdate();
    virtual bool getUpdate();
    virtual bool setUpdate();
    KsValueHandle getValue() const;

    const KsVarProjProps_THISTYPE *getProjProps() const;
    const KsVarCurrProps *getCurrProps() const;
    KsCurrPropsHandle getCurrPropsHandle();
    bool setCurrProps(KsVarCurrProps &cp);
    bool isDirty() const;

protected:

    KsVarProjProps proj_props;
    KsVarCurrProps curr_props;

    friend class _KscPackageBase; // for access to fDirty
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

typedef PltPtrHandle<KscVariable> KscVariableHandle;


//////////////////////////////////////////////////////////////////////
// Inline Implementation
//////////////////////////////////////////////////////////////////////
// class KscCommObject
//////////////////////////////////////////////////////////////////////

inline
bool
KscCommObject::hasValidPath() const
{
    return path.isValid();
}

//////////////////////////////////////////////////////////////////////

inline
KsString
KscCommObject::getName() const
{
    return path.getName();
}

//////////////////////////////////////////////////////////////////////

inline
KsString
KscCommObject::getPathOnly() const
{
    return path.getPathOnly();
}

//////////////////////////////////////////////////////////////////////

inline
const KscPath &
KscCommObject::getPathAndName() const
{
    return path.getPathAndName();
}

//////////////////////////////////////////////////////////////////////

inline
KsString
KscCommObject::getHostAndServer() const
{
    return path.getHostAndServer();
}

//////////////////////////////////////////////////////////////////////

inline
KsString
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

inline
KS_RESULT
KscCommObject::getLastResult() const
{
    return _last_result;
}

//////////////////////////////////////////////////////////////////////
// class KscDomain
//////////////////////////////////////////////////////////////////////

inline
KscDomain::KscDomain(const char *domain_path)
: KscCommObject(domain_path)
{}

//////////////////////////////////////////////////////////////////////

inline
KS_OBJ_TYPE
KscDomain::typeCode() const
{
    return KS_OT_DOMAIN;
}

//////////////////////////////////////////////////////////////////////

inline
const KsDomainProjProps_THISTYPE *
KscDomain::getProjProps() const
{
    return &proj_props;
}

//////////////////////////////////////////////////////////////////////
// class KscVariable
//////////////////////////////////////////////////////////////////////

inline
KscVariable::KscVariable(const char *var_path)
: KscCommObject(var_path),
  fDirty(false)
{}

//////////////////////////////////////////////////////////////////////

inline
KS_OBJ_TYPE
KscVariable::typeCode() const
{
    return KS_OT_VARIABLE;
}

//////////////////////////////////////////////////////////////////////

inline
const KsVarProjProps_THISTYPE *
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
    return KsCurrPropsHandle(new KsVarCurrProps(curr_props),
                             KsOsNew);
}

//////////////////////////////////////////////////////////////////////

inline
bool
KscVariable::isDirty() const
{
    return fDirty;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

#endif

//////////////////////////////////////////////////////////////////////
// EOF CommObject.h
//////////////////////////////////////////////////////////////////////






