/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/src/svrobjects.cpp,v 1.6 1997-04-03 15:14:31 martin Exp $ */
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

//////////////////////////////////////////////////////////////////////

#include "ks/svrobjects.h"

//////////////////////////////////////////////////////////////////////
// RTTI implementation
//////////////////////////////////////////////////////////////////////

PLT_IMPL_RTTI0(KssCommObject);
PLT_IMPL_RTTI1(KssDomain, KssCommObject);
PLT_IMPL_RTTI1(KssVariable, KssCommObject);

//////////////////////////////////////////////////////////////////////
// class KssDomain
//////////////////////////////////////////////////////////////////////

KssCommObjectHandle
KssDomain::getChildById(const KsString &id) const
{
    //
    // Linear search. This may be VERY inefficient! But then
    // you should implement your own version of this method.
    //
    KssCommObjectHandle res;

    KssDomainIterator * pit = newIterator();
    if (pit) {
        for (KssDomainIterator &it(*pit); it; ++it) {
            if ((*it)->getIdentifier() == id) {
                res = *it;
            }
        }
        delete pit;
    }
    return res;
}

/////////////////////////////////////////////////////////////////////////////

KsProjPropsHandle
KssDomain::getPP() const
{
    KsDomainProjProps * p = new KsDomainProjProps;
    KsProjPropsHandle h(p, KsOsNew);
    if (p && h) {
        p->identifier    = getIdentifier();
        p->creation_time = getCreationTime();
        p->comment       = getComment();
        p->access_mode   = getAccessMode();
    }
    return h;
}

//////////////////////////////////////////////////////////////////////

KssDomainIterator *
KssDomain::newMaskedIterator(const KsMask & name_mask,
                             KS_OBJ_TYPE type_mask) const
{
    PltPtrHandle<KssDomainIterator> h(newIterator(), PltOsNew);
    if (h) {
        return new KssMaskedDomainIterator(h, name_mask, type_mask);
    } else {
        return 0;
    }
}

//////////////////////////////////////////////////////////////////////

KssCommObjectHandle
KssDomain::getChildByPath(const KsPath & path) const 
{
    PLT_PRECONDITION(path.isValid());
    // lookup child
    KssCommObjectHandle hc(getChildById(path.getHead()));
    if (hc) {
        // Good. There is such a child.
        if (path.isSingle()) {
            // We are beeing asked about this child. Return it.
            return hc;
        } else {
            // They want a grandchild. That means the child must be
            // a domain.
            KssDomain * pd = PLT_DYNAMIC_PCAST(KssDomain, hc.getPtr());
            if (pd) {
                // It is a domain. Let _it_ handle the rest of the
                // request.
                KsPath tail(path.getTail());
                return pd->getChildByPath(tail);
            } else {
                // The child not a domain, fail.
                return KssCommObjectHandle();
            }
        }
    } else {
        // We don't know this child. 
        return KssCommObjectHandle();
    }
}

//////////////////////////////////////////////////////////////////////
// KssMaskedDomainIterator
//////////////////////////////////////////////////////////////////////

KssMaskedDomainIterator::KssMaskedDomainIterator
(const PltPtrHandle<KssDomainIterator> & hit,
 const KsMask & name_mask,
 const KS_OBJ_TYPE type_mask) 
: _name_mask(name_mask),
  _type_mask(type_mask),
  _hit(hit),
  _it(*hit)
{
    skipWhileNotMatching();
}

//////////////////////////////////////////////////////////////////////

KssMaskedDomainIterator::operator const void * () const
{
    return _it.operator const void * ();
}

//////////////////////////////////////////////////////////////////////

KssCommObjectHandle
KssMaskedDomainIterator::operator * () const
{
    return _it.operator *();
}

//////////////////////////////////////////////////////////////////////

KssMaskedDomainIterator & 
KssMaskedDomainIterator::operator ++ ()
{
    ++_it;
    skipWhileNotMatching();
    return *this;
}

//////////////////////////////////////////////////////////////////////

void
KssMaskedDomainIterator::skipWhileNotMatching()
{
    for ( ; _it; ++_it) {
        if (*_it) {
            // Handle is ok
            const KssCommObject & obj = **_it;
            if (   (obj.typeCode() & _type_mask)
                && 1==_name_mask.matches(obj.getIdentifier())) {
                // Match! Stop skipping
                return;
            } 
        } else {
            // Nullhandle. Mhmm. TODO: Is this a match? 
            // For now, it is not.
        }
    }
}

//////////////////////////////////////////////////////////////////////

void 
KssMaskedDomainIterator::toStart()
{
    _it.toStart();
    skipWhileNotMatching();
}

//////////////////////////////////////////////////////////////////////
// KssVariable
//////////////////////////////////////////////////////////////////////

KsProjPropsHandle
KssVariable::getPP() const
{
    KsVarProjProps * p = new KsVarProjProps;

    KsProjPropsHandle h(p, KsOsNew);
    if (p && h) {
        p->identifier    = getIdentifier();
        p->creation_time = getCreationTime();
        p->comment       = getComment();
        p->access_mode   = getAccessMode();
        p->tech_unit     = getTechUnit();
        p->type          = getType();
    }
    return h;
}

/////////////////////////////////////////////////////////////////////////////

KS_VAR_TYPE
KssVariable::getType() const 
{
    KsValueHandle vh = getValue();
    return vh ? vh->xdrTypeCode() : KS_VT_VOID;
}

//////////////////////////////////////////////////////////////////////

KS_STATE
KssVariable::getState() const
{
    if (getValue()) {
        return KS_ST_UNKNOWN;
    } else {
        return KS_ST_BAD;
    }
}
//////////////////////////////////////////////////////////////////////

KsTime
KssVariable::getTime() const
{
    return KsTime::now();
}


//////////////////////////////////////////////////////////////////////

KsCurrPropsHandle
KssVariable::getCurrProps() const
{
    KsCurrPropsHandle hprops;
    KsValueHandle vh(getValue());
    if (vh) {
        // There was a value.
        KsCurrPropsHandle hprops(new KsVarCurrProps(vh, 
                                                    getTime(),
                                                    getState()),
                                 KsOsNew);
        return hprops;
    } else {
        return KsCurrPropsHandle();
    }
}

//////////////////////////////////////////////////////////////////////

KS_RESULT
KssVariable::setValue(KsValue *p) 
{
    KsValueHandle h(p, KsOsNew);
    if (h) {
        return setValue(h);
    } else {
        return KS_ERR_GENERIC;
    }
}

//////////////////////////////////////////////////////////////////////

KS_RESULT 
KssVariable::setCurrProps(KsVarCurrProps &props) 
{
    KS_RESULT res = setValue(props.value);
    if (res == KS_ERR_OK) {
        setTime(props.time);
        setState(props.state);
    };
    return res;
}

//////////////////////////////////////////////////////////////////////
    
KS_RESULT     
KssVariable::setCurrProps(const KsCurrPropsHandle & hprops)
{
    KsVarCurrProps *pprops = 
        PLT_DYNAMIC_PCAST(KsVarCurrProps, hprops.getPtr());
    if (pprops) {
        return setCurrProps(*pprops);
    } else {
        return KS_ERR_BADTYPE;
    }
}

//////////////////////////////////////////////////////////////////////
/* EOF ks/svrobjects.cpp */
