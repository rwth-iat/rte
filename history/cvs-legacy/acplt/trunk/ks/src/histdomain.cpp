/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/src/histdomain.cpp,v 1.4 1999-04-22 15:35:54 harald Exp $ */
/*
 * Copyright (c) 1996, 1997, 1998, 1999
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


KsProjPropsHandle
KssHistoryDomain::getPP() const
{
    return KssHistory::getPP();
} // KssSimpleCommObject::getPP


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






