/* -*-plt-c++-*- */
/* 
 *  Copyright (c) 1996 PLT, RWTH-Aachen, Germany. See file copy_plt.txt!
 *  Author: Harald Albrecht <...>
 *
 */

#ifndef PLT_TIME_INCLUDED
#define PLT_TIME_INCLUDED

#include <plt/config.h>

#include <sys/time.h>

// helper typedef for the cast operator
typedef struct timeval *ptimeval;

class PltTime {
public:
    struct timeval time;

    //
    // Constructors
    //
    PltTime(long seconds = 0, long useconds = 0)
        { time.tv_sec = seconds; time.tv_usec = useconds; }
    PltTime(struct timeval &tv)
        { time.tv_sec = tv.tv_sec; time.tv_usec = tv.tv_usec; }

    //
    // Assignment operator
    //
    PltTime & operator = (struct timeval &tv)
        { 
            time.tv_sec = tv.tv_sec; time.tv_usec = tv.tv_usec; 
            return *this;
        }

    //
    // Operator overloading: +, -
    //
    friend PltTime operator + (const PltTime &t1, const PltTime &t2)
        {
            PltTime t(t1.time.tv_sec + t2.time.tv_sec,
                     t1.time.tv_usec + t2.time.tv_usec);

            if ( t.time.tv_usec >= 1000*1000 ) {
                t.time.tv_usec -= 1000*1000;
                ++t.time.tv_sec;
            } else {
                while ( t.time.tv_usec < 0 ) {
                    t.time.tv_usec += 1000*1000;
                    --t.time.tv_sec;
                }
            }
	    return t;
        }
    friend PltTime operator - (const PltTime &t1, const PltTime &t2)
        {
            PltTime t(t1.time.tv_sec - t2.time.tv_sec,
                     t1.time.tv_usec - t2.time.tv_usec);

            if ( t.time.tv_usec < 0 ) {
                t.time.tv_usec += 1000*1000;
                --t.time.tv_sec;
            } else if ( t.time.tv_usec >= 1000*1000 ) {
                t.time.tv_usec -= 1000*1000;
                ++t.time.tv_sec;
            }
	    return t;
        }

    //
    // Type-casting: cast to ordinary timeval structure.
    //
    operator ptimeval()
        { return &time; }

    //
    // Comparing operations.
    //
    friend int operator == (const PltTime &t1, const PltTime &t2)
        { return (t1.time.tv_sec == t2.time.tv_sec) && 
                 (t1.time.tv_usec == t2.time.tv_usec); }
    friend int operator != (const PltTime &t1, const PltTime &t2)
        { return (t1.time.tv_sec != t2.time.tv_sec) || 
                 (t1.time.tv_usec != t2.time.tv_usec); }
    friend int operator < (const PltTime &t1, const PltTime &t2)
        { return (t1.time.tv_sec < t2.time.tv_sec) ||
                 ((t1.time.tv_sec == t2.time.tv_sec) &&
                  (t1.time.tv_usec < t2.time.tv_usec)); }
    friend int operator > (const PltTime &t1, const PltTime &t2)
        { return (t1.time.tv_sec > t2.time.tv_sec) ||
                 ((t1.time.tv_sec == t2.time.tv_sec) &&
                  (t1.time.tv_usec > t2.time.tv_usec)); }
    friend int operator <= (const PltTime &t1, const PltTime &t2)
        { return (t1 < t2) || (t1 == t2); }
    friend int operator >= (const PltTime &t1, const PltTime &t2)
        { return (t1 > t2) || (t1 == t2); }

};


#endif

// End of time.hpp
