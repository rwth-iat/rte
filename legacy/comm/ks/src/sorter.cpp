/* -*-plt-c++-*- */

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

#include "ks/sorter.h"
#include "ks/package.h"

//////////////////////////////////////////////////////////////////////

bool
operator == (const KscSorter::Key &k1, const KscSorter::Key &k2)
{
    return k1.server == k2.server
        && k1.av_module == k2.av_module;
}

//////////////////////////////////////////////////////////////////////

unsigned long
KscSorter::Key::hash() const 
{
    return (unsigned long)av_module 
        + (unsigned long)server;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

bool
KscSorterBucket::add(KscBucketHandle hbucket)
{
    PLT_ASSERT(hbucket);

#if PLT_DEBUG
    size_t start_size = size() + hbucket->size();
#endif

    bool ok = true;

    while(ok && !(hbucket->isEmpty())) {
        ok = add(hbucket->removeFirst());
    }

    PLT_POSTCONDITION((start_size == size()) && (hbucket->size() == 0));

    return ok;
}

//////////////////////////////////////////////////////////////////////

KscSortVarPtr
KscSorterBucket::removeFirst()
{
    PLT_PRECONDITION(!var_lst.isEmpty());

    KscSortVarPtr hvar = var_lst.removeFirst();
    --var_count;
    return hvar;
}

//////////////////////////////////////////////////////////////////////


PltArray< KscSortVarPtr > 
KscSorterBucket::getSortedVars()
{
    PltArray< KscSortVarPtr > sv(size());

    if(sv.size() != size()) {
        // failed to allocate memory
        return PltArray< KscSortVarPtr >(0);
    }

    size_t count = 0;
    while(!var_lst.isEmpty()) {
        sv[count++] = var_lst.removeFirst();
    }

    var_count = 0;

    PltSort< KscSortVarPtr >::qsort(sv);

    return sv;
}       
    
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

KscSorter::KscSorter(const KscPackage &pkg,
                     bool dirty_only,
                     const KscAvModule *avm_default)
: fDirtyOnly(dirty_only)
{
    valid = sortVariables(pkg, avm_default);
}

//////////////////////////////////////////////////////////////////////

KscSorter::~KscSorter() 
{
}

//////////////////////////////////////////////////////////////////////

const KscAvModule *
KscSorter::findAvModule(const KscVariable *var,
                        const KscAvModule *avm_package)
{
    const KscAvModule *temp = var->getAvModule();

    if( temp ) {
        return temp;
    } else if( avm_package ) {
        return avm_package;
    } else if( (temp = var->getServer()->getAvModule()) ) {
        return temp;
    } else {
        return KscClient::getClient()->getAvModule();
    }
}

//////////////////////////////////////////////////////////////////////

KscBucketHandle 
KscSorter::removeFirst()
{
    PltHashIterator<Key, KscBucketHandle> it(table);
    KscBucketHandle bucket;

    if(it) {
        Key key = it->a_key;
        bucket = it->a_value;

        table.remove(key, bucket);
    } else {
        PLT_DMSG("KscSorter::removeFirst() called for empty object" << endl;);
    }

    return bucket;
}

//////////////////////////////////////////////////////////////////////

KscBucketHandle 
KscSorter::removeMatchingBucket(KscBucketHandle bucket)
{
    KscBucketHandle res;
    Key key(bucket->getAvModule(), bucket->getServer());

    table.remove(key, res);

    return res;
}

//////////////////////////////////////////////////////////////////////

bool
KscSorter::sortVariables(const KscPackage &pkg,
                         const KscAvModule *avm_default)
{
    // use AV-module of pkg if set
    //
    if(pkg.getAvModule()) {
        avm_default = pkg.getAvModule();
    }

    // iterate over variables and
    // (1) add a variable to the approbiate bucket if it exists
    // (2) create a new bucket if neccessary and add variable
    //
    PltIterator<KscVariableHandle> *var_it = 
        pkg.newVariableIterator(false);
    if(!var_it) return false;

    bool ok = true;
    (*var_it).toStart();

    while(ok && *var_it) {
        KscVariableHandle current = **var_it;
        if( !fDirtyOnly || current->isDirty() ) { 
            Key key(findAvModule(current.getPtr(), avm_default),
                    current->getServer());
            KscBucketHandle bucket;
            
            if(table.query(key, bucket)) {
                // bucket found
                //
                ok = bucket->add(KscSortVarPtr(current.getPtr()));
            } else {
                // create bucket
                //
                bucket.bindTo(new KscSorterBucket(key.getAvModule(),
                                                  key.getServer()),
                              PltOsNew);
                if(bucket) {
                    ok = table.add(key, bucket)
                        && bucket->add(KscSortVarPtr(current.getPtr()));
                } else {
                    ok = false;
                }
            }
        }

        ++(*var_it);
    }

    delete var_it;

    if(ok) {
        return sortSubpackages(pkg, avm_default);
    } else {
        return false;
    }
}

//////////////////////////////////////////////////////////////////////

bool
KscSorter::sortSubpackages(const KscPackage &pkg,
                           const KscAvModule *avm_default)
{
    bool ok = true;

    // iterate over the subpackages and sort their variables too
    //
    KscSubpackageIterator *pkg_it =
        pkg.newSubpackageIterator();
    if(!pkg_it) return false;

    while(ok && *pkg_it) {
        ok = sortVariables(***pkg_it, avm_default);
        ++(*pkg_it);
    }

    delete pkg_it;
        
    return ok;
}
                
//////////////////////////////////////////////////////////////////////
// EOF sorter.cpp
//////////////////////////////////////////////////////////////////////












