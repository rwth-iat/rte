/* -*-plt-c++-*- */
#ifndef KSC_CLNTPATH_INCLUDED
#define KSC_CLNTPATH_INCLUDED
/* $Header: /home/david/cvs/acplt/ks/include/ks/clntpath.h,v 1.8 1999-09-06 07:16:59 harald Exp $ */
/*
 * Copyright (c) 1996, 1997, 1998, 1999
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

/* Authors: Markus Juergens <markusj@plt.rwth-aachen.de> */
/*          Harald Albrecht <harald@plt.rwth-aachen.de> */


#include "ks/string.h"

// ----------------------------------------------------------------------------
// max depth of relative paths that are generated
//
const size_t KSC_MAX_REL_DEPTH = 5;


// ----------------------------------------------------------------------------
// class KscPath
//   manages an absolute path which should look like
//   /domain1/domain2/../name and provides mechanism to change
//   absolute paths to relative ones
//
class KscPath
: public KsString
{
public:
    KscPath(const char *);
    KscPath(const KsString &);
    KscPath(const KscPath &);
    KscPath(); // well -- a default ctor is most of the time nice...

    PltString & operator = (const char *);
    PltString & operator = (const PltString &);

    bool isValid() const;
    size_t countComponents() const;
    bool isRootDomain() const;

    KsString getName() const;
    bool isNamePart() const;
    KsString getPathOnly() const;

    KsString relTo(const KscPath &, size_t maxDepth = KSC_MAX_REL_DEPTH) const;

protected:

    bool parse();
    size_t inCommon(const KscPath &, size_t &) const;
    const char *getNamePtr() const;
    const char *getSubstrPtr(size_t) const;

    bool fValid;
    size_t slash_count,   // number of slashes
           last_comp;     // start of last component

    static const KsString praefixes[KSC_MAX_REL_DEPTH+1];
};


// ----------------------------------------------------------------------------
// class KscPathParser
// As of the 1.1.3 release this class has now been enhanced not only to parse
// paths (well, to be more precise: resource locators), but also the special
// ones, like //host or //host/server. This class should have named more
// appropriately "KscResourceLocator", but we won't break source compatibility
// -- yet.
//
enum KscPathType {
    KSC_PT_INVALID,         // something invalid.
    KSC_PT_RESOURCELOCATOR, // something like "//host/server/path/to/object"
    KSC_PT_HOST,            // only "//host"
    KSC_PT_HOSTSERVER,      // only "//host/server"
    KSC_PT_PATHONLY         // something like "/path/to/object"
}; // enum KscPathType


class KscPathParser
{
public:
    KscPathParser(const char *sz);

    operator KsString () const;

    KscPathType getType() const;
    bool isValid() const;
    bool isRootDomain() const;

    const KsString &getHostAndServer() const;
    const KscPath &getPathAndName() const;
    KsString getName() const;
    KsString getPathOnly() const;
    bool isNamePart() const;
    KsString getHostname() const;
    KsString getServername() const;

    KsString getParent(bool beyondRoot) const;
    KsString resolve(const char *sz) const;

    bool operator == (const KscPathParser &) const;
    bool operator != (const KscPathParser &) const;

protected:
    bool parseResourceLocator(const char *);

    KscPathType _pathType;       // type of path
    size_t      _pathStartIndex; // index of start of path
    size_t      _hostLen;        // length of host name component
    KsString    _hostserver;     // stores "//host/server"
    KscPath     _fullPath;       // stores "/path/to/object"
}; // class KscPathParser


//////////////////////////////////////////////////////////////////////
// Inline Implementation
//////////////////////////////////////////////////////////////////////

inline
KscPath::KscPath(const char *sz) 
: KsString(sz)
{
    fValid = parse();
}

//////////////////////////////////////////////////////////////////////

inline
KscPath::KscPath(const KsString &str)
: KsString(str)
{
    fValid = parse();
}

//////////////////////////////////////////////////////////////////////

inline
KscPath::KscPath(const KscPath &other)
: KsString(other),
  fValid(other.fValid),
  slash_count(other.slash_count),
  last_comp(other.last_comp)
{}

inline
KscPath::KscPath()
: KsString(),
  fValid(false)
{
} // KscPath::KscPath

// ----------------------------------------------------------------------------
//
inline
PltString &
KscPath::operator = (const char *s)
{
    PltString::operator=(s);
    fValid = parse();
    return *this;
} // KscPath::operator = (const char *)

// ----------------------------------------------------------------------------
//
inline
PltString &
KscPath::operator = (const PltString &s)
{
    PltString::operator=(s);
    fValid = parse();
    return *this;
} // KscPath::operator = (const PltString &)

//////////////////////////////////////////////////////////////////////

inline
bool
KscPath::isValid() const
{
    return fValid;
}

// ----------------------------------------------------------------------------
// Returns true, if path specifies the root domain "/". Otherwise returns
// false (you might have guessed already by now...)
//
inline
bool
KscPath::isRootDomain() const
{
    return fValid && (slash_count == 0);
} // KscPath::isRootDomain

//////////////////////////////////////////////////////////////////////

