/* -*-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/src/client.cpp,v 1.49 2003-10-15 15:55:54 harald Exp $ */
/*
 * Copyright (c) 1996, 2003
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

/* Author: Markus Juergens <markusj@plt.rwth-aachen.de> */
/* Dumb comments by: Harald Albrecht <harald@plt.rwth-aachen.de> */
/* adaption to non-blocking IO by Albrecht boshi
 * (Albrecht boshi xie zhe ge)
 */

//////////////////////////////////////////////////////////////////////

#include <string.h>

#include <plt/log.h>

#include "ks/client.h"
#include "ks/ks.h"
#include "ks/commobject.h"

#if PLT_DEBUG
#if PLT_USE_DEPRECIATED_HEADER
#include <iostream.h>
#else
#include <iostream>
#endif
#endif

#if PLT_SYSTEM_NT
#include <fcntl.h>
#elif PLT_SYSTEM_OPENVMS
#include <ioctl.h>
extern int ioctl(int d, int request, char *argp);
#else
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#endif

#include "ks/stdconnectionmgr.h"

#if PLT_USE_DEPRECIATED_HEADER
#include <iostream.h>
#else
#include <iostream>
#endif


#define DEBUG_CLN 0


//////////////////////////////////////////////////////////////////////
// RTTI Simulation
//////////////////////////////////////////////////////////////////////

PLT_IMPL_RTTI0(KscServerBase);
PLT_IMPL_RTTI1(KscServer,KscServerBase);


//////////////////////////////////////////////////////////////////////
// Initialize static data
//
// _the_client points to the one and only client object an ACPLT/KS
// client application using the C++ Communication Library can have.
KscClient *KscClient::_the_client = 0;
// _clean_up is a static object which is responsible for destroying
// the client object when the application exits.
KscClient::CleanUp KscClient::_clean_up;


//////////////////////////////////////////////////////////////////////
// Helper function to split up the combined host and server names from
// "//host/server" into "host" and "server". If something goes wrong,
// the appropriate KS error code is returned.
//
static KS_RESULT
_ksc_extractHostAndServer(KsString host_and_server,
                          KsString &host,
                          KsString &server)
{
    //
    // First, some sanity checks... I like defensive programming...
    // So where's the magic length of five comming from? Well -- the
    // shortest possible valid host&server name is "//x/x", and this
    // string is of length five. That's it. Every string which is less
    // long therefor must be invalid.
    //
    // The first two slashes, which form the prefix of the resource
    // locator, indicate that this is an "absolute" name, which contains
    // the host name (DNS) and the server name where the communication
    // object is located at.
    //
    // 1.4.0: gcc: welcome to the pool of problems of ANSI
    //
#if PLT_COMPILER_MSVC || PLT_COMPILER_DECCXX || PLT_COMPILER_GCC
    if ( (host_and_server.len() < 5) ||
	 (host_and_server.operator[](0) != '/') || 
         (host_and_server.operator[](1) != '/') ) {
	return KS_ERR_MALFORMEDPATH;
    }
#else
    if ( (host_and_server.len() < 5) ||
	 (host_and_server[0] != '/') || (host_and_server[1] != '/') ) {
	return KS_ERR_MALFORMEDPATH;
    }
#endif

    //
    // Now search the separating slash between the host and server name
    //
    size_t pos = 2;
    while ( host_and_server[pos] && (host_and_server[pos++] != '/') ) {
    }

    if( host_and_server[pos] ) {
	//
        // Okay, we succeeded and found that separator slash. Due to the
	// first check above, we only thing that can go wrong here is that
	// the strings went invalid due to memory shortage.
        //
        host = host_and_server.substr(2, pos - 3);
        server = host_and_server.substr(pos);
	return (host.ok() && server.ok()) ? KS_ERR_OK : KS_ERR_GENERIC;
    } else {
        PLT_DMSG("Invalid server identification " << host_and_server << endl);
	return KS_ERR_MALFORMEDPATH;
    }
} // _ksc_extractHostAndServer


//////////////////////////////////////////////////////////////////////
// class KscClient
//////////////////////////////////////////////////////////////////////

KscClient::KscClient()
: av_module(0),
  _rpc_timeout(KSC_RPCCALL_TIMEOUT),
  _retry_wait(0, 0),
  _tries(1)
{
    if ( !KsConnectionManager::getConnectionManagerObject() ) {
	new KsStdConnectionManager();
    }
}

//////////////////////////////////////////////////////////////////////

KscClient::~KscClient()
{

    PLT_DMSG("Servers left at KscClient dtor: " << server_table.size() << endl);

    //
    // Destroy all the KscServer objects that are still lurking around
    // in the free store. There shouldn't be any left at this time, the only
    // exception being the case that an ACPLT/KS client programmer didn't
    // remove all communication objects and created a static client
    // object which now gets destroyed.
    //
    PltHashIterator<KsString,KscServerBase *> it(server_table);
    KscServerBase *pcurr;
    while(it) {
        pcurr = it->a_value;
        if( pcurr ) {
            delete pcurr;
        }
        ++it;
    }
}

//////////////////////////////////////////////////////////////////////

void
KscClient::_createClient()
{
    PLT_PRECONDITION(!_the_client);

    KscClient *cl = new KscClient();
    if(!cl) {
        PltLog::Alert("Cannot create client, going to abort program");
        exit(-1);
    }

#if PLT_DEBUG
    bool ok = setClient(cl, KsOsNew);
    PLT_ASSERT(ok);
#else
    setClient(cl, KsOsNew);
#endif

}

//////////////////////////////////////////////////////////////////////

