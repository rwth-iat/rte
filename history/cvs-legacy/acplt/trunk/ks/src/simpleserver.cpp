/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/src/simpleserver.cpp,v 1.8 1997-04-11 17:24:40 martin Exp $ */
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

//////////////////////////////////////////////////////////////////////

KsSimpleServer::KsSimpleServer()
: _root_domain("/")
{
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
//////////////////////////////////////////////////////////////////////

void 
KsSimpleServer::setVar(KsAvTicket &ticket,
                       KsSetVarParams &params,
                       KsSetVarResult &result)
{
    size_t reqsz=params.items.size();
    PltArray<KsString> ids(reqsz);
    PltArray<KsPath> paths(reqsz);
    PltArray<KS_RESULT> pathres(reqsz);
    if (   paths.size() == reqsz 
        && ids.size() == reqsz 
        && pathres.size() == reqsz ) {
        // Allocation ok, retrieve ids.
        for (size_t j=0; j<reqsz; ++j) {
            ids[j] = params.items[j].path_and_name;
        }
        // Make them absolute
        KsPath::resolvePaths(ids, paths, pathres);
        // Iterate items
        for (size_t i = 0; i < reqsz; ++i) {
            if (pathres[i] == KS_ERR_OK) {
                // Set current properties
                setVarItem(ticket, 
                           paths[i], 
                           params.items[i].curr_props,
                           result.results[i]);
            } else {
                result.results[i].result = pathres[i];
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
                result.item = pvar->getCurrProps();
                result.result = result.item ? KS_ERR_OK : KS_ERR_GENERIC;
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
//////////////////////////////////////////////////////////////////////

void
KsSimpleServer::setVarItem(KsAvTicket &ticket, 
                           const KsPath & path,
                           const KsCurrPropsHandle & curr_props,
                           KsResult & result)
{
    PLT_PRECONDITION(path.isValid() && path.isAbsolute());
    if (ticket.canWriteVar(KsString(PltString(path)))) {
        // Access okay.
        KssCommObjectHandle hvar(_root_domain.getChildByPath(path));
        if (hvar) {
            KssVariable * pvar = 
                PLT_DYNAMIC_PCAST(KssVariable, hvar.getPtr());
            if (pvar) {
                // Hey, we have found the variable!
                result.result = pvar->setCurrProps(curr_props);
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
KsSimpleServer::getPP(KsAvTicket &ticket, 
                      const KsGetPPParams & params,
                      KsGetPPResult & result) 
{
    KsPath path(params.path);
    if (path.isValid() && path.isAbsolute()) {
        // Path is OK
        if (ticket.isVisible(params.path)) {
            KssCommObjectHandle hc;
            KssDomain *pd = 0;
            if (params.path == "/") {
                // root must be handled specially
                pd = &_root_domain;
            } else {
                hc = _root_domain.getChildByPath(path);
                if (hc) {
                    // Child found. Is it a domain?
                    pd = PLT_DYNAMIC_PCAST(KssDomain, hc.getPtr());
                }
            }
            if (pd) {
                // Yes! 
                if (params.name_mask == "") {
                    // We are being asked about the proj. props of the
                    // domain itself
                    KsProjPropsHandle h(pd->getPP());
                    if (h && result.items.addLast(h)) {
                        result.result = KS_ERR_OK;
                    } else {
                        result.result = KS_ERR_GENERIC;
                    }
                } else {
                    // Iterate over its children.
                    result.result = KS_ERR_OK; // report no other errors
                    KssDomainIterator *pit = 
                        pd->newMaskedIterator(params.name_mask, 
                                              params.type_mask);
                    if (pit) {
                        // We got an iterator. Use it.
                        for (KssDomainIterator &it(*pit); it; ++it) {
                            if (*it) {
                                // check if the child is visible
                                PltString childname(params.path,
                                                    (*it)->getIdentifier());
                                if (ticket.isVisible(childname)) {
                                    // Ask for proj properties
                                    KsProjPropsHandle hpp = (*it)->getPP();
                                    if (hpp) {
                                        hpp->access_mode &= 
                                            ticket.getAccess(childname);
                                        result.items.addLast(hpp);
                                    } else {
                                        // ignore error
                                    }
                                } else {
                                    // not visible, do nothing
                                }
                            } else {
                                // null handle, log error
                                PltLog::Error("Child iterator returned"
                                              " null handle");
                            }
                        } // for
                        delete pit;
                    } else {
                        // no iterator: ignore error
                    }
                } // needs iteration
            } else {
                // not a domain or no such child
                result.result = KS_ERR_BADPATH;
            }
        } else {
            // domain invisible
            result.result = KS_ERR_NOACCESS; // TODO or BADPATH?
        }
    } else {
        // Path syntax error
        result.result = KS_ERR_BADNAME;
    }
}

//////////////////////////////////////////////////////////////////////

bool
KsSimpleServer::addCommObject(const KsPath & dompath,
                              const KssCommObjectHandle & ho)
{
    PLT_PRECONDITION(dompath.isValid() && dompath.isAbsolute());
    KssCommObjectHandle hd;
    if (dompath.isSingle() && dompath[0] == "") {
        hd.bindTo(&_root_domain, KsOsUnmanaged);
    } else {
        hd = _root_domain.getChildByPath(dompath);
    }
    if (hd) {
        //
        // found an object, is it a simple domain?
        //
        KssSimpleDomain * pd = 
            PLT_DYNAMIC_PCAST(KssSimpleDomain, hd.getPtr());
        if (pd) {
            //
            // It is a simple domain. Add the child.
           //
           return pd->addChild(ho);
        } else {
            //
            // Not a simple domain.
            //
            return false;
        }
    } else {
        //
        // object not found or other error.
        //
        return false;
    }
}

//////////////////////////////////////////////////////////////////////
        
bool
KsSimpleServer::addDomain(const KsPath & dompath,
                                const KsString & id,
                                const KsString & comment)
{
    PLT_PRECONDITION(dompath.isValid() && dompath.isAbsolute());
    KssSimpleDomain * pd = new KssSimpleDomain(id);
    if (pd) {
        pd->setComment(comment);
        KssCommObjectHandle ho(pd, KsOsNew);
        if (ho) {
            return addCommObject(dompath, ho);
        }
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
        
bool
KsSimpleServer::addStringVar(const KsPath & dompath,
                                   const KsString & id,
                                   const KsString & str,
                                   const KsString & comment,
                                   bool lock)
{
    PLT_PRECONDITION(dompath.isValid() && dompath.isAbsolute());
    KsValueHandle hv(new KsStringValue(str), KsOsNew);
    if (hv) {
        KssSimpleVariable *po = new KssSimpleVariable(id);
        if (po) {
            po->setValue(hv);
            po->setState(KS_ST_GOOD);
            po->setComment(comment);
            if (lock) po->lock();
            KssCommObjectHandle ho(po, KsOsNew);
            if (ho) {
                return addCommObject(dompath, ho);
            }
        }
    }
    return false;
}

//////////////////////////////////////////////////////////////////////

bool
KsSimpleServer::initVendorTree()
{
    static KssTimeNowVariable server_time("server_time");
    KssCommObjectHandle server_time_handle(&server_time, KsOsUnmanaged);
    KsPath vendor("/vendor");
    addDomain(KsPath("/"), "vendor"); // ignore error, may already exist.
    return 
           addCommObject(vendor, server_time_handle)

        && addStringVar(vendor, "server_name",
                        getServerName())

        && addStringVar(vendor, "server_version",
                        getServerVersion())

        && addStringVar(vendor, "server_description",
                        getServerDescription())

        && addStringVar(vendor, "name",
                        getVendorName());
}


//////////////////////////////////////////////////////////////////////
/* EOF ks/simpleserver.cpp */
