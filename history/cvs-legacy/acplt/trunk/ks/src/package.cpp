/* -*-plt-c++-*- */

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

#include "ks/package.h"
#include "ks/client.h"

//////////////////////////////////////////////////////////////////////
// printing functions for debbugging
//////////////////////////////////////////////////////////////////////
#if PLT_DEBUG

void
KscPackage::debugPrint(ostream &os, bool printAll) const
{
    os << "KscPackage object :" << endl;
    os << "\tLast result : " << getLastResult() << endl;
    os << "\tVariables : " << sizeVariables() << endl;
    os << "\tSubpackages : " << sizeSubpackages() << endl;

    if(printAll) {
        // print variables
        //
        PltIterator<KscVariableHandle> *varit =
            newVariableIterator(false);
        if(varit) {
            while(*varit) {
                // os << (*varit)->getFullPath() << endl;
                (**varit)->debugPrint(os);
                ++(*varit);
            }
            delete varit;
        }
        // print subpackages
        //
        PltIterator<KscPackageHandle> *pkgit =
            newSubpackageIterator();
        if(pkgit) {
            while(*pkgit) {
                (**pkgit)->debugPrint(os,true);
                ++(*pkgit);
            }
            delete pkgit;
        }
    }
}

//////////////////////////////////////////////////////////////////////

void
KscExchangePackage::debugPrint(ostream &os, bool printAll) const
{
    os << "KscExchangePackage object :" << endl;
    os << "Last result : " << getLastResult() << endl;
    os << "package to get :" << endl;
    get_pkg->debugPrint(os,printAll);
    os << "package to set :" << endl;
    set_pkg->debugPrint(os,printAll);
}

#endif
//////////////////////////////////////////////////////////////////////
// end of debbugging section               
//////////////////////////////////////////////////////////////////////

KscPackage::KscPackage()
: num_vars(0),
  num_pkgs(0),
  av_module(0),
  fDirty(false),
  _result(-1)
{}

//////////////////////////////////////////////////////////////////////

KscPackage::~KscPackage()
{}

//////////////////////////////////////////////////////////////////////

bool
KscPackage::add(KscVariableHandle var)
{
    if(var) {
        bool ok = vars.addLast(var);
    
        if(ok) num_vars++;

        return ok;
    } else {
        return false;
    }
}

//////////////////////////////////////////////////////////////////////

bool
KscPackage::add(KscPackageHandle pkg) 
{
#if 0
    // TODO:
    // avoid cycles
    //
    if(&pkg == this) {
        return false;
    }
#endif

    if(pkg) {
        bool ok = pkgs.addLast(pkg);
    
        if(ok) num_pkgs++;

        return ok;
    } else {
        return false;
    }
}
    
//////////////////////////////////////////////////////////////////////

bool
KscPackage::remove(KscVariableHandle var) 
{
    if(var) {
        bool ok = vars.remove(var);
    
        if(ok) num_vars--;

        return ok;
    } else {
        return false;
    }
}

//////////////////////////////////////////////////////////////////////

bool
KscPackage::remove(KscPackageHandle pkg)
{
    if(pkg) {
        bool ok = pkgs.remove(pkg);

        if(ok) num_pkgs--;

        return ok;
    } else {
        return false;
    }
}

//////////////////////////////////////////////////////////////////////

size_t 
KscPackage::sizeVariables(bool deep) const
{
    if(deep) {
        size_t count = 0;
        // iterate over subpackages
        //
        PltListIterator<KscPackageHandle> it(pkgs);
        while(it) {
            count += (*it)->sizeVariables(true);
            ++it;
        }
        return count + num_vars;
    }

    return num_vars;
}

//////////////////////////////////////////////////////////////////////

PltIterator<KscVariableHandle> *
KscPackage::newVariableIterator(bool deep) const
{
    if(deep) {
        return new DeepIterator(*this);
    } 
    else {
        return new PltListIterator<KscVariableHandle>(vars);
    }
}

//////////////////////////////////////////////////////////////////////

PltIterator<KscPackageHandle> *
KscPackage::newSubpackageIterator() const 
{
    return new PltListIterator<KscPackageHandle>(pkgs);
}

//////////////////////////////////////////////////////////////////////

