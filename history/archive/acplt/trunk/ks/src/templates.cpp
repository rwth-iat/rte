/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/src/templates.cpp,v 1.15 2007-04-25 12:57:21 martin Exp $ */
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
// Template instances for templates from lower layer that are
// always needed in this layer
//////////////////////////////////////////////////////////////////////

#include "plt/config.h"
#include "ks/register.h"
#include "ks/serviceparams.h"
#include "ks/path.h"

#if PLT_SEE_ALL_TEMPLATES

#include "plt/priorityqueue.h"
#include "plt/hashtable.h"
#include "ks/array.h"
#include "ks/handle.h"
#include "ks/list.h"

#else

#include "plt/priorityqueue_impl.h"
#include "plt/hashtable_impl.h"
#include "ks/array_impl.h"
#include "ks/handle_impl.h"
#include "ks/list_impl.h"

#endif

#if !PLT_COMPILER_GCC
#include "ks/array_builtins.h"
#endif

#include "plt/comparable.h"


#if PLT_INSTANTIATE_TEMPLATES

template class PltArray<bool>;
template class PltArray<char>;
template class PltArray<double>;
template class PltArray<float>;
template class PltArray<int>;
template class PltArray<long>;
template class PltArray<short>;
template class PltArray<unsigned int>;
template class PltArray<unsigned long>;
template class PltArray<unsigned short>;
template class PltArrayHandle<bool>;
template class PltArrayHandle<char>;
template class PltArrayHandle<double>;
template class PltArrayHandle<float>;
template class PltArrayHandle<int>;
template class PltArrayHandle<long>;
template class PltArrayHandle<short>;
template class PltArrayHandle<unsigned int>;
template class PltArrayHandle<unsigned long>;
template class PltArrayHandle<unsigned short>;
template class PltArrayIterator<bool>;
template class PltArrayIterator<char>;
template class PltArrayIterator<double>;
template class PltArrayIterator<float>;
template class PltArrayIterator<int>;
template class PltArrayIterator<long>;
template class PltArrayIterator<short>;
template class PltArrayIterator<unsigned int>;
template class PltArrayIterator<unsigned long>;
template class PltArrayIterator<unsigned short>;
template class PltArrayed<bool>;
template class PltArrayed<char>;
template class PltArrayed<double>;
template class PltArrayed<float>;
template class PltArrayed<int>;
template class PltArrayed<long>;
template class PltArrayed<short>;
template class PltArrayed<unsigned int>;
template class PltArrayed<unsigned long>;
template class PltArrayed<unsigned short>;
template class PltBidirIterator<bool>;
template class PltBidirIterator<char>;
template class PltBidirIterator<double>;
template class PltBidirIterator<float>;
template class PltBidirIterator<int>;
template class PltBidirIterator<long>;
template class PltBidirIterator<short>;
template class PltBidirIterator<unsigned int>;
template class PltBidirIterator<unsigned long>;
template class PltBidirIterator<unsigned short>;
template class PltContainer_<bool>;
template class PltContainer_<char>;
template class PltContainer_<double>;
template class PltContainer_<float>;
template class PltContainer_<int>;
template class PltContainer_<long>;
template class PltContainer_<short>;
template class PltContainer_<unsigned int>;
template class PltContainer_<unsigned long>;
template class PltContainer_<unsigned short>;
template class PltContainer<bool>;
template class PltContainer<char>;
template class PltContainer<double>;
template class PltContainer<float>;
template class PltContainer<int>;
template class PltContainer<long>;
template class PltContainer<short>;
template class PltContainer<unsigned int>;
template class PltContainer<unsigned long>;
template class PltContainer<unsigned short>;
template class PltHandle<bool>;
template class PltHandle<char>;
template class PltHandle<double>;
template class PltHandle<float>;
template class PltHandle<int>;
template class PltHandle<long>;
template class PltHandle<short>;
template class PltHandle<unsigned int>;
template class PltHandle<unsigned long>;
template class PltHandle<unsigned short>;
template class PltIterator<bool>;
template class PltIterator<char>;
template class PltIterator<double>;
template class PltIterator<float>;
template class PltIterator<int>;
template class PltIterator<long>;
template class PltIterator<short>;
template class PltIterator<unsigned int>;
template class PltIterator<unsigned long>;
template class PltIterator<unsigned short>;
template class PltIterator_<bool>;
template class PltIterator_<char>;
template class PltIterator_<double>;
template class PltIterator_<float>;
template class PltIterator_<int>;
template class PltIterator_<long>;
template class PltIterator_<short>;
template class PltIterator_<unsigned int>;
template class PltIterator_<unsigned long>;
template class PltIterator_<unsigned short>;
template class Plt_AtArrayNew<bool>;
template class Plt_AtArrayNew<char>;
template class Plt_AtArrayNew<double>;
template class Plt_AtArrayNew<float>;
template class Plt_AtArrayNew<int>;
template class Plt_AtArrayNew<long>;
template class Plt_AtArrayNew<short>;
template class Plt_AtArrayNew<unsigned int>;
template class Plt_AtArrayNew<unsigned long>;
template class Plt_AtArrayNew<unsigned short>;
template class Plt_AtNew<bool>;
template class Plt_AtNew<char>;
template class Plt_AtNew<double>;
template class Plt_AtNew<float>;
template class Plt_AtNew<int>;
template class Plt_AtNew<long>;
template class Plt_AtNew<short>;
template class Plt_AtNew<unsigned int>;
template class Plt_AtNew<unsigned long>;
template class Plt_AtNew<unsigned short>;

