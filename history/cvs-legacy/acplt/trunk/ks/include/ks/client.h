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

#include "ks/rpc.h"
#include "ks/xdr.h"
#include "ks/register.h"
#include "ks/serviceparams.h"
#include "ks/avmodule.h"
#include "ks/abspath.h"
#include "ks/commobject.h"

#include "plt/hashtable.h"

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
//
// Only one instance of this class exists for every programm. You 
// should use KscClient::getClient() to access it, but you should not
// delete the object pointed to, since it is a static object.
//
//////////////////////////////////////////////////////////////////////

class KscClient
{
public:
    ~KscClient();

    // returns a pointer to the client object
    //
    static KscClient *getClient();

    // find server by name
    //
    KscServer *getServer(const KscAbsPath &host_and_name); 

    void setAvModule(const KscAvModule *);
    const KscAvModule *getAvModule() const;

    KscNegotiator *getNegotiator(KscServer *forServer);

#if PLT_DEBUG
    void printServers();
#endif

protected:
    KscClient();

    // find or create server,
    // should only be used by KscCommObject objects
    //
    friend KscCommObject;
    KscServer *createServer(const KscAbsPath &host_and_name); 

    // destroy an server, should only be used
    // by KscServer objects
    //
    friend class KscServer;
    void deleteServer(KscServer *);

    // for each application there is only one KscClient object
    //
    static KscClient the_client;

    const KscAvModule *av_module;

    // this negotiator is used if no AV-module is set
    // or if the creation of a negotiator failed
    static KscNegotiator *none_negotiator;
    
    PltHashTable<KscAbsPath,KscServer *> server_table;

private:
    KscClient(const KscClient &); // forbidden
    KscClient &operator = (const KscClient &); // forbidden
};

//////////////////////////////////////////////////////////////////////
// class KscServer
//
class KscServer
{
public:

    enum TStatus {
        KscOk = 0,
        KscNotInitiated,
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

    // reread server description and state 
    // from manager
    //
    bool getStateUpdate();

    // service functions
    //
    bool getPP(KscNegotiator *negotiator,
               const KsGetPPParams &params,
               KsGetPPResult &);

    bool getVar(KscNegotiator *negotiator,
                const KsGetVarParams &params,
                KsGetVarResult &result);

    bool setVar(KscNegotiator *negotiator,
                const KsSetVarParams &params,
                KsSetVarResult &result);

    bool exgData(KscNegotiator *negotiator,
                 const KsExgDataParams &params,
                 KsExgDataResult &result);

    // AV related functions
    //
    void setAvModule(const KscAvModule *);
    const KscAvModule *getAvModule() const;
    KscNegotiator *getNegotiator();

    // selectors
    //
    PltString getHost() const;
    PltString getName() const;
    KscAbsPath getHostAndName() const;
    u_short getProtocolVersion() const;
    PltTime getExpiresAt() const;
    bool isLiving() const; 

protected:
    // service functions
    //
    bool getServerDesc(struct hostent *hp,               // host 
                       const KsServerDesc &server,       // description
                       KsGetServerResult &server_info);  // result

    friend class KscCommObject;          // for access to the following functions
    void incRefcount();
    void decRefcount();

    bool createTransport();
    void destroyTransport();

    KsString host_name;
    KsServerDesc server_desc;      // server description given by user
    KsGetServerResult server_info; // server description given by manager
    enum clnt_stat errcode;        // last RPC error code
    TStatus status;                // internal status
    CLIENT *pClient;               // RPC client handle
    long ref_count;                // communication objects related to this server
    const KscAvModule *av_module;

    PltHashTable<KscAbsPath,KscCommObject *> object_table;

private:
    friend class KscClient;
    KscServer(KsString host, const KsServerDesc &server);
    ~KscServer();

    KscServer(const KscServer &);              // forbidden
    KscServer &operator = (const KscServer &); // forbidden
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
PltString 
KscServer::getHost() const
{
    return host_name;
}

//////////////////////////////////////////////////////////////////////

inline
PltString
KscServer::getName() const
{
    return server_info.server.name;
}

//////////////////////////////////////////////////////////////////////

inline
KscAbsPath
KscServer::getHostAndName() const
{
    PltString temp("/");
    temp += getHost();
    temp += "/";
    temp += getName();

    return KscAbsPath(temp);
}

//////////////////////////////////////////////////////////////////////

inline
u_short
KscServer::getProtocolVersion() const
{
    return server_info.server.protocol_version;
}

//////////////////////////////////////////////////////////////////////

inline
PltTime
KscServer::getExpiresAt() const
{
    // NOTE: without cast gcc reports internal
    // compiler error
    //
    return PltTime(server_info.expires_at);
}

//////////////////////////////////////////////////////////////////////

inline
bool
KscServer::isLiving() const
{
    return server_info.living;
}

//////////////////////////////////////////////////////////////////////

inline
void 
KscServer::setAvModule(const KscAvModule *avm)
{
    av_module = avm;
}

//////////////////////////////////////////////////////////////////////

inline
const KscAvModule *
KscServer::getAvModule() const
{
    return av_module;
}

//////////////////////////////////////////////////////////////////////

inline
KscClient *
KscClient::getClient() 
{
    return &the_client;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

inline
void 
KscClient::setAvModule(const KscAvModule *avm)
{
    av_module = avm;
}

//////////////////////////////////////////////////////////////////////

inline
const KscAvModule *
KscClient::getAvModule() const
{
    return av_module;
}
 
#endif

//////////////////////////////////////////////////////////////////////
// EOF client.h
//////////////////////////////////////////////////////////////////////