bool
KscPackage::getUpdate() 
{
    DeepIterator var_it(*this);
    KscSorter sorter(var_it);

    if(!sorter.isValid()) {
        // failed to sort variables by servers and av-types
        //
        _result = KS_ERR_GENERIC;
        return false;
    }

    PltIterator<KscBucketHandle> *pit =
        sorter.newBucketIterator();

    if(!pit) {
        _result = KS_ERR_GENERIC;
        return false;
    }

    bool ok = true;
    _result = KS_ERR_OK;

    // iterate over buckets and do the jobs
    //
    while(*pit) {
        KscBucketHandle curr_bucket = **pit;
        ok &= getSimpleUpdate(curr_bucket);
        ++(*pit);
    }

    delete pit;

    return ok;    
}

//////////////////////////////////////////////////////////////////////

bool
KscPackage::getSimpleUpdate(KscBucketHandle bucket)
{
    // create data structures for transfer
    //
    KsGetVarResult result;
    KsGetVarParams params(bucket->size());

    PltArray<KscVariableHandle> sorted_vars = 
        bucket->getSortedVars();

    if(params.identifiers.size() != bucket->size()
       || sorted_vars.size() != bucket->size()) 
    {
        // failed to allocate memory
        //
        _result = KS_ERR_GENERIC;
        return false;
    }

    // copy data to params
    //
    if(!fillGetVarParams(sorted_vars, params.identifiers)) {
        _result = KS_ERR_GENERIC;
        return false;
    }

    // request service
    //
    bool ok = bucket->getServer()->getVar(bucket->getAvModule(),
                                          params,
                                          result);

    if(!ok) {
        _result = KS_ERR_NETWORK_ERROR;
        return false;
    }

    if(result.result != KS_ERR_OK) {
        _result = result.result;
        return false;
    }

    // copy results 
    //
    return copyGetVarResults(sorted_vars, result.items);
}   

//////////////////////////////////////////////////////////////////////

bool
KscPackage::setUpdate(bool force)
{
    DeepIterator var_it(*this);
    KscSorter sorter(var_it, !force);

    if(!sorter.isValid()) {
        PLT_DMSG("Failed to sort variables" << endl);
        _result = KS_ERR_GENERIC;
        return false;
    }

    PltIterator<KscBucketHandle> *pit =
        sorter.newBucketIterator();

    if(!pit) {
        PLT_DMSG("Failed to create iterator over av/server-types" << endl);
        _result = KS_ERR_GENERIC;
        return false;
    }

    bool ok = true;
    _result = KS_ERR_OK;

    while(*pit) {
        KscBucketHandle curr_bucket = **pit;
        ok &= setSimpleUpdate(curr_bucket);
        ++*pit;
    }

    delete pit;

    return ok;
}

//////////////////////////////////////////////////////////////////////

bool
KscPackage::setSimpleUpdate(KscBucketHandle bucket)
{
    KsSetVarParams params(bucket->size());
    KsSetVarResult result(bucket->size());

    PltArray<KscVariableHandle> sorted_vars = 
        bucket->getSortedVars();

    if(params.items.size() != bucket->size() || 
       result.results.size() != bucket->size() ||
       sorted_vars.size() != bucket->size()) 
    {
        // failed to allocate space
        //
        _result = KS_ERR_GENERIC;
        return false;
    }

    // copy data to params
    //
    if( !fillSetVarParams(sorted_vars, params.items) ) {
        _result = KS_ERR_GENERIC;
        return false;
    }

    // request service
    //
    bool ok = bucket->getServer()->setVar(
        bucket->getAvModule(),
        params,
        result);

    if(!ok) {
        _result = KS_ERR_NETWORK_ERROR;
        return false;
    }
    if(result.result != KS_ERR_OK) {
        _result = result.result;
        return false;
    }

    return copySetVarResults(sorted_vars, result.results);
}

//////////////////////////////////////////////////////////////////////
// class KscPackage::DeepIterator
//////////////////////////////////////////////////////////////////////

KscPackage::DeepIterator::DeepIterator(const KscPackage &itOver)
: pkg(itOver),
  rek_it(0),
  vars_it(itOver.vars),
  pkgs_it(itOver.pkgs)
{}

//////////////////////////////////////////////////////////////////////

KscPackage::DeepIterator::~DeepIterator()
{
    if(rek_it) {
        delete rek_it;
    }
}

//////////////////////////////////////////////////////////////////////

KscPackage::DeepIterator::operator bool () const
{
    if(vars_it) {
        return true;
    }

    if(pkgs_it) {
        return true;
    }

    if(rek_it && *rek_it) {
        return true;
    }

    return false;
}