bool 
KscClient::setClient(KscClient *cl, KsOwnership os)
{
    if(_the_client || !cl) {
        return false;
    } else {
        if(os == KsOsNew) {
            _clean_up.shutdown_delete = true;
        } else if(os == KsOsUnmanaged) {
            _clean_up.shutdown_delete = false;
        } else {
            // invalid ownership status
            return false;
        }
        _the_client = cl;

        return true;
    }
}

//////////////////////////////////////////////////////////////////////
// Just ask for a server object using its host&server name (of the
// form "//host/server") and return the pointer to it or a null
// pointer if there's (yet) no such server object. We don't check here
// the host&server name for a syntactic validy but instead let this
// job up to the createServer() method. If the name is not valid, then
// why bother with syntax checks, it's not in the table of server
// objects at all and we assume that invalid names are rare and that
// lookup doesn't count.
//
KscServerBase *
KscClient::getServer(const KsString &host_and_name) 
{
    KscServerBase *pServer;

    bool ok = server_table.query(host_and_name, pServer);

    return ok ? pServer : 0;
}


//////////////////////////////////////////////////////////////////////
// Create a server object for the given host&server name (of the form
// "//host/server") and return the pointer to it. In any case, success
// or failure is indicated by the error return value -- the well known
// KS_RESULT enumeration. This allows the caller to act appropriate
// and set usefull error codes on its own behalf.
//
KS_RESULT
KscClient::createServer(KsString host_and_name,
                        u_short /*protocol_version*/, // for future use
                        KscServerBase *&pServer)
{
    KS_RESULT result = KS_ERR_OK;

    pServer = 0; // Wipe it out...

    //
    // Try to find a suitable server object which has already been created
    // for the same ACPLT/KS server.
    //
    bool ok = server_table.query(host_and_name, pServer);

    if( !ok ) {
	//
        // Create a new server object, because we didn't find a suitable
	// one in the table of servers.
        //
        KsString host, server;
        if ( (result = _ksc_extractHostAndServer(host_and_name, host, server))
	     != KS_ERR_OK ) {
	    //
	    // Gosh! The resource locator is invalid, so we can't extract
	    // the host name and the server name. Fall through and return
	    // a null pointer (that is, no server) together with the error
	    // code.
	    //
	} else {
	    //
	    // The resource locator is valid but we don't have a suitable
	    // KscServer object now. So we create one.
	    //
	    // NOTE: We use KS_MINPROTOCOL_VERSION here, so a client will
	    // accept older servers as partners (how nice) and switch back
	    // to the version 1 GetPP server when appropriate. Nevertheless
	    // a client still will always get the newest KS server if
	    // possible.
	    //
	    KscServer *temp = new KscServer(host, 
					    KsServerDesc(server,
							 KS_MINPROTOCOL_VERSION));
	    if ( temp ) {
		//
		// Server object successfully created. So inherit the
		// communication settings from this client object and add
		// the server object to the table of server objects. If
		// this operation fails, then we'll destroy the new server
		// object and return an error instead.
		//
		if ( server_table.add(host_and_name, temp) ) {
		    temp->setTimeouts(_rpc_timeout, _retry_wait, _tries);
		    pServer = temp;
		} else {
		    delete temp;
		    result = KS_ERR_GENERIC;
		}
	    } else {
		//
		// Nope. We've lost. Game over. No memory. Did we trash
		// the free store, didn't we?
		//
		result = KS_ERR_GENERIC;
	    }
        }
    }
    //
    // Final note: do not throw an assertion here if pServer is null,
    // because this is a run-time error and not a design or coding
    // error. (There used to be an assertion here in former times...)
    // Final final note: the only coding error which can happen here,
    // is that ok is true, but pServer is not set properly. Guess what,
    // I've really made it into this trap (--aldi)...
    //
    PLT_ASSERT(!ok || pServer);
    return result;
} // KscClient::createServer

//////////////////////////////////////////////////////////////////////

void
KscClient::deleteServer(KscServerBase *server)
{
    KscServerBase *temp = 0;

    // remove server from table
    //
#if PLT_DEBUG
    bool ok =
#endif 
        server_table.remove(server->getHostAndName(), temp);

    PLT_ASSERT(ok && (temp == server));

    delete server;
}


// ----------------------------------------------------------------------------
// Accessors to the timeout and retry struff...
//
void 
KscClient::setTimeouts(const PltTime &rpc_timeout,        
                       const PltTime &retry_wait,         
                       size_t tries)
{
    _rpc_timeout = rpc_timeout;
    _retry_wait  = retry_wait;
    _tries       = tries;
} // KscClient::setTimeouts

void
KscClient::getTimeouts(PltTime &rpc_timeout,
		       PltTime &retry_wait,
		       size_t &tries)
{
    rpc_timeout = _rpc_timeout;
    retry_wait  = _retry_wait;
    tries       = _tries;
} // KscClient::getTimeouts


//////////////////////////////////////////////////////////////////////

#if PLT_DEBUG

void
KscClient::printServers()
{
    PltHashIterator<KsString,KscServerBase *> it(server_table);

    cout << "Client manages " 
         << server_table.size()
         << " server objects :"
         << endl;

    while(it) {
        cout << "Server : " << it->a_key 
             << ", " << it->a_value
             << endl;
        ++it;
    }
}

#endif


// ----------------------------------------------------------------------------
// Objects of the reconnect timer class are used by KscServer objects to
// schedule a delayed reconnect procedure. This class is in principle only an
// adapter class for redirecting the timer event to an appropriate method of
// the corresponding KscServer object.
//
class KscServerReconnectTimer : public KsTimerEvent {
public:
    KscServerReconnectTimer(KscServer *server);
    virtual void trigger();

protected:
    KscServer *_server;
}; // class KscServerReconnectTimer

KscServerReconnectTimer::KscServerReconnectTimer(KscServer *server)
    : KsTimerEvent(KsTime::now()),
      _server(server)
{
} // KscServerReconnectTimer::KscServerReconnectTimer

