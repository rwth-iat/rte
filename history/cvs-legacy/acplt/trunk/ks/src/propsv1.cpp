/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/src/propsv1.cpp,v 1.1 1999-09-06 06:56:19 harald Exp $ */
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
 * propsv1.cpp -- this module contains only the so-called projected properties
 *                of the ACPLT/KS version 1 protocol. In contrast to this, the
 *                now so-called "engineered properties" defined by the current
 *                protocol version is contained in props.h
 */

/* Author: Markus Juergens <markusj@plt.rwth-aachen.de> */
/* v1+ and v2: Harald Albrecht <harald@plt.rwth-aachen.de> */


#include "ks/propsv1.h"

 
// ----------------------------------------------------------------------------
// Sigh. RTTI emulation, once more.
//
PLT_IMPL_RTTI1(KsEngPropsV1, KsXdrUnion);
PLT_IMPL_RTTI1(KsVarEngPropsV1,     KsEngPropsV1);
PLT_IMPL_RTTI1(KsDomainEngPropsV1,  KsEngPropsV1);
#if PLT_SOURCE_V1PLUS_BC
PLT_IMPL_RTTI1(KsLinkEngPropsV1,    KsEngPropsV1);
PLT_IMPL_RTTI1(KsHistoryEngPropsV1, KsEngPropsV1);
#endif

#if PLT_DEBUG
// ----------------------------------------------------------------------------
// "Diagnostic" print functions for aiding debugging of the various engineered
// properties.
//

void 
KsEngPropsV1::debugPrint(ostream &os) const
{
    os << "  identifier: " << identifier << endl;
    os << "  creation time: " << creation_time.tv_sec
       << "." << creation_time.tv_usec << endl;
    os << "  comment: " << comment << endl;
    os << "  access mode: " << access_mode << endl;
} // KsEngPropsV1::debugPrint

// ----------------------------------------------------------------------------

void 
KsDomainEngPropsV1::debugPrint(ostream &os) const
{
    os << "Version 1 Projected Properties for domain:" << endl;
    KsEngProps::debugPrint(os);
} // KsDomainEngPropsV1::debugPrint

// ----------------------------------------------------------------------------

void 
KsVarEngPropsV1::debugPrint(ostream &os) const
{
    os << "Version 1 Projected Properties for variable:" << endl;
    KsEngPropsV1::debugPrint(os);
    os << "  technical unit: " << tech_unit << endl;
    os << "  type: " << type << endl;
} // KsVarEngPropsV1::debugPrint

#if PLT_SOURCE_V1PLUS_BC

// ----------------------------------------------------------------------------

void 
KsLinkEngPropsV1::debugPrint(ostream &os) const
{
    os << "Version 1 Projected Properties for link:" << endl;
    KsEngPropsV1::debugPrint(os);
    os << "  opposite role identifier: " << opposite_role_identifier << endl;
    os << "  type: " << type << endl;
    os << "  class: " << class_identifier << endl;
} // KsLinkEngPropsV1::debugPrint

// ----------------------------------------------------------------------------

void 
KsHistoryEngPropsV1::debugPrint(ostream &os) const
{
    os << "Version 1 Projected Properties for history:" << endl;
    KsEngPropsV1::debugPrint(os);
    os << "  history type: " << type << endl;
    os << "  default interpolation mode: " << default_interpolation << endl;
    os << "  supported interpolation modes: " << supported_interpolations << endl;
    os << "  type identifier: " << type_identifier << endl;
} // KsHistoryEngPropsV1::debugPrint

#endif
#endif


// ----------------------------------------------------------------------------
// Class KsEngPropsV1: engineered properties common to all ACPLT/KS communi-
// cation objects. Remember that we represent the engineered properties as a
// XDR union, so we need to list all derived classes here. Also note that
// these are the engineered properties (formerly "projected properties") of the
// old ACPLT/KS version 1 protocol.
//
KS_BEGIN_IMPL_XDRUNION(KsEngPropsV1);
KS_XDR_MAP(KS_OT_VARIABLE, KsVarEngPropsV1);
KS_XDR_MAP(KS_OT_DOMAIN,   KsDomainEngPropsV1);
#if PLT_SOURCE_V1PLUS_BC
KS_XDR_MAP(KS_OT_LINK,     KsLinkEngPropsV1);
KS_XDR_MAP(KS_OT_HISTORY,  KsHistoryEngPropsV1);
#endif
KS_END_IMPL_XDRUNION;


// ----------------------------------------------------------------------------
// Serialize (send to wire) the field of the engineered properties which are
// common to all ACPLT/KS communication objects.
//
bool
KsEngPropsV1::xdrEncodeCommon(XDR *xdr) const 
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
    return ks_xdre_enum(xdr, &access_mode);
} // KsEngPropsV1::xdrEncodeCommon


// ----------------------------------------------------------------------------
// Similar to the previous function, but now deserialize (receive from wire).
//
bool
KsEngPropsV1::xdrDecodeCommon(XDR *xdr) 
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
    return ks_xdrd_enum(xdr, &access_mode);
} // KsEngPropsV1::xdrDecodeCommon


// ----------------------------------------------------------------------------
// Class KsVarEngPropsV1: engineered properties for variables.
//
KsVarEngPropsV1::KsVarEngPropsV1(XDR *xdr, bool &ok)
{
    ok = tech_unit.xdrDecode(xdr)
	&& ks_xdrd_enum(xdr, &type);
} // KsVarEngPropsV1::KsVarEngPropsV1


// ----------------------------------------------------------------------------
// Serialize those fields which are new to the engineered properties of an
// ACPLT/KS variable.
bool
KsVarEngPropsV1::xdrEncodeVariant(XDR *xdr) const
{
    return tech_unit.xdrEncode(xdr)
        && ks_xdre_enum(xdr, &type);
} // KsVarEngPropsV1::xdrEncodeVariant


