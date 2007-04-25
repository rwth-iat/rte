/* -*-plt-c++-*- */
#ifndef KS_HISTDOMAIN_INCLUDED
#define KS_HISTDOMAIN_INCLUDED
/* $Header: /home/david/cvs/acplt/ks/include/ks/histdomain.h,v 1.10 2007-04-25 12:57:20 martin Exp $ */
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

#include "ks/histparams.h"
#include "ks/svrsimpleobjects.h"


// ----------------------------------------------------------------------------
// The KssHistoryDomain class implements a common history proxy object which
// should be subclassed for the final interface to a real historian.
//
class KssHistoryDomain
    : public KssHistory, 
      public KssSimpleCommObject
{
public:
    KssHistoryDomain(const KsString &id,
		     KS_HIST_TYPE htype,
		     KS_INTERPOLATION_MODE defi,
		     KS_INTERPOLATION_MODE suppi,
                     KsTime ctime = KsTime::now(),
                     KsString comment = KsString(),
		     KsString ti = KsString());

    virtual void getHist(const KsGetHistParams &params,
                         KsGetHistSingleResult &result) = 0;


    // projected properties
    virtual KsString  getIdentifier() const;
    virtual KsTime    getCreationTime() const;
    virtual KsString  getComment() const;
    virtual KS_ACCESS getAccessMode() const;
    virtual KsEngPropsHandle getEP() const;

    virtual KS_HIST_TYPE          getType() const;
    virtual KS_INTERPOLATION_MODE getDefaultInterpolation() const;
    virtual KS_INTERPOLATION_MODE getSupportedInterpolations() const;
    virtual KsString              getTypeIdentifier() const;

    bool isSupported(KS_INTERPOLATION_MODE ipm) const;

    //// KssChildrenService stuff
    virtual KssChildIterator_THISTYPE * newIterator() const;
    virtual KssChildIterator_THISTYPE *
        newMaskedIterator(const KsMask & name_mask,
                          KS_OBJ_TYPE type_mask) const;
    virtual KssCommObjectHandle getChildById(const KsString & id) const;
    virtual KssCommObjectHandle getChildByPath(const KsPath & path) const;

    //// accessor
    size_t size() const;

protected:
#if 0
    class PartInfo {
    public:
        PartInfo(KsString &_id,
                 enum_t _flags,
                 KS_SELECTOR_TYPE _sel_type)
            : id(_id), flags(_flags), sel_type(_sel_type) {}

        unsigned long hash() const { return id.hash(); }
        bool operator == (const PartInfo &other) const
        { return id == other.id; }

        KsString id;
        enum { 
            READABLE = 1, 
            MASKABLE = 2, 
            BOTH = 3,
            ALWAYS_RETURNED = 4 
        };
        enum_t flags;
        KS_SELECTOR_TYPE sel_type;
    };

    bool addPart(KsString id,
                 KS_VAR_TYPE var_type,
                 enum_t flags, 
                 KS_SELECTOR_TYPE sel_type);
#endif

    KS_RESULT parseArgs(const KsGetHistParams &params,
                        KsGetHistResult &result);

    bool addPart(KsString id,
                 KS_VAR_TYPE vtype,
                 KsString comment = KsString(""));

    bool addTrack(KssCommObject * p);

    static KsSelectorHandle getSelector(const KsGetHistParams &params,
                                        KsString id);

private:
    PltHashTable<KsString, KssCommObjectHandle> _tracks;
    KS_HIST_TYPE                                _hist_type;
    KS_INTERPOLATION_MODE                       _default_interpolation;
    KS_INTERPOLATION_MODE                       _supported_interpolations;
    KsString                                    _type_identifier;

    //    PltHashTable<PartInfo> _parts;

    PLT_DECL_RTTI;
};


/////////////////////////////////////////////////////////////////////////////

class KssHistoryPart
    : public KssVariable
{ 
public:
    KssHistoryPart(const KsString &id,
                   KsTime ctime,
                   KS_VAR_TYPE vt,
                   KsString co = KsString());

    // accessors
    // TODO: Change to a typecode that fits more.
    virtual KS_OBJ_TYPE_ENUM typeCode() const { return KS_OT_VARIABLE; }

    // projected properties
    virtual KsString    getIdentifier() const { return identifier; }
    virtual KsTime      getCreationTime() const { return creation_time; }
    virtual KsString    getComment() const { return comment; }
    virtual KS_ACCESS   getAccessMode() const { return KS_AC_NONE; }
    KS_SEMANTIC_FLAGS   getSemanticFlags() const { return 0; }

    virtual KsString    getTechUnit() const { return KsString(); }
    virtual KS_VAR_TYPE getType() const { return var_type; }

    // current properties
    KsValueHandle getValue() const;
    KsCurrPropsHandle getCurrProps() const;
    
    //// modifiers
    //   current properties
    KS_RESULT setValue(const KsValueHandle &) { return KS_ERR_NOACCESS; }

    KS_RESULT setTime(const KsTime &) { return KS_ERR_NOACCESS; }

    KS_RESULT setState(KS_STATE) { return KS_ERR_NOACCESS; }

    KS_RESULT setCurrProps(KsVarCurrProps &) { return KS_ERR_NOACCESS; }
    KS_RESULT setCurrProps(const KsCurrPropsHandle &) { return KS_ERR_NOACCESS; }

protected:
    KsString    identifier; 
    KsTime      creation_time;
    KS_VAR_TYPE var_type;
    KsString    comment;

    PLT_DECL_RTTI;
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

inline
KssHistoryDomain::KssHistoryDomain(const KsString &id,
				   KS_HIST_TYPE htype,
				   KS_INTERPOLATION_MODE defi,
				   KS_INTERPOLATION_MODE suppi,
				   KsTime ctime,
				   KsString comment,
				   KsString ti)
    : KssSimpleCommObject(id, ctime, comment),
      _hist_type(htype), _default_interpolation(defi),
      _supported_interpolations(suppi), _type_identifier(ti)
{}

/////////////////////////////////////////////////////////////////////////////

inline
bool 
KssHistoryDomain::isSupported(KS_INTERPOLATION_MODE ipm) const
{
    return (ipm & getSupportedInterpolations()) ? true : false;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

inline
KssHistoryPart::KssHistoryPart(const KsString &id,
                               KsTime ctime,
                               KS_VAR_TYPE vt,
                               KsString co)
    : identifier(id),
      creation_time(ctime),
      var_type(vt),
      comment(co)
{} // KssHistoryPart::KssHistoryPart

/////////////////////////////////////////////////////////////////////////////

inline
KsValueHandle
KssHistoryPart::getValue() const
{
    return KsValueHandle();
}

/////////////////////////////////////////////////////////////////////////////

inline
KsCurrPropsHandle 
KssHistoryPart::getCurrProps() const
{
    return KsCurrPropsHandle();
}

/////////////////////////////////////////////////////////////////////////////

#endif // KS_HISTDOMAIN_INCLUDED



