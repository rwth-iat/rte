/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/src/client.cpp,v 1.29 1998-01-29 12:56:23 harald Exp $ */
/*
 * Copyright (c) 1996, 1997, 1998
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

/* Author: Markus Juergens <markusj@plt.rwth-aachen.de> */
/* Dumb comments by: Harald Albrecht <harald@plt.rwth-aachen.de> */

//////////////////////////////////////////////////////////////////////

#include <string.h>

#include <plt/log.h>

#include "ks/client.h"
#include "ks/ks.h"

#if PLT_DEBUG
#include <iostream.h>
#endif


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
    if ( (host_and_server.len() < 5) ||
	 (host_and_server[0] != '/') || (host_and_server[1] != '/') ) {
	return KS_ERR_MALFORMEDPATH;
    }

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
{}

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
KscClient::createServer(KsString host_and_name, KscServerBase *&pServer)
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
	    KscServer *temp = new KscServer(host, 
					    KsServerDesc(server,
							 KS_PROTOCOL_VERSION));
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
    PLT_ASSERT(ok && !pServer);
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

//////////////////////////////////////////////////////////////////////

void 
KscClient::setTimeouts(const PltTime &rpc_timeout,        
                       const PltTime &retry_wait,         
                       size_t tries)
{
    _rpc_timeout = rpc_timeout;
    _retry_wait = retry_wait;
    _tries = tries;
}

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
    
//////////////////////////////////////////////////////////////////////
// class KscServer
//
// Constructor
// Just copies the given data for later use.
//
KscServer::KscServer(KsString host, 
                     const KsServerDesc &server)
: KscServerBase(host, server.name),
  server_desc(server),
  _client_transport(0),
  _rpc_timeout(KSC_RPCCALL_TIMEOUT),
  _retry_wait(0, 0),
  _tries(1),
  last_ip(INADDR_NONE)
{
}

//////////////////////////////////////////////////////////////////////

KscServer::KscServer(KsString hostAndName, u_short protocolVersion)
: KscServerBase(hostAndName),
  server_desc(server_name, protocolVersion),
  _client_transport(0),
  _rpc_timeout(KSC_RPCCALL_TIMEOUT),
  _retry_wait(0, 0),
  _tries(1),
  last_ip(INADDR_NONE)
{
}

//////////////////////////////////////////////////////////////////////
// Destructor
// We close the TCP connection. 
//
KscServer::~KscServer()
{
    destroyTransport();
}


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

    if( _client_transport ) {
#if PLT_SYSTEM_SOLARIS
        bool ok = clnt_control(_client_transport,
                               CLSET_TIMEOUT,
                               (char *)((struct timeval *)(&_rpc_timeout)));
#else
        bool ok = clnt_control(_client_transport,
                               CLSET_TIMEOUT,
                               (struct timeval *)(&_rpc_timeout));
#endif
        if(!ok) {
            PltLog::Warning("Failed to specifify timeout value for RPC");
        }
    }
}

//////////////////////////////////////////////////////////////////////
// Get the IP address of the host this server object points to. This
// helper function also deals with multi-homed hosts (respective hosts
// with multiple IP addresses) and makes sure that on consecutive
// requests we will never try the same address twice in a row if the
// host has more than one address.
//
bool
KscServer::getHostAddr(struct sockaddr_in *addr)
{
#if 0
    // hostentry struct used for debbugging only
    //
    KSC_IP_TYPE ip_terra = inet_addr("134.130.125.131");
    KSC_IP_TYPE ip_pc212 = inet_addr("134.130.125.76");

    KSC_IP_TYPE *dummy1[3] = { &ip_terra, &ip_pc212, 0 };
    struct hostent dummy = { 
 	"", 0, AF_INET, sizeof(KSC_IP_TYPE),
	(char **)dummy1 };
    KscHostEnt he(&dummy);
#endif

    // TODO: check data type for 64 bits
    const unsigned long INVALID_IP = INADDR_NONE;
    const unsigned long NO_IP      = INADDR_ANY;

    //
    // As of version 1.02 we now support a explicit given port address
    // of the ACPLT/KS manager in the hostname. This is desirable in order
    // to access ACPLT/KS servers behind firewalls without making the
    // portmapper accessible from the dangerous "outback".
    //
    KsString    DNS_name;
    const char *pColon;

    pColon = strchr(host_name, ':');
    if ( pColon ) {
        //
        // Seems like the hostname contains an explicit port number. At
        // this time we only need the real hostname part, so get rid of
        // the other part here...
        //
        int hostpart_len = pColon - (const char *) host_name;
        if ( hostpart_len <= 0 ) {
            return false;
        }
        DNS_name = host_name.substr(0, hostpart_len);
    } else {
        //
        // It's the traditional hostname style without an explicit
        // port address.
        //
        DNS_name = host_name;
    }

    //
    // Clean up first and zero out the internet address.
    //
    memset(addr, 0, sizeof(struct sockaddr_in));

    //
    // HP-UX doesn't like an empty host name string. It then returns
    // 0, that is, "0.0.0.0". Okay, in principle this is an invalid IP
    // address. It can be only used by a boot-strapping client which
    // do not know yet its own IP address. But we aren't in this
    // situation here, so "0.0.0.0" is simply forbidden.
    //
    KSC_IP_TYPE ip;
    if ( DNS_name.len() ) {
	ip = inet_addr(DNS_name);
    } else {
	ip = INVALID_IP;
    }

    if ( (ip == INVALID_IP) || (ip == NO_IP) ) {
	//
        // host_name is no valid dotted IP address, so we try now to
        // resolve it as a DNS name.
        //
#if 1
	KscHostEnt he(gethostbyname((const char *) DNS_name));
#endif
	if ( (he.numIP() > 1) && (he.getIP(0) == last_ip) ) {
	    ip = he.getIP(1);
	} else if ( he.numIP() ) {
	    ip = he.getIP(0);
	} else {
	    //
	    // The name lookup failed. So we can't do anything more...
	    //
#if PLT_DEBUG
            PltString err_msg("Failed to get IP address of host: ", host_name);
            PltLog::Warning(err_msg);
#endif
            return false;
        }  
    } 

    last_ip = ip;    
    addr->sin_addr.s_addr = ip;
    return true;
} // KscServer::getHostAddr


