/* -*-plt-c++-*- */

#ifndef KSC_PACKAGE_INCLUDED 
#define KSC_PACKAGE_INCLUDED

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

/* Author: Markus Juergens <markusj@plt.rwth-aachen.de> */

//////////////////////////////////////////////////////////////////////

#include <plt/list.h>

#include "ks/commobject.h"
#include "ks/sorter.h"

//////////////////////////////////////////////////////////////////////
// class _KscPackageBase
//   encapsulates some copy routines to create service parameters and
//   copy results back to KscVariable objects
//
class _KscPackageBase
{
protected:
    _KscPackageBase() {}
    
    static bool fillGetVarParams(const PltArray<KscVariableHandle> &,
                                 KsArray<KsString> &);
    static bool copyGetVarResults(const PltArray<KscVariableHandle> &,
                                  const KsArray<KsGetVarItemResult> &);
    static bool fillSetVarParams(const PltArray<KscVariableHandle> &,
                                 KsArray<KsSetVarItem> &);
    static bool copySetVarResults(const PltArray<KscVariableHandle> &,
                                  const KsArray<KsResult> &);
    static bool optimizePaths(const PltArray<KscVariableHandle> &,
                              KsArray<KsString> &);
};                                 

//////////////////////////////////////////////////////////////////////
// class KscPackage
//
//   RESTRICTIONS:
//   - Related objects are currently referenced by pointers. Therefore
//     equality is checked by comparing pointers.
// 
//////////////////////////////////////////////////////////////////////

typedef PltPtrHandle<class KscPackage> KscPackageHandle;

class KscPackage
: private _KscPackageBase
{
public:
    KscPackage();
    ~KscPackage();

    bool add(KscVariableHandle var);
    bool add(KscPackageHandle pkg);
    bool remove(KscVariableHandle var);
    bool remove(KscPackageHandle pkg);

    size_t sizeVariables(bool deep = false) const;
    size_t sizeSubpackages() const;

    bool isDirty() const;

    bool getUpdate();
    bool setUpdate(bool force = false);

    PltIterator<KscVariableHandle> *newVariableIterator(bool deep=false) const;
    PltIterator<KscPackageHandle> *newSubpackageIterator() const;

    void setAvModule(const KscAvModule *avm);
    const KscAvModule *getAvModule() const;

protected:
//    KscNegotiator *getNegotiatorForBucket(const KscSorterBucket &);

    bool getSimpleUpdate(KscBucketHandle);
    bool setSimpleUpdate(KscBucketHandle);

    PltList<KscVariableHandle> vars;
    size_t num_vars;

    PltList<KscPackageHandle> pkgs;
    size_t num_pkgs;

    const KscAvModule *av_module;

    // TODO: iterate over elements to determine status
    bool fDirty;

    //
    // class DeepIterator
    // helper class to iterate over variables contained in a package
    // and his subpackages
    //
    class DeepIterator
    : public PltIterator<KscVariableHandle>
    {
    public:
        DeepIterator(const KscPackage &);
        ~DeepIterator();
        operator const void * () const;
        const KscVariableHandle & operator * () const;
        const KscVariableHandle * operator -> () const;
        DeepIterator & operator ++ ();
        void operator ++ (int);
        void toStart();
    protected:
        const KscPackage &pkg;
        DeepIterator *rek_it; // used to visit the subpackages recursively
        PltListIterator <KscVariableHandle> vars_it;
        PltListIterator <KscPackageHandle> pkgs_it;
    };
    friend class DeepIterator;
    // end of class DeepIterator

#if PLT_DEBUG
public:
    void debugPrint(ostream &, bool) const;
#endif
};

# if 0

//
// class KscDirectIterator
// helper class to map an iterator over pointers
// to an iterator over elements
//
template <class T>
class KscDirectIterator
: public PltIterator<T> 
{
public:
    KscDirectIterator(const PltList<T *> &);

    operator const void * () const;         // remaining element?
    const T * operator -> () const;       
    KscDirectIterator & operator ++ ();        // advance
    void operator ++ (int);                 // (postfix)
    void toStart();                         // go to the beginning
protected:
    PltListIterator<T *> it;
};
// end of class KscDirectIterator

#endif

//////////////////////////////////////////////////////////////////////
// class KscExchangePackage
//////////////////////////////////////////////////////////////////////

class KscExchangePackage
: private _KscPackageBase
{
public:
    KscExchangePackage();
    KscExchangePackage(KscPackageHandle setPkg, 
                       KscPackageHandle getPkg);
    ~KscExchangePackage();

    void setPackages(KscPackageHandle setPkg,
                     KscPackageHandle getPkg);
    void getPackages(KscPackageHandle &setPkg,
                     KscPackageHandle &getPkg);

    bool doExchange(bool force = true);

    void setAvModule(const KscAvModule *avm);
    const KscAvModule *getAvModule() const;

protected:
    bool mergeSorters(KscSorter &, KscSorter &);
    bool doSimpleExchange(KscBucketHandle, KscBucketHandle);

    KscPackageHandle get_pkg,
                     set_pkg;

    const KscAvModule *av_module;

#if PLT_DEBUG
public:
    void debugPrint(ostream &, bool) const;
#endif
};
    
//////////////////////////////////////////////////////////////////////
// Inline Implementation
//////////////////////////////////////////////////////////////////////

inline
size_t 
KscPackage::sizeSubpackages() const
{
    return num_pkgs;
}

//////////////////////////////////////////////////////////////////////

inline
bool
KscPackage::isDirty() const
{
    return fDirty;
}

//////////////////////////////////////////////////////////////////////

inline
void
KscPackage::setAvModule(const KscAvModule *avm)
{
    // don't delete old AvModule
    av_module = avm;
}

//////////////////////////////////////////////////////////////////////

inline 
const KscAvModule *
KscPackage::getAvModule() const
{
    return av_module;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

inline
KscExchangePackage::KscExchangePackage()
: av_module(0)
{}

//////////////////////////////////////////////////////////////////////

inline
KscExchangePackage::KscExchangePackage(KscPackageHandle setPkg,
                                       KscPackageHandle getPkg)
: get_pkg(getPkg),
  set_pkg(setPkg),
  av_module(0)
{}

//////////////////////////////////////////////////////////////////////

inline
KscExchangePackage::~KscExchangePackage()
{}

//////////////////////////////////////////////////////////////////////

inline void 
KscExchangePackage::setPackages(KscPackageHandle setPkg,
                                KscPackageHandle getPkg)
{
    get_pkg = getPkg;
    set_pkg = setPkg;
}

//////////////////////////////////////////////////////////////////////

inline void
KscExchangePackage::getPackages(KscPackageHandle &setPkg,
                                KscPackageHandle &getPkg)
{
    getPkg = get_pkg;
    setPkg = set_pkg;
}

//////////////////////////////////////////////////////////////////////

inline void
KscExchangePackage::setAvModule(const KscAvModule *avModule)
{
    av_module = avModule;
}

//////////////////////////////////////////////////////////////////////

inline 
const KscAvModule * 
KscExchangePackage::getAvModule() const
{
    return av_module;
}

#endif












