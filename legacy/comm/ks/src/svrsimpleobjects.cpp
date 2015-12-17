/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/src/svrsimpleobjects.cpp,v 1.23 2007-04-25 12:57:21 martin Exp $ */
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
/* Author: Martin Kneissl <martin@plt.rwth-aachen.de> */

#include "ks/svrsimpleobjects.h"

//////////////////////////////////////////////////////////////////////

PLT_IMPL_RTTI0(KssSimpleCommObject);
PLT_IMPL_RTTI2(KssSimpleDomain, KssDomain, KssSimpleCommObject);
PLT_IMPL_RTTI2(KssSimpleVariable, KssVariable, KssSimpleCommObject);
PLT_IMPL_RTTI2(KssTimeNowVariable, KssVariable, KssSimpleCommObject);
PLT_IMPL_RTTI2(KssSimpleLinkAlias, KssDomain, KssSimpleCommObject);


// ---------------------------------------------------------------------------
// Lookup an immediate child of this domain by the child's identifier.
// Returns an unbound handle in case the child does not exist.
//
KssCommObjectHandle
KssSimpleDomain::getChildById(const KsString & id) const
{
    KssCommObjectHandle h;
    if ( _children.query(id, h) ) {
	//
        // Found in own set of children, so return it.
	//
        return h;
    } else if ( _next_sister ) {
	//
        // Not found here, but delegate to our sister which might know
	// this child.
	//
        return _next_sister->getChildById(id);
    } else {
	//
        // Otherwise there is no such child, so we return an unbound
	// handle to the caller.
	//
        return KssCommObjectHandle();
    }
} // KssSimpleDomain::getChildById


//////////////////////////////////////////////////////////////////////

bool 
KssSimpleDomain::addChild(KssCommObjectHandle h) 
{
    if (h) {
        return _children.add(h->getIdentifier(), h);
    } else {
        return false;
    }
}

//////////////////////////////////////////////////////////////////////

bool 
KssSimpleDomain::addChild(KssCommObject * p) 
{
    KssCommObjectHandle h(p, PltOsNew);
    return addChild(h);
}

//////////////////////////////////////////////////////////////////////

bool
KssSimpleDomain::setNextSister(KssDomain * p)
{
    KssDomainHandle h(p, PltOsNew);
    if (h) {
        setNextSister(h);
        return true;
    } else {
        return false;
    }
}

//////////////////////////////////////////////////////////////////////

size_t 
KssSimpleDomain::size() const
{
    return _children.size();
}

//////////////////////////////////////////////////////////////////////

KssCommObjectHandle
KssSimpleDomain::removeChild(const KsString &id)
{
    KssCommObjectHandle h;
    if (_children.remove(id, h)) {
        return h;
    } else {
        return KssCommObjectHandle();
    }
}

//////////////////////////////////////////////////////////////////////
// KsSimpleDomainIterator
//////////////////////////////////////////////////////////////////////
// delegates to hashtable iterator or sister iterator

KssSimpleDomainIterator::KssSimpleDomainIterator(const KssSimpleDomain & d)
: _children_iter(d._children),
  _p_sister_iter(d._next_sister
                 ? PLT_RETTYPE_CAST((KssDomainIterator*)) 
                     d._next_sister->newIterator()
                 : 0 ),
  _at_sister(false)
{
}


KssSimpleDomainIterator::KssSimpleDomainIterator
(const PltHashTable<KsString,KssCommObjectHandle> &ht)
: _children_iter(ht),
  _p_sister_iter(0),
  _at_sister(false)
{
}


//////////////////////////////////////////////////////////////////////

KssSimpleDomainIterator::~KssSimpleDomainIterator()
{
    if (_p_sister_iter) delete _p_sister_iter;
}

//////////////////////////////////////////////////////////////////////

KssSimpleDomainIterator::operator bool () const
{
    if (_p_sister_iter && _at_sister) {
        return *_p_sister_iter;
    } else {
        return _children_iter;
    }
}

//////////////////////////////////////////////////////////////////////

KssCommObjectHandle
KssSimpleDomainIterator::operator * () const
{
    PLT_ASSERT(*this);
    if (_p_sister_iter && _at_sister) {
        return *(*_p_sister_iter);
    } else {
        return _children_iter->a_value;
    }
}

//////////////////////////////////////////////////////////////////////

KssSimpleDomainIterator_THISTYPE &
KssSimpleDomainIterator::operator ++ ()
{
    PLT_ASSERT(*this);
    if (_p_sister_iter && _at_sister) {
        ++(*_p_sister_iter);
    } else {
        ++ _children_iter;
        if (! _children_iter) {
            _at_sister = true;
        }
    }
    return *this;
}

//////////////////////////////////////////////////////////////////////

void 
KssSimpleDomainIterator::toStart()
{
    _children_iter.toStart();
    if (_p_sister_iter) _p_sister_iter->toStart();
    _at_sister = false;
}

//////////////////////////////////////////////////////////////////////

KssSimpleCommObject::KssSimpleCommObject(const KsString &id,
                                         KsTime ctime,
                                         KsString comment,
					 KS_SEMANTIC_FLAGS semflags)
    : _identifier(id),
      _creation_time(ctime),
      _comment(comment),
      _semantic_flags(semflags)
{
} // KssSimpleCommObject::KssSimpleCommObject


//////////////////////////////////////////////////////////////////////


KssSimpleDomain::KssSimpleDomain(const KsString &id,
                                 KsTime ctime,
                                 KsString comment)
