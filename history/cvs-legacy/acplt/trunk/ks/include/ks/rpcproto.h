/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/include/ks/rpcproto.h,v 1.1 1998-06-29 11:19:17 harald Exp $ */
/*
 * Copyright (c) 1998
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

/*
 * rpcproto.h -- Implements a class for handling RPC request and reply
 *               headers.
 *
 * Written by Harald Albrecht <harald@plt.rwth-aachen.de>
 */

#ifndef KS_RPCPROTO_H_INCLUDED
#define KS_RPCPROTO_H_INCLUDED

#include "ks/rpc.h"


class KsRpcHeader {
public:
    KsRpcHeader();
    KsRpcHeader(XDR *xdrs, bool &ok);
    static KsRpcHeader *xdrNew(XDR *xdrs);
    virtual bool xdrEncode(XDR *xdrs) const;
    virtual bool xdrDecode(XDR *xdrs);

    void acceptCall();
    void setAuthRefusialError(u_long why);
    void setDecodeError();
    void setNoProcedureError();
    void setNoProgrammError();
    void setProgramVersionMismatchError(u_long low, u_long high);
    void setSystemError();
    void setRPCVersionMismatchError(u_long low, u_long high);

    typedef enum {
	KsRpcCALL = 0,
	KsRpcREPLY = 1
    } KsRpcMsgType;

    typedef enum {
	KsRpcMSGACCEPTED = 0,
	KsRpcMSGDENIED = 1
    } KsRpcReplyStat;

    typedef enum {
	KsRpcSUCCESS = 0,
	KsRpcPROGUNAVAIL = 1,
	KsRpcPROGMISMATCH = 2,
	KsRpcPROCUNAVAIL = 3,
	KsRpcGARBAGEARGS = 4,
	KsRpcSYSTEMERR = 5
    } KsRpcAcceptStat;

    typedef enum {
	KsRpcRPCMISMATCH = 0,
	KsRpcAUTHERROR = 1
    } KsRpcRejectStat;

    typedef enum {
	KsRpcAUTHNONE = 0,
	KsRpcAUTHSYS = 1,
	KsRpcAUTHSHORT = 2
    } KsRpcAuthFlavor;

    u_long          _xid;           // transaction ID
    KsRpcMsgType    _msg_type;      // either a request (call) or reply
    
    u_long          _rpc_version;   // must be 2
    u_long          _prog_number;   // for ACPLT/KS: 0x49678
    u_long          _prog_version;  // RPC program version (not RPC version)
    u_long          _procedure;     // service number
    KsRpcReplyStat  _reply_status;  // accepted or denied
    KsRpcAcceptStat _accept_status; // status if call was accepted
    u_long          _low, _high;    // allowed RPC program version range
    u_long          _auth_error;    // authentification error status
    KsRpcRejectStat _reject_status; // status if call was rejected
}; // class KsRpcHeader


#endif

/* End of rpcproto.h */
