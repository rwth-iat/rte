/* -*- plt-c++ -*- */
#ifndef KS_REGISTER_INCLUDED
#define KS_REGISTER_INCLUDED
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

/////////////////////////////////////////////////////////////////////////

#include "ks/xdr.h"
#include "ks/ks.h"
#include "ks/string.h"
#include "ks/time.h"
#include "ks/result.h"

////////////////////////////////////////////////////////////////////////
// class KsServerDesc
////////////////////////////////////////////////////////////////////////

class KsServerDesc 
: public KsXdrAble 
{
public:
    KsServerDesc( const KsString &n=KsString(), u_short pv=0)
        : name(n), protocol_version(pv) {}
    KsServerDesc( XDR *, bool &);

    bool xdrEncode(XDR *) const;
    bool xdrDecode(XDR *);
    static KsServerDesc *xdrNew(XDR *);
    
    KsString  name;
    u_short   protocol_version;

    unsigned long hash() const;
    bool operator ==(const KsServerDesc & rhs) const;
};

//////////////////////////////////////////////////////////////////////////
// class KsRegistrationParams
//////////////////////////////////////////////////////////////////////////

class KsRegistrationParams 
: public KsXdrAble
{
public:
    KsRegistrationParams();
    KsRegistrationParams( const KsServerDesc &, u_short, u_long );
    KsRegistrationParams( XDR *, bool &);

    bool xdrEncode(XDR *) const;
    bool xdrDecode(XDR *);
    static KsRegistrationParams *xdrNew(XDR *);

    KsServerDesc server;
    u_short      port;
    u_long       time_to_live;
};

//////////////////////////////////////////////////////////////////////////
// class KsUnregistrationParams
//////////////////////////////////////////////////////////////////////////

class KsUnregistrationParams 
: public KsXdrAble
{
public:
    KsUnregistrationParams();
    KsUnregistrationParams( const KsServerDesc &);
    KsUnregistrationParams( XDR *, bool &);

    bool xdrEncode(XDR *) const;
    bool xdrDecode(XDR *);
    static KsUnregistrationParams *xdrNew(XDR *);

    KsServerDesc server;
};

//////////////////////////////////////////////////////////////////////
// class KsGetServerParams
//////////////////////////////////////////////////////////////////////

typedef KsUnregistrationParams KsGetServerParams;

//////////////////////////////////////////////////////////////////////////
// class KsRegistrationResult
//////////////////////////////////////////////////////////////////////////

typedef KsResult KsRegistrationResult;

//////////////////////////////////////////////////////////////////////////
// class KsUnregistrationResult
//////////////////////////////////////////////////////////////////////////

typedef KsResult KsUnregistrationResult;

//////////////////////////////////////////////////////////////////////
// class KsGetServerResult
//////////////////////////////////////////////////////////////////////

class KsGetServerResult
: public KsResult
{
public:
    KsGetServerResult(KS_RESULT res = KS_ERR_OK);
    KsGetServerResult(XDR *, bool &);

    bool xdrEncode(XDR *) const;
    bool xdrDecode(XDR *);
    static KsGetServerResult *xdrNew(XDR *);

    KsServerDesc server;
    u_short port;
    KsTime expires_at;
    bool living;
};

//////////////////////////////////////////////////////////////////////

#endif /* end of register.h */

/* End of ks/register.h */
