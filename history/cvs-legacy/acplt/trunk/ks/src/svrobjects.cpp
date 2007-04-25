/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/src/svrobjects.cpp,v 1.20 2007-04-25 12:57:21 martin Exp $ */
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

/*
 * svrobjects.cpp: contains the abstract classes which represent communication
 *                 objects within ACPLT/KS. In addition, this module also
 *                 defines some basic interfaces which apply to retrieving
 *                 children/parts of a communication object and retrieving or
 *                 setting the current properties of a variable etc.
 */

/* Author: Martin Kneissl <martin@plt.rwth-aachen.de> */
/* v1+ and v2 objects added by Harald Albrecht <harald@plt.rwth-aachen.de> */


#include "ks/svrobjects.h"

// ----------------------------------------------------------------------------
// RTTI implementation
//
PLT_IMPL_RTTI0(KssCurrPropsService);
PLT_IMPL_RTTI0(KssChildrenService);

PLT_IMPL_RTTI0(KssCommObject);
PLT_IMPL_RTTI2(KssDomain, KssCommObject, KssChildrenService);
PLT_IMPL_RTTI2(KssVariable, KssCommObject, KssCurrPropsService);
PLT_IMPL_RTTI3(KssLink, KssCommObject, KssChildrenService, KssCurrPropsService);
PLT_IMPL_RTTI2(KssHistory, KssCommObject, KssChildrenService);


// ----------------------------------------------------------------------------
// This is just a default implementation provided for backwards compatibility
// with old software. You should instead implement your own version, which is
// capable of returning more differentiated error codes.
//
KS_RESULT
KssCurrPropsService::getCurrProps(KsCurrPropsHandle &hprops) const
{
    hprops = getCurrProps();
    return hprops ? KS_ERR_OK : KS_ERR_GENERIC;
} // KssCurrPropsService::getCurrProps


// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// All the stuff belonging to the class KssDomain.
//

// ----------------------------------------------------------------------------
// Get a child using its identifier. The identifier must identify (sic!) an
// immediate child of the domain object, or else an unbound handle is returned.
//
KssCommObjectHandle
KssDomain::getChildById(const KsString &id) const
{
    //
    // Linear search. This may be VERY inefficient! But then
    // you should implement your own version of this method.
    //
    KssCommObjectHandle res;

    KssDomainIterator * pit = 
        PLT_RETTYPE_CAST((KssDomainIterator *)) newIterator();
    if (pit) {
        for (KssDomainIterator &it = *pit; it; ++it) {
            if ((*it)->getIdentifier() == id) {
                res = *it;
            }
        }
        delete pit;
    }
    return res;
} // KssDomain::getChildById


// ----------------------------------------------------------------------------
// Return the engineered properties of a domain object. This method basically
// calls the individual getXXX accessors to retrieve the various parts of the
// engineered properties a domain object has. In case this should be ineffi-
// cient, you should provide your own getEP() method to fill in all the fields
// in one shot.
//
KsEngPropsHandle
KssDomain::getEP() const
{
    KsDomainEngProps *p = new KsDomainEngProps;
    KsEngPropsHandle h(p, KsOsNew);
    if ( p && h ) {
        p->identifier       = getIdentifier();
        p->creation_time    = getCreationTime();
        p->comment          = getComment();
        p->access_mode      = getAccessMode();
	p->semantic_flags   = getSemanticFlags();
	p->class_identifier = getClassIdentifier();
    }
    return h;
} // KssDomain::getEP


// ----------------------------------------------------------------------------
// Return an iterator suitable to iterate over the children of a domain object.
// Note that the iterator returned by this method will iterate only over
// children and *not* parts.
//
KssDomainIterator_THISTYPE *
KssDomain::newMaskedIterator(const KsMask & name_mask,
                             KS_OBJ_TYPE type_mask) const
{
    PltPtrHandle<KssDomainIterator>
        h(PLT_RETTYPE_CAST((KssDomainIterator *)) newIterator(),
          PltOsNew);
    if (h) {
        return new KssMaskedDomainIterator(h, name_mask, type_mask);
    } else {
        return 0;
    }
} // KssDomain::newMaskedIterator


