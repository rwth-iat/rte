/* -*-plt-c++-*- */
#ifndef KS_SVROBJECTS_INCLUDED
#define KS_SVROBJECTS_INCLUDED
/* $Header: /home/david/cvs/acplt/ks/include/ks/svrobjects.h,v 1.13 1999-09-06 06:59:24 harald Exp $ */
/*
 * Copyright (c) 1996, 1997, 1998, 1999
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

/*
 * svrobjects.h: contains the abstract classes which represent communication
 *               objects within ACPLT/KS. In addition, this module also
 *               defines some basic interfaces which apply to retrieving
 *               children/parts of a communication object and retrieving or
 *               setting the current properties of a variable etc.
 */

/* Martin Kneissl <martin@plt.rwth-aachen.de> was here! */
/* v1+ and v2 objects added by Harald Albrecht <harald@plt.rwth-aachen.de> */


#include "plt/rtti.h"
#include "plt/container.h"
#include "ks/handle.h"
#include "ks/ks.h"
#include "ks/string.h"
#include "ks/time.h"
#include "ks/value.h"
#include "ks/path.h"
#include "ks/mask.h"
#include "ks/props.h"


// ----------------------------------------------------------------------------
// class KssCommObject: it's the parent of all communication objects within an
// ACPLT/KS server (if you're using the proxy and/or static object system
// implemented here and in other source files). All that is guarantueed you
// can do on *any* communication object is to ask for its engineered props,
// like name, creation time, comment and access mode. In addition, you have
// the chance to find out what kind of ACPLT/KS communication is really
// dangling at your pointer/handle through the typeCode() method, which
// returns exactly one out of the KS_OT_xxx constants.
//
// The bottom line about KssCommObject is that it is really, ahem, abstract:
// it just provides a generic interface to all communication objects.
//
class KssCommObject
{
public:
    //
    // ctor/dtor area
    //
    virtual ~KssCommObject() { }
    virtual KS_OBJ_TYPE_ENUM typeCode() const = 0; 
    // ^ optimization in absence of true RTTI... by looking at the typeCode
    // of a communication object in the server it's easy to deduct what kind
    // of object we have dangling at the end of a handle.
    //

    //
    // access to the engineered properties...
    //
    virtual KsString          getIdentifier() const = 0;
    virtual KsTime            getCreationTime() const = 0;
    virtual KsString          getComment() const = 0;
    virtual KS_ACCESS         getAccessMode() const = 0;
    virtual KS_SEMANTIC_FLAGS getSemanticFlags() const = 0;
    virtual KsEngPropsHandle  getEP() const = 0;

    PLT_DECL_RTTI;
}; // class KssCommObj

typedef PltPtrHandle<KssCommObject> KssCommObjectHandle;


// ----------------------------------------------------------------------------
// But now for the services which can be provided by various derived classes
// of the KssCommClass. These services are more or less the services requested
// by ACPLT/KS clients and are helpful for implementing these services in
// ACPLT/KS servers, especially the mis-nomed "simple server" class.
//
// These basic (abstract?) services are:
// - KssScanChildrenService:
//   Find out what children a communication object has.
// - KssCurrPropsService:
//   Retrieve or set the current properties (that is, the value) of an object.
//
// The various object classes provide one or more of these specialized
// "services":
// - KssDomain:    KssChildrenService
// - KssVariable:  KssCurrPropsService
// - KssLink:      KssChildrenService, KssCurrPropsService
// - KssHistory:   KssChildrenService
// - KssStructure: KssChildrenService, KssCurrPropsService
//

// ----------------------------------------------------------------------------
// Class KssCurrPropsService indicates that a particular communication object
// class supports reading and/or/whatever writing of its current properties.
//
// NOTE: getCurrProps() without arguments is depreciated! Use the other one
//       instead, which can return an error code.
//
class KssCurrPropsService {
public:
    virtual KS_RESULT getCurrProps(KsCurrPropsHandle &hprops) const;
    virtual KsCurrPropsHandle getCurrProps() const = 0;
    virtual KS_RESULT setCurrProps(const KsCurrPropsHandle &hprops) = 0;

    PLT_DECL_RTTI;
}; // class KssCurrPropsService


// ----------------------------------------------------------------------------
// Class KssChildIterator
//
#define KssChildIterator_THISTYPE PltHandleIterator_THISTYPE(KssCommObject)
typedef PltHandleIterator<KssCommObject> KssChildIterator;


