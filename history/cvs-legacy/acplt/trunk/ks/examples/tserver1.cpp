/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/examples/tserver1.cpp,v 1.11 1997-09-15 18:45:18 martin Exp $ */
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
#include <stdlib.h>


//
//  Signal handler: Signals shut down the server
//
extern "C" void handler(int) {
    KsServerBase::getServerObject().downServer();
}

//
//  Our test server. It inherits SimpleServer behaviour in addition
//  to the server behaviour.
//  SimpleServer uses recursive descent to locate communication objects.
//
class TestServer
: public KsServer,
  public KsSimpleServer
{	
public:
	TestServer();

    //// accessors
    //
    // these define the name and version of this server
    //
    virtual KsString getServerName() const
        { return KsString("tserver"); }

    virtual KsString getServerVersion() const
        { return KsString("1.0"); }

    virtual KsString getServerDescription() const 
        { return KsString("ACPLT/KS test server"); }

    virtual KsString getVendorName () const
        { return KsString("Lehrstuhl fuer Prozessleittechnik, "
                          "RWTH Aachen"); }

    //
    // To use data exchange, you must replace the exgdata service
    // method. The default exgData method returns 'not implemented'
    // which is not desired here...
    //
    virtual void exgData(KsAvTicket &ticket,
                         const KsExgDataParams &params,
                         KsExgDataResult &result);                         
    
    //
    // The TestServer::exgData method calls calculate between
    // setting and reading the variables.
    // 
    virtual bool calculate();
private:
    //
    //  The following pointers allow faster access to the
    //  variables used in data exchange. Note that lifetime of
    //  the objects is controlled by the lifetime of the domain
    //  containing the variables! (I.e. the pointers may be invalid
    //  when the containing domain dies.
    // 
    KssSimpleVariable *pVar_x;
    KssSimpleVariable *pVar_xsquared;
};

//////////////////////////////////////////////////////////////////////
//
// This is a dummy iterator. It does not return any child.
// Because TestDomain (see below) contains MANY MANY children
// it would make no sense to have an iterator.
//
class TestDomainIterator
: public KssDomainIterator
{
public:
    // portability strikes back.
#if PLT_RETTYPE_OVERLOADABLE
    typedef TestDomainIterator THISTYPE;
#define TestDomainIterator_THISTYPE TestDomainIterator
#else
#define TestDomainIterator_THISTYPE PltIterator_<KssCommObjectHandle> 
// KssDomainIterator_THISTYPE
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
//
// This is a domain which contains an infinite count of children (modulo
// computer's restrictions). It contains children 'n' where n is the string
// representation for a natural number. The value of each child is n^2.
//
// In addition there are some test variables which trigger an action when
// read. Read the source carefully to determine which.
//
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
    virtual TestDomainIterator_THISTYPE * newIterator() const;

    virtual KssCommObjectHandle getChildById(const KsString & id) const;

    // simulate RTTI when necessary
    PLT_DECL_RTTI;
};

//////////////////////////////////////////////////////////////////////

KssCommObjectHandle
TestDomain::getChildById(const KsString &str) const
{
    // construct the child
    KssSimpleVariable * var =
        new KssSimpleVariable(str);
    int j = atoi(str);
    if ( var ) {
        if (j != 0) {
            // the squares... set the variables current properties
            var->setValue(new KsIntValue(j*j));
            var->setState(KS_ST_GOOD);
            var->lock();
        } else {
            // the actions...
            if (str == "down") {
                cerr << endl << "Shutdown requested." << endl;
                TestServer::getServerObject().downServer();
            } else if (str == "exit") {
                cerr << endl << "Hard exit requested. Bye!" << endl;
                exit(1);
            } else if (str == "lockup") {
                cerr << endl << "Simulating lockup..." << endl;
                PltTime(40,0).sleep();
                cerr << endl << "... exiting." << endl;
                exit(1);
            } else if (strncmp(str,"sleep",5)==0) {
                PltTime delay(atoi(str+5),0);
                delay.sleep();
            }
            // and indicate that there is no meaningful value for
            // this child.
            var->setValue(new KsVoidValue);
            var->setState(KS_ST_BAD);
            var->lock();            
        }
    }
    //
    //  "handle" this object indicating that it has been allocated with the
    //  new operator. The handle owns the variable and will destroy it
    //  when it is not referenced any more.
    //
    return KssCommObjectHandle(var, KsOsNew);
}

