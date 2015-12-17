/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/include/ks/commobject.h,v 1.35 2007-04-25 12:57:20 martin Exp $ */
#ifndef KSC_COMMOBJECT_INCLUDED 
#define KSC_COMMOBJECT_INCLUDED
/*
 * Copyright (c) 1996, 1997, 1998, 1999
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
/* v1+ stuff added by: Harald Albrecht <harald@plt.rwth-aachen.de> */


#include <plt/debug.h>
#include <plt/rtti.h>
#include <plt/handle.h>
#include <plt/list.h>

#include "ks/props.h"
#include "ks/propsv1.h"
#include "ks/clntpath.h"
#include "ks/avmodule.h"


// ---------------------------------------------------------------------------
// Some forward declarations. Some stuff seems to be necessary everwhere...
//
class KscServerBase;

class KsGetEPResult;

typedef PltIterator<KsEngPropsHandle> KscChildIterator;


// ---------------------------------------------------------------------------
// class KscCommObject: this abstract base class provides a basic infra-
// structure to build specific ACPLT/KS communication objects on the client
// side from. If you just have a resource locator but don't have any clue
// of what what kind of object it references, then use the KscAnyCommObject
// class instead. This KscCommObject class will stay abstract.
//
class KscCommObject
{
public:
    KscCommObject(const char *object_path);
    virtual ~KscCommObject();

    //
    // selectors
    //
    bool hasValidPath() const;
    KsString getName() const;
    bool isNamePart() const;
    KsString getPathOnly() const;
    const KscPath &getPathAndName() const;
    KsString getHostAndServer() const;
    KsString getFullPath() const;

    virtual KS_OBJ_TYPE typeCode() const = 0;

    KscServerBase *getServer() const;

    virtual const KsEngProps_THISTYPE *getEngProps() const = 0;
    virtual bool getEngPropsUpdate() = 0;

#if 0 // PLT_SOURCE_V1_BC
    virtual const KsProjProps_THISTYPE *getProjProps() const = 0;
    virtual bool getProjPropsUpdate() { return getEngPropsUpdate(); }
#endif

    virtual void setAvModule(const KscAvModule *avm);
    virtual const KscAvModule *getAvModule() const;

    KS_RESULT getLastResult() const;

    //
    // Communication objects are considered equal if they occupy
    // the same memory location. Other relational operators are
    // not supported and make in fact no sense here. Yes, we've
    // already learned *that* the hard way, believe us...
    //
    bool operator == (const KscCommObject &) const;
    bool operator != (const KscCommObject &) const;

protected:
    KscServerBase *findServer();

    // returns negotiator which is only apropriate for
    // single access to this object
    KscNegotiator *getNegotiator();

    friend class KscDomain;
    virtual bool setEngProps(KsEngPropsHandle) = 0;

    KscPathParser path;
    KscServerBase *server;
    const KscAvModule *av_module;
    KS_RESULT _last_result;


    //
    // Get iterator -- this is implemented here as protected on purpose.
    // This stuff is needed in several derived classes, so we want to do
    // it only once. On the other hand, this stuff should only be used
    // if it makes sense (whatever *that* now means...)
    //
    KscChildIterator *newChildIterator_(KS_OBJ_TYPE typeMask,
					KS_EP_FLAGS scopeFlags = KS_EPF_DEFAULT,
                                        KsString nameMask = KsString("*"));

    //
    // Now the next one belongs to the stuff above: read children from
    // server and return their engineered properties in list.
    //
    bool getChildEPUpdate(KS_OBJ_TYPE typeMask,
			  KS_EP_FLAGS scopeFlags,
                          KsString nameMask,
			  KsGetEPResult *&result);
    //
    // This protected iterator class is responsible for iterating over the
    // child list of a domain communication object. The trick here is that
    // the iterator is directly connected with the result object from a
    // GetPP service call, so we don't need to copy the list. Instead, the
    // iterator destroys the result object when it gets destroyed itself.
    //
    class ChildIterator
    : public PltListIterator<KsEngPropsHandle>
    {
    public:
#if PLT_RETTYPE_OVERLOADABLE
#define KscCommObject_ChildIterator_THISTYPE KscCommObject::ChildIterator
        typedef ChildIterator THISTYPE;
#else
#define KscCommObject_ChildIterator_THISTYPE PltListIterator_THISTYPE(KsEngPropsHandle)
#endif
        ChildIterator(KsGetEPResult &getEPResult);
        ~ChildIterator();
    private:
        KsGetEPResult &_getEP_result;
    };

