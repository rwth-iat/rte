/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/src/props.cpp,v 1.11 1999-09-06 06:56:19 harald Exp $ */
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


#include "ks/props.h"

 
// ----------------------------------------------------------------------------
// Sigh. RTTI emulation, once more.
//
PLT_IMPL_RTTI1(KsEngProps, KsXdrUnion);
PLT_IMPL_RTTI1(KsVarEngProps,       KsEngProps);
PLT_IMPL_RTTI1(KsDomainEngProps,    KsEngProps);
PLT_IMPL_RTTI1(KsLinkEngProps,      KsEngProps);
PLT_IMPL_RTTI1(KsHistoryEngProps,   KsEngProps);
PLT_IMPL_RTTI1(KsStructureEngProps, KsEngProps);

PLT_IMPL_RTTI1(KsCurrProps, KsXdrUnion);
PLT_IMPL_RTTI1(KsVarCurrProps,    KsCurrProps);
PLT_IMPL_RTTI1(KsDomainCurrProps, KsCurrProps);


#if PLT_DEBUG
// ----------------------------------------------------------------------------
// "Diagnostic" print functions for aiding debugging of the various engineered
// properties.
//

void 
KsEngProps::debugPrint(ostream &os) const
{
    os << "  identifier: " << identifier << endl;
    os << "  creation time: " << creation_time.tv_sec
       << "." << creation_time.tv_usec << endl;
    os << "  comment: " << comment << endl;
    os << "  access mode: " << access_mode << endl;
    os << "  semantic flags: " << semantic_flags << endl;
} // KsEngProps::debugPrint

// ----------------------------------------------------------------------------

void 
KsDomainEngProps::debugPrint(ostream &os) const
{
    os << "Engineered Properties for domain:" << endl;
    KsEngProps::debugPrint(os);
    os << "  class identifier: " << class_identifier << endl;
} // KsDomainEngProps::debugPrint

// ----------------------------------------------------------------------------

void 
KsVarEngProps::debugPrint(ostream &os) const
{
    os << "Engineered Properties for variable:" << endl;
    KsEngProps::debugPrint(os);
    os << "  technical unit: " << tech_unit << endl;
    os << "  type: " << type << endl;
} // KsVarEngProps::debugPrint

// ----------------------------------------------------------------------------

void 
KsLinkEngProps::debugPrint(ostream &os) const
{
    os << "Engineered Properties for link:" << endl;
    KsEngProps::debugPrint(os);
    os << "  opposite role identifier: " << opposite_role_identifier << endl;
    os << "  type: " << type << endl;
    os << "  association class: " << association_identifier << endl;
} // KsLinkEngProps::debugPrint

// ----------------------------------------------------------------------------

void 
KsHistoryEngProps::debugPrint(ostream &os) const
{
    os << "Engineered Properties for history:" << endl;
    KsEngProps::debugPrint(os);
    os << "  history type: " << type << endl;
    os << "  default interpolation mode: " << default_interpolation << endl;
    os << "  supported interpolation modes: " << supported_interpolations << endl;
    os << "  type identifier: " << type_identifier << endl;
} // KsHistoryEngProps::debugPrint

// ----------------------------------------------------------------------------

void 
KsStructureEngProps::debugPrint(ostream &os) const
{
    os << "Engineered Properties for structure:" << endl;
    KsEngProps::debugPrint(os);
    os << "  structure flags: " << structure_flags << endl;
} // KsStructureEngProps::debugPrint


// ----------------------------------------------------------------------------
// "Diagnostic" print functions for aiding debugging of the various current
// properties.
//

void 
KsCurrProps::debugPrint(ostream &) const
{
} // KsCurrProps::debugPrint

// ----------------------------------------------------------------------------

void 
KsDomainCurrProps::debugPrint(ostream &os) const
{
    os << "Current properties for domain :" << endl;
    os << "  -no data-" << endl;
} // KsDomainCurrProps::debugPrint

// ----------------------------------------------------------------------------

void
KsVarCurrProps::debugPrint(ostream &os) const
{
    os << "Current properties for variable :" << endl;
    os << "  value : ";
    if(value) {
        value->debugPrint(os);
        os << endl;
    }
    else {
        os << "no value set" << endl;
    }
    os << "  time : " << time.tv_sec
       << "." << time.tv_usec << endl;
    os << "  state : " << state << endl;
} // KsVarCurrProps::debugPrint

#endif // PLT_DEBUG


