/* -*-plt-c++-*- */
#ifndef KS_VALUE_IMPL_INCLUDED
#define KS_VALUE_IMPL_INCLUDED
/* $Header: /home/david/cvs/acplt/ks/include/ks/value_impl.h,v 1.5 1999-04-22 15:27:31 harald Exp $ */
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

//////////////////////////////////////////////////////////////////////
// Author: Markus Juergens <markusj@plt.rwth-aachen.de>
//////////////////////////////////////////////////////////////////////

#if !PLT_SEE_ALL_TEMPLATES
#include "ks/value.h"
#include "ks/array_impl.h"
#if PLT_COMPILER_WATCOM
#include "ks/array_builtins.h"
#endif
#endif

// Borland's C++ Builder now needs to see the buildins
#if PLT_COMPILER_BORLAND
#include "ks/array_builtins.h"
#endif

//////////////////////////////////////////////////////////////////////

template<class T>
KsVecValueBase<T>::KsVecValueBase(size_t size)
: KsArray<T>(size)
{}

//////////////////////////////////////////////////////////////////////

template<class T>
KsVecValueBase<T>::KsVecValueBase(size_t size, T *p, PltOwnership os)
: KsArray<T>(size, p, os)
{}

//////////////////////////////////////////////////////////////////////

template <class T>
bool
KsVecValueBase<T>::xdrEncode(XDR *xdr) const
{
    return KsValue::xdrEncode(xdr);
}

//////////////////////////////////////////////////////////////////////

template <class T>
bool
KsVecValueBase<T>::xdrDecode(XDR *xdr)
{
    return KsValue::xdrDecode(xdr);
}

//////////////////////////////////////////////////////////////////////

template <class T>
bool
KsVecValueBase<T>::xdrEncodeVariant(XDR *xdr) const
{
    return KsArray<T>::xdrEncode(xdr);
}

//////////////////////////////////////////////////////////////////////
    
template <class T> 
bool
KsVecValueBase<T>::xdrDecodeVariant(XDR *xdr)
{
    return KsArray<T>::xdrDecode(xdr);
}


//////////////////////////////////////////////////////////////////////
#endif // KS_VALUE_IMPL_INCLUDED
// ks/value_impl.h
