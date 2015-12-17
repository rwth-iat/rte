/* -*-plt-c++-*- */
#ifndef KS_HANDLE_INCLUDED
#define KS_HANDLE_INCLUDED
/*
 * Copyright (c) 1996, 1997, 1998, 1999
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
    return *this ? KsPtrHandle<T>::getPtr()->xdrEncode(xdr) : false;
}

//////////////////////////////////////////////////////////////////////

#if PLT_SEE_ALL_TEMPLATES
#include "ks/handle_impl.h"
#endif

//////////////////////////////////////////////////////////////////////
#endif // PLT_HANDLE_INCLUDED
