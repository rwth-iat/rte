/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/examples/tserver1.cpp,v 1.33 2007-04-25 12:57:20 martin Exp $ */
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

/* Author: Martin Kneissl <martin@plt.rwth-aachen.de> */

#include "ext_sp.h"

#include "ks/server.h"
#include "ks/simpleserver.h"
#include "ks/histdomain.h"
#include "plt/log.h"
#include "plt/logstream.h"
#include "ks/interserver.h"

#include <signal.h>
#include <stdlib.h>
#include <time.h>

#include <iomanip.h>

//
//  Signal handler: Signals shut down the server
//
extern "C" void handler(int) {
    KsServerBase::getServerObject().downServer();
}


/////////////////////////////////////////////////////////////////////////////
// some constants needed for protocol extension
//
const u_long ext_major_opcode = 0x0010; // start with first free extension
const u_long KS_CROBJ = 
  (ext_major_opcode << 16) | KS_CROBJ_MINOR_OPCODE;  

/////////////////////////////////////////////////////////////////////////////
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
	TestServer(int port = KS_ANYPORT);

    //// accessors
    //
    // these define the name and version of this server
    //
    virtual KsString getServerName() const
        { return KsString("tserver"); }

    virtual KsString getServerVersion() const
        { return KsString(KS_VERSION_STRING); }

    virtual KsString getServerDescription() const 
        { return KsString("ACPLT/KS test server"); }

    virtual KsString getVendorName () const
        { return KsString("Lehrstuhl fuer Prozessleittechnik, "
                          "RWTH Aachen"); }

    //
    // To implement a protocol extension we have to override
    // this function.
    //   
    void dispatch(u_long serviceId,
                  KssTransport &transport,
                  XDR *xdrIn,
                  KsAvTicket &ticket);

    //
    // The protocol extension allows to create new objects.
    //
    void createObject(KsAvTicket &ticket,
                      const KsCreateParams &params,
                      KsCreateResult &result);

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
#define TestDomainIterator_THISTYPE KssDomainIterator_THISTYPE
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
    virtual KS_SEMANTIC_FLAGS getSemanticFlags() const
        { return 0; }
    virtual KsString getClassIdentifier() const
        { return KsString(""); }

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




// ----------------------------------------------------------------------------
//
class DummyHistory : public KssHistoryDomain
{
public:
    DummyHistory(KsString id);

    virtual void getHist(const KsGetHistParams &params,
                         KsGetHistSingleResult &result);
    virtual KS_SEMANTIC_FLAGS getSemanticFlags() const
        { return 0; }

}; // class DummyHistory


DummyHistory::DummyHistory(KsString id)
    : KssHistoryDomain(id, KS_HT_SINGLE, KS_IPM_NONE, KS_IPM_NONE,
		       KsTime::now(), "A (nearly) useless history")
{
    addPart("value", KS_VT_SINGLE, "value track");
    addPart("t", KS_VT_TIME, "time stamp track");
    addPart("state", KS_VT_STATE, "value state track");
    addPart("whatever", KS_VT_STRING, "useless track");
} // DummyHistory::DummyHistory


void
DummyHistory::getHist(const KsGetHistParams &params,
		      KsGetHistSingleResult &result)
{
    result.result = KS_ERR_GENERIC;
} // DummyHistory::getHist


#if PLT_USE_BUFFERED_STREAMS

class Wecker : public KsTimerEvent, public KssInterKsServerConnection {
public:
    Wecker() 
	: KsTimerEvent(PltTime::now(1)),
	KssInterKsServerConnection("localhost", "MANAGER")  
    { }
    ~Wecker() {  }
    virtual void trigger();
    virtual void async_attention(KssInterKsServerConnectionOperations op);

};


