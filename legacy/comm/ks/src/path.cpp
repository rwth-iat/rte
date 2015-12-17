/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/src/path.cpp,v 1.15 2007-04-25 12:57:21 martin Exp $ */
/*
 * Copyright (c) 1996, 2001
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
/* Author: Harald Albrecht <harald@plt.rwth-aachen.de> */

#include "ks/path.h"

#include <string.h>
#include <ctype.h>

#include "ks/ks.h"
#include "ks/string.h"
#include "ks/conversions.h"


// ---------------------------------------------------------------------------
// Parse the path string into path components, separated by slashes or dots
// depending on whether they denote children or parts. The indices of the
// beginnings of the path components are stored for later (quick) use.
// Note that the "../" and "./" components are not indexed, but rather we
// remember how many components we should go up within the object tree.
//
void KsPath::findSlashes() 
{
    if ( _slash.size() == 0 ) {
	//
	// We don't have exceptions (well, we don't want to use them),
	// so this is the only way to indicate failure.
	//
        _valid = false;
    }
    if ( isValid() ) {
        const char *s = _str;
        size_t si = 0;
        //
        // Start with skipping the "../../" garbage at the beginning
        // of the path.
	//
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
} // KsPath::findSlashes


// ---------------------------------------------------------------------------
// Parse the path string, checking for well-formedness, and at the same time
// count the number of components.
//
size_t
KsPath::checkAndCount() {
    const char * p = _str;         // running pointer
    bool was_slash = false;        // was last char == '/' ?
    _valid = true; // be optimistic

    //
    // Empty paths are not allowed, so bail out as soon as possible.
    //
    if ( !*p ) {
        _valid = false;
        return 0;
    }
    //
    //
    //
    if ( *p == '/' ) {
        // The path is absolute.
        _go_up = 0;
        was_slash = true;
        ++p;
    } else {
	//
        // The path is relative, so we need to skip the "stay here" and
	// "go up" components in front of the more interesting part of
	// the path.
	//
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
	        // two '/' in a row are not allowed
	        _valid = false;
	        return 0;
            } else {
                was_slash = true;
                ++slashes;
            }
        } else {
	    // ##TODO: Check for valid characters in string and bail out
	    // if garbage is found...
            was_slash = false;
        }
    }
    return slashes;
} // KsPath::checkAndCount


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
    _first = first + path._first;
    _last = last + path._first;
}


//////////////////////////////////////////////////////////////////////

KsPath::KsPath()
: _str("/"),
  _slash(2),
  _first(0),
  _last(0),
  _go_up(0)
{
    if (_slash.size() == 2) {
        _slash[0] = 1;
        _slash[1] = 1;
        _valid = true;
    } else {
        _valid = false;
    }
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
    return KsPath(*this, 1, size()-1);
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
    
    return _str.substr(_slash[_first + i], 
                       _slash[_first+i+1] - _slash[_first+i] - 1);
} 


// ---------------------------------------------------------------------------
// Given a context, resolve this path against it and return the result
// (which now is an absolute path) as a new path object. In case something
// goes boing, return an invalid path.
//
KsPath
KsPath::resolve(const KsPath & rel)
{
    PLT_PRECONDITION(   isValid() && isAbsolute()
                     && rel.isValid());
    if ( rel.isAbsolute() ) {
	//
	// Path is already absolute, so just return it.
	//
        return rel;
    } else {
        //
	// Relative path: we have to resolve it. But then, just delegate.
	//
        if ( rel._go_up <= size() ) {
            return KsPath(*this, rel);
        } else {
            // error
            return KsPath("//");
        }
    }
}


// ---------------------------------------------------------------------------
// Given a set of paths, resolve all relative paths into absolute paths. The
// set forms a context, were the first path must not be relative, as it will
// first set the context for the other paths to follow. This method also
// checks for validity of the paths according to the ACPLT/KS protocol
// specification.
//
void
KsPath::resolvePaths(const PltArray<KsString> &ids,
                     PltArray<KsPath> &paths,
                     PltArray<KS_RESULT> &res) 
{
    size_t size = ids.size();
    PLT_PRECONDITION(       size   == paths.size() 
                     && res.size() == paths.size());

    //
    // Start with an invalid context to prevent the first path from
    // being relative without a context in which it could be resolved.
    //
    KsPath context("//");

    size_t idx;
    for ( idx = 0; idx < size ; ++idx ) {
        KsPath & path = paths[idx]; // shortcut
        path = KsPath(ids[idx]);
        if ( path.isValid() ) {
	    //
	    // Now if we have either an absolute path or the path is
	    // relative and the context is valid either, then mark this
	    // path for later resolving.
	    //
            if ( context.isValid() ) {
		//
                // Resolve path based on the context path.
		//
                path = context.resolve(path);
		//
		// Although the relative path was syntactically correct,
		// resolving might not have been possible, so the resolve
		// method might have returned an invalid path to indicate
		// this. So we have to check validity of the resolved
		// path (again).
		//
                res[idx] = path.isValid() ? KS_ERR_OK : KS_ERR_BADPATH;
            } else {
		//
                // The previous path was invalid, so we lost context
                // and thus we can not resolve the current path in case
                // it is not absolute. We already know that at least
		// the current path is valid.
		//
                res[idx] = path.isAbsolute() ? KS_ERR_OK : KS_ERR_BADPATH;
            }
        } else {
	    //
	    // Given path does not adhere to the ACPLT/KS protocol
	    // specification. Flag it as invalid.
	    //
            res[idx] = KS_ERR_BADPATH;
        }
	// In every case, the current path now forms the context for
	// the next path to resolve, regardless of whether the current
	// path is valid or not.
	//
        context = path;
    } // for each id
    //
    // Finally decode all encoded characters (%xx) in all the absolute
    // paths. We have to do this after resolving all paths, otherwise
    // we will be badly hit when reparsing paths containing slashes or dots
    // -- and reparsing is necessary when resolving a path with relation
    // to another one.
    //
    for ( idx = 0; idx < size ; ++idx ) {
        if( res[idx] == KS_ERR_OK ) {
            res[idx] = paths[idx].decodePercents();
        }
    }
} // KsPath::resolvePaths


// ---------------------------------------------------------------------------
// Convert from external encoded representation using percents for special
// characters into internal representation.
//
KS_RESULT
KsPath::decodePercents()
{
    KsString new_str;
    KS_RESULT res = ksStringFromPercent(_str, new_str);

    if ( res == KS_ERR_OK ) {
	//
        // Iterate over parts of path and adjust slash positions.
        // Each percent found in the old string shortens the current
        // path by two as "%xx" was converted into a single char.
        // As the whole string was changed, we need to iterate over all
        // slashes except the first and last one (the last one denotes
	// the end of the path).
        //
        size_t count = 0;
        size_t slash_index = _slash[0];
        for ( size_t i = 1; i < _slash.size() - 1; i++ ) {
	    size_t next_slash_index = _slash[i];
            for ( size_t j = slash_index; j < next_slash_index; j++ ) {
                if ( _str[j] == '%' ) count++;
            }
            slash_index = next_slash_index;
            if ( count ) {
                _slash[i] -= 2 * count;
            }
        }
	//
	// Use decoded string as the new path string. The slash indices
	// have already been corrected above.
	//
        _str = new_str;
    }
    return res;
} // KsPath::decodePercents
                

// EOF ks/path.cpp

