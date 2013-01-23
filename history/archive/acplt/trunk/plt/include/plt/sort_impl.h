/* -*-plt-c++-*- */
#ifndef PLT_SORT_IMPL_INCLUDED
#define PLT_SORT_IMPL_INCLUDED
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
/* Author: Markus Juergens <markusj@plt.rwth-aachen.de> */

//////////////////////////////////////////////////////////////////////

#if !PLT_SEE_ALL_TEMPLATES
#include "plt/sort.h"
#endif

//////////////////////////////////////////////////////////////////////
// internal helper functions
//
//////////////////////////////////////////////////////////////////////
// assure that a <= b
//
template <class T>
void
PltSort<T>::_sort2(T &a, T &b)
{
    if( b < a ) {
        _swap(a, b);
    }
    PLT_POSTCONDITION(!(b < a));
}

//////////////////////////////////////////////////////////////////////
// assure that a <= b <= c
// TODO: optimize this function
//
template <class T>
void
PltSort<T>::_sort3(T &a, T &b, T &c)
{
    if( b < a ) _swap(b, a);
    if( c < a ) _swap(c, a);
    if( c < b ) _swap(c, b);
    PLT_POSTCONDITION(!(b < a || c < a || c < b));
}

//////////////////////////////////////////////////////////////////////
// recursive quicksort algorithm
//
template <class T>
void
PltSort<T>::_rek_qsort(PltArray<T> &el, size_t l, size_t r)
{
    PLT_ASSERT(l <= r);

    if( l < r ) {
        size_t distance = r - l;
    
        if( distance == 1 ) {
            _sort2(el[l], el[r]);
        } else if( distance == 2 ) {
            _sort3(el[l], el[l+1], el[r]);
        } else if( distance > 2 ) {
            // use median of three to choose middle element
            //
            size_t i = l, j = r, m = (r+l)/2;
            _sort3(el[l], el[m], el[r]);
            T v(el[m]);
            
            do {
                while(el[++i] < v) ; // NOP
                while(v < el[--j]) ; // NOP
                if( i >= j ) break;
                _swap(el[i], el[j]);
            } while(true);
            
            _rek_qsort(el, l, i-1);
            _rek_qsort(el, i, r);
        }
    }
}


//////////////////////////////////////////////////////////////////////
// wrapper to call the recursive function
//
template <class T>
void 
PltSort<T>::qsort(PltArray<T> &toSort)
{
    _rek_qsort(toSort, 0, toSort.size() - 1);
}

#endif // PLT_SORT_IMPL_INCLUDED










