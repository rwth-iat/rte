/* -*-plt-c++-*- */
/* 
 *  Copyright (c) 1996 PLT, RWTH-Aachen, Germany. See file copy_plt.txt!
 *  Authors: Harald Albrecht, Martin Kneissl.
 *
 */

#ifndef PLT_TIME_INCLUDED
#define PLT_TIME_INCLUDED

#include <plt/debug.h>

#include <sys/time.h>


//////////////////////////////////////////////////////////////////////

struct PltTime : public timeval {
    // Constructors
    PltTime(long seconds = 0, long useconds = 0);
    PltTime(struct timeval &tv);

#if PLT_DEBUG_INVARIANTS
    bool invariant() const;
#endif

    // Make representation canonical
    void normalize();

    // adding and substracting times
    PltTime & operator += (const PltTime &t);
    PltTime & operator -= (const PltTime &t);
};

PltTime operator + (const PltTime &t1, const PltTime &t2);
PltTime operator - (const PltTime &t1, const PltTime &t2);

//
// Comparison operators.
//

bool operator == (const PltTime &t1, const PltTime &t2);
bool operator != (const PltTime &t1, const PltTime &t2);
bool operator < (const PltTime &t1, const PltTime &t2);
bool operator > (const PltTime &t1, const PltTime &t2);
bool operator <= (const PltTime &t1, const PltTime &t2);
bool operator >= (const PltTime &t1, const PltTime &t2);


//////////////////////////////////////////////////////////////////////
// INLINE IMPLEMENTATION
//////////////////////////////////////////////////////////////////////

#if PLT_DEBUG_INVARIANTS
inline bool
PltTime::invariant() const
{
    return 
        tv_sec >= 0 
        && 0 <= tv_usec && tv_usec < 1000000;
}
#endif

//////////////////////////////////////////////////////////////////////

inline bool
operator == (const PltTime &t1, const PltTime &t2)
{
    return 
            t1.tv_usec == t2.tv_usec
        &&  t1.tv_sec  == t2.tv_sec;
}

//////////////////////////////////////////////////////////////////////

inline bool
operator <= (const PltTime &t1, const PltTime &t2)
{
    return 
            t1.tv_sec  <  t2.tv_sec
        || ( t1.tv_sec  == t2.tv_sec
          && t2.tv_usec <= t2.tv_usec);
}

//////////////////////////////////////////////////////////////////////

inline bool
operator != (const PltTime &t1, const PltTime &t2)
{
    return !(t1==t2);
}

//////////////////////////////////////////////////////////////////////

inline bool
operator >= (const PltTime &t1, const PltTime &t2)
{
    return t2 <= t1;
}

//////////////////////////////////////////////////////////////////////

inline bool
operator > (const PltTime &t1, const PltTime &t2)
{
    return !(t1 <= t2);
}

//////////////////////////////////////////////////////////////////////

inline bool
operator < (const PltTime &t1, const PltTime &t2)
{
    return !(t2 <= t1);
}

//////////////////////////////////////////////////////////////////////

inline void
PltTime::normalize() 
{
    while (tv_usec < 0) {
        tv_usec += 1000000;
        tv_sec  -= 1;
    }
    while (tv_usec >= 1000000) {
        tv_usec -= 1000000;
        tv_sec  += 1;
    }
    PLT_CHECK_INVARIANT();
}


//////////////////////////////////////////////////////////////////////

inline
PltTime::PltTime(long seconds, long useconds)
{ 
    tv_sec = seconds;
    tv_usec = useconds;
    normalize();
    PLT_CHECK_INVARIANT();
}

//////////////////////////////////////////////////////////////////////

inline
PltTime::PltTime(struct timeval &tv)
{
    tv_sec = tv.tv_sec;
    tv_usec = tv.tv_usec; 
    normalize();
    PLT_CHECK_INVARIANT();
}

//////////////////////////////////////////////////////////////////////

inline PltTime &
PltTime::operator += (const PltTime & t)
{
    tv_usec += t.tv_usec;
    tv_sec  += t.tv_sec;
    normalize();
    PLT_CHECK_INVARIANT();
    return *this;
}

//////////////////////////////////////////////////////////////////////

inline PltTime &
PltTime::operator -= (const PltTime & t)
{
    PLT_PRECONDITION(*this >= t);
    if (tv_usec < t.tv_usec) {
        // this->tv_usec borrows one second
        tv_usec += 1000000;
        --tv_sec;
    }
    tv_usec -= t.tv_usec;
    tv_sec  -= t.tv_sec;
    normalize();
    PLT_CHECK_INVARIANT();
    return *this;
}    

//////////////////////////////////////////////////////////////////////

inline PltTime
operator + (const PltTime & t1, const PltTime & t2) 
{
    PltTime t(t1);
    t += t2;
    return t;
}

//////////////////////////////////////////////////////////////////////

inline PltTime
operator - (const PltTime & t1, const PltTime & t2) 
{
    PltTime t(t1);
    t -= t2;
    return t;
}

//////////////////////////////////////////////////////////////////////

#endif

// End of time.hpp

