/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/include/ks/props.h,v 1.19 2007-04-25 12:57:20 martin Exp $ */
#ifndef KS_PROPS_INCLUDED
#define KS_PROPS_INCLUDED
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
/* v1+ and v2: Harald Albrecht <harald@plt.rwth-aachen.de> */


#include "ks/ks.h"
#include "ks/string.h"
#include "ks/xdr.h"
#include "ks/handle.h"
#include "ks/value.h"
#include "ks/time.h"


// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// Section: Engineered Properties

// ----------------------------------------------------------------------------
// The engineered properties common to all ACPLT/KS communication objects.
// Note that there is no such base communication object in ACPLT/KS, it is
// rather an abstract base class.
//
class KsEngProps : public KsXdrUnion
{
public:

    //
    // For the lesser C++ standard compliant compilers, we need to define
    // some helpers to case the return type of overloaded methods in derived
    // classes to the appropriate base class.
    //
    typedef KsEngProps THISTYPE;
#define KsEngProps_THISTYPE KsEngProps

    //
    // ctor/dtor area...
    //
    KsEngProps() {}
    KsEngProps(const KsEngProps &);
    KsEngProps(const KsString &ident,
               const KsTime &ct,
               const KsString &co,
               KS_ACCESS am); // for backwards compatibility with older code
    KsEngProps(const KsString &ident,
               const KsTime &ct,
               const KsString &co,
               KS_ACCESS am,
               KS_SEMANTIC_FLAGS sf);
    virtual ~KsEngProps() {}

    KsEngProps &operator = (const KsEngProps &);

    /*
     * The Engineered Properties...
     */
    KsString           identifier;     // identifier of object
    KsTime             creation_time;  // creation timestamp
    KsString           comment;        // some description about object
    KS_ACCESS          access_mode;    // access mode and other mode flags
    KS_SEMANTIC_FLAGS  semantic_flags; // semantic flags for object (NEW)

protected:
    bool xdrEncodeCommon(XDR *xdr) const;
    bool xdrDecodeCommon(XDR *xdr);
    KS_DECL_XDRUNION(KsEngProps);
    PLT_DECL_RTTI;

#if PLT_DEBUG
public:
    virtual void debugPrint(ostream &) const;
#endif
}; // class KsEngProps

typedef KsPtrHandle<KsEngProps> KsEngPropsHandle;


// ----------------------------------------------------------------------------
// Engineered properties of variable communication objects.
//
class KsVarEngProps : public KsEngProps
{
public:

    //
    // see comments above for lesser C+ compilers...
    //
#if PLT_RETTYPE_OVERLOADABLE
    typedef KsVarEngProps THISTYPE;
    #define KsVarEngProps_THISTYPE KsVarEngProps
#else
    #define KsVarEngProps_THISTYPE KsEngProps
#endif

    KsVarEngProps() {}
    KsVarEngProps(const KsVarEngProps &);
    KsVarEngProps(const KsString &ident,
		  const KsTime &ct,
		  const KsString &co,
		  KS_ACCESS am,
		  const KsString &tu,
		  KS_VAR_TYPE tp); // for backwards compatibility
    KsVarEngProps(const KsString &ident,
		  const KsTime &ct,
		  const KsString &co,
		  KS_ACCESS am,
		  KS_SEMANTIC_FLAGS sf,
		  const KsString &tu,
		  KS_VAR_TYPE tp);
    virtual ~KsVarEngProps() {}

    KsVarEngProps &operator = (const KsVarEngProps &);

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
    friend class KsEngProps;
    KsVarEngProps( XDR *, bool & );
    PLT_DECL_RTTI;

#if PLT_DEBUG
public:
    virtual void debugPrint(ostream &) const;
#endif
}; // class KsVarEngProps


// ----------------------------------------------------------------------------
// Engineered properties of domain communication objects.
//
class KsDomainEngProps : public KsEngProps
{
public:

    //
    // see comments above for lesser C+ compilers...
    //
#if PLT_RETTYPE_OVERLOADABLE
    typedef KsDomainEngProps THISTYPE;
    #define KsDomainEngProps_THISTYPE KsDomainEngProps
#else
    #define KsDomainEngProps_THISTYPE KsEngProps
#endif

