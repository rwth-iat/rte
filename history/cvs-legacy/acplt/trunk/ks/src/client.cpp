#/* -*-plt-c++-*- */
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

/* Author: Markus Juergens <markusj@plt.rwth-aachen.de> */

//////////////////////////////////////////////////////////////////////

#include <string.h>

#include <plt/log.h>

#include "ks/client.h"
#include "ks/ks.h"

#if PLT_SYSTEM_NT
#include <winsock.h>
#else
extern "C" {
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
};
#endif

#if PLT_DEBUG
#include <iostream.h>
#endif


//////////////////////////////////////////////////////////////////////
// RTTI Simulation
//////////////////////////////////////////////////////////////////////

PLT_IMPL_RTTI0(KscServerBase);
PLT_IMPL_RTTI1(KscServer,KscServerBase);

//////////////////////////////////////////////////////////////////////
// initialize static data
KscClient *KscClient::_the_client = 0;
KscClient::CleanUp KscClient::_clean_up;

//////////////////////////////////////////////////////////////////////
// helper function to split a combination of host and server names
//
void
_ksc_extractHostAndServer(KsString host_and_server,
                          KsString &host,
                          KsString &server)
{
    size_t pos = 2;

    while(host_and_server[pos] && host_and_server[pos++] != '/') ;

    if(host_and_server[pos]) {
        // ok
        //
        host = host_and_server.substr(2, pos - 3);
        server = host_and_server.substr(pos);
    } else {
        PLT_DMSG("Invalid server identification" << host_and_server << endl);
    }
}

//////////////////////////////////////////////////////////////////////
// class KscClient
//////////////////////////////////////////////////////////////////////

KscClient::KscClient()
: av_module(0),
  _rpc_timeout(KSC_RPCCALL_TIMEOUT),
  _retry_wait(0, 0),
  _retries(0)
{}

//////////////////////////////////////////////////////////////////////

KscClient::~KscClient()
{
    // TODO: neccessary ?
    //       servers should be destroyed anyway by their own
    //       since their cant be any commobjects left at this time
    PLT_DMSG("Servers left at KscClient dtor : " << server_table.size() << endl);

    // destroy related KscServer objects if any left
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
    KscClient *cl = new KscClient();
    if(!cl) {
        PltLog::Alert("Cannot create client, going to abort program");
        exit(-1);
    }
    bool ok = setClient(cl, true);
    PLT_ASSERT(ok);
}

//////////////////////////////////////////////////////////////////////

bool 
KscClient::setClient(KscClient *cl, bool shutdownDelete)
{
    if(_the_client || !cl) {
        return false;
    } else {
        _the_client = cl;
        _clean_up.shutdown_delete = shutdownDelete;
        return true;
    }
}

//////////////////////////////////////////////////////////////////////

KscServerBase *
KscClient::getServer(const KsString &host_and_name) 
{
    KscServerBase *pServer;

    bool ok = server_table.query(host_and_name, pServer);

    return ok ? pServer : 0;
}

//////////////////////////////////////////////////////////////////////

KscServerBase *
KscClient::createServer(KsString host_and_name) 
{
    KscServerBase *pServer = 0;

    bool ok = server_table.query(host_and_name, pServer);

    if(!ok) {
        // create new server
        //
        KsString host, server;
        _ksc_extractHostAndServer(host_and_name, host, server);
        KscServer *temp = new KscServer(host, 
                                        KsServerDesc(server,
                                                     KS_PROTOCOL_VERSION));
        if(temp) {
            // object successfully created
            //
            temp->setTimeouts(_rpc_timeout, _retry_wait, _retries);
            pServer = temp;
            server_table.add(host_and_name, pServer);
        }
    }

    PLT_ASSERT(pServer);

    return pServer;
}

//////////////////////////////////////////////////////////////////////

