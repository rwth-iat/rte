/* -*-plt-c++-*- */
// File: plt/string.cpp

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
/* Author: Martin Kneissl <martin@plt.rwth-aachen.de> */


//////////////////////////////////////////////////////////////////////

#include "plt/string.h"
#include <string.h>

//////////////////////////////////////////////////////////////////////

PLT_IMPL_RTTI1(PltString, PltKey);

//////////////////////////////////////////////////////////////////////

#if PLT_DEBUG_INVARIANTS
bool
PltString::invariant() const
{
    return (!p 
            || 
            (p->refcount > 0 && p->s && strlen(p->s) == p->len));
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
        
PltString::PltString(const char *s)
{
    PLT_PRECONDITION(s);
	p = new srep;
    if (p) {
        p->len = strlen(s);
        p->s = new char[p->len + 1];
        if (p->s) {
            memcpy(p->s, s, p->len + 1);
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
PltString::operator = (const char *s) 
{
    PLT_PRECONDITION( ok() && s);
    if ( --(p->refcount) == 0 ) {
        delete [] p->s;
    } else {
        p = new srep;
    }
    if (p) {
        p->len = strlen(s);
        p->s = new char[ p->len ];
        if (! p->s) {
            delete p;
        } else {
            memcpy(p->s, s, p->len+1);
        }
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
    static char dummy = 'A';
    return ok() ? (p->s)[i] : dummy;
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
        delete p->s;
        delete p;
    }
    p = np;
    PLT_CHECK_INVARIANT();
    return *this;
}
    

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void
PltString::cloneIfNeeded()
{
    PLT_PRECONDITION( ok() );
    if (p->refcount > 1) {
        // clone to maintain value semantics
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
        p->refcount --;
        p = np;
    }
    PLT_CHECK_INVARIANT();
}


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

unsigned long
PltString::hash() const
{
    // TODO: Better (and faster) hash function
    unsigned long res = 0;
    if ( ok() ) {
        for (size_t i = 0; i < len(); ++i) {
            res += (*this)[i];
        }
    }
    return res;
}

//////////////////////////////////////////////////////////////////////

bool
PltString::operator == ( const PltKey & key ) const
{
    PltString *p = PLT_DYNAMIC_PCAST(PltString, &key);
    if (p) {
        return *this == *p;
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
// EOF plt/string.cpp
//////////////////////////////////////////////////////////////////////
