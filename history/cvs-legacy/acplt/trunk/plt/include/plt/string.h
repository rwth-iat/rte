/* -*-plt-c++-*- */
#ifndef PLT_STRING_INCLUDED
#define PLT_STRING_INCLUDED
/* $Header: /home/david/cvs/acplt/plt/include/plt/string.h,v 1.12 1998-07-30 10:22:42 markusj Exp $ */
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
#include "plt/alloc.h"

#include <stdlib.h>
#include <string.h>

//////////////////////////////////////////////////////////////////////
// PltStrings reduce copying by reference counting. See 
// [Stroustrup, 2nd] for details. This implementation is a modified 
// version
// of the one in the book (Sec.7.11)
//////////////////////////////////////////////////////////////////////

class PltString 
{
public:
    PltString(const char *, size_t len=PLT_SIZE_T_MAX); 
                           // at most first len characters
    PltString();
    PltString(const PltString &);
    PltString(const char *, const char *); // concatenates
//    PltString(const PltString &, size_t first, size_t len);
    virtual ~PltString();
    PltString & operator = (const char *);
    PltString & operator = (const PltString &);
    static PltString fromInt(int );

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

    PltString substr(size_t first, size_t len=PLT_SIZE_T_MAX) const;
    // modifiers
    char & operator[] (size_t);
    PltString & operator += ( const PltString & );

    void toUpper();
    PltString toUpper() const;

    // friends (accessors)
    friend bool operator == (const PltString &, const char *);
    friend bool operator == (const PltString &, const PltString &);
    friend bool operator != (const PltString &, const char *);
    friend bool operator != (const PltString &, const PltString &);
    friend bool operator <  (const PltString &, const PltString &);
    friend bool operator <= (const PltString &, const PltString &);
    friend bool operator >  (const PltString &, const PltString &);
    friend bool operator >= (const PltString &, const PltString &);


protected:
    // takes ownership of p, 
    // p has to have been allocated by new
    //
    PltString(size_t sz, char *p);


public: // read: private ;-)
    class srep {
    public:
        char * s;
        size_t len;
        int refcount;
        srep() : s(0), len(0), refcount(1) { }

        void * operator new(size_t);
        void operator delete(void * ptr);
    };
    srep *p;
    static PltAllocator<srep> _srep_allocator;

protected:
    // helper modifiers
    void cloneIfNeeded();


};

PltString operator + (const PltString &s1, const PltString &s2);
PltString operator + (const char *s1, const PltString &s2);
PltString operator + (const PltString &s1, const char *s2);

//////////////////////////////////////////////////////////////////////
// INLINE IMPLEMENTATION
//////////////////////////////////////////////////////////////////////

inline void * 
PltString::srep::operator new(size_t 
#if PLT_DEBUG
                              sz
#endif
                              )
{
    PLT_ASSERT(sz==sizeof (srep));
    return _srep_allocator.alloc();
}

//////////////////////////////////////////////////////////////////////

inline void 
PltString::srep::operator delete(void * ptr)
{
    _srep_allocator.free(ptr);
}

//////////////////////////////////////////////////////////////////////

inline bool
PltString::ok() const 
{
    return p != 0;
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

//////////////////////////////////////////////////////////////////////

inline bool 
operator <  (const PltString &x, const PltString &y)
{
    PLT_PRECONDITION(x.ok() && y.ok());
    return strcmp(x.p->s, y.p->s) < 0;
}

//////////////////////////////////////////////////////////////////////

inline bool 
operator <= (const PltString &x, const PltString &y)
{
    PLT_PRECONDITION(x.ok() && y.ok());
    return strcmp(x.p->s, y.p->s) <= 0;
}

//////////////////////////////////////////////////////////////////////

inline bool 
operator >  (const PltString &x, const PltString &y)
{
    PLT_PRECONDITION(x.ok() && y.ok());
    return strcmp(x.p->s, y.p->s) > 0;
}

//////////////////////////////////////////////////////////////////////

inline bool 
operator >= (const PltString &x, const PltString &y)
{
    PLT_PRECONDITION(x.ok() && y.ok());
    return strcmp(x.p->s, y.p->s) >= 0;
}

//////////////////////////////////////////////////////////////////////

inline PltString 
operator + (const PltString &s1, const PltString &s2) 
{
    return PltString(s1,s2);
}

//////////////////////////////////////////////////////////////////////

inline PltString 
operator + (const char *s1, const PltString &s2) 
{
    return PltString(s1,s2);
}

//////////////////////////////////////////////////////////////////////

inline PltString 
operator + (const PltString &s1, const char *s2) 
{
    return PltString(s1,s2);
}


//////////////////////////////////////////////////////////////////////
#endif // PLT_STRING_INCLUDED








