/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/examples/tserver1.cpp,v 1.22 1999-02-25 17:12:57 harald Exp $ */
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

#if 0
// ---------------------------------------------------------------------------
// The class KssInterKsServerConnection provides client functionality to
// ACPLT/KS server, so servers can send ACPLT/KS service requests to other
// ACPLT/KS servers. This is done asynchronous during the i/o or idle slot
// and thus appears in the background.
//
class KssInterKsServerConnection : protected KssConnectionAttentionInterface
{
public:
    KssInterKsServerConnection(KsString host, KsString server);
    virtual ~KssInterKsServerConnection();

    //
    // Macro state of the connection.
    //
    enum KssInterKsServerConnectionState {
	ISC_STATE_CLOSED, // c. is closed
	ISC_STATE_OPEN,   // c. is open
	ISC_STATE_BUSY    // c. may be open and is currently in use (e.g. a
	                  // connection request may be on its way...)
    };

    //
    // Available asynchronous operations. You'll need them to find out what
    // has happened when this inter-server connection needs attention.
    //
    enum KssInterKsServerConnectionOperations {
	ISC_OP_OPEN, // open operation has been completed/failed
	ISC_OP_CALL  // service request has been completed/failed
    };

    //
    // Asynchronously open the connection (that is, in the background).
    //
    bool open();

    //
    // Immediately close the connection
    //
    void close();

    //
    // Send an ACPLT/KS message (request) to the wire:
    //   send() is just a C++ convenience,
    //   C-lovers please take the beginSend()/.../endSend() route.
    //
    bool send(u_long serviceid, KsXdrAble &params);
    bool beginSend(u_long serviceid);
    bool endSend();

    //
    // Receive an ACPLT/KS reply from the wire. Note that the same rules
    // for C++ versus C applies as mentioned just before.
    //
    bool receive(KsResult &result);
    bool beginReceive();
    bool endReceive();

    //
    // This method needs to be implemented in derived classes.
    //
    virtual void async_attention(KssInterKsServerConnectionOperations op) = 0;

    KssInterKsServerConnectionState getState() const { return _state; }
    KS_RESULT getLastResult() const { return _result; }

    //
    // The underlying XDR stream.
    //
    XDR *getXdr() const;

    //
    // Control timeout spans for connection establishment and sending a
    // RPC request/receiving the corresponding reply
    //
    void setTimeouts(unsigned long connectto, unsigned long callto)
        { _connect_timeout = connectto; _call_timeout = callto; }
    void getTimeouts(unsigned long &connectto, unsigned long &callto) const
        { connectto = _connect_timeout; callto = _call_timeout; }

protected:
    //
    // Substate of the connection. This state indicates what kind of
    // underlying connection is currently in use.
    //
    enum ISCSubState {
	ISC_SUBSTATE_NONE,
	ISC_SUBSTATE_CONNECTING_PMAP,
	ISC_SUBSTATE_CONNECTING_MANAGER,
	ISC_SUBSTATE_CONNECTING_SERVER
    };

    //
    // handler which is called by the internal connection
    //
    virtual bool attention(KssConnection &con);

    u_long makeXid();

    void activateConnection();
    void closeConnection();

    bool openPortmapperConnection();
    bool openManagerConnection(u_short port, int protocol);
    bool openServerConnection(u_short port);

    KssTCPXDRConnection             *_cln_con;
    bool                             _cln_con_once_closed;
    KssInterKsServerConnectionState  _state;
    ISCSubState                      _sub_state;

    KS_RESULT                        _result;

    KsString                         _host;
    u_short                          _host_port;
    struct sockaddr_in               _host_addr; 
    struct in_addr                   _old_ip;
    KsString                         _server;
    u_short                          _server_port;

    unsigned long                    _connect_timeout;
    unsigned long                    _call_timeout;
}; // class KssInterKsServerConnection


