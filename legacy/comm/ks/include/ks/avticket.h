/* -*-plt-c++-*- */
#ifndef KS_AVTICKET_INCLUDED
#define KS_AVTICKET_INCLUDED
/* $Header: /home/david/cvs/acplt/ks/include/ks/avticket.h,v 1.24 2009-07-08 06:24:25 henning Exp $ */
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

#include "ks/ks.h"
#include "ks/xdr.h"
#include "ks/string.h"

#if !PLT_SERVER_TRUNC_ONLY
#include "ks/array.h"
#include "plt/hashtable.h"
#endif

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

class KsAuthType 
: public KsXdrAble
{
public:
    KsAuthType(enum_t t = KS_AUTH_NONE) : _t(t) { }
    KsAuthType(XDR *xdr, bool & ok);
    virtual bool xdrEncode(XDR *) const;
    virtual bool xdrDecode(XDR *);
    operator enum_t () const { return _t; }

    unsigned long hash() const { return _t; }
private:
    enum_t _t;
};

//////////////////////////////////////////////////////////////////////

inline bool 
KsAuthType::xdrDecode(XDR *xdr)
{
    PLT_PRECONDITION(xdr->x_op == XDR_DECODE);
    return ks_xdrd_enum(xdr, & _t);
}

//////////////////////////////////////////////////////////////////////

inline bool 
KsAuthType::xdrEncode(XDR *xdr) const
{
    PLT_PRECONDITION(xdr->x_op == XDR_ENCODE);
    return ks_xdre_enum(xdr, & _t);
}


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// Typedef for a ticket factory. This way we can generalize ticket
// construction and thus provide a registry of constructable ticket
// types.
//
class KsAvTicket;
typedef KsAvTicket * (*KsTicketConstructor)(XDR *);

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

class KsAvTicket
: public KsXdrUnion
{
public:
    static KsAvTicket * xdrNew(XDR *);

    virtual KS_RESULT result() const = 0;

    virtual enum_t xdrTypeCode() const = 0;
    KsAvTicket() : _psaddr(0) { }
    virtual ~KsAvTicket();

    ////
    //// Permissions (not available for a minimalist's server)
    ////
#if !PLT_SERVER_TRUNC_ONLY
    virtual bool isVisible(const KsString & name) const
        { return (getAccess(name) & (KS_AC_READ|KS_AC_WRITE)) != KS_AC_NONE; }

    virtual bool canReadVar(const KsString &name) const = 0;    

    virtual bool canWriteVar(const KsString &name) const = 0;

    virtual bool canReadVars(const KsArray<KsString> & names,
                             KsArray<bool> &canRead) const;
    
    virtual bool canWriteVars(const KsArray<KsString> & names,
                              KsArray<bool> &canRead) const;

    KS_ACCESS getAccess(const KsString &name) const;
#endif
    
    ////
    //// sender address
    ////
    const sockaddr * getSenderAddress() const;
    struct in_addr getSenderInAddr() const; // network byte order

    ////
    //// Ticket type registration (not available for a minimalist's server)
    ////
#if !PLT_SERVER_TRUNC_ONLY
    static bool registerAvTicketType(enum_t ticketType, KsTicketConstructor);
    static bool deregisterAvTicketType(enum_t ticketType);
#endif

    ////
    //// last resort ticket
    ////
    static KsAvTicket * emergencyTicket();

#if PLT_DEBUG_INVARIANTS
    virtual bool invariant() const;
#endif

    bool setSenderAddress(const sockaddr * sockaddr, int namelen);

    virtual bool xdrEncodeTrailer(XDR *);
    virtual bool xdrDecodeTrailer(XDR *);
    
protected:
    virtual bool xdrEncodeVariant(XDR *) const = 0;
    virtual bool xdrDecodeVariant(XDR *)  = 0;

private:
#if !PLT_SERVER_TRUNC_ONLY
    static PltHashTable<KsAuthType, KsTicketConstructor> _factory;
#endif
    struct sockaddr * _psaddr;
}; // class KsAvTicket


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

class KsAvNoneTicket
: public KsAvTicket
{
public:
    KsAvNoneTicket() : _access(_default_access), _result(KS_ERR_OK) { }

    KsAvNoneTicket(KS_RESULT r, KS_ACCESS a = KS_AC_NONE);
    KsAvNoneTicket(XDR *, bool &);

    virtual KS_RESULT result() const;

    virtual enum_t xdrTypeCode() const { return KS_AUTH_NONE; }

    virtual bool canReadVar(const KsString & name) const;
    virtual bool canWriteVar(const KsString & name) const;

    static void setDefaultAccess(KS_ACCESS a)
        { _default_access = a; }

#if PLT_DEBUG_INVARIANTS
    virtual bool invariant() const;
#endif

protected:
    bool xdrDecodeVariant(XDR *);
    bool xdrEncodeVariant(XDR *) const;
    KS_ACCESS _access;
    KS_RESULT _result;

private:
    static KS_ACCESS _default_access;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

class KsAvSimpleTicket
: public KsAvNoneTicket
{
public:
    KsAvSimpleTicket() { }

    KsAvSimpleTicket(XDR *, bool &);

    virtual enum_t xdrTypeCode() const { return KS_AUTH_SIMPLE; }

    KsString getId() const { return _id; }
protected:
    bool xdrDecodeVariant(XDR *);
    bool xdrEncodeVariant(XDR *) const;
    KsString _id;
};

//////////////////////////////////////////////////////////////////////
// INLINE IMPLEMENTATION
//////////////////////////////////////////////////////////////////////

inline const struct sockaddr *
KsAvTicket::getSenderAddress() const
{
    return _psaddr;
}

//////////////////////////////////////////////////////////////////////

inline struct in_addr
KsAvTicket::getSenderInAddr() const
{
    struct in_addr inaddr;
    if (_psaddr && _psaddr->sa_family == AF_INET) {
        inaddr = ((sockaddr_in *)  _psaddr)->sin_addr;
    } else {
        inaddr.s_addr = INADDR_NONE;
    }
    return inaddr;
}

//////////////////////////////////////////////////////////////////////


#endif

/* End of avticket.h */
