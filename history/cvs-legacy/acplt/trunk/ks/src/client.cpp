/* -*-plt-c++-*- */

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

#include "ks/client.h"
#include "ks/ks.h"

#include <string.h>

extern "C" {
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
};

#if PLT_DEBUG
#include <iostream.h>
#endif

//////////////////////////////////////////////////////////////////////
// initialize static data
KscClient *KscClient::_the_client = 0;
KscClient::CleanUp KscClient::_clean_up;
KscNegotiator *KscClient::none_negotiator = 0; 


//////////////////////////////////////////////////////////////////////
// class KscClient
//////////////////////////////////////////////////////////////////////

KscClient::KscClient()
: av_module(0)
{
    none_negotiator = KscAvNoneModule::getStaticNegotiator();
}

//////////////////////////////////////////////////////////////////////

KscClient::~KscClient()
{
    // TODO: neccessary ?
    //       servers should be destroyed anyway by their own
    //       since their cant be any commobjects left at this time
    PLT_DMSG("Servers left at KscClient dtor : " << server_table.size() << endl);

    // destroy related KscServer objects if any left
    //
    PltHashIterator<KscAbsPath,KscServer *> it(server_table);
    KscServer *pcurr;
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
KscClient::createClient()
{
    KscClient *cl = new KscClient();
    // TODO: abort programm in none debugging mode if cl == 0 
    PLT_ASSERT(cl);
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

KscServer *
KscClient::getServer(const KscAbsPath &host_and_name) 
{
    PLT_PRECONDITION(host_and_name.isValid());

    KscServer *pServer;

    bool ok = server_table.query(host_and_name, pServer);

    return ok ? pServer : 0;
}

//////////////////////////////////////////////////////////////////////

KscServer *
KscClient::createServer(const KscAbsPath &host_and_name) 
{
    PLT_PRECONDITION(host_and_name.isValid());

    KscServer *pServer = 0;

    bool ok = server_table.query(host_and_name, pServer);

    if(!ok) {
        // create new server
        //
        pServer = new KscServer(host_and_name.getHost(), 
                                KsServerDesc(host_and_name.getServer(),
                                             KS_PROTOCOL_VERSION));
        if(pServer) {
            // object successfully created
            //
            server_table.add(host_and_name, pServer);
        }
    }

    PLT_ASSERT(pServer);

    return pServer;
}

//////////////////////////////////////////////////////////////////////

void
KscClient::deleteServer(KscServer *server)
{
    KscServer *temp = 0;

    // remove server from table
    //
    bool ok = 
        server_table.remove(server->getHostAndName(), temp);

    PLT_ASSERT(ok && (temp == server));

    delete server;
}

//////////////////////////////////////////////////////////////////////

KscNegotiator *
KscClient::getNegotiator(KscServer *forServer)
{
    KscNegotiator *neg;

    if(av_module) {
        neg = av_module->getNegotiator(forServer);
        if(neg) {
            return neg;
        }
    }

    // there is no special negotiator,
    // return standard AvNoneNegotiator
    return none_negotiator;
}

        
//////////////////////////////////////////////////////////////////////
#if PLT_DEBUG

void
KscClient::printServers()
{
    PltHashIterator<KscAbsPath,KscServer *> it(server_table);

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
// class KscServer
//
// Constructor
// Just copies the given data for later use.
//
KscServer::KscServer(KsString host, 
                     const KsServerDesc &server)
: host_name(host),
  server_desc(server),
  errcode(RPC_SUCCESS),
  status(KscNotInitiated),
  pClient(0),
  ref_count(0),
  av_module(0)
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

    struct hostent *hp;
    struct sockaddr_in server_addr; 
    int socket = RPC_ANYSOCK;
    
    // locate host
    //
    hp = gethostbyname(host_name);
    if( !hp ) {
        status = KscHostNotFound;
        return false;
    }

    // request server description from manager
    //
    if( !getServerDesc(hp, server_desc, server_info) ) {
        // dont change status set by getServerDescription()
        return false;
    }
    if( server_info.result != KS_ERR_OK ) {
        status = KscNoServer;
        return false;
    }

#if PLT_DEBUG
    cout << "Trying to connect server at port "
         << server_info.port
         << endl;
#endif

    // create client handle
    //
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_info.port);
    memcpy(&server_addr.sin_addr.s_addr,
           hp->h_addr, 
           hp->h_length);

    pClient = clnttcp_create(&server_addr,
                             KS_RPC_PROGRAM_NUMBER,
                             server_desc.protocol_version,
                             &socket,
                             0, 0 );

    if( !pClient ) {
        status = KscCannotCreateClientHandle;
#if PLT_DEBUG
        clnt_pcreateerror("ERROR :");
#endif
        return false;
    }

    return true;
}

//////////////////////////////////////////////////////////////////////

void 
KscServer::destroyTransport()
{
    if(pClient) {
        clnt_destroy(pClient);
    }
}

//////////////////////////////////////////////////////////////////////

bool
KscServer::getStateUpdate()
{
    struct hostent *hp;
    
    // locate host
    //
    hp = gethostbyname(host_name);
    if( !hp ) {
        status = KscHostNotFound;
        return false;
    }

    // request server description from manager
    //
    if( !getServerDesc(hp, server_desc, server_info) ) {
        // dont change status set by getServerDescription()
        return false;
    }
    if( server_info.result != KS_ERR_OK ) {
        status = KscNoServer;
        return false;
    }

    return true;
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
KscServer::getServerDesc(struct hostent *hp, 
                         const KsServerDesc &server,
                         KsGetServerResult &server_desc)
{
    struct sockaddr_in addr_manager;
    int socket = RPC_ANYSOCK;
    CLIENT *pUDP;

    // establish UDP connection to manager
    //
    memset(&addr_manager, 0, sizeof(addr_manager));
    addr_manager.sin_family = AF_INET;
    addr_manager.sin_port = htons(0);
    memcpy(&addr_manager.sin_addr.s_addr, hp->h_addr, hp->h_length);
    
    pUDP = clntudp_create(&addr_manager,
                          KS_RPC_PROGRAM_NUMBER,
                          KS_PROTOCOL_VERSION,
                          KSC_UDP_TIMEOUT,
                          &socket);
    if( !pUDP ) {
        status = KscCannotCreateUDPClient;
#if PLT_DEBUG
        clnt_pcreateerror("ERROR :");
#endif
        return false;
    }
#if PLT_DEBUG
    // ping manager
    //
    errcode = clnt_call(pUDP, 0, 
                        xdr_void, 0, 
                        xdr_void, 0, 
                        KSC_UDP_TIMEOUT);
    if(errcode != RPC_SUCCESS) {
        cout << "ping failed, error code " << errcode << endl;
    }
    else {
        cout << "manager pinged" << endl;
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
                        KscGetServerInHelper,
                        &inData,
                        KscGetServerOutHelper,
                        &outData,
                        KSC_UDP_TIMEOUT);

    // free resources used by client handle
    clnt_destroy( pUDP );

    if( errcode != RPC_SUCCESS ) {
        status = KscRPCCallFailed;
#if PLT_DEBUG
        cout << "function call failed, error code " << errcode << endl;
#endif
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
KscServer::getPP(KscNegotiator *negotiator,
                 const KsGetPPParams &params,
                 KsGetPPResult &result)
{
    // create client handle if neccessary
    //
    if(!pClient) {
        if( !createTransport() ) {
            return false;
        }
    }

    PLT_ASSERT(pClient);

#if PLT_DEBUG
    cout << "Requesting server " 
         << host_name << "/" << server_info.server.name 
         << " for getPP service." << endl;
    cout << "Parameters : " << endl; 
    cout << "\tPath : " << params.path << endl;
    cout << "\tType mask : " << params.type_mask << endl;
    cout << "\tName mask : " << params.name_mask << endl;
    cout << endl;
#endif

    KscGetPPInStruct inData(negotiator, &params);
    KscGetPPOutStruct outData(negotiator, &result);

    errcode = clnt_call(pClient, KS_GETPP,
                        KscGetPPInHelper,
                        &inData,
                        KscGetPPOutHelper,
                        &outData,
                        KSC_RPCCALL_TIMEOUT);

#if PLT_DEBUG
    if(errcode == RPC_SUCCESS) {
        cout << "request successfull" << endl;
    }
    else {
        cout << "request failed" << endl;
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
KscServer::getVar(KscNegotiator *negotiator,
                  const KsGetVarParams &params,
                  KsGetVarResult &result)
{
    // create client handle if neccessary
    //
    if(!pClient) {
        if( !createTransport() ) {
            return false;
        }
    }

    PLT_ASSERT(pClient);

#if PLT_DEBUG
    cout << "GetVar request to server "
         << host_name << "/" << server_info.server.name << endl;
#endif

    KscGetVarInStruct inData(negotiator, &params);
    KscGetVarOutStruct outData(negotiator, &result);

    errcode = clnt_call(pClient, KS_GETVAR,
                        KscGetVarInHelper,
                        &inData,
                        KscGetVarOutHelper,
                        &outData,
                        KSC_RPCCALL_TIMEOUT);

#if PLT_DEBUG
    if(errcode == RPC_SUCCESS) {
        cout << "GetVar request successfull" << endl;
    }
    else {
        cout << "GetVar request failed" << endl;
        cout << clnt_sperror(pClient, "");
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
KscServer::setVar(KscNegotiator *negotiator,
                  const KsSetVarParams &params,
                  KsSetVarResult &result)
{
    // create client handle if neccessary
    //
    if(!pClient) {
        if( !createTransport() ) {
            return false;
        }
    }

    PLT_ASSERT(pClient);

#if PLT_DEBUG
    cout << "SetVar request to server "
         << host_name << "/" << server_info.server.name << endl;
#endif

    KscSetVarInStruct inData(negotiator, &params);
    KscSetVarOutStruct outData(negotiator, &result);

    errcode = clnt_call(pClient, KS_SETVAR,
                        KscSetVarInHelper,
                        &inData,
                        KscSetVarOutHelper,
                        &outData,
                        KSC_RPCCALL_TIMEOUT);

#if PLT_DEBUG
    if( errcode == RPC_SUCCESS ) {
        cout << "SetVar request successfull" << endl;
    } else {
        cout << "SetVar request failed" << endl;
        cout << clnt_sperror(pClient, "");
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
KscServer::exgData(KscNegotiator *negotiator,
                   const KsExgDataParams &params,
                   KsExgDataResult &result)
{
    // create client handle if neccessary
    //
    if(!pClient) {
        if( !createTransport() ) {
            return false;
        }
    }

    PLT_ASSERT(pClient);

#if PLT_DEBUG
    cout << "ExgData request to server "
         << host_name << "/" << server_info.server.name << endl;
#endif

    KscExgDataInStruct inData(negotiator, &params);
    KscExgDataOutStruct outData(negotiator, &result);

    errcode = clnt_call(pClient, KS_EXGDATA,
                        KscExgDataInHelper,
                        &inData,
                        KscExgDataOutHelper,
                        &outData,
                        KSC_RPCCALL_TIMEOUT);

    return errcode == RPC_SUCCESS;
}            


//////////////////////////////////////////////////////////////////////
// Section for handling of communication objects
//

void
KscServer::incRefcount() 
{
    ref_count++;
}

//////////////////////////////////////////////////////////////////////

void
KscServer::decRefcount()
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
//////////////////////////////////////////////////////////////////////

KscNegotiator *
KscServer::getNegotiator()
{
    KscNegotiator *neg;

    if(av_module) {
        neg = av_module->getNegotiator(this);
        if(neg) {
            return neg;
        }
    }

    neg = KscClient::getClient()->getNegotiator(this);

    PLT_ASSERT(neg);

    return neg;
}        
        
//////////////////////////////////////////////////////////////////////
// EOF client.cpp
//////////////////////////////////////////////////////////////////////
                 

    
    
    







