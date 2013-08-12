
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

#include "blockparam.h"
#include "ifbslib_params.h"
#include "par_param.h"

#if (PLT_SYSTEM_LINUX == 1) || (PLT_SYSTEM_SOLARIS == 1) || defined(__MINGW32__)

template class KsArray<KsString>;
template class KsVecValueBase<KsString>;
template class KsVecValueBase<KsTime>;
template class KsVecValueBase<KsTimeSpan>;
template class KsVecValueBase<bool>;
template class KsVecValueBase<double>;
template class KsVecValueBase<float>;
template class KsVecValueBase<int>;
template class KsVecValueBase<long>;
template class KsVecValueBase<unsigned long>;
template class PltHandle<KsCurrProps>;
template class PltHandle<KsResult>;
template class PltHandle<KsSetVarItem>;
template class PltHandle<KsString>;
template class PltHandle<KsValue>;
template class PltList<KsPtrHandle<KsEngProps> >;


template class PltIterator_<PltPtrHandle<KscVariable> >;
template class PltList<PltString>;
template class PltContainer_<KsCreateObjItem>;
template class KsArray<KsGetCanonicalPathResultItem>;
template class PltList<InstVarProps>;
template class PltKeyPlainConstPtr<KscAvModule>;
template class PltList<KsSetVarItem*>;
template class KsArray<KsCreateObjResultItem>;
template class PltListIterator<PltPtrHandle<KscPackage> >;
template class KsArray<KsUnlinkItem>;
template class PltContainer_<KscSortVarPtr>;
template class PltListIterator<PltPtrHandle<KscVariable> >;
template class PltPtrHandle<KscPackage>;
template class PltHandle<KscVariable>;
template class PltArrayHandle<KsCreateObjResultItem>;
template class PltArrayHandle<KsCreateObjItem>;
template class PltArray<KscSortVarPtr>;
template class PltHandle<KsCreateObjItem>;
template class KsArray<KsLinkItem>;
template class PltArrayHandle<KsUnlinkItem>;
template class PltContainer_<KsGetCanonicalPathResultItem>;
template class PltList<ObjProps>;
template class PltPtrHandle<KscSorterBucket>;
template class PltContainer_<KsCreateObjResultItem>;
template class KsArray<KsRenameObjItem>;
template class PltArray<char*>;
template class PltHandle<KsGetCanonicalPathResultItem>;
template class PltArray<KsGetCanonicalPathResultItem>;
template class PltHandle<KscVariable*>;
template class PltList<KscSortVarPtr>;
template class KsArray<KsCreateObjItem>;
template class PltHandle<KscSorterBucket>;
template class PltArrayHandle<KsLinkItem>;
template class PltArray<KsRenameObjItem>;
template class PltList<PltPtrHandle<KscPackage> >;
template class PltContainer_<KsUnlinkItem>;
template class PltContainer_<KsRenameObjItem>;
template class PltArrayHandle<char*>;
template class PltArrayHandle<KscVariable*>;
template class PltArray<KsUnlinkItem>;
template class PltHandle<KscSortVarPtr>;
template class PltIterator<PltPtrHandle<KscVariable> >;
template class PltList<ConData>;
template class PltPtrHandle<KscNegotiator>;

template class PltContainer_<char*>;
template class PltArrayHandle<KsRenameObjItem>;
template class PltHandle<KsRenameObjItem>;
template class PltIterator_<PltPtrHandle<KscPackage> >;
template class PltPtrHandle<KscVariable>;
template class PltHandle<KscNegotiator>;
template class PltArray<KsLinkItem>;
template class PltContainer_<KscVariable*>;
template class PltSort<KscSortVarPtr>;
template class PltHandle<KsLinkItem>;
template class PltArray<KscVariable*>;
template class PltHandle<KsUnlinkItem>;
template class PltIterator_<PltPtrHandle<KscSorterBucket> >;
template class PltContainer_<KsLinkItem>;
template class PltIterator<PltPtrHandle<KscSorterBucket> >;
template class PltHandle<KsCreateObjResultItem>;
template class KsArray<KsSetVarItem>;
template class PltArray<KsCreateObjResultItem>;
template class PltArrayHandle<KscSortVarPtr>;
template class PltArray<KsCreateObjItem>;
template class PltArrayHandle<KsGetCanonicalPathResultItem>;