// ----------------------------------------------------------------------------
// Class KssChildrenService (which is an interface)
//
class KssChildrenService {
public:
    virtual KssChildIterator_THISTYPE *
        newMaskedIterator(const KsMask & name_mask,
                          KS_OBJ_TYPE type_mask) const = 0;
    virtual KssChildIterator_THISTYPE *
        newIterator() const = 0;
    virtual KssCommObjectHandle getChildById(const KsString & id) const = 0;
    virtual KssCommObjectHandle getChildByPath(const KsPath & path) const = 0;

    PLT_DECL_RTTI;
}; // class KssChildrenService


// ----------------------------------------------------------------------------
// Class KssDomainIterator. This is for backwards compatibility with old source
// code only. The use of these identifiers is depreciated, use the
// KssChildIterator stuff instead.
//
#define KssDomainIterator_THISTYPE KssChildIterator_THISTYPE
typedef KssChildIterator KssDomainIterator;


// ----------------------------------------------------------------------------
// class KssDomain
//
class KssDomain
: public KssCommObject, public KssChildrenService,
  public PltHandleContainer<KssCommObject>
{
public:
    //// KssCommObject
    //   accessors
    virtual KS_OBJ_TYPE_ENUM typeCode() const { return KS_OT_DOMAIN; }

    //   engineered properties
    virtual KsString          getIdentifier() const = 0;
    virtual KsTime            getCreationTime() const = 0;
    virtual KsString          getComment() const = 0;
    virtual KS_ACCESS         getAccessMode() const { return KS_AC_READ; }
    virtual KS_SEMANTIC_FLAGS getSemanticFlags() const = 0;
    virtual KsEngPropsHandle  getEP() const;

    //// KssDomain ////
    //// accessors
    //   engineered properties
    virtual KsString          getClassIdentifier() const = 0;

    //   children
    virtual KssDomainIterator_THISTYPE *
        newIterator() const = 0;
    virtual KssDomainIterator_THISTYPE *
        newMaskedIterator(const KsMask & name_mask,
                          KS_OBJ_TYPE type_mask)
            const;

    virtual KssCommObjectHandle getChildById(const KsString & id) const;
    // ^ defaults to linear search on iterator,
    //   you should implement a more efficient version!!!

    virtual KssCommObjectHandle getChildByPath(const KsPath & path) const;

    PLT_DECL_RTTI;
}; // class KssDomain
typedef PltPtrHandle<KssDomain> KssDomainHandle;


// ----------------------------------------------------------------------------
// class KssMaskedIterator
//
class KssMaskedDomainIterator
: public KssDomainIterator
{
public:
#if PLT_RETTYPE_OVERLOADABLE
    typedef KssMaskedDomainIterator THISTYPE;
    #define KssMaskedDomainIterator_THISTYPE KssMaskedDomainIterator
#else
    #define KssMaskedDomainIterator_THISTYPE KssDomainIterator_THISTYPE
#endif

    KssMaskedDomainIterator(const PltPtrHandle<KssDomainIterator> & hit,
                            const KsMask & name_mask,
                            const KS_OBJ_TYPE type_mask);
    virtual operator bool () const;
    virtual KssCommObjectHandle operator * () const;
    virtual THISTYPE & operator ++ ();
    virtual void toStart();

private:
    void skipWhileNotMatching();
    KsMask _name_mask;
    KS_OBJ_TYPE _type_mask;
    PltPtrHandle<KssDomainIterator> _hit; // used to hold the refence
    KssDomainIterator & _it; // used for access
}; // class KssMaskedDomainIterator


// ----------------------------------------------------------------------------
// Class KssVariable: provides a generic interface to ACPLT/KS variables. As
// this is only an abstract class, you need to derive from it and implement
// the needed functionality. This interface is kept as flexible as possible
// while making speed optimizations easily possible, if such optimizations
// can be done.
//
class KssVariable
: public KssCommObject, public KssCurrPropsService
{
public:
    //// KssCommObject
    ///  accessors
    virtual KS_OBJ_TYPE_ENUM typeCode() const { return KS_OT_VARIABLE; }

    //   engineered properties
    virtual KsString          getIdentifier() const = 0;
    virtual KsTime            getCreationTime() const = 0;
    virtual KsString          getComment() const = 0;
    virtual KS_ACCESS         getAccessMode() const = 0;
    virtual KS_SEMANTIC_FLAGS getSemanticFlags() const = 0;
    virtual KsEngPropsHandle  getEP() const;

    //// KssVariable ////
    //// accessors
    //   engineered properties
    virtual KsString          getTechUnit() const = 0;
    virtual KS_VAR_TYPE       getType() const;

    //   current properties
    virtual KsValueHandle     getValue() const = 0;
    virtual KsTime            getTime() const;
    virtual KS_STATE          getState() const;

    virtual KsCurrPropsHandle getCurrProps() const;

    //// modifiers
    //   current properties
    virtual KS_RESULT     setValue(const KsValueHandle &) = 0;

    virtual KS_RESULT     setValue(KsValue *p);
    //  implicit handle creation, usual caveats apply

    virtual KS_RESULT     setTime(const KsTime &) = 0;

    virtual KS_RESULT     setState(KS_STATE) = 0;

    virtual KS_RESULT     setCurrProps(KsVarCurrProps &props);
    virtual KS_RESULT     setCurrProps(const KsCurrPropsHandle & hprops);

    PLT_DECL_RTTI;
}; // class KssVariable