// ----------------------------------------------------------------------------
// Class KsEngProps: engineered properties common to all ACPLT/KS communi-
// cation objects. Remember that we represent the engineered properties as a
// XDR union, so we need to list all derived classes here.
//
KS_BEGIN_IMPL_XDRUNION(KsEngProps);
KS_XDR_MAP(KS_OT_VARIABLE,  KsVarEngProps);
KS_XDR_MAP(KS_OT_DOMAIN,    KsDomainEngProps);
KS_XDR_MAP(KS_OT_LINK,      KsLinkEngProps);
KS_XDR_MAP(KS_OT_HISTORY,   KsHistoryEngProps);
KS_XDR_MAP(KS_OT_STRUCTURE, KsStructureEngProps);
KS_END_IMPL_XDRUNION;


// ----------------------------------------------------------------------------
// Serialize (send to wire) the field of the engineered properties which are
// common to all ACPLT/KS communication objects.
//
bool
KsEngProps::xdrEncodeCommon(XDR *xdr) const 
{
    if ( !identifier.xdrEncode(xdr) ) {
        return false;
    }
    if ( !creation_time.xdrEncode(xdr) ) {
        return false;
    }
    if ( !comment.xdrEncode(xdr) ) {
        return false;
    }
    if ( !ks_xdre_enum(xdr, &access_mode) ) {
	return false;
    }
    return ks_xdre_u_long(xdr, &semantic_flags);
} // KsEngProps::xdrEncodeCommon


// ----------------------------------------------------------------------------
// Similar to the previous function, but now deserialize (receive from wire).
//
bool
KsEngProps::xdrDecodeCommon(XDR *xdr) 
{
    if ( !identifier.xdrDecode(xdr) ) {
        return false;
    }
    if ( !creation_time.xdrDecode(xdr) ) {
        return false;
    }
    if ( !comment.xdrDecode(xdr) ) {
        return false;
    }
    if ( !ks_xdrd_enum(xdr, &access_mode) ) {
	return false;
    }
    return ks_xdrd_u_long(xdr, &semantic_flags);
} // KsEngProps::xdrDecodeCommon


// ----------------------------------------------------------------------------
// Class KsVarEngProps: engineered properties for variables.
//
KsVarEngProps::KsVarEngProps(XDR *xdr, bool &ok)
{
    ok = tech_unit.xdrDecode(xdr)
	&& ks_xdrd_enum(xdr, &type);
} // KsVarEngProps::KsVarEngProps


// ----------------------------------------------------------------------------
// Serialize those fields which are new to the engineered properties of an
// ACPLT/KS variable.
bool
KsVarEngProps::xdrEncodeVariant(XDR *xdr) const
{
    return tech_unit.xdrEncode(xdr)
        && ks_xdre_enum(xdr, &type);
} // KsVarEngProps::xdrEncodeVariant


// ----------------------------------------------------------------------------
// Deserialize those fields which are new to the engineered properties of an
// ACPLT/KS variable.
bool
KsVarEngProps::xdrDecodeVariant(XDR *xdr)
{
    return tech_unit.xdrDecode(xdr)
        && ks_xdrd_enum(xdr, &type);
} // KsVarEngProps::xdrDecodeVariant


// ----------------------------------------------------------------------------
// Returns the object type these engineered properties belong to.
//
enum_t
KsVarEngProps::xdrTypeCode() const
{
    return KS_OT_VARIABLE;
} // KsVarEngProps::xdrTypeCode


// ----------------------------------------------------------------------------
// Class KsDomainEngProps: engineered properties of domains. Note that we don't
// need to call the decoding method for the common part, as this is automati-
// cally done from the static KsEngProps::xdrNew method of the base class.
//
KsDomainEngProps::KsDomainEngProps(XDR *xdr, bool &ok)
{
    ok = class_identifier.xdrDecode(xdr);
} // KsDomainEngProps::KsDomainEngProps


// ----------------------------------------------------------------------------
// Serialize those fields which are new to the engineered properties of a
// domain.
//
bool
KsDomainEngProps::xdrEncodeVariant(XDR *xdr) const
{
    return class_identifier.xdrEncode(xdr);
} // KsDomainEngProps::xdrEncodeVariant


// ----------------------------------------------------------------------------
// Deserialize those fields which are new to the engineered properties of a
// domain.
//
bool
KsDomainEngProps::xdrDecodeVariant(XDR *xdr)
{
    return class_identifier.xdrDecode(xdr);
} // KsDomainEngProps::xdrDecodeVariant


// ----------------------------------------------------------------------------
// Returns the object type these engineered properties belong to.
//
enum_t
KsDomainEngProps::xdrTypeCode() const {

    return KS_OT_DOMAIN;
} // KsDomainEngProps::xdrTypeCode


