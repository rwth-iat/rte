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

/////////////////////////////////////////////////////////////////////////////

#include "plt/debug.h"
#include "ks/time.h"

////////////////////////////////////////////////////////////////////////////
// class KsTime
////////////////////////////////////////////////////////////////////////////

bool
KsTime::xdrEncode(XDR *xdr) const {

    PLT_PRECONDITION(xdr->x_op == XDR_ENCODE);
    u_long sec  = tv_sec;
    u_long usec = tv_usec;

    return ks_xdre_u_long(xdr, &sec)
        && ks_xdre_u_long(xdr, &usec);
}

/////////////////////////////////////////////////////////////////////////////

bool
KsTime::xdrDecode(XDR *xdr) {
    PLT_PRECONDITION(xdr->x_op == XDR_DECODE);
    u_long sec;
    u_long usec;

    if (ks_xdrd_u_long(xdr, &sec)
        && ks_xdrd_u_long(xdr, &usec)) {
        // success
        tv_sec = sec;
        tv_usec = usec;
        normalize();
        PLT_CHECK_INVARIANT();
        return true;
    } else {
        return false;
    }
}

/////////////////////////////////////////////////////////////////////////////

KS_IMPL_XDRCTOR(KsTime);
KS_IMPL_XDRNEW(KsTime);

/////////////////////////////////////////////////////////////////////////////
// class KsTimeSpan
/////////////////////////////////////////////////////////////////////////////

KS_IMPL_XDRCTOR(KsTimeSpan);
KS_IMPL_XDRNEW(KsTimeSpan);

bool 
KsTimeSpan::xdrEncode(XDR *xdr) const
{
    PLT_PRECONDITION(xdr->x_op == XDR_ENCODE);

    return ks_xdre_long(xdr, &tv_sec)
        && ks_xdre_long(xdr, &tv_usec);
}

/////////////////////////////////////////////////////////////////////////////

bool 
KsTimeSpan::xdrDecode(XDR *xdr)
{
    PLT_PRECONDITION(xdr->x_op == XDR_DECODE);

    return ks_xdrd_long(xdr, &tv_sec)
        && ks_xdrd_long(xdr, &tv_usec);
}

///////////////////////////////////////////////////////////////////////////
// End of File time.cpp
///////////////////////////////////////////////////////////////////////////










