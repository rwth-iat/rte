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
// dbparse_inst.h

/////////////////////////////////////////////////////////////////////////////

template class KsArray<KsCreateObjItem>;
template class KsArray<KsCreateObjResultItem>;
template class KsArray<KsGetCanonicalPathResultItem>;
template class KsArray<KsLinkItem>;
template class KsArray<KsRenameObjItem>;
template class KsArray<KsUnlinkItem>;
template class PltArray<KsCreateObjItem>;
template class PltArray<KsCreateObjResultItem>;
template class PltArray<KsGetCanonicalPathResultItem>;
template class PltArray<KsLinkItem>;
template class PltArray<KsRenameObjItem>;
template class PltArray<KsUnlinkItem>;
template class PltArrayHandle<KsCreateObjItem>;
template class PltArrayHandle<KsCreateObjResultItem>;
template class PltArrayHandle<KsGetCanonicalPathResultItem>;
template class PltArrayHandle<KsLinkItem>;
template class PltArrayHandle<KsRenameObjItem>;
template class PltArrayHandle<KsUnlinkItem>;
template class PltArrayIterator<KsCreateObjItem>;
template class PltArrayIterator<KsCreateObjResultItem>;
template class PltArrayIterator<KsGetCanonicalPathResultItem>;
template class PltArrayIterator<KsLinkItem>;
template class PltArrayIterator<KsRenameObjItem>;
template class PltArrayIterator<KsUnlinkItem>;
template class PltArrayed<KsCreateObjItem>;
template class PltArrayed<KsCreateObjResultItem>;
template class PltArrayed<KsGetCanonicalPathResultItem>;
template class PltArrayed<KsLinkItem>;
template class PltArrayed<KsRenameObjItem>;
template class PltArrayed<KsUnlinkItem>;
template class PltAssoc<KsString, KscServerBase *>;
template class PltAssoc<KsString, unsigned long>;
template class PltAssoc<PltKeyPlainConstPtr<KscAvModule>, PltPtrHandle<KscNegotiator> >;
template class PltAssoc<PltString, instance *>;
template class PltAssoc<PltString, link_value *>;
template class PltAssoc<PltString, variable_value *>;
template class PltBidirIterator<KsCreateObjItem>;
template class PltBidirIterator<KsCreateObjResultItem>;
template class PltBidirIterator<KsGetCanonicalPathResultItem>;
template class PltBidirIterator<KsLinkItem>;
template class PltBidirIterator<KsRenameObjItem>;
template class PltBidirIterator<KsUnlinkItem>;
template class PltBidirIterator<LogPath>;
template class PltBidirIterator<instance *>;
template class PltContainer<KsCreateObjItem>;
template class PltContainer<KsCreateObjResultItem>;
template class PltContainer<KsGetCanonicalPathResultItem>;
template class PltContainer<KsLinkItem>;
template class PltContainer<KsRenameObjItem>;
template class PltContainer<KsUnlinkItem>;
template class PltContainer<LogPath>;
template class PltContainer<PltAssoc<KsString, KscServerBase *> >;
template class PltContainer<PltAssoc<KsString, unsigned long> >;
template class PltContainer<PltAssoc<PltKeyPlainConstPtr<KscAvModule>, PltPtrHandle<KscNegotiator> > >;
template class PltContainer<PltAssoc<PltString, instance *> >;
template class PltContainer<PltAssoc<PltString, link_value *> >;
template class PltContainer<PltAssoc<PltString, variable_value *> >;
template class PltContainer<instance *>;
template class PltContainer_<KsCreateObjItem>;
template class PltContainer_<KsCreateObjResultItem>;
template class PltContainer_<KsGetCanonicalPathResultItem>;
template class PltContainer_<KsLinkItem>;
template class PltContainer_<KsRenameObjItem>;
template class PltContainer_<KsUnlinkItem>;
template class PltContainer_<LogPath>;
template class PltContainer_<PltAssoc<KsString, KscServerBase *> >;
template class PltContainer_<PltAssoc<KsString, unsigned long> >;
template class PltContainer_<PltAssoc<PltKeyPlainConstPtr<KscAvModule>, PltPtrHandle<KscNegotiator> > >;
template class PltContainer_<PltAssoc<PltString, instance *> >;
template class PltContainer_<PltAssoc<PltString, link_value *> >;
template class PltContainer_<PltAssoc<PltString, variable_value *> >;
template class PltContainer_<instance *>;
template class PltDictionary<KsString, KscServerBase *>;
template class PltDictionary<KsString, unsigned long>;
template class PltDictionary<PltKeyPlainConstPtr<KscAvModule>, PltPtrHandle<KscNegotiator> >;
template class PltDictionary<PltString, instance *>;
template class PltDictionary<PltString, link_value *>;
template class PltDictionary<PltString, variable_value *>;
template class PltHandle<KsCreateObjItem>;
template class PltHandle<KsCreateObjResultItem>;
template class PltHandle<KsGetCanonicalPathResultItem>;
template class PltHandle<KsLinkItem>;
template class PltHandle<KsRenameObjItem>;
template class PltHandle<KsUnlinkItem>;
template class PltHandle<KscCommObject>;
template class PltHandle<KscNegotiator>;
template class PltHashIterator<KsString, KscServerBase *>;
template class PltHashIterator<KsString, unsigned long>;
template class PltHashIterator<PltKeyPlainConstPtr<KscAvModule>, PltPtrHandle<KscNegotiator> >;
template class PltHashIterator<PltString, instance *>;
template class PltHashIterator<PltString, link_value *>;
template class PltHashIterator<PltString, variable_value *>;
template class PltHashTable<KsString, KscServerBase *>;
template class PltHashTable<KsString, unsigned long>;
template class PltHashTable<PltKeyPlainConstPtr<KscAvModule>, PltPtrHandle<KscNegotiator> >;
template class PltHashTable<PltString, instance *>;
template class PltHashTable<PltString, link_value *>;
template class PltHashTable<PltString, variable_value *>;
template class PltHashTable_<KsString, KscServerBase *>;
template class PltHashTable_<KsString, unsigned long>;
template class PltHashTable_<PltKeyPlainConstPtr<KscAvModule>, PltPtrHandle<KscNegotiator> >;
template class PltHashTable_<PltString, instance *>;
template class PltHashTable_<PltString, link_value *>;
template class PltHashTable_<PltString, variable_value *>;
template class PltIterator<KsCreateObjItem>;
template class PltIterator<KsCreateObjResultItem>;
template class PltIterator<KsGetCanonicalPathResultItem>;
template class PltIterator<KsLinkItem>;
template class PltIterator<KsRenameObjItem>;
template class PltIterator<KsUnlinkItem>;
template class PltIterator<LogPath>;
template class PltIterator<PltAssoc<KsString, KscServerBase *> >;
template class PltIterator<PltAssoc<KsString, unsigned long> >;
template class PltIterator<PltAssoc<PltKeyPlainConstPtr<KscAvModule>, PltPtrHandle<KscNegotiator> > >;
template class PltIterator<PltAssoc<PltString, instance *> >;
template class PltIterator<PltAssoc<PltString, link_value *> >;
template class PltIterator<PltAssoc<PltString, variable_value *> >;
template class PltIterator<instance *>;
template class PltIterator_<KsCreateObjItem>;
template class PltIterator_<KsCreateObjResultItem>;
template class PltIterator_<KsGetCanonicalPathResultItem>;
template class PltIterator_<KsLinkItem>;
template class PltIterator_<KsRenameObjItem>;
template class PltIterator_<KsUnlinkItem>;
template class PltIterator_<LogPath>;
template class PltIterator_<PltAssoc<KsString, KscServerBase *> >;
template class PltIterator_<PltAssoc<KsString, unsigned long> >;
template class PltIterator_<PltAssoc<PltKeyPlainConstPtr<KscAvModule>, PltPtrHandle<KscNegotiator> > >;
template class PltIterator_<PltAssoc<PltString, instance *> >;
template class PltIterator_<PltAssoc<PltString, link_value *> >;
template class PltIterator_<PltAssoc<PltString, variable_value *> >;
template class PltIterator_<instance *>;
template class PltKeyPlainConstPtr<KscAvModule>;
template class PltList<LogPath>;
template class PltList<instance *>;
template class PltListIterator<LogPath>;
template class PltListIterator<instance *>;
template class PltListNode<LogPath>;
template class PltListNode<instance *>;
template class PltPtrHandle<KscCommObject>;
template class PltPtrHandle<KscNegotiator>;
template class Plt_AtArrayNew<KsCreateObjItem>;
template class Plt_AtArrayNew<KsCreateObjResultItem>;
template class Plt_AtArrayNew<KsGetCanonicalPathResultItem>;
template class Plt_AtArrayNew<KsLinkItem>;
template class Plt_AtArrayNew<KsRenameObjItem>;
template class Plt_AtArrayNew<KsUnlinkItem>;
template class Plt_AtArrayNew<KscCommObject>;
template class Plt_AtArrayNew<KscNegotiator>;
template class Plt_AtNew<KsCreateObjItem>;
template class Plt_AtNew<KsCreateObjResultItem>;
template class Plt_AtNew<KsGetCanonicalPathResultItem>;
template class Plt_AtNew<KsLinkItem>;
template class Plt_AtNew<KsRenameObjItem>;
template class Plt_AtNew<KsUnlinkItem>;
template class Plt_AtNew<KscCommObject>;
template class Plt_AtNew<KscNegotiator>;