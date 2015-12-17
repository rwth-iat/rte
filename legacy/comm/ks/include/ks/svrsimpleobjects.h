/* -*-plt-c++-*- */
#ifndef KS_SVRSIMPLEOBJECTS_INCLUDED
#define KS_SVRSIMPLEOBJECTS_INCLUDED
/* $Header: /home/david/cvs/acplt/ks/include/ks/svrsimpleobjects.h,v 1.22 2007-04-25 12:57:20 martin Exp $ */
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

/*
 * svrsimpleobjects.h: contains the not-so-abstract communication object
 *                     classes which can be instantiated and used in your
 *                     very own ACPLT/KS servers. This module is dubbed
 *                     the "server simple objects", because the classes
 *                     declared here use a simple storage mechanism to
 *                     keep their data: the free storage, also known as
 *                     the heap.
 */

/* Martin Kneissl <martin@plt.rwth-aachen.de> was here! */
/* v1+ and v2 objects added by Harald Albrecht <harald@plt.rwth-aachen.de> */

#include "ks/svrobjects.h"
#include "plt/hashtable.h"


/////////////////////////////////////////////////////////////////////////////
// forward declaration
/////////////////////////////////////////////////////////////////////////////

class KssSimpleDomainIterator;
class KssSimpleDomain;
typedef PltPtrHandle<KssSimpleDomain> KssSimpleDomainHandle;


// ----------------------------------------------------------------------------
// class KssSimpleCommObject (mixin). This is the root cause of derived
// trouble. This base/mixin class is just responsible for knowing the
// identifier, creation time and comment of an ACPLT/KS communication object
// in a server. That's all. It doesn't do anything beyond this.
//
class KssSimpleCommObject
{
    //// KssSimpleCommObject ////
    //// ctor/dtor
protected:
    KssSimpleCommObject(const KsString &id,
                        KsTime ctime = KsTime::now(),
                        KsString comment = KsString(),
                        KS_SEMANTIC_FLAGS semflags = 0);

public:
    virtual ~KssSimpleCommObject() { }

    //// KssCommObject ////
    //// accessors
    //   engineered properties
    KsString          getIdentifier() const;
    KsTime            getCreationTime() const;
    KsString          getComment() const;
    KS_SEMANTIC_FLAGS getSemanticFlags() const;

    //// modifiers
    //   engineered properties
    void setCreationTime(const KsTime &);
    void setComment(const KsString &);

private:
    KssSimpleCommObject(const KssSimpleCommObject &); // forbidden
    KssSimpleCommObject & operator = (const KssSimpleCommObject &); //forbidden
//FIXME    PltHashTable<KsString, KssCommObjectHandle> _table;

    //
    // This class is called the simple class because it just stores its
    // engineered properties in the object...and that's simple to do. More
    // "sophisticated" object classes might have to do much ado just to get
    // the engineered properties from an underlying DCS... (believe me, one
    // typically has to do *really much* ado when retrieving metainformation
    // from a DCS).
    //
    const KsString    _identifier;
    KsTime            _creation_time;
    KsString          _comment;
    KS_SEMANTIC_FLAGS _semantic_flags;

    PLT_DECL_RTTI;
}; // class KssSimpleCommObject




//////////////////////////////////////////////////////////////////////
// KssSimpleDomainIterator
//////////////////////////////////////////////////////////////////////

class KssSimpleDomainIterator
: public KssDomainIterator
{
public:
#if PLT_RETTYPE_OVERLOADABLE
    typedef KssSimpleDomainIterator THISTYPE;
    #define KssSimpleDomainIterator_THISTYPE KssSimpleDomainIterator
#else
	 #define KssSimpleDomainIterator_THISTYPE KssDomainIterator_THISTYPE
#endif

    KssSimpleDomainIterator(const KssSimpleDomain & d);
    KssSimpleDomainIterator(const PltHashTable<KsString,KssCommObjectHandle> &ht);
    virtual ~KssSimpleDomainIterator();

    virtual operator bool () const;
    virtual KssCommObjectHandle operator * () const;
    virtual THISTYPE & operator ++ ();
    virtual void toStart();
private:
    PltHashIterator<KsString,KssCommObjectHandle> _children_iter;
    KssDomainIterator * _p_sister_iter;
    bool _at_sister;
};

