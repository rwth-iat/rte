/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/src/svrobjects.cpp,v 1.1 1997-03-23 14:35:18 martin Exp $ */
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
KssDomain::child(const KsString &id) const
{
    //
    // Linear search. This may be VERY inefficient! But then
    // you should implement your own version of this method.
    //
    KssCommObjectHandle res;
    KsString name_mask("*");
    KS_OBJ_TYPE type_mask = KS_OT_ANY;

    KssDomainIterator * pit = newIterator(name_mask, type_mask);
    if (pit) {
        for (KssDomainIterator &it(*pit); it; ++it) {
            if ((*it)->getIdentifier() == id) {
                res = *it;
            }
        }
    }
    return res;
}

//////////////////////////////////////////////////////////////////////
// KssVariable
//////////////////////////////////////////////////////////////////////

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
/* EOF ks/svrobjects.cpp */
