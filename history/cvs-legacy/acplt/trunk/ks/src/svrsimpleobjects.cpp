/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/src/svrsimpleobjects.cpp,v 1.1 1997-03-23 14:35:19 martin Exp $ */
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

//////////////////////////////////////////////////////////////////////

KssCommObjectHandle
KssSimpleDomain::child(const KsString & id) const
{
    KssCommObjectHandle h;
    if (_children.query(id, h) ) {
        // found in own children
        return h;
    } else if (_next_sister) {
        // otherwise ask sister
        return _next_sister->child(id);
    } else {
        // otherwise there is no such child
        return KssCommObjectHandle();
    }
}

//////////////////////////////////////////////////////////////////////

bool 
KssSimpleDomain::addChild(KssCommObjectHandle h) 
{
    return h ? _children.add(h->getIdentifier(), h) : false;
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

KssSimpleDomainIterator::KssSimpleDomainIterator(const KssSimpleDomain & d,
                                                 const KsString & name_mask,
                                                 const KS_OBJ_TYPE type_mask)
: _name_mask(name_mask),
  _type_mask(type_mask),
  _children_iter(d._children),
  _p_sister_iter(d._next_sister ?
                 d._next_sister->newIterator(name_mask, type_mask)
                 : 0 ),
  _at_sister(false)
{
}

//////////////////////////////////////////////////////////////////////

KssSimpleDomainIterator::~KssSimpleDomainIterator()
{
    if (_p_sister_iter) delete _p_sister_iter;
}

//////////////////////////////////////////////////////////////////////

KssSimpleDomainIterator::operator const void * () const
{
    if (_p_sister_iter && _at_sister) {
        return *_p_sister_iter;
    } else {
        return _children_iter;
    }
}

//////////////////////////////////////////////////////////////////////

const KssCommObjectHandle & 
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

KssSimpleDomainIterator & 
KssSimpleDomainIterator::operator ++ ()
{
    PLT_ASSERT(*this);
    if (_p_sister_iter && _at_sister) {
        ++(*_p_sister_iter);
    } else {
        KssCommObject * p;
        do {
            ++ _children_iter;
            if (! _children_iter) {
                _at_sister = true;
                break; //>>>>
            }
            p = _children_iter->a_value.getPtr();
          } while (! (   (p->typeCode() & _type_mask) 
                      && (p->getIdentifier() == _name_mask)));
        // TODO: pattern matching    ^^^^^^^^^^^^^
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
                                     KsString comment)
: KssSimpleCommObject(id, ctime, comment),
  _time(KsTime::now()),
  _state(KS_ST_UNKNOWN)
{
}

/////////////////////////////////////////////////////////////////////////////
/* EOF svrsimpleobjects.cpp */
