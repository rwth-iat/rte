/* -*-plt-c++-*- */
#ifndef KS_SIMPLESERVER_INCLUDED
#define KS_SIMPLESERVER_INCLUDED
/* $Header: /home/david/cvs/acplt/ks/include/ks/simpleserver.h,v 1.1 1997-03-24 18:40:17 martin Exp $ */
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
: public KsServerBase
{
public:
    KsSimpleServer(const char * server_name);
    virtual void dispatch(u_long serviceId, 
                          SVCXPRT *transport,
                          XDR *incomingXdr,
                          KsAvTicket &ticket);

    // service functions
    virtual void getVar(KsAvTicket &ticket,
                        KsGetVarParams &params,
                        KsGetVarResult &result);
    virtual void setVar(KsAvTicket &ticket,
                        KsSetVarParams &params,
                        KsSetVarResult &result);

protected:
    virtual bool initObjectTree() = 0;
    KssSimpleDomain _root_domain;

private:
    void getVarItem(KsAvTicket &ticket,
                    const KsPath & path,
                    KsGetVarItemResult &result);

};


//////////////////////////////////////////////////////////////////////
#endif // KS_SIMPLESERVER_INCLUDED
