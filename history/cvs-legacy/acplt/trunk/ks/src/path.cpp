/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/src/path.cpp,v 1.1 1997-03-24 12:31:23 martin Exp $ */
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
/* Author: Harald Albrecht <harald@plt.rwth-aachen.de> */

#include "ks/path.h"

#include <string.h>
#include <ctype.h>

#include "ks/ks.h"
#include "ks/string.h"
//#include "ks/array.h"
//#include "ks/serviceparams.h"

/////////////////////////////////////////////////////////////////////////////
//
//
/////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////

void KsPath::findSlashes() 
{
    if (_slash.size() == 0) {
        _valid = false; // TODO better error indication
    }
    if (isValid()) {
        const char *s = _str;
        size_t si = 0;
        //
        // Start with skipping the ../../ garbage at the beginning
        // of the path.
        while (s[si] == '/' || s[si] == '.') ++si;
        //
        // Now save the indices of the remaining slashes
        //
        _slash[0] = si;
        size_t ai = 1; 
        size_t maxidx = _slash.size() - 1 ;
        while (ai < maxidx) {
            if (s[si] == '/') {
                _slash[ai++] = si+1;
            }
            ++si;
        }
        _slash[ai] = PLT_SIZE_T_MAX;
        //
        // and set the range we are interested in.
        // Since this is a construction from a string, 
        // set the full range.
        //
        _last = ai-1;
    }
    PLT_POSTCONDITION(  !isValid() 
                      || (_first == 0 && _slash.size() == size() + 1));
}

//////////////////////////////////////////////////////////////////////

size_t
KsPath::checkAndCount() {
    const char * p = _str;          // running pointer
    bool was_slash = false;    // was last char == '/' ?
    _valid = true; // be optimistic

    if (! *p) {
        _valid = false;  // empty paths are not allowed
        return 0;
    }
    if ( *p == '/' ) {
        // The path is absolute.
        _go_up = 0;
        was_slash = true;
        ++p;
    } else {
        // The path is relative.
        _go_up = 1;
        //
        // handle points
        //
        do {
            if ( *p == '.' ) {           
                // first '.'
                ++p;
                was_slash=false;
                if ( *p == '.' ) {
                    // second '.'
                    ++p; 
                    ++_go_up;
                }
                if ( *p == '/' ) {       
                    was_slash = true;
                    ++p;
                } else {
                    // expected '/' but found something else.
                    _valid = false;
                    return 0;
                }
            } else {
                break;
            }
        } while (was_slash);
    } 
    //
    // no more points expected from now on
    //
    size_t slashes = 0;
    for (; *p; ++p) { 
        char ch = *p;
        if (ch == '/') { 
            if (was_slash) {
                // two '/' in a row
                _valid = false;
                return 0;
            } else {
                was_slash = true;
                ++slashes;
            }
        } else {
            if (! isalnum(ch) && ch != '_') {
                _valid = false;
                return 0;
            }
            was_slash = false;
        }
    }
    return slashes;
}


//////////////////////////////////////////////////////////////////////

KsPath::KsPath(const PltString & str) 
: _str(str),
  _slash(checkAndCount()+2),
  _first(0)
{
    findSlashes();
}

//////////////////////////////////////////////////////////////////////

KsPath::KsPath(const KsPath & abs, const KsPath & rel)
: _str(abs._str.substr(0, abs._slash[abs._last - rel._go_up + 1]),
       rel._str.substr(rel._slash[rel._first], 
                       rel._slash[rel._last + 1] - rel._slash[rel._first])),
    _valid(true),
    _slash(abs.size() - rel._go_up + rel.size() + 1),
    _first(0),
    _last(abs.size()  - rel._go_up + rel.size()),
    _go_up(0)
{
    cout << "XXX" << _str << "XXX" << endl;
    findSlashes();
}
 
//////////////////////////////////////////////////////////////////////

KsPath::KsPath(const KsPath & path, size_t first, size_t last)
: _str(path._str),
  _valid(path._valid),
  _slash(path._slash),
  _go_up(1)
{
    PLT_PRECONDITION(   path.isValid() 
                     && first <= last && last < path.size());
    _first = first;
    _last = last;
}

//////////////////////////////////////////////////////////////////////

KsPath &
KsPath::operator = (const KsPath &rhs)
{
    _str = rhs._str;
    _valid = rhs._valid;
    _slash = rhs._slash;
    _first = rhs._first;
    _last = rhs._last;
    _go_up = rhs._go_up;
    return *this;
}

//////////////////////////////////////////////////////////////////////