//////////////////////////////////////////////////////////////////////
// Try to establish a RPC connection to the ACPLT/KS server. This
// includes querying the portmapper and ACPLT/KS manager on the host
// where the server is supposed to reside. If this function succeeds
// then it will return true, otherwise false. The last result will be
// set accordingly.
//
bool
KscServer::createTransport()
{
    //
    // Destroy the old transport connection if one exists already.
    // This way we can make a fresh start even after something went
    // really wrong during a previous communication procedure.
    //
    destroyTransport();

    struct sockaddr_in host_addr; 
    int socket = RPC_ANYSOCK;
    
    //
    // Fetch IP address of the host where this ACPLT/KS server is
    // (well -- supposed to be) located. If this fails, then set the
    // result state accordingly and return to the caller. Well --
    // normally setResultAfterService() will set _last_result depending
    // on the status member, but just in case someone forgets this...
    //
    if ( !getHostAddr(&host_addr) ) {
        _last_result = KS_ERR_HOSTUNKNOWN;
        return false;
    }

    //
    // Because we now support specifying the port number of the
    // ACPLT/KS manager directly within the hostname, we must first
    // watch out for an optional port number.
    //
    const char *pColon = strchr(host_name, ':');
    unsigned short port = 0; // default is "ask the portmapper"

    if ( pColon ) {
        //
        // Seems like there is a port number embedded in the hostname,
        // so we try to extract it. If there is anything wrong with this
        // port number, either because it's an invalid string or the
        // value is out of range, bail out with an malformed path error.
        //
        char *pEnd;
        long port_no;

        ++pColon; // advance to the port number string
        port_no = strtol(pColon, &pEnd, 10);
        if ( *pColon && !*pEnd ) {
            if ( (port_no > 0l) && (port_no <= 65535l) ) {
                port = (unsigned short) port_no;
            } else {
                _last_result = KS_ERR_MALFORMEDPATH;
                return false;
            }
        } else {
            _last_result = KS_ERR_MALFORMEDPATH;
            return false;
        }
    }

    //
    // Now request the ACPLT/KS server description from the ACPLT/KS
    // manager. If we got something back, then make sure that the
    // request itself succeeded and not only the basic communication.
    // That is, if the result member of the service request reply is
    // not KS_ERR_OK, then the manager could not successfully serve the
    // request.
    //
    if ( !getServerDesc(&host_addr, port, server_desc, server_info) ) {
        //
        // Note: don't change the last result set by getServerDesc().
        // Just fall through.
        //
        return false;
    }
        
    if ( server_info.result != KS_ERR_OK ) {
        //
        // If the ACPLT/KS manager returned a service error, then we
        // will fall back to the error "unknown server". If we would
        // return the service result verbatim, then it could confuse
        // the client, because it does not know that the error resulted
        // from the server lookup and not from the requested operation
        // itself.
        //
        _last_result = KS_ERR_SERVERUNKNOWN;
        PLT_DMSG("Unknown server " << getHostAndName() << endl);
        return false;
    }

#if PLT_DEBUG_PEDANTIC
    cerr << "Trying to connect server at port "
         << server_info.port
         << endl;
#endif

    //
    // Okay, we've just managed the first stage of creating a communication
    // transport. We now have to create the client handle which is necessary
    // for communicating with the ACPLT/KS server itself. This time, we're
    // using a stream transport (TCP) because we may need to transfer large
    // amount of data and it would be ridiculous to stack up reliability on
    // the UDP transport. For a nice comment on reliability for UDP
    // applications please refer to W.R. Stevens, Unix Network Programming,
    // Volume 1: "Networking APIs: Sockets and XTI", Second Edition, p. 542
    //
    host_addr.sin_family = AF_INET;
    host_addr.sin_port = htons(server_info.port);
    
    // TODO: do a timeout-controllable connect(). See R. Stevens for details
    _client_transport = clnttcp_create(&host_addr,
				       KS_RPC_PROGRAM_NUMBER,
				       server_desc.protocol_version,
				       &socket,
				       0, 0);

    if( !_client_transport ) {
        _last_result = KS_ERR_CANTCONTACT;
        return false;
    }

    //
    // Now set the timeout parameter for this particular communication
    // transport.
    //
#if PLT_SYSTEM_SOLARIS
    bool ok = clnt_control(_client_transport, CLSET_TIMEOUT, 
                           (char *)((struct timeval *)(&_rpc_timeout)));
#else
    bool ok = clnt_control(_client_transport, CLSET_TIMEOUT, 
                           (struct timeval *)(&_rpc_timeout));
#endif
    if ( !ok ) {
        PltLog::Warning("Failed to set the timeout value for the RPC transport");
    }

    _last_result = KS_ERR_OK;
    return true;
} // KscServer::createTransport

//////////////////////////////////////////////////////////////////////

void 
KscServer::destroyTransport()
{
    if ( _client_transport ) {
        clnt_destroy(_client_transport);
        _client_transport = 0;
    }
} // KscServer::destroyTransport

