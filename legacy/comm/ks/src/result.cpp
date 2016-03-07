/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/src/result.cpp,v 1.11 2007-04-25 12:57:21 martin Exp $ */
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


#include "ks/result.h"

/////////////////////////////////////////////////////////////////////////////
// class KsResult
////////////////////////////////////////////////////////////////////////////

KsResult::KsResult(KS_RESULT res)
    : result(res)
{
}

/////////////////////////////////////////////////////////////////////////////

bool 
KsResult::xdrEncode(XDR *xdr) const {
    
    PLT_PRECONDITION(xdr->x_op == XDR_ENCODE);
    enum_t xresult = result;
    return ks_xdre_enum( xdr, &xresult );
}

////////////////////////////////////////////////////////////////////////////

bool
KsResult::xdrDecode(XDR *xdr) {

    PLT_PRECONDITION(xdr->x_op == XDR_DECODE);
    enum_t xresult;
    bool ok = ks_xdrd_enum( xdr, &xresult );
    result = xresult;
    return ok;
}

///////////////////////////////////////////////////////////////////////////

KsResult::KsResult(XDR *xdr, bool & ok)
{
    PLT_PRECONDITION(xdr->x_op == XDR_DECODE);
    enum_t xresult;
    ok = ks_xdrd_enum( xdr, &xresult);
    result = xresult;
}

//////////////////////////////////////////////////////////////////////

KS_IMPL_XDRNEW(KsResult);

//////////////////////////////////////////////////////////////////////


// EOF ks/result