//////////////////////////////////////////////////////////////////////
// class KssSimpleDomain
//////////////////////////////////////////////////////////////////////

class KssSimpleDomain
: public KssDomain,
  public KssSimpleCommObject
{
    friend class KssSimpleDomainIterator;
public:
    //// KssCommObject
    //// accessors
    // projected properties
    virtual KsString  getIdentifier() const;
    virtual KsTime    getCreationTime() const;
    virtual KsString  getComment() const;
    KS_SEMANTIC_FLAGS getSemanticFlags() const;

    //// KssDomain ////
    //// accessors
    KsString getClassIdentifier() const;

    virtual KssSimpleDomainIterator_THISTYPE * newIterator() const;

    virtual KssCommObjectHandle getChildById(const KsString & id) const;

    //// KssSimpleDomain ////
    //// ctor/dtor
    KssSimpleDomain(const KsString &id,
                    KsTime ctime = KsTime::now(),
                    KsString comment = KsString());
    //// accessor
    size_t size() const;

    //// modifiers
    //   engineered properties
    void setClassIdentifier(const KsString &);

    //// modifiers
    bool                addChild(KssCommObjectHandle hChild);

    bool                addChild(KssCommObject *     pChild);
    // implicit handle creation, usual caveats

    KssCommObjectHandle removeChild(const KsString &id);

    // search path
    void setNextSister(KssDomainHandle hDomain);

    bool setNextSister(KssDomain *     pDomain);
    // implicit handle creation, usual caveats

private:
    PltHashTable<KsString, KssCommObjectHandle> _children;
    KssDomainHandle _next_sister;
    PLT_DECL_RTTI;

    KsString _class_identifier;
};


//////////////////////////////////////////////////////////////////////
// class KssSimpleVariable
//////////////////////////////////////////////////////////////////////

class KssSimpleVariable
: public KssVariable,
  public KssSimpleCommObject
{
public:
    //// KssCommObject
    //// accessors
    // projected properties
    virtual KsString  getIdentifier() const;
    virtual KsTime    getCreationTime() const;
    virtual KsString  getComment() const;
    virtual KS_ACCESS getAccessMode() const;
    KS_SEMANTIC_FLAGS getSemanticFlags() const;

    //// KssVariable ////
    //// accessors
    //   projected properties
    virtual KsString getTechUnit() const;

    //   current properties
    virtual KsValueHandle getValue() const;
    virtual KsTime        getTime() const;
    virtual KS_STATE      getState() const;

    //// modifiers
    //   projected properties
    void setTechUnit(const KsString &);

    //   current properties
    virtual KS_RESULT     setValue(const KsValueHandle &);

    virtual KS_RESULT     setValue(KsValue *p);
    //  implicit handle creation, usual caveats apply

    virtual KS_RESULT     setTime(const KsTime &);

    virtual KS_RESULT     setState(KS_STATE);

    //// KssSimpleVariable ////
    //// ctor/dtor
    KssSimpleVariable(const KsString &id,
                      KsTime ctime = KsTime::now(),
                      const KsString & comment = KsString());

    //// accessor
    bool isWriteable() const;

    //// modifier
    void lock();
    void unlock();

private:
    KS_ACCESS     _access_mode;
    KsString      _tech_unit;
    KsValueHandle _value;
    KsTime        _time;
    KS_STATE      _state;
    PLT_DECL_RTTI;
};

//////////////////////////////////////////////////////////////////////
// class KssTimeNowVariable
//////////////////////////////////////////////////////////////////////