template class PltHashTable_<PltKeyPlainConstPtr<KscAvModule>, PltPtrHandle<KscNegotiator> >;
template class PltContainer_<PltAssoc<KscSorter::Key, PltPtrHandle<KscSorterBucket> > >;
template class PltList<PltPtrHandle<KscVariable> >;
template class PltDictionary<KsString, KscServerBase*>;
template class PltHashTable_<KscSorter::Key, PltPtrHandle<KscSorterBucket> >;
template class PltDictionary<PltKeyPlainConstPtr<KscAvModule>, PltPtrHandle<KscNegotiator> >;
template class PltHashTable_<KsString, KscServerBase*>;
template class PltIterator<PltAssoc<KsString, KscServerBase*> >;
template class PltHashTable_<KsString, unsigned int>;
template class PltIterator_<PltAssoc<KscSorter::Key, PltPtrHandle<KscSorterBucket> > >;
template class PltIterator_<PltAssoc<KsString, KscServerBase*> >;
template class PltContainer_<PltAssoc<KsString, KscServerBase*> >;
template class PltContainer_<PltAssoc<PltKeyPlainConstPtr<KscAvModule>, PltPtrHandle<KscNegotiator> > >;
template class PltHashTable<KsString, KscServerBase*>;
template class PltDictionary<KsString, unsigned int>;
template class PltHashIterator<KscSorter::Key, PltPtrHandle<KscSorterBucket> >;
template class PltHashTable<KsString, unsigned int>;
template class PltHashTable<KscSorter::Key, PltPtrHandle<KscSorterBucket> >;
template class PltHandle<char*>;
template class PltDictionary<KscSorter::Key, PltPtrHandle<KscSorterBucket> >;
template class PltHashIterator<KsString, KscServerBase*>;
template class PltIterator<PltAssoc<KscSorter::Key, PltPtrHandle<KscSorterBucket> > >;
template class PltHashTable<PltKeyPlainConstPtr<KscAvModule>, PltPtrHandle<KscNegotiator> >;

template class PltContainer<PltAssoc<KsString, KscServerBase*> >;
template class PltContainer<PltAssoc<KsString, unsigned> >;
template class PltContainer<PltAssoc<KscSorter::Key, PltPtrHandle<KscSorterBucket> > >;
template class PltContainer<PltAssoc<PltKeyPlainConstPtr<KscAvModule>, PltPtrHandle<KscNegotiator> > >;
template class PltHandle<KscPackage>;
template class PltHashTable_<KsString, unsigned long>;

template class PltListIterator<KscSortVarPtr>;
template class Plt_AtNew<char*>;
template class PltContainer_<PltString>;
template class PltArrayIterator<KsRenameObjItem>;
template class PltArrayIterator<KsLinkItem>;
template class PltArrayIterator<KsCreateObjResultItem>;
template class PltListIterator<ObjProps>;
template class Plt_AtArrayNew<char*>;
template class Plt_AtNew<KsGetCanonicalPathResultItem>;
template class PltArrayIterator<KsGetCanonicalPathResultItem>;
template class PltListIterator<KsSetVarItem*>;
template class Plt_AtNew<KscNegotiator>;
template class PltHashIterator<KsString, unsigned>;
template class PltIterator<PltAssoc<PltKeyPlainConstPtr<KscAvModule>, PltPtrHandle<KscNegotiator> > >;
template class PltContainer_<KsSetVarItem*>;
template class Plt_AtArrayNew<KsLinkItem>;
template class Plt_AtArrayNew<KsCreateObjItem>;
template class PltHashTable<KsString, unsigned long>;
template class Plt_AtArrayNew<KscNegotiator>;
template class Plt_AtNew<KscPackage>;
template class PltContainer_<PltPtrHandle<KscPackage> >;
template class Plt_AtNew<KsUnlinkItem>;
template class PltContainer_<PltPtrHandle<KscVariable> >;
template class Plt_AtArrayNew<KsGetCanonicalPathResultItem>;
template class PltArrayIterator<KsUnlinkItem>;
template class PltListIterator<ConData>;
template class PltArrayIterator<char*>;
template class Plt_AtNew<KsCreateObjItem>;

