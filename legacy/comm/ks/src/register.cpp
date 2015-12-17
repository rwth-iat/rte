/* -*- plt-c++ -*- */
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

/////////////////////////////////////////////////////////////////////

#include "ks/register.h"

///////////////////////////////////////////////////////////////////
// class KsServerDesc
//////////////////////////////////////////////////////////////////

bool 
KsServerDesc::xdrDecode(XDR *xdr)
{
    return name.xdrDecode(xdr) 
        && ks_xdrd_u_short(xdr, &protocol_version);
}

////////////////////////////////////////////////////////////////////

bool
KsServerDesc::xdrEncode(XDR *xdr) const
{
    return name.xdrEncode(xdr)
        && ks_xdre_u_short(xdr, &protocol_version);
}

/////////////////////////////////////////////////////////////////////////

KS_IMPL_XDRCTOR(KsServerDesc);
KS_IMPL_XDRNEW(KsServerDesc);

//////////////////////////////////////////////////////////////////////

unsigned long
KsServerDesc::hash() const
{
    return name.hash() ^ protocol_version;
}

//////////////////////////////////////////////////////////////////////

bool 
KsServerDesc::operator ==(const KsServerDesc & rhs) const
{
    return name == rhs.name && protocol_version == rhs.protocol_version;
}

////////////////////////////////////////////////////////////////////////////
// class KsRegistrationParams
///////////////////////////////////////////////////////////////////////////

KsRegistrationParams::KsRegistrationParams(const KsServerDesc &s, 
                                           u_short p, 
                                           u_long t ) 
: server(s), 
  port(p), 
  time_to_live(t) 
{
}
    
//////////////////////////////////////////////////////////////////////

KsRegistrationParams::KsRegistrationParams()
: port(0),
  time_to_live(0)
{
}

//////////////////////////////////////////////////////////////////////

bool
KsRegistrationParams::xdrEncode(XDR *xdr) const 
{
    PLT_PRECONDITION(xdr->x_op == XDR_ENCODE);

    if( !(server.xdrEncode(xdr)) )
        return false;

    return ks_xdre_u_short(xdr, &port) 
        && ks_xdre_u_long(xdr, &time_to_live);
}

//////////////////////////////////////////////////////////////////////

bool 
KsRegistrationParams::xdrDecode(XDR *xdr) 
{
    PLT_PRECONDITION(xdr->x_op == XDR_DECODE);

    if( !(server.xdrDecode(xdr)) )
        return false;

    return ks_xdrd_u_short(xdr, &port)  
        && ks_xdrd_u_long(xdr, &time_to_live);
}

/////////////////////////////////////////////////////////////////////////

KsRegistrationParams::KsRegistrationParams(XDR *xdr, bool &ok)
: server(xdr, ok)
{
    PLT_PRECONDITION(xdr->x_op == XDR_DECODE);
    if (ok) {
        ok =  ks_xdrd_u_short(xdr, &port)
            && ks_xdrd_u_long(xdr, &time_to_live);
    }
}

//////////////////////////////////////////////////////////////////////

KS_IMPL_XDRNEW(KsRegistrationParams);

////////////////////////////////////////////////////////////////////////////
// class KsUnregistrationParams
///////////////////////////////////////////////////////////////////////////

KsUnregistrationParams::KsUnregistrationParams()
{
}

//////////////////////////////////////////////////////////////////////

KsUnregistrationParams::KsUnregistrationParams(const KsServerDesc &s)
: server(s)
{
}
    
//////////////////////////////////////////////////////////////////////

bool
KsUnregistrationParams::xdrEncode(XDR *xdr) const 
{
    PLT_PRECONDITION(xdr->x_op == XDR_ENCODE);
    return server.xdrEncode(xdr);
}

//////////////////////////////////////////////////////////////////////

bool 
KsUnregistrationParams::xdrDecode(XDR *xdr) 
{
    PLT_PRECONDITION(xdr->x_op == XDR_DECODE);
    return server.xdrDecode(xdr);
}

//////////////////////////////////////////////////////////////////////

KsUnregistrationParams::KsUnregistrationParams(XDR *xdr, bool &ok)
: server(xdr, ok)
{
}

//////////////////////////////////////////////////////////////////////

KS_IMPL_XDRNEW(KsUnregistrationParams);


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

KsGetServerResult::KsGetServerResult(KS_RESULT res)
: KsResult(res), 
  server("",0)
{
}

//////////////////////////////////////////////////////////////////////

KsGetServerResult::KsGetServerResult(XDR *xdr, bool &ok)
: KsResult(xdr, ok)
{
    if (ok) {
        if (result == KS_ERR_OK ) {
            ok = server.xdrDecode(xdr)
                && ks_xdrd_u_short(xdr, &port)
                && expires_at.xdrDecode(xdr)
                && ks_xdrd_bool(xdr, &living);
        }
    }
}

//////////////////////////////////////////////////////////////////////

KS_IMPL_XDRNEW(KsGetServerResult);

//////////////////////////////////////////////////////////////////////

bool
KsGetServerResult::xdrDecode(XDR *xdr)
{
    if (!KsResult::xdrDecode(xdr)) {
        return false;
    }
    if (result != KS_ERR_OK ) {
        return true;
    } else {
        return server.xdrDecode(xdr)
            && ks_xdrd_u_short(xdr, &port)
                && expires_at.xdrDecode(xdr)
                    && ks_xdrd_bool(xdr, &living);
    }
}
        

/////////////////////////////////////////////////////////////////////////

bool
KsGetServerResult::xdrEncode(XDR *xdr) const
{
    if (!KsResult::xdrEncode(xdr)) {
        return false;
    }
    if (result != KS_ERR_OK) {
        return true;
    } else {
        return server.xdrEncode(xdr)
           && ks_xdre_u_short(xdr, &port)
               && expires_at.xdrEncode(xdr)
                   && ks_xdre_bool(xdr, &living);
   } 
}

/////////////////////////////////////////////////////////////////////////
// End of file ks/register.cpp

