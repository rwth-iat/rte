/* -*-plt-c++-*- */
#ifndef PLT_CONFIG_INCLUDED
#define PLT_CONFIG_INCLUDED
/* $Header: /home/david/cvs/acplt/plt/include/plt/config.h,v 1.22 1999-04-22 15:29:11 harald Exp $ *//*
 * Copyright (c) 1996, 1997, 1998, 1999
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

#ifndef PLT_SYSTEM_LINUX
#define PLT_SYSTEM_LINUX 0
#endif

#ifndef PLT_SYSTEM_HPUX
#define PLT_SYSTEM_HPUX 0
#else
#ifndef PLT_SYSTEM_HPUX_MAJOR
#define PLT_SYSTEM_HPUX_MAJOR 10
#endif
#endif

#ifndef PLT_SYSTEM_IRIX
#define PLT_SYSTEM_IRIX 0
#endif

#ifndef PLT_SYSTEM_SOLARIS
#define PLT_SYSTEM_SOLARIS 0
#endif

#ifndef PLT_SYSTEM_NT
#define PLT_SYSTEM_NT 0
#endif

#ifndef PLT_SYSTEM_OS2
#define PLT_SYSTEM_OS2 0
#endif

#ifndef PLT_SYSTEM_OPENVMS
#define PLT_SYSTEM_OPENVMS 0
#endif

#if PLT_SYSTEM_LINUX + PLT_SYSTEM_HPUX + PLT_SYSTEM_IRIX + PLT_SYSTEM_SOLARIS + PLT_SYSTEM_NT + PLT_SYSTEM_OS2 + PLT_SYSTEM_OPENVMS != 1
#error Must define exactly one system!
#endif

#include <stddef.h>

// --------------------------------------------------------------------------
// Enable/disable use of the connection manager class and the buffered
// (aka dynamic) XDR memory strams. Disabled by default if the user didn't
// specified otherwise.
#ifndef PLT_USE_BUFFERED_STREAMS
#define PLT_USE_BUFFERED_STREAMS 0
#endif

// --------------------------------------------------------------------------
// When running on top of NT, we always need to compile the connection
// manager class with a hash table due to NT's habbit of allocating file
// descriptors (aka handles). For other platforms this hash table isn't
// necessary, so don't enable them.
#if PLT_SYSTEM_NT
#undef  PLT_CNX_MGR_USE_HT
#define PLT_CNX_MGR_USE_HT 1
#endif

#ifndef PLT_CNX_MGR_USE_HT
#define PLT_CNX_MGR_USE_HT 0
#endif

// --------------------------------------------------------------------------
// Enable/disable compiling a minimalist ACPLT/KS server trunc only. In this
// case, no service handling is implemented, only the registration magic.
// This option is useful if you want to do a C-only ACPLT/KS server which
// can sit on different tranport and hardware environments, like servers for
// full-blown workstations and embedded systems (yes, that's true).
#ifndef PLT_SERVER_TRUNC_ONLY
#define PLT_SERVER_TRUNC_ONLY 0
#endif

// --------------------------------------------------------------------------
// If we're running on top of Solaris and the user didn't specified other-
// wise, then we'll use XTI (for those who do not know: it's something like
// the WinSuck 2.0 architecture for unix).
#if PLT_SYSTEM_SOLARIS
#ifndef PLT_USE_XTI
#define PLT_USE_XTI 0 // temporary fix until we get XTI support to work
#endif
#endif

#if PLT_SYSTEM_LINUX || PLT_SYSTEM_HPUX || PLT_SYSTEM_SOLARIS || PLT_SYSTEM_IRIX
#ifndef PLT_USE_SYSLOG
#define PLT_USE_SYSLOG 1
#endif
#endif

#if PLT_SYSTEM_NT
#ifndef PLT_USE_NTLOG
#define PLT_USE_NTLOG 1
#endif
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

#ifndef PLT_USE_NTLOG
#define PLT_USE_NTLOG 0
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

#if defined(__CYGWIN32__) || defined(__MINGW32__)
#define PLT_COMPILER_CYGWIN 1
#else
#define PLT_COMPILER_CYGWIN 0
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

#ifdef __DECCXX
#define PLT_COMPILER_DECCXX __DECCXX
#else 
#define PLT_COMPILER_DECCXX 0
#endif

#if PLT_COMPILER_GCC
#define PLT_SIMULATE_BOOL 0
#define PLT_SIMULATE_RTTI 1
#define PLT_INSTANTIATE_TEMPLATES 1
#define PLT_AVOID_DELETE_BUG 0
#endif

#if PLT_COMPILER_MSVC
# if PLT_COMPILER_MSVC < 1100
# define PLT_SIMULATE_BOOL 1
# pragma warning (disable : 4237 )  /* disable warning about defining bool   */
# else
# define PLT_SIMULATE_BOOL 0
#endif

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
#define PLT_SEE_ALL_TEMPLATES 1

#endif /* PLT_COMPILER_MSVC */

#if PLT_COMPILER_BORLAND
#pragma warn -hid
#pragma warn -inl
#pragma warn -stv
#define PLT_SEE_ALL_TEMPLATES 1
#define PLT_INSTANTIATE_TEMPLATES 1
#define PLT_RETTYPE_OVERLOADABLE 1
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

#if PLT_COMPILER_DECCXX
#define PLT_SIMULATE_BOOL 1
#define PLT_SIMULATE_RTTI 1
#define PLT_RETTYPE_OVERLOADABLE 0
#define PLT_ARRAY_NEW_OVERLOADABLE 0
#define PLT_SEE_ALL_TEMPLATES 0
#define PLT_INSTANTIATE_TEMPLATES 1
#endif /* PLT_COMPILER_DECCXX */

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
#define PLT_RETTYPE_CAST(typ) 
#else
#define PLT_RETTYPE_CAST(typ) typ
#endif


//
// Aarrrrrghhhh!! WinSock 2 vs. Winsock I. MS is soooooo brain damaged.
// They're even try to tell you in the Winsock 2 docu that the Berkeley
// socket API is just for TCP/IP and UDP/IP but nothing other. So how
// comes that /etc/protocols just defines a bunch of other protocols to
// be used through the socket API and esp. all that Linux apps can use
// IPX, etc...? It's just again plain Microsoft FUD.
//
#if PLT_SYSTEM_NT

#if PLT_COMPILER_BORLAND
#if __BORLANDC__ >= 0x0530
#define PLT_USE_WINSOCK2 1
#define _MSWSOCK_ /* HACK!! */
#else
#define PLT_USE_WINSOCK2 0
#endif

#else

#define PLT_USE_WINSOCK2 0

#endif

#endif /* PLT_SYSTEM_NT */


#endif /* PLT_CONFIG.H */

