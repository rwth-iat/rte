// File: ks/register.cpp

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

////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// class KsServerDesc
//////////////////////////////////////////////////////////////////

bool 
KsServerDesc::xdrDecode(XDR *xdr)
{
    return name.xdrDecode(xdr) && xdr_u_short(xdr, &protocol_version);
}

////////////////////////////////////////////////////////////////////

bool
KsServerDesc::xdrEncode(XDR *xdr) const
{
    return name.xdrEncode(xdr) && xdr_u_short(xdr, &protocol_version);
}

/////////////////////////////////////////////////////////////////////////

KsServerDesc *
KsServerDesc::xdrNew(XDR *xdr)
{
    KsServerDesc *psd = new KsServerDesc;

    if( !psd ) {
      // allocation failed
      //
      return NULL;
    }
    
    if( psd->xdrDecode(xdr) ) {
      
        // success:
        // return pointer
        //
        return psd;
    } else {
        // failed:
        // clean up
        //
        delete psd;
        return NULL;
    }
}

/////////////////////////////////////////////////////////////////////////////
// class KsResult
////////////////////////////////////////////////////////////////////////////

bool 
KsResult::xdrEncode(XDR *xdr) const {
    
    PLT_PRECONDITION(xdr->x_op == XDR_ENCODE);
    
    return xdr_enum( xdr, &result );
}

////////////////////////////////////////////////////////////////////////////

bool
KsResult::xdrDecode(XDR *xdr) {

    PLT_PRECONDITION(xdr->x_op == XDR_DECODE);

    return xdr_enum( xdr, &result );
}

///////////////////////////////////////////////////////////////////////////

KsResult *
KsResult::xdrNew(XDR *xdr) {

    KsResult *pksr = new KsResult();

    if( (pksr != NULL) && (pksr->xdrDecode(xdr)) ) {

        return pksr;
    
    } else {

        delete pksr;
        return NULL;
    }
}

////////////////////////////////////////////////////////////////////////////
// class KsRegistrationParams
///////////////////////////////////////////////////////////////////////////

KsRegistrationParams::KsRegistrationParams( const KsServerDesc &s, 
  u_short p, u_long t ) : server(s), port(p), time_to_live(t) 
{}
    
bool
KsRegistrationParams::xdrEncode(XDR *xdr) const {

    if( !(server.xdrEncode(xdr)) )
        return false;

    return xdr_u_short(xdr, &port) && xdr_u_long(xdr, &time_to_live);
}

bool 
KsRegistrationParams::xdrDecode(XDR *xdr) {

    if( !(server.xdrDecode(xdr)) )
        return false;

    return xdr_u_short(xdr, &port)  && xdr_u_long(xdr, &time_to_live);
}

////////////////////////////////////////////////////////////////////////

KsRegistrationParams *KsRegistrationParams::xdrNew(XDR *xdr) {

    KsRegistrationParams *prp = new KsRegistrationParams();

    if( prp && prp->xdrDecode(xdr) ) {

        return prp;

    } else {

        delete prp;
        return NULL;
    }
}

// End of file ks/register.cpp

