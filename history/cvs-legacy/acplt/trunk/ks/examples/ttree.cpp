/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/examples/ttree.cpp,v 1.1 1997-05-20 15:20:13 harald Exp $ */
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

/* Author: Harald Albrecht <harald@plt.rwth-aachen.de> */

#include "ks/commobject.h"

#if PLT_COMPILER_GCC
#include "plt/hashtable_impl.h"
#include "plt/handle_impl.h"
#endif

#if PLT_COMPILER_BORLAND
#include "ks/array.h"
#include "ks/array_builtins.h"
#include "ks/handle.h"
#include "ks/list.h"

// Borland's linker is somewhat weird...
#if 1
#include "plt/priorityqueue.h"
#include "ks/svrbase.h"
#include "ks/svrobjects.h"
template class PltHandle<KssCommObject>;
template class PltHandle<KssDomain>;
template class PltHandle<PltHandleIterator<KssCommObject> >;
template class PltHandleContainer<KssCommObject>;
template class PltHandleIterator<KssCommObject>;
template class PltHashIterator<KsString, PltPtrHandle<KssCommObject> >;
template class PltHashTable<KsString, PltPtrHandle<KssCommObject> >;
template class PltHashTable_<KsString, PltPtrHandle<KssCommObject> >;
template class PltIterator<PltAssoc<KsString, PltPtrHandle<KssCommObject> > >;
template class PltIterator_<KssCommObject>;
template class PltIterator_<PltAssoc<KsString, PltPtrHandle<KssCommObject> > >;
template class PltPriorityQueue<PltPtrComparable<KsTimerEvent> >;
template class PltPtrComparable<KsTimerEvent>;
#endif

#endif

#if PLT_INSTANTIATE_TEMPLATES
#include "ttree_inst.h"
#endif

// EOF ttree.cpp