void
KscClient::deleteServer(KscServerBase *server)
{
    KscServerBase *temp = 0;

    // remove server from table
    //
    bool ok = 
        server_table.remove(server->getHostAndName(), temp);

    PLT_ASSERT(ok && (temp == server));

    delete server;
}

//////////////////////////////////////////////////////////////////////

void 
KscClient::setTimeouts(const PltTime &rpc_timeout,        
                       const PltTime &retry_wait,         
                       size_t retries)
{
    _rpc_timeout = rpc_timeout;
    _retry_wait = retry_wait;
    _retries = retries;
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
    _ksc_extractHostAndServer(host_and_name, 
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
  errcode(RPC_SUCCESS),
  status(KscNotInitiated),
  pClient(0),
  _rpc_timeout(KSC_RPCCALL_TIMEOUT),
  _retry_wait(0, 0),
  _retries(0)
{
}

//////////////////////////////////////////////////////////////////////

KscServer::KscServer(KsString hostAndName, u_short protocolVersion)
: KscServerBase(hostAndName),
  server_desc(server_name, protocolVersion),
  errcode(RPC_SUCCESS),
  status(KscNotInitiated),
  pClient(0),
  _rpc_timeout(KSC_RPCCALL_TIMEOUT),
  _retry_wait(0, 0),
  _retries(0)
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
                       size_t retries)
{
    _rpc_timeout = rpc_timeout;
    _retry_wait = retry_wait;
    _retries = retries;

    if(pClient) {
        bool ok = clnt_control(pClient,
                               CLSET_TIMEOUT,
                               (struct timeval *)(&_rpc_timeout));
        if(!ok) {
            PltLog::Warning("Failed to specifify timeout value for RPC");
        }
    }
}

//////////////////////////////////////////////////////////////////////
// get IP of host
//
bool
KscServer::getHostAddr(struct sockaddr_in *addr)
{
    const unsigned long INVALID_IP = INADDR_NONE;

    // clean up first
    //
    memset(addr, 0, sizeof(struct sockaddr_in));

    // FIXME: (HPUX)
    //        inet_addr() returns 0 if host_name is an empty string
    //
    unsigned long ip = inet_addr(host_name);

    if( ip == INVALID_IP ) {
        // host_name is no IP, try to resolve name
        //
        struct hostent *hp = gethostbyname(host_name);
        if(hp) {
            // copy address
            memcpy(&(addr->sin_addr.s_addr),
                   hp->h_addr, 
                   hp->h_length);
            return true;
        } else {
            PltString err_msg("Failed to get IP of host : ", host_name);
            PltLog::Warning(err_msg);
            return false;
        }
    } else {
        // hostname is an IP, just copy it
        //
        addr->sin_addr.s_addr = ip;
        return true;
    }
}

//////////////////////////////////////////////////////////////////////
// try to establish a connection to the server
//
bool
KscServer::createTransport()
{
    // destroy old link if exists
    //
    if(pClient) {
        destroyTransport();
    }

    struct sockaddr_in host_addr; 
    int socket = RPC_ANYSOCK;
    
    // locate host
    //
    if( !getHostAddr(&host_addr) ) {
        status = KscHostNotFound;
        _last_result = KS_ERR_HOSTUNKNOWN;
        return false;
    }

    // request server description from manager
    //
    if( !getServerDesc(&host_addr, server_desc, server_info) ) {
        // dont change status set by getServerDescription()
        return false;
    }

    if( server_info.result != KS_ERR_OK ) {
        status = KscNoServer;
        PLT_DMSG("Unknown server " << getHostAndName() << endl);
        return false;
    }

#if PLT_DEBUG_PEDANTIC
    cerr << "Trying to connect server at port "
         << server_info.port
         << endl;
#endif

    // create client handle
    //
    host_addr.sin_family = AF_INET;
    host_addr.sin_port = htons(server_info.port);
    
    pClient = clnttcp_create(&host_addr,
                             KS_RPC_PROGRAM_NUMBER,
                             server_desc.protocol_version,
                             &socket,
                             0, 0 );

    if( !pClient ) {
        status = KscCannotCreateClientHandle;
        _last_result = KS_ERR_CANTCONTACT;
        return false;
    }

    // set timeout 
    //
    bool ok = clnt_control(pClient, CLSET_TIMEOUT, 
                           (struct timeval *)(&_rpc_timeout));
    if(!ok) {
        PltLog::Warning("Failed to specify timeout value for RPC");
    }

    return true;
}

//////////////////////////////////////////////////////////////////////

void 
KscServer::destroyTransport()
{
    if(pClient) {
        clnt_destroy(pClient);
        pClient = 0;
    }
}

//////////////////////////////////////////////////////////////////////

bool
KscServer::getStateUpdate()
{
    struct sockaddr_in host_addr;
    
    // locate host
    //
    if( !getHostAddr(&host_addr) ) {
        status = KscHostNotFound;
        _last_result = KS_ERR_HOSTUNKNOWN;
        return false;
    }

    // request server description from manager
    //
    if( !getServerDesc(&host_addr, server_desc, server_info) ) {
        // dont change status set by getServerDescription()
        return false;
    }
    if( server_info.result != KS_ERR_OK ) {
        status = KscNoServer;
        return false;
    }
    _last_result = server_info.result;
    return true;
}

//////////////////////////////////////////////////////////////////////

bool
KscServer::reconnectServer(size_t try_count, enum_t errcode)
{
    // maximum number of tries reached ?
    //
    if(try_count > _retries) {
        return false;
    }

    //
    // TODO: make decision whether to try a reconnect configurable
    //
    if(errcode == RPC_CANTDECODERES ||
       errcode == RPC_CANTSEND ||
       errcode == RPC_CANTRECV ||
       errcode == RPC_TIMEDOUT ||
       errcode == RPC_FAILED )
    {
        wait(_retry_wait);
        createTransport();  // ignore possible error
        return true;
    } else {
        return false;
    }
}

//////////////////////////////////////////////////////////////////////

bool
KscServer::wait(PltTime howLong)
{
    if(howLong.tv_sec == 0 && howLong.tv_usec == 0) {
        return true;
    } else {
        PltTime now(PltTime::now());
        PltTime until(now);
        until += howLong;

        PLT_DMSG("Going to wait.." << howLong.tv_sec << endl);
        
        do {
#if PLT_SYSTEM_NT
            PLT_DMSG("Calling Sleep()" << endl);
            // Sleep expects time to wait as milliseconds
            //
            Sleep(howLong.tv_sec*1000 + howLong.tv_usec / 1000);
#else
            PLT_DMSG("Calling select" << endl);
            select(0, 0, 0, 0,
                   &howLong);
#endif
            now = PltTime::now();
            howLong = until - now;
        }  while(until > now);

        PLT_DMSG("Finished waiting" << endl);

        return true;
    }
}

//////////////////////////////////////////////////////////////////////
//
void
KscServer::setResultAfterService()
{
    switch(status) {
    case KscHostNotFound : 
        _last_result = KS_ERR_HOSTUNKNOWN;
        return;
    case KscCannotCreateClientHandle :
        _last_result = KS_ERR_CANTCONTACT;
        return;
    case KscCannotCreateUDPClient :
        if(errcode == RPC_PROGNOTREGISTERED ||
           errcode == RPC_PROGUNAVAIL) {
            _last_result = KS_ERR_NOMANAGER;
        } else {
            _last_result = KS_ERR_CANTCONTACT;
        }
        return;
    case KscNoServer :
        _last_result = KS_ERR_SERVERUNKNOWN;
        return;
    default:
        ;     // behavior depends on errcode, see next switch
    }

    switch(errcode) {
    case RPC_TIMEDOUT:
        _last_result = KS_ERR_TIMEOUT;
        return;
    case RPC_SUCCESS :
        _last_result = KS_ERR_OK;
        return;
    default:
        _last_result = KS_ERR_GENERIC;
    }
}        

//////////////////////////////////////////////////////////////////////
// getServerDescription and helper structs and classes
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
};