//////////////////////////////////////////////////////////////////////
#if 0
bool
KscServer::getStateUpdate()
{
    struct sockaddr_in host_addr;
    
    // locate host
    //
    if( !getHostAddr(&host_addr) ) {
        _status = KscHostNotFound;
        _last_result = KS_ERR_HOSTUNKNOWN;
        return false;
    }

    // request server description from manager
    //
    if( !getServerDesc(&host_addr, 0, server_desc, server_info) ) {
        // dont change status set by getServerDescription()
        return false;
    }
    if( server_info.result != KS_ERR_OK ) {
        _status = KscNoServer;
        return false;
    }
    _last_result = server_info.result;
    return true;
}
#endif


//////////////////////////////////////////////////////////////////////
// Here's the magic stepping in... to make ACPLT/KS more reliable in
// the face of network problems, we try to reconnect to a ACPLT/KS if
// the connection went done for any reason. A particular reason to
// throw away the old connection and to reconnect are especially
// timeouts. This way both server and client can resynchronize their
// communication. Even with the ONC/RPC record marking standard there's
// a good chance that client and server will deadlock themselves trying
// to resynchronize on the old connection. So closing the connection is
// much better as this clearly allows for a fresh start.
//
// Okay, so much for the preliminaries. Now what is this sucker below
// doing? Well - he's just responsible for deciding whether we should
// do a reconnect after a communication failure or not. He decides this
// based on the retries already carried out and the exact communication
// failure. Note that this function does *NOT* set the last result.
// If reconnectServer() decides that a reconnect is possible, then it
// will sleep (configurable) and then try to reopen a communication
// transport.
//
bool
KscServer::reconnectServer(size_t try_count, enum clnt_stat errcode)
{
    //
    // Have we reached the maximum number of tries? Then we'll bail out
    // in every case. No more chances. Game over. Tilt.
    //
    if ( try_count >= _tries ) {
        return false;
    }

    //
    // As a side note: RPC_CANTDECODEARGS is not retryable, because it
    // signals an error on the client side.
    //
    if ( (errcode == RPC_CANTSEND) ||
	 (errcode == RPC_CANTRECV) ||
	 (errcode == RPC_TIMEDOUT) ||
	 (errcode == RPC_FAILED) )
    {
	//
	// Yes, we will try to reconnect. But first wait a configurable
	// timespan before trying to re-establish a communication
	// transport.
	//
        wait(_retry_wait);
        createTransport();
        return reconnectServer(_last_result);
    } else {
	//
	// No. You loose.
	//
        return false;
    }
} // KscServer::reconnectServer


//////////////////////////////////////////////////////////////////////
// Oh yes. One more system-independent sleep(). Sometime this will be
// getting in the generic libplt...
//
bool
KscServer::wait(PltTime howLong)
{
    if ( (howLong.tv_sec == 0) && (howLong.tv_usec == 0) ) {
        return true;
    } else {
        PltTime now(PltTime::now());
        PltTime until(now);
        until += howLong;

        PLT_DMSG("Going to wait.." << howLong.tv_sec << endl);
        
        do {
#if PLT_SYSTEM_NT
	    //
            // Sleep for Windooze NT/95 expects the time to wait in
	    // milliseconds.
            //
            PLT_DMSG("Calling Sleep()" << endl);
            Sleep(howLong.tv_sec*1000 + howLong.tv_usec / 1000);
#else
	    //
	    // The traditional BSD way: call select() with no file
	    // descriptors to monitor and the timeout value. So
	    // practical that even the SYSV people like it that way.
	    //
            PLT_DMSG("Calling select()" << endl);
            select(0, 0, 0, 0, &howLong);
#endif
            now = PltTime::now();
            howLong = until - now;
        } while ( until > now );

        PLT_DMSG("Finished waiting" << endl);

        return true;
    }
} // KscServer::wait

//////////////////////////////////////////////////////////////////////
//
void
KscServer::setResultAfterService(enum clnt_stat errcode)
{
#if 0
    switch ( _status ) {
    case KscHostNotFound : 
        _last_result = KS_ERR_HOSTUNKNOWN;
        return;
    case KscCannotCreateClientHandle :
        _last_result = KS_ERR_CANTCONTACT;
        return;
    case KscCannotCreateUDPClient :
        if ((_errcode == RPC_PROGNOTREGISTERED) ||
            (_errcode == RPC_PROGUNAVAIL) ) {
            _last_result = KS_ERR_NOMANAGER;
        } else {
            _last_result = KS_ERR_CANTCONTACT;
        }
        return;
    case KscNoServer :
        _last_result = KS_ERR_SERVERUNKNOWN;
        return;
    case KscNoManager:
	//
	// Couldn't get a ACPLT/KS manager on the given host. At least
	// the portmapper was running, but no ACPLT/KS manager.
	//
	_last_result = KS_ERR_NOMANAGER;
	return;
    default:
        ;     // behavior depends on errcode, see next switch
    }
#endif

    switch ( errcode ) {
    case RPC_SUCCESS :
        _last_result = KS_ERR_OK;
        return;
    case RPC_TIMEDOUT:
        _last_result = KS_ERR_TIMEOUT;
        return;
    case RPC_CANTSEND:
    case RPC_CANTRECV:
	_last_result = KS_ERR_NETWORKERROR;
	return;
    case RPC_CANTENCODEARGS:
    case RPC_CANTDECODERES:
	_last_result = KS_ERR_GENERIC;
	return;
    case RPC_CANTDECODEARGS:
	_last_result = KS_ERR_NETWORKERROR;
	return;
    default:
        _last_result = KS_ERR_GENERIC;
    }
} // KscServer::setResultAfterService


