/* -*- plt-c++ -*- */
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

/////////////////////////////////////////////////////////////////////////
// End of file ks/register.cpp

