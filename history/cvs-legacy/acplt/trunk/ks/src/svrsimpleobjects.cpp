/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/src/svrsimpleobjects.cpp,v 1.13 1999-04-22 15:40:20 harald Exp $ */
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

#include "ks/svrsimpleobjects.h"

//////////////////////////////////////////////////////////////////////

PLT_IMPL_RTTI0(KssSimpleCommObject);
PLT_IMPL_RTTI2(KssSimpleDomain, KssDomain, KssSimpleCommObject);
PLT_IMPL_RTTI2(KssSimpleVariable, KssVariable, KssSimpleCommObject);
PLT_IMPL_RTTI2(KssTimeNowVariable, KssVariable, KssSimpleCommObject);
PLT_IMPL_RTTI2(KssSimpleLinkAlias, KssDomain, KssSimpleCommObject);

//////////////////////////////////////////////////////////////////////

KssCommObjectHandle
KssSimpleDomain::getChildById(const KsString & id) const
{
    KssCommObjectHandle h;
    if (_children.query(id, h) ) {
        // found in own children
        return h;
    } else if (_next_sister) {
        // otherwise ask sister
        return _next_sister->getChildById(id);
    } else {
        // otherwise there is no such child
        return KssCommObjectHandle();
    }
}

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
                                         KsString comment)
: _identifier(id),
  _creation_time(ctime),
  _comment(comment)
{
}


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
				       KsString comment)
: KssSimpleCommObject(id, ctime, comment),
  _halias_domain(hdom)
{ } // KssSimpleLinkAlias::KssSimpleLinkAlias


// ----------------------------------------------------------------------------
//
KS_RESULT
KssSimpleLinkAlias::getCurrProps(KsCurrPropsHandle &hprops) const
{
    class _PltStringNode {
    public:
	struct _PltStringNode *next;
	KsString               id;

	_PltStringNode() {}
	~_PltStringNode() {}
    }; // class _PltStringNode

    _PltStringNode *head = 0;
    _PltStringNode *tail = 0;
    _PltStringNode *node;
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
	node = new _PltStringNode;
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
    while ( head ) {
	node = head;
	head = head->next;
	*vec++ = node->id;
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
