/* -*-plt-c++-*- */
#ifndef KS_ARRAY_INCLUDED
#define KS_ARRAY_INCLUDED
/*
 * Copyright (c) 1996, 1997, 1998, 1999
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

