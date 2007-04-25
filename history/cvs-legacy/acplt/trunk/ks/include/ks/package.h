/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/include/ks/package.h,v 1.23 2007-04-25 12:57:20 martin Exp $ */
#ifndef KSC_PACKAGE_INCLUDED 
#define KSC_PACKAGE_INCLUDED
/*
 * Copyright (c) 1996, 1997, 1998, 1999
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
    
    static bool fillGetVarParams(const PltArray< KscSortVarPtr > &,
                                 KsArray<KsString> &);
    static bool copyGetVarResults(const PltArray< KscSortVarPtr > &,
                                  const KsArray<KsGetVarItemResult> &);
    static bool fillSetVarParams(const PltArray< KscSortVarPtr > &,
                                 KsArray<KsSetVarItem> &);
    static bool copySetVarResults(const PltArray< KscSortVarPtr > &,
                                  const KsArray<KsResult> &);
    static bool optimizePaths(const PltArray< KscSortVarPtr > &,
                              KsArray<KsString> &);
    static void distributeErrorResult(KS_RESULT result,
				      const PltArray< KscSortVarPtr > &);
}; // class _KscPackageBase

//////////////////////////////////////////////////////////////////////
// class KscPackage
//////////////////////////////////////////////////////////////////////

typedef PltPtrHandle<class KscPackage> KscPackageHandle;
typedef PltIterator<KscVariableHandle> KscPkgVariableIterator;
typedef PltIterator<KscPackageHandle>  KscSubpackageIterator; 

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
    KS_RESULT getLastResult() const;

    bool getUpdate();
    bool setUpdate(bool force = false);

    KscPkgVariableIterator *newVariableIterator(bool deep=false) const;
    KscSubpackageIterator *newSubpackageIterator() const;

    void setAvModule(const KscAvModule *avm);
    const KscAvModule *getAvModule() const;

protected:
    bool getSimpleUpdate(KscBucketHandle);
    bool setSimpleUpdate(KscBucketHandle);

    PltList<KscVariableHandle> vars;
    size_t num_vars;

    PltList<KscPackageHandle> pkgs;
    size_t num_pkgs;

    const KscAvModule *av_module;

    // flags adding/removing variables and subpackages, but not
    // subvariables or sub-subpackages.
    bool _is_dirty;

    KS_RESULT _last_result;

    //
    // class DeepIterator
    // helper class to iterate over variables contained in a package
    // and his subpackages
    //
    class DeepIterator
    : public PltIterator<KscVariableHandle>
    {
    public:
#if PLT_RETTYPE_OVERLOADABLE
	#define KscPackage_DeepIterator_THISTYPE KscPackage::DeepIterator
        typedef DeepIterator THISTYPE;
#else
  #define KscPackage_DeepIterator_THISTYPE PltIterator_THISTYPE(KscVariableHandle)
#endif
        DeepIterator(const KscPackage &);
        ~DeepIterator();
        operator bool () const;
        const KscVariableHandle & operator * () const;
        const KscVariableHandle * operator -> () const;
        THISTYPE & operator ++ ();
        void operator ++ (int);
        void toStart();
    protected:
        void normalize();
        const KscPackage &pkg;
        DeepIterator *rek_it; // used to visit the subpackages recursively
        PltListIterator <KscVariableHandle> vars_it;
        PltListIterator <KscPackageHandle> pkgs_it;
    private:
        DeepIterator(const DeepIterator &); // forbidden
        DeepIterator &operator = (const DeepIterator &); // forbidden
    };
    friend class DeepIterator;
    // end of class DeepIterator

#if PLT_DEBUG
public:
    void debugPrint(ostream &, bool) const;
#endif
};

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

    KS_RESULT getLastResult() const;

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
    KS_RESULT _last_result;

#if PLT_DEBUG
public:
    void debugPrint(ostream &, bool) const;
#endif
};

//////////////////////////////////////////////////////////////////////
// Inline Implementation
//////////////////////////////////////////////////////////////////////
// class KscPackage
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
    return _is_dirty;
}

//////////////////////////////////////////////////////////////////////

inline
KS_RESULT
KscPackage::getLastResult() const
{
    return _last_result;
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
// class KscExchangePackage
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
  av_module(0),
  _last_result(-1)
{}

//////////////////////////////////////////////////////////////////////

inline
KscExchangePackage::~KscExchangePackage()
{}

//////////////////////////////////////////////////////////////////////

inline
KS_RESULT
KscExchangePackage::getLastResult() const
{
    return _last_result;
}

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

//////////////////////////////////////////////////////////////////////

#endif

//////////////////////////////////////////////////////////////////////
// EOF package.h
//////////////////////////////////////////////////////////////////////