//////////////////////////////////////////////////////////////////////
// Decide, whether we should give the connect() call another try,
// depending on the current ACPLT/KS error result. This helper is
// used after a connect()
//
bool
KscServer::reconnectServer(KS_RESULT result)
{
    switch ( result ) {
    case KS_ERR_HOSTUNKNOWN:
    case KS_ERR_SERVERUNKNOWN:
	return false;
    default:
	return true;
    }
} // KscServer::reconnectServer


//////////////////////////////////////////////////////////////////////
// Here are the getServerDesc() and helper structs and classes
// comming. They implement the GetServer request which is used with
// ACPLT/KS managers to find where a particular ACPLT/KS server can
// be contacted.
//
// The following two structures associate an A/V ticket supplied
// ("negotiator") and the service request or reply. Both structures
// are used to supply the negotiator and the request/reply structures
// to the XDR encode/decode functions.
//
struct KscGetServerInStruct 
{
    KscGetServerInStruct(KscNegotiator *avTicket, 
                         KsGetServerParams *params)
    : avt(avTicket),
      gsp(params)
    {}

    KscNegotiator *avt; 
    KsGetServerParams *gsp;
}; // struct KscGetServerInStruct

struct KscGetServerOutStruct
{
    KscGetServerOutStruct(KscNegotiator *neg,
                          KsGetServerResult *result)
    : avt(neg),
      gsr(result)
    {}

    KscNegotiator *avt; 
    KsGetServerResult *gsr;
}; // struct KscGetServerOutStruct

//////////////////////////////////////////////////////////////////////
// Following are the two XDR toplevel encoding and decoding functions.
// They are responsible for sending and receiving the ACPLT/KS service
// request and reply for the GetServer service.
//
static bool_t 
KscGetServerInHelper(XDR *xdr, void *p)
{
    PLT_PRECONDITION((xdr->x_op == XDR_ENCODE) && p);

    KscGetServerInStruct *data = (KscGetServerInStruct *)p;
    
    PLT_ASSERT(data->gsp && data->avt);
    
    return data->avt->xdrEncode(xdr) 
        && data->gsp->xdrEncode(xdr);
} // KscGetServerInHelper

static bool_t
KscGetServerOutHelper(XDR *xdr, void *p)
{
    PLT_PRECONDITION((xdr->x_op == XDR_DECODE) && p);
    
    KscGetServerOutStruct *data = 
        (KscGetServerOutStruct *)p;
    
    PLT_ASSERT(data->avt && data->gsr);

    return data->avt->xdrDecode(xdr)
        && data->gsr->xdrDecode(xdr);
} // KscGetServerOutHelper

//////////////////////////////////////////////////////////////////////
// Okay, here's the real helper function coming. Feed it with the IP
// address of a host and the description of a ACPLT/KS server, then it
// will return more information about this server, for instance, at
// which port it can be found. If this helper fails (for various reasons,
// like network problems...), then it will return false and set the
// last result accordingly, so the call can use this information to gain
// more information about what went wrong. Please note that in case this
// function returns true, then this only means, that the request was
// successfully done, but you still must check the result code in the
// server description returned. That's necessary because although the
// communication succeeded the request itself could have been failed,
// for instance, there's no such ACPLT/KS server available at the host.
//
bool
KscServer::getServerDesc(struct sockaddr_in *host_addr, 
                         unsigned short port,
                         const KsServerDesc &server,
                         KsGetServerResult &server_desc)
{
    int socket = RPC_ANYSOCK;
    CLIENT *transport;
    enum clnt_stat errcode;

    //
    // Establish an UDP connection to the ACPLT/KS manager on the
    // given host (in host_addr). If we can't create a communication
    // transport at this point, then we'll return immediately with
    // an error code in the "last result".
    //
    // Note: as of version 1.0.2 we now support for an explicit
    // port number of the manager specified in the host name. In
    // this case we switch over to TCP instead of UDP because we
    // client most probably uses such hostnames to access ACPLT/KS
    // systems behind firewalls and UDP through a firewall is not
    // the most wanted protocol...
    //
    host_addr->sin_family = AF_INET;
    host_addr->sin_port = htons(port);
    
    if ( port ) {
        transport = clnttcp_create(host_addr,
                                   KS_RPC_PROGRAM_NUMBER,
                                   KS_PROTOCOL_VERSION,
                                   &socket,
                                   0, 0);
    } else {
        transport = clntudp_create(host_addr,
                                   KS_RPC_PROGRAM_NUMBER,
                                   KS_PROTOCOL_VERSION,
                                   KSC_UDP_TIMEOUT,
                                   &socket);
    }
    if( !transport ) {
	//
	// Try to figure out what exactly lead to our problem that
	// we can't create the UDP/TCP transport for communicating with the
	// ACPLT/KS manager. If the communication failed because there is
	// no ACPLT/KS manager, then exactly return this error, otherwise
	// fall back to the more generic "can't contact".
	//
	switch ( rpc_createerr.cf_stat ) {
	case RPC_PROGNOTREGISTERED:
	case RPC_PROGUNAVAIL:
	case RPC_PROGVERSMISMATCH:
	    _last_result = KS_ERR_NOMANAGER;
	    break;
	default:
	    _last_result = KS_ERR_CANTCONTACT;
	}
        return false;
    }

#if PLT_DEBUG_PEDANTIC
    // ping manager
    //
    errcode = clnt_call(transport, 0, 
                        xdr_void, 0, 
                        xdr_void, 0, 
                        KSC_UDP_TIMEOUT);

    if ( errcode != RPC_SUCCESS ) {
        cerr << "ping failed, error code " << (unsigned) errcode << endl;
    }
    else {
        cerr << "manager pinged" << endl;
    }
#endif

    //
    // Now ask the ACPLT/KS manager for the server description of the
    // server in question.
    //
    KsGetServerParams params(server);
    KscGetServerInStruct inData(
        KscAvNoneModule::getStaticNegotiator(), 
        &params);
    KscGetServerOutStruct outData(
        KscAvNoneModule::getStaticNegotiator(),
        &server_desc);

    errcode = clnt_call(transport, 
			KS_GETSERVER,
			(xdrproc_t) KscGetServerInHelper,
			(char *) &inData,
			(xdrproc_t) KscGetServerOutHelper,
			(char *) &outData,
			KSC_UDP_TIMEOUT);

    //
    // Free the resources used by communication transport and return
    // the information in question.
    //
    clnt_destroy(transport);

    if ( errcode != RPC_SUCCESS ) {
        PLT_DMSG("function call to MANAGER failed, error code " << (unsigned) errcode << endl);

        _last_result = KS_ERR_CANTCONTACT;
        return false;
    }

    _last_result = KS_ERR_OK;
    return true;
} // KscServer::getServerDesc


