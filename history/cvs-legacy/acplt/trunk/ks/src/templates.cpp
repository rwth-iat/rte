/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/src/templates.cpp,v 1.2 1997-03-24 18:40:25 martin Exp $ */
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

#ifdef __GNUC__

#include "ks/array_impl.h"

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

#endif

/* EOF ks/templates.cpp */

