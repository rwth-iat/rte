/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/examples/tserver1.cpp,v 1.5 1997-09-02 15:06:38 martin Exp $ */
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

/* Author: Martin Kneissl <martin@plt.rwth-aachen.de> */

#include "ks/server.h"
#include "ks/simpleserver.h"

#include "plt/log.h"
#include <signal.h>

extern "C" void handler(int) {
    KsServerBase::getServerObject().downServer();
}

class TestServer
: public KsServer,
  public KsSimpleServer
{	
//    friend void handler(int);
public:
	TestServer();

    //// accessors
    virtual KsString getServerName() const
        { return KsString("tserver"); }

    virtual KsString getServerVersion() const
        { return KsString("0.5.1"); }

    virtual KsString getServerDescription() const 
        { return KsString("ACPLT/KS test server"); }

    virtual KsString getVendorName () const
        { return KsString("Lehrstuhl fuer Prozessleittechnik, "
                          "RWTH Aachen"); }

};

//////////////////////////////////////////////////////////////////////

class TestDomainIterator
: public KssDomainIterator
{
public:
#if PLT_RETTYPE_OVERLOADABLE
    typedef TestDomainIterator THISTYPE;
#endif
    virtual operator bool () const
        { return 0; } // no iteration
    virtual KssCommObjectHandle operator * () const
        { PLT_ASSERT(0==1); return KssCommObjectHandle(); }
    virtual THISTYPE & operator ++()
        { return *this; }
    virtual void toStart()
        { }
};

//////////////////////////////////////////////////////////////////////

class TestDomain
: public KssDomain
{
    virtual KsString  getIdentifier() const
        { return KsString("TestDomain"); }
    virtual KsTime    getCreationTime() const 
        { return KsTime::now(); } // not strictly correct
    virtual KsString  getComment() const
        { return KsString("A dynamic domain"); }

    //// KssDomain
    //   accessors
    virtual TestDomainIterator::THISTYPE * newIterator() const;

    virtual KssCommObjectHandle getChildById(const KsString & id) const;

    PLT_DECL_RTTI;
};

//////////////////////////////////////////////////////////////////////

KssCommObjectHandle
TestDomain::getChildById(const KsString &str) const
{
    KssSimpleVariable * var =
        new KssSimpleVariable(str);
    int j = atoi(str);
    if ( var ) {
        if (j != 0) {
            var->setValue(new KsIntValue(j*j));
            var->setState(KS_ST_GOOD);
            var->lock();
        } else {
            var->setValue(new KsStringValue(str));
            var->setState(KS_ST_BAD);
            var->lock();
        }
    }
    return KssCommObjectHandle(var, KsOsNew);
}

//////////////////////////////////////////////////////////////////////
PLT_IMPL_RTTI1(TestDomain, KssDomain);
//////////////////////////////////////////////////////////////////////

TestDomainIterator::THISTYPE * 
TestDomain::newIterator() const
{ 
    return new TestDomainIterator; 
}

//////////////////////////////////////////////////////////////////////

TestServer::TestServer()
: KsServer(30)
{
    KsAvNoneTicket::setDefaultAccess(KS_AC_READ | KS_AC_WRITE); // TODO
    initVendorTree();

    signal(SIGINT, handler);
    signal(SIGTERM, handler);
#if !PLT_SYSTEM_NT
    signal(SIGHUP, handler);
#endif
    
    KssSimpleDomain * test_dom = new KssSimpleDomain("test");
    KssSimpleVariable * write_me = new KssSimpleVariable("write_me");
    write_me->setValue(new KsVoidValue);
    test_dom->addChild(write_me);
    KssSimpleVariable * doubles_var = new KssSimpleVariable("doubles");
    KsDoubleVecValue * doubles_val = 
        new KsDoubleVecValue(1000);
    for (size_t i = 0 ; i < doubles_val->size(); ++i) {
        (*doubles_val)[i] = i*i;
    }
    doubles_var->setValue(doubles_val);
    doubles_var->lock();
    test_dom->addChild(doubles_var);
    KssSimpleVariable * double_var = new KssSimpleVariable("double");
    double_var->setValue(new KsDoubleValue(42));
    double_var->lock();
    test_dom->addChild(double_var);
    _root_domain.addChild(test_dom);

    KssSimpleDomain *big_dom = new KssSimpleDomain("big");
    for (size_t j = 0; j < 1000; ++j) {
        KsString varid(KsString::fromInt(j));
        
        KssSimpleVariable *var = 
            new KssSimpleVariable(varid);
        var->setValue(new KsIntValue(j*j));
        var->setState(KS_ST_GOOD);
        big_dom->addChild(var);
    }
    _root_domain.addChild(big_dom);
    _root_domain.addChild(new TestDomain);
    
    // using convenience functions:
    addDomain(KsPath("/"), "restricted", "Access restricted");
    addStringVar(KsPath("/restricted"), 
                 "writeme", 
                 "the default value",
                 "that tcls",
                 false);
}




