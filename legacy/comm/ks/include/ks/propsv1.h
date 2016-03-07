/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/include/ks/propsv1.h,v 1.5 2007-04-25 12:57:20 martin Exp $ */
#ifndef KS_PROPSV1_INCLUDED
#define KS_PROPSV1_INCLUDED
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
 * propsv1.h -- this module contains only the so-called projected properties
 *              of the ACPLT/KS version 1 protocol. In contrast to this, the
 *              now so-called "engineered properties" defined by the current
 *              protocol version is contained in props.h
 */

/* Author: Markus Juergens <markusj@plt.rwth-aachen.de> */
/* v1+ and v2: Harald Albrecht <harald@plt.rwth-aachen.de> */


#include "ks/ks.h"
#include "ks/string.h"
#include "ks/xdr.h"
#include "ks/handle.h"
#include "ks/value.h"
#include "ks/time.h"

#include "ks/props.h"


// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// Section: Here come the old and depreciated Projected Properties

// ----------------------------------------------------------------------------
// The engineered properties common to all ACPLT/KS communication objects.
// Note that there is no such base communication object in ACPLT/KS, it is
// rather an abstract base class.
//
class KsEngPropsV1 : public KsXdrUnion
{
public:

    //
    // For the lesser C++ standard compliant compilers, we need to define
    // some helpers to case the return type of overloaded methods in derived
    // classes to the appropriate base class.
    //
    typedef KsEngPropsV1 THISTYPE;
#define KsEngPropsV1_THISTYPE KsEngPropsV1

    //
    // ctor/dtor area...
    //
    KsEngPropsV1() {}
    KsEngPropsV1(const KsEngPropsV1 &);
    KsEngPropsV1(const KsString &ident,
		 const KsTime &ct,
		 const KsString &co,
		 KS_ACCESS am);
    virtual ~KsEngPropsV1() {}

    KsEngPropsV1 &operator = (const KsEngPropsV1 &);

    /*
     * The Engineered Properties...
     */
    KsString           identifier;     // identifier of object
    KsTime             creation_time;  // creation timestamp
    KsString           comment;        // some description about object
    KS_ACCESS          access_mode;    // access mode and other mode flags

protected:
    bool xdrEncodeCommon(XDR *xdr) const;
    bool xdrDecodeCommon(XDR *xdr);
    KS_DECL_XDRUNION(KsEngPropsV1);
    PLT_DECL_RTTI;

#if PLT_DEBUG
public:
    virtual void debugPrint(ostream &) const;
#endif
}; // class KsEngPropsV1

typedef KsPtrHandle<KsEngPropsV1> KsEngPropsV1Handle;


// ----------------------------------------------------------------------------
// Engineered properties of variable communication objects.
//
class KsVarEngPropsV1 : public KsEngPropsV1
{
public:

    //
    // see comments above for lesser C+ compilers...
    //
#if PLT_RETTYPE_OVERLOADABLE
    typedef KsVarEngPropsV1 THISTYPE;
    #define KsVarEngPropsV1_THISTYPE KsVarEngPropsV1
#else
    #define KsVarEngPropsV1_THISTYPE KsEngPropsV1
#endif

    KsVarEngPropsV1() {}
    KsVarEngPropsV1(const KsVarEngPropsV1 &);
    KsVarEngPropsV1(const KsString &ident,
		    const KsTime &ct,
		    const KsString &co,
		    KS_ACCESS am,
		    const KsString &tu,
		    KS_VAR_TYPE tp);
    virtual ~KsVarEngPropsV1() {}

    KsVarEngPropsV1 &operator = (const KsVarEngPropsV1 &);

    virtual enum_t xdrTypeCode() const;

