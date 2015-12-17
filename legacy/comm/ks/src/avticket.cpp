/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/src/avticket.cpp,v 1.23 2009-07-08 06:23:50 henning Exp $ */
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


#include "ks/avticket.h"
#include <string.h>
#include <stdlib.h>


//////////////////////////////////////////////////////////////////////
#if PLT_DEBUG_INVARIANTS

bool
KsAvTicket::invariant() const
{
    enum_t res = result();
    return res == KS_ERR_OK
        || res == KS_ERR_GENERIC
        || res == KS_ERR_UNKNOWNAUTH
        || res == KS_ERR_BADAUTH;
}

#endif // PLT_DEBUG_INVARIANTS
//////////////////////////////////////////////////////////////////////

#if !PLT_SERVER_TRUNC_ONLY
PltHashTable<KsAuthType, KsTicketConstructor>
KsAvTicket::_factory;

//////////////////////////////////////////////////////////////////////

bool
KsAvTicket::registerAvTicketType(enum_t ticketType, KsTicketConstructor ctor)
{
    return _factory.add(KsAuthType(ticketType), ctor);
}

//////////////////////////////////////////////////////////////////////

bool
KsAvTicket::deregisterAvTicketType(enum_t ticketType)
{
    KsTicketConstructor dummy;
    return _factory.remove(KsAuthType(ticketType), dummy);
}
#endif

//////////////////////////////////////////////////////////////////////

KS_IMPL_XDRCTOR(KsAuthType);

//////////////////////////////////////////////////////////////////////

KsAvTicket::~KsAvTicket()
{
    if (_psaddr) {
        free(_psaddr);
    }
}

//////////////////////////////////////////////////////////////////////

