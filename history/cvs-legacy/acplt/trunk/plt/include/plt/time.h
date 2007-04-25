/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/plt/include/plt/time.h,v 1.19 2007-04-25 12:57:21 martin Exp $ */
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
#ifndef PLT_TIME_INCLUDED
#define PLT_TIME_INCLUDED

#include "plt/debug.h"

#if PLT_SYSTEM_NT

// does not even have gettimeofday...
#if PLT_USE_WINSOCK2
#include <winsock2.h>
#else
#include <winsock.h>
#endif

#if PLT_COMPILER_MSVC
#pragma warning (disable : 4237 )  /* disable warning about defining bool... */
#endif

#elif PLT_SYSTEM_OS2
// no comment
struct timeval
{
    long tv_sec;
    long tv_usec;
};
#else
// Good.
#include <sys/time.h>
#endif

/////////////////////////////////////////////////////////////////////////////
// A PltTimeSpan object represents the length of a time interval. 
/////////////////////////////////////////////////////////////////////////////

class PltTimeSpan
{
public:
    PltTimeSpan(long sec = 0, long usec = 0);
    PltTimeSpan(const PltTimeSpan &ts);

    PltTimeSpan &operator += (const PltTimeSpan &ts);
    PltTimeSpan &operator -= (const PltTimeSpan &ts);

    void normalize();

#if PLT_DEBUG_INVARIANTS
    bool invariant() const;
#endif

    long tv_sec;
    long tv_usec;
};

//////////////////////////////////////////////////////////////////////
// a PltTime object is a timeval with some operations added
//////////////////////////////////////////////////////////////////////

struct PltTime : public timeval {
    // Constructors
    PltTime(long seconds = 0, long useconds = 0);
    PltTime(const struct timeval &tv);

#if PLT_DEBUG_INVARIANTS
    bool invariant() const;
#endif

    bool isZero () const;

    // Make representation canonical
    void normalize();

    // adding and substracting times
    PltTime & operator += (const PltTime &t);
    PltTime & operator -= (const PltTime &t);
    PltTime & operator += (const PltTimeSpan &ts);
    PltTime & operator -= (const PltTimeSpan &ts);

    void sleep() const; // delay for at least *this time

    static PltTime now(long seconds=0, long useconds = 0);
    static PltTime now(const PltTime &);
};

//
// Return type should be PltTimeSpan, but changing operators  
// would break existing code.
//
PltTime operator + (const PltTime &t1, const PltTime &t2);
PltTime operator - (const PltTime &t1, const PltTime &t2);
//
// Return type PltTime is ok.
// Errors due to underflow of unsigned tv_sec are ignored.
//
PltTime operator + (const PltTime &t, const PltTimeSpan &ts);
PltTime operator + (const PltTimeSpan &ts, const PltTime &t);
PltTime operator - (const PltTime &t, const PltTimeSpan &ts);
PltTime operator - (const PltTimeSpan &ts, const PltTime &t);

//
// Comparison operators.
//
inline bool operator == (const PltTime &t1, const PltTime &t2);
inline bool operator != (const PltTime &t1, const PltTime &t2);
inline bool operator < (const PltTime &t1, const PltTime &t2);
inline bool operator > (const PltTime &t1, const PltTime &t2);
inline bool operator <= (const PltTime &t1, const PltTime &t2);
inline bool operator >= (const PltTime &t1, const PltTime &t2);

inline bool operator == (const PltTimeSpan &ts1, const PltTimeSpan &ts2);
inline bool operator != (const PltTimeSpan &ts1, const PltTimeSpan &ts2);
inline bool operator < (const PltTimeSpan &ts1, const PltTimeSpan &ts2);
inline bool operator > (const PltTimeSpan &ts1, const PltTimeSpan &ts2);
inline bool operator <= (const PltTimeSpan &ts1, const PltTimeSpan &ts2);
inline bool operator >= (const PltTimeSpan &t1, const PltTimeSpan &ts2);

//////////////////////////////////////////////////////////////////////
// INLINE IMPLEMENTATION
//////////////////////////////////////////////////////////////////////

#if PLT_SYSTEM_HPUX
#define PLT_TIME_TV_SEC_SIGNED 0
#endif

#if PLT_SYSTEM_HPUX
#define PLT_TIME_TV_USEC_SIGNED 1
#endif

#ifndef PLT_TIME_TV_SEC_SIGNED
#define PLT_TIME_TV_SEC_SIGNED 1
#endif

#ifndef PLT_TIME_TV_USEC_SIGNED
#define PLT_TIME_TV_USEC_SIGNED 1
#endif

/////////////////////////////////////////////////////////////////////////////
// class PltTimeSpan
/////////////////////////////////////////////////////////////////////////////

inline
void
PltTimeSpan::normalize()
{
    // Forces tv_usec to be in range from 0 to 999999
    //
    while(tv_usec >= 1000000) {
        tv_usec -= 1000000;
        tv_sec += 1;
    }

    while(tv_usec < 0) {
        tv_usec += 1000000;
        tv_sec -= 1;
    }

    PLT_CHECK_INVARIANT();
}

/////////////////////////////////////////////////////////////////////////////

inline
PltTimeSpan::PltTimeSpan(long sec, long usec)
    : tv_sec(sec), tv_usec(usec)
{
    normalize();
}

/////////////////////////////////////////////////////////////////////////////

inline 
PltTimeSpan::PltTimeSpan(const PltTimeSpan &ts)
    : tv_sec(ts.tv_sec),
      tv_usec(ts.tv_usec)
{
    PLT_CHECK_INVARIANT();
}

