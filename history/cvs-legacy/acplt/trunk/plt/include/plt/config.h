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

#ifdef __OS2__
#define PLT_SYSTEM_OS2 1
#endif

#ifdef __NT__
#define PLT_SYSTEM_NT 1
#define WIN32
#define i386 1
#define mc68000 0
#define sparc 0
#define vax 0
#endif

#ifndef PLT_SYSTEM_HPUX
#define PLT_SYSTEM_HPUX 0
#endif

#ifndef PLT_SYSTEM_NT
#define PLT_SYSTEM_NT 0
#endif

#ifndef PLT_SYSTEM_OS2
#define PLT_SYSTEM_OS2 0
#endif

#if PLT_SYSTEM_LINUX + PLT_SYSTEM_HPUX + PLT_SYSTEM_SOLARIS + PLT_SYSTEM_NT + PLT_SYSTEM_OS2 != 1
#error Must define exactly one system!
#endif

#include <stddef.h>

#if PLT_SYSTEM_LINUX || PLT_SYSTEM_HPUX || PLT_SYSTEM_SOLARIS
#ifndef PLT_USE_SYSLOG
#define PLT_USE_SYSLOG 1
#endif
#endif

#if PLT_SYSTEM_NT
#ifndef WIN32
#define WIN32 1
#endif
#endif

#ifndef PLT_USE_SYSLOG
#define PLT_USE_SYSLOG 0
#endif

#ifndef PLT_USE_CERRLOG
#define PLT_USE_CERRLOG 1
#endif

#ifndef PLT_SIZE_T_MAX
#include <sys/types.h>
#define PLT_SIZE_T_MAX ((size_t)-1)
#endif

#ifdef __GNUC__
#define PLT_COMPILER_GCC __GNUC__
#else
#define PLT_COMPILER_GCC 0
#endif

#ifdef _MSC_VER
#define PLT_COMPILER_MSVC _MSC_VER
#else
#define PLT_COMPILER_MSVC 0
#endif

#ifdef __WATCOMC__
#define PLT_COMPILER_WATCOM __WATCOMC__
#else
#define PLT_COMPILER_WATCOM 0
#endif

#ifdef __BORLANDC__
#define PLT_COMPILER_BORLAND __BORLANDC__
#else
#define PLT_COMPILER_BORLAND 0
#endif

#if PLT_COMPILER_GCC
#define PLT_SIMULATE_BOOL 0
#define PLT_SIMULATE_RTTI 1
#define PLT_INSTANTIATE_TEMPLATES 1
#define PLT_AVOID_DELETE_BUG 0
#endif

#if PLT_COMPILER_MSVC
#define PLT_SIMULATE_BOOL 1
#pragma warning (disable : 4237 )  /* disable warning about defining bool... */
#pragma warning (disable : 4284 )  /* disable warning about operator ->      */
                                   /* for non-struct data types              */
#ifdef _CPPRTTI
#define PLT_SIMULATE_RTTI 0
#else
#define PLT_SIMULATE_RTTI 1
#endif

#define PLT_ARRAY_NEW_OVERLOADABLE 0
#define PLT_RETTYPE_OVERLOADABLE 0
#define PLT_POSTFIX_OVERLOADABLE 0
#define PLT_INSTANTIATE_TEMPLATES 0

#endif /* PLT_COMPILER_MSVC */

#if PLT_COMPILER_BORLAND
#pragma warn -hid
#pragma warn -inl
#pragma warn -stv
#define PLT_SEE_ALL_TEMPLATES 1
#define PLT_INSTANTIATE_TEMPLATES 1
#endif


#if PLT_COMPILER_WATCOM
/* Don't warn about nested comments */
#pragma warning 15 9
#define PLT_SIMULATE_BOOL 1
#define PLT_SIMULATE_RTTI 1
#define PLT_INSTANTIATE_TEMPLATES 0
#define PLT_SEE_ALL_TEMPLATES 1
#define _declspec(x) __declspec(x)
#endif

#ifndef PLT_SIMULATE_BOOL
#define PLT_SIMULATE_BOOL 0			/* Is bool not defined by the system? */
#endif

#ifndef PLT_SIMULATE_RTTI           /* Do we have to provide RTTI? */
#define PLT_SIMULATE_RTTI 1
#endif

#ifndef PLT_INSTANTIATE_TEMPLATES    /* Explicit template instantiation? */
#define PLT_INSTANTIATE_TEMPLATES 0
#endif

#ifndef PLT_SEE_ALL_TEMPLATES
#define PLT_SEE_ALL_TEMPLATES 0
#endif

#ifndef PLT_AVOID_DELETE_BUG         /* If you can't call delete through */
#define PLT_AVOID_DELETE_BUG 0       /* a pointer to a base class object */
#endif                               /* define this as 1.                */


#ifndef PLT_ARRAY_NEW_OVERLOADABLE
#define PLT_ARRAY_NEW_OVERLOADABLE 1
#endif

#ifndef PLT_POSTFIX_OVERLOADABLE      /* Can operator ++ (int) be overloaded */
#define PLT_POSTFIX_OVERLOADABLE 1
#endif

#ifndef PLT_RETTYPE_OVERLOADABLE      /* Can a subclass redefine the return */
#define PLT_RETTYPE_OVERLOADABLE 1    /* type  to a covariant one?          */
#endif                                /* Every compiler should support this.*/

#if PLT_SIMULATE_BOOL
typedef int bool;
enum { false=0, true=1 };
#endif

#if PLT_RETTYPE_OVERLOADABLE
#define PLT_RETTYPE_CAST(typ,expr) (expr) 
#else
#define PLT_RETTYPE_CAST(typ,expr) (dynamic_cast< typ > (expr)) 
#endif

#endif /* PLT_CONFIG.H */

