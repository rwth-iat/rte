/* -*-plt-c++-*- */
/* 
 *  Copyright (c) 1996 PLT, RWTH-Aachen, Germany. See file copy_plt.txt 
 *  Author: Martin Kneissl <martin@plt.rwth-aachen.de>
 *
 */

#ifndef PLT_DEBUG_INCLUDED
#define PLT_DEBUG_INCLUDED

/* 
 * File debug.h provides some simple macros that should aid debugging.
 * 
 */

#include <plt/config.h>

#ifdef NDEBUG
#define PLT_DEBUG 0
#else
#define PLT_DEBUG 1
#endif

#if !PLT_DEBUG

#define PLT_DEBUG_PRECONDITIONS 0
#define PLT_DEBUG_POSTCONDITIONS 0
#define PLT_DEBUG_INVARIANTS 0
#define PLT_DEBUG_SLOW 0

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

#ifndef PLT_DEBUG_SLOW
#define PLT_DEBUG_SLOW 0
#endif

#endif /* !PLT_DEBUG */

#ifdef __cplusplus
extern "C"
#endif
void plt_canthappen(const char *what, const char *file, int line);

#if PLT_DEBUG

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

#ifdef __cplusplus

/* 
 * class PLTDebuggable is an abstract class that represents objects
 * which can be debugged.
 * 
 * invariant() is a predicate which must be true at the end of each
 * method invocation (and should be checked before return with
 * PLT_CHECK_INVARIANT). invariant() should check the invariants of
 * parent classes. To avoid unnecessary virtual calls in production
 * code, declaration and definition of invariant() has to be in
 * #if PLT_DEBUG_INVARIANTS / #endif pairs.
 */

class PltDebuggable {
protected:
#if PLT_DEBUG_INVARIANTS
    virtual bool invariant() const =0;
#endif
};

#if PLT_DEBUG_INVARIANTS
#define PLT_CHECK_INVARIANT() PLT_ASSERT(this->invariant())
#else
#define PLT_CHECK_INVARIANT() ((void)0)
#endif

#endif /* __cplusplus */

#endif /* Header */