struct KscGetServerOutStruct
{
    KscGetServerOutStruct(KscNegotiator *neg,
                          KsGetServerResult *result)
    : avt(neg),
      gsr(result)
    {}

    KscNegotiator *avt; 
    KsGetServerResult *gsr;
};

bool_t 
KscGetServerInHelper(XDR *xdr, void *p)
{
    PLT_PRECONDITION((xdr->x_op == XDR_ENCODE) && p);

    KscGetServerInStruct *data = (KscGetServerInStruct *)p;
    
    PLT_ASSERT(data->gsp && data->avt);
    
    return data->avt->xdrEncode(xdr) 
        && data->gsp->xdrEncode(xdr);
}

bool_t
KscGetServerOutHelper(XDR *xdr, void *p)
{
    PLT_PRECONDITION((xdr->x_op == XDR_DECODE) && p);
    
    KscGetServerOutStruct *data = 
        (KscGetServerOutStruct *)p;
    
    PLT_ASSERT(data->avt && data->gsr);

    return data->avt->xdrDecode(xdr)
        && data->gsr->xdrDecode(xdr);
}

bool
KscServer::getServerDesc(struct sockaddr_in *host_addr, 
                         const KsServerDesc &server,
                         KsGetServerResult &server_desc)
{
    int socket = RPC_ANYSOCK;
    CLIENT *pUDP;

    // establish UDP connection to manager
    //
    host_addr->sin_family = AF_INET;
    host_addr->sin_port = htons(0);
    
    pUDP = clntudp_create(host_addr,
                          KS_RPC_PROGRAM_NUMBER,
                          KS_PROTOCOL_VERSION,
                          KSC_UDP_TIMEOUT,
                          &socket);
    if( !pUDP ) {
        status = KscCannotCreateUDPClient;
        _last_result = KS_ERR_CANTCONTACT;
        return false;
    }

#if PLT_DEBUG_PEDANTIC
    // ping manager
    //
    errcode = clnt_call(pUDP, 0, 
                        xdr_void, 0, 
                        xdr_void, 0, 
                        KSC_UDP_TIMEOUT);

    if(errcode != RPC_SUCCESS) {
        cerr << "ping failed, error code " << (unsigned) errcode << endl;
    }
    else {
        cerr << "manager pinged" << endl;
    }
#endif

    // ask manager for server description
    //
    KsGetServerParams params(server);
    KscGetServerInStruct inData(
        KscAvNoneModule::getStaticNegotiator(), 
        &params);
    KscGetServerOutStruct outData(
        KscAvNoneModule::getStaticNegotiator(),
        &server_desc);

    errcode = clnt_call(pUDP, 
                        KS_GETSERVER,
                        (xdrproc_t) KscGetServerInHelper,
                        (char *) &inData,
                        (xdrproc_t) KscGetServerOutHelper,
                        (char *) &outData,
                        KSC_UDP_TIMEOUT);

    // free resources used by client handle
    clnt_destroy( pUDP );

    if( errcode != RPC_SUCCESS ) {
        status = KscRPCCallFailed;
        _last_result = KS_ERR_CANTCONTACT;

        PLT_DMSG("function call to MANAGER failed, error code " << (unsigned) errcode << endl);

        return false;
    }

    return true;
}

