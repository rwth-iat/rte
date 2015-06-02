/* THIS IS A SINGLE-FILE DISTRIBUTION CONCATENATED FROM THE OPEN62541 SOURCES 
 * visit http://open62541.org/ for information about this software
 * Git-Revision: v0.1.0-RC4-31-g1a03d02
 */
 
 /*
 * Copyright (C) 2015 the contributors as stated in the AUTHORS file
 *
 * This file is part of open62541. open62541 is free software: you can
 * redistribute it and/or modify it under the terms of the GNU Lesser General
 * Public License, version 3 (as published by the Free Software Foundation) with
 * a static linking exception as stated in the LICENSE file provided with
 * open62541.
 *
 * open62541 is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 */

#define UA_AMALGAMATE
#ifndef UA_DYNAMIC_LINKING
# define UA_DYNAMIC_LINKING
#endif

#ifndef UA_CONFIG_H_
#define UA_CONFIG_H_

/* Buid options and configuration (set by cmake) */

#define UA_LOGLEVEL 300
/* #undef UA_MULTITHREADING */

/* Function Export */
#ifdef _WIN32
#  ifdef UA_DYNAMIC_LINKING
#    ifdef __GNUC__
#      define UA_EXPORT __attribute__ ((dllexport))
#    else
#      define UA_EXPORT __declspec(dllexport)
#    endif
#  else
#    ifdef __GNUC__
#      define UA_EXPORT __attribute__ ((dllexport))
#    else
#      define UA_EXPORT __declspec(dllimport)
#    endif
#  endif
#else
#  if __GNUC__ || __clang__
#    define UA_EXPORT __attribute__ ((visibility ("default")))
#  else
#    define UA_EXPORT
#  endif
#endif

/*	Define your own htoleXX and leXXtoh here if needed.
	Otherwise the ones defined in endian.h are used		*/
//	#define htole16(x)	{...}(x)
//	#define htole32(x)	{...}(x)
//	#define htole64(x)	{...}(x)
//	#define le16toh(x)	{...}(x)
//	#define le32toh(x)	{...}(x)
//	#define le64toh(x)	{...}(x)

#endif /* UA_CONFIG_H_ */
#ifndef UA_UTIL_H_
#define UA_UTIL_H_

#ifndef UA_AMALGAMATE
# include "ua_config.h"
#endif

#ifndef __USE_POSIX
# define __USE_POSIX
#endif
#ifndef _POSIX_SOURCE
# define _POSIX_SOURCE
#endif
#ifndef _POSIX_C_SOURCE
# define _POSIX_C_SOURCE 199309L
#endif
#ifndef _BSD_SOURCE
# define _BSD_SOURCE
#endif
#ifndef _DEFAULT_SOURCE
# define _DEFAULT_SOURCE
#endif

/*********************/
/* Memory Management */
/*********************/

#include <stdlib.h> // malloc, free
#include <string.h> // memcpy
#include <assert.h> // assert

#ifdef _WIN32
# include <malloc.h>
#endif

#define UA_NULL ((void *)0)

// subtract from nodeids to get from the encoding to the content
#define UA_ENCODINGOFFSET_XML 1
#define UA_ENCODINGOFFSET_BINARY 2

#define UA_assert(ignore) assert(ignore)

/* Replace the macros with functions for custom allocators if necessary */
#define UA_free(ptr) free(ptr)
#define UA_malloc(size) malloc(size)
#define UA_calloc(num, size) calloc(num, size)
#define UA_realloc(ptr, size) realloc(ptr, size)
#define UA_memcpy(dst, src, size) memcpy(dst, src, size)
#define UA_memset(ptr, value, size) memset(ptr, value, size)

#ifdef NO_ALLOCA
#else
#ifdef _WIN32
    # define UA_alloca(SIZE) _alloca(SIZE)
#else
 #ifdef __GNUC__
    # define UA_alloca(size)   __builtin_alloca (size)
 #else
    # include <alloca.h>
    # define UA_alloca(SIZE) alloca(SIZE)
 #endif
#endif
#endif /* NO_ALLOCA */
/********************/
/* System Libraries */
/********************/

#include <stdarg.h> // va_start, va_end
#include <time.h>
#include <stdio.h> // printf
#include <inttypes.h>

#ifdef _WIN32
# include <winsock2.h> //needed for amalgation
# include <windows.h>
# undef SLIST_ENTRY
# define RAND(SEED) (UA_UInt32)rand()
#else
# if !(defined htole16 && defined htole32 && defined htole64 && defined le16toh && defined le32toh && defined le64toh)
#  include <endian.h>
#  if !(defined htole16 && defined htole32 && defined htole64 && defined le16toh && defined le32toh && defined le64toh)
#   if ( __BYTE_ORDER != __LITTLE_ENDIAN )
#    error "Host byte order is not little-endian and no appropriate conversion functions are defined. (Have a look at ua_config.h)"
#   else
#    define htole16(x) x
#    define htole32(x) x
#    define htole64(x) x
#    define le16toh(x) x
#    define le32toh(x) x
#    define le64toh(x) x
#   endif
#  endif
# endif
# include <sys/time.h>
# define RAND(SEED) (UA_UInt32)rand_r(SEED)
#endif

/*************************/
/* External Dependencies */
/*************************/

#ifndef UA_AMALGAMATE
# include "queue.h"
#endif

#ifdef UA_MULTITHREADING
# define _LGPL_SOURCE
# include <urcu.h>
# include <urcu/wfcqueue.h>
# include <urcu/compiler.h> // for caa_container_of
# include <urcu/uatomic.h>
# include <urcu/rculfhash.h>
#endif

#endif /* UA_UTIL_H_ */
#include "open62541.h"
/*********************************** amalgamated original file "C:/AcpltDevelopmentKit/acplt/dev/open62541WrapperProject/open62541/src_generated/ua_config.h" ***********************************/

/* Buid options and configuration (set by cmake) */

#define UA_LOGLEVEL 300
/* #undef UA_MULTITHREADING */

/* Function Export */
#ifdef _WIN32
#  ifdef UA_DYNAMIC_LINKING
#    ifdef __GNUC__
#      define UA_EXPORT __attribute__ ((dllexport))
#    else
#      define UA_EXPORT __declspec(dllexport)
#    endif
#  else
#    ifdef __GNUC__
#      define UA_EXPORT __attribute__ ((dllexport))
#    else
#      define UA_EXPORT __declspec(dllimport)
#    endif
#  endif
#else
#  if __GNUC__ || __clang__
#    define UA_EXPORT __attribute__ ((visibility ("default")))
#  else
#    define UA_EXPORT
#  endif
#endif

/*	Define your own htoleXX and leXXtoh here if needed.
	Otherwise the ones defined in endian.h are used		*/
//	#define htole16(x)	{...}(x)
//	#define htole32(x)	{...}(x)
//	#define htole64(x)	{...}(x)
//	#define le16toh(x)	{...}(x)
//	#define le32toh(x)	{...}(x)
//	#define le64toh(x)	{...}(x)

/*********************************** amalgamated original file "C:/AcpltDevelopmentKit/acplt/dev/open62541WrapperProject/open62541/deps/queue.h" ***********************************/
/*	$OpenBSD: queue.h,v 1.38 2013/07/03 15:05:21 fgsch Exp $	*/
/*	$NetBSD: queue.h,v 1.11 1996/05/16 05:17:14 mycroft Exp $	*/

/*
 * Copyright (c) 1991, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)queue.h	8.5 (Berkeley) 8/20/94
 */

#ifndef	_SYS_QUEUE_H_
#define	_SYS_QUEUE_H_

/*
 * This file defines five types of data structures: singly-linked lists, 
 * lists, simple queues, tail queues, and circular queues.
 *
 *
 * A singly-linked list is headed by a single forward pointer. The elements
 * are singly linked for minimum space and pointer manipulation overhead at
 * the expense of O(n) removal for arbitrary elements. New elements can be
 * added to the list after an existing element or at the head of the list.
 * Elements being removed from the head of the list should use the explicit
 * macro for this purpose for optimum efficiency. A singly-linked list may
 * only be traversed in the forward direction.  Singly-linked lists are ideal
 * for applications with large datasets and few or no removals or for
 * implementing a LIFO queue.
 *
 * A list is headed by a single forward pointer (or an array of forward
 * pointers for a hash table header). The elements are doubly linked
 * so that an arbitrary element can be removed without a need to
 * traverse the list. New elements can be added to the list before
 * or after an existing element or at the head of the list. A list
 * may only be traversed in the forward direction.
 *
 * A simple queue is headed by a pair of pointers, one the head of the
 * list and the other to the tail of the list. The elements are singly
 * linked to save space, so elements can only be removed from the
 * head of the list. New elements can be added to the list before or after
 * an existing element, at the head of the list, or at the end of the
 * list. A simple queue may only be traversed in the forward direction.
 *
 * A tail queue is headed by a pair of pointers, one to the head of the
 * list and the other to the tail of the list. The elements are doubly
 * linked so that an arbitrary element can be removed without a need to
 * traverse the list. New elements can be added to the list before or
 * after an existing element, at the head of the list, or at the end of
 * the list. A tail queue may be traversed in either direction.
 *
 * A circle queue is headed by a pair of pointers, one to the head of the
 * list and the other to the tail of the list. The elements are doubly
 * linked so that an arbitrary element can be removed without a need to
 * traverse the list. New elements can be added to the list before or after
 * an existing element, at the head of the list, or at the end of the list.
 * A circle queue may be traversed in either direction, but has a more
 * complex end of list detection.
 *
 * For details on the use of these macros, see the queue(3) manual page.
 */

#if defined(QUEUE_MACRO_DEBUG) || (defined(_KERNEL) && defined(DIAGNOSTIC))
#define _Q_INVALIDATE(a) (a) = ((void *)-1)
#else
#define _Q_INVALIDATE(a)
#endif

/*
 * Singly-linked List definitions.
 */
#define SLIST_HEAD(name, type)						\
struct name {								\
	struct type *slh_first;	/* first element */			\
}
 
#define	SLIST_HEAD_INITIALIZER(head)					\
	{ NULL }
 
#define SLIST_ENTRY(type)						\
struct {								\
	struct type *sle_next;	/* next element */			\
}
 
/*
 * Singly-linked List access methods.
 */
#define	SLIST_FIRST(head)	((head)->slh_first)
#define	SLIST_END(head)		NULL
#define	SLIST_EMPTY(head)	(SLIST_FIRST(head) == SLIST_END(head))
#define	SLIST_NEXT(elm, field)	((elm)->field.sle_next)

#define	SLIST_FOREACH(var, head, field)					\
	for((var) = SLIST_FIRST(head);					\
	    (var) != SLIST_END(head);					\
	    (var) = SLIST_NEXT(var, field))

#define	SLIST_FOREACH_SAFE(var, head, field, tvar)			\
	for ((var) = SLIST_FIRST(head);				\
	    (var) && ((tvar) = SLIST_NEXT(var, field), 1);		\
	    (var) = (tvar))

/*
 * Singly-linked List functions.
 */
#define	SLIST_INIT(head) {						\
	SLIST_FIRST(head) = SLIST_END(head);				\
}

#define	SLIST_INSERT_AFTER(slistelm, elm, field) do {			\
	(elm)->field.sle_next = (slistelm)->field.sle_next;		\
	(slistelm)->field.sle_next = (elm);				\
} while (0)

#define	SLIST_INSERT_HEAD(head, elm, field) do {			\
	(elm)->field.sle_next = (head)->slh_first;			\
	(head)->slh_first = (elm);					\
} while (0)

#define	SLIST_REMOVE_AFTER(elm, field) do {				\
	(elm)->field.sle_next = (elm)->field.sle_next->field.sle_next;	\
} while (0)

#define	SLIST_REMOVE_HEAD(head, field) do {				\
	(head)->slh_first = (head)->slh_first->field.sle_next;		\
} while (0)

#define SLIST_REMOVE(head, elm, type, field) do {			\
	if ((head)->slh_first == (elm)) {				\
		SLIST_REMOVE_HEAD((head), field);			\
	} else {							\
		struct type *curelm = (head)->slh_first;		\
									\
		while (curelm->field.sle_next != (elm))			\
			curelm = curelm->field.sle_next;		\
		curelm->field.sle_next =				\
		    curelm->field.sle_next->field.sle_next;		\
		_Q_INVALIDATE((elm)->field.sle_next);			\
	}								\
} while (0)

/*
 * List definitions.
 */
#define LIST_HEAD(name, type)						\
struct name {								\
	struct type *lh_first;	/* first element */			\
}

#define LIST_HEAD_INITIALIZER(head)					\
	{ NULL }

#define LIST_ENTRY(type)						\
struct {								\
	struct type *le_next;	/* next element */			\
	struct type **le_prev;	/* address of previous next element */	\
}

/*
 * List access methods
 */
#define	LIST_FIRST(head)		((head)->lh_first)
#define	LIST_END(head)			NULL
#define	LIST_EMPTY(head)		(LIST_FIRST(head) == LIST_END(head))
#define	LIST_NEXT(elm, field)		((elm)->field.le_next)

#define LIST_FOREACH(var, head, field)					\
	for((var) = LIST_FIRST(head);					\
	    (var)!= LIST_END(head);					\
	    (var) = LIST_NEXT(var, field))

#define	LIST_FOREACH_SAFE(var, head, field, tvar)			\
	for ((var) = LIST_FIRST(head);				\
	    (var) && ((tvar) = LIST_NEXT(var, field), 1);		\
	    (var) = (tvar))

/*
 * List functions.
 */
#define	LIST_INIT(head) do {						\
	LIST_FIRST(head) = LIST_END(head);				\
} while (0)

#define LIST_INSERT_AFTER(listelm, elm, field) do {			\
	if (((elm)->field.le_next = (listelm)->field.le_next) != NULL)	\
		(listelm)->field.le_next->field.le_prev =		\
		    &(elm)->field.le_next;				\
	(listelm)->field.le_next = (elm);				\
	(elm)->field.le_prev = &(listelm)->field.le_next;		\
} while (0)

#define	LIST_INSERT_BEFORE(listelm, elm, field) do {			\
	(elm)->field.le_prev = (listelm)->field.le_prev;		\
	(elm)->field.le_next = (listelm);				\
	*(listelm)->field.le_prev = (elm);				\
	(listelm)->field.le_prev = &(elm)->field.le_next;		\
} while (0)

#define LIST_INSERT_HEAD(head, elm, field) do {				\
	if (((elm)->field.le_next = (head)->lh_first) != NULL)		\
		(head)->lh_first->field.le_prev = &(elm)->field.le_next;\
	(head)->lh_first = (elm);					\
	(elm)->field.le_prev = &(head)->lh_first;			\
} while (0)

#define LIST_REMOVE(elm, field) do {					\
	if ((elm)->field.le_next != NULL)				\
		(elm)->field.le_next->field.le_prev =			\
		    (elm)->field.le_prev;				\
	*(elm)->field.le_prev = (elm)->field.le_next;			\
	_Q_INVALIDATE((elm)->field.le_prev);				\
	_Q_INVALIDATE((elm)->field.le_next);				\
} while (0)

#define LIST_REPLACE(elm, elm2, field) do {				\
	if (((elm2)->field.le_next = (elm)->field.le_next) != NULL)	\
		(elm2)->field.le_next->field.le_prev =			\
		    &(elm2)->field.le_next;				\
	(elm2)->field.le_prev = (elm)->field.le_prev;			\
	*(elm2)->field.le_prev = (elm2);				\
	_Q_INVALIDATE((elm)->field.le_prev);				\
	_Q_INVALIDATE((elm)->field.le_next);				\
} while (0)

/*
 * Simple queue definitions.
 */
#define SIMPLEQ_HEAD(name, type)					\
struct name {								\
	struct type *sqh_first;	/* first element */			\
	struct type **sqh_last;	/* addr of last next element */		\
}

#define SIMPLEQ_HEAD_INITIALIZER(head)					\
	{ NULL, &(head).sqh_first }

#define SIMPLEQ_ENTRY(type)						\
struct {								\
	struct type *sqe_next;	/* next element */			\
}

/*
 * Simple queue access methods.
 */
#define	SIMPLEQ_FIRST(head)	    ((head)->sqh_first)
#define	SIMPLEQ_END(head)	    NULL
#define	SIMPLEQ_EMPTY(head)	    (SIMPLEQ_FIRST(head) == SIMPLEQ_END(head))
#define	SIMPLEQ_NEXT(elm, field)    ((elm)->field.sqe_next)

#define SIMPLEQ_FOREACH(var, head, field)				\
	for((var) = SIMPLEQ_FIRST(head);				\
	    (var) != SIMPLEQ_END(head);					\
	    (var) = SIMPLEQ_NEXT(var, field))

#define	SIMPLEQ_FOREACH_SAFE(var, head, field, tvar)			\
	for ((var) = SIMPLEQ_FIRST(head);				\
	    (var) && ((tvar) = SIMPLEQ_NEXT(var, field), 1);		\
	    (var) = (tvar))

/*
 * Simple queue functions.
 */
#define	SIMPLEQ_INIT(head) do {						\
	(head)->sqh_first = NULL;					\
	(head)->sqh_last = &(head)->sqh_first;				\
} while (0)

#define SIMPLEQ_INSERT_HEAD(head, elm, field) do {			\
	if (((elm)->field.sqe_next = (head)->sqh_first) == NULL)	\
		(head)->sqh_last = &(elm)->field.sqe_next;		\
	(head)->sqh_first = (elm);					\
} while (0)

#define SIMPLEQ_INSERT_TAIL(head, elm, field) do {			\
	(elm)->field.sqe_next = NULL;					\
	*(head)->sqh_last = (elm);					\
	(head)->sqh_last = &(elm)->field.sqe_next;			\
} while (0)

#define SIMPLEQ_INSERT_AFTER(head, listelm, elm, field) do {		\
	if (((elm)->field.sqe_next = (listelm)->field.sqe_next) == NULL)\
		(head)->sqh_last = &(elm)->field.sqe_next;		\
	(listelm)->field.sqe_next = (elm);				\
} while (0)

#define SIMPLEQ_REMOVE_HEAD(head, field) do {			\
	if (((head)->sqh_first = (head)->sqh_first->field.sqe_next) == NULL) \
		(head)->sqh_last = &(head)->sqh_first;			\
} while (0)

#define SIMPLEQ_REMOVE_AFTER(head, elm, field) do {			\
	if (((elm)->field.sqe_next = (elm)->field.sqe_next->field.sqe_next) \
	    == NULL)							\
		(head)->sqh_last = &(elm)->field.sqe_next;		\
} while (0)

/*
 * XOR Simple queue definitions.
 */
#define XSIMPLEQ_HEAD(name, type)					\
struct name {								\
	struct type *sqx_first;	/* first element */			\
	struct type **sqx_last;	/* addr of last next element */		\
	unsigned long sqx_cookie;					\
}

#define XSIMPLEQ_ENTRY(type)						\
struct {								\
	struct type *sqx_next;	/* next element */			\
}

/*
 * XOR Simple queue access methods.
 */
#define XSIMPLEQ_XOR(head, ptr)	    ((__typeof(ptr))((head)->sqx_cookie ^ \
					(unsigned long)(ptr)))
#define	XSIMPLEQ_FIRST(head)	    XSIMPLEQ_XOR(head, ((head)->sqx_first))
#define	XSIMPLEQ_END(head)	    NULL
#define	XSIMPLEQ_EMPTY(head)	    (XSIMPLEQ_FIRST(head) == XSIMPLEQ_END(head))
#define	XSIMPLEQ_NEXT(head, elm, field)    XSIMPLEQ_XOR(head, ((elm)->field.sqx_next))


#define XSIMPLEQ_FOREACH(var, head, field)				\
	for ((var) = XSIMPLEQ_FIRST(head);				\
	    (var) != XSIMPLEQ_END(head);				\
	    (var) = XSIMPLEQ_NEXT(head, var, field))

#define	XSIMPLEQ_FOREACH_SAFE(var, head, field, tvar)			\
	for ((var) = XSIMPLEQ_FIRST(head);				\
	    (var) && ((tvar) = XSIMPLEQ_NEXT(head, var, field), 1);	\
	    (var) = (tvar))

/*
 * XOR Simple queue functions.
 */
#define	XSIMPLEQ_INIT(head) do {					\
	arc4random_buf(&(head)->sqx_cookie, sizeof((head)->sqx_cookie)); \
	(head)->sqx_first = XSIMPLEQ_XOR(head, NULL);			\
	(head)->sqx_last = XSIMPLEQ_XOR(head, &(head)->sqx_first);	\
} while (0)

#define XSIMPLEQ_INSERT_HEAD(head, elm, field) do {			\
	if (((elm)->field.sqx_next = (head)->sqx_first) ==		\
	    XSIMPLEQ_XOR(head, NULL))					\
		(head)->sqx_last = XSIMPLEQ_XOR(head, &(elm)->field.sqx_next); \
	(head)->sqx_first = XSIMPLEQ_XOR(head, (elm));			\
} while (0)

#define XSIMPLEQ_INSERT_TAIL(head, elm, field) do {			\
	(elm)->field.sqx_next = XSIMPLEQ_XOR(head, NULL);		\
	*(XSIMPLEQ_XOR(head, (head)->sqx_last)) = XSIMPLEQ_XOR(head, (elm)); \
	(head)->sqx_last = XSIMPLEQ_XOR(head, &(elm)->field.sqx_next);	\
} while (0)

#define XSIMPLEQ_INSERT_AFTER(head, listelm, elm, field) do {		\
	if (((elm)->field.sqx_next = (listelm)->field.sqx_next) ==	\
	    XSIMPLEQ_XOR(head, NULL))					\
		(head)->sqx_last = XSIMPLEQ_XOR(head, &(elm)->field.sqx_next); \
	(listelm)->field.sqx_next = XSIMPLEQ_XOR(head, (elm));		\
} while (0)

#define XSIMPLEQ_REMOVE_HEAD(head, field) do {				\
	if (((head)->sqx_first = XSIMPLEQ_XOR(head,			\
	    (head)->sqx_first)->field.sqx_next) == XSIMPLEQ_XOR(head, NULL)) \
		(head)->sqx_last = XSIMPLEQ_XOR(head, &(head)->sqx_first); \
} while (0)

#define XSIMPLEQ_REMOVE_AFTER(head, elm, field) do {			\
	if (((elm)->field.sqx_next = XSIMPLEQ_XOR(head,			\
	    (elm)->field.sqx_next)->field.sqx_next)			\
	    == XSIMPLEQ_XOR(head, NULL))				\
		(head)->sqx_last = 					\
		    XSIMPLEQ_XOR(head, &(elm)->field.sqx_next);		\
} while (0)

		    
/*
 * Tail queue definitions.
 */
#define TAILQ_HEAD(name, type)						\
struct name {								\
	struct type *tqh_first;	/* first element */			\
	struct type **tqh_last;	/* addr of last next element */		\
}

#define TAILQ_HEAD_INITIALIZER(head)					\
	{ NULL, &(head).tqh_first }

#define TAILQ_ENTRY(type)						\
struct {								\
	struct type *tqe_next;	/* next element */			\
	struct type **tqe_prev;	/* address of previous next element */	\
}

/* 
 * tail queue access methods 
 */
#define	TAILQ_FIRST(head)		((head)->tqh_first)
#define	TAILQ_END(head)			NULL
#define	TAILQ_NEXT(elm, field)		((elm)->field.tqe_next)
#define TAILQ_LAST(head, headname)					\
	(*(((struct headname *)((head)->tqh_last))->tqh_last))
/* XXX */
#define TAILQ_PREV(elm, headname, field)				\
	(*(((struct headname *)((elm)->field.tqe_prev))->tqh_last))
#define	TAILQ_EMPTY(head)						\
	(TAILQ_FIRST(head) == TAILQ_END(head))

#define TAILQ_FOREACH(var, head, field)					\
	for((var) = TAILQ_FIRST(head);					\
	    (var) != TAILQ_END(head);					\
	    (var) = TAILQ_NEXT(var, field))

#define	TAILQ_FOREACH_SAFE(var, head, field, tvar)			\
	for ((var) = TAILQ_FIRST(head);					\
	    (var) != TAILQ_END(head) &&					\
	    ((tvar) = TAILQ_NEXT(var, field), 1);			\
	    (var) = (tvar))


#define TAILQ_FOREACH_REVERSE(var, head, headname, field)		\
	for((var) = TAILQ_LAST(head, headname);				\
	    (var) != TAILQ_END(head);					\
	    (var) = TAILQ_PREV(var, headname, field))

#define	TAILQ_FOREACH_REVERSE_SAFE(var, head, headname, field, tvar)	\
	for ((var) = TAILQ_LAST(head, headname);			\
	    (var) != TAILQ_END(head) &&					\
	    ((tvar) = TAILQ_PREV(var, headname, field), 1);		\
	    (var) = (tvar))

/*
 * Tail queue functions.
 */
#define	TAILQ_INIT(head) do {						\
	(head)->tqh_first = NULL;					\
	(head)->tqh_last = &(head)->tqh_first;				\
} while (0)

#define TAILQ_INSERT_HEAD(head, elm, field) do {			\
	if (((elm)->field.tqe_next = (head)->tqh_first) != NULL)	\
		(head)->tqh_first->field.tqe_prev =			\
		    &(elm)->field.tqe_next;				\
	else								\
		(head)->tqh_last = &(elm)->field.tqe_next;		\
	(head)->tqh_first = (elm);					\
	(elm)->field.tqe_prev = &(head)->tqh_first;			\
} while (0)

#define TAILQ_INSERT_TAIL(head, elm, field) do {			\
	(elm)->field.tqe_next = NULL;					\
	(elm)->field.tqe_prev = (head)->tqh_last;			\
	*(head)->tqh_last = (elm);					\
	(head)->tqh_last = &(elm)->field.tqe_next;			\
} while (0)

#define TAILQ_INSERT_AFTER(head, listelm, elm, field) do {		\
	if (((elm)->field.tqe_next = (listelm)->field.tqe_next) != NULL)\
		(elm)->field.tqe_next->field.tqe_prev =			\
		    &(elm)->field.tqe_next;				\
	else								\
		(head)->tqh_last = &(elm)->field.tqe_next;		\
	(listelm)->field.tqe_next = (elm);				\
	(elm)->field.tqe_prev = &(listelm)->field.tqe_next;		\
} while (0)

#define	TAILQ_INSERT_BEFORE(listelm, elm, field) do {			\
	(elm)->field.tqe_prev = (listelm)->field.tqe_prev;		\
	(elm)->field.tqe_next = (listelm);				\
	*(listelm)->field.tqe_prev = (elm);				\
	(listelm)->field.tqe_prev = &(elm)->field.tqe_next;		\
} while (0)

#define TAILQ_REMOVE(head, elm, field) do {				\
	if (((elm)->field.tqe_next) != NULL)				\
		(elm)->field.tqe_next->field.tqe_prev =			\
		    (elm)->field.tqe_prev;				\
	else								\
		(head)->tqh_last = (elm)->field.tqe_prev;		\
	*(elm)->field.tqe_prev = (elm)->field.tqe_next;			\
	_Q_INVALIDATE((elm)->field.tqe_prev);				\
	_Q_INVALIDATE((elm)->field.tqe_next);				\
} while (0)

#define TAILQ_REPLACE(head, elm, elm2, field) do {			\
	if (((elm2)->field.tqe_next = (elm)->field.tqe_next) != NULL)	\
		(elm2)->field.tqe_next->field.tqe_prev =		\
		    &(elm2)->field.tqe_next;				\
	else								\
		(head)->tqh_last = &(elm2)->field.tqe_next;		\
	(elm2)->field.tqe_prev = (elm)->field.tqe_prev;			\
	*(elm2)->field.tqe_prev = (elm2);				\
	_Q_INVALIDATE((elm)->field.tqe_prev);				\
	_Q_INVALIDATE((elm)->field.tqe_next);				\
} while (0)

/*
 * Circular queue definitions.
 */
#define CIRCLEQ_HEAD(name, type)					\
struct name {								\
	struct type *cqh_first;		/* first element */		\
	struct type *cqh_last;		/* last element */		\
}

#define CIRCLEQ_HEAD_INITIALIZER(head)					\
	{ CIRCLEQ_END(&head), CIRCLEQ_END(&head) }

#define CIRCLEQ_ENTRY(type)						\
struct {								\
	struct type *cqe_next;		/* next element */		\
	struct type *cqe_prev;		/* previous element */		\
}

/*
 * Circular queue access methods 
 */
#define	CIRCLEQ_FIRST(head)		((head)->cqh_first)
#define	CIRCLEQ_LAST(head)		((head)->cqh_last)
#define	CIRCLEQ_END(head)		((void *)(head))
#define	CIRCLEQ_NEXT(elm, field)	((elm)->field.cqe_next)
#define	CIRCLEQ_PREV(elm, field)	((elm)->field.cqe_prev)
#define	CIRCLEQ_EMPTY(head)						\
	(CIRCLEQ_FIRST(head) == CIRCLEQ_END(head))

#define CIRCLEQ_FOREACH(var, head, field)				\
	for((var) = CIRCLEQ_FIRST(head);				\
	    (var) != CIRCLEQ_END(head);					\
	    (var) = CIRCLEQ_NEXT(var, field))

#define	CIRCLEQ_FOREACH_SAFE(var, head, field, tvar)			\
	for ((var) = CIRCLEQ_FIRST(head);				\
	    (var) != CIRCLEQ_END(head) &&				\
	    ((tvar) = CIRCLEQ_NEXT(var, field), 1);			\
	    (var) = (tvar))

#define CIRCLEQ_FOREACH_REVERSE(var, head, field)			\
	for((var) = CIRCLEQ_LAST(head);					\
	    (var) != CIRCLEQ_END(head);					\
	    (var) = CIRCLEQ_PREV(var, field))

#define	CIRCLEQ_FOREACH_REVERSE_SAFE(var, head, headname, field, tvar)	\
	for ((var) = CIRCLEQ_LAST(head, headname);			\
	    (var) != CIRCLEQ_END(head) && 				\
	    ((tvar) = CIRCLEQ_PREV(var, headname, field), 1);		\
	    (var) = (tvar))

/*
 * Circular queue functions.
 */
#define	CIRCLEQ_INIT(head) do {						\
	(head)->cqh_first = CIRCLEQ_END(head);				\
	(head)->cqh_last = CIRCLEQ_END(head);				\
} while (0)

#define CIRCLEQ_INSERT_AFTER(head, listelm, elm, field) do {		\
	(elm)->field.cqe_next = (listelm)->field.cqe_next;		\
	(elm)->field.cqe_prev = (listelm);				\
	if ((listelm)->field.cqe_next == CIRCLEQ_END(head))		\
		(head)->cqh_last = (elm);				\
	else								\
		(listelm)->field.cqe_next->field.cqe_prev = (elm);	\
	(listelm)->field.cqe_next = (elm);				\
} while (0)

#define CIRCLEQ_INSERT_BEFORE(head, listelm, elm, field) do {		\
	(elm)->field.cqe_next = (listelm);				\
	(elm)->field.cqe_prev = (listelm)->field.cqe_prev;		\
	if ((listelm)->field.cqe_prev == CIRCLEQ_END(head))		\
		(head)->cqh_first = (elm);				\
	else								\
		(listelm)->field.cqe_prev->field.cqe_next = (elm);	\
	(listelm)->field.cqe_prev = (elm);				\
} while (0)

#define CIRCLEQ_INSERT_HEAD(head, elm, field) do {			\
	(elm)->field.cqe_next = (head)->cqh_first;			\
	(elm)->field.cqe_prev = CIRCLEQ_END(head);			\
	if ((head)->cqh_last == CIRCLEQ_END(head))			\
		(head)->cqh_last = (elm);				\
	else								\
		(head)->cqh_first->field.cqe_prev = (elm);		\
	(head)->cqh_first = (elm);					\
} while (0)

#define CIRCLEQ_INSERT_TAIL(head, elm, field) do {			\
	(elm)->field.cqe_next = CIRCLEQ_END(head);			\
	(elm)->field.cqe_prev = (head)->cqh_last;			\
	if ((head)->cqh_first == CIRCLEQ_END(head))			\
		(head)->cqh_first = (elm);				\
	else								\
		(head)->cqh_last->field.cqe_next = (elm);		\
	(head)->cqh_last = (elm);					\
} while (0)

#define	CIRCLEQ_REMOVE(head, elm, field) do {				\
	if ((elm)->field.cqe_next == CIRCLEQ_END(head))			\
		(head)->cqh_last = (elm)->field.cqe_prev;		\
	else								\
		(elm)->field.cqe_next->field.cqe_prev =			\
		    (elm)->field.cqe_prev;				\
	if ((elm)->field.cqe_prev == CIRCLEQ_END(head))			\
		(head)->cqh_first = (elm)->field.cqe_next;		\
	else								\
		(elm)->field.cqe_prev->field.cqe_next =			\
		    (elm)->field.cqe_next;				\
	_Q_INVALIDATE((elm)->field.cqe_prev);				\
	_Q_INVALIDATE((elm)->field.cqe_next);				\
} while (0)

#define CIRCLEQ_REPLACE(head, elm, elm2, field) do {			\
	if (((elm2)->field.cqe_next = (elm)->field.cqe_next) ==		\
	    CIRCLEQ_END(head))						\
		(head)->cqh_last = (elm2);				\
	else								\
		(elm2)->field.cqe_next->field.cqe_prev = (elm2);	\
	if (((elm2)->field.cqe_prev = (elm)->field.cqe_prev) ==		\
	    CIRCLEQ_END(head))						\
		(head)->cqh_first = (elm2);				\
	else								\
		(elm2)->field.cqe_prev->field.cqe_next = (elm2);	\
	_Q_INVALIDATE((elm)->field.cqe_prev);				\
	_Q_INVALIDATE((elm)->field.cqe_next);				\
} while (0)

#endif	/* !_SYS_QUEUE_H_ */
/*********************************** amalgamated original file "C:/AcpltDevelopmentKit/acplt/dev/open62541WrapperProject/open62541/src_generated/ua_transport_generated.h" ***********************************/
/**
* @file ua_transport_generated.h
*
* @brief Autogenerated data types
*
* Generated from Custom.Opc.Ua.Transport.bsd with script C:/AcpltDevelopmentKit/acplt/dev/open62541WrapperProject/open62541/tools/generate_datatypes.py
* on host SPECTRE by user lars at 2015-05-06 12:57:37
*/


#ifdef __cplusplus
extern "C" {
#endif



/**
* @ingroup types
*
* @defgroup ua_transport_generated Autogenerated ua_transport Types
*
* @brief Data structures that are autogenerated from an XML-Schema.
*
* @{
*/

#define UA_TRANSPORT_COUNT 10

extern UA_EXPORT const UA_DataType *UA_TRANSPORT;


/** @brief Message Type and whether the message contains the final chunk */
typedef enum { 
    UA_MESSAGETYPEANDFINAL_ACKF = 1179337537,
    UA_MESSAGETYPEANDFINAL_CLOF = 1179601987,
    UA_MESSAGETYPEANDFINAL_HELF = 1179403592,
    UA_MESSAGETYPEANDFINAL_MSGF = 1179079501,
    UA_MESSAGETYPEANDFINAL_OPNF = 1179537487
} UA_MessageTypeAndFinal;
#define UA_TRANSPORT_MESSAGETYPEANDFINAL 0
#define UA_MessageTypeAndFinal_new (UA_MessageTypeAndFinal*)UA_Int32_new
#define UA_MessageTypeAndFinal_init(p) UA_Int32_init((UA_Int32*)p)
#define UA_MessageTypeAndFinal_delete(p) UA_Int32_delete((UA_Int32*)p)
#define UA_MessageTypeAndFinal_deleteMembers(p) UA_Int32_deleteMembers((UA_Int32*)p)
#define UA_MessageTypeAndFinal_copy(src, dst) UA_Int32_copy((const UA_Int32*)src, (UA_Int32*)dst)
#define UA_MessageTypeAndFinal_calcSizeBinary(p) UA_Int32_calcSizeBinary((UA_Int32*)p)
#define UA_MessageTypeAndFinal_encodeBinary(src, dst, offset) UA_Int32_encodeBinary((UA_Int32*)src, dst, offset)
#define UA_MessageTypeAndFinal_decodeBinary(src, offset, dst) UA_Int32_decodeBinary(src, offset, (UA_Int32*)dst)

/** @brief TCP Header */
typedef struct {
    UA_UInt32 messageTypeAndFinal;
    UA_UInt32 messageSize;
} UA_TcpMessageHeader;
#define UA_TRANSPORT_TCPMESSAGEHEADER 1
#define UA_TcpMessageHeader_new() (UA_TcpMessageHeader*)UA_new(&UA_TRANSPORT[UA_TRANSPORT_TCPMESSAGEHEADER])
#define UA_TcpMessageHeader_init(p) UA_init(p, &UA_TRANSPORT[UA_TRANSPORT_TCPMESSAGEHEADER])
#define UA_TcpMessageHeader_delete(p) UA_delete(p, &UA_TRANSPORT[UA_TRANSPORT_TCPMESSAGEHEADER])
#define UA_TcpMessageHeader_deleteMembers(p) UA_deleteMembers(p, &UA_TRANSPORT[UA_TRANSPORT_TCPMESSAGEHEADER])
#define UA_TcpMessageHeader_copy(src, dst) UA_copy(src, dst, &UA_TRANSPORT[UA_TRANSPORT_TCPMESSAGEHEADER])
#define UA_TcpMessageHeader_calcSizeBinary(p) UA_calcSizeBinary(p, &UA_TRANSPORT[UA_TRANSPORT_TCPMESSAGEHEADER])
#define UA_TcpMessageHeader_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TRANSPORT[UA_TRANSPORT_TCPMESSAGEHEADER], dst, offset)
#define UA_TcpMessageHeader_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TRANSPORT[UA_TRANSPORT_TCPMESSAGEHEADER])

/** @brief Hello Message */
typedef struct {
    UA_UInt32 protocolVersion;
    UA_UInt32 receiveBufferSize;
    UA_UInt32 sendBufferSize;
    UA_UInt32 maxMessageSize;
    UA_UInt32 maxChunkCount;
    UA_String endpointUrl;
} UA_TcpHelloMessage;
#define UA_TRANSPORT_TCPHELLOMESSAGE 2
#define UA_TcpHelloMessage_new() (UA_TcpHelloMessage*)UA_new(&UA_TRANSPORT[UA_TRANSPORT_TCPHELLOMESSAGE])
#define UA_TcpHelloMessage_init(p) UA_init(p, &UA_TRANSPORT[UA_TRANSPORT_TCPHELLOMESSAGE])
#define UA_TcpHelloMessage_delete(p) UA_delete(p, &UA_TRANSPORT[UA_TRANSPORT_TCPHELLOMESSAGE])
#define UA_TcpHelloMessage_deleteMembers(p) UA_deleteMembers(p, &UA_TRANSPORT[UA_TRANSPORT_TCPHELLOMESSAGE])
#define UA_TcpHelloMessage_copy(src, dst) UA_copy(src, dst, &UA_TRANSPORT[UA_TRANSPORT_TCPHELLOMESSAGE])
#define UA_TcpHelloMessage_calcSizeBinary(p) UA_calcSizeBinary(p, &UA_TRANSPORT[UA_TRANSPORT_TCPHELLOMESSAGE])
#define UA_TcpHelloMessage_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TRANSPORT[UA_TRANSPORT_TCPHELLOMESSAGE], dst, offset)
#define UA_TcpHelloMessage_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TRANSPORT[UA_TRANSPORT_TCPHELLOMESSAGE])

/** @brief Acknowledge Message */
typedef struct {
    UA_UInt32 protocolVersion;
    UA_UInt32 receiveBufferSize;
    UA_UInt32 sendBufferSize;
    UA_UInt32 maxMessageSize;
    UA_UInt32 maxChunkCount;
} UA_TcpAcknowledgeMessage;
#define UA_TRANSPORT_TCPACKNOWLEDGEMESSAGE 3
#define UA_TcpAcknowledgeMessage_new() (UA_TcpAcknowledgeMessage*)UA_new(&UA_TRANSPORT[UA_TRANSPORT_TCPACKNOWLEDGEMESSAGE])
#define UA_TcpAcknowledgeMessage_init(p) UA_init(p, &UA_TRANSPORT[UA_TRANSPORT_TCPACKNOWLEDGEMESSAGE])
#define UA_TcpAcknowledgeMessage_delete(p) UA_delete(p, &UA_TRANSPORT[UA_TRANSPORT_TCPACKNOWLEDGEMESSAGE])
#define UA_TcpAcknowledgeMessage_deleteMembers(p) UA_deleteMembers(p, &UA_TRANSPORT[UA_TRANSPORT_TCPACKNOWLEDGEMESSAGE])
#define UA_TcpAcknowledgeMessage_copy(src, dst) UA_copy(src, dst, &UA_TRANSPORT[UA_TRANSPORT_TCPACKNOWLEDGEMESSAGE])
#define UA_TcpAcknowledgeMessage_calcSizeBinary(p) UA_calcSizeBinary(p, &UA_TRANSPORT[UA_TRANSPORT_TCPACKNOWLEDGEMESSAGE])
#define UA_TcpAcknowledgeMessage_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TRANSPORT[UA_TRANSPORT_TCPACKNOWLEDGEMESSAGE], dst, offset)
#define UA_TcpAcknowledgeMessage_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TRANSPORT[UA_TRANSPORT_TCPACKNOWLEDGEMESSAGE])

/** @brief Secure Layer Sequence Header */
typedef struct {
    UA_TcpMessageHeader messageHeader;
    UA_UInt32 secureChannelId;
} UA_SecureConversationMessageHeader;
#define UA_TRANSPORT_SECURECONVERSATIONMESSAGEHEADER 4
#define UA_SecureConversationMessageHeader_new() (UA_SecureConversationMessageHeader*)UA_new(&UA_TRANSPORT[UA_TRANSPORT_SECURECONVERSATIONMESSAGEHEADER])
#define UA_SecureConversationMessageHeader_init(p) UA_init(p, &UA_TRANSPORT[UA_TRANSPORT_SECURECONVERSATIONMESSAGEHEADER])
#define UA_SecureConversationMessageHeader_delete(p) UA_delete(p, &UA_TRANSPORT[UA_TRANSPORT_SECURECONVERSATIONMESSAGEHEADER])
#define UA_SecureConversationMessageHeader_deleteMembers(p) UA_deleteMembers(p, &UA_TRANSPORT[UA_TRANSPORT_SECURECONVERSATIONMESSAGEHEADER])
#define UA_SecureConversationMessageHeader_copy(src, dst) UA_copy(src, dst, &UA_TRANSPORT[UA_TRANSPORT_SECURECONVERSATIONMESSAGEHEADER])
#define UA_SecureConversationMessageHeader_calcSizeBinary(p) UA_calcSizeBinary(p, &UA_TRANSPORT[UA_TRANSPORT_SECURECONVERSATIONMESSAGEHEADER])
#define UA_SecureConversationMessageHeader_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TRANSPORT[UA_TRANSPORT_SECURECONVERSATIONMESSAGEHEADER], dst, offset)
#define UA_SecureConversationMessageHeader_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TRANSPORT[UA_TRANSPORT_SECURECONVERSATIONMESSAGEHEADER])

/** @brief Security Header */
typedef struct {
    UA_ByteString securityPolicyUri;
    UA_ByteString senderCertificate;
    UA_ByteString receiverCertificateThumbprint;
} UA_AsymmetricAlgorithmSecurityHeader;
#define UA_TRANSPORT_ASYMMETRICALGORITHMSECURITYHEADER 5
#define UA_AsymmetricAlgorithmSecurityHeader_new() (UA_AsymmetricAlgorithmSecurityHeader*)UA_new(&UA_TRANSPORT[UA_TRANSPORT_ASYMMETRICALGORITHMSECURITYHEADER])
#define UA_AsymmetricAlgorithmSecurityHeader_init(p) UA_init(p, &UA_TRANSPORT[UA_TRANSPORT_ASYMMETRICALGORITHMSECURITYHEADER])
#define UA_AsymmetricAlgorithmSecurityHeader_delete(p) UA_delete(p, &UA_TRANSPORT[UA_TRANSPORT_ASYMMETRICALGORITHMSECURITYHEADER])
#define UA_AsymmetricAlgorithmSecurityHeader_deleteMembers(p) UA_deleteMembers(p, &UA_TRANSPORT[UA_TRANSPORT_ASYMMETRICALGORITHMSECURITYHEADER])
#define UA_AsymmetricAlgorithmSecurityHeader_copy(src, dst) UA_copy(src, dst, &UA_TRANSPORT[UA_TRANSPORT_ASYMMETRICALGORITHMSECURITYHEADER])
#define UA_AsymmetricAlgorithmSecurityHeader_calcSizeBinary(p) UA_calcSizeBinary(p, &UA_TRANSPORT[UA_TRANSPORT_ASYMMETRICALGORITHMSECURITYHEADER])
#define UA_AsymmetricAlgorithmSecurityHeader_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TRANSPORT[UA_TRANSPORT_ASYMMETRICALGORITHMSECURITYHEADER], dst, offset)
#define UA_AsymmetricAlgorithmSecurityHeader_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TRANSPORT[UA_TRANSPORT_ASYMMETRICALGORITHMSECURITYHEADER])

/** @brief Secure Layer Symmetric Algorithm Header */
typedef struct {
    UA_UInt32 tokenId;
} UA_SymmetricAlgorithmSecurityHeader;
#define UA_TRANSPORT_SYMMETRICALGORITHMSECURITYHEADER 6
#define UA_SymmetricAlgorithmSecurityHeader_new() (UA_SymmetricAlgorithmSecurityHeader*)UA_new(&UA_TRANSPORT[UA_TRANSPORT_SYMMETRICALGORITHMSECURITYHEADER])
#define UA_SymmetricAlgorithmSecurityHeader_init(p) UA_init(p, &UA_TRANSPORT[UA_TRANSPORT_SYMMETRICALGORITHMSECURITYHEADER])
#define UA_SymmetricAlgorithmSecurityHeader_delete(p) UA_delete(p, &UA_TRANSPORT[UA_TRANSPORT_SYMMETRICALGORITHMSECURITYHEADER])
#define UA_SymmetricAlgorithmSecurityHeader_deleteMembers(p) UA_deleteMembers(p, &UA_TRANSPORT[UA_TRANSPORT_SYMMETRICALGORITHMSECURITYHEADER])
#define UA_SymmetricAlgorithmSecurityHeader_copy(src, dst) UA_copy(src, dst, &UA_TRANSPORT[UA_TRANSPORT_SYMMETRICALGORITHMSECURITYHEADER])
#define UA_SymmetricAlgorithmSecurityHeader_calcSizeBinary(p) UA_calcSizeBinary(p, &UA_TRANSPORT[UA_TRANSPORT_SYMMETRICALGORITHMSECURITYHEADER])
#define UA_SymmetricAlgorithmSecurityHeader_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TRANSPORT[UA_TRANSPORT_SYMMETRICALGORITHMSECURITYHEADER], dst, offset)
#define UA_SymmetricAlgorithmSecurityHeader_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TRANSPORT[UA_TRANSPORT_SYMMETRICALGORITHMSECURITYHEADER])

/** @brief Secure Layer Sequence Header */
typedef struct {
    UA_UInt32 sequenceNumber;
    UA_UInt32 requestId;
} UA_SequenceHeader;
#define UA_TRANSPORT_SEQUENCEHEADER 7
#define UA_SequenceHeader_new() (UA_SequenceHeader*)UA_new(&UA_TRANSPORT[UA_TRANSPORT_SEQUENCEHEADER])
#define UA_SequenceHeader_init(p) UA_init(p, &UA_TRANSPORT[UA_TRANSPORT_SEQUENCEHEADER])
#define UA_SequenceHeader_delete(p) UA_delete(p, &UA_TRANSPORT[UA_TRANSPORT_SEQUENCEHEADER])
#define UA_SequenceHeader_deleteMembers(p) UA_deleteMembers(p, &UA_TRANSPORT[UA_TRANSPORT_SEQUENCEHEADER])
#define UA_SequenceHeader_copy(src, dst) UA_copy(src, dst, &UA_TRANSPORT[UA_TRANSPORT_SEQUENCEHEADER])
#define UA_SequenceHeader_calcSizeBinary(p) UA_calcSizeBinary(p, &UA_TRANSPORT[UA_TRANSPORT_SEQUENCEHEADER])
#define UA_SequenceHeader_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TRANSPORT[UA_TRANSPORT_SEQUENCEHEADER], dst, offset)
#define UA_SequenceHeader_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TRANSPORT[UA_TRANSPORT_SEQUENCEHEADER])

/** @brief Secure Conversation Message Footer */
typedef struct {
    UA_Int32 paddingSize;
    UA_Byte *padding;
    UA_Byte signature;
} UA_SecureConversationMessageFooter;
#define UA_TRANSPORT_SECURECONVERSATIONMESSAGEFOOTER 8
#define UA_SecureConversationMessageFooter_new() (UA_SecureConversationMessageFooter*)UA_new(&UA_TRANSPORT[UA_TRANSPORT_SECURECONVERSATIONMESSAGEFOOTER])
#define UA_SecureConversationMessageFooter_init(p) UA_init(p, &UA_TRANSPORT[UA_TRANSPORT_SECURECONVERSATIONMESSAGEFOOTER])
#define UA_SecureConversationMessageFooter_delete(p) UA_delete(p, &UA_TRANSPORT[UA_TRANSPORT_SECURECONVERSATIONMESSAGEFOOTER])
#define UA_SecureConversationMessageFooter_deleteMembers(p) UA_deleteMembers(p, &UA_TRANSPORT[UA_TRANSPORT_SECURECONVERSATIONMESSAGEFOOTER])
#define UA_SecureConversationMessageFooter_copy(src, dst) UA_copy(src, dst, &UA_TRANSPORT[UA_TRANSPORT_SECURECONVERSATIONMESSAGEFOOTER])
#define UA_SecureConversationMessageFooter_calcSizeBinary(p) UA_calcSizeBinary(p, &UA_TRANSPORT[UA_TRANSPORT_SECURECONVERSATIONMESSAGEFOOTER])
#define UA_SecureConversationMessageFooter_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TRANSPORT[UA_TRANSPORT_SECURECONVERSATIONMESSAGEFOOTER], dst, offset)
#define UA_SecureConversationMessageFooter_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TRANSPORT[UA_TRANSPORT_SECURECONVERSATIONMESSAGEFOOTER])

/** @brief Secure Conversation Message Abort Body */
typedef struct {
    UA_UInt32 error;
    UA_String reason;
} UA_SecureConversationMessageAbortBody;
#define UA_TRANSPORT_SECURECONVERSATIONMESSAGEABORTBODY 9
#define UA_SecureConversationMessageAbortBody_new() (UA_SecureConversationMessageAbortBody*)UA_new(&UA_TRANSPORT[UA_TRANSPORT_SECURECONVERSATIONMESSAGEABORTBODY])
#define UA_SecureConversationMessageAbortBody_init(p) UA_init(p, &UA_TRANSPORT[UA_TRANSPORT_SECURECONVERSATIONMESSAGEABORTBODY])
#define UA_SecureConversationMessageAbortBody_delete(p) UA_delete(p, &UA_TRANSPORT[UA_TRANSPORT_SECURECONVERSATIONMESSAGEABORTBODY])
#define UA_SecureConversationMessageAbortBody_deleteMembers(p) UA_deleteMembers(p, &UA_TRANSPORT[UA_TRANSPORT_SECURECONVERSATIONMESSAGEABORTBODY])
#define UA_SecureConversationMessageAbortBody_copy(src, dst) UA_copy(src, dst, &UA_TRANSPORT[UA_TRANSPORT_SECURECONVERSATIONMESSAGEABORTBODY])
#define UA_SecureConversationMessageAbortBody_calcSizeBinary(p) UA_calcSizeBinary(p, &UA_TRANSPORT[UA_TRANSPORT_SECURECONVERSATIONMESSAGEABORTBODY])
#define UA_SecureConversationMessageAbortBody_encodeBinary(src, dst, offset) UA_encodeBinary(src, &UA_TRANSPORT[UA_TRANSPORT_SECURECONVERSATIONMESSAGEABORTBODY], dst, offset)
#define UA_SecureConversationMessageAbortBody_decodeBinary(src, offset, dst) UA_decodeBinary(src, offset, dst, &UA_TRANSPORT[UA_TRANSPORT_SECURECONVERSATIONMESSAGEABORTBODY])

/// @} /* end of group */

#ifdef __cplusplus
} // extern "C"
#endif

/*********************************** amalgamated original file "C:/AcpltDevelopmentKit/acplt/dev/open62541WrapperProject/open62541/src/ua_types_encoding_binary.h" ***********************************/


/**
 * @ingroup types
 *
 * @defgroup encoding_binary Binary Encoding
 *
 * @brief Functions for binary en- and decoding of built-in datatypes as defined
 * in the standard. The encoding of the remaining datatypes is autogenerated
 * from XML descriptions.
 *
 * All datatypes have similar functions with a common postfix. DO NOT CALL THESE
 * FUNCTIONS WITH NULL-POINTERS IF IT IS NOT EXPLICITLY PERMITTED.
 *
 * - _calcSize: Returns the size of the (encoded) variable in bytes. This length
 *   is used to allocate the bytestring for later encoding.
 *
 * - _encode: Encodes a variable into a bytestring. If an error occurs
 *   (indicated by the return value), the bytestring may be left in an
 *   inconsistent state.
 *
 * - _decode: Decodes a variable stored in a bytestring. The destination is
 *   cleaned up (init) before decoding into it. If an error occurs (indicated by
 *   the return value), the destination is cleaned up (deleteMembers, but no
 *   init) before returning.
 *
 * @{
 */

#define UA_TYPE_BINARY_ENCODING(TYPE)                                   \
    size_t TYPE##_calcSizeBinary(TYPE const *p);                        \
    UA_StatusCode TYPE##_encodeBinary(TYPE const *src, UA_ByteString *dst, size_t *restrict offset); \
    UA_StatusCode TYPE##_decodeBinary(UA_ByteString const *src, size_t *restrict offset, TYPE *dst);

UA_TYPE_BINARY_ENCODING(UA_Boolean)
UA_TYPE_BINARY_ENCODING(UA_SByte)
UA_TYPE_BINARY_ENCODING(UA_Byte)
UA_TYPE_BINARY_ENCODING(UA_Int16)
UA_TYPE_BINARY_ENCODING(UA_UInt16)
UA_TYPE_BINARY_ENCODING(UA_Int32)
UA_TYPE_BINARY_ENCODING(UA_UInt32)
UA_TYPE_BINARY_ENCODING(UA_Int64)
UA_TYPE_BINARY_ENCODING(UA_UInt64)
UA_TYPE_BINARY_ENCODING(UA_Float)
UA_TYPE_BINARY_ENCODING(UA_Double)
UA_TYPE_BINARY_ENCODING(UA_String)
UA_TYPE_BINARY_ENCODING(UA_DateTime)
UA_TYPE_BINARY_ENCODING(UA_Guid)
UA_TYPE_BINARY_ENCODING(UA_ByteString)
UA_TYPE_BINARY_ENCODING(UA_XmlElement)
UA_TYPE_BINARY_ENCODING(UA_NodeId)
UA_TYPE_BINARY_ENCODING(UA_ExpandedNodeId)
UA_TYPE_BINARY_ENCODING(UA_StatusCode)
UA_TYPE_BINARY_ENCODING(UA_QualifiedName)
UA_TYPE_BINARY_ENCODING(UA_LocalizedText)
UA_TYPE_BINARY_ENCODING(UA_ExtensionObject)
UA_TYPE_BINARY_ENCODING(UA_DataValue)
UA_TYPE_BINARY_ENCODING(UA_Variant)
UA_TYPE_BINARY_ENCODING(UA_DiagnosticInfo)

size_t UA_calcSizeBinary(const void *p, const UA_DataType *dataType);
UA_StatusCode UA_encodeBinary(const void *src, const UA_DataType *dataType, UA_ByteString *dst,
                              size_t *restrict offset);
UA_StatusCode UA_decodeBinary(const UA_ByteString *src, size_t *restrict offset, void *dst,
                              const UA_DataType *dataType);
/// @} /* end of group */

/*********************************** amalgamated original file "C:/AcpltDevelopmentKit/acplt/dev/open62541WrapperProject/open62541/src/ua_securechannel.h" ***********************************/


/**
 *  @ingroup communication
 *
 * @{
 */

struct UA_Session;
typedef struct UA_Session UA_Session;

struct SessionEntry {
    LIST_ENTRY(SessionEntry) pointers;
    UA_Session *session; // Just a pointer. The session is held in the session manager or the client
};

struct UA_SecureChannel {
    UA_MessageSecurityMode  securityMode;
    UA_ChannelSecurityToken securityToken; // the channelId is contained in the securityToken
    UA_AsymmetricAlgorithmSecurityHeader clientAsymAlgSettings;
    UA_AsymmetricAlgorithmSecurityHeader serverAsymAlgSettings;
    UA_ByteString  clientNonce;
    UA_ByteString  serverNonce;
    UA_UInt32      requestId;
    UA_UInt32      sequenceNumber;
    UA_Connection *connection;
    LIST_HEAD(session_pointerlist, SessionEntry) sessions;
};

void UA_SecureChannel_init(UA_SecureChannel *channel);
void UA_SecureChannel_deleteMembersCleanup(UA_SecureChannel *channel);

UA_StatusCode UA_SecureChannel_generateNonce(UA_ByteString *nonce);
UA_StatusCode UA_SecureChannel_updateRequestId(UA_SecureChannel *channel, UA_UInt32 requestId);
UA_StatusCode UA_SecureChannel_updateSequenceNumber(UA_SecureChannel *channel, UA_UInt32 sequenceNumber);

void UA_SecureChannel_attachSession(UA_SecureChannel *channel, UA_Session *session);
void UA_SecureChannel_detachSession(UA_SecureChannel *channel, UA_Session *session);
UA_Session * UA_SecureChannel_getSession(UA_SecureChannel *channel, UA_NodeId *token);

/** @} */

/*********************************** amalgamated original file "C:/AcpltDevelopmentKit/acplt/dev/open62541WrapperProject/open62541/src/ua_session.h" ***********************************/


#define MAXCONTINUATIONPOINTS 5

/**
 *  @ingroup communication
 *
 * @{
 */

struct ContinuationPointEntry {
    LIST_ENTRY(ContinuationPointEntry) pointers;
    UA_ByteString        identifier;
    UA_BrowseDescription browseDescription;
    UA_Int32            continuationIndex;
    UA_UInt32            maxReferences;
};

struct UA_Session {
    UA_ApplicationDescription clientDescription;
    UA_Boolean        activated;
    UA_String         sessionName;
    UA_NodeId         authenticationToken;
    UA_NodeId         sessionId;
    UA_UInt32         maxRequestMessageSize;
    UA_UInt32         maxResponseMessageSize;
    UA_Int64          timeout;
    UA_DateTime       validTill;
    UA_SecureChannel *channel;
    UA_UInt16 availableContinuationPoints;
    LIST_HEAD(ContinuationPointList, ContinuationPointEntry) continuationPoints;
};

extern UA_Session anonymousSession; ///< If anonymous access is allowed, this session is used internally (Session ID: 0)
extern UA_Session adminSession; ///< Local access to the services (for startup and maintenance) uses this Session with all possible access rights (Session ID: 1)

void UA_Session_init(UA_Session *session);
void UA_Session_deleteMembersCleanup(UA_Session *session);

/** If any activity on a session happens, the timeout is extended */
void UA_Session_updateLifetime(UA_Session *session);

/** @} */

/*********************************** amalgamated original file "C:/AcpltDevelopmentKit/acplt/dev/open62541WrapperProject/open62541/src/server/ua_nodes.h" ***********************************/


#define UA_STANDARD_NODEMEMBERS                 \
    UA_NodeId nodeId;                           \
    UA_NodeClass nodeClass;                     \
    UA_QualifiedName browseName;                \
    UA_LocalizedText displayName;               \
    UA_LocalizedText description;               \
    UA_UInt32 writeMask;                        \
    UA_UInt32 userWriteMask;                    \
    UA_Int32 referencesSize;                    \
    UA_ReferenceNode *references;

typedef struct {
    UA_STANDARD_NODEMEMBERS
} UA_Node;

typedef struct {
    UA_STANDARD_NODEMEMBERS
    UA_Byte eventNotifier;
} UA_ObjectNode;
UA_TYPE_HANDLING_FUNCTIONS(UA_ObjectNode)

typedef struct {
    UA_STANDARD_NODEMEMBERS
    UA_Boolean isAbstract;
} UA_ObjectTypeNode;
UA_TYPE_HANDLING_FUNCTIONS(UA_ObjectTypeNode)

typedef enum {
    UA_VALUESOURCE_VARIANT,
    UA_VALUESOURCE_DATASOURCE
} UA_ValueSource;

typedef struct {
    UA_STANDARD_NODEMEMBERS
    UA_Int32 valueRank; /**< n >= 1: the value is an array with the specified number of dimensions.
                             n = 0: the value is an array with one or more dimensions.
                             n = -1: the value is a scalar.
                             n = -2: the value can be a scalar or an array with any number of dimensions.
                             n = -3:  the value can be a scalar or a one dimensional array. */
    UA_ValueSource valueSource;
    union {
        UA_Variant variant;
        UA_DataSource dataSource;
    } value;
    /* <--- similar to variabletypenodes up to there--->*/
    UA_Byte accessLevel;
    UA_Byte userAccessLevel;
    UA_Double minimumSamplingInterval;
    UA_Boolean historizing;
} UA_VariableNode;
UA_TYPE_HANDLING_FUNCTIONS(UA_VariableNode)
/** Make a copy but leave out the references and the variable */
UA_StatusCode UA_VariableNode_copyWithoutRefsAndVariable(const UA_VariableNode *src, UA_VariableNode *dst);

typedef struct {
    UA_STANDARD_NODEMEMBERS
    UA_Int32 valueRank;
    UA_ValueSource valueSource;
    union {
        UA_Variant variant;
        UA_DataSource dataSource;
    } value;
    /* <--- similar to variablenodes up to there--->*/
    UA_Boolean isAbstract;
} UA_VariableTypeNode;
UA_TYPE_HANDLING_FUNCTIONS(UA_VariableTypeNode)

typedef struct {
    UA_STANDARD_NODEMEMBERS
    UA_Boolean isAbstract;
    UA_Boolean symmetric;
    UA_LocalizedText inverseName;
} UA_ReferenceTypeNode;
UA_TYPE_HANDLING_FUNCTIONS(UA_ReferenceTypeNode)

typedef struct {
    UA_STANDARD_NODEMEMBERS
    UA_Boolean executable;
    UA_Boolean userExecutable;
} UA_MethodNode;
UA_TYPE_HANDLING_FUNCTIONS(UA_MethodNode)

typedef struct {
    UA_STANDARD_NODEMEMBERS
    UA_Boolean containsNoLoops;
    UA_Byte eventNotifier;
} UA_ViewNode;
UA_TYPE_HANDLING_FUNCTIONS(UA_ViewNode)

typedef struct {
    UA_STANDARD_NODEMEMBERS
    UA_Boolean isAbstract;
} UA_DataTypeNode;
UA_TYPE_HANDLING_FUNCTIONS(UA_DataTypeNode)

/*********************************** amalgamated original file "C:/AcpltDevelopmentKit/acplt/dev/open62541WrapperProject/open62541/src/server/ua_nodestore.h" ***********************************/


/**
 * @ingroup server
 *
 * @defgroup nodestore NodeStore
 *
 * @brief Stores the nodes in the address space. Internally, it is based on a
 * hash-map that maps nodes to their nodeid.
 *
 * Nodes need to be allocated on the heap before adding them to the nodestore
 * with. When adding, the node is copied to a new (managed) location in the
 * nodestore and the original memory is freed. The nodes in the nodestore are
 * immutable. To change the content of a node, it needs to be replaced as a
 * whole.
 *
 * Every node you _get from the nodestore needs to be _released when it is no
 * longer needed. In the background, reference counting is used to know if
 * somebody still uses the node in multi-threaded environments.
 *
 * @{
 */

struct UA_NodeStore;
typedef struct UA_NodeStore UA_NodeStore;

/** Create a new namespace */
UA_NodeStore * UA_NodeStore_new(void);

/** Delete the namespace and all nodes in it */
void UA_NodeStore_delete(UA_NodeStore *ns);

/**
 * Inserts a new node into the namespace. If the nodeid is zero, then a fresh
 * numeric nodeid from namespace 1 is assigned. The memory of the original node
 * is freed and the content is moved to a managed (immutable) node. If inserted
 * is not NULL, then a pointer to the managed node is returned (and must be
 * released).
 */
UA_StatusCode UA_NodeStore_insert(UA_NodeStore *ns, UA_Node *node, const UA_Node **inserted);

/**
 * Replace an existing node in the nodestore. If the node was already replaced,
 * UA_STATUSCODE_BADINTERNALERROR is returned. If inserted is not NULL, a
 * pointer to the managed (immutable) node is returned.
 */
UA_StatusCode UA_NodeStore_replace(UA_NodeStore *ns, const UA_Node *oldNode, UA_Node *node, const UA_Node **inserted);

/**
 * Remove a node from the namespace. Always succeeds, even if the node was not
 * found.
 */
UA_StatusCode UA_NodeStore_remove(UA_NodeStore *ns, const UA_NodeId *nodeid);

/**
 * Retrieve a node (read-only) from the namespace. Nodes are immutable. They
 * can only be replaced. After the Node is no longer used, the locked entry
 * needs to be released.
 */
const UA_Node * UA_NodeStore_get(const UA_NodeStore *ns, const UA_NodeId *nodeid);

/**
 * Release a managed node. Do never insert a node that isn't stored in a
 * namespace.
 */
void UA_NodeStore_release(const UA_Node *managed);

/**
 * A function that can be evaluated on all entries in a namespace via
 * UA_NodeStore_iterate. Note that the visitor is read-only on the nodes.
 */
typedef void (*UA_NodeStore_nodeVisitor)(const UA_Node *node);

/** Iterate over all nodes in a namespace. */
void UA_NodeStore_iterate(const UA_NodeStore *ns, UA_NodeStore_nodeVisitor visitor);

/** @} */

/*********************************** amalgamated original file "C:/AcpltDevelopmentKit/acplt/dev/open62541WrapperProject/open62541/src/server/ua_session_manager.h" ***********************************/


typedef struct session_list_entry {
    LIST_ENTRY(session_list_entry) pointers;
    UA_Session session;
} session_list_entry;

typedef struct UA_SessionManager {
    LIST_HEAD(session_list, session_list_entry) sessions; // doubly-linked list of sessions
    UA_UInt32    maxSessionCount;
    UA_Int32     lastSessionId;
    UA_UInt32    currentSessionCount;
    UA_DateTime  maxSessionLifeTime;
} UA_SessionManager;

UA_StatusCode UA_SessionManager_init(UA_SessionManager *sessionManager, UA_UInt32 maxSessionCount,
                                    UA_UInt32 maxSessionLifeTime, UA_UInt32 startSessionId);

void UA_SessionManager_deleteMembers(UA_SessionManager *sessionManager);

void UA_SessionManager_cleanupTimedOut(UA_SessionManager *sessionManager, UA_DateTime now);

UA_StatusCode UA_SessionManager_createSession(UA_SessionManager *sessionManager,
                                              UA_SecureChannel *channel, const UA_CreateSessionRequest *request,
                                              UA_Session **session);

UA_StatusCode UA_SessionManager_removeSession(UA_SessionManager *sessionManager, const UA_NodeId *token);

/** Finds the session which is identified by the authentication token */
UA_Session * UA_SessionManager_getSession(UA_SessionManager *sessionManager, const UA_NodeId *token);

/*********************************** amalgamated original file "C:/AcpltDevelopmentKit/acplt/dev/open62541WrapperProject/open62541/src/server/ua_securechannel_manager.h" ***********************************/


typedef struct channel_list_entry {
    UA_SecureChannel channel;
    LIST_ENTRY(channel_list_entry) pointers;
} channel_list_entry;

typedef struct UA_SecureChannelManager {
    LIST_HEAD(channel_list, channel_list_entry) channels; // doubly-linked list of channels
    UA_Int32    maxChannelCount;
    UA_DateTime maxChannelLifetime;
    UA_MessageSecurityMode securityMode;
    UA_DateTime channelLifeTime;
    UA_Int32    lastChannelId;
    UA_UInt32   lastTokenId;
} UA_SecureChannelManager;

UA_StatusCode UA_SecureChannelManager_init(UA_SecureChannelManager *cm, UA_UInt32 maxChannelCount,
                                           UA_UInt32 tokenLifetime, UA_UInt32 startChannelId,
                                           UA_UInt32 startTokenId);
void UA_SecureChannelManager_deleteMembers(UA_SecureChannelManager *cm);
void UA_SecureChannelManager_cleanupTimedOut(UA_SecureChannelManager *cm, UA_DateTime now);
UA_StatusCode UA_SecureChannelManager_open(UA_SecureChannelManager *cm, UA_Connection *conn,
                                           const UA_OpenSecureChannelRequest *request,
                                           UA_OpenSecureChannelResponse *response);
UA_StatusCode UA_SecureChannelManager_renew(UA_SecureChannelManager *cm, UA_Connection *conn,
                                            const UA_OpenSecureChannelRequest *request,
                                            UA_OpenSecureChannelResponse *response);
UA_SecureChannel * UA_SecureChannelManager_get(UA_SecureChannelManager *cm, UA_UInt32 channelId);
UA_StatusCode UA_SecureChannelManager_close(UA_SecureChannelManager *cm, UA_UInt32 channelId);

/*********************************** amalgamated original file "C:/AcpltDevelopmentKit/acplt/dev/open62541WrapperProject/open62541/src/server/ua_server_internal.h" ***********************************/


#define PRODUCT_URI "http://open62541.org"
#define ANONYMOUS_POLICY "open62541-anonymous-policy"
#define USERNAME_POLICY "open62541-username-policy"

/** Mapping of namespace-id and url to an external nodestore. For namespaces
    that have no mapping defined, the internal nodestore is used by default. */
typedef struct UA_ExternalNamespace {
	UA_UInt16 index;
	UA_String url;
	UA_ExternalNodeStore externalNodeStore;
} UA_ExternalNamespace;

// forward declarations
struct TimedWork;

struct DelayedWork;

struct UA_Server {
    /* Config */
    UA_ServerConfig config;
    UA_Logger logger;
    UA_UInt32 random_seed;

    /* Meta */
    UA_DateTime startTime;
    UA_DateTime buildDate;
    UA_ApplicationDescription description;
    UA_Int32 endpointDescriptionsSize;
    UA_EndpointDescription *endpointDescriptions;

    /* Communication */
    size_t networkLayersSize;
    UA_ServerNetworkLayer *networkLayers;

    /* Security */
    UA_ByteString serverCertificate;
    UA_SecureChannelManager secureChannelManager;
    UA_SessionManager sessionManager;

    /* Address Space */
    UA_NodeStore *nodestore;
    size_t namespacesSize;
    UA_String *namespaces;
    size_t externalNamespacesSize;
    UA_ExternalNamespace *externalNamespaces;

    /* Workload Management */
    LIST_HEAD(TimedWorkList, TimedWork) timedWork;

#ifdef UA_MULTITHREADING
    UA_Boolean *running;
    UA_UInt16 nThreads;
    UA_UInt32 **workerCounters;
    pthread_t *thr;
    struct DelayedWork *delayedWork;

    // worker threads wait on the queue
	struct cds_wfcq_head dispatchQueue_head;
	struct cds_wfcq_tail dispatchQueue_tail;
    pthread_cond_t dispatchQueue_condition; // so the workers don't spin if the queue is empty
#endif
};

void UA_Server_processBinaryMessage(UA_Server *server, UA_Connection *connection, UA_ByteString *msg);

UA_AddNodesResult UA_Server_addNodeWithSession(UA_Server *server, UA_Session *session, UA_Node *node,
                                               const UA_ExpandedNodeId parentNodeId,
                                               const UA_NodeId referenceTypeId);

UA_AddNodesResult UA_Server_addNode(UA_Server *server, UA_Node *node, const UA_ExpandedNodeId parentNodeId,
                                    const UA_NodeId referenceTypeId);

UA_StatusCode UA_Server_addReferenceWithSession(UA_Server *server, UA_Session *session, const UA_AddReferencesItem *item);

void UA_Server_deleteTimedWork(UA_Server *server);

#define ADDREFERENCE(NODEID, REFTYPE_NODEID, TARGET_EXPNODEID) do {     \
        UA_AddReferencesItem item;                                      \
        UA_AddReferencesItem_init(&item);                               \
        item.sourceNodeId = NODEID;                                     \
        item.referenceTypeId = REFTYPE_NODEID;                          \
        item.isForward = UA_TRUE;                                       \
        item.targetNodeId = TARGET_EXPNODEID;                           \
        UA_Server_addReference(server, &item);                          \
    } while(0)

/*********************************** amalgamated original file "C:/AcpltDevelopmentKit/acplt/dev/open62541WrapperProject/open62541/src/server/ua_services.h" ***********************************/


/**
 * @ingroup server
 * @defgroup services Services
 *
 * @brief The UA services that can be called from a remote user
 *
 * @{
 */

/**
 * @name Discovery Service Set
 *
 * This Service Set defines Services used to discover the Endpoints implemented
 * by a Server and to read the security configuration for those Endpoints.
 *
 * @{
 */
// Service_FindServers
void Service_FindServers(UA_Server                    *server,
                          const UA_FindServersRequest *request,
                          UA_FindServersResponse      *response);
/**
 * Returns the Endpoints supported by a Server and all of the configuration
 * information required to establish a SecureChannel and a Session.
 */
void Service_GetEndpoints(UA_Server *server, const UA_GetEndpointsRequest *request,
                          UA_GetEndpointsResponse *response);
// Service_RegisterServer
/** @} */

/**
 * @name SecureChannel Service Set
 *
 * This Service Set defines Services used to open a communication channel that
 * ensures the confidentiality and Integrity of all Messages exchanged with the
 * Server.
 *
 * @{
 */

/**
 * Open or renew a SecureChannel that can be used to ensure Confidentiality and
 * Integrity for Message exchange during a Session.
 */
void Service_OpenSecureChannel(UA_Server *server, UA_Connection *connection,
                               const UA_OpenSecureChannelRequest *request,
                               UA_OpenSecureChannelResponse *response);

/** Used to terminate a SecureChannel. */
void Service_CloseSecureChannel(UA_Server *server, UA_Int32 channelId);

/** @} */

/**
 * @name Session Service Set
 *
 * This Service Set defines Services for an application layer connection
 * establishment in the context of a Session.
 *
 * @{
 */

/**
 * Used by an OPC UA Client to create a Session and the Server returns two
 * values which uniquely identify the Session. The first value is the sessionId
 * which is used to identify the Session in the audit logs and in the Serverâ€™s
 * address space. The second is the authenticationToken which is used to
 * associate an incoming request with a Session.
 */
void Service_CreateSession(UA_Server *server, UA_SecureChannel *channel,
                           const UA_CreateSessionRequest *request, UA_CreateSessionResponse *response);

/**
 * Used by the Client to submit its SoftwareCertificates to the Server for
 * validation and to specify the identity of the user associated with the
 * Session. This Service request shall be issued by the Client before it issues
 * any other Service request after CreateSession. Failure to do so shall cause
 * the Server to close the Session.
 */
void Service_ActivateSession(UA_Server *server, UA_SecureChannel *channel,
                             const UA_ActivateSessionRequest *request, UA_ActivateSessionResponse *response);

/** Used to terminate a Session. */
void Service_CloseSession(UA_Server *server, UA_Session *session, const UA_CloseSessionRequest *request,
                          UA_CloseSessionResponse *response);
// Service_Cancel
/** @} */

/**
 * @name NodeManagement Service Set
 *
 * This Service Set defines Services to add and delete AddressSpace Nodes and References between
 * them. All added Nodes continue to exist in the AddressSpace even if the Client that created them
 * disconnects from the Server.
 *
 * @{
 */

/** Used to add one or more Nodes into the AddressSpace hierarchy. */
void Service_AddNodes(UA_Server *server, UA_Session *session, const UA_AddNodesRequest *request,
                      UA_AddNodesResponse *response);

/** Used to add one or more References to one or more Nodes. */
void Service_AddReferences(UA_Server *server, UA_Session *session, const UA_AddReferencesRequest *request,
                           UA_AddReferencesResponse *response);

/** Used to delete one or more Nodes from the AddressSpace. */
void Service_DeleteNodes(UA_Server *server, UA_Session *session, const UA_DeleteNodesRequest *request,
                         UA_DeleteNodesResponse *response);

/** Used to delete one or more References of a Node. */
void Service_DeleteReferences(UA_Server *server, UA_Session *session, const UA_DeleteReferencesRequest *request,
                              UA_DeleteReferencesResponse *response);

/** @} */

/**
 * @name View Service Set
 *
 * Clients use the browse Services of the View Service Set to navigate through
 * the AddressSpace or through a View which is a subset of the AddressSpace.
 *
 * @{
 */

/**
 * Used to discover the References of a specified Node. The browse can be
 * further limited by the use of a View. This Browse Service also supports a
 * primitive filtering capability.
 */
void Service_Browse(UA_Server *server, UA_Session *session, const UA_BrowseRequest *request,
                    UA_BrowseResponse *response);

/**
 * Used to request the next set of Browse or BrowseNext response information
 * that is too large to be sent in a single response. â€œToo largeâ€� in this
 * context means that the Server is not able to return a larger response or that
 * the number of results to return exceeds the maximum number of results to
 * return that was specified by the Client in the original Browse request.
 */
void Service_BrowseNext(UA_Server *server, UA_Session *session, const UA_BrowseNextRequest *request,
                        UA_BrowseNextResponse *response);

/** Used to translate textual node paths to their respective ids. */
void Service_TranslateBrowsePathsToNodeIds(UA_Server *server, UA_Session *session,
                                           const UA_TranslateBrowsePathsToNodeIdsRequest *request,
                                           UA_TranslateBrowsePathsToNodeIdsResponse *response);

// Service_RegisterNodes
void Service_RegisterNodes(UA_Server *server, UA_Session *session, const UA_RegisterNodesRequest *request,
                           UA_RegisterNodesResponse *response);
// Service_UnregisterNodes
void Service_UnregisterNodes(UA_Server *server, UA_Session *session, const UA_UnregisterNodesRequest *request,
                             UA_UnregisterNodesResponse *response);
/** @} */

/**
 * @name Query Service Set
 *
 * This Service Set is used to issue a Query to a Server. OPC UA Query is
 * generic in that it provides an underlying storage mechanism independent Query
 * capability that can be used to access a wide variety of OPC UA data stores
 * and information management systems. OPC UA Query permits a Client to access
 * data maintained by a Server without any knowledge of the logical schema used
 * for internal storage of the data. Knowledge of the AddressSpace is
 * sufficient.
 *
 * @{
 */
// Service_QueryFirst
// Service_QueryNext
/** @} */

/**
 * @name Attribute Service Set
 *
 * This Service Set provides Services to access Attributes that are part of
 * Nodes.
 *
 * @{
 */

/**
 * Used to read one or more Attributes of one or more Nodes. For constructed
 * Attribute values whose elements are indexed, such as an array, this Service
 * allows Clients to read the entire set of indexed values as a composite, to
 * read individual elements or to read ranges of elements of the composite.
 */
void Service_Read(UA_Server *server, UA_Session *session, const UA_ReadRequest *request,
                  UA_ReadResponse *response);
// Service_HistoryRead

/**
 * Used to write one or more Attributes of one or more Nodes. For constructed
 * Attribute values whose elements are indexed, such as an array, this Service
 * allows Clients to write the entire set of indexed values as a composite, to
 * write individual elements or to write ranges of elements of the composite.
 */
void Service_Write(UA_Server *server, UA_Session *session, const UA_WriteRequest *request,
                   UA_WriteResponse *response);
// Service_HistoryUpdate
/** @} */

/**
 * @name Method Service Set
 *
 * The Method Service Set defines the means to invoke methods. A method shall be
 * a component of an Object.
 *
 * @{
 */
// Service_Call
/** @} */

/**
 * @name MonitoredItem Service Set
 *
 * Clients define MonitoredItems to subscribe to data and Events. Each
 * MonitoredItem identifies the item to be monitored and the Subscription to use
 * to send Notifications. The item to be monitored may be any Node Attribute.
 *
 * @{
 */

/*
 * Used to create and add one or more MonitoredItems to a Subscription. A
 * MonitoredItem is deleted automatically by the Server when the Subscription is
 * deleted. Deleting a MonitoredItem causes its entire set of triggered item
 * links to be deleted, but has no effect on the MonitoredItems referenced by
 * the triggered items.
 */
/* UA_Int32 Service_CreateMonitoredItems(UA_Server *server, UA_Session *session, */
/*                                       const UA_CreateMonitoredItemsRequest *request, */
/*                                       UA_CreateMonitoredItemsResponse *response); */
// Service_ModifyMonitoredItems
// Service_SetMonitoringMode
// Service_SetTriggering
// Service_DeleteMonitoredItems
/** @} */

/**
 * @name Subscription Service Set
 *
 * Subscriptions are used to report Notifications to the Client.
 *
 * @{
 */
// Service_CreateSubscription
/* UA_Int32 Service_CreateSubscription(UA_Server *server, UA_Session *session, */
/*                                     const UA_CreateSubscriptionRequest *request, */
/*                                     UA_CreateSubscriptionResponse *response); */
// Service_ModifySubscription
// Service_SetPublishingMode
/* UA_Int32 Service_SetPublishingMode(UA_Server *server, UA_Session *session, */
/*                                    const UA_SetPublishingModeRequest *request, */
/*                                    UA_SetPublishingModeResponse *response); */

/* UA_Int32 Service_Publish(UA_Server *server, UA_Session *session, */
/*                          const UA_PublishRequest *request, */
/*                          UA_PublishResponse *response); */

// Service_Republish
// Service_TransferSubscription
// Service_DeleteSubscription
/** @} */
/** @} */

/*********************************** amalgamated original file "C:/AcpltDevelopmentKit/acplt/dev/open62541WrapperProject/open62541/src/server/ua_nodestore_hash.inc" ***********************************/
typedef UA_UInt32 hash_t;

static hash_t mod(hash_t h, hash_t size) { return h % size; }
static hash_t mod2(hash_t h, hash_t size) { return 1 + (h % (size - 2)); }

/* Based on Murmur-Hash 3 by Austin Appleby (public domain, freely usable) */
static hash_t hash_array(const UA_Byte *data, UA_UInt32 len, UA_UInt32 seed) {
    if(data == UA_NULL)
        return 0;

    const int32_t   nblocks = len / 4;
    const uint32_t *blocks;
    static const uint32_t c1 = 0xcc9e2d51;
    static const uint32_t c2 = 0x1b873593;
    static const uint32_t r1 = 15;
    static const uint32_t r2 = 13;
    static const uint32_t m  = 5;
    static const uint32_t n  = 0xe6546b64;
    hash_t hash = seed;
    /* Somce compilers emit a warning when casting from a byte array to ints. */
#if ((__GNUC__ == 4 && __GNUC_MINOR__ >= 6) || __GNUC__ > 4 || defined(__clang__))
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
#endif
    blocks = (const uint32_t *)data;
#if ((__GNUC__ == 4 && __GNUC_MINOR__ >= 6) || __GNUC__ > 4 || defined(__clang__))
#pragma GCC diagnostic pop
#endif
    for(int32_t i = 0;i < nblocks;i++) {
        uint32_t k = blocks[i];
        k    *= c1;
        k     = (k << r1) | (k >> (32 - r1));
        k    *= c2;
        hash ^= k;
        hash  = ((hash << r2) | (hash >> (32 - r2))) * m + n;
    }

    const uint8_t *tail = (const uint8_t *)(data + nblocks * 4);
    uint32_t       k1   = 0;
    switch(len & 3) {
    case 3:
        k1 ^= tail[2] << 16;
    case 2:
        k1 ^= tail[1] << 8;
    case 1:
        k1   ^= tail[0];
        k1   *= c1;
        k1    = (k1 << r1) | (k1 >> (32 - r1));
        k1   *= c2;
        hash ^= k1;
    }

    hash ^= len;
    hash ^= (hash >> 16);
    hash *= 0x85ebca6b;
    hash ^= (hash >> 13);
    hash *= 0xc2b2ae35;
    hash ^= (hash >> 16);
    return hash;
}

static hash_t hash(const UA_NodeId *n) {
    switch(n->identifierType) {
    case UA_NODEIDTYPE_NUMERIC:
        /*  Knuth's multiplicative hashing */
        return (n->identifier.numeric + n->namespaceIndex) * 2654435761;   // mod(2^32) is implicit
    case UA_NODEIDTYPE_STRING:
        return hash_array(n->identifier.string.data, n->identifier.string.length, n->namespaceIndex);
    case UA_NODEIDTYPE_GUID:
        return hash_array((const UA_Byte *)&(n->identifier.guid), sizeof(UA_Guid), n->namespaceIndex);
    case UA_NODEIDTYPE_BYTESTRING:
        return hash_array((const UA_Byte *)n->identifier.byteString.data, n->identifier.byteString.length, n->namespaceIndex);
    default:
        UA_assert(UA_FALSE);
        return 0;
    }
}

/*********************************** amalgamated original file "C:/AcpltDevelopmentKit/acplt/dev/open62541WrapperProject/open62541/src/ua_types.c" ***********************************/

/*****************/
/* Helper Macros */
/*****************/

#define UA_TYPE_DEFAULT(TYPE)            \
    UA_TYPE_NEW_DEFAULT(TYPE)            \
    UA_TYPE_INIT_DEFAULT(TYPE)           \
    UA_TYPE_DELETEMEMBERS_NOACTION(TYPE) \
    UA_TYPE_DELETE_DEFAULT(TYPE)         \
    UA_TYPE_COPY_DEFAULT(TYPE)           \

#define UA_TYPE_NEW_DEFAULT(TYPE)                              \
    TYPE * TYPE##_new() {                                      \
        TYPE *p = UA_malloc(sizeof(TYPE));                     \
        if(p) TYPE##_init(p);                                  \
        return p;                                              \
    }

#define UA_TYPE_INIT_DEFAULT(TYPE) \
    void TYPE##_init(TYPE * p) {   \
        *p = (TYPE)0;              \
    }

#define UA_TYPE_DELETEMEMBERS_NOACTION(TYPE) \
    void TYPE##_deleteMembers(TYPE *p) {    \
    }

#define UA_TYPE_DELETE_DEFAULT(TYPE) \
    void TYPE##_delete(TYPE *p) {    \
        TYPE##_deleteMembers(p);     \
        UA_free(p);                  \
    }

#define UA_TYPE_COPY_DEFAULT(TYPE)                             \
    UA_StatusCode TYPE##_copy(TYPE const *src, TYPE *dst) {    \
        *dst = *src;                                           \
        return UA_STATUSCODE_GOOD;                             \
    }

/*****************/
/* Builtin Types */
/*****************/

/* Boolean */
UA_TYPE_DEFAULT(UA_Boolean)

/* SByte */
UA_TYPE_DEFAULT(UA_SByte)

/* Byte */
UA_TYPE_DEFAULT(UA_Byte)

/* Int16 */
UA_TYPE_DEFAULT(UA_Int16)

/* UInt16 */
UA_TYPE_DEFAULT(UA_UInt16)

/* Int32 */
UA_TYPE_DEFAULT(UA_Int32)

/* UInt32 */
UA_TYPE_DEFAULT(UA_UInt32)

/* Int64 */
UA_TYPE_DEFAULT(UA_Int64)

/* UInt64 */
UA_TYPE_DEFAULT(UA_UInt64)

/* Float */
UA_TYPE_DEFAULT(UA_Float)

/* Double */
UA_TYPE_DEFAULT(UA_Double)

/* String */
UA_TYPE_NEW_DEFAULT(UA_String)
UA_TYPE_DELETE_DEFAULT(UA_String)

void UA_String_init(UA_String *p) {
    p->length = -1;
    p->data   = UA_NULL;
}

void UA_String_deleteMembers(UA_String *p) {
    UA_free(p->data);
    p->data = UA_NULL;
}

UA_StatusCode UA_String_copy(UA_String const *src, UA_String *dst) {
    UA_String_init(dst);
    if(src->length > 0) {
        if(!(dst->data = UA_malloc((UA_UInt32)src->length)))
            return UA_STATUSCODE_BADOUTOFMEMORY;
        UA_memcpy((void *)dst->data, src->data, (UA_UInt32)src->length);
    }
    dst->length = src->length;
    return UA_STATUSCODE_GOOD;
}

UA_String UA_String_fromChars(char const src[]) {
    UA_String str;
    size_t length = strlen(src);
    if(length == 0) {
        str.length = 0;
        str.data = UA_NULL;
        return str;
    }
    str.data = UA_malloc(length);
    if(!str.data) {
        str.length = -1;
        return str;
    }
    UA_memcpy(str.data, src, length);
    str.length = length;
    return str;
}

#define UA_STRING_ALLOCPRINTF_BUFSIZE 1024
UA_StatusCode UA_String_copyprintf(char const fmt[], UA_String *dst, ...) {
    char src[UA_STRING_ALLOCPRINTF_BUFSIZE];
    va_list ap;
    va_start(ap, dst);
#if ((__GNUC__ == 4 && __GNUC_MINOR__ >= 6) || __GNUC__ > 4 || defined(__clang__))
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-nonliteral"
#endif
    // vsnprintf should only take a literal and no variable to be secure
    UA_Int32 len = vsnprintf(src, UA_STRING_ALLOCPRINTF_BUFSIZE, fmt, ap);
#if ((__GNUC__ == 4 && __GNUC_MINOR__ >= 6) || __GNUC__ > 4 || defined(__clang__))
#pragma GCC diagnostic pop
#endif
    va_end(ap);
    if(len < 0)  // FIXME: old glibc 2.0 would return -1 when truncated
        return UA_STATUSCODE_BADINTERNALERROR;
    // since glibc 2.1 vsnprintf returns the len that would have resulted if buf were large enough
    len = ( len > UA_STRING_ALLOCPRINTF_BUFSIZE ? UA_STRING_ALLOCPRINTF_BUFSIZE : len );
    if(!(dst->data = UA_malloc((UA_UInt32)len)))
        return UA_STATUSCODE_BADOUTOFMEMORY;
    UA_memcpy((void *)dst->data, src, (UA_UInt32)len);
    dst->length = len;
    return UA_STATUSCODE_GOOD;
}

UA_Boolean UA_String_equal(const UA_String *string1, const UA_String *string2) {
    if(string1->length <= 0 && string2->length <= 0)
        return UA_TRUE;
    if(string1->length != string2->length)
        return UA_FALSE;

    // casts are needed to overcome signed warnings
    UA_Int32 is = strncmp((char const *)string1->data, (char const *)string2->data, (size_t)string1->length);
    return (is == 0) ? UA_TRUE : UA_FALSE;
}

UA_Boolean UA_String_equalchars(const UA_String *string1, char *charString) {
    UA_String string2 = UA_STRING(charString);
    return UA_String_equal(string1, &string2);
}

/* DateTime */
#define UNIX_EPOCH_BIAS_SEC 11644473600LL // Number of seconds from 1 Jan. 1601 00:00 to 1 Jan 1970 00:00 UTC
#define HUNDRED_NANOSEC_PER_USEC 10LL
#define HUNDRED_NANOSEC_PER_SEC (HUNDRED_NANOSEC_PER_USEC * 1000000LL)

#ifdef __MINGW32__
#ifndef _TIMEZONE_DEFINED
#define _TIMEZONE_DEFINED
struct timezone {
  int tz_minuteswest;
  int tz_dsttime;
};
#endif
#endif
#ifdef _WIN32
static const unsigned __int64 epoch = 116444736000000000;
int gettimeofday(struct timeval *tp, struct timezone *tzp);
int gettimeofday(struct timeval *tp, struct timezone *tzp) {
    FILETIME       ft;
    SYSTEMTIME     st;
    ULARGE_INTEGER ul;
    GetSystemTime(&st);
    SystemTimeToFileTime(&st, &ft);
    ul.LowPart  = ft.dwLowDateTime;
    ul.HighPart = ft.dwHighDateTime;
    tp->tv_sec  = (ul.QuadPart - epoch) / 10000000L;
    tp->tv_usec = st.wMilliseconds * 1000;
    return 0;
}
#endif

UA_DateTime UA_DateTime_now() {
    UA_DateTime    dateTime;
    struct timeval tv;
    gettimeofday(&tv, UA_NULL);
    dateTime = (tv.tv_sec + UNIX_EPOCH_BIAS_SEC)
               * HUNDRED_NANOSEC_PER_SEC + tv.tv_usec * HUNDRED_NANOSEC_PER_USEC;
    return dateTime;
}

UA_DateTimeStruct UA_DateTime_toStruct(UA_DateTime atime) {
    UA_DateTimeStruct dateTimeStruct;
    //calcualting the the milli-, micro- and nanoseconds
    dateTimeStruct.nanoSec  = (UA_Int16)((atime % 10) * 100);
    dateTimeStruct.microSec = (UA_Int16)((atime % 10000) / 10);
    dateTimeStruct.milliSec = (UA_Int16)((atime % 10000000) / 10000);

    //calculating the unix time with #include <time.h>
    time_t secSinceUnixEpoch = (atime/10000000) - UNIX_EPOCH_BIAS_SEC;
    struct tm ts = *gmtime(&secSinceUnixEpoch);
    dateTimeStruct.sec    = (UA_Int16)ts.tm_sec;
    dateTimeStruct.min    = (UA_Int16)ts.tm_min;
    dateTimeStruct.hour   = (UA_Int16)ts.tm_hour;
    dateTimeStruct.day    = (UA_Int16)ts.tm_mday;
    dateTimeStruct.month  = (UA_Int16)(ts.tm_mon + 1);
    dateTimeStruct.year   = (UA_Int16)(ts.tm_year + 1900);
    return dateTimeStruct;
}

UA_StatusCode UA_DateTime_toString(UA_DateTime atime, UA_String *timeString) {
    // length of the string is 31 (incl. \0 at the end)
    if(!(timeString->data = UA_malloc(32)))
        return UA_STATUSCODE_BADOUTOFMEMORY;
    timeString->length = 31;

    UA_DateTimeStruct tSt = UA_DateTime_toStruct(atime);
    sprintf((char*)timeString->data, "%02d/%02d/%04d %02d:%02d:%02d.%03d.%03d.%03d", tSt.month, tSt.day, tSt.year,
            tSt.hour, tSt.min, tSt.sec, tSt.milliSec, tSt.microSec, tSt.nanoSec);
    return UA_STATUSCODE_GOOD;
}

/* Guid */
UA_TYPE_NEW_DEFAULT(UA_Guid)
UA_TYPE_DELETEMEMBERS_NOACTION(UA_Guid)
UA_TYPE_DELETE_DEFAULT(UA_Guid)

UA_Boolean UA_Guid_equal(const UA_Guid *g1, const UA_Guid *g2) {
    if(memcmp(g1, g2, sizeof(UA_Guid)) == 0)
        return UA_TRUE;
    return UA_FALSE;
}

UA_Guid UA_Guid_random(UA_UInt32 *seed) {
    UA_Guid result;
    result.data1 = RAND(seed);
    UA_UInt32 r = RAND(seed);
    result.data2 = (UA_UInt16) r;
    result.data3 = (UA_UInt16) (r >> 16);
    r = RAND(seed);
    result.data4[0] = (UA_Byte)r;
    result.data4[1] = (UA_Byte)(r >> 4);
    result.data4[2] = (UA_Byte)(r >> 8);
    result.data4[3] = (UA_Byte)(r >> 12);
    r = RAND(seed);
    result.data4[4] = (UA_Byte)r;
    result.data4[5] = (UA_Byte)(r >> 4);
    result.data4[6] = (UA_Byte)(r >> 8);
    result.data4[7] = (UA_Byte)(r >> 12);
    return result;
}

void UA_Guid_init(UA_Guid *p) {
    p->data1 = 0;
    p->data2 = 0;
    p->data3 = 0;
    memset(p->data4, 0, sizeof(UA_Byte)*8);
}

UA_StatusCode UA_Guid_copy(UA_Guid const *src, UA_Guid *dst) {
    UA_memcpy((void *)dst, (const void *)src, sizeof(UA_Guid));
    return UA_STATUSCODE_GOOD;
}

/* ByteString */
UA_StatusCode UA_ByteString_newMembers(UA_ByteString *p, UA_Int32 length) {
    if(length > 0) {
        if(!(p->data = UA_malloc((UA_UInt32)length)))
            return UA_STATUSCODE_BADOUTOFMEMORY;
        p->length = length;
    } else {
        p->data = UA_NULL;
        if(length == 0)
            p->length = 0;
        else
            p->length = -1;
    }
    return UA_STATUSCODE_GOOD;
}

/* XmlElement */

/* NodeId */
UA_TYPE_NEW_DEFAULT(UA_NodeId)
UA_TYPE_DELETE_DEFAULT(UA_NodeId)

void UA_NodeId_init(UA_NodeId *p) {
    p->identifierType = UA_NODEIDTYPE_NUMERIC;
    p->namespaceIndex = 0;
    memset(&p->identifier, 0, sizeof(p->identifier));
}

UA_StatusCode UA_NodeId_copy(UA_NodeId const *src, UA_NodeId *dst) {
    UA_StatusCode retval = UA_STATUSCODE_GOOD;
    switch(src->identifierType) {
    case UA_NODEIDTYPE_NUMERIC:
        *dst = *src;
        return UA_STATUSCODE_GOOD;
    case UA_NODEIDTYPE_STRING: // Table 6, second entry
        retval |= UA_String_copy(&src->identifier.string, &dst->identifier.string);
        break;
    case UA_NODEIDTYPE_GUID: // Table 6, third entry
        retval |= UA_Guid_copy(&src->identifier.guid, &dst->identifier.guid);
        break;
    case UA_NODEIDTYPE_BYTESTRING: // Table 6, "OPAQUE"
        retval |= UA_ByteString_copy(&src->identifier.byteString, &dst->identifier.byteString);
        break;
    default:
        UA_NodeId_init(dst);
        return UA_STATUSCODE_BADINTERNALERROR;
    }
    dst->namespaceIndex = src->namespaceIndex;
    dst->identifierType = src->identifierType;
    if(retval) {
        UA_NodeId_deleteMembers(dst);
        UA_NodeId_init(dst);
    }
    return retval;
}

void UA_NodeId_deleteMembers(UA_NodeId *p) {
    switch(p->identifierType) {
    case UA_NODEIDTYPE_STRING:
    case UA_NODEIDTYPE_BYTESTRING:
        UA_ByteString_deleteMembers(&p->identifier.byteString);
        break;
    default:
        break;
    }
}

UA_Boolean UA_NodeId_equal(const UA_NodeId *n1, const UA_NodeId *n2) {
	if(n1->namespaceIndex != n2->namespaceIndex || n1->identifierType!=n2->identifierType)
        return UA_FALSE;

    switch(n1->identifierType) {
    case UA_NODEIDTYPE_NUMERIC:
        if(n1->identifier.numeric == n2->identifier.numeric)
            return UA_TRUE;
        else
            return UA_FALSE;

    case UA_NODEIDTYPE_STRING:
        return UA_String_equal(&n1->identifier.string, &n2->identifier.string);

    case UA_NODEIDTYPE_GUID:
        return UA_Guid_equal(&n1->identifier.guid, &n2->identifier.guid);

    case UA_NODEIDTYPE_BYTESTRING:
        return UA_ByteString_equal(&n1->identifier.byteString, &n2->identifier.byteString);
    }
    return UA_FALSE;
}

UA_Boolean UA_NodeId_isNull(const UA_NodeId *p) {
    switch(p->identifierType) {
    case UA_NODEIDTYPE_NUMERIC:
        if(p->namespaceIndex != 0 || p->identifier.numeric != 0)
            return UA_FALSE;
        break;

    case UA_NODEIDTYPE_STRING:
        if(p->namespaceIndex != 0 || p->identifier.string.length > 0)
            return UA_FALSE;
        break;

    case UA_NODEIDTYPE_GUID:
        if(p->namespaceIndex != 0 ||
           memcmp(&p->identifier.guid, (char[sizeof(UA_Guid)]) { 0 }, sizeof(UA_Guid)) != 0)
            return UA_FALSE;
        break;

    case UA_NODEIDTYPE_BYTESTRING:
        if(p->namespaceIndex != 0 || p->identifier.byteString.length > 0)
            return UA_FALSE;
        break;

    default:
        return UA_FALSE;
    }
    return UA_TRUE;
}

UA_NodeId UA_NodeId_fromInteger(UA_UInt16 nsIndex, UA_Int32 identifier) {
    return (UA_NodeId) { .namespaceIndex = nsIndex, .identifierType = UA_NODEIDTYPE_NUMERIC,
                         .identifier.numeric = identifier };
}

UA_NodeId UA_NodeId_fromCharString(UA_UInt16 nsIndex, char identifier[]) {
    return (UA_NodeId) { .namespaceIndex = nsIndex, .identifierType = UA_NODEIDTYPE_STRING,
                         .identifier.string = UA_STRING(identifier) };
}

UA_NodeId UA_NodeId_fromCharStringCopy(UA_UInt16 nsIndex, char const identifier[]) {
    return (UA_NodeId) {.namespaceIndex = nsIndex, .identifierType = UA_NODEIDTYPE_STRING,
                        .identifier.string = UA_STRING_ALLOC(identifier) };
}

UA_NodeId UA_NodeId_fromString(UA_UInt16 nsIndex, UA_String identifier) {
    return (UA_NodeId) { .namespaceIndex = nsIndex, .identifierType = UA_NODEIDTYPE_STRING,
                         .identifier.string = identifier };
}

UA_NodeId UA_NodeId_fromStringCopy(UA_UInt16 nsIndex, UA_String identifier) {
    UA_NodeId id;
    id.namespaceIndex = nsIndex;
    id.identifierType = UA_NODEIDTYPE_STRING;
    UA_String_copy(&identifier, &id.identifier.string);
    return id;
}

UA_NodeId UA_NodeId_fromGuid(UA_UInt16 nsIndex, UA_Guid identifier) {
    return (UA_NodeId) { .namespaceIndex = nsIndex, .identifierType = UA_NODEIDTYPE_GUID,
                         .identifier.guid = identifier };
}

UA_NodeId UA_NodeId_fromCharByteString(UA_UInt16 nsIndex, char identifier[]) {
    return (UA_NodeId) { .namespaceIndex = nsIndex, .identifierType = UA_NODEIDTYPE_BYTESTRING,
                         .identifier.byteString = UA_STRING(identifier) };
}

UA_NodeId UA_NodeId_fromCharByteStringCopy(UA_UInt16 nsIndex, char const identifier[]) {
    return (UA_NodeId) { .namespaceIndex = nsIndex, .identifierType = UA_NODEIDTYPE_BYTESTRING,
                         .identifier.byteString = UA_STRING_ALLOC(identifier) };
}

UA_NodeId UA_NodeId_fromByteString(UA_UInt16 nsIndex, UA_ByteString identifier) {
    return (UA_NodeId) { .namespaceIndex = nsIndex, .identifierType = UA_NODEIDTYPE_BYTESTRING,
                         .identifier.byteString = identifier };
}

UA_NodeId UA_NodeId_fromByteStringCopy(UA_UInt16 nsIndex, UA_ByteString identifier) {
    UA_NodeId id;
    id.namespaceIndex = nsIndex;
    id.identifierType = UA_NODEIDTYPE_BYTESTRING;
    UA_ByteString_copy(&identifier, &id.identifier.byteString);
    return id;
}

/* ExpandedNodeId */
UA_TYPE_NEW_DEFAULT(UA_ExpandedNodeId)
UA_TYPE_DELETE_DEFAULT(UA_ExpandedNodeId)

void UA_ExpandedNodeId_deleteMembers(UA_ExpandedNodeId *p) {
    UA_NodeId_deleteMembers(&p->nodeId);
    UA_String_deleteMembers(&p->namespaceUri);
}

void UA_ExpandedNodeId_init(UA_ExpandedNodeId *p) {
    UA_NodeId_init(&p->nodeId);
    UA_String_init(&p->namespaceUri);
    p->serverIndex = 0;
}

UA_StatusCode UA_ExpandedNodeId_copy(UA_ExpandedNodeId const *src, UA_ExpandedNodeId *dst) {
    UA_StatusCode retval = UA_NodeId_copy(&src->nodeId, &dst->nodeId);
    retval |= UA_String_copy(&src->namespaceUri, &dst->namespaceUri);
    dst->serverIndex = src->serverIndex;
    if(retval) {
        UA_ExpandedNodeId_deleteMembers(dst);
        UA_ExpandedNodeId_init(dst);
    }
    return retval;
}

UA_Boolean UA_ExpandedNodeId_isNull(const UA_ExpandedNodeId *p) {
    return UA_NodeId_isNull(&p->nodeId);
}

/* StatusCode */

/* QualifiedName */
UA_TYPE_NEW_DEFAULT(UA_QualifiedName)
UA_TYPE_DELETE_DEFAULT(UA_QualifiedName)

void UA_QualifiedName_deleteMembers(UA_QualifiedName *p) {
    UA_String_deleteMembers(&p->name);
}

void UA_QualifiedName_init(UA_QualifiedName *p) {
    UA_String_init(&p->name);
    p->namespaceIndex = 0;
}

UA_StatusCode UA_QualifiedName_copy(UA_QualifiedName const *src, UA_QualifiedName *dst) {
    UA_StatusCode retval = UA_String_copy(&src->name, &dst->name);
    dst->namespaceIndex = src->namespaceIndex;
    if(retval) {
        UA_QualifiedName_deleteMembers(dst);
        UA_QualifiedName_init(dst);
    }
    return retval;
}

/* LocalizedText */
UA_TYPE_NEW_DEFAULT(UA_LocalizedText)
UA_TYPE_DELETE_DEFAULT(UA_LocalizedText)

void UA_LocalizedText_deleteMembers(UA_LocalizedText *p) {
    UA_String_deleteMembers(&p->locale);
    UA_String_deleteMembers(&p->text);
}

void UA_LocalizedText_init(UA_LocalizedText *p) {
    UA_String_init(&p->locale);
    UA_String_init(&p->text);
}

UA_StatusCode UA_LocalizedText_copy(UA_LocalizedText const *src, UA_LocalizedText *dst) {
    UA_StatusCode retval = UA_String_copy(&src->locale, &dst->locale);
    retval |= UA_String_copy(&src->text, &dst->text);
    if(retval) {
        UA_LocalizedText_deleteMembers(dst);
        UA_LocalizedText_init(dst);
    }
    return retval;
}

/* ExtensionObject */
UA_TYPE_NEW_DEFAULT(UA_ExtensionObject)
UA_TYPE_DELETE_DEFAULT(UA_ExtensionObject)

void UA_ExtensionObject_deleteMembers(UA_ExtensionObject *p) {
    UA_NodeId_deleteMembers(&p->typeId);
    UA_ByteString_deleteMembers(&p->body);
}

void UA_ExtensionObject_init(UA_ExtensionObject *p) {
    UA_NodeId_init(&p->typeId);
    p->encoding = UA_EXTENSIONOBJECT_ENCODINGMASK_NOBODYISENCODED;
    UA_ByteString_init(&p->body);
}

UA_StatusCode UA_ExtensionObject_copy(UA_ExtensionObject const *src, UA_ExtensionObject *dst) {
    UA_StatusCode retval = UA_ByteString_copy(&src->body, &dst->body);
    retval |= UA_NodeId_copy(&src->typeId, &dst->typeId);
    dst->encoding = src->encoding;
    if(retval) {
        UA_ExtensionObject_deleteMembers(dst);
        UA_ExtensionObject_init(dst);
    }
    return retval;
}

/* DataValue */
UA_TYPE_NEW_DEFAULT(UA_DataValue)
UA_TYPE_DELETE_DEFAULT(UA_DataValue)

void UA_DataValue_deleteMembers(UA_DataValue *p) {
    UA_Variant_deleteMembers(&p->value);
}

void UA_DataValue_init(UA_DataValue *p) {
    *((UA_Byte*)p) = 0; // zero out the bitfield
    p->serverPicoseconds = 0;
    UA_DateTime_init(&p->serverTimestamp);
    p->sourcePicoseconds = 0;
    UA_DateTime_init(&p->sourceTimestamp);
    UA_StatusCode_init(&p->status);
    UA_Variant_init(&p->value);
}

UA_StatusCode UA_DataValue_copy(UA_DataValue const *src, UA_DataValue *dst) {
    UA_DataValue_init(dst);
    *((UA_Byte*)dst) = *((const UA_Byte*)src); // the bitfield
    UA_StatusCode retval = UA_DateTime_copy(&src->serverTimestamp, &dst->serverTimestamp);
    retval |= UA_DateTime_copy(&src->sourceTimestamp, &dst->sourceTimestamp);
    retval |= UA_Variant_copy(&src->value, &dst->value);
    dst->serverPicoseconds = src->serverPicoseconds;
    dst->sourcePicoseconds = src->sourcePicoseconds;
    dst->status = src->status;
    if(retval) {
        UA_DataValue_deleteMembers(dst);
        UA_DataValue_init(dst);
    }
    return retval;
}

/* Variant */
UA_TYPE_NEW_DEFAULT(UA_Variant)
UA_TYPE_DELETE_DEFAULT(UA_Variant)

void UA_Variant_init(UA_Variant *p) {
    p->storageType = UA_VARIANT_DATA;
    p->arrayLength = -1;
    p->data = UA_NULL;
    p->arrayDimensions = UA_NULL;
    p->arrayDimensionsSize = -1;
    p->type = &UA_TYPES[UA_TYPES_BOOLEAN];
}

void UA_Variant_deleteMembers(UA_Variant *p) {
    if(p->storageType != UA_VARIANT_DATA)
        return;
    if(p->data) {
        if(p->arrayLength == -1)
            p->arrayLength = 1;
        UA_Array_delete(p->data, p->type, p->arrayLength);
        p->data = UA_NULL;
        p->arrayLength = -1;
    }
    if(p->arrayDimensions) {
        UA_free(p->arrayDimensions);
        p->arrayDimensions = UA_NULL;
        p->arrayDimensionsSize = -1;
    }
}

UA_StatusCode UA_Variant_copy(UA_Variant const *src, UA_Variant *dst) {
    UA_Variant_init(dst);
    UA_Int32 elements = src->arrayLength;
    if(UA_Variant_isScalar(src))
        elements = 1;
    UA_StatusCode retval = UA_Array_copy(src->data, &dst->data, src->type, elements);
    if(retval != UA_STATUSCODE_GOOD) {
        UA_Variant_deleteMembers(dst);
        UA_Variant_init(dst);
        return retval;
    }
    dst->arrayLength = src->arrayLength;
    dst->type = src->type;
    dst->storageType = UA_VARIANT_DATA;

    if(src->arrayDimensions) {
        retval |= UA_Array_copy(src->arrayDimensions, (void **)&dst->arrayDimensions,
                                &UA_TYPES[UA_TYPES_INT32], src->arrayDimensionsSize);
        if(retval != UA_STATUSCODE_GOOD) {
            UA_Variant_deleteMembers(dst);
            UA_Variant_init(dst);
            return retval;
        }
        dst->arrayDimensionsSize = src->arrayDimensionsSize;
    }
    return retval;
}

UA_Boolean UA_Variant_isScalar(const UA_Variant *v) {
    return (v->data != UA_NULL && v->arrayLength == -1);
}

/**
 * Tests if a range is compatible with a variant. If yes, the following values are set:
 * - total: how many elements are indicated by the range
 * - block_size: how big is each contiguous block of elements in the variant denoted by the range
 * - block_distance: how many elements are between the blocks (beginning to beginning)
 * - first_elem: where does the first block begin
 */
static UA_StatusCode
testRangeWithVariant(const UA_Variant *v, const UA_NumericRange range, size_t *total,
                     size_t *block_size, size_t *block_distance, size_t *first_elem)
{
    /* Test the integrity of the source variant dimensions */
    UA_Int32 dims_count = 1;
    const UA_Int32 *dims = &v->arrayLength; // default: the array has only one dimension
    if(v->arrayDimensionsSize > 0) {
        dims_count = v->arrayDimensionsSize;
        dims = v->arrayDimensions;
        UA_Int32 elements = 1;
        for(UA_Int32 i = 0; i < dims_count; i++)
            elements *= dims[i];
        if(elements != v->arrayLength)
            return UA_STATUSCODE_BADINTERNALERROR;
    }

    /* Test the integrity of the range */
    size_t count = 1;
    if(range.dimensionsSize != dims_count)
        return UA_STATUSCODE_BADINDEXRANGEINVALID;
    for(UA_Int32 i = 0; i < dims_count; i++) {
        if(range.dimensions[i].min > range.dimensions[i].max)
            return UA_STATUSCODE_BADINDEXRANGEINVALID;
        if(range.dimensions[i].max > (UA_UInt32)*(dims+i))
            return UA_STATUSCODE_BADINDEXRANGENODATA;
        count *= (range.dimensions[i].max - range.dimensions[i].min) + 1;
    }

    /* Compute the block size and the position of the first element */
    size_t bs = 0;
    size_t bd = 0;
    size_t fe = 0;
    size_t running_dimssize = 1; // elements per block of dimensions k to k_max
    UA_Boolean found_contiguous = UA_FALSE;
    for(UA_Int32 k = dims_count - 1; k >= 0; k--) {
        if(!found_contiguous && (range.dimensions[k].min != 0 ||
                                 range.dimensions[k].max + 1 != (UA_UInt32)dims[k])) {
            found_contiguous = UA_TRUE;
            bs = (range.dimensions[k].max - range.dimensions[k].min + 1) * running_dimssize;
            bd = dims[k] * running_dimssize;
        } 
        fe += running_dimssize * range.dimensions[k].min;
        running_dimssize *= dims[k];
    }
    *total = count;
    *block_size = bs;
    *block_distance = bd;
    *first_elem = fe;
    return UA_STATUSCODE_GOOD;
}

UA_StatusCode UA_Variant_copyRange(const UA_Variant *src, UA_Variant *dst, const UA_NumericRange range) {
    size_t count, block_size, block_distance, first_elem;
    UA_StatusCode retval = testRangeWithVariant(src, range, &count, &block_size, &block_distance, &first_elem);
    if(retval != UA_STATUSCODE_GOOD)
        return retval;

    UA_Variant_init(dst);
    size_t elem_size = src->type->memSize;
    dst->data = UA_malloc(elem_size * count);
    if(!dst->data)
        return UA_STATUSCODE_BADOUTOFMEMORY;

    /* Copy the range */
    size_t block_count = count / block_size;
    uintptr_t nextdst = (uintptr_t)dst->data;
    uintptr_t nextsrc = (uintptr_t)src->data + (elem_size * first_elem);
    if(src->type->fixedSize) {
        for(size_t i = 0; i < block_count; i++) {
            memcpy((void*)nextdst, (void*)nextsrc, elem_size * block_size);
            nextdst += block_size * elem_size;
            nextsrc += block_distance * elem_size;
        }
    } else {
        for(size_t i = 0; i < block_count; i++) {
            for(size_t j = 0; j < block_size && retval == UA_STATUSCODE_GOOD; j++) {
                retval = UA_copy((const void*)nextsrc, (void*)nextdst, src->type);
                nextdst += elem_size;
                nextsrc += elem_size;
            }
            nextsrc += (block_distance - block_size) * elem_size;
        }
        if(retval != UA_STATUSCODE_GOOD) {
            size_t copied = ((nextdst - elem_size) - (uintptr_t)dst->data) / elem_size;
            UA_Array_delete(dst->data, src->type, copied);
            return retval;
        }
    }

    /* Copy the range dimensions*/
    if(src->arrayDimensionsSize > 0) {
        dst->arrayDimensions = UA_malloc(sizeof(UA_Int32) * src->arrayDimensionsSize);
        if(!dst->arrayDimensions) {
            UA_Array_delete(dst->data, src->type, count);
            return UA_STATUSCODE_BADOUTOFMEMORY;
        }
        for(UA_Int32 k = 0; k < src->arrayDimensionsSize; k++)
            dst->arrayDimensions[k] = range.dimensions[k].max - range.dimensions[k].min + 1;
        dst->arrayDimensionsSize = src->arrayDimensionsSize;
    }
    dst->arrayLength = count;
    dst->type = src->type;
    return UA_STATUSCODE_GOOD;
}

UA_StatusCode UA_Variant_setRange(UA_Variant *v, void *dataArray, UA_Int32 dataArraySize,
                                  const UA_NumericRange range) {
    size_t count, block_size, block_distance, first_elem;
    UA_StatusCode retval = testRangeWithVariant(v, range, &count, &block_size, &block_distance, &first_elem);
    if(retval != UA_STATUSCODE_GOOD)
        return retval;
    if((UA_Int32)count != dataArraySize)
        return UA_STATUSCODE_BADINDEXRANGEINVALID;

    size_t block_count = count / block_size;
    size_t elem_size = v->type->memSize;
    uintptr_t nextdst = (uintptr_t)v->data + (first_elem * elem_size);
    uintptr_t nextsrc = (uintptr_t)dataArray;
    for(size_t i = 0; i < block_count; i++) {
        if(!v->type->fixedSize) {
            for(size_t j = 0; j < block_size; j++) {
                UA_deleteMembers((void*)nextdst, v->type);
                nextdst += elem_size;
            }
            nextdst -= block_size * elem_size;
        }
        memcpy((void*)nextdst, (void*)nextsrc, elem_size * block_size);
        nextsrc += block_size * elem_size;
        nextdst += block_distance * elem_size;
    }
    return UA_STATUSCODE_GOOD;
}

UA_StatusCode UA_EXPORT UA_Variant_setRangeCopy(UA_Variant *v, const void *dataArray, UA_Int32 dataArraySize,
                                                const UA_NumericRange range) {
    size_t count, block_size, block_distance, first_elem;
    UA_StatusCode retval = testRangeWithVariant(v, range, &count, &block_size, &block_distance, &first_elem);
    if(retval != UA_STATUSCODE_GOOD)
        return retval;
    if((UA_Int32)count != dataArraySize)
        return UA_STATUSCODE_BADINDEXRANGEINVALID;

    size_t block_count = count / block_size;
    size_t elem_size = v->type->memSize;
    uintptr_t nextdst = (uintptr_t)v->data + (first_elem * elem_size);
    uintptr_t nextsrc = (uintptr_t)dataArray;
    if(v->type->fixedSize) {
        for(size_t i = 0; i < block_count; i++) {
            memcpy((void*)nextdst, (void*)nextsrc, elem_size * block_size);
            nextsrc += block_size * elem_size;
            nextdst += block_distance * elem_size;
        }
    } else {
        for(size_t i = 0; i < block_count; i++) {
            for(size_t j = 0; j < block_size; j++) {
                UA_deleteMembers((void*)nextdst, v->type);
                UA_copy((void*)nextsrc, (void*)nextdst, v->type);
                nextdst += elem_size;
                nextsrc += elem_size;
            }
            nextdst += (block_distance - block_size) * elem_size;
        }
    }
    return UA_STATUSCODE_GOOD;
}

UA_StatusCode UA_Variant_setScalar(UA_Variant *v, void *p, const UA_DataType *type) {
    return UA_Variant_setArray(v, p, -1, type);
}

UA_StatusCode UA_Variant_setScalarCopy(UA_Variant *v, const void *p, const UA_DataType *type) {
    void *new = UA_malloc(type->memSize);
    if(!new)
        return UA_STATUSCODE_BADOUTOFMEMORY;
    UA_StatusCode retval = UA_copy(p, new, type);
	if(retval != UA_STATUSCODE_GOOD) {
		UA_delete(new, type);
		return retval;
	}
    return UA_Variant_setArray(v, new, -1, type);
}

UA_StatusCode UA_Variant_setArray(UA_Variant *v, void *array, UA_Int32 noElements,
                                  const UA_DataType *type) {
    v->type = type;
    v->arrayLength = noElements;
    v->data = array;
    return UA_STATUSCODE_GOOD;
}

UA_StatusCode UA_Variant_setArrayCopy(UA_Variant *v, const void *array, UA_Int32 noElements,
                                      const UA_DataType *type) {
    void *new;
    UA_StatusCode retval = UA_Array_copy(array, &new, type, noElements);
    if(retval != UA_STATUSCODE_GOOD)
        return retval;
    return UA_Variant_setArray(v, new, noElements, type);
}

/* DiagnosticInfo */
UA_TYPE_NEW_DEFAULT(UA_DiagnosticInfo)
UA_TYPE_DELETE_DEFAULT(UA_DiagnosticInfo)

void UA_DiagnosticInfo_deleteMembers(UA_DiagnosticInfo *p) {
    UA_String_deleteMembers(&p->additionalInfo);
    if(p->hasInnerDiagnosticInfo && p->innerDiagnosticInfo) {
        UA_DiagnosticInfo_delete(p->innerDiagnosticInfo);
        p->innerDiagnosticInfo = UA_NULL;
    }
}

void UA_DiagnosticInfo_init(UA_DiagnosticInfo *p) {
	*((UA_Byte*)p) = 0; // zero out the bitfield
    p->symbolicId          = 0;
    p->namespaceUri        = 0;
    p->localizedText       = 0;
    p->locale              = 0;
    UA_String_init(&p->additionalInfo);
    p->innerDiagnosticInfo = UA_NULL;
    UA_StatusCode_init(&p->innerStatusCode);
}

UA_StatusCode UA_DiagnosticInfo_copy(UA_DiagnosticInfo const *src, UA_DiagnosticInfo *dst) {
    UA_DiagnosticInfo_init(dst);
    *((UA_Byte*)dst) = *((const UA_Byte*)src); // the bitfield
    dst->symbolicId = src->symbolicId;
    dst->namespaceUri = src->namespaceUri;
    dst->localizedText = src->localizedText;
    dst->locale = src->locale;
    dst->innerStatusCode = src->innerStatusCode;
    UA_StatusCode retval = UA_STATUSCODE_GOOD;
    if(src->hasAdditionalInfo)
       retval = UA_String_copy(&src->additionalInfo, &dst->additionalInfo);
    if(src->hasInnerDiagnosticInfo && src->innerDiagnosticInfo) {
        if((dst->innerDiagnosticInfo = UA_malloc(sizeof(UA_DiagnosticInfo)))) {
            retval |= UA_DiagnosticInfo_copy(src->innerDiagnosticInfo, dst->innerDiagnosticInfo);
            dst->hasInnerDiagnosticInfo = src->hasInnerDiagnosticInfo;
        }
        else
            retval |= UA_STATUSCODE_BADOUTOFMEMORY;
    }
    if(retval) {
        UA_DiagnosticInfo_deleteMembers(dst);
        UA_DiagnosticInfo_init(dst);
    }
    return retval;
}

/*******************/
/* Structure Types */
/*******************/

void UA_init(void *p, const UA_DataType *dataType) {
    /* Do not check if the index is a builtin-type here. Builtins will be called
       with their very own _init functions normally. In the off case, that the
       generic function is called with the index of a builtin, their layout
       contains a single member of the builtin type, that will be inited in the
       for loop. */

    uintptr_t ptr = (uintptr_t)p;
    UA_Byte membersSize = dataType->membersSize;
    for(size_t i=0;i<membersSize; i++) {
        const UA_DataTypeMember *member = &dataType->members[i];
        if(member->isArray) {
            /* Padding contains bit-magic to split into padding before and after
               the length integer */
            ptr += (member->padding >> 3);
            *((UA_Int32*)ptr) = -1;
            ptr += sizeof(UA_Int32) + (member->padding & 0x07);
            *((void**)ptr) = UA_NULL;
            ptr += sizeof(void*);
            continue;
        }

        ptr += member->padding;
        if(!member->namespaceZero) {
            // pointer arithmetic
            const UA_DataType *memberType = &dataType[member->memberTypeIndex - dataType->typeIndex];
            UA_init((void*)ptr, memberType);
            ptr += memberType->memSize;
            continue;
        }

        switch(member->memberTypeIndex) {
        case UA_TYPES_BOOLEAN:
        case UA_TYPES_SBYTE:
        case UA_TYPES_BYTE:
            *(UA_Byte*)ptr = 0;
            break;
        case UA_TYPES_INT16:
        case UA_TYPES_UINT16:
            *(UA_Int16*)ptr = 0;
            break;
        case UA_TYPES_INT32:
        case UA_TYPES_UINT32:
        case UA_TYPES_STATUSCODE:
        case UA_TYPES_FLOAT:
            *(UA_Int32*)ptr = 0;
            break;
        case UA_TYPES_INT64:
        case UA_TYPES_UINT64:
        case UA_TYPES_DOUBLE:
        case UA_TYPES_DATETIME:
            *(UA_Int64*)ptr = 0;
            break;
        case UA_TYPES_GUID:
            UA_Guid_init((UA_Guid*)ptr);
            break;
        case UA_TYPES_NODEID:
            UA_NodeId_init((UA_NodeId*)ptr);
            break;
        case UA_TYPES_EXPANDEDNODEID:
            UA_ExpandedNodeId_init((UA_ExpandedNodeId*)ptr);
            break;
        case UA_TYPES_LOCALIZEDTEXT:
            UA_LocalizedText_init((UA_LocalizedText*)ptr);
            break;
        case UA_TYPES_EXTENSIONOBJECT:
            UA_ExtensionObject_init((UA_ExtensionObject*)ptr);
            break;
        case UA_TYPES_DATAVALUE:
            UA_DataValue_init((UA_DataValue*)ptr);
            break;
        case UA_TYPES_VARIANT:
            UA_Variant_init((UA_Variant*)ptr);
            break;
        case UA_TYPES_DIAGNOSTICINFO:
            UA_DiagnosticInfo_init((UA_DiagnosticInfo*)ptr);
            break;
        default:
            // QualifiedName, LocalizedText and strings are treated as structures, also
            UA_init((void*)ptr, &UA_TYPES[member->memberTypeIndex]);
        }
        ptr += UA_TYPES[member->memberTypeIndex].memSize;
    }
}

void * UA_new(const UA_DataType *dataType) {
    void *p = UA_malloc(dataType->memSize);
    if(p)
        UA_init(p, dataType);
    return p;
}

UA_StatusCode UA_copy(const void *src, void *dst, const UA_DataType *dataType) {
    if(dataType->fixedSize) {
        memcpy(dst, src, dataType->memSize);
        return UA_STATUSCODE_GOOD;
    }
    UA_init(dst, dataType);
    UA_StatusCode retval = UA_STATUSCODE_GOOD;
    uintptr_t ptrs = (uintptr_t)src;
    uintptr_t ptrd = (uintptr_t)dst;
    UA_Byte membersSize = dataType->membersSize;
    for(size_t i=0;i<membersSize; i++) {
        const UA_DataTypeMember *member = &dataType->members[i];
        const UA_DataType *memberType;
        if(member->namespaceZero)
            memberType = &UA_TYPES[member->memberTypeIndex];
        else
            memberType = &dataType[member->memberTypeIndex - dataType->typeIndex];

        if(member->isArray) {
            ptrs += (member->padding >> 3);
            ptrd += (member->padding >> 3);
            UA_Int32 *dstNoElements = (UA_Int32*)ptrd;
            const UA_Int32 noElements = *((const UA_Int32*)ptrs);
            ptrs += sizeof(UA_Int32) + (member->padding & 0x07);
            ptrd += sizeof(UA_Int32) + (member->padding & 0x07);
            retval = UA_Array_copy(*(void* const*)ptrs, (void**)ptrd, memberType, noElements);
            if(retval != UA_STATUSCODE_GOOD) {
                UA_deleteMembers(dst, dataType);
                UA_init(dst, dataType);
                return retval;
            }
            *dstNoElements = noElements;
            ptrs += sizeof(void*);
            ptrd += sizeof(void*);
            continue;
        }

        ptrs += member->padding;
        ptrd += member->padding;
        if(!member->namespaceZero) {
            retval = UA_copy((const void*)ptrs, (void*)ptrd, memberType);
            if(retval != UA_STATUSCODE_GOOD) {
                UA_deleteMembers(dst, dataType);
                UA_init(dst, dataType);
                return retval;
            }
            ptrs += memberType->memSize;
            ptrd += memberType->memSize;
            continue;
        }

        switch(member->memberTypeIndex) {
        case UA_TYPES_BOOLEAN:
        case UA_TYPES_SBYTE:
        case UA_TYPES_BYTE:
            *((UA_Byte*)ptrd) = *((const UA_Byte*)ptrs);
            break;
        case UA_TYPES_INT16:
        case UA_TYPES_UINT16:
            *((UA_Int16*)ptrd) = *((const UA_Byte*)ptrs);
            break;
        case UA_TYPES_INT32:
        case UA_TYPES_UINT32:
        case UA_TYPES_STATUSCODE:
        case UA_TYPES_FLOAT:
            *((UA_Int32*)ptrd) = *((const UA_Int32*)ptrs);
            break;
        case UA_TYPES_INT64:
        case UA_TYPES_UINT64:
        case UA_TYPES_DOUBLE:
        case UA_TYPES_DATETIME:
            *((UA_Int64*)ptrd) = *((const UA_Int64*)ptrs);
            break;
        case UA_TYPES_GUID:
            *((UA_Guid*)ptrd) = *((const UA_Guid*)ptrs);
            break;
        case UA_TYPES_NODEID:
            retval |= UA_NodeId_copy((const UA_NodeId*)ptrs, (UA_NodeId*)ptrd);
            break;
        case UA_TYPES_EXPANDEDNODEID:
            retval |= UA_ExpandedNodeId_copy((const UA_ExpandedNodeId*)ptrs, (UA_ExpandedNodeId*)ptrd);
            break;
        case UA_TYPES_LOCALIZEDTEXT:
            retval |= UA_LocalizedText_copy((const UA_LocalizedText*)ptrs, (UA_LocalizedText*)ptrd);
            break;
        case UA_TYPES_EXTENSIONOBJECT:
            retval |= UA_ExtensionObject_copy((const UA_ExtensionObject*)ptrs, (UA_ExtensionObject*)ptrd);
            break;
        case UA_TYPES_DATAVALUE:
            retval |= UA_DataValue_copy((const UA_DataValue*)ptrs, (UA_DataValue*)ptrd);
            break;
        case UA_TYPES_VARIANT:
            retval |= UA_Variant_copy((const UA_Variant*)ptrs, (UA_Variant*)ptrd);
            break;
        case UA_TYPES_DIAGNOSTICINFO:
            retval |= UA_DiagnosticInfo_copy((const UA_DiagnosticInfo*)ptrs, (UA_DiagnosticInfo*)ptrd);
            break;
        default:
            // QualifiedName, LocalizedText and strings are treated as structures, also
            retval |= UA_copy((const void *)ptrs, (void*)ptrd, memberType);
        }
        ptrs += memberType->memSize;
        ptrd += memberType->memSize;
    }
    if(retval != UA_STATUSCODE_GOOD) {
        UA_deleteMembers(dst, dataType);
        UA_init(dst, dataType);
    }
    return retval;
}

void UA_deleteMembers(void *p, const UA_DataType *dataType) {
    uintptr_t ptr = (uintptr_t)p;
    if(dataType->fixedSize)
        return;
    UA_Byte membersSize = dataType->membersSize;
    for(size_t i=0;i<membersSize; i++) {
        const UA_DataTypeMember *member = &dataType->members[i];
        const UA_DataType *memberType;
        if(member->namespaceZero)
            memberType = &UA_TYPES[member->memberTypeIndex];
        else
            memberType = &dataType[member->memberTypeIndex - dataType->typeIndex];

        if(member->isArray) {
            ptr += (member->padding >> 3);
            UA_Int32 noElements = *((UA_Int32*)ptr);
            ptr += sizeof(UA_Int32) + (member->padding & 0x07);
            UA_Array_delete(*(void**)ptr, memberType, noElements);
            *(void**)ptr = UA_NULL;
            ptr += sizeof(void*);
            continue;
        }

        ptr += member->padding;
        if(!member->namespaceZero) {
            UA_deleteMembers((void*)ptr, memberType);
            ptr += memberType->memSize;
            continue;
        }

        switch(member->memberTypeIndex) {
        case UA_TYPES_BOOLEAN:
        case UA_TYPES_SBYTE:
        case UA_TYPES_BYTE:
        case UA_TYPES_INT16:
        case UA_TYPES_UINT16:
        case UA_TYPES_INT32:
        case UA_TYPES_UINT32:
        case UA_TYPES_STATUSCODE:
        case UA_TYPES_FLOAT:
        case UA_TYPES_INT64:
        case UA_TYPES_UINT64:
        case UA_TYPES_DOUBLE:
        case UA_TYPES_DATETIME:
        case UA_TYPES_GUID:
            break;
        case UA_TYPES_NODEID:
            UA_NodeId_deleteMembers((UA_NodeId*)ptr);
            break;
        case UA_TYPES_EXPANDEDNODEID:
            UA_ExpandedNodeId_deleteMembers((UA_ExpandedNodeId*)ptr);
            break;
        case UA_TYPES_LOCALIZEDTEXT:
            UA_LocalizedText_deleteMembers((UA_LocalizedText*)ptr);
            break;
        case UA_TYPES_EXTENSIONOBJECT:
            UA_ExtensionObject_deleteMembers((UA_ExtensionObject*)ptr);
            break;
        case UA_TYPES_DATAVALUE:
            UA_DataValue_deleteMembers((UA_DataValue*)ptr);
            break;
        case UA_TYPES_VARIANT:
            UA_Variant_deleteMembers((UA_Variant*)ptr);
            break;
        case UA_TYPES_DIAGNOSTICINFO:
            UA_DiagnosticInfo_deleteMembers((UA_DiagnosticInfo*)ptr);
            break;
        default:
            // QualifiedName, LocalizedText and strings are treated as structures, also
            UA_deleteMembers((void*)ptr, memberType);
        }
        ptr += memberType->memSize;
    }
}

void UA_delete(void *p, const UA_DataType *dataType) {
    UA_deleteMembers(p, dataType);
    UA_free(p);
}

/******************/
/* Array Handling */
/******************/

void* UA_Array_new(const UA_DataType *dataType, UA_Int32 noElements) {
    if(noElements <= 0)
        return UA_NULL;

    if((UA_Int32)dataType->memSize * noElements < 0 || dataType->memSize * noElements > MAX_ARRAY_SIZE )
        return UA_NULL;

    if(dataType->fixedSize)
        return calloc(noElements, dataType->memSize);

    void *p = malloc(dataType->memSize * (size_t)noElements);
    if(!p)
        return p;

    uintptr_t ptr = (uintptr_t)p;
    for(int i = 0; i<noElements; i++) {
        UA_init((void*)ptr, dataType);
        ptr += dataType->memSize;
    }
    return p;
}

UA_StatusCode UA_Array_copy(const void *src, void **dst, const UA_DataType *dataType, UA_Int32 noElements) {
    if(noElements <= 0) {
        *dst = UA_NULL;
        return UA_STATUSCODE_GOOD;
    }

    if(!(*dst = UA_malloc((size_t)noElements * dataType->memSize)))
        return UA_STATUSCODE_BADOUTOFMEMORY;

    if(dataType->fixedSize) {
        memcpy(*dst, src, dataType->memSize * (size_t)noElements);
        return UA_STATUSCODE_GOOD;
    }

    uintptr_t ptrs = (uintptr_t)src;
    uintptr_t ptrd = (uintptr_t)*dst;
    UA_StatusCode retval = UA_STATUSCODE_GOOD;
    for(int i = 0; i < noElements; i++) {
        retval = UA_copy((void*)ptrs, (void*)ptrd, dataType);
        ptrs += dataType->memSize;
        ptrd += dataType->memSize;
    }

    if(retval != UA_STATUSCODE_GOOD)
        UA_Array_delete(*dst, dataType, noElements);

    return retval;
}

void UA_Array_delete(void *p, const UA_DataType *dataType, UA_Int32 noElements) {
    if(!dataType->fixedSize) {
        uintptr_t ptr = (uintptr_t)p;
        for(UA_Int32 i = 0; i<noElements; i++) {
            UA_deleteMembers((void*)ptr, dataType);
            ptr += dataType->memSize;
        }
    }
    UA_free(p);
}
/*********************************** amalgamated original file "C:/AcpltDevelopmentKit/acplt/dev/open62541WrapperProject/open62541/src/ua_types_encoding_binary.c" ***********************************/

/******************/
/* Array Handling */
/******************/

static size_t UA_Array_calcSizeBinary(const void *p, UA_Int32 noElements, const UA_DataType *dataType) {
    if(noElements <= 0)
        return 4;
    size_t size = 4; // the array size encoding
    if(dataType->fixedSize) {
        size += noElements * UA_calcSizeBinary(p, dataType);
        return size;
    }
    uintptr_t ptr = (uintptr_t)p;
    for(int i=0;i<noElements;i++) {
        size += UA_calcSizeBinary((void*)ptr, dataType);
        ptr += dataType->memSize;
    }
    return size;
}

static UA_StatusCode UA_Array_encodeBinary(const void *src, UA_Int32 noElements, const UA_DataType *dataType,
                                           UA_ByteString *dst, size_t *restrict offset) {
    if(noElements <= -1)
        noElements = -1;
    UA_Int32_encodeBinary(&noElements, dst, offset);
    UA_StatusCode retval = UA_STATUSCODE_GOOD;
    uintptr_t ptr = (uintptr_t)src;
    for(int i=0;i<noElements && retval == UA_STATUSCODE_GOOD;i++) {
        retval = UA_encodeBinary((const void*)ptr, dataType, dst, offset);
        ptr += dataType->memSize;
    }
    return retval;
}

static UA_StatusCode UA_Array_decodeBinary(const UA_ByteString *src, size_t *restrict offset,
                                           UA_Int32 noElements, void **dst, const UA_DataType *dataType) {
    if(noElements <= 0) {
        *dst = UA_NULL;
        return UA_STATUSCODE_GOOD;
    }

    if((UA_Int32)dataType->memSize * noElements < 0 || dataType->memSize * noElements > MAX_ARRAY_SIZE )
        return UA_STATUSCODE_BADOUTOFMEMORY;

    // filter out arrays that can obviously not be parsed
    if(*offset + ((dataType->memSize * noElements)/32) > (UA_UInt32)src->length)
        return UA_STATUSCODE_BADDECODINGERROR;

    *dst = UA_malloc(dataType->memSize * noElements);
    if(!*dst)
        return UA_STATUSCODE_BADOUTOFMEMORY;

    uintptr_t ptr = (uintptr_t)*dst;
    UA_StatusCode retval = UA_STATUSCODE_GOOD;
    UA_Int32 i;
    for(i=0;i<noElements && retval == UA_STATUSCODE_GOOD;i++) {
        retval = UA_decodeBinary(src, offset, (void*)ptr, dataType);
        ptr += dataType->memSize;
    }
    if(retval != UA_STATUSCODE_GOOD) {
        UA_Array_delete(*dst, dataType, i);
        *dst = UA_NULL;
    }
    return retval;
}

/*****************/
/* Builtin Types */
/*****************/

#define UA_TYPE_CALCSIZEBINARY_MEMSIZE(TYPE) \
    size_t TYPE##_calcSizeBinary(TYPE const *p) { return sizeof(TYPE); }

#define UA_TYPE_BINARY_ENCODING_AS(TYPE, TYPE_AS)                       \
    size_t TYPE##_calcSizeBinary(TYPE const *p) {                       \
        return TYPE_AS##_calcSizeBinary((const TYPE_AS *)p);            \
    }                                                                   \
    UA_StatusCode TYPE##_encodeBinary(TYPE const *src, UA_ByteString *dst, size_t *restrict offset) { \
        return TYPE_AS##_encodeBinary((const TYPE_AS *)src, dst, offset); \
    }                                                                   \
    UA_StatusCode TYPE##_decodeBinary(UA_ByteString const *src, size_t *restrict offset, TYPE *dst) { \
        return TYPE_AS##_decodeBinary(src, offset, (TYPE_AS *)dst);     \
    }

/* Boolean */
UA_TYPE_CALCSIZEBINARY_MEMSIZE(UA_Boolean)
UA_StatusCode UA_Boolean_encodeBinary(const UA_Boolean *src, UA_ByteString *dst, size_t *restrict offset) {
    if((UA_Int32)(*offset + sizeof(UA_Boolean)) > dst->length)
        return UA_STATUSCODE_BADENCODINGERROR;
    dst->data[*offset] = (UA_Byte)*src;
    ++(*offset);
    return UA_STATUSCODE_GOOD;
}

UA_StatusCode UA_Boolean_decodeBinary(UA_ByteString const *src, size_t *restrict offset, UA_Boolean *dst) {
    if((UA_Int32)(*offset + sizeof(UA_Boolean)) > src->length )
        return UA_STATUSCODE_BADDECODINGERROR;
    *dst = (src->data[*offset] > 0) ? UA_TRUE : UA_FALSE;
    ++(*offset);
    return UA_STATUSCODE_GOOD;
}

/* SByte */
UA_TYPE_BINARY_ENCODING_AS(UA_SByte, UA_Byte)

/* Byte */
UA_TYPE_CALCSIZEBINARY_MEMSIZE(UA_Byte)
UA_StatusCode UA_Byte_encodeBinary(const UA_Byte *src, UA_ByteString *dst, size_t *restrict offset) {
    if((UA_Int32)(*offset + sizeof(UA_Byte)) > dst->length)
        return UA_STATUSCODE_BADENCODINGERROR;
    dst->data[*offset] = (UA_Byte)*src;
    ++(*offset);
    return UA_STATUSCODE_GOOD;
}

UA_StatusCode UA_Byte_decodeBinary(UA_ByteString const *src, size_t *restrict offset, UA_Byte *dst) {
    if((UA_Int32)(*offset + sizeof(UA_Byte)) > src->length )
        return UA_STATUSCODE_BADDECODINGERROR;
    *dst = src->data[*offset];
    ++(*offset);
    return UA_STATUSCODE_GOOD;
}

/* Int16 */
UA_TYPE_BINARY_ENCODING_AS(UA_Int16, UA_UInt16)

/* UInt16 */
UA_TYPE_CALCSIZEBINARY_MEMSIZE(UA_UInt16)
UA_StatusCode UA_UInt16_encodeBinary(UA_UInt16 const *src, UA_ByteString *dst, size_t *restrict offset) {
    if((UA_Int32)(*offset + sizeof(UA_UInt16)) > dst->length )
        return UA_STATUSCODE_BADENCODINGERROR;
    UA_UInt16 *dst_ptr = (UA_UInt16*)&dst->data[*offset];
#ifndef _WIN32
    *dst_ptr = htole16(*src);
#else
    *dst_ptr = *src;
#endif
    *offset += 2;
    return UA_STATUSCODE_GOOD;
}

UA_StatusCode UA_UInt16_decodeBinary(UA_ByteString const *src, size_t *restrict offset, UA_UInt16 *dst) {
    if((UA_Int32)(*offset + sizeof(UA_UInt16)) > src->length)
        return UA_STATUSCODE_BADDECODINGERROR;
    UA_UInt16 value = *((UA_UInt16*)&src->data[*offset]);
#ifndef _WIN32
    value = le16toh(value);
#endif
    *dst = value;
    *offset += 2;
    return UA_STATUSCODE_GOOD;
}

/* Int32 */
UA_TYPE_BINARY_ENCODING_AS(UA_Int32, UA_UInt32)

/* UInt32 */
UA_TYPE_CALCSIZEBINARY_MEMSIZE(UA_UInt32)
UA_StatusCode UA_UInt32_encodeBinary(UA_UInt32 const *src, UA_ByteString * dst, size_t *restrict offset) {
    if((UA_Int32)(*offset + sizeof(UA_UInt32)) > dst->length )
        return UA_STATUSCODE_BADENCODINGERROR;
    UA_UInt32 *dst_ptr = (UA_UInt32*)&dst->data[*offset];
#ifndef _WIN32
    *dst_ptr = htole32(*src);
#else
    *dst_ptr = *src;
#endif
    *offset += 4;
    return UA_STATUSCODE_GOOD;
}

UA_StatusCode UA_UInt32_decodeBinary(UA_ByteString const *src, size_t *restrict offset, UA_UInt32 * dst) {
    if((UA_Int32)(*offset + sizeof(UA_UInt32)) > src->length)
        return UA_STATUSCODE_BADDECODINGERROR;
    UA_UInt32 value = *((UA_UInt32*)&src->data[*offset]);
#ifndef _WIN32
    value = le32toh(value);
#endif
    *dst = value;
    *offset += 4;
    return UA_STATUSCODE_GOOD;
}

/* Int64 */
UA_TYPE_BINARY_ENCODING_AS(UA_Int64, UA_UInt64)

/* UInt64 */
UA_TYPE_CALCSIZEBINARY_MEMSIZE(UA_UInt64)
UA_StatusCode UA_UInt64_encodeBinary(UA_UInt64 const *src, UA_ByteString *dst, size_t *restrict offset) {
    if((UA_Int32)(*offset + sizeof(UA_UInt64)) > dst->length )
        return UA_STATUSCODE_BADENCODINGERROR;
    UA_UInt64 *dst_ptr = (UA_UInt64*)&dst->data[*offset];
#ifndef _WIN32
    *dst_ptr = htole64(*src);
#else
    *dst_ptr = *src;
#endif
    *offset += 8;
    return UA_STATUSCODE_GOOD;
}

UA_StatusCode UA_UInt64_decodeBinary(UA_ByteString const *src, size_t *restrict offset, UA_UInt64 * dst) {
    if((UA_Int32)(*offset + sizeof(UA_UInt64)) > src->length)
        return UA_STATUSCODE_BADDECODINGERROR;
    UA_UInt64 value = *((UA_UInt64*)&src->data[*offset]);
#ifndef _WIN32
    value = le64toh(value);
#endif
    *dst = value;
    *offset += 8;
    return UA_STATUSCODE_GOOD;
}

/* Float */
/* In case that the float endiannes is different from the integer endianness (very unlikely on
   modern cpus), use the following code:

   UA_Byte UA_FLOAT_ZERO[] = { 0x00, 0x00, 0x00, 0x00 };
   UA_Float mantissa;
   UA_UInt32 biasedExponent;
   UA_Float sign;
   if(memcmp(&src->data[*offset], UA_FLOAT_ZERO, 4) == 0)
       return UA_Int32_decodeBinary(src, offset, (UA_Int32 *)dst);
   mantissa = (UA_Float)(src->data[*offset] & 0xFF);                                   // bits 0-7
   mantissa = (mantissa / (UA_Float)256.0 ) + (UA_Float)(src->data[*offset+1] & 0xFF); // bits 8-15
   mantissa = (mantissa / (UA_Float)256.0 ) + (UA_Float)(src->data[*offset+2] & 0x7F); // bits 16-22
   biasedExponent  = (src->data[*offset+2] & 0x80) >>  7;                              // bits 23
   biasedExponent |= (UA_UInt32)(src->data[*offset+3] & 0x7F) <<  1;                   // bits 24-30
   sign = ( src->data[*offset+ 3] & 0x80 ) ? -1.0 : 1.0;                               // bit 31
   if(biasedExponent >= 127)
       *dst = (UA_Float)sign * (1ULL << (biasedExponent-127)) * (1.0 + mantissa / 128.0 );
   else
       *dst = (UA_Float)sign * 2.0 * (1.0 + mantissa / 128.0 ) / ((UA_Float)(biasedExponent-127));}
 */
UA_TYPE_BINARY_ENCODING_AS(UA_Float, UA_UInt32)

/* Double */
/* UA_Byte UA_DOUBLE_ZERO[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
   UA_Double sign;
   UA_Double mantissa;
   UA_UInt32 biasedExponent;
   if(memcmp(&src->data[*offset], UA_DOUBLE_ZERO, 8) == 0)
       return UA_Int64_decodeBinary(src, offset, (UA_Int64 *)dst);
   mantissa = (UA_Double)(src->data[*offset] & 0xFF);                         // bits 0-7
   mantissa = (mantissa / 256.0 ) + (UA_Double)(src->data[*offset+1] & 0xFF); // bits 8-15
   mantissa = (mantissa / 256.0 ) + (UA_Double)(src->data[*offset+2] & 0xFF); // bits 16-23
   mantissa = (mantissa / 256.0 ) + (UA_Double)(src->data[*offset+3] & 0xFF); // bits 24-31
   mantissa = (mantissa / 256.0 ) + (UA_Double)(src->data[*offset+4] & 0xFF); // bits 32-39
   mantissa = (mantissa / 256.0 ) + (UA_Double)(src->data[*offset+5] & 0xFF); // bits 40-47
   mantissa = (mantissa / 256.0 ) + (UA_Double)(src->data[*offset+6] & 0x0F); // bits 48-51
   biasedExponent  = (src->data[*offset+6] & 0xF0) >>  4; // bits 52-55
   biasedExponent |= ((UA_UInt32)(src->data[*offset+7] & 0x7F)) <<  4; // bits 56-62
   sign = ( src->data[*offset+7] & 0x80 ) ? -1.0 : 1.0; // bit 63
   if(biasedExponent >= 1023)
       *dst = (UA_Double)sign * (1ULL << (biasedExponent-1023)) * (1.0 + mantissa / 8.0 );
   else
     *dst = (UA_Double)sign * 2.0 * (1.0 + mantissa / 8.0 ) / ((UA_Double)(biasedExponent-1023));
*/
UA_TYPE_BINARY_ENCODING_AS(UA_Double, UA_UInt64)

/* String */
size_t UA_String_calcSizeBinary(UA_String const *string) {
    if(string->length > 0)
        return sizeof(UA_Int32) + (string->length * sizeof(UA_Byte));
    else
        return sizeof(UA_Int32);
}

UA_StatusCode UA_String_encodeBinary(UA_String const *src, UA_ByteString *dst, size_t *restrict offset) {
    if((UA_Int32)(*offset + UA_String_calcSizeBinary(src)) > dst->length)
        return UA_STATUSCODE_BADENCODINGERROR;

    UA_StatusCode retval = UA_Int32_encodeBinary(&src->length, dst, offset);
    if(src->length > 0) {
        UA_memcpy(&dst->data[*offset], src->data, src->length);
        *offset += src->length;
    }
    return retval;
}

UA_StatusCode UA_String_decodeBinary(UA_ByteString const *src, size_t *restrict offset, UA_String *dst) {
    UA_String_init(dst);
    UA_Int32 length;
    if(UA_Int32_decodeBinary(src, offset, &length))
        return UA_STATUSCODE_BADINTERNALERROR;
    if(length <= 0) {
        if(length == 0)
            dst->length = 0;
        else
            dst->length = -1;
        return UA_STATUSCODE_GOOD;
    }
        
    if((UA_Int32)*offset + length > src->length)
        return UA_STATUSCODE_BADDECODINGERROR;
    
    if(!(dst->data = UA_malloc(length)))
        return UA_STATUSCODE_BADOUTOFMEMORY;

    UA_memcpy(dst->data, &src->data[*offset], length);
    dst->length = length;
    *offset += length;
    return UA_STATUSCODE_GOOD;
}

/* DateTime */
UA_TYPE_BINARY_ENCODING_AS(UA_DateTime, UA_Int64)

/* Guid */
size_t UA_Guid_calcSizeBinary(UA_Guid const *p) {
    return 16;
}

UA_StatusCode UA_Guid_encodeBinary(UA_Guid const *src, UA_ByteString * dst, size_t *restrict offset) {
    UA_StatusCode retval = UA_UInt32_encodeBinary(&src->data1, dst, offset);
    retval |= UA_UInt16_encodeBinary(&src->data2, dst, offset);
    retval |= UA_UInt16_encodeBinary(&src->data3, dst, offset);
    for(UA_Int32 i = 0;i < 8;i++)
        retval |= UA_Byte_encodeBinary(&src->data4[i], dst, offset);
    return retval;
}

UA_StatusCode UA_Guid_decodeBinary(UA_ByteString const *src, size_t *restrict offset, UA_Guid * dst) {
    // This could be done with a single memcpy (if the compiler does no fancy realigning of structs)
    UA_StatusCode retval = UA_UInt32_decodeBinary(src, offset, &dst->data1);
    retval |= UA_UInt16_decodeBinary(src, offset, &dst->data2);
    retval |= UA_UInt16_decodeBinary(src, offset, &dst->data3);
    for(size_t i = 0;i < 8;i++)
        retval |= UA_Byte_decodeBinary(src, offset, &dst->data4[i]);
    if(retval)
        UA_Guid_deleteMembers(dst);
    return retval;
}

/* ByteString */
UA_TYPE_BINARY_ENCODING_AS(UA_ByteString, UA_String)

/* XmlElement */
UA_TYPE_BINARY_ENCODING_AS(UA_XmlElement, UA_String)

/* NodeId */

/* The shortened numeric nodeid types. */
#define UA_NODEIDTYPE_TWOBYTE 0
#define UA_NODEIDTYPE_FOURBYTE 1

size_t UA_NodeId_calcSizeBinary(UA_NodeId const *p) {
    size_t length = 0;
    switch(p->identifierType) {
    case UA_NODEIDTYPE_NUMERIC:
        if(p->identifier.numeric > UA_UINT16_MAX || p->namespaceIndex > UA_BYTE_MAX)
            length = sizeof(UA_Byte) + sizeof(UA_UInt16) + sizeof(UA_UInt32);
        else if(p->identifier.numeric > UA_BYTE_MAX || p->namespaceIndex > 0)
            length = 4;  /* UA_NODEIDTYPE_FOURBYTE */
        else
            length = 2;  /* UA_NODEIDTYPE_TWOBYTE*/
        break;
    case UA_NODEIDTYPE_STRING:
        length = sizeof(UA_Byte) + sizeof(UA_UInt16) + UA_String_calcSizeBinary(&p->identifier.string);
        break;
    case UA_NODEIDTYPE_GUID:
        length = sizeof(UA_Byte) + sizeof(UA_UInt16) + UA_Guid_calcSizeBinary(&p->identifier.guid);
        break;
    case UA_NODEIDTYPE_BYTESTRING:
        length = sizeof(UA_Byte) + sizeof(UA_UInt16) + UA_ByteString_calcSizeBinary(&p->identifier.byteString);
        break;
    default:
        UA_assert(UA_FALSE); // this must never happen
        break;
    }
    return length;
}

static UA_StatusCode UA_NodeId_encodeBinary_nodeid_offset(UA_NodeId const *src, UA_ByteString * dst,
                                                          size_t *restrict offset, UA_Int32 nodeid_offset) {
    UA_StatusCode retval = UA_STATUSCODE_GOOD;
    // temporary variables for endian-save code
    UA_Byte srcByte;
    UA_UInt16 srcUInt16;
    UA_UInt32 srcUInt32;
    switch(src->identifierType) {
    case UA_NODEIDTYPE_NUMERIC:
        if(src->identifier.numeric+nodeid_offset > UA_UINT16_MAX || src->namespaceIndex > UA_BYTE_MAX) {
            srcByte = UA_NODEIDTYPE_NUMERIC;
            retval |= UA_Byte_encodeBinary(&srcByte, dst, offset);
            retval |= UA_UInt16_encodeBinary(&src->namespaceIndex, dst, offset);
            srcUInt32 = src->identifier.numeric+nodeid_offset;
            retval |= UA_UInt32_encodeBinary(&srcUInt32, dst, offset);
        } else if(src->identifier.numeric+nodeid_offset > UA_BYTE_MAX || src->namespaceIndex > 0) { /* UA_NODEIDTYPE_FOURBYTE */
            srcByte = UA_NODEIDTYPE_FOURBYTE;
            retval |= UA_Byte_encodeBinary(&srcByte, dst, offset);
            srcByte = src->namespaceIndex;
            srcUInt16 = src->identifier.numeric+nodeid_offset;
            retval |= UA_Byte_encodeBinary(&srcByte, dst, offset);
            retval |= UA_UInt16_encodeBinary(&srcUInt16, dst, offset);
        } else { /* UA_NODEIDTYPE_TWOBYTE */
            srcByte = UA_NODEIDTYPE_TWOBYTE;
            retval |= UA_Byte_encodeBinary(&srcByte, dst, offset);
            srcByte = src->identifier.numeric+nodeid_offset;
            retval |= UA_Byte_encodeBinary(&srcByte, dst, offset);
        }
        break;
    case UA_NODEIDTYPE_STRING:
        srcByte = UA_NODEIDTYPE_STRING;
        retval |= UA_Byte_encodeBinary(&srcByte, dst, offset);
        retval |= UA_UInt16_encodeBinary(&src->namespaceIndex, dst, offset);
        retval |= UA_String_encodeBinary(&src->identifier.string, dst, offset);
        break;
    case UA_NODEIDTYPE_GUID:
        srcByte = UA_NODEIDTYPE_GUID;
        retval |= UA_Byte_encodeBinary(&srcByte, dst, offset);
        retval |= UA_UInt16_encodeBinary(&src->namespaceIndex, dst, offset);
        retval |= UA_Guid_encodeBinary(&src->identifier.guid, dst, offset);
        break;
    case UA_NODEIDTYPE_BYTESTRING:
        srcByte = UA_NODEIDTYPE_BYTESTRING;
        retval |= UA_Byte_encodeBinary(&srcByte, dst, offset);
        retval |= UA_UInt16_encodeBinary(&src->namespaceIndex, dst, offset);
        retval |= UA_ByteString_encodeBinary(&src->identifier.byteString, dst, offset);
        break;
    default:
        UA_assert(UA_FALSE);
    }
    return retval;
}

UA_StatusCode UA_NodeId_encodeBinary(UA_NodeId const *src, UA_ByteString * dst, size_t *restrict offset) {
	return UA_NodeId_encodeBinary_nodeid_offset(src, dst, offset, 0);
}

UA_StatusCode UA_NodeId_decodeBinary(UA_ByteString const *src, size_t *restrict offset, UA_NodeId *dst) {
    // temporary variables to overcome decoder's non-endian-saveness for datatypes with different length
    UA_Byte   dstByte = 0;
    UA_UInt16 dstUInt16 = 0;
    UA_Byte   encodingByte = 0;

    UA_StatusCode retval = UA_Byte_decodeBinary(src, offset, &encodingByte);
    if(retval) {
        UA_NodeId_init(dst);
        return retval;
    }
    
    switch(encodingByte) {
    case UA_NODEIDTYPE_TWOBYTE: // Table 7
        dst->identifierType     = UA_NODEIDTYPE_NUMERIC;
        retval = UA_Byte_decodeBinary(src, offset, &dstByte);
        dst->identifier.numeric = dstByte;
        dst->namespaceIndex     = 0; // default namespace
        break;
    case UA_NODEIDTYPE_FOURBYTE: // Table 8
        dst->identifierType     = UA_NODEIDTYPE_NUMERIC;
        retval |= UA_Byte_decodeBinary(src, offset, &dstByte);
        dst->namespaceIndex     = dstByte;
        retval |= UA_UInt16_decodeBinary(src, offset, &dstUInt16);
        dst->identifier.numeric = dstUInt16;
        break;
    case UA_NODEIDTYPE_NUMERIC: // Table 6, first entry
        dst->identifierType = UA_NODEIDTYPE_NUMERIC;
        retval |= UA_UInt16_decodeBinary(src, offset, &dst->namespaceIndex);
        retval |= UA_UInt32_decodeBinary(src, offset, &dst->identifier.numeric);
        break;
    case UA_NODEIDTYPE_STRING: // Table 6, second entry
        dst->identifierType = UA_NODEIDTYPE_STRING;
        retval |= UA_UInt16_decodeBinary(src, offset, &dst->namespaceIndex);
        retval |= UA_String_decodeBinary(src, offset, &dst->identifier.string);
        break;
    case UA_NODEIDTYPE_GUID: // Table 6, third entry
        dst->identifierType = UA_NODEIDTYPE_GUID;
        retval |= UA_UInt16_decodeBinary(src, offset, &dst->namespaceIndex);
        retval |= UA_Guid_decodeBinary(src, offset, &dst->identifier.guid);
        break;
    case UA_NODEIDTYPE_BYTESTRING: // Table 6, "OPAQUE"
        dst->identifierType = UA_NODEIDTYPE_BYTESTRING;
        retval |= UA_UInt16_decodeBinary(src, offset, &dst->namespaceIndex);
        retval |= UA_ByteString_decodeBinary(src, offset, &dst->identifier.byteString);
        break;
    default:
        UA_NodeId_init(dst);
        retval |= UA_STATUSCODE_BADINTERNALERROR; // the client sends an encodingByte we do not recognize
        break;
    }
    if(retval)
        UA_NodeId_deleteMembers(dst);
    return retval;
}

/* ExpandedNodeId */
size_t UA_ExpandedNodeId_calcSizeBinary(UA_ExpandedNodeId const *p) {
    size_t length = UA_NodeId_calcSizeBinary(&p->nodeId);
    if(p->namespaceUri.length > 0)
        length += UA_String_calcSizeBinary(&p->namespaceUri);
    if(p->serverIndex > 0)
        length += sizeof(UA_UInt32);
    return length;
}

#define UA_EXPANDEDNODEID_NAMESPACEURI_FLAG 0x80
#define UA_EXPANDEDNODEID_SERVERINDEX_FLAG 0x40

UA_StatusCode UA_ExpandedNodeId_encodeBinary(UA_ExpandedNodeId const *src, UA_ByteString *dst,
                                             size_t *restrict offset) {
    UA_Byte flags = 0;
    UA_UInt32 start = *offset;
    UA_StatusCode retval = UA_NodeId_encodeBinary(&src->nodeId, dst, offset);
    if(src->namespaceUri.length > 0) {
        // TODO: Set namespaceIndex to 0 in the nodeid as the namespaceUri takes precedence
        retval |= UA_String_encodeBinary(&src->namespaceUri, dst, offset);
        flags |= UA_EXPANDEDNODEID_NAMESPACEURI_FLAG;
    }
    if(src->serverIndex > 0) {
        retval |= UA_UInt32_encodeBinary(&src->serverIndex, dst, offset);
        flags |= UA_EXPANDEDNODEID_SERVERINDEX_FLAG;
    }
    if(flags != 0)
        dst->data[start] |= flags;
    return retval;
}

UA_StatusCode UA_ExpandedNodeId_decodeBinary(UA_ByteString const *src, size_t *restrict offset,
                                             UA_ExpandedNodeId *dst) {
    UA_ExpandedNodeId_init(dst);
    // get encodingflags and leave a "clean" nodeidtype
    if((UA_Int32)*offset >= src->length)
        return UA_STATUSCODE_BADDECODINGERROR;
    UA_Byte encodingByte = src->data[*offset];
    src->data[*offset] = encodingByte & ~(UA_EXPANDEDNODEID_NAMESPACEURI_FLAG |
                                          UA_EXPANDEDNODEID_SERVERINDEX_FLAG);
    UA_StatusCode retval = UA_NodeId_decodeBinary(src, offset, &dst->nodeId);
    if(encodingByte & UA_EXPANDEDNODEID_NAMESPACEURI_FLAG) {
        dst->nodeId.namespaceIndex = 0;
        retval |= UA_String_decodeBinary(src, offset, &dst->namespaceUri);
    }
    if(encodingByte & UA_EXPANDEDNODEID_SERVERINDEX_FLAG)
        retval |= UA_UInt32_decodeBinary(src, offset, &dst->serverIndex);
    if(retval)
        UA_ExpandedNodeId_deleteMembers(dst);
    return retval;
}

/* StatusCode */
UA_TYPE_BINARY_ENCODING_AS(UA_StatusCode, UA_UInt32)

/* QualifiedName */
size_t UA_QualifiedName_calcSizeBinary(UA_QualifiedName const *p) {
    size_t length = sizeof(UA_UInt16); //qualifiedName->namespaceIndex
    // length += sizeof(UA_UInt16); //qualifiedName->reserved
    length += UA_String_calcSizeBinary(&p->name); //qualifiedName->name
    return length;
}

UA_StatusCode UA_QualifiedName_decodeBinary(UA_ByteString const *src, size_t *restrict offset,
                                            UA_QualifiedName *dst) {
    UA_QualifiedName_init(dst);
    UA_StatusCode retval = UA_UInt16_decodeBinary(src, offset, &dst->namespaceIndex);
    retval |= UA_String_decodeBinary(src, offset, &dst->name);
    if(retval)
        UA_QualifiedName_deleteMembers(dst);
    return retval;
}

UA_StatusCode UA_QualifiedName_encodeBinary(UA_QualifiedName const *src, UA_ByteString* dst,
                                            size_t *restrict offset) {
    UA_StatusCode retval = UA_UInt16_encodeBinary(&src->namespaceIndex, dst, offset);
    retval |= UA_String_encodeBinary(&src->name, dst, offset);
    return retval;
}

/* LocalizedText */
#define UA_LOCALIZEDTEXT_ENCODINGMASKTYPE_LOCALE 0x01
#define UA_LOCALIZEDTEXT_ENCODINGMASKTYPE_TEXT 0x02

size_t UA_LocalizedText_calcSizeBinary(UA_LocalizedText const *p) {
    size_t length = 1; // for encodingMask
    if(p->locale.data != UA_NULL)
        length += UA_String_calcSizeBinary(&p->locale);
    if(p->text.data != UA_NULL)
        length += UA_String_calcSizeBinary(&p->text);
    return length;
}

UA_StatusCode UA_LocalizedText_encodeBinary(UA_LocalizedText const *src, UA_ByteString * dst,
                                            size_t *restrict offset) {
    UA_Byte encodingMask = 0;
    if(src->locale.data != UA_NULL)
        encodingMask |= UA_LOCALIZEDTEXT_ENCODINGMASKTYPE_LOCALE;
    if(src->text.data != UA_NULL)
        encodingMask |= UA_LOCALIZEDTEXT_ENCODINGMASKTYPE_TEXT;
    UA_StatusCode retval = UA_Byte_encodeBinary(&encodingMask, dst, offset);
    if(encodingMask & UA_LOCALIZEDTEXT_ENCODINGMASKTYPE_LOCALE)
        retval |= UA_String_encodeBinary(&src->locale, dst, offset);
    if(encodingMask & UA_LOCALIZEDTEXT_ENCODINGMASKTYPE_TEXT)
        retval |= UA_String_encodeBinary(&src->text, dst, offset);
    return retval;
}

UA_StatusCode UA_LocalizedText_decodeBinary(UA_ByteString const *src, size_t *restrict offset,
                                            UA_LocalizedText *dst) {
    UA_LocalizedText_init(dst);
    UA_Byte encodingMask = 0;
    UA_StatusCode retval = UA_Byte_decodeBinary(src, offset, &encodingMask);
    if(encodingMask & UA_LOCALIZEDTEXT_ENCODINGMASKTYPE_LOCALE)
        retval |= UA_String_decodeBinary(src, offset, &dst->locale);
    if(encodingMask & UA_LOCALIZEDTEXT_ENCODINGMASKTYPE_TEXT)
        retval |= UA_String_decodeBinary(src, offset, &dst->text);
    if(retval)
        UA_LocalizedText_deleteMembers(dst);
    return retval;
}

/* ExtensionObject */
size_t UA_ExtensionObject_calcSizeBinary(UA_ExtensionObject const *p) {
    size_t length = UA_NodeId_calcSizeBinary(&p->typeId);
    length += 1; // encoding
    switch(p->encoding) {
    case UA_EXTENSIONOBJECT_ENCODINGMASK_NOBODYISENCODED:
        break;
    case UA_EXTENSIONOBJECT_ENCODINGMASK_BODYISBYTESTRING:
        length += UA_ByteString_calcSizeBinary(&p->body);
        break;
    case UA_EXTENSIONOBJECT_ENCODINGMASK_BODYISXML:
        length += UA_XmlElement_calcSizeBinary((const UA_XmlElement *)&p->body);
        break;
    default:
        UA_assert(UA_FALSE);
    }
    return length;
}

UA_StatusCode UA_ExtensionObject_encodeBinary(UA_ExtensionObject const *src, UA_ByteString * dst,
                                              size_t *restrict offset) {
    UA_StatusCode retval = UA_NodeId_encodeBinary(&src->typeId, dst, offset);
    retval |= UA_Byte_encodeBinary((const UA_Byte*)&src->encoding, dst, offset);
    switch(src->encoding) {
    case UA_EXTENSIONOBJECT_ENCODINGMASK_NOBODYISENCODED:
        break;
    case UA_EXTENSIONOBJECT_ENCODINGMASK_BODYISBYTESTRING:
        retval |= UA_ByteString_encodeBinary(&src->body, dst, offset);
        break;
    case UA_EXTENSIONOBJECT_ENCODINGMASK_BODYISXML:
        retval |= UA_ByteString_encodeBinary(&src->body, dst, offset);
        break;
    default:
        UA_assert(UA_FALSE);
    }
    return retval;
}

UA_StatusCode UA_ExtensionObject_decodeBinary(UA_ByteString const *src, size_t *restrict offset,
                                              UA_ExtensionObject *dst) {
    UA_ExtensionObject_init(dst);
    UA_Byte encoding = 0;
    UA_StatusCode retval = UA_NodeId_decodeBinary(src, offset, &dst->typeId);
    retval |= UA_Byte_decodeBinary(src, offset, &encoding);
    dst->encoding = encoding;
    retval |= UA_String_copy(&UA_STRING_NULL, (UA_String *)&dst->body);
    if(retval) {
        UA_ExtensionObject_init(dst);
        return retval;
    }
    switch(dst->encoding) {
    case UA_EXTENSIONOBJECT_ENCODINGMASK_NOBODYISENCODED:
        break;
    case UA_EXTENSIONOBJECT_ENCODINGMASK_BODYISBYTESTRING:
    case UA_EXTENSIONOBJECT_ENCODINGMASK_BODYISXML:
        retval |= UA_ByteString_decodeBinary(src, offset, &dst->body);
        break;
    default:
        UA_ExtensionObject_deleteMembers(dst);
        return UA_STATUSCODE_BADDECODINGERROR;
    }
    if(retval)
        UA_ExtensionObject_deleteMembers(dst);
    return retval;
}

/* DataValue */
size_t UA_DataValue_calcSizeBinary(UA_DataValue const *p) {
    size_t length = sizeof(UA_Byte);
    if(p->hasValue)
        length += UA_Variant_calcSizeBinary(&p->value);
    if(p->hasStatus)
        length += sizeof(UA_UInt32);
    if(p->hasSourceTimestamp)
        length += sizeof(UA_DateTime);
    if(p->hasSourcePicoseconds)
        length += sizeof(UA_Int16);
    if(p->hasServerTimestamp)
        length += sizeof(UA_DateTime);
    if(p->hasServerPicoseconds)
        length += sizeof(UA_Int16);
    return length;
}

UA_StatusCode UA_DataValue_encodeBinary(UA_DataValue const *src, UA_ByteString *dst, size_t *restrict offset) {
    UA_StatusCode retval = UA_Byte_encodeBinary((const UA_Byte*)src, dst, offset);
    if(src->hasValue)
        retval |= UA_Variant_encodeBinary(&src->value, dst, offset);
    if(src->hasStatus)
        retval |= UA_StatusCode_encodeBinary(&src->status, dst, offset);
    if(src->hasSourceTimestamp)
        retval |= UA_DateTime_encodeBinary(&src->sourceTimestamp, dst, offset);
    if(src->hasSourcePicoseconds)
        retval |= UA_Int16_encodeBinary(&src->sourcePicoseconds, dst, offset);
    if(src->hasServerTimestamp)
        retval |= UA_DateTime_encodeBinary(&src->serverTimestamp, dst, offset);
    if(src->hasServerPicoseconds)
        retval |= UA_Int16_encodeBinary(&src->serverPicoseconds, dst, offset);
    return retval;
}

#define MAX_PICO_SECONDS 1000
UA_StatusCode UA_DataValue_decodeBinary(UA_ByteString const *src, size_t *restrict offset, UA_DataValue *dst) {
    UA_DataValue_init(dst);
    UA_StatusCode retval = UA_Byte_decodeBinary(src, offset, (UA_Byte*)dst);
    if(retval != UA_STATUSCODE_GOOD)
        return retval;
    if(dst->hasValue)
        retval |= UA_Variant_decodeBinary(src, offset, &dst->value);
    if(dst->hasStatus)
        retval |= UA_StatusCode_decodeBinary(src, offset, &dst->status);
    if(dst->hasSourceTimestamp)
        retval |= UA_DateTime_decodeBinary(src, offset, &dst->sourceTimestamp);
    if(dst->hasSourcePicoseconds) {
        retval |= UA_Int16_decodeBinary(src, offset, &dst->sourcePicoseconds);
        if(dst->sourcePicoseconds > MAX_PICO_SECONDS)
            dst->sourcePicoseconds = MAX_PICO_SECONDS;
    }
    if(dst->hasServerTimestamp)
        retval |= UA_DateTime_decodeBinary(src, offset, &dst->serverTimestamp);
    if(dst->hasServerPicoseconds) {
        retval |= UA_Int16_decodeBinary(src, offset, &dst->serverPicoseconds);
        if(dst->serverPicoseconds > MAX_PICO_SECONDS)
            dst->serverPicoseconds = MAX_PICO_SECONDS;
    }
    if(retval)
        UA_DataValue_deleteMembers(dst);
    return retval;
}

/* Variant */
/* We can store all data types in a variant internally. But for communication we
 * encode them in an ExtensionObject if they are not one of the built in types.
 * Officially, only builtin types are contained in a variant.
 *
 * Every ExtensionObject incurrs an overhead of 4 byte (nodeid) + 1 byte (encoding) */

/* VariantBinaryEncoding - Part: 6, Chapter: 5.2.2.16, Page: 22 */
enum UA_VARIANT_ENCODINGMASKTYPE_enum {
    UA_VARIANT_ENCODINGMASKTYPE_TYPEID_MASK = 0x3F,            // bits 0:5
    UA_VARIANT_ENCODINGMASKTYPE_DIMENSIONS  = (0x01 << 6),     // bit 6
    UA_VARIANT_ENCODINGMASKTYPE_ARRAY       = (0x01 << 7)      // bit 7
};

size_t UA_Variant_calcSizeBinary(UA_Variant const *p) {
    UA_Boolean isArray = p->arrayLength != -1 || !p->data;  // a single element is not an array
    UA_Boolean hasDimensions = isArray && p->arrayDimensions != UA_NULL;
    UA_Boolean isBuiltin = p->type->namespaceZero && UA_IS_BUILTIN(p->type->typeIndex);
    size_t length = sizeof(UA_Byte); //p->encodingMask
    UA_Int32 arrayLength = p->arrayLength;
    if(!isArray) {
    	arrayLength = 1;
        length += UA_calcSizeBinary(p->data, p->type);
    }
    else
        length += UA_Array_calcSizeBinary(p->data, arrayLength, p->type);
        
    // if the type is not builtin, we encode it as an extensionobject
    if(!isBuiltin) {
        if(arrayLength > 0)
            length += 9 * arrayLength;  // overhead for extensionobjects: 4 byte nodeid + 1 byte
                                        // encoding + 4 byte bytestring length
    }
    if(hasDimensions)
        length += UA_Array_calcSizeBinary(p->arrayDimensions, p->arrayDimensionsSize,
                                          &UA_TYPES[UA_TYPES_INT32]);
    return length;
}

UA_StatusCode UA_Variant_encodeBinary(UA_Variant const *src, UA_ByteString *dst, size_t *restrict offset) {
    UA_Byte encodingByte = 0;
    UA_Boolean isArray = src->arrayLength != -1 || !src->data;  // a single element is not an array
    UA_Boolean hasDimensions = isArray && src->arrayDimensions != UA_NULL;
    UA_Boolean isBuiltin = src->type->namespaceZero && UA_IS_BUILTIN(src->type->typeIndex);

    if(isArray) {
        encodingByte |= UA_VARIANT_ENCODINGMASKTYPE_ARRAY;
        if(hasDimensions)
            encodingByte |= UA_VARIANT_ENCODINGMASKTYPE_DIMENSIONS;
    }

    if(isBuiltin)
        encodingByte |= UA_VARIANT_ENCODINGMASKTYPE_TYPEID_MASK &
            (UA_Byte)UA_TYPES[src->type->typeIndex].typeId.identifier.numeric;
    else
        encodingByte |= UA_VARIANT_ENCODINGMASKTYPE_TYPEID_MASK & (UA_Byte)22; // wrap in an extensionobject

    UA_StatusCode retval = UA_Byte_encodeBinary(&encodingByte, dst, offset);

    if(isArray)
        retval |= UA_Int32_encodeBinary(&src->arrayLength, dst, offset);
    uintptr_t ptr = (uintptr_t)src->data;
    ptrdiff_t memSize = src->type->memSize;
    UA_Int32 numToEncode = src->arrayLength;
    if(!isArray)
        numToEncode = 1;
    for(UA_Int32 i=0;i<numToEncode;i++) {
        if(!isBuiltin) {
            /* The type is wrapped inside an extensionobject*/
            // todo: offest holds only for namespace 0
            retval |= UA_NodeId_encodeBinary_nodeid_offset(&src->type->typeId, dst, offset,
                                                           UA_ENCODINGOFFSET_BINARY);
            UA_Byte eoEncoding = UA_EXTENSIONOBJECT_ENCODINGMASK_BODYISBYTESTRING;
            retval |= UA_Byte_encodeBinary(&eoEncoding, dst, offset);
            UA_Int32 eoEncodingLength = UA_calcSizeBinary(src->data, src->type);
            retval |= UA_Int32_encodeBinary(&eoEncodingLength, dst, offset);
        }
        retval |= UA_encodeBinary((void*)ptr, src->type, dst, offset);
        ptr += memSize;
    }
    if(hasDimensions)
        retval |= UA_Array_encodeBinary(src->arrayDimensions, src->arrayDimensionsSize,
                                        &UA_TYPES[UA_TYPES_INT32], dst, offset);
    return retval;
}

/* The resulting variant always has the storagetype UA_VARIANT_DATA. Currently,
   we only support ns0 types (todo: attach typedescriptions to datatypenodes) */
UA_StatusCode UA_Variant_decodeBinary(UA_ByteString const *src, size_t *restrict offset, UA_Variant *dst) {
    UA_Variant_init(dst);
    UA_Byte encodingByte;
    UA_StatusCode retval = UA_Byte_decodeBinary(src, offset, &encodingByte);
    if(retval != UA_STATUSCODE_GOOD)
        return retval;

    UA_Boolean isArray = encodingByte & UA_VARIANT_ENCODINGMASKTYPE_ARRAY;
    size_t typeIndex = (encodingByte & UA_VARIANT_ENCODINGMASKTYPE_TYPEID_MASK) - 1;
    if(typeIndex > 24) /* must be builtin */
        return UA_STATUSCODE_BADDECODINGERROR;

    if(isArray || typeIndex != UA_TYPES_EXTENSIONOBJECT) {
        /* an array or a single builtin */
        const UA_DataType *dataType = &UA_TYPES[typeIndex];
        UA_Int32 arraySize = -1;
        if(isArray) {
            retval |= UA_Int32_decodeBinary(src, offset, &arraySize);
            if(retval != UA_STATUSCODE_GOOD)
                return retval;
        }
        retval |= UA_Array_decodeBinary(src, offset, (!isArray && arraySize==-1) ? 1: arraySize,
                                        &dst->data, dataType);
        if(retval != UA_STATUSCODE_GOOD)
            return retval;
        dst->arrayLength = arraySize; // for deleteMembers
        dst->type = dataType;
    } else {
        /* a single extensionobject */
        size_t oldoffset = *offset;
        UA_NodeId typeId;
        if((retval |= UA_NodeId_decodeBinary(src, offset, &typeId)) != UA_STATUSCODE_GOOD)
            return retval;
        UA_Byte EOencodingByte;
        if((retval |= UA_Byte_decodeBinary(src, offset, &EOencodingByte)) != UA_STATUSCODE_GOOD) {
            UA_NodeId_deleteMembers(&typeId);
            return retval;
        }
        const UA_DataType *dataType = UA_NULL;
        if(typeId.namespaceIndex == 0 && EOencodingByte == UA_EXTENSIONOBJECT_ENCODINGMASK_BODYISBYTESTRING) {
            for(typeIndex = 0;typeIndex < UA_TYPES_COUNT; typeIndex++) {
                if(UA_NodeId_equal(&typeId, &UA_TYPES[typeIndex].typeId)) {
                    dataType = &UA_TYPES[typeIndex];
                    break;
                }
            }
        }
        UA_NodeId_deleteMembers(&typeId);
        if(!dataType) {
            *offset = oldoffset;
            dataType = &UA_TYPES[UA_TYPES_EXTENSIONOBJECT];
        }
        if((retval |= UA_decodeBinary(src, offset, dst->data, dataType)) != UA_STATUSCODE_GOOD)
            return retval;
        dst->type = dataType;
        dst->arrayLength = -1;
    }
    
    /* array dimensions */
    if(isArray && (encodingByte & UA_VARIANT_ENCODINGMASKTYPE_DIMENSIONS)) {
        retval |= UA_Int32_decodeBinary(src, offset, &dst->arrayDimensionsSize);
        if(retval == UA_STATUSCODE_GOOD)
            retval |= UA_Array_decodeBinary(src, offset, dst->arrayDimensionsSize,
                                            &dst->data, &UA_TYPES[UA_TYPES_INT32]);
        if(retval != UA_STATUSCODE_GOOD) {
            dst->arrayDimensionsSize = -1;
            UA_Variant_deleteMembers(dst);
            return retval;
        }
    }
    return UA_STATUSCODE_GOOD;
}

/* DiagnosticInfo */
size_t UA_DiagnosticInfo_calcSizeBinary(UA_DiagnosticInfo const *ptr) {
    size_t length = sizeof(UA_Byte);
    if(ptr->hasSymbolicId)
        length += sizeof(UA_Int32);
    if(ptr->hasNamespaceUri)
        length += sizeof(UA_Int32);
    if(ptr->hasLocalizedText)
        length += sizeof(UA_Int32);
    if(ptr->hasLocale)
        length += sizeof(UA_Int32);
    if(ptr->hasAdditionalInfo)
        length += UA_String_calcSizeBinary(&ptr->additionalInfo);
    if(ptr->hasInnerStatusCode)
        length += sizeof(UA_StatusCode);
    if(ptr->hasInnerDiagnosticInfo)
        length += UA_DiagnosticInfo_calcSizeBinary(ptr->innerDiagnosticInfo);
    return length;
}

UA_StatusCode UA_DiagnosticInfo_encodeBinary(const UA_DiagnosticInfo *src, UA_ByteString * dst,
                                             size_t *restrict offset) {
    UA_StatusCode retval = UA_Byte_encodeBinary((const UA_Byte *)src, dst, offset);
    if(src->hasSymbolicId)
        retval |= UA_Int32_encodeBinary(&src->symbolicId, dst, offset);
    if(src->hasNamespaceUri)
        retval |= UA_Int32_encodeBinary( &src->namespaceUri, dst, offset);
    if(src->hasLocalizedText)
        retval |= UA_Int32_encodeBinary(&src->localizedText, dst, offset);
    if(src->hasLocale)
        retval |= UA_Int32_encodeBinary(&src->locale, dst, offset);
    if(src->hasAdditionalInfo)
        retval |= UA_String_encodeBinary(&src->additionalInfo, dst, offset);
    if(src->hasInnerStatusCode)
        retval |= UA_StatusCode_encodeBinary(&src->innerStatusCode, dst, offset);
    if(src->hasInnerDiagnosticInfo)
        retval |= UA_DiagnosticInfo_encodeBinary(src->innerDiagnosticInfo, dst, offset);
    return retval;
}

UA_StatusCode UA_DiagnosticInfo_decodeBinary(UA_ByteString const *src, size_t *restrict offset,
                                             UA_DiagnosticInfo *dst) {
    UA_DiagnosticInfo_init(dst);
    UA_StatusCode retval = UA_Byte_decodeBinary(src, offset, (UA_Byte*)dst);
    if(!retval)
        return retval;
    if(dst->hasSymbolicId)
        retval |= UA_Int32_decodeBinary(src, offset, &dst->symbolicId);
    if(dst->hasNamespaceUri)
        retval |= UA_Int32_decodeBinary(src, offset, &dst->namespaceUri);
    if(dst->hasLocalizedText)
        retval |= UA_Int32_decodeBinary(src, offset, &dst->localizedText);
    if(dst->hasLocale)
        retval |= UA_Int32_decodeBinary(src, offset, &dst->locale);
    if(dst->hasAdditionalInfo)
        retval |= UA_String_decodeBinary(src, offset, &dst->additionalInfo);
    if(dst->hasInnerStatusCode)
        retval |= UA_StatusCode_decodeBinary(src, offset, &dst->innerStatusCode);
    if(dst->hasInnerDiagnosticInfo) {
        // innerDiagnosticInfo is a pointer to struct, therefore allocate
        if((dst->innerDiagnosticInfo = UA_malloc(sizeof(UA_DiagnosticInfo)))) {
            if(UA_DiagnosticInfo_decodeBinary(src, offset, dst->innerDiagnosticInfo) != UA_STATUSCODE_GOOD) {
                UA_free(dst->innerDiagnosticInfo);
                dst->innerDiagnosticInfo = UA_NULL;
                retval |= UA_STATUSCODE_BADINTERNALERROR;
            }
        } else {
            retval |= UA_STATUSCODE_BADOUTOFMEMORY;
        }
    }
    if(retval != UA_STATUSCODE_GOOD)
        UA_DiagnosticInfo_deleteMembers(dst);
    return retval;
}

/********************/
/* Structured Types */
/********************/

size_t UA_calcSizeBinary(const void *p, const UA_DataType *dataType) {
    size_t size = 0;
    uintptr_t ptr = (uintptr_t)p;
    UA_Byte membersSize = dataType->membersSize;
    for(size_t i = 0; i < membersSize; i++) {
        const UA_DataTypeMember *member = &dataType->members[i];
        const UA_DataType *memberType;
        if(member->namespaceZero)
            memberType = &UA_TYPES[member->memberTypeIndex];
        else
            memberType = &dataType[member->memberTypeIndex - dataType->typeIndex];

        if(member->isArray) {
            ptr += (member->padding >> 3);
            const UA_Int32 noElements = *((const UA_Int32*)ptr);
            ptr += sizeof(UA_Int32) + (member->padding & 0x07);
            size += UA_Array_calcSizeBinary(*(void * const *)ptr, noElements, memberType);
            ptr += sizeof(void*);
            continue;
        }

        ptr += member->padding;
        if(!member->namespaceZero) {
            size += UA_calcSizeBinary((const void*)ptr, memberType);
            ptr += memberType->memSize;
            continue;
        }

        switch(member->memberTypeIndex) {
        case UA_TYPES_BOOLEAN:
        case UA_TYPES_SBYTE:
        case UA_TYPES_BYTE:
            size += 1;
            break;
        case UA_TYPES_INT16:
        case UA_TYPES_UINT16:
            size += 2;
            break;
        case UA_TYPES_INT32:
        case UA_TYPES_UINT32:
        case UA_TYPES_STATUSCODE:
        case UA_TYPES_FLOAT:
            size += 4;
            break;
        case UA_TYPES_INT64:
        case UA_TYPES_UINT64:
        case UA_TYPES_DOUBLE:
        case UA_TYPES_DATETIME:
            size += 8;
            break;
        case UA_TYPES_GUID:
            size += 16;
            break;
        case UA_TYPES_NODEID:
            size += UA_NodeId_calcSizeBinary((const UA_NodeId*)ptr);
            break;
        case UA_TYPES_EXPANDEDNODEID:
            size += UA_ExpandedNodeId_calcSizeBinary((const UA_ExpandedNodeId*)ptr);
            break;
        case UA_TYPES_LOCALIZEDTEXT:
            size += UA_LocalizedText_calcSizeBinary((const UA_LocalizedText*)ptr);
            break;
        case UA_TYPES_EXTENSIONOBJECT:
            size += UA_ExtensionObject_calcSizeBinary((const UA_ExtensionObject*)ptr);
            break;
        case UA_TYPES_DATAVALUE:
            size += UA_DataValue_calcSizeBinary((const UA_DataValue*)ptr);
            break;
        case UA_TYPES_VARIANT:
            size += UA_Variant_calcSizeBinary((const UA_Variant*)ptr);
            break;
        case UA_TYPES_DIAGNOSTICINFO:
            size += UA_DiagnosticInfo_calcSizeBinary((const UA_DiagnosticInfo*)ptr);
            break;
        default:
            // UA_TYPES_QUALIFIEDNAME, UA_TYPES_STRING, UA_TYPES_BYTESTRING, UA_TYPES_XMLELEMENT:
            size += UA_calcSizeBinary((const void*)ptr, memberType);
        }
        ptr += memberType->memSize;
    }
    return size;
}

UA_StatusCode UA_encodeBinary(const void *src, const UA_DataType *dataType, UA_ByteString *dst,
                              size_t *restrict offset) {
    uintptr_t ptr = (uintptr_t)src;
    UA_StatusCode retval = UA_STATUSCODE_GOOD;
    UA_Byte membersSize = dataType->membersSize;
    for(size_t i = 0; i < membersSize && retval == UA_STATUSCODE_GOOD; i++) {
        const UA_DataTypeMember *member = &dataType->members[i];
        const UA_DataType *memberType;
        if(member->namespaceZero)
            memberType = &UA_TYPES[member->memberTypeIndex];
        else
            memberType = &dataType[member->memberTypeIndex - dataType->typeIndex];

        if(member->isArray) {
            ptr += (member->padding >> 3);
            const UA_Int32 noElements = *((const UA_Int32*)ptr);
            ptr += sizeof(UA_Int32) + (member->padding & 0x07);
            retval = UA_Array_encodeBinary(*(void * const *)ptr, noElements, memberType, dst, offset);
            ptr += sizeof(void*);
            continue;
        }

        ptr += member->padding;
        if(!member->namespaceZero) {
            UA_encodeBinary((const void*)ptr, memberType, dst, offset);
            ptr += memberType->memSize;
            continue;
        }
        
        switch(member->memberTypeIndex) {
        case UA_TYPES_BOOLEAN:
        case UA_TYPES_SBYTE:
        case UA_TYPES_BYTE:
            retval = UA_Byte_encodeBinary((const UA_Byte*)ptr, dst, offset);
            break;
        case UA_TYPES_INT16:
        case UA_TYPES_UINT16:
            retval = UA_UInt16_encodeBinary((const UA_UInt16*)ptr, dst, offset);
            break;
        case UA_TYPES_INT32:
        case UA_TYPES_UINT32:
        case UA_TYPES_FLOAT:
        case UA_TYPES_STATUSCODE:
            retval = UA_UInt32_encodeBinary((const UA_UInt32*)ptr, dst, offset);
            break;
        case UA_TYPES_INT64:
        case UA_TYPES_UINT64:
        case UA_TYPES_DOUBLE:
        case UA_TYPES_DATETIME:
            retval = UA_UInt64_encodeBinary((const UA_UInt64*)ptr, dst, offset);
            break;
        case UA_TYPES_GUID:
            retval = UA_Guid_encodeBinary((const UA_Guid*)ptr, dst, offset);
            break;
        case UA_TYPES_NODEID:
            retval = UA_NodeId_encodeBinary((const UA_NodeId*)ptr, dst, offset);
            break;
        case UA_TYPES_EXPANDEDNODEID:
            retval = UA_ExpandedNodeId_encodeBinary((const UA_ExpandedNodeId*)ptr, dst, offset);
            break;
        case UA_TYPES_LOCALIZEDTEXT:
            retval = UA_LocalizedText_encodeBinary((const UA_LocalizedText*)ptr, dst, offset);
            break;
        case UA_TYPES_EXTENSIONOBJECT:
            retval = UA_ExtensionObject_encodeBinary((const UA_ExtensionObject*)ptr, dst, offset);
            break;
        case UA_TYPES_DATAVALUE:
            retval = UA_DataValue_encodeBinary((const UA_DataValue*)ptr, dst, offset);
            break;
        case UA_TYPES_VARIANT:
            retval = UA_Variant_encodeBinary((const UA_Variant*)ptr, dst, offset);
            break;
        case UA_TYPES_DIAGNOSTICINFO:
            retval = UA_DiagnosticInfo_encodeBinary((const UA_DiagnosticInfo*)ptr, dst, offset);
            break;
        default:
            // UA_TYPES_QUALIFIEDNAME, UA_TYPES_STRING, UA_TYPES_BYTESTRING, UA_TYPES_XMLELEMENT:
            retval = UA_encodeBinary((const void*)ptr, memberType, dst, offset);
        }
        ptr += memberType->memSize;
    }
    return retval;
}

UA_StatusCode UA_decodeBinary(const UA_ByteString *src, size_t *restrict offset, void *dst,
                              const UA_DataType *dataType) {
    UA_init(dst, dataType);
    uintptr_t ptr = (uintptr_t)dst;
    UA_StatusCode retval = UA_STATUSCODE_GOOD;
    UA_Byte membersSize = dataType->membersSize;
    for(size_t i = 0; i < membersSize && retval == UA_STATUSCODE_GOOD; i++) {
        const UA_DataTypeMember *member = &dataType->members[i];
        const UA_DataType *memberType;
        if(member->namespaceZero)
            memberType = &UA_TYPES[member->memberTypeIndex];
        else
            memberType = &dataType[member->memberTypeIndex - dataType->typeIndex] ;

        if(member->isArray) {
            ptr += (member->padding >> 3);
            UA_Int32 *noElements = (UA_Int32*)ptr;
            UA_Int32 tempNoElements = 0;
            retval |= UA_Int32_decodeBinary(src, offset, &tempNoElements);
            if(retval)
                continue;
            ptr += sizeof(UA_Int32) + (member->padding & 0x07);
            retval = UA_Array_decodeBinary(src, offset, tempNoElements, (void**)ptr, memberType);
            if(retval == UA_STATUSCODE_GOOD)
                *noElements = tempNoElements;
            ptr += sizeof(void*);
            continue;
        }

        ptr += member->padding;
        if(!member->namespaceZero) {
            UA_decodeBinary(src, offset, (void*)ptr, memberType);
            ptr += memberType->memSize;
            continue;
        }

        switch(member->memberTypeIndex) {
        case UA_TYPES_BOOLEAN:
        case UA_TYPES_SBYTE:
        case UA_TYPES_BYTE:
            retval = UA_Byte_decodeBinary(src, offset, (UA_Byte*)ptr);
            break;
        case UA_TYPES_INT16:
        case UA_TYPES_UINT16:
            retval = UA_Int16_decodeBinary(src, offset, (UA_Int16*)ptr);
            break;
        case UA_TYPES_INT32:
        case UA_TYPES_UINT32:
        case UA_TYPES_FLOAT:
        case UA_TYPES_STATUSCODE:
            retval = UA_UInt32_decodeBinary(src, offset, (UA_UInt32*)ptr);
            break;
        case UA_TYPES_INT64:
        case UA_TYPES_UINT64:
        case UA_TYPES_DOUBLE:
        case UA_TYPES_DATETIME:
            retval = UA_UInt64_decodeBinary(src, offset, (UA_UInt64*)ptr);
            break;
        case UA_TYPES_GUID:
            retval = UA_Guid_decodeBinary(src, offset, (UA_Guid*)ptr);
            break;
        case UA_TYPES_NODEID:
            retval = UA_NodeId_decodeBinary(src, offset, (UA_NodeId*)ptr);
            break;
        case UA_TYPES_EXPANDEDNODEID:
            retval = UA_ExpandedNodeId_decodeBinary(src, offset, (UA_ExpandedNodeId*)ptr);
            break;
        case UA_TYPES_LOCALIZEDTEXT:
            retval = UA_LocalizedText_decodeBinary(src, offset, (UA_LocalizedText*)ptr);
            break;
        case UA_TYPES_EXTENSIONOBJECT:
            retval = UA_ExtensionObject_decodeBinary(src, offset, (UA_ExtensionObject*)ptr);
            break;
        case UA_TYPES_DATAVALUE:
            retval = UA_DataValue_decodeBinary(src, offset, (UA_DataValue*)ptr);
            break;
        case UA_TYPES_VARIANT:
            retval = UA_Variant_decodeBinary(src, offset, (UA_Variant*)ptr);
            break;
        case UA_TYPES_DIAGNOSTICINFO:
            retval = UA_DiagnosticInfo_decodeBinary(src, offset, (UA_DiagnosticInfo*)ptr);
            break;
        default:
            // UA_TYPES_QUALIFIEDNAME, UA_TYPES_STRING, UA_TYPES_BYTESTRING, UA_TYPES_XMLELEMENT:
            retval = UA_decodeBinary(src, offset, (void*)ptr, memberType);
        }
        ptr += memberType->memSize;
    }
    if(retval != UA_STATUSCODE_GOOD)
        UA_deleteMembers(dst, dataType);
    return retval;
}
/*********************************** amalgamated original file "C:/AcpltDevelopmentKit/acplt/dev/open62541WrapperProject/open62541/src_generated/ua_types_generated.c" ***********************************/
/**
* @file ua_types_generated.c
*
* @brief Autogenerated data types
*
* Generated from Opc.Ua.Types.bsd with script C:/AcpltDevelopmentKit/acplt/dev/open62541WrapperProject/open62541/tools/generate_datatypes.py
* on host SPECTRE by user lars at 2015-05-06 12:57:37
*/


const UA_DataType *UA_TYPES = (UA_DataType[]){

/* UA_Boolean */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 1}, .memSize = sizeof(UA_Boolean), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_BOOLEAN,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_BOOLEAN },

/* UA_SByte */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 2}, .memSize = sizeof(UA_SByte), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_SBYTE,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_SBYTE },

/* UA_Byte */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 3}, .memSize = sizeof(UA_Byte), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_BYTE,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_BYTE },

/* UA_Int16 */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 4}, .memSize = sizeof(UA_Int16), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_INT16,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_INT16 },

/* UA_UInt16 */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 5}, .memSize = sizeof(UA_UInt16), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_UINT16,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_UINT16 },

/* UA_Int32 */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 6}, .memSize = sizeof(UA_Int32), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_INT32,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_INT32 },

/* UA_UInt32 */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 7}, .memSize = sizeof(UA_UInt32), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_UINT32,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_UINT32 },

/* UA_Int64 */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 8}, .memSize = sizeof(UA_Int64), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_INT64,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_INT64 },

/* UA_UInt64 */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 9}, .memSize = sizeof(UA_UInt64), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_UINT64,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_UINT64 },

/* UA_Float */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 10}, .memSize = sizeof(UA_Float), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_FLOAT,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_FLOAT },

/* UA_Double */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 11}, .memSize = sizeof(UA_Double), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_DOUBLE,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_DOUBLE },

/* UA_String */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 12}, .memSize = sizeof(UA_String), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_BYTE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_String, data) - sizeof(UA_Int32), .isArray = UA_TRUE }}, .typeIndex = UA_TYPES_STRING },

/* UA_DateTime */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 13}, .memSize = sizeof(UA_DateTime), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_DATETIME,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_DATETIME },

/* UA_Guid */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 14}, .memSize = sizeof(UA_Guid), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_FALSE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_GUID,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_GUID },

/* UA_ByteString */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 15}, .memSize = sizeof(UA_ByteString), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_BYTE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_String, data) - sizeof(UA_Int32), .isArray = UA_TRUE }}, .typeIndex = UA_TYPES_BYTESTRING },

/* UA_XmlElement */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 16}, .memSize = sizeof(UA_XmlElement), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_BYTE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_String, data) - sizeof(UA_Int32), .isArray = UA_TRUE }}, .typeIndex = UA_TYPES_XMLELEMENT },

/* UA_NodeId */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 17}, .memSize = sizeof(UA_NodeId), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_NODEID,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_NODEID },

/* UA_ExpandedNodeId */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 18}, .memSize = sizeof(UA_ExpandedNodeId), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_EXPANDEDNODEID,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_EXPANDEDNODEID },

/* UA_StatusCode */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 19}, .memSize = sizeof(UA_StatusCode), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_STATUSCODE,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_STATUSCODE },

/* UA_QualifiedName */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 20}, .memSize = sizeof(UA_QualifiedName), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .membersSize = 2, .members = {
	{.memberTypeIndex = UA_TYPES_UINT16, .namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE },
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_QualifiedName, name) - sizeof(UA_UInt16), .isArray = UA_FALSE }},
.typeIndex = UA_TYPES_QUALIFIEDNAME },

/* UA_LocalizedText */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 21}, .memSize = sizeof(UA_LocalizedText), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_LOCALIZEDTEXT,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_LOCALIZEDTEXT },

/* UA_ExtensionObject */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 22}, .memSize = sizeof(UA_ExtensionObject), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_EXTENSIONOBJECT,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_EXTENSIONOBJECT },

/* UA_DataValue */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 23}, .memSize = sizeof(UA_DataValue), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_DATAVALUE,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_DATAVALUE },

/* UA_Variant */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 24}, .memSize = sizeof(UA_Variant), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_VARIANT,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_VARIANT },

/* UA_DiagnosticInfo */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 25}, .memSize = sizeof(UA_DiagnosticInfo), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_DIAGNOSTICINFO,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_DIAGNOSTICINFO },

/* UA_IdType */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 256}, .memSize = sizeof(UA_IdType), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_INT32,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_INT32 },

/* UA_NodeClass */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 257}, .memSize = sizeof(UA_NodeClass), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_INT32,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_INT32 },

/* UA_ReferenceNode */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 285}, .memSize = sizeof(UA_ReferenceNode), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_REFERENCENODE, .membersSize = 3,
	.members={
	{.memberTypeIndex = UA_TYPES_NODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReferenceNode, referenceTypeId) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BOOLEAN, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReferenceNode, isInverse) - (offsetof(UA_ReferenceNode, referenceTypeId) + sizeof(UA_NodeId)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_EXPANDEDNODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReferenceNode, targetId) - (offsetof(UA_ReferenceNode, isInverse) + sizeof(UA_Boolean)), .isArray = UA_FALSE }, }},

/* UA_ApplicationType */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 307}, .memSize = sizeof(UA_ApplicationType), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_INT32,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_INT32 },

/* UA_ApplicationDescription */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 308}, .memSize = sizeof(UA_ApplicationDescription), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_APPLICATIONDESCRIPTION, .membersSize = 7,
	.members={
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ApplicationDescription, applicationUri) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ApplicationDescription, productUri) - (offsetof(UA_ApplicationDescription, applicationUri) + sizeof(UA_String)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_LOCALIZEDTEXT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ApplicationDescription, applicationName) - (offsetof(UA_ApplicationDescription, productUri) + sizeof(UA_String)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_APPLICATIONTYPE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ApplicationDescription, applicationType) - (offsetof(UA_ApplicationDescription, applicationName) + sizeof(UA_LocalizedText)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ApplicationDescription, gatewayServerUri) - (offsetof(UA_ApplicationDescription, applicationType) + sizeof(UA_ApplicationType)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ApplicationDescription, discoveryProfileUri) - (offsetof(UA_ApplicationDescription, gatewayServerUri) + sizeof(UA_String)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_ApplicationDescription, discoveryUrlsSize) - (offsetof(UA_ApplicationDescription, discoveryProfileUri) + sizeof(UA_String))) << 3) + (offsetof(UA_ApplicationDescription, discoveryUrls) - sizeof(UA_Int32) - offsetof(UA_ApplicationDescription, discoveryUrlsSize)), .isArray = UA_TRUE }, }},

/* UA_RequestHeader */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 389}, .memSize = sizeof(UA_RequestHeader), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_REQUESTHEADER, .membersSize = 7,
	.members={
	{.memberTypeIndex = UA_TYPES_NODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_RequestHeader, authenticationToken) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_DATETIME, .namespaceZero = UA_TRUE, .padding = offsetof(UA_RequestHeader, timestamp) - (offsetof(UA_RequestHeader, authenticationToken) + sizeof(UA_NodeId)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_RequestHeader, requestHandle) - (offsetof(UA_RequestHeader, timestamp) + sizeof(UA_DateTime)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_RequestHeader, returnDiagnostics) - (offsetof(UA_RequestHeader, requestHandle) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_RequestHeader, auditEntryId) - (offsetof(UA_RequestHeader, returnDiagnostics) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_RequestHeader, timeoutHint) - (offsetof(UA_RequestHeader, auditEntryId) + sizeof(UA_String)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_EXTENSIONOBJECT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_RequestHeader, additionalHeader) - (offsetof(UA_RequestHeader, timeoutHint) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, }},

/* UA_ResponseHeader */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 392}, .memSize = sizeof(UA_ResponseHeader), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_RESPONSEHEADER, .membersSize = 6,
	.members={
	{.memberTypeIndex = UA_TYPES_DATETIME, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ResponseHeader, timestamp) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ResponseHeader, requestHandle) - (offsetof(UA_ResponseHeader, timestamp) + sizeof(UA_DateTime)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STATUSCODE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ResponseHeader, serviceResult) - (offsetof(UA_ResponseHeader, requestHandle) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_DIAGNOSTICINFO, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ResponseHeader, serviceDiagnostics) - (offsetof(UA_ResponseHeader, serviceResult) + sizeof(UA_StatusCode)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_ResponseHeader, stringTableSize) - (offsetof(UA_ResponseHeader, serviceDiagnostics) + sizeof(UA_DiagnosticInfo))) << 3) + (offsetof(UA_ResponseHeader, stringTable) - sizeof(UA_Int32) - offsetof(UA_ResponseHeader, stringTableSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_EXTENSIONOBJECT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ResponseHeader, additionalHeader) - (offsetof(UA_ResponseHeader, stringTable) + sizeof(void*)), .isArray = UA_FALSE }, }},

/* UA_FindServersRequest */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 420}, .memSize = sizeof(UA_FindServersRequest), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_FINDSERVERSREQUEST, .membersSize = 4,
	.members={
	{.memberTypeIndex = UA_TYPES_REQUESTHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_FindServersRequest, requestHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_FindServersRequest, endpointUrl) - (offsetof(UA_FindServersRequest, requestHeader) + sizeof(UA_RequestHeader)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_FindServersRequest, localeIdsSize) - (offsetof(UA_FindServersRequest, endpointUrl) + sizeof(UA_String))) << 3) + (offsetof(UA_FindServersRequest, localeIds) - sizeof(UA_Int32) - offsetof(UA_FindServersRequest, localeIdsSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_FindServersRequest, serverUrisSize) - (offsetof(UA_FindServersRequest, localeIds) + sizeof(void*))) << 3) + (offsetof(UA_FindServersRequest, serverUris) - sizeof(UA_Int32) - offsetof(UA_FindServersRequest, serverUrisSize)), .isArray = UA_TRUE }, }},

/* UA_FindServersResponse */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 423}, .memSize = sizeof(UA_FindServersResponse), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_FINDSERVERSRESPONSE, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_RESPONSEHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_FindServersResponse, responseHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_APPLICATIONDESCRIPTION, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_FindServersResponse, serversSize) - (offsetof(UA_FindServersResponse, responseHeader) + sizeof(UA_ResponseHeader))) << 3) + (offsetof(UA_FindServersResponse, servers) - sizeof(UA_Int32) - offsetof(UA_FindServersResponse, serversSize)), .isArray = UA_TRUE }, }},

/* UA_MessageSecurityMode */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 302}, .memSize = sizeof(UA_MessageSecurityMode), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_INT32,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_INT32 },

/* UA_UserTokenType */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 303}, .memSize = sizeof(UA_UserTokenType), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_INT32,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_INT32 },

/* UA_UserTokenPolicy */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 304}, .memSize = sizeof(UA_UserTokenPolicy), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_USERTOKENPOLICY, .membersSize = 5,
	.members={
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_UserTokenPolicy, policyId) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_USERTOKENTYPE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_UserTokenPolicy, tokenType) - (offsetof(UA_UserTokenPolicy, policyId) + sizeof(UA_String)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_UserTokenPolicy, issuedTokenType) - (offsetof(UA_UserTokenPolicy, tokenType) + sizeof(UA_UserTokenType)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_UserTokenPolicy, issuerEndpointUrl) - (offsetof(UA_UserTokenPolicy, issuedTokenType) + sizeof(UA_String)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_UserTokenPolicy, securityPolicyUri) - (offsetof(UA_UserTokenPolicy, issuerEndpointUrl) + sizeof(UA_String)), .isArray = UA_FALSE }, }},

/* UA_EndpointDescription */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 312}, .memSize = sizeof(UA_EndpointDescription), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_ENDPOINTDESCRIPTION, .membersSize = 8,
	.members={
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_EndpointDescription, endpointUrl) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_APPLICATIONDESCRIPTION, .namespaceZero = UA_TRUE, .padding = offsetof(UA_EndpointDescription, server) - (offsetof(UA_EndpointDescription, endpointUrl) + sizeof(UA_String)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BYTESTRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_EndpointDescription, serverCertificate) - (offsetof(UA_EndpointDescription, server) + sizeof(UA_ApplicationDescription)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_MESSAGESECURITYMODE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_EndpointDescription, securityMode) - (offsetof(UA_EndpointDescription, serverCertificate) + sizeof(UA_ByteString)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_EndpointDescription, securityPolicyUri) - (offsetof(UA_EndpointDescription, securityMode) + sizeof(UA_MessageSecurityMode)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_USERTOKENPOLICY, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_EndpointDescription, userIdentityTokensSize) - (offsetof(UA_EndpointDescription, securityPolicyUri) + sizeof(UA_String))) << 3) + (offsetof(UA_EndpointDescription, userIdentityTokens) - sizeof(UA_Int32) - offsetof(UA_EndpointDescription, userIdentityTokensSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_EndpointDescription, transportProfileUri) - (offsetof(UA_EndpointDescription, userIdentityTokens) + sizeof(void*)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BYTE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_EndpointDescription, securityLevel) - (offsetof(UA_EndpointDescription, transportProfileUri) + sizeof(UA_String)), .isArray = UA_FALSE }, }},

/* UA_GetEndpointsRequest */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 426}, .memSize = sizeof(UA_GetEndpointsRequest), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_GETENDPOINTSREQUEST, .membersSize = 4,
	.members={
	{.memberTypeIndex = UA_TYPES_REQUESTHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_GetEndpointsRequest, requestHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_GetEndpointsRequest, endpointUrl) - (offsetof(UA_GetEndpointsRequest, requestHeader) + sizeof(UA_RequestHeader)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_GetEndpointsRequest, localeIdsSize) - (offsetof(UA_GetEndpointsRequest, endpointUrl) + sizeof(UA_String))) << 3) + (offsetof(UA_GetEndpointsRequest, localeIds) - sizeof(UA_Int32) - offsetof(UA_GetEndpointsRequest, localeIdsSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_GetEndpointsRequest, profileUrisSize) - (offsetof(UA_GetEndpointsRequest, localeIds) + sizeof(void*))) << 3) + (offsetof(UA_GetEndpointsRequest, profileUris) - sizeof(UA_Int32) - offsetof(UA_GetEndpointsRequest, profileUrisSize)), .isArray = UA_TRUE }, }},

/* UA_GetEndpointsResponse */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 429}, .memSize = sizeof(UA_GetEndpointsResponse), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_GETENDPOINTSRESPONSE, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_RESPONSEHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_GetEndpointsResponse, responseHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_ENDPOINTDESCRIPTION, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_GetEndpointsResponse, endpointsSize) - (offsetof(UA_GetEndpointsResponse, responseHeader) + sizeof(UA_ResponseHeader))) << 3) + (offsetof(UA_GetEndpointsResponse, endpoints) - sizeof(UA_Int32) - offsetof(UA_GetEndpointsResponse, endpointsSize)), .isArray = UA_TRUE }, }},

/* UA_SecurityTokenRequestType */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 315}, .memSize = sizeof(UA_SecurityTokenRequestType), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_INT32,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_INT32 },

/* UA_ChannelSecurityToken */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 441}, .memSize = sizeof(UA_ChannelSecurityToken), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = sizeof(UA_ChannelSecurityToken) == 20, .typeIndex = UA_TYPES_CHANNELSECURITYTOKEN, .membersSize = 4,
	.members={
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ChannelSecurityToken, channelId) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ChannelSecurityToken, tokenId) - (offsetof(UA_ChannelSecurityToken, channelId) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_DATETIME, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ChannelSecurityToken, createdAt) - (offsetof(UA_ChannelSecurityToken, tokenId) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ChannelSecurityToken, revisedLifetime) - (offsetof(UA_ChannelSecurityToken, createdAt) + sizeof(UA_DateTime)), .isArray = UA_FALSE }, }},

/* UA_OpenSecureChannelRequest */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 444}, .memSize = sizeof(UA_OpenSecureChannelRequest), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_OPENSECURECHANNELREQUEST, .membersSize = 6,
	.members={
	{.memberTypeIndex = UA_TYPES_REQUESTHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_OpenSecureChannelRequest, requestHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_OpenSecureChannelRequest, clientProtocolVersion) - (offsetof(UA_OpenSecureChannelRequest, requestHeader) + sizeof(UA_RequestHeader)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_SECURITYTOKENREQUESTTYPE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_OpenSecureChannelRequest, requestType) - (offsetof(UA_OpenSecureChannelRequest, clientProtocolVersion) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_MESSAGESECURITYMODE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_OpenSecureChannelRequest, securityMode) - (offsetof(UA_OpenSecureChannelRequest, requestType) + sizeof(UA_SecurityTokenRequestType)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BYTESTRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_OpenSecureChannelRequest, clientNonce) - (offsetof(UA_OpenSecureChannelRequest, securityMode) + sizeof(UA_MessageSecurityMode)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_OpenSecureChannelRequest, requestedLifetime) - (offsetof(UA_OpenSecureChannelRequest, clientNonce) + sizeof(UA_ByteString)), .isArray = UA_FALSE }, }},

/* UA_OpenSecureChannelResponse */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 447}, .memSize = sizeof(UA_OpenSecureChannelResponse), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_OPENSECURECHANNELRESPONSE, .membersSize = 4,
	.members={
	{.memberTypeIndex = UA_TYPES_RESPONSEHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_OpenSecureChannelResponse, responseHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_OpenSecureChannelResponse, serverProtocolVersion) - (offsetof(UA_OpenSecureChannelResponse, responseHeader) + sizeof(UA_ResponseHeader)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_CHANNELSECURITYTOKEN, .namespaceZero = UA_TRUE, .padding = offsetof(UA_OpenSecureChannelResponse, securityToken) - (offsetof(UA_OpenSecureChannelResponse, serverProtocolVersion) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BYTESTRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_OpenSecureChannelResponse, serverNonce) - (offsetof(UA_OpenSecureChannelResponse, securityToken) + sizeof(UA_ChannelSecurityToken)), .isArray = UA_FALSE }, }},

/* UA_CloseSecureChannelRequest */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 450}, .memSize = sizeof(UA_CloseSecureChannelRequest), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_CLOSESECURECHANNELREQUEST, .membersSize = 1,
	.members={
	{.memberTypeIndex = UA_TYPES_REQUESTHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CloseSecureChannelRequest, requestHeader) - 0, .isArray = UA_FALSE }, }},

/* UA_CloseSecureChannelResponse */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 453}, .memSize = sizeof(UA_CloseSecureChannelResponse), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_CLOSESECURECHANNELRESPONSE, .membersSize = 1,
	.members={
	{.memberTypeIndex = UA_TYPES_RESPONSEHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CloseSecureChannelResponse, responseHeader) - 0, .isArray = UA_FALSE }, }},

/* UA_SignedSoftwareCertificate */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 344}, .memSize = sizeof(UA_SignedSoftwareCertificate), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_SIGNEDSOFTWARECERTIFICATE, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_BYTESTRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_SignedSoftwareCertificate, certificateData) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BYTESTRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_SignedSoftwareCertificate, signature) - (offsetof(UA_SignedSoftwareCertificate, certificateData) + sizeof(UA_ByteString)), .isArray = UA_FALSE }, }},

/* UA_SignatureData */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 456}, .memSize = sizeof(UA_SignatureData), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_SIGNATUREDATA, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_SignatureData, algorithm) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BYTESTRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_SignatureData, signature) - (offsetof(UA_SignatureData, algorithm) + sizeof(UA_String)), .isArray = UA_FALSE }, }},

/* UA_CreateSessionRequest */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 459}, .memSize = sizeof(UA_CreateSessionRequest), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_CREATESESSIONREQUEST, .membersSize = 9,
	.members={
	{.memberTypeIndex = UA_TYPES_REQUESTHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSessionRequest, requestHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_APPLICATIONDESCRIPTION, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSessionRequest, clientDescription) - (offsetof(UA_CreateSessionRequest, requestHeader) + sizeof(UA_RequestHeader)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSessionRequest, serverUri) - (offsetof(UA_CreateSessionRequest, clientDescription) + sizeof(UA_ApplicationDescription)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSessionRequest, endpointUrl) - (offsetof(UA_CreateSessionRequest, serverUri) + sizeof(UA_String)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSessionRequest, sessionName) - (offsetof(UA_CreateSessionRequest, endpointUrl) + sizeof(UA_String)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BYTESTRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSessionRequest, clientNonce) - (offsetof(UA_CreateSessionRequest, sessionName) + sizeof(UA_String)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BYTESTRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSessionRequest, clientCertificate) - (offsetof(UA_CreateSessionRequest, clientNonce) + sizeof(UA_ByteString)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_DOUBLE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSessionRequest, requestedSessionTimeout) - (offsetof(UA_CreateSessionRequest, clientCertificate) + sizeof(UA_ByteString)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSessionRequest, maxResponseMessageSize) - (offsetof(UA_CreateSessionRequest, requestedSessionTimeout) + sizeof(UA_Double)), .isArray = UA_FALSE }, }},

/* UA_CreateSessionResponse */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 462}, .memSize = sizeof(UA_CreateSessionResponse), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_CREATESESSIONRESPONSE, .membersSize = 10,
	.members={
	{.memberTypeIndex = UA_TYPES_RESPONSEHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSessionResponse, responseHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_NODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSessionResponse, sessionId) - (offsetof(UA_CreateSessionResponse, responseHeader) + sizeof(UA_ResponseHeader)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_NODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSessionResponse, authenticationToken) - (offsetof(UA_CreateSessionResponse, sessionId) + sizeof(UA_NodeId)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_DOUBLE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSessionResponse, revisedSessionTimeout) - (offsetof(UA_CreateSessionResponse, authenticationToken) + sizeof(UA_NodeId)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BYTESTRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSessionResponse, serverNonce) - (offsetof(UA_CreateSessionResponse, revisedSessionTimeout) + sizeof(UA_Double)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BYTESTRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSessionResponse, serverCertificate) - (offsetof(UA_CreateSessionResponse, serverNonce) + sizeof(UA_ByteString)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_ENDPOINTDESCRIPTION, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_CreateSessionResponse, serverEndpointsSize) - (offsetof(UA_CreateSessionResponse, serverCertificate) + sizeof(UA_ByteString))) << 3) + (offsetof(UA_CreateSessionResponse, serverEndpoints) - sizeof(UA_Int32) - offsetof(UA_CreateSessionResponse, serverEndpointsSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_SIGNEDSOFTWARECERTIFICATE, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_CreateSessionResponse, serverSoftwareCertificatesSize) - (offsetof(UA_CreateSessionResponse, serverEndpoints) + sizeof(void*))) << 3) + (offsetof(UA_CreateSessionResponse, serverSoftwareCertificates) - sizeof(UA_Int32) - offsetof(UA_CreateSessionResponse, serverSoftwareCertificatesSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_SIGNATUREDATA, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSessionResponse, serverSignature) - (offsetof(UA_CreateSessionResponse, serverSoftwareCertificates) + sizeof(void*)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSessionResponse, maxRequestMessageSize) - (offsetof(UA_CreateSessionResponse, serverSignature) + sizeof(UA_SignatureData)), .isArray = UA_FALSE }, }},

/* UA_UserIdentityToken */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 316}, .memSize = sizeof(UA_UserIdentityToken), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_USERIDENTITYTOKEN, .membersSize = 1,
	.members={
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_UserIdentityToken, policyId) - 0, .isArray = UA_FALSE }, }},

/* UA_AnonymousIdentityToken */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 319}, .memSize = sizeof(UA_AnonymousIdentityToken), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_ANONYMOUSIDENTITYTOKEN, .membersSize = 1,
	.members={
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_AnonymousIdentityToken, policyId) - 0, .isArray = UA_FALSE }, }},

/* UA_UserNameIdentityToken */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 322}, .memSize = sizeof(UA_UserNameIdentityToken), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_USERNAMEIDENTITYTOKEN, .membersSize = 4,
	.members={
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_UserNameIdentityToken, policyId) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_UserNameIdentityToken, userName) - (offsetof(UA_UserNameIdentityToken, policyId) + sizeof(UA_String)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BYTESTRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_UserNameIdentityToken, password) - (offsetof(UA_UserNameIdentityToken, userName) + sizeof(UA_String)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_UserNameIdentityToken, encryptionAlgorithm) - (offsetof(UA_UserNameIdentityToken, password) + sizeof(UA_ByteString)), .isArray = UA_FALSE }, }},

/* UA_ActivateSessionRequest */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 465}, .memSize = sizeof(UA_ActivateSessionRequest), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_ACTIVATESESSIONREQUEST, .membersSize = 6,
	.members={
	{.memberTypeIndex = UA_TYPES_REQUESTHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ActivateSessionRequest, requestHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_SIGNATUREDATA, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ActivateSessionRequest, clientSignature) - (offsetof(UA_ActivateSessionRequest, requestHeader) + sizeof(UA_RequestHeader)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_SIGNEDSOFTWARECERTIFICATE, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_ActivateSessionRequest, clientSoftwareCertificatesSize) - (offsetof(UA_ActivateSessionRequest, clientSignature) + sizeof(UA_SignatureData))) << 3) + (offsetof(UA_ActivateSessionRequest, clientSoftwareCertificates) - sizeof(UA_Int32) - offsetof(UA_ActivateSessionRequest, clientSoftwareCertificatesSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_ActivateSessionRequest, localeIdsSize) - (offsetof(UA_ActivateSessionRequest, clientSoftwareCertificates) + sizeof(void*))) << 3) + (offsetof(UA_ActivateSessionRequest, localeIds) - sizeof(UA_Int32) - offsetof(UA_ActivateSessionRequest, localeIdsSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_EXTENSIONOBJECT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ActivateSessionRequest, userIdentityToken) - (offsetof(UA_ActivateSessionRequest, localeIds) + sizeof(void*)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_SIGNATUREDATA, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ActivateSessionRequest, userTokenSignature) - (offsetof(UA_ActivateSessionRequest, userIdentityToken) + sizeof(UA_ExtensionObject)), .isArray = UA_FALSE }, }},

/* UA_ActivateSessionResponse */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 468}, .memSize = sizeof(UA_ActivateSessionResponse), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_ACTIVATESESSIONRESPONSE, .membersSize = 4,
	.members={
	{.memberTypeIndex = UA_TYPES_RESPONSEHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ActivateSessionResponse, responseHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BYTESTRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ActivateSessionResponse, serverNonce) - (offsetof(UA_ActivateSessionResponse, responseHeader) + sizeof(UA_ResponseHeader)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STATUSCODE, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_ActivateSessionResponse, resultsSize) - (offsetof(UA_ActivateSessionResponse, serverNonce) + sizeof(UA_ByteString))) << 3) + (offsetof(UA_ActivateSessionResponse, results) - sizeof(UA_Int32) - offsetof(UA_ActivateSessionResponse, resultsSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_DIAGNOSTICINFO, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_ActivateSessionResponse, diagnosticInfosSize) - (offsetof(UA_ActivateSessionResponse, results) + sizeof(void*))) << 3) + (offsetof(UA_ActivateSessionResponse, diagnosticInfos) - sizeof(UA_Int32) - offsetof(UA_ActivateSessionResponse, diagnosticInfosSize)), .isArray = UA_TRUE }, }},

/* UA_CloseSessionRequest */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 471}, .memSize = sizeof(UA_CloseSessionRequest), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_CLOSESESSIONREQUEST, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_REQUESTHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CloseSessionRequest, requestHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BOOLEAN, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CloseSessionRequest, deleteSubscriptions) - (offsetof(UA_CloseSessionRequest, requestHeader) + sizeof(UA_RequestHeader)), .isArray = UA_FALSE }, }},

/* UA_CloseSessionResponse */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 474}, .memSize = sizeof(UA_CloseSessionResponse), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_CLOSESESSIONRESPONSE, .membersSize = 1,
	.members={
	{.memberTypeIndex = UA_TYPES_RESPONSEHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CloseSessionResponse, responseHeader) - 0, .isArray = UA_FALSE }, }},

/* UA_NodeAttributesMask */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 348}, .memSize = sizeof(UA_NodeAttributesMask), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_INT32,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_INT32 },

/* UA_NodeAttributes */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 349}, .memSize = sizeof(UA_NodeAttributes), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_NODEATTRIBUTES, .membersSize = 5,
	.members={
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_NodeAttributes, specifiedAttributes) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_LOCALIZEDTEXT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_NodeAttributes, displayName) - (offsetof(UA_NodeAttributes, specifiedAttributes) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_LOCALIZEDTEXT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_NodeAttributes, description) - (offsetof(UA_NodeAttributes, displayName) + sizeof(UA_LocalizedText)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_NodeAttributes, writeMask) - (offsetof(UA_NodeAttributes, description) + sizeof(UA_LocalizedText)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_NodeAttributes, userWriteMask) - (offsetof(UA_NodeAttributes, writeMask) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, }},

/* UA_ObjectAttributes */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 352}, .memSize = sizeof(UA_ObjectAttributes), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_OBJECTATTRIBUTES, .membersSize = 6,
	.members={
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ObjectAttributes, specifiedAttributes) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_LOCALIZEDTEXT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ObjectAttributes, displayName) - (offsetof(UA_ObjectAttributes, specifiedAttributes) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_LOCALIZEDTEXT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ObjectAttributes, description) - (offsetof(UA_ObjectAttributes, displayName) + sizeof(UA_LocalizedText)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ObjectAttributes, writeMask) - (offsetof(UA_ObjectAttributes, description) + sizeof(UA_LocalizedText)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ObjectAttributes, userWriteMask) - (offsetof(UA_ObjectAttributes, writeMask) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BYTE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ObjectAttributes, eventNotifier) - (offsetof(UA_ObjectAttributes, userWriteMask) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, }},

/* UA_VariableAttributes */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 355}, .memSize = sizeof(UA_VariableAttributes), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_VARIABLEATTRIBUTES, .membersSize = 13,
	.members={
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_VariableAttributes, specifiedAttributes) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_LOCALIZEDTEXT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_VariableAttributes, displayName) - (offsetof(UA_VariableAttributes, specifiedAttributes) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_LOCALIZEDTEXT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_VariableAttributes, description) - (offsetof(UA_VariableAttributes, displayName) + sizeof(UA_LocalizedText)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_VariableAttributes, writeMask) - (offsetof(UA_VariableAttributes, description) + sizeof(UA_LocalizedText)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_VariableAttributes, userWriteMask) - (offsetof(UA_VariableAttributes, writeMask) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_VARIANT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_VariableAttributes, value) - (offsetof(UA_VariableAttributes, userWriteMask) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_NODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_VariableAttributes, dataType) - (offsetof(UA_VariableAttributes, value) + sizeof(UA_Variant)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_INT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_VariableAttributes, valueRank) - (offsetof(UA_VariableAttributes, dataType) + sizeof(UA_NodeId)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_VariableAttributes, arrayDimensionsSize) - (offsetof(UA_VariableAttributes, valueRank) + sizeof(UA_Int32))) << 3) + (offsetof(UA_VariableAttributes, arrayDimensions) - sizeof(UA_Int32) - offsetof(UA_VariableAttributes, arrayDimensionsSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_BYTE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_VariableAttributes, accessLevel) - (offsetof(UA_VariableAttributes, arrayDimensions) + sizeof(void*)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BYTE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_VariableAttributes, userAccessLevel) - (offsetof(UA_VariableAttributes, accessLevel) + sizeof(UA_Byte)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_DOUBLE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_VariableAttributes, minimumSamplingInterval) - (offsetof(UA_VariableAttributes, userAccessLevel) + sizeof(UA_Byte)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BOOLEAN, .namespaceZero = UA_TRUE, .padding = offsetof(UA_VariableAttributes, historizing) - (offsetof(UA_VariableAttributes, minimumSamplingInterval) + sizeof(UA_Double)), .isArray = UA_FALSE }, }},

/* UA_ObjectTypeAttributes */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 361}, .memSize = sizeof(UA_ObjectTypeAttributes), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_OBJECTTYPEATTRIBUTES, .membersSize = 6,
	.members={
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ObjectTypeAttributes, specifiedAttributes) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_LOCALIZEDTEXT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ObjectTypeAttributes, displayName) - (offsetof(UA_ObjectTypeAttributes, specifiedAttributes) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_LOCALIZEDTEXT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ObjectTypeAttributes, description) - (offsetof(UA_ObjectTypeAttributes, displayName) + sizeof(UA_LocalizedText)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ObjectTypeAttributes, writeMask) - (offsetof(UA_ObjectTypeAttributes, description) + sizeof(UA_LocalizedText)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ObjectTypeAttributes, userWriteMask) - (offsetof(UA_ObjectTypeAttributes, writeMask) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BOOLEAN, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ObjectTypeAttributes, isAbstract) - (offsetof(UA_ObjectTypeAttributes, userWriteMask) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, }},

/* UA_ReferenceTypeAttributes */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 367}, .memSize = sizeof(UA_ReferenceTypeAttributes), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_REFERENCETYPEATTRIBUTES, .membersSize = 8,
	.members={
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReferenceTypeAttributes, specifiedAttributes) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_LOCALIZEDTEXT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReferenceTypeAttributes, displayName) - (offsetof(UA_ReferenceTypeAttributes, specifiedAttributes) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_LOCALIZEDTEXT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReferenceTypeAttributes, description) - (offsetof(UA_ReferenceTypeAttributes, displayName) + sizeof(UA_LocalizedText)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReferenceTypeAttributes, writeMask) - (offsetof(UA_ReferenceTypeAttributes, description) + sizeof(UA_LocalizedText)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReferenceTypeAttributes, userWriteMask) - (offsetof(UA_ReferenceTypeAttributes, writeMask) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BOOLEAN, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReferenceTypeAttributes, isAbstract) - (offsetof(UA_ReferenceTypeAttributes, userWriteMask) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BOOLEAN, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReferenceTypeAttributes, symmetric) - (offsetof(UA_ReferenceTypeAttributes, isAbstract) + sizeof(UA_Boolean)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_LOCALIZEDTEXT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReferenceTypeAttributes, inverseName) - (offsetof(UA_ReferenceTypeAttributes, symmetric) + sizeof(UA_Boolean)), .isArray = UA_FALSE }, }},

/* UA_ViewAttributes */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 373}, .memSize = sizeof(UA_ViewAttributes), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_VIEWATTRIBUTES, .membersSize = 7,
	.members={
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ViewAttributes, specifiedAttributes) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_LOCALIZEDTEXT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ViewAttributes, displayName) - (offsetof(UA_ViewAttributes, specifiedAttributes) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_LOCALIZEDTEXT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ViewAttributes, description) - (offsetof(UA_ViewAttributes, displayName) + sizeof(UA_LocalizedText)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ViewAttributes, writeMask) - (offsetof(UA_ViewAttributes, description) + sizeof(UA_LocalizedText)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ViewAttributes, userWriteMask) - (offsetof(UA_ViewAttributes, writeMask) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BOOLEAN, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ViewAttributes, containsNoLoops) - (offsetof(UA_ViewAttributes, userWriteMask) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BYTE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ViewAttributes, eventNotifier) - (offsetof(UA_ViewAttributes, containsNoLoops) + sizeof(UA_Boolean)), .isArray = UA_FALSE }, }},

/* UA_AddNodesItem */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 376}, .memSize = sizeof(UA_AddNodesItem), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_ADDNODESITEM, .membersSize = 7,
	.members={
	{.memberTypeIndex = UA_TYPES_EXPANDEDNODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_AddNodesItem, parentNodeId) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_NODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_AddNodesItem, referenceTypeId) - (offsetof(UA_AddNodesItem, parentNodeId) + sizeof(UA_ExpandedNodeId)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_EXPANDEDNODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_AddNodesItem, requestedNewNodeId) - (offsetof(UA_AddNodesItem, referenceTypeId) + sizeof(UA_NodeId)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_QUALIFIEDNAME, .namespaceZero = UA_TRUE, .padding = offsetof(UA_AddNodesItem, browseName) - (offsetof(UA_AddNodesItem, requestedNewNodeId) + sizeof(UA_ExpandedNodeId)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_NODECLASS, .namespaceZero = UA_TRUE, .padding = offsetof(UA_AddNodesItem, nodeClass) - (offsetof(UA_AddNodesItem, browseName) + sizeof(UA_QualifiedName)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_EXTENSIONOBJECT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_AddNodesItem, nodeAttributes) - (offsetof(UA_AddNodesItem, nodeClass) + sizeof(UA_NodeClass)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_EXPANDEDNODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_AddNodesItem, typeDefinition) - (offsetof(UA_AddNodesItem, nodeAttributes) + sizeof(UA_ExtensionObject)), .isArray = UA_FALSE }, }},

/* UA_AddNodesResult */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 483}, .memSize = sizeof(UA_AddNodesResult), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_ADDNODESRESULT, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_STATUSCODE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_AddNodesResult, statusCode) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_NODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_AddNodesResult, addedNodeId) - (offsetof(UA_AddNodesResult, statusCode) + sizeof(UA_StatusCode)), .isArray = UA_FALSE }, }},

/* UA_AddNodesRequest */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 486}, .memSize = sizeof(UA_AddNodesRequest), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_ADDNODESREQUEST, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_REQUESTHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_AddNodesRequest, requestHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_ADDNODESITEM, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_AddNodesRequest, nodesToAddSize) - (offsetof(UA_AddNodesRequest, requestHeader) + sizeof(UA_RequestHeader))) << 3) + (offsetof(UA_AddNodesRequest, nodesToAdd) - sizeof(UA_Int32) - offsetof(UA_AddNodesRequest, nodesToAddSize)), .isArray = UA_TRUE }, }},

/* UA_AddNodesResponse */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 489}, .memSize = sizeof(UA_AddNodesResponse), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_ADDNODESRESPONSE, .membersSize = 3,
	.members={
	{.memberTypeIndex = UA_TYPES_RESPONSEHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_AddNodesResponse, responseHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_ADDNODESRESULT, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_AddNodesResponse, resultsSize) - (offsetof(UA_AddNodesResponse, responseHeader) + sizeof(UA_ResponseHeader))) << 3) + (offsetof(UA_AddNodesResponse, results) - sizeof(UA_Int32) - offsetof(UA_AddNodesResponse, resultsSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_DIAGNOSTICINFO, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_AddNodesResponse, diagnosticInfosSize) - (offsetof(UA_AddNodesResponse, results) + sizeof(void*))) << 3) + (offsetof(UA_AddNodesResponse, diagnosticInfos) - sizeof(UA_Int32) - offsetof(UA_AddNodesResponse, diagnosticInfosSize)), .isArray = UA_TRUE }, }},

/* UA_AddReferencesItem */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 379}, .memSize = sizeof(UA_AddReferencesItem), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_ADDREFERENCESITEM, .membersSize = 6,
	.members={
	{.memberTypeIndex = UA_TYPES_NODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_AddReferencesItem, sourceNodeId) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_NODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_AddReferencesItem, referenceTypeId) - (offsetof(UA_AddReferencesItem, sourceNodeId) + sizeof(UA_NodeId)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BOOLEAN, .namespaceZero = UA_TRUE, .padding = offsetof(UA_AddReferencesItem, isForward) - (offsetof(UA_AddReferencesItem, referenceTypeId) + sizeof(UA_NodeId)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_AddReferencesItem, targetServerUri) - (offsetof(UA_AddReferencesItem, isForward) + sizeof(UA_Boolean)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_EXPANDEDNODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_AddReferencesItem, targetNodeId) - (offsetof(UA_AddReferencesItem, targetServerUri) + sizeof(UA_String)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_NODECLASS, .namespaceZero = UA_TRUE, .padding = offsetof(UA_AddReferencesItem, targetNodeClass) - (offsetof(UA_AddReferencesItem, targetNodeId) + sizeof(UA_ExpandedNodeId)), .isArray = UA_FALSE }, }},

/* UA_AddReferencesRequest */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 492}, .memSize = sizeof(UA_AddReferencesRequest), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_ADDREFERENCESREQUEST, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_REQUESTHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_AddReferencesRequest, requestHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_ADDREFERENCESITEM, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_AddReferencesRequest, referencesToAddSize) - (offsetof(UA_AddReferencesRequest, requestHeader) + sizeof(UA_RequestHeader))) << 3) + (offsetof(UA_AddReferencesRequest, referencesToAdd) - sizeof(UA_Int32) - offsetof(UA_AddReferencesRequest, referencesToAddSize)), .isArray = UA_TRUE }, }},

/* UA_AddReferencesResponse */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 495}, .memSize = sizeof(UA_AddReferencesResponse), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_ADDREFERENCESRESPONSE, .membersSize = 3,
	.members={
	{.memberTypeIndex = UA_TYPES_RESPONSEHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_AddReferencesResponse, responseHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STATUSCODE, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_AddReferencesResponse, resultsSize) - (offsetof(UA_AddReferencesResponse, responseHeader) + sizeof(UA_ResponseHeader))) << 3) + (offsetof(UA_AddReferencesResponse, results) - sizeof(UA_Int32) - offsetof(UA_AddReferencesResponse, resultsSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_DIAGNOSTICINFO, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_AddReferencesResponse, diagnosticInfosSize) - (offsetof(UA_AddReferencesResponse, results) + sizeof(void*))) << 3) + (offsetof(UA_AddReferencesResponse, diagnosticInfos) - sizeof(UA_Int32) - offsetof(UA_AddReferencesResponse, diagnosticInfosSize)), .isArray = UA_TRUE }, }},

/* UA_DeleteNodesItem */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 382}, .memSize = sizeof(UA_DeleteNodesItem), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_DELETENODESITEM, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_NODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_DeleteNodesItem, nodeId) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BOOLEAN, .namespaceZero = UA_TRUE, .padding = offsetof(UA_DeleteNodesItem, deleteTargetReferences) - (offsetof(UA_DeleteNodesItem, nodeId) + sizeof(UA_NodeId)), .isArray = UA_FALSE }, }},

/* UA_DeleteNodesRequest */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 498}, .memSize = sizeof(UA_DeleteNodesRequest), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_DELETENODESREQUEST, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_REQUESTHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_DeleteNodesRequest, requestHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_DELETENODESITEM, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_DeleteNodesRequest, nodesToDeleteSize) - (offsetof(UA_DeleteNodesRequest, requestHeader) + sizeof(UA_RequestHeader))) << 3) + (offsetof(UA_DeleteNodesRequest, nodesToDelete) - sizeof(UA_Int32) - offsetof(UA_DeleteNodesRequest, nodesToDeleteSize)), .isArray = UA_TRUE }, }},

/* UA_DeleteNodesResponse */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 501}, .memSize = sizeof(UA_DeleteNodesResponse), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_DELETENODESRESPONSE, .membersSize = 3,
	.members={
	{.memberTypeIndex = UA_TYPES_RESPONSEHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_DeleteNodesResponse, responseHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STATUSCODE, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_DeleteNodesResponse, resultsSize) - (offsetof(UA_DeleteNodesResponse, responseHeader) + sizeof(UA_ResponseHeader))) << 3) + (offsetof(UA_DeleteNodesResponse, results) - sizeof(UA_Int32) - offsetof(UA_DeleteNodesResponse, resultsSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_DIAGNOSTICINFO, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_DeleteNodesResponse, diagnosticInfosSize) - (offsetof(UA_DeleteNodesResponse, results) + sizeof(void*))) << 3) + (offsetof(UA_DeleteNodesResponse, diagnosticInfos) - sizeof(UA_Int32) - offsetof(UA_DeleteNodesResponse, diagnosticInfosSize)), .isArray = UA_TRUE }, }},

/* UA_DeleteReferencesItem */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 385}, .memSize = sizeof(UA_DeleteReferencesItem), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_DELETEREFERENCESITEM, .membersSize = 5,
	.members={
	{.memberTypeIndex = UA_TYPES_NODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_DeleteReferencesItem, sourceNodeId) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_NODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_DeleteReferencesItem, referenceTypeId) - (offsetof(UA_DeleteReferencesItem, sourceNodeId) + sizeof(UA_NodeId)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BOOLEAN, .namespaceZero = UA_TRUE, .padding = offsetof(UA_DeleteReferencesItem, isForward) - (offsetof(UA_DeleteReferencesItem, referenceTypeId) + sizeof(UA_NodeId)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_EXPANDEDNODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_DeleteReferencesItem, targetNodeId) - (offsetof(UA_DeleteReferencesItem, isForward) + sizeof(UA_Boolean)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BOOLEAN, .namespaceZero = UA_TRUE, .padding = offsetof(UA_DeleteReferencesItem, deleteBidirectional) - (offsetof(UA_DeleteReferencesItem, targetNodeId) + sizeof(UA_ExpandedNodeId)), .isArray = UA_FALSE }, }},

/* UA_DeleteReferencesRequest */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 504}, .memSize = sizeof(UA_DeleteReferencesRequest), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_DELETEREFERENCESREQUEST, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_REQUESTHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_DeleteReferencesRequest, requestHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_DELETEREFERENCESITEM, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_DeleteReferencesRequest, referencesToDeleteSize) - (offsetof(UA_DeleteReferencesRequest, requestHeader) + sizeof(UA_RequestHeader))) << 3) + (offsetof(UA_DeleteReferencesRequest, referencesToDelete) - sizeof(UA_Int32) - offsetof(UA_DeleteReferencesRequest, referencesToDeleteSize)), .isArray = UA_TRUE }, }},

/* UA_DeleteReferencesResponse */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 507}, .memSize = sizeof(UA_DeleteReferencesResponse), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_DELETEREFERENCESRESPONSE, .membersSize = 3,
	.members={
	{.memberTypeIndex = UA_TYPES_RESPONSEHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_DeleteReferencesResponse, responseHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STATUSCODE, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_DeleteReferencesResponse, resultsSize) - (offsetof(UA_DeleteReferencesResponse, responseHeader) + sizeof(UA_ResponseHeader))) << 3) + (offsetof(UA_DeleteReferencesResponse, results) - sizeof(UA_Int32) - offsetof(UA_DeleteReferencesResponse, resultsSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_DIAGNOSTICINFO, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_DeleteReferencesResponse, diagnosticInfosSize) - (offsetof(UA_DeleteReferencesResponse, results) + sizeof(void*))) << 3) + (offsetof(UA_DeleteReferencesResponse, diagnosticInfos) - sizeof(UA_Int32) - offsetof(UA_DeleteReferencesResponse, diagnosticInfosSize)), .isArray = UA_TRUE }, }},

/* UA_BrowseDirection */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 510}, .memSize = sizeof(UA_BrowseDirection), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_INT32,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_INT32 },

/* UA_ViewDescription */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 511}, .memSize = sizeof(UA_ViewDescription), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_VIEWDESCRIPTION, .membersSize = 3,
	.members={
	{.memberTypeIndex = UA_TYPES_NODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ViewDescription, viewId) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_DATETIME, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ViewDescription, timestamp) - (offsetof(UA_ViewDescription, viewId) + sizeof(UA_NodeId)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ViewDescription, viewVersion) - (offsetof(UA_ViewDescription, timestamp) + sizeof(UA_DateTime)), .isArray = UA_FALSE }, }},

/* UA_BrowseDescription */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 514}, .memSize = sizeof(UA_BrowseDescription), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_BROWSEDESCRIPTION, .membersSize = 6,
	.members={
	{.memberTypeIndex = UA_TYPES_NODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_BrowseDescription, nodeId) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BROWSEDIRECTION, .namespaceZero = UA_TRUE, .padding = offsetof(UA_BrowseDescription, browseDirection) - (offsetof(UA_BrowseDescription, nodeId) + sizeof(UA_NodeId)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_NODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_BrowseDescription, referenceTypeId) - (offsetof(UA_BrowseDescription, browseDirection) + sizeof(UA_BrowseDirection)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BOOLEAN, .namespaceZero = UA_TRUE, .padding = offsetof(UA_BrowseDescription, includeSubtypes) - (offsetof(UA_BrowseDescription, referenceTypeId) + sizeof(UA_NodeId)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_BrowseDescription, nodeClassMask) - (offsetof(UA_BrowseDescription, includeSubtypes) + sizeof(UA_Boolean)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_BrowseDescription, resultMask) - (offsetof(UA_BrowseDescription, nodeClassMask) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, }},

/* UA_BrowseResultMask */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 517}, .memSize = sizeof(UA_BrowseResultMask), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_INT32,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_INT32 },

/* UA_ReferenceDescription */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 518}, .memSize = sizeof(UA_ReferenceDescription), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_REFERENCEDESCRIPTION, .membersSize = 7,
	.members={
	{.memberTypeIndex = UA_TYPES_NODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReferenceDescription, referenceTypeId) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BOOLEAN, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReferenceDescription, isForward) - (offsetof(UA_ReferenceDescription, referenceTypeId) + sizeof(UA_NodeId)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_EXPANDEDNODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReferenceDescription, nodeId) - (offsetof(UA_ReferenceDescription, isForward) + sizeof(UA_Boolean)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_QUALIFIEDNAME, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReferenceDescription, browseName) - (offsetof(UA_ReferenceDescription, nodeId) + sizeof(UA_ExpandedNodeId)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_LOCALIZEDTEXT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReferenceDescription, displayName) - (offsetof(UA_ReferenceDescription, browseName) + sizeof(UA_QualifiedName)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_NODECLASS, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReferenceDescription, nodeClass) - (offsetof(UA_ReferenceDescription, displayName) + sizeof(UA_LocalizedText)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_EXPANDEDNODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReferenceDescription, typeDefinition) - (offsetof(UA_ReferenceDescription, nodeClass) + sizeof(UA_NodeClass)), .isArray = UA_FALSE }, }},

/* UA_BrowseResult */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 522}, .memSize = sizeof(UA_BrowseResult), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_BROWSERESULT, .membersSize = 3,
	.members={
	{.memberTypeIndex = UA_TYPES_STATUSCODE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_BrowseResult, statusCode) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BYTESTRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_BrowseResult, continuationPoint) - (offsetof(UA_BrowseResult, statusCode) + sizeof(UA_StatusCode)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_REFERENCEDESCRIPTION, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_BrowseResult, referencesSize) - (offsetof(UA_BrowseResult, continuationPoint) + sizeof(UA_ByteString))) << 3) + (offsetof(UA_BrowseResult, references) - sizeof(UA_Int32) - offsetof(UA_BrowseResult, referencesSize)), .isArray = UA_TRUE }, }},

/* UA_BrowseRequest */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 525}, .memSize = sizeof(UA_BrowseRequest), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_BROWSEREQUEST, .membersSize = 4,
	.members={
	{.memberTypeIndex = UA_TYPES_REQUESTHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_BrowseRequest, requestHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_VIEWDESCRIPTION, .namespaceZero = UA_TRUE, .padding = offsetof(UA_BrowseRequest, view) - (offsetof(UA_BrowseRequest, requestHeader) + sizeof(UA_RequestHeader)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_BrowseRequest, requestedMaxReferencesPerNode) - (offsetof(UA_BrowseRequest, view) + sizeof(UA_ViewDescription)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BROWSEDESCRIPTION, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_BrowseRequest, nodesToBrowseSize) - (offsetof(UA_BrowseRequest, requestedMaxReferencesPerNode) + sizeof(UA_UInt32))) << 3) + (offsetof(UA_BrowseRequest, nodesToBrowse) - sizeof(UA_Int32) - offsetof(UA_BrowseRequest, nodesToBrowseSize)), .isArray = UA_TRUE }, }},

/* UA_BrowseResponse */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 528}, .memSize = sizeof(UA_BrowseResponse), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_BROWSERESPONSE, .membersSize = 3,
	.members={
	{.memberTypeIndex = UA_TYPES_RESPONSEHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_BrowseResponse, responseHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BROWSERESULT, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_BrowseResponse, resultsSize) - (offsetof(UA_BrowseResponse, responseHeader) + sizeof(UA_ResponseHeader))) << 3) + (offsetof(UA_BrowseResponse, results) - sizeof(UA_Int32) - offsetof(UA_BrowseResponse, resultsSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_DIAGNOSTICINFO, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_BrowseResponse, diagnosticInfosSize) - (offsetof(UA_BrowseResponse, results) + sizeof(void*))) << 3) + (offsetof(UA_BrowseResponse, diagnosticInfos) - sizeof(UA_Int32) - offsetof(UA_BrowseResponse, diagnosticInfosSize)), .isArray = UA_TRUE }, }},

/* UA_BrowseNextRequest */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 531}, .memSize = sizeof(UA_BrowseNextRequest), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_BROWSENEXTREQUEST, .membersSize = 3,
	.members={
	{.memberTypeIndex = UA_TYPES_REQUESTHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_BrowseNextRequest, requestHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BOOLEAN, .namespaceZero = UA_TRUE, .padding = offsetof(UA_BrowseNextRequest, releaseContinuationPoints) - (offsetof(UA_BrowseNextRequest, requestHeader) + sizeof(UA_RequestHeader)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BYTESTRING, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_BrowseNextRequest, continuationPointsSize) - (offsetof(UA_BrowseNextRequest, releaseContinuationPoints) + sizeof(UA_Boolean))) << 3) + (offsetof(UA_BrowseNextRequest, continuationPoints) - sizeof(UA_Int32) - offsetof(UA_BrowseNextRequest, continuationPointsSize)), .isArray = UA_TRUE }, }},

/* UA_BrowseNextResponse */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 534}, .memSize = sizeof(UA_BrowseNextResponse), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_BROWSENEXTRESPONSE, .membersSize = 3,
	.members={
	{.memberTypeIndex = UA_TYPES_RESPONSEHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_BrowseNextResponse, responseHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BROWSERESULT, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_BrowseNextResponse, resultsSize) - (offsetof(UA_BrowseNextResponse, responseHeader) + sizeof(UA_ResponseHeader))) << 3) + (offsetof(UA_BrowseNextResponse, results) - sizeof(UA_Int32) - offsetof(UA_BrowseNextResponse, resultsSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_DIAGNOSTICINFO, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_BrowseNextResponse, diagnosticInfosSize) - (offsetof(UA_BrowseNextResponse, results) + sizeof(void*))) << 3) + (offsetof(UA_BrowseNextResponse, diagnosticInfos) - sizeof(UA_Int32) - offsetof(UA_BrowseNextResponse, diagnosticInfosSize)), .isArray = UA_TRUE }, }},

/* UA_RelativePathElement */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 537}, .memSize = sizeof(UA_RelativePathElement), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_RELATIVEPATHELEMENT, .membersSize = 4,
	.members={
	{.memberTypeIndex = UA_TYPES_NODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_RelativePathElement, referenceTypeId) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BOOLEAN, .namespaceZero = UA_TRUE, .padding = offsetof(UA_RelativePathElement, isInverse) - (offsetof(UA_RelativePathElement, referenceTypeId) + sizeof(UA_NodeId)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BOOLEAN, .namespaceZero = UA_TRUE, .padding = offsetof(UA_RelativePathElement, includeSubtypes) - (offsetof(UA_RelativePathElement, isInverse) + sizeof(UA_Boolean)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_QUALIFIEDNAME, .namespaceZero = UA_TRUE, .padding = offsetof(UA_RelativePathElement, targetName) - (offsetof(UA_RelativePathElement, includeSubtypes) + sizeof(UA_Boolean)), .isArray = UA_FALSE }, }},

/* UA_RelativePath */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 540}, .memSize = sizeof(UA_RelativePath), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_RELATIVEPATH, .membersSize = 1,
	.members={
	{.memberTypeIndex = UA_TYPES_RELATIVEPATHELEMENT, .namespaceZero = UA_TRUE, .padding = (offsetof(UA_RelativePath, elements) - sizeof(UA_Int32) - offsetof(UA_RelativePath, elementsSize)), .isArray = UA_TRUE }, }},

/* UA_BrowsePath */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 543}, .memSize = sizeof(UA_BrowsePath), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_BROWSEPATH, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_NODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_BrowsePath, startingNode) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_RELATIVEPATH, .namespaceZero = UA_TRUE, .padding = offsetof(UA_BrowsePath, relativePath) - (offsetof(UA_BrowsePath, startingNode) + sizeof(UA_NodeId)), .isArray = UA_FALSE }, }},

/* UA_BrowsePathTarget */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 546}, .memSize = sizeof(UA_BrowsePathTarget), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_BROWSEPATHTARGET, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_EXPANDEDNODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_BrowsePathTarget, targetId) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_BrowsePathTarget, remainingPathIndex) - (offsetof(UA_BrowsePathTarget, targetId) + sizeof(UA_ExpandedNodeId)), .isArray = UA_FALSE }, }},

/* UA_BrowsePathResult */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 549}, .memSize = sizeof(UA_BrowsePathResult), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_BROWSEPATHRESULT, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_STATUSCODE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_BrowsePathResult, statusCode) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BROWSEPATHTARGET, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_BrowsePathResult, targetsSize) - (offsetof(UA_BrowsePathResult, statusCode) + sizeof(UA_StatusCode))) << 3) + (offsetof(UA_BrowsePathResult, targets) - sizeof(UA_Int32) - offsetof(UA_BrowsePathResult, targetsSize)), .isArray = UA_TRUE }, }},

/* UA_TranslateBrowsePathsToNodeIdsRequest */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 552}, .memSize = sizeof(UA_TranslateBrowsePathsToNodeIdsRequest), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_TRANSLATEBROWSEPATHSTONODEIDSREQUEST, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_REQUESTHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_TranslateBrowsePathsToNodeIdsRequest, requestHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BROWSEPATH, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_TranslateBrowsePathsToNodeIdsRequest, browsePathsSize) - (offsetof(UA_TranslateBrowsePathsToNodeIdsRequest, requestHeader) + sizeof(UA_RequestHeader))) << 3) + (offsetof(UA_TranslateBrowsePathsToNodeIdsRequest, browsePaths) - sizeof(UA_Int32) - offsetof(UA_TranslateBrowsePathsToNodeIdsRequest, browsePathsSize)), .isArray = UA_TRUE }, }},

/* UA_TranslateBrowsePathsToNodeIdsResponse */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 555}, .memSize = sizeof(UA_TranslateBrowsePathsToNodeIdsResponse), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_TRANSLATEBROWSEPATHSTONODEIDSRESPONSE, .membersSize = 3,
	.members={
	{.memberTypeIndex = UA_TYPES_RESPONSEHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_TranslateBrowsePathsToNodeIdsResponse, responseHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BROWSEPATHRESULT, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_TranslateBrowsePathsToNodeIdsResponse, resultsSize) - (offsetof(UA_TranslateBrowsePathsToNodeIdsResponse, responseHeader) + sizeof(UA_ResponseHeader))) << 3) + (offsetof(UA_TranslateBrowsePathsToNodeIdsResponse, results) - sizeof(UA_Int32) - offsetof(UA_TranslateBrowsePathsToNodeIdsResponse, resultsSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_DIAGNOSTICINFO, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_TranslateBrowsePathsToNodeIdsResponse, diagnosticInfosSize) - (offsetof(UA_TranslateBrowsePathsToNodeIdsResponse, results) + sizeof(void*))) << 3) + (offsetof(UA_TranslateBrowsePathsToNodeIdsResponse, diagnosticInfos) - sizeof(UA_Int32) - offsetof(UA_TranslateBrowsePathsToNodeIdsResponse, diagnosticInfosSize)), .isArray = UA_TRUE }, }},

/* UA_RegisterNodesRequest */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 558}, .memSize = sizeof(UA_RegisterNodesRequest), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_REGISTERNODESREQUEST, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_REQUESTHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_RegisterNodesRequest, requestHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_NODEID, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_RegisterNodesRequest, nodesToRegisterSize) - (offsetof(UA_RegisterNodesRequest, requestHeader) + sizeof(UA_RequestHeader))) << 3) + (offsetof(UA_RegisterNodesRequest, nodesToRegister) - sizeof(UA_Int32) - offsetof(UA_RegisterNodesRequest, nodesToRegisterSize)), .isArray = UA_TRUE }, }},

/* UA_RegisterNodesResponse */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 561}, .memSize = sizeof(UA_RegisterNodesResponse), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_REGISTERNODESRESPONSE, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_RESPONSEHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_RegisterNodesResponse, responseHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_NODEID, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_RegisterNodesResponse, registeredNodeIdsSize) - (offsetof(UA_RegisterNodesResponse, responseHeader) + sizeof(UA_ResponseHeader))) << 3) + (offsetof(UA_RegisterNodesResponse, registeredNodeIds) - sizeof(UA_Int32) - offsetof(UA_RegisterNodesResponse, registeredNodeIdsSize)), .isArray = UA_TRUE }, }},

/* UA_UnregisterNodesRequest */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 564}, .memSize = sizeof(UA_UnregisterNodesRequest), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_UNREGISTERNODESREQUEST, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_REQUESTHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_UnregisterNodesRequest, requestHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_NODEID, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_UnregisterNodesRequest, nodesToUnregisterSize) - (offsetof(UA_UnregisterNodesRequest, requestHeader) + sizeof(UA_RequestHeader))) << 3) + (offsetof(UA_UnregisterNodesRequest, nodesToUnregister) - sizeof(UA_Int32) - offsetof(UA_UnregisterNodesRequest, nodesToUnregisterSize)), .isArray = UA_TRUE }, }},

/* UA_UnregisterNodesResponse */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 567}, .memSize = sizeof(UA_UnregisterNodesResponse), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_UNREGISTERNODESRESPONSE, .membersSize = 1,
	.members={
	{.memberTypeIndex = UA_TYPES_RESPONSEHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_UnregisterNodesResponse, responseHeader) - 0, .isArray = UA_FALSE }, }},

/* UA_TimestampsToReturn */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 625}, .memSize = sizeof(UA_TimestampsToReturn), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_INT32,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_INT32 },

/* UA_ReadValueId */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 626}, .memSize = sizeof(UA_ReadValueId), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_READVALUEID, .membersSize = 4,
	.members={
	{.memberTypeIndex = UA_TYPES_NODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReadValueId, nodeId) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReadValueId, attributeId) - (offsetof(UA_ReadValueId, nodeId) + sizeof(UA_NodeId)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReadValueId, indexRange) - (offsetof(UA_ReadValueId, attributeId) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_QUALIFIEDNAME, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReadValueId, dataEncoding) - (offsetof(UA_ReadValueId, indexRange) + sizeof(UA_String)), .isArray = UA_FALSE }, }},

/* UA_ReadRequest */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 629}, .memSize = sizeof(UA_ReadRequest), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_READREQUEST, .membersSize = 4,
	.members={
	{.memberTypeIndex = UA_TYPES_REQUESTHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReadRequest, requestHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_DOUBLE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReadRequest, maxAge) - (offsetof(UA_ReadRequest, requestHeader) + sizeof(UA_RequestHeader)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_TIMESTAMPSTORETURN, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReadRequest, timestampsToReturn) - (offsetof(UA_ReadRequest, maxAge) + sizeof(UA_Double)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_READVALUEID, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_ReadRequest, nodesToReadSize) - (offsetof(UA_ReadRequest, timestampsToReturn) + sizeof(UA_TimestampsToReturn))) << 3) + (offsetof(UA_ReadRequest, nodesToRead) - sizeof(UA_Int32) - offsetof(UA_ReadRequest, nodesToReadSize)), .isArray = UA_TRUE }, }},

/* UA_ReadResponse */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 632}, .memSize = sizeof(UA_ReadResponse), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_READRESPONSE, .membersSize = 3,
	.members={
	{.memberTypeIndex = UA_TYPES_RESPONSEHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReadResponse, responseHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_DATAVALUE, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_ReadResponse, resultsSize) - (offsetof(UA_ReadResponse, responseHeader) + sizeof(UA_ResponseHeader))) << 3) + (offsetof(UA_ReadResponse, results) - sizeof(UA_Int32) - offsetof(UA_ReadResponse, resultsSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_DIAGNOSTICINFO, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_ReadResponse, diagnosticInfosSize) - (offsetof(UA_ReadResponse, results) + sizeof(void*))) << 3) + (offsetof(UA_ReadResponse, diagnosticInfos) - sizeof(UA_Int32) - offsetof(UA_ReadResponse, diagnosticInfosSize)), .isArray = UA_TRUE }, }},

/* UA_WriteValue */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 668}, .memSize = sizeof(UA_WriteValue), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_WRITEVALUE, .membersSize = 4,
	.members={
	{.memberTypeIndex = UA_TYPES_NODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_WriteValue, nodeId) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_WriteValue, attributeId) - (offsetof(UA_WriteValue, nodeId) + sizeof(UA_NodeId)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_WriteValue, indexRange) - (offsetof(UA_WriteValue, attributeId) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_DATAVALUE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_WriteValue, value) - (offsetof(UA_WriteValue, indexRange) + sizeof(UA_String)), .isArray = UA_FALSE }, }},

/* UA_WriteRequest */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 671}, .memSize = sizeof(UA_WriteRequest), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_WRITEREQUEST, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_REQUESTHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_WriteRequest, requestHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_WRITEVALUE, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_WriteRequest, nodesToWriteSize) - (offsetof(UA_WriteRequest, requestHeader) + sizeof(UA_RequestHeader))) << 3) + (offsetof(UA_WriteRequest, nodesToWrite) - sizeof(UA_Int32) - offsetof(UA_WriteRequest, nodesToWriteSize)), .isArray = UA_TRUE }, }},

/* UA_WriteResponse */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 674}, .memSize = sizeof(UA_WriteResponse), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_WRITERESPONSE, .membersSize = 3,
	.members={
	{.memberTypeIndex = UA_TYPES_RESPONSEHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_WriteResponse, responseHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STATUSCODE, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_WriteResponse, resultsSize) - (offsetof(UA_WriteResponse, responseHeader) + sizeof(UA_ResponseHeader))) << 3) + (offsetof(UA_WriteResponse, results) - sizeof(UA_Int32) - offsetof(UA_WriteResponse, resultsSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_DIAGNOSTICINFO, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_WriteResponse, diagnosticInfosSize) - (offsetof(UA_WriteResponse, results) + sizeof(void*))) << 3) + (offsetof(UA_WriteResponse, diagnosticInfos) - sizeof(UA_Int32) - offsetof(UA_WriteResponse, diagnosticInfosSize)), .isArray = UA_TRUE }, }},

/* UA_MonitoringMode */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 716}, .memSize = sizeof(UA_MonitoringMode), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_INT32,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_INT32 },

/* UA_MonitoringParameters */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 740}, .memSize = sizeof(UA_MonitoringParameters), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_MONITORINGPARAMETERS, .membersSize = 5,
	.members={
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_MonitoringParameters, clientHandle) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_DOUBLE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_MonitoringParameters, samplingInterval) - (offsetof(UA_MonitoringParameters, clientHandle) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_EXTENSIONOBJECT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_MonitoringParameters, filter) - (offsetof(UA_MonitoringParameters, samplingInterval) + sizeof(UA_Double)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_MonitoringParameters, queueSize) - (offsetof(UA_MonitoringParameters, filter) + sizeof(UA_ExtensionObject)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BOOLEAN, .namespaceZero = UA_TRUE, .padding = offsetof(UA_MonitoringParameters, discardOldest) - (offsetof(UA_MonitoringParameters, queueSize) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, }},

/* UA_MonitoredItemCreateRequest */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 743}, .memSize = sizeof(UA_MonitoredItemCreateRequest), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_MONITOREDITEMCREATEREQUEST, .membersSize = 3,
	.members={
	{.memberTypeIndex = UA_TYPES_READVALUEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_MonitoredItemCreateRequest, itemToMonitor) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_MONITORINGMODE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_MonitoredItemCreateRequest, monitoringMode) - (offsetof(UA_MonitoredItemCreateRequest, itemToMonitor) + sizeof(UA_ReadValueId)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_MONITORINGPARAMETERS, .namespaceZero = UA_TRUE, .padding = offsetof(UA_MonitoredItemCreateRequest, requestedParameters) - (offsetof(UA_MonitoredItemCreateRequest, monitoringMode) + sizeof(UA_MonitoringMode)), .isArray = UA_FALSE }, }},

/* UA_MonitoredItemCreateResult */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 746}, .memSize = sizeof(UA_MonitoredItemCreateResult), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_MONITOREDITEMCREATERESULT, .membersSize = 5,
	.members={
	{.memberTypeIndex = UA_TYPES_STATUSCODE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_MonitoredItemCreateResult, statusCode) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_MonitoredItemCreateResult, monitoredItemId) - (offsetof(UA_MonitoredItemCreateResult, statusCode) + sizeof(UA_StatusCode)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_DOUBLE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_MonitoredItemCreateResult, revisedSamplingInterval) - (offsetof(UA_MonitoredItemCreateResult, monitoredItemId) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_MonitoredItemCreateResult, revisedQueueSize) - (offsetof(UA_MonitoredItemCreateResult, revisedSamplingInterval) + sizeof(UA_Double)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_EXTENSIONOBJECT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_MonitoredItemCreateResult, filterResult) - (offsetof(UA_MonitoredItemCreateResult, revisedQueueSize) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, }},

/* UA_CreateMonitoredItemsRequest */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 749}, .memSize = sizeof(UA_CreateMonitoredItemsRequest), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_CREATEMONITOREDITEMSREQUEST, .membersSize = 4,
	.members={
	{.memberTypeIndex = UA_TYPES_REQUESTHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateMonitoredItemsRequest, requestHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateMonitoredItemsRequest, subscriptionId) - (offsetof(UA_CreateMonitoredItemsRequest, requestHeader) + sizeof(UA_RequestHeader)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_TIMESTAMPSTORETURN, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateMonitoredItemsRequest, timestampsToReturn) - (offsetof(UA_CreateMonitoredItemsRequest, subscriptionId) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_MONITOREDITEMCREATEREQUEST, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_CreateMonitoredItemsRequest, itemsToCreateSize) - (offsetof(UA_CreateMonitoredItemsRequest, timestampsToReturn) + sizeof(UA_TimestampsToReturn))) << 3) + (offsetof(UA_CreateMonitoredItemsRequest, itemsToCreate) - sizeof(UA_Int32) - offsetof(UA_CreateMonitoredItemsRequest, itemsToCreateSize)), .isArray = UA_TRUE }, }},

/* UA_CreateMonitoredItemsResponse */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 752}, .memSize = sizeof(UA_CreateMonitoredItemsResponse), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_CREATEMONITOREDITEMSRESPONSE, .membersSize = 3,
	.members={
	{.memberTypeIndex = UA_TYPES_RESPONSEHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateMonitoredItemsResponse, responseHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_MONITOREDITEMCREATERESULT, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_CreateMonitoredItemsResponse, resultsSize) - (offsetof(UA_CreateMonitoredItemsResponse, responseHeader) + sizeof(UA_ResponseHeader))) << 3) + (offsetof(UA_CreateMonitoredItemsResponse, results) - sizeof(UA_Int32) - offsetof(UA_CreateMonitoredItemsResponse, resultsSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_DIAGNOSTICINFO, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_CreateMonitoredItemsResponse, diagnosticInfosSize) - (offsetof(UA_CreateMonitoredItemsResponse, results) + sizeof(void*))) << 3) + (offsetof(UA_CreateMonitoredItemsResponse, diagnosticInfos) - sizeof(UA_Int32) - offsetof(UA_CreateMonitoredItemsResponse, diagnosticInfosSize)), .isArray = UA_TRUE }, }},

/* UA_CreateSubscriptionRequest */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 785}, .memSize = sizeof(UA_CreateSubscriptionRequest), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_CREATESUBSCRIPTIONREQUEST, .membersSize = 7,
	.members={
	{.memberTypeIndex = UA_TYPES_REQUESTHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSubscriptionRequest, requestHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_DOUBLE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSubscriptionRequest, requestedPublishingInterval) - (offsetof(UA_CreateSubscriptionRequest, requestHeader) + sizeof(UA_RequestHeader)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSubscriptionRequest, requestedLifetimeCount) - (offsetof(UA_CreateSubscriptionRequest, requestedPublishingInterval) + sizeof(UA_Double)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSubscriptionRequest, requestedMaxKeepAliveCount) - (offsetof(UA_CreateSubscriptionRequest, requestedLifetimeCount) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSubscriptionRequest, maxNotificationsPerPublish) - (offsetof(UA_CreateSubscriptionRequest, requestedMaxKeepAliveCount) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BOOLEAN, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSubscriptionRequest, publishingEnabled) - (offsetof(UA_CreateSubscriptionRequest, maxNotificationsPerPublish) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BYTE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSubscriptionRequest, priority) - (offsetof(UA_CreateSubscriptionRequest, publishingEnabled) + sizeof(UA_Boolean)), .isArray = UA_FALSE }, }},

/* UA_CreateSubscriptionResponse */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 788}, .memSize = sizeof(UA_CreateSubscriptionResponse), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_CREATESUBSCRIPTIONRESPONSE, .membersSize = 5,
	.members={
	{.memberTypeIndex = UA_TYPES_RESPONSEHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSubscriptionResponse, responseHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSubscriptionResponse, subscriptionId) - (offsetof(UA_CreateSubscriptionResponse, responseHeader) + sizeof(UA_ResponseHeader)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_DOUBLE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSubscriptionResponse, revisedPublishingInterval) - (offsetof(UA_CreateSubscriptionResponse, subscriptionId) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSubscriptionResponse, revisedLifetimeCount) - (offsetof(UA_CreateSubscriptionResponse, revisedPublishingInterval) + sizeof(UA_Double)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSubscriptionResponse, revisedMaxKeepAliveCount) - (offsetof(UA_CreateSubscriptionResponse, revisedLifetimeCount) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, }},

/* UA_SetPublishingModeRequest */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 797}, .memSize = sizeof(UA_SetPublishingModeRequest), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_SETPUBLISHINGMODEREQUEST, .membersSize = 3,
	.members={
	{.memberTypeIndex = UA_TYPES_REQUESTHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_SetPublishingModeRequest, requestHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BOOLEAN, .namespaceZero = UA_TRUE, .padding = offsetof(UA_SetPublishingModeRequest, publishingEnabled) - (offsetof(UA_SetPublishingModeRequest, requestHeader) + sizeof(UA_RequestHeader)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_SetPublishingModeRequest, subscriptionIdsSize) - (offsetof(UA_SetPublishingModeRequest, publishingEnabled) + sizeof(UA_Boolean))) << 3) + (offsetof(UA_SetPublishingModeRequest, subscriptionIds) - sizeof(UA_Int32) - offsetof(UA_SetPublishingModeRequest, subscriptionIdsSize)), .isArray = UA_TRUE }, }},

/* UA_SetPublishingModeResponse */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 800}, .memSize = sizeof(UA_SetPublishingModeResponse), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_SETPUBLISHINGMODERESPONSE, .membersSize = 3,
	.members={
	{.memberTypeIndex = UA_TYPES_RESPONSEHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_SetPublishingModeResponse, responseHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STATUSCODE, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_SetPublishingModeResponse, resultsSize) - (offsetof(UA_SetPublishingModeResponse, responseHeader) + sizeof(UA_ResponseHeader))) << 3) + (offsetof(UA_SetPublishingModeResponse, results) - sizeof(UA_Int32) - offsetof(UA_SetPublishingModeResponse, resultsSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_DIAGNOSTICINFO, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_SetPublishingModeResponse, diagnosticInfosSize) - (offsetof(UA_SetPublishingModeResponse, results) + sizeof(void*))) << 3) + (offsetof(UA_SetPublishingModeResponse, diagnosticInfos) - sizeof(UA_Int32) - offsetof(UA_SetPublishingModeResponse, diagnosticInfosSize)), .isArray = UA_TRUE }, }},

/* UA_NotificationMessage */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 803}, .memSize = sizeof(UA_NotificationMessage), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_NOTIFICATIONMESSAGE, .membersSize = 3,
	.members={
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_NotificationMessage, sequenceNumber) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_DATETIME, .namespaceZero = UA_TRUE, .padding = offsetof(UA_NotificationMessage, publishTime) - (offsetof(UA_NotificationMessage, sequenceNumber) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_EXTENSIONOBJECT, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_NotificationMessage, notificationDataSize) - (offsetof(UA_NotificationMessage, publishTime) + sizeof(UA_DateTime))) << 3) + (offsetof(UA_NotificationMessage, notificationData) - sizeof(UA_Int32) - offsetof(UA_NotificationMessage, notificationDataSize)), .isArray = UA_TRUE }, }},

/* UA_SubscriptionAcknowledgement */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 821}, .memSize = sizeof(UA_SubscriptionAcknowledgement), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = sizeof(UA_SubscriptionAcknowledgement) == 8, .typeIndex = UA_TYPES_SUBSCRIPTIONACKNOWLEDGEMENT, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_SubscriptionAcknowledgement, subscriptionId) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_SubscriptionAcknowledgement, sequenceNumber) - (offsetof(UA_SubscriptionAcknowledgement, subscriptionId) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, }},

/* UA_PublishRequest */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 824}, .memSize = sizeof(UA_PublishRequest), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_PUBLISHREQUEST, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_REQUESTHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_PublishRequest, requestHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_SUBSCRIPTIONACKNOWLEDGEMENT, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_PublishRequest, subscriptionAcknowledgementsSize) - (offsetof(UA_PublishRequest, requestHeader) + sizeof(UA_RequestHeader))) << 3) + (offsetof(UA_PublishRequest, subscriptionAcknowledgements) - sizeof(UA_Int32) - offsetof(UA_PublishRequest, subscriptionAcknowledgementsSize)), .isArray = UA_TRUE }, }},

/* UA_PublishResponse */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 827}, .memSize = sizeof(UA_PublishResponse), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_PUBLISHRESPONSE, .membersSize = 7,
	.members={
	{.memberTypeIndex = UA_TYPES_RESPONSEHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_PublishResponse, responseHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_PublishResponse, subscriptionId) - (offsetof(UA_PublishResponse, responseHeader) + sizeof(UA_ResponseHeader)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_PublishResponse, availableSequenceNumbersSize) - (offsetof(UA_PublishResponse, subscriptionId) + sizeof(UA_UInt32))) << 3) + (offsetof(UA_PublishResponse, availableSequenceNumbers) - sizeof(UA_Int32) - offsetof(UA_PublishResponse, availableSequenceNumbersSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_BOOLEAN, .namespaceZero = UA_TRUE, .padding = offsetof(UA_PublishResponse, moreNotifications) - (offsetof(UA_PublishResponse, availableSequenceNumbers) + sizeof(void*)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_NOTIFICATIONMESSAGE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_PublishResponse, notificationMessage) - (offsetof(UA_PublishResponse, moreNotifications) + sizeof(UA_Boolean)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STATUSCODE, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_PublishResponse, resultsSize) - (offsetof(UA_PublishResponse, notificationMessage) + sizeof(UA_NotificationMessage))) << 3) + (offsetof(UA_PublishResponse, results) - sizeof(UA_Int32) - offsetof(UA_PublishResponse, resultsSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_DIAGNOSTICINFO, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_PublishResponse, diagnosticInfosSize) - (offsetof(UA_PublishResponse, results) + sizeof(void*))) << 3) + (offsetof(UA_PublishResponse, diagnosticInfos) - sizeof(UA_Int32) - offsetof(UA_PublishResponse, diagnosticInfosSize)), .isArray = UA_TRUE }, }},

/* UA_BuildInfo */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 338}, .memSize = sizeof(UA_BuildInfo), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_BUILDINFO, .membersSize = 6,
	.members={
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_BuildInfo, productUri) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_BuildInfo, manufacturerName) - (offsetof(UA_BuildInfo, productUri) + sizeof(UA_String)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_BuildInfo, productName) - (offsetof(UA_BuildInfo, manufacturerName) + sizeof(UA_String)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_BuildInfo, softwareVersion) - (offsetof(UA_BuildInfo, productName) + sizeof(UA_String)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_BuildInfo, buildNumber) - (offsetof(UA_BuildInfo, softwareVersion) + sizeof(UA_String)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_DATETIME, .namespaceZero = UA_TRUE, .padding = offsetof(UA_BuildInfo, buildDate) - (offsetof(UA_BuildInfo, buildNumber) + sizeof(UA_String)), .isArray = UA_FALSE }, }},

/* UA_ServerState */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 852}, .memSize = sizeof(UA_ServerState), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_INT32,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_INT32 },

/* UA_ServerStatusDataType */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 862}, .memSize = sizeof(UA_ServerStatusDataType), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_SERVERSTATUSDATATYPE, .membersSize = 6,
	.members={
	{.memberTypeIndex = UA_TYPES_DATETIME, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ServerStatusDataType, startTime) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_DATETIME, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ServerStatusDataType, currentTime) - (offsetof(UA_ServerStatusDataType, startTime) + sizeof(UA_DateTime)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_SERVERSTATE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ServerStatusDataType, state) - (offsetof(UA_ServerStatusDataType, currentTime) + sizeof(UA_DateTime)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BUILDINFO, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ServerStatusDataType, buildInfo) - (offsetof(UA_ServerStatusDataType, state) + sizeof(UA_ServerState)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ServerStatusDataType, secondsTillShutdown) - (offsetof(UA_ServerStatusDataType, buildInfo) + sizeof(UA_BuildInfo)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_LOCALIZEDTEXT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ServerStatusDataType, shutdownReason) - (offsetof(UA_ServerStatusDataType, secondsTillShutdown) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, }},
};

/*********************************** amalgamated original file "C:/AcpltDevelopmentKit/acplt/dev/open62541WrapperProject/open62541/src_generated/ua_transport_generated.c" ***********************************/
/**
* @file ua_transport_generated.c
*
* @brief Autogenerated data types
*
* Generated from Custom.Opc.Ua.Transport.bsd with script C:/AcpltDevelopmentKit/acplt/dev/open62541WrapperProject/open62541/tools/generate_datatypes.py
* on host SPECTRE by user lars at 2015-05-06 12:57:37
*/


const UA_DataType *UA_TRANSPORT = (UA_DataType[]){

/* UA_MessageTypeAndFinal */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 0}, .memSize = sizeof(UA_MessageTypeAndFinal), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_INT32,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_INT32 },

/* UA_TcpMessageHeader */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 0}, .memSize = sizeof(UA_TcpMessageHeader), .namespaceZero = UA_FALSE, .fixedSize = UA_TRUE, .zeroCopyable = sizeof(UA_TcpMessageHeader) == 8, .typeIndex = UA_TRANSPORT_TCPMESSAGEHEADER, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_TcpMessageHeader, messageTypeAndFinal) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_TcpMessageHeader, messageSize) - (offsetof(UA_TcpMessageHeader, messageTypeAndFinal) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, }},

/* UA_TcpHelloMessage */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 0}, .memSize = sizeof(UA_TcpHelloMessage), .namespaceZero = UA_FALSE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TRANSPORT_TCPHELLOMESSAGE, .membersSize = 6,
	.members={
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_TcpHelloMessage, protocolVersion) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_TcpHelloMessage, receiveBufferSize) - (offsetof(UA_TcpHelloMessage, protocolVersion) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_TcpHelloMessage, sendBufferSize) - (offsetof(UA_TcpHelloMessage, receiveBufferSize) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_TcpHelloMessage, maxMessageSize) - (offsetof(UA_TcpHelloMessage, sendBufferSize) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_TcpHelloMessage, maxChunkCount) - (offsetof(UA_TcpHelloMessage, maxMessageSize) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_TcpHelloMessage, endpointUrl) - (offsetof(UA_TcpHelloMessage, maxChunkCount) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, }},

/* UA_TcpAcknowledgeMessage */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 0}, .memSize = sizeof(UA_TcpAcknowledgeMessage), .namespaceZero = UA_FALSE, .fixedSize = UA_TRUE, .zeroCopyable = sizeof(UA_TcpAcknowledgeMessage) == 20, .typeIndex = UA_TRANSPORT_TCPACKNOWLEDGEMESSAGE, .membersSize = 5,
	.members={
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_TcpAcknowledgeMessage, protocolVersion) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_TcpAcknowledgeMessage, receiveBufferSize) - (offsetof(UA_TcpAcknowledgeMessage, protocolVersion) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_TcpAcknowledgeMessage, sendBufferSize) - (offsetof(UA_TcpAcknowledgeMessage, receiveBufferSize) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_TcpAcknowledgeMessage, maxMessageSize) - (offsetof(UA_TcpAcknowledgeMessage, sendBufferSize) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_TcpAcknowledgeMessage, maxChunkCount) - (offsetof(UA_TcpAcknowledgeMessage, maxMessageSize) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, }},

/* UA_SecureConversationMessageHeader */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 0}, .memSize = sizeof(UA_SecureConversationMessageHeader), .namespaceZero = UA_FALSE, .fixedSize = UA_TRUE, .zeroCopyable = sizeof(UA_SecureConversationMessageHeader) == 12, .typeIndex = UA_TRANSPORT_SECURECONVERSATIONMESSAGEHEADER, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TRANSPORT_TCPMESSAGEHEADER, .namespaceZero = UA_FALSE, .padding = offsetof(UA_SecureConversationMessageHeader, messageHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_SecureConversationMessageHeader, secureChannelId) - (offsetof(UA_SecureConversationMessageHeader, messageHeader) + sizeof(UA_TcpMessageHeader)), .isArray = UA_FALSE }, }},

/* UA_AsymmetricAlgorithmSecurityHeader */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 0}, .memSize = sizeof(UA_AsymmetricAlgorithmSecurityHeader), .namespaceZero = UA_FALSE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TRANSPORT_ASYMMETRICALGORITHMSECURITYHEADER, .membersSize = 3,
	.members={
	{.memberTypeIndex = UA_TYPES_BYTESTRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_AsymmetricAlgorithmSecurityHeader, securityPolicyUri) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BYTESTRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_AsymmetricAlgorithmSecurityHeader, senderCertificate) - (offsetof(UA_AsymmetricAlgorithmSecurityHeader, securityPolicyUri) + sizeof(UA_ByteString)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BYTESTRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_AsymmetricAlgorithmSecurityHeader, receiverCertificateThumbprint) - (offsetof(UA_AsymmetricAlgorithmSecurityHeader, senderCertificate) + sizeof(UA_ByteString)), .isArray = UA_FALSE }, }},

/* UA_SymmetricAlgorithmSecurityHeader */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 0}, .memSize = sizeof(UA_SymmetricAlgorithmSecurityHeader), .namespaceZero = UA_FALSE, .fixedSize = UA_TRUE, .zeroCopyable = sizeof(UA_SymmetricAlgorithmSecurityHeader) == 4, .typeIndex = UA_TRANSPORT_SYMMETRICALGORITHMSECURITYHEADER, .membersSize = 1,
	.members={
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_SymmetricAlgorithmSecurityHeader, tokenId) - 0, .isArray = UA_FALSE }, }},

/* UA_SequenceHeader */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 0}, .memSize = sizeof(UA_SequenceHeader), .namespaceZero = UA_FALSE, .fixedSize = UA_TRUE, .zeroCopyable = sizeof(UA_SequenceHeader) == 8, .typeIndex = UA_TRANSPORT_SEQUENCEHEADER, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_SequenceHeader, sequenceNumber) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_SequenceHeader, requestId) - (offsetof(UA_SequenceHeader, sequenceNumber) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, }},

/* UA_SecureConversationMessageFooter */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 0}, .memSize = sizeof(UA_SecureConversationMessageFooter), .namespaceZero = UA_FALSE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TRANSPORT_SECURECONVERSATIONMESSAGEFOOTER, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_BYTE, .namespaceZero = UA_TRUE, .padding = (offsetof(UA_SecureConversationMessageFooter, padding) - sizeof(UA_Int32) - offsetof(UA_SecureConversationMessageFooter, paddingSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_BYTE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_SecureConversationMessageFooter, signature) - (offsetof(UA_SecureConversationMessageFooter, padding) + sizeof(void*)), .isArray = UA_FALSE }, }},

/* UA_SecureConversationMessageAbortBody */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 0}, .memSize = sizeof(UA_SecureConversationMessageAbortBody), .namespaceZero = UA_FALSE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TRANSPORT_SECURECONVERSATIONMESSAGEABORTBODY, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_SecureConversationMessageAbortBody, error) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_SecureConversationMessageAbortBody, reason) - (offsetof(UA_SecureConversationMessageAbortBody, error) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, }},
};

/*********************************** amalgamated original file "C:/AcpltDevelopmentKit/acplt/dev/open62541WrapperProject/open62541/src/ua_connection.c" ***********************************/

// max message size is 64k
const UA_ConnectionConfig UA_ConnectionConfig_standard =
    {.protocolVersion = 0, .sendBufferSize = 65536, .recvBufferSize  = 65536,
     .maxMessageSize = 65536, .maxChunkCount   = 1};

void UA_Connection_init(UA_Connection *connection) {
    connection->state = UA_CONNECTION_CLOSED;
    connection->localConf = (UA_ConnectionConfig){0,0,0,0,0};
    connection->remoteConf = (UA_ConnectionConfig){0,0,0,0,0};
    connection->channel = UA_NULL;
    connection->sockfd = 0;
    connection->handle = UA_NULL;
    UA_ByteString_init(&connection->incompleteMessage);
    connection->write = UA_NULL;
    connection->close = UA_NULL;
    connection->recv = UA_NULL;
    connection->getBuffer = UA_NULL;
    connection->releaseBuffer = UA_NULL;
}

void UA_Connection_deleteMembers(UA_Connection *connection) {
    UA_ByteString_deleteMembers(&connection->incompleteMessage);
}

UA_ByteString UA_Connection_completeMessages(UA_Connection *connection, UA_ByteString received)
{
    if(received.length == -1)
        return received;

    /* concat the existing incomplete message with the new message */
    UA_ByteString current;
    if(connection->incompleteMessage.length < 0)
        current = received;
    else {
        current.data = UA_realloc(connection->incompleteMessage.data,
                                  connection->incompleteMessage.length + received.length);
        if(!current.data) {
            /* not enough memory */
            UA_ByteString_deleteMembers(&connection->incompleteMessage);
            connection->incompleteMessage.length = -1;
            UA_ByteString_deleteMembers(&received);
            received.length = -1;
            return received;
        }
        UA_memcpy(current.data + connection->incompleteMessage.length, received.data, received.length);
        current.length = connection->incompleteMessage.length + received.length;
        UA_ByteString_deleteMembers(&received);
        UA_ByteString_init(&connection->incompleteMessage);
    }

    /* find the first non-complete message */
    size_t end_pos = 0; // the end of the last complete message
    while(current.length - end_pos >= 16) {
        if(!(current.data[0] == 'M' && current.data[1] == 'S' && current.data[2] == 'G') &&
           !(current.data[0] == 'O' && current.data[1] == 'P' && current.data[2] == 'N') &&
           !(current.data[0] == 'H' && current.data[1] == 'E' && current.data[2] == 'L') &&
           !(current.data[0] == 'A' && current.data[1] == 'C' && current.data[2] == 'K') &&
           !(current.data[0] == 'C' && current.data[1] == 'L' && current.data[2] == 'O')) {
            current.length = end_pos; // throw the remaining bytestring away
            break;
        }
        UA_Int32 length = 0;
        size_t pos = end_pos + 4;
        UA_StatusCode retval = UA_Int32_decodeBinary(&current, &pos, &length);
        if(retval != UA_STATUSCODE_GOOD || length < 16 ||
           length > (UA_Int32)connection->localConf.maxMessageSize) {
            current.length = end_pos; // throw the remaining bytestring away
            break;
        }
        if(length + (UA_Int32)end_pos > current.length)
            break; // the message is incomplete
        end_pos += length;
    }

    if(current.length == 0) {
        /* throw everything away */
        UA_ByteString_deleteMembers(&current);
        current.length = -1;
        return current;
    }

    if(end_pos == 0) {
        /* no complete message in current */
        connection->incompleteMessage = current;
        UA_ByteString_init(&current);
    } else if(current.length != (UA_Int32)end_pos) {
        /* there is an incomplete message at the end of current */
        connection->incompleteMessage.data = UA_malloc(current.length - end_pos);
        if(connection->incompleteMessage.data) {
            UA_memcpy(connection->incompleteMessage.data, &current.data[end_pos], current.length - end_pos);
            connection->incompleteMessage.length = current.length - end_pos;
        }
        current.length = end_pos;
    }
    return current;
}

void UA_Connection_detachSecureChannel(UA_Connection *connection) {
#ifdef UA_MULTITHREADING
    UA_SecureChannel *channel = connection->channel;
    if(channel)
        uatomic_cmpxchg(&channel->connection, connection, UA_NULL);
    uatomic_set(&connection->channel, UA_NULL);
#else
    if(connection->channel)
        connection->channel->connection = UA_NULL;
    connection->channel = UA_NULL;
#endif
}

void UA_Connection_attachSecureChannel(UA_Connection *connection, UA_SecureChannel *channel) {
#ifdef UA_MULTITHREADING
    if(uatomic_cmpxchg(&channel->connection, UA_NULL, connection) == UA_NULL)
        uatomic_set(&connection->channel, channel);
#else
    if(channel->connection != UA_NULL)
        return;
    channel->connection = connection;
    connection->channel = channel;
#endif
}
/*********************************** amalgamated original file "C:/AcpltDevelopmentKit/acplt/dev/open62541WrapperProject/open62541/src/ua_securechannel.c" ***********************************/

void UA_SecureChannel_init(UA_SecureChannel *channel) {
    UA_MessageSecurityMode_init(&channel->securityMode);
    UA_ChannelSecurityToken_init(&channel->securityToken);
    UA_AsymmetricAlgorithmSecurityHeader_init(&channel->clientAsymAlgSettings);
    UA_AsymmetricAlgorithmSecurityHeader_init(&channel->serverAsymAlgSettings);
    UA_ByteString_init(&channel->clientNonce);
    UA_ByteString_init(&channel->serverNonce);
    channel->requestId = 0;
    channel->sequenceNumber = 0;
    channel->connection = UA_NULL;
    LIST_INIT(&channel->sessions);
}

void UA_SecureChannel_deleteMembersCleanup(UA_SecureChannel *channel) {
    UA_AsymmetricAlgorithmSecurityHeader_deleteMembers(&channel->serverAsymAlgSettings);
    UA_ByteString_deleteMembers(&channel->serverNonce);
    UA_AsymmetricAlgorithmSecurityHeader_deleteMembers(&channel->clientAsymAlgSettings);
    UA_ByteString_deleteMembers(&channel->clientNonce);
    UA_ChannelSecurityToken_deleteMembers(&channel->securityToken);
    UA_Connection *c = channel->connection;
    if(c) {
        UA_Connection_detachSecureChannel(c);
        c->close(c);
    }
    /* just remove the pointers and free the linked list (not the sessions) */
    struct SessionEntry *se;
    while((se = LIST_FIRST(&channel->sessions))) {
        UA_SecureChannel_detachSession(channel, se->session); /* the se is deleted inside */
    }
}

//TODO implement real nonce generator - DUMMY function
UA_StatusCode UA_SecureChannel_generateNonce(UA_ByteString *nonce) {
    if(!(nonce->data = UA_malloc(1)))
        return UA_STATUSCODE_BADOUTOFMEMORY;
    nonce->length  = 1;
    nonce->data[0] = 'a';
    return UA_STATUSCODE_GOOD;
}

UA_StatusCode UA_SecureChannel_updateRequestId(UA_SecureChannel *channel, UA_UInt32 requestId) {
    //TODO review checking of request id
    if(channel->requestId+1 != requestId)
        return UA_STATUSCODE_BADINTERNALERROR;
    channel->requestId++;
    return UA_STATUSCODE_GOOD;
}

UA_StatusCode UA_SecureChannel_updateSequenceNumber(UA_SecureChannel *channel, UA_UInt32 sequenceNumber) {
    //TODO review checking of sequence
    if(channel->sequenceNumber+1  != sequenceNumber)
        return UA_STATUSCODE_BADINTERNALERROR;
    channel->sequenceNumber++;
    return UA_STATUSCODE_GOOD;
}

void UA_SecureChannel_attachSession(UA_SecureChannel *channel, UA_Session *session) {
    struct SessionEntry *se = UA_malloc(sizeof(struct SessionEntry));
    if(!se)
        return;
    se->session = session;
#ifdef UA_MULTITHREADING
    if(uatomic_cmpxchg(&session->channel, UA_NULL, channel) != UA_NULL) {
        UA_free(se);
        return;
    }
#else
    if(session->channel != UA_NULL) {
        UA_free(se);
        return;
    }
    session->channel = channel;
#endif
    LIST_INSERT_HEAD(&channel->sessions, se, pointers);
}

void UA_SecureChannel_detachSession(UA_SecureChannel *channel, UA_Session *session) {
    if(session)
        session->channel = UA_NULL;
    struct SessionEntry *se;
    LIST_FOREACH(se, &channel->sessions, pointers) {
        if(se->session != session)
            continue;
        LIST_REMOVE(se, pointers);
        UA_free(se);
        break;
    }
}

UA_Session * UA_SecureChannel_getSession(UA_SecureChannel *channel, UA_NodeId *token) {
    struct SessionEntry *se;
    LIST_FOREACH(se, &channel->sessions, pointers) {
        if(UA_NodeId_equal(&se->session->authenticationToken, token))
            break;
    }
    if(se)
        return se->session;
    else
        return UA_NULL;
}
/*********************************** amalgamated original file "C:/AcpltDevelopmentKit/acplt/dev/open62541WrapperProject/open62541/src/ua_session.c" ***********************************/

UA_Session anonymousSession = {
    .clientDescription =  {.applicationUri = {-1, UA_NULL}, .productUri = {-1, UA_NULL},
                           .applicationName = {.locale = {-1, UA_NULL}, .text = {-1, UA_NULL}},
                           .applicationType = UA_APPLICATIONTYPE_CLIENT,
                           .gatewayServerUri = {-1, UA_NULL}, .discoveryProfileUri = {-1, UA_NULL},
                           .discoveryUrlsSize = -1, .discoveryUrls = UA_NULL},
    .sessionName = {sizeof("Anonymous Session")-1, (UA_Byte*)"Anonymous Session"},
    .authenticationToken = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC,
                            .identifier.numeric = 0}, 
    .sessionId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 0},
    .maxRequestMessageSize = UA_UINT32_MAX, .maxResponseMessageSize = UA_UINT32_MAX,
    .timeout = UA_INT64_MAX, .validTill = UA_INT64_MAX, .channel = UA_NULL,
    .continuationPoints = {UA_NULL}};

UA_Session adminSession = {
    .clientDescription =  {.applicationUri = {-1, UA_NULL}, .productUri = {-1, UA_NULL},
                           .applicationName = {.locale = {-1, UA_NULL}, .text = {-1, UA_NULL}},
                           .applicationType = UA_APPLICATIONTYPE_CLIENT,
                           .gatewayServerUri = {-1, UA_NULL}, .discoveryProfileUri = {-1, UA_NULL},
                           .discoveryUrlsSize = -1, .discoveryUrls = UA_NULL},
    .sessionName = {sizeof("Administrator Session")-1, (UA_Byte*)"Administrator Session"},
    .authenticationToken = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC,
                            .identifier.numeric = 1},
    .sessionId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 1},
    .maxRequestMessageSize = UA_UINT32_MAX, .maxResponseMessageSize = UA_UINT32_MAX,
    .timeout = UA_INT64_MAX, .validTill = UA_INT64_MAX, .channel = UA_NULL,
    .continuationPoints = {UA_NULL}};

void UA_Session_init(UA_Session *session) {
    UA_ApplicationDescription_init(&session->clientDescription);
    session->activated = UA_FALSE;
    UA_NodeId_init(&session->authenticationToken);
    UA_NodeId_init(&session->sessionId);
    UA_String_init(&session->sessionName);
    session->maxRequestMessageSize  = 0;
    session->maxResponseMessageSize = 0;
    session->timeout = 0;
    UA_DateTime_init(&session->validTill);
    session->channel = UA_NULL;
    session->availableContinuationPoints = MAXCONTINUATIONPOINTS;
    LIST_INIT(&session->continuationPoints);
}

void UA_Session_deleteMembersCleanup(UA_Session *session) {
    UA_ApplicationDescription_deleteMembers(&session->clientDescription);
    UA_NodeId_deleteMembers(&session->authenticationToken);
    UA_NodeId_deleteMembers(&session->sessionId);
    UA_String_deleteMembers(&session->sessionName);
    struct ContinuationPointEntry *cp;
    while((cp = LIST_FIRST(&session->continuationPoints))) {
        LIST_REMOVE(cp, pointers);
        UA_ByteString_deleteMembers(&cp->identifier);
        UA_BrowseDescription_deleteMembers(&cp->browseDescription);
        UA_free(cp);
    }
    if(session->channel)
        UA_SecureChannel_detachSession(session->channel, session);
}

void UA_Session_updateLifetime(UA_Session *session) {
    session->validTill = UA_DateTime_now() + session->timeout * 10000; //timeout in ms
}
/*********************************** amalgamated original file "C:/AcpltDevelopmentKit/acplt/dev/open62541WrapperProject/open62541/src/server/ua_server.c" ***********************************/

const UA_EXPORT UA_ServerConfig UA_ServerConfig_standard = {
    .Login_enableAnonymous = UA_TRUE,
    .Login_enableUsernamePassword = UA_TRUE,
    .Login_usernames = (char *[]){"user1","user2"},
    .Login_passwords = (char *[]){"password","password1"},
    .Login_loginsCount = 2,
    .Application_applicationURI = "urn:unconfigured:open62541:open62541Server",
    .Application_applicationName = "open62541" };

/**********************/
/* Namespace Handling */
/**********************/

#ifdef UA_EXTERNAL_NAMESPACE
static void UA_ExternalNamespace_init(UA_ExternalNamespace *ens) {
    ens->index = 0;
    UA_String_init(&ens->url);
}

static void UA_ExternalNamespace_deleteMembers(UA_ExternalNamespace *ens) {
    UA_String_deleteMembers(&ens->url);
    ens->externalNodeStore.destroy(ens->externalNodeStore.ensHandle);
}

UA_StatusCode UA_EXPORT
UA_Server_addExternalNamespace(UA_Server *server, UA_UInt16 namespaceIndex, const UA_String *url, UA_ExternalNodeStore *nodeStore)
{
	if(nodeStore == UA_NULL){
		return UA_STATUSCODE_BADARGUMENTSMISSING;
	}
	//do not allow double indices
//	if(server->externalNamespaces->index == namespaceIndex){
//		return UA_STATUSCODE_BADINDEXRANGEINVALID;
//	}
    server->externalNamespaces = UA_realloc(server->externalNamespaces, sizeof(UA_ExternalNamespace) * (server->externalNamespacesSize+1));
    server->externalNamespaces[server->externalNamespacesSize].externalNodeStore = *nodeStore;
    server->externalNamespaces[server->externalNamespacesSize].index = namespaceIndex;
    UA_String_copy(url,&server->externalNamespaces[server->externalNamespacesSize].url);
    server->externalNamespacesSize++;
    return UA_STATUSCODE_GOOD;


}
#endif /* UA_EXTERNAL_NAMESPACE*/

UA_UInt16 UA_Server_addNamespace(UA_Server *server, const char* name) {
    server->namespaces = UA_realloc(server->namespaces, sizeof(UA_String) * (server->namespacesSize+1));
    server->namespaces[server->namespacesSize] = UA_STRING_ALLOC(name);
    server->namespacesSize++;
    return server->namespacesSize-1;
}
/*****************/
/* Configuration */
/*****************/

UA_Logger UA_Server_getLogger(UA_Server *server) {
    return server->logger;
}

void UA_Server_addNetworkLayer(UA_Server *server, UA_ServerNetworkLayer networkLayer) {
    UA_ServerNetworkLayer *newlayers =
        UA_realloc(server->networkLayers, sizeof(UA_ServerNetworkLayer)*(server->networkLayersSize+1));
    if(!newlayers) {
        UA_LOG_ERROR(server->logger, UA_LOGCATEGORY_SERVER, "Networklayer added");
        return;
    }
    server->networkLayers = newlayers;
    server->networkLayers[server->networkLayersSize] = networkLayer;
    server->networkLayersSize++;

    if(networkLayer.discoveryUrl){
        if(server->description.discoveryUrlsSize < 0)
            server->description.discoveryUrlsSize = 0;
        UA_String* newUrls = UA_realloc(server->description.discoveryUrls,
                                        sizeof(UA_String)*(server->description.discoveryUrlsSize+1));
        if(!newUrls) {
            UA_LOG_ERROR(server->logger, UA_LOGCATEGORY_SERVER, "Adding discoveryUrl");
            return;
        }
        server->description.discoveryUrls = newUrls;
        UA_String_copy(networkLayer.discoveryUrl,
                       &server->description.discoveryUrls[server->description.discoveryUrlsSize]);
        server->description.discoveryUrlsSize++;
        for(UA_Int32 i = 0; i < server->endpointDescriptionsSize; i++)
            if(!server->endpointDescriptions[i].endpointUrl.data)
                UA_String_copy(networkLayer.discoveryUrl, &server->endpointDescriptions[i].endpointUrl);
    }
}

void UA_Server_setServerCertificate(UA_Server *server, UA_ByteString certificate) {
    for(UA_Int32 i = 0; i < server->endpointDescriptionsSize; i++)
        UA_ByteString_copy(&certificate, &server->endpointDescriptions[i].serverCertificate);
}

void UA_Server_setLogger(UA_Server *server, UA_Logger logger) {
    server->logger = logger;
}


/**********/
/* Server */
/**********/

/* The server needs to be stopped before it can be deleted */
void UA_Server_delete(UA_Server *server) {
    // Delete the timed work
    UA_Server_deleteTimedWork(server);

    // Delete all internal data
    UA_ApplicationDescription_deleteMembers(&server->description);
    UA_SecureChannelManager_deleteMembers(&server->secureChannelManager);
    UA_SessionManager_deleteMembers(&server->sessionManager);
    UA_NodeStore_delete(server->nodestore);
    UA_ByteString_deleteMembers(&server->serverCertificate);
    UA_Array_delete(server->namespaces, &UA_TYPES[UA_TYPES_STRING], server->namespacesSize);
    UA_Array_delete(server->endpointDescriptions, &UA_TYPES[UA_TYPES_ENDPOINTDESCRIPTION],
                    server->endpointDescriptionsSize);

    // Delete the network layers
    for(size_t i = 0; i < server->networkLayersSize; i++) {
        server->networkLayers[i].free(server->networkLayers[i].nlHandle);
    }
    UA_free(server->networkLayers);

#ifdef UA_MULTITHREADING
    pthread_cond_destroy(&server->dispatchQueue_condition); // so the workers don't spin if the queue is empty
    rcu_barrier(); // wait for all scheduled call_rcu work to complete
#endif
    UA_free(server);
}

/* Recurring cleanup. Removing unused and timed-out channels and sessions */
static void UA_Server_cleanup(UA_Server *server, void *nothing) {
    UA_DateTime now = UA_DateTime_now();
    UA_SessionManager_cleanupTimedOut(&server->sessionManager, now);
    UA_SecureChannelManager_cleanupTimedOut(&server->secureChannelManager, now);
}

#define MANUFACTURER_NAME "open62541"
#define PRODUCT_NAME "open62541 OPC UA Server"
#define STRINGIFY(x) #x //some magic
#define TOSTRING(x) STRINGIFY(x) //some magic
#define SOFTWARE_VERSION TOSTRING(VERSION)
#define BUILD_NUMBER "0"

static void getBulidInfo(const UA_Server* server, UA_BuildInfo *buildInfo) {
    buildInfo->productUri = UA_STRING_ALLOC(PRODUCT_URI);
    buildInfo->manufacturerName = UA_STRING_ALLOC(MANUFACTURER_NAME);
    buildInfo->productName = UA_STRING_ALLOC(PRODUCT_NAME);
    buildInfo->softwareVersion = UA_STRING_ALLOC(SOFTWARE_VERSION);
    buildInfo->buildNumber = UA_STRING_ALLOC(BUILD_NUMBER);
    buildInfo->buildDate = server->buildDate;
}

static UA_StatusCode readStatus(void *handle, UA_Boolean sourceTimeStamp,
                                const UA_NumericRange *range, UA_DataValue *value) {
    if(range) {
        value->hasStatus = UA_TRUE;
        value->status = UA_STATUSCODE_BADINDEXRANGEINVALID;
        return UA_STATUSCODE_GOOD;
    }
    UA_ServerStatusDataType *status = UA_ServerStatusDataType_new();
    status->startTime   = ((const UA_Server*)handle)->startTime;
    status->currentTime = UA_DateTime_now();
    status->state       = UA_SERVERSTATE_RUNNING;
    getBulidInfo(((const UA_Server*)handle), &status->buildInfo);
    status->secondsTillShutdown = 0;

    value->value.type = &UA_TYPES[UA_TYPES_SERVERSTATUSDATATYPE];
    value->value.arrayLength = -1;
    value->value.data = status;
    value->value.arrayDimensionsSize = -1;
    value->value.arrayDimensions = UA_NULL;
    value->hasValue = UA_TRUE;
    if(sourceTimeStamp) {
        value->hasSourceTimestamp = UA_TRUE;
        value->sourceTimestamp = UA_DateTime_now();
    }
    return UA_STATUSCODE_GOOD;
}

static void releaseStatus(void *handle, UA_DataValue *value) {
    if(!value->hasValue)
        return;
    UA_ServerStatusDataType_delete((UA_ServerStatusDataType*)value->value.data);
    value->value.data = UA_NULL;
    value->hasValue = UA_FALSE;
    UA_DataValue_deleteMembers(value);
}

static UA_StatusCode readNamespaces(void *handle, UA_Boolean sourceTimestamp,
                                    const UA_NumericRange *range, UA_DataValue *value) {
    if(range) {
        value->hasStatus = UA_TRUE;
        value->status = UA_STATUSCODE_BADINDEXRANGEINVALID;
        return UA_STATUSCODE_GOOD;
    }
    UA_Server *server = (UA_Server*)handle;
    value->hasValue = UA_TRUE;
    value->value.storageType = UA_VARIANT_DATA_NODELETE;
    value->value.type = &UA_TYPES[UA_TYPES_STRING];
    value->value.arrayLength = server->namespacesSize;
    value->value.data = server->namespaces;
    if(sourceTimestamp) {
        value->hasSourceTimestamp = UA_TRUE;
        value->sourceTimestamp = UA_DateTime_now();
    }
    return UA_STATUSCODE_GOOD;
}

static void releaseNamespaces(void *handle, UA_DataValue *value) {
}

static UA_StatusCode readCurrentTime(void *handle, UA_Boolean sourceTimeStamp,
                                     const UA_NumericRange *range, UA_DataValue *value) {
    if(range) {
        value->hasStatus = UA_TRUE;
        value->status = UA_STATUSCODE_BADINDEXRANGEINVALID;
        return UA_STATUSCODE_GOOD;
    }
    UA_DateTime *currentTime = UA_DateTime_new();
    if(!currentTime)
        return UA_STATUSCODE_BADOUTOFMEMORY;
    *currentTime = UA_DateTime_now();
    value->value.type = &UA_TYPES[UA_TYPES_DATETIME];
    value->value.data = currentTime;
    value->hasValue = UA_TRUE;
    if(sourceTimeStamp) {
        value->hasSourceTimestamp = UA_TRUE;
        value->sourceTimestamp = *currentTime;
    }
    return UA_STATUSCODE_GOOD;
}

static void releaseCurrentTime(void *handle, UA_DataValue *value) {
    if(value->hasValue)
        UA_DateTime_delete((UA_DateTime*)value->value.data);
}

static void copyNames(UA_Node *node, char *name) {
    node->browseName = UA_QUALIFIEDNAME_ALLOC(0, name);
    node->displayName = UA_LOCALIZEDTEXT_ALLOC("", name);
    node->description = UA_LOCALIZEDTEXT_ALLOC("", name);
}

static void addDataTypeNode(UA_Server *server, char* name, UA_UInt32 datatypeid, UA_Int32 parent) {
    UA_DataTypeNode *datatype = UA_DataTypeNode_new();
    copyNames((UA_Node*)datatype, name);
    datatype->nodeId.identifier.numeric = datatypeid;
    UA_Server_addNode(server, (UA_Node*)datatype,
                      UA_EXPANDEDNODEID_NUMERIC(0, parent),
                      UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES));
}

static void addObjectTypeNode(UA_Server *server, char* name, UA_UInt32 objecttypeid, UA_Int32 parent,
                              UA_Int32 parentreference) {
    UA_ObjectTypeNode *objecttype = UA_ObjectTypeNode_new();
    copyNames((UA_Node*)objecttype, name);
    objecttype->nodeId.identifier.numeric = objecttypeid;
    UA_Server_addNode(server, (UA_Node*)objecttype,
                      UA_EXPANDEDNODEID_NUMERIC(0, parent),
                      UA_NODEID_NUMERIC(0, parentreference));
}

static UA_VariableTypeNode*
createVariableTypeNode(UA_Server *server, char* name, UA_UInt32 variabletypeid,
                       UA_Int32 parent, UA_Boolean abstract) {
    UA_VariableTypeNode *variabletype = UA_VariableTypeNode_new();
    copyNames((UA_Node*)variabletype, name);
    variabletype->nodeId.identifier.numeric = variabletypeid;
    variabletype->isAbstract = abstract;
    variabletype->value.variant.type = &UA_TYPES[UA_TYPES_VARIANT];
    return variabletype;
}

static void addVariableTypeNode_organized(UA_Server *server, char* name, UA_UInt32 variabletypeid,
                                          UA_Int32 parent, UA_Boolean abstract) {
    UA_VariableTypeNode *variabletype = createVariableTypeNode(server, name, variabletypeid, parent, abstract);
    UA_Server_addNode(server, (UA_Node*)variabletype,
                      UA_EXPANDEDNODEID_NUMERIC(0, parent),
                      UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES));
}

static void addVariableTypeNode_subtype(UA_Server *server, char* name, UA_UInt32 variabletypeid,
                                        UA_Int32 parent, UA_Boolean abstract) {
    UA_VariableTypeNode *variabletype = createVariableTypeNode(server, name, variabletypeid, parent, abstract);
    UA_Server_addNode(server, (UA_Node*)variabletype,
                      UA_EXPANDEDNODEID_NUMERIC(0, parent),
                      UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE));
}

UA_Server * UA_Server_new(UA_ServerConfig config) {
    UA_Server *server = UA_malloc(sizeof(UA_Server));
    if(!server)
        return UA_NULL;

    //FIXME: config contains strings, for now its okay, but consider copying them aswell
    server->config = config;

    LIST_INIT(&server->timedWork);
#ifdef UA_MULTITHREADING
    rcu_init();
    cds_wfcq_init(&server->dispatchQueue_head, &server->dispatchQueue_tail);
    server->delayedWork = UA_NULL;
#endif

    // logger
    server->logger = UA_NULL;

    // random seed
    server->random_seed = (UA_UInt32)UA_DateTime_now();

    // networklayers
    server->networkLayers = UA_NULL;
    server->networkLayersSize = 0;

    UA_ByteString_init(&server->serverCertificate);

    // mockup application description
    UA_ApplicationDescription_init(&server->description);
    server->description.productUri = UA_STRING_ALLOC(PRODUCT_URI);
    server->description.applicationUri = UA_STRING_ALLOC(server->config.Application_applicationURI);
    server->description.discoveryUrlsSize = 0;

    server->description.applicationName = UA_LOCALIZEDTEXT_ALLOC("", server->config.Application_applicationName);
    server->description.applicationType = UA_APPLICATIONTYPE_SERVER;
    server->externalNamespacesSize = 0;
    server->externalNamespaces = UA_NULL;

    /* ns0 and ns1 */
    server->namespaces = UA_Array_new(&UA_TYPES[UA_TYPES_STRING], 2);
    server->namespaces[0] = UA_STRING_ALLOC("http://opcfoundation.org/UA/");
    UA_String_copy(&server->description.applicationUri, &server->namespaces[1]);
    server->namespacesSize = 2;

    server->endpointDescriptions = UA_NULL;
    server->endpointDescriptionsSize = 0;

    UA_EndpointDescription *endpoint = UA_EndpointDescription_new(); // todo: check return code
    if(endpoint) {
        endpoint->securityMode = UA_MESSAGESECURITYMODE_NONE;
        endpoint->securityPolicyUri = UA_STRING_ALLOC("http://opcfoundation.org/UA/SecurityPolicy#None");
        endpoint->transportProfileUri = UA_STRING_ALLOC("http://opcfoundation.org/UA-Profile/Transport/uatcp-uasc-uabinary");

        int size = 0;
        if(server->config.Login_enableAnonymous){
            size++;
        }
        if(server->config.Login_enableUsernamePassword){
            size++;
        }
        endpoint->userIdentityTokensSize = size;
        endpoint->userIdentityTokens = UA_Array_new(&UA_TYPES[UA_TYPES_USERTOKENPOLICY], size);

        int currentIndex = 0;
        if(server->config.Login_enableAnonymous){
            UA_UserTokenPolicy_init(&endpoint->userIdentityTokens[currentIndex]);
            endpoint->userIdentityTokens[currentIndex].tokenType = UA_USERTOKENTYPE_ANONYMOUS;
            endpoint->userIdentityTokens[currentIndex].policyId = UA_STRING_ALLOC(ANONYMOUS_POLICY); // defined per server
            currentIndex++;
        }

        if(server->config.Login_enableUsernamePassword){
            UA_UserTokenPolicy_init(&endpoint->userIdentityTokens[currentIndex]);
            endpoint->userIdentityTokens[currentIndex].tokenType = UA_USERTOKENTYPE_USERNAME;
            endpoint->userIdentityTokens[currentIndex].policyId = UA_STRING_ALLOC(USERNAME_POLICY); // defined per server
            currentIndex++;
        }

        /* UA_String_copy(endpointUrl, &endpoint->endpointUrl); */
        /* /\* The standard says "the HostName specified in the Server Certificate is the */
        /*    same as the HostName contained in the endpointUrl provided in the */
        /*    EndpointDescription *\/ */
        /* UA_String_copy(&server->serverCertificate, &endpoint->serverCertificate); */
        UA_ApplicationDescription_copy(&server->description, &endpoint->server);

        server->endpointDescriptions = endpoint;
        server->endpointDescriptionsSize = 1;
    } 

#define MAXCHANNELCOUNT 100
#define STARTCHANNELID 1
#define TOKENLIFETIME 600000
#define STARTTOKENID 1
    UA_SecureChannelManager_init(&server->secureChannelManager, MAXCHANNELCOUNT,
                                 TOKENLIFETIME, STARTCHANNELID, STARTTOKENID);

#define MAXSESSIONCOUNT 1000
#define MAXSESSIONLIFETIME 10000
#define STARTSESSIONID 1
    UA_SessionManager_init(&server->sessionManager, MAXSESSIONCOUNT, MAXSESSIONLIFETIME, STARTSESSIONID);

    server->nodestore = UA_NodeStore_new();

    UA_WorkItem cleanup = {.type = UA_WORKITEMTYPE_METHODCALL,
                           .work.methodCall = {.method = UA_Server_cleanup, .data = NULL} };
    UA_Server_addRepeatedWorkItem(server, &cleanup, 10000, NULL);

    /**********************/
    /* Server Information */
    /**********************/

    server->startTime = UA_DateTime_now();
    static struct tm ct;
    ct.tm_year = (__DATE__[7] - '0') * 1000 +  (__DATE__[8] - '0') * 100 + (__DATE__[9] - '0') * 10 + (__DATE__[10] - '0')- 1900;
    if ((__DATE__[0]=='J') && (__DATE__[1]=='a') && (__DATE__[2]=='n')) ct.tm_mon = 1-1;
    else if ((__DATE__[0]=='F') && (__DATE__[1]=='e') && (__DATE__[2]=='b')) ct.tm_mon = 2-1;
    else if ((__DATE__[0]=='M') && (__DATE__[1]=='a') && (__DATE__[2]=='r')) ct.tm_mon = 3-1;
    else if ((__DATE__[0]=='A') && (__DATE__[1]=='p') && (__DATE__[2]=='r')) ct.tm_mon = 4-1;
    else if ((__DATE__[0]=='M') && (__DATE__[1]=='a') && (__DATE__[2]=='y')) ct.tm_mon = 5-1;
    else if ((__DATE__[0]=='J') && (__DATE__[1]=='u') && (__DATE__[2]=='n')) ct.tm_mon = 6-1;
    else if ((__DATE__[0]=='J') && (__DATE__[1]=='u') && (__DATE__[2]=='l')) ct.tm_mon = 7-1;
    else if ((__DATE__[0]=='A') && (__DATE__[1]=='u') && (__DATE__[2]=='g')) ct.tm_mon = 8-1;
    else if ((__DATE__[0]=='S') && (__DATE__[1]=='e') && (__DATE__[2]=='p')) ct.tm_mon = 9-1;
    else if ((__DATE__[0]=='O') && (__DATE__[1]=='c') && (__DATE__[2]=='t')) ct.tm_mon = 10-1;
    else if ((__DATE__[0]=='N') && (__DATE__[1]=='o') && (__DATE__[2]=='v')) ct.tm_mon = 11-1;
    else if ((__DATE__[0]=='D') && (__DATE__[1]=='e') && (__DATE__[2]=='c')) ct.tm_mon = 12-1;

    // special case to handle __DATE__ not inserting leading zero on day of month
    // if Day of month is less than 10 - it inserts a blank character
    // this results in a negative number for tm_mday

    if(__DATE__[4] == ' ')
        ct.tm_mday =  __DATE__[5]-'0';
    else
        ct.tm_mday = (__DATE__[4]-'0')*10 + (__DATE__[5]-'0');
    ct.tm_hour = ((__TIME__[0] - '0') * 10 + __TIME__[1] - '0');
    ct.tm_min = ((__TIME__[3] - '0') * 10 + __TIME__[4] - '0');
    ct.tm_sec = ((__TIME__[6] - '0') * 10 + __TIME__[7] - '0');
    ct.tm_isdst = -1;  // information is not available.

    //FIXME: next 3 lines are copy-pasted from ua_types.c
#define UNIX_EPOCH_BIAS_SEC 11644473600LL // Number of seconds from 1 Jan. 1601 00:00 to 1 Jan 1970 00:00 UTC
#define HUNDRED_NANOSEC_PER_USEC 10LL
#define HUNDRED_NANOSEC_PER_SEC (HUNDRED_NANOSEC_PER_USEC * 1000000LL)
    server->buildDate = (mktime(&ct) + UNIX_EPOCH_BIAS_SEC) * HUNDRED_NANOSEC_PER_SEC;

    /**************/
    /* References */
    /**************/
    
    /* bootstrap by manually inserting "references" and "hassubtype" */
    UA_ReferenceTypeNode *references = UA_ReferenceTypeNode_new();
    copyNames((UA_Node*)references, "References");
    references->nodeId.identifier.numeric = UA_NS0ID_REFERENCES;
    references->isAbstract = UA_TRUE;
    references->symmetric  = UA_TRUE;
    references->inverseName = UA_LOCALIZEDTEXT_ALLOC("", "References");
    // this node has no parent??
    UA_NodeStore_insert(server->nodestore, (UA_Node*)references, UA_NULL);

    UA_ReferenceTypeNode *hassubtype = UA_ReferenceTypeNode_new();
    copyNames((UA_Node*)hassubtype, "HasSubtype");
    hassubtype->inverseName = UA_LOCALIZEDTEXT_ALLOC("", "HasSupertype");
    hassubtype->nodeId.identifier.numeric = UA_NS0ID_HASSUBTYPE;
    hassubtype->isAbstract = UA_FALSE;
    hassubtype->symmetric  = UA_FALSE;
    UA_NodeStore_insert(server->nodestore, (UA_Node*)hassubtype, UA_NULL);

    /* continue adding reference types with normal "addnode" */
    UA_ReferenceTypeNode *hierarchicalreferences = UA_ReferenceTypeNode_new();
    copyNames((UA_Node*)hierarchicalreferences, "Hierarchicalreferences");
    hierarchicalreferences->nodeId.identifier.numeric = UA_NS0ID_HIERARCHICALREFERENCES;
    hierarchicalreferences->isAbstract = UA_TRUE;
    hierarchicalreferences->symmetric  = UA_FALSE;
    UA_Server_addNode(server, (UA_Node*)hierarchicalreferences,
                      UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_REFERENCES),
                      UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE));

    UA_ReferenceTypeNode *nonhierarchicalreferences = UA_ReferenceTypeNode_new();
    copyNames((UA_Node*)nonhierarchicalreferences, "NonHierarchicalReferences");
    nonhierarchicalreferences->nodeId.identifier.numeric = UA_NS0ID_NONHIERARCHICALREFERENCES;
    nonhierarchicalreferences->isAbstract = UA_TRUE;
    nonhierarchicalreferences->symmetric  = UA_FALSE;
    UA_Server_addNode(server, (UA_Node*)nonhierarchicalreferences,
                      UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_REFERENCES),
                      UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE));

    UA_ReferenceTypeNode *haschild = UA_ReferenceTypeNode_new();
    copyNames((UA_Node*)haschild, "HasChild");
    haschild->nodeId.identifier.numeric = UA_NS0ID_HASCHILD;
    haschild->isAbstract = UA_TRUE;
    haschild->symmetric  = UA_FALSE;
    UA_Server_addNode(server, (UA_Node*)haschild,
                      UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_HIERARCHICALREFERENCES),
                      UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE));

    UA_ReferenceTypeNode *organizes = UA_ReferenceTypeNode_new();
    copyNames((UA_Node*)organizes, "Organizes");
    organizes->inverseName = UA_LOCALIZEDTEXT_ALLOC("", "OrganizedBy");
    organizes->nodeId.identifier.numeric = UA_NS0ID_ORGANIZES;
    organizes->isAbstract = UA_FALSE;
    organizes->symmetric  = UA_FALSE;
    UA_Server_addNode(server, (UA_Node*)organizes,
                      UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_HIERARCHICALREFERENCES),
                      UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE));

    UA_ReferenceTypeNode *haseventsource = UA_ReferenceTypeNode_new();
    copyNames((UA_Node*)haseventsource, "HasEventSource");
    haseventsource->inverseName = UA_LOCALIZEDTEXT_ALLOC("", "EventSourceOf");
    haseventsource->nodeId.identifier.numeric = UA_NS0ID_HASEVENTSOURCE;
    haseventsource->isAbstract = UA_FALSE;
    haseventsource->symmetric  = UA_FALSE;
    UA_Server_addNode(server, (UA_Node*)haseventsource,
                      UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_HIERARCHICALREFERENCES),
                      UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE));

    UA_ReferenceTypeNode *hasmodellingrule = UA_ReferenceTypeNode_new();
    copyNames((UA_Node*)hasmodellingrule, "HasModellingRule");
    hasmodellingrule->inverseName = UA_LOCALIZEDTEXT_ALLOC("", "ModellingRuleOf");
    hasmodellingrule->nodeId.identifier.numeric = UA_NS0ID_HASMODELLINGRULE;
    hasmodellingrule->isAbstract = UA_FALSE;
    hasmodellingrule->symmetric  = UA_FALSE;
    UA_Server_addNode(server, (UA_Node*)hasmodellingrule,
                      UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_NONHIERARCHICALREFERENCES),
                      UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE));

    UA_ReferenceTypeNode *hasencoding = UA_ReferenceTypeNode_new();
    copyNames((UA_Node*)hasencoding, "HasEncoding");
    hasencoding->inverseName = UA_LOCALIZEDTEXT_ALLOC("", "EncodingOf");
    hasencoding->nodeId.identifier.numeric = UA_NS0ID_HASENCODING;
    hasencoding->isAbstract = UA_FALSE;
    hasencoding->symmetric  = UA_FALSE;
    UA_Server_addNode(server, (UA_Node*)hasencoding,
                      UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_NONHIERARCHICALREFERENCES),
                      UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE));

    UA_ReferenceTypeNode *hasdescription = UA_ReferenceTypeNode_new();
    copyNames((UA_Node*)hasdescription, "HasDescription");
    hasdescription->inverseName = UA_LOCALIZEDTEXT_ALLOC("", "DescriptionOf");
    hasdescription->nodeId.identifier.numeric = UA_NS0ID_HASDESCRIPTION;
    hasdescription->isAbstract = UA_FALSE;
    hasdescription->symmetric  = UA_FALSE;
    UA_Server_addNode(server, (UA_Node*)hasdescription,
                      UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_NONHIERARCHICALREFERENCES),
                      UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE));

    UA_ReferenceTypeNode *hastypedefinition = UA_ReferenceTypeNode_new();
    copyNames((UA_Node*)hastypedefinition, "HasTypeDefinition");
    hastypedefinition->inverseName = UA_LOCALIZEDTEXT_ALLOC("", "TypeDefinitionOf");
    hastypedefinition->nodeId.identifier.numeric = UA_NS0ID_HASTYPEDEFINITION;
    hastypedefinition->isAbstract = UA_FALSE;
    hastypedefinition->symmetric  = UA_FALSE;
    UA_Server_addNode(server, (UA_Node*)hastypedefinition,
                      UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_NONHIERARCHICALREFERENCES),
                      UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE));

    UA_ReferenceTypeNode *generatesevent = UA_ReferenceTypeNode_new();
    copyNames((UA_Node*)generatesevent, "GeneratesEvent");
    generatesevent->inverseName = UA_LOCALIZEDTEXT_ALLOC("", "GeneratedBy");
    generatesevent->nodeId.identifier.numeric = UA_NS0ID_GENERATESEVENT;
    generatesevent->isAbstract = UA_FALSE;
    generatesevent->symmetric  = UA_FALSE;
    UA_Server_addNode(server, (UA_Node*)generatesevent,
                      UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_NONHIERARCHICALREFERENCES),
                      UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE));

    UA_ReferenceTypeNode *aggregates = UA_ReferenceTypeNode_new();
    copyNames((UA_Node*)aggregates, "Aggregates");
    // Todo: Is there an inverse name?
    aggregates->nodeId.identifier.numeric = UA_NS0ID_AGGREGATES;
    aggregates->isAbstract = UA_TRUE;
    aggregates->symmetric  = UA_FALSE;
    UA_Server_addNode(server, (UA_Node*)aggregates,
                      UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_HASCHILD),
                      UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE));

    // complete bootstrap of hassubtype
    ADDREFERENCE(UA_NODEID_NUMERIC(0, UA_NS0ID_HASCHILD), UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE),
                 UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE));

    UA_ReferenceTypeNode *hasproperty = UA_ReferenceTypeNode_new();
    copyNames((UA_Node*)hasproperty, "HasProperty");
    hasproperty->inverseName = UA_LOCALIZEDTEXT_ALLOC("", "PropertyOf");
    hasproperty->nodeId.identifier.numeric = UA_NS0ID_HASPROPERTY;
    hasproperty->isAbstract = UA_FALSE;
    hasproperty->symmetric  = UA_FALSE;
    UA_Server_addNode(server, (UA_Node*)hasproperty,
                      UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_AGGREGATES),
                      UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE));

    UA_ReferenceTypeNode *hascomponent = UA_ReferenceTypeNode_new();
    copyNames((UA_Node*)hascomponent, "HasComponent");
    hascomponent->inverseName = UA_LOCALIZEDTEXT_ALLOC("", "ComponentOf");
    hascomponent->nodeId.identifier.numeric = UA_NS0ID_HASCOMPONENT;
    hascomponent->isAbstract = UA_FALSE;
    hascomponent->symmetric  = UA_FALSE;
    UA_Server_addNode(server, (UA_Node*)hascomponent,
                      UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_AGGREGATES),
                      UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE));

    UA_ReferenceTypeNode *hasnotifier = UA_ReferenceTypeNode_new();
    copyNames((UA_Node*)hasnotifier, "HasNotifier");
    hasnotifier->inverseName = UA_LOCALIZEDTEXT_ALLOC("", "NotifierOf");
    hasnotifier->nodeId.identifier.numeric = UA_NS0ID_HASNOTIFIER;
    hasnotifier->isAbstract = UA_FALSE;
    hasnotifier->symmetric  = UA_FALSE;
    UA_Server_addNode(server, (UA_Node*)hasnotifier,
                      UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_HASEVENTSOURCE),
                      UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE));

    UA_ReferenceTypeNode *hasorderedcomponent = UA_ReferenceTypeNode_new();
    copyNames((UA_Node*)hasorderedcomponent, "HasOrderedComponent");
    hasorderedcomponent->inverseName = UA_LOCALIZEDTEXT_ALLOC("", "OrderedComponentOf");
    hasorderedcomponent->nodeId.identifier.numeric = UA_NS0ID_HASORDEREDCOMPONENT;
    hasorderedcomponent->isAbstract = UA_FALSE;
    hasorderedcomponent->symmetric  = UA_FALSE;
    UA_Server_addNode(server, (UA_Node*)hasorderedcomponent,
                      UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                      UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE));

    UA_ReferenceTypeNode *hasmodelparent = UA_ReferenceTypeNode_new();
    copyNames((UA_Node*)hasmodelparent, "HasModelParent");
    hasmodelparent->inverseName = UA_LOCALIZEDTEXT_ALLOC("", "ModelParentOf");
    hasmodelparent->nodeId.identifier.numeric = UA_NS0ID_HASMODELPARENT;
    hasmodelparent->isAbstract = UA_FALSE;
    hasmodelparent->symmetric  = UA_FALSE;
    UA_Server_addNode(server, (UA_Node*)hasmodelparent,
                      UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_NONHIERARCHICALREFERENCES),
                      UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE));

    UA_ReferenceTypeNode *fromstate = UA_ReferenceTypeNode_new();
    copyNames((UA_Node*)fromstate, "FromState");
    fromstate->inverseName = UA_LOCALIZEDTEXT_ALLOC("", "ToTransition");
    fromstate->nodeId.identifier.numeric = UA_NS0ID_FROMSTATE;
    fromstate->isAbstract = UA_FALSE;
    fromstate->symmetric  = UA_FALSE;
    UA_Server_addNode(server, (UA_Node*)fromstate,
                      UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_NONHIERARCHICALREFERENCES),
                      UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE));

    UA_ReferenceTypeNode *tostate = UA_ReferenceTypeNode_new();
    copyNames((UA_Node*)tostate, "ToState");
    tostate->inverseName = UA_LOCALIZEDTEXT_ALLOC("", "FromTransition");
    tostate->nodeId.identifier.numeric = UA_NS0ID_TOSTATE;
    tostate->isAbstract = UA_FALSE;
    tostate->symmetric  = UA_FALSE;
    UA_Server_addNode(server, (UA_Node*)tostate,
                      UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_NONHIERARCHICALREFERENCES),
                      UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE));

    UA_ReferenceTypeNode *hascause = UA_ReferenceTypeNode_new();
    copyNames((UA_Node*)hascause, "HasCause");
    hascause->inverseName = UA_LOCALIZEDTEXT_ALLOC("", "MayBeCausedBy");
    hascause->nodeId.identifier.numeric = UA_NS0ID_HASCAUSE;
    hascause->isAbstract = UA_FALSE;
    hascause->symmetric  = UA_FALSE;
    UA_Server_addNode(server, (UA_Node*)hascause,
                      UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_NONHIERARCHICALREFERENCES),
                      UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE));
    
    UA_ReferenceTypeNode *haseffect = UA_ReferenceTypeNode_new();
    copyNames((UA_Node*)haseffect, "HasEffect");
    haseffect->inverseName = UA_LOCALIZEDTEXT_ALLOC("", "MayBeEffectedBy");
    haseffect->nodeId.identifier.numeric = UA_NS0ID_HASEFFECT;
    haseffect->isAbstract = UA_FALSE;
    haseffect->symmetric  = UA_FALSE;
    UA_Server_addNode(server, (UA_Node*)haseffect,
                      UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_NONHIERARCHICALREFERENCES),
                      UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE));

    UA_ReferenceTypeNode *hashistoricalconfiguration = UA_ReferenceTypeNode_new();
    copyNames((UA_Node*)hashistoricalconfiguration, "HasHistoricalConfiguration");
    hashistoricalconfiguration->inverseName = UA_LOCALIZEDTEXT_ALLOC("", "HistoricalConfigurationOf");
    hashistoricalconfiguration->nodeId.identifier.numeric = UA_NS0ID_HASHISTORICALCONFIGURATION;
    hashistoricalconfiguration->isAbstract = UA_FALSE;
    hashistoricalconfiguration->symmetric  = UA_FALSE;
    UA_Server_addNode(server, (UA_Node*)hashistoricalconfiguration,
                      UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_AGGREGATES),
                      UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE));

    /*****************/
    /* Basic Folders */
    /*****************/

    UA_ObjectNode *root = UA_ObjectNode_new();
    copyNames((UA_Node*)root, "Root");
    root->nodeId.identifier.numeric = UA_NS0ID_ROOTFOLDER;
    UA_NodeStore_insert(server->nodestore, (UA_Node*)root, UA_NULL);

    UA_ObjectNode *objects = UA_ObjectNode_new();
    copyNames((UA_Node*)objects, "Objects");
    objects->nodeId.identifier.numeric = UA_NS0ID_OBJECTSFOLDER;
    UA_Server_addNode(server, (UA_Node*)objects,
        UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_ROOTFOLDER),
        UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES));

    UA_ObjectNode *types = UA_ObjectNode_new();
    copyNames((UA_Node*)types, "Types");
    types->nodeId.identifier.numeric = UA_NS0ID_TYPESFOLDER;
    UA_Server_addNode(server, (UA_Node*)types,
        UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_ROOTFOLDER),
        UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES));

    UA_ObjectNode *views = UA_ObjectNode_new();
    copyNames((UA_Node*)views, "Views");
    views->nodeId.identifier.numeric = UA_NS0ID_VIEWSFOLDER;
    UA_Server_addNode(server, (UA_Node*)views,
        UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_ROOTFOLDER),
        UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES));

    UA_ObjectNode *referencetypes = UA_ObjectNode_new();
    copyNames((UA_Node*)referencetypes, "ReferenceTypes");
    referencetypes->nodeId.identifier.numeric = UA_NS0ID_REFERENCETYPESFOLDER;
    UA_Server_addNode(server, (UA_Node*)referencetypes,
        UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_TYPESFOLDER),
        UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES));

    ADDREFERENCE(UA_NODEID_NUMERIC(0, UA_NS0ID_REFERENCETYPESFOLDER), UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
        UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_REFERENCES));

    /**********************/
    /* Basic Object Types */
    /**********************/

    UA_ObjectNode *objecttypes = UA_ObjectNode_new();
    copyNames((UA_Node*)objecttypes, "ObjectTypes");
    objecttypes->nodeId.identifier.numeric = UA_NS0ID_OBJECTTYPESFOLDER;
    UA_Server_addNode(server, (UA_Node*)objecttypes,
        UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_TYPESFOLDER),
        UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES));

    addObjectTypeNode(server, "BaseObjectType", UA_NS0ID_BASEOBJECTTYPE, UA_NS0ID_OBJECTTYPESFOLDER, UA_NS0ID_ORGANIZES);
    addObjectTypeNode(server, "FolderType", UA_NS0ID_FOLDERTYPE, UA_NS0ID_BASEOBJECTTYPE, UA_NS0ID_HASSUBTYPE);
    ADDREFERENCE(UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTTYPESFOLDER), UA_NODEID_NUMERIC(0, UA_NS0ID_HASTYPEDEFINITION),
        UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_FOLDERTYPE));
    ADDREFERENCE(UA_NODEID_NUMERIC(0, UA_NS0ID_ROOTFOLDER), UA_NODEID_NUMERIC(0, UA_NS0ID_HASTYPEDEFINITION),
        UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_FOLDERTYPE));
    ADDREFERENCE(UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER), UA_NODEID_NUMERIC(0, UA_NS0ID_HASTYPEDEFINITION),
        UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_FOLDERTYPE));
    ADDREFERENCE(UA_NODEID_NUMERIC(0, UA_NS0ID_TYPESFOLDER), UA_NODEID_NUMERIC(0, UA_NS0ID_HASTYPEDEFINITION),
        UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_FOLDERTYPE));
    ADDREFERENCE(UA_NODEID_NUMERIC(0, UA_NS0ID_VIEWSFOLDER), UA_NODEID_NUMERIC(0, UA_NS0ID_HASTYPEDEFINITION),
        UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_FOLDERTYPE));
    ADDREFERENCE(UA_NODEID_NUMERIC(0, UA_NS0ID_REFERENCETYPESFOLDER), UA_NODEID_NUMERIC(0, UA_NS0ID_HASTYPEDEFINITION),
        UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_FOLDERTYPE));
    addObjectTypeNode(server, "ServerType", UA_NS0ID_SERVERTYPE, UA_NS0ID_BASEOBJECTTYPE, UA_NS0ID_HASSUBTYPE);
    addObjectTypeNode(server, "ServerDiagnosticsType", UA_NS0ID_SERVERDIAGNOSTICSTYPE, UA_NS0ID_BASEOBJECTTYPE, UA_NS0ID_HASSUBTYPE);
    addObjectTypeNode(server, "ServerCapatilitiesType", UA_NS0ID_SERVERCAPABILITIESTYPE, UA_NS0ID_BASEOBJECTTYPE, UA_NS0ID_HASSUBTYPE);
    addObjectTypeNode(server, "ServerStatusType", UA_NS0ID_SERVERSTATUSTYPE, UA_NS0ID_BASEOBJECTTYPE, UA_NS0ID_HASSUBTYPE);
    addObjectTypeNode(server, "BuildInfoType", UA_NS0ID_BUILDINFOTYPE, UA_NS0ID_BASEOBJECTTYPE, UA_NS0ID_HASSUBTYPE);

    /**************/
    /* Data Types */
    /**************/

    UA_ObjectNode *datatypes = UA_ObjectNode_new();
    copyNames((UA_Node*)datatypes, "DataTypes");
    datatypes->nodeId.identifier.numeric = UA_NS0ID_DATATYPESFOLDER;
    UA_Server_addNode(server, (UA_Node*)datatypes,
                      UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_TYPESFOLDER),
                      UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES));
    ADDREFERENCE(UA_NODEID_NUMERIC(0, UA_NS0ID_DATATYPESFOLDER), UA_NODEID_NUMERIC(0, UA_NS0ID_HASTYPEDEFINITION),
                 UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_FOLDERTYPE));

    addDataTypeNode(server, "BaseDataType", UA_NS0ID_BASEDATATYPE, UA_NS0ID_DATATYPESFOLDER);
    addDataTypeNode(server, "Boolean", UA_NS0ID_BOOLEAN, UA_NS0ID_BASEDATATYPE);
    addDataTypeNode(server, "Number", UA_NS0ID_NUMBER, UA_NS0ID_BASEDATATYPE);
        addDataTypeNode(server, "Float", UA_NS0ID_FLOAT, UA_NS0ID_NUMBER);
        addDataTypeNode(server, "Double", UA_NS0ID_DOUBLE, UA_NS0ID_NUMBER);
        addDataTypeNode(server, "Integer", UA_NS0ID_INTEGER, UA_NS0ID_NUMBER);
            addDataTypeNode(server, "SByte", UA_NS0ID_SBYTE, UA_NS0ID_INTEGER);
            addDataTypeNode(server, "Int16", UA_NS0ID_INT16, UA_NS0ID_INTEGER);
            addDataTypeNode(server, "Int32", UA_NS0ID_INT32, UA_NS0ID_INTEGER);
            addDataTypeNode(server, "Int64", UA_NS0ID_INT64, UA_NS0ID_INTEGER);
            addDataTypeNode(server, "UInteger", UA_NS0ID_UINTEGER, UA_NS0ID_INTEGER);
                addDataTypeNode(server, "Byte", UA_NS0ID_BYTE, UA_NS0ID_UINTEGER);
                addDataTypeNode(server, "UInt16", UA_NS0ID_UINT16, UA_NS0ID_UINTEGER);
                addDataTypeNode(server, "UInt32", UA_NS0ID_UINT32, UA_NS0ID_UINTEGER);
                addDataTypeNode(server, "UInt64", UA_NS0ID_UINT64, UA_NS0ID_UINTEGER);
    addDataTypeNode(server, "String", UA_NS0ID_STRING, UA_NS0ID_BASEDATATYPE);
    addDataTypeNode(server, "DateTime", UA_NS0ID_DATETIME, UA_NS0ID_BASEDATATYPE);
    addDataTypeNode(server, "Guid", UA_NS0ID_GUID, UA_NS0ID_BASEDATATYPE);
    addDataTypeNode(server, "ByteString", UA_NS0ID_BYTESTRING, UA_NS0ID_BASEDATATYPE);
    addDataTypeNode(server, "XmlElement", UA_NS0ID_XMLELEMENT, UA_NS0ID_BASEDATATYPE);
    addDataTypeNode(server, "NodeId", UA_NS0ID_NODEID, UA_NS0ID_BASEDATATYPE);
    addDataTypeNode(server, "ExpandedNodeId", UA_NS0ID_EXPANDEDNODEID, UA_NS0ID_BASEDATATYPE);
    addDataTypeNode(server, "StatusCode", UA_NS0ID_STATUSCODE, UA_NS0ID_BASEDATATYPE);
    addDataTypeNode(server, "QualifiedName", UA_NS0ID_QUALIFIEDNAME, UA_NS0ID_BASEDATATYPE);
    addDataTypeNode(server, "LocalizedText", UA_NS0ID_LOCALIZEDTEXT, UA_NS0ID_BASEDATATYPE);
    addDataTypeNode(server, "Structure", UA_NS0ID_STRUCTURE, UA_NS0ID_BASEDATATYPE);
        addDataTypeNode(server, "ServerStatusDataType", UA_NS0ID_SERVERSTATUSDATATYPE, UA_NS0ID_STRUCTURE);
        addDataTypeNode(server, "BuildInfo", UA_NS0ID_BUILDINFO, UA_NS0ID_STRUCTURE);
    addDataTypeNode(server, "DataValue", UA_NS0ID_DATAVALUE, UA_NS0ID_BASEDATATYPE);
    addDataTypeNode(server, "DiagnosticInfo", UA_NS0ID_DIAGNOSTICINFO, UA_NS0ID_BASEDATATYPE);
    addDataTypeNode(server, "Enumeration", UA_NS0ID_ENUMERATION, UA_NS0ID_BASEDATATYPE);
        addDataTypeNode(server, "ServerState", UA_NS0ID_SERVERSTATE, UA_NS0ID_ENUMERATION);


   UA_ObjectNode *variabletypes = UA_ObjectNode_new();
   copyNames((UA_Node*)variabletypes, "VariableTypes");
   variabletypes->nodeId.identifier.numeric = UA_NS0ID_VARIABLETYPESFOLDER;
   UA_Server_addNode(server, (UA_Node*)variabletypes,
                     UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_TYPESFOLDER),
                     UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES));
   ADDREFERENCE(UA_NODEID_NUMERIC(0, UA_NS0ID_VARIABLETYPESFOLDER), UA_NODEID_NUMERIC(0, UA_NS0ID_HASTYPEDEFINITION),
                UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_FOLDERTYPE));
   addVariableTypeNode_organized(server, "BaseVariableType", UA_NS0ID_BASEVARIABLETYPE, UA_NS0ID_VARIABLETYPESFOLDER, UA_TRUE);
   addVariableTypeNode_subtype(server, "BaseDataVariableType", UA_NS0ID_BASEDATAVARIABLETYPE, UA_NS0ID_BASEVARIABLETYPE, UA_FALSE);
   addVariableTypeNode_subtype(server, "PropertyType", UA_NS0ID_PROPERTYTYPE, UA_NS0ID_BASEVARIABLETYPE, UA_FALSE);

   /*********************/
   /* The Server Object */
   /*********************/

   UA_ObjectNode *servernode = UA_ObjectNode_new();
   copyNames((UA_Node*)servernode, "Server");
   servernode->nodeId.identifier.numeric = UA_NS0ID_SERVER;
   UA_Server_addNode(server, (UA_Node*)servernode,
           UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
           UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES));
   ADDREFERENCE(UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER),
       UA_NODEID_NUMERIC(0, UA_NS0ID_HASTYPEDEFINITION),
       UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_SERVERTYPE));

   UA_VariableNode *namespaceArray = UA_VariableNode_new();
   copyNames((UA_Node*)namespaceArray, "NamespaceArray");
   namespaceArray->nodeId.identifier.numeric = UA_NS0ID_SERVER_NAMESPACEARRAY;
   namespaceArray->valueSource = UA_VALUESOURCE_DATASOURCE;
   namespaceArray->value.dataSource = (UA_DataSource) {.handle = server, .read = readNamespaces,
       .release = releaseNamespaces, .write = UA_NULL};
   namespaceArray->valueRank = 1;
   namespaceArray->minimumSamplingInterval = 1.0;
   namespaceArray->historizing = UA_FALSE;
   UA_Server_addNode(server, (UA_Node*)namespaceArray,
           UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_SERVER),
           UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY));
   ADDREFERENCE(UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_NAMESPACEARRAY),
                UA_NODEID_NUMERIC(0, UA_NS0ID_HASTYPEDEFINITION),
                UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_PROPERTYTYPE));

   UA_VariableNode *serverArray = UA_VariableNode_new();
   copyNames((UA_Node*)serverArray, "ServerArray");
   serverArray->nodeId.identifier.numeric = UA_NS0ID_SERVER_SERVERARRAY;
   serverArray->value.variant.data = UA_Array_new(&UA_TYPES[UA_TYPES_STRING], 1);
   serverArray->value.variant.arrayLength = 1;
   serverArray->value.variant.type = &UA_TYPES[UA_TYPES_STRING];
   *(UA_String *)serverArray->value.variant.data = UA_STRING_ALLOC(server->config.Application_applicationURI);
   serverArray->valueRank = 1;
   serverArray->minimumSamplingInterval = 1.0;
   serverArray->historizing = UA_FALSE;
   UA_Server_addNode(server, (UA_Node*)serverArray,
           UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_SERVER),
           UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY));
   ADDREFERENCE(UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERARRAY),
                UA_NODEID_NUMERIC(0, UA_NS0ID_HASTYPEDEFINITION),
                UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_PROPERTYTYPE));

   UA_ObjectNode *servercapablities = UA_ObjectNode_new();
   copyNames((UA_Node*)servercapablities, "ServerCapabilities");
   servercapablities->nodeId.identifier.numeric = UA_NS0ID_SERVER_SERVERCAPABILITIES;
   UA_Server_addNode(server, (UA_Node*)servercapablities,
           UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_SERVER),
           UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT));
   ADDREFERENCE(UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERCAPABILITIES), UA_NODEID_NUMERIC(0, UA_NS0ID_HASTYPEDEFINITION),
       UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_SERVERCAPABILITIESTYPE));

   UA_VariableNode *localeIdArray = UA_VariableNode_new();
   copyNames((UA_Node*)localeIdArray, "LocaleIdArray");
   localeIdArray->nodeId.identifier.numeric = UA_NS0ID_SERVER_SERVERCAPABILITIES_LOCALEIDARRAY;
   localeIdArray->value.variant.data = UA_Array_new(&UA_TYPES[UA_TYPES_STRING], 1);
   localeIdArray->value.variant.arrayLength = 1;
   localeIdArray->value.variant.type = &UA_TYPES[UA_TYPES_STRING];
   *(UA_String *)localeIdArray->value.variant.data = UA_STRING_ALLOC("en");
   localeIdArray->valueRank = 1;
   localeIdArray->minimumSamplingInterval = 1.0;
   localeIdArray->historizing = UA_FALSE;
   UA_Server_addNode(server, (UA_Node*)localeIdArray,
           UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERCAPABILITIES),
           UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY));
   ADDREFERENCE(UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERCAPABILITIES_LOCALEIDARRAY), UA_NODEID_NUMERIC(0, UA_NS0ID_HASTYPEDEFINITION),
           UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_PROPERTYTYPE));

   UA_VariableNode *maxBrowseContinuationPoints = UA_VariableNode_new();
    copyNames((UA_Node*)maxBrowseContinuationPoints, "MaxBrowseContinuationPoints");
    maxBrowseContinuationPoints->nodeId.identifier.numeric = UA_NS0ID_SERVER_SERVERCAPABILITIES_MAXBROWSECONTINUATIONPOINTS;
    maxBrowseContinuationPoints->value.variant.data = UA_UInt16_new();
    *((UA_UInt16*)maxBrowseContinuationPoints->value.variant.data) = MAXCONTINUATIONPOINTS;
    maxBrowseContinuationPoints->value.variant.type = &UA_TYPES[UA_TYPES_UINT16];
    UA_Server_addNode(server, (UA_Node*)maxBrowseContinuationPoints,
        UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERCAPABILITIES),
        UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY));
    ADDREFERENCE(UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERCAPABILITIES_MAXBROWSECONTINUATIONPOINTS),
        UA_NODEID_NUMERIC(0, UA_NS0ID_HASTYPEDEFINITION), UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_PROPERTYTYPE));

    UA_ObjectNode *serverdiagnostics = UA_ObjectNode_new();
    copyNames((UA_Node*)serverdiagnostics, "ServerDiagnostics");
    serverdiagnostics->nodeId.identifier.numeric = UA_NS0ID_SERVER_SERVERDIAGNOSTICS;
    UA_Server_addNode(server, (UA_Node*)serverdiagnostics,
            UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_SERVER),
            UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT));
    ADDREFERENCE(UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERDIAGNOSTICS), UA_NODEID_NUMERIC(0, UA_NS0ID_HASTYPEDEFINITION),
        UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_SERVERDIAGNOSTICSTYPE));

    UA_VariableNode *enabledFlag = UA_VariableNode_new();
     copyNames((UA_Node*)enabledFlag, "EnabledFlag");
     enabledFlag->nodeId.identifier.numeric = UA_NS0ID_SERVER_SERVERDIAGNOSTICS_ENABLEDFLAG;
     enabledFlag->value.variant.data = UA_Boolean_new(); //itialized as false
     enabledFlag->value.variant.type = &UA_TYPES[UA_TYPES_BOOLEAN];
     enabledFlag->valueRank = 1;
     enabledFlag->minimumSamplingInterval = 1.0;
     enabledFlag->historizing = UA_FALSE;
     UA_Server_addNode(server, (UA_Node*)enabledFlag,
             UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERDIAGNOSTICS),
             UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY));
     ADDREFERENCE(UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERDIAGNOSTICS_ENABLEDFLAG), UA_NODEID_NUMERIC(0, UA_NS0ID_HASTYPEDEFINITION),
             UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_PROPERTYTYPE));

     UA_VariableNode *serverstatus = UA_VariableNode_new();
      copyNames((UA_Node*)serverstatus, "ServerStatus");
      serverstatus->nodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS);
      serverstatus->valueSource = UA_VALUESOURCE_DATASOURCE;
      serverstatus->value.dataSource = (UA_DataSource) {.handle = server, .read = readStatus,
          .release = releaseStatus, .write = UA_NULL};
      UA_Server_addNode(server, (UA_Node*)serverstatus, UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_SERVER),
              UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT));
      ADDREFERENCE(UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS), UA_NODEID_NUMERIC(0, UA_NS0ID_HASTYPEDEFINITION),
          UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_SERVERSTATUSTYPE));

      UA_VariableNode *starttime = UA_VariableNode_new();
      copyNames((UA_Node*)starttime, "StartTime");
      starttime->nodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_STARTTIME);
      starttime->value.variant.storageType = UA_VARIANT_DATA_NODELETE;
      starttime->value.variant.data = &server->startTime;
      starttime->value.variant.type = &UA_TYPES[UA_TYPES_DATETIME];
      UA_Server_addNode(server, (UA_Node*)starttime, UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS),
              UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT));
      ADDREFERENCE(UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_STARTTIME), UA_NODEID_NUMERIC(0, UA_NS0ID_HASTYPEDEFINITION),
          UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE));

      UA_VariableNode *currenttime = UA_VariableNode_new();
      copyNames((UA_Node*)currenttime, "CurrentTime");
      currenttime->nodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_CURRENTTIME);
      currenttime->valueSource = UA_VALUESOURCE_DATASOURCE;
      currenttime->value.dataSource = (UA_DataSource) {.handle = NULL, .read = readCurrentTime,
          .release = releaseCurrentTime, .write = UA_NULL};
      UA_Server_addNode(server, (UA_Node*)currenttime, UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS),
              UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT));
      ADDREFERENCE(UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_CURRENTTIME), UA_NODEID_NUMERIC(0, UA_NS0ID_HASTYPEDEFINITION),
          UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE));

      UA_VariableNode *state = UA_VariableNode_new();
      UA_ServerState *stateEnum = UA_ServerState_new();
      *stateEnum = UA_SERVERSTATE_RUNNING;
      copyNames((UA_Node*)state, "State");
      state->nodeId.identifier.numeric = UA_NS0ID_SERVER_SERVERSTATUS_STATE;
      state->value.variant.type = &UA_TYPES[UA_TYPES_SERVERSTATE];
      state->value.variant.arrayLength = -1;
      state->value.variant.data = stateEnum; // points into the other object.
      UA_Server_addNode(server, (UA_Node*)state, UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS),
              UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT));
      ADDREFERENCE(UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_STATE), UA_NODEID_NUMERIC(0, UA_NS0ID_HASTYPEDEFINITION),
          UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE));

      UA_VariableNode *buildinfo = UA_VariableNode_new();
       copyNames((UA_Node*)buildinfo, "BuildInfo");
       buildinfo->nodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_BUILDINFO);
       buildinfo->value.variant.data = UA_BuildInfo_new();
       buildinfo->value.variant.type = &UA_TYPES[UA_TYPES_BUILDINFO];
       getBulidInfo(server, (UA_BuildInfo*)buildinfo->value.variant.data);
       UA_Server_addNode(server, (UA_Node*)buildinfo, UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS),
               UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT));
       ADDREFERENCE(UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_BUILDINFO), UA_NODEID_NUMERIC(0, UA_NS0ID_HASTYPEDEFINITION),
           UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_BUILDINFOTYPE));

       UA_VariableNode *producturi = UA_VariableNode_new();
       copyNames((UA_Node*)producturi, "ProductUri");
       producturi->nodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_BUILDINFO_PRODUCTURI);
       producturi->value.variant.data = UA_String_new();
       *((UA_String*)producturi->value.variant.data) = UA_STRING_ALLOC(PRODUCT_URI);
       producturi->value.variant.type = &UA_TYPES[UA_TYPES_STRING];
       UA_Server_addNode(server, (UA_Node*)producturi, UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_BUILDINFO),
               UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT));
       ADDREFERENCE(UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_BUILDINFO_PRODUCTURI), UA_NODEID_NUMERIC(0, UA_NS0ID_HASTYPEDEFINITION),
           UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE));

       UA_VariableNode *manufacturername = UA_VariableNode_new();
       copyNames((UA_Node*)manufacturername, "ManufacturererName");
       manufacturername->nodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_BUILDINFO_MANUFACTURERNAME);
       manufacturername->value.variant.data = UA_String_new();
       *((UA_String*)manufacturername->value.variant.data) = UA_STRING_ALLOC(MANUFACTURER_NAME);
       manufacturername->value.variant.type = &UA_TYPES[UA_TYPES_STRING];
       UA_Server_addNode(server, (UA_Node*)manufacturername, UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_BUILDINFO),
               UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT));
       ADDREFERENCE(UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_BUILDINFO_MANUFACTURERNAME), UA_NODEID_NUMERIC(0, UA_NS0ID_HASTYPEDEFINITION),
           UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE));

       UA_VariableNode *productname = UA_VariableNode_new();
       copyNames((UA_Node*)productname, "ProductName");
       productname->nodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_BUILDINFO_PRODUCTNAME);
       productname->value.variant.data = UA_String_new();
       *((UA_String*)productname->value.variant.data) = UA_STRING_ALLOC(PRODUCT_NAME);
       productname->value.variant.type = &UA_TYPES[UA_TYPES_STRING];
       UA_Server_addNode(server, (UA_Node*)productname, UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_BUILDINFO),
               UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT));
       ADDREFERENCE(UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_BUILDINFO_PRODUCTNAME), UA_NODEID_NUMERIC(0, UA_NS0ID_HASTYPEDEFINITION),
           UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE));

       UA_VariableNode *softwareversion = UA_VariableNode_new();
       copyNames((UA_Node*)softwareversion, "SoftwareVersion");
       softwareversion->nodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_BUILDINFO_SOFTWAREVERSION);
       softwareversion->value.variant.data = UA_String_new();
       *((UA_String*)softwareversion->value.variant.data) = UA_STRING_ALLOC(SOFTWARE_VERSION);
       softwareversion->value.variant.type = &UA_TYPES[UA_TYPES_STRING];
       UA_Server_addNode(server, (UA_Node*)softwareversion, UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_BUILDINFO),
               UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT));
       ADDREFERENCE(UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_BUILDINFO_SOFTWAREVERSION), UA_NODEID_NUMERIC(0, UA_NS0ID_HASTYPEDEFINITION),
           UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE));

       UA_VariableNode *buildnumber = UA_VariableNode_new();
       copyNames((UA_Node*)buildnumber, "BuildNumber");
       buildnumber->nodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_BUILDINFO_BUILDNUMBER);
       buildnumber->value.variant.data = UA_String_new();
       *((UA_String*)buildnumber->value.variant.data) = UA_STRING_ALLOC(BUILD_NUMBER);
       buildnumber->value.variant.type = &UA_TYPES[UA_TYPES_STRING];
       UA_Server_addNode(server, (UA_Node*)buildnumber, UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_BUILDINFO),
               UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT));
       ADDREFERENCE(UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_BUILDINFO_BUILDNUMBER), UA_NODEID_NUMERIC(0, UA_NS0ID_HASTYPEDEFINITION),
           UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE));

       UA_VariableNode *builddate = UA_VariableNode_new();
       copyNames((UA_Node*)builddate, "BuildDate");
       builddate->nodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_BUILDINFO_BUILDDATE);
       builddate->value.variant.storageType = UA_VARIANT_DATA_NODELETE;
       builddate->value.variant.data = &server->buildDate;
       builddate->value.variant.type = &UA_TYPES[UA_TYPES_DATETIME];
       UA_Server_addNode(server, (UA_Node*)builddate, UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_BUILDINFO),
               UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT));
       ADDREFERENCE(UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_BUILDINFO_BUILDNUMBER), UA_NODEID_NUMERIC(0, UA_NS0ID_HASTYPEDEFINITION),
           UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE));

       UA_VariableNode *secondstillshutdown = UA_VariableNode_new();
       copyNames((UA_Node*)secondstillshutdown, "SecondsTillShutdown");
       secondstillshutdown->nodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_SECONDSTILLSHUTDOWN);
       secondstillshutdown->value.variant.data = UA_UInt32_new();
       secondstillshutdown->value.variant.type = &UA_TYPES[UA_TYPES_UINT32];
       UA_Server_addNode(server, (UA_Node*)secondstillshutdown, UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS),
               UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT));
       ADDREFERENCE(UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_SECONDSTILLSHUTDOWN), UA_NODEID_NUMERIC(0, UA_NS0ID_HASTYPEDEFINITION),
           UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE));

       UA_VariableNode *shutdownreason = UA_VariableNode_new();
       copyNames((UA_Node*)shutdownreason, "ShutdownReason");
       shutdownreason->nodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_SHUTDOWNREASON);
       shutdownreason->value.variant.data = UA_LocalizedText_new();
       shutdownreason->value.variant.type = &UA_TYPES[UA_TYPES_LOCALIZEDTEXT];
       UA_Server_addNode(server, (UA_Node*)shutdownreason, UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS),
               UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT));
       ADDREFERENCE(UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_SHUTDOWNREASON), UA_NODEID_NUMERIC(0, UA_NS0ID_HASTYPEDEFINITION),
           UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE));

   return server;
}
/*********************************** amalgamated original file "C:/AcpltDevelopmentKit/acplt/dev/open62541WrapperProject/open62541/src/server/ua_server_addressspace.c" ***********************************/

UA_StatusCode
UA_Server_addVariableNode(UA_Server *server, UA_Variant *value, const UA_QualifiedName browseName, 
                          UA_NodeId nodeId, const UA_NodeId parentNodeId, const UA_NodeId referenceTypeId)
{
    UA_VariableNode *node = UA_VariableNode_new();
    node->value.variant = *value; // copy content
    UA_NodeId_copy(&nodeId, &node->nodeId);
    UA_QualifiedName_copy(&browseName, &node->browseName);
    UA_String_copy(&browseName.name, &node->displayName.text);
    UA_ExpandedNodeId parentId; // we need an expandednodeid
    UA_ExpandedNodeId_init(&parentId);
    UA_NodeId_copy(&parentNodeId, &parentId.nodeId);
    UA_AddNodesResult res =
        UA_Server_addNodeWithSession(server, &adminSession, (UA_Node*)node, parentId, referenceTypeId);
    ADDREFERENCE(res.addedNodeId, UA_NODEID_NUMERIC(0, UA_NS0ID_HASTYPEDEFINITION),
                 UA_EXPANDEDNODEID_NUMERIC(0, value->type->typeId.identifier.numeric));
    if(res.statusCode != UA_STATUSCODE_GOOD) {
        UA_Variant_init(&node->value.variant);
        UA_VariableNode_delete(node);
    } else
        UA_free(value);
    UA_AddNodesResult_deleteMembers(&res);
    return res.statusCode;
}

UA_StatusCode
UA_Server_addObjectNode(UA_Server *server, const UA_QualifiedName browseName,
                          UA_NodeId nodeId, const UA_NodeId parentNodeId, const UA_NodeId referenceTypeId, const UA_NodeId typeDefinition)
{
    UA_ObjectNode *node = UA_ObjectNode_new();
    UA_NodeId_copy(&nodeId, &node->nodeId);
    UA_QualifiedName_copy(&browseName, &node->browseName);
    UA_String_copy(&browseName.name, &node->displayName.text);
    UA_ExpandedNodeId parentId; // we need an expandednodeid
    UA_ExpandedNodeId_init(&parentId);
    UA_NodeId_copy(&parentNodeId, &parentId.nodeId);
    UA_AddNodesResult res =
        UA_Server_addNodeWithSession(server, &adminSession, (UA_Node*)node, parentId, referenceTypeId);
    if(res.statusCode != UA_STATUSCODE_GOOD) {
        UA_ObjectNode_delete(node);
    }
    UA_AddNodesResult_deleteMembers(&res);

    if(!(UA_NodeId_isNull(&typeDefinition))){
        UA_ExpandedNodeId typeDefid; // we need an expandednodeid
        UA_ExpandedNodeId_init(&typeDefid);
        UA_NodeId_copy(&typeDefinition, &typeDefid.nodeId);
        ADDREFERENCE(res.addedNodeId, UA_NODEID_NUMERIC(0, UA_NS0ID_HASTYPEDEFINITION), typeDefid);
    }
    return res.statusCode;
}

UA_StatusCode
UA_Server_addDataSourceVariableNode(UA_Server *server, UA_DataSource dataSource,
                                    const UA_QualifiedName browseName, UA_NodeId nodeId,
                                    const UA_NodeId parentNodeId, const UA_NodeId referenceTypeId)
{
    UA_VariableNode *node = UA_VariableNode_new();
    node->valueSource = UA_VALUESOURCE_DATASOURCE;
    node->value.dataSource = dataSource;
    UA_NodeId_copy(&nodeId, &node->nodeId);
    UA_QualifiedName_copy(&browseName, &node->browseName);
    UA_String_copy(&browseName.name, &node->displayName.text);
    UA_ExpandedNodeId parentId; // dummy exapndednodeid
    UA_ExpandedNodeId_init(&parentId);
    UA_NodeId_copy(&parentNodeId, &parentId.nodeId);
    UA_AddNodesResult res =
        UA_Server_addNodeWithSession(server, &adminSession, (UA_Node*)node, parentId, referenceTypeId);
    ADDREFERENCE(res.addedNodeId, UA_NODEID_NUMERIC(0, UA_NS0ID_HASTYPEDEFINITION),
                 UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE));
    if(res.statusCode != UA_STATUSCODE_GOOD)
        UA_VariableNode_delete(node);
    UA_AddNodesResult_deleteMembers(&res);
    return res.statusCode;
}

/* Adds a one-way reference to the local nodestore */
static UA_StatusCode
addOneWayReferenceWithSession(UA_Server *server, UA_Session *session, const UA_AddReferencesItem *item) {
    const UA_Node *node = UA_NodeStore_get(server->nodestore, &item->sourceNodeId);
    if(!node)
        return UA_STATUSCODE_BADINTERNALERROR;
	UA_StatusCode retval = UA_STATUSCODE_GOOD;
#ifndef UA_MULTITHREADING
	size_t i = node->referencesSize;
	if(node->referencesSize < 0)
		i = 0;
    size_t refssize = (i+1) | 3; // so the realloc is not necessary every time
	UA_ReferenceNode *new_refs = UA_realloc(node->references, sizeof(UA_ReferenceNode) * refssize);
	if(!new_refs)
		retval = UA_STATUSCODE_BADOUTOFMEMORY;
	else {
		UA_ReferenceNode_init(&new_refs[i]);
		retval = UA_NodeId_copy(&item->referenceTypeId, &new_refs[i].referenceTypeId);
		new_refs[i].isInverse = !item->isForward;
		retval |= UA_ExpandedNodeId_copy(&item->targetNodeId, &new_refs[i].targetId);
		/* hack. be careful! possible only in the single-threaded case. */
		UA_Node *mutable_node = (UA_Node*)(uintptr_t)node;
		mutable_node->references = new_refs;
		if(retval != UA_STATUSCODE_GOOD) {
			UA_NodeId_deleteMembers(&new_refs[node->referencesSize].referenceTypeId);
			UA_ExpandedNodeId_deleteMembers(&new_refs[node->referencesSize].targetId);
		} else
			mutable_node->referencesSize = i+1;
	}
	UA_NodeStore_release(node);
	return retval;
#else
    UA_Node *newNode = UA_NULL;
    void (*deleteNode)(UA_Node*) = UA_NULL;
    switch(node->nodeClass) {
    case UA_NODECLASS_OBJECT:
        newNode = (UA_Node*)UA_ObjectNode_new();
        UA_ObjectNode_copy((const UA_ObjectNode*)node, (UA_ObjectNode*)newNode);
        deleteNode = (void (*)(UA_Node*))UA_ObjectNode_delete;
        break;
    case UA_NODECLASS_VARIABLE:
        newNode = (UA_Node*)UA_VariableNode_new();
        UA_VariableNode_copy((const UA_VariableNode*)node, (UA_VariableNode*)newNode);
        deleteNode = (void (*)(UA_Node*))UA_VariableNode_delete;
        break;
    case UA_NODECLASS_METHOD:
        newNode = (UA_Node*)UA_MethodNode_new();
        UA_MethodNode_copy((const UA_MethodNode*)node, (UA_MethodNode*)newNode);
        deleteNode = (void (*)(UA_Node*))UA_MethodNode_delete;
        break;
    case UA_NODECLASS_OBJECTTYPE:
        newNode = (UA_Node*)UA_ObjectTypeNode_new();
        UA_ObjectTypeNode_copy((const UA_ObjectTypeNode*)node, (UA_ObjectTypeNode*)newNode);
        deleteNode = (void (*)(UA_Node*))UA_ObjectTypeNode_delete;
        break;
    case UA_NODECLASS_VARIABLETYPE:
        newNode = (UA_Node*)UA_VariableTypeNode_new();
        UA_VariableTypeNode_copy((const UA_VariableTypeNode*)node, (UA_VariableTypeNode*)newNode);
        deleteNode = (void (*)(UA_Node*))UA_VariableTypeNode_delete;
        break;
    case UA_NODECLASS_REFERENCETYPE:
        newNode = (UA_Node*)UA_ReferenceTypeNode_new();
        UA_ReferenceTypeNode_copy((const UA_ReferenceTypeNode*)node, (UA_ReferenceTypeNode*)newNode);
        deleteNode = (void (*)(UA_Node*))UA_ReferenceTypeNode_delete;
        break;
    case UA_NODECLASS_DATATYPE:
        newNode = (UA_Node*)UA_DataTypeNode_new();
        UA_DataTypeNode_copy((const UA_DataTypeNode*)node, (UA_DataTypeNode*)newNode);
        deleteNode = (void (*)(UA_Node*))UA_DataTypeNode_delete;
        break;
    case UA_NODECLASS_VIEW:
        newNode = (UA_Node*)UA_ViewNode_new();
        UA_ViewNode_copy((const UA_ViewNode*)node, (UA_ViewNode*)newNode);
        deleteNode = (void (*)(UA_Node*))UA_ViewNode_delete;
        break;
    default:
        return UA_STATUSCODE_BADINTERNALERROR;
    }

    UA_Int32 count = node->referencesSize;
    if(count < 0)
        count = 0;
    UA_ReferenceNode *old_refs = newNode->references;
    UA_ReferenceNode *new_refs = UA_malloc(sizeof(UA_ReferenceNode)*(count+1));
    if(!new_refs) {
        deleteNode(newNode);
        UA_NodeStore_release(node);
        return UA_STATUSCODE_BADOUTOFMEMORY;
    }

    // insert the new reference
    UA_memcpy(new_refs, old_refs, sizeof(UA_ReferenceNode)*count);
    UA_ReferenceNode_init(&new_refs[count]);
    retval = UA_NodeId_copy(&item->referenceTypeId, &new_refs[count].referenceTypeId);
    new_refs[count].isInverse = !item->isForward;
    retval |= UA_ExpandedNodeId_copy(&item->targetNodeId, &new_refs[count].targetId);
    if(retval != UA_STATUSCODE_GOOD) {
        UA_Array_delete(new_refs, &UA_TYPES[UA_TYPES_REFERENCENODE], ++count);
        newNode->references = UA_NULL;
        newNode->referencesSize = 0;
        deleteNode(newNode);
        UA_NodeStore_release(node);
        return UA_STATUSCODE_BADOUTOFMEMORY;
    }

    UA_free(old_refs);
    newNode->references = new_refs;
    newNode->referencesSize = ++count;
    retval = UA_NodeStore_replace(server->nodestore, node, newNode, UA_NULL);
	UA_NodeStore_release(node);
	if (retval == UA_STATUSCODE_BADINTERNALERROR) {
		/* presumably because the node was replaced and an old version was updated at the same time. just try again */
		deleteNode(newNode);
		return addOneWayReferenceWithSession(server, session, item);
	}
	return retval;
#endif
}

/* userland version of addReferenceWithSession */
UA_StatusCode
UA_Server_addReference(UA_Server *server, const UA_AddReferencesItem *item)
{
    return UA_Server_addReferenceWithSession(server, &adminSession, item);
}

UA_StatusCode
UA_Server_addReferenceWithSession(UA_Server *server, UA_Session *session, const UA_AddReferencesItem *item)
{
    if(item->targetServerUri.length > 0)
        return UA_STATUSCODE_BADNOTIMPLEMENTED; // currently no expandednodeids are allowed
    
    UA_StatusCode retval = UA_STATUSCODE_GOOD;

#ifdef UA_EXTERNAL_NAMESPACES
    UA_ExternalNodeStore *ensFirst = UA_NULL;
    UA_ExternalNodeStore *ensSecond = UA_NULL;
    for(size_t j = 0;j<server->externalNamespacesSize && (!ensFirst || !ensSecond);j++) {
        if(item->sourceNodeId.namespaceIndex == server->externalNamespaces[j].index)
            ensFirst = &server->externalNamespaces[j].externalNodeStore;
        if(item->targetNodeId.nodeId.namespaceIndex == server->externalNamespaces[j].index)
            ensSecond = &server->externalNamespaces[j].externalNodeStore;
    }

    if(ensFirst) {
        // todo: use external nodestore
    } else
#endif
        retval = addOneWayReferenceWithSession(server, session, item);

    if(retval) return retval;

    UA_AddReferencesItem secondItem;
    secondItem = *item;
    secondItem.targetNodeId.nodeId = item->sourceNodeId;
    secondItem.sourceNodeId = item->targetNodeId.nodeId;
    secondItem.isForward = !item->isForward;
#ifdef UA_EXTERNAL_NAMESPACES
    if(ensSecond) {
        // todo: use external nodestore
    } else
#endif
        retval = addOneWayReferenceWithSession (server, session, &secondItem);

    // todo: remove reference if the second direction failed
    return retval;
} 

/* userland version of addNodeWithSession */
UA_AddNodesResult
UA_Server_addNode(UA_Server *server, UA_Node *node, const UA_ExpandedNodeId parentNodeId,
                  const UA_NodeId referenceTypeId)
{
    return UA_Server_addNodeWithSession(server, &adminSession, node, parentNodeId, referenceTypeId);
}

UA_AddNodesResult
UA_Server_addNodeWithSession(UA_Server *server, UA_Session *session, UA_Node *node,
                             const UA_ExpandedNodeId parentNodeId, const UA_NodeId referenceTypeId)
{
    UA_AddNodesResult result;
    UA_AddNodesResult_init(&result);

    if(node->nodeId.namespaceIndex >= server->namespacesSize) {
        result.statusCode = UA_STATUSCODE_BADNODEIDINVALID;
        return result;
    }

    const UA_Node *parent = UA_NodeStore_get(server->nodestore, &parentNodeId.nodeId);
    if(!parent) {
        result.statusCode = UA_STATUSCODE_BADPARENTNODEIDINVALID;
        return result;
    }

    const UA_ReferenceTypeNode *referenceType =
        (const UA_ReferenceTypeNode *)UA_NodeStore_get(server->nodestore, &referenceTypeId);
    if(!referenceType) {
        result.statusCode = UA_STATUSCODE_BADREFERENCETYPEIDINVALID;
        goto ret;
    }

    if(referenceType->nodeClass != UA_NODECLASS_REFERENCETYPE) {
        result.statusCode = UA_STATUSCODE_BADREFERENCETYPEIDINVALID;
        goto ret2;
    }

    if(referenceType->isAbstract == UA_TRUE) {
        result.statusCode = UA_STATUSCODE_BADREFERENCENOTALLOWED;
        goto ret2;
    }

    // todo: test if the referencetype is hierarchical
    const UA_Node *managed = UA_NULL;
    if(UA_NodeId_isNull(&node->nodeId)) {
        if(UA_NodeStore_insert(server->nodestore, node, &managed) != UA_STATUSCODE_GOOD) {
            result.statusCode = UA_STATUSCODE_BADOUTOFMEMORY;
            goto ret2;
        }
        result.addedNodeId = managed->nodeId; // cannot fail as unique nodeids are numeric
    } else {
        if(UA_NodeId_copy(&node->nodeId, &result.addedNodeId) != UA_STATUSCODE_GOOD) {
            result.statusCode = UA_STATUSCODE_BADOUTOFMEMORY;
            goto ret2;
        }

        if(UA_NodeStore_insert(server->nodestore, node, &managed) != UA_STATUSCODE_GOOD) {
            result.statusCode = UA_STATUSCODE_BADNODEIDEXISTS;  // todo: differentiate out of memory
            UA_NodeId_deleteMembers(&result.addedNodeId);
            goto ret2;
        }
    }
    
    // reference back to the parent
    UA_AddReferencesItem item;
    UA_AddReferencesItem_init(&item);
    item.sourceNodeId = managed->nodeId;
    item.referenceTypeId = referenceType->nodeId;
    item.isForward = UA_FALSE;
    item.targetNodeId.nodeId = parent->nodeId;
    UA_Server_addReference(server, &item);

    // todo: error handling. remove new node from nodestore

    UA_NodeStore_release(managed);
    
 ret2:
    UA_NodeStore_release((const UA_Node*)referenceType);
 ret:
    UA_NodeStore_release(parent);

    return result;
}
/*********************************** amalgamated original file "C:/AcpltDevelopmentKit/acplt/dev/open62541WrapperProject/open62541/src/server/ua_server_binary.c" ***********************************/

/** Max size of messages that are allocated on the stack */
#define MAX_STACK_MESSAGE 65536

static void processHEL(UA_Connection *connection, const UA_ByteString *msg, size_t *pos) {
    UA_TcpHelloMessage helloMessage;
    if(UA_TcpHelloMessage_decodeBinary(msg, pos, &helloMessage) != UA_STATUSCODE_GOOD) {
        connection->close(connection);
        return;
    }

    connection->remoteConf.maxChunkCount = helloMessage.maxChunkCount;
    connection->remoteConf.maxMessageSize = helloMessage.maxMessageSize;
    connection->remoteConf.protocolVersion = helloMessage.protocolVersion;
    connection->remoteConf.recvBufferSize = helloMessage.receiveBufferSize;
    if(connection->localConf.sendBufferSize > helloMessage.receiveBufferSize)
        connection->localConf.sendBufferSize = helloMessage.receiveBufferSize;
    if(connection->localConf.recvBufferSize > helloMessage.sendBufferSize)
        connection->localConf.recvBufferSize = helloMessage.sendBufferSize;
    connection->remoteConf.sendBufferSize = helloMessage.sendBufferSize;
    connection->state = UA_CONNECTION_ESTABLISHED;
    UA_TcpHelloMessage_deleteMembers(&helloMessage);

    // build acknowledge response
    UA_TcpAcknowledgeMessage ackMessage;
    ackMessage.protocolVersion = connection->localConf.protocolVersion;
    ackMessage.receiveBufferSize = connection->localConf.recvBufferSize;
    ackMessage.sendBufferSize = connection->localConf.sendBufferSize;
    ackMessage.maxMessageSize = connection->localConf.maxMessageSize;
    ackMessage.maxChunkCount = connection->localConf.maxChunkCount;

    UA_TcpMessageHeader ackHeader;
    ackHeader.messageTypeAndFinal = UA_MESSAGETYPEANDFINAL_ACKF;
    ackHeader.messageSize = UA_TcpMessageHeader_calcSizeBinary(&ackHeader) 
        + UA_TcpAcknowledgeMessage_calcSizeBinary(&ackMessage);

    UA_ByteString ack_msg;
    if(connection->getBuffer(connection, &ack_msg, ackHeader.messageSize) != UA_STATUSCODE_GOOD)
        return;

    size_t tmpPos = 0;
    UA_TcpMessageHeader_encodeBinary(&ackHeader, &ack_msg, &tmpPos);
    UA_TcpAcknowledgeMessage_encodeBinary(&ackMessage, &ack_msg, &tmpPos);
    connection->write(connection, &ack_msg);
    connection->releaseBuffer(connection, &ack_msg);
}

static void processOPN(UA_Connection *connection, UA_Server *server, const UA_ByteString *msg,
                       size_t *pos) {
    if(connection->state != UA_CONNECTION_ESTABLISHED) {
        connection->close(connection);
        return;
    }

    UA_UInt32 secureChannelId;
    UA_StatusCode retval = UA_UInt32_decodeBinary(msg, pos, &secureChannelId);

    UA_AsymmetricAlgorithmSecurityHeader asymHeader;
    retval |= UA_AsymmetricAlgorithmSecurityHeader_decodeBinary(msg, pos, &asymHeader);

    UA_SequenceHeader seqHeader;
    retval |= UA_SequenceHeader_decodeBinary(msg, pos, &seqHeader);

    UA_NodeId requestType;
    retval |= UA_NodeId_decodeBinary(msg, pos, &requestType);

    UA_OpenSecureChannelRequest r;
    retval |= UA_OpenSecureChannelRequest_decodeBinary(msg, pos, &r);

    if(retval != UA_STATUSCODE_GOOD || requestType.identifier.numeric != 446) {
        UA_AsymmetricAlgorithmSecurityHeader_deleteMembers(&asymHeader);
        UA_SequenceHeader_deleteMembers(&seqHeader);
        UA_NodeId_deleteMembers(&requestType);
        UA_OpenSecureChannelRequest_deleteMembers(&r);
        connection->close(connection);
        return;
    }

    UA_OpenSecureChannelResponse p;
    UA_OpenSecureChannelResponse_init(&p);
    Service_OpenSecureChannel(server, connection, &r, &p);
    UA_OpenSecureChannelRequest_deleteMembers(&r);

    UA_SecureConversationMessageHeader respHeader;
    respHeader.messageHeader.messageTypeAndFinal = UA_MESSAGETYPEANDFINAL_OPNF;
    respHeader.messageHeader.messageSize = 0;
    respHeader.secureChannelId = p.securityToken.channelId;

    UA_NodeId responseType = UA_NODEID_NUMERIC(0, UA_NS0ID_OPENSECURECHANNELRESPONSE +
                                               UA_ENCODINGOFFSET_BINARY);

    respHeader.messageHeader.messageSize =
        UA_SecureConversationMessageHeader_calcSizeBinary(&respHeader)
        + UA_AsymmetricAlgorithmSecurityHeader_calcSizeBinary(&asymHeader)
        + UA_SequenceHeader_calcSizeBinary(&seqHeader)
        + UA_NodeId_calcSizeBinary(&responseType)
        + UA_OpenSecureChannelResponse_calcSizeBinary(&p);

    UA_ByteString resp_msg;
    retval = connection->getBuffer(connection, &resp_msg, respHeader.messageHeader.messageSize);
    if(retval != UA_STATUSCODE_GOOD) {
        UA_OpenSecureChannelResponse_deleteMembers(&p);
        UA_AsymmetricAlgorithmSecurityHeader_deleteMembers(&asymHeader);
        return;
    }
        
    size_t tmpPos = 0;
    UA_SecureConversationMessageHeader_encodeBinary(&respHeader, &resp_msg, &tmpPos);
    UA_AsymmetricAlgorithmSecurityHeader_encodeBinary(&asymHeader, &resp_msg, &tmpPos); // just mirror back
    UA_SequenceHeader_encodeBinary(&seqHeader, &resp_msg, &tmpPos); // just mirror back
    UA_NodeId_encodeBinary(&responseType, &resp_msg, &tmpPos);
    UA_OpenSecureChannelResponse_encodeBinary(&p, &resp_msg, &tmpPos);
    UA_OpenSecureChannelResponse_deleteMembers(&p);
    UA_AsymmetricAlgorithmSecurityHeader_deleteMembers(&asymHeader);

    connection->write(connection, &resp_msg);
    connection->releaseBuffer(connection, &resp_msg);
}

static void init_response_header(const UA_RequestHeader *p, UA_ResponseHeader *r) {
    r->requestHandle = p->requestHandle;
    r->stringTableSize = 0;
    r->timestamp = UA_DateTime_now();
}

/* The request/response are casted to the header (first element of their struct) */
static void invoke_service(UA_Server *server, UA_SecureChannel *channel,
                           UA_RequestHeader *request, UA_ResponseHeader *response,
                           void (*service)(UA_Server*, UA_Session*, void*, void*)) {
    init_response_header(request, response);
    /* try to get the session from the securechannel first */
    UA_Session *session = UA_SecureChannel_getSession(channel, &request->authenticationToken);
    if(!session)
        session = UA_SessionManager_getSession(&server->sessionManager, &request->authenticationToken);
    if(!session)
        response->serviceResult = UA_STATUSCODE_BADSESSIONIDINVALID;
    else if(session->activated == UA_FALSE) {
        response->serviceResult = UA_STATUSCODE_BADSESSIONNOTACTIVATED;
        /* the session is invalidated */
        UA_SessionManager_removeSession(&server->sessionManager, &request->authenticationToken);
    }
    else if(session->channel != channel)
        response->serviceResult = UA_STATUSCODE_BADSESSIONIDINVALID;
    else {
            UA_Session_updateLifetime(session);
            service(server, session, request, response);
    }
}

#define INVOKE_SERVICE(TYPE) do {                                       \
        UA_##TYPE##Request p;                                           \
        UA_##TYPE##Response r;                                          \
        if(UA_##TYPE##Request_decodeBinary(msg, pos, &p))               \
            return;                                                     \
        UA_##TYPE##Response_init(&r);                                   \
        invoke_service(server, clientChannel, &p.requestHeader,         \
                       &r.responseHeader,                               \
                       (void (*)(UA_Server*, UA_Session*, void*,void*))Service_##TYPE); \
        UA_##TYPE##Request_deleteMembers(&p);                           \
        retval = connection->getBuffer(connection, &message,            \
                     headerSize + UA_##TYPE##Response_calcSizeBinary(&r)); \
        if(retval != UA_STATUSCODE_GOOD) {                              \
            UA_##TYPE##Response_deleteMembers(&r);                      \
            return;                                                     \
        }                                                               \
        UA_##TYPE##Response_encodeBinary(&r, &message, &messagePos);    \
        UA_##TYPE##Response_deleteMembers(&r);                          \
} while(0)

static void processMSG(UA_Connection *connection, UA_Server *server, const UA_ByteString *msg, size_t *pos) {
    // 1) Read in the securechannel
    UA_UInt32 secureChannelId;
    UA_StatusCode retval = UA_UInt32_decodeBinary(msg, pos, &secureChannelId);
    if(retval != UA_STATUSCODE_GOOD)
        return;

    UA_SecureChannel *clientChannel = connection->channel;
#ifdef EXTENSION_STATELESS
    UA_SecureChannel anonymousChannel;
    if(!clientChannel) {
        UA_SecureChannel_init(&anonymousChannel);
        anonymousChannel.session = &anonymousSession;
        clientChannel = &anonymousChannel;
    }
#endif

    // 2) Read the security header
    UA_UInt32 tokenId;
    UA_SequenceHeader sequenceHeader;
    retval = UA_UInt32_decodeBinary(msg, pos, &tokenId);
    retval |= UA_SequenceHeader_decodeBinary(msg, pos, &sequenceHeader);
    if(retval != UA_STATUSCODE_GOOD)
        return;

    //UA_SecureChannel_checkSequenceNumber(channel,sequenceHeader.sequenceNumber);
    //UA_SecureChannel_checkRequestId(channel,sequenceHeader.requestId);
    clientChannel->sequenceNumber = sequenceHeader.sequenceNumber;
    clientChannel->requestId = sequenceHeader.requestId;

    // 3) Build the header and compute the header size
    UA_SecureConversationMessageHeader respHeader;
    respHeader.messageHeader.messageTypeAndFinal = UA_MESSAGETYPEANDFINAL_MSGF;
    respHeader.messageHeader.messageSize = 0;
    respHeader.secureChannelId = clientChannel->securityToken.channelId;

    UA_SymmetricAlgorithmSecurityHeader symSecHeader;
    symSecHeader.tokenId = clientChannel->securityToken.tokenId;

    UA_SequenceHeader seqHeader;
    seqHeader.sequenceNumber = clientChannel->sequenceNumber;
    seqHeader.requestId = clientChannel->requestId;

    // 4) process the request
    UA_ByteString message;
    UA_NodeId requestType;
    if(UA_NodeId_decodeBinary(msg, pos, &requestType))
        return;
    if(requestType.identifierType != UA_NODEIDTYPE_NUMERIC) {
        UA_NodeId_deleteMembers(&requestType);
        return;
    }

    UA_NodeId response_nodeid = UA_NODEID_NUMERIC(0, requestType.identifier.numeric + 3);
    UA_UInt32 headerSize = UA_SecureConversationMessageHeader_calcSizeBinary(&respHeader)
        + UA_SymmetricAlgorithmSecurityHeader_calcSizeBinary(&symSecHeader)
        + UA_SequenceHeader_calcSizeBinary(&seqHeader)
        + UA_NodeId_calcSizeBinary(&response_nodeid);
    size_t messagePos = headerSize;

    //subtract UA_ENCODINGOFFSET_BINARY for binary encoding
    switch(requestType.identifier.numeric - UA_ENCODINGOFFSET_BINARY) {
    case UA_NS0ID_GETENDPOINTSREQUEST: {
        UA_GetEndpointsRequest  p;
        UA_GetEndpointsResponse r;
        if(UA_GetEndpointsRequest_decodeBinary(msg, pos, &p))
            return;
        UA_GetEndpointsResponse_init(&r);
        init_response_header(&p.requestHeader, &r.responseHeader);
        Service_GetEndpoints(server, &p, &r);
        UA_GetEndpointsRequest_deleteMembers(&p);
        retval = connection->getBuffer(connection, &message, headerSize + UA_GetEndpointsResponse_calcSizeBinary(&r));
        if(retval != UA_STATUSCODE_GOOD) {
            UA_GetEndpointsResponse_deleteMembers(&r);
            return;
        }
        UA_GetEndpointsResponse_encodeBinary(&r, &message, &messagePos);
        UA_GetEndpointsResponse_deleteMembers(&r);
        break;
    }

    case UA_NS0ID_FINDSERVERSREQUEST: {
        UA_FindServersRequest  p;
        UA_FindServersResponse r;
        if(UA_FindServersRequest_decodeBinary(msg, pos, &p))
            return;
        UA_FindServersResponse_init(&r);
        init_response_header(&p.requestHeader, &r.responseHeader);
        Service_FindServers(server, &p, &r);
        UA_FindServersRequest_deleteMembers(&p);
        retval = connection->getBuffer(connection, &message, headerSize + UA_FindServersResponse_calcSizeBinary(&r));
        if(retval != UA_STATUSCODE_GOOD) {
            UA_FindServersResponse_deleteMembers(&r);
            return;
        }
        UA_FindServersResponse_encodeBinary(&r, &message, &messagePos);
        UA_FindServersResponse_deleteMembers(&r);
        break;
    }

    case UA_NS0ID_CREATESESSIONREQUEST: {
        UA_CreateSessionRequest  p;
        UA_CreateSessionResponse r;
        if(UA_CreateSessionRequest_decodeBinary(msg, pos, &p))
            return;
        UA_CreateSessionResponse_init(&r);
        init_response_header(&p.requestHeader, &r.responseHeader);
        Service_CreateSession(server, clientChannel, &p, &r);
        UA_CreateSessionRequest_deleteMembers(&p);
        retval = connection->getBuffer(connection, &message, headerSize + UA_CreateSessionResponse_calcSizeBinary(&r));
        if(retval != UA_STATUSCODE_GOOD) {
            UA_CreateSessionResponse_deleteMembers(&r);
            return;
        }
        UA_CreateSessionResponse_encodeBinary(&r, &message, &messagePos);
        UA_CreateSessionResponse_deleteMembers(&r);
        break;
    }

    case UA_NS0ID_ACTIVATESESSIONREQUEST: {
        UA_ActivateSessionRequest  p;
        UA_ActivateSessionResponse r;
        if(UA_ActivateSessionRequest_decodeBinary(msg, pos, &p))
            return;
        UA_ActivateSessionResponse_init(&r);
        init_response_header(&p.requestHeader, &r.responseHeader);
        Service_ActivateSession(server, clientChannel, &p, &r);
        UA_ActivateSessionRequest_deleteMembers(&p);
        retval = connection->getBuffer(connection, &message, headerSize + UA_ActivateSessionResponse_calcSizeBinary(&r));
        if(retval != UA_STATUSCODE_GOOD) {
            UA_ActivateSessionResponse_deleteMembers(&r);
            return;
        }
        UA_ActivateSessionResponse_encodeBinary(&r, &message, &messagePos);
        UA_ActivateSessionResponse_deleteMembers(&r);
        break;
    }

    case UA_NS0ID_CLOSESESSIONREQUEST:
        INVOKE_SERVICE(CloseSession);
        break;
    case UA_NS0ID_READREQUEST:
        INVOKE_SERVICE(Read);
        break;
    case UA_NS0ID_WRITEREQUEST:
        INVOKE_SERVICE(Write);
        break;
    case UA_NS0ID_BROWSEREQUEST:
        INVOKE_SERVICE(Browse);
        break;
    case UA_NS0ID_BROWSENEXTREQUEST:
        INVOKE_SERVICE(BrowseNext);
        break;
    case UA_NS0ID_ADDREFERENCESREQUEST:
        INVOKE_SERVICE(AddReferences);
        break;
    case UA_NS0ID_REGISTERNODESREQUEST:
        INVOKE_SERVICE(RegisterNodes);
        break;
    case UA_NS0ID_UNREGISTERNODESREQUEST:
        INVOKE_SERVICE(UnregisterNodes);
        break;
    case UA_NS0ID_TRANSLATEBROWSEPATHSTONODEIDSREQUEST:
        INVOKE_SERVICE(TranslateBrowsePathsToNodeIds);
        break;

    default: {
        if(requestType.namespaceIndex == 0 && requestType.identifier.numeric==787){
            UA_LOG_INFO(server->logger, UA_LOGCATEGORY_COMMUNICATION,
                        "Client requested a subscription that are not supported, the message will be skipped");
        }else{
            UA_LOG_INFO(server->logger, UA_LOGCATEGORY_COMMUNICATION, "Unknown request: NodeId(ns=%d, i=%d)",
                        requestType.namespaceIndex, requestType.identifier.numeric);
        }
        UA_RequestHeader  p;
        UA_ResponseHeader r;
        if(UA_RequestHeader_decodeBinary(msg, pos, &p) != UA_STATUSCODE_GOOD)
            return;
        UA_ResponseHeader_init(&r);
        init_response_header(&p, &r);
        r.serviceResult = UA_STATUSCODE_BADSERVICEUNSUPPORTED;
#ifdef EXTENSION_STATELESS
        if(retval != UA_STATUSCODE_GOOD)
            r.serviceResult = retval;
#endif
        UA_RequestHeader_deleteMembers(&p);
        retval = connection->getBuffer(connection, &message, headerSize + UA_ResponseHeader_calcSizeBinary(&r));
        if(retval != UA_STATUSCODE_GOOD) {
            UA_ResponseHeader_deleteMembers(&r);
            return;
        }
        UA_ResponseHeader_encodeBinary(&r, &message, &messagePos);
        UA_ResponseHeader_deleteMembers(&r);
        response_nodeid = UA_NODEID_NUMERIC(0, UA_NS0ID_RESPONSEHEADER + UA_ENCODINGOFFSET_BINARY);
        break;
    }
    }

    messagePos = 0;
    respHeader.messageHeader.messageSize = message.length;
    UA_SecureConversationMessageHeader_encodeBinary(&respHeader, &message, &messagePos);
    UA_SymmetricAlgorithmSecurityHeader_encodeBinary(&symSecHeader, &message, &messagePos);
    UA_SequenceHeader_encodeBinary(&seqHeader, &message, &messagePos);
    UA_NodeId_encodeBinary(&response_nodeid, &message, &messagePos);

    // todo: sign & encrypt

    // 5) Send it over the wire.
    connection->write(connection, &message);
    connection->releaseBuffer(connection, &message);
}

static void processCLO(UA_Connection *connection, UA_Server *server, const UA_ByteString *msg, size_t *pos) {
    UA_UInt32 secureChannelId;
    UA_StatusCode retval = UA_UInt32_decodeBinary(msg, pos, &secureChannelId);
    if(retval != UA_STATUSCODE_GOOD || !connection->channel ||
       connection->channel->securityToken.channelId != secureChannelId)
        return;
    Service_CloseSecureChannel(server, secureChannelId);
}

void UA_Server_processBinaryMessage(UA_Server *server, UA_Connection *connection, UA_ByteString *msg) {
    if(msg->length <= 0)
        return;
    size_t pos = 0;
    UA_TcpMessageHeader tcpMessageHeader;
    do {
        if(UA_TcpMessageHeader_decodeBinary(msg, &pos, &tcpMessageHeader) != UA_STATUSCODE_GOOD) {
            UA_LOG_INFO(server->logger, UA_LOGCATEGORY_COMMUNICATION, "Decoding of message header failed");
            connection->close(connection);
            break;
        }

        size_t targetpos = pos - 8 + tcpMessageHeader.messageSize;

        switch(tcpMessageHeader.messageTypeAndFinal & 0xffffff) {
        case UA_MESSAGETYPEANDFINAL_HELF & 0xffffff:
            processHEL(connection, msg, &pos);
            break;
        case UA_MESSAGETYPEANDFINAL_OPNF & 0xffffff:
            processOPN(connection, server, msg, &pos);
            break;
        case UA_MESSAGETYPEANDFINAL_MSGF & 0xffffff:
#ifdef EXTENSION_STATELESS
            processMSG(connection, server, msg, &pos);
            break;
#endif
            if(connection->state != UA_CONNECTION_ESTABLISHED) {
                connection->close(connection);
                break;
            }
            processMSG(connection, server, msg, &pos);
            break;
        case UA_MESSAGETYPEANDFINAL_CLOF & 0xffffff:
            processCLO(connection, server, msg, &pos);
            connection->close(connection);
            return;
        }

        UA_TcpMessageHeader_deleteMembers(&tcpMessageHeader);
        if(pos != targetpos) {
            UA_LOG_INFO(server->logger, UA_LOGCATEGORY_COMMUNICATION,
                        "The message was not entirely processed, skipping to the end");
            pos = targetpos;
        }
    } while(msg->length > (UA_Int32)pos);
}
/*********************************** amalgamated original file "C:/AcpltDevelopmentKit/acplt/dev/open62541WrapperProject/open62541/src/server/ua_nodes.c" ***********************************/

/* UA_Node */
static void UA_Node_init(UA_Node *p) {
	UA_NodeId_init(&p->nodeId);
	UA_NodeClass_init(&p->nodeClass);
	UA_QualifiedName_init(&p->browseName);
	UA_LocalizedText_init(&p->displayName);
	UA_LocalizedText_init(&p->description);
	UA_UInt32_init(&p->writeMask);
	UA_UInt32_init(&p->userWriteMask);
	p->referencesSize = -1;
	p->references = UA_NULL;
}

static void UA_Node_deleteMembers(UA_Node *p) {
	UA_NodeId_deleteMembers(&p->nodeId);
	UA_QualifiedName_deleteMembers(&p->browseName);
	UA_LocalizedText_deleteMembers(&p->displayName);
	UA_LocalizedText_deleteMembers(&p->description);
	UA_Array_delete(p->references, &UA_TYPES[UA_TYPES_REFERENCENODE], p->referencesSize);
}

static UA_StatusCode UA_Node_copy(const UA_Node *src, UA_Node *dst) {
	UA_StatusCode retval = UA_STATUSCODE_GOOD;
	UA_Node_init(dst);
	retval |= UA_NodeId_copy(&src->nodeId, &dst->nodeId);
	dst->nodeClass = src->nodeClass;
	retval |= UA_QualifiedName_copy(&src->browseName, &dst->browseName);
	retval |= UA_LocalizedText_copy(&src->displayName, &dst->displayName);
	retval |= UA_LocalizedText_copy(&src->description, &dst->description);
	dst->writeMask = src->writeMask;
	dst->userWriteMask = src->userWriteMask;
	dst->referencesSize = src->referencesSize;
	retval |= UA_Array_copy(src->references, (void**)&dst->references, &UA_TYPES[UA_TYPES_REFERENCENODE],
                            src->referencesSize);
	if(retval)
    	UA_Node_deleteMembers(dst);
	return retval;
}

/* UA_ObjectNode */
void UA_ObjectNode_init(UA_ObjectNode *p) {
	UA_Node_init((UA_Node*)p);
    p->nodeClass = UA_NODECLASS_OBJECT;
    p->eventNotifier = 0;
}

UA_ObjectNode * UA_ObjectNode_new(void) {
    UA_ObjectNode *p = (UA_ObjectNode*)UA_malloc(sizeof(UA_ObjectNode));
    if(p)
        UA_ObjectNode_init(p);
    return p;
}

void UA_ObjectNode_deleteMembers(UA_ObjectNode *p) {
    UA_Node_deleteMembers((UA_Node*)p);
}

void UA_ObjectNode_delete(UA_ObjectNode *p) {
    UA_ObjectNode_deleteMembers(p);
    UA_free(p);
}

UA_StatusCode UA_ObjectNode_copy(const UA_ObjectNode *src, UA_ObjectNode *dst) {
    dst->eventNotifier = src->eventNotifier;
	return UA_Node_copy((const UA_Node*)src, (UA_Node*)dst);
}

/* UA_ObjectTypeNode */
void UA_ObjectTypeNode_init(UA_ObjectTypeNode *p) {
	UA_Node_init((UA_Node*)p);
    p->nodeClass = UA_NODECLASS_OBJECTTYPE;
    p->isAbstract = UA_FALSE;
}

UA_ObjectTypeNode * UA_ObjectTypeNode_new(void) {
    UA_ObjectTypeNode *p = (UA_ObjectTypeNode*)UA_malloc(sizeof(UA_ObjectTypeNode));
    if(p)
        UA_ObjectTypeNode_init(p);
    return p;
}

void UA_ObjectTypeNode_deleteMembers(UA_ObjectTypeNode *p) {
    UA_Node_deleteMembers((UA_Node*)p);
}

void UA_ObjectTypeNode_delete(UA_ObjectTypeNode *p) {
    UA_ObjectTypeNode_deleteMembers(p);
    UA_free(p);
}

UA_StatusCode UA_ObjectTypeNode_copy(const UA_ObjectTypeNode *src, UA_ObjectTypeNode *dst) {
    dst->isAbstract = src->isAbstract;
	return UA_Node_copy((const UA_Node*)src, (UA_Node*)dst);
}

/* UA_VariableNode */
void UA_VariableNode_init(UA_VariableNode *p) {
	UA_Node_init((UA_Node*)p);
    p->nodeClass = UA_NODECLASS_VARIABLE;
    p->valueSource = UA_VALUESOURCE_VARIANT;
    UA_Variant_init(&p->value.variant);
    p->valueRank = -2; // scalar or array of any dimension
    p->accessLevel = 0;
    p->userAccessLevel = 0;
    p->minimumSamplingInterval = 0.0;
    p->historizing = UA_FALSE;
}

UA_VariableNode * UA_VariableNode_new(void) {
    UA_VariableNode *p = (UA_VariableNode*)UA_malloc(sizeof(UA_VariableNode));
    if(p)
        UA_VariableNode_init(p);
    return p;
}

void UA_VariableNode_deleteMembers(UA_VariableNode *p) {
    UA_Node_deleteMembers((UA_Node*)p);
    if(p->valueSource == UA_VALUESOURCE_VARIANT)
        UA_Variant_deleteMembers(&p->value.variant);
}

void UA_VariableNode_delete(UA_VariableNode *p) {
    UA_VariableNode_deleteMembers(p);
    UA_free(p);
}

UA_StatusCode UA_VariableNode_copy(const UA_VariableNode *src, UA_VariableNode *dst) {
    UA_VariableNode_init(dst);
	UA_StatusCode retval = UA_Node_copy((const UA_Node*)src, (UA_Node*)dst);
    dst->valueRank = src->valueRank;
    dst->valueSource = src->valueSource;
    if(src->valueSource == UA_VALUESOURCE_VARIANT)
        retval = UA_Variant_copy(&src->value.variant, &dst->value.variant);
    else
        dst->value.dataSource = src->value.dataSource;
    if(retval) {
        UA_VariableNode_deleteMembers(dst);
        return retval;
    }
    dst->accessLevel = src->accessLevel;
    dst->userAccessLevel = src->accessLevel;
    dst->minimumSamplingInterval = src->minimumSamplingInterval;
    dst->historizing = src->historizing;
    return UA_STATUSCODE_GOOD;
}

/* UA_VariableTypeNode */
void UA_VariableTypeNode_init(UA_VariableTypeNode *p) {
	UA_Node_init((UA_Node*)p);
    p->nodeClass = UA_NODECLASS_VARIABLETYPE;
    p->valueSource = UA_VALUESOURCE_VARIANT;
    UA_Variant_init(&p->value.variant);
    p->valueRank = -2; // scalar or array of any dimension
    p->isAbstract = UA_FALSE;
}

UA_VariableTypeNode * UA_VariableTypeNode_new(void) {
    UA_VariableTypeNode *p = (UA_VariableTypeNode*)UA_malloc(sizeof(UA_VariableTypeNode));
    if(p)
        UA_VariableTypeNode_init(p);
    return p;
}

void UA_VariableTypeNode_deleteMembers(UA_VariableTypeNode *p) {
    UA_Node_deleteMembers((UA_Node*)p);
    if(p->valueSource == UA_VALUESOURCE_VARIANT)
        UA_Variant_deleteMembers(&p->value.variant);
}

void UA_VariableTypeNode_delete(UA_VariableTypeNode *p) {
    UA_VariableTypeNode_deleteMembers(p);
    UA_free(p);
}

UA_StatusCode UA_VariableTypeNode_copy(const UA_VariableTypeNode *src, UA_VariableTypeNode *dst) {
    UA_VariableTypeNode_init(dst);
	UA_StatusCode retval = UA_Node_copy((const UA_Node*)src, (UA_Node*)dst);
    dst->valueRank = src->valueRank;
    dst->valueSource = src->valueSource;
    if(src->valueSource == UA_VALUESOURCE_VARIANT)
        UA_Variant_copy(&src->value.variant, &dst->value.variant);
    else
        dst->value.dataSource = src->value.dataSource;
    if(retval) {
        UA_VariableTypeNode_deleteMembers(dst);
        return retval;
    }
    dst->isAbstract = src->isAbstract;
    return UA_STATUSCODE_GOOD;
}

/* UA_ReferenceTypeNode */
void UA_ReferenceTypeNode_init(UA_ReferenceTypeNode *p) {
	UA_Node_init((UA_Node*)p);
    p->nodeClass = UA_NODECLASS_REFERENCETYPE;
    p->isAbstract = UA_FALSE;
    p->symmetric = UA_FALSE;
    UA_LocalizedText_init(&p->inverseName);
}

UA_ReferenceTypeNode * UA_ReferenceTypeNode_new(void) {
    UA_ReferenceTypeNode *p = (UA_ReferenceTypeNode*)UA_malloc(sizeof(UA_ReferenceTypeNode));
    if(p)
        UA_ReferenceTypeNode_init(p);
    return p;
}

void UA_ReferenceTypeNode_deleteMembers(UA_ReferenceTypeNode *p) {
    UA_Node_deleteMembers((UA_Node*)p);
    UA_LocalizedText_deleteMembers(&p->inverseName);
}

void UA_ReferenceTypeNode_delete(UA_ReferenceTypeNode *p) {
    UA_ReferenceTypeNode_deleteMembers(p);
    UA_free(p);
}

UA_StatusCode UA_ReferenceTypeNode_copy(const UA_ReferenceTypeNode *src, UA_ReferenceTypeNode *dst) {
	UA_StatusCode retval = UA_Node_copy((const UA_Node*)src, (UA_Node*)dst);
    if(retval)
        return retval;
    retval = UA_LocalizedText_copy(&src->inverseName, &dst->inverseName);
    if(retval) {
        UA_ReferenceTypeNode_deleteMembers(dst);
        return retval;
    }
    dst->isAbstract = src->isAbstract;
    dst->symmetric = src->symmetric;
    return UA_STATUSCODE_GOOD;
}

/* UA_MethodNode */
void UA_MethodNode_init(UA_MethodNode *p) {
	UA_Node_init((UA_Node*)p);
    p->nodeClass = UA_NODECLASS_METHOD;
    p->executable = UA_FALSE;
    p->userExecutable = UA_FALSE;
}

UA_MethodNode * UA_MethodNode_new(void) {
    UA_MethodNode *p = (UA_MethodNode*)UA_malloc(sizeof(UA_MethodNode));
    if(p)
        UA_MethodNode_init(p);
    return p;
}

void UA_MethodNode_deleteMembers(UA_MethodNode *p) {
    UA_Node_deleteMembers((UA_Node*)p);
}

void UA_MethodNode_delete(UA_MethodNode *p) {
    UA_MethodNode_deleteMembers(p);
    UA_free(p);
}

UA_StatusCode UA_MethodNode_copy(const UA_MethodNode *src, UA_MethodNode *dst) {
    dst->executable = src->executable;
    dst->userExecutable = src->userExecutable;
	return UA_Node_copy((const UA_Node*)src, (UA_Node*)dst);
}

/* UA_ViewNode */
void UA_ViewNode_init(UA_ViewNode *p) {
	UA_Node_init((UA_Node*)p);
    p->nodeClass = UA_NODECLASS_VIEW;
    p->containsNoLoops = UA_FALSE;
    p->eventNotifier = 0;
}

UA_ViewNode * UA_ViewNode_new(void) {
    UA_ViewNode *p = UA_malloc(sizeof(UA_ViewNode));
    if(p)
        UA_ViewNode_init(p);
    return p;
}

void UA_ViewNode_deleteMembers(UA_ViewNode *p) {
    UA_Node_deleteMembers((UA_Node*)p);
}

void UA_ViewNode_delete(UA_ViewNode *p) {
    UA_ViewNode_deleteMembers(p);
    UA_free(p);
}

UA_StatusCode UA_ViewNode_copy(const UA_ViewNode *src, UA_ViewNode *dst) {
    dst->containsNoLoops = src->containsNoLoops;
    dst->eventNotifier = src->eventNotifier;
	return UA_Node_copy((const UA_Node*)src, (UA_Node*)dst);
}

/* UA_DataTypeNode */
void UA_DataTypeNode_init(UA_DataTypeNode *p) {
	UA_Node_init((UA_Node*)p);
    p->nodeClass = UA_NODECLASS_DATATYPE;
    p->isAbstract = UA_FALSE;
}

UA_DataTypeNode * UA_DataTypeNode_new(void) {
    UA_DataTypeNode *p = UA_malloc(sizeof(UA_DataTypeNode));
    if(p)
        UA_DataTypeNode_init(p);
    return p;
}

void UA_DataTypeNode_deleteMembers(UA_DataTypeNode *p) {
    UA_Node_deleteMembers((UA_Node*)p);
}

void UA_DataTypeNode_delete(UA_DataTypeNode *p) {
    UA_DataTypeNode_deleteMembers(p);
    UA_free(p);
}

UA_StatusCode UA_DataTypeNode_copy(const UA_DataTypeNode *src, UA_DataTypeNode *dst) {
    dst->isAbstract = src->isAbstract;
	return UA_Node_copy((const UA_Node*)src, (UA_Node*)dst);
}
/*********************************** amalgamated original file "C:/AcpltDevelopmentKit/acplt/dev/open62541WrapperProject/open62541/src/server/ua_server_worker.c" ***********************************/

/**
 * There are three types of work:
 *
 * 1. Ordinary WorkItems (that are dispatched to worker threads if
 *    multithreading is activated)
 * 2. Timed work that is executed at a precise date (with an optional repetition
 *    interval)
 * 3. Delayed work that is executed at a later time when it is guaranteed that
 *    all previous work has actually finished (only for multithreading)
 */

#define MAXTIMEOUT 50000 // max timeout in microsec until the next main loop iteration
#define BATCHSIZE 20 // max size of worklists that are dispatched to workers

static void processWork(UA_Server *server, UA_WorkItem *work, size_t workSize) {
    for(size_t i = 0; i < workSize; i++) {
        UA_WorkItem *item = &work[i];
        switch(item->type) {
        case UA_WORKITEMTYPE_BINARYMESSAGE:
            UA_Server_processBinaryMessage(server, item->work.binaryMessage.connection,
                                           &item->work.binaryMessage.message);
            item->work.binaryMessage.connection->releaseBuffer(item->work.binaryMessage.connection,
                                                               &item->work.binaryMessage.message);
            break;
        case UA_WORKITEMTYPE_CLOSECONNECTION:
            UA_Connection_detachSecureChannel(item->work.closeConnection);
            item->work.closeConnection->close(item->work.closeConnection);
            break;
        case UA_WORKITEMTYPE_METHODCALL:
        case UA_WORKITEMTYPE_DELAYEDMETHODCALL:
            item->work.methodCall.method(server, item->work.methodCall.data);
            break;
        default:
            break;
        }
    }
}

/*******************************/
/* Worker Threads and Dispatch */
/*******************************/

#ifdef UA_MULTITHREADING

/** Entry in the dipatch queue */
struct workListNode {
    struct cds_wfcq_node node; // node for the queue
    UA_UInt32 workSize;
    UA_WorkItem *work;
};

/** Dispatch work to workers. Slices the work up if it contains more than
    BATCHSIZE items. The work array is freed by the worker threads. */
static void dispatchWork(UA_Server *server, UA_Int32 workSize, UA_WorkItem *work) {
    UA_Int32 startIndex = workSize; // start at the end
    while(workSize > 0) {
        UA_Int32 size = BATCHSIZE;
        if(size > workSize)
            size = workSize;
        startIndex = startIndex - size;
        struct workListNode *wln = UA_malloc(sizeof(struct workListNode));
        if(startIndex > 0) {
            UA_WorkItem *workSlice = UA_malloc(size * sizeof(UA_WorkItem));
            UA_memcpy(workSlice, &work[startIndex], size * sizeof(UA_WorkItem));
            *wln = (struct workListNode){.workSize = size, .work = workSlice};
        }
        else {
            // do not alloc, but forward the original array
            *wln = (struct workListNode){.workSize = size, .work = work};
        }
        cds_wfcq_node_init(&wln->node);
        cds_wfcq_enqueue(&server->dispatchQueue_head, &server->dispatchQueue_tail, &wln->node);
        workSize -= size;
    } 
}

// throwaway struct to bring data into the worker threads
struct workerStartData {
    UA_Server *server;
    UA_UInt32 **workerCounter;
};

/** Waits until work arrives in the dispatch queue (restart after 10ms) and
    processes it. */
static void * workerLoop(struct workerStartData *startInfo) {
   	rcu_register_thread();
    UA_UInt32 *c = UA_malloc(sizeof(UA_UInt32));
    uatomic_set(c, 0);

    *startInfo->workerCounter = c;
    UA_Server *server = startInfo->server;
    UA_free(startInfo);
    
    pthread_mutex_t mutex; // required for the condition variable
    pthread_mutex_init(&mutex,0);
    pthread_mutex_lock(&mutex);
    struct timespec to;

    while(*server->running) {
        struct workListNode *wln = (struct workListNode*)
            cds_wfcq_dequeue_blocking(&server->dispatchQueue_head, &server->dispatchQueue_tail);
        if(wln) {
            processWork(server, wln->work, wln->workSize);
            UA_free(wln->work);
            UA_free(wln);
        } else {
            clock_gettime(CLOCK_REALTIME, &to);
            to.tv_sec += 2;
            pthread_cond_timedwait(&server->dispatchQueue_condition, &mutex, &to);
        }
        uatomic_inc(c); // increase the workerCounter;
    }
    pthread_mutex_unlock(&mutex);
    pthread_mutex_destroy(&mutex);
   	rcu_unregister_thread();
    return UA_NULL;
}

static void emptyDispatchQueue(UA_Server *server) {
    while(!cds_wfcq_empty(&server->dispatchQueue_head, &server->dispatchQueue_tail)) {
        struct workListNode *wln = (struct workListNode*)
            cds_wfcq_dequeue_blocking(&server->dispatchQueue_head, &server->dispatchQueue_tail);
        processWork(server, wln->work, wln->workSize);
        UA_free(wln->work);
        UA_free(wln);
    }
}

#endif

/**************/
/* Timed Work */
/**************/

/**
 * The TimedWork structure contains an array of workitems that are either executed at the same time
 * or in the same repetition inverval. The linked list is sorted, so we can stop traversing when the
 * first element has nextTime > now.
 */
struct TimedWork {
    LIST_ENTRY(TimedWork) pointers;
    UA_DateTime nextTime;
    UA_UInt32 interval; ///> in 100ns resolution, 0 means no repetition
    size_t workSize;
    UA_WorkItem *work;
    UA_Guid workIds[];
};

/* Traverse the list until there is a TimedWork to which the item can be added or we reached the
   end. The item is copied into the TimedWork and not freed by this function. The interval is in
   100ns resolution */
static UA_StatusCode addTimedWork(UA_Server *server, const UA_WorkItem *item, UA_DateTime firstTime,
                                  UA_UInt32 interval, UA_Guid *resultWorkGuid) {
    struct TimedWork *matchingTw = UA_NULL; // add the item here
    struct TimedWork *lastTw = UA_NULL; // if there is no matchingTw, add a new TimedWork after this entry
    struct TimedWork *tempTw;

    /* search for matching entry */
    tempTw = LIST_FIRST(&server->timedWork);
    if(interval == 0) {
        /* single execution. the time needs to match */
        while(tempTw) {
            if(tempTw->nextTime >= firstTime) {
                if(tempTw->nextTime == firstTime)
                    matchingTw = tempTw;
                break;
            }
            lastTw = tempTw;
            tempTw = LIST_NEXT(lastTw, pointers);
        }
    } else {
        /* repeated execution. the interval needs to match */
        while(tempTw) {
            if(interval == tempTw->interval) {
                matchingTw = tempTw;
                break;
            }
            if(tempTw->nextTime > firstTime)
                break;
            lastTw = tempTw;
            tempTw = LIST_NEXT(lastTw, pointers);
        }
    }
    
    if(matchingTw) {
        /* append to matching entry */
        matchingTw = UA_realloc(matchingTw, sizeof(struct TimedWork) + sizeof(UA_Guid)*(matchingTw->workSize + 1));
        if(!matchingTw)
            return UA_STATUSCODE_BADOUTOFMEMORY;
        if(matchingTw->pointers.le_next)
            matchingTw->pointers.le_next->pointers.le_prev = &matchingTw->pointers.le_next;
        if(matchingTw->pointers.le_prev)
            *matchingTw->pointers.le_prev = matchingTw;
        UA_WorkItem *newItems = UA_realloc(matchingTw->work, sizeof(UA_WorkItem)*(matchingTw->workSize + 1));
        if(!newItems)
            return UA_STATUSCODE_BADOUTOFMEMORY;
        matchingTw->work = newItems;
    } else {
        /* create a new entry */
        matchingTw = UA_malloc(sizeof(struct TimedWork) + sizeof(UA_Guid));
        if(!matchingTw)
            return UA_STATUSCODE_BADOUTOFMEMORY;
        matchingTw->work = UA_malloc(sizeof(UA_WorkItem));
        if(!matchingTw->work) {
            UA_free(matchingTw);
            return UA_STATUSCODE_BADOUTOFMEMORY;
        }
        matchingTw->workSize = 0;
        matchingTw->nextTime = firstTime;
        matchingTw->interval = interval;
        if(lastTw)
            LIST_INSERT_AFTER(lastTw, matchingTw, pointers);
        else
            LIST_INSERT_HEAD(&server->timedWork, matchingTw, pointers);
    }
    matchingTw->work[matchingTw->workSize] = *item;
    matchingTw->workSize++;

    /* create a guid for finding and deleting the timed work later on */
    if(resultWorkGuid) {
        matchingTw->workIds[matchingTw->workSize] = UA_Guid_random(&server->random_seed);
        *resultWorkGuid = matchingTw->workIds[matchingTw->workSize];
    }
    return UA_STATUSCODE_GOOD;
}

UA_StatusCode UA_Server_addTimedWorkItem(UA_Server *server, const UA_WorkItem *work, UA_DateTime executionTime,
                                         UA_Guid *resultWorkGuid) {
    return addTimedWork(server, work, executionTime, 0, resultWorkGuid);
}

UA_StatusCode UA_Server_addRepeatedWorkItem(UA_Server *server, const UA_WorkItem *work, UA_UInt32 interval,
                                            UA_Guid *resultWorkGuid) {
    return addTimedWork(server, work, UA_DateTime_now() + interval * 10000, interval * 10000, resultWorkGuid);
}

/** Dispatches timed work, returns the timeout until the next timed work in ms */
static UA_UInt16 processTimedWork(UA_Server *server) {
    UA_DateTime current = UA_DateTime_now();
    struct TimedWork *next = LIST_FIRST(&server->timedWork);
    struct TimedWork *tw = UA_NULL;

    while(next) {
        tw = next;
        if(tw->nextTime > current)
            break;
        next = LIST_NEXT(tw, pointers);

#ifdef UA_MULTITHREADING
        if(tw->interval > 0) {
            // copy the entry and insert at the new location
            UA_WorkItem *workCopy = UA_malloc(sizeof(UA_WorkItem) * tw->workSize);
            UA_memcpy(workCopy, tw->work, sizeof(UA_WorkItem) * tw->workSize);
            dispatchWork(server, tw->workSize, workCopy); // frees the work pointer
            tw->nextTime += tw->interval;
            struct TimedWork *prevTw = tw; // after which tw do we insert?
            while(UA_TRUE) {
                struct TimedWork *n = LIST_NEXT(prevTw, pointers);
                if(!n || n->nextTime > tw->nextTime)
                    break;
                prevTw = n;
            }
            if(prevTw != tw) {
                LIST_REMOVE(tw, pointers);
                LIST_INSERT_AFTER(prevTw, tw, pointers);
            }
        } else {
            dispatchWork(server, tw->workSize, tw->work); // frees the work pointer
            LIST_REMOVE(tw, pointers);
            UA_free(tw);
        }
#else
        // 1) Process the work since it is past its due date
        processWork(server, tw->work, tw->workSize); // does not free the work ptr

        // 2) If the work is repeated, add it back into the list. Otherwise remove it.
        if(tw->interval > 0) {
            tw->nextTime += tw->interval;
            if(tw->nextTime < current)
                tw->nextTime = current;
            struct TimedWork *prevTw = tw;
            while(UA_TRUE) {
                struct TimedWork *n = LIST_NEXT(prevTw, pointers);
                if(!n || n->nextTime > tw->nextTime)
                    break;
                prevTw = n;
            }
            if(prevTw != tw) {
                LIST_REMOVE(tw, pointers);
                LIST_INSERT_AFTER(prevTw, tw, pointers);
            }
        } else {
            LIST_REMOVE(tw, pointers);
            UA_free(tw->work);
            UA_free(tw);
        }
#endif
    }

    // check if the next timed work is sooner than the usual timeout
    struct TimedWork *first = LIST_FIRST(&server->timedWork);
    UA_UInt16 timeout = MAXTIMEOUT;
    if(first) {
        timeout = (first->nextTime - current)/10;
        if(timeout > MAXTIMEOUT)
            return MAXTIMEOUT;
    }
    return timeout;
}

void UA_Server_deleteTimedWork(UA_Server *server) {
    struct TimedWork *current;
    struct TimedWork *next = LIST_FIRST(&server->timedWork);
    while(next) {
        current = next;
        next = LIST_NEXT(current, pointers);
        LIST_REMOVE(current, pointers);
        UA_free(current->work);
        UA_free(current);
    }
}

/****************/
/* Delayed Work */
/****************/

#ifdef UA_MULTITHREADING

#define DELAYEDWORKSIZE 100 // Collect delayed work until we have DELAYEDWORKSIZE items

struct DelayedWork {
    struct DelayedWork *next;
    UA_UInt32 *workerCounters; // initially UA_NULL until a workitem gets the counters
    UA_UInt32 workItemsCount; // the size of the array is DELAYEDWORKSIZE, the count may be less
    UA_WorkItem *workItems; // when it runs full, a new delayedWork entry is created
};

// Dispatched as a methodcall-WorkItem when the delayedwork is added
static void getCounters(UA_Server *server, struct DelayedWork *delayed) {
    UA_UInt32 *counters = UA_malloc(server->nThreads * sizeof(UA_UInt32));
    for(UA_UInt16 i = 0;i<server->nThreads;i++)
        counters[i] = *server->workerCounters[i];
    delayed->workerCounters = counters;
}

// Call from the main thread only. This is the only function that modifies
// server->delayedWork. processDelayedWorkQueue modifies the "next" (after the
// head).
static void addDelayedWork(UA_Server *server, UA_WorkItem work) {
    struct DelayedWork *dw = server->delayedWork;
    if(!dw || dw->workItemsCount >= DELAYEDWORKSIZE) {
        struct DelayedWork *newwork = UA_malloc(sizeof(struct DelayedWork));
        newwork->workItems = UA_malloc(sizeof(UA_WorkItem)*DELAYEDWORKSIZE);
        newwork->workItemsCount = 0;
        newwork->workerCounters = UA_NULL;
        newwork->next = server->delayedWork;

        // dispatch a method that sets the counter
        if(dw && dw->workItemsCount >= DELAYEDWORKSIZE) {
            UA_WorkItem *setCounter = UA_malloc(sizeof(UA_WorkItem));
            *setCounter = (UA_WorkItem)
                {.type = UA_WORKITEMTYPE_METHODCALL,
                 .work.methodCall = {.method = (void (*)(UA_Server*, void*))getCounters, .data = dw}};
            dispatchWork(server, 1, setCounter);
        }

        server->delayedWork = newwork;
        dw = newwork;
    }
    dw->workItems[dw->workItemsCount] = work;
    dw->workItemsCount++;
}

static void processDelayedWork(UA_Server *server) {
    struct DelayedWork *dw = server->delayedWork;
    while(dw) {
        processWork(server, dw->workItems, dw->workItemsCount);
        struct DelayedWork *next = dw->next;
        UA_free(dw->workerCounters);
        UA_free(dw->workItems);
        UA_free(dw);
        dw = next;
    }
}

// Execute this every N seconds (repeated work) to execute delayed work that is ready
static void dispatchDelayedWork(UA_Server *server, void *data /* not used, but needed for the signature*/) {
    struct DelayedWork *dw = UA_NULL;
    struct DelayedWork *readydw = UA_NULL;
    struct DelayedWork *beforedw = server->delayedWork;

    // start at the second...
    if(beforedw)
        dw = beforedw->next;

    // find the first delayedwork where the counters are set and have been moved
    while(dw) {
        if(!dw->workerCounters) {
            beforedw = dw;
            dw = dw->next;
            continue;
        }

        UA_Boolean countersMoved = UA_TRUE;
        for(UA_UInt16 i=0;i<server->nThreads;i++) {
            if(*server->workerCounters[i] == dw->workerCounters[i])
                countersMoved = UA_FALSE;
                break;
        }
        
        if(countersMoved) {
            readydw = uatomic_xchg(&beforedw->next, UA_NULL);
            break;
        } else {
            beforedw = dw;
            dw = dw->next;
        }
    }

    // we have a ready entry. all afterwards are also ready
    while(readydw) {
        dispatchWork(server, readydw->workItemsCount, readydw->workItems);
        beforedw = readydw;
        readydw = readydw->next;
        UA_free(beforedw->workerCounters);
        UA_free(beforedw);
    }
}

#endif

/********************/
/* Main Server Loop */
/********************/

UA_StatusCode UA_Server_run_startup(UA_Server *server, UA_UInt16 nThreads, UA_Boolean *running){
#ifdef UA_MULTITHREADING
    // 1) Prepare the threads
    server->running = running; // the threads need to access the variable
    server->nThreads = nThreads;
    pthread_cond_init(&server->dispatchQueue_condition, 0);
    server->thr = UA_malloc(nThreads * sizeof(pthread_t));
    server->workerCounters = UA_malloc(nThreads * sizeof(UA_UInt32 *));
    for(UA_UInt32 i=0;i<nThreads;i++) {
        struct workerStartData *startData = UA_malloc(sizeof(struct workerStartData));
        startData->server = server;
        startData->workerCounter = &server->workerCounters[i];
        pthread_create(&server->thr[i], UA_NULL, (void* (*)(void*))workerLoop, startData);
    }

    UA_WorkItem processDelayed = {.type = UA_WORKITEMTYPE_METHODCALL,
            .work.methodCall = {.method = dispatchDelayedWork,
                    .data = UA_NULL} };
    UA_Server_addRepeatedWorkItem(server, &processDelayed, 10000000, UA_NULL);
#endif

// 2) Start the networklayers
for(size_t i = 0; i <server->networkLayersSize; i++)
    server->networkLayers[i].start(server->networkLayers[i].nlHandle, &server->logger);

return UA_STATUSCODE_GOOD;
}

UA_StatusCode UA_Server_run_getAndProcessWork(UA_Server *server, UA_Boolean *running){
    // 3.1) Process timed work
    UA_UInt16 timeout = processTimedWork(server);

    // 3.2) Get work from the networklayer and dispatch it
    for(size_t i = 0; i < server->networkLayersSize; i++) {
        UA_ServerNetworkLayer *nl = &server->networkLayers[i];
        UA_WorkItem *work;
        UA_Int32 workSize;
        if(*running) {
            if(i == server->networkLayersSize-1)
                workSize = nl->getWork(nl->nlHandle, &work, timeout);
            else
                workSize = nl->getWork(nl->nlHandle, &work, 0);
        } else {
            workSize = server->networkLayers[i].stop(nl->nlHandle, &work);
        }

#ifdef UA_MULTITHREADING
// Filter out delayed work
for(UA_Int32 k=0;k<workSize;k++) {
    if(work[k].type != UA_WORKITEMTYPE_DELAYEDMETHODCALL)
        continue;
    addDelayedWork(server, work[k]);
    work[k].type = UA_WORKITEMTYPE_NOTHING;
}
dispatchWork(server, workSize, work);
if(workSize > 0)
    pthread_cond_broadcast(&server->dispatchQueue_condition);
#else
processWork(server, work, workSize);
if(workSize > 0)
    UA_free(work);
#endif
    }
    return UA_STATUSCODE_GOOD;
}

UA_StatusCode UA_Server_run_shutdown(UA_Server *server, UA_UInt16 nThreads){
#ifdef UA_MULTITHREADING
    // 4) Clean up: Wait until all worker threads finish, then empty the
    // dispatch queue, then process the remaining delayed work
    for(UA_UInt32 i=0;i<nThreads;i++) {
        pthread_join(server->thr[i], UA_NULL);
        UA_free(server->workerCounters[i]);
    }
    UA_free(server->workerCounters);
    UA_free(server->thr);
    emptyDispatchQueue(server);
    processDelayedWork(server);
#endif

    return UA_STATUSCODE_GOOD;
}

UA_StatusCode UA_Server_run(UA_Server *server, UA_UInt16 nThreads, UA_Boolean *running) {
    UA_Server_run_startup(server, nThreads, running);

    // 3) The loop
    while(1) {
        UA_Server_run_getAndProcessWork(server, running);

        // 3.3) Exit?
        if(!*running)
            break;
    }

    UA_Server_run_shutdown(server, nThreads);

    return UA_STATUSCODE_GOOD;
}
/*********************************** amalgamated original file "C:/AcpltDevelopmentKit/acplt/dev/open62541WrapperProject/open62541/src/server/ua_securechannel_manager.c" ***********************************/

UA_StatusCode UA_SecureChannelManager_init(UA_SecureChannelManager *cm, UA_UInt32 maxChannelCount,
                                           UA_UInt32 tokenLifetime, UA_UInt32 startChannelId,
                                           UA_UInt32 startTokenId) {
    LIST_INIT(&cm->channels);
    cm->lastChannelId      = startChannelId;
    cm->lastTokenId        = startTokenId;
    cm->maxChannelLifetime = tokenLifetime;
    cm->maxChannelCount    = maxChannelCount;
    return UA_STATUSCODE_GOOD;
}

void UA_SecureChannelManager_deleteMembers(UA_SecureChannelManager *cm) {
    channel_list_entry *current = LIST_FIRST(&cm->channels);
    while(current) {
        LIST_REMOVE(current, pointers);
        UA_SecureChannel_deleteMembersCleanup(&current->channel);
        UA_free(current);
        current = LIST_FIRST(&cm->channels);
    }
}

void UA_SecureChannelManager_cleanupTimedOut(UA_SecureChannelManager *cm, UA_DateTime now) {
    channel_list_entry *entry = LIST_FIRST(&cm->channels);
    /* remove channels that were not renewed or who have no connection attached */
    while(entry) {
        if(entry->channel.securityToken.createdAt +
            (10000 * entry->channel.securityToken.revisedLifetime) > now &&
            entry->channel.connection) {
            entry = LIST_NEXT(entry, pointers);
        }
        else {
            channel_list_entry *next = LIST_NEXT(entry, pointers);
            LIST_REMOVE(entry, pointers);
            UA_SecureChannel_deleteMembersCleanup(&entry->channel);
            UA_free(entry);
            entry = next;
        }
    }
}


UA_StatusCode UA_SecureChannelManager_open(UA_SecureChannelManager *cm, UA_Connection *conn,
                                           const UA_OpenSecureChannelRequest *request,
                                           UA_OpenSecureChannelResponse *response) {
    switch(request->securityMode) {
    case UA_MESSAGESECURITYMODE_NONE:
        break;
    case UA_MESSAGESECURITYMODE_INVALID:
    case UA_MESSAGESECURITYMODE_SIGN:
    case UA_MESSAGESECURITYMODE_SIGNANDENCRYPT:
    default:
        response->responseHeader.serviceResult = UA_STATUSCODE_BADSECURITYMODEREJECTED;
        return response->responseHeader.serviceResult;
    }

    channel_list_entry *entry = UA_malloc(sizeof(channel_list_entry));
    if(!entry)
        return UA_STATUSCODE_BADOUTOFMEMORY;

    UA_SecureChannel_init(&entry->channel);
    response->responseHeader.stringTableSize = 0;
    response->responseHeader.timestamp       = UA_DateTime_now();
    response->serverProtocolVersion = 0;

    entry->channel.securityToken.channelId       = cm->lastChannelId++;
    entry->channel.securityToken.tokenId         = cm->lastTokenId++;
    entry->channel.securityToken.createdAt       = UA_DateTime_now();
    entry->channel.securityToken.revisedLifetime = (request->requestedLifetime > cm->maxChannelLifetime) ?
                                                   cm->maxChannelLifetime : request->requestedLifetime;
    //FIXME: pragmatic workaround to get clients requesting lifetime of 0 working
    if(entry->channel.securityToken.revisedLifetime == 0) {
        entry->channel.securityToken.revisedLifetime = cm->maxChannelLifetime;
        //FIXME: I'd log it, but there is no pointer to the logger
        // printf("Warning: client requests token lifetime of 0 in OpenSecureChannelRequest setting it to %llu\n", cm->maxChannelLifetime);
    }

    UA_ByteString_copy(&request->clientNonce, &entry->channel.clientNonce);
    entry->channel.serverAsymAlgSettings.securityPolicyUri =
        UA_STRING_ALLOC("http://opcfoundation.org/UA/SecurityPolicy#None");

    UA_SecureChannel_generateNonce(&entry->channel.serverNonce);
    UA_ByteString_copy(&entry->channel.serverNonce, &response->serverNonce);
    UA_ChannelSecurityToken_copy(&entry->channel.securityToken, &response->securityToken);

    UA_Connection_attachSecureChannel(conn, &entry->channel);
    LIST_INSERT_HEAD(&cm->channels, entry, pointers);

    return UA_STATUSCODE_GOOD;
}

UA_StatusCode UA_SecureChannelManager_renew(UA_SecureChannelManager *cm, UA_Connection *conn,
                                            const UA_OpenSecureChannelRequest *request,
                                            UA_OpenSecureChannelResponse *response)
{
    UA_SecureChannel *channel = conn->channel;
    if(channel == UA_NULL)
        return UA_STATUSCODE_BADINTERNALERROR;

    channel->securityToken.tokenId         = cm->lastTokenId++;
    channel->securityToken.createdAt       = UA_DateTime_now(); // todo: is wanted?
    channel->securityToken.revisedLifetime = (request->requestedLifetime > cm->maxChannelLifetime) ?
                                             cm->maxChannelLifetime : request->requestedLifetime;
    //FIXME: pragmatic workaround to get clients requesting lifetime of 0 working
    if(channel->securityToken.revisedLifetime == 0){
        channel->securityToken.revisedLifetime = cm->maxChannelLifetime;
        //FIXME: I'd log it, but there is no pointer to the logger
        // printf("Warning: client requests token lifetime of 0 in renewing SecureChannel setting it to %llu\n", cm->maxChannelLifetime);
    }

    if(channel->serverNonce.data != UA_NULL)
        UA_ByteString_deleteMembers(&channel->serverNonce);
    UA_SecureChannel_generateNonce(&channel->serverNonce);
    UA_ByteString_copy(&channel->serverNonce, &response->serverNonce);
    UA_ChannelSecurityToken_copy(&channel->securityToken, &response->securityToken);
    return UA_STATUSCODE_GOOD;
}

UA_SecureChannel * UA_SecureChannelManager_get(UA_SecureChannelManager *cm, UA_UInt32 channelId) {
    channel_list_entry *entry;
    LIST_FOREACH(entry, &cm->channels, pointers) {
        if(entry->channel.securityToken.channelId == channelId)
            return &entry->channel;
    }
    return UA_NULL;
}

UA_StatusCode UA_SecureChannelManager_close(UA_SecureChannelManager *cm, UA_UInt32 channelId) {
    channel_list_entry *entry;
    LIST_FOREACH(entry, &cm->channels, pointers) {
        if(entry->channel.securityToken.channelId == channelId)
            break;
    }
    if(!entry)
        return UA_STATUSCODE_BADINTERNALERROR;
    LIST_REMOVE(entry, pointers);
    UA_SecureChannel_deleteMembersCleanup(&entry->channel);
    UA_free(entry);
    return UA_STATUSCODE_GOOD;
}
/*********************************** amalgamated original file "C:/AcpltDevelopmentKit/acplt/dev/open62541WrapperProject/open62541/src/server/ua_session_manager.c" ***********************************/

UA_StatusCode UA_SessionManager_init(UA_SessionManager *sessionManager, UA_UInt32 maxSessionCount,
                                    UA_UInt32 maxSessionLifeTime, UA_UInt32 startSessionId) {
    LIST_INIT(&sessionManager->sessions);
    sessionManager->maxSessionCount = maxSessionCount;
    sessionManager->lastSessionId   = startSessionId;
    sessionManager->maxSessionLifeTime  = maxSessionLifeTime;
    sessionManager->currentSessionCount = 0;
    return UA_STATUSCODE_GOOD;
}

void UA_SessionManager_deleteMembers(UA_SessionManager *sessionManager) {
    session_list_entry *current;
    while((current = LIST_FIRST(&sessionManager->sessions))) {
        LIST_REMOVE(current, pointers);
        UA_Session_deleteMembersCleanup(&current->session);
        UA_free(current);
    }
}

void UA_SessionManager_cleanupTimedOut(UA_SessionManager *sessionManager, UA_DateTime now) {
    session_list_entry *sentry = LIST_FIRST(&sessionManager->sessions);
    while(sentry) {
        if(sentry->session.validTill < now) {
            session_list_entry *next = LIST_NEXT(sentry, pointers);
            LIST_REMOVE(sentry, pointers);
            UA_Session_deleteMembersCleanup(&sentry->session);
            UA_free(sentry);
            sessionManager->currentSessionCount--;
            sentry = next;
        } else {
            sentry = LIST_NEXT(sentry, pointers);
        }
    }
}

UA_Session * UA_SessionManager_getSession(UA_SessionManager *sessionManager, const UA_NodeId *token) {
    session_list_entry *current = UA_NULL;
    LIST_FOREACH(current, &sessionManager->sessions, pointers) {
        if(UA_NodeId_equal(&current->session.authenticationToken, token))
            break;
    }
    if(!current || UA_DateTime_now() > current->session.validTill)
        return UA_NULL;
    return &current->session;
}

/** Creates and adds a session. But it is not yet attached to a secure channel. */
UA_StatusCode UA_SessionManager_createSession(UA_SessionManager *sessionManager, UA_SecureChannel *channel,
                                              const UA_CreateSessionRequest *request, UA_Session **session) {
    if(sessionManager->currentSessionCount >= sessionManager->maxSessionCount)
        return UA_STATUSCODE_BADTOOMANYSESSIONS;

    session_list_entry *newentry = UA_malloc(sizeof(session_list_entry));
    if(!newentry)
        return UA_STATUSCODE_BADOUTOFMEMORY;

    sessionManager->currentSessionCount++;
    UA_Session_init(&newentry->session);
    newentry->session.sessionId = UA_NODEID_NUMERIC(1, sessionManager->lastSessionId++);
    UA_UInt32 randSeed = sessionManager->lastSessionId;
    newentry->session.authenticationToken = UA_NODEID_GUID(1, UA_Guid_random(&randSeed));
    if(request->requestedSessionTimeout <= sessionManager->maxSessionLifeTime &&
       request->requestedSessionTimeout > 0)
        newentry->session.timeout = request->requestedSessionTimeout;
    else
        newentry->session.timeout = sessionManager->maxSessionLifeTime; // todo: remove when the CTT is fixed
    UA_Session_updateLifetime(&newentry->session);
    LIST_INSERT_HEAD(&sessionManager->sessions, newentry, pointers);
    *session = &newentry->session;
    return UA_STATUSCODE_GOOD;
}

UA_StatusCode UA_SessionManager_removeSession(UA_SessionManager *sessionManager, const UA_NodeId *token) {
    session_list_entry *current;
    LIST_FOREACH(current, &sessionManager->sessions, pointers) {
        if(UA_NodeId_equal(&current->session.authenticationToken, token))
            break;
    }
    if(!current)
        return UA_STATUSCODE_BADSESSIONIDINVALID;
    LIST_REMOVE(current, pointers);
    UA_Session_deleteMembersCleanup(&current->session);
    UA_free(current);
    sessionManager->currentSessionCount--;
    return UA_STATUSCODE_GOOD;
}
/*********************************** amalgamated original file "C:/AcpltDevelopmentKit/acplt/dev/open62541WrapperProject/open62541/src/server/ua_services_discovery.c" ***********************************/

void Service_FindServers(UA_Server *server, const UA_FindServersRequest *request, UA_FindServersResponse *response) {
    response->servers = UA_malloc(sizeof(UA_ApplicationDescription));
    if(!response->servers) {
        response->responseHeader.serviceResult = UA_STATUSCODE_BADOUTOFMEMORY;
        return;
    }
    if(UA_ApplicationDescription_copy(&server->description, response->servers) != UA_STATUSCODE_GOOD) {
        UA_free(response->servers);
        response->responseHeader.serviceResult = UA_STATUSCODE_BADOUTOFMEMORY;
        return;
    }
	response->serversSize = 1;
}

void Service_GetEndpoints(UA_Server *server, const UA_GetEndpointsRequest *request, UA_GetEndpointsResponse *response) {
    /* test if the supported binary profile shall be returned */
#ifdef NO_ALLOCA
	UA_Boolean relevant_endpoints[server->endpointDescriptionsSize];
#else
	UA_Boolean *relevant_endpoints = UA_alloca(sizeof(UA_Boolean)*server->endpointDescriptionsSize);
#endif /*NO_ALLOCA */
    size_t relevant_count = 0;
    for(UA_Int32 j = 0; j < server->endpointDescriptionsSize; j++) {
        relevant_endpoints[j] = UA_FALSE;
        if(request->profileUrisSize <= 0) {
            relevant_endpoints[j] = UA_TRUE;
            relevant_count++;
            continue;
        }
        for(UA_Int32 i = 0; i < request->profileUrisSize; i++) {
            if(UA_String_equal(&request->profileUris[i], &server->endpointDescriptions->transportProfileUri)) {
                relevant_endpoints[j] = UA_TRUE;
                relevant_count++;
                break;
            }
        }
    }

    if(relevant_count == 0) {
        response->endpointsSize = 0;
        return;
    }

    response->endpoints = UA_malloc(sizeof(UA_EndpointDescription) * relevant_count);
    if(!response->endpoints) {
        response->responseHeader.serviceResult = UA_STATUSCODE_BADOUTOFMEMORY;
        return;
    }

    size_t k = 0;
    UA_StatusCode retval = UA_STATUSCODE_GOOD;
    for(UA_Int32 j = 0; j < server->endpointDescriptionsSize && retval == UA_STATUSCODE_GOOD; j++) {
        if(relevant_endpoints[j] != UA_TRUE)
            continue;
        retval = UA_EndpointDescription_copy(&server->endpointDescriptions[j], &response->endpoints[k]);
        k++;
    }

    if(retval != UA_STATUSCODE_GOOD) {
        response->responseHeader.serviceResult = retval;
        UA_Array_delete(response->endpoints, &UA_TYPES[UA_TYPES_ENDPOINTDESCRIPTION], --k);
        return;
    }
    response->endpointsSize = relevant_count;
}

/*********************************** amalgamated original file "C:/AcpltDevelopmentKit/acplt/dev/open62541WrapperProject/open62541/src/server/ua_services_securechannel.c" ***********************************/

void Service_OpenSecureChannel(UA_Server *server, UA_Connection *connection,
                               const UA_OpenSecureChannelRequest *request,
                               UA_OpenSecureChannelResponse *response) {
    // todo: if(request->clientProtocolVersion != protocolVersion)
    if(request->requestType == UA_SECURITYTOKENREQUESTTYPE_ISSUE)
        UA_SecureChannelManager_open(&server->secureChannelManager, connection, request, response);
    else
        UA_SecureChannelManager_renew(&server->secureChannelManager, connection, request, response);
}

void Service_CloseSecureChannel(UA_Server *server, UA_Int32 channelId) {
	//Sten: this service is a bit assymmetric to OpenSecureChannel since CloseSecureChannelRequest does not contain any indormation
    UA_SecureChannelManager_close(&server->secureChannelManager, channelId);
    // 62451 Part 6 Chapter 7.1.4 - The server does not send a CloseSecureChannel response
}
/*********************************** amalgamated original file "C:/AcpltDevelopmentKit/acplt/dev/open62541WrapperProject/open62541/src/server/ua_services_session.c" ***********************************/

void Service_CreateSession(UA_Server *server, UA_SecureChannel *channel,
                           const UA_CreateSessionRequest *request,
                           UA_CreateSessionResponse *response) {
    response->responseHeader.serviceResult =
        UA_Array_copy(server->endpointDescriptions, (void**)&response->serverEndpoints,
                      &UA_TYPES[UA_TYPES_ENDPOINTDESCRIPTION], server->endpointDescriptionsSize);
    if(response->responseHeader.serviceResult != UA_STATUSCODE_GOOD)
        return;
    response->serverEndpointsSize = server->endpointDescriptionsSize;

	UA_Session *newSession;
    response->responseHeader.serviceResult = UA_SessionManager_createSession(&server->sessionManager,
                                                                             channel, request, &newSession);
	if(response->responseHeader.serviceResult != UA_STATUSCODE_GOOD)
		return;

    //TODO get maxResponseMessageSize internally
    newSession->maxResponseMessageSize = request->maxResponseMessageSize;
    response->sessionId = newSession->sessionId;
    response->revisedSessionTimeout = newSession->timeout;
    response->authenticationToken = newSession->authenticationToken;
    response->responseHeader.serviceResult = UA_String_copy(&request->sessionName, &newSession->sessionName);
    if(server->endpointDescriptions)
        response->responseHeader.serviceResult |=
            UA_ByteString_copy(&server->endpointDescriptions->serverCertificate, &response->serverCertificate);
    if(response->responseHeader.serviceResult != UA_STATUSCODE_GOOD) {
        UA_SessionManager_removeSession(&server->sessionManager, &newSession->authenticationToken);
         return;
    }
}

void Service_ActivateSession(UA_Server *server, UA_SecureChannel *channel,
                             const UA_ActivateSessionRequest *request,
                             UA_ActivateSessionResponse *response) {
    // make the channel know about the session
	UA_Session *foundSession =
        UA_SessionManager_getSession(&server->sessionManager,
                                     (const UA_NodeId*)&request->requestHeader.authenticationToken);

	if(foundSession == UA_NULL) {
        response->responseHeader.serviceResult = UA_STATUSCODE_BADSESSIONIDINVALID;
        return;
	} else if(foundSession->validTill < UA_DateTime_now()) {
        response->responseHeader.serviceResult = UA_STATUSCODE_BADSESSIONIDINVALID;
        return;
	}

    UA_UserIdentityToken token;
    UA_UserIdentityToken_init(&token);
    size_t offset = 0;
    UA_UserIdentityToken_decodeBinary(&request->userIdentityToken.body, &offset, &token);

    UA_UserNameIdentityToken username_token;
    UA_UserNameIdentityToken_init(&username_token);

    if(token.policyId.data == UA_NULL) {
        /* 1) no policy defined */
        response->responseHeader.serviceResult = UA_STATUSCODE_BADIDENTITYTOKENINVALID;
    } else if(server->config.Login_enableAnonymous &&
              UA_String_equalchars(&token.policyId, ANONYMOUS_POLICY)) {
        /* 2) anonymous logins */
        if(foundSession->channel && foundSession->channel != channel)
            UA_SecureChannel_detachSession(foundSession->channel, foundSession);
        UA_SecureChannel_attachSession(channel, foundSession);
        foundSession->activated = UA_TRUE;
        UA_Session_updateLifetime(foundSession);
    } else if(server->config.Login_enableUsernamePassword &&
              UA_String_equalchars(&token.policyId, USERNAME_POLICY)) {
        /* 3) username logins */
        offset = 0;
        UA_UserNameIdentityToken_decodeBinary(&request->userIdentityToken.body, &offset, &username_token);
        if(username_token.encryptionAlgorithm.data != UA_NULL) {
            /* 3.1) we only support encryption */
            response->responseHeader.serviceResult = UA_STATUSCODE_BADIDENTITYTOKENINVALID;
        } else  if(username_token.userName.length == -1 && username_token.password.length == -1){
            /* 3.2) empty username and password */
            response->responseHeader.serviceResult = UA_STATUSCODE_BADIDENTITYTOKENINVALID;
        } else {
            /* 3.3) ok, trying to match the username */
            UA_UInt32 i = 0;
            for(; i < server->config.Login_loginsCount; ++i) {
                if(UA_String_equalchars(&username_token.userName, server->config.Login_usernames[i])
                    && UA_String_equalchars(&username_token.password, server->config.Login_passwords[i])) {
                    /* success - activate */
                    if(foundSession->channel && foundSession->channel != channel)
                        UA_SecureChannel_detachSession(foundSession->channel, foundSession);
                    UA_SecureChannel_attachSession(channel, foundSession);
                    foundSession->activated = UA_TRUE;
                    UA_Session_updateLifetime(foundSession);
                    break;
                }
            }
            /* no username/pass matched */
            if(i >= server->config.Login_loginsCount)
                response->responseHeader.serviceResult = UA_STATUSCODE_BADUSERACCESSDENIED;
        }
    } else {
        response->responseHeader.serviceResult = UA_STATUSCODE_BADIDENTITYTOKENINVALID;
    }
    UA_UserIdentityToken_deleteMembers(&token);
    UA_UserNameIdentityToken_deleteMembers(&username_token);
    return;
}

void Service_CloseSession(UA_Server *server, UA_Session *session, const UA_CloseSessionRequest *request,
                          UA_CloseSessionResponse *response) {
	UA_Session *foundSession =
        UA_SessionManager_getSession(&server->sessionManager,
		                             (const UA_NodeId*)&request->requestHeader.authenticationToken);
	if(foundSession == UA_NULL)
		response->responseHeader.serviceResult = UA_STATUSCODE_BADSESSIONIDINVALID;
	else 
        response->responseHeader.serviceResult =
            UA_SessionManager_removeSession(&server->sessionManager, &session->authenticationToken);
}
/*********************************** amalgamated original file "C:/AcpltDevelopmentKit/acplt/dev/open62541WrapperProject/open62541/src/server/ua_services_attribute.c" ***********************************/

static UA_StatusCode parse_numericrange(const UA_String str, UA_NumericRange *range) {
    if(str.length < 0 || str.length >= 1023)
        return UA_STATUSCODE_BADINTERNALERROR;
#ifdef NO_ALLOCA
    char cstring[str.length+1];
#else
    char *cstring = UA_alloca(str.length+1);
#endif
    UA_memcpy(cstring, str.data, str.length);
    cstring[str.length] = 0;
    UA_Int32 index = 0;
    size_t dimensionsIndex = 0;
    size_t dimensionsMax = 3; // more should be uncommon, realloc if necessary
    struct UA_NumericRangeDimension *dimensions = UA_malloc(sizeof(struct UA_NumericRangeDimension) * 3);
    if(!dimensions)
        return UA_STATUSCODE_BADOUTOFMEMORY;
    
    UA_StatusCode retval = UA_STATUSCODE_GOOD;
    do {
        UA_Int32 min, max;
        UA_Int32 progress;
        UA_Int32 res = sscanf(&cstring[index], "%" SCNu32 "%n", &min, &progress);
        if(res <= 0 || min < 0) {
            retval = UA_STATUSCODE_BADINDEXRANGEINVALID;
            break;
        }
        index += progress;
        if(index >= str.length || cstring[index] == ',')
            max = min;
        else {
            res = sscanf(&cstring[index], ":%" SCNu32 "%n", &max, &progress);
            if(res <= 0 || max < 0 || min >= max) {
                retval = UA_STATUSCODE_BADINDEXRANGEINVALID;
                break;
            }
            index += progress;
        }
        
        if(dimensionsIndex >= dimensionsMax) {
            struct UA_NumericRangeDimension *newDimensions =
                UA_realloc(dimensions, sizeof(struct UA_NumericRangeDimension) * 2 * dimensionsMax);
            if(!newDimensions) {
                UA_free(dimensions);
                return UA_STATUSCODE_BADOUTOFMEMORY;
            }
            dimensions = newDimensions;
            dimensionsMax *= 2;
        }

        dimensions[dimensionsIndex].min = min;
        dimensions[dimensionsIndex].max = max;
        dimensionsIndex++;
    } while(retval == UA_STATUSCODE_GOOD && index + 1 < str.length && cstring[index] == ',' && ++index);

    if(retval != UA_STATUSCODE_GOOD) {
        UA_free(dimensions);
        return retval;
    }
        
    range->dimensions = dimensions;
    range->dimensionsSize = dimensionsIndex;
    return retval;
}

#define CHECK_NODECLASS(CLASS)                                  \
    if(!(node->nodeClass & (CLASS))) {                          \
        v->hasStatus = UA_TRUE;                                 \
        v->status = UA_STATUSCODE_BADATTRIBUTEIDINVALID;        \
        break;                                                  \
    }

static void handleServerTimestamps(UA_TimestampsToReturn timestamps, UA_DataValue* v) {
	if (v && (timestamps == UA_TIMESTAMPSTORETURN_SERVER
			|| timestamps == UA_TIMESTAMPSTORETURN_BOTH)) {
		v->hasServerTimestamp = UA_TRUE;
		v->serverTimestamp = UA_DateTime_now();
	}
}

static void handleSourceTimestamps(UA_TimestampsToReturn timestamps, UA_DataValue* v) {
	if(timestamps == UA_TIMESTAMPSTORETURN_SOURCE || timestamps == UA_TIMESTAMPSTORETURN_BOTH) {
		v->hasSourceTimestamp = UA_TRUE;
		v->sourceTimestamp = UA_DateTime_now();
	}
}

/** Reads a single attribute from a node in the nodestore. */
static void readValue(UA_Server *server, UA_TimestampsToReturn timestamps,
                      const UA_ReadValueId *id, UA_DataValue *v) {
    UA_String binEncoding = UA_STRING("DefaultBinary");
    UA_String xmlEncoding = UA_STRING("DefaultXml");
	if(id->dataEncoding.name.length >= 0){
		if(!UA_String_equal(&binEncoding, &id->dataEncoding.name) &&
           !UA_String_equal(&xmlEncoding, &id->dataEncoding.name)) {
			v->hasStatus = UA_TRUE;
			v->status = UA_STATUSCODE_BADDATAENCODINGINVALID;
			return;
		}
	}

	//index range for a non-value
	if(id->indexRange.length >= 0 && id->attributeId != UA_ATTRIBUTEID_VALUE){
		v->hasStatus = UA_TRUE;
		v->status = UA_STATUSCODE_BADINDEXRANGENODATA;
		return;
	}

    UA_Node const *node = UA_NodeStore_get(server->nodestore, &(id->nodeId));
    if(!node) {
        v->hasStatus = UA_TRUE;
        v->status = UA_STATUSCODE_BADNODEIDUNKNOWN;
        return;
    }

    UA_StatusCode retval = UA_STATUSCODE_GOOD;
    switch(id->attributeId) {
    case UA_ATTRIBUTEID_NODEID:
        v->hasValue = UA_TRUE;
        retval |= UA_Variant_setScalarCopy(&v->value, &node->nodeId, &UA_TYPES[UA_TYPES_NODEID]);
        break;
    case UA_ATTRIBUTEID_NODECLASS:
        v->hasValue = UA_TRUE;
        retval |= UA_Variant_setScalarCopy(&v->value, &node->nodeClass, &UA_TYPES[UA_TYPES_INT32]);
        break;
    case UA_ATTRIBUTEID_BROWSENAME:
        v->hasValue = UA_TRUE;
        retval |= UA_Variant_setScalarCopy(&v->value, &node->browseName, &UA_TYPES[UA_TYPES_QUALIFIEDNAME]);
        break;
    case UA_ATTRIBUTEID_DISPLAYNAME:
        retval |= UA_Variant_setScalarCopy(&v->value, &node->displayName, &UA_TYPES[UA_TYPES_LOCALIZEDTEXT]);
        if(retval == UA_STATUSCODE_GOOD)
            v->hasValue = UA_TRUE;
        break;
    case UA_ATTRIBUTEID_DESCRIPTION:
        v->hasValue = UA_TRUE;
        retval |= UA_Variant_setScalarCopy(&v->value, &node->description, &UA_TYPES[UA_TYPES_LOCALIZEDTEXT]);
        break;
    case UA_ATTRIBUTEID_WRITEMASK:
        v->hasValue = UA_TRUE;
        retval |= UA_Variant_setScalarCopy(&v->value, &node->writeMask, &UA_TYPES[UA_TYPES_UINT32]);
        break;
    case UA_ATTRIBUTEID_USERWRITEMASK:
        v->hasValue = UA_TRUE;
        retval |= UA_Variant_setScalarCopy(&v->value, &node->userWriteMask, &UA_TYPES[UA_TYPES_UINT32]);
        break;
    case UA_ATTRIBUTEID_ISABSTRACT:
        CHECK_NODECLASS(UA_NODECLASS_REFERENCETYPE | UA_NODECLASS_OBJECTTYPE | UA_NODECLASS_VARIABLETYPE |
                        UA_NODECLASS_DATATYPE);
        v->hasValue = UA_TRUE;
        retval |= UA_Variant_setScalarCopy(&v->value, &((const UA_ReferenceTypeNode *)node)->isAbstract,
                                          &UA_TYPES[UA_TYPES_BOOLEAN]);
        break;
    case UA_ATTRIBUTEID_SYMMETRIC:
        CHECK_NODECLASS(UA_NODECLASS_REFERENCETYPE);
        v->hasValue = UA_TRUE;
        retval |= UA_Variant_setScalarCopy(&v->value, &((const UA_ReferenceTypeNode *)node)->symmetric,
                                          &UA_TYPES[UA_TYPES_BOOLEAN]);
        break;
    case UA_ATTRIBUTEID_INVERSENAME:
        CHECK_NODECLASS(UA_NODECLASS_REFERENCETYPE);
        v->hasValue = UA_TRUE;
        retval |= UA_Variant_setScalarCopy(&v->value, &((const UA_ReferenceTypeNode *)node)->inverseName,
                                          &UA_TYPES[UA_TYPES_LOCALIZEDTEXT]);
        break;
    case UA_ATTRIBUTEID_CONTAINSNOLOOPS:
        CHECK_NODECLASS(UA_NODECLASS_VIEW);
        v->hasValue = UA_TRUE;
        retval |= UA_Variant_setScalarCopy(&v->value, &((const UA_ViewNode *)node)->containsNoLoops,
                                          &UA_TYPES[UA_TYPES_BOOLEAN]);
        break;
    case UA_ATTRIBUTEID_EVENTNOTIFIER:
        CHECK_NODECLASS(UA_NODECLASS_VIEW | UA_NODECLASS_OBJECT);
        v->hasValue = UA_TRUE;
        if(node->nodeClass == UA_NODECLASS_VIEW){
        	retval |= UA_Variant_setScalarCopy(&v->value, &((const UA_ViewNode *)node)->eventNotifier,
                                          	  &UA_TYPES[UA_TYPES_BYTE]);
        } else {
        	retval |= UA_Variant_setScalarCopy(&v->value, &((const UA_ObjectNode *)node)->eventNotifier,
                                              &UA_TYPES[UA_TYPES_BYTE]);
        }
        break;

    case UA_ATTRIBUTEID_VALUE:
        CHECK_NODECLASS(UA_NODECLASS_VARIABLE | UA_NODECLASS_VARIABLETYPE);
        {
        	if(node->nodeClass != UA_NODECLASS_VARIABLE) {
    			v->hasValue = UA_FALSE;
    			handleSourceTimestamps(timestamps, v);
            }

            UA_Boolean hasRange = UA_FALSE;
            UA_NumericRange range;
            if(id->indexRange.length > 0) {
                retval = parse_numericrange(id->indexRange, &range);
                if(retval != UA_STATUSCODE_GOOD)
                    break;
                hasRange = UA_TRUE;
            }

            const UA_VariableNode *vn = (const UA_VariableNode*)node;
            if(vn->valueSource == UA_VALUESOURCE_VARIANT) {
                if(hasRange)
                    retval |= UA_Variant_copyRange(&vn->value.variant, &v->value, range);
                else
                    retval |= UA_Variant_copy(&vn->value.variant, &v->value);
                if(retval == UA_STATUSCODE_GOOD) {
                    v->hasValue = UA_TRUE;
                    handleSourceTimestamps(timestamps, v);
                }
            } else {
                UA_DataValue val;
                UA_DataValue_init(&val);
                UA_Boolean sourceTimeStamp = (timestamps == UA_TIMESTAMPSTORETURN_SOURCE ||
                                              timestamps == UA_TIMESTAMPSTORETURN_BOTH);
                if(hasRange)
                    retval |= vn->value.dataSource.read(vn->value.dataSource.handle, sourceTimeStamp, &range, &val);
                else
                    retval |= vn->value.dataSource.read(vn->value.dataSource.handle, sourceTimeStamp, UA_NULL, &val);
                if(retval == UA_STATUSCODE_GOOD) {
                    retval |= UA_DataValue_copy(&val, v); // todo: still too much copying necessary!!
                    vn->value.dataSource.release(vn->value.dataSource.handle, &val);
                }
            }

            if(hasRange)
                UA_free(range.dimensions);
        }
        break;

    case UA_ATTRIBUTEID_DATATYPE: {
		CHECK_NODECLASS(UA_NODECLASS_VARIABLE | UA_NODECLASS_VARIABLETYPE);
        const UA_VariableNode *vn = (const UA_VariableNode*)node;
        if(vn->valueSource == UA_VALUESOURCE_VARIANT)
            retval = UA_Variant_setScalarCopy(&v->value, &vn->value.variant.type->typeId,
                                              &UA_TYPES[UA_TYPES_NODEID]);
        else {
            UA_DataValue val;
            UA_DataValue_init(&val);
            retval = vn->value.dataSource.read(vn->value.dataSource.handle, UA_FALSE, UA_NULL, &val);
            if(retval != UA_STATUSCODE_GOOD)
                break;
            retval = UA_Variant_setScalarCopy(&v->value, &val.value.type->typeId, &UA_TYPES[UA_TYPES_NODEID]);
            vn->value.dataSource.release(vn->value.dataSource.handle, &val);
        }
        if(retval == UA_STATUSCODE_GOOD)
            v->hasValue = UA_TRUE;
        }
        break;

    case UA_ATTRIBUTEID_VALUERANK:
        CHECK_NODECLASS(UA_NODECLASS_VARIABLE | UA_NODECLASS_VARIABLETYPE);
        v->hasValue = UA_TRUE;
        retval |= UA_Variant_setScalarCopy(&v->value, &((const UA_VariableTypeNode *)node)->valueRank,
                                          &UA_TYPES[UA_TYPES_INT32]);
        break;

    case UA_ATTRIBUTEID_ARRAYDIMENSIONS:
        CHECK_NODECLASS(UA_NODECLASS_VARIABLE | UA_NODECLASS_VARIABLETYPE);
        {
            const UA_VariableNode *vn = (const UA_VariableNode *)node;
            if(vn->valueSource == UA_VALUESOURCE_VARIANT) {
                retval = UA_Variant_setArrayCopy(&v->value, vn->value.variant.arrayDimensions,
                                                 vn->value.variant.arrayDimensionsSize,
                                                 &UA_TYPES[UA_TYPES_INT32]);
                if(retval == UA_STATUSCODE_GOOD)
                    v->hasValue = UA_TRUE;
            } else {
                UA_DataValue val;
                UA_DataValue_init(&val);
                retval |= vn->value.dataSource.read(vn->value.dataSource.handle, UA_FALSE, UA_NULL, &val);
                if(retval != UA_STATUSCODE_GOOD)
                    break;
                if(!val.hasValue)
                    retval = UA_STATUSCODE_BADNOTREADABLE;
                else
                    retval = UA_Variant_setArrayCopy(&v->value, val.value.arrayDimensions,
                                                     val.value.arrayDimensionsSize, &UA_TYPES[UA_TYPES_INT32]);
                vn->value.dataSource.release(vn->value.dataSource.handle, &val);
            }
        }
        break;

    case UA_ATTRIBUTEID_ACCESSLEVEL:
        CHECK_NODECLASS(UA_NODECLASS_VARIABLE);
        v->hasValue = UA_TRUE;
        retval |= UA_Variant_setScalarCopy(&v->value, &((const UA_VariableNode *)node)->accessLevel,
                                          &UA_TYPES[UA_TYPES_BYTE]);
        break;

    case UA_ATTRIBUTEID_USERACCESSLEVEL:
        CHECK_NODECLASS(UA_NODECLASS_VARIABLE);
        v->hasValue = UA_TRUE;
        retval |= UA_Variant_setScalarCopy(&v->value, &((const UA_VariableNode *)node)->userAccessLevel,
                                          &UA_TYPES[UA_TYPES_BYTE]);
        break;

    case UA_ATTRIBUTEID_MINIMUMSAMPLINGINTERVAL:
        CHECK_NODECLASS(UA_NODECLASS_VARIABLE);
        v->hasValue = UA_TRUE;
        retval |= UA_Variant_setScalarCopy(&v->value, &((const UA_VariableNode *)node)->minimumSamplingInterval,
                                          &UA_TYPES[UA_TYPES_DOUBLE]);
        break;

    case UA_ATTRIBUTEID_HISTORIZING:
        CHECK_NODECLASS(UA_NODECLASS_VARIABLE);
        v->hasValue = UA_TRUE;
        retval |= UA_Variant_setScalarCopy(&v->value, &((const UA_VariableNode *)node)->historizing,
                                          &UA_TYPES[UA_TYPES_BOOLEAN]);
        break;

    case UA_ATTRIBUTEID_EXECUTABLE:
        CHECK_NODECLASS(UA_NODECLASS_METHOD);
        v->hasValue = UA_TRUE;
        retval |= UA_Variant_setScalarCopy(&v->value, &((const UA_MethodNode *)node)->executable,
                                          &UA_TYPES[UA_TYPES_BOOLEAN]);
        break;

    case UA_ATTRIBUTEID_USEREXECUTABLE:
        CHECK_NODECLASS(UA_NODECLASS_METHOD);
        v->hasValue = UA_TRUE;
        retval |= UA_Variant_setScalarCopy(&v->value, &((const UA_MethodNode *)node)->userExecutable,
                                          &UA_TYPES[UA_TYPES_BOOLEAN]);
        break;

    default:
        v->hasStatus = UA_TRUE;
        v->status = UA_STATUSCODE_BADATTRIBUTEIDINVALID;
        break;
    }

    UA_NodeStore_release(node);

    if(retval != UA_STATUSCODE_GOOD) {
        v->hasStatus = UA_TRUE;
        v->status = retval;
    }

    handleServerTimestamps(timestamps, v);
}

void Service_Read(UA_Server *server, UA_Session *session, const UA_ReadRequest *request,
                  UA_ReadResponse *response) {

    if(request->nodesToReadSize <= 0) {
        response->responseHeader.serviceResult = UA_STATUSCODE_BADNOTHINGTODO;
        return;
    }

    if(request->timestampsToReturn > 3){
    	response->responseHeader.serviceResult = UA_STATUSCODE_BADTIMESTAMPSTORETURNINVALID;
    	return;
    }

    size_t size = request->nodesToReadSize;

    response->results = UA_Array_new(&UA_TYPES[UA_TYPES_DATAVALUE], size);
    if(!response->results) {
        response->responseHeader.serviceResult = UA_STATUSCODE_BADOUTOFMEMORY;
        return;
    }

    response->resultsSize = size;

    if(request->maxAge < 0) {
    	response->responseHeader.serviceResult = UA_STATUSCODE_BADMAXAGEINVALID;
        return;
    }

#ifdef UA_EXTERNAL_NAMESPACES
#ifdef NO_ALLOCA
    UA_Boolean isExternal[size];
    UA_UInt32 indices[size];
#else
    UA_Boolean *isExternal = UA_alloca(sizeof(UA_Boolean) * size);
    UA_UInt32 *indices = UA_alloca(sizeof(UA_UInt32) * size);
#endif /*NO_ALLOCA */
    UA_memset(isExternal, UA_FALSE, sizeof(UA_Boolean) * size);
    for(size_t j = 0;j<server->externalNamespacesSize;j++) {
        size_t indexSize = 0;
        for(size_t i = 0;i < size;i++) {
            if(request->nodesToRead[i].nodeId.namespaceIndex != server->externalNamespaces[j].index)
                continue;
            isExternal[i] = UA_TRUE;
            indices[indexSize] = i;
            indexSize++;
        }
        if(indexSize == 0)
            continue;
        UA_ExternalNodeStore *ens = &server->externalNamespaces[j].externalNodeStore;
        ens->readNodes(ens->ensHandle, &request->requestHeader, request->nodesToRead,
                       indices, indexSize, response->results, UA_FALSE, response->diagnosticInfos);
    }
#endif

    for(size_t i = 0;i < size;i++) {
#ifdef UA_EXTERNAL_NAMESPACES
        if(!isExternal[i])
#endif
            readValue(server, request->timestampsToReturn, &request->nodesToRead[i], &response->results[i]);
    }

#ifdef EXTENSION_STATELESS
    if(session==&anonymousSession){
		/* expiry header */
		UA_ExtensionObject additionalHeader;
		UA_ExtensionObject_init(&additionalHeader);
		additionalHeader.encoding = UA_EXTENSIONOBJECT_ENCODINGMASK_BODYISBYTESTRING;

		UA_Variant variant;
		UA_Variant_init(&variant);
		variant.type = &UA_TYPES[UA_TYPES_DATETIME];
		variant.arrayLength = request->nodesToReadSize;

		UA_DateTime* expireArray = UA_NULL;
		expireArray = UA_Array_new(&UA_TYPES[UA_TYPES_DATETIME], request->nodesToReadSize);
		variant.data = expireArray;

		UA_ByteString str;
		UA_ByteString_init(&str);

		/*expires in 20 seconds*/
		for(UA_Int32 i = 0;i < response->resultsSize;i++) {
			expireArray[i] = UA_DateTime_now() + 20 * 100 * 1000 * 1000;
		}
		size_t offset = 0;
		str.data = UA_malloc(UA_Variant_calcSizeBinary(&variant));
		str.length = UA_Variant_calcSizeBinary(&variant);
		UA_Variant_encodeBinary(&variant, &str, &offset);
		additionalHeader.body = str;
		response->responseHeader.additionalHeader = additionalHeader;
    }
#endif
}

static UA_StatusCode writeValue(UA_Server *server, UA_WriteValue *wvalue) {
    UA_StatusCode retval = UA_STATUSCODE_GOOD;

    /* is there a value at all */
    if(!wvalue->value.hasValue)
        return UA_STATUSCODE_BADTYPEMISMATCH;

    // we might repeat writing, e.g. when the node got replaced mid-work
    UA_Boolean done = UA_FALSE;
    while(!done) {
        const UA_Node *node = UA_NodeStore_get(server->nodestore, &wvalue->nodeId);
        if(!node)
            return UA_STATUSCODE_BADNODEIDUNKNOWN;

        switch(wvalue->attributeId) {
        case UA_ATTRIBUTEID_NODEID:
        case UA_ATTRIBUTEID_NODECLASS:
        case UA_ATTRIBUTEID_BROWSENAME:
        case UA_ATTRIBUTEID_DISPLAYNAME:
        case UA_ATTRIBUTEID_DESCRIPTION:
        case UA_ATTRIBUTEID_WRITEMASK:
        case UA_ATTRIBUTEID_USERWRITEMASK:
        case UA_ATTRIBUTEID_ISABSTRACT:
        case UA_ATTRIBUTEID_SYMMETRIC:
        case UA_ATTRIBUTEID_INVERSENAME:
        case UA_ATTRIBUTEID_CONTAINSNOLOOPS:
        case UA_ATTRIBUTEID_EVENTNOTIFIER:
            retval = UA_STATUSCODE_BADWRITENOTSUPPORTED;
            break;
        case UA_ATTRIBUTEID_VALUE: {
            if(node->nodeClass != UA_NODECLASS_VARIABLE &&
               node->nodeClass != UA_NODECLASS_VARIABLETYPE) {
                retval = UA_STATUSCODE_BADTYPEMISMATCH;
                break;
            }

            /* parse the range */
            UA_Boolean hasRange = UA_FALSE;
            UA_NumericRange range;
            if(wvalue->indexRange.length > 0) {
                retval = parse_numericrange(wvalue->indexRange, &range);
                if(retval != UA_STATUSCODE_GOOD)
                    break;
                hasRange = UA_TRUE;
            }

            /* the relevant members are similar for variables and variabletypes */
            const UA_VariableNode *vn = (const UA_VariableNode*)node;
            if(vn->valueSource == UA_VALUESOURCE_DATASOURCE) {
                if(!vn->value.dataSource.write) {
                    retval = UA_STATUSCODE_BADWRITENOTSUPPORTED;
                    goto clean_up_range;
                }
                // todo: writing ranges
                if(hasRange)
                    retval = vn->value.dataSource.write(vn->value.dataSource.handle, &wvalue->value.value, &range);
                else
                    retval = vn->value.dataSource.write(vn->value.dataSource.handle, &wvalue->value.value, UA_NULL);
                done = UA_TRUE;
                goto clean_up_range;
            }
            const UA_Variant *oldV = &vn->value.variant;

            /* the nodeid on the wire may be != the nodeid in the node: opaque types, enums and bytestrings */
            if(!UA_NodeId_equal(&oldV->type->typeId, &wvalue->value.value.type->typeId)) {
                if(oldV->type->namespaceZero && wvalue->value.value.type->namespaceZero &&
                   oldV->type->typeIndex == wvalue->value.value.type->typeIndex)
                    /* An enum was sent as an int32, or an opaque type as a bytestring. This is
                       detected with the typeIndex indicated the "true" datatype. */
                    wvalue->value.value.type = oldV->type;
                else if(oldV->type == &UA_TYPES[UA_TYPES_BYTE] && !UA_Variant_isScalar(oldV) &&
                        wvalue->value.value.type == &UA_TYPES[UA_TYPES_BYTESTRING] &&
                        UA_Variant_isScalar(&wvalue->value.value)) {
                    /* a string is written to a byte array */
                    UA_ByteString *str = (UA_ByteString*) wvalue->value.value.data;
                    wvalue->value.value.arrayLength = str->length;
                    wvalue->value.value.data = str->data;
                    wvalue->value.value.type = &UA_TYPES[UA_TYPES_BYTE];
                    UA_free(str);
                } else {
                    retval = UA_STATUSCODE_BADTYPEMISMATCH;
                    goto clean_up_range;
                }
            }

            /* copy the node */
            UA_VariableNode *newVn = (node->nodeClass == UA_NODECLASS_VARIABLE) ?
                UA_VariableNode_new() : (UA_VariableNode*)UA_VariableTypeNode_new();
            if(!newVn) {
                retval = UA_STATUSCODE_BADOUTOFMEMORY;
                goto clean_up_range;
            }
            retval = (node->nodeClass == UA_NODECLASS_VARIABLE) ? UA_VariableNode_copy(vn, newVn) : 
                UA_VariableTypeNode_copy((const UA_VariableTypeNode*)vn, (UA_VariableTypeNode*)newVn);
            if(retval != UA_STATUSCODE_GOOD)
                goto clean_up;
                
            /* insert the new value */
            if(hasRange)
                retval = UA_Variant_setRangeCopy(&newVn->value.variant, wvalue->value.value.data,
                                                 wvalue->value.value.arrayLength, range);
            else {
                UA_Variant_deleteMembers(&newVn->value.variant);
                retval = UA_Variant_copy(&wvalue->value.value, &newVn->value.variant);
            }

            if(retval == UA_STATUSCODE_GOOD && UA_NodeStore_replace(server->nodestore, node,
                                                   (UA_Node*)newVn, UA_NULL) == UA_STATUSCODE_GOOD) {
                done = UA_TRUE;
                goto clean_up_range;
            }

            clean_up:
            if(node->nodeClass == UA_NODECLASS_VARIABLE)
                UA_VariableNode_delete(newVn);
            else
                UA_VariableTypeNode_delete((UA_VariableTypeNode*)newVn);
            clean_up_range:
            if(hasRange)
                UA_free(range.dimensions);
            }
            break;
        case UA_ATTRIBUTEID_DATATYPE:
        case UA_ATTRIBUTEID_VALUERANK:
        case UA_ATTRIBUTEID_ARRAYDIMENSIONS:
        case UA_ATTRIBUTEID_ACCESSLEVEL:
        case UA_ATTRIBUTEID_USERACCESSLEVEL:
        case UA_ATTRIBUTEID_MINIMUMSAMPLINGINTERVAL:
        case UA_ATTRIBUTEID_HISTORIZING:
        case UA_ATTRIBUTEID_EXECUTABLE:
        case UA_ATTRIBUTEID_USEREXECUTABLE:
            retval = UA_STATUSCODE_BADWRITENOTSUPPORTED;
            break;
        default:
            retval = UA_STATUSCODE_BADATTRIBUTEIDINVALID;
            break;
        }

        UA_NodeStore_release(node);
        if(retval != UA_STATUSCODE_GOOD)
            break;
    }

    return retval;
}

void Service_Write(UA_Server *server, UA_Session *session, const UA_WriteRequest *request,
                   UA_WriteResponse *response) {
    UA_assert(server != UA_NULL && session != UA_NULL && request != UA_NULL && response != UA_NULL);

    if(request->nodesToWriteSize <= 0){
        response->responseHeader.serviceResult = UA_STATUSCODE_BADNOTHINGTODO;
        return;
    }

    response->results = UA_Array_new(&UA_TYPES[UA_TYPES_STATUSCODE], request->nodesToWriteSize);
    if(!response->results) {
        response->responseHeader.serviceResult = UA_STATUSCODE_BADOUTOFMEMORY;
        return;
    }

#ifdef UA_EXTERNAL_NAMESPACES
#ifdef NO_ALLOCA
    UA_Boolean isExternal[request->nodesToWriteSize];
    UA_UInt32 indices[request->nodesToWriteSize];
#else
    UA_Boolean *isExternal = UA_alloca(sizeof(UA_Boolean) * request->nodesToWriteSize);
    UA_UInt32 *indices = UA_alloca(sizeof(UA_UInt32) * request->nodesToWriteSize);
#endif /*NO_ALLOCA */
    UA_memset(isExternal, UA_FALSE, sizeof(UA_Boolean)*request->nodesToWriteSize);
    for(size_t j = 0; j < server->externalNamespacesSize; j++) {
        UA_UInt32 indexSize = 0;
        for(UA_Int32 i = 0; i < request->nodesToWriteSize; i++) {
            if(request->nodesToWrite[i].nodeId.namespaceIndex !=
               server->externalNamespaces[j].index)
                continue;
            isExternal[i] = UA_TRUE;
            indices[indexSize] = i;
            indexSize++;
        }
        if(indexSize == 0)
            continue;
        UA_ExternalNodeStore *ens = &server->externalNamespaces[j].externalNodeStore;
        ens->writeNodes(ens->ensHandle, &request->requestHeader, request->nodesToWrite,
                        indices, indexSize, response->results, response->diagnosticInfos);
    }
#endif
    
    response->resultsSize = request->nodesToWriteSize;
    for(UA_Int32 i = 0;i < request->nodesToWriteSize;i++) {
#ifdef UA_EXTERNAL_NAMESPACES
        if(!isExternal[i])
#endif
            response->results[i] = writeValue(server, &request->nodesToWrite[i]);
    }
}
/*********************************** amalgamated original file "C:/AcpltDevelopmentKit/acplt/dev/open62541WrapperProject/open62541/src/server/ua_services_nodemanagement.c" ***********************************/

#define COPY_STANDARDATTRIBUTES do {                                    \
    if(attr.specifiedAttributes & UA_NODEATTRIBUTESMASK_DISPLAYNAME) {  \
        vnode->displayName = attr.displayName;                          \
        UA_LocalizedText_init(&attr.displayName);                       \
    }                                                                   \
    if(attr.specifiedAttributes & UA_NODEATTRIBUTESMASK_DESCRIPTION) {  \
        vnode->description = attr.description;                          \
        UA_LocalizedText_init(&attr.description);                       \
    }                                                                   \
    if(attr.specifiedAttributes & UA_NODEATTRIBUTESMASK_WRITEMASK)      \
        vnode->writeMask = attr.writeMask;                              \
    if(attr.specifiedAttributes & UA_NODEATTRIBUTESMASK_USERWRITEMASK)  \
        vnode->userWriteMask = attr.userWriteMask;                      \
    } while(0)

static UA_StatusCode parseVariableNode(UA_ExtensionObject *attributes, UA_Node **new_node) {
    if(attributes->typeId.identifier.numeric !=
       UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES].typeId.identifier.numeric + UA_ENCODINGOFFSET_BINARY)
        return UA_STATUSCODE_BADNODEATTRIBUTESINVALID;

    UA_VariableAttributes attr;
    size_t pos = 0;
    // todo return more informative error codes from decodeBinary
    if(UA_VariableAttributes_decodeBinary(&attributes->body, &pos, &attr) != UA_STATUSCODE_GOOD)
        return UA_STATUSCODE_BADNODEATTRIBUTESINVALID;

    UA_VariableNode *vnode = UA_VariableNode_new();
    if(!vnode) {
        UA_VariableAttributes_deleteMembers(&attr);
        return UA_STATUSCODE_BADOUTOFMEMORY;
    }

    // now copy all the attributes. This potentially removes them from the decoded attributes.
    COPY_STANDARDATTRIBUTES;
    if(attr.specifiedAttributes & UA_NODEATTRIBUTESMASK_ACCESSLEVEL)
        vnode->accessLevel = attr.accessLevel;
    if(attr.specifiedAttributes & UA_NODEATTRIBUTESMASK_USERACCESSLEVEL)
        vnode->userAccessLevel = attr.userAccessLevel;
    if(attr.specifiedAttributes & UA_NODEATTRIBUTESMASK_HISTORIZING)
        vnode->historizing = attr.historizing;
    if(attr.specifiedAttributes & UA_NODEATTRIBUTESMASK_MINIMUMSAMPLINGINTERVAL)
        vnode->minimumSamplingInterval = attr.minimumSamplingInterval;
    if(attr.specifiedAttributes & UA_NODEATTRIBUTESMASK_VALUERANK)
        vnode->valueRank = attr.valueRank;

    // don't use extra dimension spec. This comes from the value.
    /* if(attr.specifiedAttributes & UA_NODEATTRIBUTESMASK_ARRAYDIMENSIONS) { */
    /*     vnode->arrayDimensionsSize = attr.arrayDimensionsSize; */
    /*     vnode->arrayDimensions = attr.arrayDimensions; */
    /*     attr.arrayDimensionsSize = -1; */
    /*     attr.arrayDimensions = UA_NULL; */
    /* } */

    // don't use the extra type id. This comes from the value.
    /* if(attr.specifiedAttributes & UA_NODEATTRIBUTESMASK_DATATYPE || */
    /*    attr.specifiedAttributes & UA_NODEATTRIBUTESMASK_OBJECTTYPEORDATATYPE) { */
    /*     vnode->dataType = attr.dataType; */
    /*     UA_NodeId_init(&attr.dataType); */
    /* } */

    if(attr.specifiedAttributes & UA_NODEATTRIBUTESMASK_VALUE) {
        vnode->value.variant = attr.value;
        UA_Variant_init(&attr.value);
    }

    UA_VariableAttributes_deleteMembers(&attr);

    *new_node = (UA_Node*)vnode;
    return UA_STATUSCODE_GOOD;
}

static UA_StatusCode parseObjectNode(UA_ExtensionObject *attributes, UA_Node **new_node) {
    if(attributes->typeId.identifier.numeric !=
       UA_TYPES[UA_TYPES_OBJECTATTRIBUTES].typeId.identifier.numeric + UA_ENCODINGOFFSET_BINARY)
        return UA_STATUSCODE_BADNODEATTRIBUTESINVALID;

    UA_ObjectAttributes attr;
    size_t pos = 0;
    // todo return more informative error codes from decodeBinary
    if (UA_ObjectAttributes_decodeBinary(&attributes->body, &pos, &attr) != UA_STATUSCODE_GOOD)
        return UA_STATUSCODE_BADNODEATTRIBUTESINVALID;
    UA_ObjectNode *vnode = UA_ObjectNode_new();
    if(!vnode) {
        UA_ObjectAttributes_deleteMembers(&attr);
        return UA_STATUSCODE_BADOUTOFMEMORY;
    }

    // now copy all the attributes. This potentially removes them from the decoded attributes.
    COPY_STANDARDATTRIBUTES;
    if(attr.specifiedAttributes & UA_NODEATTRIBUTESMASK_EVENTNOTIFIER)
        vnode->eventNotifier = attr.eventNotifier;
    UA_ObjectAttributes_deleteMembers(&attr);
    *new_node = (UA_Node*) vnode;
    return UA_STATUSCODE_GOOD;
}

static UA_StatusCode parseReferenceTypeNode(UA_ExtensionObject *attributes, UA_Node **new_node) {
    UA_ReferenceTypeAttributes attr;
    size_t pos = 0;
    // todo return more informative error codes from decodeBinary
    if(UA_ReferenceTypeAttributes_decodeBinary(&attributes->body, &pos, &attr) != UA_STATUSCODE_GOOD)
        return UA_STATUSCODE_BADNODEATTRIBUTESINVALID;
    UA_ReferenceTypeNode *vnode = UA_ReferenceTypeNode_new();
    if(!vnode) {
        UA_ReferenceTypeAttributes_deleteMembers(&attr);
        return UA_STATUSCODE_BADOUTOFMEMORY;
    }

    // now copy all the attributes. This potentially removes them from the decoded attributes.
    COPY_STANDARDATTRIBUTES;
    if(attr.specifiedAttributes & UA_NODEATTRIBUTESMASK_ISABSTRACT)
        vnode->isAbstract = attr.isAbstract;
    if(attr.specifiedAttributes & UA_NODEATTRIBUTESMASK_SYMMETRIC)
        vnode->symmetric = attr.symmetric;
    if(attr.specifiedAttributes & UA_NODEATTRIBUTESMASK_INVERSENAME) {
        vnode->inverseName = attr.inverseName;
        attr.inverseName.text.length = -1;
        attr.inverseName.text.data = UA_NULL;
        attr.inverseName.locale.length = -1;
        attr.inverseName.locale.data = UA_NULL;
    }
    UA_ReferenceTypeAttributes_deleteMembers(&attr);
    *new_node = (UA_Node*) vnode;
    return UA_STATUSCODE_GOOD;
}

static UA_StatusCode parseObjectTypeNode(UA_ExtensionObject *attributes, UA_Node **new_node) {
    UA_ObjectTypeAttributes attr;
    size_t pos = 0;
    // todo return more informative error codes from decodeBinary
    if(UA_ObjectTypeAttributes_decodeBinary(&attributes->body, &pos, &attr) != UA_STATUSCODE_GOOD)
        return UA_STATUSCODE_BADNODEATTRIBUTESINVALID;
    UA_ObjectTypeNode *vnode = UA_ObjectTypeNode_new();
    if(!vnode) {
        UA_ObjectTypeAttributes_deleteMembers(&attr);
        return UA_STATUSCODE_BADOUTOFMEMORY;
    }
    
    // now copy all the attributes. This potentially removes them from the decoded attributes.
    COPY_STANDARDATTRIBUTES;
    if(attr.specifiedAttributes & UA_NODEATTRIBUTESMASK_ISABSTRACT) {
        vnode->isAbstract = attr.isAbstract;
    }
    UA_ObjectTypeAttributes_deleteMembers(&attr);
    *new_node = (UA_Node*) vnode;
    return UA_STATUSCODE_GOOD;
}

static UA_StatusCode parseViewNode(UA_ExtensionObject *attributes, UA_Node **new_node) {
    UA_ViewAttributes attr;
    size_t pos = 0;

    // todo return more informative error codes from decodeBinary
    if(UA_ViewAttributes_decodeBinary(&attributes->body, &pos, &attr) != UA_STATUSCODE_GOOD)
        return UA_STATUSCODE_BADNODEATTRIBUTESINVALID;
    UA_ViewNode *vnode = UA_ViewNode_new();
    if(!vnode) {
        UA_ViewAttributes_deleteMembers(&attr);
        return UA_STATUSCODE_BADOUTOFMEMORY;
    }

    // now copy all the attributes. This potentially removes them from the decoded attributes.
    COPY_STANDARDATTRIBUTES;
    if(attr.specifiedAttributes & UA_NODEATTRIBUTESMASK_CONTAINSNOLOOPS)
        vnode->containsNoLoops = attr.containsNoLoops;
    if(attr.specifiedAttributes & UA_NODEATTRIBUTESMASK_EVENTNOTIFIER)
        vnode->eventNotifier = attr.eventNotifier;
    UA_ViewAttributes_deleteMembers(&attr);
    *new_node = (UA_Node*) vnode;
    return UA_STATUSCODE_GOOD;
}

static void addNodeFromAttributes(UA_Server *server, UA_Session *session, UA_AddNodesItem *item,
                                  UA_AddNodesResult *result) {
    // adding nodes to ns0 is not allowed over the wire
    if(item->requestedNewNodeId.nodeId.namespaceIndex == 0) {
        result->statusCode = UA_STATUSCODE_BADNODEIDREJECTED;
        return;
    }

    // parse the node
    UA_Node *node = UA_NULL;

    switch (item->nodeClass) {
    case UA_NODECLASS_OBJECT:
        result->statusCode = parseObjectNode(&item->nodeAttributes, &node);
        break;
    case UA_NODECLASS_OBJECTTYPE:
        result->statusCode = parseObjectTypeNode(&item->nodeAttributes, &node);
        break;
    case UA_NODECLASS_REFERENCETYPE:
        result->statusCode = parseReferenceTypeNode(&item->nodeAttributes, &node);
        break;
    case UA_NODECLASS_VARIABLE:
        result->statusCode = parseVariableNode(&item->nodeAttributes, &node);
        break;
    default:
        result->statusCode = UA_STATUSCODE_BADNOTIMPLEMENTED;
    }

    if(result->statusCode != UA_STATUSCODE_GOOD)
        return;

    // add the node
    *result = UA_Server_addNodeWithSession(server, session, node, item->parentNodeId,
                                           item->referenceTypeId);
    if(result->statusCode != UA_STATUSCODE_GOOD) {
        switch (node->nodeClass) {
        case UA_NODECLASS_OBJECT:
            UA_ObjectNode_delete((UA_ObjectNode*)node);
            break;
        case UA_NODECLASS_OBJECTTYPE:
            UA_ObjectTypeNode_delete((UA_ObjectTypeNode*)node);
            break;
        case UA_NODECLASS_REFERENCETYPE:
            UA_ReferenceTypeNode_delete((UA_ReferenceTypeNode*)node);
            break;
        case UA_NODECLASS_VARIABLE:
            UA_VariableNode_delete((UA_VariableNode*)node);
            break;
        default:
            UA_assert(UA_FALSE);
        }
    }
}

void Service_AddNodes(UA_Server *server, UA_Session *session, const UA_AddNodesRequest *request,
                      UA_AddNodesResponse *response) {
    if(request->nodesToAddSize <= 0) {
        response->responseHeader.serviceResult = UA_STATUSCODE_BADNOTHINGTODO;
        return;
    }
    size_t size = request->nodesToAddSize;

    response->results = UA_Array_new(&UA_TYPES[UA_TYPES_ADDNODESRESULT], size);
    if(!response->results) {
        response->responseHeader.serviceResult = UA_STATUSCODE_BADOUTOFMEMORY;
        return;
    }

#ifdef UA_EXTERNAL_NAMESPACES
#ifdef NO_ALLOCA
    UA_Boolean isExternal[size];
    UA_UInt32 indices[size];
#else
    UA_Boolean *isExternal = UA_alloca(sizeof(UA_Boolean) * size);
    UA_UInt32 *indices = UA_alloca(sizeof(UA_UInt32) * size);
#endif /*NO_ALLOCA */
    UA_memset(isExternal, UA_FALSE, sizeof(UA_Boolean) * size);
    for(size_t j = 0; j <server->externalNamespacesSize; j++) {
        size_t indexSize = 0;
        for(size_t i = 0;i < size;i++) {
            if(request->nodesToAdd[i].requestedNewNodeId.nodeId.namespaceIndex !=
               server->externalNamespaces[j].index)
                continue;
            isExternal[i] = UA_TRUE;
            indices[indexSize] = i;
            indexSize++;
        }
        if(indexSize == 0)
            continue;
        UA_ExternalNodeStore *ens = &server->externalNamespaces[j].externalNodeStore;
        ens->addNodes(ens->ensHandle, &request->requestHeader, request->nodesToAdd,
                      indices, indexSize, response->results, response->diagnosticInfos);
    }
#endif
    
    response->resultsSize = size;
    for(size_t i = 0;i < size;i++) {
#ifdef UA_EXTERNAL_NAMESPACES
        if(!isExternal[i])
#endif
            addNodeFromAttributes(server, session, &request->nodesToAdd[i], &response->results[i]);
    }
}

void Service_AddReferences(UA_Server *server, UA_Session *session, const UA_AddReferencesRequest *request,
                           UA_AddReferencesResponse *response) {
	if(request->referencesToAddSize <= 0) {
		response->responseHeader.serviceResult = UA_STATUSCODE_BADNOTHINGTODO;
		return;
	}
    size_t size = request->referencesToAddSize;
	
    if(!(response->results = UA_malloc(sizeof(UA_StatusCode) * size))) {
		response->responseHeader.serviceResult = UA_STATUSCODE_BADOUTOFMEMORY;
		return;
	}
	response->resultsSize = size;
	UA_memset(response->results, UA_STATUSCODE_GOOD, sizeof(UA_StatusCode) * size);

#ifdef UA_EXTERNAL_NAMESPACES
#ifdef NO_ALLOCA
    UA_Boolean isExternal[size];
    UA_UInt32 indices[size];
#else
    UA_Boolean *isExternal = UA_alloca(sizeof(UA_Boolean) * size);
    UA_UInt32 *indices = UA_alloca(sizeof(UA_UInt32) * size);
#endif /*NO_ALLOCA */
    UA_memset(isExternal, UA_FALSE, sizeof(UA_Boolean) * size);
	for(size_t j = 0; j < server->externalNamespacesSize; j++) {
		size_t indicesSize = 0;
		for(size_t i = 0;i < size;i++) {
			if(request->referencesToAdd[i].sourceNodeId.namespaceIndex
					!= server->externalNamespaces[j].index)
				continue;
			isExternal[i] = UA_TRUE;
			indices[indicesSize] = i;
			indicesSize++;
		}
		if (indicesSize == 0)
			continue;
		UA_ExternalNodeStore *ens = &server->externalNamespaces[j].externalNodeStore;
		ens->addReferences(ens->ensHandle, &request->requestHeader, request->referencesToAdd,
                           indices, indicesSize, response->results, response->diagnosticInfos);
	}
#endif

	response->resultsSize = size;
	for(UA_Int32 i = 0; i < response->resultsSize; i++) {
#ifdef UA_EXTERNAL_NAMESPACES
		if(!isExternal[i])
#endif
			UA_Server_addReference(server, &request->referencesToAdd[i]);
	}
}

void Service_DeleteNodes(UA_Server *server, UA_Session *session, const UA_DeleteNodesRequest *request,
                         UA_DeleteNodesResponse *response) {

}

void Service_DeleteReferences(UA_Server *server, UA_Session *session, const UA_DeleteReferencesRequest *request,
                              UA_DeleteReferencesResponse *response) {

}
/*********************************** amalgamated original file "C:/AcpltDevelopmentKit/acplt/dev/open62541WrapperProject/open62541/src/server/ua_services_view.c" ***********************************/

static UA_StatusCode fillrefdescr(UA_NodeStore *ns, const UA_Node *curr, UA_ReferenceNode *ref,
                                  UA_UInt32 mask, UA_ReferenceDescription *descr)
{
    UA_StatusCode retval = UA_STATUSCODE_GOOD;
    UA_ReferenceDescription_init(descr);
    retval |= UA_NodeId_copy(&curr->nodeId, &descr->nodeId.nodeId);
    //TODO: ExpandedNodeId is mocked up
    descr->nodeId.serverIndex = 0;
    descr->nodeId.namespaceUri.length = -1;

    if(mask & UA_BROWSERESULTMASK_REFERENCETYPEID)
        retval |= UA_NodeId_copy(&ref->referenceTypeId, &descr->referenceTypeId);
    if(mask & UA_BROWSERESULTMASK_ISFORWARD)
        descr->isForward = !ref->isInverse;
    if(mask & UA_BROWSERESULTMASK_NODECLASS)
        retval |= UA_NodeClass_copy(&curr->nodeClass, &descr->nodeClass);
    if(mask & UA_BROWSERESULTMASK_BROWSENAME)
        retval |= UA_QualifiedName_copy(&curr->browseName, &descr->browseName);
    if(mask & UA_BROWSERESULTMASK_DISPLAYNAME)
        retval |= UA_LocalizedText_copy(&curr->displayName, &descr->displayName);
    if(mask & UA_BROWSERESULTMASK_TYPEDEFINITION){
        if(curr->nodeClass == UA_NODECLASS_OBJECT || curr->nodeClass == UA_NODECLASS_VARIABLE) {
            for(UA_Int32 i = 0; i < curr->referencesSize; i++) {
                UA_ReferenceNode *refnode = &curr->references[i];
                if(refnode->referenceTypeId.identifier.numeric != UA_NS0ID_HASTYPEDEFINITION)
                    continue;
                retval |= UA_ExpandedNodeId_copy(&refnode->targetId, &descr->typeDefinition);
                break;
            }
        }
    }

    if(retval)
        UA_ReferenceDescription_deleteMembers(descr);
    return retval;
}

/* Tests if the node is relevant to the browse request and shall be returned. If
   so, it is retrieved from the Nodestore. If not, null is returned. */
static const UA_Node *relevant_node(UA_NodeStore *ns, const UA_BrowseDescription *descr,
                                    UA_Boolean return_all, UA_ReferenceNode *reference,
                                    UA_NodeId *relevant, size_t relevant_count)
{
    if(reference->isInverse == UA_TRUE && descr->browseDirection == UA_BROWSEDIRECTION_FORWARD)
        return UA_NULL;
    else if(reference->isInverse == UA_FALSE && descr->browseDirection == UA_BROWSEDIRECTION_INVERSE)
        return UA_NULL;

    if(!return_all) {
        for(size_t i = 0; i < relevant_count; i++) {
            if(UA_NodeId_equal(&reference->referenceTypeId, &relevant[i]))
                goto is_relevant;
        }
        return UA_NULL;
    }
is_relevant: ;
    const UA_Node *node = UA_NodeStore_get(ns, &reference->targetId.nodeId);
    if(node && descr->nodeClassMask != 0 && (node->nodeClass & descr->nodeClassMask) == 0) {
        UA_NodeStore_release(node);
        return UA_NULL;
    }
    return node;
}

/**
 * We find all subtypes by a single iteration over the array. We start with an array with a single
 * root nodeid at the beginning. When we find relevant references, we add the nodeids to the back of
 * the array and increase the size. Since the hierarchy is not cyclic, we can safely progress in the
 * array to process the newly found referencetype nodeids (emulated recursion).
 */
static UA_StatusCode findsubtypes(UA_NodeStore *ns, const UA_NodeId *root, UA_NodeId **reftypes,
                                  size_t *reftypes_count) {
    const UA_Node *node = UA_NodeStore_get(ns, root);
    if(!node)
        return UA_STATUSCODE_BADNOMATCH;
    if(node->nodeClass != UA_NODECLASS_REFERENCETYPE)  {
        UA_NodeStore_release(node);
        return UA_STATUSCODE_BADREFERENCETYPEIDINVALID;
    }
    UA_NodeStore_release(node);

    size_t results_size = 20; // probably too big, but saves mallocs
    UA_NodeId *results = UA_malloc(sizeof(UA_NodeId) * results_size);
    if(!results)
        return UA_STATUSCODE_BADOUTOFMEMORY;

    UA_StatusCode retval = UA_NodeId_copy(root, &results[0]);
    if(retval != UA_STATUSCODE_GOOD) {
        UA_free(results);
        return retval;
    }
        
    size_t index = 0; // where are we currently in the array?
    size_t last = 0; // where is the last element in the array?
    do {
        node = UA_NodeStore_get(ns, &results[index]);
        if(!node || node->nodeClass != UA_NODECLASS_REFERENCETYPE)
            continue;
        for(UA_Int32 i = 0; i < node->referencesSize; i++) {
            if(node->references[i].referenceTypeId.identifier.numeric != UA_NS0ID_HASSUBTYPE ||
               node->references[i].isInverse == UA_TRUE)
                continue;

            if(++last >= results_size) { // is the array big enough?
                UA_NodeId *new_results = UA_realloc(results, sizeof(UA_NodeId) * results_size * 2);
                if(!new_results) {
                    retval = UA_STATUSCODE_BADOUTOFMEMORY;
                    break;
                }
                results = new_results;
                results_size *= 2;
            }

            retval = UA_NodeId_copy(&node->references[i].targetId.nodeId, &results[last]);
            if(retval != UA_STATUSCODE_GOOD) {
                last--; // for array_delete
                break;
            }
        }
        UA_NodeStore_release(node);
    } while(++index <= last && retval == UA_STATUSCODE_GOOD);

    if(retval) {
        UA_Array_delete(results, &UA_TYPES[UA_TYPES_NODEID], last);
        return retval;
    }

    *reftypes = results;
    *reftypes_count = last + 1;
    return UA_STATUSCODE_GOOD;
}

static void removeCp(struct ContinuationPointEntry *cp, UA_Session* session){
    session->availableContinuationPoints++;
    UA_ByteString_deleteMembers(&cp->identifier);
    UA_BrowseDescription_deleteMembers(&cp->browseDescription);
    LIST_REMOVE(cp, pointers);
    UA_free(cp);
}

/**
 * Results for a single browsedescription. This is the inner loop for both Browse and BrowseNext
 * @param session Session to save continuationpoints
 * @param ns The nodstore where the to-be-browsed node can be found
 * @param cp If cp points to a continuationpoint, we continue from there.
 *           If cp is null, we can add a new continuation point if possible and necessary.
 * @param descr If no cp is set, we take the browsedescription from there
 * @param maxrefs The maximum number of references the client has requested
 * @param result The entry in the request
 */
static void browse(UA_Session *session, UA_NodeStore *ns, struct ContinuationPointEntry *cp,
                   const UA_BrowseDescription *descr, UA_UInt32 maxrefs, UA_BrowseResult *result) {
    UA_UInt32 continuationIndex = 0;
    size_t referencesCount = 0;
    UA_Int32 referencesIndex = 0;
    
    /* set the browsedescription if a cp is given */
    if(cp) {
        descr = &cp->browseDescription;
        maxrefs = cp->maxReferences;
        continuationIndex = cp->continuationIndex;
    }

    /* is the browsedirection valid? */
    if(descr->browseDirection != UA_BROWSEDIRECTION_BOTH &&
       descr->browseDirection != UA_BROWSEDIRECTION_FORWARD &&
       descr->browseDirection != UA_BROWSEDIRECTION_INVERSE) {
        result->statusCode = UA_STATUSCODE_BADBROWSEDIRECTIONINVALID;
        return;
    }
    
    /* get the references that match the browsedescription */
    size_t relevant_refs_size = 0;
    UA_NodeId *relevant_refs = UA_NULL;
    UA_Boolean all_refs = UA_NodeId_isNull(&descr->referenceTypeId);
    if(!all_refs) {
        if(descr->includeSubtypes) {
            result->statusCode = findsubtypes(ns, &descr->referenceTypeId, &relevant_refs, &relevant_refs_size);
            if(result->statusCode != UA_STATUSCODE_GOOD)
                return;
        } else {
            const UA_Node *rootRef = UA_NodeStore_get(ns, &descr->referenceTypeId);
            if(!rootRef) {
                result->statusCode = UA_STATUSCODE_BADREFERENCETYPEIDINVALID;
                return;
            } else if(rootRef->nodeClass != UA_NODECLASS_REFERENCETYPE) {
                UA_NodeStore_release(rootRef);
                result->statusCode = UA_STATUSCODE_BADREFERENCETYPEIDINVALID;
                return;
            }
            UA_NodeStore_release(rootRef);
            relevant_refs = (UA_NodeId*)(uintptr_t)&descr->referenceTypeId;
            relevant_refs_size = 1;
        }
    }

    /* get the node */
    const UA_Node *node = UA_NodeStore_get(ns, &descr->nodeId);
    if(!node) {
        result->statusCode = UA_STATUSCODE_BADNODEIDUNKNOWN;
        if(!all_refs && descr->includeSubtypes)
            UA_Array_delete(relevant_refs, &UA_TYPES[UA_TYPES_NODEID], relevant_refs_size);
        return;
    }

    /* if the node has no references, just return */
    if(node->referencesSize <= 0) {
        result->referencesSize = 0;
        UA_NodeStore_release(node);
        if(!all_refs && descr->includeSubtypes)
            UA_Array_delete(relevant_refs, &UA_TYPES[UA_TYPES_NODEID], relevant_refs_size);
        return;
    }

    /* how many references can we return at most? */
    UA_UInt32 real_maxrefs = maxrefs;
    if(real_maxrefs == 0)
        real_maxrefs = node->referencesSize;
    result->references = UA_malloc(sizeof(UA_ReferenceDescription) * real_maxrefs);
    if(!result->references) {
        result->statusCode = UA_STATUSCODE_BADOUTOFMEMORY;
        goto cleanup;
    }

    /* loop over the node's references */
    size_t skipped = 0;
    for(; referencesIndex < node->referencesSize && referencesCount < real_maxrefs; referencesIndex++) {
        const UA_Node *current = relevant_node(ns, descr, all_refs, &node->references[referencesIndex],
                                               relevant_refs, relevant_refs_size);
        if(!current)
            continue;
        if(skipped < continuationIndex) {
            UA_NodeStore_release(current);
            skipped++;
            continue;
        }
        UA_StatusCode retval = fillrefdescr(ns, current, &node->references[referencesIndex], descr->resultMask,
                                            &result->references[referencesCount]);
        UA_NodeStore_release(current);
        if(retval != UA_STATUSCODE_GOOD) {
            UA_Array_delete(result->references, &UA_TYPES[UA_TYPES_REFERENCEDESCRIPTION], referencesCount);
            result->references = UA_NULL;
            result->referencesSize = 0;
            result->statusCode = UA_STATUSCODE_UNCERTAINNOTALLNODESAVAILABLE;
            goto cleanup;
        }
        referencesCount++;
    }

    result->referencesSize = referencesCount;
    if(referencesCount == 0) {
        UA_free(result->references);
        result->references = UA_NULL;
    }

    cleanup:
    UA_NodeStore_release(node);
    if(!all_refs && descr->includeSubtypes)
        UA_Array_delete(relevant_refs, &UA_TYPES[UA_TYPES_NODEID], relevant_refs_size);
    if(result->statusCode != UA_STATUSCODE_GOOD)
        return;

    /* create, update, delete continuation points */
    if(cp) {
        if(referencesIndex == node->referencesSize) {
            /* all done, remove a finished continuationPoint */
            removeCp(cp, session);
        } else {
            /* update the cp and return the cp identifier */
            cp->continuationIndex += referencesCount;
            UA_ByteString_copy(&cp->identifier, &result->continuationPoint);
        }
    } else if(maxrefs != 0 && referencesCount >= maxrefs) {
        /* create a cp */
        if(session->availableContinuationPoints <= 0 || !(cp = UA_malloc(sizeof(struct ContinuationPointEntry)))) {
            result->statusCode = UA_STATUSCODE_BADNOCONTINUATIONPOINTS;
            return;
        }
        UA_BrowseDescription_copy(descr, &cp->browseDescription);
        cp->maxReferences = maxrefs;
        cp->continuationIndex = referencesCount;
        UA_Guid *ident = UA_Guid_new();
        UA_UInt32 seed = (uintptr_t)cp;
        *ident = UA_Guid_random(&seed);
        cp->identifier.data = (UA_Byte*)ident;
        cp->identifier.length = sizeof(UA_Guid);
        UA_ByteString_copy(&cp->identifier, &result->continuationPoint);

        /* store the cp */
        LIST_INSERT_HEAD(&session->continuationPoints, cp, pointers);
        session->availableContinuationPoints--;
    }
}

void Service_Browse(UA_Server *server, UA_Session *session, const UA_BrowseRequest *request,
                    UA_BrowseResponse *response) {
    if(!UA_NodeId_isNull(&request->view.viewId)) {
        response->responseHeader.serviceResult = UA_STATUSCODE_BADVIEWIDUNKNOWN;
        return;
    }
    
    if(request->nodesToBrowseSize <= 0) {
        response->responseHeader.serviceResult = UA_STATUSCODE_BADNOTHINGTODO;
        return;
    }

    size_t size = request->nodesToBrowseSize;
    response->results = UA_Array_new(&UA_TYPES[UA_TYPES_BROWSERESULT], size);
    if(!response->results) {
        response->responseHeader.serviceResult = UA_STATUSCODE_BADOUTOFMEMORY;
        return;
    }
    response->resultsSize = size;
    
#ifdef UA_EXTERNAL_NAMESPACES
#ifdef NO_ALLOCA
    UA_Boolean isExternal[size];
    UA_UInt32 indices[size];
#else
    UA_Boolean *isExternal = UA_alloca(sizeof(UA_Boolean) * size);
    UA_UInt32 *indices = UA_alloca(sizeof(UA_UInt32) * size);
#endif /*NO_ALLOCA */
    UA_memset(isExternal, UA_FALSE, sizeof(UA_Boolean) * size);
    for(size_t j = 0; j < server->externalNamespacesSize; j++) {
        size_t indexSize = 0;
        for(size_t i = 0; i < size; i++) {
            if(request->nodesToBrowse[i].nodeId.namespaceIndex != server->externalNamespaces[j].index)
                continue;
            isExternal[i] = UA_TRUE;
            indices[indexSize] = i;
            indexSize++;
        }
        if(indexSize == 0)
            continue;
        UA_ExternalNodeStore *ens = &server->externalNamespaces[j].externalNodeStore;
        ens->browseNodes(ens->ensHandle, &request->requestHeader, request->nodesToBrowse, indices, indexSize,
                         request->requestedMaxReferencesPerNode, response->results, response->diagnosticInfos);
    }
#endif

    for(size_t i = 0; i < size; i++) {
#ifdef UA_EXTERNAL_NAMESPACES
        if(!isExternal[i])
#endif
            browse(session, server->nodestore, UA_NULL, &request->nodesToBrowse[i],
                   request->requestedMaxReferencesPerNode, &response->results[i]);
    }
}

void Service_BrowseNext(UA_Server *server, UA_Session *session, const UA_BrowseNextRequest *request,
                        UA_BrowseNextResponse *response) {
   if(request->continuationPointsSize <= 0) {
        response->responseHeader.serviceResult = UA_STATUSCODE_BADNOTHINGTODO;
        return;
   }
   size_t size = request->continuationPointsSize;
   if(!request->releaseContinuationPoints) {
       /* continue with the cp */
       response->results = UA_Array_new(&UA_TYPES[UA_TYPES_BROWSERESULT], size);
       if(!response->results) {
           response->responseHeader.serviceResult = UA_STATUSCODE_BADOUTOFMEMORY;
           return;
       }
       response->resultsSize = size;
       for(size_t i = 0; i < size; i++) {
           struct ContinuationPointEntry *cp;
           LIST_FOREACH(cp, &session->continuationPoints, pointers) {
               if(UA_ByteString_equal(&cp->identifier, &request->continuationPoints[i])) {
                   browse(session, server->nodestore, cp, UA_NULL, 0, &response->results[i]);
                   break;
               }
           }
           if(!cp)
               response->results[i].statusCode = UA_STATUSCODE_BADCONTINUATIONPOINTINVALID;
       }
   } else {
       /* remove the cp */
       response->results = UA_Array_new(&UA_TYPES[UA_TYPES_BROWSERESULT], size);
       if(!response->results) {
           response->responseHeader.serviceResult = UA_STATUSCODE_BADOUTOFMEMORY;
           return;
       }
       response->resultsSize = size;
       for(size_t i = 0; i < size; i++) {
           struct ContinuationPointEntry *cp = UA_NULL;
           LIST_FOREACH(cp, &session->continuationPoints, pointers) {
               if(UA_ByteString_equal(&cp->identifier, &request->continuationPoints[i])) {
                   removeCp(cp, session);
                   break;
               }
           }
           if(!cp)
               response->results[i].statusCode = UA_STATUSCODE_BADCONTINUATIONPOINTINVALID;
       }
   }
}

/***********************/
/* TranslateBrowsePath */
/***********************/

static UA_StatusCode
walkBrowsePath(UA_Server *server, UA_Session *session, const UA_Node *node, const UA_RelativePath *path,
               UA_Int32 pathindex, UA_BrowsePathTarget **targets, UA_Int32 *targets_size,
               UA_Int32 *target_count)
{
    const UA_RelativePathElement *elem = &path->elements[pathindex];
    if(elem->targetName.name.length == -1)
        return UA_STATUSCODE_BADBROWSENAMEINVALID;

    UA_StatusCode retval = UA_STATUSCODE_GOOD;
    UA_NodeId *reftypes;
    size_t reftypes_count = 1; // all_refs or no subtypes => 1
    UA_Boolean all_refs = UA_FALSE;
    if(UA_NodeId_isNull(&elem->referenceTypeId))
        all_refs = UA_TRUE;
    else if(!elem->includeSubtypes)
        reftypes = (UA_NodeId*)(uintptr_t)&elem->referenceTypeId; // ptr magic due to const cast
    else {
        retval = findsubtypes(server->nodestore, &elem->referenceTypeId, &reftypes, &reftypes_count);
        if(retval != UA_STATUSCODE_GOOD)
            return retval;
    }

    for(UA_Int32 i = 0; i < node->referencesSize && retval == UA_STATUSCODE_GOOD; i++) {
        UA_Boolean match = all_refs;
        for(size_t j = 0; j < reftypes_count && !match; j++) {
            if(node->references[i].isInverse == elem->isInverse &&
               UA_NodeId_equal(&node->references[i].referenceTypeId, &reftypes[j]))
                match = UA_TRUE;
        }
        if(!match)
            continue;

        // get the node, todo: expandednodeid
        const UA_Node *next = UA_NodeStore_get(server->nodestore, &node->references[i].targetId.nodeId);
        if(!next)
            continue;

        // test the browsename
        if(elem->targetName.namespaceIndex != next->browseName.namespaceIndex ||
           !UA_String_equal(&elem->targetName.name, &next->browseName.name)) {
            UA_NodeStore_release(next);
            continue;
        }

        if(pathindex + 1 < path->elementsSize) {
            // recursion if the path is longer
            retval = walkBrowsePath(server, session, next, path, pathindex + 1,
                                    targets, targets_size, target_count);
            UA_NodeStore_release(next);
        } else {
            // add the browsetarget
            if(*target_count >= *targets_size) {
                UA_BrowsePathTarget *newtargets;
                newtargets = UA_realloc(targets, sizeof(UA_BrowsePathTarget) * (*targets_size) * 2);
                if(!newtargets) {
                    retval = UA_STATUSCODE_BADOUTOFMEMORY;
                    UA_NodeStore_release(next);
                    break;
                }
                *targets = newtargets;
                *targets_size *= 2;
            }

            UA_BrowsePathTarget *res = *targets;
            UA_ExpandedNodeId_init(&res[*target_count].targetId);
            retval = UA_NodeId_copy(&next->nodeId, &res[*target_count].targetId.nodeId);
            UA_NodeStore_release(next);
            if(retval != UA_STATUSCODE_GOOD)
                break;
            res[*target_count].remainingPathIndex = UA_UINT32_MAX;
            *target_count += 1;
        }
    }

    if(!all_refs && elem->includeSubtypes)
        UA_Array_delete(reftypes, &UA_TYPES[UA_TYPES_NODEID], (UA_Int32)reftypes_count);
    return retval;
}

static void translateBrowsePath(UA_Server *server, UA_Session *session, const UA_BrowsePath *path,
                                UA_BrowsePathResult *result) {
    if(path->relativePath.elementsSize <= 0) {
        result->statusCode = UA_STATUSCODE_BADNOTHINGTODO;
        return;
    }
        
    UA_Int32 arraySize = 10;
    result->targets = UA_malloc(sizeof(UA_BrowsePathTarget) * arraySize);
    if(!result->targets) {
        result->statusCode = UA_STATUSCODE_BADOUTOFMEMORY;
        return;
    }
    result->targetsSize = 0;
    const UA_Node *firstNode = UA_NodeStore_get(server->nodestore, &path->startingNode);
    if(!firstNode) {
        result->statusCode = UA_STATUSCODE_BADNODEIDUNKNOWN;
        UA_free(result->targets);
        result->targets = UA_NULL;
        return;
    }
    result->statusCode = walkBrowsePath(server, session, firstNode, &path->relativePath, 0,
                                        &result->targets, &arraySize, &result->targetsSize);
    UA_NodeStore_release(firstNode);
    if(result->targetsSize == 0 && result->statusCode == UA_STATUSCODE_GOOD)
        result->statusCode = UA_STATUSCODE_BADNOMATCH;
    if(result->statusCode != UA_STATUSCODE_GOOD) {
        UA_Array_delete(result->targets, &UA_TYPES[UA_TYPES_BROWSEPATHTARGET], result->targetsSize);
        result->targets = UA_NULL;
        result->targetsSize = -1;
    }
}

void Service_TranslateBrowsePathsToNodeIds(UA_Server *server, UA_Session *session,
                                           const UA_TranslateBrowsePathsToNodeIdsRequest *request,
                                           UA_TranslateBrowsePathsToNodeIdsResponse *response) {
	if(request->browsePathsSize <= 0) {
        response->responseHeader.serviceResult = UA_STATUSCODE_BADNOTHINGTODO;
        return;
    }

    size_t size = request->browsePathsSize;

    response->results = UA_Array_new(&UA_TYPES[UA_TYPES_BROWSEPATHRESULT], size);
    if(!response->results) {
        response->responseHeader.serviceResult = UA_STATUSCODE_BADOUTOFMEMORY;
        return;
    }

#ifdef UA_EXTERNAL_NAMESPACES
#ifdef NO_ALLOCA
    UA_Boolean isExternal[size];
    UA_UInt32 indices[size];
#else
    UA_Boolean *isExternal = UA_alloca(sizeof(UA_Boolean) * size);
    UA_UInt32 *indices = UA_alloca(sizeof(UA_UInt32) * size);
#endif /*NO_ALLOCA */
    UA_memset(isExternal, UA_FALSE, sizeof(UA_Boolean) * size);
    for(size_t j = 0; j < server->externalNamespacesSize; j++) {
    	size_t indexSize = 0;
    	for(size_t i = 0;i < size;i++) {
    		if(request->browsePaths[i].startingNode.namespaceIndex != server->externalNamespaces[j].index)
    			continue;
    		isExternal[i] = UA_TRUE;
    		indices[indexSize] = i;
    		indexSize++;
    	}
    	if(indexSize == 0)
    		continue;
    	UA_ExternalNodeStore *ens = &server->externalNamespaces[j].externalNodeStore;
    	ens->translateBrowsePathsToNodeIds(ens->ensHandle, &request->requestHeader, request->browsePaths,
    			indices, indexSize, response->results, response->diagnosticInfos);
    }
#endif

    response->resultsSize = size;
    for(size_t i = 0; i < size; i++) {
#ifdef UA_EXTERNAL_NAMESPACES
    	if(!isExternal[i])
#endif
    		translateBrowsePath(server, session, &request->browsePaths[i], &response->results[i]);
    }
}

void Service_RegisterNodes(UA_Server *server, UA_Session *session, const UA_RegisterNodesRequest *request,
                           UA_RegisterNodesResponse *response) {
	//TODO: hang the nodeids to the session if really needed
	response->responseHeader.timestamp = UA_DateTime_now();
    if(request->nodesToRegisterSize <= 0)
        response->responseHeader.serviceResult = UA_STATUSCODE_BADNOTHINGTODO;
    else {
        response->responseHeader.serviceResult =
            UA_Array_copy(request->nodesToRegister, (void**)&response->registeredNodeIds, &UA_TYPES[UA_TYPES_NODEID],
                request->nodesToRegisterSize);
        if(response->responseHeader.serviceResult == UA_STATUSCODE_GOOD)
            response->registeredNodeIdsSize = request->nodesToRegisterSize;
    }
}

void Service_UnregisterNodes(UA_Server *server, UA_Session *session, const UA_UnregisterNodesRequest *request,
                             UA_UnregisterNodesResponse *response) {
	//TODO: remove the nodeids from the session if really needed
	response->responseHeader.timestamp = UA_DateTime_now();
	if(request->nodesToUnregisterSize==0)
		response->responseHeader.serviceResult = UA_STATUSCODE_BADNOTHINGTODO;
}
/*********************************** amalgamated original file "C:/AcpltDevelopmentKit/acplt/dev/open62541WrapperProject/open62541/src/client/ua_client.c" ***********************************/

struct UA_Client {
    /* Connection */
    UA_Connection connection;
    UA_String endpointUrl;

    UA_UInt32 sequenceNumber;
    UA_UInt32 requestId;

    /* Secure Channel */
    UA_ChannelSecurityToken securityToken;
    UA_ByteString clientNonce;
    UA_ByteString serverNonce;
    /* UA_SequenceHeader sequenceHdr; */
    /* UA_NodeId authenticationToken; */

    /* IdentityToken */
    UA_UserTokenPolicy token;

    /* Session */
    UA_NodeId sessionId;
    UA_NodeId authenticationToken;

    /* Config */
    UA_Logger logger;
    UA_ClientConfig config;
    UA_DateTime scExpiresAt;
};

const UA_EXPORT UA_ClientConfig UA_ClientConfig_standard =
    { 5 /* ms receive timout */, 30000, 2000,
      {.protocolVersion = 0, .sendBufferSize = 65536, .recvBufferSize  = 65536,
       .maxMessageSize = 65536, .maxChunkCount = 1}};

UA_Client * UA_Client_new(UA_ClientConfig config, UA_Logger logger) {
    UA_Client *client = UA_malloc(sizeof(UA_Client));
    if(!client)
        return UA_NULL;
    client->config = config;
    client->logger = logger;
    UA_String_init(&client->endpointUrl);
    UA_Connection_init(&client->connection);

    client->sequenceNumber = 0;
    client->requestId = 0;

    client->scExpiresAt = 0;

    /* Secure Channel */
    UA_ChannelSecurityToken_deleteMembers(&client->securityToken);
    UA_ByteString_init(&client->clientNonce);
    UA_ByteString_init(&client->serverNonce);
    
    UA_NodeId_init(&client->authenticationToken);

    return client;
}

void UA_Client_delete(UA_Client* client){
    UA_Connection_deleteMembers(&client->connection);
    UA_Connection_deleteMembers(&client->connection);
    UA_String_deleteMembers(&client->endpointUrl);

    /* Secure Channel */
    UA_ByteString_deleteMembers(&client->clientNonce);
    UA_ByteString_deleteMembers(&client->serverNonce);

    UA_UserTokenPolicy_deleteMembers(&client->token);

    free(client);
}

static UA_StatusCode HelAckHandshake(UA_Client *c) {
    UA_TcpMessageHeader messageHeader;
    messageHeader.messageTypeAndFinal = UA_MESSAGETYPEANDFINAL_HELF;

    UA_TcpHelloMessage hello;
    UA_String_copy(&c->endpointUrl, &hello.endpointUrl);

    UA_Connection *conn = &c->connection;
    hello.maxChunkCount = conn->localConf.maxChunkCount;
    hello.maxMessageSize = conn->localConf.maxMessageSize;
    hello.protocolVersion = conn->localConf.protocolVersion;
    hello.receiveBufferSize = conn->localConf.recvBufferSize;
    hello.sendBufferSize = conn->localConf.sendBufferSize;

    messageHeader.messageSize = UA_TcpHelloMessage_calcSizeBinary((UA_TcpHelloMessage const*)&hello) +
                                UA_TcpMessageHeader_calcSizeBinary((UA_TcpMessageHeader const*)&messageHeader);
    UA_ByteString message;
    UA_StatusCode retval = c->connection.getBuffer(&c->connection, &message, messageHeader.messageSize);
    if(retval != UA_STATUSCODE_GOOD)
        return retval;

    size_t offset = 0;
    UA_TcpMessageHeader_encodeBinary(&messageHeader, &message, &offset);
    UA_TcpHelloMessage_encodeBinary(&hello, &message, &offset);
    UA_TcpHelloMessage_deleteMembers(&hello);

    retval = c->connection.write(&c->connection, &message);
    c->connection.releaseBuffer(&c->connection, &message);
    if(retval)
        return retval;

    UA_ByteString reply;
    UA_ByteString_init(&reply);
    do {
        retval = c->connection.recv(&c->connection, &reply, c->config.timeout);
        if(retval == UA_STATUSCODE_BADCONNECTIONCLOSED)
            return retval;
    } while(retval != UA_STATUSCODE_GOOD);

    offset = 0;
    UA_TcpMessageHeader_decodeBinary(&reply, &offset, &messageHeader);
    UA_TcpAcknowledgeMessage ackMessage;
    retval = UA_TcpAcknowledgeMessage_decodeBinary(&reply, &offset, &ackMessage);
    c->connection.releaseBuffer(&c->connection, &reply);
    if(retval != UA_STATUSCODE_GOOD)
        return retval;
    conn->remoteConf.maxChunkCount = ackMessage.maxChunkCount;
    conn->remoteConf.maxMessageSize = ackMessage.maxMessageSize;
    conn->remoteConf.protocolVersion = ackMessage.protocolVersion;
    conn->remoteConf.recvBufferSize = ackMessage.receiveBufferSize;
    conn->remoteConf.sendBufferSize = ackMessage.sendBufferSize;
    conn->state = UA_CONNECTION_ESTABLISHED;
    return UA_STATUSCODE_GOOD;
}

static UA_StatusCode SecureChannelHandshake(UA_Client *client, UA_Boolean renew) {
    UA_ByteString_deleteMembers(&client->clientNonce); // if the handshake is repeated
    UA_ByteString_newMembers(&client->clientNonce, 1);
    client->clientNonce.data[0] = 0;

    UA_SecureConversationMessageHeader messageHeader;
    messageHeader.messageHeader.messageTypeAndFinal = UA_MESSAGETYPEANDFINAL_OPNF;
    messageHeader.secureChannelId = 0;

    UA_SequenceHeader seqHeader;
    seqHeader.sequenceNumber = ++client->sequenceNumber;
    seqHeader.requestId = ++client->requestId;

    UA_AsymmetricAlgorithmSecurityHeader asymHeader;
    UA_AsymmetricAlgorithmSecurityHeader_init(&asymHeader);
    asymHeader.securityPolicyUri = UA_STRING_ALLOC("http://opcfoundation.org/UA/SecurityPolicy#None");

    /* id of opensecurechannelrequest */
    UA_NodeId requestType = UA_NODEID_NUMERIC(0, UA_NS0ID_OPENSECURECHANNELREQUEST + UA_ENCODINGOFFSET_BINARY);

    UA_OpenSecureChannelRequest opnSecRq;
    UA_OpenSecureChannelRequest_init(&opnSecRq);
    opnSecRq.requestHeader.timestamp = UA_DateTime_now();
    opnSecRq.requestHeader.authenticationToken = client->authenticationToken;
    opnSecRq.requestedLifetime = client->config.secureChannelLifeTime;
    if(renew) {
        opnSecRq.requestType = UA_SECURITYTOKENREQUESTTYPE_RENEW;
    } else {
        opnSecRq.requestType = UA_SECURITYTOKENREQUESTTYPE_ISSUE;
        UA_ByteString_copy(&client->clientNonce, &opnSecRq.clientNonce);
        opnSecRq.securityMode = UA_MESSAGESECURITYMODE_NONE;
    }

    messageHeader.messageHeader.messageSize =
        UA_SecureConversationMessageHeader_calcSizeBinary(&messageHeader) +
        UA_AsymmetricAlgorithmSecurityHeader_calcSizeBinary(&asymHeader) +
        UA_SequenceHeader_calcSizeBinary(&seqHeader) +
        UA_NodeId_calcSizeBinary(&requestType) +
        UA_OpenSecureChannelRequest_calcSizeBinary(&opnSecRq);

    UA_ByteString message;
    UA_StatusCode retval = client->connection.getBuffer(&client->connection, &message,
                                                        messageHeader.messageHeader.messageSize);
    if(retval != UA_STATUSCODE_GOOD) {
        UA_AsymmetricAlgorithmSecurityHeader_deleteMembers(&asymHeader);
        UA_OpenSecureChannelRequest_deleteMembers(&opnSecRq);
        return retval;
    }

    size_t offset = 0;
    UA_SecureConversationMessageHeader_encodeBinary(&messageHeader, &message, &offset);
    UA_AsymmetricAlgorithmSecurityHeader_encodeBinary(&asymHeader, &message, &offset);
    UA_SequenceHeader_encodeBinary(&seqHeader, &message, &offset);
    UA_NodeId_encodeBinary(&requestType, &message, &offset);
    UA_OpenSecureChannelRequest_encodeBinary(&opnSecRq, &message, &offset);

    UA_AsymmetricAlgorithmSecurityHeader_deleteMembers(&asymHeader);
    UA_OpenSecureChannelRequest_deleteMembers(&opnSecRq);

    retval = client->connection.write(&client->connection, &message);
    client->connection.releaseBuffer(&client->connection, &message);
    if(retval)
        return retval;

    // parse the response
    UA_ByteString reply;
    UA_ByteString_init(&reply);
    do {
        retval = client->connection.recv(&client->connection, &reply, client->config.timeout);
        if(retval == UA_STATUSCODE_BADCONNECTIONCLOSED)
            return retval;
    } while(retval != UA_STATUSCODE_GOOD);

    offset = 0;
    UA_SecureConversationMessageHeader_decodeBinary(&reply, &offset, &messageHeader);
    UA_AsymmetricAlgorithmSecurityHeader_decodeBinary(&reply, &offset, &asymHeader);
    UA_SequenceHeader_decodeBinary(&reply, &offset, &seqHeader);
    UA_NodeId_decodeBinary(&reply, &offset, &requestType);
    UA_NodeId expectedRequest = UA_NODEID_NUMERIC(0, UA_NS0ID_OPENSECURECHANNELRESPONSE +
                                                  UA_ENCODINGOFFSET_BINARY);
    if(!UA_NodeId_equal(&requestType, &expectedRequest)) {
        client->connection.releaseBuffer(&client->connection, &reply);
        UA_AsymmetricAlgorithmSecurityHeader_deleteMembers(&asymHeader);
        UA_NodeId_deleteMembers(&requestType);
        return UA_STATUSCODE_BADINTERNALERROR;
    }

    UA_OpenSecureChannelResponse response;
    UA_OpenSecureChannelResponse_decodeBinary(&reply, &offset, &response);
    client->scExpiresAt = UA_DateTime_now() + response.securityToken.revisedLifetime * 10000;
    client->connection.releaseBuffer(&client->connection, &reply);
    retval = response.responseHeader.serviceResult;

    if(!renew && retval == UA_STATUSCODE_GOOD) {
        UA_ChannelSecurityToken_copy(&response.securityToken, &client->securityToken);
        UA_ByteString_deleteMembers(&client->serverNonce); // if the handshake is repeated
        UA_ByteString_copy(&response.serverNonce, &client->serverNonce);
    }

    UA_OpenSecureChannelResponse_deleteMembers(&response);
    UA_AsymmetricAlgorithmSecurityHeader_deleteMembers(&asymHeader);
    return retval;

}

/** If the request fails, then the response is cast to UA_ResponseHeader (at the beginning of every
    response) and filled with the appropriate error code */
static void sendReceiveRequest(UA_RequestHeader *request, const UA_DataType *requestType,
                               void *response, const UA_DataType *responseType, UA_Client *client,
                               UA_Boolean sendOnly) {

    //check if sc needs to be renewed
    if(client->scExpiresAt-UA_DateTime_now() <= client->config.timeToRenewSecureChannel * 10000){ //less than 3 seconds left to expire -> renew
        UA_Client_renewSecureChannel(client);
    }

    if(response)
        UA_init(response, responseType);
    else
        return;

    UA_NodeId_copy(&client->authenticationToken, &request->authenticationToken);

    UA_SecureConversationMessageHeader msgHeader;
    if(sendOnly)
        msgHeader.messageHeader.messageTypeAndFinal = UA_MESSAGETYPEANDFINAL_CLOF;
    else
        msgHeader.messageHeader.messageTypeAndFinal = UA_MESSAGETYPEANDFINAL_MSGF;
    msgHeader.secureChannelId = client->securityToken.channelId;

    UA_SymmetricAlgorithmSecurityHeader symHeader;
    symHeader.tokenId = client->securityToken.tokenId;
    
    UA_SequenceHeader seqHeader;
    seqHeader.sequenceNumber = ++client->sequenceNumber;
    seqHeader.requestId = ++client->requestId;

    UA_NodeId requestId = UA_NODEID_NUMERIC(0, requestType->typeId.identifier.numeric +
                                           UA_ENCODINGOFFSET_BINARY);

    msgHeader.messageHeader.messageSize =
        UA_SecureConversationMessageHeader_calcSizeBinary(&msgHeader) +
        UA_SymmetricAlgorithmSecurityHeader_calcSizeBinary(&symHeader) +
        UA_SequenceHeader_calcSizeBinary(&seqHeader) +
        UA_NodeId_calcSizeBinary(&requestId) +
        UA_calcSizeBinary(request, requestType);

    UA_ByteString message;
    UA_StatusCode retval = client->connection.getBuffer(&client->connection, &message, msgHeader.messageHeader.messageSize);
    if(retval != UA_STATUSCODE_GOOD) {
        // todo: print error message
        return;
    }

    size_t offset = 0;
    retval |= UA_SecureConversationMessageHeader_encodeBinary(&msgHeader, &message, &offset);
    retval |= UA_SymmetricAlgorithmSecurityHeader_encodeBinary(&symHeader, &message, &offset);
    retval |= UA_SequenceHeader_encodeBinary(&seqHeader, &message, &offset);

    retval |= UA_NodeId_encodeBinary(&requestId, &message, &offset);
    retval |= UA_encodeBinary(request, requestType, &message, &offset);

    retval |= client->connection.write(&client->connection, &message);

    UA_ResponseHeader *respHeader = (UA_ResponseHeader*)response;

    client->connection.releaseBuffer(&client->connection, &message);

    if(retval != UA_STATUSCODE_GOOD) {
        //send failed
        respHeader->serviceResult = retval;
        return;
    }

    //TODO: rework to get return value
    if(sendOnly)
        return;

    /* Response */
    UA_ByteString reply;
    UA_ByteString_init(&reply);
    do {
        retval = client->connection.recv(&client->connection, &reply, client->config.timeout);
        if(retval == UA_STATUSCODE_BADCONNECTIONCLOSED) {
            respHeader->serviceResult = retval;
            return;
        }
    } while(retval != UA_STATUSCODE_GOOD);

    offset = 0;
    retval |= UA_SecureConversationMessageHeader_decodeBinary(&reply, &offset, &msgHeader);
    retval |= UA_SymmetricAlgorithmSecurityHeader_decodeBinary(&reply, &offset, &symHeader);
    retval |= UA_SequenceHeader_decodeBinary(&reply, &offset, &seqHeader);
    UA_NodeId responseId;
    retval |= UA_NodeId_decodeBinary(&reply, &offset, &responseId);
    UA_NodeId expectedNodeId = UA_NODEID_NUMERIC(0, responseType->typeId.identifier.numeric +
                                                 UA_ENCODINGOFFSET_BINARY);
    if(!UA_NodeId_equal(&responseId, &expectedNodeId)) {
        client->connection.releaseBuffer(&client->connection, &reply);
        UA_SymmetricAlgorithmSecurityHeader_deleteMembers(&symHeader);
        respHeader->serviceResult = retval;
        return;
    }

    retval = UA_decodeBinary(&reply, &offset, response, responseType);
    client->connection.releaseBuffer(&client->connection, &reply);
    if(retval != UA_STATUSCODE_GOOD)
        respHeader->serviceResult = retval;
}

static void synchronousRequest(void *request, const UA_DataType *requestType, void *response,
                               const UA_DataType *responseType, UA_Client *client) {
    sendReceiveRequest(request, requestType, response, responseType, client, UA_FALSE);
}

static UA_StatusCode ActivateSession(UA_Client *client) {
    UA_ActivateSessionRequest request;
    UA_ActivateSessionRequest_init(&request);

    request.requestHeader.requestHandle = 2; //TODO: is it a magic number?
    request.requestHeader.authenticationToken = client->authenticationToken;
    request.requestHeader.timestamp = UA_DateTime_now();
    request.requestHeader.timeoutHint = 10000;

    UA_AnonymousIdentityToken identityToken;
    UA_AnonymousIdentityToken_init(&identityToken);
    UA_String_copy(&client->token.policyId, &identityToken.policyId);

    //manual ExtensionObject encoding of the identityToken
    request.userIdentityToken.encoding = UA_EXTENSIONOBJECT_ENCODINGMASK_BODYISBYTESTRING;
    request.userIdentityToken.typeId = UA_TYPES[UA_TYPES_ANONYMOUSIDENTITYTOKEN].typeId;
    request.userIdentityToken.typeId.identifier.numeric+=UA_ENCODINGOFFSET_BINARY;

    UA_ByteString_newMembers(&request.userIdentityToken.body, identityToken.policyId.length+4);
    size_t offset = 0;
    UA_ByteString_encodeBinary(&identityToken.policyId,&request.userIdentityToken.body,&offset);

    UA_ActivateSessionResponse response;
    synchronousRequest(&request, &UA_TYPES[UA_TYPES_ACTIVATESESSIONREQUEST],
                       &response, &UA_TYPES[UA_TYPES_ACTIVATESESSIONRESPONSE],
                       client);

    UA_AnonymousIdentityToken_deleteMembers(&identityToken);
    UA_ActivateSessionRequest_deleteMembers(&request);
    UA_ActivateSessionResponse_deleteMembers(&response);
    return response.responseHeader.serviceResult; // not deleted
}

static UA_StatusCode EndpointsHandshake(UA_Client *client) {
    UA_GetEndpointsRequest request;
    UA_GetEndpointsRequest_init(&request);

    // todo: is this needed for all requests?
    UA_NodeId_copy(&client->authenticationToken, &request.requestHeader.authenticationToken);

    request.requestHeader.timestamp = UA_DateTime_now();
    request.requestHeader.timeoutHint = 10000;
    UA_String_copy(&client->endpointUrl, &request.endpointUrl);

    request.profileUrisSize = 1;
    request.profileUris = UA_Array_new(&UA_TYPES[UA_TYPES_STRING], request.profileUrisSize);
    request.profileUris[0] = UA_STRING_ALLOC("http://opcfoundation.org/UA-Profile/Transport/uatcp-uasc-uabinary");

    UA_GetEndpointsResponse response;
    UA_GetEndpointsResponse_init(&response);
    synchronousRequest(&request, &UA_TYPES[UA_TYPES_GETENDPOINTSREQUEST],
                       &response, &UA_TYPES[UA_TYPES_GETENDPOINTSRESPONSE],
                       client);

    UA_Boolean endpointFound = UA_FALSE;
    UA_Boolean tokenFound = UA_FALSE;
    for(UA_Int32 i=0; i<response.endpointsSize; ++i){
        UA_EndpointDescription* endpoint = &response.endpoints[i];
        /* look out for an endpoint without security */
        if(UA_String_equal(&endpoint->securityPolicyUri, &UA_STRING("http://opcfoundation.org/UA/SecurityPolicy#None"))){
            endpointFound = UA_TRUE;
            /* endpoint with no security found */
            /* look for a user token policy with an anonymous token */
            for(UA_Int32 j=0; j<endpoint->userIdentityTokensSize; ++j){
                UA_UserTokenPolicy* userToken = &endpoint->userIdentityTokens[j];
                if(userToken->tokenType == UA_USERTOKENTYPE_ANONYMOUS){
                    tokenFound = UA_TRUE;
                    UA_UserTokenPolicy_copy(userToken, &client->token);
                    break;
                }
            }
        }
    }

    UA_GetEndpointsRequest_deleteMembers(&request);
    UA_GetEndpointsResponse_deleteMembers(&response);

    if(!endpointFound){
        printf("No suitable endpoint found\n");
        return UA_STATUSCODE_BADINTERNALERROR;
    }
    if(!tokenFound){
        printf("No anonymous token found\n");
        return UA_STATUSCODE_BADINTERNALERROR;
    }

    return response.responseHeader.serviceResult; // not deleted
}

static UA_StatusCode SessionHandshake(UA_Client *client) {
    UA_CreateSessionRequest request;
    UA_CreateSessionRequest_init(&request);

    // todo: is this needed for all requests?
    UA_NodeId_copy(&client->authenticationToken, &request.requestHeader.authenticationToken);

    request.requestHeader.timestamp = UA_DateTime_now();
    request.requestHeader.timeoutHint = 10000;
    UA_ByteString_copy(&client->clientNonce, &request.clientNonce);
    request.requestedSessionTimeout = 1200000;
    request.maxResponseMessageSize = UA_INT32_MAX;

    /* UA_String_copy(endpointUrl, &rq.endpointUrl); */
    /* UA_String_copycstring("mysession", &rq.sessionName); */
    /* UA_String_copycstring("abcd", &rq.clientCertificate); */

    UA_CreateSessionResponse response;
    UA_CreateSessionResponse_init(&response);
    synchronousRequest(&request, &UA_TYPES[UA_TYPES_CREATESESSIONREQUEST],
                       &response, &UA_TYPES[UA_TYPES_CREATESESSIONRESPONSE],
                       client);

    UA_NodeId_copy(&response.authenticationToken, &client->authenticationToken);

    UA_CreateSessionRequest_deleteMembers(&request);
    UA_CreateSessionResponse_deleteMembers(&response);
    return response.responseHeader.serviceResult; // not deleted
}

static UA_StatusCode CloseSession(UA_Client *client) {
    UA_CloseSessionRequest request;
    UA_CloseSessionRequest_init(&request);

    request.requestHeader.timestamp = UA_DateTime_now();
    request.requestHeader.timeoutHint = 10000;
    request.deleteSubscriptions = UA_TRUE;
    UA_NodeId_copy(&client->authenticationToken, &request.requestHeader.authenticationToken);
    UA_CreateSessionResponse response;
    synchronousRequest(&request, &UA_TYPES[UA_TYPES_CLOSESESSIONREQUEST],
                       &response, &UA_TYPES[UA_TYPES_CLOSESESSIONRESPONSE],
                       client);

    UA_CloseSessionRequest_deleteMembers(&request);
    UA_CloseSessionResponse_deleteMembers(&response);
    return response.responseHeader.serviceResult; // not deleted
}

static UA_StatusCode CloseSecureChannel(UA_Client *client) {
    UA_CloseSecureChannelRequest request;
    UA_CloseSecureChannelRequest_init(&request);

    request.requestHeader.requestHandle = 1; //TODO: magic number?
    request.requestHeader.timestamp = UA_DateTime_now();
    request.requestHeader.timeoutHint = 10000;
    request.requestHeader.authenticationToken = client->authenticationToken;
    sendReceiveRequest(&request.requestHeader, &UA_TYPES[UA_TYPES_CLOSESECURECHANNELREQUEST], UA_NULL, UA_NULL,
                       client, UA_TRUE);

    return UA_STATUSCODE_GOOD;

}

/*************************/
/* User-Facing Functions */
/*************************/

UA_StatusCode UA_Client_connect(UA_Client *client, UA_ConnectClientConnection connectFunc, char *endpointUrl) {
    client->connection = connectFunc(endpointUrl, &client->logger);
    if(client->connection.state != UA_CONNECTION_OPENING)
        return UA_STATUSCODE_BADCONNECTIONCLOSED;

    client->endpointUrl = UA_STRING_ALLOC(endpointUrl);
    if(client->endpointUrl.length < 0)
        return UA_STATUSCODE_BADOUTOFMEMORY;

    client->connection.localConf = client->config.localConnectionConfig;
    UA_StatusCode retval = HelAckHandshake(client);
    if(retval == UA_STATUSCODE_GOOD)
        retval = SecureChannelHandshake(client, UA_FALSE);
    if(retval == UA_STATUSCODE_GOOD)
        retval = EndpointsHandshake(client);
    if(retval == UA_STATUSCODE_GOOD)
        retval = SessionHandshake(client);
    if(retval == UA_STATUSCODE_GOOD)
        retval = ActivateSession(client);
    return retval;
}

UA_StatusCode UA_Client_disconnect(UA_Client *client) {
    UA_StatusCode retval;
    retval = CloseSession(client);
    if(retval == UA_STATUSCODE_GOOD)
        retval = CloseSecureChannel(client);
    return retval;
}

UA_StatusCode UA_Client_renewSecureChannel(UA_Client *client) {
    return SecureChannelHandshake(client, UA_TRUE);
}

UA_ReadResponse UA_Client_read(UA_Client *client, UA_ReadRequest *request) {
    UA_ReadResponse response;
    synchronousRequest(request, &UA_TYPES[UA_TYPES_READREQUEST], &response,
                       &UA_TYPES[UA_TYPES_READRESPONSE], client);
    return response;
}

UA_WriteResponse UA_Client_write(UA_Client *client, UA_WriteRequest *request) {
    UA_WriteResponse response;
    synchronousRequest(request, &UA_TYPES[UA_TYPES_WRITEREQUEST], &response,
                       &UA_TYPES[UA_TYPES_WRITERESPONSE], client);
    return response;
}

UA_BrowseResponse UA_Client_browse(UA_Client *client, UA_BrowseRequest *request) {
    UA_BrowseResponse response;
    synchronousRequest(request, &UA_TYPES[UA_TYPES_BROWSEREQUEST], &response,
                       &UA_TYPES[UA_TYPES_BROWSERESPONSE], client);
    return response;
}

UA_BrowseNextResponse UA_Client_browseNext(UA_Client *client, UA_BrowseNextRequest *request) {
    UA_BrowseNextResponse response;
    synchronousRequest(request, &UA_TYPES[UA_TYPES_BROWSENEXTREQUEST], &response,
                       &UA_TYPES[UA_TYPES_BROWSENEXTRESPONSE], client);
    return response;
}

UA_TranslateBrowsePathsToNodeIdsResponse
    UA_Client_translateTranslateBrowsePathsToNodeIds(UA_Client *client,
                                                     UA_TranslateBrowsePathsToNodeIdsRequest *request) {
    UA_TranslateBrowsePathsToNodeIdsResponse response;
    synchronousRequest(request, &UA_TYPES[UA_TYPES_BROWSEREQUEST], &response,
                       &UA_TYPES[UA_TYPES_BROWSERESPONSE], client);
    return response;
}

UA_AddNodesResponse UA_Client_addNodes(UA_Client *client, UA_AddNodesRequest *request) {
    UA_AddNodesResponse response;
    synchronousRequest(request, &UA_TYPES[UA_TYPES_BROWSEREQUEST], &response,
                       &UA_TYPES[UA_TYPES_BROWSERESPONSE], client);
    return response;
}

UA_AddReferencesResponse UA_Client_addReferences(UA_Client *client, UA_AddReferencesRequest *request) {
    UA_AddReferencesResponse response;
    synchronousRequest(request, &UA_TYPES[UA_TYPES_BROWSEREQUEST], &response,
                       &UA_TYPES[UA_TYPES_BROWSERESPONSE], client);
    return response;
}

UA_DeleteNodesResponse UA_Client_deleteNodes(UA_Client *client, UA_DeleteNodesRequest *request) {
    UA_DeleteNodesResponse response;
    synchronousRequest(request, &UA_TYPES[UA_TYPES_BROWSEREQUEST], &response,
                       &UA_TYPES[UA_TYPES_BROWSERESPONSE], client);
    return response;
}

UA_DeleteReferencesResponse UA_Client_deleteReferences(UA_Client *client, UA_DeleteReferencesRequest *request) {
    UA_DeleteReferencesResponse response;
    synchronousRequest(request, &UA_TYPES[UA_TYPES_BROWSEREQUEST], &response,
                       &UA_TYPES[UA_TYPES_BROWSERESPONSE], client);
    return response;;
}
/*********************************** amalgamated original file "C:/AcpltDevelopmentKit/acplt/dev/open62541WrapperProject/open62541/examples/networklayer_tcp.c" ***********************************/
 /*
 * This work is licensed under a Creative Commons CCZero 1.0 Universal License.
 * See http://creativecommons.org/publicdomain/zero/1.0/ for more information.
 */

#ifdef NOT_AMALGATED
# define _XOPEN_SOURCE 500 //some users need this for some reason
# include <stdlib.h> // malloc, free
# include <string.h> // memset
#endif

#ifdef _WIN32
# include <malloc.h>
# include <winsock2.h>
# include <ws2tcpip.h>
# define CLOSESOCKET(S) closesocket(S)
#else
# include <fcntl.h>
# include <sys/select.h> 
# include <netinet/in.h>
# include <netinet/tcp.h>
# include <sys/ioctl.h>
# include <netdb.h> //gethostbyname for the client
# include <unistd.h> // read, write, close
# include <arpa/inet.h>
# define CLOSESOCKET(S) close(S)
#endif

#ifdef UA_MULTITHREADING
# include <urcu/uatomic.h>
#endif

/* with a space, so the include is not removed during amalgamation */
# include <errno.h>

/****************************/
/* Generic Socket Functions */
/****************************/

static UA_StatusCode socket_write(UA_Connection *connection, const UA_ByteString *buf) {
    if(buf->length < 0)
        return UA_STATUSCODE_BADINTERNALERROR;
    UA_Int32 nWritten = 0;
    while (nWritten < buf->length) {
        UA_Int32 n = 0;
        do {
#ifdef _WIN32
            n = send((SOCKET)connection->sockfd, (const char*)buf->data, buf->length, 0);
            if(n < 0 && WSAGetLastError() != WSAEINTR && WSAGetLastError() != WSAEWOULDBLOCK)
                return UA_STATUSCODE_BADCONNECTIONCLOSED;
#else
            n = send(connection->sockfd, (const char*)buf->data, buf->length, MSG_NOSIGNAL);
            if(n == -1L && errno != EINTR && errno != EAGAIN)
                return UA_STATUSCODE_BADCONNECTIONCLOSED;
#endif
        } while (n == -1L);
        nWritten += n;
    }
    return UA_STATUSCODE_GOOD;
}

static UA_StatusCode socket_recv(UA_Connection *connection, UA_ByteString *response, UA_UInt32 timeout) {
    UA_StatusCode retval = UA_STATUSCODE_GOOD;
	if(response->data == NULL)
        retval = connection->getBuffer(connection, response, connection->localConf.recvBufferSize);
    if(retval != UA_STATUSCODE_GOOD)
        return retval;
    struct timeval tmptv = {0, timeout * 1000};
    if(0 != setsockopt(connection->sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&tmptv, sizeof(struct timeval))){
        return UA_STATUSCODE_BADINTERNALERROR;
    }
    int ret = recv(connection->sockfd, (char*)response->data, response->length, 0);
	if(ret == 0) {
		connection->releaseBuffer(connection, response);
		connection->close(connection);
		return UA_STATUSCODE_BADCONNECTIONCLOSED;
	} else if(ret < 0) {
#ifdef _WIN32
		if(WSAGetLastError() == WSAEINTR || WSAGetLastError() == WSAEWOULDBLOCK) {
#else
		if (errno == EAGAIN) {
#endif
            return UA_STATUSCODE_BADCOMMUNICATIONERROR;
        }
        connection->releaseBuffer(connection, response);
		connection->close(connection);
        return UA_STATUSCODE_BADCONNECTIONCLOSED;
    }
    response->length = ret;
    *response = UA_Connection_completeMessages(connection, *response);
    return UA_STATUSCODE_GOOD;
}

static void socket_close(UA_Connection *connection) {
    connection->state = UA_CONNECTION_CLOSED;
    shutdown(connection->sockfd,2);
    CLOSESOCKET(connection->sockfd);
}

static UA_StatusCode socket_set_nonblocking(UA_Int32 sockfd) {
#ifdef _WIN32
    u_long iMode = 1;
    if(ioctlsocket(sockfd, FIONBIO, &iMode) != NO_ERROR)
        return UA_STATUSCODE_BADINTERNALERROR;
#else
    int opts = fcntl(sockfd, F_GETFL);
    if(opts < 0 || fcntl(sockfd, F_SETFL, opts|O_NONBLOCK) < 0)
        return UA_STATUSCODE_BADINTERNALERROR;
#endif
    return UA_STATUSCODE_GOOD;
}

/*****************************/
/* Generic Buffer Management */
/*****************************/

static UA_StatusCode GetMallocedBuffer(UA_Connection *connection, UA_ByteString *buf, size_t minSize) {
    return UA_ByteString_newMembers(buf, minSize);
}

static void ReleaseMallocedBuffer(UA_Connection *connection, UA_ByteString *buf) {
    UA_ByteString_deleteMembers(buf);
}

/***************************/
/* Server NetworkLayer TCP */
/***************************/

/**
 * For the multithreaded mode, assume a single thread that periodically "gets work" from the network
 * layer. In addition, several worker threads are asynchronously calling into the callbacks of the
 * UA_Connection that holds a single connection.
 *
 * Creating a connection: When "GetWork" encounters a new connection, it creates a UA_Connection
 * with the socket information. This is added to the mappings array that links sockets to
 * UA_Connection structs.
 *
 * Reading data: In "GetWork", we listen on the sockets in the mappings array. If data arrives (or
 * the connection closes), a WorkItem is created that carries the work and a pointer to the
 * connection.
 *
 * Closing a connection: Closing can happen in two ways. Either it is triggered by the server in an
 * asynchronous callback. Or the connection is close by the client and this is detected in
 * "GetWork". The server needs to do some internal cleanups (close attached securechannels, etc.).
 * So even when a closed connection is detected in "GetWork", we trigger the server to close the
 * connection (with a WorkItem) and continue from the callback.
 *
 * - Server calls close-callback: We close the socket, set the connection-state to closed and add
 *   the connection to a linked list from which it is deleted later. The connection cannot be freed
 *   right away since other threads might still be using it.
 *
 * - GetWork: We remove the connection from the mappings array. In the non-multithreaded case, the
 *   connection is freed. For multithreading, we return a workitem that is delayed, i.e. that is
 *   called only after all workitems created before are finished in all threads. This workitems
 *   contains a callback that goes through the linked list of connections to be freed.
 *
 */

#define MAXBACKLOG 100

typedef struct {
    /* config */
    UA_Logger *logger;
    UA_UInt32 port;
    UA_String discoveryUrl;
    UA_ConnectionConfig conf; /* todo: rename to localconf. */

    /* open sockets and connections */
    fd_set fdset;
    UA_Int32 serversockfd;
    UA_Int32 highestfd;
    size_t mappingsSize;
    struct ConnectionMapping {
        UA_Connection *connection;
        UA_Int32 sockfd;
    } *mappings;

    /* to-be-deleted connections */
    struct DeleteList {
        struct DeleteList *next;
        UA_Connection *connection;
    } *deletes;
} ServerNetworkLayerTCP;

/* after every select, we need to reset the sockets we want to listen on */
static void setFDSet(ServerNetworkLayerTCP *layer) {
    FD_ZERO(&layer->fdset);
#ifdef _WIN32
    FD_SET((UA_UInt32)layer->serversockfd, &layer->fdset);
#else
    FD_SET(layer->serversockfd, &layer->fdset);
#endif
    layer->highestfd = layer->serversockfd;
    for(size_t i = 0; i < layer->mappingsSize; i++) {
#ifdef _WIN32
        FD_SET((UA_UInt32)layer->mappings[i].sockfd, &layer->fdset);
#else
        FD_SET(layer->mappings[i].sockfd, &layer->fdset);
#endif
        if(layer->mappings[i].sockfd > layer->highestfd)
            layer->highestfd = layer->mappings[i].sockfd;
    }
}

/* callback triggered from the server */
static void ServerNetworkLayerTCP_closeConnection(UA_Connection *connection) {
#ifdef UA_MULTITHREADING
    if(uatomic_xchg(&connection->state, UA_CONNECTION_CLOSED) == UA_CONNECTION_CLOSED)
        return;
#else
    if(connection->state == UA_CONNECTION_CLOSED)
        return;
    connection->state = UA_CONNECTION_CLOSED;
#endif
    socket_close(connection);
    ServerNetworkLayerTCP *layer = (ServerNetworkLayerTCP*)connection->handle;
    struct DeleteList *d = malloc(sizeof(struct DeleteList));
    if(!d){
        return;
    }
    d->connection = connection;
#ifdef UA_MULTITHREADING
    while(1) {
        d->next = layer->deletes;
        if(uatomic_cmpxchg(&layer->deletes, d->next, d) == d->next)
            break;
    }
#else
    d->next = layer->deletes;
    layer->deletes = d;
#endif
}

/* call only from the single networking thread */
static UA_StatusCode ServerNetworkLayerTCP_add(ServerNetworkLayerTCP *layer, UA_Int32 newsockfd) {
    UA_Connection *c = malloc(sizeof(UA_Connection));
    if(!c)
        return UA_STATUSCODE_BADINTERNALERROR;
    UA_Connection_init(c);
    c->sockfd = newsockfd;
    c->handle = layer;
    c->localConf = layer->conf;
    c->write = socket_write;
    c->close = ServerNetworkLayerTCP_closeConnection;
    c->getBuffer = GetMallocedBuffer;
    c->releaseBuffer = ReleaseMallocedBuffer;
    c->state = UA_CONNECTION_OPENING;
    struct ConnectionMapping *nm =
        realloc(layer->mappings, sizeof(struct ConnectionMapping)*(layer->mappingsSize+1));
    if(!nm) {
        free(c);
        return UA_STATUSCODE_BADINTERNALERROR;
    }
    layer->mappings = nm;
    layer->mappings[layer->mappingsSize] = (struct ConnectionMapping){c, newsockfd};
    layer->mappingsSize++;
    return UA_STATUSCODE_GOOD;
}

static UA_StatusCode ServerNetworkLayerTCP_start(ServerNetworkLayerTCP *layer, UA_Logger *logger) {
    layer->logger = logger;
#ifdef _WIN32
    if((layer->serversockfd = socket(PF_INET, SOCK_STREAM,0)) == (UA_Int32)INVALID_SOCKET) {
        UA_LOG_WARNING((*layer->logger), UA_LOGCATEGORY_COMMUNICATION, "Error opening socket, code: %d",
                       WSAGetLastError());
        return UA_STATUSCODE_BADINTERNALERROR;
    }
#else
    if((layer->serversockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        UA_LOG_WARNING((*layer->logger), UA_LOGCATEGORY_COMMUNICATION, "Error opening socket");
        return UA_STATUSCODE_BADINTERNALERROR;
    } 
#endif
    const struct sockaddr_in serv_addr =
        {.sin_family = AF_INET, .sin_addr.s_addr = INADDR_ANY,
         .sin_port = htons(layer->port), .sin_zero = {0}};
    int optval = 1;
    if(setsockopt(layer->serversockfd, SOL_SOCKET,
                  SO_REUSEADDR, (const char *)&optval,
                  sizeof(optval)) == -1) {
        UA_LOG_WARNING((*layer->logger), UA_LOGCATEGORY_COMMUNICATION, "Error during setting of socket options");
        CLOSESOCKET(layer->serversockfd);
        return UA_STATUSCODE_BADINTERNALERROR;
    }
    if(bind(layer->serversockfd, (const struct sockaddr *)&serv_addr,
            sizeof(serv_addr)) < 0) {
        UA_LOG_WARNING((*layer->logger), UA_LOGCATEGORY_COMMUNICATION, "Error during socket binding");
        CLOSESOCKET(layer->serversockfd);
        return UA_STATUSCODE_BADINTERNALERROR;
    }
    socket_set_nonblocking(layer->serversockfd);
    listen(layer->serversockfd, MAXBACKLOG);
    UA_LOG_INFO((*layer->logger), UA_LOGCATEGORY_COMMUNICATION, "Listening on %.*s",
                layer->discoveryUrl.length, layer->discoveryUrl.data);
    return UA_STATUSCODE_GOOD;
}

/* delayed callback that frees old connections */
static void freeConnections(UA_Server *server, struct DeleteList *d) {
    while(d) {
        UA_Connection_deleteMembers(d->connection);
        free(d->connection);
        struct DeleteList *old = d;
        d = d->next;
        free(old);
    }
}

/* remove the closed sockets from the mappings array */
static void removeMappings(ServerNetworkLayerTCP *layer, struct DeleteList *d) {
    while(d) {
        size_t i = 0;
        for(; i < layer->mappingsSize; i++) {
            if(layer->mappings[i].sockfd == d->connection->sockfd)
                break;
        }
        if(i >= layer->mappingsSize)
            continue;
        layer->mappingsSize--;
        layer->mappings[i] = layer->mappings[layer->mappingsSize];
        d = d->next;
    }
}

static UA_Int32 ServerNetworkLayerTCP_getWork(ServerNetworkLayerTCP *layer, UA_WorkItem **workItems,
                                              UA_UInt16 timeout) {
    struct DeleteList *deletes;
#ifdef UA_MULTITHREADING
        deletes = uatomic_xchg(&layer->deletes, NULL);
#else
        deletes = layer->deletes;
        layer->deletes = NULL;
#endif
    removeMappings(layer, deletes);
    setFDSet(layer);
    struct timeval tmptv = {0, timeout};
    UA_Int32 resultsize = select(layer->highestfd+1, &layer->fdset, NULL, NULL, &tmptv);
    UA_WorkItem *items;
    if(resultsize < 0 || !(items = malloc(sizeof(UA_WorkItem)*(resultsize+1)))) {
        /* abort .. reattach the deletes so that they get deleted eventually.. */
#ifdef UA_MULTITHREADING
        struct DeleteList *last_delete = deletes;
        while(last_delete->next != NULL)
            last_delete = last_delete->next;
        while(1) {
            last_delete->next = layer->deletes;
            if(uatomic_cmpxchg(&layer->deletes, last_delete->next, deletes) == last_delete->next)
                break;
        }
#else
        layer->deletes = deletes;
#endif
        *workItems = NULL;
        return 0;
    }

    /* accept new connections (can only be a single one) */
    if(FD_ISSET(layer->serversockfd, &layer->fdset)) {
        resultsize--;
        struct sockaddr_in cli_addr;
        socklen_t cli_len = sizeof(cli_addr);
        int newsockfd = accept(layer->serversockfd, (struct sockaddr *) &cli_addr, &cli_len);
        int i = 1;
        setsockopt(newsockfd, IPPROTO_TCP, TCP_NODELAY, (void *)&i, sizeof(i));
        if (newsockfd >= 0) {
            socket_set_nonblocking(newsockfd);
            ServerNetworkLayerTCP_add(layer, newsockfd);
        }
    }
    
    /* read from established sockets */
    UA_Int32 j = 0;
    UA_ByteString buf = UA_BYTESTRING_NULL;
    for(size_t i = 0; i < layer->mappingsSize && j < resultsize; i++) {
        if(!(FD_ISSET(layer->mappings[i].sockfd, &layer->fdset)))
            continue;
        if(!buf.data) {
            buf.data = malloc(sizeof(UA_Byte) * layer->conf.recvBufferSize);
            buf.length = layer->conf.recvBufferSize;
            if(!buf.data)
                break;
        }
        if(socket_recv(layer->mappings[i].connection, &buf, 0) == UA_STATUSCODE_GOOD) {
            items[j].type = UA_WORKITEMTYPE_BINARYMESSAGE;
            items[j].work.binaryMessage.message = buf;
            items[j].work.binaryMessage.connection = layer->mappings[i].connection;
            buf.data = NULL;
        } else {
            items[j].type = UA_WORKITEMTYPE_CLOSECONNECTION;
            items[j].work.closeConnection = layer->mappings[i].connection;
        }
        j++;
    }

    /* add the delayed work that frees the connections */
    if(deletes) {
        items[j].type = UA_WORKITEMTYPE_DELAYEDMETHODCALL;
        items[j].work.methodCall.data = deletes;
        items[j].work.methodCall.method = (void (*)(UA_Server *server, void *data))freeConnections;
        j++;
    }

    if(buf.data)
        free(buf.data);

    /* free the array if there is no work */
    if(j == 0) {
        free(items);
        *workItems = NULL;
    } else
        *workItems = items;
    return j;
}

static UA_Int32 ServerNetworkLayerTCP_stop(ServerNetworkLayerTCP *layer, UA_WorkItem **workItems) {
    struct DeleteList *deletes;
#ifdef UA_MULTITHREADING
        deletes = uatomic_xchg(&layer->deletes, NULL);
#else
        deletes = layer->deletes;
        layer->deletes = NULL;
#endif
    removeMappings(layer, deletes);
    UA_WorkItem *items = malloc(sizeof(UA_WorkItem) * layer->mappingsSize);
    if(!items)
        return 0;
    for(size_t i = 0; i < layer->mappingsSize; i++) {
        items[i].type = UA_WORKITEMTYPE_CLOSECONNECTION;
        items[i].work.closeConnection = layer->mappings[i].connection;
    }
#ifdef _WIN32
    WSACleanup();
#endif
    *workItems = items;
    return layer->mappingsSize;
}

/* run only when the server is stopped */
static void ServerNetworkLayerTCP_delete(ServerNetworkLayerTCP *layer) {
    UA_String_deleteMembers(&layer->discoveryUrl);
    removeMappings(layer, layer->deletes);
    freeConnections(NULL, layer->deletes);
    for(size_t i = 0; i < layer->mappingsSize; i++)
        free(layer->mappings[i].connection);
    free(layer->mappings);
    free(layer);
}

UA_ServerNetworkLayer ServerNetworkLayerTCP_new(UA_ConnectionConfig conf, UA_UInt32 port) {
#ifdef _WIN32
    WORD wVersionRequested;
    WSADATA wsaData;
    wVersionRequested = MAKEWORD(2, 2);
    WSAStartup(wVersionRequested, &wsaData);
#endif
    UA_ServerNetworkLayer nl;
    memset(&nl, 0, sizeof(UA_ServerNetworkLayer));

    ServerNetworkLayerTCP *layer = malloc(sizeof(ServerNetworkLayerTCP));
    if(!layer){
        return nl;
    }
    layer->conf = conf;
    layer->mappingsSize = 0;
    layer->mappings = NULL;
    layer->port = port;
    layer->deletes = NULL;
    char hostname[256];
    gethostname(hostname, 255);
    UA_String_copyprintf("opc.tcp://%s:%d", &layer->discoveryUrl, hostname, port);

    nl.nlHandle = layer;
    nl.start = (UA_StatusCode (*)(void*, UA_Logger *logger))ServerNetworkLayerTCP_start;
    nl.getWork = (UA_Int32 (*)(void*, UA_WorkItem**, UA_UInt16))ServerNetworkLayerTCP_getWork;
    nl.stop = (UA_Int32 (*)(void*, UA_WorkItem**))ServerNetworkLayerTCP_stop;
    nl.free = (void (*)(void*))ServerNetworkLayerTCP_delete;
    nl.discoveryUrl = &layer->discoveryUrl;
    return nl;
}

/***************************/
/* Client NetworkLayer TCP */
/***************************/

UA_Connection ClientNetworkLayerTCP_connect(char *endpointUrl, UA_Logger *logger) {
    UA_Connection connection;
    UA_Connection_init(&connection);

    size_t urlLength = strlen(endpointUrl);
    if(urlLength < 11 || urlLength >= 512) {
        UA_LOG_WARNING((*logger), UA_LOGCATEGORY_COMMUNICATION, "Server url size invalid");
        return connection;
    }
    if(strncmp(endpointUrl, "opc.tcp://", 10) != 0) {
        UA_LOG_WARNING((*logger), UA_LOGCATEGORY_COMMUNICATION, "Server url does not begin with opc.tcp://");
        return connection;
    }

    UA_UInt16 portpos = 9;
    UA_UInt16 port = 0;
    for(;portpos < urlLength-1; portpos++) {
        if(endpointUrl[portpos] == ':') {
            port = atoi(&endpointUrl[portpos+1]);
            break;
        }
    }
    if(port == 0) {
        UA_LOG_WARNING((*logger), UA_LOGCATEGORY_COMMUNICATION, "Port invalid");
        return connection;
    }

    char hostname[512];
    for(int i=10; i < portpos; i++)
        hostname[i-10] = endpointUrl[i];
    hostname[portpos-10] = 0;
#ifdef _WIN32
    WORD wVersionRequested;
    WSADATA wsaData;
    wVersionRequested = MAKEWORD(2, 2);
    WSAStartup(wVersionRequested, &wsaData);
    if((connection.sockfd = socket(PF_INET, SOCK_STREAM,0)) == (UA_Int32)INVALID_SOCKET) {
#else
    if((connection.sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
#endif
        UA_LOG_WARNING((*logger), UA_LOGCATEGORY_COMMUNICATION, "Could not create socket");
        return connection;
    }
    struct hostent *server = gethostbyname(hostname);
    if(server == NULL) {
        UA_LOG_WARNING((*logger), UA_LOGCATEGORY_COMMUNICATION, "DNS lookup of %s failed", hostname);
        return connection;
    }
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    memcpy((char *)&server_addr.sin_addr.s_addr, (char *)server->h_addr_list[0], server->h_length);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if(connect(connection.sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        UA_LOG_WARNING((*logger), UA_LOGCATEGORY_COMMUNICATION, "Connection failed");
        return connection;
    }
    connection.state = UA_CONNECTION_OPENING;
    //socket_set_nonblocking(connection.sockfd);
    connection.write = socket_write;
    connection.recv = socket_recv;
    connection.close = socket_close;
    connection.getBuffer = GetMallocedBuffer;
    connection.releaseBuffer = ReleaseMallocedBuffer;
    return connection;
}
/*********************************** amalgamated original file "C:/AcpltDevelopmentKit/acplt/dev/open62541WrapperProject/open62541/examples/logger_stdout.c" ***********************************/
/*
 * This work is licensed under a Creative Commons CCZero 1.0 Universal License.
 * See http://creativecommons.org/publicdomain/zero/1.0/ for more information.
 */


static void print_time(void) {
	UA_DateTime now = UA_DateTime_now();
	UA_ByteString str;
	UA_DateTime_toString(now, &str);
	printf("%.27s", str.data); //a bit hacky way not to display nanoseconds
	UA_ByteString_deleteMembers(&str);
}

const char *LogLevelNames[6] = {"trace", "debug", "info", "warning", "error", "fatal"};
const char *LogCategoryNames[4] = {"communication", "server", "client", "userland"};

#if ((__GNUC__ == 4 && __GNUC_MINOR__ >= 6) || __GNUC__ > 4 || defined(__clang__))
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-nonliteral"
#endif
static void Logger_Stdout(UA_LogLevel level, UA_LogCategory category, const char *msg, ...) {
    printf("[");
    print_time();
    va_list ap;
    va_start(ap, msg);
    printf("] %s/%s\t", LogLevelNames[level], LogCategoryNames[category]);
    vprintf(msg, ap);
    printf("\n");
    va_end(ap);
}
#if ((__GNUC__ == 4 && __GNUC_MINOR__ >= 6) || __GNUC__ > 4 || defined(__clang__))
#pragma GCC diagnostic pop
#endif

UA_Logger Logger_Stdout_new(void) {
	return Logger_Stdout;
}
/*********************************** amalgamated original file "C:/AcpltDevelopmentKit/acplt/dev/open62541WrapperProject/open62541/src/server/ua_nodestore.c" ***********************************/

/* It could happen that we want to delete a node even though a function higher
   in the call-chain still has a reference. So we count references and delete
   once the count falls to zero. That means we copy every node to a new place
   where it is right behind the refcount integer.

   Since we copy nodes on the heap, we make the alloc for the nodeEntry bigger
   to accommodate for the different nodeclasses (the nodeEntry may have an
   overlength "tail"). */
#define ALIVE_BIT (1 << 15) /* Alive bit in the refcount */
struct nodeEntry {
    UA_UInt16 refcount;
    UA_Node node; // could be const, but then we cannot free it without compilers warnings
};

struct UA_NodeStore {
    struct nodeEntry **entries;
    UA_UInt32          size;
    UA_UInt32          count;
    UA_UInt32          sizePrimeIndex;
};


/* The size of the hash-map is always a prime number. They are chosen to be
   close to the next power of 2. So the size ca. doubles with each prime. */
static hash_t const primes[] = {
    7,         13,         31,         61,         127,         251,
    509,       1021,       2039,       4093,       8191,        16381,
    32749,     65521,      131071,     262139,     524287,      1048573,
    2097143,   4194301,    8388593,    16777213,   33554393,    67108859,
    134217689, 268435399,  536870909,  1073741789, 2147483647,  4294967291
};

static UA_Int16 higher_prime_index(hash_t n) {
    UA_UInt16 low  = 0;
    UA_UInt16 high = sizeof(primes) / sizeof(hash_t);
    while(low != high) {
        UA_UInt16 mid = low + (high - low) / 2;
        if(n > primes[mid])
            low = mid + 1;
        else
            high = mid;
    }
    return low;
}

/* Returns UA_TRUE if an entry was found under the nodeid. Otherwise, returns
   false and sets slot to a pointer to the next free slot. */
static UA_Boolean containsNodeId(const UA_NodeStore *ns, const UA_NodeId *nodeid, struct nodeEntry ***entry) {
    hash_t         h     = hash(nodeid);
    UA_UInt32      size  = ns->size;
    hash_t         index = mod(h, size);
    struct nodeEntry **e = &ns->entries[index];

    if(*e == UA_NULL) {
        *entry = e;
        return UA_FALSE;
    }

    if(UA_NodeId_equal(&(*e)->node.nodeId, nodeid)) {
        *entry = e;
        return UA_TRUE;
    }

    hash_t hash2 = mod2(h, size);
    for(;;) {
        index += hash2;
        if(index >= size)
            index -= size;

        e = &ns->entries[index];

        if(*e == UA_NULL) {
            *entry = e;
            return UA_FALSE;
        }

        if(UA_NodeId_equal(&(*e)->node.nodeId, nodeid)) {
            *entry = e;
            return UA_TRUE;
        }
    }

    /* NOTREACHED */
    return UA_TRUE;
}

/* The following function changes size of memory allocated for the entries and
   repeatedly inserts the table elements. The occupancy of the table after the
   call will be about 50%. */
static UA_StatusCode expand(UA_NodeStore *ns) {
    UA_Int32 osize = ns->size;
    UA_Int32 count = ns->count;
    /* Resize only when table after removal of unused elements is either too full or too empty.  */
    if(count * 2 < osize && (count * 8 > osize || osize <= 32))
        return UA_STATUSCODE_GOOD;


    UA_UInt32 nindex = higher_prime_index(count * 2);
    UA_Int32 nsize = primes[nindex];
    struct nodeEntry **nentries;
    if(!(nentries = UA_malloc(sizeof(struct nodeEntry *) * nsize)))
        return UA_STATUSCODE_BADOUTOFMEMORY;

    UA_memset(nentries, 0, nsize * sizeof(struct nodeEntry *));
    struct nodeEntry **oentries = ns->entries;
    ns->entries = nentries;
    ns->size    = nsize;
    ns->sizePrimeIndex = nindex;

    // recompute the position of every entry and insert the pointer
    for(UA_Int32 i=0, j=0;i<osize && j<count;i++) {
        if(!oentries[i])
            continue;
        struct nodeEntry **e;
        containsNodeId(ns, &(*oentries[i]).node.nodeId, &e);  /* We know this returns an empty entry here */
        *e = oentries[i];
        j++;
    }

    UA_free(oentries);
    return UA_STATUSCODE_GOOD;
}

/* Marks the entry dead and deletes if necessary. */
static void deleteEntry(struct nodeEntry *entry) {
    if(entry->refcount > 0)
        return;

    switch(entry->node.nodeClass) {
    case UA_NODECLASS_OBJECT:
        UA_ObjectNode_deleteMembers((UA_ObjectNode*)&entry->node);
        break;
    case UA_NODECLASS_VARIABLE:
        UA_VariableNode_deleteMembers((UA_VariableNode*)&entry->node);
        break;
    case UA_NODECLASS_METHOD:
        UA_MethodNode_deleteMembers((UA_MethodNode *)&entry->node);
        break;
    case UA_NODECLASS_OBJECTTYPE:
        UA_ObjectTypeNode_deleteMembers((UA_ObjectTypeNode*)&entry->node);
        break;
    case UA_NODECLASS_VARIABLETYPE:
        UA_VariableTypeNode_deleteMembers((UA_VariableTypeNode*)&entry->node);
        break;
    case UA_NODECLASS_REFERENCETYPE:
        UA_ReferenceTypeNode_deleteMembers((UA_ReferenceTypeNode*)&entry->node);
        break;
    case UA_NODECLASS_DATATYPE:
        UA_DataTypeNode_deleteMembers((UA_DataTypeNode*)&entry->node);
        break;
    case UA_NODECLASS_VIEW:
        UA_ViewNode_deleteMembers((UA_ViewNode*)&entry->node);
        break;
    default:
        UA_assert(UA_FALSE);
        break;
    }
    UA_free(entry);
}

/** Copies the node into the entry. Then free the original node (but not its content). */
static struct nodeEntry * nodeEntryFromNode(UA_Node *node) {
    size_t nodesize = 0;
    switch(node->nodeClass) {
    case UA_NODECLASS_OBJECT:
        nodesize = sizeof(UA_ObjectNode);
        break;
    case UA_NODECLASS_VARIABLE:
        nodesize = sizeof(UA_VariableNode);
        break;
    case UA_NODECLASS_METHOD:
        nodesize = sizeof(UA_MethodNode);
        break;
    case UA_NODECLASS_OBJECTTYPE:
        nodesize = sizeof(UA_ObjectTypeNode);
        break;
    case UA_NODECLASS_VARIABLETYPE:
        nodesize = sizeof(UA_VariableTypeNode);
        break;
    case UA_NODECLASS_REFERENCETYPE:
        nodesize = sizeof(UA_ReferenceTypeNode);
        break;
    case UA_NODECLASS_DATATYPE:
        nodesize = sizeof(UA_DataTypeNode);
        break;
    case UA_NODECLASS_VIEW:
        nodesize = sizeof(UA_ViewNode);
        break;
    default:
        UA_assert(UA_FALSE);
    }

    struct nodeEntry *newEntry;
    if(!(newEntry = UA_malloc(sizeof(struct nodeEntry) - sizeof(UA_Node) + nodesize)))
        return UA_NULL;

    UA_memcpy(&newEntry->node, node, nodesize);
    UA_free(node);
    return newEntry;
}

/**********************/
/* Exported functions */
/**********************/

UA_NodeStore * UA_NodeStore_new(void) {
    UA_NodeStore *ns;
    if(!(ns = UA_malloc(sizeof(UA_NodeStore))))
        return UA_NULL;

    ns->sizePrimeIndex = higher_prime_index(32);
    ns->size = primes[ns->sizePrimeIndex];
    ns->count = 0;
    if(!(ns->entries = UA_malloc(sizeof(struct nodeEntry *) * ns->size))) {
        UA_free(ns);
        return UA_NULL;
    }
    UA_memset(ns->entries, 0, ns->size * sizeof(struct nodeEntry *));
    return ns;
}

void UA_NodeStore_delete(UA_NodeStore *ns) {
    UA_UInt32 size = ns->size;
    struct nodeEntry **entries = ns->entries;
    for(UA_UInt32 i = 0;i < size;i++) {
        if(entries[i] != UA_NULL) {
            entries[i]->refcount &= ~ALIVE_BIT; // mark dead
            deleteEntry(entries[i]);
            entries[i] = UA_NULL;
            ns->count--;
        }
    }

    UA_free(ns->entries);
    UA_free(ns);
}

UA_StatusCode UA_NodeStore_insert(UA_NodeStore *ns, UA_Node *node, const UA_Node **inserted) {
    if(ns->size * 3 <= ns->count * 4) {
        if(expand(ns) != UA_STATUSCODE_GOOD)
            return UA_STATUSCODE_BADINTERNALERROR;
    }
    
    // get a free slot
    struct nodeEntry **slot;
    if(UA_NodeId_isNull(&node->nodeId)) {
        // find a unique nodeid that is not taken
        node->nodeId.identifierType = UA_NODEIDTYPE_NUMERIC;
        node->nodeId.namespaceIndex = 1; // namespace 1 is always in the local nodestore
        if(node->nodeClass==UA_NODECLASS_VARIABLE){ //set namespaceIndex in browseName in case id is generated
        	((UA_VariableNode*)node)->browseName.namespaceIndex=node->nodeId.namespaceIndex;
        }
        UA_Int32 identifier = ns->count+1; // start value
        UA_Int32 size = ns->size;
        hash_t increase = mod2(identifier, size);
        while(UA_TRUE) {
            node->nodeId.identifier.numeric = identifier;
            if(!containsNodeId(ns, &node->nodeId, &slot))
                break;
            identifier += increase;
            if(identifier >= size)
                identifier -= size;
        }
    } else {
        if(containsNodeId(ns, &node->nodeId, &slot))
            return UA_STATUSCODE_BADNODEIDEXISTS;
    }

    struct nodeEntry *entry = nodeEntryFromNode(node);
    if(!entry)
        return UA_STATUSCODE_BADOUTOFMEMORY;

    *slot = entry;
    ns->count++;

    if(inserted) {
        entry->refcount = ALIVE_BIT + 1;
        *inserted = &entry->node;
    } else {
        entry->refcount = ALIVE_BIT;
    }

    return UA_STATUSCODE_GOOD;
}

UA_StatusCode UA_NodeStore_replace(UA_NodeStore *ns, const UA_Node *oldNode, UA_Node *node,
                                   const UA_Node **inserted) {
    struct nodeEntry **slot;
    const UA_NodeId *nodeId = &node->nodeId;
    if(!containsNodeId(ns, nodeId, &slot))
        return UA_STATUSCODE_BADNODEIDUNKNOWN;

    // you try to replace an obsolete node (without threading this can't happen
    // if the user doesn't do it deliberately in his code)
    if(&(*slot)->node != oldNode)
        return UA_STATUSCODE_BADINTERNALERROR;

    struct nodeEntry *entry = nodeEntryFromNode(node);
    if(!entry)
        return UA_STATUSCODE_BADOUTOFMEMORY;

    (*slot)->refcount &= ~ALIVE_BIT; // mark dead
    *slot = entry;

    if(inserted) {
        entry->refcount = ALIVE_BIT + 1;
        *inserted = &entry->node;
    } else {
        entry->refcount = ALIVE_BIT;
    }
    return UA_STATUSCODE_GOOD;
}

const UA_Node * UA_NodeStore_get(const UA_NodeStore *ns, const UA_NodeId *nodeid) {
    struct nodeEntry **slot;
    if(!containsNodeId(ns, nodeid, &slot))
        return UA_NULL;
    (*slot)->refcount++;
    return &(*slot)->node;
}

UA_StatusCode UA_NodeStore_remove(UA_NodeStore *ns, const UA_NodeId *nodeid) {
    struct nodeEntry **slot;
    if(!containsNodeId(ns, nodeid, &slot))
        return UA_STATUSCODE_BADNODEIDUNKNOWN;

    // Check before if deleting the node makes the UA_NodeStore inconsistent.
    (*slot)->refcount &= ~ALIVE_BIT; // mark dead
    deleteEntry(*slot);
    *slot = UA_NULL;
    ns->count--;

    /* Downsize the hashmap if it is very empty */
    if(ns->count * 8 < ns->size && ns->size > 32)
        expand(ns); // this can fail. we just continue with the bigger hashmap.

    return UA_STATUSCODE_GOOD;
}

void UA_NodeStore_iterate(const UA_NodeStore *ns, UA_NodeStore_nodeVisitor visitor) {
    for(UA_UInt32 i = 0;i < ns->size;i++) {
        if(ns->entries[i] != UA_NULL)
            visitor(&ns->entries[i]->node);
    }
}

void UA_NodeStore_release(const UA_Node *managed) {
    /* We know what we are doing here and remove a compiler warning. Nobody has
       a reference to the const pointer, so we can free it. */
    struct nodeEntry *entry = (struct nodeEntry *) ((uintptr_t)managed - offsetof(struct nodeEntry, node));
    entry->refcount--;
    deleteEntry(entry);
}
