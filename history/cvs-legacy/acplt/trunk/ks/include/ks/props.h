/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/include/ks/props.h,v 1.14 1999-01-12 16:22:07 harald Exp $ */
#ifndef KS_PROPS_INCLUDED
#define KS_PROPS_INCLUDED
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

/* Author: Markus Juergens <markusj@plt.rwth-aachen.de> */
/* v1+ and v2: Harald Albrecht <harald@plt.rwth-aachen.de> */


#include "ks/ks.h"
#include "ks/string.h"
#include "ks/xdr.h"
#include "ks/handle.h"
#include "ks/value.h"
#include "ks/time.h"


// ----------------------------------------------------------------------------
// The engineered properties common to all ACPLT/KS communication objects.
// Note that there is no such base communication object in ACPLT/KS, it is
// rather an abstract base class.
//
class KsProjProps :
public KsXdrUnion
{
public:
    typedef KsProjProps THISTYPE;
#define KsProjProps_THISTYPE KsProjProps

    KsProjProps() {}
    KsProjProps(const KsProjProps &);
    KsProjProps(const KsString &ident,
                const KsTime &ct,
                const KsString &co,
                KS_ACCESS am);
    virtual ~KsProjProps() {}

    KsProjProps &operator = (const KsProjProps &);

    /*
     * The Engineered Properties...
     */
    KsString            identifier;
    KsTime              creation_time;
    KsString            comment;
    KS_ACCESS           access_mode;

protected:
    bool xdrEncodeCommon(XDR *xdr) const;
    bool xdrDecodeCommon(XDR *xdr);
    KS_DECL_XDRUNION(KsProjProps);
    PLT_DECL_RTTI;

#if PLT_DEBUG
public:
    virtual void debugPrint(ostream &) const;
#endif
}; // class KsProjProps

typedef KsPtrHandle<KsProjProps> KsProjPropsHandle;


// ----------------------------------------------------------------------------
// Engineered properties of variable communication objects.
//
class KsVarProjProps :
public KsProjProps
{
public:
#if PLT_RETTYPE_OVERLOADABLE
    typedef KsVarProjProps THISTYPE;
    #define KsVarProjProps_THISTYPE KsVarProjProps
#else
    #define KsVarProjProps_THISTYPE KsProjProps
#endif

    KsVarProjProps() {}
    KsVarProjProps(const KsVarProjProps &);
    KsVarProjProps(const KsString &ident,
                   const KsTime &ct,
                   const KsString &co,
                   KS_ACCESS am,
                   const KsString &tu,
                   KS_VAR_TYPE tp);
    virtual ~KsVarProjProps() {}

    KsVarProjProps &operator = (const KsVarProjProps &);

    virtual enum_t xdrTypeCode() const;

    /*
     * Additional Engineered Properties...
     */
    KsString tech_unit;
    KS_VAR_TYPE type;

protected:
    bool xdrEncodeVariant(XDR *) const;
    bool xdrDecodeVariant(XDR *);

private:
    friend class KsProjProps;
    KsVarProjProps( XDR *, bool & );
    PLT_DECL_RTTI;

#if PLT_DEBUG
public:
    virtual void debugPrint(ostream &) const;
#endif
}; // class KsVarProjProps


// ----------------------------------------------------------------------------
// Engineered properties of domain communication objects.
//
class KsDomainProjProps :
public KsProjProps
{
public:
#if PLT_RETTYPE_OVERLOADABLE
    typedef KsDomainProjProps THISTYPE;
    #define KsDomainProjProps_THISTYPE KsDomainProjProps
#else
    #define KsDomainProjProps_THISTYPE KsProjProps
#endif
    KsDomainProjProps() {}
    KsDomainProjProps(const KsDomainProjProps &);
    KsDomainProjProps(const KsString &ident,
                      const KsTime &ct,
                      const KsString &co,
                      KS_ACCESS am);
    virtual ~KsDomainProjProps() {}

    KsDomainProjProps &operator = (const KsDomainProjProps &);

    virtual enum_t xdrTypeCode() const;

    /*
     * No additional Engineered Properties.
     */

protected:
    bool xdrEncodeVariant(XDR *) const { return true; }
    bool xdrDecodeVariant(XDR *) { return true; }

private:
    friend class KsProjProps;
    KsDomainProjProps( XDR *, bool & );
    PLT_DECL_RTTI;

#if PLT_DEBUG
public:
    virtual void debugPrint(ostream &) const;
#endif
}; // class KsDomainProjProps


