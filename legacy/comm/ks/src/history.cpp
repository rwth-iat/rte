/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/src/history.cpp,v 1.11 2007-04-25 12:57:20 martin Exp $ */
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


#include "ks/history.h"


/////////////////////////////////////////////////////////////////////////////

const KsEngProps_THISTYPE *
KscHistory::getEngProps() const
{
    // TODO: implementation
    return 0;
}

/////////////////////////////////////////////////////////////////////////////

bool 
KscHistory::getEngPropsUpdate()
{
    // TODO: implementation
    return false;
}

/////////////////////////////////////////////////////////////////////////////

bool 
KscHistory::setStringSelector(KsString id,
                         KsString mask)
{
    KsSelectorHandle hsel(new KsStringSel(mask), KsOsNew);
    return setSelector(id, hsel);
}

/////////////////////////////////////////////////////////////////////////////

bool
KscHistory::getParts(KsList<KsEngPropsHandle> &parts)
{
    if( !hasValidPath() ) return false;

    KscServerBase *server = getServer();
    if( !server ) {
        _last_result = KS_ERR_GENERIC;
        return false;
    }

    // Prepare the service parameters
    //
    KsGetEPParams params;
    KsGetEPResult result;

    params.path      = getPathAndName();
    params.type_mask = KS_OT_VARIABLE;
    params.name_mask = KsString("*");
    params.scope_flags = KS_EPF_CHILDREN; // FIXME!!!

    // Request service
    //
    bool ok = server->getEP(av_module, params, result);
    
    if( ok ) {
        if( result.result == KS_ERR_OK ) {
            // Request succeeded, now copy data
            //
            PltListIterator<KsEngPropsHandle> it(result.items);
            bool copyok = true;
            while( it ) {
                copyok = copyok && parts.addLast(*it);
                ++it;
            }
            if( copyok ) {
                _last_result = KS_ERR_OK;
            } else {
                _last_result = KS_ERR_GENERIC;
            }
        } else {
            // Error within request
            _last_result = result.result;
        }
    } else {
        // Communication failed
        _last_result = server->getLastResult();
    }

    return _last_result == KS_ERR_OK;
}

/////////////////////////////////////////////////////////////////////////////

bool 
KscHistory::getHist(KsGetHistResult &result)
{
    if( !hasValidPath() ) return false;

    KscServerBase *server = getServer();
    if( !server ) {
        _last_result = KS_ERR_GENERIC;
        return false;
    }

    // Build parameters
    //
    const size_t sz = selectors.size(); 
    KsGetHistParams params(1, sz);
    if( params.paths.size() != 1 
        || params.items.size() != sz ) {
        _last_result = KS_ERR_GENERIC;
        return false;
    }

    params.paths[0] = getPathAndName();

    params.max_entries = max_entries;

    PltListIterator<KsGetHistItem> it(selectors);
    size_t i = 0;
    while( it ) {
        params.items[i++] = *it;
        ++it;
    }

    PLT_ASSERT( i == sz );

    // Request service
    //
    bool ok = server->requestByOpcode(KS_GETHIST, av_module,
				      params, result);

    if( ok ) {
        _last_result = result.result;
    } else {
        _last_result = server->getLastResult();
    }

    return _last_result == KS_ERR_OK;
}

/////////////////////////////////////////////////////////////////////////////

bool 
KscHistory::updateHist()
{
    KsGetHistResult result;

    if( getHist( result ) ) {
        if( result.result == KS_ERR_OK ) {
            if( result.replies.size() == 1 ) {
                gh_result = result.replies[0];
                _last_result = gh_result.result;
                return _last_result == KS_ERR_OK; 
            } else {
                _last_result = KS_ERR_GENERIC;
            }
        }
    } else {
        gh_result.result = getLastResult();
    }

    return false;
}

/////////////////////////////////////////////////////////////////////////////

KS_RESULT 
KscHistory::getSelectorValue(KsString part, 
                             KsValueHandle &hval)
{
    if( gh_result.result == KS_ERR_OK ) {
        // Search position of part
        //
        size_t i = 0;
        PltListIterator<KsGetHistItem> it(selectors);
        while( it ) {
            if( it->part_id == part ) break;
            ++it;
            ++i;
        }
        if( i >= selectors.size() || i >= gh_result.items.size() ) {
            // Selector does not exist
            return KS_ERR_BADPATH;
        }
        if( gh_result.items[i].result == KS_ERR_OK ) {
            hval = gh_result.items[i].value;
        }
        return gh_result.items[i].result;
    } else {
        return gh_result.result;
    }
}

/////////////////////////////////////////////////////////////////////////////

