/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/plt/src/time.cpp,v 1.10 1997-09-13 08:19:49 martin Exp $ */
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


#include "plt/time.h"

#if PLT_SYSTEM_OS2 || PLT_SYSTEM_NT
#include <sys/timeb.h>
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

void
PltTime::sleep() const
{
    PltTime now(PltTime::now());
    PltTime then(now + *this);
 
    // avoid falling to early out of this method because of
    // interrupted system calls

    while (now < then) {
        PltTime delay = then - now;
        select(0,
               0,0,0,
               (struct timeval*) &delay);
        now = PltTime::now();
    }
}

//////////////////////////////////////////////////////////////////////
/* EOF plt/time.cpp */
