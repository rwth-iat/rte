/* -*-plt-c++-*- */

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

#include "ks/props.h"
 
////////////////////////////////////////////////////////////////////////////

PLT_IMPL_RTTI1(KsProjProps, KsXdrUnion);
PLT_IMPL_RTTI1(KsVarProjProps, KsProjProps);
PLT_IMPL_RTTI1(KsDomainProjProps, KsProjProps);
PLT_IMPL_RTTI1(KsCurrProps, KsXdrUnion);
PLT_IMPL_RTTI1(KsVarCurrProps, KsCurrProps);
PLT_IMPL_RTTI1(KsDomainCurrProps, KsCurrProps);

//////////////////////////////////////////////////////////////////////
// print functions for debugging
//////////////////////////////////////////////////////////////////////
#if PLT_DEBUG

void 
KsProjProps::debugPrint(ostream &os) const
{
    os << "\tidentifier : " << identifier << endl;
    os << "\tcreation time : " << creation_time.tv_sec
       << "." << creation_time.tv_usec << endl;
    os << "\tcomment : " << comment << endl;
    os << "\taccess mode : " << access_mode << endl;
}

//////////////////////////////////////////////////////////////////////

void 
KsDomainProjProps::debugPrint(ostream &os) const
{
    os << "Projected properties for domain :" << endl;
    KsProjProps::debugPrint(os);
}

//////////////////////////////////////////////////////////////////////

void 
KsVarProjProps::debugPrint(ostream &os) const
{
    os << "Projected Properties for variable :" << endl;
    KsProjProps::debugPrint(os);
    os << "\ttechnical unit : " << tech_unit << endl;
    os << "\ttype : " << type << endl;
}

//////////////////////////////////////////////////////////////////////

void 
KsCurrProps::debugPrint(ostream &) const
{
}

//////////////////////////////////////////////////////////////////////

void 
KsDomainCurrProps::debugPrint(ostream &os) const
{
    os << "Current properties for domain :" << endl;
    os << "\t-no data-" << endl;
}

//////////////////////////////////////////////////////////////////////

void
KsVarCurrProps::debugPrint(ostream &os) const
{
    os << "Current properties for variable :" << endl;
    os << "\tvalue : ";
    if(value) {
        value->debugPrint(os);
        os << endl;
    }
    else {
        os << "no value set" << endl;
    }
    os << "\ttime : " << time.tv_sec
       << "." << time.tv_usec << endl;
    os << "\tstate : " << state << endl;
}

#endif // PLT_DEBUG

////////////////////////////////////////////////////////////////////////////
// class KsProjProps
////////////////////////////////////////////////////////////////////////////

KS_BEGIN_IMPL_XDRUNION(KsProjProps);
KS_XDR_MAP(KS_OT_VARIABLE, KsVarProjProps);
KS_XDR_MAP(KS_OT_DOMAIN, KsDomainProjProps);
KS_END_IMPL_XDRUNION;

bool
KsProjProps::xdrEncodeCommon(XDR *xdr) const 
{
    if( !(identifier.xdrEncode(xdr)) )
        return false;
    if( !(creation_time.xdrEncode(xdr)) )
        return false;
    if( !(comment.xdrEncode(xdr)) )
        return false;
    return ks_xdre_enum( xdr, &access_mode );
}

///////////////////////////////////////////////////////////////////////////

bool
KsProjProps::xdrDecodeCommon(XDR *xdr) 
{
    if( !(identifier.xdrDecode(xdr)) )
        return false;
    if( !(creation_time.xdrDecode(xdr)) )
        return false;
    if( !(comment.xdrDecode(xdr)) )
        return false;
    return ks_xdrd_enum( xdr, &access_mode );
}


///////////////////////////////////////////////////////////////////////////
// class KsVarProjProps
///////////////////////////////////////////////////////////////////////////

// KS_IMPL_XDRCTOR(KsVarProjProps);

KsVarProjProps::KsVarProjProps(XDR *xdr, bool &ok)
{
    ok = tech_unit.xdrDecode(xdr)
        && ks_xdrd_enum(xdr, &type);
}

//////////////////////////////////////////////////////////////////////

bool
KsVarProjProps::xdrEncodeVariant(XDR *xdr) const
{
    return tech_unit.xdrEncode(xdr)
        && ks_xdre_enum(xdr, &type);
}

//////////////////////////////////////////////////////////////////////////
    
bool
KsVarProjProps::xdrDecodeVariant(XDR *xdr)
{
    return tech_unit.xdrDecode(xdr)
        && ks_xdrd_enum(xdr, &type);
}

//////////////////////////////////////////////////////////////////////////

enum_t
KsVarProjProps::xdrTypeCode() const
{
    return KS_OT_VARIABLE;
}

////////////////////////////////////////////////////////////////////////////
// class KsDomainProjProps
////////////////////////////////////////////////////////////////////////////

// KS_IMPL_XDRCTOR(KsDomainProjProps);

KsDomainProjProps::KsDomainProjProps(XDR *, bool &)
{}

enum_t
KsDomainProjProps::xdrTypeCode() const {

    return KS_OT_DOMAIN;
}

////////////////////////////////////////////////////////////////////////////
// class KsCurrProps
////////////////////////////////////////////////////////////////////////////

KS_BEGIN_IMPL_XDRUNION(KsCurrProps);
KS_XDR_MAP(KS_OT_VARIABLE,KsVarCurrProps);
KS_XDR_MAP(KS_OT_DOMAIN,KsDomainCurrProps);
KS_END_IMPL_XDRUNION;

KS_IMPL_XDR_C(KsCurrProps);

////////////////////////////////////////////////////////////////////////////
// class KsVarCurrProps
////////////////////////////////////////////////////////////////////////////


KsVarCurrProps::KsVarCurrProps(XDR *xdr, bool &ok)
{
    ok = value.xdrDecode(xdr) &&
        time.xdrDecode(xdr) && 
        ks_xdrd_enum(xdr, &state);
}

//////////////////////////////////////////////////////////////////////

bool
KsVarCurrProps::xdrEncodeVariant(XDR *xdr) const 
{
    return value.xdrEncode(xdr)
        && time.xdrEncode(xdr)
        && ks_xdre_enum(xdr, &state);
}

//////////////////////////////////////////////////////////////////////

bool
KsVarCurrProps::xdrDecodeVariant(XDR *xdr) 
{
    return value.xdrDecode(xdr) 
        && time.xdrDecode(xdr)
        && ks_xdrd_enum(xdr, &state);
}    

enum_t
KsVarCurrProps::xdrTypeCode() const
{
    return KS_OT_VARIABLE;
}

///////////////////////////////////////////////////////////////////////////
// class KsDomainCurrProps
///////////////////////////////////////////////////////////////////////////

// KS_IMPL_XDR_C(KsDomainCurrProps);

enum_t
KsDomainCurrProps::xdrTypeCode() const 
{
    return KS_OT_DOMAIN;
}

//////////////////////////////////////////////////////////////////////

bool
KsDomainCurrProps::xdrEncodeVariant(XDR *) const 
{
    return true;
}

//////////////////////////////////////////////////////////////////////

bool
KsDomainCurrProps::xdrDecodeVariant(XDR *)
{
    return true;
}

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// End of File props.cpp
//////////////////////////////////////////////////////////////////////////



