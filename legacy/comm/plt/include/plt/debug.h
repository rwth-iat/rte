/* -*-plt-c++-*- */
#ifndef PLT_DEBUG_INCLUDED
#define PLT_DEBUG_INCLUDED
/* $Header: /home/david/cvs/acplt/plt/include/plt/debug.h,v 1.22 2007-04-25 12:57:21 martin Exp $ */
/*
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
/* 
 * plt/debug.h provides some simple macros that should aid debugging.
 */
#include "plt/config.h"

#ifdef NDEBUG
#define PLT_DEBUG 0
#else
#define PLT_DEBUG 1
#endif

#if !PLT_DEBUG

#define PLT_DEBUG_PRECONDITIONS    0
#define PLT_DEBUG_POSTCONDITIONS   0
#define PLT_DEBUG_NEW              0
#define PLT_DEBUG_INVARIANTS       0
#define PLT_DEBUG_PEDANTIC         0
#define PLT_DEBUG_VERBOSE          0

#else /* !PLT_DEBUG */

#ifndef PLT_DEBUG_PRECONDITIONS
#define PLT_DEBUG_PRECONDITIONS 1
#endif

#ifndef PLT_DEBUG_POSTCONDITIONS
#define PLT_DEBUG_POSTCONDITIONS 1
#endif

#ifndef PLT_DEBUG_INVARIANTS
#define PLT_DEBUG_INVARIANTS 1
#endif

#ifndef PLT_DEBUG_NEW
#define PLT_DEBUG_NEW 1
#endif

#ifndef PLT_DEBUG_PEDANTIC
#define PLT_DEBUG_PEDANTIC 0
#endif

#ifndef PLT_DEBUG_VERBOSE
#define PLT_DEBUG_VERBOSE 1
#endif

#endif /* !PLT_DEBUG */


/*****************************************************************/

#if PLT_DEBUG && defined(__cplusplus)

#include "plt/logstream.h"
extern PltLogStream plt_dlog;

#include <iostream.h>
#include <iomanip.h>

#ifdef __cplusplus
extern "C"
#endif
void plt_canthappen(const char *what, const char *file, int line);


#define PLT_FAILED_ASSERTION(expr_str,file,line) \
     plt_canthappen("assertion failed: "##expr_str,file,line)

#define PLT_ASSERT(expr) \
  ((expr) ? (void)0 : PLT_FAILED_ASSERTION(#expr,__FILE__,__LINE__))

#else

#define PLT_ASSERT(x) ((void)0)

#endif

#if PLT_DEBUG_PRECONDITIONS
#define PLT_PRECONDITION(x) PLT_ASSERT(x)
#else
#define PLT_PRECONDITION(x) ((void)0)
#endif

#if PLT_DEBUG_POSTCONDITIONS
#define PLT_POSTCONDITION(x) PLT_ASSERT(x)
#else
#define PLT_POSTCONDITION(x) ((void)0)
#endif

/*
 * Classes should implement virtual bool invariant() const
 * invariant() is a predicate which must be true at the end of each
 * method invocation (and should be checked before return with
 * PLT_CHECK_INVARIANT). invariant() should check the invariants of
 * parent classes. To avoid unnecessary vtables in production
 * code, declaration and definition of invariant() has to be in
 * #if PLT_DEBUG_INVARIANTS / #endif pairs.
 */

#ifdef __cplusplus

#if PLT_DEBUG_INVARIANTS
#define PLT_CHECK_INVARIANT() PLT_ASSERT(this->invariant())
#else
#define PLT_CHECK_INVARIANT() ((void)0)
#endif

/********************************************************************/
#if PLT_DEBUG_NEW
/********************************************************************/

#ifndef PLT_DEBUG_NEW_REPORT
#define PLT_DEBUG_NEW_REPORT true
#endif

class PltDebugMemState {
public:
    size_t newcount;
    size_t deletecount;
    size_t newed;
    size_t deleted;
};
    
class PltDebugNewTracker
{
    friend void * operator new(size_t);
    friend void operator delete(void *);
#if PLT_ARRAY_NEW_OVERLOADABLE 
    friend void * operator new[](size_t);
    friend void operator delete[](void *);
#endif

public:    
    PltDebugNewTracker();
    ~PltDebugNewTracker();

    static void memState(PltDebugMemState &);
private:
    static size_t refcount;
    static size_t newcount;
    static size_t deletecount;
    static size_t newed;
    static size_t deleted;
    static size_t max;
    static size_t inuse();
    static bool report_always;
};

void * operator new(size_t);
void operator delete(void *);

#if PLT_ARRAY_NEW_OVERLOADABLE
void * operator new[](size_t);
void operator delete[](void *);
#endif

static PltDebugNewTracker plt_debug_new_tracker;

/********************************************************************/
#endif //PLT_DEBUG_NEW
/********************************************************************/

#if PLT_DEBUG_VERBOSE

#define PLT_DMSG(inserters) cerr<<inserters
#define PLT_DMSG_ADD(inserters) plt_dlog << inserters
#define PLT_DMSG_END plt_dlog.debug()

#else

#define PLT_DMSG(inserters) ((void)0)
#define PLT_DMSG_ADD(inserters) ((void)0)
#define PLT_DMSG_END ((void)0)

#endif

#endif /* __cplusplus */

/********************************************************************/
#endif /* PLT_DEBUG_INCLUDED */

/* End of plt/debug.h */
