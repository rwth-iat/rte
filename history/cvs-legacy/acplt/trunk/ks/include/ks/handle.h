/* -*-plt-c++-*- */
#ifndef KS_HANDLE_INCLUDED
#define KS_HANDLE_INCLUDED
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
// ks/handle.h provides memory managing pointer replacements with xdr
//////////////////////////////////////////////////////////////////////

#include "plt/handle.h"
#include "ks/xdr.h"

//////////////////////////////////////////////////////////////////////
// Ks...Handle<T>
//////////////////////////////////////////////////////////////////////
//
// These classes provide an easier management of object ownership:
// they can handle malloced and newed objects semi-automatically by
// doing reference counting. Used with care they can also hold
// objects with other duration.
//
// Three kinds of memory ownership are provided:
// - unmanaged storage: No attempt will be made to free this objects
//   use KsOsUnmanaged
// - malloced storage: will be freed with free()
//   use KsOsMalloc
// - newed storage: will be freed with delete or delete[].
//   use KsOsNew
//
// Two templates are provided:
// - KsPtrHandle<T> acts like a pointer
// - KsArrayHandle<T> acts like an array of T
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

#define KsOwnership       PltOwnership
#define KsOsUnmanaged     PltOsUnmanaged
#define KsOsMalloc        PltOsMalloc
#define KsOsNew           PltOsNew
#define KsOsArrayNew      PltOsArrayNew

//////////////////////////////////////////////////////////////////////
// Pointer-like handle
//////////////////////////////////////////////////////////////////////

template<class T>
class KsPtrHandle
: public PltPtrHandle<T>,
  public KsXdrAble
{
public:
    KsPtrHandle();
    KsPtrHandle(T *p, enum KsOwnership);  // no default to avoid conversion!
    KsPtrHandle(const KsPtrHandle &);

    // KsXdrAble interface
    KsPtrHandle(XDR *, bool & success);
    bool xdrEncode(XDR *) const;
    bool xdrDecode(XDR *);
    static KsPtrHandle * xdrNew(XDR *);
};

//////////////////////////////////////////////////////////////////////
// Implementation
//////////////////////////////////////////////////////////////////////

template <class T>
inline
KsPtrHandle<T>::KsPtrHandle()
{
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline
KsPtrHandle<T>::KsPtrHandle(T *p, enum KsOwnership os) 
: PltPtrHandle<T>(p,os)
{
    PLT_PRECONDITION(os==KsOsUnmanaged || os==KsOsMalloc 
                     || os==KsOsNew);
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline
KsPtrHandle<T>::KsPtrHandle(const KsPtrHandle &h)
: PltPtrHandle<T>(h)
{
}


//////////////////////////////////////////////////////////////////////

template <class T>
inline bool
KsPtrHandle<T>::xdrEncode(XDR *xdr) const
{
    return *this ? getPtr()->xdrEncode(xdr) : false;
}


//////////////////////////////////////////////////////////////////////
#endif // PLT_HANDLE_INCLUDED
