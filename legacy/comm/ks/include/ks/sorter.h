/* -*-plt-c++-*- */

#ifndef KSC_SORTER_INCLUDED 
#define KSC_SORTER_INCLUDED

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
#include <plt/hashtable.h>
#include <plt/sort.h>

#include "ks/commobject.h"
#include "ks/avmodule.h"
#include "ks/client.h"

//////////////////////////////////////////////////////////////////////
// forward declaration
//
class KscPackage;

//////////////////////////////////////////////////////////////////////
// Helper class which defines a special ordering 
// for KscVariable-objects 
//  
class KscSortVarPtr
{
public:
    KscSortVarPtr(KscVariable *ptr = 0) : p(ptr) {}

    // this is only needed to instantiate PltList
    //
    bool operator == (const KscSortVarPtr &) const { PLT_ASSERT(0); return false; }
    bool operator < (const KscSortVarPtr &rhs) const;
    KscVariable *operator -> () const;

private:
    KscVariable *p;
};

//////////////////////////////////////////////////////////////////////
// This is a helper class which groups together KscVariable objects
// belonging to the same server/av-module pair.
//
// Intented for internal use only.
//
class KscSorterBucket;
typedef PltPtrHandle<KscSorterBucket> KscBucketHandle;

class KscSorterBucket
{
public:
    KscSorterBucket(const KscAvModule *,
                    KscServerBase *);

    bool add(KscSortVarPtr);
    bool add(KscBucketHandle);
    size_t size() const;
    PltIterator< KscSortVarPtr > *newVarIterator() const;
    // call only once
    PltArray< KscSortVarPtr > getSortedVars();

    KscServerBase *getServer() const;
    const KscAvModule *getAvModule() const;

private:
    KscSorterBucket(const KscSorterBucket &);                // forbidden
    KscSorterBucket &operator = (const KscSorterBucket &);   // forbidden

    bool isEmpty() const;
    KscSortVarPtr removeFirst();

    PltList< KscSortVarPtr > var_lst;

    const KscAvModule *av_module;
    KscServerBase *server;
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
    KscSorter(const KscPackage &pkg,
              bool dirty_only = false,
              // This is used instead of pkg->getAvModule() 
              // if the later one is NULL.
              const KscAvModule *avm_default = 0);
    ~KscSorter();

    bool isValid() const;
    bool isEmpty() const;
    KscBucketHandle removeFirst();
    KscBucketHandle removeMatchingBucket(KscBucketHandle);
    PltIterator<KscBucketHandle> *newBucketIterator() const;

// FIXME private:
public:
    class Key
    {
    public:
        Key(const KscAvModule *,
            KscServerBase *);

        const KscAvModule *getAvModule() const
            { return av_module; }
        KscServerBase *getServer() const
            { return server; }

        unsigned long hash() const;
        friend bool operator == (const Key &, const Key &);

    private:
        const KscAvModule *av_module;
        KscServerBase *server;
    };

    friend class Key;

//FIXME END
private:

    class ValueIterator
    : public PltIterator<KscBucketHandle>
    {
    public:
#if PLT_RETTYPE_OVERLOADABLE
	#define KscSorter_ValueIterator_THISTYPE KscSorter::ValueIterator
        typedef ValueIterator THISTYPE;
#else
	#define KscSorter_ValueIterator_THISTYPE PltIterator_THISTYPE(KscBucketHandle)
#endif
        ValueIterator(const PltHashTable<Key, KscBucketHandle> &);

        operator bool () const;
        const KscBucketHandle * operator -> () const;
        THISTYPE & operator ++ ();
        void toStart();
    private:
        PltHashIterator<Key,KscBucketHandle> it;
    };

    friend class ValueIterator;

    bool sortVariables(const KscPackage &, 
                       const KscAvModule *);
    bool sortSubpackages(const KscPackage &,
                         const KscAvModule *);
    const KscAvModule *findAvModule(const KscVariable *, 
                                    const KscAvModule *);

    bool valid;
    bool fDirtyOnly;
    PltHashTable<Key,KscBucketHandle> table;
};


//////////////////////////////////////////////////////////////////////
// Inline Implementation
//////////////////////////////////////////////////////////////////////
// class KscSortVarPtr
//////////////////////////////////////////////////////////////////////

inline
bool
KscSortVarPtr::operator < (const KscSortVarPtr &other) const
{
    PLT_PRECONDITION(p && other.p);

    return p->getPathAndName() < other.p->getPathAndName();
}

//////////////////////////////////////////////////////////////////////

inline
KscVariable *
KscSortVarPtr::operator -> () const
{
    PLT_ASSERT(p);
    return p;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

inline
KscSorter::Key::Key(const KscAvModule *avm,
                    KscServerBase *svr)
: av_module(avm),
  server(svr)
{}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

inline
KscSorter::ValueIterator::ValueIterator(
    const PltHashTable<Key, KscBucketHandle> &table)
: it(table)
{}

//////////////////////////////////////////////////////////////////////

inline
KscSorter::ValueIterator::operator bool () const
{
    return it;
}

//////////////////////////////////////////////////////////////////////

inline
const KscBucketHandle *
KscSorter::ValueIterator::operator -> () const
{
    return &(it->a_value);
}

//////////////////////////////////////////////////////////////////////

inline
KscSorter_ValueIterator_THISTYPE &
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
                                 KscServerBase *svr)
: av_module(avm),
  server(svr),
  var_count(0)
{}

//////////////////////////////////////////////////////////////////////

inline
KscServerBase *
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
KscSorterBucket::isEmpty() const
{
    PLT_PRECONDITION((var_count == 0 && var_lst.isEmpty()) ||
                     (var_count > 0 && !var_lst.isEmpty()));
    return var_lst.isEmpty();
}

//////////////////////////////////////////////////////////////////////

inline
bool
KscSorterBucket::add(KscSortVarPtr var)
{
    bool ok = var_lst.addFirst(var);

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
    return var_count;
}

//////////////////////////////////////////////////////////////////////

inline
PltIterator< KscSortVarPtr > *
KscSorterBucket::newVarIterator() const
{
    return PLT_RETTYPE_CAST((PltIterator<KscSortVarPtr> *))
        var_lst.newIterator();
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

inline
bool
KscSorter::isValid() const
{
    return valid;
}

//////////////////////////////////////////////////////////////////////

inline
bool
KscSorter::isEmpty() const
{
    return table.isEmpty();
}

//////////////////////////////////////////////////////////////////////

inline
PltIterator<KscBucketHandle> *
KscSorter::newBucketIterator() const
{
    return new ValueIterator(table);
}

//////////////////////////////////////////////////////////////////////

#endif

//////////////////////////////////////////////////////////////////////
// EOF sorter.h
//////////////////////////////////////////////////////////////////////