// ---------------------------------------------------------------------------
// Construct a new inter server connection object. This involves parsing the
// given host and server names for optional port numbers.
//
KssInterKsServerConnection::KssInterKsServerConnection(
    KsString host, KsString server)
    : _cln_con(0),
      _cln_con_once_closed(false),
      _state(ISC_STATE_CLOSED),
      _sub_state(ISC_SUBSTATE_NONE),
      _result(KS_ERR_OK),
      _host(host), _host_port(0),
      _server(server), _server_port(0),
      _connect_timeout(15), _call_timeout(30)
{
    const char *pColon;

    memset(&_old_ip, 0, sizeof(_old_ip));

    pColon = strchr(_host, ':');
    if ( pColon ) {
        //
        // Seems like the hostname contains an explicit port number. Split
	// the port number from the name and store both parts in _host_name
	// and _host_port. If there is anything wrong with this port number,
        // either because it's an invalid string or the value is out of
        // range, bail out with an malformed path error.
        //
	//
	int hostpart_len = pColon - (const char *) _host;
        if ( hostpart_len <= 0 ) {
	    _result = KS_ERR_MALFORMEDPATH;
	}
            
        char *pEnd;
        long port_no;

        ++pColon; // advance to the port number string
        port_no = strtol(pColon, &pEnd, 10);
        if ( *pColon && !*pEnd ) {
            if ( (port_no > 0l) && (port_no <= 65535l) ) {
                _host_port = (unsigned short) port_no;
		_host = _host.substr(0, hostpart_len);
            } else {
                _result = KS_ERR_MALFORMEDPATH;
            }
        } else {
            _result = KS_ERR_MALFORMEDPATH;
        }
    }
} // KssInterKsServerConnection::KssInterKsServerConnection


// ---------------------------------------------------------------------------
// Clean up the mess we've made during runtime...
//
KssInterKsServerConnection::~KssInterKsServerConnection()
{
    closeConnection(); // just in case...
} // KssInterKsServerConnection::~KssInterKsServerConnection


// ---------------------------------------------------------------------------
// Return a new transaction id for use with ONC/RPC. This one is identical
// with the algorithm used by the Sun ONC/RPC package.
//
u_long KssInterKsServerConnection::makeXid()
{
    PltTime now(PltTime::now());

    return getpid() ^ now.tv_sec ^ now.tv_usec;
} // KssInterKsServerConnection::makeXid


// ---------------------------------------------------------------------------
// Helper: given a (internal) connection, we make ourselves the handle for
// incomming attentions on this connection and put the connection under
// control of the connection manager.
//
void KssInterKsServerConnection::activateConnection()
{
    if ( _cln_con ) {
	_cln_con->setAttentionPartner(this);
	KsServerBase::getServerObject().getConnectionManager()->
	    addConnection(*_cln_con);
    }
} // KssInterKsServerConnection::activateConnection


// ---------------------------------------------------------------------------
// Helper: remove a connection from the control of the connection manager and
// then shut the connection down. Finally delete the connection object.
//
void KssInterKsServerConnection::closeConnection()
{
    if ( _cln_con ) {
	KsServerBase::getServerObject().getConnectionManager()->
	    removeConnection(*_cln_con);    
	_cln_con->shutdown();
	delete _cln_con;
	_cln_con = 0;
    }
    _cln_con_once_closed = true;
    _state = ISC_STATE_CLOSED;
} // KssInterKsServerConnection::closeConnection


