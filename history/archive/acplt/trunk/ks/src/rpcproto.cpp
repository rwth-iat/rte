/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/src/rpcproto.cpp,v 1.8 2007-04-25 12:57:21 martin Exp $ */
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
 * rpcproto.cpp -- Implements a class for handling RPC request and reply
 *                 header.
 *
 * Written by Harald Albrecht <harald@plt.rwth-aachen.de>
 */

#include "ks/rpcproto.h"


// ---------------------------------------------------------------------------
// Factory method to create a new RPC header object from a XDR stream. It
// allocates and constructs a fresh header object and then tries to deseria-
// lize the header from the XDR stream. If it can't allocate the object or
// deserialization failes, then the object is destroyed and a null pointer is
// returned.
//
KsRpcHeader *KsRpcHeader::xdrNew(XDR *xdrs)
{
    KsRpcHeader *p = new KsRpcHeader;
    if ( p ) {
        if ( !p->xdrDecode(xdrs) ) {
	    delete p;
	    p = 0;
	}
    }
    return p;
} // KsRpcHeader::xdrNew


// ---------------------------------------------------------------------------
// Constructs a fresh new RPC header or constructs it from a XDR stream...
//
KsRpcHeader::KsRpcHeader()
    : _xid(0), _msg_type(KsRpcCALL),
      _rpc_version(2),
      _prog_number(0), _prog_version(0), _procedure(0),
      _reply_status(KsRpcMSGACCEPTED),
      _accept_status(KsRpcSYSTEMERR),
      _low(0), _high(0),
      _auth_error(KsRpcAUTHNONE),
      _reject_status(KsRpcRPCMISMATCH)
{
} // KsRpcHeader::KsRpcHeader

KsRpcHeader::KsRpcHeader(XDR *xdrs, bool &ok)
{
    ok = xdrDecode(xdrs);
} // KsRpcHeader::KsRpcHeader


// ---------------------------------------------------------------------------
// Read in (decode) a RPC header from a XDR stream.
//
bool KsRpcHeader::xdrDecode(XDR *xdrs)
{
    u_long dummy, dummy2;

    _rpc_version = 0; // make sure its invalid now...    
    if ( !xdr_u_long(xdrs, &_xid) ||
    	 !xdr_u_long(xdrs, &dummy) ) {
    	return false;
    }
    _msg_type = (KsRpcMsgType) dummy;
    switch ( _msg_type ) {
    //
    // It's an incomming request (call). So we're usually on the server
    // side when we need do deserialize a call. We don't allow authentification
    // here, so the authentification flavour must be "none" and there must
    // not be any opaque data following it!
    //
    case KsRpcCALL:
    	if ( !xdr_u_long(xdrs, &_rpc_version)  ||
	     !xdr_u_long(xdrs, &_prog_number)  ||
	     !xdr_u_long(xdrs, &_prog_version) ||
	     !xdr_u_long(xdrs, &_procedure)    ||
	     (_rpc_version != 2) ) {
	    return false;
	}
    	if ( !xdr_u_long(xdrs, &dummy)  ||
	     !xdr_u_long(xdrs, &dummy2) ||
	     (dummy != KsRpcAUTHNONE)   || dummy2 ) {
	    return false;
	}
    	if ( !xdr_u_long(xdrs, &dummy)  ||
	     !xdr_u_long(xdrs, &dummy2) ||
	     (dummy != KsRpcAUTHNONE)   || dummy2 ) {
	    return false;
	}
    	return true;
    //
    // It's an incomming reply, so we're on the client side when deserializing
    // a reply.
    //
    case KsRpcREPLY:
    	if ( !xdr_u_long(xdrs, &dummy) ) {
	    return false;
	}
	_reply_status = (KsRpcReplyStat) dummy;
    	switch ( _reply_status ) {
	case KsRpcMSGACCEPTED:
    	    if ( !xdr_u_long(xdrs, &dummy)  ||
	    	 !xdr_u_long(xdrs, &dummy2) ||
	    	 (dummy != KsRpcAUTHNONE)   || dummy2 ) {
	    	return false;
	    }
	    if ( !xdr_u_long(xdrs, &dummy) ) {
	    	return false;
	    }
	    _accept_status = (KsRpcAcceptStat) dummy;
	    switch ( _accept_status ) {
	    case KsRpcSUCCESS:
	    case KsRpcPROGUNAVAIL:
	    case KsRpcPROCUNAVAIL:
	    case KsRpcGARBAGEARGS:
	    case KsRpcSYSTEMERR:
	    	return true;
	    case KsRpcPROGMISMATCH:
	    	return xdr_u_long(xdrs, &_low) &&
		       xdr_u_long(xdrs, &_high);
	    default:
	    	return false;
	    }
	case KsRpcMSGDENIED:
	    if ( !xdr_u_long(xdrs, &dummy) ) {
	    	return false;
	    }
	    _reject_status = (KsRpcRejectStat) dummy;
	    switch ( _reject_status ) {
	    case KsRpcRPCMISMATCH:
	    	return xdr_u_long(xdrs, &_low) &&
		       xdr_u_long(xdrs, &_high);
	    case KsRpcAUTHERROR:
	    	return xdr_u_long(xdrs, &_auth_error);
	    default:
	    	return false;
	    }
	default:
	    return false;
	}
    //
    // Every other message is just invalid!
    //
    default:
    	return false; // invalid RPC message type
    }
} // KsRpcHeader::xdrDecode


