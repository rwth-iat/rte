/* -*-c++-*- */
#ifndef KS_SERVER_INCLUDED
#define KS_SERVER_INCLUDED
/* $Header: /home/david/cvs/acplt/ks/include/ks/server.h,v 1.10 2003-10-13 11:32:25 harald Exp $ */
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

/*
 * server.h: this module contains the knowledge how to register ACPLT/KS
 *           servers with the ACPLT/KS manager. There isn't really much
 *           more to it, folks.
 */

/*
 * Written by Harald Albrecht & Martin Kneissl
 * <harald@plt.rwth-aachen.de> <martin@plt.rwth-aachen.de>
 */

#include "ks/svrbase.h"


//////////////////////////////////////////////////////////////////////

class KsReregisterServerEvent;

//////////////////////////////////////////////////////////////////////

class KsManagerRegistration;

class KsServer
: virtual public KsServerBase
{
public:
    KsServer(u_long ttl, int port = KS_ANYPORT);
    virtual ~KsServer();

    //// accessors
    virtual KsString getServerName() const=0;
    virtual KsString getServerVersion() const=0;
    virtual KsString getServerDescription() const=0;
    virtual KsString getVendorName() const=0;

    virtual void startServer();    // start answering requests
    virtual void stopServer();     // stop answering requests asap

protected:
    u_long _ttl;
    KsManagerRegistration *_ks_mgr;
};

    
#endif // KS_SERVER_INCLUDED


// End of ks/server.h