    KsDomainEngProps() {}
    KsDomainEngProps(const KsDomainEngProps &);
    KsDomainEngProps(const KsString &ident,
		     const KsTime &ct,
		     const KsString &co,
		     KS_ACCESS am); // for backwards compatibility
    KsDomainEngProps(const KsString &ident,
		     const KsTime &ct,
		     const KsString &co,
		     KS_ACCESS am,
		     KS_SEMANTIC_FLAGS sf,
		     const KsString &clsid);
    virtual ~KsDomainEngProps() {}

    KsDomainEngProps &operator = (const KsDomainEngProps &);

    virtual enum_t xdrTypeCode() const;

    /*
     * Additional Engineered Properties.
     */
    KsString  class_identifier;

protected:
    bool xdrEncodeVariant(XDR *) const;
    bool xdrDecodeVariant(XDR *);

private:
    friend class KsEngProps;
    KsDomainEngProps( XDR *, bool & );
    PLT_DECL_RTTI;

#if PLT_DEBUG
public:
    virtual void debugPrint(ostream &) const;
#endif
}; // class KsDomainEngProps


// ----------------------------------------------------------------------------
// Engineered properties of link communication objects.
//
class KsLinkEngProps : public KsEngProps
{
public:

    //
    // see comments above for lesser C+ compilers...
    //
#if PLT_RETTYPE_OVERLOADABLE
    typedef KsLinkEngProps THISTYPE;
    #define KsLinkEngProps_THISTYPE KsLinkEngProps
#else
    #define KsLinkEngProps_THISTYPE KsEngProps
#endif

    KsLinkEngProps() {}
    KsLinkEngProps(const KsLinkEngProps &);
    KsLinkEngProps(const KsString &ident,
		   const KsTime &ct,
		   const KsString &co,
		   KS_ACCESS am,
		   KS_SEMANTIC_FLAGS sf,
		   KS_LINK_TYPE lt, 
		   const KsString &ori,
		   const KsString &associd);
    virtual ~KsLinkEngProps() {}

    KsLinkEngProps &operator = (const KsLinkEngProps &);

    virtual enum_t xdrTypeCode() const;

    /*
     * Additional Engineered Properties...
     */
    KS_LINK_TYPE  type;
    KsString      opposite_role_identifier;
    KsString      association_identifier;

protected:
    bool xdrEncodeVariant(XDR *) const;
    bool xdrDecodeVariant(XDR *);

private:
    friend class KsEngProps;
    KsLinkEngProps( XDR *, bool & );
    PLT_DECL_RTTI;

#if PLT_DEBUG
public:
    virtual void debugPrint(ostream &) const;
#endif
}; // class KsLinkEngProps


// ----------------------------------------------------------------------------
// Engineered properties of history communication objects.
//
class KsHistoryEngProps : public KsEngProps
{
public:

    //
    // see comments above for lesser C+ compilers...
    //
#if PLT_RETTYPE_OVERLOADABLE
    typedef KsHistoryEngProps THISTYPE;
    #define KsHistoryEngProps_THISTYPE KsHistoryEngProps
#else
    #define KsHistoryEngProps_THISTYPE KsEngProps
#endif

    KsHistoryEngProps() {}
    KsHistoryEngProps(const KsHistoryEngProps &);
    KsHistoryEngProps(const KsString &ident,
		      const KsTime &ct,
		      const KsString &co,
		      KS_ACCESS am,
		      KS_SEMANTIC_FLAGS sf,
		      KS_HIST_TYPE ht,
		      KS_INTERPOLATION_MODE defaulti,
		      KS_INTERPOLATION_MODE suppi,
		      const KsString &ti);
    virtual ~KsHistoryEngProps() {}

    KsHistoryEngProps &operator = (const KsHistoryEngProps &);

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
    friend class KsEngProps;
    KsHistoryEngProps(XDR *, bool &);
    PLT_DECL_RTTI;

#if PLT_DEBUG
public:
    virtual void debugPrint(ostream &) const;
#endif
}; // class KsHistoryEngProps


// ----------------------------------------------------------------------------
// Engineered properties of structure communication objects.
//
class KsStructureEngProps : public KsEngProps
{
public:

    //
    // see comments above for lesser C+ compilers...
    //
#if PLT_RETTYPE_OVERLOADABLE
    typedef KsStructureEngProps THISTYPE;
    #define KsStructureEngProps_THISTYPE KsStructureEngProps
#else
    #define KsStructureEngProps_THISTYPE KsEngProps
#endif

    KsStructureEngProps() {}
    KsStructureEngProps(const KsStructureEngProps &);
    KsStructureEngProps(const KsString &ident,
			const KsTime &ct,
			const KsString &co,
			KS_ACCESS am,
			KS_SEMANTIC_FLAGS sf,
			KS_STRUCTURE_FLAGS stf);
    virtual ~KsStructureEngProps() {}