/////////////////////////////////////////////////////////////////////////////

inline
PltTimeSpan &
PltTimeSpan::operator += (const PltTimeSpan &ts)
{
    tv_sec += ts.tv_sec;
    tv_usec += ts.tv_usec;
    normalize();
    return *this;
}

/////////////////////////////////////////////////////////////////////////////

inline
PltTimeSpan &
PltTimeSpan::operator -= (const PltTimeSpan &ts)
{
    tv_sec -= ts.tv_sec;
    tv_usec -= ts.tv_usec;
    normalize();
    return *this;
}

/////////////////////////////////////////////////////////////////////////////

#if PLT_DEBUG_INVARIANTS

inline
bool
PltTimeSpan::invariant() const
{
    return 0 <= tv_usec &&
        tv_usec < 1000000;
}

#endif

//////////////////////////////////////////////////////////////////////
// class PltTime
/////////////////////////////////////////////////////////////////////////////

inline bool
PltTime::isZero() const
{
    return (tv_usec == 0 && tv_sec == 0) ;
}

//////////////////////////////////////////////////////////////////////

inline void
PltTime::normalize() 
{

#if PLT_TIME_TV_USEC_SIGNED
    while (tv_usec < 0) {
        tv_usec += 1000000;
        tv_sec  -= 1;
    }
#endif

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
PltTime::PltTime(const struct timeval &tv)
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

/////////////////////////////////////////////////////////////////////////////

inline
PltTime &
PltTime::operator += (const PltTimeSpan &ts)
{
    tv_usec += ts.tv_usec;
    tv_sec += ts.tv_sec;
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

/////////////////////////////////////////////////////////////////////////////

inline PltTime &
PltTime::operator -= (const PltTimeSpan &ts)
{
    if (tv_usec < ts.tv_usec) {
        // this->tv_usec borrows one second
        tv_usec += 1000000;
        --tv_sec;
    }
    tv_usec -= ts.tv_usec;
    tv_sec  -= ts.tv_sec;
    normalize();
    PLT_CHECK_INVARIANT();
    return *this;
}    

/////////////////////////////////////////////////////////////////////////////
// Comparison operators for class PltTimeSpan
/////////////////////////////////////////////////////////////////////////////

inline
bool 
operator == (const PltTimeSpan &ts1, const PltTimeSpan &ts2)
{
    return ts1.tv_sec == ts2.tv_sec
        && ts1.tv_usec == ts2.tv_usec;
}

/////////////////////////////////////////////////////////////////////////////

inline
bool 
operator != (const PltTimeSpan &ts1, const PltTimeSpan &ts2)
{
    return !(ts1 == ts2);
}

/////////////////////////////////////////////////////////////////////////////

inline
bool 
operator < (const PltTimeSpan &ts1, const PltTimeSpan &ts2)
{
    return ts1.tv_sec < ts2.tv_sec
        || (ts1.tv_sec == ts2.tv_sec && ts1.tv_usec < ts2.tv_usec);
}

/////////////////////////////////////////////////////////////////////////////

inline 
bool 
operator > (const PltTimeSpan &ts1, const PltTimeSpan &ts2)
{
    return ts1.tv_sec > ts2.tv_sec
        || (ts1.tv_sec == ts2.tv_sec && ts1.tv_usec > ts2.tv_usec);
}

/////////////////////////////////////////////////////////////////////////////

inline
bool 
operator <= (const PltTimeSpan &ts1, const PltTimeSpan &ts2)
{
    return ts1.tv_sec < ts2.tv_sec
        || (ts1.tv_sec == ts2.tv_sec && ts1.tv_usec <= ts2.tv_usec);
}

/////////////////////////////////////////////////////////////////////////////

inline
bool 
operator >= (const PltTimeSpan &ts1, const PltTimeSpan &ts2)
{
    return ts1.tv_sec > ts2.tv_sec
        || (ts1.tv_sec == ts2.tv_sec && ts1.tv_usec >= ts2.tv_usec);
}

/////////////////////////////////////////////////////////////////////////////
// Comparison operators for class PltTime
/////////////////////////////////////////////////////////////////////////////

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
          && t1.tv_usec <= t2.tv_usec);
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

/////////////////////////////////////////////////////////////////////////////

inline
PltTime 
operator + (const PltTime &t, const PltTimeSpan &ts)
{
    
    return PltTime(t.tv_sec + ts.tv_sec,
                   t.tv_usec + ts.tv_usec);
    
}

/////////////////////////////////////////////////////////////////////////////

inline
PltTime 
operator + (const PltTimeSpan &ts, const PltTime &t)
{
    return PltTime(t.tv_sec + ts.tv_sec,
                   t.tv_usec + ts.tv_usec);
}

/////////////////////////////////////////////////////////////////////////////

inline
PltTime 
operator - (const PltTime &t, const PltTimeSpan &ts)
{
    return PltTime(t.tv_sec - ts.tv_sec,
                   t.tv_usec - ts.tv_usec);
}

/////////////////////////////////////////////////////////////////////////////

inline
PltTime 
operator - (const PltTimeSpan &ts, const PltTime &t)
{
    return PltTime(t.tv_sec - ts.tv_sec,
                   t.tv_usec - ts.tv_usec);
}

/////////////////////////////////////////////////////////////////////////////

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

inline PltTime
PltTime::now(const PltTime & t)
{
    return PltTime::now(t.tv_sec, t.tv_usec);
}

/////////////////////////////////////////////////////////////////////////////


#endif // PLT_TIME_INCLUDED

