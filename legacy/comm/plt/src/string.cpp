/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/plt/src/string.cpp,v 1.28 2008-09-22 08:26:09 henning Exp $ */
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
/* Author: Martin Kneissl <martin@plt.rwth-aachen.de> */


//////////////////////////////////////////////////////////////////////

#include "plt/string.h"
#include <ctype.h>
#include <stdio.h>

#if PLT_SYSTEM_NT && !PLT_COMPILER_CYGWIN
// That's ridiculous:
#include <strstrea.h>
#else
#if PLT_USE_DEPRECIATED_HEADER
#include <strstream.h>
#else
#include <sstream>
#endif
#endif

PltAllocator<PltString::srep> PltString::_srep_allocator;
#if PLT_INSTANTIATE_TEMPLATES
template class PltAllocator<PltString::srep>;
#endif

//////////////////////////////////////////////////////////////////////

#if PLT_DEBUG_INVARIANTS
bool
PltString::invariant() const
{
    return (!p 
            || 
            (p->refcount > 0 && p->s 
// && strlen(p->s) == p->len
));
}
#endif

//////////////////////////////////////////////////////////////////////

PltString::PltString()
{
    p = new srep;
    if (p) {
        p->len = 0;
        p->s = new char [1];
        if (p->s) {
            (p->s)[0]=0;
        } else {
            delete p;
            p = 0;
        }
    }
    PLT_CHECK_INVARIANT();
}

//////////////////////////////////////////////////////////////////////
        
PltString::PltString(const char *s, size_t len_arg)
{
    PLT_PRECONDITION(s);
	p = new srep;
    if (p) {
        size_t s_len = strlen(s);
        size_t len = s_len < len_arg ? s_len : len_arg;
        p->s = new char[len + 1];
        if (p->s) {
            memcpy(p->s, s, len);
            p->s[len] = 0;
            p->len = len;
        } else {
            delete p;
            p = 0;
        }
    }
    PLT_CHECK_INVARIANT();
}

//////////////////////////////////////////////////////////////////////

PltString::PltString(const PltString &r)
{
    p = r.p ;   
    if (r.p) {
        r.p->refcount++;
    }
    PLT_CHECK_INVARIANT();
}

//////////////////////////////////////////////////////////////////////

PltString::~PltString()
{
    if (p) {
        if ( --(p->refcount) == 0 ) {
            delete [] (p->s);
            delete p;
        }
    }
}

//////////////////////////////////////////////////////////////////////

PltString::PltString(size_t sz, char *s)
{
    PLT_PRECONDITION(s);

    p = new srep;

    if (p) {
        p->len = sz;
        p->s = s;
        (p->s)[sz]=0;
    } else {
        // free memory
        delete [] s;
    }

    PLT_CHECK_INVARIANT(); // p->len is temporarily != strlen(p->s)
}

//////////////////////////////////////////////////////////////////////

PltString
PltString::fromInt(int i)
{
    static char buffer[64];
    sprintf(buffer, "%d", i);
    return PltString(buffer);
}

//////////////////////////////////////////////////////////////////////

PltString & 
PltString::operator = (const PltString & r) 
{
    PLT_PRECONDITION( ok() && r.ok() );
    
    (r.p->refcount) ++;

    if ( --(p->refcount) == 0 ) {
        delete [] p->s;
        delete p;
    }
    p = r.p;
    PLT_CHECK_INVARIANT();
    PLT_POSTCONDITION( ok() );
    return *this;
}

//////////////////////////////////////////////////////////////////////

PltString &
PltString::operator = (const char *sa) 
{
    PLT_PRECONDITION(sa);

    if (!p) return *this; // remain in bad state

    size_t len = strlen(sa);
    char * s = new char[len + 1];

    if (s) {
        // Storage has been reserved.
        if (p->refcount > 1) {
            // clone to maintain value semantics
            p->refcount --;
            p = new srep;
        } else {
            delete [] p->s;
        }
        if (p) {
            // Now copy string
            strcpy(s, sa);
            p->s = s;
            p->len = len;
        } else {
            // No representation, no copy... (in bad state now)
        }
    } else {
        // Failed to reserve storage for the characters.
        // Go into bad state.
        if (--(p->refcount) == 0) {
            delete [] p->s;
            delete p;
        }
        p = 0;
    }
    PLT_CHECK_INVARIANT();
    return *this;
}

//////////////////////////////////////////////////////////////////////

const char &
PltString::operator [] (size_t i) const 
{
    PLT_PRECONDITION( ok() && i < p->len );
    return (p->s)[i];
}

//////////////////////////////////////////////////////////////////////

