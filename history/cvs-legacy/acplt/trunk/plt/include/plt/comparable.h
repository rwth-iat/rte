/* -*-plt-c++-*- */
/* 
 *  Copyright (c) 1996 PLT, RWTH-Aachen, Germany. See file copy_plt.txt!
 *  Author: Martin Kneissl <martin@plt.rwth-aachen.de>
 *
 */

#ifndef PLT_COMPARABLE_INCLUDED
#define PLT_COMPARABLE_INCLUDED

#include <plt/debug.h>

//////////////////////////////////////////////////////////////////////

template <class T>
class PltPtrComparable
{
    friend bool operator <  ( PltPtrComparable<T> t1, PltPtrComparable<T> t2);
    friend bool operator >  ( PltPtrComparable<T> t1, PltPtrComparable<T> t2);
    friend bool operator != ( PltPtrComparable<T> t1, PltPtrComparable<T> t2);
    friend bool operator == ( PltPtrComparable<T> t1, PltPtrComparable<T> t2);
    friend bool operator <= ( PltPtrComparable<T> t1, PltPtrComparable<T> t2);
    friend bool operator >= ( PltPtrComparable<T> t1, PltPtrComparable<T> t2);
public:
    operator T * ();

    PltPtrComparable(T * = 0);

private:
    T *a_p;
};

//////////////////////////////////////////////////////////////////////
// INLINE IMPLEMENTATION
//////////////////////////////////////////////////////////////////////

template <class T>
inline bool 
operator <  ( PltPtrComparable<T> t1, PltPtrComparable<T> t2) 
{
    return *t1.a_p < *t2.a_p;
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline bool 
operator >  ( PltPtrComparable<T> t1, PltPtrComparable<T> t2) 
{
    return *t1.a_p > *t2.a_p;
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline bool 
operator <=  ( PltPtrComparable<T> t1, PltPtrComparable<T> t2) 
{
    return *t1.a_p <= *t2.a_p;
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline bool 
operator >=  ( PltPtrComparable<T> t1, PltPtrComparable<T> t2) 
{
    return *t1.a_p >= *t2.a_p;
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline bool 
operator ==  ( PltPtrComparable<T> t1, PltPtrComparable<T> t2) 
{
    return *t1.a_p == *t2.a_p;
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline bool 
operator !=  ( PltPtrComparable<T> t1, PltPtrComparable<T> t2) 
{
    return *t1.a_p != *t2.a_p;
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline
PltPtrComparable<T>::operator T * ()
{
    return a_p;
}

template <class T>
inline
PltPtrComparable<T>::PltPtrComparable(T *p)
: a_p(p)
{
}

//////////////////////////////////////////////////////////////////////

#endif
