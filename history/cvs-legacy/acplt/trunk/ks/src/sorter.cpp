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

#include "ks/sorter.h"

//////////////////////////////////////////////////////////////////////

KscNegotiator *
KscSorterBucket::getNegotiator() const
{
    if(av_module) {
        KscNegotiator *neg = 
            av_module->getNegotiator(server);
        if(neg) return neg;
    }
    
    return server->getNegotiator();
}

//////////////////////////////////////////////////////////////////////

KscSorter::~KscSorter() 
{
    // clean up hash table
    //
    PltHashIterator<Key,KscSorterBucket*> it(table);
    while(it) {
        delete it->a_value;
        ++it;
    }
}

//////////////////////////////////////////////////////////////////////

bool
KscSorter::sortVars(PltIterator<KscVariable> &var_it)
{
    // iterate over variables and
    // (1) add a variable to the approbiate bucket if it exists
    // (2) create a new bucket if neccessary and add variable
    //
    bool ok = true;
    var_it.toStart();

    while(ok && var_it) {
        KscVariable *current = &(*var_it);
        if( !fDirtyOnly || current->isDirty() ) { 
            Key key(current->getAvModule(),
                    current->getServer());
            KscSorterBucket *bucket;
            
            if(table.query(key, bucket)) {
                // bucket found
                //
                ok = bucket->add(current);
            } else {
                // create bucket
                //
                bucket = new KscSorterBucket(key.getAvModule(),
                                             key.getServer());
                if(bucket) {
                    ok = table.add(key, bucket)
                        && bucket->add(current);
                } else {
                    ok = false;
                }
            }
        }

        ++var_it;
    }

    return ok;
}
                
//////////////////////////////////////////////////////////////////////
// EOF sorter.cpp
//////////////////////////////////////////////////////////////////////











