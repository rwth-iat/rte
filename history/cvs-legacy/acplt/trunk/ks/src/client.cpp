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

#include <ks/client.h>
#include <ks/ks.h>

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
// Currently KscNegotiator / KscAvModule not implemented.
// Instead we are using this simple KsAvNoneTicket.
//
KsAvNoneTicket ksc_the_ticket(KS_ERR_OK, KS_AC_READ | KS_AC_WRITE);

//////////////////////////////////////////////////////////////////////
// initialize static data
KscClient *KscClient::the_client = 0;

//////////////////////////////////////////////////////////////////////
// class KscClient
//////////////////////////////////////////////////////////////////////

KscClient::KscClient() 
{
    PLT_PRECONDITION(the_client == 0);
    the_client = this;
}

//////////////////////////////////////////////////////////////////////

KscClient::~KscClient()
{
    PLT_PRECONDITION(the_client == this);

    // destroy related KscServer objects
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

KscServer *
KscClient::createServer(KsString host, 
                        const KsServerDesc &desc)
{
    KsString temp("/");
    temp += host;
    temp += "/";
    temp += desc.name;

    KscAbsPath full_name(temp);
    PLT_ASSERT(full_name.isValid());

    KscServer *pServer = 0;

    if( server_table.query(full_name, pServer) ) {
        // server already exists, return pointer to it
        //
        return pServer;
    }
    else {
        // create new server
        //
        pServer = new KscServer(host, desc);
        if( pServer && pServer->getStatus() == KscServer::KscOk ) {
            // object successfully created
            //
            server_table.add(full_name, pServer);
            return pServer;
        }

        // creation failed, clean up if necessary
        if( pServer ) {
            delete pServer;
        }
        return 0;
    }
}

//////////////////////////////////////////////////////////////////////

KscServer *
KscClient::getServer(const KscAbsPath &host_and_name) 
{
    PLT_PRECONDITION(host_and_name.isValid());

    KscServer *pServer;

#if PLT_DEBUG
    cout << "KscClient : Server "
         << host_and_name
         << " requested"
         << endl;
#endif

    bool ok = server_table.query(host_and_name, pServer);

#if PLT_DEBUG
    if( ok ) {
        cout << "Success: located at "
             << pServer << endl;
    }
    else {
        cout << "failed"
             << endl;
        printServers();
    }
#endif

    return ok ? pServer : 0;
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
//   We try to establish an TCP connect to the named server.
//   After construction error conditions should be checked 
//   with getStatus() and getErrcode().
//
KscServer::KscServer(KsString host, 
                     const KsServerDesc &server)
: host_name(host)
{
    struct hostent *hp;
    struct sockaddr_in server_addr; 
    int socket = RPC_ANYSOCK;
    
    status = KscOk;         // assumes everything is ok
    errcode = RPC_SUCCESS;  // 

    // locate host
    //
    hp = gethostbyname(host);
    if( !hp ) {
        status = KscHostNotFound;
        return;
    }

    // request server description from manager
    //
    if( !getServerDesc(hp, server, server_info) ) {
        // dont change status set by getServerDescription()
        return;
    }
    if( server_info.result != KS_ERR_OK ) {
        status = KscNoServer;
        return;
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
                             server.protocol_version,
                             &socket,
                             0, 0 );

    if( !pClient ) {
        status = KscCannotCreateClientHandle;
#if PLT_DEBUG
        clnt_pcreateerror("ERROR :");
#endif
        return;
    }

}

//////////////////////////////////////////////////////////////////////
// Destructor
// We close the TCP connection and destroy all related communication
// objects.
//
KscServer::~KscServer()
{
    if(pClient) {
        clnt_destroy(pClient);
    }

    PltHashIterator<KscAbsPath,KscCommObject *>  it(object_table);
    while(it) {
        delete it->a_value;
        ++it;
    }
}

//////////////////////////////////////////////////////////////////////
// getServerDescription and helper structs and classes
//

struct KscGetServerInStruct 
{
    KscGetServerInStruct(KsAvTicket *avTicket, 
                        KsGetServerParams *params)
    : avt(avTicket),
      gsp(params)
    {}

    KsAvTicket *avt; // TODO : change to KscAvModule
    KsGetServerParams *gsp;
};

struct KscGetServerOutStruct
{
    KscGetServerOutStruct(KsGetServerResult *result)
    : avt(0),
      gsr(result)
    {}

    KsAvTicket *avt; // TODO : change to KscAvModule
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
    
    data->avt = KsAvTicket::xdrNew(xdr);

    if( !data->avt ) {
        return false;
    } 

    PLT_ASSERT(data->gsr);

    return data->gsr->xdrDecode(xdr);
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

    // changed to TCP for testing
    //
    pUDP = clnttcp_create(&addr_manager,
                          KS_RPC_PROGRAM_NUMBER,
                          KS_PROTOCOL_VERSION,
                          // wait,
                          &socket, 0, 0);
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
    KsAvNoneTicket avTicket(KS_ERR_OK, KS_AC_READ | KS_AC_WRITE);
    KscGetServerInStruct inData(&avTicket, &params);
    KscGetServerOutStruct outData(&server_desc);

    errcode = clnt_call(pUDP, 
                        KS_GETSERVER,
                        KscGetServerInHelper,
                        &inData,
                        KscGetServerOutHelper,
                        &outData,
                        KSC_UDP_TIMEOUT);

    // TODO
    if( outData.avt ) {
        delete outData.avt;
    }

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
    KscGetPPInStruct(KsAvTicket *avTicket,
                      const KsGetPPParams *params)
    : avt(avTicket),
      gpp(params)
    {}

    KsAvTicket *avt;
    const KsGetPPParams *gpp;
};

struct KscGetPPOutStruct
{
    KscGetPPOutStruct(KsGetPPResult *result)
    : avt(0),
      gpr(result)
    {}

    KsAvTicket *avt;
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
    
    data->avt = KsAvTicket::xdrNew(xdr);

    if( !data->avt ) {
        return false;
    } 

    PLT_ASSERT(data->gpr);

    return data->gpr->xdrDecode(xdr);
}


bool 
KscServer::getPP(const KsGetPPParams &params,
                 KsGetPPResult &result)
{
    PLT_PRECONDITION(pClient);

#if PLT_DEBUG
    cout << "Requesting server " 
         << host_name << "/" << server_info.server.name 
         << "for getPP service." << endl;
    cout << "Parameters : " << endl; 
    cout << "\tPath : " << params.path << endl;
    cout << "\tType mask : " << params.type_mask << endl;
    cout << "\tName mask : " << params.name_mask << endl;
    cout << endl;
#endif

    KscGetPPInStruct inData(&ksc_the_ticket,&params);
    KscGetPPOutStruct outData(&result);

    errcode = clnt_call(pClient, KS_GETVAR,
                        KscGetPPInHelper,
                        &inData,
                        KscGetPPOutHelper,
                        &outData,
                        KSC_RPCCALL_TIMEOUT);

    if(outData.avt) {
        // AvTicket currently unused
        delete outData.avt;
    }

    return errcode == RPC_SUCCESS;
}


//////////////////////////////////////////////////////////////////////
// helper structs and functions for getVar()
// 

struct KscGetVarInStruct
{
    KscGetVarInStruct(KsAvTicket *avTicket,
                      const KsGetVarParams *params)
    : avt(avTicket),
      gvp(params)
    {}

    KsAvTicket *avt;
    const KsGetVarParams *gvp;
};

struct KscGetVarOutStruct
{
    KscGetVarOutStruct(KsGetVarResult *result)
    : avt(0),
      gvr(result)
    {}

    KsAvTicket *avt;
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
    
    data->avt = KsAvTicket::xdrNew(xdr);

    if( !data->avt ) {
        return false;
    } 

    PLT_ASSERT(data->gvr);

    return data->gvr->xdrDecode(xdr);
}


bool 
KscServer::getVar(const KsGetVarParams &params,
                  KsGetVarResult &result)
{
    PLT_PRECONDITION(pClient);

#if PLT_DEBUG
    cout << "GetVar request to server "
         << host_name << "/" << server_info.server.name << endl;
#endif

    KscGetVarInStruct inData(&ksc_the_ticket,&params);
    KscGetVarOutStruct outData(&result);

    errcode = clnt_call(pClient, KS_GETVAR,
                        KscGetVarInHelper,
                        &inData,
                        KscGetVarOutHelper,
                        &outData,
                        KSC_RPCCALL_TIMEOUT);

    if(outData.avt) {
        // AvTicket currently unused
        delete outData.avt;
    }

    return errcode == RPC_SUCCESS;
}
              

//////////////////////////////////////////////////////////////////////
// helper structs and functions for setVar()
// 

struct KscSetVarInStruct
{
    KscSetVarInStruct(KsAvTicket *avTicket,
                      const KsSetVarParams *params)
    : avt(avTicket),
      svp(params)
    {}

    KsAvTicket *avt;
    const KsSetVarParams *svp;
};

struct KscSetVarOutStruct
{
    KscSetVarOutStruct(KsSetVarResult *result)
    : avt(0),
      svr(result)
    {}

    KsAvTicket *avt;
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
    
    data->avt = KsAvTicket::xdrNew(xdr);

    if( !data->avt ) {
        return false;
    } 

    PLT_ASSERT(data->svr);

    return data->svr->xdrDecode(xdr);
}


bool 
KscServer::setVar(const KsSetVarParams &params,
                  KsSetVarResult &result)
{
    PLT_PRECONDITION(pClient);

#if PLT_DEBUG
    cout << "SetVar request to server "
         << host_name << "/" << server_info.server.name << endl;
#endif

    KscSetVarInStruct inData(&ksc_the_ticket,&params);
    KscSetVarOutStruct outData(&result);

    errcode = clnt_call(pClient, KS_SETVAR,
                        KscSetVarInHelper,
                        &inData,
                        KscSetVarOutHelper,
                        &outData,
                        KSC_RPCCALL_TIMEOUT);

    if(outData.avt) {
        // AvTicket currently unused
        delete outData.avt;
    }

    return errcode == RPC_SUCCESS;
}
  


//////////////////////////////////////////////////////////////////////
// helper structs and functions for exgData()
// 

struct KscExgDataInStruct
{
    KscExgDataInStruct(KsAvTicket *avTicket,
                      const KsExgDataParams *params)
    : avt(avTicket),
      edp(params)
    {}

    KsAvTicket *avt;
    const KsExgDataParams *edp;
};

struct KscExgDataOutStruct
{
    KscExgDataOutStruct(KsExgDataResult *result)
    : avt(0),
      edr(result)
    {}

    KsAvTicket *avt;
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
    
    data->avt = KsAvTicket::xdrNew(xdr);

    if( !data->avt ) {
        return false;
    } 

    PLT_ASSERT(data->edr);

    return data->edr->xdrDecode(xdr);
}


bool 
KscServer::exgData(const KsExgDataParams &params,
                  KsExgDataResult &result)
{
    PLT_PRECONDITION(pClient);

#if PLT_DEBUG
    cout << "ExgData request to server "
         << host_name << "/" << server_info.server.name << endl;
#endif

    KscExgDataInStruct inData(&ksc_the_ticket,&params);
    KscExgDataOutStruct outData(&result);

    errcode = clnt_call(pClient, KS_EXGDATA,
                        KscExgDataInHelper,
                        &inData,
                        KscExgDataOutHelper,
                        &outData,
                        KSC_RPCCALL_TIMEOUT);

    if(outData.avt) {
        // AvTicket currently unused
        delete outData.avt;
    }

    return errcode == RPC_SUCCESS;
}            


//////////////////////////////////////////////////////////////////////

bool
KscServer::registerVar(KscCommObject *newObject) 
{
    if(newObject) {
        object_table.add(newObject->getFullPath(), newObject);
        return true;
    }

    return false;
}

//////////////////////////////////////////////////////////////////////

bool
KscServer::deregisterVar(KscCommObject *object)
{
    if(object) {
        KscCommObject *temp;
        if(object_table.query(object->getFullPath(),temp)) {
            PLT_ASSERT(temp == object);
            return true;
        }
    }

    return false;
}
        
//////////////////////////////////////////////////////////////////////
// EOF client.cpp
//////////////////////////////////////////////////////////////////////
                 

    
    
    