//////////////////////////////////////////////////////////////////////
// Generate RTTI if needed (one base class for TestDomain, namely
// KssDomain).
PLT_IMPL_RTTI1(TestDomain, KssDomain);
//////////////////////////////////////////////////////////////////////

TestDomainIterator_THISTYPE * 
TestDomain::newIterator() const
{ 
    return new TestDomainIterator; 
}

//////////////////////////////////////////////////////////////////////
//
// The constructor of the server.
//
TestServer::TestServer()
: KsServer(30)
{
    // 
    // When requests are made with authentification method "none"
    // objects that don't restrict access further can be read and
    // written to.
    //
    KsAvNoneTicket::setDefaultAccess(KS_AC_READ | KS_AC_WRITE);
    
    //
    // construct the /vendor tree (uses the virtual constants specified
    // in the class declaration)
    //
    initVendorTree();

    //
    // catch some signals
    //
    signal(SIGINT, handler);
    signal(SIGTERM, handler);
#if !PLT_SYSTEM_NT
    signal(SIGHUP, handler);
#endif
    
    //
    // create additional communication objects, doing it the hard way.
    // Note that error handling is incomplete here ( TODO )
    //

    // a domain
    KssSimpleDomain * test_dom = new KssSimpleDomain("test");
    // a writable variable
    KssSimpleVariable * write_me = new KssSimpleVariable("write_me");
    // no value, yet
    write_me->setValue(new KsVoidValue);
    // add it to the domain
    test_dom->addChild(write_me);
    // a vector of 1000 doubles
    KssSimpleVariable * doubles_var = new KssSimpleVariable("doubles");
    KsDoubleVecValue * doubles_val = 
        new KsDoubleVecValue(1000);
    for (size_t i = 0 ; i < doubles_val->size(); ++i) {
        (*doubles_val)[i] = i*i;
    }
    doubles_var->setValue(doubles_val);
    // write protection on!
    doubles_var->lock();
    // add it to the domain
    test_dom->addChild(doubles_var);
    // and so on...
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
    
    //
    // Now using convenience functions. No hard errors will happen
    // here, but you should still check the return values of this
    // calls. False would indicate failure.
    // A little bit easier this way, isn't it?
    //
    addDomain(KsPath("/"), "restricted", "Access restricted");
    addStringVar(KsPath("/restricted"), 
                 "writeme", 
                 "the default value",
                 "that tcls",
                 false);

    //// construct data exchange demo domain
    //   variables
    pVar_x = new KssSimpleVariable("x");
    pVar_xsquared = new KssSimpleVariable("xsquared");
    KssCommObjectHandle hVar_x(pVar_x, KsOsNew);
    KssCommObjectHandle hVar_xsquared(pVar_xsquared, KsOsNew);
    if (hVar_x && hVar_xsquared) {
        if (pVar_x->setValue(new KsSingleValue(0)) == KS_ERR_OK) {
            pVar_x->setState(KS_ST_GOOD);
        }
        if (pVar_xsquared->setValue(new KsSingleValue(0)) == KS_ERR_OK) {
            pVar_xsquared->setState(KS_ST_GOOD);
        }
        // domain
        addDomain(KsPath("/"), "exchange", "Data exchange demo");
        KsPath exchange("/exchange");
        if (! (addCommObject(exchange, hVar_x) &&
               addCommObject(exchange, hVar_xsquared))) {
            // The variables will die when hVar_... goes out of scope.
            // We disable our pointers to avoid using them any longer.
            pVar_x = pVar_xsquared = 0;
        }
    } else {
        // Don't use pointers to nonexistant objects!
        pVar_x = pVar_xsquared =0;
    }    

}

//////////////////////////////////////////////////////////////////////
//
// Data exchange is easily implemented. If you do complicated things
// you might have to do it a little smarter.
//
// exgData is implemented as wrapper around setVar and getVar
// calling compute() in between. Some attention has to be paid to
// error handling.

void 
TestServer::exgData(KsAvTicket &ticket,
                    const KsExgDataParams &params,
                    KsExgDataResult &result)
{
    size_t setsz = params.set_vars.size();
    size_t getsz = params.get_vars.size();

    KsSetVarParams setvar_params(setsz);
    KsSetVarResult setvar_result(setsz);

    setvar_params.items = params.set_vars;
    setVar(ticket, setvar_params, setvar_result);
    if (setvar_result.result == KS_ERR_OK) {
        // setvar ok, continue
        calculate();

        KsGetVarParams getvar_params(getsz);
        KsGetVarResult getvar_result(getsz);
        getvar_params.identifiers = params.get_vars;
        getVar(ticket, getvar_params, getvar_result);
    
        result.results = setvar_result.results;
        result.items = getvar_result.items;

        result.result = getvar_result.result;
    } else {
        // abort and return error
        result.result = setvar_result.result;
    }
}


