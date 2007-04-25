/*
 * $Id: dbdump_templ.cpp,v 1.1 2007-04-25 13:59:03 martin Exp $
 *
 * Copyright (c) 1996-2002
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

// Includes, defines and typedefs for ACPLT/OV database dumper
// Author : 
// dbdump_templ.h
// last change:

/////////////////////////////////////////////////////////////////////////////

#include "plt/config.h"
#include "ks/register.h"
#include "ks/objmgrparams.h"
#include "ks/path.h"
#include "ks/sorter.h"
#include "ks/package.h"

#if PLT_SEE_ALL_TEMPLATES

#include "plt/priorityqueue.h"
#include "plt/hashtable.h"
#include "ks/array.h"
#include "ks/handle.h"
#include "ks/list.h"
#include "plt/sort.h"

#else

#include "plt/priorityqueue_impl.h"
#include "plt/hashtable_impl.h"
#include "ks/array_impl.h"
#include "ks/handle_impl.h"
#include "ks/list_impl.h"
#include "plt/sort_impl.h"

#endif

#if !PLT_COMPILER_GCC
#include "ks/array_builtins.h"
#endif

#include "plt/comparable.h"

template class PltList<PltString>;
template class PltSort<KscSortVarPtr>;
template class PltContainer_<PltString>;
template class PltContainer<PltString>;

#if OV_SYSTEM_LINUX
template class PltContainer_<PltAssoc<KsString, KscServerBase*> >;
template class PltContainer_<PltAssoc<KsString, unsigned long> >;
template class PltContainer_<PltAssoc<PltKeyPlainConstPtr<KscAvModule>, PltPtrHandle<KscNegotiator> > >;
template class PltDictionary<KsString, KscServerBase*>;
template class PltDictionary<KsString, unsigned long>;
template class PltDictionary<PltKeyPlainConstPtr<KscAvModule>, PltPtrHandle<KscNegotiator> >;
template class PltHandle<KscNegotiator>;
template class PltHashIterator<KsString, KscServerBase*>;
template class PltHashTable<KsString, KscServerBase*>;
template class PltHashTable<KsString, unsigned long>;
template class PltHashTable<PltKeyPlainConstPtr<KscAvModule>, PltPtrHandle<KscNegotiator> >;
template class PltHashTable_<KsString, KscServerBase*>;
template class PltHashTable_<KsString, unsigned long>;
template class PltHashTable_<PltKeyPlainConstPtr<KscAvModule>, PltPtrHandle<KscNegotiator> >;
template class PltIterator<PltAssoc<KsString, KscServerBase*> >;
template class PltIterator<PltPtrHandle<KscSorterBucket> >;
template class PltIterator_<PltAssoc<KsString, KscServerBase*> >;
template class PltIterator_<PltAssoc<KscSorter::Key, PltPtrHandle<KscSorterBucket> > >;
template class PltIterator_<PltPtrHandle<KscSorterBucket> >;
template class PltListIterator<PltString>;
template class PltPtrHandle<KscNegotiator>;
template class PltAssoc<KsString, KscServerBase*>;
template class PltAssoc<KsString, unsigned long>;
template class PltAssoc<PltKeyPlainConstPtr<KscAvModule>, PltPtrHandle<KscNegotiator> >;
template class PltBidirIterator<PltString>;
template class PltContainer<PltAssoc<KsString, KscServerBase*> >;
template class PltContainer<PltAssoc<KsString, unsigned long> >;
template class PltContainer<PltAssoc<PltKeyPlainConstPtr<KscAvModule>, PltPtrHandle<KscNegotiator> > >;
template class PltHashIterator<KsString, unsigned long>;
template class PltHashIterator<PltKeyPlainConstPtr<KscAvModule>, PltPtrHandle<KscNegotiator> >;
template class PltIterator<PltAssoc<KsString, unsigned long> >;
template class PltIterator<PltAssoc<PltKeyPlainConstPtr<KscAvModule>, PltPtrHandle<KscNegotiator> > >;
template class PltIterator_<PltAssoc<KsString, unsigned long> >;
template class PltIterator_<PltAssoc<PltKeyPlainConstPtr<KscAvModule>, PltPtrHandle<KscNegotiator> > >;
template class PltIterator_<PltString>;
template class Plt_AtArrayNew<KscNegotiator>;
template class Plt_AtNew<KscNegotiator>;
template class PltIterator<PltString>;

#endif
