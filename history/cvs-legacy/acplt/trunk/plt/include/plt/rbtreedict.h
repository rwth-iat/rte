/* -*-plt-c++-*- */
#ifndef PLT_RBTREEDICT_INCLUDED
#define PLT_RBTREEDICT_INCLUDED




/*
 *  
 *                 PACKAGE UNDER CONSTRUCTION 
 *            
 */





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
/* Author: Martin Kneissl <martin@plt.rwth-aachen.de> */

#include "plt/dictionary.h"
#include "plt/comparable.h" // This should be reworked


//////////////////////////////////////////////////////////////////////
// forward declarations
class PltRbTreeDictionary_base;

//////////////////////////////////////////////////////////////////////

template <class K, class V>
class PltRbTreeDictionary 
: 
  public PltRbTreeDictionary_base, 
  public PltDictionary<K,V>
{
public:
    virtual bool query(const K&, V&) const;
#if PLT_DEBUG_INVARIANTS
    bool invariant() const;
#endif

    // modifiers
    virtual bool add(const K&, const V&);
    virtual bool remove(const K&, V&);
private:
    PltRbTreeDictionary(const PltRbTreeDictionary &); //forbidden
    PltRbTreeDictionary<K,V> & operator = ( const PltRbTreeDictionary &); 
    // ^^^ forbidden
};

//////////////////////////////////////////////////////////////////////
// IMPLEMENTATION
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// forward declarations

template <class K, class V> class PltRbTreeDictionary;
class PltRbTreeNode_base;

//////////////////////////////////////////////////////////////////////

class PltRbTreeNode_base 
: protected PltComparable
{
    friend class PltRbTreeDictionary_base;
public:
    PltRbTreeNode_base();
protected:
    static PltRbTreeNode_base * nil;
    PltRbTreeNode_base *l;
    PltRbTreeNode_base *r;
    bool red;

    PltRbTreeNode_base *
        child(const PltRbTreeNode_base *p) const;
    void 
        setChild(PltRbTreeNode_base *p);

    virtual bool operator <  (const PltComparable &) const;
private:
    static PltRbTreeNode_base nil_obj;

};

//////////////////////////////////////////////////////////////////////

class PltRbTreeDictionary_base
{
public:
    PltRbTreeDictionary_base();
    virtual ~PltRbTreeDictionary_base();

#if PLT_DEBUG_INVARIANTS
    bool invariant() const = 0;
#endif

protected:
    bool addNode(PltRbTreeNode_base *p);
    const PltRbTreeNode_base *lookupNode(const PltRbTreeNode_base &) const;
private:
    PltRbTreeNode_base *head;
    PltRbTreeNode_base *z;
    //
    PltRbTreeNode_base *rotateNode(PltRbTreeNode_base *v,
                                   PltRbTreeNode_base *y);
    PltRbTreeNode_base *
        splitNode(PltRbTreeNode_base *v,
                  PltRbTreeNode_base *gg,
                  PltRbTreeNode_base *g,
                  PltRbTreeNode_base *p,
                  PltRbTreeNode_base *x);

    PltRbTreeDictionary_base(const PltRbTreeDictionary_base &); //forbidden
    PltRbTreeDictionary_base & operator = ( const PltRbTreeDictionary_base &); 
    // ^^^ forbidden
};

//////////////////////////////////////////////////////////////////////

template <class K, class V>
class PltRbTreeNode : public PltRbTreeNode_base 
{
    friend class PltRbTreeDictionary<K,V>;
public:
    PltRbTreeNode(const K &, const V &);
protected:
    virtual bool operator < (const PltComparable &) const;
    K key;
    V value;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////


inline
PltRbTreeNode_base::PltRbTreeNode_base() 
: l(nil), 
  r(nil), 
  red(false)
{
}
    
//////////////////////////////////////////////////////////////////////

inline PltRbTreeNode_base *
PltRbTreeNode_base::child(const PltRbTreeNode_base *p) const
{
    PLT_PRECONDITION( p );
    return operator < (*p) ? r : l;
}

//////////////////////////////////////////////////////////////////////
inline void 
PltRbTreeNode_base::setChild(PltRbTreeNode_base *p) 
{
    PLT_PRECONDITION( p ); 
    if ( operator < (*p) ) {
        r = p;
    } else {
        l = p;
    };
}

//////////////////////////////////////////////////////////////////////

inline bool
PltRbTreeNode_base::operator < (const PltComparable &) const
{
    // MAGIC INSIDE
    // PltRbTreeNode_base nodes are smaller than every "real"
    // nodes. This enables us too use them as sentinels.
    return true;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

template <class K, class V>
inline
PltRbTreeNode<K,V>::PltRbTreeNode(const K & k , const V & v)
: key(k), value(v)
{
}

//////////////////////////////////////////////////////////////////////

template <class K, class V>
inline bool
PltRbTreeNode<K,V>::operator < (const PltComparable & r) const
{
    bool result = key < ((PltRbTreeNode<K,V> &) r).key;
    return result;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
    
template <class K, class V>
inline bool
PltRbTreeDictionary<K,V>::add(const K & k, const V & v) {
    PltRbTreeNode<K,V> *p = new PltRbTreeNode<K,V>(k,v);
    if (!p) {
        return false;
    } else {
        return addNode(p);
    }
}

//////////////////////////////////////////////////////////////////////

template <class K, class V>
inline bool
PltRbTreeDictionary<K,V>::invariant() const
{
    // TODO
    return true;
}

//////////////////////////////////////////////////////////////////////

template <class K, class V>
inline bool
PltRbTreeDictionary<K,V>::query(const K & k, V & v) const
{
    const PltRbTreeNode<K,V> n(k,v);
    const PltRbTreeNode<K,V> *p = (PltRbTreeNode<K,V>*) lookupNode(n);
    if (p != PltRbTreeNode_base::nil) {
        v = p->value;
        return true;
    } else {
        return false;
    }
}
        
//////////////////////////////////////////////////////////////////////

template <class K, class V>
inline bool
PltRbTreeDictionary<K,V>::remove(const K & k, V & v)
{ // TODO: IMPLEMENT IT
        return false;
}
        
//////////////////////////////////////////////////////////////////////

#endif // PLT_RBTREEDICT_INCLUDED