//////////////////////////////////////////////////////////////////////

bool
TestServer::calculate()
{
    if (pVar_x && pVar_xsquared) {
        // the variables are 'physikalisch vorhanden'(TM)
        // get a pointer to the value
        KsValue * tmp = pVar_x->getValue().getPtr();
        // assume it is a KsSingleValue...
        KsSingleValue * pVal_x = 
            PLT_DYNAMIC_PCAST(KsSingleValue, tmp);
        if (pVal_x) {
            // oh yes, it is. Square it and save the square in xsquared.
            float x = *pVal_x;
            pVar_xsquared->setValue(new KsSingleValue(x*x));
            pVar_xsquared->setState(KS_ST_GOOD);
            return true;
        } else {
            // no, no, no... someone has written something else
            // into x . No value in this case...
            pVar_xsquared->setValue(new KsVoidValue);
            pVar_xsquared->setState(KS_ST_BAD);
            return true;
        }
    }
    return false;
}

//////////////////////////////////////////////////////////////////////
// Access restrictions:
//
// Derive from the AvTicket classes extending the can... methods.
//////////////////////////////////////////////////////////////////////

// static 
const char restricted[] = "/restricted";

//////////////////////////////////////////////////////////////////////

class TestAvNone
: public KsAvNoneTicket
{
public:
    TestAvNone(XDR* xdr, bool & ok) : KsAvNoneTicket(xdr,ok) { }
    virtual bool canReadVar(const KsString & name) const;
    virtual bool canWriteVar(const KsString & name) const;
    static KsAvTicket * xdrNew(XDR *);
};

//////////////////////////////////////////////////////////////////////

class TestAvSimple
: public KsAvSimpleTicket
{
public:
    TestAvSimple(XDR* xdr, bool & ok) : KsAvSimpleTicket(xdr,ok) { }
    bool isVisible(const KsString & name) const;
    bool canReadVar(const KsString & name) const;
    bool canWriteVar(const KsString & name) const;

    static KsAvTicket * xdrNew(XDR *);
};

//////////////////////////////////////////////////////////////////////

KS_IMPL_XDRNEW2(KsAvTicket,TestAvNone);
KS_IMPL_XDRNEW2(KsAvTicket,TestAvSimple);

//////////////////////////////////////////////////////////////////////

bool
TestAvNone::canReadVar(const KsString & name) const
{
    //
    // You can't read /restricted with AvNone.
    //
    if (strncmp(name, restricted, sizeof(restricted) - 1) == 0) {
        return false;
    } else {
        return KsAvNoneTicket::canReadVar(name);
    }
}

//////////////////////////////////////////////////////////////////////

bool
TestAvNone::canWriteVar(const KsString & name) const
{
    //
    // You can't write /restricted with AvNone.
    //
    if (strncmp(name, restricted, sizeof restricted - 1) == 0) {
        return false;
    } else {
        return KsAvNoneTicket::canWriteVar(name);
    }
}

//////////////////////////////////////////////////////////////////////

bool
TestAvSimple::isVisible(const KsString & name) const
{
    //
    // You can see /restricted only if you are "reader" or "writer"
    //
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
    //
    // You can read /restricted only if you are "reader" or "writer"
    //
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
    //
    // You can write /restricted only if you are "writer"
    //
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
// Startup code:
//////////////////////////////////////////////////////////////////////

int main(int, char **) {
    //
    // Use the standard error output for logging.
    // (This is the portable logger)
    //
    PltCerrLog log("tserver");

    //
    // Register the extended authentification ticket classes.
    // The will be used instead of the builtin ones.
    //
    KsAvTicket::registerAvTicketType(KS_AUTH_NONE, TestAvNone::xdrNew);
    KsAvTicket::registerAvTicketType(KS_AUTH_SIMPLE, TestAvSimple::xdrNew);

    //
    // Create the server object
    //
	TestServer ts;

    //
    // Start it.
    //
    ts.startServer();
    PltLog::Info("Started.");
    //
    // Enter the service loop.
    //
	ts.run();
    //
    // After leaving the service loop clean up and stop serving.
    //
    PltLog::Info("Stopping...");
    ts.stopServer();
    PltLog::Info("Stopped.");
    //
    // Return to the operating system.
    //
    return 0;
}

// EOF tserver1.cpp
