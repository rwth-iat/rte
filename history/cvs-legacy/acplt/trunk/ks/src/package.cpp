/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/src/package.cpp,v 1.29 2007-04-25 12:57:21 martin Exp $ */
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
/* Improved error code propagation and QC by:
           Harald Albrecht <harald@plt.rwth-aachen.de> */


#include "ks/package.h"
#include "ks/client.h"


// ----------------------------------------------------------------------------
// printing functions for debugging
//
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


// ----------------------------------------------------------------------------
// class KscPackage: this class acts as a container for a couple of variables
// and other (sub-) packages to read/write in one request. Well -- in case
// variables from several servers are in a package, the request is broken
// down into individual requests, one for each server referenced. In addition,
// A/V modules are handled properly too.
//
KscPackage::KscPackage()
: num_vars(0),
  num_pkgs(0),
  av_module(0),
  _is_dirty(false),
  _last_result(-1)
{} // KscPackage::KscPackage


KscPackage::~KscPackage()
{} // KscPackage::~KscPackage


// ----------------------------------------------------------------------------
// Pack another variable object into this bag. To be more precise, we only
// pack a handle into our bag, but this ensures that the variable object will
// live as long as we use it (except when the programmer did bull sh*t and
// destroyed the variable object itself after he put it under the control of
// a handle...).
//
bool
KscPackage::add(KscVariableHandle var)
{
    //
    // Make sure the handle is bound (it has a variable object under
    // control) and that it isn't invalid. To prevent further problems
    // we don't accept invalid variable objects here.
    //
    if ( var && var->hasValidPath() ) {
        bool ok = vars.addLast(var);
        
        if ( ok ) {
            //
            // Yet another sucker to take care of in the future...
            //
            _is_dirty = true;
            num_vars++;
        }
        
        return ok;
    }
    return false;
} // KscPackage::add


// ----------------------------------------------------------------------------
// Pack another package into this bag. To be more precise, we only pack a
// handle into our pacakge, but this ensures that the subpackage object will
// live as long as we use it (exceptions as explained above may apply).
//
bool
KscPackage::add(KscPackageHandle pkg) 
{
    if ( pkg ) {
        bool ok = pkgs.addLast(pkg);
    
        if ( ok ) {
            num_pkgs++;
            _is_dirty = true;
        }

        return ok;
    }
    return false;
} // KscPackage::add


// ----------------------------------------------------------------------------
// Remove a variable object from our bag. As before, we first make sure that
// the handle is bound, so we have a variable to remove.
//
bool
KscPackage::remove(KscVariableHandle var) 
{
    if ( var ) {
        bool ok = vars.remove(var);
    
        if ( ok ) {
            num_vars--;
            _is_dirty = true;
        }
        
        return ok;
    }
    return false;
} // KscPackage::remove


// ----------------------------------------------------------------------------
// Remove a subbag from our bag.
//
bool
KscPackage::remove(KscPackageHandle pkg)
{
    if ( pkg ) {
        bool ok = pkgs.remove(pkg);

        if ( ok ) {
            num_pkgs--;
            _is_dirty = true;
        }
        
        return ok;
    } else {
        return false;
    }
} // KscPackage::remove


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

KscPkgVariableIterator *
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

KscSubpackageIterator *
KscPackage::newSubpackageIterator() const 
{
    return new PltListIterator<KscPackageHandle>(pkgs);
}


