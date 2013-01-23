/* -*-plt-c++-*- */
#ifndef KS_VALUE_IMPL_INCLUDED
#define KS_VALUE_IMPL_INCLUDED
/* $Header: /home/david/cvs/acplt/ks/include/ks/value_impl.h,v 1.9 2007-04-25 12:57:20 martin Exp $ */
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
