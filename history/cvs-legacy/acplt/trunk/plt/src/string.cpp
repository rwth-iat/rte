/* -*-plt-c++-*- */
/* 
 *  Copyright (c) 1996 PLT, RWTH-Aachen, Germany. See file copy_plt.txt!
 *  Author: Martin Kneissl <martin@plt.rwth-aachen.de>
 *
 */

#include <plt/debug.h>
#include <plt/string.hpp>
#include <string.h>


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
    PLT_PRECONDITION( ok() && i < p->len );
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