template class PltContainer_<ConData>;
template class PltHashIterator<KsString, unsigned long>;
template class PltContainer_<InstVarProps>;
template class Plt_AtNew<KscSortVarPtr>;
template class Plt_AtArrayNew<KsUnlinkItem>;
template class PltIterator_<PltAssoc<PltKeyPlainConstPtr<KscAvModule>, PltPtrHandle<KscNegotiator> > >;
template class PltAssoc<KscSorter::Key, PltPtrHandle<KscSorterBucket> >;
template class Plt_AtNew<KscVariable>;
template class PltArrayIterator<KsCreateObjItem>;
template class Plt_AtArrayNew<KscSortVarPtr>;
template class Plt_AtNew<KscVariable*>;
template class PltAssoc<KsString, KscServerBase*>;
template class PltContainer_<ObjProps>;
template class Plt_AtNew<KsCreateObjResultItem>;
template class PltArrayIterator<KscVariable*>;
template class PltIterator_<PltAssoc<KsString, unsigned> >;
template class PltIterator<PltAssoc<KsString, unsigned long> >;
template class PltDictionary<KsString, unsigned long>;
template class Plt_AtArrayNew<KsRenameObjItem>;
template class PltAssoc<KsString, unsigned>;
template class Plt_AtArrayNew<KsCreateObjResultItem>;
template class Plt_AtNew<KsLinkItem>;
template class Plt_AtArrayNew<KscPackage>;
template class Plt_AtArrayNew<KscSorterBucket>;
template class PltIterator<PltAssoc<KsString, unsigned> >;
template class PltContainer_<PltAssoc<KsString, unsigned long> >;
template class PltHashIterator<PltKeyPlainConstPtr<KscAvModule>, PltPtrHandle<KscNegotiator> >;
template class Plt_AtArrayNew<KscVariable*>;
template class Plt_AtNew<KsRenameObjItem>;
template class PltAssoc<KsString, unsigned long>;
template class PltAssoc<PltKeyPlainConstPtr<KscAvModule>, PltPtrHandle<KscNegotiator> >;
template class PltArrayIterator<KscSortVarPtr>;
template class PltListIterator<InstVarProps>;
template class PltIterator_<PltAssoc<KsString, unsigned long> >;
template class PltContainer_<PltAssoc<KsString, unsigned> >;
template class Plt_AtArrayNew<KscVariable>;
template class Plt_AtNew<KscSorterBucket>;
template class PltListIterator<PltString>;

template class PltIterator_<KsUnlinkItem>;
template class PltIterator_<ObjProps>;
template class PltIterator_<PltString>;
template class PltIterator_<char*>;
template class PltIterator_<KscSortVarPtr>;
template class PltIterator_<KscVariable*>;
template class PltIterator_<KsGetCanonicalPathResultItem>;
template class PltIterator_<InstVarProps>;
template class PltIterator_<KsRenameObjItem>;
template class PltIterator_<ConData>;
template class PltIterator_<KsLinkItem>;
template class PltIterator_<KsCreateObjResultItem>;
template class PltIterator_<KsCreateObjItem>;
template class PltIterator_<KsSetVarItem*>;
template class PltContainer<PltAssoc<KsString, unsigned long> >;

