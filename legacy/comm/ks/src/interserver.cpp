/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/src/interserver.cpp,v 1.12 2007-04-25 12:57:20 martin Exp $ */
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
 * interserver.cpp -- implements an interserver connection object which can
 *                    send ACPLT/KS service requests to other ACPLT/KS
 *                    servers and receive the service replies. This i/o is
 *                    done in the background using the connection manager
 *                    magic. Therefore it's only available when the buffered
 *                    XDR streams have been enabled.
 *
 * Written by Harald Albrecht <harald@plt.rwth-aachen.de>
 */

/* Historical sidenote: somehow "interserver" sounds like "intershop"... */

#if PLT_USE_BUFFERED_STREAMS

#include "ks/interserver.h"

#if !PLT_SYSTEM_NT
#include <unistd.h>
#endif

#define DEBUG_STATES 0


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
      _connect_timeout(15), _call_timeout(30),
      _protocol_version(0)
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
    // When connecting to an ACPLT/KS server, start with the highest
    // supported ACPLT/KS protocol version first.
    //
    _protocol_version = KS_PROTOCOL_VERSION;
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
// Return the XDR stream for this connection or 0 if no XDR stream is
// currently allocated to this interserver connection.
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
// Return the ACPLT/KS protocol version spoken by server.
//
u_short KssInterKsServerConnection::getProtocolVersion() const {
    if ( _state != ISC_STATE_OPEN ) {
	return 0;
    }
    return _protocol_version;
}


// ---------------------------------------------------------------------------
// Start building a request telegramme. After calling this method you can
// serialize data into the connection using its associated XDR stream (which
// can be retrieved through the getXdr() accessor). When you're done, call
// endSend() to finish the telegramme and do the actual i/o -- well, it's more
// "o" than "i" in the case of the request...
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
				 _protocol_version,
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
    enum_t avscheme = 0;
    bool ok = xdr_enum(_cln_con->getXdr(), &avscheme);
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
    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
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
    _cln_con = new KssUDPXDRConnection(sock, 
				       (_connect_timeout > 10) ?
				           10 : _connect_timeout,
				       2, /* secs between consecutive
                                             retries */
				       &_host_addr, sizeof(_host_addr),
				       KssConnection::CNX_TYPE_CLIENT);
    if ( !_cln_con || 
	 (_cln_con->getState() == KssConnection::CNX_STATE_DEAD) ) {
	if ( _cln_con ) {
	    _cln_con->shutdown();
	    delete _cln_con;
	    _cln_con = 0;
	} else {
#if PLT_SYSTEM_NT
	    ::closesocket(sock);
#else
	    ::close(sock);
#endif
	}
	_result = KS_ERR_GENERIC;
	return false;
    }
    //
    // Okay. We now have the socket as well as the connection object, so we
    // can now enter the connection establishment state.
    //
#if DEBUG_STATES
    cout << "opening portmapper connection" << endl;
#endif
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
    int sock = socket(AF_INET, 
		      (protocol == IPPROTO_UDP) ? SOCK_DGRAM : SOCK_STREAM,
		      protocol);
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
    if ( protocol == IPPROTO_UDP ) {
	_cln_con = new KssUDPXDRConnection(sock, 
					   _connect_timeout,
					   5, /* secs between
                                                 consecutive retries */
					   &_host_addr, sizeof(_host_addr),
					   KssConnection::CNX_TYPE_CLIENT);
    } else {
	_cln_con = new KssTCPXDRConnection(sock, _connect_timeout, 
					   _host_addr, sizeof(_host_addr),
					   KssConnection::CNX_TYPE_CLIENT);
    }
    if ( !_cln_con || 
	 (_cln_con->getState() == KssConnection::CNX_STATE_DEAD) ) {
	if ( _cln_con ) {
	    _cln_con->shutdown();
	    delete _cln_con;
	    _cln_con = 0;
	} else {
#if PLT_SYSTEM_NT
	    ::closesocket(sock);
#else
	    ::close(sock);
#endif
	}
	_result = KS_ERR_GENERIC;
	return false;
    }
    //
    // Okay. We now have the socket as well as the connection object, so we
    // can now enter the connection establishment state.
    //
#if DEBUG_STATES
    cout << "opening manager connection" << endl;
#endif
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
#if PLT_SYSTEM_NT
	    ::closesocket(sock);
#else
	    ::close(sock);
