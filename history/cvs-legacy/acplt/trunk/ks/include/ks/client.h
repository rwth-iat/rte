/* -*-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/include/ks/client.h,v 1.37 2003-10-21 14:46:59 harald Exp $ */
#ifndef KSC_CLIENT_INCLUDED
#define KSC_CLIENT_INCLUDED
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
/* updated to work with nonblocking IO by Albrecht boshi */

//////////////////////////////////////////////////////////////////////

#include <plt/hashtable.h>

#include "ks/rpc.h"
#include "ks/xdr.h"
#include "ks/register.h"
#include "ks/serviceparams.h"
#include "ks/avmodule.h"
#include "ks/clnrequest.h"
#include "ks/serverconnection.h"

//////////////////////////////////////////////////////////////////////
// forward declaration
class KscServerBase;


const PltTime KSC_RPCCALL_TIMEOUT(30);


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
    // Support for those who want to have several connections at the
    // same time to the same KS server... Usually callers will want to
    // specify KS_MINPROTOCOL_VERSION as the protocol_version...
    //
    virtual KS_RESULT newServer(KsString host_and_name,
				u_short protocol_version,
				KscServerBase *&pServer);

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
    virtual u_short getProtocolVersion() = 0;
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


class KscServerConnection;
class KscServerReconnectTimer;


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
    u_short getProtocolVersion();

    //
    // new nonblocking stuff
    void terminateRequests();
    bool requestAsyncByOpcode(KscServiceRequestHandle hreq);

    bool open(); // this is async
    bool openOnlyAndWait(); // this is sync
    void close(); // this is sync and immediately closes the connection

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
    void init();

    KscNegotiator *getNegotiator(const KscAvModule *);

    friend class KscAvModule;
    void dismissNegotiator(const KscAvModule *);


    //
    // new nonblocking transport stuff
    friend class KscServerConnection;
    friend class KscServerReconnectTimer;

    KsServerConnection *_svr_con;
    KscServiceRequestHandle _current_request;
    PltList<KscServiceRequestHandle> _request_queue;
    
    // manage requests queue and request initiation
    void initiateRequestIfPossible(KscServiceRequestHandle newRequest);

    // blocking wait for request to become finished
    void waitForRequest(KscServiceRequestHandle newRequest);

    // manage events from the server connection and from the timer queue
    void async_attention(KsServerConnection::KsServerConnectionOperations op);
    void reconnectTimerTrigger();

    // reconnect control stuff
    KscServerReconnectTimer *_reconnect_timer;
    PltTime _rpc_timeout;
    PltTime _retry_wait;
    size_t _tries;
    size_t _tries_remaining;

    virtual bool shouldReconnectServer(KS_RESULT result);
    void finishAndNotify(KS_RESULT result);

    void initExtTable();


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

// End of ks/client.h
