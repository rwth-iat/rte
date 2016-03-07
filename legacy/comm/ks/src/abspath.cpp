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

KscAbsPath
operator + (const KscAbsPath &path,
            const PltString &var)
{
    PltString temp((const char *)PltString(path));
    if(temp[temp.len()-1] != '/') {
        temp += "/";
    }
    temp += var;
    return KscAbsPath(temp);
}

//////////////////////////////////////////////////////////////////////
// EOF abspath.cpp
//////////////////////////////////////////////////////////////////////

    
    