void
KscServerReconnectTimer::trigger()
{
    _server->reconnectTimerTrigger();
} // KscServerReconnectTimer::trigger



// ----------------------------------------------------------------------------
// In order to handle the attention callbacks, we need to subclass the
// server connection class. The only thing we do here is to redirect the
// callback to the same callback on the KscServer object we belong to.
//
class KscServerConnection : public KsServerConnection {
public:
    KscServerConnection(KscServer *server,
			KsString host_name, KsString server_name);
    virtual void async_attention(KsServerConnectionOperations op);

protected:
    KscServer *_server;
}; // class KscServerConnection

KscServerConnection::KscServerConnection(KscServer *server,
					 KsString host_name,
					 KsString server_name)
    : KsServerConnection(host_name, server_name),
      _server(server)
{
} // KscServerConnection::KscServerConnection

void
KscServerConnection::async_attention(KsServerConnectionOperations op)
{
    _server->async_attention(op);
} // KscServerConnection::async_attention




//////////////////////////////////////////////////////////////////////
// KscServerBase
//////////////////////////////////////////////////////////////////////

KscServerBase::KscServerBase(KsString host, KsString name)
: host_name(host),
  server_name(name),
  host_and_name("//", host),
  av_module(0),
  ref_count(0),
  _last_result(KS_ERR_OK)
{
    host_and_name += "/";
    host_and_name += name;
}

KscServerBase::KscServerBase(KsString hostAndName)
: host_and_name(hostAndName),
  av_module(0),
  ref_count(0),
  _last_result(KS_ERR_OK)
{
    _last_result = _ksc_extractHostAndServer(host_and_name, 
					     host_name, server_name);
}

//////////////////////////////////////////////////////////////////////
// Section for handling of communication objects
//

void
KscServerBase::incRefcount() 
{
    ref_count++;
}

//////////////////////////////////////////////////////////////////////

void
KscServerBase::decRefcount()
{
    if( !(--ref_count) ) {
        // no more referring objects left
        // destroy this object
        //
        KscClient *the_client =
            KscClient::getClient();
        the_client->deleteServer(this);
    }
}


// ----------------------------------------------------------------------------
// Note: this is a *blocking* method
bool
KscServer::getServerVersion(u_long &version)
{
    if ( _svr_con->getState() == KsServerConnection::ISC_STATE_CLOSED ) {
	if ( !ping() ) {
	    return false;
	}
    }
    version = _svr_con->getProtocolVersion();
    return true;
} // KscServer::getServerVersion

// ----------------------------------------------------------------------------
// Note: this is a *blocking* method
u_short
KscServer::getProtocolVersion()
{
    if ( _svr_con->getState() == KsServerConnection::ISC_STATE_CLOSED ) {
	if ( !ping() ) {
	    return 0;
	}
    }
    return _svr_con->getProtocolVersion();
}


