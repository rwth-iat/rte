/* -*-plt-c++-*- */
#ifndef PLT_STRING_INCLUDED
#define PLT_STRING_INCLUDED
/* $Header: /home/david/cvs/acplt/plt/include/plt/string.h,v 1.4 1997-03-12 16:19:22 martin Exp $ */
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

#include "plt/debug.h"

#include <stdlib.h>

//////////////////////////////////////////////////////////////////////
// PltStrings reduce copying by reference counting. See 
// [Stroustrup, 2nd] for details. This implementation is a modified 
// version
// of the one in the book (Sec.7.11)
//////////////////////////////////////////////////////////////////////

class PltString 
{
public:
    PltString(const char *);
    PltString();
    PltString(const PltString &);
    virtual ~PltString();
    PltString & operator = (const char *);
    PltString & operator = (const PltString &);

#if PLT_DEBUG_INVARIANTS
    virtual bool invariant() const;
#endif

    // key interface
    unsigned long hash() const;
    // bool operator == (const PltString &) const; // see below

    // accessors
    bool ok() const;
    size_t len() const;
    const char & operator[] (size_t) const;
    operator const char *() const;
    
    // modifiers
    char & operator[] (size_t);
    PltString & operator += ( const PltString & );

    // friends (accessors)
    friend bool operator == (const PltString &, const char *);
    friend bool operator == (const PltString &, const PltString &);
    friend bool operator != (const PltString &, const char *);
    friend bool operator != (const PltString &, const PltString &);

protected:
    PltString(size_t sz, char *p);

    class srep {
    public:
        char * s;
        size_t len;
        int refcount;
        srep() : s(0), len(0), refcount(1) { }
    };
    srep *p;

    // helper modifiers
    void cloneIfNeeded();
};

//////////////////////////////////////////////////////////////////////
// INLINE IMPLEMENTATION
//////////////////////////////////////////////////////////////////////

inline bool
PltString::ok() const 
{
    return p;
}

//////////////////////////////////////////////////////////////////////

inline size_t
PltString::len() const
{
    PLT_PRECONDITION( ok() );
    return p->len;
}

//////////////////////////////////////////////////////////////////////

inline 
PltString::operator const char * () const
{
    return ok() ? p->s : 0;
}

//////////////////////////////////////////////////////////////////////

inline bool
operator == (const PltString & s1 , const PltString & s2)
{
    PLT_PRECONDITION( s1.ok() && s2.ok() );
    return strcmp(s1.p->s, s2.p->s) == 0;
}

//////////////////////////////////////////////////////////////////////

inline bool
operator == (const PltString & str , const char *pc)
{
    PLT_PRECONDITION( str.ok() && pc );
    return strcmp(str.p->s, pc) == 0;
}

//////////////////////////////////////////////////////////////////////

inline bool
operator != (const PltString & s1 , const PltString & s2)
{
    PLT_PRECONDITION( s1.ok() && s2.ok() );
    return ! (s1 == s2);
}

//////////////////////////////////////////////////////////////////////

inline bool
operator != (const PltString & str , const char *pc)
{
    PLT_PRECONDITION( str.ok() && pc );
    return ! (str == pc);
}

#endif // PLT_STRING_INCLUDED
