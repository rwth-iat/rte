/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/include/ks/history.h,v 1.11 2007-04-25 12:57:20 martin Exp $ */
#ifndef KS_HISTORY_INCLUDED
#define KS_HISTORY_INCLUDED
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

#include "ks/client.h"
#include "ks/commobject.h"
#include "ks/histparams.h"

/////////////////////////////////////////////////////////////////////////////

class KscHistory
    : public KscCommObject
{
public:
    KscHistory(const char *object_path);

    KS_OBJ_TYPE typeCode() const { return KS_OT_HISTORY; }

    bool setEngProps(KsEngPropsHandle) { return true; }
    const KsEngProps_THISTYPE *getEngProps() const;
    bool getEngPropsUpdate();

    // Add or update a selector
    bool setSelector(KsString id, KsSelectorHandle hsel);
    // Implicit handle creation with usual caveats
    bool setSelector(KsString id, KsSelector *psel);
    // Remove a selector
    bool removeSelector(KsString id);
    // Add a string selector(convenience function)
    bool setStringSelector(KsString id, KsString mask);
    // Add a none selector
    bool setNoneSelector(KsString id);
    // Get a selector if exists
    KsSelectorHandle getSelector(KsString id) const;

    // Read max entries at most
    void setMaxEntries(u_long max = ULONG_MAX);

    // Read parts of a history
    bool getParts(KsList<KsEngPropsHandle> &parts);

    // Read history with currently set parameters,
    // result is not stored in object
    bool getHist(KsGetHistResult &result);

    // Read history with current parameters and
    // store result
    bool updateHist();
    // Access selectors if possible
    KS_RESULT getSelectorValue(KsString selector, KsValueHandle &hval);
    KS_RESULT getTimeValue(KsString selector, KsTimeVecValue &val);
    KS_RESULT getStringValue(KsString selector, KsStringVecValue &val);
    KS_RESULT getDoubleValue(KsString selector, KsDoubleVecValue &val);
    KS_RESULT getUIntValue(KsString selector, KsUIntVecValue &val);
    KS_RESULT getIntValue(KsString selector, KsIntVecValue &val);
    KS_RESULT getStateValue(KsString selector, KsStateVecValue &val);

protected:
    u_long                 max_entries;

    KsList<KsGetHistItem>  selectors;

    KsGetHistSingleResult  gh_result;

    KsEngPropsHandle      hpp;
};

/////////////////////////////////////////////////////////////////////////////
// INLINE IMPLEMENTATION
/////////////////////////////////////////////////////////////////////////////

inline
KscHistory::KscHistory(const char *object_path)
    : KscCommObject(object_path),
      max_entries(ULONG_MAX)
{}

/////////////////////////////////////////////////////////////////////////////

inline bool 
KscHistory::setSelector(KsString id, KsSelectorHandle hsel)
{
    if( hsel ) {
        KsGetHistItem item(id,hsel);
        selectors.remove(item); // remove any item with same id
        return selectors.addLast(item);
    } else {
        return false;
    }
}

/////////////////////////////////////////////////////////////////////////////

inline bool 
KscHistory::setSelector(KsString id, KsSelector *psel)
{
    KsSelectorHandle hsel(psel, KsOsNew);

    return setSelector(id, hsel);
}

/////////////////////////////////////////////////////////////////////////////

inline 
bool
KscHistory::removeSelector(KsString id)
{
    return selectors.remove(KsGetHistItem(id, KsSelectorHandle()));
}

/////////////////////////////////////////////////////////////////////////////

inline 
bool
KscHistory::setNoneSelector(KsString id)
{
    return setSelector(id, new KsNoneSel());
}

/////////////////////////////////////////////////////////////////////////////

inline
void
KscHistory::setMaxEntries(u_long max)
{
    max_entries = max;
}


#endif // KS_HISTORY_INCLUDED
// End of history.h
