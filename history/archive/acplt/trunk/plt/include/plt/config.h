/* -*-plt-c++-*- */
#ifndef PLT_CONFIG_INCLUDED
#define PLT_CONFIG_INCLUDED
/* $Header: /home/david/cvs/acplt/plt/include/plt/config.h,v 1.41 2011-07-15 13:26:55 sten Exp $ *//*
 * Copyright (c) 1996, 1997, 1998, 1999, 2000
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

#ifdef __OS2__
#define PLT_SYSTEM_OS2 1
#endif

#ifdef __NT__
#define PLT_SYSTEM_NT 1
#ifndef WIN32			/* 	add by Sten Gruener to avoid warnings on Windows plattform	*/
#define WIN32
#endif				/* 	add by Sten Gruener to avoid warnings on Windows plattform	*/
#define i386 1
#define mc68000 0
#define sparc 0
#define vax 0
#endif

#ifndef PLT_SYSTEM_LINUX
#define PLT_SYSTEM_LINUX 0
#endif

#ifndef PLT_SYSTEM_FREEBSD
#define PLT_SYSTEM_FREEBSD 0
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

#ifndef PLT_SYSTEM_MC164
#define PLT_SYSTEM_MC164 0
#endif

#if PLT_SYSTEM_LINUX + PLT_SYSTEM_FREEBSD + PLT_SYSTEM_HPUX + PLT_SYSTEM_IRIX + PLT_SYSTEM_SOLARIS + PLT_SYSTEM_NT + PLT_SYSTEM_OS2 + PLT_SYSTEM_OPENVMS + PLT_SYSTEM_MC164 != 1
#error Must define exactly one system!
#endif

#include <stddef.h>


/* --------------------------------------------------------------------------
*  Provide backwards compatibility with old ACPLT/KS applications written
*  for the 1.0.x series of the C++ Communication Library
*/
#ifndef PLT_SOURCE_V1_BC
#define PLT_SOURCE_V1_BC 1
#endif

#ifndef PLT_SOURCE_V1PLUS_BC
#define PLT_SOURCE_V1PLUS_BC 1
#endif

/* --------------------------------------------------------------------------
*  Enable/disable use of the connection manager class and the buffered
*  (aka dynamic) XDR memory strams. Disabled by default if the user didn�t
*  specified otherwise.
*/
#ifndef PLT_USE_BUFFERED_STREAMS
#define PLT_USE_BUFFERED_STREAMS 0
#endif

/* --------------------------------------------------------------------------
*  When running on top of NT, we always need to compile the connection
*  manager class with a hash table due to NT's habbit of allocating file
*  descriptors (aka handles). For other platforms this hash table isn�t
*  necessary, so don't enable them.
*/
#if PLT_SYSTEM_NT
#undef  PLT_CNX_MGR_USE_HT
#define PLT_CNX_MGR_USE_HT 1
#endif

#ifndef PLT_CNX_MGR_USE_HT
#define PLT_CNX_MGR_USE_HT 0
#endif

/* --------------------------------------------------------------------------
*  Enable/disable compiling a minimalist ACPLT/KS server trunc only. In this
*  case, no service handling is implemented, only the registration magic.
*  This option is useful if you want to do a C-only ACPLT/KS server which
*  can sit on different tranport and hardware environments, like servers for
*  full-blown workstations and embedded systems (yes, that's true).
*/
#ifndef PLT_SERVER_TRUNC_ONLY
#define PLT_SERVER_TRUNC_ONLY 0
#endif

/*  --------------------------------------------------------------------------
*  If we're running on top of Solaris and the user didn't specified other-
*  wise, then we'll use XTI (for those who do not know: it's something like
*  the WinSuck 2.0 architecture for unix).
*/
#if PLT_SYSTEM_SOLARIS
#ifndef PLT_USE_XTI
#define PLT_USE_XTI 0 /* temporary fix until we get XTI support to work */
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
#if !PLT_SYSTEM_MC164
#include <sys/types.h>
#else
#include <stddef.h>
#endif
#define PLT_SIZE_T_MAX ((size_t)-1)
#endif

#ifdef __GNUC__
#define PLT_COMPILER_GCC ((__GNUC__ << 16) + __GNUC_MINOR__)
#else
#define PLT_COMPILER_GCC 0
#endif

#if defined(__CYGWIN32__) || defined(__MINGW32__)
#define PLT_COMPILER_CYGWIN 1
#else
#define PLT_COMPILER_CYGWIN 0
#endif

