/* -*-plt-c++-*- */

#ifndef KSC_CLNTPATH_INCLUDED
#define KSC_CLNTPATH_INCLUDED

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

#include "ks/string.h"

//////////////////////////////////////////////////////////////////////
// max depth of relative paths that are generated
//
const size_t KSC_MAX_REL_DEPTH = 5;

//////////////////////////////////////////////////////////////////////
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

    bool isValid() const;
    size_t countComponents() const;
    KsString getName() const;
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


//////////////////////////////////////////////////////////////////////
// class KscPathParser
//
class KscPathParser
{
public:
    KscPathParser(const char *);

    operator KsString () const;

    bool isValid() const;

    const KsString &getHostAndServer() const;
    const KscPath &getPathAndName() const;
    KsString getName() const;
    KsString getPathOnly() const;

    // TODO:
    //   currently paths with different host and server 
    //   are considered to be not comparable by <, <=, >, >=
    //   => find a better solution
    //
    bool operator == (const KscPathParser &) const;
    bool operator != (const KscPathParser &) const;
    bool operator <  (const KscPathParser &) const;
    bool operator <= (const KscPathParser &) const;
    bool operator >  (const KscPathParser &) const;
    bool operator >= (const KscPathParser &) const;

protected:
    size_t pathStart(const char *);

    bool fValid;
    KsString host_and_server;
    KscPath path_and_name;
};

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

//////////////////////////////////////////////////////////////////////

inline
bool
KscPath::isValid() const
{
    return fValid;
}

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

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

inline
KscPathParser::KscPathParser(const char *sz)
: host_and_server(sz, pathStart(sz)-1),
  path_and_name(sz + host_and_server.len())
{
    fValid &= path_and_name.isValid();
}

//////////////////////////////////////////////////////////////////////

inline
bool
KscPathParser::isValid() const
{
    return fValid;
}

//////////////////////////////////////////////////////////////////////

inline
const KsString &
KscPathParser::getHostAndServer() const
{
    PLT_PRECONDITION(isValid());

    return host_and_server;
}

//////////////////////////////////////////////////////////////////////

inline
const KscPath &
KscPathParser::getPathAndName() const
{
    PLT_PRECONDITION(isValid());

    return path_and_name;
}

//////////////////////////////////////////////////////////////////////

inline
KsString
KscPathParser::getName() const
{
    PLT_PRECONDITION(isValid());

    return path_and_name.getName();
}

//////////////////////////////////////////////////////////////////////

inline
KsString
KscPathParser::getPathOnly() const
{
    PLT_PRECONDITION(isValid());

    return path_and_name.getPathOnly();
}

//////////////////////////////////////////////////////////////////////

inline bool 
KscPathParser::operator == (const KscPathParser &other) const
{
    PLT_PRECONDITION(isValid() && other.isValid());

    return host_and_server == other.host_and_server
        && path_and_name == other.path_and_name;
}
        
//////////////////////////////////////////////////////////////////////

inline bool 
KscPathParser::operator != (const KscPathParser &other) const
{
    PLT_PRECONDITION(isValid() && other.isValid());

    return host_and_server != other.host_and_server
        || path_and_name != other.path_and_name;
}

//////////////////////////////////////////////////////////////////////
    
inline bool 
KscPathParser::operator <  (const KscPathParser &other) const
{
    PLT_PRECONDITION(isValid() && other.isValid() && 
                     host_and_server == other.host_and_server);

    return path_and_name < other.path_and_name;
}

//////////////////////////////////////////////////////////////////////
    
inline bool 
KscPathParser::operator <= (const KscPathParser &other) const
{
    PLT_PRECONDITION(isValid() && other.isValid() && 
                     host_and_server == other.host_and_server);

    return path_and_name <= other.path_and_name;
}

//////////////////////////////////////////////////////////////////////
    
inline bool 
KscPathParser::operator > (const KscPathParser &other) const
{
    PLT_PRECONDITION(isValid() && other.isValid() && 
                     host_and_server == other.host_and_server);

    return path_and_name > other.path_and_name;
}

//////////////////////////////////////////////////////////////////////
    
inline bool 
KscPathParser::operator >= (const KscPathParser &other) const
{
    PLT_PRECONDITION(isValid() && other.isValid() && 
                     host_and_server == other.host_and_server);

    return path_and_name >= other.path_and_name;
}

//////////////////////////////////////////////////////////////////////
// EOF clntpath.h
//////////////////////////////////////////////////////////////////////

#endif