//////////////////////////////////////////////////////////////////////
// Here are the getPP() and helper structs and classes comming. They
// implement the GetPP request which is used to query the projected
// properties of zero, one, or more communication objects within an
// ACPLT/KS server.
//
// The following two structures associate an A/V ticket supplied
// ("negotiator") and the service request or reply. Both structures
// are used to supply the negotiator and the request/reply structures
// to the XDR encode/decode functions.
//
struct KscGetPPInStruct
{
    KscGetPPInStruct(KscNegotiator *avTicket,
                     const KsGetPPParams *params)
    : avt(avTicket),
      gpp(params)
    {}

    KscNegotiator *avt;
    const KsGetPPParams *gpp;
}; // struct KscGetPPInStruct

struct KscGetPPOutStruct
{
    KscGetPPOutStruct(KscNegotiator *neg,
                      KsGetPPResult *result)
    : avt(neg),
      gpr(result)
    {}

    KscNegotiator *avt;
    KsGetPPResult *gpr;
}; // struct KscGetPPOutStruct

//////////////////////////////////////////////////////////////////////
// Following are the two XDR toplevel encoding and decoding functions.
// They are responsible for sending and receiving the ACPLT/KS service
// request and reply for the GetPP service.
//
static bool_t 
KscGetPPInHelper(XDR *xdr, void *p)
{
    PLT_PRECONDITION((xdr->x_op == XDR_ENCODE) && p);

    KscGetPPInStruct *data = (KscGetPPInStruct *)p;
    
    PLT_ASSERT(data->gpp && data->avt);
    
    return data->avt->xdrEncode(xdr) 
        && data->gpp->xdrEncode(xdr);
} // KscGetPPInHelper

static bool_t
KscGetPPOutHelper(XDR *xdr, void *p)
{
    PLT_PRECONDITION((xdr->x_op == XDR_DECODE) && p);
    
    KscGetPPOutStruct *data = 
        (KscGetPPOutStruct *)p;
    
    PLT_ASSERT(data->avt);
    PLT_ASSERT(data->gpr);

    bool ok = data->avt->xdrDecode(xdr)
        && data->gpr->xdrDecode(xdr);

    return ok;
} // KscGetPPOutHelper

//////////////////////////////////////////////////////////////////////
// Okay. Now for the real getPP() function which issues the ACPLT/KS
// GetPP service request.
bool 
KscServer::getPP(const KscAvModule *avm,
                 const KsGetPPParams &params,
                 KsGetPPResult &result)
{
    enum clnt_stat errcode;

    //
    // Create client transport for the communication with the ACPLT/KS
    // server if that hasn't been done already. If there is no such
    // host or server then we will fail at this point and return to the
    // caller immediately. It makes no sense here to try reconnects yet.
    //
    if ( !_client_transport ) {
        if( !createTransport() ) {
            if ( !reconnectServer(_last_result) ) {
                return false;
            }
        }
    }

    //
    // Now locate the negotiator which will handle the A/V tickets for the
    // service request and reply. Then set up the ingoing and outcomming
    // service parameters, so the server object can lateron build the
    // data packets.
    //
    KscNegotiator *negotiator = getNegotiator(avm);

#if PLT_DEBUG
    cerr << "Requesting server " 
         << host_name << "/" << server_info.server.name 
         << " for getPP service." << endl;
    cerr << "Parameters : " << endl; 
    cerr << "\tPath : " << params.path << endl;
    cerr << "\tType mask : " << params.type_mask << endl;
    cerr << "\tName mask : " << params.name_mask << endl;
    cerr << "Using AV-Type : " << negotiator->typeCode() << endl; 
    cerr << endl;
#endif

    KscGetPPInStruct inData(negotiator, &params);
    KscGetPPOutStruct outData(negotiator, &result);

    //
    // Try to issue a GetPP service request until either the communication
    // succeeds or we've run out of tries.
    //
    size_t try_count = 0;
    do {
#if PLT_DEBUG
        cerr << "Trying for the " << (try_count+1) << "th. time" << endl;
#endif
        if ( _client_transport ) {
            errcode = clnt_call(_client_transport, KS_GETPP,
				(xdrproc_t) KscGetPPInHelper,
				(char *) &inData,
				(xdrproc_t) KscGetPPOutHelper,
				(char *) &outData,
				KSC_RPCCALL_TIMEOUT);
        } else {
	    //
	    // The reason why we're finding ourselves here at this branch is
	    // that trying to establish a new communication transport might
	    // have failed the last time.
	    //
            errcode = RPC_FAILED;
        }
    } while ( (errcode != RPC_SUCCESS)
	      && reconnectServer(++try_count, errcode) );

    //
    // If the communication itself failed (*NOT* a failure on the ACPLT/KS
    // level but on the RPC level), then destroy the transport. This has two
    // important reasons: first, the caller can easily reissue the request
    // with another call. Second, if the connection has timed out, then the
    // server will soon see that the client won't receive any information
    // and discard a potential service reply very soon.
    //
    if ( errcode != RPC_SUCCESS ) {
        destroyTransport();
    }

    setResultAfterService(errcode);
        
#if PLT_DEBUG
    if ( errcode == RPC_SUCCESS ) {
        cerr << "request successfull" << endl;
    }
    else {
        cerr << "request failed" << endl;
    }
#endif

    //
    // Quick Hint(tm): let the caller know whether something bad
    // happened or not. It can get the details with getLastResult()
    // later...
    //
    return _last_result == KS_ERR_OK;
} // KscServer::getPP