#if 0
KsPath 
KsPath::copy(size_t first, size_t last)
{
    PLT_PRECONDITION(isValid() && first <= last && last < size());
    return KsPath(*this, first, last);
}
#endif

//////////////////////////////////////////////////////////////////////

PltString
KsPath::getHead() const
{
    PLT_ASSERT(isValid());
    return (*this)[0];
}

//////////////////////////////////////////////////////////////////////

KsPath
KsPath::getTail() const
{
    PLT_ASSERT(size() > 1);
    return KsPath(*this, 1, _last);
}

//////////////////////////////////////////////////////////////////////

KsPath::operator PltString () const
{
    PltString prefix;
    if (isAbsolute()) {
        prefix = "/";
    } else {
        for (size_t i = 0; i<_go_up-1; ++i) {
            prefix += "../";
        }
    }
    return PltString(prefix,
                     _str.substr(_slash[_first], 
                                 _slash[_last+1] - _slash[_first]));
}

//////////////////////////////////////////////////////////////////////

PltString
KsPath::operator[] (size_t i) const
{
    PLT_PRECONDITION(isValid() && i < size());
    return _str.substr(_slash[i], _slash[i+1] - _slash[i] - 1);
} 

//////////////////////////////////////////////////////////////////////

KsPath
KsPath::resolve(const KsPath & rel)
{
    PLT_PRECONDITION(   isValid() && isAbsolute()
                     && rel.isValid());
    if (rel.isAbsolute()) {
        return rel;
    } else {
        // relative
        if (rel._go_up <= size()) {
            return KsPath(*this, rel);
        } else {
            // error
            return KsPath("//");
        }
    }
}

//////////////////////////////////////////////////////////////////////

#if 0

void resolvePaths(KsArray<KsString> &identifiers,
                  KsArray<KsGetVarItemResult> &results)
{
    int size;
    char const *path_start, *path_end, *p;
    bool bail_out;

    path_start = 0;

    size = identifiers.size();
    for ( int i = 0; i < size; i++ ) {
        if ( *(p = identifiers[i]) != '/' ) {
            //
            // This may be a relative path, as the path name lacks the
            // leading slash. But first we bark at the user, if there is
            // no "current working path", that is, we havn't yet seen a
            // full path name.
            //
            if ( !path_start ) {
                results[i].result = KS_ERR_BADNAME;
                continue;
            }
            bail_out = false;
            while ( strncmp(p, "../", 3) == 0 ) {
                //
                // Go up one domain. This one is called as long as there
                // are leading "../" in the path.
                //
                if ( path_end != path_start ) {
                    path_end = strrchr(path_end - 1, '/');
                    if ( !path_end ) {
                        path_end = path_start;
                    }
                    p += 3;
                } else {
                    path_start = 0;
                    results[i].result = KS_ERR_BADNAME;
                    bail_out = true;
                    break;
                }
            }
            if ( bail_out ) {
                continue;
            }
            //
            // The path is relative to the current "working" path. If
            // the path we're currently parsing contains "./" at the
            // beginning, then we'll skip that.
            //
            if ( strncmp(p, "./", 2) == 0 ) {
                p += 2;
            }
            PltString full_path(path_start, path_end - path_start);
            full_path += p;
            identifiers[i] = full_path;
        } else {
            //
            // It is an absolute path name. We don't need to resolve it,
            // but we have to remember the path for later reference. The
            // "current" path (within the scope of this function) is
            // remembered by "path_start" and "path_end", the latter
            // pointing to the final "/" in the path.
            //
            path_start = p;
            path_end   = p;
            while ( *p ) {
                if ( *p == '/' ) {
                    path_end = p;
                }
                ++p;
            }
        }
    }
} // resolvePaths

//////////////////////////////////////////////////////////////////////

bool checkPath(PltString &path, bool allow_slash, bool allow_point)
{
    const char * p = path;
    if (! *p) {
        return false; // empty paths are not allowed
    }
    //
    // handle points
    //
    if ( allow_point ) {
        do {
            if ( *p == '.' ) {           // first '.'
                ++p;
                if ( *p == '.' ) ++p;    // second '.'
            }
        } while (allow_slash && *p == '/');
    }
    //
    // no more points expected from now on
    //
    while (char ch = *p++) {
        if (   (!isascii(ch)  && !isprint(ch))
            || (!allow_slash && ch == '/')) {
            return false;
        }
    }
    return true;
} 

#endif
//////////////////////////////////////////////////////////////////////
// EOF ks/path.cpp