template class PltBidirIterator<ConData>;
template class PltContainer<PltPtrHandle<KscVariable> >;
template class PltArrayed<KsUnlinkItem>;
template class PltContainer<KsSetVarItem*>;
template class PltBidirIterator<InstVarProps>;
template class PltBidirIterator<KsCreateObjResultItem>;
template class PltBidirIterator<KsSetVarItem*>;
template class PltBidirIterator<KsCreateObjItem>;
template class PltArrayed<KsRenameObjItem>;
template class PltArrayed<KscVariable*>;
template class PltArrayed<KsGetCanonicalPathResultItem>;
template class PltContainer<ObjProps>;
template class PltContainer<PltString>;
template class PltBidirIterator<PltString>;
template class PltContainer<KscSortVarPtr>;
template class PltArrayed<KsCreateObjResultItem>;
template class PltBidirIterator<char*>;
template class PltContainer<PltPtrHandle<KscPackage> >;
template class PltBidirIterator<KscSortVarPtr>;
template class PltArrayed<KsCreateObjItem>;
template class PltBidirIterator<PltPtrHandle<KscPackage> >;
template class PltContainer<InstVarProps>;
template class PltBidirIterator<PltPtrHandle<KscVariable> >;
template class PltBidirIterator<KsLinkItem>;
template class PltArrayed<char*>;
template class PltArrayed<KscSortVarPtr>;
template class PltBidirIterator<KsUnlinkItem>;
template class PltBidirIterator<ObjProps>;
template class PltArrayed<KsLinkItem>;
template class PltBidirIterator<KsRenameObjItem>;
template class PltBidirIterator<KscVariable*>;
template class PltContainer<ConData>;
template class PltBidirIterator<KsGetCanonicalPathResultItem>;

template class PltContainer<KsLinkItem>;
template class PltContainer<KsCreateObjResultItem>;
template class PltContainer<KsCreateObjItem>;
template class PltIterator<KsUnlinkItem>;
template class PltIterator<ObjProps>;
template class PltContainer<KsUnlinkItem>;
template class PltIterator<KsRenameObjItem>;
template class PltIterator<KscVariable*>;
template class PltContainer<char*>;
template class PltIterator<KsGetCanonicalPathResultItem>;
template class PltContainer<KscVariable*>;
template class PltIterator<ConData>;
template class PltContainer<KsGetCanonicalPathResultItem>;
template class PltIterator<InstVarProps>;
template class PltIterator<KsCreateObjResultItem>;
template class PltIterator<KsSetVarItem*>;
template class PltIterator<KsCreateObjItem>;
template class PltIterator<PltString>;
template class PltIterator<char*>;
template class PltIterator<KscSortVarPtr>;
template class PltIterator<PltPtrHandle<KscPackage> >;
template class PltContainer<KsRenameObjItem>;
template class PltIterator<KsLinkItem>;

template class PltHandle<PltString>;
template class PltArrayed<PltString>;
template class PltArray<PltString>;
template class PltArrayIterator<PltString>;
template class PltArrayHandle<PltString>;
template class Plt_AtArrayNew<PltString>;
template class Plt_AtNew<PltString>;

template class PltHandle<Variables*>;
template class PltArray<Variables*>;
template class PltContainer_<Variables*>;
template class PltArrayHandle<Variables*>;
template class Plt_AtArrayNew<Variables*>;
template class PltArrayIterator<Variables*>;
template class Plt_AtNew<Variables*>;
template class PltIterator<Variables*>;
template class PltIterator_<Variables*>;
template class PltArrayed<Variables*>;
template class PltContainer<Variables*>;
template class PltBidirIterator<Variables*>;


#endif

#if PLT_SYSTEM_NT == 1 && !defined(__MINGW32__)

