/* -*-plt-c++-*- */

#ifndef KSC_VARIABLES_INCLUDED 
#define KSC_VARIABLES_INCLUDED

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
//   TODO: Implement this behaviour
// 
class KscTypedVar
: public KscVariable
{
public:
    KscTypedVar(const char *name);

    // check for type errors but dont fail
    // due to it
    bool getEngPropsUpdate();
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
// constructor needs to be defined here to avoid some
// problems with the macros following below
//
inline
KscTypedVar::KscTypedVar(const char *name)
: KscVariable(name),
  fTypeError(false)
{}

//////////////////////////////////////////////////////////////////////
    
class KscIntVar
: public KscTypedVar
{
public:
    KscIntVar(const char *name);

    KscIntVar &operator = (long);
    operator long ();

protected:
    KsIntValue *getCastedValue();
    KS_VAR_TYPE varType() const;

    PLT_DECL_RTTI;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
    
class KscUIntVar
: public KscTypedVar
{
public:
    KscUIntVar(const char *name);

    KscUIntVar &operator = (u_long);
    operator u_long ();

protected:
    KsUIntValue *getCastedValue();
    KS_VAR_TYPE varType() const;

    PLT_DECL_RTTI;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

class KscSingleVar
: public KscTypedVar
{
public:
    KscSingleVar(const char *name);

    KscSingleVar &operator = (float);
    operator float ();

protected:
    KsSingleValue *getCastedValue();
    KS_VAR_TYPE varType() const;

    PLT_DECL_RTTI;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

class KscDoubleVar
: public KscTypedVar
{
public:
    KscDoubleVar(const char *name);

    KscDoubleVar &operator = (double);
    operator double ();

protected:
    KsDoubleValue *getCastedValue();
    KS_VAR_TYPE varType() const;

    PLT_DECL_RTTI;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

class KscStringVar
: public KscTypedVar
{
public:
    KscStringVar(const char *name);

    KscStringVar &operator = (const KsString &);
    operator KsString ();

protected:
    KsStringValue *getCastedValue();
    KS_VAR_TYPE varType() const;

    PLT_DECL_RTTI;
};


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

class KscTimeVar
: public KscTypedVar
{
public:
    KscTimeVar(const char *name);

    KscTimeVar &operator = (const KsTime &);
    operator KsTime ();

protected:
    KsTimeValue *getCastedValue();
    KS_VAR_TYPE varType() const;

    PLT_DECL_RTTI;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

class KscVoidVar
: public KscTypedVar
{
public:
    KscVoidVar(const char *name);

protected:
    KS_VAR_TYPE varType() const;

    PLT_DECL_RTTI;
};


//////////////////////////////////////////////////////////////////////

#if 0

class KscDoubleVecVar
: public KscTypedVar
{
public:
    KscDoubleVecVar(const char *name);

    KscDoubleVecVar &operator = (const KsDoubleVecValue &);
    operator KsDoubleVecValue ();

protected:
    KsDoubleVecValue *getCastedValue();
    KS_VAR_TYPE varType() const;

    PLT_DECL_RTTI;
};

#endif

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// macro to implement the getCastedValue() function
//
#define PLT_IMPL_GET_CASTED_VALUE(forClass, castTo)                  \
    castTo *                                                         \
    forClass::getCastedValue() {                                     \
        if(curr_props.value) {                                       \
            if(curr_props.value->xdrTypeCode() == varType()) {       \
              return (castTo *)(curr_props.value.getPtr());          \
            } else {                                                 \
                fTypeError = true;                                   \
            }                                                        \
        }                                                            \
        castTo *val = new castTo;                                    \
        curr_props.value =                                           \
            KsPtrHandle<KsValue>(val, KsOsNew);                      \
        return val;                                                  \
    }                                                                \
typedef void ks_dummy_typedef

//////////////////////////////////////////////////////////////////////
// some macros to declare and implement the vector types
//
// note: first letter of elemtype should be uppercase
//       (Int, Double, etc.) 
//
#define PLT_DECL_VEC_VAR(elemtype, typecode)                         \
    class Ksc##elemtype##VecVar                                      \
    : public KscTypedVar                                             \
    {                                                                \
    public:                                                          \
        Ksc##elemtype##VecVar(const char *name)                      \
            : KscTypedVar(name) {}                                   \
        Ksc##elemtype##VecVar                                        \
        &operator=(const Ks##elemtype##VecValue &);                  \
        operator Ks##elemtype##VecValue();                           \
    protected:                                                       \
        Ks##elemtype##VecValue *getCastedValue();                    \
        KS_VAR_TYPE varType() const {                                \
          return typecode; }                                         \
        PLT_DECL_RTTI;                                               \
    }


#define PLT_IMPL_VEC_VAR(elemtype)                                   \
    Ks##elemtype##VecValue *                                         \
    Ksc##elemtype##VecVar::getCastedValue() {                        \
        if(curr_props.value) {                                       \
            if(curr_props.value->xdrTypeCode() == varType()) {       \
              return (Ks##elemtype##VecValue *)                      \
                  (curr_props.value.getPtr());                       \
            } else {                                                 \
                fTypeError = true;                                   \
                return 0;                                            \
            }                                                        \
        } else {                                                     \
            Ks##elemtype##VecValue *val =                            \
                new Ks##elemtype##VecValue(0);                       \
            curr_props.value =                                       \
                KsPtrHandle<KsValue>(val, KsOsNew);                  \
            return val;                                              \
        }                                                            \
    }                                                                \
/************************************/                               \
    Ksc##elemtype##VecVar &                                          \
    Ksc##elemtype##VecVar::operator =                                \
    (const Ks##elemtype##VecValue &newVal)                           \
    {                                                                \
        Ks##elemtype##VecValue *val = getCastedValue();              \
        if(isValid()) {                                              \
            *val = newVal;                                           \
            fDirty = true;                                           \
        }                                                            \
        return *this;                                                \
    }                                                                \
/************************************/                               \
    Ksc##elemtype##VecVar::operator Ks##elemtype##VecValue()         \
    {                                                                \
        Ks##elemtype##VecValue *val = getCastedValue();              \
        if(isValid()) {                                              \
            return *val;                                             \
        }                                                            \
        return Ks##elemtype##VecValue(0);                            \
    }                                                                \
    typedef void ks_dummy_typedef


//////////////////////////////////////////////////////////////////////
// declare vector types
//
PLT_DECL_VEC_VAR(Byte, KS_VT_BYTE_VEC);
PLT_DECL_VEC_VAR(Int, KS_VT_INT_VEC);
PLT_DECL_VEC_VAR(UInt, KS_VT_UINT_VEC);
PLT_DECL_VEC_VAR(Single, KS_VT_SINGLE_VEC);
PLT_DECL_VEC_VAR(Double, KS_VT_DOUBLE_VEC);
PLT_DECL_VEC_VAR(String, KS_VT_STRING_VEC);
PLT_DECL_VEC_VAR(Time, KS_VT_TIME_VEC);


//////////////////////////////////////////////////////////////////////
// Inline Implementation
//////////////////////////////////////////////////////////////////////

inline
bool
KscTypedVar::isValid()
{
    return !fTypeError 
        && curr_props.value;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

inline
KS_VAR_TYPE 
KscIntVar::varType() const
{
    return KS_VT_INT;
}

//////////////////////////////////////////////////////////////////////

inline
KscIntVar::KscIntVar(const char *name)
: KscTypedVar(name)
{}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

inline
KS_VAR_TYPE 
KscUIntVar::varType() const
{
    return KS_VT_UINT;
}

//////////////////////////////////////////////////////////////////////

inline
KscUIntVar::KscUIntVar(const char *name)
: KscTypedVar(name)
{}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

inline
KscSingleVar::KscSingleVar(const char *name)
: KscTypedVar(name)
{}

//////////////////////////////////////////////////////////////////////

inline
KS_VAR_TYPE
KscSingleVar::varType() const
{
    return KS_VT_SINGLE;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

inline
KscDoubleVar::KscDoubleVar(const char *name)
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
//////////////////////////////////////////////////////////////////////

inline
KscStringVar::KscStringVar(const char *name)
: KscTypedVar(name)
{}

//////////////////////////////////////////////////////////////////////

inline
KS_VAR_TYPE
KscStringVar::varType() const
{
    return KS_VT_STRING;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

inline
KscTimeVar::KscTimeVar(const char *name)
: KscTypedVar(name)
{}

//////////////////////////////////////////////////////////////////////

inline
KS_VAR_TYPE
KscTimeVar::varType() const
{
    return KS_VT_TIME;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

inline
KscVoidVar::KscVoidVar(const char *name)
: KscTypedVar(name)
{}

//////////////////////////////////////////////////////////////////////

inline
KS_VAR_TYPE
KscVoidVar::varType() const
{
    return KS_VT_VOID;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

#if 0

inline
KscDoubleVecVar::KscDoubleVecVar(const char *name)
: KscTypedVar(name)
{}

//////////////////////////////////////////////////////////////////////

inline
KS_VAR_TYPE
KscDoubleVecVar::varType() const
{
    return KS_VT_DOUBLE_VEC;
}

#endif

//////////////////////////////////////////////////////////////////////


#endif

//////////////////////////////////////////////////////////////////////
// EOF variables.h
//////////////////////////////////////////////////////////////////////