: KssSimpleCommObject(id, ctime, comment)
{
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

KssSimpleVariable::KssSimpleVariable(const KsString &id,
                                     KsTime ctime,
                                     const KsString &comment)
: KssSimpleCommObject(id, ctime, comment),
  _access_mode(KS_AC_READ|KS_AC_WRITE),
  _time(KsTime::now()),
  _state(KS_ST_UNKNOWN)
{
}

//////////////////////////////////////////////////////////////////////

KS_RESULT
KssSimpleVariable::setValue(const KsValueHandle &h)
{
    if (isWriteable()) {
        _value = h;
        return KS_ERR_OK;
    } else {
        return KS_ERR_NOACCESS;
    }
}

//////////////////////////////////////////////////////////////////////

KS_RESULT
KssSimpleVariable::setTime(const KsTime &t)
{
    if (isWriteable()) {
        _time = t;
        return KS_ERR_OK;
    } else {
        return KS_ERR_NOACCESS;
    }
}

//////////////////////////////////////////////////////////////////////

KS_RESULT
KssSimpleVariable::setState(KS_STATE st) 
{
    if (isWriteable()) {
        _state =  st;
        return KS_ERR_OK;
    } else {
        return KS_ERR_NOACCESS;
    }
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
KsValueHandle
KssTimeNowVariable::getValue() const
{
    static KsTimeValue tv;
    KsTime now(KsTime::now());
    tv.tv_sec  = now.tv_sec;
    tv.tv_usec = now.tv_usec;
    return KsValueHandle(&tv, KsOsUnmanaged);
}



// ----------------------------------------------------------------------------
//
KssSimpleLinkAlias::KssSimpleLinkAlias(KssDomainHandle hdom, 
				       const KsString &id,
				       KsTime ctime,
				       const KsString &comment,
				       const KsString &aliasPath)
: KssSimpleCommObject(id, ctime, comment),
  _halias_domain(hdom),
  _alias_path(aliasPath)
{ } // KssSimpleLinkAlias::KssSimpleLinkAlias


// ----------------------------------------------------------------------------
// Helper class to build a very simple linked list without all the bells and
// whistles PltList<> provides. Someday, I'll maybe convert this one...
// One upon a time, this class was local to KssSimpleLinkAlias::getCurrProps,
// but VC++ 4.2 fails on this, so I've moved it to this new place. What a
// bloody compiler VC 4.2 is.
//
class _PltSimpleStringNode {
public:
    class _PltSimpleStringNode *next;
    KsString                    id;
    
    _PltSimpleStringNode() {}
    ~_PltSimpleStringNode() {}
}; // class _PltSimpleStringNode


// ----------------------------------------------------------------------------
//
KS_RESULT
KssSimpleLinkAlias::getCurrProps(KsCurrPropsHandle &hprops) const
{
    _PltSimpleStringNode *head = 0;
    _PltSimpleStringNode *tail = 0;
    _PltSimpleStringNode *node;
    unsigned long   itemcount = 0;

    //
    // Try to get an iterator from the underlying (simple) domain server
    // object. We will then use this iterator for calculating the value.
    //
    KssDomainIterator *pit = PLT_RETTYPE_CAST((KssDomainIterator *))
	_halias_domain->newIterator();
    if ( !pit ) {
	return KS_ERR_GENERIC;
    }
    //
    // Now collect the identifiers of all children of the domain we're
    // about to scan. The identifiers are put into a linked list -- we don't
    // know yet how many identifiers we'll receive during the iteration
    // process.
    //
    while ( *pit ) {
	node = new _PltSimpleStringNode;
	if ( !node || !(*pit) ) {
	    while ( head ) {
		node = head;
		head = head->next;
		delete node;
	    }
	    return KS_ERR_GENERIC;
	}
	++itemcount;
	node->id = (**pit)->getIdentifier();
	node->next = 0;
	if ( !tail ) {
	    head = node;
	    tail = node;
	} else {
	    tail->next = node;
	    tail = node;
	}
	++(*pit);
    }
    delete pit; // don't forget to free the iterator!
    //
    // Okay. Now create a value from the list of identifiers.
    //
    KsString *vec = new KsString[itemcount];
    KsValueHandle hVal(new KsStringVecValue(itemcount, vec, KsOsArrayNew), 
                       KsOsNew);
    if ( (itemcount && !vec) || !hVal ) {
	//
        // Failed to allocate enough memory. So clean up the mess and
        // leave the stage.
	//
	if ( vec && !hVal ) {
	    delete [] vec;
	}
	while ( head ) {
	    node = head;
	    head = head->next;
	    delete node;
	}
	return KS_ERR_GENERIC;
    }
    //
    // Copy the identifiers from the list into the value array.
    //
    bool absPath = _alias_path.len() != 0;
    while ( head ) {
	node = head;
	head = head->next;
	if ( absPath ) {
	    *vec++ = _alias_path + "/" + node->id;
	} else {
            *vec++ = node->id;
	}
	delete node;
    }
    //
    // Finally set up the current properties and return them.
    //
    KsVarCurrProps *pvcurrprops = new KsVarCurrProps;
    if ( !pvcurrprops ) {
	return KS_ERR_GENERIC;
    }
    pvcurrprops->value = hVal;
    pvcurrprops->state = KS_ST_GOOD;
    pvcurrprops->time = KsTime::now();

    hprops.bindTo(pvcurrprops, KsOsNew);

    return KS_ERR_OK;
} // KssSimpleLinkAlias::getCurrProps

//////////////////////////////////////////////////////////////////////
/* EOF svrsimpleobjects.cpp */