// ---------------------------------------------------------------------------
// Write out (encode) a RPC header to a XDR stream. NB: the pointer casts are
// driving me nuts... but they're needed because this member function is
// declared const and the function prototypes for the XDR functions have
// never heard of this, as they're in and out functions...
//
bool KsRpcHeader::xdrEncode(XDR *xdrs) const
{
    u_long dummy, dummy2;
    
    dummy = (u_long) _msg_type;
    if ( !xdr_u_long(xdrs, (u_long *) &_xid) ||
    	 !xdr_u_long(xdrs, (u_long *) &dummy) ) {
    	return false;
    }
    switch ( _msg_type ) {
    //
    // Send a request (call), thus we're probably on the client side of
    // the connection when we have to serialize (encode) a request.
    //
    case KsRpcCALL:
    	if ( (_rpc_version != 2)                          ||
	     !xdr_u_long(xdrs, (u_long *) &_rpc_version)  ||
	     !xdr_u_long(xdrs, (u_long *) &_prog_number)  ||
	     !xdr_u_long(xdrs, (u_long *) &_prog_version) ||
	     !xdr_u_long(xdrs, (u_long *) &_procedure) ) {
	    return false;
	}
	dummy  = KsRpcAUTHNONE;
	dummy2 = 0;
    	if ( !xdr_u_long(xdrs, &dummy)  ||
	     !xdr_u_long(xdrs, &dummy2) ||
             !xdr_u_long(xdrs, &dummy)  ||
	     !xdr_u_long(xdrs, &dummy2) ) {
	    return false;
	}
    	return true;
    //
    // Send a reply. We're on the server side...
    //
    case KsRpcREPLY:
    	dummy = _reply_status;
    	if ( !xdr_u_long(xdrs, &dummy) ) {
	    return false;
	}
    	switch ( _reply_status ) {
	case KsRpcMSGACCEPTED:
	    dummy  = KsRpcAUTHNONE;
	    dummy2 = 0;
	    if ( !xdr_u_long(xdrs, &dummy) ||
	         !xdr_u_long(xdrs, &dummy2) ) {
	    	return false;
    	    }
	    dummy = _accept_status;
	    if ( !xdr_u_long(xdrs, &dummy) ) {
	    	return false;
	    }
	    switch ( _accept_status ) {
	    case KsRpcSUCCESS:
	    case KsRpcPROGUNAVAIL:
	    case KsRpcPROCUNAVAIL:
	    case KsRpcGARBAGEARGS:
	    case KsRpcSYSTEMERR:
	    	return true;
	    case KsRpcPROGMISMATCH:
	    	return xdr_u_long(xdrs, (u_long *) &_low) &&
		       xdr_u_long(xdrs, (u_long *) &_high);
	    default:
	    	return false;
	    }
	case KsRpcMSGDENIED:
	    dummy = _reject_status;
	    if ( !xdr_u_long(xdrs, &dummy) ) {
	    	return false;
	    }
	    switch ( _reject_status ) {
	    case KsRpcRPCMISMATCH:
	    	return xdr_u_long(xdrs, (u_long *) &_low) &&
		       xdr_u_long(xdrs, (u_long *) &_high);
	    case KsRpcAUTHERROR:
	    	return xdr_u_long(xdrs, (u_long *) &_auth_error);
	    default:
	    	return false;
	    }
	default:
	    return false;
	}
    //
    // Every other message is just invalid!
    //
    default:
    	return false; // invalid RPC message type
    }
} // KsRpcHeader::xdrEncode