//////////////////////////////////////////////////////////////////////
// Here are the getVar() and helper structs and classes comming. They
// implement the GetVar request which is used to query the values (or
// current properties) of variables in an ACPLT/KS server.
//
// The following two structures associate an A/V ticket supplied
// ("negotiator") and the service request or reply. Both structures
// are used to supply the negotiator and the request/reply structures
// to the XDR encode/decode functions.
//
struct KscGetVarInStruct
{
    KscGetVarInStruct(KscNegotiator *avTicket,
                      const KsGetVarParams *params)
    : avt(avTicket),
      gvp(params)
    {}

    KscNegotiator *avt;
    const KsGetVarParams *gvp;
}; // KscGetVarInStruct

struct KscGetVarOutStruct
{
    KscGetVarOutStruct(KscNegotiator *neg,
                       KsGetVarResult *result)
    : avt(neg),
      gvr(result)
    {}

    KscNegotiator *avt;
    KsGetVarResult *gvr;
}; // KscGetVarOutStruct

//////////////////////////////////////////////////////////////////////
// Following are the two XDR toplevel encoding and decoding functions.
// They are responsible for sending and receiving the ACPLT/KS service
// request and reply for the GetVar service.
//
static bool_t 
KscGetVarInHelper(XDR *xdr, void *p)
{
    PLT_PRECONDITION((xdr->x_op == XDR_ENCODE) && p);

    KscGetVarInStruct *data = (KscGetVarInStruct *)p;
    
    PLT_ASSERT(data->gvp && data->avt);
    
    return data->avt->xdrEncode(xdr) 
        && data->gvp->xdrEncode(xdr);
} // KscGetVarInHelper

static bool_t
KscGetVarOutHelper(XDR *xdr, void *p)
{
    PLT_PRECONDITION((xdr->x_op == XDR_DECODE) && p);
    
    KscGetVarOutStruct *data = 
        (KscGetVarOutStruct *)p;
    
    PLT_ASSERT(data->gvr && data->avt);

    return data->avt->xdrDecode(xdr)
        && data->gvr->xdrDecode(xdr);
} // KscGetVarOutHelper

//////////////////////////////////////////////////////////////////////
// Okay. Now for the real getVar() function which issues the ACPLT/KS
// GetVar service request. Because that would be too less work, it also
// takes care of opening the connection, connection breakdowns, and
// other network-related nasties. Imagine: just twenty years ago,
// computers worked more or less, nowadays (thanks to network technology)
// computers usually work even less...
//
bool
KscServer::getVar(const KscAvModule *avm,
                  const KsGetVarParams &params,
                  KsGetVarResult &result)
{
    enum clnt_stat errcode;

    //
    // Create client transport for the communication with the ACPLT/KS
    // server if that hasn't been done already. If there is no such
    // host or server then we will fail at this point and return to the
    // caller immediately. It makes no sense here to try reconnects yet.
    //
    if ( !_client_transport ) {
        if( !createTransport() ) {
            if ( !reconnectServer(_last_result) ) {
                return false;
            }
        }
    }

    //
    // Now locate the negotiator which will handle the A/V tickets for the
    // service request and reply. Then set up the ingoing and outcomming
    // service parameters, so the server object can lateron build the
    // data packets.
    //
    KscNegotiator *negotiator = getNegotiator(avm);

#if PLT_DEBUG
    cerr << "GetVar request to server "
         << host_name << "/" << server_info.server.name << endl;
    cerr << "Using AV-type : " << negotiator->typeCode() << endl;
#endif

    KscGetVarInStruct inData(negotiator, &params);
    KscGetVarOutStruct outData(negotiator, &result);
    
    //
    // Try to issue a GetVar service request until either the communication
    // succeeds or we've run out of tries.
    //
    size_t try_count = 0;
    do {
#if PLT_DEBUG
        cerr << "Trying for the " << (try_count+1) << ".th time" << endl;
#endif
        if ( _client_transport ) {
            errcode = clnt_call(_client_transport, KS_GETVAR,
				(xdrproc_t) KscGetVarInHelper,
				(char *) &inData,
				(xdrproc_t) KscGetVarOutHelper,
				(char *) &outData,
				KSC_RPCCALL_TIMEOUT);
        } else {
	    //
	    // The reason why we're finding ourselves here at this branch is
	    // that trying to establish a new communication transport might
	    // have failed the last time.
	    //
            errcode = RPC_FAILED;
        }
    } while ( (errcode != RPC_SUCCESS)
	      && reconnectServer(++try_count, errcode) );

    //
    // If the communication itself failed (*NOT* a failure on the ACPLT/KS
    // level but on the RPC level), then destroy the transport. This has two
    // important reasons: first, the caller can easily reissue the request
    // with another call. Second, if the connection has timed out, then the
    // server will soon see that the client won't receive any information
    // and discard a potential service reply very soon.
    //
    if ( errcode != RPC_SUCCESS ) {
        destroyTransport();
    }

    setResultAfterService(errcode);

#if PLT_DEBUG
    if ( errcode == RPC_SUCCESS ) {
        cerr << "GetVar request successfull" << endl;
    }
    else {
        cerr << "GetVar request failed, error code : "
             << (unsigned) errcode << endl;
    }
#endif

    //
    // Quick Hint(tm): let the caller know whether something bad
    // happened or not. It can get the details with getLastResult()
    // later...
    //
    return _last_result == KS_ERR_OK;
} // KscServer::getVar


