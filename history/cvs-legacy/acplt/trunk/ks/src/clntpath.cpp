/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/src/clntpath.cpp,v 1.3 1999-09-06 06:50:07 harald Exp $ */
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

/*
 * Written by Markus Juergens <markusj@plt.rwth-aachen.de> with changes
 * for the ACPLT/KS core protocol version 2 (delemiters "/" and ".")
 * by Harald Albrecht <harald@plt.rwth-aachen.de>
 */

//////////////////////////////////////////////////////////////////////

#include "ks/clntpath.h"

//////////////////////////////////////////////////////////////////////

const KsString KscPath::praefixes[KSC_MAX_REL_DEPTH+1] = {
    KsString(""), // "." is now depreciated and my cause problems when
                  // concatenating the componants in face of parts...
    KsString("../"),
    KsString("../../"),
    KsString("../../../"),
    KsString("../../../../"),
    KsString("../../../../../")
};

//////////////////////////////////////////////////////////////////////
// class KscPath
//////////////////////////////////////////////////////////////////////


// ----------------------------------------------------------------------------
// When parsing a KscPath, we check for the following conditions:
//   - path must start with a slash
//   - no double slashes are allowed (esp. not at the beginning!)
//   - no double delemiters are allowed (like "/a..b", "/a/.b", ...), with
//     the exception of "/.a" which is valid and indicates that a is a part
//     of the root domain.
//   - last character is not a slash (exception: the root "/" is valid)
//
bool
KscPath::parse()
{
    const char *sz = (const char *)(*this);
    
    if ( sz && (*sz == '/') ) {

        if ( !sz[1] ) {
	    //
            // It is just the root "/", which is valid, while all other
	    // paths are not allowed to end with a slash.
	    //
            slash_count = 0;
            last_comp = 1;
            return true;
        }

        slash_count = 1;
        last_comp = 1;

        bool previous_was_delemiter = true;
        size_t pos = 0;

        while ( sz[++pos] ) {
            if ( (sz[pos] == '/') || (sz[pos] == '.') ) {
                if ( previous_was_delemiter ) {
		    //
                    // A doubled delemiter...which is usually just plain
		    // invalid. The only exception is a part of a root domain,
		    // like "/.a", which is considered valid.
		    //
		    if ( (sz[pos] != '.') || (pos != 1) ) {
			return false;
		    }
		    //
		    // In case of "/.part" just fall though and accept.
		    // Note that "/.X" currently counts as *ONLY ONE*
		    // component! Especially the relTo() method relies on
		    // this behaviour. And in fact this makes sense (well,
		    // not relying on this behaviour), as "/.X" means the
		    // part "X" of the root object, so there is really only
		    // one component, because we don't count "/" at all.
		    //
                }
		previous_was_delemiter = true;
		slash_count++;
		last_comp = pos+1;
            } else {
                previous_was_delemiter = false;
            }
        }

	//
	// The path must not end with a slash or a dot. Check this and bail
	// out if this doesn't hold true.
	//
        if ( !previous_was_delemiter ) {
	    return true;
	}
    }

    return false;
} // KscPath::parse



// ----------------------------------------------------------------------------
// Given a context path in "to", this method returns a path which is relative
// to the path given in "to". In case both paths share too few components, the
// path returned might also be absolute.
//
KsString
KscPath::relTo(const KscPath &to, size_t maxDepth) const
{
    if ( slash_count && to.slash_count ) {
	//
        // This path as well as the other path are not the root path, so it
	// might make sense to use the relative addressing scheme. But first
	// clamp the heuristic about what differences between paths we still
	// accept for relative addressing...
        //
        if ( maxDepth > KSC_MAX_REL_DEPTH ) {
            maxDepth = KSC_MAX_REL_DEPTH;
        }

        size_t last_common;
	size_t common = inCommon(to, last_common);
	if ( common > 0 ) {
	    size_t up = to.countComponents() - common - 1;
	    if ( up <= maxDepth ) {
		//
		// Skip a leading "/" but not a ".". This is necessary
		// because we're now using the relative addressing scheme
		// without a leading "./". In this case leaving the leading
		// slash in the final relative path would effectively let
		// look it like an absolute path. Bang.
		//
		if ( (*this)[last_common] == '/' ) {
		    ++last_common;
		}
		return KsString(praefixes[up], getSubstrPtr(last_common));
	    }
	}
    } 
    //
    // no relative path if other path or this are root or distance is too
    // big: that is, they don't share enough components to justify relative
    // addressing (note that this is although just calculated using a very
    // simple heuristic, so don't expect too much or anything at all...).
    //
    return *this;
} // KscPath::relTo