// ----------------------------------------------------------------------------
// The new ACPLT/KS protocol version 2 GETEP service. This superceedes the old
// GETPP service and works with the new engineered properties. To make live
// easier for client writers, the getEP() method backwards compatibility with
// version 1 ACPLT/KS servers: if a we detect a version 1 server (this info is
// returned by the GETSERVER service from the MANAGER), then we transparently
// issue a GETPP and convert the result back into a list of engineered
// properties.
//
bool 
KscServerBase::getEP(const KscAvModule *avm,
		     const KsGetEPParams &params,
		     KsGetEPResult &result)
{
    u_long version;

    //
    // Retrieve the server's protocol version. This will try to connect if
    // not already done. We need the protocol version so we can transparently
    // issue a GETPP service request for old v1 servers.
    //
    if ( !getServerVersion(version) ) {
	//
	// If we can't open the connection then bail out immediately. We
	// don't touch the _last_result here, because it has already been
	// set by the getServerVersion() method.
	//
	return false;
    }

    if ( version == 1 ) {
	//
	// We're talking to a ACPLT/KS core protocol version 1 server, so we
	// need to use the old GETPP service instead of GETEP.
	//
	KsGetPPParams getPPParams;
	KsGetPPResult getPPResult;

	//
	// The KS_EPF_FLATTEN scope flag is ignored as it has no equivalent
	// with the GETPP service. Also the KS_EPF_PARTS flag can be ignored
	// with the old version 1 protocol (flattening works only with parts
	// but not with children in v2). In case the KS_EPF_CHILDREN flag
	// is missing we can shortcut the service request immediately, as
	// we know that the result list must be empty.
	//
	if ( !(params.scope_flags & KS_EPF_CHILDREN) ) {
	    result.result = KS_ERR_OK;
	    return true;
	}
	
	//
	// Execute the old and now depreciated GetPP service (at least try
	// to execute it...). If something fails, bail out. Note that we make
	// sure that the service params will always ask only for variables and
	// domain communication objects to avoid potential trouble.
	//
	getPPParams.path      = params.path;
	getPPParams.type_mask = params.type_mask &
	                        (KS_OT_VARIABLE | KS_OT_DOMAIN |
				    KS_OT_LINK | KS_OT_HISTORY);
	getPPParams.name_mask = params.name_mask;
	if ( !requestByOpcode(KS_GETPP, avm, params, getPPResult) ) {
	    result.result = getPPResult.result;
	    return false;
	}
	//
	// Check that the service request was accepted by the ACPLT/KS server
	// and then convert the old projected properties into engineered
	// properties.
	//
	result.result = getPPResult.result;
	if ( getPPResult.result == KS_ERR_OK ) {
	    KsEngPropsV1Handle hv1ep;
	    
	    while ( !getPPResult.items.isEmpty() ) {
		//
		// Remove one set of old projected properties at a time and
		// make sure that we get a bound handle before trying to
		// convert the old projected properties into new engineered
		// properties.
		//
		hv1ep = getPPResult.items.removeFirst();
		if ( hv1ep ) {
		    KsEngProps *ep = 0;
		    switch ( hv1ep->xdrTypeCode() ) {
		    case KS_OT_DOMAIN:
			ep = new KsDomainEngProps(
			    hv1ep->identifier,
			    hv1ep->creation_time,
			    hv1ep->comment,
			    hv1ep->access_mode,
			    0,         // ...no semantic flags
			    KsString() // ...no class identifier
			    );
			break;
			
		    case KS_OT_VARIABLE:
			ep = new KsVarEngProps(
			    hv1ep->identifier,
			    hv1ep->creation_time,
			    hv1ep->comment,
			    hv1ep->access_mode,
			    0,         // ...no semantic flags
			    ((KsVarEngPropsV1 *)hv1ep.getPtr())->tech_unit,
			    ((KsVarEngPropsV1 *)hv1ep.getPtr())->type
			    );
			break;

#if PLT_SOURCE_V1PLUS_BC
		    case KS_OT_LINK:
			ep = new KsLinkEngProps(
			    hv1ep->identifier,
			    hv1ep->creation_time,
			    hv1ep->comment,
			    hv1ep->access_mode,
			    0,         // ...no semantic flags
			    ((KsLinkEngPropsV1 *)hv1ep.getPtr())->type,
			    ((KsLinkEngPropsV1 *)hv1ep.getPtr())->opposite_role_identifier,
			    ((KsLinkEngPropsV1 *)hv1ep.getPtr())->class_identifier
			    );
			break;

		    case KS_OT_HISTORY:
			ep = new KsHistoryEngProps(
			    hv1ep->identifier,
			    hv1ep->creation_time,
			    hv1ep->comment,
			    hv1ep->access_mode,
			    0,         // ...no semantic flags
			    ((KsHistoryEngPropsV1 *)hv1ep.getPtr())->type,
			    ((KsHistoryEngPropsV1 *)hv1ep.getPtr())->default_interpolation,
			    ((KsHistoryEngPropsV1 *)hv1ep.getPtr())->supported_interpolations,
			    ((KsHistoryEngPropsV1 *)hv1ep.getPtr())->type_identifier
			    );
			break;
#endif
			
		    default:
			continue; // be nice and ignore unknown objects.
		    }
		    //
		    // Make sure that the engineered properties could be
		    // converted and that we could add them to the result
		    // list.
		    //
		    if ( !ep || 
			 !result.items.addLast(KsEngPropsHandle(ep, 
								KsOsNew)) ) {
			result.result = KS_ERR_GENERIC;
			return false;
		    }
		}
	    }
	}
	return true;
    } else {
	//
	// We're talking to a new protocol v2 version server, so we can use
	// the GETEP service (as intended...).
	//
	return requestByOpcode(KS_GETEP, avm, params, result);
    }
} // KscServerBase::getEP



    
//////////////////////////////////////////////////////////////////////
// class KscServer
//
// Constructor
// Just copies the given data for later use.
//
KscServer::KscServer(KsString host, 
                     const KsServerDesc &server)
: KscServerBase(host, server.name),
  _svr_con(0),
  _reconnect_timer(0),
  _rpc_timeout(KSC_RPCCALL_TIMEOUT),
  _retry_wait(0, 0),
  _tries(1)
{
    initExtTable(); // make mandatory services available
    _reconnect_timer = new KscServerReconnectTimer(this);
    _svr_con = new KscServerConnection(this, host_name, server_name);
    // TODO & FIXME: timeout settings
    // TODO & FIXME: error checks
}

//////////////////////////////////////////////////////////////////////

KscServer::KscServer(KsString hostAndName, u_short protocolVersion)
: KscServerBase(hostAndName),
  _svr_con(0),
  _reconnect_timer(0),
  _rpc_timeout(KSC_RPCCALL_TIMEOUT),
  _retry_wait(0, 0),
  _tries(1)
{
    initExtTable(); // make mandatory services available
    _reconnect_timer = new KscServerReconnectTimer(this);
    _svr_con = new KscServerConnection(this, host_name, server_name);
    // TODO & FIXME: timeout settings
    // TODO & FIXME: error checks
}

/////////////////////////////////////////////////////////////////////////////
// initExtTable() fills ext_opcodes with all mandatory 
// services. Currently this is ks_core only.
//
void
KscServer::initExtTable()
{
    ext_opcodes.add(KsString("ks_core"), 0);
}


// ----------------------------------------------------------------------------
// Destructor. When cleaning up we first terminate all waiting requests, also
// kill any pending reconnect timer and finally close the connection to the
// server and kill the corresponding transport object.
//
KscServer::~KscServer()
{
    terminateRequests();
    if ( _reconnect_timer ) {
	delete _reconnect_timer;
	_reconnect_timer = 0;
    }
    if ( _svr_con ) {
	close();
	delete _svr_con;
	_svr_con = 0;
    }
} // KscServer::~KscServer


// ----------------------------------------------------------------------------
//
bool
KscServer::open()
{
    return _svr_con->open();
}

void
KscServer::close()
{
    _svr_con->close();
    ext_opcodes.reset();
    initExtTable();
    neg_table.reset();
} // KscServer::close


//////////////////////////////////////////////////////////////////////
// set timeouts and retry parameters
//
void 
KscServer::setTimeouts(const PltTime &rpc_timeout,        
                       const PltTime &retry_wait,         
                       size_t tries)
{
    _rpc_timeout = rpc_timeout;
    _retry_wait = retry_wait;
    _tries = tries;

    _svr_con->setTimeouts(rpc_timeout.tv_sec, rpc_timeout.tv_sec);
} // KscServer::setTimeouts


