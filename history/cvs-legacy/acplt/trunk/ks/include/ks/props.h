/* -*-plt-c++-*- */

#ifndef KS_PROPS_INCLUDED
#define KS_PROPS_INCLUDED

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
 

/////////////////////////////////////////////////////////////////////////////

#include <ks/ks.h>
#include <ks/string.h>
#include <ks/xdr.h>
#include <ks/handle.h>
#include <ks/value.h>
#include <ks/time.h>

////////////////////////////////////////////////////////////////////////////

class KsProjProps : 
public KsXdrUnion 
{
public:
    KsProjProps() {}
    KsProjProps(const KsProjProps &);
    KsProjProps(const KsString &ident,
                const KsTime &ct,
                const KsString &co,
                KS_ACCESS am);
    virtual ~KsProjProps() {}

    KsProjProps &operator = (const KsProjProps &);

    KsString            identifier;
    KsTime              creation_time;
    KsString            comment;
    KS_ACCESS           access_mode;

protected:    
    bool xdrEncodeCommon(XDR *xdr) const;    
    bool xdrDecodeCommon(XDR *xdr);
    KS_DECL_XDRUNION(KsProjProps);
};

typedef KsPtrHandle<KsProjProps> KsProjPropsHandle;

///////////////////////////////////////////////////////////////////////////

class KsVarProjProps : 
public KsProjProps 
{
public:
    KsVarProjProps() {}
    KsVarProjProps(const KsVarProjProps &);
    KsVarProjProps(const KsString &ident,
                   const KsTime &ct,
                   const KsString &co,
                   KS_ACCESS am,
                   const KsString &tu,
                   KS_VAR_TYPE tp);
    ~KsVarProjProps() {}

    KsVarProjProps &operator = (const KsVarProjProps &);

    virtual enum_t xdrTypeCode() const;
    
    KsString tech_unit;
    KS_VAR_TYPE type;

protected:
    bool xdrEncodeVariant(XDR *) const;
    bool xdrDecodeVariant(XDR *); 
private:
    friend KsProjProps;
    KsVarProjProps( XDR *, bool & );
};

///////////////////////////////////////////////////////////////////////////

class KsDomainProjProps : 
public KsProjProps 
{
public:
    KsDomainProjProps() {}
    KsDomainProjProps(const KsDomainProjProps &);
    KsDomainProjProps(const KsString &ident,
                      const KsTime &ct,
                      const KsString &co,
                      KS_ACCESS am);
    ~KsDomainProjProps() {}

    KsDomainProjProps &operator = (const KsDomainProjProps &);
 
    virtual enum_t xdrTypeCode() const;

protected:
    bool xdrEncodeVariant(XDR *) const { return true; }
    bool xdrDecodeVariant(XDR *) { return true; }

private:
    friend KsProjProps;
    KsDomainProjProps( XDR *, bool & );
};


////////////////////////////////////////////////////////////////////////////
// class KsCurrProps
//////////////////////////////////////////////////////////////////////

class KsCurrProps : 
public KsXdrUnion 
{
public:
    virtual ~KsCurrProps() {}

    KS_DECL_XDRUNION(KsCurrProps);
};

typedef KsPtrHandle<KsCurrProps> KsCurrPropsHandle;

////////////////////////////////////////////////////////////////////////////
// class KsVarCurrProps  
////////////////////////////////////////////////////////////////////////////

class KsVarCurrProps
: public KsCurrProps 
{
public:
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
    friend KsCurrProps;
    KsVarCurrProps( XDR *, bool & );
};

////////////////////////////////////////////////////////////////////////////

class KsDomainCurrProps : public KsCurrProps {
public:
    KsDomainCurrProps() {}
    ~KsDomainCurrProps() {}

    virtual enum_t xdrTypeCode() const;

protected:
    bool xdrEncodeVariant(XDR *) const;
    bool xdrDecodeVariant(XDR *);

private:
    friend KsCurrProps;
    KsDomainCurrProps(XDR *, bool &);
};


//////////////////////////////////////////////////////////////////////
// Inline Implementation
//////////////////////////////////////////////////////////////////////

inline
KsProjProps::KsProjProps(const KsProjProps &other)
: identifier(other.identifier),
  creation_time(other.creation_time),
  comment(other.comment),
  access_mode(other.access_mode)
{}

//////////////////////////////////////////////////////////////////////

inline
KsProjProps::KsProjProps(const KsString &ident,
                         const KsTime &ct,
                         const KsString &co,
                         KS_ACCESS am)
: identifier(ident),
  creation_time(ct),
  comment(co),
  access_mode(am)
{}

//////////////////////////////////////////////////////////////////////

inline
KsProjProps &
KsProjProps::operator = (const KsProjProps &other)
{
    identifier = other.identifier;
    creation_time = other.creation_time;
    comment = other.comment;
    access_mode = other.access_mode;
    return *this;
}

//////////////////////////////////////////////////////////////////////

inline
KsVarProjProps::KsVarProjProps(const KsVarProjProps &other)
: KsProjProps(other.identifier, other.creation_time,
              other.comment, other.access_mode),
  tech_unit(other.tech_unit),
  type(other.type)
{}

//////////////////////////////////////////////////////////////////////
    
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
{}

//////////////////////////////////////////////////////////////////////

inline
KsVarProjProps &
KsVarProjProps::operator = (const KsVarProjProps &other)
{
    KsProjProps::operator = (other);
    tech_unit = other.tech_unit;
    type = other.type;
    return *this;
}

//////////////////////////////////////////////////////////////////////

inline
KsDomainProjProps::KsDomainProjProps(const KsDomainProjProps &other)
: KsProjProps(other.identifier, other.creation_time,
  other.comment, other.access_mode)
{}

//////////////////////////////////////////////////////////////////////

inline
KsDomainProjProps::KsDomainProjProps(const KsString &ident,
                                     const KsTime &ct,
                                     const KsString &co,
                                     KS_ACCESS am)
: KsProjProps(ident, ct, co, am) 
{}

//////////////////////////////////////////////////////////////////////

inline
KsDomainProjProps &
KsDomainProjProps::operator = (const KsDomainProjProps &other)
{
    KsProjProps::operator = (other);
    return *this;
}

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