KsAvTicket *
KsAvTicket::xdrNew(XDR * xdrs)                                
{                                                              
    PLT_PRECONDITION(xdrs->x_op == XDR_DECODE);                
    KsAvTicket * p = 0;                                              
    KsAuthType typecode;                                           
    if ( typecode.xdrDecode(xdrs) ) {                       
        /* typecode successfully decoded */                    
#if !PLT_SERVER_TRUNC_ONLY
        KsTicketConstructor ctor;
                
        if ( _factory.query(typecode, ctor) ) {
            // found a custom constructor for this av-typecode
            p = ctor(xdrs);
        } else {
#else
	{
#endif
            // try builtins
            switch(typecode) {                                     
                KS_XDR_MAP(KS_AUTH_NONE, KsAvNoneTicket);
                KS_XDR_MAP(KS_AUTH_SIMPLE, KsAvSimpleTicket);
            default:                                                
                {
                    // unknown auth:
                    // We MUST generate a ticket (an invalid one in this case)
                    p = new KsAvNoneTicket(KS_ERR_UNKNOWNAUTH);
                }
            }                                                      
        }
    }                                                          
    if (p) {                                                   
        PLT_ASSERT(   p->xdrTypeCode() == typecode 
                   || p->xdrTypeCode() == KS_AUTH_NONE);              
        if (!p->xdrDecodeCommon(xdrs)) {                       
            delete p;                                          
            return 0;                                          
        }                                                      
    }                                                          
    return p;                                                  
}                                                              

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

bool
KsAvTicket::setSenderAddress(const sockaddr * pSaddr,
                             int namelen)
{
    PLT_PRECONDITION(namelen>=0 && pSaddr);
    _psaddr = (sockaddr *) malloc(namelen);
    if (_psaddr) {
        memcpy(_psaddr, pSaddr, namelen);
    } else {
        if (_psaddr) {
            free(_psaddr);
        }
        _psaddr = 0;
    }
    return _psaddr != 0;
}

//////////////////////////////////////////////////////////////////////

#if !PLT_SERVER_TRUNC_ONLY
bool 
KsAvTicket::canReadVars(const KsArray<KsString> & names,
                        KsArray<bool> &canRead) 
    const
{
    PLT_PRECONDITION(names.size() == canRead.size());
    bool res = true;
    size_t size = names.size();
    for (size_t i=0;
         i < size;
         ++i) {
        bool ok = canReadVar(names[i]);
        canRead[i] = ok;
        res = res && ok;
    }
    return res;
}

//////////////////////////////////////////////////////////////////////

bool 
KsAvTicket::canWriteVars(const KsArray<KsString> & names,
                        KsArray<bool> &canWrite) 
    const
{
    PLT_PRECONDITION(names.size() == canWrite.size());
    size_t size = names.size();
    bool res = true;
    for (size_t i=0;
         i < size;
         ++i) {
        bool b = canWriteVar(names[i]);
        canWrite[i] = b;
        res = res && b;
    }
    return res;
}


//////////////////////////////////////////////////////////////////////

KS_ACCESS
KsAvTicket::getAccess(const KsString &name) const
{
    enum_t res = KS_AC_NONE;
    if (canReadVar(name))   res |= KS_AC_READ;
    if (canWriteVar(name))  res |= KS_AC_WRITE;
    return (KS_ACCESS) res;
}
#endif

/////////////////////////////////////////////////////////////////////////////

KsAvTicket *
KsAvTicket::emergencyTicket()
{
    static KsAvNoneTicket e_ticket(KS_ERR_GENERIC, KS_AC_NONE);
    return &e_ticket;
}


bool
KsAvTicket::xdrEncodeTrailer(XDR *)
{
    return true;
} // KsAvTicket::xdrEncodeTrailer

bool
KsAvTicket::xdrDecodeTrailer(XDR *)
{
    return true;
} // KsAvTicket::xdrDecodeTrailer


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
#if PLT_DEBUG_INVARIANTS

bool
KsAvNoneTicket::invariant() const
{
    return KsAvTicket::invariant() 
        && (_access & ~(KS_AC_READ|KS_AC_WRITE)) == 0;
}

#endif // PLT_DEBUG_INVARIANTS
//////////////////////////////////////////////////////////////////////

KS_ACCESS
KsAvNoneTicket::_default_access = KS_AC_NONE;

//////////////////////////////////////////////////////////////////////

KsAvNoneTicket::KsAvNoneTicket(XDR *, bool & ok)
: _access(_default_access),
  _result(KS_ERR_OK)
{
    ok = true;
    PLT_CHECK_INVARIANT();
}

//////////////////////////////////////////////////////////////////////

KsAvNoneTicket::KsAvNoneTicket(KS_RESULT r, KS_ACCESS a)
: _access(a),
  _result(r)
{
}

//////////////////////////////////////////////////////////////////////

KS_RESULT
KsAvNoneTicket::result() const
{
    return _result;
}

//////////////////////////////////////////////////////////////////////

bool
KsAvNoneTicket::xdrDecodeVariant(XDR *)
{
    return true;
}

//////////////////////////////////////////////////////////////////////

bool
KsAvNoneTicket::xdrEncodeVariant(XDR *) const
{
    return true;
}

//////////////////////////////////////////////////////////////////////

bool
KsAvNoneTicket::canReadVar(const KsString &) const
{
    return ( _access & KS_AC_READ ) != 0;
}

//////////////////////////////////////////////////////////////////////

bool
KsAvNoneTicket::canWriteVar(const KsString &) const
{
    return ( _access & KS_AC_WRITE ) != 0;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

KsAvSimpleTicket::KsAvSimpleTicket(XDR *xdr, bool & ok)
: KsAvNoneTicket(xdr,ok)
{
    if (ok) {
        ok = _id.xdrDecode(xdr);
    }
    PLT_CHECK_INVARIANT();
}

//////////////////////////////////////////////////////////////////////

bool
KsAvSimpleTicket::xdrDecodeVariant(XDR *xdr)
{
    return _id.xdrDecode(xdr);
}

//////////////////////////////////////////////////////////////////////

bool
KsAvSimpleTicket::xdrEncodeVariant(XDR *xdr) const
{
#if 1
    return true;
#else
    // @ Martin: Seit wann hat SimpleTicket eigenen EncodeVariant()
    //           und noch in dieser Form ???
    return _id.xdrEncode(xdr);
#endif
}

//////////////////////////////////////////////////////////////////////

/* EOF ks/avticket.cpp */