KS_RESULT
KscHistory::getTimeValue(KsString id, KsTimeVecValue &val)
{
    KsValueHandle hval;
    KS_RESULT res = getSelectorValue(id, hval);
 
    if( res == KS_ERR_OK ) {
        if( hval ) {
            KsTimeVecValue *time_val = 
                PLT_DYNAMIC_PCAST(KsTimeVecValue, hval.getPtr());
            if( time_val ) {
                val = *time_val;
                return KS_ERR_OK;
            } else {
                return KS_ERR_TYPEMISMATCH;
            }
        } else {
            return  KS_ERR_GENERIC;
        }
    } else {
        return res;
    }
}

/////////////////////////////////////////////////////////////////////////////

KS_RESULT
KscHistory::getStringValue(KsString id, KsStringVecValue &val)
{
    KsValueHandle hval;
    KS_RESULT res = getSelectorValue(id, hval);
 
    if( res == KS_ERR_OK ) {
        if( hval ) {
            KsStringVecValue *string_val = 
                PLT_DYNAMIC_PCAST(KsStringVecValue, hval.getPtr());
            if( string_val ) {
                val = *string_val;
                return KS_ERR_OK;
            } else {
                return KS_ERR_TYPEMISMATCH;
            }
        } else {
            return  KS_ERR_GENERIC;
        }
    } else {
        return res;
    }
}

/////////////////////////////////////////////////////////////////////////////

KS_RESULT
KscHistory::getDoubleValue(KsString id, KsDoubleVecValue &val)
{
    KsValueHandle hval;
    KS_RESULT res = getSelectorValue(id, hval);
 
    if( res == KS_ERR_OK ) {
        if( hval ) {
            KsDoubleVecValue *dbl_val = 
                PLT_DYNAMIC_PCAST(KsDoubleVecValue, hval.getPtr());
            if( dbl_val ) {
                val = *dbl_val;
                return KS_ERR_OK;
            } else {
                return KS_ERR_TYPEMISMATCH;
            }
        } else {
            return  KS_ERR_GENERIC;
        }
    } else {
        return res;
    }
}

/////////////////////////////////////////////////////////////////////////////

KS_RESULT
KscHistory::getUIntValue(KsString id, KsUIntVecValue &val)
{
    KsValueHandle hval;
    KS_RESULT res = getSelectorValue(id, hval);
 
    if( res == KS_ERR_OK ) {
        if( hval ) {
            KsUIntVecValue *uint_val = 
                PLT_DYNAMIC_PCAST(KsUIntVecValue, hval.getPtr());
            if( uint_val ) {
                val = *uint_val;
                return KS_ERR_OK;
            } else {
                return KS_ERR_TYPEMISMATCH;
            }
        } else {
            return  KS_ERR_GENERIC;
        }
    } else {
        return res;
    }
}

/////////////////////////////////////////////////////////////////////////////

KS_RESULT
KscHistory::getIntValue(KsString id, KsIntVecValue &val)
{
    KsValueHandle hval;
    KS_RESULT res = getSelectorValue(id, hval);
    
    if( res == KS_ERR_OK ) {
        if( hval ) {
            KsIntVecValue *uint_val = 
                PLT_DYNAMIC_PCAST(KsIntVecValue, hval.getPtr());
            if( uint_val ) {
                val = *uint_val;
                return KS_ERR_OK;
            } else {
                return KS_ERR_TYPEMISMATCH;
            }
        } else {
            return  KS_ERR_GENERIC;
        }
    } else {
        return res;
    }
}

/////////////////////////////////////////////////////////////////////////////

KS_RESULT
KscHistory::getStateValue(KsString id, KsStateVecValue &val)
{
    KsValueHandle hval;
    KS_RESULT res = getSelectorValue(id, hval);
    
    if( res == KS_ERR_OK ) {
        if( hval ) {
            KsStateVecValue *uint_val = 
                PLT_DYNAMIC_PCAST(KsStateVecValue, hval.getPtr());
            if( uint_val ) {
                val = *uint_val;
                return KS_ERR_OK;
            } else {
                return KS_ERR_TYPEMISMATCH;
            }
        } else {
            return  KS_ERR_GENERIC;
        }
    } else {
        return res;
    }
}

/////////////////////////////////////////////////////////////////////////////

KsSelectorHandle 
KscHistory::getSelector(KsString id) const
{
    PltListIterator<KsGetHistItem> it(selectors);

    while( it ) {
        if( it->part_id == id ) {
            return it->sel;
        }
        ++it;
    }

    return KsSelectorHandle();
}

/////////////////////////////////////////////////////////////////////////////
// EOF history.cpp
/////////////////////////////////////////////////////////////////////////////






