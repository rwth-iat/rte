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

#include <plt/hashtable.h>

#include "ks/rpc.h"
#include "ks/xdr.h"
#include "ks/register.h"
#include "ks/serviceparams.h"
#include "ks/avmodule.h"

#if PLT_SYSTEM_OPENVMS
#include <netdb.h>
#endif

//////////////////////////////////////////////////////////////////////
// forward declaration
class KscServerBase;

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
// delete the object pointed to, since it is managed internally.
//
// TODO: change server representation from pointers to handles
//
//////////////////////////////////////////////////////////////////////

class KscClient
{
public:
    virtual ~KscClient();

    // returns a pointer to the client object
    //
    static KscClient *getClient();
    // use this function before you create any KscVariable or 
    // KscDomain object in order to use a class derived from
    // KscClient as client object
    // returns false if a client is already set
    // os must be KsOsNew or KsOsUnmanaged
    //
    static bool setClient(KscClient *cl, KsOwnership os);

    // find server by name, maybe returns 0
    //
    KscServerBase *getServer(const KsString &host_and_name); 

    //
    // AV related functions
    //
    void setAvModule(const KscAvModule *);
    const KscAvModule *getAvModule() const;

    //
    // set timeout and numbers of retries
    // (affects only server-objects that will be created later)
    //
    void setTimeouts(const PltTime &rpc_timeout,        // time to complete a RPC
                     const PltTime &retry_wait,         // time between retries
                     size_t retries);                   // number of retries

#if PLT_DEBUG
    void printServers();
#endif

protected:
    KscClient();

    // find or create server,
    // should only be used by KscCommObject objects
    //
    friend class KscCommObject;
    virtual KscServerBase *createServer(KsString host_and_name); 

    // destroy an server, should only be used
    // by KscServer objects
    //
    friend class KscServerBase;
    void deleteServer(KscServerBase *);

    const KscAvModule *av_module;

    PltTime _rpc_timeout;
    PltTime _retry_wait;
    size_t _retries;

    PltHashTable<KsString,KscServerBase *> server_table;

private:
    KscClient(const KscClient &); // forbidden
    KscClient &operator = (const KscClient &); // forbidden

    static void _createClient();

    // helper class to clean up memory
    // at end of programm
    //
    class CleanUp {
    public:
        CleanUp();
        ~CleanUp();

        bool shutdown_delete;
    };

    friend class CleanUp;

    static KscClient *_the_client;
    static CleanUp _clean_up;
};

//////////////////////////////////////////////////////////////////////
// class KscServerBase
//
class KscServerBase
{
    PLT_DECL_RTTI;
public:
    //
    // service functions
    //
    virtual bool getPP(const KscAvModule *avm,
                       const KsGetPPParams &params,
                       KsGetPPResult &result) = 0;

    virtual bool getVar(const KscAvModule *avm,
                        const KsGetVarParams &params,
                        KsGetVarResult &result) = 0;

    virtual bool setVar(const KscAvModule *avm,
                        const KsSetVarParams &params,
                        KsSetVarResult &result) = 0;

    virtual bool exgData(const KscAvModule *avm,
                         const KsExgDataParams &params,
                         KsExgDataResult &result) = 0;

    // AV related functions
    //
    virtual void setAvModule(const KscAvModule *);
    virtual const KscAvModule *getAvModule() const;

    // selectors
    //
    KsString getHost() const;           // host
    KsString getName() const;           // name
    KsString getHostAndName() const;    // eg. "//host/name"
    virtual u_short getProtocolVersion() const = 0;
    virtual PltTime getExpiresAt() const = 0;
    virtual bool isLiving() const = 0;
    KS_RESULT getLastResult() const;

protected:
    KscServerBase(KsString host, KsString name);
    KscServerBase(KsString hostAndName);
    virtual ~KscServerBase();
    
    friend class KscClient;              // for access to ctor and dtor
    friend class KscCommObject;          // for access to the following functions
    
    void incRefcount();
    void decRefcount();

    KsString host_name, server_name, host_and_name;
    const KscAvModule *av_module;
    long ref_count;                // communication objects related to this server
    KS_RESULT _last_result;
};

