/* -*-plt-c++-*- */

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

//////////////////////////////////////////////////////////////////////

#include "ks/avsimplemodule.h"

//////////////////////////////////////////////////////////////////////

bool
KscSimpleNegotiator::xdrEncode(XDR *xdr)
{
    enum_t auth_simple = KS_AUTH_SIMPLE;
    return ks_xdre_enum(xdr, &auth_simple)
        && _id.xdrEncode(xdr);
}

//////////////////////////////////////////////////////////////////////

bool
KscSimpleNegotiator::xdrDecode(XDR *xdr)
{
    enum_t auth_type;

    bool ok = ks_xdrd_enum(xdr, &auth_type);

    return ok &&
        (auth_type == KS_AUTH_SIMPLE || auth_type == KS_AUTH_NONE);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

KscNegotiatorHandle
KscAvSimpleModule::_getNegotiator(const KscServer *) const
{
        return _negotiator;
}

// End of avmodule.cpp
