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

#include <assert.h>
#include <plt/config.h>

#ifdef NDEBUG

#define PLT_DEBUG_PRECONDITIONS 0
#define PLT_DEBUG_POSTCONDITIONS 0
#define PLT_DEBUG_INVARIANTS 0

#else /* NDEBUG */

#ifndef PLT_DEBUG_PRECONDITIONS
#define PLT_DEBUG_PRECONDITIONS 1
#endif

#ifndef PLT_DEBUG_POSTCONDITIONS
#define PLT_DEBUG_POSTCONDITIONS 1
#endif

#ifndef PLT_DEBUG_INVARIANTS
#define PLT_DEBUG_INVARIANTS 1
#endif

#endif /* NDEBUG */

#if PLT_DEBUG_PRECONDITIONS
#define PLT_PRECONDITION(x) assert(x)
#else
#define PLT_PRECONDITION(x) ((void)0)
#endif

#if PLT_DEBUG_POSTCONDITIONS
#define PLT_POSTCONDITION(x) assert(x)
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
 * code declaration and definition of invariant() has to be in
 * #if PLT_DEBUG_INVARIANTS / #endif pairs.
 */

class PltDebuggable {
protected:
#if PLT_DEBUG_INVARIANTS
    virtual bool invariant() const =0;
#endif
};

#if PLT_DEBUG_INVARIANTS
#define PLT_CHECK_INVARIANT() assert(this->invariant())
#else
#define PLT_CHECK_INVARIANT() ((void)0)
#endif

#endif /* __cplusplus */

#endif /* Header */