inline
KsString
KscPath::getName() const
{
    PLT_PRECONDITION(isValid());

    if(slash_count) { 
        return KsString(substr(last_comp));
    } else {
	return KsString();
    } 
}


inline
bool
KscPath::isNamePart() const
{
    PLT_PRECONDITION(isValid());
    
    if ( slash_count && last_comp ) {
	return ((*this)[last_comp - 1] == '.') ? true : false;
    } else {
	return false;
    }
} // KscPath::isNamePart


//////////////////////////////////////////////////////////////////////

inline
KsString
KscPath::getPathOnly() const
{
    PLT_PRECONDITION(isValid());

    if(slash_count > 1) { 
        return KsString(substr(0, last_comp - 1));
    } else { 
	return KsString("/");
    } 
}

//////////////////////////////////////////////////////////////////////

inline
size_t
KscPath::countComponents() const
{
    return slash_count;
}

//////////////////////////////////////////////////////////////////////

inline
const char *
KscPath::getNamePtr() const
{
    return (const char *)(*this) + last_comp;
}

//////////////////////////////////////////////////////////////////////

inline
const char *
KscPath::getSubstrPtr(size_t start_at) const
{
    return (const char *)(*this) + start_at;
}



// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// KscPathParser inline implementation part
//

// ----------------------------------------------------------------------------
// Returns true, if path is valid. Note that this is also the case for special
// resource locators like "//host", "//host/server" and paths without a host
// and server part ("/some/object").
//
inline
bool
KscPathParser::isValid() const
{
    return _pathType != KSC_PT_INVALID;
} // KscPathParser::isValid

// ----------------------------------------------------------------------------
// Returns true, if the resource locator identifies the root domain "/".
//
inline
bool
KscPathParser::isRootDomain() const
{
    return ((_pathType == KSC_PT_RESOURCELOCATOR)
	    || (_pathType == KSC_PT_PATHONLY))
	&& _fullPath.isRootDomain();
} // KscPathParser::isRootDomain


// ----------------------------------------------------------------------------
// Returns the type of resource locator/path once put into this KscPathParser
// object.
//
inline
KscPathType
KscPathParser::getType() const
{
    return _pathType;
} // KscPathParser::getType

// ----------------------------------------------------------------------------
// Returns the host name specified in the resource locator (if applicable),
// otherwise returns an empty string.
//
inline
KsString
KscPathParser::getHostname() const
{
    PLT_PRECONDITION(isValid());

    if ( !isValid() ) {
	return KsString();
    } else {
	return _hostserver.substr(2, _hostLen);
    }
} // KscPathParser::getHostname

// ----------------------------------------------------------------------------
// Returns the server name specified in the resource locator (if applicable),
// otherwise returns an empty string.
inline
KsString
KscPathParser::getServername() const
{
    PLT_PRECONDITION(isValid());

    if ( !isValid() ) {
	return KsString();
    } else {
	return _hostserver.substr(_hostLen + 3);
    }    
} // KscPathParser::getServername

// ----------------------------------------------------------------------------
// Returns the host and server part as "//host/server" or an empty string in
// case the resource locator is invalid or does not contain both the host and
// server components.
//
inline
const KsString &
KscPathParser::getHostAndServer() const
{
    PLT_PRECONDITION(isValid());
    static KsString empty;

	return _hostserver;
} // KscPathParser::getHostAndServer

// ----------------------------------------------------------------------------
// Returns the path and name part of the resource locator, that is, the
// so-called "full path".
//
inline
const KscPath &
KscPathParser::getPathAndName() const
{
    PLT_PRECONDITION(isValid());

    return _fullPath;
} // KscPathParser::getPathAndName

//////////////////////////////////////////////////////////////////////

inline
KsString
KscPathParser::getName() const
{
    PLT_PRECONDITION(isValid());

    return _fullPath.getName();
}

// ----------------------------------------------------------------------------
// Returns true, if the name (or identifier) of the ACPLT/KS communication
// object addressed by the path is a part (thus the last component, the name,
// is separated by "." instead of "/" from the previous components).
//
inline
bool
KscPathParser::isNamePart() const
{
    PLT_PRECONDITION(isValid());

    return (_pathType != KSC_PT_INVALID) ?
	_fullPath.isNamePart() :
	false;
} // KscPathParser::isNamePart

//////////////////////////////////////////////////////////////////////

inline
KsString
KscPathParser::getPathOnly() const
{
    PLT_PRECONDITION(isValid());

    return _fullPath.getPathOnly();
}

//////////////////////////////////////////////////////////////////////

inline bool 
KscPathParser::operator == (const KscPathParser &other) const
{
    PLT_PRECONDITION(isValid() && other.isValid());

    return _pathType == other._pathType
        && _hostserver == other._hostserver
        && _fullPath == other._fullPath;
}
        
//////////////////////////////////////////////////////////////////////

inline bool 
KscPathParser::operator != (const KscPathParser &other) const
{
    PLT_PRECONDITION(isValid() && other.isValid());

    return _pathType != other._pathType
        || _hostserver != other._hostserver
        || _fullPath != other._fullPath;
}


#endif
// End of file ks/clntpath.h
