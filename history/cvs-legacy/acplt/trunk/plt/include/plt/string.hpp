/* -*-plt-c++-*- */
/* 
 *  Copyright (c) 1996 PLT, RWTH-Aachen, Germany. See file copy_plt.txt!
 *  Author: Martin Kneissl <martin@plt.rwth-aachen.de>
 *
 */

#ifndef PLT_STRING_INCLUDED
#define PLT_STRING_INCLUDED

#include <plt/debug.h>
#include <plt/rtti.hpp>
#include <plt/hashkey.hpp>
#include <stdlib.h>


//////////////////////////////////////////////////////////////////////
// PltStrings minimize copying by reference counting. See 
// [Stroustrup, 2nd] for details. This implementation is modified 
// version
// of the one in the book (Sec.7.11)
//////////////////////////////////////////////////////////////////////

class PltString : public PltHashKey, PltDebuggable {
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

    // PltHashKey interface
    virtual unsigned long hash() const;
    virtual bool operator == (const PltHashKey &) const;

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
  

private:
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
    PLT_DECL_RTTI;
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

#endif
