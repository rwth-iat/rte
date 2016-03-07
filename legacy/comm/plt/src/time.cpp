/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/plt/src/time.cpp,v 1.16 2007-04-25 12:57:21 martin Exp $ */
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


#include "plt/time.h"

#if PLT_SYSTEM_OS2 || PLT_SYSTEM_NT
#include <sys/timeb.h>
#endif

#if !PLT_SYSTEM_NT
#include <unistd.h>
#endif

#if PLT_SYSTEM_NT
#include <windows.h>
#endif

//////////////////////////////////////////////////////////////////////

#if PLT_DEBUG_INVARIANTS
bool
PltTime::invariant() const
{
    bool ok = true;

// We allow negative times, so the following check is disabled.

#if  0 && PLT_TIME_TV_SEC_SIGNED
    ok = ok && tv_sec >= 0;
    if (!ok) { 
        PLT_DMSG_ADD("tv_sec=" << tv_sec);
        PLT_DMSG_END;
    }
#endif

#if PLT_TIME_TV_USEC_SIGNED
    ok = ok && 0 <= tv_usec;
    if (!ok) {
        PLT_DMSG_ADD("tv_usec=" << tv_usec);
        PLT_DMSG_END;
    }
#endif

    ok = ok && tv_usec < 1000000;
    if (!ok) {
        PLT_DMSG_ADD("tv_usec=" << tv_usec);
        PLT_DMSG_END;
    }
    return ok;
}
#endif

//////////////////////////////////////////////////////////////////////

#if PLT_SYSTEM_OS2 || PLT_SYSTEM_NT || PLT_SYSTEM_OPENVMS

PltTime
PltTime::now(long secs, long usecs)
{
    struct timeb t;
    ftime(&t);

    PltTime res(t.time+secs, t.millitm*1000+usecs);
    res.normalize();
    return res;
}

#endif

//////////////////////////////////////////////////////////////////////
#if PLT_SYSTEM_LINUX || PLT_SYSTEM_HPUX || PLT_SYSTEM_SOLARIS || PLT_SYSTEM_IRIX

PltTime
PltTime::now(long secs, long usecs)
{
    PltTime res;
    struct timezone tz;
    gettimeofday(&res, &tz);
    res.tv_usec += usecs;
    res.tv_sec += secs;
    res.normalize();
    return res;
}
#endif

//////////////////////////////////////////////////////////////////////
// Provide a system-independent sleep with resolutions below one
// second.
//
void
PltTime::sleep() const
{
    PltTime now(PltTime::now());
    PltTime then(now + *this);
 
    // avoid falling to early out of this method because of
    // interrupted system calls

    while (now < then) {
        PltTime delay = then - now;
#if !PLT_SYSTEM_NT
        select(0,
               0,0,0,
               (struct timeval*) &delay);
#else
    	Sleep(delay.tv_sec * 1000 + delay.tv_usec / 1000);
#endif
        now = PltTime::now();
    }
}

//////////////////////////////////////////////////////////////////////
/* EOF plt/time.cpp */