// ----------------------------------------------------------------------------
// Class KsLinkEngProps: engineered properties of links.
//
KsLinkEngProps::KsLinkEngProps(XDR *xdr, bool &ok)
{
    ok = ks_xdrd_enum(xdr, &type)
      && opposite_role_identifier.xdrDecode(xdr)
      && association_identifier.xdrDecode(xdr);
} // KsLinkEngProps::KsLinkEngProps


// ----------------------------------------------------------------------------
// Serialize those fields which are new to the engineered properties of an
// ACPLT/KS link.
//
bool
KsLinkEngProps::xdrEncodeVariant(XDR *xdr) const
{
    return ks_xdre_enum(xdr, &type)
        && opposite_role_identifier.xdrEncode(xdr)
        && association_identifier.xdrEncode(xdr);
} // KsLinkEngProps::xdrEncodeVariant


// ----------------------------------------------------------------------------
// Deserialize those fields which are new to the engineered properties of an
// ACPLT/KS link.
//
bool
KsLinkEngProps::xdrDecodeVariant(XDR *xdr)
{
    return ks_xdrd_enum(xdr, &type)
        && opposite_role_identifier.xdrDecode(xdr)
        && association_identifier.xdrDecode(xdr);
} // KsLinkEngProps::xdrDecodeVariant


// ----------------------------------------------------------------------------
// Returns the object type these engineered properties belong to, in this case
// this is for a link object.
//
enum_t
KsLinkEngProps::xdrTypeCode() const {

    return KS_OT_LINK;
} // KsLinkEngProps::xdrTypeCode


// ----------------------------------------------------------------------------
// Class KsHistoryProjProps: engineered properties of histories.
//
KsHistoryEngProps::KsHistoryEngProps(XDR *xdr, bool &ok)
{
    ok = ks_xdrd_enum(xdr, &type)
      && ks_xdrd_enum(xdr, &default_interpolation)
      && ks_xdrd_enum(xdr, &supported_interpolations)
      && type_identifier.xdrDecode(xdr);
} // KsHistoryEngProps::KsHistoryEngProps


// ----------------------------------------------------------------------------
// Serialize those fields which are new to the engineered properties of an
// ACPLT/KS history.
bool
KsHistoryEngProps::xdrEncodeVariant(XDR *xdr) const
{
    return ks_xdre_enum(xdr, &type)
        && ks_xdre_enum(xdr, &default_interpolation)
        && ks_xdre_enum(xdr, &supported_interpolations)
        && type_identifier.xdrEncode(xdr);
} // KsHistoryEngProps::xdrEncodeVariant

// ----------------------------------------------------------------------------
// Deserialize those fields which are new to the engineered properties of an
// ACPLT/KS history.
bool
KsHistoryEngProps::xdrDecodeVariant(XDR *xdr)
{
    return ks_xdrd_enum(xdr, &type)
        && ks_xdrd_enum(xdr, &default_interpolation)
        && ks_xdrd_enum(xdr, &supported_interpolations)
        && type_identifier.xdrDecode(xdr);
} // KsHistoryEngProps::xdrDecodeVariant


// ----------------------------------------------------------------------------
// Returns the object type these engineered properties belong to, in this case
// this is for a history object.
//
enum_t
KsHistoryEngProps::xdrTypeCode() const {

    return KS_OT_HISTORY;
} // KsHistoryEngProps::xdrTypeCode


// ----------------------------------------------------------------------------
// Class KsStructureEngProps: engineered properties of structures.
//
KsStructureEngProps::KsStructureEngProps(XDR *xdr, bool &ok)
{
    ok = ks_xdrd_enum(xdr, &structure_flags);
} // KsStructureEngProps::KsStructureEngProps


// ----------------------------------------------------------------------------
// Serialize those fields which are new to the engineered properties of a
// structure.
//
bool
KsStructureEngProps::xdrEncodeVariant(XDR *xdr) const
{
    return ks_xdre_enum(xdr, &structure_flags);
} // KsStructureEngProps::xdrEncodeVariant


// ----------------------------------------------------------------------------
// Deserialize those fields which are new to the engineered properties of a
// domain.
//
bool
KsStructureEngProps::xdrDecodeVariant(XDR *xdr)
{
    return ks_xdrd_enum(xdr, &structure_flags);
} // KsStructureEngProps::xdrDecodeVariant


// ----------------------------------------------------------------------------
// Returns the object type these engineered properties belong to.
//
enum_t
KsStructureEngProps::xdrTypeCode() const {

    return KS_OT_STRUCTURE;
} // KsStructureEngProps::xdrTypeCode



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



/* End of ks/props.cpp */
