/* -*-plt-c++-*- */
#ifndef PLT_CONTAINER_INCLUDED
#define PLT_CONTAINER_INCLUDED
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
// plt/container.h contains Container and Iterator interfaces
//////////////////////////////////////////////////////////////////////

#include "plt/handle.h"

#include <stdlib.h>

//////////////////////////////////////////////////////////////////////

#if PLT_RETTYPE_OVERLOADABLE
#define PltArrayIterator_THISTYPE(T) PltArrayIterator<T>
#else
#define PltArrayIterator_THISTYPE(T) PltIterator_<T>
#endif

#if PLT_COMPILER_DECCXX
#define PltArrayIterator_CLASSTHISTYPE(T) PltArrayIterator<T>::THISTYPE
#else
#define PltArrayIterator_CLASSTHISTYPE(T) PltArrayIterator_THISTYPE(T) 
#endif

//////////////////////////////////////////////////////////////////////
// Iterator interfaces
//////////////////////////////////////////////////////////////////////
// PltIterators iterate in one "direction" over a container,
// PltBidirIterators can iterate forward and backward.
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// CAVEAT: at least one of operator-> and operator * must be
// overridden in a derived class to avoid infinite recursion!
//////////////////////////////////////////////////////////////////////

template<class T>
class PltIterator_
{
public:
    typedef PltIterator_ THISTYPE;
    virtual ~PltIterator_() { }
    virtual operator bool () const = 0;   // remaining element?
    virtual THISTYPE & operator ++ () = 0;        // advance
    virtual void toStart() = 0;                   // go to the beginning
};


//////////////////////////////////////////////////////////////////////

template<class T>
class PltIterator
: public PltIterator_<T>
{
public:
#if PLT_RETTYPE_OVERLOADABLE
    typedef PltIterator THISTYPE;
#define PltIterator_THISTYPE(T) PltIterator<T>
#else
#define PltIterator_THISTYPE(T) PltIterator_<T>
#endif
    virtual const T & operator * () const;        //  /current element
    virtual const T * operator -> () const;       //  \override at least one!
    virtual THISTYPE & operator ++ () = 0;        // advance
    virtual void toStart() = 0;                   // go to the beginning
};


//////////////////////////////////////////////////////////////////////

template<class T>
class PltHandleIterator
: public PltIterator_<T>
{
public:
#if PLT_RETTYPE_OVERLOADABLE
    typedef PltHandleIterator THISTYPE;
    #define PltHandleIterator_THISTYPE(T) PltHandleIterator<T>
#else
    #define PltHandleIterator_THISTYPE(T) PltIterator_<T>
#endif
    virtual PltPtrHandle<T> operator * () const = 0; // current element
    virtual THISTYPE & operator ++ () = 0;  // advance
    virtual void toStart() = 0;                      // go to the beginning
};


//////////////////////////////////////////////////////////////////////

template <class T>
class PltBidirIterator : public PltIterator<T>
{
#if PLT_RETTYPE_OVERLOADABLE
    typedef PltBidirIterator THISTYPE;
#endif
public:
    virtual THISTYPE & operator -- () = 0;  // step backwards
    virtual void toEnd() = 0;                       // go to the end
};

//////////////////////////////////////////////////////////////////////
// Container interface
//////////////////////////////////////////////////////////////////////

template<class T>
class PltContainer_
{
public:
    virtual ~PltContainer_() { }

    virtual bool isEmpty() const;
    virtual size_t size() const = 0;
    virtual PltIterator_<T> * newIterator() const = 0;
};

//////////////////////////////////////////////////////////////////////

template<class T>
class PltContainer
: public PltContainer_<T>
{
public:
    virtual size_t size() const;
    // virtual PltIterator<T>::THISTYPE * newIterator() const = 0;
    virtual PltIterator_THISTYPE(T) * newIterator() const = 0;
};

//////////////////////////////////////////////////////////////////////

template<class T>
class PltHandleContainer
: public PltContainer_<T>
{
public:
    virtual size_t size() const;
    // virtual PltHandleIterator<T>::THISTYPE * newIterator() const = 0;
    virtual PltHandleIterator_THISTYPE(T) * newIterator() const = 0;
};


//////////////////////////////////////////////////////////////////////
// Arrayed Container Interface
//////////////////////////////////////////////////////////////////////

template<class T> class PltArrayIterator;   // forward declaration

template <class T>
class PltArrayed
: public PltContainer<T>
{
public:
    virtual size_t size() const = 0;

    virtual const T & operator[] (size_t idx) const = 0;

    virtual PltArrayIterator_THISTYPE(T) * newIterator() const;
};


//////////////////////////////////////////////////////////////////////
// Array Iterator
//////////////////////////////////////////////////////////////////////

template<class T>
class PltArrayIterator
: public PltBidirIterator<T>
{
public:
#if PLT_RETTYPE_OVERLOADABLE
    typedef PltArrayIterator THISTYPE;
#endif
    PltArrayIterator(const PltArrayed<T> &);

    // BidirIterator interface
    virtual operator bool () const;       // remaining element?
    virtual const T & operator * () const;        // current element
    virtual THISTYPE & operator ++ ();            // advance
    virtual void toStart();                       // go to the beginning
    virtual THISTYPE & operator -- ();            // step backwards
    virtual void toEnd();                         // go to the end
private:
    const PltArrayed<T> & a_cont;
    size_t a_idx;
};

//////////////////////////////////////////////////////////////////////
// INLINE IMPLEMENTATION
//////////////////////////////////////////////////////////////////////

template <class T>
inline const T *
PltIterator<T>::operator -> () const
{
    return & (operator * () );
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline const T &
PltIterator<T>::operator * () const
{
    return * (operator -> () );
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

template <class T>
inline bool
PltContainer_<T>::isEmpty() const
{
    return size() == 0;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

template <class T>
inline
PltArrayIterator<T>::PltArrayIterator(const PltArrayed<T> & a)
: a_cont(a), 
  a_idx(0)
{
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline
PltArrayIterator<T>::operator bool () const
{
    return ( a_idx != (size_t) -1 && a_idx < a_cont.size() );
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline const T & 
PltArrayIterator<T>::operator * () const 
{
    PLT_PRECONDITION(*this);
    return a_cont[a_idx];
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline PltArrayIterator_CLASSTHISTYPE(T) & 
PltArrayIterator<T>::operator ++ ()
{
    a_idx = ( a_idx == (size_t) -1 ) ?  0 : a_idx + 1;

    return *this;
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline void 
PltArrayIterator<T>::toStart()
{
    a_idx = 0;
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline PltArrayIterator_CLASSTHISTYPE(T) & 
PltArrayIterator<T>::operator -- ()
{
    a_idx = ( a_idx == 0 ) ? (size_t) -1 : a_idx - 1;
    
    return *this;
}
//////////////////////////////////////////////////////////////////////

template <class T>
inline void 
PltArrayIterator<T>::toEnd()
{
    size_t sz = a_cont.size();
    a_idx = sz > 0 ? sz - 1 : (size_t) -1;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

template <class T>
inline PltArrayIterator_THISTYPE(T) *
PltArrayed<T>::newIterator() const
{
    return new PltArrayIterator<T>(*this);
}

//////////////////////////////////////////////////////////////////////

#if PLT_SEE_ALL_TEMPLATES
#include "plt/container_impl.h"
#endif

//////////////////////////////////////////////////////////////////////

#endif // PLT_CONTAINER_INCLUDED
