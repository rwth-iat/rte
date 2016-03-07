/* -*-plt-c++-*- */
#ifndef PLT_HANDLE_INCLUDED
#define PLT_HANDLE_INCLUDED
/* $Header: /home/david/cvs/acplt/plt/include/plt/handle.h,v 1.21 2008-09-22 08:26:09 henning Exp $ */
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
// plt/handle.h provides memory managing pointer replacements
//////////////////////////////////////////////////////////////////////

#include <limits.h>
#include <stdlib.h>

#include "plt/alloc.h"

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

enum PltOwnership 
{ 
    PltOsUnmanaged,         // Objects won't be freed or deleted
    PltOsMalloc,            // Objects will be free()d
    PltOsNew,               // Objects will be deleted
    PltOsArrayNew           // Objects will be delete[]d
    };


//////////////////////////////////////////////////////////////////////
//// BEGIN OF INTERNAL BASE CLASSES PART
//////////////////////////////////////////////////////////////////////

struct Plt_AllocTracker 
{
    virtual ~Plt_AllocTracker() { }
    unsigned count;
    enum PltOwnership type;
    Plt_AllocTracker() : count(1) {}
    virtual void destroy(void * p) const = 0;
private:
    static PltAllocator<Plt_AllocTracker> _allocator;
public:
    void * operator new(size_t 
#if PLT_DEBUG
                        sz
#endif
                        )
        {
            PLT_ASSERT(sz==sizeof (Plt_AllocTracker));
            return _allocator.alloc();
        }
    void operator delete(void * ptr)
        {
            _allocator.free(ptr);
        }
};

struct Plt_AtMalloc
: public Plt_AllocTracker
{
    virtual void destroy(void *p) const { free(p); }
};

template <class T>
struct Plt_AtNew
: public Plt_AllocTracker
{
    virtual void destroy(void *p) const
        { 
            delete (T*)p;
        }
};

template <class T>
struct Plt_AtArrayNew
: public Plt_AllocTracker
{
    virtual void destroy (void *p) const
        {
            delete[] (T*) p;
        }
};

//////////////////////////////////////////////////////////////////////

class PltHandle_base
{
    static PltAllocator<PltHandle_base> _allocator;
protected:
    PltHandle_base(); 
    PltHandle_base(const PltHandle_base &);
#ifdef PLT_DEBUG_INVARIANTS
    virtual
#endif
    ~PltHandle_base();

    // accessor
    operator bool () const;
    unsigned getRefCount() const;
    void * getPtr() const; 
    // ^^ CAUTION: DON'T STORE ANY REFERENCES TO THE REPRESENTATION

    // modifiers
    PltHandle_base & operator = (const PltHandle_base &rhs);
    void bindTo(void *, Plt_AllocTracker * a);

    void addRef() const;
    void removeRef();

    // state representation
    void *prep;
    Plt_AllocTracker *palloc;

    void * operator new(size_t
#if PLT_DEBUG
                        sz
#endif
                        )
        {
            PLT_ASSERT(sz==sizeof (PltHandle_base));
            return _allocator.alloc();
        }
    void operator delete(void * ptr)
        {
            _allocator.free(ptr);
        }
    
#if PLT_DEBUG_INVARIANTS
public:
    virtual bool invariant() const;
#endif
};

//////////////////////////////////////////////////////////////////////

template <class T>
class PltHandle
: public PltHandle_base
{
protected:
    PltHandle() : PltHandle_base() { }
    PltHandle(PltHandle &r) : PltHandle_base(r) { }
    PltHandle(T *, enum PltOwnership);
    bool bindTo(T *, enum PltOwnership);
};

//////////////////////////////////////////////////////////////////////
//// END OF INTERNAL BASE CLASSES PART
//////////////////////////////////////////////////////////////////////

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

    bool operator == (const PltPtrHandle &rhs) const;
    bool operator != (const PltPtrHandle &rhs) const;
    
    // modifiers
    PltPtrHandle & operator =(const PltPtrHandle &rhs);
    bool bindTo(T *, enum PltOwnership = PltOsNew);

    void * operator new (size_t size) 
        { return PltHandle<T>::operator new(size); }
    void operator delete (void * ptr) 
        { PltHandle<T>::operator delete(ptr); }
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

    bool operator == (const PltArrayHandle &rhs) const;
    bool operator != (const PltArrayHandle &rhs) const;
    
    PltArrayHandle & operator = (const PltArrayHandle &rhs);
    bool bindTo(T *, enum PltOwnership = PltOsArrayNew);

    void * operator new (size_t size) 
        { return PltHandle<T>::operator new(size); }
    void operator delete (void * ptr) 
        { PltHandle<T>::operator delete(ptr); }
};

//////////////////////////////////////////////////////////////////////
// Implementation
//////////////////////////////////////////////////////////////////////

inline
PltHandle_base::operator bool() const
{
    return prep != 0;
}

//////////////////////////////////////////////////////////////////////
// Create a PltHandle

inline
PltHandle_base::PltHandle_base() 
: prep(0), 
  palloc(0)
{
    PLT_CHECK_INVARIANT();
}

//////////////////////////////////////////////////////////////////////
// Destructor. If the handle is bound it removes one reference 
// from the handled object.

inline
PltHandle_base::~PltHandle_base()
{
    removeRef();
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
: PltHandle<T>(p, os)
{
    PLT_PRECONDITION(os==PltOsUnmanaged || os==PltOsMalloc 
                     || os==PltOsNew);
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline
PltPtrHandle<T>::PltPtrHandle(const PltPtrHandle &h)
: PltHandle<T>( (PltHandle<T> &) h)
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
    return (T *) PltPtrHandle<T>::prep;
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline PltPtrHandle<T> & 
PltPtrHandle<T>::operator = (const PltPtrHandle &h)
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
// operators == and != compare the pointers the handles manage.
template<class T>
inline bool
PltPtrHandle<T>::operator==(const PltPtrHandle &rhs) const
{
    return getPtr() == rhs.getPtr();
}

template<class T>
inline bool
PltPtrHandle<T>::operator!=(const PltPtrHandle &rhs) const
{
    return !(*this == rhs);
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
: PltHandle<T>(p,os)
{
    PLT_PRECONDITION(os==PltOsUnmanaged || os==PltOsMalloc 
                     || os==PltOsArrayNew);

}

//////////////////////////////////////////////////////////////////////

template <class T>
inline
PltArrayHandle<T>::PltArrayHandle(const PltArrayHandle & h)
: PltHandle<T>( (PltHandle<T> &) h)
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
    return (T*) PltArrayHandle<T>::prep;
    // static cast
}

//////////////////////////////////////////////////////////////////////
template <class T>
inline PltArrayHandle<T> & 
PltArrayHandle<T>::operator = ( const PltArrayHandle &h)
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
// operators == and != compare the pointers the handles manage.
template<class T>
inline bool
PltArrayHandle<T>::operator==(const PltArrayHandle &rhs) const
{
    return getPtr() == rhs.getPtr();
}

template<class T>
inline bool
PltArrayHandle<T>::operator!=(const PltArrayHandle &rhs) const
{
    return !(*this == rhs);
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

#if PLT_SEE_ALL_TEMPLATES
#include "plt/handle_impl.h"
#endif

//////////////////////////////////////////////////////////////////////

#endif // PLT_HANDLE_INCLUDED
