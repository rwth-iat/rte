/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/src/clntpath.cpp,v 1.10 2007-04-25 12:57:20 martin Exp $ */
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

/*
 * Written by Markus Juergens <markusj@plt.rwth-aachen.de> with changes
 * and enhancements for the ACPLT/KS core protocol version 2 (delemiters
 * "/" and now ".") by Harald Albrecht <harald@plt.rwth-aachen.de>
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



// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// class KscPathParser


// ----------------------------------------------------------------------------
// Construct a KscPathParser object and parse the given resource locator.
// Harald: This has been changed to cope for //host and //host/server resource
// locators as well as for paths without the host/server components. This makes
// the KscPathParser class much more versatile and will please client writers
// much (so as myself hacking down the !OleOle! library...).
//
KscPathParser::KscPathParser(const char *sz)
: _pathType(KSC_PT_INVALID),
  _pathStartIndex(0),
  _hostLen(0),
  _hostserver(),
  _fullPath()
{
    if ( parseResourceLocator(sz) ) {
	//
	// In case of a full blown resource locator or a simple path, we need
	// to parse the path.
	//
	if ( (_pathType == KSC_PT_RESOURCELOCATOR)
	     || (_pathType == KSC_PT_PATHONLY) ) {
	    _fullPath = sz + _pathStartIndex;
	    if ( !_fullPath.isValid() ) {
		_pathType = KSC_PT_INVALID;
		_fullPath = sz; // store for possible later references by caller.
	    }
	}
	// else: leave _path empty, because there's no path.
    } else {
	_fullPath = sz; // store for possible later references by caller.
    }
} // KscPathParser::KscPathParser


// ----------------------------------------------------------------------------
// Return the full path as once given to the path parser. Note that in case of
// invalid paths, _fullPath will contain the old path as handed to the ctor
// of this class.
//
KscPathParser::operator KsString () const
{
    switch ( _pathType ) {
    case KSC_PT_HOST:
    case KSC_PT_HOSTSERVER:
	return KsString(_hostserver);
    case KSC_PT_RESOURCELOCATOR:
	return KsString(_hostserver, _fullPath);
    default:
	return _fullPath;
    }
} // KscPathParser::operator KsString ()


// ----------------------------------------------------------------------------
// Parse a full resource locator (including the "//host/server" part) and
// return true if parsing succeeded. In case of an error, false is returned
// instead. As a necessary side effect, this method also sets the _pathType
// and _pathStartIndex member attributes.
//
bool
KscPathParser::parseResourceLocator(const char *sz)
{
    const char *p = sz;

    //
    // If the given path/resource locator is either empty or does not start
    // with at least one slash, it is invalid.
    //
    if ( !p || (*p++ != '/') ) {
	_pathType = KSC_PT_INVALID;
	_pathStartIndex = 0;
	_hostLen = 0;
	return false; // just plain invalid...
    }
    //
    // Now check for full resource locators, starting with two slashes, or
    // for paths, starting with only a single slash.
    //
    if ( *p == '/' ) {
	//
	// Looks very promising ... like a full resource locator. So let's
	// find out whether there's more than just the host name, like an
	// ACPLT/KS server name, or even a path...
	//
	// Skip the first two slashes "//" and then make sure that there's
	// at least something that could be a host name, i.e. there's at least
	// one character following the second slash.
	//
	++p;
	if ( !*p && (*p != '/') ) {
	    _pathType = KSC_PT_INVALID;
	    _pathStartIndex = 0;
	    _hostLen = 0;
	    return false; // It's just plain invalid...
	}
	//
	// Search for the next slash, which separates the server name from
	// the host name part. If it is missing, then this resource locator
	// contains only a host name. But that's also okay.
	//
	while ( *++p && (*p != '/') ) { 
	}
	_hostLen = p - sz - 2;
	if ( !*p ) {
	    _pathType = KSC_PT_HOST;
	    _pathStartIndex = 0;
	    _hostserver = sz;
	    return true;
	}
	//
	// Now find the slash of the root domain, which just comes directly
	// after the server name component. If it's missing, then this path
	// is of the form "//host/server". Otherwise it is a full blown
	// resource locator with even a path part. Once again, make sure that
	// the server name consists at least of a single character, and not
	// of a slash or something other...
	//
	++p;
	if ( !*p && (*p != '/') ) {
	    _pathType = KSC_PT_INVALID;
	    _pathStartIndex = 0;
	    return false; // It's just plain invalid...
	}
	while ( *++p && (*p != '/') ) { 
	}
	if ( !*p ) {
	    _pathType = KSC_PT_HOSTSERVER;
	    _pathStartIndex = 0;
	    _hostserver = sz;
	    return true;
	}
	//
	// Wow. Looks like a valid resource locator including host and server
	// name components. To decide whether the path is valid is now up to
	// the caller. We've only decided that is looks quite promising.
	//
	_pathType = KSC_PT_RESOURCELOCATOR;
	_pathStartIndex = p - sz; // and here comes the path...
	_hostserver = KsString(sz, _pathStartIndex);
	return true;
    } else {
	//
	// Well, it doesn't contain the host/server part, so we just parse
	// the path itself. We don't touch the host and server components,
	// so they will remain empty.
	//
	_pathType = KSC_PT_PATHONLY;
	_pathStartIndex = 0; // and here comes the path...
	return true;
    }
} // KscPathParser::parseResourceLocator


// ----------------------------------------------------------------------------
// Return the path of the next higher hierarchy, that is what we call the
// "parent" of a given ACPLT/KS communication object (specified by path).
//
KsString
KscPathParser::getParent(bool beyondRoot) const
{
    PLT_PRECONDITION(isValid());

    switch ( _pathType ) {
    case KSC_PT_HOST:
	//
	// In case of only a host name, there's no higher level of hierarchy,
	// so we'll return an empty string.
	//
	return KsString();

    case KSC_PT_HOSTSERVER:
	//
	// In case of "//host/server", now return "//host" only.
	//
	return _hostserver.substr(0, _hostLen + 2);

    case KSC_PT_RESOURCELOCATOR:
	//
	// Return next higher level of hierarchy. In case we've already
	// standing at the root domain, depending on the parameter beyondRoot
        // we either return an empty string or the root domain path again.
	//
	if ( _fullPath.isRootDomain() ) {
	    if ( beyondRoot ) {
		//
		// The next level above "//host/server/" is "//host" (and not
		// "//host/server").
		//
		return _hostserver.substr(0, _hostLen + 2);
	    } else {
		//
		// Don't go beyond root.
		//
		return KsString(_hostserver, _fullPath);
	    }
	}
	//
	// Now that's easy: the only case which now remains is returning the
	// next higher hierarchy of the path.
	//
	return KsString(_hostserver, _fullPath.getPathOnly());

    case KSC_PT_PATHONLY:
	//
	// Return next higher level of hierarchy without any "//host/server"
	// part. In case we've already standing at the root domain, depending
	// on the parameter beyondRoot we either return an empty string or the
	// root domain path again.
	//
	if ( _fullPath.isRootDomain() ) {
        if ( beyondRoot ) {
	        return KsString();
        } else {
            return (KsString) _fullPath;
        }
	}
	return _fullPath.getPathOnly();

    default:
	//
	// Well, invalid resource locators/paths are just invalid. So we can't
	// return a valid path.
	//
	return KsString();
    }
} // KscPathParser::getParent


// ----------------------------------------------------------------------------
// Helper structure needed by KscPathParser::resolve
//
struct _KscPPComponent {
    _KscPPComponent *prev;
    _KscPPComponent *next;
    const char      *p;    // points to separator of component
    size_t           len;  // len of component including separator
    bool             joinWithPrevious;
}; // struct _KscPPComponent


// ----------------------------------------------------------------------------
// A nice workhorse for ACPLT/KS clients doing much path magic, like resolving
// relative paths, going up within a path / resource locator component by
// component, and so on...
//
KsString
KscPathParser::resolve(const char *sz) const
{
    if ( *sz == '/' ) {
        //
	// It is either a full blown resource locator or a path starting
	// with the root domain. So check what we've got...
	//
        if ( *++sz == '/' ) {
	    //
	    // It's a real resource locator. So just return it to the
	    // caller. We don't have to do anything more.
	    //
	    return KsString(sz);
	} else {
	    //
	    // The caller only gave us a path, starting at the root
	    // domain. So if this object holds a resource locator and is
	    // valid, then we can complete the path with host and server
	    // components.
	    //
	    if ( (_pathType == KSC_PT_RESOURCELOCATOR)
		 || (_pathType == KSC_PT_HOSTSERVER) ) {
		return _hostserver + sz;
	    }
	    //
	    // Error! We can't complete the path to get a resource locator.
	    //
	    return KsString();
	}
    }
    //
    // It's a relative path, we have to resolve now. So things will get more
    // complicated than in the easy cases above. We can only resolve the given
    // path if we're currently not invalid. In case we're holding a path only
    // (no host, no server specified), then we will later return a path
    // (albeit a resolved one).
    //
    if ( (_pathType == KSC_PT_INVALID) || (_pathType == KSC_PT_HOST) ) {
	return KsString();
    }

    if ( !*sz ) {
	return (KsString) *this;
    }

    //
    // The idea is to store each component parsed from the given path (in sz)
    // in a stack. This way we can easily back up in case of ".." (go up one
    // level). To back up within the resource locator stored by this object,
    // keepComponents is used again as some kind of stack.
    //
    size_t           keepComponents = _fullPath.countComponents();
    _KscPPComponent *stackTop = 0;
    _KscPPComponent *stackBottom = 0;
    const char      *p = sz;
    char             c;
    bool             dotAllowed = true;
    KsString         aliasWithRoot;
    bool             canBeJoined = false;

    while ( *p ) {
	//
	// We're now standing at the beginning of a new component.
	//
	if ( *p == '.' ) {
	    //
	    // Find out whether we're not standing at a dot delemiter but
	    // rather have found one of the special components "../" and "./".
	    //
	    if ( *++p == '/' ) {
		//
		// We've found the "./" component, this just means "stay at
		// this level" and therefore can be skipped.
		//
		sz = p;
		if ( *p ) {
                    p++;
                }
		dotAllowed = true;
		canBeJoined = false;
		continue; // parse next component.
	    }
	    if ( *p == '.' ) {
		//
		// Only "../" or "..\0" is now allowed at this point. This
		// then instructs us to go up one level.
		//
		if ( (*++p != '/') && *p ) {
		    return KsString(); // invalid path
		}

		if ( stackTop ) {
		    //
		    // Remove top item from the stack which stores the
		    // items parsed in this path (pop it off the stack).
		    //
		    _KscPPComponent *comp = stackTop;
		    stackTop = comp->prev;
		    if ( !stackTop ) {
			//
			// Now the stack is empty.
			//
			stackBottom = 0;
		    }
		    delete comp;
		} else {
		    //
		    // Remove top item from path stored inside this object.
		    // This is the resource locator we resolve the new path
		    // against. Note that we don't go up beyond root here.
		    //
		    if ( keepComponents > 0 ) {
			--keepComponents;
		    }
		}
                //
                // Remember beginning of next component. Also skip the
                // preceeding delemiter (if we´ve not already reached the
                // end of the string).
                //
		sz = p;
		if ( *p ) {
                    p++;
                }
		dotAllowed = true;
		canBeJoined = false;
		continue;
	    }
	    //
	    // The remaining case now can only be a dot delemiter indicating
	    // a part following the special components "./" or "../", for
	    // instance, "../.X" which means "part X of the parent".
	    //
	    if ( !dotAllowed ) {
		return KsString(); // invalid path
	    }
            //
            // Make sure that something like an identifier seems to follow,
            // otherwise return an invalid path.
            //
            if ( !*p ) {
                return KsString();
            }
	    sz = p - 1;
	    dotAllowed = false;
	    //
	    // Now fall through and parse the following identifier.
	    //
	}
	//
	// Now at this place only ACPLT/KS identifiers are allowed.
	// Well -- I'm relaxing here and just parsing for the end of the
	// component, which can be either the end of the string, or
	// one of the delemiters "/" and ".".
	//
	while ( ((c = *p) != 0) && (c != '/') && (c != '.') ) {
	    ++p;
	}
	//
	// We've found the end of the component. "sz" still points to
	// the beginning of the components (its separator), while p now
	// points to the first character after the component. All we
	// need to do now is to add this component to the stack. Note that
	// in case the first component of the path to be resolved specifies
	// a child (and not a part or "./" or "../"), then the delemiter is
	// missing. It will be added later then building the final resolved
	// path.
	//
	_KscPPComponent *comp = new _KscPPComponent;
	if ( !comp ) {
	    return KsString();
	}
	if ( !stackBottom ) {
	    stackBottom = comp;
	}
	if ( !stackTop ) {
	    //
	    // First item in stack...
	    //
	    comp->prev = 0;
	    comp->next = 0;
	    stackTop = comp;
	} else {
	    //
	    // Put item on top of stack...
	    //
	    comp->prev = stackTop;
	    comp->next = 0;
	    stackTop->next = comp;
	    stackTop = comp;
	}

	comp->p = sz;
	comp->len = p - sz;
	comp->joinWithPrevious = canBeJoined;
	sz = p; // advance to next separator
	if ( c ) {
	    ++p;
	}
	canBeJoined = true;
    }

    //
    // Now build the final resource locator from the components stored in the
    // two stacks.
    //
    KsString final(_hostserver);
    //
    // Add all components from the first stack which we had to keep for the
    // resolved resource locator.
    //
    if ( keepComponents > 0 ) {
	sz = (const char *) _fullPath;
	//
	// Skip the leading (root domain) slash and an optionally following
	// part delemiter. Then find the end of the last component to keep.
	//
	if ( sz[1] == '.' ) { // okay, because its always at least "/\0".
	    ++sz;
	}
	for ( int comps = keepComponents; comps; --comps ) {
	    ++sz; // skip previous delemiter char
	    while ( ((c = *sz) != 0) && (c != '/') && (c != '.') ) {
		++sz;
	    }
	}
	final += _fullPath.substr(0, sz - (const char *) _fullPath);
    }
    //
    //
    // Finally add all components waiting on the second stack containing the
    // new components introduced the the path to be resolved.
    //
    _KscPPComponent *comp;
    while ( stackBottom ) {
	comp = stackBottom;

	if ( keepComponents == 0 ) {
	    //
	    // Special case: if the first component of the resulting path
	    // (that is the root component) is a part, then we need to add
	    // a leading slash first, so we get "//host/server/.part" instead
	    // of "//host/server.part" (the latter would be invalid).
	    //
	    if ( *(comp->p) == '.' ) {
		final += KsString("/");
	    }
	    keepComponents = 1;
	}
	//
	// Now add the component to the final path. As an optimization, we
	// swallow as much components as possible and adding them in one run.
	// This can only be done if they have been stored consecutive within
	// the path to be resolved.
	//
	const char *p = comp->p;
	size_t      len = comp->len;

	if ( ((c = *p) != '/') && (c != '.') ) {
	    //
	    // If the first component of the given path to be resolved
	    // specifies a child, it will miss its "/" delemiter (because it's
	    // a relative path). So we need to add it now.
	    //
	    final += "/";
	}

	stackBottom = stackBottom->next;
	delete comp;
	while ( stackBottom && stackBottom->joinWithPrevious ) {
	    comp = stackBottom;
	    len += comp->len;
	    stackBottom = stackBottom->next;
	    delete comp;
	}
	final += KsString(p, len);
    }
    //
    // That's it. Now just return the resolved path.
    //
    return final;
} // KscPathParser::resolve

    
/* End of ks/clntpath.cpp */
