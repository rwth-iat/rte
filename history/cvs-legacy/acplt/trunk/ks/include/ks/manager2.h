/* -*-plt-c++-*- */

#ifndef KS_MANAGER_INCLUDED 
#define KS_MANAGER_INCLUDED

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