    /*
     * Additional Engineered Properties...
     */
    KsString     tech_unit;
    KS_VAR_TYPE  type;

protected:
    bool xdrEncodeVariant(XDR *) const;
    bool xdrDecodeVariant(XDR *);

private:
    friend class KsEngPropsV1;
    KsVarEngPropsV1( XDR *, bool & );
    PLT_DECL_RTTI;

#if PLT_DEBUG
public:
    virtual void debugPrint(ostream &) const;
#endif
}; // class KsVarEngPropsV1


// ----------------------------------------------------------------------------
// Engineered properties of domain communication objects.
//
class KsDomainEngPropsV1 : public KsEngPropsV1
{
public:

    //
    // see comments above for lesser C+ compilers...
    //
#if PLT_RETTYPE_OVERLOADABLE
    typedef KsDomainEngPropsV1 THISTYPE;
    #define KsDomainEngPropsV1_THISTYPE KsDomainEngPropsV1
#else
    #define KsDomainEngPropsV1_THISTYPE KsEngPropsV1
#endif

    KsDomainEngPropsV1() {}
    KsDomainEngPropsV1(const KsDomainEngPropsV1 &);
    KsDomainEngPropsV1(const KsString &ident,
		       const KsTime &ct,
		       const KsString &co,
		       KS_ACCESS am);
    virtual ~KsDomainEngPropsV1() {}

    KsDomainEngPropsV1 &operator = (const KsDomainEngPropsV1 &);

    virtual enum_t xdrTypeCode() const;

    /*
     * No additional Engineered Properties.
     */

protected:
    bool xdrEncodeVariant(XDR *) const;
    bool xdrDecodeVariant(XDR *);

private:
    friend class KsEngPropsV1;
    KsDomainEngPropsV1( XDR *, bool & );
    PLT_DECL_RTTI;

#if PLT_DEBUG
public:
    virtual void debugPrint(ostream &) const;
#endif
}; // class KsDomainEngPropsV1


#if PLT_SOURCE_V1PLUS_BC

// ----------------------------------------------------------------------------
// Engineered properties of link communication objects.
//
class KsLinkEngPropsV1 : public KsEngPropsV1
{
public:
#if PLT_RETTYPE_OVERLOADABLE
    typedef KsLinkEngPropsV1 THISTYPE;
    #define KsLinkEngPropsV1_THISTYPE KsLinkEngPropsV1
#else
    #define KsLinkEngPropsV1_THISTYPE KsEngPropsV1
#endif
    KsLinkEngPropsV1() {}
    KsLinkEngPropsV1(const KsLinkEngPropsV1 &);
    KsLinkEngPropsV1(const KsString &ident,
		     const KsTime &ct,
		     const KsString &co,
		     KS_ACCESS am,
		     KS_LINK_TYPE lt, 
		     const KsString &ori,
		     const KsString &ci);
    virtual ~KsLinkEngPropsV1() {}

    KsLinkEngPropsV1 &operator = (const KsLinkEngPropsV1 &);

    virtual enum_t xdrTypeCode() const;

    /*
     * Additional Engineered Properties...
     */
    KS_LINK_TYPE  type;
    KsString      opposite_role_identifier;
    KsString      class_identifier;

protected:
    bool xdrEncodeVariant(XDR *) const;
    bool xdrDecodeVariant(XDR *);

private:
    friend class KsEngPropsV1;
    KsLinkEngPropsV1( XDR *, bool & );
    PLT_DECL_RTTI;

#if PLT_DEBUG
public:
    virtual void debugPrint(ostream &) const;
#endif
}; // class KsLinkEngPropsV1


