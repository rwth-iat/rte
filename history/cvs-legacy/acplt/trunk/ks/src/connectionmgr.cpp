/* -*-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/src/connectionmgr.cpp,v 1.11 2003-10-13 12:13:56 harald Exp $ */
/*
 * Copyright (c) 1996, 2003
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

/*
 * connectionmgr.cpp -- Abstract base class for connection managers which can
 *   handle connection objects, which in turn encapsulate XDR streams. The
 *   connection manager makes it possible to use so-called buffered XDR
 *   streams, which decouple RPC I/O from service handling and thus
 *   serialization and deserialization. In addition, the connection manager
 *   can also handle other kinds of connections.
 *
 * Written by Harald Albrecht <harald@plt.rwth-aachen.de>
 */

#include "ks/connectionmgr.h"


// ---------------------------------------------------------------------------
// The one and only Connection Manager object per application.
//
KsConnectionManager *KsConnectionManager::the_cnx_manager = 0;


// ---------------------------------------------------------------------------
// Construct a new connection manager object.
//
KsConnectionManager::KsConnectionManager()
    : _is_ok(true)
{
    PLT_PRECONDITION( the_cnx_manager == 0 );
    the_cnx_manager = this;
} // KsConnectionManager::KsConnectionManager


// ---------------------------------------------------------------------------
// Destroy a connection manager object.
//
KsConnectionManager::~KsConnectionManager()
{
} // KsConnectionManager::~KsConnectionManager


/* End of connectionmgr.cpp */
