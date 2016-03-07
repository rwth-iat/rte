/*****************************************************************************
*                                                                            *
*   x i F B S p r o                                                          *
*   ###############                                                          *
*                                                                            *
*   L T S o f t                                                              *
*   Agentur für Leittechnik Software GmbH                                    *
*   Heinrich-Hertz-Straße 10                                                 *
*   50170 Kerpen                                                             *
*   Tel      : 02273/9893-0                                                  *
*   Fax      : 02273/52526                                                   *
*   e-Mail   : ltsoft@ltsoft.de                                              *
*   Internet : http://www.ltsoft.de                                          *
*                                                                            *
******************************************************************************
*                                                                            *
*   xiFBSpro - Bibliothek                                                    *
*   xIFBSPRO - iFBSpro  ACPLT/KS Dienste-Schnittstelle (C++)                 *
*   ========================================================                 *
*                                                                            *
*   Datei                                                                    *
*   -----                                                                    *
*   xfb_templates.cpp                                                       *
*                                                                            *
*   Historie                                                                 *
*   --------                                                                 *
*   1999-04-07  Alexander Neugebauer: Erstellung, LTSoft, Kerpen             *
*                                                                            *
*                                                                            *
*   Beschreibung                                                             *
*   ------------                                                             *
*   Definition der Templates                                                 *
*                                                                            *
*****************************************************************************/

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

#if PLT_INSTANTIATE_TEMPLATES

template class PltList<ConData>;
template class PltList<InstVarProps>;
template class PltList<KsSetVarItem*>;
template class PltList<ObjProps>;
template class PltList<PltPtrHandle<KscVariable> >;
template class PltList<PltPtrHandle<KscPackage> >;
template class PltList<PltString>;
template class PltList<KscSortVarPtr>;
template class PltHandle<KsCreateObjItem>;
template class PltHandle<KscNegotiator>;
template class PltHandle<KscVariable>;
template class PltHandle<KscSortVarPtr>;
template class PltHandle<KsLinkItem>;
template class PltHandle<KscSorterBucket>;
template class PltHashTable_<KsString,KscServerBase*>;
template class PltHashTable_<KsString,unsigned long>;
template class PltHashTable_<PltKeyCPtr<KscAvModule>,PltPtrHandle<KscNegotiator> >;
template class PltHandle<KsCreateObjResultItem>;
template class PltHandle<KsUnlinkItem>;
template class PltHandle<KsGetCanonicalPathResultItem>;
template class PltHandle<KsRenameObjItem>;
template class KsArray<KsRenameObjItem>;
template class KsArray<KsGetCanonicalPathResultItem>;
template class KsArray<KsUnlinkItem>;
KsArray<KsCreateObjResultItem>;
template class PltContainer_<PltPtrHandle<KscPackage> >;
template class PltContainer_<PltPtrHandle<KscVariable> >;
template class PltContainer_<PltString>;
template class PltContainer<PltString>;
template class PltContainer_<InstVarProps>;
template class PltContainer<InstVarProps>;
template class PltContainer_<KsCreateObjItem>;
template class PltArray<KsCreateObjItem>;
template class PltArrayed<KsCreateObjItem>;
template class PltContainer<KsCreateObjItem>;
template class PltContainer_<KsCreateObjResultItem>;
template class PltContainer_<KsLinkItem>;
template class PltArray<KsLinkItem>;
template class PltArrayed<KsLinkItem>;
template class PltContainer<KsLinkItem>;
template class PltContainer_<KsSetVarItem*>;
template class PltContainer<KsSetVarItem*>;
template class PltContainer_<ConData>;
template class PltContainer<ConData>;
template class PltContainer_<ObjProps>;
template class PltContainer<ObjProps>;
template class PltContainer_<PltAssoc<KsString,unsigned long> >;
template class PltContainer<PltAssoc<KsString,unsigned long> >;
template class PltContainer_<PltAssoc<PltKeyCPtr<KscAvModule>,PltPtrHandle<KscNegotiator> > >;
template class PltContainer<PltAssoc<PltKeyCPtr<KscAvModule>,PltPtrHandle<KscNegotiator> > >;
template class PltHashTable<PltKeyCPtr<KscAvModule>,PltPtrHandle<KscNegotiator> >;
template class PltIterator<PltAssoc<KsString,KscServerBase*> >;
template class PltHashIterator<KsString,KscServerBase*>;
template class PltContainer_<PltAssoc<KsString,KscServerBase*> >;
template class PltContainer<PltAssoc<KsString,KscServerBase*> >;
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
template class PltContainer_<KsRenameObjItem>;
template class PltContainer<KsRenameObjItem>;
template class PltArrayed<KsRenameObjItem>;
template class PltArray<KsRenameObjItem>;
template class PltContainer_<KsGetCanonicalPathResultItem>;
template class PltContainer<KsGetCanonicalPathResultItem>;
template class PltArrayed<KsGetCanonicalPathResultItem>;
template class PltArray<KsGetCanonicalPathResultItem>;
template class PltContainer_<KsUnlinkItem>;
template class PltContainer<KsUnlinkItem>;
template class PltArrayed<KsUnlinkItem>;
template class PltArray<KsUnlinkItem>;
template class PltContainer<KsCreateObjResultItem>;
template class PltArrayed<KsCreateObjResultItem>;
template class PltArray<KsCreateObjResultItem>;
template class PltSort<KscSortVarPtr>;

#endif
