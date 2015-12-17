/* -*-plt-c++-*- */
#ifndef KS_STRING_INCLUDED
#define KS_STRING_INCLUDED
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
/* Author: Martin Kneissl <martin@plt.rwth-aachen.de> */

#include "plt/string.h"
#include "ks/xdr.h"

class KsString
: public PltString,
  public KsXdrAble
{
public:
    KsString(const char *, size_t = PLT_SIZE_T_MAX);
    KsString(const char *, const char *);
    KsString();
    KsString(const PltString &);

    // KsXdrAble interface
    KsString(XDR *, bool & success);
    virtual bool xdrEncode(XDR *) const;
    virtual bool xdrDecode(XDR *);
    static KsString * xdrNew(XDR *);
protected:
    KsString(size_t sz, char *s);
};


//////////////////////////////////////////////////////////////////////
// INLINE IMPLEMENTATION
//////////////////////////////////////////////////////////////////////

inline
KsString::KsString(const char *p, size_t len) 
: PltString(p,len)
{
}

//////////////////////////////////////////////////////////////////////

inline
KsString::KsString(const char *sz1, const char *sz2)
: PltString(sz1, sz2)
{}

//////////////////////////////////////////////////////////////////////

inline
KsString::KsString(size_t sz, char *p) 
: PltString(sz,p)
{
}

//////////////////////////////////////////////////////////////////////

inline 
KsString::KsString()
{
}

//////////////////////////////////////////////////////////////////////

inline
KsString::KsString(const PltString &s)
: PltString(s)
{
}

//////////////////////////////////////////////////////////////////////

#endif // KS_STRING_INCLUDED
