/* -*-plt-c++-*- */
#ifndef KS_SVRSIMPLEOBJECTS_INCLUDED
#define KS_SVRSIMPLEOBJECTS_INCLUDED
/* $Header: /home/david/cvs/acplt/ks/include/ks/svrsimpleobjects.h,v 1.9 1997-09-05 11:04:58 markusj Exp $ */
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

#include "ks/svrobjects.h"
#include "plt/hashtable.h"
/////////////////////////////////////////////////////////////////////////////
// forward declaration
/////////////////////////////////////////////////////////////////////////////

class KssSimpleDomainIterator;
class KssSimpleDomain;
typedef PltPtrHandle<KssSimpleDomain> KssSimpleDomainHandle;

//////////////////////////////////////////////////////////////////////
// class KssSimpleCommObject (mixin)
//////////////////////////////////////////////////////////////////////

class KssSimpleCommObject
{
    //// KssSimpleCommObject ////
    //// ctor/dtor
protected:
    KssSimpleCommObject(const KsString &id,
                        KsTime ctime = KsTime::now(),
                        KsString comment = KsString());
public:
    virtual ~KssSimpleCommObject() { }

    //// accessors
    // projected properties
    KsString  getIdentifier() const;
    KsTime    getCreationTime() const;
    KsString  getComment() const;

	//// modifiers
    // projected properties
    void setCreationTime(const KsTime &);
    void setComment(const KsString &);

private:
    KssSimpleCommObject(const KssSimpleCommObject &); // forbidden
    KssSimpleCommObject & operator = (const KssSimpleCommObject &); //forbidden
    PltHashTable<KsString, KssCommObjectHandle> _table;
    const KsString _identifier;
    KsTime         _creation_time;
    KsString       _comment;

    PLT_DECL_RTTI;
};

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
    virtual KsString getIdentifier() const;
    virtual KsTime   getCreationTime() const;
    virtual KsString getComment() const;

    //// KssDomain ////
    //// accessors
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

//////////////////////////////////////////////////////////////////////
// IMPLEMENTATION
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// KssSimpleCommObject
//////////////////////////////////////////////////////////////////////

inline KsString
KssSimpleCommObject::getIdentifier() const
{
    return _identifier;
}

//////////////////////////////////////////////////////////////////////

inline KsTime
KssSimpleCommObject::getCreationTime() const
{
    return _creation_time;
}

//////////////////////////////////////////////////////////////////////

inline KsString
KssSimpleCommObject::getComment() const
{
    return _comment;
}

//////////////////////////////////////////////////////////////////////

inline void
KssSimpleCommObject::setCreationTime(const KsTime &t)
{
    _creation_time = t;
}

//////////////////////////////////////////////////////////////////////

inline void
KssSimpleCommObject::setComment(const KsString &s)
{
    _comment = s;
}

//////////////////////////////////////////////////////////////////////
// KssSimpleDomain
//////////////////////////////////////////////////////////////////////
// TODO should be easier through inheritance...
// forward to base
inline KsString KssSimpleDomain::getIdentifier() const
{  return KssSimpleCommObject::getIdentifier(); }

inline KsTime KssSimpleDomain::getCreationTime() const
{ return KssSimpleCommObject::getCreationTime(); }

inline KsString KssSimpleDomain::getComment() const
{ return KssSimpleCommObject::getComment(); }

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
{ return KsString("Current time"); }

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