// ----------------------------------------------------------------------------
// Find out the number of path components this KscPath as well as another
// KscPath have in common. As you might (or better: should) remember, path
// components are seperated by either "/" or "." depending on the kind of
// relationship between components. In addition to the number of identical
// components an index is returned which indicates the start of the first
// component not being indentical in both KscPath objects. To make this more
// clear:
//   given to strings: this  = "/servers/tserver/2.part"
//   and:              other = "/servers/tserver/2/expires_at"
//   the out parameter last_common points to the slash after the "2":
//                              /servers/tserver/2.part
//                                                ^
//                                                +- last_common
//   another example:  this  = "/A/B.C"
//                     other = "/A/B.C.D"
//                                    ^
//                                    +- last_common
//
size_t
KscPath::inCommon(const KscPath &other, size_t &last_common) const 
{
    const char *sz1 = (const char *)(*this);
    const char *sz2 = (const char *)other;
    const char *sz1begin = sz1;

    size_t delemiters = 0;
    size_t lc = 0;

    while ( *sz1 ) {
	//
	// Sometimes my code (--aldi) looks like what the optimizer's duty
	// should be...
	//
	if ( (*sz1 == '/') || (*sz1 == '.') ) {
	    if ( (*sz2 != '/') && (*sz2 != '.') ) {
		//
		// No delemiter found at the same position in the second
		// string, so we can stop searching now.
		//
		break;
	    }
	    //
	    // Now check for a "/." which will count only as *ONE* delemiter.
	    // In this case we need to skip the apropriate number of
	    // characters in the path...
	    //
	    if ( (*sz1 == '/') && (sz1[1] == '.') 
		 && (*sz2 == '/') && (sz2[1] == '.') ) {
		++sz1; ++sz2;
	    }
	    ++delemiters;
	    lc = sz1 - sz1begin;
	    //
	    // Now fall through and let the next code part check whether
	    // there's no a difference: this time it will check whether the
	    // previous component is terminated by different delemiters
	    // ("." versus "/"). In this case, the search has come to an
	    // end. Otherwise keep on "searchink" (well, I read UFI too).
	    //
	}

	if ( *sz1 != *sz2 ) {
	    //
	    // Oops. This component is different in both paths.
	    //
	    break;
	}
	//
	// Next one (character) please...
	//
	++sz1; ++sz2;
    }
    last_common = lc;
    return delemiters - 1; 
} // KscPath::inCommon



//////////////////////////////////////////////////////////////////////
// class KscPathParser
//////////////////////////////////////////////////////////////////////

KscPathParser::operator KsString () const
{
    return KsString(host_and_server, path_and_name);
}


// ----------------------------------------------------------------------------
// Parse a full resource locator (including the "//host/server" part) and
// return an index of the character where the path part begins.
//
size_t
KscPathParser::pathStart(const char *sz)
{
    if ( sz && (*sz == '/') && (sz[1] == '/') ) {
        size_t pos = 2;
        // search end of first component
        //
        while(sz[pos] && (sz[pos++] != '/')) ;
        if(!sz[pos]) {
            fValid = false;
            return 0;
        }
        // search end of second component
        //
        while(sz[pos] && (sz[pos++] != '/')) ;
        if(sz[pos-1] == '/') {
            fValid = true;
            return pos;
        }
    } 
    
    fValid = false;
    return 0;
} // KscPathParser::pathStart

    
/* End of ks/clntpath.cpp */
