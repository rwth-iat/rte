/* -*-plt-c++-*- */

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

#include "ks/abspath.h"

//////////////////////////////////////////////////////////////////////

KscAbsPath::KscAbsPath(const PltString &s)
: KsPath(s)
{
    _valid = _valid && validAbsPath();
}

//////////////////////////////////////////////////////////////////////

KscAbsPath::KscAbsPath(const char *s)
: KsPath(s)
{
    _valid = _valid && validAbsPath();
}

//////////////////////////////////////////////////////////////////////

bool
KscAbsPath::validAbsPath() const
{
    return (size() >= 2)
        && isAbsolute();
}

//////////////////////////////////////////////////////////////////////

PltString
KscAbsPath::getHost() const
{
    PLT_ASSERT(validAbsPath());
    return getHead();
}

//////////////////////////////////////////////////////////////////////

PltString
KscAbsPath::getServer() const
{
    PLT_ASSERT(validAbsPath());
    return (*this)[1];
}

//////////////////////////////////////////////////////////////////////

KscAbsPath
KscAbsPath::getHostAndServer() const
{
    PLT_ASSERT(validAbsPath());

#if 0
    PltString prefix("/");

    prefix += _str.substr(_slash[_first], 
                          _slash[_first+2] - _slash[_first] - 1);

    return prefix;
#endif

    return _str.substr(0, _slash[_first+2] - 1);
}

//////////////////////////////////////////////////////////////////////

KsPath
KscAbsPath::getVarPath() const
{
    PLT_PRECONDITION(validAbsPath() && (size() > 2));
    return KsPath( _str.substr(_slash[_first+2]-1,
                               _slash[_last+1] - _slash[_first+2]));
}

//////////////////////////////////////////////////////////////////////

PltString
KscAbsPath::getNameOnly() const
{
    PLT_PRECONDITION(validAbsPath() && size() > 2);

    return _str.substr(_slash[_last],
                       _slash[_last+1] - _slash[_last] - 1);
}

//////////////////////////////////////////////////////////////////////

PltString
KscAbsPath::getPathOnly() const
{
    PLT_PRECONDITION(validAbsPath() && size() > 2);

    if( size() == 3 ) {
        return PltString("/");
    }
    else {
        return _str.substr(_slash[_first+2]-1,
                           _slash[_last] - _slash[_first+2]);
    }
}

//////////////////////////////////////////////////////////////////////
// EOF abspath.cpp
//////////////////////////////////////////////////////////////////////

    
    



