/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/src/templates.cpp,v 1.5 1997-09-15 13:08:06 martin Exp $ */
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

template class KsArray<KsGetVarItemResult>;
template class KsArray<KsResult>;
template class KsArray<KsSetVarItem>;
template class KsArray<KsString>;
template class KsArray<KsTime>;
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
template class PltArrayHandle<KsGetVarItemResult>;
template class PltArrayHandle<KsPath>;
template class PltArrayHandle<KsResult>;
template class PltArrayHandle<KsSetVarItem>;
template class PltArrayHandle<KsString>;
template class PltArrayHandle<KsTime>;
template class PltArrayIterator<KsGetVarItemResult>;
template class PltArrayIterator<KsPath>;
template class PltArrayIterator<KsResult>;
template class PltArrayIterator<KsSetVarItem>;
template class PltArrayIterator<KsString>;
template class PltArrayIterator<KsTime>;
template class PltArrayed<KsGetVarItemResult>;
template class PltArrayed<KsPath>;
template class PltArrayed<KsResult>;
template class PltArrayed<KsSetVarItem>;
template class PltArrayed<KsString>;
template class PltArrayed<KsTime>;
template class PltBidirIterator<KsGetVarItemResult>;
template class PltBidirIterator<KsPath>;
template class PltBidirIterator<KsPtrHandle<KsProjProps> >;
template class PltBidirIterator<KsResult>;
template class PltBidirIterator<KsSetVarItem>;
template class PltBidirIterator<KsString>;
template class PltBidirIterator<KsTime>;
template class PltContainer<KsGetVarItemResult>;
template class PltContainer<KsPath>;
template class PltContainer<KsPtrHandle<KsProjProps> >;
template class PltContainer<KsResult>;
template class PltContainer<KsSetVarItem>;
template class PltContainer<KsString>;
template class PltContainer<KsTime>;
template class PltContainer_<KsGetVarItemResult>;
template class PltContainer_<KsPath>;
template class PltContainer_<KsPtrHandle<KsProjProps> >;
template class PltContainer_<KsResult>;
template class PltContainer_<KsSetVarItem>;
template class PltContainer_<KsString>;
template class PltContainer_<KsTime>;
template class PltHandle<KsCurrProps>;
template class PltHandle<KsGetVarItemResult>;
template class PltHandle<KsPath>;
template class PltHandle<KsProjProps>;
template class PltHandle<KsResult>;
template class PltHandle<KsSetVarItem>;
template class PltHandle<KsString>;
template class PltHandle<KsTime>;
template class PltHandle<KsValue>;
template class PltIterator<KsGetVarItemResult>;
template class PltIterator<KsPath>;
template class PltIterator<KsPtrHandle<KsProjProps> >;
template class PltIterator<KsResult>;
template class PltIterator<KsSetVarItem>;
template class PltIterator<KsString>;
template class PltIterator<KsTime>;
template class PltIterator_<KsGetVarItemResult>;
template class PltIterator_<KsPath>;
template class PltIterator_<KsPtrHandle<KsProjProps> >;
template class PltIterator_<KsResult>;
template class PltIterator_<KsSetVarItem>;
template class PltIterator_<KsString>;
template class PltIterator_<KsTime>;
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
template class Plt_AtArrayNew<KsValue>;
template class Plt_AtNew<KsCurrProps>;
template class Plt_AtNew<KsGetVarItemResult>;
template class Plt_AtNew<KsPath>;
template class Plt_AtNew<KsProjProps>;
template class Plt_AtNew<KsResult>;
template class Plt_AtNew<KsSetVarItem>;
template class Plt_AtNew<KsString>;
template class Plt_AtNew<KsTime>;
template class Plt_AtNew<KsValue>;

#endif

/* EOF ks/templates.cpp */