//////////////////////////////////////////////////////////////////////
// Here are the setVar() and helper structs and classes comming. They
// implement the SetVar request which is used to write new values (or
// current properties) to variables within an ACPLT/KS server.
//
// The following two structures associate an A/V ticket supplied
// ("negotiator") and the service request or reply. Both structures
// are used to supply the negotiator and the request/reply structures
// to the XDR encode/decode functions.
//
struct KscSetVarInStruct
{
    KscSetVarInStruct(KscNegotiator *avTicket,
                      const KsSetVarParams *params)
    : avt(avTicket),
      svp(params)
    {}

    KscNegotiator *avt;
    const KsSetVarParams *svp;
}; // KscSetVarInStruct

struct KscSetVarOutStruct
{
    KscSetVarOutStruct(KscNegotiator *neg,
                       KsSetVarResult *result)
    : avt(neg),
      svr(result)
    {}

    KscNegotiator *avt;
    KsSetVarResult *svr;
}; // KscSetVarOutStruct

//////////////////////////////////////////////////////////////////////
// Following are the two XDR toplevel encoding and decoding functions.
// They are responsible for sending and receiving the ACPLT/KS service
// request and reply for the SetVar service.
//
static bool_t 
KscSetVarInHelper(XDR *xdr, void *p)
{
    PLT_PRECONDITION((xdr->x_op == XDR_ENCODE) && p);

    KscSetVarInStruct *data = (KscSetVarInStruct *)p;
    
    PLT_ASSERT(data->svp && data->avt);
    
    return data->avt->xdrEncode(xdr) 
        && data->svp->xdrEncode(xdr);
} // KscSetVarInHelper

static bool_t
KscSetVarOutHelper(XDR *xdr, void *p)
{
    PLT_PRECONDITION((xdr->x_op == XDR_DECODE) && p);
    
    KscSetVarOutStruct *data = 
        (KscSetVarOutStruct *)p;
    
    PLT_ASSERT(data->avt && data->svr);

    return data->avt->xdrDecode(xdr)
        && data->svr->xdrDecode(xdr);
} // KscSetVarOutHelper

//////////////////////////////////////////////////////////////////////
// Okay. Now for the real setVar() function which issues the ACPLT/KS
// SetVar service request.
bool 
KscServer::setVar(const KscAvModule *avm,
                  const KsSetVarParams &params,
                  KsSetVarResult &result)
{
    enum clnt_stat errcode;

    //
    // Create client transport for the communication with the ACPLT/KS
    // server if that hasn't been done already. If there is no such
    // host or server then we will fail at this point and return to the
    // caller immediately. It makes no sense here to try reconnects yet.
    //
    if ( !_client_transport ) {
        if( !createTransport() ) {
            if ( !reconnectServer(_last_result) ) {
                return false;
            }
        }
    }

    //
    // Now locate the negotiator which will handle the A/V tickets for the
    // service request and reply. Then set up the ingoing and outcomming
    // service parameters, so the server object can lateron build the
    // data packets.
    //
    KscNegotiator *negotiator = getNegotiator(avm);

#if PLT_DEBUG
    cerr << "SetVar request to server "
         << host_name << "/" << server_info.server.name << endl;
    cerr << "Using AV-type : " << negotiator->typeCode() << endl;
#endif

    KscSetVarInStruct inData(negotiator, &params);
    KscSetVarOutStruct outData(negotiator, &result);

    //
    // Try to issue a SetVar service request until either the communication
    // succeeds or we've run out of tries.
    //
    size_t try_count = 0;
    do {
#if PLT_DEBUG
        cerr << "Trying for the " << (try_count+1) << "th time" << endl;
#endif
        if ( _client_transport ) {
            errcode = clnt_call(_client_transport, KS_SETVAR,
				(xdrproc_t) KscSetVarInHelper,
				(char *) &inData,
				(xdrproc_t) KscSetVarOutHelper,
				(char *) &outData,
				KSC_RPCCALL_TIMEOUT);
        } else {
	    //
	    // The reason why we're finding ourselves here at this branch is
	    // that trying to establish a new communication transport might
	    // have failed the last time.
	    //
            errcode = RPC_FAILED;
        }
    } while( (errcode != RPC_SUCCESS)
	     && reconnectServer(++try_count, errcode) );

    //
    // If the communication itself failed (*NOT* a failure on the ACPLT/KS
    // level but on the RPC level), then destroy the transport. This has two
    // important reasons: first, the caller can easily reissue the request
    // with another call. Second, if the connection has timed out, then the
    // server will soon see that the client won't receive any information
    // and discard a potential service reply very soon.
    //
    if ( errcode != RPC_SUCCESS ) {
        destroyTransport();
    }

    setResultAfterService(errcode);

#if PLT_DEBUG
    if( errcode == RPC_SUCCESS ) {
        cerr << "SetVar request successfull" << endl;
    } else {
        cerr << "SetVar request failed, error code : " 
             << (unsigned) errcode << endl;
    }
#endif

    //
    // Quick Hint(tm): let the caller know whether something bad
    // happened or not. It can get the details with getLastResult()
    // later...
    //
    return _last_result == KS_ERR_OK;
} // KscServer::setVar
  

