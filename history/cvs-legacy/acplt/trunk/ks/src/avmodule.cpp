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

#include "ks/avmodule.h"

//////////////////////////////////////////////////////////////////////
// define static data
//
KscNoneNegotiator KscAvNoneModule::the_negotiator;

//////////////////////////////////////////////////////////////////////

bool
KscNoneNegotiator::xdrEncode(XDR *xdr)
{
    enum_t auth_none = KS_AUTH_NONE;
    return ks_xdre_enum(xdr, &auth_none);
}

//////////////////////////////////////////////////////////////////////

bool
KscNoneNegotiator::xdrDecode(XDR *xdr)
{
    enum_t auth_type;

    bool ok = ks_xdrd_enum(xdr, &auth_type);

    return ok &&
        auth_type == KS_AUTH_NONE;
}

//////////////////////////////////////////////////////////////////////

KscNegotiatorHandle
KscAvNoneModule::getNegotiator(const KscServer *) const
{
    return KscNegotiatorHandle(&the_negotiator, PltOsUnmanaged);
}

//////////////////////////////////////////////////////////////////////
// EOF avmodule.cpp
//////////////////////////////////////////////////////////////////////




