/* -*-plt-c++-*- */

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

#include "ks/history.h"

/////////////////////////////////////////////////////////////////////////////

const KsProjProps_THISTYPE *
KscHistory::getProjProps() const
{
    // TODO: implementation
    return 0;
}

/////////////////////////////////////////////////////////////////////////////

bool 
KscHistory::getProjPropsUpdate()
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
KscHistory::getParts(KsList<KsProjPropsHandle> &parts)
{
    if( !hasValidPath() ) return 0;

    KscServerBase *server = getServer();
    if( !server ) {
        _last_result = KS_ERR_GENERIC;
        return false;
    }

    // Prepare the service parameters
    //
    KsGetPPParams params;
    KsGetPPResult result;

    params.path      = getPathAndName();
    params.type_mask = KS_OT_VARIABLE;
    params.name_mask = KsString("*");

    // Request service
    //
    bool ok = server->getPP(av_module, params, result);
    
    if( ok ) {
        if( result.result == KS_ERR_OK ) {
            // Request succeeded, now copy data
            //
            PltListIterator<KsProjPropsHandle> it(result.items);
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

    PltListIterator<KsGetHistItem> it(selectors);
    size_t i = 0;
    while( it ) {
        params.items[i++] = *it;
        ++it;
    }

    PLT_ASSERT( i == sz );

    // Request service
    //
    bool ok = server->requestService(KS_HISTORY_PROTOCOL_NAME,
                                     KS_GETHIST_MINOR_OPCODE,
                                     av_module,
                                     params,
                                     result);

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
// EOF history.cpp
/////////////////////////////////////////////////////////////////////////////