//////////////////////////////////////////////////////////////////////

const KscVariableHandle &
KscPackage::DeepIterator::operator * () const
{
    PLT_PRECONDITION(*this);

    // check for elements in this package
    //
    if(vars_it) {
        return *vars_it;
    }

    // check for elements in the subpackages
    //
    if(rek_it && *rek_it) {
        return **rek_it;
    }

    // this is an error condition
    //
    PLT_ASSERT(0);
    // not reachede
    static KscVariableHandle dummy;
    return dummy; 
}
    
//////////////////////////////////////////////////////////////////////
    
const KscVariableHandle *
KscPackage::DeepIterator::operator -> () const
{
    PLT_PRECONDITION(*this);

    // check for elements in this package
    //
    if(vars_it) {
        return &(*vars_it);
    }

    // check for elements in the subpackages
    //
    if(rek_it && *rek_it) {
        return &(**rek_it);
    }

    // this is an error condition
    //
    PLT_ASSERT(0);
    return 0; // not reached
}

//////////////////////////////////////////////////////////////////////

KscPackage_DeepIterator_THISTYPE &
KscPackage::DeepIterator::operator ++()
{
    PLT_PRECONDITION(*this);
    // check for variables in this package left
    //
    if(vars_it) {
        ++vars_it;
        // if we have more variables
        // there is no need to check the packages
        //
        if(vars_it) {
            return *this;
        }
    }

    // check for variables in the current subpackage
    //
    if(rek_it && *rek_it) {
        ++(*rek_it);
        // if this iterator holds more variables
        // we are finished
        if( *rek_it ) {
            return *this;
        }
    }

    // check wether there are more subpackages left
    //
    while(pkgs_it) {
        if(rek_it) {
            delete rek_it;
        }
        rek_it = new DeepIterator(**pkgs_it);
        PLT_ASSERT(rek_it);
        ++pkgs_it;
        if(*rek_it) {
            return *this;
        }
    }

    // we ran out of variables
    //
    return *this;
}

//////////////////////////////////////////////////////////////////////

void
KscPackage::DeepIterator::operator ++ (int)
{
  PLT_PRECONDITION(*this);
    // check for variables in this package left
    //
    if(vars_it) {
        ++vars_it;
        // if we have more variables
        // there is no need to check the packages
        //
        if(vars_it) {
            return;
        }
    }

    // check for variables in the current subpackage
    //
    if(rek_it && *rek_it) {
        ++(*rek_it);
        // if this iterator holds more variables
        // we are finished
        if( *rek_it ) {
            return;
        }
    }

    // check wether there are more subpackages left
    //
    while(pkgs_it) {
        if(rek_it) {
            delete rek_it;
        }
        rek_it = new DeepIterator(**pkgs_it);
        PLT_ASSERT(rek_it);
        ++pkgs_it;
        if(*rek_it) {
            return;
        }
    }
  
    // no more variables left
    //
    return;
}

//////////////////////////////////////////////////////////////////////

void
KscPackage::DeepIterator::toStart()
{
    if(rek_it) {
        delete rek_it;
        rek_it = 0;
    }
    pkgs_it.toStart();
    vars_it.toStart();
}


//////////////////////////////////////////////////////////////////////
// class KscExchangePackage
//////////////////////////////////////////////////////////////////////

bool 
KscExchangePackage::doExchange(bool force)
{
    // if none of the packages exists we consider
    // the operation to be succesfull
    if( !get_pkg && !set_pkg ) {
        _result = KS_ERR_OK;
        return true;
    }

    // if one of the package does exist we use
    // his functions
    if( get_pkg && !set_pkg ) {
        bool ok = get_pkg->getUpdate();
        _result = get_pkg->getLastResult();
        return ok;
    }

    if( !get_pkg && set_pkg ) {
        bool ok = set_pkg->setUpdate(force);
        _result = set_pkg->getLastResult();
        return ok;
    }

    // both packages exist, now sort variables
    //
    bool ok = false;
    PltIterator<KscVariableHandle> *get_it =
        get_pkg->newVariableIterator(true);
    PltIterator<KscVariableHandle> *set_it =
        set_pkg->newVariableIterator(true);

    _result = KS_ERR_GENERIC;

    if( get_it && set_it ) {
        KscSorter get_sorter(*get_it);
        KscSorter set_sorter(*set_it, !force);

        if(get_sorter.isValid() && set_sorter.isValid()) {
            ok = mergeSorters(get_sorter, set_sorter);
        }
    }

    if(get_it) delete get_it;
    if(set_it) delete set_it;

    return ok;
}