// ----------------------------------------------------------------------------
// Engineered properties of history communication objects.
//
class KsHistoryEngPropsV1 : public KsEngPropsV1
{
public:
#if PLT_RETTYPE_OVERLOADABLE
    typedef KsHistoryEngPropsV1 THISTYPE;
    #define KsHistoryEngPropsV1_THISTYPE KsHistoryEngPropsV1
#else
    #define KsHistoryEngPropsV1_THISTYPE KsEngPropsV1
#endif
    KsHistoryEngPropsV1() {}
    KsHistoryEngPropsV1(const KsHistoryEngPropsV1 &);
    KsHistoryEngPropsV1(const KsString &ident,
			const KsTime &ct,
			const KsString &co,
			KS_ACCESS am,
			KS_HIST_TYPE ht,
			KS_INTERPOLATION_MODE defaulti,
			KS_INTERPOLATION_MODE suppi,
			const KsString &ti);
    virtual ~KsHistoryEngPropsV1() {}

    KsHistoryEngPropsV1 &operator = (const KsHistoryEngPropsV1 &);

    virtual enum_t xdrTypeCode() const;

    /*
     * Additional Engineered Properties...
     */
    KS_HIST_TYPE           type;
    KS_INTERPOLATION_MODE  default_interpolation;
    KS_INTERPOLATION_MODE  supported_interpolations;
    KsString               type_identifier;

protected:
    bool xdrEncodeVariant(XDR *) const;
    bool xdrDecodeVariant(XDR *);

private:
    friend class KsEngPropsV1;
    KsHistoryEngPropsV1(XDR *, bool &);
    PLT_DECL_RTTI;

#if PLT_DEBUG
public:
    virtual void debugPrint(ostream &) const;
#endif
}; // class KsHistoryEngPropsV1

#endif


// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// Section: Current Properties
//
// Nothing to declare or define here as protocol version 1 and 2 share the
// same current properties for communication objects. The only difference is
// that the version 2 protocol of ACPLT/KS features a few fancy new data types.
//


// ----------------------------------------------------------------------------
// Inline Implementation
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// KsEngPropsV1 inline stuff...
//
inline
KsEngPropsV1::KsEngPropsV1(const KsEngPropsV1 &other)
    : identifier(other.identifier),
      creation_time(other.creation_time),
      comment(other.comment),
      access_mode(other.access_mode)
{} // KsEngPropsV1::KsEngPropsV1


inline
KsEngPropsV1::KsEngPropsV1(const KsString &ident,
			   const KsTime &ct,
			   const KsString &co,
			   KS_ACCESS am)
    : identifier(ident),
      creation_time(ct),
      comment(co),
      access_mode(am)
{} // KsEngPropsV1::KsEngPropsV1


inline
KsEngPropsV1 &
KsEngPropsV1::operator=(const KsEngPropsV1 &other)
{
    identifier     = other.identifier;
    creation_time  = other.creation_time;
    comment        = other.comment;
    access_mode    = other.access_mode;
    return *this;
} // KsEngPropsV1::operator=


// ----------------------------------------------------------------------------
// KsVarEngPropsV1 inline stuff...
//
inline
KsVarEngPropsV1::KsVarEngPropsV1(const KsVarEngPropsV1 &other)
    : KsEngPropsV1(other.identifier, other.creation_time,
		   other.comment, other.access_mode),
      tech_unit(other.tech_unit),
      type(other.type)
{} // KsVarEngPropsV1::KsVarEngPropsV1

    
inline
KsVarEngPropsV1::KsVarEngPropsV1(const KsString &ident,
				 const KsTime &ct,
				 const KsString &co,
				 KS_ACCESS am,
				 const KsString &tu,
				 KS_VAR_TYPE tp)
    : KsEngPropsV1(ident, ct, co, am),
      tech_unit(tu),
      type(tp)
{} // KsVarEngPropsV1::KsVarEngPropsV1


inline
KsVarEngPropsV1 &
KsVarEngPropsV1::operator=(const KsVarEngPropsV1 &other)
{
    KsEngPropsV1::operator=(other);
    tech_unit = other.tech_unit;
    type      = other.type;
    return *this;
} // KsVarEngPropsV1::operator=


