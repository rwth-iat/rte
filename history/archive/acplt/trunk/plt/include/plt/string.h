/* -*-plt-c++-*- */
#ifndef PLT_STRING_INCLUDED
#define PLT_STRING_INCLUDED
/* $Header: /home/david/cvs/acplt/plt/include/plt/string.h,v 1.19 2007-04-25 12:57:21 martin Exp $ */
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
    // Constructs a new string which takes ownership of p
    static PltString takeOwnership(char *p, size_t len);


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
    void toLower();
    PltString toLower() const;

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

inline
PltString
PltString::takeOwnership(char *s, size_t len) 
{
    return PltString(len, s);
}

/////////////////////////////////////////////////////////////////////////////

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








