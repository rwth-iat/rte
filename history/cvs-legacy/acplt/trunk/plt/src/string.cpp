/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/plt/src/string.cpp,v 1.21 1999-01-08 13:12:32 harald Exp $ */
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
/* Author: Martin Kneissl <martin@plt.rwth-aachen.de> */


//////////////////////////////////////////////////////////////////////

#include "plt/string.h"
#include <ctype.h>

#if PLT_SYSTEM_NT && !PLT_SYSTEM_CYGWIN
// That's ridiculous:
#include <strstrea.h>
#else
#include <strstream.h>
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

static char conv_buffer[32];

PltString
PltString::fromInt(int i)
{
    strstream s(conv_buffer, sizeof conv_buffer, ios::out);
    s << i << ends;
#if PLT_COMPILER_GCC
    s.freeze();
#endif
    // TODO
    return PltString(s.str());
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
            (p->s)[i] = toupper( (p->s)[i] );
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
                upper[i] = toupper( (p->s)[i] );
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
            (p->s)[i] = tolower( (p->s)[i] );
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
                lower[i] = tolower( (p->s)[i] );
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
            if( (g = res & 0xf0000000) ) {
                res = (res ^ (g >> 24)) ^ g;
            }
        }
    }

    return res;
}

//////////////////////////////////////////////////////////////////////
// EOF plt/string.cpp
//////////////////////////////////////////////////////////////////////