void Wecker::trigger()
{
    PltLog::Info("request timer event triggered");
    switch ( getState() ) {
    case ISC_STATE_CLOSED:
	//
	// In case the connection is not already open, open it now.
	//
	PltLog::Info("beginning open in the background");
	if ( !open() ) {
	    PltLog::Error("can not open interserver connection");
	    _trigger_at = KsTime::now(10);
	    KsServerBase::getServerObject().addTimerEvent(this);
	}
	break;
	
    case ISC_STATE_OPEN: {
	//
	// Send an ACPLT/KS GETVAR request.
	//
    	PltLogStream ls;
        ls << "Protocol version spoken by ACPLT/KS server="
           << getProtocolVersion();
        ls.info();
    	
	KsGetVarParams getvar(2);
	getvar.identifiers[0] = "/vendor/name";
	getvar.identifiers[1] = "/vendor/server_time";
	if ( !send(KS_GETVAR, getvar) ) {
	    PltLog::Error("can not send GETVAR request");
	    close();
	    _trigger_at = KsTime::now(10);
	    KsServerBase::getServerObject().addTimerEvent(this);
	} else {
	    PltLog::Info("GETVAR request sent");
	}
    }
    break;
    
    case ISC_STATE_BUSY:
	break; // don't do anything now.
    }
}


