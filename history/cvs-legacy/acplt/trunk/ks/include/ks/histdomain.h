/* -*-plt-c++-*- */

#ifndef KS_HISTDOMAIN_INCLUDED
#define KS_HISTDOMAIN_INCLUDED

/*
 * Copyright (c) 1996, 1997, 1998
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

#include "ks/histparams.h"
#include "ks/svrsimpleobjects.h"

/////////////////////////////////////////////////////////////////////////////

class KssHistoryDomain
    : public KssSimpleDomain
{
public:
    KssHistoryDomain(const KsString &id,
                     KsTime ctime = KsTime::now(),
                     KsString comment = KsString());

    virtual void getHist(const KsGetHistParams &params,
                         KsGetHistSingleResult &result) = 0;

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

    static KsSelectorHandle getSelector(const KsGetHistParams &params,
                                        KsString id);

private:
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
    virtual KsString  getIdentifier() const { return identifier; }
    virtual KsTime    getCreationTime() const { return creation_time; }
    virtual KsString  getComment() const { return comment; }
    virtual KS_ACCESS getAccessMode() const { return KS_AC_NONE; }
    virtual KsString getTechUnit() const { return KsString(); }
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
                                   KsTime ctime,
                                   KsString comment)
    : KssSimpleDomain(id, ctime, comment)
{}

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
{}

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