    KsStructureEngProps &operator = (const KsStructureEngProps &);

    virtual enum_t xdrTypeCode() const;

    /*
     * Additional Engineered Properties.
     */
    KS_STRUCTURE_FLAGS  structure_flags;

protected:
    bool xdrEncodeVariant(XDR *) const;
    bool xdrDecodeVariant(XDR *);

private:
    friend class KsEngProps;
    KsStructureEngProps( XDR *, bool & );
    PLT_DECL_RTTI;

#if PLT_DEBUG
public:
    virtual void debugPrint(ostream &) const;
#endif
}; // class KsStructureEngProps


// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// Section: Current Properties

// ----------------------------------------------------------------------------
// The current properties common to all ACPLT/KS communication objects. Well,
// not much common stuff here at this time...
//
class KsCurrProps :
public KsXdrUnion
{
public:
#define KsCurrProps_THISTYPE KsCurrProps
    typedef KsCurrProps THISTYPE;
    virtual ~KsCurrProps() {}

    KS_DECL_XDRUNION(KsCurrProps);
    PLT_DECL_RTTI;

#if PLT_DEBUG
    virtual void debugPrint(ostream &) const;
#endif
};

typedef KsPtrHandle<KsCurrProps> KsCurrPropsHandle;

////////////////////////////////////////////////////////////////////////////
// class KsVarCurrProps
////////////////////////////////////////////////////////////////////////////

class KsVarCurrProps
: public KsCurrProps
{
public:
#if PLT_RETTYPE_OVERLOADABLE
    typedef KsVarCurrProps THISTYPE;
    #define KsVarCurrProps_THISTYPE KsVarCurrProps
#else
    #define KsVarCurrProps_THISTYPE KsCurrProps
#endif
    KsVarCurrProps();
    KsVarCurrProps(KsValueHandle v);
    KsVarCurrProps(const KsVarCurrProps &);
    KsVarCurrProps(KsValueHandle v,
                   const KsTime &t,
                   KS_STATE s);
    ~KsVarCurrProps() {}

    KsVarCurrProps &operator = (KsVarCurrProps &);

    virtual enum_t xdrTypeCode() const;

    KsValueHandle value;
    KsTime time;
    KS_STATE state;

protected:
    bool xdrEncodeVariant(XDR *) const;
    bool xdrDecodeVariant(XDR *);

private:
    friend class KsCurrProps;
    KsVarCurrProps( XDR *, bool & );
    PLT_DECL_RTTI;

#if PLT_DEBUG
public:
    virtual void debugPrint(ostream &) const;
#endif
};

////////////////////////////////////////////////////////////////////////////

class KsDomainCurrProps : public KsCurrProps {
public:
#if PLT_RETTYPE_OVERLOADABLE
    typedef KsDomainCurrProps THISTYPE;
    #define KsDomainCurrProps_THISTYPE KsDomainCurrProps
#else
    #define KsDomainCurrProps_THISTYPE KsCurrProps
#endif
    KsDomainCurrProps() {}
    ~KsDomainCurrProps() {}

    virtual enum_t xdrTypeCode() const;

protected:
    bool xdrEncodeVariant(XDR *) const;
    bool xdrDecodeVariant(XDR *);

private:
    friend class KsCurrProps;
    KsDomainCurrProps(XDR *, bool &);
    PLT_DECL_RTTI;

#if PLT_DEBUG
public:
    virtual void debugPrint(ostream &) const;
#endif
};


// ----------------------------------------------------------------------------
// Inline Implementation
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// KsEngProps inline stuff...
//
inline
KsEngProps::KsEngProps(const KsEngProps &other)
    : identifier(other.identifier),
      creation_time(other.creation_time),
      comment(other.comment),
      access_mode(other.access_mode),
      semantic_flags(other.semantic_flags)
{} // KsEngProps::KsEngProps


inline
KsEngProps::KsEngProps(const KsString &ident,
                       const KsTime &ct,
                       const KsString &co,
                       KS_ACCESS am)
    : identifier(ident),
      creation_time(ct),
      comment(co),
      access_mode(am),
      semantic_flags(0)
{} // KsEngProps::KsEngProps