void
KscServer::getTimeouts(PltTime &rpc_timeout,
		       PltTime &retry_wait,
		       size_t &tries)
{
    rpc_timeout = _rpc_timeout;
    retry_wait  = _retry_wait;
    tries       = _tries;
} // KscServer::getTimeouts



// ----------------------------------------------------------------------------
// Terminate the current request as well as all other waiting requests. The
// owners of the requests are notified of this new situation.
//
void
KscServer::terminateRequests()
{
    KsConnectionManager::getConnectionManagerObject()->
	removeTimerEvent(_reconnect_timer);

    if ( _current_request
	 && _current_request->_status == KscServiceRequest::REQ_BUSY ) {
	//
	// Only close the connection if there was a request currently going
	// on. Otherwise, we just leave the connection open to avoid another
	// connection establishment phase later.
	//
	close();
	//
	// Terminate the current request and notify its owner.
	//
	KscServiceRequestHandle request = _current_request;
	_current_request.bindTo(0);
	request->_result.result = KS_ERR_TERMINATED;
	request->_status = KscServiceRequest::REQ_DONE;
	request->attention();
    }
    //
    // Now pull all waiting requests from the queue, terminate them and
    // notify their owners of the new situation.
    //
    while ( !_request_queue.isEmpty() ) {
	KscServiceRequestHandle request = _request_queue.removeFirst();
	request->_result.result = KS_ERR_TERMINATED;
	request->_status = KscServiceRequest::REQ_DONE;
	request->attention();
    }
} // KscServer::terminateRequests


// ----------------------------------------------------------------------------
// Pull the next waiting request off the ACPLT/KS service request queue.
// Optionally, a new request can be specified that will be either queued (if
// there are other requests waiting) or will be immediately serviced (if
// no other requests are currently waiting).
//
void
KscServer::initiateRequestIfPossible(KscServiceRequestHandle newRequest)
{
    if ( newRequest ) {
	if ( !_current_request ) {
	    //
	    // No current request, so check the queue first to decide whether
	    // this new request can immediately be served or not.
	    //
	    if ( !_request_queue.isEmpty() ) {
		//
		// Others are waiting too, so queue this new request at the
		// end of the queue and go strolling the road.
		//
#if DEBUG_CLN
		PltLog::Debug("KscServer::initiateRequestIfPossible: queuing new request");
#endif
		newRequest->_status = KscServiceRequest::REQ_WAITING;
		_request_queue.addLast(newRequest);
		return;
	    } else {
		//
		// No other request waiting, thus we can serve the new one.
		//
#if DEBUG_CLN
		PltLog::Debug("KscServer::initiateRequestIfPossible: serving now");
#endif
		_current_request = newRequest;
	    }
	} else {
	    //
	    // The transport connection is busy, so queue this new request
	    // and do something other in the meanwhile.
	    //
	    PltLog::Debug("KscServer::initiateRequestIfPossible: queuing (busy)");
#if DEBUG_CLN
	    newRequest->_status = KscServiceRequest::REQ_WAITING;
#endif
	    _request_queue.addLast(newRequest);
	    return;
	}
    } else {
	//
	// Pull off the next request of the queue, if there is one still
	// waiting.
	//
	if ( _current_request 
	     || _request_queue.isEmpty() ) {
#if DEBUG_CLN
	    PltLog::Debug("KscServer::initiateRequestIfPossible: queue empty and no new request");
#endif
	    return;
	}
#if DEBUG_CLN
	PltLog::Debug("KscServer::initiateRequestIfPossible: serving next request from queue");
#endif
	_current_request = _request_queue.removeFirst();
    }
    //
    // Set up the request for further processing... This in particular implies
    // getting the right negotiator.
    //
    _current_request->_status = KscServiceRequest::REQ_BUSY;
    _current_request->_neg = getNegotiator(_current_request->_avm);
    //
    // When we have finally reached this bright cute place here deep in the
    // code then there is a new request available which now needs to be
    // sent to the server...
    //
    _tries_remaining = _tries; // new request, so new game

    switch ( _svr_con->getState() ) {
    case KsServerConnection::ISC_STATE_BUSY:
	//
	// This situation must not happen! It indicates that there has been
	// something going boing.
	//
	PltLog::Error("KscServer::pullNextRequest(): no current request but connection busy, terminating busy request.");
	close();
	// fall through...

    //
    // The connection is currently closed. So, before we can send anything,
    // we need to establish the connection to the server first.
    //
    case KsServerConnection::ISC_STATE_CLOSED:
	if ( !open() ) {
	    //
	    // Immediately failed to open the connection. Since that usually
	    // indicates a severe problem, bail out immediately.
	    //
#if DEBUG_CLN
	    PltLog::Debug("KscServer::initiateRequestIfPossible: open failed");
#endif
	    finishAndNotify(getLastResult());
	} else {
	    //
	    // Nothing to do here now but to wait for the establishment
	    // acknowledgement to arriver later at the attention callback.
	    //
#if DEBUG_CLN
	    PltLog::Debug("KscServer::initiateRequestIfPossible: opening...");
#endif
	}
	break;

    //
    // The connection is already (or still) open. So just issue the service
    // request to the server and wait for the things to happen.
    //
    case KsServerConnection::ISC_STATE_OPEN:
#if DEBUG_CLN
	PltLog::Debug("KscServer::initiateRequestIfPossible: opened, now sending request");
#endif
	//
	// 1azyn3ss ru13z! Just delegate this...
	//
	_last_result = KS_ERR_OK;
	async_attention(KsServerConnection::ISC_OP_OPEN);
	break;
    }
} // KscServer::initiateRequestIfPossible