//////////////////////////////////////////////////////////////////////
// helper structs and functions for getPP()
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
};

struct KscGetPPOutStruct
{
    KscGetPPOutStruct(KscNegotiator *neg,
                      KsGetPPResult *result)
    : avt(neg),
      gpr(result)
    {}

    KscNegotiator *avt;
    KsGetPPResult *gpr;
};

bool_t 
KscGetPPInHelper(XDR *xdr, void *p)
{
    PLT_PRECONDITION((xdr->x_op == XDR_ENCODE) && p);

    KscGetPPInStruct *data = (KscGetPPInStruct *)p;
    
    PLT_ASSERT(data->gpp && data->avt);
    
    return data->avt->xdrEncode(xdr) 
        && data->gpp->xdrEncode(xdr);
}

bool_t
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
}


bool 
KscServer::getPP(const KscAvModule *avm,
                 const KsGetPPParams &params,
                 KsGetPPResult &result)
{
    // create client handle if neccessary
    //
    if(!pClient) {
        if( !createTransport() ) {
            // dont try to reconnect if host
            // cannot be located or server is not registered
            //
            if(status == KscHostNotFound 
               || status == KscNoServer) 
            {
                return false;
            }
        }
    }

#if PLT_DEBUG
    cerr << "Requesting server " 
         << host_name << "/" << server_info.server.name 
         << " for getPP service." << endl;
    cerr << "Parameters : " << endl; 
    cerr << "\tPath : " << params.path << endl;
    cerr << "\tType mask : " << params.type_mask << endl;
    cerr << "\tName mask : " << params.name_mask << endl;
    cerr << endl;
#endif

    // get negotiator
    //
    KscNegotiator *negotiator = getNegotiator(avm);

    KscGetPPInStruct inData(negotiator, &params);
    KscGetPPOutStruct outData(negotiator, &result);

    size_t try_count = 0;
    
    do {
#if PLT_DEBUG
        cerr << "Trying for the " << (try_count+1) << ". time" << endl;
#endif
        if(pClient) {
            errcode = clnt_call(pClient, KS_GETPP,
                                (xdrproc_t) KscGetPPInHelper,
                                (char *) &inData,
                                (xdrproc_t) KscGetPPOutHelper,
                                (char *) &outData,
                                KSC_RPCCALL_TIMEOUT);
        } else {
            errcode = RPC_FAILED;
        }
    } while(errcode != RPC_SUCCESS 
            && reconnectServer(++try_count, errcode));
 
    setResultAfterService();
        
#if PLT_DEBUG
    if(errcode == RPC_SUCCESS) {
        cerr << "request successfull" << endl;
    }
    else {
        cerr << "request failed" << endl;
    }
#endif

    return errcode == RPC_SUCCESS;
}