    friend class ChildIterator;


    //
    // Ah, yes... the RTTI stuff.
    //
    PLT_DECL_RTTI;

private:
    KscCommObject(const KscCommObject &other); // forbidden
    KscCommObject &operator = (const KscCommObject &other); // forbidden

#if PLT_DEBUG
public:
    virtual void debugPrint(ostream &os) const;
#endif
}; // class KscCommObject;


typedef PltPtrHandle<KscCommObject> KscCommObjectHandle;


// ---------------------------------------------------------------------------
// sorry, inline comp ops need to be defined here to avoid compiler errors.
//
inline bool
KscCommObject::operator == (const KscCommObject &other) const
{
    return this == &other;
} // KscCommObject::operator==


inline bool
KscCommObject::operator != (const KscCommObject &other) const
{
    return this != &other;
} // KscCommObject::operator!=


// ---------------------------------------------------------------------------
// class KscAnyCommObject: not really an ACPLT/KS communication object (proxy)
// but rather some swiss army knive kind of object: it provides access to the
// engineered and current properties and also to its children as long as you
// just have a valid resource locator. The KscAnyCommObject can be used if
// you have a resource locator but no clue what kind of object is hidden
// behind the locator.
//
class KscAnyCommObject : public KscCommObject
{
public:
    KscAnyCommObject(const char *object_path);
    ~KscAnyCommObject();

    virtual KS_OBJ_TYPE typeCode() const;

    const KsEngProps_THISTYPE *getEngProps() const;
#if 0 // PLT_SOURCE_V1_BC
    virtual const KsProjProps_THISTYPE *getProjProps() const;
#endif

    // reread projected props
    bool getEngPropsUpdate();

    // get iterator
    KscChildIterator *newChildIterator(KS_OBJ_TYPE typeMask,
				       KS_EP_FLAGS scopeFlags = KS_EPF_DEFAULT,
                                       KsString nameMask = KsString("*"));

protected:
    bool setEngProps(KsEngPropsHandle h);

    KsEngPropsHandle _heng_props;

    PLT_DECL_RTTI;

private:
    KscAnyCommObject(const KscAnyCommObject &other); // forbidden
    KscAnyCommObject &operator = (const KscAnyCommObject &other); // forbidden

#if PLT_DEBUG
public:
    virtual void debugPrint(ostream &os) const;
#endif
}; // class KscAnyCommObject


// ---------------------------------------------------------------------------
// class KscDomain: represents an ACPLT/KS domain object in an ACPLT/KS
// server. Can be used to query the children of a domain or the engineered
// properties of the domain itself.
//
class KscDomain : public KscCommObject
{
public:
    KscDomain(const char *domain_path);
    ~KscDomain();

    KS_OBJ_TYPE typeCode() const;

    const KsDomainEngProps_THISTYPE *getEngProps() const;

    // reread projected props
    bool getEngPropsUpdate();

    // get iterator
    KscChildIterator *newChildIterator(KS_OBJ_TYPE typeMask,
				       KS_EP_FLAGS scopeFlags = KS_EPF_DEFAULT,
                                       KsString nameMask = KsString("*"));

protected:
    bool setEngProps(KsEngPropsHandle);

    KsDomainEngProps eng_props;

    PLT_DECL_RTTI;

private:
    KscDomain(const KscDomain &other); // forbidden
    KscDomain &operator = (const KscDomain &other); // forbidden

#if PLT_DEBUG
public:
    virtual void debugPrint(ostream &os) const;
#endif
}; // class KscDomain

typedef PltPtrHandle<KscDomain> KscDomainHandle;


#if 0
// ---------------------------------------------------------------------------
// class KscLink:
//
class KscLink : public KscCommObject
{
public:
    KscLink(const char *link_name);
    ~KscLink();

    virtual KS_OBJ_TYPE typeCode() const;

    bool getEngPropsUpdate();
    virtual bool getUpdate();
    KsValueHandle getValue() const;
    const KsVarEngProps_THISTYPE *getEngProps() const;
    const KsVarCurrProps *getCurrProps() const;
    KsCurrPropsHandle getCurrPropsHandle();