// ----------------------------------------------------------------------------
// Engineered properties of link communication objects.
//
class KsLinkProjProps :
public KsProjProps
{
public:
#if PLT_RETTYPE_OVERLOADABLE
    typedef KsLinkProjProps THISTYPE;
    #define KsLinkProjProps_THISTYPE KsLinkProjProps
#else
    #define KsLinkProjProps_THISTYPE KsProjProps
#endif
    KsLinkProjProps() {}
    KsLinkProjProps(const KsLinkProjProps &);
    KsLinkProjProps(const KsString &ident,
		    const KsTime &ct,
		    const KsString &co,
		    KS_ACCESS am,
		    KS_LINK_TYPE lt, 
		    const KsString &ori,
		    const KsString &ci);
    virtual ~KsLinkProjProps() {}

    KsLinkProjProps &operator = (const KsLinkProjProps &);

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
    friend class KsProjProps;
    KsLinkProjProps( XDR *, bool & );
    PLT_DECL_RTTI;

#if PLT_DEBUG
public:
    virtual void debugPrint(ostream &) const;
#endif
}; // class KsLinkProjProps


// ----------------------------------------------------------------------------
// Engineered properties of history communication objects.
//
class KsHistoryProjProps :
public KsProjProps
{
public:
#if PLT_RETTYPE_OVERLOADABLE
    typedef KsHistoryProjProps THISTYPE;
    #define KsHistoryProjProps_THISTYPE KsHistoryProjProps
#else
    #define KsHistoryProjProps_THISTYPE KsProjProps
#endif
    KsHistoryProjProps() {}
    KsHistoryProjProps(const KsHistoryProjProps &);
    KsHistoryProjProps(const KsString &ident,
		       const KsTime &ct,
		       const KsString &co,
		       KS_ACCESS am,
		       KS_HIST_TYPE ht,
		       KS_INTERPOLATION_MODE defaulti,
		       KS_INTERPOLATION_MODE suppi,
		       const KsString &ti);
    virtual ~KsHistoryProjProps() {}

    KsHistoryProjProps &operator = (const KsHistoryProjProps &);

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
    friend class KsProjProps;
    KsHistoryProjProps(XDR *, bool &);
    PLT_DECL_RTTI;

#if PLT_DEBUG
public:
    virtual void debugPrint(ostream &) const;
#endif
}; // class KsHistoryProjProps



////////////////////////////////////////////////////////////////////////////
// class KsCurrProps
//////////////////////////////////////////////////////////////////////

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
// KsProjProps inline stuff...
//
inline
KsProjProps::KsProjProps(const KsProjProps &other)
: identifier(other.identifier),
  creation_time(other.creation_time),
  comment(other.comment),
  access_mode(other.access_mode)
{} // KsProjProps::KsProjProps


inline
KsProjProps::KsProjProps(const KsString &ident,
                         const KsTime &ct,
                         const KsString &co,
                         KS_ACCESS am)
: identifier(ident),
  creation_time(ct),
  comment(co),
  access_mode(am)
{} // KsProjProps::KsProjProps


inline
KsProjProps &
KsProjProps::operator=(const KsProjProps &other)
{
    identifier = other.identifier;
    creation_time = other.creation_time;
    comment = other.comment;
    access_mode = other.access_mode;
    return *this;
} // KsProjProps::operator=


// ----------------------------------------------------------------------------
// KsVarProjProps inline stuff...
//
inline
KsVarProjProps::KsVarProjProps(const KsVarProjProps &other)
: KsProjProps(other.identifier, other.creation_time,
              other.comment, other.access_mode),
  tech_unit(other.tech_unit),
  type(other.type)
{} // KsVarProjProps::KsVarProjProps

    
inline
KsVarProjProps::KsVarProjProps(const KsString &ident,
                               const KsTime &ct,
                               const KsString &co,
                               KS_ACCESS am,
                               const KsString &tu,
                               KS_VAR_TYPE tp)