// ---------------------------------------------------------------------------
// Open the connection to another ACPLT/KS server. This can only be done if
// the connection is currently closed (thus neither in open nor in busy
// state). We also take care of optionally contacting the ACPLT/KS manager
// behind a firewall using TCP/IP.
//
bool KssInterKsServerConnection::open()
{
    if ( _state != ISC_STATE_CLOSED ) {
	_result = KS_ERR_OK;
	return false; // Already open or busy. Close first.
    }
    //
    // Now try to resolve the given hostname, which can be either a DNS
    // name or a dotted address. Unfortunately, we can't currently do a
    // async DNS resolver request.
    //
    // Clean up first and zero out the internet address.
    //
    memset(&_host_addr, 0, sizeof(_host_addr));
    if ( !_host.len() ) {
	_result = KS_ERR_HOSTUNKNOWN;
	return false;
    }
    
    struct in_addr ip, ip_none, ip_any;
    ip.s_addr = inet_addr(_host);
    ip_none.s_addr = INADDR_NONE;
    ip_any.s_addr = INADDR_ANY;

    if ( (memcmp(&ip, &ip_none, sizeof(ip)) == 0)
	 || (memcmp(&ip, &ip_any, sizeof(ip)) == 0) ) {
	//
	// This was no valid dotted address. So try to resolve it as a DNS
	// name. Let's see...
	//
	struct hostent *he = gethostbyname((const char *) _host);
	if ( !he ) {
	    //
	    // The name lockup failed. So we can't do anything more...
	    //
	    _result = KS_ERR_HOSTUNKNOWN;
	    return false;
	}
	ip = *(struct in_addr *)(he->h_addr_list[0]);
	if ( (memcmp(&ip, &_old_ip, sizeof(ip)) == 0)
	     && he->h_addr_list[1] ) {
	    ip = *(struct in_addr *)(he->h_addr_list[1]);
	}
    }
    //
    // Okay. We've got the IP address.
    //
    _old_ip = ip;
    _host_addr.sin_family = AF_INET; // preset some things...
    _host_addr.sin_addr = ip;
    //
    // Depending on whether a port number has been specified for either the
    // host name or server name, we start at different points of the
    // contacting procedure.
    //
    if ( _server_port ) {
	return openServerConnection(_server_port);
    }
    if ( _host_port ) {
	return openManagerConnection(_host_port, IPPROTO_TCP);
    }
    return openPortmapperConnection();
} // KssInterKsServerConnection::open


// ---------------------------------------------------------------------------
// Close a connection. This can be done even when the connection is dead, or
// busy, or whatever...
//
void KssInterKsServerConnection::close()
{
    closeConnection();
} // KssInterKsServerConnection::close


// ---------------------------------------------------------------------------
//
XDR *KssInterKsServerConnection::getXdr() const
{
    if ( _cln_con ) {
	return _cln_con->getXdr();
    } else {
	return 0;
    }
} // KssInterKsServerConnection::getXdr


// ---------------------------------------------------------------------------
//
bool KssInterKsServerConnection::beginSend(u_long serviceid)
{
    if ( _state != ISC_STATE_OPEN ) {
	_result = KS_ERR_OK;
	return false;
    }
    //
    // Build the request in the XDR stream.
    //
    if ( !_cln_con->beginRequest(makeXid(),
				 KS_RPC_PROGRAM_NUMBER,
				 KS_PROTOCOL_VERSION,
				 serviceid) ) {
	_result = KS_ERR_GENERIC;
	return false;
    }
    _result = KS_ERR_OK;
    return true;
} // KssInterKsServerConnection::beginSend


// ---------------------------------------------------------------------------
// Finish building the request telegramme and put the connection into sending
// mode. Make sure to use the right timeout span.
//
bool KssInterKsServerConnection::endSend()
{
    if ( _state != ISC_STATE_OPEN ) {
	_result = KS_ERR_OK;
	return false;
    }

    _cln_con->sendRequest();
    _cln_con->setTimeout(_call_timeout);
    if ( !KsServerBase::getServerObject().getConnectionManager()->
	 trackConnection(*_cln_con) ) {
	_result = KS_ERR_GENERIC;
	return false;
    }
    _state = ISC_STATE_BUSY;
    _result = KS_ERR_OK;
    return true;
} // KssInterKsServerConnection::endSend


// ---------------------------------------------------------------------------
//
bool KssInterKsServerConnection::send(u_long serviceid, KsXdrAble &params)
{
    if ( !beginSend(serviceid) ) {
	return false;
    }
    //
    // Use the A/V NONE scheme at this time...
    //
    u_long avscheme = 0;
    bool ok = xdr_u_long(_cln_con->getXdr(), &avscheme);
    if ( avscheme == KS_AUTH_NONE ) {
	ok &= params.xdrEncode(_cln_con->getXdr());
    } else {
	ok = false;
    }
    if ( !ok ) {
	_result = KS_ERR_GENERIC;
	return false;
    }
    return endSend();
} // KssInterKsServerConnection::send


// ---------------------------------------------------------------------------
//
bool KssInterKsServerConnection::beginReceive()
{
    if ( _state != ISC_STATE_OPEN ) {
	_result = KS_ERR_OK;
	return false;
    }
    //
    // First check that the RPC reply header indicates success. Otherweise
    // the RPC layer already indicated trouble...
    //
    KsRpcHeader rpc_header = _cln_con->getRpcHeader();
    if ( (rpc_header._reply_status != KsRpcHeader::KsRpcMSGACCEPTED)
	 || (rpc_header._accept_status != KsRpcHeader::KsRpcSUCCESS) ) {
	_result = KS_ERR_NETWORKERROR;
	return false;
    }
    _result = KS_ERR_OK;
    return true;
} // KssInterKsServerConnection::beginReceive


