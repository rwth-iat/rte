/* -*-plt-c++-*- */
#ifndef PLT_SORT_IMPL_INCLUDED
#define PLT_SORT_IMPL_INCLUDED
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