#if defined(__GLIBC__)
#if defined(__GLIBC_MINOR__)
#define PLT_RUNTIME_GLIBC ((__GLIBC__ << 16) + __GLIBC_MINOR__)
#else
#define PLT_RUNTIME_GLIBC (__GLIBC__ << 16)
#endif
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

#define PLT_COMPILER_KEIL 0

#if PLT_COMPILER_GCC
#define PLT_SIMULATE_BOOL 0
#define PLT_SIMULATE_RTTI 1
#define PLT_INSTANTIATE_TEMPLATES 1
#define PLT_AVOID_DELETE_BUG 0

#if PLT_COMPILER_GCC >= 0x40000
#define FEATURE_TEMPL_SPEC template <>
#endif

#endif

#if PLT_COMPILER_MSVC
# if PLT_COMPILER_MSVC < 1100
# define PLT_SIMULATE_BOOL 1
# pragma warning (disable : 4237 )  /* disable warning about defining bool   */
# else
# define PLT_SIMULATE_BOOL 0
# endif

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
#define PLT_SIMULATE_BOOL 0 /* Newer CXX releases now support this... */
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

#ifndef FEATURE_TEMPL_SPEC
#define FEATURE_TEMPL_SPEC
#endif

#if PLT_SIMULATE_BOOL
typedef int bool;
enum { false=0, true=1 };
#endif

#if PLT_RETTYPE_OVERLOADABLE
#define PLT_RETTYPE_CAST(typ) 
#else
#define PLT_RETTYPE_CAST(typ) typ
#endif


/* 
 *  Aarrrrrghhhh!! WinSock 2 vs. Winsock I. MS is soooooo brain damaged.
 *  They're even try to tell you in the Winsock 2 docu that the Berkeley
 *  socket API is just for TCP/IP and UDP/IP but nothing other. So how
 *  comes that /etc/protocols just defines a bunch of other protocols to
 *  be used through the socket API and esp. all that Linux apps can use
 *  IPX, etc...? It's just again plain Microsoft FUD.
 */ 
#if PLT_SYSTEM_NT

/*
 * Borland makes trouble... If we find a newer Borland C++ compiler, then
 * we switch over to WinSock2. To get around header idiosyncrasies, we need
 * to define _MSWSOCK_ otherwise we can not compile. Where have all the good
 * men gone -- not to MS and not to Borland either.
 */
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

/*
 * BigQ is even much more brain-damaged than Microsoft: defaulting the
 * file descriptor set size to 32 is soooo idiotic, so we should slap
 * the person who came up with this size over and over again.
 *
 * Per default, we allocate 1024 file descriptors, so plenty of clients
 * can connect to ACPLT/KS servers and bog them down. Oh, greetings to
 * Bogdan, by the way...<grin> You can avoid setting FD_SETSIZE by defining
 * the symbol PLT_DONTTOUCH_FD_SETSIZE.
 */
#if PLT_SYSTEM_OPENVMS
#ifndef PLT_DONTTOUCH_FD_SETSIZE
#define FD_SETSIZE 1024
#endif
#endif

/*
 * Set maximum pool block count. Every block (fragment) is 4k in size.
 * This pool block maximum is just a simple insurance against clients
 * gone mad and sending and sending data without stopping.
 */
#ifndef PLT_POOL_BLOCK_COUNT
#define PLT_POOL_BLOCK_COUNT 2048
#endif

/* --------------------------------------------------------------------------
 * Enable or disable use of (now) depreciated header files. If this define
 * has not been set and if we are compiling using certain newer compilers,
 * then we automatically switch to the new C++ header files.
 */
#ifndef PLT_USE_DEPRECIATED_HEADER

#if PLT_COMPILER_GCC != 0
#if PLT_COMPILER_GCC >= 0x30000
#define PLT_USE_DEPRECIATED_HEADER 0
#else
#define PLT_USE_DEPRECIATED_HEADER 1
#endif
#endif

#if PLT_COMPILER_MSVC != 0
#define PLT_USE_DEPRECIATED_HEADER 1
#endif


#if PLT_COMPILER_BORLAND
#define PLT_USE_DEPRECIATED_HEADER 1
#endif


#endif

/*
 * Take care of namespace games played by Bjoern Syroup.
 */
#if PLT_USE_DEPRECIATED_HEADER
#define STDNS
#else
#define STDNS std
#endif

#endif /* PLT_CONFIG.H */

/* End of plt/config.h */