// ----------------------------------------------------------------------------
// Helper method: moves the current request from the BUSY state into DONE
// state and indicates that the request did or did not succeed. If there are
// other requests waiting at this time, the first one of them is automatically
// served (before notification of the failed one!).
//
void
KscServer::finishAndNotify(KS_RESULT result)
{
    if ( result != KS_ERR_OK ) {
	close();
    }
    KscServiceRequestHandle request = _current_request;
    _current_request.bindTo(0); // make room for a new request
    if ( result != KS_ERR_OK ) {
	//
	// only touch the result of the answer if the result argument
	// was not okay: this indicates that the _result field has not
	// been properly set by a previous deserialization and thus we
	// set a proper error value.
	//
	request->_result.result = result;
    }
    request->_status = KscServiceRequest::REQ_DONE;
    initiateRequestIfPossible(KscServiceRequestHandle());
    request->attention();
} // KscServer::finishAndNotify


// ----------------------------------------------------------------------------
// The last connection or call attempt failed, so we have been triggered to
// start another attempt...
//
void
KscServer::reconnectTimerTrigger()
{
    if ( _current_request
	 && _current_request->_status == KscServiceRequest::REQ_BUSY ) {
	//
	// Restart the whole procedure by attempting a connection establishment
	//
#if DEBUG_CLN
	PltLog::Debug("KscServer::reconnectTimerTrigger: restarting timer");
#endif
	if ( !open() ) {
	    //
	    // Immediately failed to open the connection. Since that usually
	    // indicates a severe problem, bail out immediately.
	    //
	    finishAndNotify(getLastResult());
	}
    } else {
	PltLog::Error("KscServer::reconnectTimerTrigger: timer event without waiting request");
    }
} // KscServer::reconnectTimerTrigger


// ----------------------------------------------------------------------------
// Attention callback that is responsible for handling events from the
// transport connection, such as establishing a connection or receiving a
// reply.
//
void
KscServer::async_attention(KsServerConnection::KsServerConnectionOperations op)
{
    KscServiceRequestHandle request = _current_request;
    KS_RESULT res = _svr_con->getLastResult();

    switch ( op ) {
    //
    // Connection has been successful established (or not).
    //
    case KsServerConnection::ISC_OP_OPEN:
	if ( res != KS_ERR_OK ) {
	    //
	    // Connection establishment has failed. So count down and
	    // try a reconnect later... but only do this if there is a
	    // chance of repeated attempts succeeding.
	    //
	    // Note: do not retry ping attempts.
	    //
#if DEBUG_CLN
	    PltLog::Debug("KscServer::async_attention: open failed");
#endif
	    if ( request->_service
		 && shouldReconnectServer(res)
		 && --_tries_remaining > 0 ) {
		_reconnect_timer->setTrigger(PltTime::now(_retry_wait));
		KsConnectionManager::getConnectionManagerObject()->
		    addTimerEvent(_reconnect_timer);
	    } else {
		//
		// Exhausted all attempts for this request, so hand it over
		// to the requestor (terminator?!)
		//
#if DEBUG_CLN
		PltLog::Debug("KscServer::async_attention: no more retries, notifying owner");
#endif
		finishAndNotify(res);
	    }
	} else {
	    //
	    // Connection successfully established. So we can now send the
	    // ACPLT/KS service request.
	    //
	    if ( ! (
		     (request->_service != 0) ?
		         _svr_con->send(request->_service, 
					request->_neg, 
					request->_params)
		     :
		         _svr_con->sendPing()
		     ) ) {
		//
		// We failed to serialize the request. This usually indicates
		// a problem with the IO subsystem, so we don't try to redo
		// but instead bail out with an error. Release the current
		// request and notify the owner of the request of the problem.
		//
#if DEBUG_CLN
		PltLog::Debug("KscServer::async_attention: send failed");
#endif
		finishAndNotify(_svr_con->getLastResult());
	    } else {
		//
		// Since we could successfully serialize the service request
		// we now have to wait for a reply.
		//
#if DEBUG_CLN
		PltLog::Debug("KscServer::async_attention: successfully send request");
#endif
	    }
	}
	break;
    //
    // Reply for call has been arrived (or possibly not).
    //
    case KsServerConnection::ISC_OP_CALL:
	if ( res != KS_ERR_OK ) {
	    //
	    // Find out whether a reconnect would make sense.
	    //
	    // Note: do not retry ping attempts.
	    //
#if DEBUG_CLN
	    PltLog::Debug("KscServer::async_attention: call failed");
#endif
	    if ( request->_service
		 && shouldReconnectServer(res)
		 && --_tries_remaining > 0 ) {
		_reconnect_timer->setTrigger(PltTime::now(_retry_wait));
		KsConnectionManager::getConnectionManagerObject()->
		    addTimerEvent(_reconnect_timer);
	    } else {
		//
		// Exhausted all attempts for this request.
		//
#if DEBUG_CLN
		PltLog::Debug("KscServer::async_attention: no more retries, notifying owner");
#endif
		finishAndNotify(res);
	    }
	} else {	
	    //
	    // We got a reply back from the server. So pull it off of the
	    // network stream and look at it, wonder, and try to find out
	    // what it could mean...
	    //
	    if ( ! (
		     (request->_service != 0) ?
		         _svr_con->receive(request->_neg,
					   request->_result)
		     :
		         _svr_con->receivePing()
		     ) ) {
		//
		// Nope. Could not interpret the result. In order to be on
		// the safe side we close the transport connection and leave
		// an error indication.
		//
#if DEBUG_CLN
		PltLog::Debug("KscServer::async_attention: could not retrieve reply");
#endif
		if ( request->_service
		     && shouldReconnectServer(_svr_con->getLastResult())
		     && --_tries_remaining > 0 ) {
		    //
		    // Try once more...
		    //
		    _reconnect_timer->setTrigger(PltTime::now(_retry_wait));
		    KsConnectionManager::getConnectionManagerObject()->
			addTimerEvent(_reconnect_timer);
		} else {
		    //
		    // Exhausted all attempts for this request.
		    //
#if DEBUG_CLN
		    PltLog::Debug("KscServer::async_attention: no more retries, notifying owner");
#endif
		    finishAndNotify(_svr_con->getLastResult());
		}
	    } else {
		//
		// Got the reply and could successfully deserialize it into
		// the apropriate KS C++ objects. Notify the request's owner
		// that the request is done.
		//
#if DEBUG_CLN
		PltLog::Debug("KscServer::async_attention: reply successfully decoded");
#endif
		//
		// In case of a successfull ping, we need to explitly clear
		// the result field, since finishAndNotify() won't touch it
		// in case of KS_ERR_OK.
		//
		if ( !request->_service ) {
		    request->_result.result = KS_ERR_OK;
		}
		finishAndNotify(KS_ERR_OK);
	    }
	}
    }
} // KscServer::async_attention