class KssTimeNowVariable
: public KssVariable,
  public KssSimpleCommObject
{
public:
    //// KssCommObject
    //// accessors
    // projected properties
    virtual KsString getIdentifier() const;
    virtual KsTime   getCreationTime() const;
    virtual KsString getComment() const;
    virtual KS_ACCESS getAccessMode() const;
    KS_SEMANTIC_FLAGS getSemanticFlags() const;

    //// KssVariable ////
    //// accessors
    //   projected properties
    virtual KsString getTechUnit() const;

    //   current properties
    virtual KsValueHandle getValue() const;
    virtual KsTime        getTime() const;
    virtual KS_STATE      getState() const;

    //// modifiers
    //   projected properties
    void setTechUnit(const KsString &);

    //   current properties
    virtual KS_RESULT     setValue(const KsValueHandle &);

    virtual KS_RESULT     setValue(KsValue *p);
    //  implicit handle creation, usual caveats apply

    virtual KS_RESULT     setTime(const KsTime &);

    virtual KS_RESULT     setState(KS_STATE);

    //// KssSimpleVariable ////
    //// ctor/dtor
    KssTimeNowVariable(const KsString &id,
                       KsTime ctime = KsTime::now(),
                       const KsString & comment = KsString());
private:
    PLT_DECL_RTTI;
};




// ----------------------------------------------------------------------------
// The class KssSimpleLinkAlias implements an 1:m link, with an underlying
// domain as the real container.
//
class KssSimpleLinkAlias
: public KssLink,
  public KssSimpleCommObject
{
public:
    //// KssCommObject
    //// accessors
    // projected properties
    virtual KsString  getIdentifier() const;
    virtual KsTime    getCreationTime() const;
    virtual KsString  getComment() const;
    KS_SEMANTIC_FLAGS getSemanticFlags() const;

    //// KssLink/KssChildrenService ////
    //// accessors
    virtual KS_LINK_TYPE getType() const { return KS_LT_LOCAL_1_MANY; }
    virtual KsString getAssociationIdentifier() const { return KsString(); }
    virtual KsString getOppositeRoleIdentifier() const { return KsString(); }

    //// KssCurrPropsService stuff
    virtual KS_RESULT getCurrProps(KsCurrPropsHandle &hprops) const;
    virtual KsCurrPropsHandle getCurrProps() const
	{ return KsCurrPropsHandle(); }
    virtual KS_RESULT setCurrProps(const KsCurrPropsHandle &)
	{ return KS_ERR_NOACCESS; }

    //// KssChildrenService stuff
    virtual KssChildIterator_THISTYPE * newIterator() const;
    virtual KssChildIterator_THISTYPE *
        newMaskedIterator(const KsMask & name_mask,
                          KS_OBJ_TYPE type_mask) const;
    virtual KssCommObjectHandle getChildById(const KsString & id) const;
    virtual KssCommObjectHandle getChildByPath(const KsPath & path) const;

    //// KssSimpleLinkAlias ////
    //// ctor/dtor
    KssSimpleLinkAlias(KssDomainHandle hdom,
		       const KsString &id,
		       KsTime ctime = KsTime::now(),
		       const KsString & comment = KsString(),
		       const KsString & aliasPath = KsString());
private:
    KssDomainHandle _halias_domain;
    KsString _alias_path;

    PLT_DECL_RTTI;
}; // class KssSimpleLinkAlias





//////////////////////////////////////////////////////////////////////
// IMPLEMENTATION
//////////////////////////////////////////////////////////////////////

// ----------------------------------------------------------------------------
// KssSimpleCommObject
//
inline KsString
KssSimpleCommObject::getIdentifier() const
{
    return _identifier;
} // KssSimpleCommObject::getIdentifier


inline KsTime
KssSimpleCommObject::getCreationTime() const
{
    return _creation_time;
} // KssSimpleCommObject::getCreationTime


inline KsString
KssSimpleCommObject::getComment() const
{
    return _comment;
} // KssSimpleCommObject::getComment


inline KS_SEMANTIC_FLAGS
KssSimpleCommObject::getSemanticFlags() const
{
    return _semantic_flags;
} // KssSimpleCommObject::getSemanticFlags


