/* -*-plt-c++-*- */
/* 
 *  Copyright (c) 1996 PLT, RWTH-Aachen, Germany. See file copy_plt.txt!
 *  Author: Martin Kneissl <martin@plt.rwth-aachen.de>
 *
 */

#ifndef PLT_ITERATOR_INCLUDED
#define PLT_ITERATOR_INCLUDED

#include <plt/debug.h>


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
    virtual void restart() = 0;                   // from beginning
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

#endif // end of header file