//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

static const char restricted[] = "/restricted";

//////////////////////////////////////////////////////////////////////

class TestAvNone
: public KsAvNoneTicket
{
public:
    TestAvNone(XDR* xdr, bool & ok) : KsAvNoneTicket(xdr,ok) { }
    KS_ACCESS getAccess(const KsString &) const;
    static KsAvTicket * xdrNew(XDR *);
};

//////////////////////////////////////////////////////////////////////

class TestAvSimple
: public KsAvSimpleTicket
{
public:
    TestAvSimple(XDR* xdr, bool & ok) : KsAvSimpleTicket(xdr,ok) { }
    bool isVisible(const KsString & name) const;
    KS_ACCESS getAccess(const KsString & name) const
        { return KsAvTicket::getAccess(name); }
    bool canReadVar(const KsString & name) const;
    bool canWriteVar(const KsString & name) const;
    static KsAvTicket * xdrNew(XDR *);
};

//////////////////////////////////////////////////////////////////////

KS_IMPL_XDRNEW2(KsAvTicket,TestAvNone);
KS_IMPL_XDRNEW2(KsAvTicket,TestAvSimple);

//////////////////////////////////////////////////////////////////////

KS_ACCESS
TestAvNone::getAccess(const KsString & name) const
{
    if (strncmp(name, restricted, sizeof restricted - 1) == 0) {
        return KS_AC_NONE;
    } else {
        return KS_AC_READ | KS_AC_WRITE;
    }
}

//////////////////////////////////////////////////////////////////////

bool
TestAvSimple::isVisible(const KsString & name) const
{
    if (name == restricted) {
        // check id
        if (_id == "reader" || _id == "writer") {
            return true;
        } else {
            return false;
        }
    } else {
        // delegate to base class
        return KsAvSimpleTicket::isVisible(name);
    }
}

//////////////////////////////////////////////////////////////////////

bool
TestAvSimple::canReadVar(const KsString & name) const
{
    if (strncmp(name, restricted, sizeof restricted - 1) == 0) {
        // check id
        if (_id == "reader" || _id == "writer") {
            return true;
        } else {
            return false;
        }
    } else {
        // delegate to base class
        return KsAvSimpleTicket::canReadVar(name);
    }
}

//////////////////////////////////////////////////////////////////////

bool
TestAvSimple::canWriteVar(const KsString & name) const
{
    if (strncmp(name, restricted, sizeof restricted - 1) == 0) {
        // check id
        if (_id == "writer") {
            return true;
        } else {
            return false;
        }
    } else {
        // delegate to base class
        return KsAvSimpleTicket::canWriteVar(name);
    }
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

int main(int, char **) {
    PltCerrLog log("tserver");
    KsAvTicket::registerAvTicketType(KS_AUTH_NONE, TestAvNone::xdrNew);
    KsAvTicket::registerAvTicketType(KS_AUTH_SIMPLE, TestAvSimple::xdrNew);
	TestServer ts;
    ts.startServer();
	ts.run();
    ts.stopServer();
    return 0;
}

// EOF tserver1.cpp