//////////////////////////////////////////////////////////////////////
// class KscServer
//
class KscServer 
: public KscServerBase
{
    PLT_DECL_RTTI;
public:
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
    bool getPP(const KscAvModule *avm,
               const KsGetPPParams &params,
               KsGetPPResult &);

    bool getVar(const KscAvModule *avm,
                const KsGetVarParams &params,
                KsGetVarResult &result);

    bool setVar(const KscAvModule *avm,
                const KsSetVarParams &params,
                KsSetVarResult &result);

    bool exgData(const KscAvModule *avm,
                 const KsExgDataParams &params,
                 KsExgDataResult &result);

    //
    // accessors
    //
    u_short getProtocolVersion() const;
    PltTime getExpiresAt() const;
    bool isLiving() const;

    //
    // set timeout and numbers of retries
    //
    void setTimeouts(const PltTime &rpc_timeout,        // time to complete a RPC
                     const PltTime &retry_wait,         // time between retries
                     size_t retries);                   // number of retries
 

protected:
    // internal status
    //
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
    
    KscNegotiator *getNegotiator(const KscAvModule *);

    // service functions
    //
    bool getServerDesc(struct sockaddr_in *host_addr,    // host 
                       const KsServerDesc &server,       // description
                       KsGetServerResult &server_info);  // result


    bool createTransport();
    void destroyTransport();
    virtual bool reconnectServer(size_t try_count, enum_t errcode );
    virtual bool wait(PltTime howLong);
    bool getHostAddr(struct sockaddr_in *addr);
    void setResultAfterService();

    KsServerDesc server_desc;      // server description given by user
    KsGetServerResult server_info; // server description given by manager
    enum clnt_stat errcode;        // last RPC error code
    TStatus status;                // internal status
    CLIENT *pClient;               // RPC client handle

    PltTime _rpc_timeout;
    PltTime _retry_wait;
    size_t _retries;

    PltHashTable<PltKeyCPtr<KscAvModule>,KscNegotiatorHandle> neg_table;

    friend class KscClient;
    KscServer(KsString host, const KsServerDesc &server);
    KscServer(KsString hostAndName, u_short protocolVersion);
    ~KscServer();

private:
    KscServer(const KscServer &);              // forbidden
    KscServer &operator = (const KscServer &); // forbidden
};


//////////////////////////////////////////////////////////////////////
// Inline Implementation
//////////////////////////////////////////////////////////////////////
// KscServerBase
//////////////////////////////////////////////////////////////////////

inline
KscServerBase::~KscServerBase()
{}

//////////////////////////////////////////////////////////////////////

inline
void 
KscServerBase::setAvModule(const KscAvModule *avm)
{
    av_module = avm;
}

//////////////////////////////////////////////////////////////////////

inline
const KscAvModule *
KscServerBase::getAvModule() const
{
    return av_module;
}

//////////////////////////////////////////////////////////////////////

inline
KsString 
KscServerBase::getHost() const
{
    return host_name;
}

//////////////////////////////////////////////////////////////////////

inline
KsString
KscServerBase::getName() const
{
    return server_name;
}

//////////////////////////////////////////////////////////////////////

inline
KsString
KscServerBase::getHostAndName() const
{
    return host_and_name;
}

//////////////////////////////////////////////////////////////////////

inline
KS_RESULT
KscServerBase::getLastResult() const
{
    return _last_result;
}

//////////////////////////////////////////////////////////////////////
// KscServer
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
                        (xdrproc_t) xdr_void, 0, 
                        (xdrproc_t) xdr_void, 0, 
                        KSC_RPCCALL_TIMEOUT);

    return errcode == RPC_SUCCESS;
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
KscClient *
KscClient::getClient() 
{
    if(!_the_client) {
        _createClient();
    }
    return _the_client;
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
 

//////////////////////////////////////////////////////////////////////
// class KscClient::CleanUp
//////////////////////////////////////////////////////////////////////

inline
KscClient::CleanUp::CleanUp()
: shutdown_delete(false) 
{}

inline
KscClient::CleanUp::~CleanUp()
{
    if(shutdown_delete && _the_client) {
        delete _the_client;
    }
}

//////////////////////////////////////////////////////////////////////

#endif

//////////////////////////////////////////////////////////////////////
// EOF client.h
//////////////////////////////////////////////////////////////////////










