/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/plt/src/time.cpp,v 1.5 1997-04-01 12:57:23 martin Exp $ */
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

#if 0 // was: PLT_SYSTEM_NT
#include <time.h>
#include <windows.h>
#endif

//////////////////////////////////////////////////////////////////////

#if PLT_SYSTEM_OS2 || PLT_SYSTEM_NT

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

#if 0 // was: PLT_SYSTEM_NT

PltTime
PltTime::now(long secs_arg, long usecs_arg)
{
    SYSTEMTIME st;
    GetSystemTime(&st);
    FILETIME ft;
    if (SystemTimeToFileTime(&st,&ft)) {  
        // not very efficient
        static const DWORD loEpoch    = 3577643008;
        static const DWORD loEpochNeg =  717324288; // 2^32 - loEpoch
        static const DWORD hiEpoch =   27111902;
        DWORD h = ft.dwHighDateTime;
        DWORD l = ft.dwLowDateTime;

        // let M==2^32, l1+M*h1<l0+M*h0
        // calc. x = l1+M*h1 - l0+M*h0:
        if (l >= loEpoch) {
            l -= loEpoch;   // l1>=l0: x = (l1-l0)+ M(h1-h0)
        } else {
            --h;         // l1<l0: x = (l1+(M-l0))+M(h1-1-h0)
            l += loEpochNeg;
        }
        h -= hiEpoch;
        // (h*M+l) * 100ns(==0.1us) since epoch
        double secs  = h*429.4967296+l/10000000.0;
        PltTime res(secs+secs_arg, st.wMilliseconds*1000 + usecs_arg);
        res.normalize();
        return res;
        }
    else {
        return PltTime(time(0)+secs_arg,usecs_arg);
    }
}
#endif

//////////////////////////////////////////////////////////////////////
#if PLT_SYSTEM_LINUX || PLT_SYSTEM_HPUX || PLT_SYSTEM_SOLARIS

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
/* EOF plt/time.cpp */