//////////////////////////////////////////////////////////////////////

bool
KscExchangePackage::mergeSorters(KscSorter &get_sorter,
                                 KscSorter &set_sorter)
{
    bool ok = true;
    _result = KS_ERR_OK;

    // first use all buckets contained in get_sorter and 
    // buckets out of set_sorter with similar AV/server type
    //
    while(!get_sorter.isEmpty()) {
        KscBucketHandle curr_get(get_sorter.removeFirst());
        KscBucketHandle curr_set(set_sorter.removeMatchingBucket(curr_get));
        ok &= doSimpleExchange(curr_get, curr_set);
    }

    // get_sorter is flushed, now handle the remaining
    // buckets in set_sorter
    KscBucketHandle dummy_get;
    while(!set_sorter.isEmpty()) {
        KscBucketHandle curr_set(set_sorter.removeFirst());
        ok &= doSimpleExchange(dummy_get, curr_set);
    }

    return ok;
}

//////////////////////////////////////////////////////////////////////

bool
KscExchangePackage::doSimpleExchange(
    KscBucketHandle get_bucket,
    KscBucketHandle set_bucket)
{
    _result = KS_ERR_GENERIC;

    if(!get_bucket && !set_bucket) return false;

    PltArray<KscVariableHandle> set_vars, get_vars;
    size_t get_size, set_size;
    KscServer *server = 0;
    const KscAvModule *avm = 0;

    if(get_bucket) {
        get_size = get_bucket->size();
        get_vars = get_bucket->getSortedVars();
        server = get_bucket->getServer();
        avm = get_bucket->getAvModule();
    } else {
        get_size = 0;
    }

    if(set_bucket) {
        set_size = set_bucket->size();
        set_vars = set_bucket->getSortedVars();
        server = set_bucket->getServer();
        avm = set_bucket->getAvModule();
    } else {
        set_size = 0;
    }
    
    KsExgDataParams params(set_size, get_size);
    KsExgDataResult res(set_size, get_size);

    if(params.set_vars.size() != set_size ||
       params.get_vars.size() != get_size ||
       res.results.size() != set_size ||
       res.items.size() != get_size ||
       get_vars.size() != get_size ||
       set_vars.size() != set_size )
    {
        return false;
    }

    // copy data to params
    //
    bool ok = fillGetVarParams(get_vars, params.get_vars) &&
        fillSetVarParams(set_vars, params.set_vars);

    if(!ok) return false;

    // request server for service
    //
    ok = server->exgData(avm, params, res);

#if PLT_DEBUG
    if(ok && res.result != KS_ERR_OK) {
        PLT_DMSG("error code : " << res.result << endl);
    }
#endif

    if(!ok) {
        _result = KS_ERR_NETWORK_ERROR;
        return false;
    }

    _result = res.result;
    if(res.result != KS_ERR_OK) return false;

    // copy results
    //
    ok = copyGetVarResults(get_vars, res.items) &&
        copySetVarResults(set_vars, res.results);

    return ok;
}
    
//////////////////////////////////////////////////////////////////////
// class _KscPackageBase
//////////////////////////////////////////////////////////////////////

bool 
_KscPackageBase::fillGetVarParams(const PltArray<KscVariableHandle> &sorted_vars,
                                  KsArray<KsString> &identifiers)
{
    return optimizePaths(sorted_vars, identifiers);
}

//////////////////////////////////////////////////////////////////////

bool 
_KscPackageBase::copyGetVarResults(
    const PltArray<KscVariableHandle> &sorted_vars,
    const KsArray<KsGetVarItemResult> &res)
{
    PLT_PRECONDITION(sorted_vars.size() == res.size());

    bool ok = true;
    size_t count = 0,
           to_copy = res.size();

    //
    // TODO : add more specific error codes
    //
    while(count < to_copy) {
        if(res[count].result == KS_ERR_OK) {
            KsVarCurrProps *cp = 
                PLT_DYNAMIC_PCAST(KsVarCurrProps,
                                  res[count].item.getPtr()); 
            if(cp) {
                // TODO :  
                ok &= sorted_vars[count]->setCurrProps(*cp);
                sorted_vars[count]->fDirty = false;
                sorted_vars[count]->last_result = KS_ERR_OK;
            } else {
                // type mismatch
                sorted_vars[count]->last_result = KS_ERR_TYPE_MISMATCH;
                ok = false;
            }
        } else {
            sorted_vars[count]->last_result = res[count].result;
            ok = false;
        }
        ++count;
    }

    return ok;
}