inline void
KssSimpleCommObject::setCreationTime(const KsTime &t)
{
    _creation_time = t;
} // KssSimpleCommObject::setCreationTime


inline void
KssSimpleCommObject::setComment(const KsString &s)
{
    _comment = s;
} // KssSimpleCommObject::setComment


//////////////////////////////////////////////////////////////////////
// KssSimpleDomain
//////////////////////////////////////////////////////////////////////
// TODO should be easier through inheritance...
// forward to base
inline KsString KssSimpleDomain::getIdentifier() const
{ return KssSimpleCommObject::getIdentifier(); }

inline KsTime KssSimpleDomain::getCreationTime() const
{ return KssSimpleCommObject::getCreationTime(); }

inline KsString KssSimpleDomain::getComment() const
{ return KssSimpleCommObject::getComment(); }

inline KS_SEMANTIC_FLAGS KssSimpleDomain::getSemanticFlags() const
{ return KssSimpleCommObject::getSemanticFlags(); }

inline KsString KssSimpleDomain::getClassIdentifier() const
{ return _class_identifier; }

inline void
KssSimpleDomain::setClassIdentifier(const KsString &ci)
{
    _class_identifier = ci;
} // KssSimpleCommObject::setCreationTime

/////////////////////////////////////////////////////////////////////////////


inline KssSimpleDomainIterator_THISTYPE *
KssSimpleDomain::newIterator() const
{
    return new KssSimpleDomainIterator(*this);
}

//////////////////////////////////////////////////////////////////////

inline void
KssSimpleDomain::setNextSister(KssDomainHandle dh)
{
    _next_sister = dh;
}

//////////////////////////////////////////////////////////////////////
// KssSimpleVariable
/////////////////////////////////////////////////////////////////////
// TODO should be easier through inheritance...
// forward to base

inline KsString KssSimpleVariable::getIdentifier() const
{  return KssSimpleCommObject::getIdentifier(); }

inline KsTime KssSimpleVariable::getCreationTime() const
{ return KssSimpleCommObject::getCreationTime(); }

inline KsString KssSimpleVariable::getComment() const
{ return KssSimpleCommObject::getComment(); }

inline KS_SEMANTIC_FLAGS KssSimpleVariable::getSemanticFlags() const
{ return KssSimpleCommObject::getSemanticFlags(); }


/////////////////////////////////////////////////////////////////////////////

inline KS_ACCESS
KssSimpleVariable::getAccessMode() const
{
    return _access_mode;
}

/////////////////////////////////////////////////////////////////////////////

inline KsString
KssSimpleVariable::getTechUnit() const
{
    return _tech_unit;
}

//////////////////////////////////////////////////////////////////////

inline KsValueHandle
KssSimpleVariable::getValue() const
{
    return _value;
}

//////////////////////////////////////////////////////////////////////

inline KsTime
KssSimpleVariable::getTime() const
{
    return _time;
}

//////////////////////////////////////////////////////////////////////

inline KS_STATE
KssSimpleVariable::getState() const
{
    return _state;
}


//////////////////////////////////////////////////////////////////////

inline bool
KssSimpleVariable::isWriteable() const
{
    return _access_mode & KS_AC_WRITE;
}

//////////////////////////////////////////////////////////////////////

inline void
KssSimpleVariable::lock()
{
    _access_mode &= ~KS_AC_WRITE;
}

//////////////////////////////////////////////////////////////////////

inline void
KssSimpleVariable::unlock()
{
    _access_mode |= KS_AC_WRITE;
}

//////////////////////////////////////////////////////////////////////

inline void
KssSimpleVariable::setTechUnit(const KsString &tu)
{
    _tech_unit = tu;
}

//////////////////////////////////////////////////////////////////////

inline KS_RESULT     
KssSimpleVariable::setValue(KsValue *p) 
{
    return KssVariable::setValue(p);
}