// ---------------------------------------------------------------------------
// Afterwards reset the connection and thus the underlying XDR stream to free
// up some memory (at least free it for the fragment cache).
//
bool KssInterKsServerConnection::endReceive()
{
    if ( _state != ISC_STATE_OPEN ) {
	_result = KS_ERR_OK;
	return false;
    }
    KsServerBase::getServerObject().getConnectionManager()->
	resetConnection(*_cln_con);
    _result = KS_ERR_OK;
    return true;
} // KssInterKsServerConnection::endReceive


// ---------------------------------------------------------------------------
// Retrieve the answer from the connection. In case the RPC reply indicated
// an error on the RPC level, or we run out of memory or into trouble, this
// function returns false. If everything succeeds, it'll return true.
//
bool KssInterKsServerConnection::receive(KsResult &result)
{
    if ( !beginReceive() ) {
	return false; // _result already set.
    }
    //
    // Use the A/V NONE scheme at this time, and then deserialize the
    // result into an appropriate object.
    //
    u_long avdummy = 0;
    bool ok = xdr_u_long(_cln_con->getXdr(), &avdummy);
    ok &= result.xdrDecode(_cln_con->getXdr());
    if ( !ok ) {
	endReceive();
	_result = KS_ERR_GENERIC;
	return false;
    }
    if ( !endReceive() ) {
	return false;
    }
    _result = KS_ERR_OK;
    return true;
} // KssInterKsServerConnection::receive


// ---------------------------------------------------------------------------
// Initiate the connection establishment sequence to the portmapper. If some-
// thing goes wrong, then we'll return false, otherwise true.
//
bool KssInterKsServerConnection::openPortmapperConnection()
{
    //
    // Now initiate the opening sequence to the portmapper...
    //
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if ( sock < 0 ) {
	_result = KS_ERR_GENERIC;
	return false;
    }
    // IP address and address family are still valid...
    _host_addr.sin_port = htons(PMAPPORT);
    //
    // Create the encapsulating connection object which will handle the
    // protocol issues on this particular socket. Make sure that we didn't
    // run into a memory shortage.
    //
    _sub_state = ISC_SUBSTATE_CONNECTING_PMAP;
    _cln_con = new KssTCPXDRConnection(sock, _connect_timeout, 
				       _host_addr, sizeof(_host_addr),
				       KssConnection::CNX_TYPE_CLIENT);
    if ( !_cln_con || 
	 (_cln_con->getState() == KssConnection::CNX_STATE_DEAD) ) {
	if ( _cln_con ) {
	    _cln_con->shutdown();
	    delete _cln_con;
	    _cln_con = 0;
	} else {
	    ::close(sock);
	}
	_result = KS_ERR_GENERIC;
	return false;
    }
    //
    // Okay. We now have the socket as well as the connection object, so we
    // can now enter the connection establishment state.
    //
    _result = KS_ERR_OK;
    _state = ISC_STATE_BUSY;
    activateConnection();
    return true;
} // KssInterKsServerConnection::openPortmapperConnection


// ---------------------------------------------------------------------------
// Initiate the connection establishment sequence to the manager. If something
// goes boing, then we'll return false, otherwise true.
//
bool KssInterKsServerConnection::openManagerConnection(u_short port,
						       int protocol)
{
    //
    // Now initiate the opening sequence to the ACPLT/KS manager...
    //
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if ( sock < 0 ) {
	_result = KS_ERR_GENERIC;
	return false;
    }
    // IP address and address family are still valid...
    _host_addr.sin_port = htons(port);
    //
    // Create the encapsulating connection object which will handle the
    // protocol issues on this particular socket. Make sure that we didn't
    // run into a memory shortage.
    //
    _sub_state = ISC_SUBSTATE_CONNECTING_MANAGER;
    _cln_con = new KssTCPXDRConnection(sock, _connect_timeout, 
				       _host_addr, sizeof(_host_addr),
				       KssConnection::CNX_TYPE_CLIENT);
    if ( !_cln_con || 
	 (_cln_con->getState() == KssConnection::CNX_STATE_DEAD) ) {
	if ( _cln_con ) {
	    _cln_con->shutdown();
	    delete _cln_con;
	    _cln_con = 0;
	} else {
	    ::close(sock);
	}
	_result = KS_ERR_GENERIC;
	return false;
    }
    //
    // Okay. We now have the socket as well as the connection object, so we
    // can now enter the connection establishment state.
    //
    _result = KS_ERR_OK;
    _state = ISC_STATE_BUSY;
    activateConnection();
    return true;
} // KssInterKsServerConnection::openManagerConnection