// ----------------------------------------------------------------------------
// class KssLink: ACPLT/KS links can be references to other communication
// objects with either local scope (naming scope) or global scope (within the
// same ACPLT/KS server). In addition, they can be either 1:1 or 1:many links.
//
class KssLink :
public KssCommObject,
public KssChildrenService, public KssCurrPropsService
{
public:
    //// KssCommObject
    ///  accessors
    virtual KS_OBJ_TYPE_ENUM typeCode() const { return KS_OT_LINK; }

    //   engineered properties
    virtual KsString          getIdentifier() const = 0;
    virtual KsTime            getCreationTime() const = 0;
    virtual KsString          getComment() const = 0;
    virtual KS_ACCESS         getAccessMode() const { return KS_AC_READ; }
    virtual KS_SEMANTIC_FLAGS getSemanticFlags() const = 0;
    virtual KsEngPropsHandle  getEP() const;

    //// KssLink ////
    //// accessors
    //   children
    virtual KssChildIterator_THISTYPE *
        newIterator() const = 0;
    virtual KssChildIterator_THISTYPE *
        newMaskedIterator(const KsMask & name_mask,
                          KS_OBJ_TYPE type_mask)
            const = 0;

    virtual KssCommObjectHandle getChildById(const KsString & id) const = 0;
    virtual KssCommObjectHandle getChildByPath(const KsPath & path) const = 0;

    //// KssCurrPropsService stuff
    //FIXMEvirtual KS_RESULT getCurrProps(KsCurrPropsHandle &hprops) const;
    virtual KsCurrPropsHandle getCurrProps() const = 0;
    virtual KS_RESULT setCurrProps(const KsCurrPropsHandle &hprops) = 0;

    //// KssLink
    ///  accessors
    // projected properties
    virtual KS_LINK_TYPE getType() const = 0;
    virtual KsString     getOppositeRoleIdentifier() const = 0;

#if 0
    // FIXME
    virtual KsValueHandle getValue() const = 0;
    virtual KsTime        getTime() const;
    virtual KS_STATE      getState() const;
#endif

    PLT_DECL_RTTI;
}; // class KssLink


// ----------------------------------------------------------------------------
// class KssHistory: ACPLT/KS histories provide access to archived data stored
// in time series or message logs.
//
class KssHistory
: public KssCommObject, public KssChildrenService,
  public PltHandleContainer<KssCommObject>
{
public:
    //// KssCommObject
    //   accessors
    virtual KS_OBJ_TYPE_ENUM typeCode() const { return KS_OT_HISTORY; }

    //   engineered properties
    virtual KsString          getIdentifier() const = 0;
    virtual KsTime            getCreationTime() const = 0;
    virtual KsString          getComment() const = 0;
    virtual KS_ACCESS         getAccessMode() const { return KS_AC_READ; }
    virtual KS_SEMANTIC_FLAGS getSemanticFlags() const = 0;
    virtual KsEngPropsHandle  getEP() const;

    //// KssHistory ////
    //// accessors
    //   children
    virtual KssChildIterator_THISTYPE *
        newIterator() const = 0;
    virtual KssChildIterator_THISTYPE *
        newMaskedIterator(const KsMask & name_mask,
                          KS_OBJ_TYPE type_mask)
            const = 0;

    virtual KssCommObjectHandle getChildById(const KsString & id) const = 0;
    virtual KssCommObjectHandle getChildByPath(const KsPath & path) const = 0;

    //// KssHistory
    ///  accessors
    // projected properties
    virtual KS_HIST_TYPE          getType() const = 0;
    virtual KS_INTERPOLATION_MODE getDefaultInterpolation() const = 0;
    virtual KS_INTERPOLATION_MODE getSupportedInterpolations() const = 0;
    virtual KsString              getTypeIdentifier() const = 0;

    PLT_DECL_RTTI;
}; // class KssHistory


#endif // KS_SVROBJECTS_INCLUDED
/* End of ks/svrobjects.h */

