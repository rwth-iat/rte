/* -*-plt-c++-*- */

#ifndef KS_MANAGER_INCLUDED 
#define KS_MANAGER_INCLUDED

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

/* Author: Markus Juergens <markusj@plt.rwth-aachen.de> */

//////////////////////////////////////////////////////////////////////

// #include <ks/svrbase.h>
#include <ks/objecttree.h>

//////////////////////////////////////////////////////////////////////

const KsString KS_MANAGER_NAME("manager");
const KsString KS_MANAGER_VERSION("1");
const KsString KS_MANAGER_DESCRIPTION("manager");

//////////////////////////////////////////////////////////////////////

class KsManager
// : public KsServerBase
{
public:
    KsManager();
    ~KsManager();

#ifdef DEBUG
    void printTree() const;
    void printTreeRek(int indent, KsObjectTreeBase *) const;
    void printTable() const;
#endif

#ifndef DEBUG

protected:

#endif

#if 0
    //
    // service functions
    //
    void getPP(KsAvTicket &ticket,
               const KsGetPPParams &params,
               KsGetPPResult &result );
    void getVar(KsAvTicket &ticket,
                KsGetVarParams &params,
                KsGetVarResult &result);
    void setVar(KsAvTicket &ticket,
                KsSetVarParams &params,
                KsSetVarResult &result);
    void exgData(KsAvTicket &ticket,
                 KsExgDataParams &params,
                 KsExgDataResult &result);
#endif
    //
    // data and functions for the object tree
    //
    KsObjectTree objectTree;
    
    // initialize object tree with predefined values
    void initObjectTree();
    // add new server description
    bool addServerDescription(const KsString &name,
                              u_long protocol_version,
                              const KsTime &expires_at,
                              u_long port);
    // remove server description
    bool removeServerDescription(const KsString &name,
                                 u_long protocol_version);    
};

#endif





