/* -*-plt-c++-*- */

#ifndef KS_TIME_INCLUDED
#define KS_TIME_INCLUDED

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

#include "ks/xdr.h"
#include "plt/time.h"

////////////////////////////////////////////////////////////////////////////
// class KsTime
////////////////////////////////////////////////////////////////////////////

class KsTime : public PltTime, public KsXdrAble {
public:
    KsTime( long sec = 0L, long usec = 0L )
        : PltTime(sec, usec) {}
    KsTime(XDR *, bool &);
    KsTime(const PltTime &r)
        : PltTime(r) { }
    bool xdrEncode(XDR *xdr) const;
    bool xdrDecode(XDR *xdr);
    static KsTime *xdrNew(XDR *xdr);
};

/////////////////////////////////////////////////////////////////////////////

class KsTimeSpan
    : public PltTimeSpan,
      public KsXdrAble
{
public:
    KsTimeSpan(long sec = 0, long usec = 0)
        : PltTimeSpan(sec, usec) {}
    KsTimeSpan(XDR *, bool &);
    KsTimeSpan(const PltTimeSpan &ts)
        : PltTimeSpan(ts) {}

    bool xdrEncode(XDR *xdr) const;
    bool xdrDecode(XDR *xdr);
    static KsTimeSpan *xdrNew(XDR *xdr);
};

//////////////////////////////////////////////////////////////////////
// Inline Implementation
//////////////////////////////////////////////////////////////////////

#endif

///////////////////////////////////////////////////////////////////////////
// End of File time.h
///////////////////////////////////////////////////////////////////////////

