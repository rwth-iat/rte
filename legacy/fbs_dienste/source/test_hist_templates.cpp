// ACPLT/KS
#include "plt/config.h"
#include "ks/history.h"
#include "ks/clntpath.h"
#include "ks/package.h"
#include "ks/avsimplemodule.h"
#include "ks/objmgrparams.h"

#if PLT_COMPILER_GCC || PLT_COMPILER_DECCXX
#include "plt/config.h"
#include "plt/hashtable_impl.h"
#include "plt/handle_impl.h"
#endif

// Templates
#if PLT_SEE_ALL_TEMPLATES
#include "plt/hashtable.h"
#include "plt/priorityqueue.h"
#include "ks/array.h"
#include "ks/list.h"
#include "ks/handle.h"
#include "plt/sort.h"
#include "ks/conversions.h"
#else
#include "plt/hashtable_impl.h"
#include "plt/priorityqueue_impl.h"
#include "ks/array_impl.h"
#include "ks/list_impl.h"
#include "ks/handle_impl.h"
#include "plt/sort_impl.h"
#endif

template class KsArray<KsCreateObjItem>;
template class KsArray<KsCreateObjResultItem>;
template class KsArray<KsGetCanonicalPathResultItem>;
template class KsArray<KsGetHistItem>;
template class KsArray<KsGetHistResultItem>;
template class KsArray<KsGetHistSingleResult>;
template class KsArray<KsLinkItem>;
template class KsArray<KsRenameObjItem>;
template class KsArray<KsUnlinkItem>;
template class KsPtrHandle<KsSelector>;
template class PltArray<KsCreateObjItem>;
template class PltArray<KsCreateObjResultItem>;
template class PltArray<KsGetCanonicalPathResultItem>;
template class PltArray<KsGetHistItem>;
template class PltArray<KsGetHistResultItem>;
template class PltArray<KsGetHistSingleResult>;
template class PltArray<KsLinkItem>;
template class PltArray<KsRenameObjItem>;
template class PltArray<KsUnlinkItem>;
template class PltArrayHandle<KsCreateObjItem>;
template class PltArrayHandle<KsCreateObjResultItem>;
template class PltArrayHandle<KsGetCanonicalPathResultItem>;
template class PltArrayHandle<KsGetHistItem>;
template class PltArrayHandle<KsGetHistResultItem>;
template class PltArrayHandle<KsGetHistSingleResult>;
template class PltArrayHandle<KsLinkItem>;
template class PltArrayHandle<KsRenameObjItem>;
template class PltArrayHandle<KsUnlinkItem>;
template class PltContainer<KsGetHistItem>;
template class PltContainer<PltAssoc<KsString, KscServerBase *> >;
template class PltContainer<PltAssoc<KsString, unsigned long> >;
template class PltContainer<PltAssoc<PltKeyPlainConstPtr<KscAvModule>, PltPtrHandle<KscNegotiator> > >;
template class PltContainer_<KsCreateObjItem>;
template class PltContainer_<KsCreateObjResultItem>;
template class PltContainer_<KsGetCanonicalPathResultItem>;
template class PltContainer_<KsGetHistItem>;
template class PltContainer_<KsGetHistResultItem>;
template class PltContainer_<KsGetHistSingleResult>;
template class PltContainer_<KsLinkItem>;
template class PltContainer_<KsRenameObjItem>;
template class PltContainer_<KsUnlinkItem>;
template class PltContainer_<PltAssoc<KsString, KscServerBase *> >;
template class PltContainer_<PltAssoc<KsString, unsigned long> >;
template class PltContainer_<PltAssoc<PltKeyPlainConstPtr<KscAvModule>, PltPtrHandle<KscNegotiator> > >;
template class PltDictionary<KsString, KscServerBase *>;
template class PltDictionary<KsString, unsigned long>;
template class PltDictionary<PltKeyPlainConstPtr<KscAvModule>, PltPtrHandle<KscNegotiator> >;
template class PltHandle<KsCreateObjItem>;
template class PltHandle<KsCreateObjResultItem>;
template class PltHandle<KsGetCanonicalPathResultItem>;
template class PltHandle<KsGetHistItem>;
template class PltHandle<KsGetHistResultItem>;
template class PltHandle<KsGetHistSingleResult>;
template class PltHandle<KsLinkItem>;
template class PltHandle<KsRenameObjItem>;
template class PltHandle<KsSelector>;
template class PltHandle<KsUnlinkItem>;
template class PltHandle<KscNegotiator>;
template class PltHashIterator<KsString, KscServerBase *>;
template class PltHashTable<KsString, KscServerBase *>;
template class PltHashTable<KsString, unsigned long>;
template class PltHashTable<PltKeyPlainConstPtr<KscAvModule>, PltPtrHandle<KscNegotiator> >;
template class PltHashTable_<KsString, KscServerBase *>;
template class PltHashTable_<KsString, unsigned long>;
template class PltHashTable_<PltKeyPlainConstPtr<KscAvModule>, PltPtrHandle<KscNegotiator> >;
template class PltIterator<PltAssoc<KsString, KscServerBase *> >;
template class PltIterator_<PltAssoc<KsString, KscServerBase *> >;
template class PltList<KsGetHistItem *>;
template class PltList<KsSetVarItem *>;
template class PltList<KsString>;
template class PltPtrHandle<KscNegotiator>;


