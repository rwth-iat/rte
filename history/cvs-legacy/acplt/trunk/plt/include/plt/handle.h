/* -*-plt-c++-*- */
#ifndef PLT_HANDLE_INCLUDED
#define PLT_HANDLE_INCLUDED
/* $Header: /home/david/cvs/acplt/plt/include/plt/handle.h,v 1.6 1997-03-06 12:14:25 martin Exp $ */
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

//////////////////////////////////////////////////////////////////////
// plt/handle.h provides memory managing pointer replacements
//////////////////////////////////////////////////////////////////////

#include <limits.h>
#include <stdlib.h>

#include "plt/debug.h"

//////////////////////////////////////////////////////////////////////
// Plt...Handle<T>
//////////////////////////////////////////////////////////////////////
//
// These classes provide an easier management of object ownership:
// they can handle malloced and newed objects semi-automatically by
// doing reference counting. Used with care they can also hold
// objects with other duration.
//
// Three kinds of memory ownership are provided:
// - unmanaged storage: No attempt will be made to free this objects
//   use PltOsUnmanaged
// - malloced storage: will be freed with free()
//   use PltOsMalloc
// - newed storage: will be freed with delete or delete[].
//   use PltOsNew
//
// Two templates are provided:
// - PltPtrHandle<T> acts like a pointer
// - PltArrayHandle<T> acts like an array of T
//
// How to use these classes
// ------------------------
//
// Create an object:
//   int * pi = new int(3);
//
// Create an unbound handle:
//   PltPtrHandle<int> hi;
//
// Try to bind the object and the handle specifying how the object has been
// created:
//   bool success = hi.bindTo(pi, PltOsNew);
// (bindTo may fail due to lack of free space).
//
// Don't use the pointer pi any more!
//
// Use the PltPtrHandle<int> like a pointer to int:
//   int a = *hi + 3;
// 
// You can safely create another handle to the object:
//   PltPtrHandle<int> hk(hi);
//
// The object will be deleted when all handles to it are deleted or
// go out of scope. 
//
//
// You can construct a bound handle for an object that you are creating.
// This contructor must be used with care, semantics follow:
//
//    Plt...Handle(T *p, PltOwnership os);
// 
// p should point to an object you own. os specifies how the object has
// been created. 
// If there is enough memory for binding the handle,
// the constructed handle is bound to the object p points to.
// If there is not enough memory to bind, the object pointed to by
// p WILL BE DESTROYED IMMEDIATELY by this constructor and an unbound
// handle is constructed.
//
// Why is this dangerous operation implemented? Often you need a handle
// for an object you are creating. If you can't get the handle you don't
// need the object. So you would delete the object anyway. In this case
// you should use this constructor which will do all the work for you.
// Example:
//   PltHandle<int> hi(new int(3), PltOsNew);
//   if (hi) {
//       ... // use it
//   } else {
//       // failed to bind the handle, int(3) is already deleted
//   }
//
// Remarks:
// --------
//
// [1] Be careful with the pointer to the representation that you
//     get. Object lifetime is controled by the Plt...Handle object!
//
//
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// forward declaration
template<class T> class PltHandle;
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////

enum PltOwnership 
{ 
    PltOsUnmanaged,         // Objects won't be freed or deleted
    PltOsMalloc,            // Objects will be free()d
    PltOsNew,               // Objects will be deleted
    PltOsArrayNew           // Objects will be delete[]d
    };

//////////////////////////////////////////////////////////////////////
// Pointer-like handle
//////////////////////////////////////////////////////////////////////

template<class T>
class PltPtrHandle
: private PltHandle<T>
{
public:
    PltPtrHandle();
    PltPtrHandle(T *p, enum PltOwnership);  // no default to avoid conversion!
    PltPtrHandle(const PltPtrHandle &);
    
    // accessors
    operator bool () const;
    T& operator*() const;
    T* operator->() const;
    T* getPtr() const;                                         // [1]
  
    
    // modifiers
    PltPtrHandle & operator=(PltPtrHandle &rhs);
    bool bindTo(T *, enum PltOwnership = PltOsNew);
};

//////////////////////////////////////////////////////////////////////
// Array-like handle
//////////////////////////////////////////////////////////////////////