// ----------------------------------------------------------------------------
// KsDomainEngPropsV1 inline stuff...
//
inline
KsDomainEngPropsV1::KsDomainEngPropsV1(const KsDomainEngPropsV1 &other)
    : KsEngPropsV1(other.identifier, other.creation_time,
		   other.comment, other.access_mode)
{} // KsDomainEngProps::KsDomainEngProps


inline
KsDomainEngPropsV1::KsDomainEngPropsV1(const KsString &ident,
				       const KsTime &ct,
				       const KsString &co,
				       KS_ACCESS am)
    : KsEngPropsV1(ident, ct, co, am)
{} // KsDomainEngPropsV1::KsDomainEngPropsV1


inline
KsDomainEngPropsV1 &
KsDomainEngPropsV1::operator=(const KsDomainEngPropsV1 &other)
{
    KsEngPropsV1::operator=(other);
    return *this;
} // KsDomainEngPropsV1::operator=


#if PLT_SOURCE_V1PLUS_BC

// ----------------------------------------------------------------------------
// KsLinkEngPropsV1 inline stuff...
//
inline
KsLinkEngPropsV1::KsLinkEngPropsV1(const KsLinkEngPropsV1 &other)
    : KsEngPropsV1(other.identifier, other.creation_time,
		   other.comment, other.access_mode),
      type(other.type),
      opposite_role_identifier(other.opposite_role_identifier),
      class_identifier(other.class_identifier)
{} // KsLinkEngPropsV1::KsLinkEngPropsV1


inline
KsLinkEngPropsV1::KsLinkEngPropsV1(const KsString &ident,
				   const KsTime &ct,
				   const KsString &co,
				   KS_ACCESS am,
				   KS_LINK_TYPE lt, 
				   const KsString &ori,
				   const KsString &ci)
    : KsEngPropsV1(ident, ct, co, am),
      type(lt), opposite_role_identifier(ori),
      class_identifier(ci)
{} // KsLinkEngPropsV1::KsLinkEngPropsV1


inline
KsLinkEngPropsV1 &
KsLinkEngPropsV1::operator=(const KsLinkEngPropsV1 &other)
{
    KsEngPropsV1::operator=(other);
    type = other.type;
    opposite_role_identifier = other.opposite_role_identifier;
    class_identifier = other.class_identifier;
    return *this;
} // KsLinkEngPropsV1::operator=


// ----------------------------------------------------------------------------
// KsHistoryEngPropsV1 inline stuff...
//
inline
KsHistoryEngPropsV1::KsHistoryEngPropsV1(const KsHistoryEngPropsV1 &other)
    : KsEngPropsV1(other.identifier, other.creation_time,
		   other.comment, other.access_mode),
      type(other.type),
      default_interpolation(other.default_interpolation),
      supported_interpolations(other.supported_interpolations),
      type_identifier(other.type_identifier)
{} // KsHistoryEngPropsV1::KsHistoryEngPropsV1


inline
KsHistoryEngPropsV1::KsHistoryEngPropsV1(const KsString &ident,
					 const KsTime &ct,
					 const KsString &co,
					 KS_ACCESS am,
					 KS_HIST_TYPE ht,
					 KS_INTERPOLATION_MODE defaulti,
					 KS_INTERPOLATION_MODE suppi,
					 const KsString &ti)
    : KsEngPropsV1(ident, ct, co, am),
      type(ht), default_interpolation(defaulti),
      supported_interpolations(suppi), type_identifier(ti)
{} // KsHistoryEngPropsV1::KsHistoryEngPropsV1


inline
KsHistoryEngPropsV1 &
KsHistoryEngPropsV1::operator=(const KsHistoryEngPropsV1 &other)
{
    KsEngPropsV1::operator=(other);
    type = other.type;
    default_interpolation = other.default_interpolation;
    supported_interpolations = other.supported_interpolations;
    type_identifier = other.type_identifier;
    return *this;
} // KsHistoryEngPropsV1::operator=

#endif

#endif
/* End of ks/propsv1.h */
