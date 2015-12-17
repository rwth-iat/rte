/* -*-plt-c++-*- */

#ifndef KSC_ABSPATH_INCLUDED
#define KSC_ABSPATH_INCLUDED

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

#include "ks/path.h"

//////////////////////////////////////////////////////////////////////
// class KscAbsPath
//   manages an absolute path consisting of host, server and the 
//   path to the communication object within the KS server. The path
//   must look like "//host/server/domain1/../var".
//
class KscAbsPath
: public KsPath
{
public:
    KscAbsPath();
    KscAbsPath(const char *);
    KscAbsPath(const PltString &);

    KscAbsPath &operator = (const KscAbsPath &);
    friend bool operator == (const KscAbsPath &, 
                             const KscAbsPath &);
    friend bool operator != (const KscAbsPath &, 
                             const KscAbsPath &);
    friend KscAbsPath operator + (const KscAbsPath &,
                                  const PltString &);

    unsigned long hash() const;

    PltString getHost() const;              // host eg "host"
    PltString getServer() const;            // server eg "server"
    KscAbsPath getHostAndServer() const;    // eg "//host/server"
    KsPath getVarPath() const;              // path + name
    PltString getNameOnly() const;          // name
    PltString getPathOnly() const;          // path

protected:
    bool validAbsPath() const;
};

//////////////////////////////////////////////////////////////////////
// Inline Implementation
//////////////////////////////////////////////////////////////////////

inline
KscAbsPath::KscAbsPath()
{}

//////////////////////////////////////////////////////////////////////

inline
KscAbsPath &
KscAbsPath::operator = (const KscAbsPath &ap)
{
    KsPath::operator = (ap);
    return *this;
}

//////////////////////////////////////////////////////////////////////

inline
bool
operator == (const KscAbsPath &a, const KscAbsPath &b)
{
    return a._str == b._str;
}

//////////////////////////////////////////////////////////////////////

inline
bool
operator != (const KscAbsPath &a, const KscAbsPath &b)
{
    return a._str != b._str;
}

//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////

inline
unsigned long
KscAbsPath::hash() const
{
    return _str.hash();
}


#endif






