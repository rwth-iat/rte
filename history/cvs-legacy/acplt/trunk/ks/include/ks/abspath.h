/* -*-plt-c++-*- */

#ifndef KSC_FULLPATH_INCLUDED
#define KSC_FULLPATH_INCLUDED

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

//////////////////////////////////////////////////////////////////////

#include "ks/path.h"

//////////////////////////////////////////////////////////////////////
// class KscAbsPath
//   manage an absolute path consisting of host, server and in the 
//   server path information. The path should be formed like 
//   "//host/server/domain1/../var".
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
    KscAbsPath getHostAndServer() const;    // eg "/host/server"
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

inline
KscAbsPath
operator + (const KscAbsPath &path,
            const PltString &var)
{
    PltString temp(path._str);
    temp += "/";
    temp += var;
    return KscAbsPath(temp);
}

//////////////////////////////////////////////////////////////////////

inline
unsigned long
KscAbsPath::hash() const
{
    return _str.hash();
}


#endif






