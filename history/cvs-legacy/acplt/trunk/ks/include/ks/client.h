/* -*-plt-c++-*- */

#ifndef KSC_CLIENT_INCLUDED 
#define KSC_CLIENT_INCLUDED

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

#include <ks/rpc.h>
#include <ks/xdr.h>
#include <ks/register.h>
#include <ks/serviceparams.h>
#include <ks/avticket.h>
#include <ks/abspath.h>
#include <ks/commobject.h>

#include <plt/hashtable.h>

//////////////////////////////////////////////////////////////////////
// forward declaration
class KscServer;

//////////////////////////////////////////////////////////////////////
// timeout and max tries when contacting manager via UDP
// timeout when calling rpc function via TCP
//
const int KSC_UDP_MAX_TRIES = 5;
const struct timeval KSC_UDP_TIMEOUT = {10, 0};      // DONT USE KsTime 
const struct timeval KSC_RPCCALL_TIMEOUT = {10, 0};  // or PltTime

//////////////////////////////////////////////////////////////////////
// class KscClient
//
// Tasks:
// - keep track of server objects
// - allow access to a variable by full name(not completed yet)
//
//////////////////////////////////////////////////////////////////////

class KscClient
{
public:
    KscClient();
    ~KscClient();

    static KscClient *getClient();
    
    // try to create specified server
    // 
    KscServer *createServer(KsString host, 
                            const KsServerDesc &desc);
    // find server
    //
    KscServer *getServer(const KscAbsPath &host_and_name); 

#if PLT_DEBUG
    void printServers();
#endif

protected:
    // for each application there is only one KscClient object
    //
    static KscClient *the_client;
    
    PltHashTable<KscAbsPath,KscServer *> server_table;
};

//////////////////////////////////////////////////////////////////////
// class KscServer
//
class KscServer
{
public:
    KscServer(KsString host, const KsServerDesc &server);
    ~KscServer();

    enum TStatus {
        KscOk = 0,
        KscHostNotFound,
        KscCannotCreateClientHandle,
        KscCannotCreateUDPClient,
        KscRPCCallFailed,
        KscNoServer
    };

    // return status of the server (see beyond)
    TStatus getStatus() const;
    // return error code of the last rpc call, 
    // see rpc library for definition of the values
    enum_t getErrcode() const;
    // ping server
    bool ping();

    // TODO : handling of variables
    // 
    // KscCommObject *newCommObject(const KscAbsPath &);
    // bool deleteCommObject(const KscAbsPath &);
    // bool deleteCommObject(KscCommObject *);
    
    // service functions
    // TODO: integrate AV
    //
    bool getPP(const KsGetPPParams &params,
               KsGetPPResult &);

    bool getVar(const KsGetVarParams &params,
                KsGetVarResult &result);

    bool setVar(const KsSetVarParams &params,
                KsSetVarResult &result);

    bool exgData(const KsExgDataParams &params,
                 KsExgDataResult &result);

    // int initProjProps(); // TODO

protected:
    // service functions
    //
    bool getServerDesc(struct hostent *hp,               // host 
                       const KsServerDesc &server,       // description
                       KsGetServerResult &server_info);  // result

    KsString host_name;
    KsGetServerResult server_info;
    enum clnt_stat errcode;
    TStatus status;
    CLIENT *pClient;

    PltHashTable<KscAbsPath,KscCommObject *> object_table;

    friend class KscCommObject;          // for access to the following functions
    bool registerVar(KscCommObject *);
    bool deregisterVar(KscCommObject *);
};


//////////////////////////////////////////////////////////////////////
// Inline Implementation
//////////////////////////////////////////////////////////////////////

inline 
KscServer::TStatus
KscServer::getStatus() const
{
    return status;
}

//////////////////////////////////////////////////////////////////////

inline
enum_t
KscServer::getErrcode() const
{
    return errcode;
} 

//////////////////////////////////////////////////////////////////////

inline
bool
KscServer::ping()
{
    errcode = clnt_call(pClient, 0, 
                        xdr_void, 0, 
                        xdr_void, 0, 
                        KSC_RPCCALL_TIMEOUT);

    return errcode == RPC_SUCCESS;
}

//////////////////////////////////////////////////////////////////////

inline
KscClient *
KscClient::getClient() 
{
    return the_client;
}
 
#endif








