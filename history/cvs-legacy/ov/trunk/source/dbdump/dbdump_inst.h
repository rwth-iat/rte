/*
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

// This file is created automatically, do not change.
//
// Author : Christian Poensgen <chris@plt.rwth-aachen.de>
// dbdump_inst.h

/////////////////////////////////////////////////////////////////////////////

template class PltAssoc<KsString, KscServerBase *>;
template class PltAssoc<KsString, unsigned long>;
template class PltAssoc<PltKeyPlainConstPtr<KscAvModule>, PltPtrHandle<KscNegotiator> >;
template class PltBidirIterator<PltString>;
template class PltContainer<PltAssoc<KsString, KscServerBase *> >;
template class PltContainer<PltAssoc<KsString, unsigned long> >;
template class PltContainer<PltAssoc<PltKeyPlainConstPtr<KscAvModule>, PltPtrHandle<KscNegotiator> > >;
template class PltContainer<PltString>;
template class PltContainer_<PltAssoc<KsString, KscServerBase *> >;
template class PltContainer_<PltAssoc<KsString, unsigned long> >;
template class PltContainer_<PltAssoc<PltKeyPlainConstPtr<KscAvModule>, PltPtrHandle<KscNegotiator> > >;
template class PltContainer_<PltString>;
template class PltDictionary<KsString, KscServerBase *>;
template class PltDictionary<KsString, unsigned long>;
template class PltDictionary<PltKeyPlainConstPtr<KscAvModule>, PltPtrHandle<KscNegotiator> >;
template class PltHandle<KscNegotiator>;
template class PltHashIterator<KsString, KscServerBase *>;
template class PltHashIterator<KsString, unsigned long>;
template class PltHashIterator<PltKeyPlainConstPtr<KscAvModule>, PltPtrHandle<KscNegotiator> >;
template class PltHashTable<KsString, KscServerBase *>;
template class PltHashTable<KsString, unsigned long>;
template class PltHashTable<PltKeyPlainConstPtr<KscAvModule>, PltPtrHandle<KscNegotiator> >;
template class PltHashTable_<KsString, KscServerBase *>;
template class PltHashTable_<KsString, unsigned long>;
template class PltHashTable_<PltKeyPlainConstPtr<KscAvModule>, PltPtrHandle<KscNegotiator> >;
template class PltIterator<PltAssoc<KsString, KscServerBase *> >;
template class PltIterator<PltAssoc<KsString, unsigned long> >;
template class PltIterator<PltAssoc<PltKeyPlainConstPtr<KscAvModule>, PltPtrHandle<KscNegotiator> > >;
template class PltIterator<PltString>;
template class PltIterator_<PltAssoc<KsString, KscServerBase *> >;
template class PltIterator_<PltAssoc<KsString, unsigned long> >;
template class PltIterator_<PltAssoc<PltKeyPlainConstPtr<KscAvModule>, PltPtrHandle<KscNegotiator> > >;
template class PltIterator_<PltString>;
template class PltKeyPlainConstPtr<KscAvModule>;
template class PltList<PltString>;
template class PltListIterator<PltString>;
template class PltListNode<PltString>;
template class PltPtrHandle<KscNegotiator>;
template class Plt_AtArrayNew<KscNegotiator>;
template class Plt_AtNew<KscNegotiator>;