// ----------------------------------------------------------------------------
// Lookup a child using its path name.
// FIXME: this needs to be checked for parts...
//
KssCommObjectHandle
KssDomain::getChildByPath(const KsPath & path) const
{
    PLT_PRECONDITION(path.isValid());

    //
    // First, lookup immediate child. This falls back to using the
    // getChildById() method.
    //
    KssCommObjectHandle hc(getChildById(path.getHead()));
    if ( hc ) {
	//
        // Good. There is such a child.
	//
        if ( path.isSingle() ) {
	    //
            // We are being asked about *this* child. Return it. That's it.
	    //
            return hc;
        } else {
	    //
            // "They" want a grandchild. That means that the immediate child
	    // can either be a:
            //   - domain,
	    //   - link,
	    //   - history,
	    //   - structure. FIXME
	    // Unfortunately, because we want to use the lowest common C++
	    // denominator, we can't use a real dynamic cast, so we have to
	    // explicitly up- and downcast to get the real pointer and let
	    // the compiler fix up the offsets for retrieving the right
	    // "this" pointer.
	    //
            KssChildrenService *pcs = 0;
	    switch ( hc->typeCode() ) {
	    case KS_OT_DOMAIN:
	        pcs = (KssDomain *) hc.getPtr();
	        break;
	    case KS_OT_LINK:
		pcs = (KssLink *) hc.getPtr();
		break;
	    case KS_OT_HISTORY:
		pcs = (KssHistory *) hc.getPtr();
		break;
	    default:
		break;
	    }

            if ( pcs ) {
		//
                // It is an object which can handle questions about childen.
		// Let *it* handle the rest of the request.
		//
                KsPath tail(path.getTail());
                return pcs->getChildByPath(tail);
            } else {
		//
                // This object doesn't know anything about children, so fail.
		// To indicate this, we just return an unbound handle.
		//
                return KssCommObjectHandle();
            }
        }
    } else {
	//
        // We don't know *this* child. So fail and return an unbound handle.
	//
        return KssCommObjectHandle();
    }
} // KssDomain::getChildByPath


// ----------------------------------------------------------------------------
// KssMaskedDomainIterator
//
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

KssMaskedDomainIterator::operator bool () const
{
    return _it.operator bool();
}

//////////////////////////////////////////////////////////////////////

KssCommObjectHandle
KssMaskedDomainIterator::operator * () const
{
    return _it.operator *();
}

//////////////////////////////////////////////////////////////////////