//////////////////////////////////////////////////////////////////////
// Here are the exgData() and helper structs and classes comming. They
// implement the ExgData request which is used to exchange information
// with an ACPLT/KS server within a request context.
//
// The following two structures associate an A/V ticket supplied
// ("negotiator") and the service request or reply. Both structures
// are used to supply the negotiator and the request/reply structures
// to the XDR encode/decode functions.
//
struct KscExgDataInStruct
{
    KscExgDataInStruct(KscNegotiator *avTicket,
                       const KsExgDataParams *params)
    : avt(avTicket),
      edp(params)
    {}

    KscNegotiator *avt;
    const KsExgDataParams *edp;
}; // KscExgDataInStruct

struct KscExgDataOutStruct
{
    KscExgDataOutStruct(KscNegotiator *neg,
                        KsExgDataResult *result)
    : avt(neg),
      edr(result)
    {}

    KscNegotiator *avt;
    KsExgDataResult *edr;
}; // KscExgDataOutStruct

//////////////////////////////////////////////////////////////////////
// Following are the two XDR toplevel encoding and decoding functions.
// They are responsible for sending and receiving the ACPLT/KS service
// request and reply for the ExgData service.
//
static bool_t 
KscExgDataInHelper(XDR *xdr, void *p)
{
    PLT_PRECONDITION((xdr->x_op == XDR_ENCODE) && p);

    KscExgDataInStruct *data = (KscExgDataInStruct *)p;
    
    PLT_ASSERT(data->edp && data->avt);
    
    return data->avt->xdrEncode(xdr) 
        && data->edp->xdrEncode(xdr);
} // KscExgDataInHelper

static bool_t
KscExgDataOutHelper(XDR *xdr, void *p)
{
    PLT_PRECONDITION((xdr->x_op == XDR_DECODE) && p);
    
    KscExgDataOutStruct *data = 
        (KscExgDataOutStruct *)p;
    
    PLT_ASSERT(data->avt && data->edr);

    return data->avt->xdrDecode(xdr)
        && data->edr->xdrDecode(xdr);
} // KscExgDataOutHelper

//////////////////////////////////////////////////////////////////////
// Okay. Now for the real exgData() function which issues the ACPLT/KS
// ExgData service request.
bool 
KscServer::exgData(const KscAvModule *avm,
                   const KsExgDataParams &params,
                   KsExgDataResult &result)
{
    enum clnt_stat errcode;

    //
    // Create client transport for the communication with the ACPLT/KS
    // server if that hasn't been done already. If there is no such
    // host or server then we will fail at this point and return to the
    // caller immediately. It makes no sense here to try reconnects yet.
    //
    if ( !_client_transport ) {
        if( !createTransport() ) {
            if ( !reconnectServer(_last_result) ) {
                return false;
            }
        }
    }

    //
    // Now locate the negotiator which will handle the A/V tickets for the
    // service request and reply. Then set up the ingoing and outcomming
    // service parameters, so the server object can lateron build the
    // data packets.
    //
    KscNegotiator *negotiator = getNegotiator(avm);

#if PLT_DEBUG
    cerr << "ExgData request to server "
         << host_name << "/" << server_info.server.name << endl;
//    cerr << "Variables to set: " << endl;
//    for(size_t i = 0; i < params.set_vars.size(); i++) {
//        cerr << (params.set_vars)[i].path_and_name << endl;
//    }
    cerr << "Using AV-type : " << negotiator->typeCode() << endl;
#endif

    KscExgDataInStruct inData(negotiator, &params);
    KscExgDataOutStruct outData(negotiator, &result);

    //
    // Try to issue a exgData service request until either the communication
    // succeeds or we've run out of tries.
    //
    size_t try_count = 0;
    do {
#if PLT_DEBUG
        cerr << "Trying for the " << (try_count+1) << "th time" << endl;
#endif
        if ( _client_transport ) {
	    errcode = clnt_call(_client_transport, KS_EXGDATA,
				(xdrproc_t) KscExgDataInHelper,
				(char *) &inData,
				(xdrproc_t) KscExgDataOutHelper,
				(char *) &outData,
				KSC_RPCCALL_TIMEOUT);
        } else {
	    //
	    // The reason why we're finding ourselves here at this branch is
	    // that trying to establish a new communication transport might
	    // have failed the last time.
	    //
            errcode = RPC_FAILED;
        }
    } while ( (errcode != RPC_SUCCESS)
	      && reconnectServer(++try_count, errcode) );

    //
    // If the communication itself failed (*NOT* a failure on the ACPLT/KS
    // level but on the RPC level), then destroy the transport. This has two
    // important reasons: first, the caller can easily reissue the request
    // with another call. Second, if the connection has timed out, then the
    // server will soon see that the client won't receive any information
    // and discard a potential service reply very soon.
    //
    if ( errcode != RPC_SUCCESS ) {
        destroyTransport();
    }

    setResultAfterService(errcode);

#if PLT_DEBUG
    if(errcode == RPC_SUCCESS) {
        cerr << "ExgData request successfull" << endl;
    }
    else {
        cerr << "ExgData request failed, error code : " 
             << (unsigned)errcode << endl;
    }
#endif

    //
    // Quick Hint(tm): let the caller know whether something bad
    // happened or not. It can get the details with getLastResult()
    // later...
    //
    return _last_result == KS_ERR_OK;
} // KscServer::exgData


//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
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
        PltKeyCPtr<KscAvModule> tkey(avm);
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

        
//////////////////////////////////////////////////////////////////////
// EOF client.cpp
//////////////////////////////////////////////////////////////////////
