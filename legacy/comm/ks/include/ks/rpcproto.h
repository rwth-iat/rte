/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/include/ks/rpcproto.h,v 1.7 2008-09-22 08:26:09 henning Exp $ */
/*
 * Copyright (c) 1996, 1997, 1998, 1999
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
    virtual ~KsRpcHeader() {}

    void acceptCall();
    void setAuthRefusialError(u_long why);
    void setDecodeError();
    void setNoProcedureError();
    void setNoProgrammError();
    void setProgramVersionMismatchError(u_long low, u_long high);
    void setSystemError();
    void setRPCVersionMismatchError(u_long low, u_long high);

    void setRequest(u_long xid, u_long prog_number, u_long prog_version,
		    u_long proc_nr);

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
