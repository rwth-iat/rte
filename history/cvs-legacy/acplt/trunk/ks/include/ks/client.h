/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/include/ks/client.h,v 1.41 2008-09-22 08:26:09 henning Exp $ */
#ifndef KSC_CLIENT_INCLUDED
#define KSC_CLIENT_INCLUDED
/*
 * Copyright (c) 1996, 1997, 1998, 1999, 2000
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

//////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <plt/hashtable.h>

#include "ks/rpc.h"
#include "ks/hostent.h"
#include "ks/xdr.h"
#include "ks/register.h"
#include "ks/serviceparams.h"
#include "ks/avmodule.h"

//////////////////////////////////////////////////////////////////////
// forward declaration
class KscServerBase;

//////////////////////////////////////////////////////////////////////
// timeout and max tries when contacting manager via UDP
// timeout when calling rpc function via TCP
//
const int KSC_UDP_MAX_TRIES = 5;
const struct timeval KSC_UDP_TIMEOUT = {10, 0};      // DONT USE KsTime 
const struct timeval KSC_RPCCALL_TIMEOUT = {30, 0};  // or PltTime

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
    
    //
    // returns a pointer to the client object
    //
    static KscClient *getClient();
    
    //
    // use this function before you create any KscVariable or 
    // KscDomain object in order to use a class derived from
    // KscClient as client object
    // returns false if a client is already set
    // os must be KsOsNew or KsOsUnmanaged
    //
    static bool setClient(KscClient *cl, KsOwnership os);

    //
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
    void setTimeouts(const PltTime &rpc_timeout, // time to complete a RPC
                     const PltTime &retry_wait,  // time between retries
                     size_t tries);              // number of tries
    void getTimeouts(PltTime &rpc_timeout,
		     PltTime &retry_wait,
		     size_t &tries);

#if PLT_DEBUG
    void printServers();
#endif

protected:
    KscClient();

    //
    // find or create server, should only be used by KscCommObject objects
    //
    friend class KscCommObject;
    virtual KS_RESULT createServer(KsString host_and_name,
                                   u_short protocol_version,
				   KscServerBase *&pServer);
    //
    // destroy an server, should only be used
    // by KscServer objects
    //
    friend class KscServerBase;
    void deleteServer(KscServerBase *);

    const KscAvModule *av_module;

    PltTime _rpc_timeout;
    PltTime _retry_wait;
    size_t _tries;

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
    // service functions defined in ks_core
    //
    virtual bool getPP(const KscAvModule *avm,
                       const KsGetPPParams &params,
                       KsGetPPResult &result) = 0;
    virtual bool getEP(const KscAvModule *avm,
                       const KsGetEPParams &params,
                       KsGetEPResult &result);

    virtual bool getVar(const KscAvModule *avm,
                        const KsGetVarParams &params,
                        KsGetVarResult &result) = 0;

    virtual bool setVar(const KscAvModule *avm,
                        const KsSetVarParams &params,
                        KsSetVarResult &result) = 0;

    virtual bool exgData(const KscAvModule *avm,
                         const KsExgDataParams &params,
                         KsExgDataResult &result) = 0;

    // 
    // general service function
    //
    virtual bool requestService(const KsString &extension,
                        	u_short minor_opcode,
                        	const KscAvModule *avm,
                        	const KsXdrAble &params,
                        	KsResult &result) = 0;
    virtual bool requestByOpcode(u_long service, 
                                 const KscAvModule *avm,
                                 const KsXdrAble &params,
                                 KsResult &result) = 0;

    //
    // transport helper...
    //
    virtual bool getServerVersion(u_long &version) = 0;

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
    // ping server
    bool ping();

#if 0
    // reread server description and state 
    // from manager
    //
    bool getStateUpdate();
#endif

    //
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
    // general service function
    //
    virtual bool requestService(const KsString &extension,
                        	u_short minor_opcode,
                        	const KscAvModule *avm,
                        	const KsXdrAble &params,
                        	KsResult &result);
    virtual bool requestByOpcode(u_long service, 
                                 const KscAvModule *avm,
                                 const KsXdrAble &params,
                                 KsResult &result);

    virtual bool getServerVersion(u_long &version);


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
                     size_t tries);                     // number of tries
    void getTimeouts(PltTime &rpc_timeout,
		     PltTime &retry_wait,
		     size_t &tries);


protected:
    KscNegotiator *getNegotiator(const KscAvModule *);

    friend class KscAvModule;
    void dismissNegotiator(const KscAvModule *);

    // service functions
    //
    bool getServerDesc(struct sockaddr_in *host_addr,    // host 
                       unsigned short port,              // opt. port of manager
                       const KsServerDesc &server,       // description
                       KsGetServerResult &server_info);  // result


    bool createTransport();
    void destroyTransport();
    virtual bool reconnectServer(size_t try_count, enum clnt_stat errcode);
    virtual bool reconnectServer(KS_RESULT result);
    bool getHostAddr(struct sockaddr_in *addr);
    void setResultAfterService(enum clnt_stat errcode);

    void initExtTable();


    KsServerDesc server_desc;      // server description given by user
    KsGetServerResult server_info; // server description given by manager
    CLIENT *_client_transport;     // RPC client handle

    PltTime _rpc_timeout;
    PltTime _retry_wait;
    size_t _tries;
    KSC_IP_TYPE last_ip;	
      	// last IP used to connect to server/manager   

    PltHashTable<PltKeyPlainConstPtr<KscAvModule>,KscNegotiatorHandle> neg_table;
    PltHashTable<KsString, u_long> ext_opcodes;

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

//////////////////////////////////////////////////////////////////////

inline
bool
KscServer::ping()
{
    enum clnt_stat errcode;
    
    if ( !_client_transport ) {
        if( !createTransport() ) {
            if ( !reconnectServer(_last_result) ) {
                return false;
            }
        }
    }
    errcode = clnt_call(_client_transport, 0, 
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

/////////////////////////////////////////////////////////////////////////////

#if 0
inline bool 
KscServer::getPP(const KscAvModule *avm,
                 const KsGetPPParams &params,
                 KsGetPPResult &result)
{
    return requestByOpcode(KS_GETPP, avm, &params, &result);
}

/////////////////////////////////////////////////////////////////////////////

inline bool 
KscServer::getVar(const KscAvModule *avm,
                  const KsGetVarParams &params,
                  KsGetVarResult &result)
{
    return requestByOpcode(KS_GETVAR, avm, &params, &result);
}

/////////////////////////////////////////////////////////////////////////////

inline bool 
KscServer::setVar(const KscAvModule *avm,
                  const KsSetVarParams &params,
                  KsSetVarResult &result)
{
    return requestByOpcode(KS_SETVAR, avm, &params, &result);
}

/////////////////////////////////////////////////////////////////////////////

inline bool
KscServer::exgData(const KscAvModule *avm,
                   const KsExgDataParams &params,
                   KsExgDataResult &result)
{
    return requestByOpcode(KS_EXGDATA, avm, &params, &result);
}

#endif

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

// End of ks/client.h