    KscChildIterator *newChildIterator(KS_OBJ_TYPE typeMask,
                                       KsString nameMask = KsString("*"));

protected:
    bool setEngProps(KsEngPropsHandle h);

    KsLinkEngProps _proj_props;
    KsVarCurrProps  _curr_props;

    PLT_DECL_RTTI;

private:
    KscLink(const KscLink &other); // forbidden
    KscLink &operator = (const KscLink &other); // forbidden

#if PLT_DEBUG
public:
    virtual void debugPrint(ostream &os) const;
#endif
}; // class KscLink

typedef PltPtrHandle<KscLink> KscLinkHandle;
#endif


// ---------------------------------------------------------------------------
// class KscVariable
//
class KscVariable : public KscCommObject
{
public:
    KscVariable(const char *var_name);

    virtual KS_OBJ_TYPE typeCode() const;

    bool getEngPropsUpdate();
    virtual bool getUpdate();
    virtual bool setUpdate();
    KsValueHandle getValue() const;

    const KsVarEngProps_THISTYPE *getEngProps() const;
    const KsVarCurrProps *getCurrProps() const;
    KsCurrPropsHandle getCurrPropsHandle();
    bool setCurrProps(KsVarCurrProps &cp);
    bool isDirty() const;

protected:

    KsVarEngProps eng_props;
    KsVarCurrProps curr_props;

    friend class _KscPackageBase; // for access to fDirty
    bool fDirty;

    bool setEngProps(KsEngPropsHandle);

    PLT_DECL_RTTI;

private:
    KscVariable(const KscVariable &other); // forbidden
    KscVariable &operator = (const KscVariable &other); // forbidden

#if PLT_DEBUG
public:
    virtual void debugPrint(ostream &os) const;
#endif
}; // class KscVariable

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
    return path.getType() == KSC_PT_RESOURCELOCATOR;
}

//////////////////////////////////////////////////////////////////////

inline
KscServerBase *
KscCommObject::getServer() const
{
    return server;
}

//////////////////////////////////////////////////////////////////////

inline
KsString
KscCommObject::getName() const
{
    return path.getName();
}


inline
bool
KscCommObject::isNamePart() const
{
    return path.isNamePart();
} // KscCommObject::isNamePart


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


// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// class KscAnyCommObject inline stuff...
//
inline
KscAnyCommObject::KscAnyCommObject(const char *object_path)
    : KscCommObject(object_path)
{
} // KscAnyCommObject::KscAnyCommObject


inline
KS_OBJ_TYPE
KscAnyCommObject::typeCode() const
{
    return 0; // we don't have a valid type code!
} // KscAnyCommObject::typeCode


inline
const KsEngProps_THISTYPE *
KscAnyCommObject::getEngProps() const
{
    return _heng_props.getPtr();
} // KscAnyCommObject::getEngProps


// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// class KscDomain inline stuff...
//
inline
KscDomain::KscDomain(const char *domain_path)
: KscCommObject(domain_path)
{}


inline
KS_OBJ_TYPE
KscDomain::typeCode() const
{
    return KS_OT_DOMAIN;
}


inline
const KsDomainEngProps_THISTYPE *
KscDomain::getEngProps() const
{
    return &eng_props;
}


// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// class KscLink inline stuff...
//
#if 0
inline
KscLink::KscLink(const char *link_path)
: KscCommObject(link_path)
{} // KscLink::KscLink
#endif


// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// class KscVariable inline stuff...
//
inline
KscVariable::KscVariable(const char *var_path)
: KscCommObject(var_path),
  fDirty(false)
{}


inline
KS_OBJ_TYPE
KscVariable::typeCode() const
{
    return KS_OT_VARIABLE;
}


inline
const KsVarEngProps_THISTYPE *
KscVariable::getEngProps() const
{
    return &eng_props;
}


inline
KsValueHandle
KscVariable::getValue() const
{
    return curr_props.value; 
}


inline
const KsVarCurrProps *
KscVariable::getCurrProps() const
{
    return &curr_props;
}


inline
KsCurrPropsHandle
KscVariable::getCurrPropsHandle()
{
    return KsCurrPropsHandle(new KsVarCurrProps(curr_props),
                             KsOsNew);
}


inline
bool
KscVariable::isDirty() const
{
    return fDirty;
}


#endif

// End of ks/commobject.h