void Wecker::async_attention(KssInterKsServerConnectionOperations op)
{
    PltLogStream ls;

    ls << "operation=" << (op == ISC_OP_OPEN ? "open" : "call")
       << " state=";
    switch ( getState() ) {
    case ISC_STATE_CLOSED:
	ls << "closed"; break;
    case ISC_STATE_OPEN:
	ls << "open"; break;
    case ISC_STATE_BUSY:
	ls << "busy (not allowed!)"; break;
    }
    ls << " result=0x" << hex << _result << dec;
    ls.info();

    if ( op == ISC_OP_OPEN ) {
	//
	// An open() was completed in the background.
	//
	if ( getState() != ISC_STATE_OPEN ) {
	    //
	    // open() has finally failed.
	    //
	    PltLog::Error("open failed");
	    _trigger_at = KsTime::now(10);
	    KsServerBase::getServerObject().addTimerEvent(this);
	} else {
	    //
	    // The open() succeeded, so send now the first GETVAR request.
	    //
	    trigger(); // let this sucker do the work.
	}
    } else {
	//
	// A send() was completed in the background. Now try to deserialize
	// the reply. Make sure that we *have* received a reply, that is,
	// getLastResult() must return KS_ERR_OK.
	//
	KsGetVarResult getvarresult;

	if ( (getLastResult() != KS_ERR_OK) ||
	     !receive(getvarresult) ) {
	    close();
	    PltLog::Error("could not receive ACPLT/KS GETVAR reply");
	    trigger();
	    return;
	}

	if ( getvarresult.result == KS_ERR_OK ) {
	    KsTime             time;
	    time_t             value_time;
	    size_t             size, i;
	    const KsVarCurrProps *curr_props;
	    int                   idx;

	    for ( idx = 0; idx < getvarresult.items.size(); ++idx ) {

		if ( getvarresult.items[idx].result != KS_ERR_OK ) {
		    ls << "reading variable: result=0x" << hex
		       << getvarresult.items[idx].result << dec;
		    ls.error();
		    continue;
		}

		curr_props = (KsVarCurrProps *)
		    getvarresult.items[idx].item.getPtr();

		switch ( curr_props->value->xdrTypeCode() ) {
		case KS_VT_VOID:
		    ls << "void";
		    break;
		case KS_VT_BOOL:
		    ls << "bool "
		       << ((bool) ((KsBoolValue &) *curr_props->value) ? 
			   "true" : "false");
		    break;
		case KS_VT_INT:
		    ls << "integer " 
		       << (long) ((KsIntValue &) *curr_props->value);
		    break;
		case KS_VT_UINT:
		    ls << "unsigned integer " 
		       << (u_long) ((KsUIntValue &) *curr_props->value);
		    break;
		case KS_VT_SINGLE:
		    ls << "single " 
		       << (float) ((KsSingleValue &) *curr_props->value);
		    break;
		case KS_VT_DOUBLE:
		    ls << "double " 
		       << (double) ((KsDoubleValue &) *curr_props->value);
		    break;
		case KS_VT_STRING:
		    ls << "string \"";
		    if ( ((KsStringValue &) *curr_props->value).len() > 50 ) {
			ls << ((KsStringValue &) *curr_props->value).substr(0, 50)
			     << "...";
		    } else {
			ls << (const char *) ((KsStringValue &) *curr_props->value);
		    }
		    ls << "\"";
		    break;
		case KS_VT_TIME:
		    time = ((KsTimeValue &) *curr_props->value);
		    value_time = time.tv_sec;
		    ls << "time " 
		       << PltString(ctime(&value_time)).substr(4, 15);
		    break;
		case KS_VT_BYTE_VEC:
		    size = ((KsByteVecValue &) *curr_props->value).size();
		    ls << "byte vector <" << size << "> {";
		    if ( size > 5 ) {
			size = 5;
		    }
		    for ( i = 0; i < size; ++i ) {
			ls <<  hex << setfill('0') << setw(2)
			   << (unsigned int) ((KsByteVecValue &) *curr_props->value)[i]
			   << dec << ",";
		    }
		    ls << "...}";
		    break;
		case KS_VT_BOOL_VEC:
		    size = ((KsByteVecValue &) *curr_props->value).size();
		    ls << "bool vector <" << size << "> {";
		    if ( size > 5 ) {
			size = 5;
		    }
		    for ( i = 0; i < size; ++i ) {
			ls <<  hex << setfill('0') << setw(2)
			   << ((bool) ((KsBoolVecValue &) *curr_props->value)[i] ? "true" : "false")
			   << dec << ",";
		    }
		    ls << "...}";
		    break;
		case KS_VT_INT_VEC:
		    size = ((KsIntVecValue &) *curr_props->value).size();
		    ls << "integer vector <" << size << "> {";
		    if ( size > 5 ) {
			size = 5;
		    }
		    for ( i = 0; i < size; ++i ) {
			ls << ((KsIntVecValue &) *curr_props->value)[i]
			   << ",";
		    }
		    ls << "...}";
		    break;
		case KS_VT_UINT_VEC:
		    size = ((KsUIntVecValue &) *curr_props->value).size();
		    ls << "unsigned integer vector <" << size << "> {";
		    if ( size > 5 ) {
			size = 5;
		    }
		    for ( i = 0; i < size; ++i ) {
			ls << ((KsUIntVecValue &) *curr_props->value)[i]
			   << ",";
		    }
		    ls << "...}";
		    break;
		case KS_VT_SINGLE_VEC:
		    size = ((KsSingleVecValue &) *curr_props->value).size();
		    ls << "single vector <" << size << "> {";
		    if ( size > 5 ) {
			size = 5;
		    }
		    for ( i = 0; i < size; ++i ) {
			ls << ((KsSingleVecValue &) *curr_props->value)[i]
			   << ",";
		    }
		    ls << "...}";
		    break;
		case KS_VT_DOUBLE_VEC:
		    size = ((KsDoubleVecValue &) *curr_props->value).size();
		    ls << "double vector <" << size << "> {";
		    if ( size > 5 ) {
			size = 5;
		    }
		    for ( i = 0; i < size; ++i ) {
			ls << ((KsDoubleVecValue &) *curr_props->value)[i]
			   << ",";
		    }
		    ls << "...}";
		    break;
		case KS_VT_STRING_VEC:
		    size = ((KsStringVecValue &) *curr_props->value).size();
		    ls << "string vector <" << size << "> {";
		    if ( size > 5 ) {
			size = 5;
		    }
		    for ( i = 0; i < size; ++i ) {
			if ( ((KsStringVecValue &) *curr_props->value)[i].len() > 50 ) {
			    ls << ((KsStringVecValue &) *curr_props->value)[i].substr(0, 50)
			       << "...";
			} else {
			    ls << (const char *) ((KsStringVecValue &) *curr_props->value)[i];
			}
			ls << ",";
		    }
		    ls << "...}";
		    
		    break;
		case KS_VT_TIME_VEC:
		    size = ((KsTimeVecValue &) *curr_props->value).size();
		    ls << "time vector <" << size << "> {";
		    if ( size > 5 ) {
			size = 5;
		    }
		    for ( i = 0; i < size; ++i ) {
			time = ((KsTimeVecValue &) *curr_props->value)[i];
			value_time = time.tv_sec;
			ls << PltString(ctime(&value_time)).substr(4, 15)
			   << ",";
		    }
		    ls << "...}";
		    
		    break;
		default:
		    ls << "???";
		}
		ls.info();
	    }
	} else {
	    PltLog::Error("ACPLT/KS service failed");
	}

	_trigger_at = KsTime::now(20);
	KsServerBase::getServerObject().addTimerEvent(this);
    }
}