//////////////////////////////////////////////////////////////////////

#if 0

// unoptimized version
//
bool 
_KscPackageBase::fillSetVarParams(
    const PltArray<KscVariableHandle> &sorted_vars,
    KsArray<KsSetVarItem> &items)
{
    PLT_PRECONDITION(sorted_vars.size() == items.size());

    size_t to_copy = sorted_vars.size();
 
    for(size_t count = 0; count < to_copy; count++) { 
        items[count].path_and_name = 
            sorted_vars[count]->getPathAndName();
        items[count].curr_props =
            sorted_vars[count]->getCurrPropsHandle();
    }

    return true;
}

#endif

#if 1

// optimized version
//
bool 
_KscPackageBase::fillSetVarParams(
    const PltArray<KscVariableHandle> &sorted_vars,
    KsArray<KsSetVarItem> &items)
{
    PLT_PRECONDITION(sorted_vars.size() == items.size());

    size_t to_copy = sorted_vars.size();
    items[0].path_and_name = sorted_vars[0]->getPathAndName();
    items[0].curr_props = sorted_vars[0]->getCurrPropsHandle();

    PLT_DMSG("Optimizing paths :" << endl);
    PLT_DMSG("Input field :" << endl);
#if PLT_DEBUG
    for(size_t i = 0; i < sorted_vars.size(); i++) {
        cerr << (sorted_vars[i])->getPathAndName() << endl;
    }
#endif
    PLT_DMSG("absolute\t\trelative" << endl);
    PLT_DMSG(items[0].path_and_name << endl); 

    for(size_t count = 1; count < to_copy; count++) { 
        items[count].path_and_name = 
            sorted_vars[count]->getPathAndName().relTo(
                sorted_vars[count-1]->getPathAndName());
        items[count].curr_props =
            sorted_vars[count]->getCurrPropsHandle();
        PLT_DMSG(sorted_vars[count]->getPathAndName() << "\t" \
                 << items[count].path_and_name << endl);
                 
    }

    return true;
}

#endif

//////////////////////////////////////////////////////////////////////

bool 
_KscPackageBase::copySetVarResults(const PltArray<KscVariableHandle> &sorted_vars,
                                   const KsArray<KsResult> &res)
{
    PLT_PRECONDITION(sorted_vars.size() == res.size());

    bool ok = true;
    size_t size = sorted_vars.size();

    for(size_t count = 0; count < size; count++) {
        sorted_vars[count]->last_result = res[count].result;
        if(res[count].result == KS_ERR_OK) {
            sorted_vars[count]->fDirty = false;
        } else {
            ok = false;
        }
    }

    return ok;
}

//////////////////////////////////////////////////////////////////////

#if 0

// unoptimized version
//
bool 
_KscPackageBase::optimizePaths(
    const PltArray<KscVariableHandle> &sorted_vars,
    KsArray<KsString> &paths) 
{
    // TODO: implement use of relative paths
    // currently we just copy the paths
    //
    PLT_PRECONDITION(sorted_vars.size() == paths.size());

    size_t to_copy = sorted_vars.size();

    for(size_t count = 0; count < to_copy; count++) {
        paths[count] = sorted_vars[count]->getName();
     }

    return true;
}

#endif

//////////////////////////////////////////////////////////////////////

// optimized version
//
bool 
_KscPackageBase::optimizePaths(
    const PltArray<KscVariableHandle> &sorted_vars,
    KsArray<KsString> &paths) 
{
    PLT_PRECONDITION(sorted_vars.size() == paths.size());

    size_t to_copy = sorted_vars.size();
    paths[0] = sorted_vars[0]->getPathAndName();

    PLT_DMSG("Optimizing paths :" << endl);
    PLT_DMSG("absolute\t\trelative" << endl);
    PLT_DMSG(paths[0] << endl);

    for(size_t count = 1; count < to_copy; count++) {
        paths[count] = 
            sorted_vars[count]->getPathAndName().relTo(
                sorted_vars[count-1]->getPathAndName());
        PLT_DMSG(sorted_vars[count]->getPathAndName() << "\t" << paths[count] << endl);
     }

    return true;
}