template class PltArrayIterator<KsCreateObjItem>;
template class PltArrayIterator<KsCreateObjResultItem>;
template class PltArrayIterator<KsGetCanonicalPathResultItem>;
template class PltArrayIterator<KsGetHistItem>;
template class PltArrayIterator<KsGetHistResultItem>;
template class PltArrayIterator<KsGetHistSingleResult>;
template class PltArrayIterator<KsLinkItem>;
template class PltArrayIterator<KsRenameObjItem>;
template class PltArrayIterator<KsUnlinkItem>;
template class PltAssoc<KsString, KscServerBase *>;
template class PltAssoc<KsString, unsigned long>;
template class PltAssoc<PltKeyPlainConstPtr<KscAvModule>, PltPtrHandle<KscNegotiator> >;
template class PltBidirIterator<KsGetHistItem *>;
template class PltBidirIterator<KsSetVarItem *>;
template class PltContainer<KsGetHistItem *>;
template class PltContainer<KsSetVarItem *>;
template class PltContainer_<KsGetHistItem *>;
template class PltContainer_<KsSetVarItem *>;
template class PltHashIterator<KsString, unsigned long>;
template class PltHashIterator<PltKeyPlainConstPtr<KscAvModule>, PltPtrHandle<KscNegotiator> >;
template class PltIterator<PltAssoc<KsString, unsigned long> >;
template class PltIterator<PltAssoc<PltKeyPlainConstPtr<KscAvModule>, PltPtrHandle<KscNegotiator> > >;
template class PltIterator_<PltAssoc<KsString, unsigned long> >;
template class PltIterator_<PltAssoc<PltKeyPlainConstPtr<KscAvModule>, PltPtrHandle<KscNegotiator> > >;
template class PltListIterator<KsGetHistItem *>;
template class PltListIterator<KsSetVarItem *>;
template class PltListIterator<KsString>;
template class Plt_AtArrayNew<KsCreateObjItem>;
template class Plt_AtArrayNew<KsCreateObjResultItem>;
template class Plt_AtArrayNew<KsGetCanonicalPathResultItem>;
template class Plt_AtArrayNew<KsGetHistItem>;
template class Plt_AtArrayNew<KsGetHistResultItem>;
template class Plt_AtArrayNew<KsGetHistSingleResult>;
template class Plt_AtArrayNew<KsLinkItem>;
template class Plt_AtArrayNew<KsRenameObjItem>;
template class Plt_AtArrayNew<KsSelector>;
template class Plt_AtArrayNew<KsUnlinkItem>;
template class Plt_AtArrayNew<KscNegotiator>;
template class Plt_AtNew<KsCreateObjItem>;
template class Plt_AtNew<KsCreateObjResultItem>;
template class Plt_AtNew<KsGetCanonicalPathResultItem>;
template class Plt_AtNew<KsGetHistItem>;
template class Plt_AtNew<KsGetHistResultItem>;
template class Plt_AtNew<KsGetHistSingleResult>;
template class Plt_AtNew<KsLinkItem>;
template class Plt_AtNew<KsRenameObjItem>;
template class Plt_AtNew<KsSelector>;
template class Plt_AtNew<KsUnlinkItem>;
template class Plt_AtNew<KscNegotiator>;

template class PltIterator_<KsCreateObjItem>;
template class PltIterator_<KsCreateObjResultItem>;
template class PltIterator_<KsGetCanonicalPathResultItem>;
template class PltIterator_<KsGetHistItem *>;
template class PltIterator_<KsGetHistItem>;
template class PltIterator_<KsGetHistResultItem>;
template class PltIterator_<KsGetHistSingleResult>;
template class PltIterator_<KsLinkItem>;
template class PltIterator_<KsRenameObjItem>;
template class PltIterator_<KsSetVarItem *>;
template class PltIterator_<KsUnlinkItem>;


template class PltBidirIterator<KsLinkItem>;
template class PltArrayed<KsLinkItem>;
template class PltBidirIterator<KsCreateObjItem>;
template class PltArrayed<KsCreateObjItem>;
template class PltBidirIterator<KsCreateObjResultItem>;
template class PltArrayed<KsCreateObjResultItem>;
template class PltBidirIterator<KsUnlinkItem>;
template class PltArrayed<KsUnlinkItem>;
template class PltBidirIterator<KsGetCanonicalPathResultItem>;
template class PltArrayed<KsGetCanonicalPathResultItem>;
template class PltBidirIterator<KsRenameObjItem>;
template class PltArrayed<KsRenameObjItem>;
template class PltPtrHandle<KsSelector>;
template class PltBidirIterator<KsGetHistItem>;
template class PltArrayed<KsGetHistItem>;
template class PltBidirIterator<KsGetHistResultItem>;
template class PltArrayed<KsGetHistResultItem>;
template class PltBidirIterator<KsGetHistSingleResult>;
template class PltArrayed<KsGetHistSingleResult>;
template class PltIterator<KsGetHistItem *>;
template class PltIterator<KsSetVarItem *>;