#endif


//////////////////////////////////////////////////////////////////////
//
// The constructor of the server.
//
TestServer::TestServer(int port)
: KsServer(30, port)
{
    setBufferSizes(131072, 131072);
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

#if 1
    // a domain
    KssSimpleDomain * test_dom = new KssSimpleDomain("test");
    KssDomainHandle htest_dom(test_dom, KsOsNew);
    KssSimpleLinkAlias *test_dom_alias = 
        new KssSimpleLinkAlias(htest_dom, "test_links");

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
    _root_domain.addChild(*(KssCommObjectHandle*)&htest_dom);
    _root_domain.addChild(test_dom_alias);

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
    _root_domain.addChild(new DummyHistory("timeseries"));
    
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

    addDomain(KsPath("/"), "newobjects", 
              "Area of dynamically created objects");
    addDomain(KsPath("/vendor"), "extensions");
    addDomain(KsPath("/vendor/extensions"), "ks_test");
    addDomain(KsPath("/vendor/extensions"), "ks_core");

    KssCommObjectHandle hext_dom = _root_domain.getChildByPath(KsPath("/vendor/extensions"));
    KssSimpleLinkAlias *vendorext_alias =
        new KssSimpleLinkAlias(*(KssDomainHandle*)&hext_dom, 
                               "extensions_link",
                               KsTime::now(),
                               "", "/vendor/extensions");
    _root_domain.addChild(vendorext_alias);

    KsPath ep("/vendor/extensions/ks_test");

    KssSimpleVariable *tmp_var = new KssSimpleVariable("major_opcode");
    tmp_var->setValue(new KsIntValue(ext_major_opcode));
    addCommObject(ep, KssCommObjectHandle(tmp_var, KsOsNew));

    tmp_var = new KssSimpleVariable("services");
    KsStringVecValue *services_vec = new KsStringVecValue(1);
    (*services_vec)[0] = KsString("createObject");
    tmp_var->setValue(services_vec);
    addCommObject(ep, KssCommObjectHandle(tmp_var, KsOsNew));

    KsPath ec("/vendor/extensions/ks_core");

    tmp_var = new KssSimpleVariable("major_opcode");
    tmp_var->setValue(new KsIntValue(0));
    addCommObject(ec, KssCommObjectHandle(tmp_var, KsOsNew));

    tmp_var = new KssSimpleVariable("services");
    KsStringVecValue * core_services_val
            = new KsStringVecValue(5);
    (*core_services_val)[0] = "ObjDict";
    (*core_services_val)[1] = "Var";
    (*core_services_val)[2] = "ObjMgmnt";
    (*core_services_val)[3] = "LinkMgmnt";
    (*core_services_val)[4] = "Manager";
    tmp_var->setValue(core_services_val);
    tmp_var->lock();
    addCommObject(ec, KssCommObjectHandle(tmp_var, KsOsNew));
#endif

    KsPath encoded("/encoded");
    addDomain(KsPath("/"), "encoded", "contains objects with dump names");
    addStringVar(encoded, "slash/slash", "this has no value at all.",
                 "object name contains a slash");
    addStringVar(encoded, "slash/.dot", "./", "well-known rumor site");
    addStringVar(encoded, "%AN-OPENVMS-COMPATIBLE-IDENT-$/{}~", "forget it",
                 "no comment");
    KsPath xx("/encoded/MessStellen/AA%2FAA");
    KsPath xxx("/encoded/MessStellen/AA%2FAA/MW");
    xx.decodePercents();
    xxx.decodePercents();
    addDomain(KsPath("/encoded"), "MessStellen");
    addDomain(KsPath("/encoded/MessStellen"), "AA/AA");
    addDomain(xx, "MW");
    addStringVar(xxx, "Austrag", "nope, nothing, nada, niente, njet");
    addStringVar(xxx, "Beschreibung", "Beschreibung");
    addStringVar(xxx, "Einheit", "MI-5");
    addStringVar(xxx, "Transformation", "XSLT, was sonst?!");
    
    KssSimpleVariable * state_var = new KssSimpleVariable("pv_with_state");
    state_var->setValue(new KsDoubleValue(42));
    state_var->setState(0x1234FFC3);
    state_var->lock();
    addCommObject(xxx, KssCommObjectHandle(state_var, KsOsNew));

#if PLT_USE_BUFFERED_STREAMS
    addTimerEvent(new Wecker);
#endif
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


/////////////////////////////////////////////////////////////////////////////
// Simple protocol extension:
//   KS_CROBJ creates a new variable with path /newobjects/<var_name>
//
void 
TestServer::dispatch(u_long serviceId,
                     KssTransport &transport,
                     XDR *xdrIn,
                     KsAvTicket &ticket)
{
    switch(serviceId) {

    case KS_CROBJ:
        {
	    bool ok = false;
	    KsCreateParams params(xdrIn, ok);
	    transport.finishRequestDeserialization(ticket, ok);
	    if ( ok ) {
            	// execute service function
            	KsCreateResult result;
            	createObject(ticket, params, result);
            	// send back result
            	transport.sendReply(ticket, result);
	    } else {
            	// not properly decoded
            	transport.sendErrorReply(ticket, KS_ERR_GENERIC);
	    }
        }
    	break;
    default:
	KsServer::dispatch(serviceId, transport, xdrIn, ticket);
    }
} // TestServer::dispatch


void 
TestServer::createObject(KsAvTicket &ticket,
                         const KsCreateParams &params,
                         KsCreateResult &result)
{
  if( addStringVar(KsPath("/newobjects"),
                   params.name,
                   KsString("Newly created")) ) {
    result.result = KS_ERR_OK;
  } else {
    result.result = KS_ERR_GENERIC;
  }  
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
    if ( !ts.isOk() ) {
        PltLog::Error("Could not initialize the tserver object.");
        return 42;
    }
    //
    // Start it.
    //
#if !PLT_SYSTEM_NT && !PLT_SYSTEM_OPENVMS
    signal(SIGPIPE, SIG_IGN);
#endif
    ts.startServer();
    if ( ts.isOk() ) {
        PltLog::Info("Started.");
        //
        // Enter the service loop.
        //
        ts.run();
        //
        // After leaving the service loop clean up and stop serving.
        //
        PltLog::Info("Stopping...");
    } else {
        PltLog::Error("Could not start the tserver object.");
    }
    ts.stopServer();
    PltLog::Info("Stopped.");
    //
    // Return to the operating system.
    //
    return ts.isOk() ? 0 : 42;
}

/* End of tserver1.cpp */