// ----------------------------------------------------------------------------
// Decide, whether we should give the connect() call another try, depending on
// the current ACPLT/KS error result. This helper is used whenever we fail to
// open, send or receive with respect to the transport to the server.
//
bool
KscServer::shouldReconnectServer(KS_RESULT result)
{
    switch ( result ) {
    case KS_ERR_HOSTUNKNOWN:
    case KS_ERR_SERVERUNKNOWN:
    case KS_ERR_GENERIC:
	return false;
    default:
	return true;
    }
} // KscServer::shouldReconnectServer


// ----------------------------------------------------------------------------
// Block and wait for a particular request to become finished. When waiting,
// background IO is given some CPU time, so it can be carried out.
//
void
KscServer::waitForRequest(KscServiceRequestHandle req)
{
    //
    // It does not make sense to wait for a non-waiting or non-pending
    // request, so we immediately return in this case.
    //
    switch ( req->_status ) {
    case KscServiceRequest::REQ_NONE:
    case KscServiceRequest::REQ_DONE:
	return;
    default:
	break;
    }
    //
    // The request has been queued or is even currently serviced. Since the
    // caller wants to wait for its completion, we now give idle time to the
    // connection manager, so that it can do IO. After doing a little bit, the
    // connection manager will return control and thus give us the chance to
    // check whether our request was completed. If yes, we return to the
    // caller with the service result. Otherwise, we call the connection
    // manager again to do background IO.
    //
    while ( req->_status != KscServiceRequest::REQ_DONE ) {
#if DEBUG_CLN
	PltLog::Debug("waiting for request to become finished");
#endif
	KsConnectionManager::getConnectionManagerObject()->
	    servePendingEvents(PltTime(1));
    }
} // KscServer::waitForRequest






// ----------------------------------------------------------------------------
// requestService() implements a general service request.
// The name of the protocol extension determines the major opcode which is
// dynamically loaded from the server. The caller is responsible for
// providing a valid minor opcode.
//
bool 
KscServer::requestService(const KsString &extension,
                          u_short minor_opcode,
                          const KscAvModule *avm,
                          const KsXdrAble &params,
                          KsResult &result)
{
#if 0
    u_long opcode = 0;
    //
    // Lookup the major opcode in cache. If the lookup fails then query the
    // ACPLT/KS server (ask for /vendor/extensions/pipapo/major_opcode).
    //
    if( !ext_opcodes.query(extension, opcode) ) {
	//
      	// Fetch the opcode from the ACPLT/KS server.
      	// NOTE: It is safe to use a KscVariable as
      	//       the services of ks_core are implemented
      	//       independent from requestService().
	//
	KsString path(getHostAndName());
	path += "/vendor/extensions/";
	path += extension;
	path += "/major_opcode";

	KscVariable var(path);
	KsValueHandle hval; 

	//
	// Yes. There might be errors. In case there isn't a description of
	// the extension available, then return KS_ERR_NOTIMPLEMENTED, other-
	// wise return the error code resulting from the failed communication.
	//
	if ( !var.hasValidPath() ) {
	    _last_result = KS_ERR_NOTIMPLEMENTED;
	    return false;
	}
	if ( !var.getUpdate() ) {
	    _last_result = var.getLastResult();
	    if ( (_last_result == KS_ERR_BADNAME) ||
	         (_last_result == KS_ERR_BADPATH) ) {
		_last_result = KS_ERR_NOTIMPLEMENTED;
	    }
	    return false;
	}
	
	hval = var.getValue();
	if ( !hval ) {
	    _last_result = KS_ERR_NOTIMPLEMENTED;
	    return false;
	}

	KsIntValue *pval = PLT_DYNAMIC_PCAST(KsIntValue, hval.getPtr());
	if ( !pval ) {
	    _last_result = KS_ERR_NOTIMPLEMENTED;
	    return false;
	}

	// We succeeded in reading the opcode.
	// 
	opcode = (u_long)((long)(*pval));
    } 
    //
    // We got the major opcode from the cache or from the server.
    // Now mix the opcodes and call internal function for transfer.
    //
    opcode <<= 16;
    opcode |= minor_opcode;

    return requestByOpcode(opcode, avm, params, result);
#endif
    _last_result = KS_ERR_GENERIC;
    return false;
} // KscServer::requestService







// ---------------------------------------------------------------------------
// Synchronous ACPLT/KS service request. This method does not return before
// this request has been served. However, while waiting for the completion of
// this request, other requests will be served in the background.
//
bool
KscServer::requestByOpcode(u_long service, 
                           const KscAvModule *avm,
                           const KsXdrAble &params,
                           KsResult &result) 
{
    KscServiceRequest req(service, avm, params, result);
    KscServiceRequestHandle hreq(&req, KsOsUnmanaged);
    if ( !hreq ) {
	_last_result = KS_ERR_GENERIC;
	return false;
    }
    requestAsyncByOpcode(hreq);
    waitForRequest(hreq);
    return req._result.result == KS_ERR_OK;
} // KscServer::requestByOpcode