// ---------------------------------------------------------------------------
// Initiate the connection establishment sequence to the manager. If something
// goes boing, then we'll return false, otherwise true.
//
bool KssInterKsServerConnection::openServerConnection(u_short port)
{
    //
    // Now initiate the opening sequence to an ACPLT/KS server...
    //
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if ( sock < 0 ) {
	_result = KS_ERR_GENERIC;
	return false;
    }
    // IP address and address family are still valid...
    _host_addr.sin_port = htons(port);
    //
    // Create the encapsulating connection object which will handle the
    // protocol issues on this particular socket. Make sure that we didn't
    // run into a memory shortage.
    //
    _sub_state = ISC_SUBSTATE_CONNECTING_SERVER;
    _cln_con = new KssTCPXDRConnection(sock, _connect_timeout, 
				       _host_addr, sizeof(_host_addr),
				       KssConnection::CNX_TYPE_CLIENT);
    if ( !_cln_con || 
	 (_cln_con->getState() == KssConnection::CNX_STATE_DEAD) ) {
	if ( _cln_con ) {
	    _cln_con->shutdown();
	    delete _cln_con;
	    _cln_con = 0;
	} else {
	    ::close(sock);
	}
	_result = KS_ERR_GENERIC;
	return false;
    }
    //
    // Okay. We now have the socket as well as the connection object, so we
    // can now enter the connection establishment state.
    //
    _result = KS_ERR_OK;
    _state = ISC_STATE_BUSY;
    activateConnection();
    return true;
} // KssInterKsServerConnection::openServerConnection