template class PltContainer<KsCreateObjItem>;
template class PltIterator<KsCreateObjResultItem>;
template class PltContainer<KsCreateObjResultItem>;
template class PltIterator<KsUnlinkItem>;
template class PltContainer<KsUnlinkItem>;
template class PltIterator<KsGetCanonicalPathResultItem>;
template class PltContainer<KsGetCanonicalPathResultItem>;
template class PltIterator<KsRenameObjItem>;
template class PltContainer<KsRenameObjItem>;
template class PltIterator<KsGetHistItem>;
template class PltIterator<KsGetHistResultItem>;
template class PltContainer<KsGetHistResultItem>;
template class PltIterator<KsGetHistSingleResult>;
template class PltContainer<KsGetHistSingleResult>;

template class PltIterator<KsLinkItem>;
template class PltContainer<KsLinkItem>;
template class PltIterator<KsCreateObjItem>;



template class PltContainer_<KscSortVarPtr>;
template class PltIterator<PltAssoc<KscSorter::Key, PltPtrHandle<KscSorterBucket> > >;
template class PltHashIterator<KscSorter::Key, PltPtrHandle<KscSorterBucket> >;
template class PltIterator_<PltAssoc<KscSorter::Key, PltPtrHandle<KscSorterBucket> > >;
template class PltIterator_<PltPtrHandle<KscSorterBucket> >;
template class PltPtrHandle<KscSorterBucket>;
template class PltBidirIterator<PltPtrHandle<KscVariable> >;
template class PltListIterator<PltPtrHandle<KscVariable> >;
template class PltBidirIterator<PltPtrHandle<KscPackage> >;
template class PltListIterator<PltPtrHandle<KscPackage> >;
template class PltIterator_<PltPtrHandle<KscPackage> > ;
template class PltIterator_<PltPtrHandle<KscVariable> >;
template class PltPtrHandle<KscVariable>;
template class PltArray<KscSortVarPtr>;
template class PltHandle<KscSortVarPtr>;
template class PltArrayHandle<KscSortVarPtr>;
template class PltIterator<PltPtrHandle<KscSorterBucket> >;
template class PltIterator<PltPtrHandle<KscVariable> >;
template class PltList<KscSortVarPtr>;
template class PltSort<KscSortVarPtr>;
template class PltDictionary<KscSorter::Key, PltPtrHandle<KscSorterBucket> >;
template class PltHashTable_<KscSorter::Key, PltPtrHandle<KscSorterBucket> >;
template class PltContainer_<PltAssoc<KscSorter::Key, PltPtrHandle<KscSorterBucket> > >;
template class PltHashTable<KscSorter::Key, PltPtrHandle<KscSorterBucket> >;
template class PltContainer<KscSortVarPtr>;
template class PltHandle<KscSorterBucket>;


template class PltIterator<PltPtrHandle<KscPackage> >;
template class PltHandle<KscVariable>;
template class PltArrayed<KscSortVarPtr>;
template class PltArrayIterator<KscSortVarPtr>;
template class PltList<KsGetHistItem>;
template class PltArray<KscVariable *>;
template class PltHandle<KscVariable *>;
template class PltArrayHandle<KscVariable *>;
template class PltContainer_<KscVariable *>;
template class KsList<KsGetHistItem>;
template class PltListIterator<KsGetHistItem>;
template class PltContainer<PltPtrHandle<KscVariable> >;
template class PltList<PltPtrHandle<KscVariable> >;
template class PltContainer<PltPtrHandle<KscPackage> >;
template class PltList<PltPtrHandle<KscPackage> >;

template class Plt_AtNew<KscSorterBucket>;
template class Plt_AtArrayNew<KscSorterBucket>;
template class Plt_AtNew<KscVariable>;
template class Plt_AtArrayNew<KscVariable>;
template class Plt_AtNew<KscSortVarPtr>;
template class Plt_AtArrayNew<KscSortVarPtr>;
template class PltAssoc<KscSorter::Key, PltPtrHandle<KscSorterBucket> >;
template class Plt_AtNew<KscVariable *>;
template class Plt_AtArrayNew<KscVariable *>;
template class PltContainer_<PltPtrHandle<KscVariable> >;
template class PltPtrHandle<KscPackage>;
template class PltContainer<PltAssoc<KscSorter::Key, PltPtrHandle<KscSorterBucket> > >;
template class PltBidirIterator<KscSortVarPtr>;
template class PltListIterator<KscSortVarPtr>;
template class PltContainer_<PltPtrHandle<KscPackage> >;
template class PltIterator_<KscSortVarPtr>;
template class PltArrayed<KscVariable *>;
template class PltArrayIterator<KscVariable *>;


template class PltIterator<KscSortVarPtr>;
template class PltIterator_<KscVariable *>;
template class PltBidirIterator<KscVariable *>;
template class PltContainer<KscVariable *>;
template class PltHandle<KscPackage>;


template class Plt_AtNew<KscPackage>;
template class Plt_AtArrayNew<KscPackage>;
template class PltIterator<KscVariable *>;