// general ks templates

#if PLT_COMPILER_CYGWIN
template class KsArray<bool>;
template class KsArray<char>;
template class KsArray<u_long>;
template class KsArray<long>;
template class KsArray<u_int>;
template class KsArray<int>;
template class KsArray<short>;
template class KsArray<u_short>;
template class KsArray<float>;
template class KsArray<double>;
#endif

template class KsArray<KsGetVarItemResult>;
template class KsArray<KsResult>;
template class KsArray<KsSetVarItem>;
template class KsArray<KsString>;
template class KsArray<KsTime>;
template class KsArray<KsTimeSpan>;
template class KsList<KsPtrHandle<KsProjProps> >;
template class KsPtrHandle<KsCurrProps>;
template class KsPtrHandle<KsProjProps>;
template class KsPtrHandle<KsValue>;
template class PltArray<KsGetVarItemResult>;
template class PltArray<KsPath>;
template class PltArray<KsResult>;
template class PltArray<KsSetVarItem>;
template class PltArray<KsString>;
template class PltArray<KsTime>;
template class PltArray<KsTimeSpan>;
template class PltArrayHandle<KsGetVarItemResult>;
template class PltArrayHandle<KsPath>;
template class PltArrayHandle<KsResult>;
template class PltArrayHandle<KsSetVarItem>;
template class PltArrayHandle<KsString>;
template class PltArrayHandle<KsTime>;
template class PltArrayHandle<KsTimeSpan>;
template class PltArrayIterator<KsGetVarItemResult>;
template class PltArrayIterator<KsPath>;
template class PltArrayIterator<KsResult>;
template class PltArrayIterator<KsSetVarItem>;
template class PltArrayIterator<KsString>;
template class PltArrayIterator<KsTime>;
template class PltArrayIterator<KsTimeSpan>;
template class PltArrayed<KsGetVarItemResult>;
template class PltArrayed<KsPath>;
template class PltArrayed<KsResult>;
template class PltArrayed<KsSetVarItem>;
template class PltArrayed<KsString>;
template class PltArrayed<KsTime>;
template class PltArrayed<KsTimeSpan>;
template class PltBidirIterator<KsGetVarItemResult>;
template class PltBidirIterator<KsPath>;
template class PltBidirIterator<KsPtrHandle<KsProjProps> >;
template class PltBidirIterator<KsResult>;
template class PltBidirIterator<KsSetVarItem>;
template class PltBidirIterator<KsString>;
template class PltBidirIterator<KsTime>;
template class PltBidirIterator<KsTimeSpan>;
template class PltContainer<KsGetVarItemResult>;
template class PltContainer<KsPath>;
template class PltContainer<KsPtrHandle<KsProjProps> >;
template class PltContainer<KsResult>;
template class PltContainer<KsSetVarItem>;
template class PltContainer<KsString>;
template class PltContainer<KsTime>;
template class PltContainer<KsTimeSpan>;
template class PltContainer_<KsGetVarItemResult>;
template class PltContainer_<KsPath>;
template class PltContainer_<KsPtrHandle<KsProjProps> >;
template class PltContainer_<KsResult>;
template class PltContainer_<KsSetVarItem>;
template class PltContainer_<KsString>;
template class PltContainer_<KsTime>;
template class PltContainer_<KsTimeSpan>;
template class PltHandle<KsCurrProps>;
template class PltHandle<KsGetVarItemResult>;
template class PltHandle<KsPath>;
template class PltHandle<KsProjProps>;
template class PltHandle<KsResult>;
template class PltHandle<KsSetVarItem>;
template class PltHandle<KsString>;
template class PltHandle<KsTime>;
template class PltHandle<KsTimeSpan>;
template class PltHandle<KsValue>;
template class PltIterator<KsGetVarItemResult>;
template class PltIterator<KsPath>;
template class PltIterator<KsPtrHandle<KsProjProps> >;
template class PltIterator<KsResult>;
template class PltIterator<KsSetVarItem>;
template class PltIterator<KsString>;
template class PltIterator<KsTime>;
template class PltIterator<KsTimeSpan>;
template class PltIterator_<KsGetVarItemResult>;
template class PltIterator_<KsPath>;
template class PltIterator_<KsPtrHandle<KsProjProps> >;
template class PltIterator_<KsResult>;
template class PltIterator_<KsSetVarItem>;
template class PltIterator_<KsString>;
template class PltIterator_<KsTime>;
template class PltIterator_<KsTimeSpan>;
template class PltList<KsPtrHandle<KsProjProps> >;
template class PltListIterator<KsPtrHandle<KsProjProps> >;
template class PltListNode<KsPtrHandle<KsProjProps> >;
template class PltPtrHandle<KsCurrProps>;
template class PltPtrHandle<KsProjProps>;
template class PltPtrHandle<KsValue>;
template class Plt_AtArrayNew<KsCurrProps>;
template class Plt_AtArrayNew<KsGetVarItemResult>;
template class Plt_AtArrayNew<KsPath>;
template class Plt_AtArrayNew<KsProjProps>;
template class Plt_AtArrayNew<KsResult>;
template class Plt_AtArrayNew<KsSetVarItem>;
template class Plt_AtArrayNew<KsString>;
template class Plt_AtArrayNew<KsTime>;
template class Plt_AtArrayNew<KsTimeSpan>;
template class Plt_AtArrayNew<KsValue>;
template class Plt_AtNew<KsCurrProps>;
template class Plt_AtNew<KsGetVarItemResult>;
template class Plt_AtNew<KsPath>;
template class Plt_AtNew<KsProjProps>;
template class Plt_AtNew<KsResult>;
template class Plt_AtNew<KsSetVarItem>;
template class Plt_AtNew<KsString>;
template class Plt_AtNew<KsTime>;
template class Plt_AtNew<KsTimeSpan>;
template class Plt_AtNew<KsValue>;


template class KsList<KsPtrHandle<KsEngPropsV1> >;
template class KsPtrHandle<KsEngPropsV1>;
template class PltBidirIterator<KsPtrHandle<KsEngPropsV1> >;
template class PltContainer<KsPtrHandle<KsEngPropsV1> >;
template class PltContainer_<KsPtrHandle<KsEngPropsV1> >;
template class PltHandle<KsEngPropsV1>;
template class PltIterator<KsPtrHandle<KsEngPropsV1> >;
template class PltIterator_<KsPtrHandle<KsEngPropsV1> >;
template class PltList<KsPtrHandle<KsEngPropsV1> >;
template class PltListNode<KsPtrHandle<KsEngPropsV1> >;
template class PltListIterator<KsPtrHandle<KsEngPropsV1> >;
template class PltPtrHandle<KsEngPropsV1>;
template class Plt_AtArrayNew<KsEngPropsV1>;
template class Plt_AtNew<KsEngPropsV1>;


#endif

/* EOF ks/templates.cpp */