//////////////////////////////////////////////////////////////////////
// Retrieve the values (current properties) of the variables con-
// tained in this package and sub-packages from one or more ACPLT/KS
// servers.
//
bool
KscPackage::getUpdate() 
{
    KscSorter sorter(*this);

    if ( !sorter.isValid() ) {
        //
        // The sorter object failed to sort the variables by ACPLT/KS
        // servers and A/V modules. So fail with the generic error.
        //
        PLT_DMSG("KscPackage::getUpdate(): failed to sort variables" << endl);
        _last_result = KS_ERR_GENERIC;
        return false;
    }
    
    //
    // Otherwise get an iterator from the sorter which iterates over
    // so-called "buckets". Each bucket contains variables which are all
    // located in the same ACPLT/KS server and need the same A/V module
    // for accessing them. If we can't get the iterator, fail with the
    // usual generic error.
    //
    PltIterator<KscBucketHandle> *pit =
        sorter.newBucketIterator();

    if ( !pit ) {
        PLT_DMSG("KscPackage::getUpdate(): failed to create iterator over av/server-types" << endl);
        _last_result = KS_ERR_GENERIC;
        return false;
    }

    //
    // Iterate over buckets and delegate the work, that is, the
    // requests for the variable's current properties. We first assume
    // success. In every case we'll try to issue the ACPLT/KS GetVar
    // request on every bucket, so if one or more fails, we will
    // nevertheless continue until all buckets have been visited. If
    // there was a failure, then getUpdate() will return false, although
    // getLastResult() might reveal KS_ERR_OK. In every case, getLast-
    // Result() will return the last error encountered during the
    // getUpdate() operation. If this is KS_ERR_OK (no error), but the
    // ok flag is false, then reading one or more variables failed on
    // a per-variable basis and not on the request-level itself.
    //
    bool ok = true;
    _last_result = KS_ERR_OK;
    _is_dirty = false;

    while ( *pit ) {
        KscBucketHandle curr_bucket = **pit;
        ok &= getSimpleUpdate(curr_bucket);
        ++(*pit);
    }

    delete pit; // Throw the iterator away...

    return ok;    
} // KscPackage::getUpdate


//////////////////////////////////////////////////////////////////////
// Handle the GetVar request for a single ACPLT/KS server and with
// a single A/V module. This helping method is called from getUpdate()
// for every single ACPLT/KS server we have to deal with in a package.
// If we get into trouble, we'll set the last result accordingly and
// return a "false".
//
bool
KscPackage::getSimpleUpdate(KscBucketHandle bucket)
{
    // 
    // Create the data structures for the service request.
    //
    size_t num_vars = bucket->size();
    KsGetVarParams params(num_vars);
    KsGetVarResult result;

    PltArray< KscSortVarPtr > sorted_vars = 
        bucket->getSortedVars();

    if ( (params.identifiers.size() != num_vars)
         || (sorted_vars.size() != num_vars) )
        {
            //
            // We failed to allocate memory, so we fail with the well known
            // "generic" error.
            //
            _last_result = KS_ERR_GENERIC;
            return false;
    }

    //
    // Copy data into the service parameters (that is, the names of
    // the variables to query). Once again, bail out if we fail here.
    //
    if ( !fillGetVarParams(sorted_vars, params.identifiers) ) {
        _last_result = KS_ERR_GENERIC;
        return false;
    }

    //
    // Now query the ACPLT/KS server with the help of the server object. The
    // server object will handle the request accordingly to whatever kind
    // of ACPLT/KS server it is connected with. The information about which
    // server object to use as well as which A/V negotiator used for the
    // communication is contained in the bucket objects.
    //
    bool ok = bucket->getServer()->getVar(bucket->getAvModule(),
                                          params,
                                          result);

    if ( !ok ) {
	//
        // The exgData service request failed on the communication level.
	// In this case, the server object supplies more precise information
	// about the cause of the failure. At the level of this package
	// object we just indicate a communication level failure but
	// distribute the real error reason from the server object to the
	// variable objects.
	//
        _last_result = KS_ERR_NETWORKERROR;
	distributeErrorResult(bucket->getServer()->getLastResult(),
			      sorted_vars);
        return false;
    }

    //
    // Only set the last result, if the ACPLT/KS server reported an error
    // on the request level. Otherwise don't touch here the last result,
    // as other functions will set it when an error occours. In every case
    // we propagate the error code down to the individual variable objects.
    //
    if ( result.result != KS_ERR_OK ) {
        _last_result = KS_ERR_NETWORKERROR;
	distributeErrorResult(result.result, sorted_vars);
        return false;
    }

    //
    // Copy back the service results...
    //
    return copyGetVarResults(sorted_vars, result.items);
} // KscPackage::getSimpleUpdate


