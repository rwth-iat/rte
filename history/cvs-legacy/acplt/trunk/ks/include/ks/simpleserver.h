/* -*-plt-c++-*- */
#ifndef KS_SIMPLESERVER_INCLUDED
#define KS_SIMPLESERVER_INCLUDED
/* $Header: /home/david/cvs/acplt/ks/include/ks/simpleserver.h,v 1.11 1998-10-01 09:59:10 markusj Exp $ */
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

#include "ks/svrbase.h"
#include "ks/serviceparams.h"
#include "ks/path.h"
#include "ks/svrsimpleobjects.h"

//////////////////////////////////////////////////////////////////////
// forward declarations
//////////////////////////////////////////////////////////////////////

class KsSimpleServer
: virtual public KsServerBase
{
public:
    KsSimpleServer(int port = KS_ANYPORT);

    //// accessors
    virtual KsString getServerName() const=0;
    virtual KsString getServerVersion() const=0;
    virtual KsString getServerDescription() const = 0;
    virtual KsString getVendorName () const = 0;

    //// modifiers
    // service functions
    virtual void getVar(KsAvTicket &ticket,
                        const KsGetVarParams &params,
                        KsGetVarResult &result);
    virtual void setVar(KsAvTicket &ticket,
                        const KsSetVarParams &params,
                        KsSetVarResult &result);
    virtual void getPP(KsAvTicket &ticket, 
                       const KsGetPPParams & params,
                       KsGetPPResult & result);

protected:
    KssSimpleDomain _root_domain;

    virtual void getVarItem(KsAvTicket &ticket,
                            const KsPath & path,
                            KsGetVarItemResult &result);
    virtual void setVarItem(KsAvTicket &ticket,
                            const KsPath & path,
                            const KsCurrPropsHandle & curr_props,
                            KsResult &result);

    void getPPOfDomain(KssDomain *pd,
                       const PltString &prefix,
                       KsAvTicket &ticket,
                       const KsGetPPParams &params,
                       KsGetPPResult &result);

    bool addCommObject(const KsPath & dompath,
                       const KssCommObjectHandle & ho);

    bool removeCommObject(const KsPath & dompath,
                          const KsString & id);

    bool addDomain(const KsPath & dompath,
                   const KsString & id,
                   const KsString & comment = KsString());

    bool addStringVar(const KsPath & dompath,
                      const KsString & id,
                      const KsString & str,
                      const KsString & comment = KsString(),
                      bool lock = true);
    
    bool initVendorTree();
    
private:
#if PLT_USE_BUFFERED_STREAMS
    bool initStatistics();
#endif
}; // class KsSimpleServer


//////////////////////////////////////////////////////////////////////
#endif // KS_SIMPLESERVER_INCLUDED
