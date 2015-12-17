/* -*-plt-c++-*- */
#ifndef KS_ARRAY_INCLUDED
#define KS_ARRAY_INCLUDED
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

//////////////////////////////////////////////////////////////////////
// Author: Martin Kneissl <martin@plt.rwth-aachen.de>
//////////////////////////////////////////////////////////////////////
// uses array_impl.h / template.cpp mechanism

#include "plt/array.h"
#include "ks/xdr.h"

//////////////////////////////////////////////////////////////////////

template <class T>
class KsArray
: public PltArray<T>,
  public KsXdrAble
{
 public:
    KsArray(size_t size = 0); // array size          // [1]
    KsArray(size_t size,      // array size 
             T * p,            // address of first element
             enum PltOwnership);                      // [1]
    
    // KsXdrAble interface
    KsArray(XDR *, bool & success);
    virtual bool xdrEncode(XDR *) const;  // serialize
    virtual bool xdrDecode(XDR *);        // deserialize
    static KsArray * xdrNew(XDR *);   // deserialization factory meth. 
};


//////////////////////////////////////////////////////////////////////
// IMPLEMENTATION
//////////////////////////////////////////////////////////////////////

#if PLT_COMPILER_GCC
#if (__GNUC__<2) || ((__GNUC__==2) && (__GNUC_MINOR__<=7))

bool KsArray<char>::xdrEncode(XDR *) const;
bool KsArray<char>::xdrDecode(XDR *);

bool KsArray<bool>::xdrEncode(XDR *) const;
bool KsArray<bool>::xdrDecode(XDR *);

bool KsArray<long>::xdrEncode(XDR *) const;
bool KsArray<long>::xdrDecode(XDR *);

bool KsArray<u_long>::xdrEncode(XDR *) const;
bool KsArray<u_long>::xdrDecode(XDR *);

bool KsArray<int>::xdrEncode(XDR *) const;
bool KsArray<int>::xdrDecode(XDR *);

bool KsArray<u_int>::xdrEncode(XDR *) const;
bool KsArray<u_int>::xdrDecode(XDR *);

bool KsArray<short>::xdrEncode(XDR *) const;
bool KsArray<short>::xdrDecode(XDR *);

bool KsArray<u_short>::xdrEncode(XDR *) const;
bool KsArray<u_short>::xdrDecode(XDR *);

bool KsArray<float>::xdrEncode(XDR *) const;
bool KsArray<float>::xdrDecode(XDR *);

bool KsArray<double>::xdrEncode(XDR *) const;
bool KsArray<double>::xdrDecode(XDR *);

#endif
#endif

#if PLT_COMPILER_MSVC
#include "ks/array_builtins.h"
#endif

//////////////////////////////////////////////////////////////////////
// defaults
//////////////////////////////////////////////////////////////////////

template <class T>
inline
KsArray<T>::KsArray(size_t size, T *p, enum PltOwnership os)
: PltArray<T>(size, p, os)
{
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline
KsArray<T>::KsArray(size_t size)
: PltArray<T>(size)
{
}

//////////////////////////////////////////////////////////////////////

template <class T>
inline
KsArray<T>::KsArray(XDR * xdr, bool & ok)
{
    PLT_PRECONDITION(xdr->x_op == XDR_DECODE);
    ok = xdrDecode(xdr);
}

//////////////////////////////////////////////////////////////////////

#if PLT_SEE_ALL_TEMPLATES

#include "ks/array_impl.h"

#if PLT_COMPILER_BORLAND

template class extern KsArray<bool>;
template class extern KsArray<char>;
template class extern KsArray<long>;
template class extern KsArray<u_long>;
template class extern KsArray<int>;
template class extern KsArray<u_int>;
template class extern KsArray<short>;
template class extern KsArray<u_short>;
template class extern KsArray<double>;
template class extern KsArray<float>;

#else
#include "ks/array_builtins.h"
#endif

#endif

//////////////////////////////////////////////////////////////////////
#endif // KS_ARRAY_INCLUDED