void KsRpcHeader::acceptCall()
{
    _msg_type      = KsRpcREPLY;
    _reply_status  = KsRpcMSGACCEPTED;
    _accept_status = KsRpcSUCCESS;
} // KsRpcHeader::acceptCall


void KsRpcHeader::setAuthRefusialError(u_long why)
{
    _msg_type      = KsRpcREPLY;
    _reply_status  = KsRpcMSGDENIED;
    _reject_status = KsRpcAUTHERROR;
    _auth_error    = why;
} // KsRpcHeader::setAuthRefusialError

void KsRpcHeader::setDecodeError()
{
    _msg_type      = KsRpcREPLY;
    _reply_status  = KsRpcMSGACCEPTED;
    _accept_status = KsRpcGARBAGEARGS;
} // KsRpcHeader::setDecodeError

void KsRpcHeader::setNoProcedureError()
{
    _msg_type      = KsRpcREPLY;
    _reply_status  = KsRpcMSGACCEPTED;
    _accept_status = KsRpcPROCUNAVAIL;
} // KsRpcHeader::setNoProcedureError

void KsRpcHeader::setNoProgrammError()
{
    _msg_type      = KsRpcREPLY;
    _reply_status  = KsRpcMSGACCEPTED;
    _accept_status = KsRpcPROGUNAVAIL;
} // KsRpcHeader::setNoProgrammError

void KsRpcHeader::setProgramVersionMismatchError(u_long low, u_long high)
{
    _msg_type      = KsRpcREPLY;
    _reply_status  = KsRpcMSGACCEPTED;
    _accept_status = KsRpcPROGMISMATCH;
    _low           = low;
    _high          = high;
} // KsRpcHeader::setProgramVersionMismatchError

void KsRpcHeader::setSystemError()
{
    _msg_type      = KsRpcREPLY;
    _reply_status  = KsRpcMSGACCEPTED;
    _accept_status = KsRpcSYSTEMERR;
} // KsRpcHeader::setSystemError

void KsRpcHeader::setRPCVersionMismatchError(u_long low, u_long high)
{
    _msg_type      = KsRpcREPLY;
    _reply_status  = KsRpcMSGDENIED;
    _reject_status = KsRpcRPCMISMATCH;
    _low           = low;
    _high          = high;
} // KsRpcHeader::setRPCVersionMismatchError


void KsRpcHeader::setRequest(u_long xid, 
			     u_long prog_number, u_long prog_version,
			     u_long proc_nr)
{
    _xid          = xid;
    _msg_type     = KsRpcCALL;
    _rpc_version  = 2;
    _prog_number  = prog_number;
    _prog_version = prog_version;
    _procedure    = proc_nr;
} // KsRpcHeader::setRequest


/* End of rpcproto.cpp */