inline
KsEngProps::KsEngProps(const KsString &ident,
                       const KsTime &ct,
                       const KsString &co,
                       KS_ACCESS am,
		       KS_SEMANTIC_FLAGS sf)
    : identifier(ident),
      creation_time(ct),
      comment(co),
      access_mode(am),
      semantic_flags(sf)
{} // KsEngProps::KsEngProps


inline
KsEngProps &
KsEngProps::operator=(const KsEngProps &other)
{
    identifier     = other.identifier;
    creation_time  = other.creation_time;
    comment        = other.comment;
    access_mode    = other.access_mode;
    semantic_flags = other.semantic_flags;
    return *this;
} // KsEngProps::operator=


// ----------------------------------------------------------------------------
// KsVarEngProps inline stuff...
//
inline
KsVarEngProps::KsVarEngProps(const KsVarEngProps &other)
    : KsEngProps(other.identifier, other.creation_time,
		 other.comment, other.access_mode,
		 other.semantic_flags),
      tech_unit(other.tech_unit),
      type(other.type)
{} // KsVarEngProps::KsVarEngProps

    
inline
KsVarEngProps::KsVarEngProps(const KsString &ident,
			     const KsTime &ct,
			     const KsString &co,
			     KS_ACCESS am,
			     const KsString &tu,
			     KS_VAR_TYPE tp)
    : KsEngProps(ident, ct, co, am, 0),
      tech_unit(tu),
      type(tp)
{} // KsVarEngProps::KsVarEngProps


inline
KsVarEngProps::KsVarEngProps(const KsString &ident,
			     const KsTime &ct,
			     const KsString &co,
			     KS_ACCESS am,
			     KS_SEMANTIC_FLAGS sf,
			     const KsString &tu,
			     KS_VAR_TYPE tp)
    : KsEngProps(ident, ct, co, am, sf),
      tech_unit(tu),
      type(tp)
{} // KsVarEngProps::KsVarEngProps


inline
KsVarEngProps &
KsVarEngProps::operator=(const KsVarEngProps &other)
{
    KsEngProps::operator=(other);
    tech_unit = other.tech_unit;
    type      = other.type;
    return *this;
} // KsVarEngProps::operator=


// ----------------------------------------------------------------------------
// KsDomainEngProps inline stuff...
//
inline
KsDomainEngProps::KsDomainEngProps(const KsDomainEngProps &other)
    : KsEngProps(other.identifier, other.creation_time,
		 other.comment, other.access_mode,
		 other.semantic_flags),
      class_identifier(other.class_identifier)
{} // KsDomainEngProps::KsDomainEngProps


inline
KsDomainEngProps::KsDomainEngProps(const KsString &ident,
				   const KsTime &ct,
				   const KsString &co,
				   KS_ACCESS am)
    : KsEngProps(ident, ct, co, am, 0),
      class_identifier("")
{} // KsDomainEngProps::KsDomainEngProps


inline
KsDomainEngProps::KsDomainEngProps(const KsString &ident,
				   const KsTime &ct,
				   const KsString &co,
				   KS_ACCESS am,
				   KS_SEMANTIC_FLAGS sf,
				   const KsString &clsid)
    : KsEngProps(ident, ct, co, am, sf),
      class_identifier(clsid)
{} // KsDomainEngProps::KsDomainEngProps


inline
KsDomainEngProps &
KsDomainEngProps::operator=(const KsDomainEngProps &other)
{
    KsEngProps::operator=(other);
    class_identifier = other.class_identifier;
    return *this;
} // KsDomainEngProps::operator=


// ----------------------------------------------------------------------------
// KsLinkEngProps inline stuff...
//
inline
KsLinkEngProps::KsLinkEngProps(const KsLinkEngProps &other)
    : KsEngProps(other.identifier, other.creation_time,
		 other.comment, other.access_mode, other.semantic_flags),
      type(other.type),
      opposite_role_identifier(other.opposite_role_identifier),
      association_identifier(other.association_identifier)
{} // KsLinkEngProps::KsLinkEngProps


inline
KsLinkEngProps::KsLinkEngProps(const KsString &ident,
			       const KsTime &ct,
			       const KsString &co,
			       KS_ACCESS am,
			       KS_SEMANTIC_FLAGS sf,
			       KS_LINK_TYPE lt, 
			       const KsString &ori,
			       const KsString &associd)
: KsEngProps(ident, ct, co, am, sf),
  type(lt), opposite_role_identifier(ori),
  association_identifier(associd)
{} // KsLinkEngProps::KsLinkEngProps


