/* -*-plt-c++-*- */
#ifndef PLT_DEBUG_INCLUDED
#define PLT_DEBUG_INCLUDED
/* $Header: /home/david/cvs/acplt/plt/include/plt/debug.h,v 1.12 1997-03-23 17:20:09 martin Exp $ */
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
/* 
//////////////////////////////////////////////////////////////////////
 * plt/debug.h provides some simple macros that should aid debugging.
//////////////////////////////////////////////////////////////////////
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

#endif /* !PLT_DEBUG */


/*****************************************************************/

#if PLT_DEBUG

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

//////////////////////////////////////////////////////////////////////
#if PLT_DEBUG_NEW
//////////////////////////////////////////////////////////////////////

#ifndef PLT_DEBUG_NEW_REPORT
#define PLT_DEBUG_NEW_REPORT true
#endif

class PltDebugNewTracker
{
    friend void * operator new(size_t);
    friend void operator delete(void *);
    friend void * operator new[](size_t);
    friend void operator delete[](void *);

public:    
    PltDebugNewTracker();
    ~PltDebugNewTracker();

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
void * operator new[](size_t);
void operator delete[](void *);

static PltDebugNewTracker plt_debug_new_tracker;

//////////////////////////////////////////////////////////////////////
#endif //PLT_DEBUG_NEW
//////////////////////////////////////////////////////////////////////

#endif /* __cplusplus */

//////////////////////////////////////////////////////////////////////
#endif /* PLT_DEBUG_INCLUDED */