//////////////////////////////////////////////////////////////////////
// helper structs and functions for getVar()
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
};

struct KscGetVarOutStruct
{
    KscGetVarOutStruct(KscNegotiator *neg,
                       KsGetVarResult *result)
    : avt(neg),
      gvr(result)
    {}

    KscNegotiator *avt;
    KsGetVarResult *gvr;
};

bool_t 
KscGetVarInHelper(XDR *xdr, void *p)
{
    PLT_PRECONDITION((xdr->x_op == XDR_ENCODE) && p);

    KscGetVarInStruct *data = (KscGetVarInStruct *)p;
    
    PLT_ASSERT(data->gvp && data->avt);
    
    return data->avt->xdrEncode(xdr) 
        && data->gvp->xdrEncode(xdr);
}

bool_t
KscGetVarOutHelper(XDR *xdr, void *p)
{
    PLT_PRECONDITION((xdr->x_op == XDR_DECODE) && p);
    
    KscGetVarOutStruct *data = 
        (KscGetVarOutStruct *)p;
    
    PLT_ASSERT(data->gvr && data->avt);

    return data->avt->xdrDecode(xdr)
        && data->gvr->xdrDecode(xdr);
}


bool 
KscServer::getVar(const KscAvModule *avm,
                  const KsGetVarParams &params,
                  KsGetVarResult &result)
{
    // create client handle if neccessary
    //
    if(!pClient) {
        if( !createTransport() ) {
            if(status == KscHostNotFound 
               || status == KscNoServer) 
            {
                setResultAfterService();
                return false;
            }
        }
    }

#if PLT_DEBUG
    cerr << "GetVar request to server "
         << host_name << "/" << server_info.server.name << endl;
#endif

    KscNegotiator *negotiator = getNegotiator(avm);

    KscGetVarInStruct inData(negotiator, &params);
    KscGetVarOutStruct outData(negotiator, &result);
    size_t try_count = 0;
    
    do {
#if PLT_DEBUG
        cerr << "Trying for the " << (try_count+1) << ". time" << endl;
#endif
        if(pClient) {
            errcode = clnt_call(pClient, KS_GETVAR,
                                (xdrproc_t) KscGetVarInHelper,
                                (char *) &inData,
                                (xdrproc_t) KscGetVarOutHelper,
                                (char *) &outData,
                                KSC_RPCCALL_TIMEOUT);
        } else {
            errcode = RPC_FAILED;
        }
    } while(errcode != RPC_SUCCESS
            && reconnectServer(++try_count, errcode));

    setResultAfterService();

#if PLT_DEBUG
    if(errcode == RPC_SUCCESS) {
        cerr << "GetVar request successfull" << endl;
    }
    else {
        cerr << "GetVar request failed, error code : "
             << (unsigned)errcode << endl;
    }
#endif

    return errcode == RPC_SUCCESS;
}
              