template<class T>
class PltArrayHandle
: private PltHandle<T>
{
public:
    PltArrayHandle();
    PltArrayHandle(T *p, enum PltOwnership); // no default to avoid conversion!
    PltArrayHandle(const PltArrayHandle &);

    operator bool () const;
    T& operator[](size_t) const;
    T* getPtr() const;                                          // [1]

    PltArrayHandle & operator=(PltArrayHandle &rhs);
    bool bindTo(T *, enum PltOwnership = PltOsArrayNew);
};

//////////////////////////////////////////////////////////////////////
// Implementation
//////////////////////////////////////////////////////////////////////


template<class T>
class PltHandle
{
protected:
    PltHandle(); 
    PltHandle(const PltHandle &);
#ifdef PLT_DEBUG_INVARIANTS
    virtual
#endif
    ~PltHandle();

    // accessor
    operator bool () const;
    T* getPtr() const; 
    // ^^ CAUTION: DON'T STORE ANY REFERENCES TO THE REPRESENTATION

    // modifiers
    PltHandle & operator=(PltHandle &rhs);
    bool bindTo(T *, enum PltOwnership = PltOsNew);

    void addRef();
    void removeRef();

    // static helper
    void destroy(T *p, PltOwnership os);
    
    // state representation
    T *prep;
    struct AllocTracker 
        {
            unsigned count;
            enum PltOwnership type;
            AllocTracker(enum PltOwnership t) : count(1), type(t) {}
        } *palloc;

#if PLT_DEBUG_INVARIANTS
public:
    virtual bool invariant() const;
#endif
};


//////////////////////////////////////////////////////////////////////

template<class T>
inline
PltHandle<T>::operator bool() const
{
    return prep != 0;
}


//////////////////////////////////////////////////////////////////////

#if PLT_DEBUG_INVARIANTS
template<class T>
inline bool 
PltHandle<T>::invariant() const 
{
    return (prep==0 && palloc==0) 
        || (prep && (palloc ? palloc->count > 0 : true));
}
#endif

//////////////////////////////////////////////////////////////////////
// Add one reference to the handled object

template<class T>
inline void 
PltHandle<T>::addRef()
{
    PLT_PRECONDITION(palloc ? palloc->count<UINT_MAX : true);
    if (*this && palloc) {
        palloc->count++;
    }
    PLT_CHECK_INVARIANT();
}


//////////////////////////////////////////////////////////////////////

template<class T>
inline void 
PltHandle<T>::destroy(T *p, PltOwnership os) // static member fn
{
    if (p) {
        switch (os) {
        case PltOsNew:
            delete p;
            break;
        case PltOsArrayNew:
            delete[] p;
            break;
        case PltOsMalloc:
            free(p);
            break;
        default:
            PLT_ASSERT(0==1);
        }
    }
}

//////////////////////////////////////////////////////////////////////
// Remove one reference to the handled object.

template<class T>
inline void
PltHandle<T>::removeRef()
{
    if ( *this && palloc && --(palloc->count) == 0) {
        destroy(prep, palloc->type);
        prep = 0;
        delete palloc; palloc = 0;
    }
    PLT_CHECK_INVARIANT();
}


//////////////////////////////////////////////////////////////////////
// Bind to a new object.
// If binding succeeds p is no longer valid.
// Returns true on success.

template <class T>
inline bool
PltHandle<T>::bindTo(T* p, enum PltOwnership t)
{
    bool ok = true;

    // unbind old handled object
    removeRef();

    PLT_ASSERT( palloc == 0 );

    // bind new handled object
    if (p && t != PltOsUnmanaged) {
        // only managed non-NULL objects get an AllocTracker
        palloc = new AllocTracker(t);
        if (!palloc) {
            ok = false;
        }
    }
    if (ok) {
        prep = p;
    }
    PLT_CHECK_INVARIANT();
    return ok;
}
        
//////////////////////////////////////////////////////////////////////
// Create a PltHandle

template<class T>
inline
PltHandle<T>::PltHandle() 
: prep(0), 
  palloc(0)
{
    PLT_CHECK_INVARIANT();
}



//////////////////////////////////////////////////////////////////////
// Copy constructor. The handled object -- if any -- gets an 
// additional reference.

template<class T>
inline
PltHandle<T>::PltHandle(const PltHandle &h)
: prep(h.prep), palloc(h.palloc)
{
    if (h) {
        addRef();
    }
    PLT_CHECK_INVARIANT();
}

