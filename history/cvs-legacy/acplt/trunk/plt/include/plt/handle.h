/* -*-plt-c++-*- */
/* 
 *  Copyright (c) 1996,1997 PLT, RWTH-Aachen, Germany. See file copy_plt.txt!
 *  Author: Martin Kneissl <martin@plt.rwth-aachen.de>
 *
 */
#ifndef PLT_HANDLE_INCLUDED
#define PLT_HANDLE_INCLUDED

#include <limits.h>
#include <stdlib.h>

#include <plt/debug.h>

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
// Try to bind the object and the handle spec. how the object has been
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
    PltPtrHandle(T *p, enum PltOwnership); // no default to avoid conversion!
    PltPtrHandle(const PltPtrHandle &);
    
    // accessors
    operator bool () const;
    T& operator*() const;
    T* operator->() const;
    T* getPtr() const; 
    // ^^ CAUTION: DON'T STORE ANY REFERENCES TO THE REPRESENTATION
    
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
    T* getPtr() const; 
    // ^^ CAUTION: DON'T STORE ANY REFERENCES TO THE REPRESENTATION

    PltArrayHandle & operator=(PltArrayHandle &rhs);
    bool bindTo(T *, enum PltOwnership = PltOsArrayNew);
};

//////////////////////////////////////////////////////////////////////
// Implementation
//////////////////////////////////////////////////////////////////////


template<class T>
class PltHandle : virtual public PltDebuggable {
protected:
    PltHandle(); 
    PltHandle(const PltHandle &);
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
        } *palloc;

#if PLT_DEBUG_INVARIANTS
protected:
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
    if (t != PltOsUnmanaged) {
        palloc = new AllocTracker;
        if (palloc) {
            palloc->count = 1;
            palloc->type = t;
        } else {
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
                     || os==PltOsNew || os==PltOsArrayNew);
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
                     || t==PltOsNew || t==PltOsArrayNew );
    if (t == PltOsNew) {
        t = PltOsArrayNew;
    }
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

#endif // EOF handle.h
