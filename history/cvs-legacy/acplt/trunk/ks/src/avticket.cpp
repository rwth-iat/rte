/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/src/avticket.cpp,v 1.3 1997-03-13 15:36:32 martin Exp $ */
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


#include "ks/avticket.h"


//////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////

KS_IMPL_XDRCTOR(KsAuthType);

//////////////////////////////////////////////////////////////////////

KsAvTicket *
KsAvTicket::xdrNew(XDR * xdrs)                                
{                                                              
    PLT_PRECONDITION(xdrs->x_op == XDR_DECODE);                
    KsAvTicket * p = 0;                                              
    KsAuthType typecode;                                           
    if ( typecode.xdrDecode(xdrs) ) {                       
        /* typecode successfully decoded */                    
        switch(typecode) {                                     
            
            KS_XDR_MAP(KS_AUTH_NONE, KsAvNoneTicket);
            
        default:                                                
            {
                // not a fixed av-type: get c'tor at runtime
                KsTicketConstructor ctor;
                
                if ( _factory.query(typecode, ctor) ) {
                    // found it
                    p = ctor(xdrs);
                } else {
                    // unknown auth
                    p = new KsAvNoneTicket(KS_ERR_UNKNOWNAUTH);
                }
            }                                                      
        }
    }                                                          
    if (p) {                                                   
        PLT_ASSERT(p->xdrTypeCode() == typecode);              
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
KsAvTicket::canReadVars(const KsArray<KsString> & names,
                        KsArray<bool> &canRead) 
    const
{
    PLT_PRECONDITION(names.size() == canRead.size());
    bool res = true;
    size_t size = names.size();
    for (size_t i;
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
    for (size_t i;
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
    

//////////////////////////////////////////////////////////////////////

const KsAvTicket *
KsAvTicket::emergencyTicket()
{
    static KsAvNoneTicket e_ticket(KS_ERR_GENERIC, KS_AC_NONE);
    return &e_ticket;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

KsAvNoneTicket::KsAvNoneTicket(XDR *, bool & ok)
{
    ok = true;
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
KS_ACCESS 
KsAvNoneTicket::getAccess(const KsString &name) const
{
    return _access;
}

//////////////////////////////////////////////////////////////////////

bool
KsAvNoneTicket::canReadVar(const KsString &) const
{
    return (_access & KS_AC_READ) != 0;
}

//////////////////////////////////////////////////////////////////////

bool
KsAvNoneTicket::canWriteVar(const KsString &) const
{
    return (_access & KS_AC_WRITE) != 0;
}

//////////////////////////////////////////////////////////////////////
/* EOF ks/avticket.cpp */

