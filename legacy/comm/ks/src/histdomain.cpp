/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/src/histdomain.cpp,v 1.10 2007-04-25 12:57:20 martin Exp $ */
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

/* Author : Markus Juergens <markusj@plt.rwth-aachen.de> */

/////////////////////////////////////////////////////////////////////////////

#include "ks/histdomain.h"

/////////////////////////////////////////////////////////////////////////////

PLT_IMPL_RTTI1(KssHistoryDomain, KssSimpleDomain);
PLT_IMPL_RTTI1(KssHistoryPart, KssVariable);

/////////////////////////////////////////////////////////////////////////////




// ----------------------------------------------------------------------------
// Add another track to this history, so clients can browse and find out what
// tracks are provided by a particular history object.
//
bool
KssHistoryDomain::addTrack(KssCommObject * p)
{
    KssCommObjectHandle h(p, KsOsNew);

    if ( !h ) {
	return false;
    }
    return _tracks.add(h->getIdentifier(), h);
} // KssHistoryDomain::addTrack


// ----------------------------------------------------------------------------
// Returns the number of tracks provided by a history.
//
size_t
KssHistoryDomain::size() const
{
    return _tracks.size();
} // KssHistoryDomain::size


// ----------------------------------------------------------------------------
//
KssChildIterator_THISTYPE *
KssHistoryDomain::newIterator() const
{
    return new KssSimpleDomainIterator(_tracks);
} // KssHistoryDomain::newIterator


// ----------------------------------------------------------------------------
//
KssChildIterator_THISTYPE *
KssHistoryDomain::newMaskedIterator(const KsMask & name_mask,
				    KS_OBJ_TYPE type_mask) const
{
    PltPtrHandle<KssChildIterator>
	h(PLT_RETTYPE_CAST((KssChildIterator *)) newIterator(), PltOsNew); 
    if ( h ) {
	return new KssMaskedDomainIterator(h, name_mask, type_mask);
    }
    return 0;
} // KssHistoryDomain::newMaskedIterator


// ----------------------------------------------------------------------------
// Provide access to the various engineered properties of a history object.
// Argh, this is dump coding stuff...
//
KsString
KssHistoryDomain::getIdentifier() const
{
    return KssSimpleCommObject::getIdentifier();
} // KssHistoryDomain::getIdentifier


KsTime
KssHistoryDomain::getCreationTime() const
{
    return KssSimpleCommObject::getCreationTime();
} // KssHistoryDomain::getCreationTime


KsString
KssHistoryDomain::getComment() const
{
    return KssSimpleCommObject::getComment();
} // KssHistoryDomain::getComment


KS_ACCESS
KssHistoryDomain::getAccessMode() const
{
    return KssHistory::getAccessMode();
} // KssHistoryDomain::getAccessMode


KsEngPropsHandle
KssHistoryDomain::getEP() const
{
    return KssHistory::getEP();
} // KssSimpleCommObject::getEP


KS_HIST_TYPE
KssHistoryDomain::getType() const
{
    return _hist_type;
} // KssHistoryDomain::getType


KS_INTERPOLATION_MODE
KssHistoryDomain::getDefaultInterpolation() const
{
    return _default_interpolation;
} // KssHistoryDomain::getDefaultInterpolation


KS_INTERPOLATION_MODE
KssHistoryDomain::getSupportedInterpolations() const
{
    return _supported_interpolations;
} // KssHistoryDomain::getSupportedInterpolations


KsString
KssHistoryDomain::getTypeIdentifier() const
{
    return _type_identifier;
} // KssHistoryDomain::getTypeIdentifier


// ----------------------------------------------------------------------------
// Phew, it's now getting somehow interesting again...
//
KssCommObjectHandle
KssHistoryDomain::getChildById(const KsString & id) const
{
    KssCommObjectHandle h;

    if ( _tracks.query(id, h) ) {
	return h;
    }
    return KssCommObjectHandle(); // return unbound handle
} // KssHistoryDomain::getChildById


KssCommObjectHandle
KssHistoryDomain::getChildByPath(const KsPath & path) const
{
    PLT_PRECONDITION(path.isValid());

    //
    // try to look up the child indicated by the first part of the given
    // identifier and path.
    //
    KssCommObjectHandle hc(getChildById(path.getHead()));
    if ( hc ) {
	//
        // Good. There is such a child, so the first component in the
	// path was okay.
	//
        if ( path.isSingle() ) {
	    //
            // We were asked about this child. So return it.
	    //
            return hc;
        }
	//
	// The caller wants a grandchild. But histories have no grand-
	// children, so the path is definitly wrong. Just fall through
	// at this point and trigger an error.
	//
    }
    //
    // We don't know of such a child...
    //
    return KssCommObjectHandle();
} // KssHistoryDomain::getChildByPath




// ----------------------------------------------------------------------------
bool
KssHistoryDomain::addPart(KsString id,
                          KS_VAR_TYPE vtype,
                          KsString comment)
{
    KssHistoryPart *p =
        new KssHistoryPart(id, getCreationTime(),
                           vtype, comment);

    return addTrack(p);
}

/////////////////////////////////////////////////////////////////////////////

#if 0
bool 
KssHistoryDomain::addPart(KsString id, 
                          enum_t flags, 
                          KS_SELECTOR_TYPE sel_type,
                          KS_VAR_TYPE var_type)
{
    PartInfo inf(id, flags, sel_type);

    if( _parts.add( inf ) ) {
        KsString comment;
        if( flags & BOTH ) {
            comment = "Maskable and readable";
        } else if( flags & MASKABLE ) {
            comment = "Maskable";
        } else if( flags & READABLE ) {
            comment = "Readable";
        } else {
            comment = "No access";
        }

        KssHistoryPart *p = 
            new KssHistoryPart(id, getCreationTime(),
                               var_type, comment);

        return addCommObject(p);
    }

    return false;
}

/////////////////////////////////////////////////////////////////////////////

KS_RESULT 
KssHistoryDomain::parseArgs(const KsGetHistParams &params,
                            KsGetHistResult &result)
{
}
#endif

/////////////////////////////////////////////////////////////////////////////

KsSelectorHandle 
KssHistoryDomain::getSelector(const KsGetHistParams &params,
                              KsString id)
{
    PltArrayIterator<KsGetHistItem> it(params.items);

    for( ; it; ++it) {
        if( it->part_id == id ) {
            return it->sel;
        }
    }

    return KsSelectorHandle();
}

/////////////////////////////////////////////////////////////////////////////
// EOF histdomain.cpp
/////////////////////////////////////////////////////////////////////////////