//////////////////////////////////////////////////////////////////////
// helper structs and functions for setVar()
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
};

struct KscSetVarOutStruct
{
    KscSetVarOutStruct(KscNegotiator *neg,
                       KsSetVarResult *result)
    : avt(neg),
      svr(result)
    {}

    KscNegotiator *avt;
    KsSetVarResult *svr;
};

bool_t 
KscSetVarInHelper(XDR *xdr, void *p)
{
    PLT_PRECONDITION((xdr->x_op == XDR_ENCODE) && p);

    KscSetVarInStruct *data = (KscSetVarInStruct *)p;
    
    PLT_ASSERT(data->svp && data->avt);
    
    return data->avt->xdrEncode(xdr) 
        && data->svp->xdrEncode(xdr);
}

bool_t
KscSetVarOutHelper(XDR *xdr, void *p)
{
    PLT_PRECONDITION((xdr->x_op == XDR_DECODE) && p);
    
    KscSetVarOutStruct *data = 
        (KscSetVarOutStruct *)p;
    
    PLT_ASSERT(data->avt && data->svr);

    return data->avt->xdrDecode(xdr)
        && data->svr->xdrDecode(xdr);
}


bool 
KscServer::setVar(const KscAvModule *avm,
                  const KsSetVarParams &params,
                  KsSetVarResult &result)
{
    // create client handle if neccessary
    //
    if(!pClient) {
        if( !createTransport() ) {
            if(status == KscHostNotFound 
               || status == KscNoServer) {
                setResultAfterService();
                return false;
            }
        }
    }


#if PLT_DEBUG
    cerr << "SetVar request to server "
         << host_name << "/" << server_info.server.name << endl;
#endif

    // get negotiator
    //
    KscNegotiator *negotiator = getNegotiator(avm);

    KscSetVarInStruct inData(negotiator, &params);
    KscSetVarOutStruct outData(negotiator, &result);

    size_t try_count = 0;

    do {
#if PLT_DEBUG
        cerr << "Trying for the " << (try_count+1) << ". time" << endl;
#endif
        if(pClient) {
            errcode = clnt_call(pClient, KS_SETVAR,
                                (xdrproc_t) KscSetVarInHelper,
                                (char *) &inData,
                                (xdrproc_t) KscSetVarOutHelper,
                                (char *) &outData,
                                KSC_RPCCALL_TIMEOUT);
        } else {
            errcode = RPC_FAILED;
        }
    } while(errcode != RPC_SUCCESS
            && reconnectServer(++try_count, errcode));

    setResultAfterService();

#if PLT_DEBUG
    if( errcode == RPC_SUCCESS ) {
        cerr << "SetVar request successfull" << endl;
    } else {
        cerr << "SetVar request failed, error code : " 
             << (unsigned)errcode << endl;
    }
#endif

    return errcode == RPC_SUCCESS;
}
  

