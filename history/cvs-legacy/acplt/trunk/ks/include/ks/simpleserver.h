/* -*-plt-c++-*- */
#ifndef KS_SIMPLESERVER_INCLUDED
#define KS_SIMPLESERVER_INCLUDED
/* $Header: /home/david/cvs/acplt/ks/include/ks/simpleserver.h,v 1.20 2007-04-25 12:57:20 martin Exp $ */
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

/* Author: Martin Kneissl <martin@plt.rwth-aachen.de> */


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
    virtual void getEP(KsAvTicket &ticket, 
                       const KsGetEPParams & params,
                       KsGetEPResult & result);

protected:
    KssSimpleDomain _root_domain;

    virtual void getVarItem(KsAvTicket &ticket,
                            const KsPath & path,
                            KsGetVarItemResult &result);
    virtual void setVarItem(KsAvTicket &ticket,
                            const KsPath & path,
                            const KsCurrPropsHandle & curr_props,
                            KsResult &result);

    void getEPOfObject(KssCommObject *pd,
                       const PltString &prefix,
                       KsAvTicket &ticket,
                       const KsGetEPParams &params,
                       KsGetEPResult &result);

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
    
    KsString getOsIdent();

private:
#if PLT_USE_BUFFERED_STREAMS
    bool initStatistics();
#endif
}; // class KsSimpleServer


//////////////////////////////////////////////////////////////////////
#endif // KS_SIMPLESERVER_INCLUDED