: KsProjProps(ident, ct, co, am),
  tech_unit(tu),
  type(tp)
{} // KsVarProjProps::KsVarProjProps


inline
KsVarProjProps &
KsVarProjProps::operator=(const KsVarProjProps &other)
{
    KsProjProps::operator=(other);
    tech_unit = other.tech_unit;
    type = other.type;
    return *this;
} // KsVarProjProps::operator=


// ----------------------------------------------------------------------------
// KsDomainProjProps inline stuff...
//
inline
KsDomainProjProps::KsDomainProjProps(const KsDomainProjProps &other)
: KsProjProps(other.identifier, other.creation_time,
              other.comment, other.access_mode)
{} // KsDomainProjProps::KsDomainProjProps


inline
KsDomainProjProps::KsDomainProjProps(const KsString &ident,
                                     const KsTime &ct,
                                     const KsString &co,
                                     KS_ACCESS am)
: KsProjProps(ident, ct, co, am) 
{} // KsDomainProjProps::KsDomainProjProps


inline
KsDomainProjProps &
KsDomainProjProps::operator=(const KsDomainProjProps &other)
{
    KsProjProps::operator=(other);
    return *this;
} // KsDomainProjProps::operator=


// ----------------------------------------------------------------------------
// KsLinkProjProps inline stuff...
//
inline
KsLinkProjProps::KsLinkProjProps(const KsLinkProjProps &other)
: KsProjProps(other.identifier, other.creation_time,
              other.comment, other.access_mode),
  type(other.type),
  opposite_role_identifier(other.opposite_role_identifier),
  class_identifier(other.class_identifier)
{} // KsLinkProjProps::KsLinkProjProps


inline
KsLinkProjProps::KsLinkProjProps(const KsString &ident,
				 const KsTime &ct,
				 const KsString &co,
				 KS_ACCESS am,
				 KS_LINK_TYPE lt, 
				 const KsString &ori,
				 const KsString &ci)
: KsProjProps(ident, ct, co, am),
  type(lt), opposite_role_identifier(ori),
  class_identifier(ci)
{} // KsLinkProjProps::KsLinkProjProps


inline
KsLinkProjProps &
KsLinkProjProps::operator=(const KsLinkProjProps &other)
{
    KsProjProps::operator=(other);
    type = other.type;
    opposite_role_identifier = other.opposite_role_identifier;
    class_identifier = other.class_identifier;
    return *this;
} // KsLinkProjProps::operator=


// ----------------------------------------------------------------------------
// KsHistoryProjProps inline stuff...
//
inline
KsHistoryProjProps::KsHistoryProjProps(const KsHistoryProjProps &other)
: KsProjProps(other.identifier, other.creation_time,
              other.comment, other.access_mode),
  type(other.type),
  default_interpolation(other.default_interpolation),
  supported_interpolations(other.supported_interpolations),
  type_identifier(other.type_identifier)
{} // KsHistoryProjProps::KsHistoryProjProps


inline
KsHistoryProjProps::KsHistoryProjProps(const KsString &ident,
				       const KsTime &ct,
				       const KsString &co,
				       KS_ACCESS am,
				       KS_HIST_TYPE ht,
				       KS_INTERPOLATION_MODE defaulti,
				       KS_INTERPOLATION_MODE suppi,
				       const KsString &ti)
: KsProjProps(ident, ct, co, am),
  type(ht), default_interpolation(defaulti),
  supported_interpolations(suppi), type_identifier(ti)
{} // KsHistoryProjProps::KsHistoryProjProps


inline
KsHistoryProjProps &
KsHistoryProjProps::operator=(const KsHistoryProjProps &other)
{
    KsProjProps::operator=(other);
    type = other.type;
    default_interpolation = other.default_interpolation;
    supported_interpolations = other.supported_interpolations;
    type_identifier = other.type_identifier;
    return *this;
} // KsHistoryProjProps::operator=




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

#endif

////////////////////////////////////////////////////////////////////////////
// End of File props.h
////////////////////////////////////////////////////////////////////////////