//////////////////////////////////////////////////////////////////////
// Write the values (current properties) of the variables contained
// in this package and sub-packages from one or more ACPLT/KS servers.
//
bool
KscPackage::setUpdate(bool force)
{
    KscSorter sorter(*this, !force);

    if ( !sorter.isValid() ) {
	//
        // The sorter object failed to sort the variables by ACPLT/KS
	// servers and A/V modules. So fail with the generic error.
        //
        PLT_DMSG("KscPackage::setUpdate(): failed to sort variables" << endl);
        _last_result = KS_ERR_GENERIC;
        return false;
    }

    //
    // Otherwise get an iterator from the sorter which iterates over
    // so-called "buckets". Each bucket contains variables which are all
    // located in the same ACPLT/KS server and need the same A/V module
    // for accessing them. If we can't get the iterator, fail with the
    // usual generic error.
    // This particular sorter has a special feature: if asked, it can
    // include only such variables, which have been changed recently
    // (got new current properties).
    //
    PltIterator<KscBucketHandle> *pit =
        sorter.newBucketIterator();

    if ( !pit ) {
        PLT_DMSG("KscPackage::setUpdate(): failed to create iterator over av/server-types" << endl);
        _last_result = KS_ERR_GENERIC;
        return false;
    }

    //
    // Iterate over buckets and delegate the work, that is, the
    // requests for the variable's current properties. We first assume
    // success. In every case we'll try to issue the ACPLT/KS SetVar
    // request on every bucket, so if one or more fails, we will
    // nevertheless continue until all buckets have been visited. If
    // there was a failure, then setUpdate() will return false, although
    // getLastResult() might reveal KS_ERR_OK. In every case, getLast-
    // Result() will return the last error encountered during the
    // getUpdate() operation. If this is KS_ERR_OK (no error), but the
    // ok flag is false, then writing one or more variables failed on
    // a per-variable basis and not on the request-level itself.
    //
    bool ok = true;
    _last_result = KS_ERR_OK;
    _is_dirty = false;

    while ( *pit ) {
        KscBucketHandle curr_bucket = **pit;
        ok &= setSimpleUpdate(curr_bucket);
        ++(*pit);
    }

    delete pit; // Throw the iterator away...

    return ok;
} // KscPackage::setUpdate


//////////////////////////////////////////////////////////////////////
// Handle the SetVar request for a single ACPLT/KS server and with
// a single A/V module.
//
bool
KscPackage::setSimpleUpdate(KscBucketHandle bucket)
{
    // 
    // Create the data structures for the service request.
    //
    size_t num_vars = bucket->size();
    KsSetVarParams params(num_vars);
    KsSetVarResult result(num_vars);

    PltArray< KscSortVarPtr > sorted_vars = 
        bucket->getSortedVars();

    if ( (params.items.size() != num_vars) ||
	 (result.results.size() != num_vars) ||
	 (sorted_vars.size() != num_vars) ) {
        //
        // We failed to allocate memory, so we fail with the well known
        // "generic" error.
        //
        _last_result = KS_ERR_GENERIC;
        return false;
    }

    //
    // Copy data into the service parameters (that is, the names and
    // values of the variables to query). Once again, bail out if we
    // fail here.
    //
    if ( !fillSetVarParams(sorted_vars, params.items) ) {
        _last_result = KS_ERR_GENERIC;
        return false;
    }

    //
    // Now query the ACPLT/KS server with the help of the server object. The
    // server object will handle the request accordingly to whatever kind
    // of ACPLT/KS server it is connected with. The information about which
    // server object to use as well as which A/V negotiator used for the
    // communication is contained in the bucket objects.
    //
    bool ok = bucket->getServer()->setVar(bucket->getAvModule(),
					  params,
					  result);

    if ( !ok ) {
	//
        // The exgData service request failed on the communication level.
	// In this case, the server object supplies more precise information
	// about the cause of the failure and we distribute the error
	// information down to the individual variable objects once again.
	//
        _last_result = KS_ERR_NETWORKERROR;
	distributeErrorResult(bucket->getServer()->getLastResult(),
			      sorted_vars);
        return false;
    }

    //
    // Only set the last result, if the ACPLT/KS server reported an error
    // on the request level. Otherwise don't touch here the last result,
    // as other functions will set it when an error occours. In every case
    // we propagate the error code down to the individual variable objects.
    //
    if ( result.result != KS_ERR_OK ) {
        _last_result = KS_ERR_NETWORKERROR;
	distributeErrorResult(result.result, sorted_vars);
        return false;
    }

    //
    // Copy back the service results...
    //
    return copySetVarResults(sorted_vars, result.results);
} // KscPackage::setSimpleUpdate


