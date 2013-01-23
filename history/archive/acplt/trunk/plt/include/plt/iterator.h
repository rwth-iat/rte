/* -*-plt-c++-*- */
/* 
 *  Copyright (c) 1997 PLT, RWTH-Aachen, Germany. See file copy_plt.txt!
 *  Author: Martin Kneissl <martin@plt.rwth-aachen.de>
 *
 */

#ifndef PLT_ITERATOR_INCLUDED
#define PLT_ITERATOR_INCLUDED

#include <plt/debug.h>


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
class PltIterator
{
public:
    virtual operator const void * () const = 0;   // remaining element?
    virtual const T & operator * () const;        //  /current element
    virtual const T * operator -> () const;       //  \override at least one!
    virtual PltIterator & operator ++ () = 0;     // advance
    virtual void operator ++ (int);               // (postfix)
    virtual void toStart() = 0;                   // go to the beginning
};


//////////////////////////////////////////////////////////////////////

template <class T>
class PltBidirIterator : public PltIterator<T>
{
public:
    virtual PltBidirIterator & operator -- () = 0;  // step backwards
    virtual void operator -- (int);                 // (postfix)
    virtual void toEnd() = 0;                       // go to the end
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

template <class T>
inline void
PltIterator<T>::operator ++ (int)
{
    operator ++ ();
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline void
PltBidirIterator<T>::operator -- (int)
{
    operator -- ();
}

//////////////////////////////////////////////////////////////////////

#endif // end of header file
