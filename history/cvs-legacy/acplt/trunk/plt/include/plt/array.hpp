/* -*-plt-c++-*- */
/* 
 *  Copyright (c) 1996 PLT, RWTH-Aachen, Germany. See file copy_plt.txt!
 *  Author: Martin Kneissl <martin@plt.rwth-aachen.de>
 *
 */

#ifndef PLT_ARRAY_INCLUDED
#define PLT_ARRAY_INCLUDED

#include <plt/debug.h>

#include <stdlib.h>


//////////////////////////////////////////////////////////////////////

template <class T>
class PltArray
{
public:
    PltArray(size_t sz);
    ~PltArray();

    // accessors
    size_t size() const;
    const T & operator[] (size_t i) const;

    // modifiers
    T & operator[] (size_t i);

private:
    PltArray(const PltArray<T> &); // forbidden
    PltArray<T> & operator = ( const PltArray<T> &); // forbidden
    T * a_array;
    size_t a_size;
};

//////////////////////////////////////////////////////////////////////
// INLINE IMPLEMENTATION
//////////////////////////////////////////////////////////////////////

template <class T>
inline
PltArray<T>::PltArray(size_t sz)
{
    a_array = new T[sz];
    a_size = a_array ? sz : 0;
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline
PltArray<T>::~PltArray()
{
    if (a_array) {
        delete a_array;
    }
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline size_t
PltArray<T>::size() const
{
    return a_size;
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline T &
PltArray<T>::operator [] (size_t i)
{
    PLT_PRECONDITION( i < a_size );
    return a_array[i];
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline const T &
PltArray<T>::operator [] (size_t i) const
{
    PLT_PRECONDITION( i < a_size );
    return a_array[i];
}

//////////////////////////////////////////////////////////////////////

#endif