// ----------------------------------------------------------------------------
// KssSimpleLinkAlias inline implementation stuff...
//
inline KsString
KssSimpleLinkAlias::getIdentifier() const
{
    return KssSimpleCommObject::getIdentifier();
} // KssSimpleLinkAlias::getIdentifier

inline KsTime
KssSimpleLinkAlias::getCreationTime() const
{
    return KssSimpleCommObject::getCreationTime();
} // KssSimpleLinkAlias::getCreationTime

inline KsString
KssSimpleLinkAlias::getComment() const
{
    return KssSimpleCommObject::getComment();
} // KssSimpleLinkAlias::getComment

inline KssChildIterator_THISTYPE *
KssSimpleLinkAlias::newIterator() const
{
    return (KssChildIterator_THISTYPE *) _halias_domain->newIterator();
} // KssSimpleLinkAlias::newIterator

inline KssChildIterator_THISTYPE *
KssSimpleLinkAlias::newMaskedIterator(const KsMask & name_mask, 
                                      KS_OBJ_TYPE type_mask) const
{
    return _halias_domain->newMaskedIterator(name_mask, type_mask);
} // KssSimpleLinkAlias::newMaskedIterator

inline KssCommObjectHandle
KssSimpleLinkAlias::getChildById(const KsString & id) const
{
    return _halias_domain->getChildById(id);
} // KssSimpleLinkAlias::getChildById

inline KssCommObjectHandle
KssSimpleLinkAlias::getChildByPath(const KsPath & path) const
{
    return _halias_domain->getChildByPath(path);
} // KssSimpleLinkAlias::getChildByPath

inline KS_SEMANTIC_FLAGS
KssSimpleLinkAlias::getSemanticFlags() const
{ return KssSimpleCommObject::getSemanticFlags(); }



//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

inline
KssTimeNowVariable::KssTimeNowVariable(const KsString &id,
                                     KsTime ctime,
                                     const KsString &comment)
: KssSimpleCommObject(id, ctime, comment)
{
}

//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// KssTimeNowVariable
/////////////////////////////////////////////////////////////////////
// TODO should be easier through inheritance...
// forward to base

inline KsString KssTimeNowVariable::getIdentifier() const
{  return KssSimpleCommObject::getIdentifier(); }

inline KsTime KssTimeNowVariable::getCreationTime() const
{ return KssSimpleCommObject::getCreationTime(); }

inline KsString KssTimeNowVariable::getComment() const
{ return KsString("current time"); }

inline KS_SEMANTIC_FLAGS KssTimeNowVariable::getSemanticFlags() const
{ return KssSimpleCommObject::getSemanticFlags(); }


/////////////////////////////////////////////////////////////////////////////

inline KS_ACCESS
KssTimeNowVariable::getAccessMode() const
{
    return KS_AC_READ;
}

//////////////////////////////////////////////////////////////////////

inline KsString
KssTimeNowVariable::getTechUnit() const
{
    return KsString("UTC");
}

//////////////////////////////////////////////////////////////////////

inline KsTime
KssTimeNowVariable::getTime() const
{
    return KsTime::now();
}

//////////////////////////////////////////////////////////////////////

inline KS_STATE
KssTimeNowVariable::getState() const
{
    return KS_ST_GOOD;
}


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

inline KS_RESULT     
KssTimeNowVariable::setValue(KsValue *) 
{
    return KS_ERR_NOACCESS;
}


//////////////////////////////////////////////////////////////////////
inline KS_RESULT
KssTimeNowVariable::setValue(const KsValueHandle &)
{
    return KS_ERR_NOACCESS;
}

//////////////////////////////////////////////////////////////////////

inline KS_RESULT
KssTimeNowVariable::setTime(const KsTime &)
{
    return KS_ERR_NOACCESS;
}

//////////////////////////////////////////////////////////////////////

inline KS_RESULT
KssTimeNowVariable::setState(KS_STATE) 
{
    return KS_ERR_NOACCESS;
}

/////////////////////////////////////////////////////////////////////////////
#endif // KS_SVROBJECTS_INCLUDED

/* End of ks/svrsimpleobjects.h */