//////////////////////////////////////////////////////////////////////
// Destructor. If the handle is bound it removes one reference 
// from the handled object.

template<class T>
inline
PltHandle<T>::~PltHandle()
{
    removeRef();
    PLT_CHECK_INVARIANT();
}

//////////////////////////////////////////////////////////////////////
//  Assignment
template<class T>
inline PltHandle<T> & 
PltHandle<T>::operator=(PltHandle &rhs) 
{
    // order is important if called as 'handle = handle;'
    rhs.addRef();
    removeRef();
    
    palloc = rhs.palloc;
    prep = rhs.prep;

    PLT_CHECK_INVARIANT();
    return *this;
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline T * 
PltHandle<T>::getPtr () const
{
    return prep;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

template <class T>
inline
PltPtrHandle<T>::PltPtrHandle()
{
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline
PltPtrHandle<T>::PltPtrHandle(T *p, enum PltOwnership os) 
{
    PLT_PRECONDITION(os==PltOsUnmanaged || os==PltOsMalloc 
                     || os==PltOsNew);
    if (! bindTo(p, os)) {
        destroy(p, os);
    }
    PLT_CHECK_INVARIANT();
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline
PltPtrHandle<T>::PltPtrHandle(const PltPtrHandle &h)
: PltHandle<T>(h)
{
}



//////////////////////////////////////////////////////////////////////

template <class T>
inline
PltPtrHandle<T>::operator bool() const
{
    return PltHandle<T>::operator bool();
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline T *
PltPtrHandle<T>::getPtr () const
{
    return PltHandle<T>::getPtr();
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline PltPtrHandle<T> & 
PltPtrHandle<T>::operator = ( PltPtrHandle &h)
{
    return (PltPtrHandle<T> &) (PltHandle<T>::operator = ( h )); 
    // static cast
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline bool
PltPtrHandle<T>::bindTo(T * p, enum PltOwnership t)
{
    PLT_PRECONDITION(t==PltOsUnmanaged || t==PltOsMalloc 
                     || t==PltOsNew);
    return PltHandle<T>::bindTo(p, t); // forward to parent
}

//////////////////////////////////////////////////////////////////////
// Derefencing. Note that it is ILLEGAL to hold a pointer/reference
// to the representation!

template<class T>
inline T *
PltPtrHandle<T>::operator->() const
{
    return *this ? getPtr() : 0;
}

//////////////////////////////////////////////////////////////////////
// Derefencing. Note that it is ILLEGAL to hold a pointer/reference
// to the representation!

template<class T>
inline T &
PltPtrHandle<T>::operator*() const
{
    PLT_PRECONDITION( *this );
    return *(operator->());
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

template <class T>
inline
PltArrayHandle<T>::PltArrayHandle()
: PltHandle<T>()
{
}

//////////////////////////////////////////////////////////////////////
template <class T>
inline
PltArrayHandle<T>::PltArrayHandle(T *p, enum PltOwnership os) 
{
    PLT_PRECONDITION(os==PltOsUnmanaged || os==PltOsMalloc 
                     || os==PltOsArrayNew);
    if (! bindTo(p, os)) {
        destroy(p, os);
    }
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline
PltArrayHandle<T>::PltArrayHandle(const PltArrayHandle & h)
: PltHandle<T>(h)
{
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline
PltArrayHandle<T>::operator bool() const
{
    return PltHandle<T>::operator bool();
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline T*
PltArrayHandle<T>::getPtr () const
{
    return PltHandle<T>::getPtr();
}

//////////////////////////////////////////////////////////////////////
template <class T>
inline PltArrayHandle<T> & 
PltArrayHandle<T>::operator = ( PltArrayHandle &h)
{
    return (PltArrayHandle<T> &) PltHandle<T>::operator = (h);
    // static cast
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline bool
PltArrayHandle<T>::bindTo(T * p, enum PltOwnership t)
{
    PLT_PRECONDITION(t==PltOsUnmanaged || t==PltOsMalloc 
                     || t==PltOsArrayNew );
    return PltHandle<T>::bindTo(p,t); // forward to parent
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline T & 
PltArrayHandle<T>::operator[](size_t i) const
{
    PLT_PRECONDITION( *this );
    return getPtr()[i];
}

//////////////////////////////////////////////////////////////////////

#endif // PLT_HANDLE_INCLUDED