inline
KsLinkEngProps &
KsLinkEngProps::operator=(const KsLinkEngProps &other)
{
    KsEngProps::operator=(other);
    type                     = other.type;
    opposite_role_identifier = other.opposite_role_identifier;
    association_identifier   = other.association_identifier;
    return *this;
} // KsLinkEngProps::operator=


// ----------------------------------------------------------------------------
// KsHistoryEngProps inline stuff...
//
inline
KsHistoryEngProps::KsHistoryEngProps(const KsHistoryEngProps &other)
    : KsEngProps(other.identifier, other.creation_time,
		 other.comment, other.access_mode,
		 other.semantic_flags),
      type(other.type),
      default_interpolation(other.default_interpolation),
      supported_interpolations(other.supported_interpolations),
      type_identifier(other.type_identifier)
{} // KsHistoryEngProps::KsHistoryEngProps


inline
KsHistoryEngProps::KsHistoryEngProps(const KsString &ident,
				     const KsTime &ct,
				     const KsString &co,
				     KS_ACCESS am,
				     KS_SEMANTIC_FLAGS sf,
				     KS_HIST_TYPE ht,
				     KS_INTERPOLATION_MODE defaulti,
				     KS_INTERPOLATION_MODE suppi,
				     const KsString &ti)
    : KsEngProps(ident, ct, co, am, sf),
      type(ht), default_interpolation(defaulti),
      supported_interpolations(suppi), type_identifier(ti)
{} // KsHistoryEngProps::KsHistoryEngProps


inline
KsHistoryEngProps &
KsHistoryEngProps::operator=(const KsHistoryEngProps &other)
{
    KsEngProps::operator=(other);
    type                     = other.type;
    default_interpolation    = other.default_interpolation;
    supported_interpolations = other.supported_interpolations;
    type_identifier          = other.type_identifier;
    return *this;
} // KsHistoryEngProps::operator=


// ----------------------------------------------------------------------------
// KsStructureEngProps inline stuff...
//
inline
KsStructureEngProps::KsStructureEngProps(const KsStructureEngProps &other)
    : KsEngProps(other.identifier, other.creation_time,
		 other.comment, other.access_mode,
		 other.semantic_flags),
      structure_flags(other.structure_flags)
{} // KsStructureEngProps::KsStructureEngProps


inline
KsStructureEngProps::KsStructureEngProps(const KsString &ident,
					 const KsTime &ct,
					 const KsString &co,
					 KS_ACCESS am,
					 KS_SEMANTIC_FLAGS sf,
					 KS_STRUCTURE_FLAGS stf)
    : KsEngProps(ident, ct, co, am, sf),
      structure_flags(stf)
{} // KsStructureEngProps::KsStructureEngProps


inline
KsStructureEngProps &
KsStructureEngProps::operator=(const KsStructureEngProps &other)
{
    KsEngProps::operator=(other);
    structure_flags = other.structure_flags;
    return *this;
} // KsStructureEngProps::operator=



//////////////////////////////////////////////////////////////////////

inline
KsVarCurrProps::KsVarCurrProps()
{}

//////////////////////////////////////////////////////////////////////

inline
KsVarCurrProps::KsVarCurrProps(const KsVarCurrProps &other)
: value(other.value),
  time(other.time),
  state(other.state)
{}

//////////////////////////////////////////////////////////////////////

inline
KsVarCurrProps::KsVarCurrProps(KsValueHandle v,
                               const KsTime &t,
                               KS_STATE s)
: value(v),
  time(t),
  state(s)
{}

//////////////////////////////////////////////////////////////////////

inline
KsVarCurrProps::KsVarCurrProps( KsValueHandle v )
{
    PLT_PRECONDITION(v);
    value = v;
}

//////////////////////////////////////////////////////////////////////

inline
KsVarCurrProps &
KsVarCurrProps::operator = (KsVarCurrProps &other)
{
    value = other.value;
    time = other.time;
    state = other.state;
    return *this;
}

//////////////////////////////////////////////////////////////////////

inline
KsDomainCurrProps::KsDomainCurrProps(XDR *, bool &ok)
{
    ok = true;
}



// --------------------------------------------------------------------------
// Enable source code compatibility with the v1 C++ Communication Library as
// good as possible.
//
#if PLT_SOURCE_V1_BC

typedef KsEngProps KsProjProps;
typedef KsVarEngProps KsVarProjProps;
typedef KsDomainEngProps KsDomainProjProps;

#endif

#endif

/* End of ks/props.h */
