/* -*-plt-c++-*- */
#ifndef PLT_CONFIG_INCLUDED
#define PLT_CONFIG_INCLUDED
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

#ifndef PLT_SYSTEM_HPUX
#define PLT_SYSTEM_HPUX 0
#endif

#ifndef PLT_SYSTEM_NT
#define PLT_SYSTEM_NT 0
#endif

/* End of configuration section */

#if PLT_SYSTEM_LINUX + PLT_SYSTEM_HPUX + PLT_SYSTEM_NT != 1
#error Must define exactly one system!
#endif

#if PLT_SYSTEM_LINUX
#define PLT_USE_SYSLOG 1
#endif

#if PLT_SYSTEM_HPUX
#define PLT_USE_SYSLOG 1
#endif

#if PLT_SYSTEM_NT
#define PLT_USE_SYSLOG 0
#endif

#ifndef PLT_USE_SYSLOG
#define PLT_USE_SYSLOG 0
#endif

#ifdef __GNUC__
#define PLT_SIMULATE_BOOL 0
#define PLT_SIMULATE_RTTI 1
#endif



#ifndef PLT_SIMULATE_BOOL
#define PLT_SIMULATE_BOOL 0			/* Is bool not defined by the system? */
#endif

#ifndef PLT_SIMULATE_RTTI           /* Do we have to provide RTTI? */
#define PLT_SIMULATE_RTTI 1
#endif


#if PLT_SIMULATE_BOOL
typedef int bool;
enum { false=0, true=1 };
#endif

#endif /* PLT_CONFIG.H */

