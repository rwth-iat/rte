/* -*-plt-c++-*- */

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

/* Author: Markus Juergens <markusj@plt.rwth-aachen.de> */

//////////////////////////////////////////////////////////////////////

#include "ks/variables.h"

//////////////////////////////////////////////////////////////////////
// RTTI implementation
//   abstract class KscTypedVar is skipped
// 
PLT_IMPL_RTTI1(KscIntVar,KscVariable);
PLT_IMPL_RTTI1(KscDoubleVar,KscVariable);
PLT_IMPL_RTTI1(KscStringVar,KscVariable);
PLT_IMPL_RTTI1(KscTimeVar,KscVariable);
PLT_IMPL_RTTI1(KscDoubleVecVar,KscVariable);

//////////////////////////////////////////////////////////////////////
// class KscTypedVar
//////////////////////////////////////////////////////////////////////

bool
KscTypedVar::typeErrorOccured() const
{
    return fTypeError;
}

//////////////////////////////////////////////////////////////////////

bool
KscTypedVar::checkType()
{
    fTypeError =
        (proj_props.type != varType());

    if(curr_props.value) {
        fTypeError |= 
            curr_props.value->xdrTypeCode() != varType();
    }

    return !fTypeError;
}

//////////////////////////////////////////////////////////////////////

bool
KscTypedVar::getProjPropsUpdate()
{
    bool ok = KscVariable::getProjPropsUpdate();

    if(ok) {
        fTypeError = 
            proj_props.type != varType();
        if(fTypeError) {
            PLT_DMSG("WARNING : projected type does not match desired type" << endl);
        }
    }

    return ok;
}

//////////////////////////////////////////////////////////////////////

bool
KscTypedVar::getUpdate() 
{
    bool ok = KscVariable::getUpdate();

    if(ok) {
        ok |= checkType();
    }

    return ok;
}

//////////////////////////////////////////////////////////////////////

bool
KscTypedVar::setCurrProps(KsVarCurrProps &cp) 
{
    if(!cp.value || 
       cp.value->xdrTypeCode() == varType()) {
        return KscVariable::setCurrProps(cp);
    }

    fTypeError = true;

    return false;
}


//////////////////////////////////////////////////////////////////////

KsIntValue *
KscIntVar::getCastedValue() 
{
    if(curr_props.value) {
        if(curr_props.value->xdrTypeCode() == varType()) {
            return (KsIntValue *)(curr_props.value.getPtr());
        }
        else {
            fTypeError = true;
            return 0;
        }
    }
    else {
        KsIntValue *val = new KsIntValue(0);
        curr_props.value = 
            KsPtrHandle<KsValue>(val, KsOsNew);
        return val;
    }
}

//////////////////////////////////////////////////////////////////////

KscIntVar &
KscIntVar::operator = (long newVal)
{
    KsIntValue *val = getCastedValue(); 

    if(isValid()) {
        val->setInt(newVal);
        fDirty = true;
    }
    
        return *this;
}
        
//////////////////////////////////////////////////////////////////////

KscIntVar::operator long()
{
    KsIntValue *val = getCastedValue();

    if(isValid()) {
        return *val;
    }

    return 0;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

KsDoubleValue *
KscDoubleVar::getCastedValue() 
{
    if(curr_props.value) {
        if(curr_props.value->xdrTypeCode() == varType()) {
            return (KsDoubleValue *)(curr_props.value.getPtr());
        }
        else {
            fTypeError = true;
            return 0;
        }
    }
    else {
        KsDoubleValue *val = new KsDoubleValue(0);
        curr_props.value = 
            KsPtrHandle<KsValue>(val, KsOsNew);
        return val;
    }
}

//////////////////////////////////////////////////////////////////////

KscDoubleVar &
KscDoubleVar::operator = (double newVal)
{
    KsDoubleValue *val = getCastedValue(); 

    if(isValid()) {
        val->setDouble(newVal);

        fDirty = true;
    }
    
    return *this;
}
        
//////////////////////////////////////////////////////////////////////

KscDoubleVar::operator double()
{
    KsDoubleValue *val = getCastedValue();

    if(isValid()) {
        return *val;
    }

    return 0;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

KsStringValue *
KscStringVar::getCastedValue() 
{
    if(curr_props.value) {
        if(curr_props.value->xdrTypeCode() == varType()) {
            return (KsStringValue *)(curr_props.value.getPtr());
        }
        else {
            fTypeError = true;
            return 0;
        }
    }
    else {
        KsStringValue *val = new KsStringValue();
        curr_props.value = 
            KsPtrHandle<KsValue>(val, KsOsNew);
        return val;
    }
}

//////////////////////////////////////////////////////////////////////

KscStringVar &
KscStringVar::operator = (const KsString &newVal)
{
    KsStringValue *val = getCastedValue(); 

    if(isValid()) {
        *val = newVal;

        fDirty = true;
    }
    
    return *this;
}
        
//////////////////////////////////////////////////////////////////////

KscStringVar::operator KsString()
{
    KsStringValue *val = getCastedValue();

    if(isValid()) {
        return *val;
    }

    return 0;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

KsTimeValue *
KscTimeVar::getCastedValue() 
{
    if(curr_props.value) {
        if(curr_props.value->xdrTypeCode() == varType()) {
            return (KsTimeValue *)(curr_props.value.getPtr());
        }
        else {
            fTypeError = true;
            return 0;
        }
    }
    else {
        KsTimeValue *val = new KsTimeValue();
        curr_props.value = 
            KsPtrHandle<KsValue>(val, KsOsNew);
        return val;
    }
}

//////////////////////////////////////////////////////////////////////

KscTimeVar &
KscTimeVar::operator = (const KsTime &newVal)
{
    KsTimeValue *val = getCastedValue(); 

    if(isValid()) {
        *val = newVal;

        fDirty = true;
    }
    
    return *this;
}
        
//////////////////////////////////////////////////////////////////////

KscTimeVar::operator KsTime()
{
    KsTimeValue *val = getCastedValue();

    if(isValid()) {
        return *val;
    }

    return 0;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

KsDoubleVecValue * 
KscDoubleVecVar::getCastedValue() 
{
    if(curr_props.value) {
        if(curr_props.value->xdrTypeCode() == varType()) {
            return (KsDoubleVecValue *)(curr_props.value.getPtr());
        }
        else {
            fTypeError = true;
            return 0;
        }
    }
    else {
        KsDoubleVecValue *val = new KsDoubleVecValue(0);
        curr_props.value = 
            KsPtrHandle<KsValue>(val, KsOsNew);
        return val;
    }
}

//////////////////////////////////////////////////////////////////////

KscDoubleVecVar &
KscDoubleVecVar::operator = (const KsDoubleVecValue &newVal)
{
    KsDoubleVecValue *val = getCastedValue(); 

    if(isValid()) {
        *val = newVal;
        fDirty = true;
    }
    
    return *this;
}
        
//////////////////////////////////////////////////////////////////////

KscDoubleVecVar::operator KsDoubleVecValue()
{
    KsDoubleVecValue *val = getCastedValue();

    if(isValid()) {
        return *val;
    }

    return KsDoubleVecValue(0);
}



//////////////////////////////////////////////////////////////////////
// EOF variables.cpp
//////////////////////////////////////////////////////////////////////

    