#endif
	}
	_result = KS_ERR_GENERIC;
	return false;
    }
    //
    // Okay. We now have the socket as well as the connection object, so we
    // can now enter the connection establishment state.
    //
#if DEBUG_STATES
    cout << "opening server connection" << endl;
#endif
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

#if DEBUG_STATES
    cout << "macro and sub connection state: ";
    switch ( _state ) {
    case ISC_STATE_CLOSED:
	cout << "CLOSED."; break;
    case ISC_STATE_OPEN:
	cout << "OPEN."; break;
    case ISC_STATE_BUSY:
	cout << "BUSY."; break;
    default:
	cout << (int) _state;
    }
    cout << " ";
    switch ( _sub_state ) {
    case ISC_SUBSTATE_NONE:
	cout << "(NONE)"; break;
    case ISC_SUBSTATE_CONNECTING_PMAP:
	cout << "(PMAP)"; break;
    case ISC_SUBSTATE_CONNECTING_MANAGER:
	cout << "(MANAGER)"; break;
    case ISC_SUBSTATE_CONNECTING_SERVER:
	cout << "(SERVER)"; break;
    default:
	cout << (int) _sub_state;
    }
    cout << endl;

    cout << "micro (delegated) connection state: ";
    switch ( state ) {
    case KssConnection::CNX_STATE_CONNECTING:
	cout << "CONNECTING."; break;
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
	    //
	    // Note that when contacting the portmapper or the Manager
	    // using UDP we never get CNX_STATE_CONN_FAILED, as UDP is
	    // a connectionless transport, thus no connection phase which
	    // could fail. When either the portmapper or the Manager do
	    // not respond, we assume a failed "virtual connection".
	    //
	    state = KssConnection::CNX_STATE_CONN_FAILED;
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
	    // Note that we ask for the appropriate ACPLT/KS protocol version
	    // and will lower our expectations lateron as we can't find
	    // a suitable server...
	    //
	    struct pmap pm_request;

	    pm_request.pm_prog = KS_RPC_PROGRAM_NUMBER;
	    pm_request.pm_vers = _protocol_version;
	    pm_request.pm_prot = IPPROTO_UDP;
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
			if ( !openManagerConnection(port, IPPROTO_UDP) ) {
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
		    	// Please note that the portmapper will typically lie
		    	// in case it finds the program requested although it
		    	// has the wrong version. In these cases the portmapper
		    	// will return the port number of the ACPLT/KS manager
		    	// nevertheless.
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
	    KsServerDesc serverdesc(_server, _protocol_version);

	    bool ok = _cln_con->beginRequest(makeXid(),
					     KS_RPC_PROGRAM_NUMBER,
					     _protocol_version,
					     KS_GETSERVER);
	    //
	    // Always use the A/V NONE mechanism when asking the manager.
	    //
	    u_long avdummy = 0;
	    ok = ok && xdr_u_long(_cln_con->getXdr(), &avdummy);
	    ok = ok && serverdesc.xdrEncode(_cln_con->getXdr());
	    if ( !ok ) {
		_result = KS_ERR_GENERIC;
		break;
	    }
	    _cln_con->sendRequest();
	    return true; // "reactivate" connection (...watch it...)
	}

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
#if 0
		    cout << "server = " << serverresult.server.name << endl;
		    cout << "port = " << serverresult.port << endl;
		    cout << "protocol version = " << _protocol_version << endl;
#endif
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
		    //
		    // In case we couldn't find the server, but we have still
		    // an older ACPLT/KS protocol version we can try, ask
		    // the Manager again.
		    //
		    if ( --_protocol_version >= KS_MINPROTOCOL_VERSION ) {
		    	//
		    	// Reask for older protocol version. As the connection
		    	// has been closed, we need to reopen it. In case the
		    	// port of the ACPLT/KS Manager hasn't been specified, we
		    	// will recontact the portmapper first. Otherwise we can
		    	// immediately try to connect to the Manager.
		    	//
                        if ( _host_port ) {
	                    if ( !openManagerConnection(_host_port, IPPROTO_TCP) ) {
	                    	break; // Fall through and report error...
	                    }
                        } else {
                            if ( !openPortmapperConnection() ) {
                            	break; // Fall through and report error...
                            }
		        }
			return false; // never "reactivate" the old connection,
			              // because it has been gone...
		    }
		    //
		    // Sigh. Unknown server...
		    //
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


#endif /* PLT_USE_BUFFERED_STREAMS */

// End of ks/interserver.cpp
