/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/src/avmodule.cpp,v 1.7 2000-04-10 15:00:57 harald Exp $ */
/*
 * Copyright (c) 1996, 1997, 1998, 1999, 2000
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

#include "ks/avmodule.h"
#include "ks/client.h"

//////////////////////////////////////////////////////////////////////
// define static data
//
KscNoneNegotiator KscAvNoneModule::the_negotiator;


// ----------------------------------------------------------------------------
// Construct a new av module (at least the base class part).
//
// Note that the list referencing servers (by name) for which negotiators have
// been created is done by a dynamically allocated PltList object. Which is
// used to circumvent all the const contrains imposed on several parts of the
// code. Sigh.
//
KscAvModule::KscAvModule()
{
    _server_namelist = new PltList<KsString>;
} // KscAvModule::KscAvModule


// ----------------------------------------------------------------------------
// Destroy an a/v module. This involves destroying a/v negotiators which once
// have been created to maintain a/v state for particular ACPLT/KS server
// connections.
//
KscAvModule::~KscAvModule()
{
    KsString       serverName;
    KscServerBase *serverBase;
    KscServer     *server;

    while ( !_server_namelist->isEmpty() ) {
	serverName = _server_namelist->removeFirst();
	serverBase = KscClient::getClient()->getServer(serverName);
	if ( serverBase && 
             (server = PLT_DYNAMIC_PCAST(KscServer, serverBase)) ) {
	    server->dismissNegotiator(this);
	}
    }
    delete _server_namelist;
} // KscAvModule::~KscAvModule


// ----------------------------------------------------------------------------
//
//
KscNegotiatorHandle
KscAvModule::getNegotiator(const KscServer *server) const {
    KscNegotiatorHandle hNegotiator =
	_getNegotiator(server);
    _server_namelist->addFirst(server->getHostAndName());
    return hNegotiator;
} // KscAvModule::getNegotiator


//////////////////////////////////////////////////////////////////////

bool
KscNoneNegotiator::xdrEncode(XDR *xdr)
{
    enum_t auth_none = KS_AUTH_NONE;
    return ks_xdre_enum(xdr, &auth_none);
}

//////////////////////////////////////////////////////////////////////

bool
KscNoneNegotiator::xdrDecode(XDR *xdr)
{
    enum_t auth_type;

    bool ok = ks_xdrd_enum(xdr, &auth_type);

    return ok &&
        auth_type == KS_AUTH_NONE;
}

//////////////////////////////////////////////////////////////////////

KscNegotiatorHandle
KscAvNoneModule::_getNegotiator(const KscServer *) const
{
    return KscNegotiatorHandle(&the_negotiator, PltOsUnmanaged);
}


// End of avmodule.cpp