KssMaskedDomainIterator_THISTYPE	 &
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
        KssCommObjectHandle hobj(*_it);
        if (hobj) {
            // Handle is ok
            const KssCommObject & obj = *hobj;
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


// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// All the stuff belonging to the class KssVariable.
//

// ----------------------------------------------------------------------------
// Return the engineered properties of a variable object. This method basically
// works the same as KssDomain::getEP(), so the comments made there apply to
// this sucker too.
//
KsEngPropsHandle
KssVariable::getEP() const
{
    KsVarEngProps * p = new KsVarEngProps;
    KsEngPropsHandle h(p, KsOsNew);
    if ( p && h ) {
        p->identifier     = getIdentifier();
        p->creation_time  = getCreationTime();
        p->comment        = getComment();
        p->access_mode    = getAccessMode();
	p->semantic_flags = getSemanticFlags();
        p->tech_unit      = getTechUnit();
        p->type           = getType();
    }
    return h;
} // KssVariable::getEP


// ----------------------------------------------------------------------------
// Return the type of variable, which is basically just another description
// for the data type used by this particular variable. In case no value is
// currently set with this variable, then we assume "void".
//
KS_VAR_TYPE
KssVariable::getType() const
{
    KsValueHandle vh = getValue();
    return vh ? vh->xdrTypeCode() : KS_VT_VOID;
} // KssVariable::getType


// ----------------------------------------------------------------------------
// Return information about the variable's value state. The default
// implementation just boils down to checking if a value exists and then
// returning "bad" or otherwise "unknown", if no value currently exists for
// this variable.
//
KS_STATE
KssVariable::getState() const
{
    if ( getValue() ) {
        return KS_ST_UNKNOWN;
    } else {
        return KS_ST_BAD;
    }
} // KssVariable::getState


// ----------------------------------------------------------------------------
// Return the timestamp of the value. Default is just "now". No future, no
// past, no thermodynamic arrow of time at all. Yeeesss!!!!
//
KsTime
KssVariable::getTime() const
{
    return KsTime::now();
} // KssVariable::getTime


// ----------------------------------------------------------------------------
// Retrieve the current properties of a variable, that is, its value.
//
KsCurrPropsHandle
KssVariable::getCurrProps() const
{
    KsCurrPropsHandle hprops;
    KsValueHandle vh(getValue());
    if ( vh ) {
	//
        // There was a value. So we put into a structure representing the
	// current properties of this variable, add the timestamp and state.
	//
        KsCurrPropsHandle hprops(new KsVarCurrProps(vh,
                                                    getTime(),
                                                    getState()),
                                 KsOsNew);
        return hprops;
    } else {
	//
	// Return an unbound handle to signal failure.
	//
        return KsCurrPropsHandle();
    }
} // KssVariable::getCurrProps


// ----------------------------------------------------------------------------
// Set a variable's new value. This just sets the value, but *not* the time-
// stamp nor state. This is just a convenience function in case no handle is
// already at hand. The usual failure reasons thus apply.
//
KS_RESULT
KssVariable::setValue(KsValue *p) 
{
    KsValueHandle h(p, KsOsNew);
    if ( h ) {
        return setValue(h);
    } else {
        return KS_ERR_GENERIC;
    }
} // KssVariable::setValue


// ----------------------------------------------------------------------------
// Set the current properties of a variable. This means, set the value, time-
// stamp and state at oncce.
//
KS_RESULT 
KssVariable::setCurrProps(KsVarCurrProps &props) 
{
    KS_RESULT res = setValue(props.value);
    if ( res == KS_ERR_OK ) {
        setTime(props.time);
        setState(props.state);
    }
    return res;
} // KssVariable::setCurrProps


// ----------------------------------------------------------------------------
// Set the current properties of a variable. This means, set the value, time-
// stamp and state at oncce.
//  
KS_RESULT     
KssVariable::setCurrProps(const KsCurrPropsHandle & hprops)
{
    KsVarCurrProps *pprops = 
        PLT_DYNAMIC_PCAST(KsVarCurrProps, hprops.getPtr());
    if ( pprops ) {
        return setCurrProps(*pprops);
    } else {
        return KS_ERR_BADTYPE;
    }
} // KssVariable::setCurrProps


// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// All the stuff belonging to the class KssLink.
//

// ----------------------------------------------------------------------------
// Return the engineered properties for a link communication object. This is
// per default implemented by retrieving the individual attributes of a link
// and putting this into a link engineered properties object/structure.
//
KsEngPropsHandle
KssLink::getEP() const
{
    KsLinkEngProps * p = new KsLinkEngProps;
    KsEngPropsHandle h(p, KsOsNew);
    if ( p && h ) {
        p->identifier               = getIdentifier();
        p->creation_time            = getCreationTime();
        p->comment                  = getComment();
        p->access_mode              = getAccessMode();
	p->semantic_flags           = getSemanticFlags();
	p->type                     = getType();
	p->association_identifier   = getAssociationIdentifier();
	p->opposite_role_identifier = getOppositeRoleIdentifier();
    }
    return h;
} // KssLink::getEP


// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// All the stuff belonging to the class KssHistory.
//

// ----------------------------------------------------------------------------
// Return the engineered properties for a history communication object. This is
// per default implemented by retrieving the individual attributes of a history
// and putting this into a history engineered properties object/structure.
//
KsEngPropsHandle
KssHistory::getEP() const
{
    KsHistoryEngProps * p = new KsHistoryEngProps;
    KsEngPropsHandle h(p, KsOsNew);
    if ( p && h ) {
        p->identifier               = getIdentifier();
        p->creation_time            = getCreationTime();
        p->comment                  = getComment();
        p->access_mode              = getAccessMode();
	p->semantic_flags           = getSemanticFlags();
	p->type                     = getType();
	p->default_interpolation    = getDefaultInterpolation();
	p->supported_interpolations = getSupportedInterpolations();
	p->type_identifier          = getTypeIdentifier();
    }
    return h;
} // KssHistory::getEP


/* EOF ks/svrobjects.cpp */