template class PltList<ObjProps>;
template class PltHandle<KscVariable>;
template class PltList<PltPtrHandle<KscVariable> >;
template class PltList<PltPtrHandle<KscPackage> >;
template class PltList<InstVarProps>;
template class PltList<PltString>;
template class PltList<ConData>;
template class PltHandle<KsCreateObjItem>;
template class PltList<KsSetVarItem*>;
template class PltHandle<KsLinkItem>;
template class KsArray<KsLinkItem>;
template class PltHandle<KscNegotiator>;
template class PltHandle<KsCreateObjResultItem>;
template class PltHandle<KsUnlinkItem>;
template class PltHandle<KsGetCanonicalPathResultItem>;
template class PltHandle<KsRenameObjItem>;
template class KsArray<KsRenameObjItem>;
template class KsArray<KsUnlinkItem>;
template class KsArray<KsGetCanonicalPathResultItem>;
template class KsArray<KsCreateObjResultItem>;
template class PltHashTable_<KsString,KscServerBase*>;
template class PltHashTable_<KsString,unsigned long>;
template class PltHashTable_<PltKeyCPtr<KscAvModule>,PltPtrHandle<KscNegotiator> >;
template class PltHandle<KscSortVarPtr>;
template class PltList<KscSortVarPtr>;
template class PltHandle<KscSorterBucket>;
template class PltContainer_<PltPtrHandle<KscPackage> >;
template class PltContainer_<PltPtrHandle<KscVariable> >;
template class PltContainer_<PltString>;
template class PltContainer_<InstVarProps>;
template class PltArray<KsCreateObjItem>;
template class PltArrayed<KsCreateObjItem>;
template class PltContainer<KsCreateObjItem>;
template class PltContainer_<KsCreateObjResultItem>;
template class PltContainer_<KsLinkItem>;
template class PltContainer_<KsSetVarItem*>;
template class PltContainer<KsSetVarItem*>;
template class PltArray<KsLinkItem>;
template class PltArrayed<KsLinkItem>;
template class PltContainer<KsLinkItem>;
template class PltContainer_<KsRenameObjItem>;
template class PltArray<KsRenameObjItem>;
template class PltArrayed<KsRenameObjItem>;
template class PltContainer<KsRenameObjItem>;
template class PltContainer_<KsUnlinkItem>;
template class PltArray<KsUnlinkItem>;
template class PltArrayed<KsUnlinkItem>;
template class PltContainer<KsUnlinkItem>;
template class PltContainer_<KsGetCanonicalPathResultItem>;
template class PltContainer<KsGetCanonicalPathResultItem>;
template class PltArrayed<KsGetCanonicalPathResultItem>;
template class PltArray<KsGetCanonicalPathResultItem>;
template class PltContainer<KsCreateObjResultItem>;
template class PltArrayed<KsCreateObjResultItem>;
template class PltArray<KsCreateObjResultItem>;
template class PltContainer_<PltAssoc<KsString,unsigned long> >;
template class PltContainer<PltAssoc<KsString,unsigned long> >;
template class PltHashTable<KsString,unsigned long>;
template class PltContainer_<PltAssoc<PltKeyCPtr<KscAvModule>,PltPtrHandle<KscNegotiator> > >;
template class PltContainer<PltAssoc<PltKeyCPtr<KscAvModule>,PltPtrHandle<KscNegotiator> > >;
template class PltHashTable<PltKeyCPtr<KscAvModule>,PltPtrHandle<KscNegotiator> >;
template class PltIterator<PltAssoc<KsString,KscServerBase*> >;
template class PltHashIterator<KsString,KscServerBase*>;
template class PltContainer_<PltAssoc<KsString,KscServerBase*> >;
template class PltContainer<PltAssoc<KsString,KscServerBase*> >;
template class PltHashTable<KsString,KscServerBase*>;
template class PltContainer_<KscSortVarPtr>;
template class PltContainer<KscSortVarPtr>;
template class PltArrayed<KscSortVarPtr>;
template class PltArray<KscSortVarPtr>;
template class PltIterator<PltPtrHandle<KscSorterBucket> >;
template class PltIterator<PltPtrHandle<KscVariable> >;
template class PltListIterator<PltPtrHandle<KscVariable> >;
template class PltIterator<PltPtrHandle<KscPackage> >;
template class PltListIterator<PltPtrHandle<KscPackage> >;
template class PltContainer<PltPtrHandle<KscPackage> >;
template class PltContainer<PltPtrHandle<KscVariable> >;
template class PltSort<KscSortVarPtr>;

template class PltHandle<KscVariable*>;
template class PltContainer_<KscVariable*>;
template class PltContainer<KscVariable*>;
template class PltArrayed<KscVariable*>;
template class PltArray<KscVariable*>;

template class PltHandle<char*>;
template class PltContainer_<char*>;
template class PltContainer<char*>;
template class PltArrayed<char*>;
template class PltArray<char*>;
template class PltList<int>;

template class PltHandle<PltString>;
template class PltContainer_<PltString>;
template class PltContainer<PltString>;
template class PltArrayed<PltString>;
template class PltArray<PltString>;

template class PltHandle<Variables*>;
template class PltContainer_<Variables*>;
template class PltContainer<Variables*>;
template class PltArrayed<Variables*>;
template class PltArray<Variables*>;


#endif