//////////////////////////////////////////////////////////////////////
// class KscPackage::DeepIterator
//////////////////////////////////////////////////////////////////////

KscPackage::DeepIterator::DeepIterator(const KscPackage &itOver)
: pkg(itOver),
  rek_it(0),
  vars_it(itOver.vars),
  pkgs_it(itOver.pkgs)
{
    normalize();
}

//////////////////////////////////////////////////////////////////////

KscPackage::DeepIterator::~DeepIterator()
{
    if(rek_it) {
        delete rek_it;
    }
}

//////////////////////////////////////////////////////////////////////

void
KscPackage::DeepIterator::normalize()
{
    if(vars_it) {
        return;
    }

    if(rek_it && *rek_it) {
        return;
    }

    while(pkgs_it) {
        if(rek_it) {
            delete rek_it;
        }
        rek_it = new DeepIterator(**pkgs_it);
        ++pkgs_it;
        if(rek_it && *rek_it) {
            return;
        }
    }
}

//////////////////////////////////////////////////////////////////////

KscPackage::DeepIterator::operator bool () const
{
    if(vars_it) {
        return true;
    }

#if 0
    if(pkgs_it) {
        return true;
    }
#endif

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
    // not reached
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
    //
    // Are there any variables left in this package? If there are
    // some left, then we have (yet) no need to dive into the next
    // subpackage. BTW -- defensive programming: we first make sure
    // that we're not already at the end.
    //
    if ( vars_it ) {
        ++vars_it;
        if ( vars_it ) {
            return *this;
        }
    }

    //
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

    //
    // Now check whether there are more subpackages left over which
    // we can happily iterate...
    //
    while ( pkgs_it ) {
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

    //
    // We finally ran out of variables.
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
    normalize();
}


//////////////////////////////////////////////////////////////////////
// class KscExchangePackage
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Carry out a data exchange operation on this special exchange
// package, thus issuing an ACPLT/KS ExgData service request.
//
bool 
KscExchangePackage::doExchange(bool force)
{
    //
    // If none of the packages exists which we need for a true data
    // *exchange* we consider the operation to be successfull.
    //
    if ( !get_pkg && !set_pkg ) {
        _last_result = KS_ERR_OK;
        return true;
    }

    //
    // If only one of the packages exists, then we will handle
    // this as either an ordinary getUpdate() or setUpdate() on
    // the package that exists. If needed we set the AV-module temporary.
    //
    if ( get_pkg && !set_pkg ) {
        bool temp_set = (get_pkg->getAvModule() == 0) 
            && (av_module != 0);
        if( temp_set ) get_pkg->setAvModule(av_module);

        bool ok = get_pkg->getUpdate();
        _last_result = get_pkg->getLastResult();

        if( temp_set ) get_pkg->setAvModule(0);

        return ok;
    }

    if ( !get_pkg && set_pkg ) {
        bool temp_set = (set_pkg->getAvModule() == 0) 
            && (av_module != 0);
        if( temp_set ) set_pkg->setAvModule(av_module);

        bool ok = set_pkg->setUpdate(force);
        _last_result = set_pkg->getLastResult();

        if( temp_set ) set_pkg->setAvModule(0);

        return ok;
    }

    //
    // But now back to our real "destination": both packages exist,
    // now sort the variables.
    //
    const KscAvModule *avm_get_default = 
        get_pkg->getAvModule() ? get_pkg->getAvModule() : av_module;
    const KscAvModule *avm_set_default = 
        set_pkg->getAvModule() ? set_pkg->getAvModule() : av_module;

    KscSorter get_sorter(*get_pkg, false, avm_get_default);
    KscSorter set_sorter(*set_pkg, !force, avm_set_default);

    if ( get_sorter.isValid() && set_sorter.isValid() ) {
	//
	// mergeSorters() will set the last result in every case.
	//
        return mergeSorters(get_sorter, set_sorter);
    }

    _last_result = KS_ERR_GENERIC;
    return false;
} // KscExchangePackage::doExchange


//////////////////////////////////////////////////////////////////////
// Now "merge" all packages belonging to the same ACPLT/KS server and
// A/V module/negotiator into a single data exchange request.
//
bool
KscExchangePackage::mergeSorters(KscSorter &get_sorter,
                                 KscSorter &set_sorter)
{
    bool ok = true;
    _last_result = KS_ERR_OK;

    //
    // First use all buckets contained in get_sorter and buckets out
    // of set_sorter with same AV module and server object.
    //
    // --aldi: this powerful behaviour, when an exchange package
    // contains heterogeneous packages, was not intented first, but it
    // can be quite useful: just put packages for several servers into
    // the same exchange package it this will take care of breaking
    // the request down into several requests, one for each
    // server. Hey markusj, you did an excellent job!
    //
    while ( !get_sorter.isEmpty() ) {
        KscBucketHandle curr_get(get_sorter.removeFirst());
        KscBucketHandle curr_set(set_sorter.removeMatchingBucket(curr_get));
        ok &= doSimpleExchange(curr_get, curr_set);
    }

    //
    // The get_sorter is now flushed, so handle the remaining
    // (left-over) buckets in the set_sorter.
    //
    KscBucketHandle dummy_get;
    while ( !set_sorter.isEmpty() ) {
        KscBucketHandle curr_set(set_sorter.removeFirst());
        ok &= doSimpleExchange(dummy_get, curr_set);
    }

    return ok;
} // KscExchangePackage::mergeSorters


//////////////////////////////////////////////////////////////////////
// Carry out a data exchange operation for exactly one ACPLT/KS server
// using exactly one A/V negotiatior module.
//
// This function only touches the last result, if there was an error.
//
bool
KscExchangePackage::doSimpleExchange(
    KscBucketHandle get_bucket,
    KscBucketHandle set_bucket)
{
    //
    // There are no buckets left. Oops...
    //
    if ( !get_bucket && !set_bucket ) {
	_last_result = KS_ERR_GENERIC;
	return false;
    }

    PltArray< KscSortVarPtr > set_vars, get_vars;
    size_t get_size, set_size;
    KscServerBase *server = 0;
    const KscAvModule *avm = 0;

    //
    // If we have variables to retrieve during the data exchange, then
    // set up the parameters accordingly.
    //
    if ( get_bucket ) {
        get_size = get_bucket->size();
        get_vars = get_bucket->getSortedVars();
        server = get_bucket->getServer();
        avm = get_bucket->getAvModule();
    } else {
        get_size = 0;
    }

    //
    // Same for the variables to set.
    //
    if ( set_bucket ) {
        set_size = set_bucket->size();
        set_vars = set_bucket->getSortedVars();
        server = set_bucket->getServer();
        avm = set_bucket->getAvModule();
    } else {
        set_size = 0;
    }
    
    KsExgDataParams params(set_size, get_size);
    KsExgDataResult res(set_size, get_size);

    if ( (params.set_vars.size() != set_size) ||
	 (params.get_vars.size() != get_size) ||
	 (res.results.size() != set_size) ||
	 (res.items.size() != get_size) ||
	 (get_vars.size() != get_size) ||
	 (set_vars.size() != set_size) ) {
	_last_result = KS_ERR_GENERIC;
        return false;
    }

    //
    // Copy data into the service parameters. Note: here it is
    // perfectly okay to use "&&" when carrying out setting the
    // parameters. Both functions return false if they fail due
    // to memory constraints or the like, so a boolean shortcut
    // is fine. At other places this might not what we want, so
    // be careful!
    //
    bool ok = fillGetVarParams(get_vars, params.get_vars) &&
              fillSetVarParams(set_vars, params.set_vars);

    if ( !ok ) {
        _last_result = KS_ERR_GENERIC;
	return false;
    }

    //
    // Now exchange the date with the ACPLT/KS server with the help of
    // the server object. The server object will handle the request
    // accordingly to whatever kind of ACPLT/KS server it is connected
    // with.
    //
    ok = server->exgData(avm, params, res);

#if PLT_DEBUG
    if ( ok && res.result != KS_ERR_OK ) {
        PLT_DMSG("exchange data error code : " << res.result << endl);
    }
#endif

    if ( !ok ) {
	//
        // The exgData service request failed on the communication level.
	// In this case, the server object supplies more precise information
	// about the cause of the failure.
	//
        _last_result = KS_ERR_NETWORKERROR;
	distributeErrorResult(server->getLastResult(),
			      set_vars);
	distributeErrorResult(server->getLastResult(),
			      get_vars);
        return false;
    }

    //
    // Only set the last result, if the ACPLT/KS server reported an error
    // on the request level. Otherwise don't touch here the last result,
    // as other functions will set it when an error occours. Once again,
    // propagate the error down to the individual variable objects.
    //
    if ( res.result != KS_ERR_OK ) {
        _last_result = KS_ERR_NETWORKERROR;
	distributeErrorResult(res.result, set_vars);
	distributeErrorResult(res.result, get_vars);
        return false;
    }

    //
    // Copy back the results from the data exchange operation. This will
    // not touch the last result, but instead deliver only the flag
    // indicating success or failure for particular variables. Note that
    // we can't use here the shortcutted "&&" operator, as this would
    // prefent us from retrieving both the "getVars" and the "setVars" if
    // we could not retrieve the value for at least one of the "getVars"
    // variables -- and this would be the wrong behaviour!
    //
    ok = copyGetVarResults(get_vars, res.items);
    ok &= copySetVarResults(set_vars, res.results);
    return ok;
} // KscExchangePackage::doSimpleExchange

    
//////////////////////////////////////////////////////////////////////
// class _KscPackageBase
//////////////////////////////////////////////////////////////////////

bool 
_KscPackageBase::fillGetVarParams(const PltArray< KscSortVarPtr > &sorted_vars,
                                  KsArray<KsString> &identifiers)
{
    PLT_PRECONDITION( sorted_vars.size() == identifiers.size() );

    if( sorted_vars.size() ) {
        return optimizePaths(sorted_vars, identifiers);
    } else {
        return true;
    }
}


//////////////////////////////////////////////////////////////////////
// Distribute (or propagate) an error which occoured at the communi-
// cation level down to the individual variable objects.
//
void
_KscPackageBase::distributeErrorResult(
    KS_RESULT result,
    const PltArray< KscSortVarPtr > &sorted_vars)
{
    size_t count,
           to_copy = sorted_vars.size();

    for ( count = 0; count < to_copy; ++count ) {
	sorted_vars[count]->_last_result = result;
    }
} // _KscPackageBase::distributeErrorResult


//////////////////////////////////////////////////////////////////////
// Copy back the results of an ACPLT/KS GetVar request into these
// variable communication objects, which have been queried this time.
// If something went wrong, we will return false, otherwise true. We
// will not touch the last result of a (exchange) package here, but
// instead report an error through the corresponging variable object,
// because we already know at this point, that the whole ACPLT/KS
// request was granted.
//
bool 
_KscPackageBase::copyGetVarResults(
    const PltArray< KscSortVarPtr > &sorted_vars,
    const KsArray<KsGetVarItemResult> &res)
{
    PLT_PRECONDITION(sorted_vars.size() == res.size());

    //
    // Iterate over all results for the individual variables we've
    // got back from the ACPLT/KS server and fill the results back
    // into the appropriate variable objects.
    //
    bool ok = true; // be optimistic...
    size_t count = 0,
           to_copy = res.size();

    while ( count < to_copy ) {
        if ( res[count].result == KS_ERR_OK ) {
	    //
	    // Fine. We got back a value (projected property) from
	    // the ACPLT/KS server for this variable. Well -- at least
	    // we hope so, because the server didn't flag an error for
	    // this particular variable.
	    //
	    // TODO: markusj: do we need the dynamic cast here, or is
	    //                a static cast together with a xdrTypeCode()
	    //                suitable, because the GetVar service can not
	    //                return other current properties than the
	    //                ones for variables...?
	    //
            KsVarCurrProps *cp = 
                PLT_DYNAMIC_PCAST(KsVarCurrProps,
                                  res[count].item.getPtr()); 
            if ( cp ) {
		//
		// Fine. We've got back the value for this particular
		// variable. So set the current properties of the
		// associated variable object.
		//
                ok &= sorted_vars[count]->setCurrProps(*cp);
                sorted_vars[count]->fDirty = false;
                sorted_vars[count]->_last_result = KS_ERR_OK;
            } else {
		//
                // Ooops. The handle was unbound, so we didn't get back
		// the current properties. We assume this being a
		// type mismatch.
		//
                sorted_vars[count]->_last_result = KS_ERR_TYPEMISMATCH;
                ok = false;
            }
        } else {
	    //
	    // This variable could not be read, so flag this error and
	    // return the exact error reason through the corresponding
	    // variable object. This behaviour is the reason for
	    // getUpdate() signalling an error but getLastResult()
	    // returning KS_ERR_OK.
	    //
            sorted_vars[count]->_last_result = res[count].result;
            ok = false;
        }
        ++count;
    }

    return ok; // return the outcome of our little operation...
} // _KscPackageBase::copyGetVarResults


//////////////////////////////////////////////////////////////////////

bool 
_KscPackageBase::fillSetVarParams(
    const PltArray< KscSortVarPtr > &sorted_vars,
    KsArray<KsSetVarItem> &items)
{
    PLT_PRECONDITION(sorted_vars.size() == items.size());

    size_t to_copy = sorted_vars.size();
    
    if( !to_copy ) return true;

    items[0].path_and_name = sorted_vars[0]->getPathAndName();
    items[0].curr_props = sorted_vars[0]->getCurrPropsHandle();

    for(size_t count = 1; count < to_copy; count++) { 
        items[count].path_and_name = 
            sorted_vars[count]->getPathAndName().relTo(
                sorted_vars[count-1]->getPathAndName());
        items[count].curr_props =
            sorted_vars[count]->getCurrPropsHandle();
    }

    return true;
}


//////////////////////////////////////////////////////////////////////
// Copy back the results (error codes) of an ACPLT/KS SetVar request
// into the corresponding variable objects. If there was an error for
// at least one variable in this set, then the function will return
// false, otherwise true. This helps client programmers to speed up
// checking whether the complete setUpdate() operation succeeded.
//
bool 
_KscPackageBase::copySetVarResults(
    const PltArray< KscSortVarPtr > &sorted_vars,
    const KsArray<KsResult> &res)
{
    PLT_PRECONDITION(sorted_vars.size() == res.size());

    bool ok = true; // Be optimistic once again...
    size_t size = sorted_vars.size();

    for ( size_t count = 0; count < size; count++ ) {
        sorted_vars[count]->_last_result = res[count].result;
        if ( res[count].result == KS_ERR_OK ) {
	    //
	    // Because the variable could be written successfully to
	    // the ACPLT/KS server, we can now reset the dirty flag.
	    //
            sorted_vars[count]->fDirty = false;
        } else {
	    //
	    // Setting a new value for this particular variable failed,
	    // so notify the caller of at least one failure.
	    //
            ok = false;
        }
    }

    return ok;
} // _KscPackageBase::copySetVarResults


//////////////////////////////////////////////////////////////////////

//#include <iostream.h>
//#include <iomanip.h>
//#define PLT_DEBUG_VERBOSE 1

bool 
_KscPackageBase::optimizePaths(
    const PltArray< KscSortVarPtr > &sorted_vars,
    KsArray<KsString> &paths) 
{
    PLT_PRECONDITION(sorted_vars.size() == paths.size());

    size_t to_copy = sorted_vars.size();
    paths[0] = sorted_vars[0]->getPathAndName();

#if PLT_DEBUG_VERBOSE
    cout << "Optimizing paths:" << endl;
    cout << setw(35) << "absolute" << setw(35) << "relative" << endl;
    cout << setw(35) << paths[0] << endl;
#endif

    for(size_t count = 1; count < to_copy; count++) {
        paths[count] = 
            sorted_vars[count]->getPathAndName().relTo(
                sorted_vars[count-1]->getPathAndName());
#if PLT_DEBUG_VERBOSE
        cout << setw(35) << sorted_vars[count]->getPathAndName()
             << setw(35) << paths[count]
             << endl;
#endif
     }

    return true;
}

//////////////////////////////////////////////////////////////////////
// EOF package.cpp
//////////////////////////////////////////////////////////////////////
