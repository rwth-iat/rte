/* -*-plt-c++-*- */

#ifndef KSC_VARIABLES_INCLUDED 
#define KSC_VARIABLES_INCLUDED

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

#include "plt/debug.h"
#include "plt/rtti.h"

#include "ks/ks.h"
#include "ks/commobject.h"

//////////////////////////////////////////////////////////////////////
// KscTypedVar
//   Base class for communication objects with known type.
//   Derived classes will always have a value in their current 
//   properties with correct type. Therefore data may be lost if
//   incoming data has a false type. In this case the data is ignored
//   and the read operation is considered to be failed.
// 
class KscTypedVar
: public KscVariable
{
public:
    KscTypedVar(const KscAbsPath &name);

    // may fail due to type error
    bool getUpdate();
    // may fail due to type error
    bool setCurrProps(KsVarCurrProps &cp);

    bool typeErrorOccured() const;

protected:
    virtual KS_VAR_TYPE varType() const = 0;

    bool checkType();
    bool isValid();

    bool fTypeError;
};

//////////////////////////////////////////////////////////////////////
    
class KscIntVar
: public KscTypedVar
{
public:
    KscIntVar(const KscAbsPath &name);

    KscIntVar &operator = (long);
    operator long ();

protected:
    KsIntValue *getCastedValue();
    KS_VAR_TYPE varType() const;

    PLT_DECL_RTTI;
};

//////////////////////////////////////////////////////////////////////

class KscDoubleVar
: public KscTypedVar
{
public:
    KscDoubleVar(const KscAbsPath &name);

    KscDoubleVar &operator = (double);
    operator double ();

protected:
    KsDoubleValue *getCastedValue();
    KS_VAR_TYPE varType() const;

    PLT_DECL_RTTI;
};

//////////////////////////////////////////////////////////////////////

class KscDoubleVecVar
: public KscTypedVar
{
public:
    KscDoubleVecVar(const KscAbsPath &name);

    KscDoubleVecVar &operator = (const KsDoubleVecValue &);
    operator KsDoubleVecValue ();

protected:
    KsDoubleVecValue *getCastedValue();
    KS_VAR_TYPE varType() const;

    PLT_DECL_RTTI;
};

//////////////////////////////////////////////////////////////////////
// Inline Implementation
//////////////////////////////////////////////////////////////////////

inline
KscTypedVar::KscTypedVar(const KscAbsPath &name)
: KscVariable(name),
  fTypeError(false)
{}

//////////////////////////////////////////////////////////////////////

inline
bool
KscTypedVar::isValid()
{
    return !fTypeError 
        && curr_props.value;
}

//////////////////////////////////////////////////////////////////////

inline
KS_VAR_TYPE 
KscIntVar::varType() const
{
    return KS_VT_INT;
}

//////////////////////////////////////////////////////////////////////

inline
KscIntVar::KscIntVar(const KscAbsPath &name)
: KscTypedVar(name)
{}

//////////////////////////////////////////////////////////////////////

inline
KscDoubleVar::KscDoubleVar(const KscAbsPath &name)
: KscTypedVar(name)
{}

//////////////////////////////////////////////////////////////////////

inline
KS_VAR_TYPE
KscDoubleVar::varType() const
{
    return KS_VT_DOUBLE;
}

//////////////////////////////////////////////////////////////////////

inline
KscDoubleVecVar::KscDoubleVecVar(const KscAbsPath &name)
: KscTypedVar(name)
{}

//////////////////////////////////////////////////////////////////////

inline
KS_VAR_TYPE
KscDoubleVecVar::varType() const
{
    return KS_VT_DOUBLE_VEC;
}

//////////////////////////////////////////////////////////////////////


#endif

//////////////////////////////////////////////////////////////////////
// EOF variables.h
//////////////////////////////////////////////////////////////////////