PltString
PltString::substr(size_t first, size_t len) const
{
    if (p) {
        if (first == 0 && len >= p->len) {
            // whole string
            return *this;
        } else {
            // substring
            if (first < p->len) {
                // at least one character
                return PltString(p->s + first, len);
            }
        }
    }
    return PltString(); // empty String
}

//////////////////////////////////////////////////////////////////////


// This one is critical. If the user had to check ok() all the time
// it would be awful, but the check has to be done.
// Here are the rules:
// 1. check ok()
// 2. use operator[]
// 3. repeat 2.
// 4. check ok().
// If 4. tells you NOT ok(), none of your references were ok.

char &
PltString::operator [] (size_t i)
{
    if ( ok() ) {
        cloneIfNeeded();
    }
    PLT_CHECK_INVARIANT();
    if (ok()) {
        return p->s[i];
    } else {
        static char dummy = 'A';
        return dummy;
    }
}

//////////////////////////////////////////////////////////////////////

PltString &
PltString::operator += (const PltString & str)
{
    PLT_PRECONDITION( ok() && str.ok() );

    srep *np = new srep;
    if (np) {
        np->len = p->len + str.len();
        np->s = new char[ np->len + 1 ];
        if (np->s) {
            strcpy(np->s, p->s);
            strcpy(np->s + p->len, str.p->s);
        } else {
            delete np;
            np = 0;
        }
    }
    if ( --(p->refcount) == 0 ) {
        delete [] p->s;
        delete p;
    }
    p = np;
    PLT_CHECK_INVARIANT();
    return *this;
}
    

//////////////////////////////////////////////////////////////////////

PltString::PltString(const char *p1, const char *p2)
{
    PLT_PRECONDITION(p1 && p2);
    size_t len = strlen(p1) + strlen(p2);
    char * s = new char[len+1];
    if (s) {
        strcpy(s,p1);
        strcat(s,p2);
        p = new srep;
        if (p) {
            p->len = len;
            p->s = s;
        }
    } else {
        p = 0;
    }
    PLT_CHECK_INVARIANT(); // p->len is temporarily != strlen(p->s)
}

/////////////////////////////////////////////////////////////////////////////

void
PltString::toUpper()
{
    cloneIfNeeded();

    if(p) {
        for(size_t i = 0; i < len(); i++) {
            (p->s)[i] = (char)toupper( (p->s)[i] );
        }
    }
}

/////////////////////////////////////////////////////////////////////////////

PltString
PltString::toUpper() const
{
    if(p) {
        char *upper = new char[p->len+1];
        if(upper) {
            for(size_t i = 0; i < p->len; i++) {
                upper[i] = (char)toupper( (p->s)[i] );
            }
            upper[p->len] = 0;
            return PltString(p->len, upper);
        }
    }

    // memory allocation failed
    //
    return PltString();
}

//////////////////////////////////////////////////////////////////////

void
PltString::toLower()
{
    cloneIfNeeded();

    if(p) {
        for(size_t i = 0; i < len(); i++) {
            (p->s)[i] = (char)tolower( (p->s)[i] );
        }
    }
}

/////////////////////////////////////////////////////////////////////////////

PltString
PltString::toLower() const
{
    if(p) {
        char *lower = new char[p->len+1];
        if(lower) {
            for(size_t i = 0; i < p->len; i++) {
                lower[i] = (char)tolower( (p->s)[i] );
            }
            lower[p->len] = 0;
            return PltString(p->len, lower);
        }
    }

    // memory allocation failed
    //
    return PltString();
}

//////////////////////////////////////////////////////////////////////

void
PltString::cloneIfNeeded()
{
    PLT_PRECONDITION( ok() );
    if ((p->refcount) > 1) { 
        // clone to maintain value semantics
        --(p->refcount);
        srep *np = new srep;
        if (np) {
            np->len = p->len;
            np->s = new char[ p->len + 1 ];
            if (np->s) {
                memcpy(np->s, p->s, np->len + 1);
            } else {
                delete np;
                np = 0;
            }
        }
        p = np;
    }
    PLT_CHECK_INVARIANT();
}


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

unsigned long
PltString::hash() const
{
    unsigned long res = 0;
    unsigned long g;
    
    if(p) {
        const char *s = p->s;

        while( *s ) {
            res = (res << 4) + *(s++);
            g = res & 0xf0000000;
            if( g ) {
                res = (res ^ (g >> 24)) ^ g;
            }
        }
    }

    return res;
}

//////////////////////////////////////////////////////////////////////
// EOF plt/string.cpp
//////////////////////////////////////////////////////////////////////