// ----------------------------------------------------------------------------
// Deserialize those fields which are new to the engineered properties of an
// ACPLT/KS variable.
bool
KsVarEngPropsV1::xdrDecodeVariant(XDR *xdr)
{
    return tech_unit.xdrDecode(xdr)
        && ks_xdrd_enum(xdr, &type);
} // KsVarEngPropsV1::xdrDecodeVariant


// ----------------------------------------------------------------------------
// Returns the object type these engineered properties belong to.
//
enum_t
KsVarEngPropsV1::xdrTypeCode() const
{
    return KS_OT_VARIABLE;
} // KsVarEngPropsV1::xdrTypeCode


// ----------------------------------------------------------------------------
// Class KsDomainEngPropsV1: engineered properties of domains. Note that we
// don't need to call the decoding method for the common part, as this is
// automatically done from the static KsEngProps::xdrNew method of the base
// class.
//
KsDomainEngPropsV1::KsDomainEngPropsV1(XDR *, bool &ok)
{
    ok = true;
} // KsDomainEngPropsV1::KsDomainEngPropsV1


// ----------------------------------------------------------------------------
// Serialize those fields which are new to the engineered properties of a
// domain.
//
bool
KsDomainEngPropsV1::xdrEncodeVariant(XDR *) const
{
    return true;
} // KsDomainEngPropsV1::xdrEncodeVariant


// ----------------------------------------------------------------------------
// Deserialize those fields which are new to the engineered properties of a
// domain.
//
bool
KsDomainEngPropsV1::xdrDecodeVariant(XDR *)
{
    return true;
} // KsDomainEngPropsV1::xdrDecodeVariant


// ----------------------------------------------------------------------------
// Returns the object type these engineered properties belong to.
//
enum_t
KsDomainEngPropsV1::xdrTypeCode() const {

    return KS_OT_DOMAIN;
} // KsDomainEngPropsV1::xdrTypeCode


#if PLT_SOURCE_V1PLUS_BC

// ----------------------------------------------------------------------------
// Class KsLinkEngProps:V1 engineered properties of links.
//
KsLinkEngPropsV1::KsLinkEngPropsV1(XDR *xdr, bool &ok)
{
    ok = ks_xdrd_enum(xdr, &type)
      && opposite_role_identifier.xdrDecode(xdr)
      && class_identifier.xdrDecode(xdr);
} // KsLinkEngPropsV1::KsLinkEngPropsV1


// ----------------------------------------------------------------------------
// Serialize those fields which are new to the engineered properties of an
// ACPLT/KS link.
bool
KsLinkEngPropsV1::xdrEncodeVariant(XDR *xdr) const
{
    return ks_xdre_enum(xdr, &type)
        && opposite_role_identifier.xdrEncode(xdr)
        && class_identifier.xdrEncode(xdr);
} // KsLinkEngPropsV1::xdrEncodeVariant

// ----------------------------------------------------------------------------
// Deserialize those fields which are new to the engineered properties of an
// ACPLT/KS link.
bool
KsLinkEngPropsV1::xdrDecodeVariant(XDR *xdr)
{
    return ks_xdrd_enum(xdr, &type)
        && opposite_role_identifier.xdrDecode(xdr)
        && class_identifier.xdrDecode(xdr);
} // KsLinkEngPropsV1::xdrDecodeVariant


// ----------------------------------------------------------------------------
// Returns the object type these engineered properties belong to, in this case
// this is for a link object.
//
enum_t
KsLinkEngPropsV1::xdrTypeCode() const {

    return KS_OT_LINK;
} // KsLinkEngPropsV1::xdrTypeCode


// ----------------------------------------------------------------------------
// Class KsHistoryProjProps: engineered properties of histories.
//
KsHistoryEngPropsV1::KsHistoryEngPropsV1(XDR *xdr, bool &ok)
{
    ok = ks_xdrd_enum(xdr, &type)
      && ks_xdrd_enum(xdr, &default_interpolation)
      && ks_xdrd_enum(xdr, &supported_interpolations)
      && type_identifier.xdrDecode(xdr);
} // KsHistoryEngPropsV1::KsHistoryEngPropsV1


// ----------------------------------------------------------------------------
// Serialize those fields which are new to the engineered properties of an
// ACPLT/KS history.
bool
KsHistoryEngPropsV1::xdrEncodeVariant(XDR *xdr) const
{
    return ks_xdre_enum(xdr, &type)
        && ks_xdre_enum(xdr, &default_interpolation)
        && ks_xdre_enum(xdr, &supported_interpolations)
        && type_identifier.xdrEncode(xdr);
} // KsHistoryEngPropsV1::xdrEncodeVariant

// ----------------------------------------------------------------------------
// Deserialize those fields which are new to the engineered properties of an
// ACPLT/KS history.
bool
KsHistoryEngPropsV1::xdrDecodeVariant(XDR *xdr)
{
    return ks_xdrd_enum(xdr, &type)
        && ks_xdrd_enum(xdr, &default_interpolation)
        && ks_xdrd_enum(xdr, &supported_interpolations)
        && type_identifier.xdrDecode(xdr);
} // KsHistoryEngPropsV1::xdrDecodeVariant


// ----------------------------------------------------------------------------
// Returns the object type these engineered properties belong to, in this case
// this is for a history object.
//
enum_t
KsHistoryEngPropsV1::xdrTypeCode() const {

    return KS_OT_HISTORY;
} // KsHistoryEngPropsV1::xdrTypeCode


#endif

/* End of ks/propsv1.cpp */