// ----------------------------------------------------------------------------
// Issue an ACPLT/KS request asynchronously in the background.
//
// Note: if the request is for service id 0 (_service == 0), then the params
// and the result object references are ignored, but instead an ordinary
// ONC/RPC ping is carried out (progid = KS).
//
bool
KscServer::requestAsyncByOpcode(KscServiceRequestHandle hreq) 
{
    if ( !hreq
	 || hreq->_status != KscServiceRequest::REQ_NONE ) {
	_last_result = KS_ERR_GENERIC;
	return false;
    }
    initiateRequestIfPossible(hreq);
    _last_result = KS_ERR_OK;
    return true;
} // KscServer::requestAsyncByOpcode



// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// And now for the core ACPLT/KS services. They're simply implemented on top
// of the requestByOpcode method.
//
bool 
KscServer::getPP(const KscAvModule *avm,
                 const KsGetPPParams &params,
                 KsGetPPResult &result)
{
    return requestByOpcode(KS_GETPP, avm, params, result);
} // KscServer::getPP


bool 
KscServer::getVar(const KscAvModule *avm,
                  const KsGetVarParams &params,
                  KsGetVarResult &result)
{
    return requestByOpcode(KS_GETVAR, avm, params, result);
} // KscServer::getVar


bool 
KscServer::setVar(const KscAvModule *avm,
                  const KsSetVarParams &params,
                  KsSetVarResult &result)
{
    return requestByOpcode(KS_SETVAR, avm, params, result);
} // KscServer::setVar


bool
KscServer::exgData(const KscAvModule *avm,
                   const KsExgDataParams &params,
                   KsExgDataResult &result)
{
    return requestByOpcode(KS_EXGDATA, avm, params, result);
} // KscServer::exgData




// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// Now for something completely different... the A/V modules &
// negotiator mechanism. "What's this?" you'll surely ask. Well --
// that's something special... Okay, a someone better explanation:
// A/V modules are the abstract view of A/V mechanisms. For every A/V
// mechanism there is a suitable A/V module class. Instances of such
// a class will mainly hold information necessary for the A/V mecha-
// nism, like a password etc. In contrast to this, the negotiators
// are used to carry out the low-level communication of an A/V mecha-
// nism. Whenever you use a A/V module with a particular server, then
// there's also a negotiator for that A/V module and particular
// server. The negotiator takes care of the communication and can hold
// state information on a per-connection (aka per-server) basis.
//
// But now for the next function below. It returns a suitable nego-
// tiator for the given A/V module object. If no such negotiator for
// this server currently exists, then a new negotiator is created.
//
// CAUTION:
//   pointers returned by this function are for temporary use only
//
KscNegotiator *
KscServer::getNegotiator(const KscAvModule *avm)
{
    //
    // If no A/V module object has been specified, then fall back
    // to the A/V module set for this server. If no such A/V module
    // has been set for this server, then fall back to the client's
    // A/V module. This scheme allows us to call getNegotiator() with
    // a A/V module object pointer stolen from a communication object.
    // So, if that communication object has no A/V module object set,
    // then can simply fall back to the defaults.
    //
    if ( !avm ) {
        if ( av_module ) {
            avm = av_module;
        } else {
            avm = KscClient::getClient()->getAvModule();
        }
    }

    if ( avm ) {
	//
        // Try to find a negotiator in the cache of this server object.
        //
        PltKeyPlainConstPtr<KscAvModule> tkey(avm);
        KscNegotiatorHandle hneg;

        if ( neg_table.query(tkey, hneg) ) {
	    //
            // Okay, we've found one. Return it to the caller.
            //
            if ( hneg ) {
		return hneg.getPtr();
	    }
        } else {
	    //
            // None found, so create one and remember it in the
	    // cache.
            //
            hneg = avm->getNegotiator(this);
            if ( hneg ) {
                if ( neg_table.add(tkey, hneg) ) {
                    return hneg.getPtr();
                }
            }
        }
    }

    //
    // Well -- either still no A/V module object set, or we failed
    // to create a negotiator object. So use the A/V NONE negotiator,
    // which is always available (static object).
    //
    return KscAvNoneModule::getStaticNegotiator(); 
} // KscServer::getNegotiator


// ----------------------------------------------------------------------------
// Dismiss (aka as "fire") a negotiator when its A/V module is just about to
// hit the bit bucket -- looks like my descriptions could directly stemm from
// Monty Phyton's famous RAF sketches.
//
// Negotiators can only live as long as their corresponding A/V module or as
// the server connection they're negotiating on is closed -- whichever comes
// first. In case of the A/V module being destroyed ("dtor'd"), we need to
// remove the negotiator from a server proxy's negotiator cache, because it
// must not be used anymore.
//
void
KscServer::dismissNegotiator(const KscAvModule *avm)
{
    //
    // Try to find a negotiator in the cache of this server object for the
    // given A/V module. Note that there can be only at most one negotiator
    // for a given A/V module and server connection.
    //
    if ( avm != 0 ) {
	PltKeyPlainConstPtr<KscAvModule> key(avm);
	KscNegotiatorHandle hNegotiator;
    	neg_table.remove(key, hNegotiator);
    }
} // KscServer::dismissNegotiator


// ----------------------------------------------------------------------------
// Ping a server. In case the connection is not already open, we open it first.
// Please note that this does not reconnect automatically *multiple* times.
// This is a *blocking* function. Use requestAsyncByOpcode() with a service
// of zero to carry out an asynchronous ping.
//
bool
KscServer::ping()
{
    KsResult dummy;
    return requestByOpcode(0, 0, dummy, dummy);
} // KscServer::ping

        
// End of client.cpp
