/* -*-plt-c++-*- */

#ifndef KSC_SORTER_INCLUDED 
#define KSC_SORTER_INCLUDED

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
#include <plt/hashtable.h>

#include "ks/commobject.h"
#include "ks/avmodule.h"
#include "ks/client.h"


//////////////////////////////////////////////////////////////////////
// This is a helper class which groups together KscVariable objects
// belonging to the same server/av-module pair.
//  
// Intented for internal use only.
//
class KscSorterBucket
{
public:
    KscSorterBucket(const KscAvModule *,
                    KscServer *);
//    KscSorterBucket(const KscSorterBucket &);
//    KscSorterBucket &operator = (const KscSorterBucket &);

    bool add(KscVariable *);
    size_t size() const;
    PltIterator<KscVariable *> *newVarIterator() const;

    KscServer *getServer() const;
    const KscAvModule *getAvModule() const;
    KscNegotiator *getNegotiator() const;
 
private:
    PltList<KscVariable *> var_lst;
    const KscAvModule *av_module;
    KscServer *server;
    size_t var_count;
};

//////////////////////////////////////////////////////////////////////
// This is a helper class for KscPackage/KscExchangePackage which
// sorts KscVariable objects by their servers and av-types.
//
// Intented for internal use only.
//
class KscSorter 
{
public:
    KscSorter(PltIterator<KscVariable> &var_it, bool dirty_only = false);
    ~KscSorter();

    bool isValid() const;
    PltIterator<KscSorterBucket *> *newBucketIterator() const;

private:
    class Key
    {
    public:
        Key(const KscAvModule *,
            KscServer *);
        
        const KscAvModule *getAvModule() const 
            { return av_module; }
        KscServer *getServer() const 
            { return server; }
        
        unsigned long hash() const;
        friend bool operator == (const Key &, const Key &);
        
    private:
        const KscAvModule *av_module;
        KscServer *server;
    };

    class ValueIterator
    : public PltIterator<KscSorterBucket *>
    {
    public:
        typedef KscSorterBucket *T;

        ValueIterator(const PltHashTable<Key, T> &);

        operator const void * () const;
        const T * operator -> () const;
        ValueIterator & operator ++ ();
        void toStart();
    private:
        PltHashIterator<Key,T> it;
    };      

    friend class ValueIterator;

    bool sortVars(PltIterator<KscVariable> &);

    bool valid;
    bool fDirtyOnly;
    PltHashTable<Key,KscSorterBucket *> table;
};


//////////////////////////////////////////////////////////////////////
// Inline Implementation
//////////////////////////////////////////////////////////////////////

inline
bool
operator == (const KscSorter::Key &k1, const KscSorter::Key &k2)
{
    return k1.server == k2.server
        && k1.av_module == k2.av_module;
}

//////////////////////////////////////////////////////////////////////

inline
unsigned long
KscSorter::Key::hash() const 
{
    return (unsigned long)av_module +
        (unsigned long)server;
}

//////////////////////////////////////////////////////////////////////

inline
KscSorter::Key::Key(const KscAvModule *avm,
                    KscServer *svr)
: av_module(avm),
  server(svr)
{}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

inline
KscSorter::ValueIterator::ValueIterator(
    const PltHashTable<Key, KscSorterBucket *> &table)
: it(table)
{}

//////////////////////////////////////////////////////////////////////

inline
KscSorter::ValueIterator::operator const void * () const
{
    return it;
}

//////////////////////////////////////////////////////////////////////

inline
KscSorterBucket * const * 
KscSorter::ValueIterator::operator -> () const 
{
    return &(it->a_value);
}

//////////////////////////////////////////////////////////////////////

inline
KscSorter::ValueIterator & 
KscSorter::ValueIterator::operator ++ ()
{
    ++it;
    return *this;
}

//////////////////////////////////////////////////////////////////////

inline
void 
KscSorter::ValueIterator::toStart() 
{
    it.toStart();
}


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

inline
KscSorterBucket::KscSorterBucket(const KscAvModule *avm,
                                 KscServer *svr)
: av_module(avm),
  server(svr),
  var_count(0)
{}

//////////////////////////////////////////////////////////////////////

inline
KscServer *
KscSorterBucket::getServer() const
{
    return server;
}

//////////////////////////////////////////////////////////////////////

inline
const KscAvModule *
KscSorterBucket::getAvModule() const
{
    return av_module;
}

//////////////////////////////////////////////////////////////////////

inline
bool
KscSorterBucket::add(KscVariable *var)
{
    bool ok = var_lst.addLast(var);

    if(ok) {
        var_count++;
    }

    return ok;
} 

//////////////////////////////////////////////////////////////////////

inline
size_t
KscSorterBucket::size() const
{
    PLT_ASSERT(var_count == var_lst.size());
    return var_count;
}

//////////////////////////////////////////////////////////////////////

inline
PltIterator<KscVariable *> *
KscSorterBucket::newVarIterator() const
{
    return var_lst.newIterator();
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

inline
KscSorter::KscSorter(PltIterator<KscVariable> &var_it,
                     bool dirty_only)
: fDirtyOnly(dirty_only) 
{
    valid = sortVars(var_it);
}

//////////////////////////////////////////////////////////////////////

inline 
bool
KscSorter::isValid() const
{
    return valid;
}

//////////////////////////////////////////////////////////////////////

inline
PltIterator<KscSorterBucket *> *
KscSorter::newBucketIterator() const
{
    return new ValueIterator(table);
}

//////////////////////////////////////////////////////////////////////

#endif

//////////////////////////////////////////////////////////////////////
// EOF sorter.h
//////////////////////////////////////////////////////////////////////






