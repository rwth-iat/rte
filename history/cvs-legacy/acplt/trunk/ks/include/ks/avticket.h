/* -*-plt-c++-*- */
#ifndef KS_AVTICKET_INCLUDED
#define KS_AVTICKET_INCLUDED
/* $Header: /home/david/cvs/acplt/ks/include/ks/avticket.h,v 1.18 1999-01-12 16:12:36 harald Exp $ */
/*
 * Copyright (c) 1996, 1997, 1998, 1999
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
        { return getAccess(name) != KS_AC_NONE; }

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