// ---------------------------------------------------------------------------
// Okay. Now for the biggest method: this sucker is responsible to act like a
// (sometimes) finite automata. Here we control the connection sequences as
// well as sending and receiving RPC telegrammes.
//
bool KssInterKsServerConnection::attention(KssConnection &con)
{
    KssConnection::ConnectionState state = con.getState();

#if 0
    cout << endl << "connection state: ";
    switch ( state ) {
    case KssConnection::CNX_STATE_CONNECTING:
	cout << "CONNECTED."; break;
    case KssConnection::CNX_STATE_CONNECTED:
	cout << "CONNECTED."; break;
    case KssConnection::CNX_STATE_CONN_FAILED:
	cout << "CONNECTION NOT ESTABLISHED."; break;
    case KssConnection::CNX_STATE_READY:
	cout << "READY."; break;
    case KssConnection::CNX_STATE_READY_FAILED:
	cout << "READY FAILED."; break;
    default:
	cout << (int) state;
    }
    cout << endl;
#endif

    //
    // Try to find out what comes next... Here we basically implement the
    // finite automata to open a ACPLT/KS connection step by step. Whenever
    // either connection establishment or sending requests/receiving replies
    // fails, then we enter the closed mode.
    //
    if ( (state == KssConnection::CNX_STATE_CONN_FAILED)
	 || (state == KssConnection::CNX_STATE_READY_FAILED)
	 || (state == KssConnection::CNX_STATE_DEAD) ) {
	//
	// The macro state of the connection is now "closed". We use the
	// substate to determine the exact error result we want to show the
	// user of this connection. Also notify the user that there was a
	// problem with the connection.
	//
	_state = ISC_STATE_CLOSED;
	switch ( _sub_state ) {
	case ISC_SUBSTATE_CONNECTING_PMAP:
	case ISC_SUBSTATE_CONNECTING_MANAGER:
	    _result = KS_ERR_NOMANAGER;
	    break;
	default:
	    _result = KS_ERR_NETWORKERROR;
	}
	closeConnection();
	async_attention(state == KssConnection::CNX_STATE_READY_FAILED ?
	                    ISC_OP_CALL : ISC_OP_OPEN);
	return false; // do *not* "reactivate" the connection in _cln_con!
    }

    switch ( _sub_state ) {
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case ISC_SUBSTATE_CONNECTING_PMAP:
	//
	// We're in step 1: querying the portmapper on the target system
	// to find out where the ACPLT/KS manager lives.
	//
	switch ( state ) {
	case KssConnection::CNX_STATE_CONNECTED: {
	    //
	    // The connection to the portmapper is established, so try to
	    // ask for the ACPLT/KS manager. This is the usual query for
	    // a RPC server (in our case with running protocol 300664).
	    //
	    struct pmap pm_request;

	    pm_request.pm_prog = KS_RPC_PROGRAM_NUMBER;
	    pm_request.pm_vers = KS_PROTOCOL_VERSION;
	    pm_request.pm_prot = IPPROTO_TCP;
	    pm_request.pm_port = 0; // unused

	    if ( !_cln_con->beginRequest(makeXid(),
					 PMAPPROG, PMAPVERS, 
					 PMAPPROC_GETPORT)
		 || !xdr_pmap(_cln_con->getXdr(), &pm_request) ) {
		_result = KS_ERR_GENERIC;
		break; // fall through and report an error.
	    }
	    _cln_con->sendRequest();
	    return true; // "reactivate" connection (...to watch it...)
	}
	break;

	case KssConnection::CNX_STATE_READY: {
	    //
	    // We got an answer from the portmapper. So check whether we
	    // got a realy reply without errors and then fetch the port
	    // number in question of the ACPLT/KS manager from the answer.
	    //
	    KsRpcHeader rpc_head = _cln_con->getRpcHeader();

	    if ( (rpc_head._reply_status == KsRpcHeader::KsRpcMSGACCEPTED)
		 && (rpc_head._accept_status == KsRpcHeader::KsRpcSUCCESS) ) {
		//
		// Now retrieve the port number from the reply telegramme.
		//
		u_long port;
		if ( xdr_u_long(_cln_con->getXdr(), &port) ) {
		    //
		    // Okay. We've finally got it. Now check that it isn't
		    // zero (that means that there is no such rpc server
		    // registered).
		    //
		    if ( port ) {
			//
			// The ACPLT/KS manager is registered. So we close
			// the connection to the portmapper and open another
			// to the ACPLT/KS manager instead.
			//
			closeConnection();
			if ( !openManagerConnection(port, IPPROTO_TCP) ) {
			    break;
			}
			return false; // never "reactivate" the old connection,
			              // because it has been gone...
		    } else {
			//
			// There's no ACPLT/KS running so presumely there
			// aren't ACPLT/KS servers running either. Yes, this
			// does not always hold true, but who cares? If I
			// can't find the manager, I am not able to find the
			// server. That's it. Got it?
			//
			_result = KS_ERR_SERVERUNKNOWN;
			break; // fall through and report an error.
		    }
		}
	    }
	    _result = KS_ERR_NETWORKERROR;
	    break; // fall through and report an error.
	}

	default:
	    //
	    // All other states: can not occour, we just have this branch
	    // to keep compilers silent.
	    //
	    break;
	}
	break;
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case ISC_SUBSTATE_CONNECTING_MANAGER:
	//
	// We're in step 2: now we have to query the ACPLT/KS Manager for
	// the server in question.
	//
	switch ( state ) {
	case KssConnection::CNX_STATE_CONNECTED: {
	    //
	    // The connection with the ACPLT/KS manager has been established.
	    // We can now ask it for the server we're looking for.
	    //
	    KsServerDesc serverdesc(_server, KS_PROTOCOL_VERSION);

	    bool ok = _cln_con->beginRequest(makeXid(),
					     KS_RPC_PROGRAM_NUMBER,
					     KS_PROTOCOL_VERSION,
					     KS_GETSERVER);
	    //
	    // Always use the A/V NONE mechanism when asking the manager.
	    //
	    u_long avdummy = 0;
	    ok &= xdr_u_long(_cln_con->getXdr(), &avdummy);
	    ok &= serverdesc.xdrEncode(_cln_con->getXdr());
	    if ( !ok ) {
		_result = KS_ERR_GENERIC;
		break;
	    }
	    _cln_con->sendRequest();
	    return true; // "reactivate" connection (...watch it...)
	}
	break;

	case KssConnection::CNX_STATE_READY: {
	    //
	    // We got an answer from the ACPLT/KS manager. So let's see
	    // where the ACPLT/KS server in question lives.
	    //
	    KsRpcHeader rpc_head = _cln_con->getRpcHeader();
	    
	    if ( (rpc_head._reply_status == KsRpcHeader::KsRpcMSGACCEPTED)
		 && (rpc_head._accept_status == KsRpcHeader::KsRpcSUCCESS) ) {
		//
		// Now retrieve the ACPLT/KS service result and find out
		// behind which port number the server in question hides.
		//
		// Always expect the A/V NONE scheme in the reply.
		//
		u_long avscheme;
		bool ok = xdr_u_long(_cln_con->getXdr(), &avscheme);
		KsGetServerResult serverresult;
		if ( avscheme == KS_AUTH_NONE ) {
		    ok &= serverresult.xdrDecode(_cln_con->getXdr());
		} else {
		    ok = false;
		}
		closeConnection();

		if ( ok && (serverresult.result == KS_ERR_OK) ) {
		    cout << "server = " << serverresult.server.name << endl;
		    cout << "port = " << serverresult.port << endl;
		    //
		    // Phew. Now we can try to connect to the server as
		    // the caller of the open() method once wanted...
		    //
		    if ( !openServerConnection(serverresult.port) ) {
			break;
		    }
		    return false; // never "reactivate" the old connection,
			          // because it has gone...
		} else if ( ok ) {
		    _result = KS_ERR_SERVERUNKNOWN;
		    break;
		}
	    }
	    _result = KS_ERR_NETWORKERROR;
	    break;
	}

        default:
	    //
	    // All other states: can not occour, we just have this branch
	    // to keep compilers silent.
	    //
	    break;
	}
	break;
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case ISC_SUBSTATE_CONNECTING_SERVER:
	//
	// We have finally reached step 3:
	//
	switch ( state ) {
	case KssConnection::CNX_STATE_CONNECTED: {
	    //
	    // The connection to a particular ACPLT/KS server has been
	    // established. We are now notifying the user of this connection.
	    // The problem now is that the user is allowed to close the
	    // connection at this time and reopen it (for whatever obscure
	    // reasons). In this case we're not allowed to reactivate the
	    // (old) connection (or...bang!). So we track a possible close
	    // on this connection.
	    //
	    _state = ISC_STATE_OPEN;
	    _result = KS_ERR_OK;
	    _cln_con_once_closed = false;
	    async_attention(ISC_OP_OPEN);
	    return !_cln_con_once_closed;
	}

	case KssConnection::CNX_STATE_READY: {
	    //
	    // An answer for a RPC request sent a long time ago has been
	    // arrived. We leave it up to the async_attention() handler
	    // to read it in and check for errors.
	    //
	    _state = ISC_STATE_OPEN;
	    _result = KS_ERR_OK;
	    _cln_con_once_closed = false;
	    async_attention(ISC_OP_CALL);
	    return !_cln_con_once_closed;
	}

	default:
	    //
	    // All other states: can not occour, we just have this branch
	    // to keep compilers silent.
	    //
	    break;
	}
	break;

    default:
	break;
    }
    //
    // If we reach this stage, then there were some errors which results in
    // the connection being killed/closed. We report that this has happened
    // to the user of this connection.
    //
    closeConnection();
    async_attention(state == KssConnection::CNX_STATE_READY_FAILED ?
		    ISC_OP_CALL : ISC_OP_OPEN);
    return false; // do not reactivate...
} // KssInterKsServerConnection::attention

#endif



class Wecker : public KsTimerEvent, public KssInterKsServerConnection {
public:
    Wecker() 
	: KsTimerEvent(PltTime::now(1)),
	KssInterKsServerConnection("acplt:4444", "MANAGER")  
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

	_trigger_at = KsTime::now(2);
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
                               "extensions_link");
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
    addCommObject(ec, KssCommObjectHandle(tmp_var, KsOsNew));

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