//////////////////////////////////////////////////////////////////////
// helper structs and functions for exgData()
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
};

struct KscExgDataOutStruct
{
    KscExgDataOutStruct(KscNegotiator *neg,
                        KsExgDataResult *result)
    : avt(neg),
      edr(result)
    {}

    KscNegotiator *avt;
    KsExgDataResult *edr;
};

bool_t 
KscExgDataInHelper(XDR *xdr, void *p)
{
    PLT_PRECONDITION((xdr->x_op == XDR_ENCODE) && p);

    KscExgDataInStruct *data = (KscExgDataInStruct *)p;
    
    PLT_ASSERT(data->edp && data->avt);
    
    return data->avt->xdrEncode(xdr) 
        && data->edp->xdrEncode(xdr);
}

bool_t
KscExgDataOutHelper(XDR *xdr, void *p)
{
    PLT_PRECONDITION((xdr->x_op == XDR_DECODE) && p);
    
    KscExgDataOutStruct *data = 
        (KscExgDataOutStruct *)p;
    
    PLT_ASSERT(data->avt && data->edr);

    return data->avt->xdrDecode(xdr)
        && data->edr->xdrDecode(xdr);
}


bool 
KscServer::exgData(const KscAvModule *avm,
                   const KsExgDataParams &params,
                   KsExgDataResult &result)
{
    // create client handle if neccessary
    //
    if(!pClient) {
        if( !createTransport() ) {
            if(status == KscHostNotFound 
               || status == KscNoServer) {
                setResultAfterService();
                return false;
            }
        }
    }

#if PLT_DEBUG
    cerr << "ExgData request to server "
         << host_name << "/" << server_info.server.name << endl;
#endif

    // get negotiator
    //
    KscNegotiator *negotiator = getNegotiator(avm);

    KscExgDataInStruct inData(negotiator, &params);
    KscExgDataOutStruct outData(negotiator, &result);

    size_t try_count = 0;

    do {
#if PLT_DEBUG
        cerr << "Trying for the " << (try_count+1) << ". time" << endl;
#endif
        if(pClient) {
            errcode = clnt_call(pClient, KS_EXGDATA,
                                (xdrproc_t) KscExgDataInHelper,
                                (char *) &inData,
                                (xdrproc_t) KscExgDataOutHelper,
                                (char *) &outData,
                                KSC_RPCCALL_TIMEOUT);
        } else {
            errcode = RPC_FAILED;
        }
    } while(errcode != RPC_SUCCESS
            && reconnectServer(++try_count, errcode));

    setResultAfterService();

#if PLT_DEBUG
    if(errcode == RPC_SUCCESS) {
        cerr << "ExgData request successfull" << endl;
    }
    else {
        cerr << "ExgData request failed, error code : " 
             << (unsigned)errcode << endl;
    }
#endif

    return errcode == RPC_SUCCESS;
}            


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

// CAUTION :
//   pointers returned by this function are for temporary use only
//
KscNegotiator *
KscServer::getNegotiator(const KscAvModule *avm)
{
    if(!avm) {
        if(av_module) {
            avm = av_module;
        } else {
            avm = KscClient::getClient()->getAvModule();
        }
    }

    if(avm) {
        // try to find it at the cache
        //
        PltKeyCPtr<KscAvModule> tkey(avm);
        KscNegotiatorHandle hneg;
        if(neg_table.query(tkey, hneg)) {
            // found
            //
            if(hneg) return hneg.getPtr();
        } else {
            // create one
            //
            hneg = avm->getNegotiator(this);
            if(hneg) {
                if(neg_table.add(tkey, hneg)) {
                    return hneg.getPtr();
                }
            }
        }
    }

    // we failed to create a negotiator
    //
    return KscAvNoneModule::getStaticNegotiator(); 
}        
        
//////////////////////////////////////////////////////////////////////
// EOF client.cpp
//////////////////////////////////////////////////////////////////////

