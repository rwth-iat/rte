/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/src/simpleserver.cpp,v 1.1 1997-03-24 18:40:23 martin Exp $ */
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

#include "ks/simpleserver.h"
#include "ks/path.h"
#include "plt/log.h"
#include <ctype.h>
#if PLT_DEBUG
#include <iostream.h>
#include <iomanip.h>
#endif

//////////////////////////////////////////////////////////////////////

KsSimpleServer::KsSimpleServer(const char * str)
: KsServerBase(str),
  _root_domain("//root//")
{
}

//////////////////////////////////////////////////////////////////////
void
KsSimpleServer::dispatch(u_long serviceId, 
                         SVCXPRT *xprt,
                         XDR *xdrIn,
                         KsAvTicket &ticket)
{
    bool decodedOk = true;
    switch(serviceId) {
        
    case KS_GETVAR:
        {
            KsGetVarParams params(xdrIn, decodedOk);
            if (decodedOk) {
#if PLT_DEBUG
                cerr << "GETVAR " << endl;
#endif
                // properly decoded
                KsGetVarResult result(params.identifiers.size());
                getVar(ticket, params, result);
#if PLT_DEBUG
                cerr << result.result << endl;
#endif
                sendReply(xprt, ticket, result);
            } else {
                // not properly decoded
                sendErrorReply(xprt, ticket, KS_ERR_GENERIC);
            }
        }
        break;

    case KS_SETVAR:
        {
            KsSetVarParams params(xdrIn, decodedOk);
            if (decodedOk) {
#if PLT_DEBUG
                cerr << "SETVAR " << endl;
#endif
                // properly decoded
                KsSetVarResult result(params.items.size());
                setVar(ticket, params, result);
#if PLT_DEBUG
                cerr << result.result << endl;
#endif
                sendReply(xprt, ticket, result);
            } else {
                // not properly decoded
                sendErrorReply(xprt, ticket, KS_ERR_GENERIC);
            }
        }
        break;

        // TODO: remaining services

    default:
        // fall back on base class
        KsServerBase::dispatch(serviceId, xprt, xdrIn, ticket);
    }
}

//////////////////////////////////////////////////////////////////////

void 
KsSimpleServer::getVar(KsAvTicket &ticket,
                       KsGetVarParams &params,
                       KsGetVarResult &result)
{
    size_t reqsz=params.identifiers.size();
    PltArray<KsPath> paths(reqsz);
    PltArray<KS_RESULT> pathres(reqsz);
    
    if (   paths.size() == pathres.size() 
        && paths.size() == reqsz) {
        // Allocation ok.
        KsPath::resolvePaths(params.identifiers, paths, pathres);
        for (size_t i = 0; i < reqsz; ++i) {
            if (pathres[i] == KS_ERR_OK) {
                getVarItem(ticket, paths[i], result.items[i]);
            } else {
                result.items[i].result = pathres[i];
            }
        }
        result.result = KS_ERR_OK;
    } else {
        // Allocation failed.
        result.result = KS_ERR_GENERIC;
    }
}

//////////////////////////////////////////////////////////////////////

void
KsSimpleServer::getVarItem(KsAvTicket &ticket, 
                             const KsPath & path,
                             KsGetVarItemResult & result)
{
    PLT_PRECONDITION(path.isValid() && path.isAbsolute());
    if (ticket.canReadVar(KsString(PltString(path)))) {
        // Access okay.
        KssCommObjectHandle hvar(_root_domain.getChildByPath(path));
        if (hvar) {
            KssVariable * pvar = 
                PLT_DYNAMIC_PCAST(KssVariable, hvar.getPtr());
            if (pvar) {
                // Hey, we have found the variable!
                KsValueHandle vh (pvar->getValue());
                if (vh) {
                    KsCurrPropsHandle 
                        hprops(new KsVarCurrProps(vh, 
                                                  pvar->getTime(),
                                                  pvar->getState()),
                               KsOsNew);
                    if (hprops) {
                        result.item = hprops;
                        result.result = KS_ERR_OK;
                    } else {
                        result.result = KS_ERR_GENERIC;
                    }
                } else {
                    // no value!
                    result.result = KS_ERR_GENERIC;
                }
            } else {
                // Not a variable.
                result.result = KS_ERR_BADTYPE;
            }
        } else {
            // No such object.
            result.result = KS_ERR_BADPATH;
        }
    } else {
        // Access denied.
        result.result = KS_ERR_NOACCESS;
    }
}
        
//////////////////////////////////////////////////////////////////////
    
void 
KsSimpleServer::setVar(KsAvTicket &,
                  KsSetVarParams &,
                  KsSetVarResult & result) 
{
    result.result = KS_ERR_NOACCESS;
}

//////////////////////////////////////////////////////////////////////

/* EOF ks/simpleserver.cpp */
