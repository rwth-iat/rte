/*
*   $Id: ov_ksserver.h,v 1.4 1999-09-15 10:48:16 dirk Exp $
*
*   Copyright (C) 1998-1999
*   Lehrstuhl fuer Prozessleittechnik,
*   RWTH Aachen, D-52056 Aachen, Germany.
*   All rights reserved.
*
*   Author: Dirk Meyer <dirk@plt.rwth-aachen.de>
*
*   This file is part of the ACPLT/OV Package which is licensed as open
*   source under the Artistic License; you can use, redistribute and/or
*   modify it under the terms of that license.
*
*   You should have received a copy of the Artistic License along with
*   this Package; see the file ARTISTIC-LICENSE. If not, write to the
*   Copyright Holder.
*
*   THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR IMPLIED
*   WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES
*   OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*/
/*
*	History:
*	--------
*	28-Jul-1998 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*	21-Apr-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: Major revision.
*/

#ifndef OV_KSSERVER_H_INCLUDED
#define OV_KSSERVER_H_INCLUDED

#include "libovks/ov_ksserver_xdr.h"
#include "libov/ov_ov.h"
#include "libov/ov_element.h"
#include "libov/ov_memstack.h"
#include "libov/ov_logfile.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
*	OV_FNC_SIGHANDLER:
*	------------------
*	Signal hander function prototype for server shutdown
*/
typedef OV_DLLFNCEXPORT void OV_FNC_SIGHANDLER(
	int signal
);

/*
*	Terminate an ACPLT/KS server (RMOS only)
*/
#if OV_SYSTEM_RMOS
OV_DLLFNCEXPORT OV_RESULT ov_ksserver_terminate(
	OV_UINT			taskid
);
#endif

/*
*	Create the ACPLT/KS server for ACPLT/OV
*/
OV_DLLFNCEXPORT OV_RESULT ov_ksserver_create(
	OV_STRING			servername,
	int					port,
	OV_FNC_SIGHANDLER	*sighandler
);

/*
*	Delete the ACPLT/KS server for ACPLKT/OV
*/
OV_DLLFNCEXPORT void ov_ksserver_delete(void);

/*
*	Start the ACPLT/KS server for ACPLT/OV
*/
OV_DLLFNCEXPORT void ov_ksserver_start(void);

/*
*	Run the ACPLT/KS server for ACPLKT/OV
*/
OV_DLLFNCEXPORT void ov_ksserver_run(void);

/*
*	Stop the ACPLT/KS server for ACPLKT/OV
*/
OV_DLLFNCEXPORT void ov_ksserver_stop(void);

/*
*	Tell the server to shut down
*/
OV_DLLFNCEXPORT void ov_ksserver_downserver(void);

/*
*	Test if the ACPLT/KS server for ACPLKT/OV is going down
*/
OV_DLLFNCEXPORT OV_BOOL ov_ksserver_isgoingdown(void);

/*
*	Test the ACPLT/KS server for ACPLKT/OV has pending I/O events
*/
OV_DLLFNCEXPORT OV_BOOL ov_ksserver_haspendingevents(void);

/*
*	Serve pending I/O events of the ACPLT/KS server for ACPLKT/OV
*/
OV_DLLFNCEXPORT OV_BOOL ov_ksserver_servependingevents(
	OV_TIME_SPAN	*ptimeout
);

/*
*	Dispatch a service (subroutine)
*/
void ov_ksserver_dispatch(
	OV_SVC		service,
	OV_UINT		version,
	OV_TICKET	*pticket,
	XDR			*xdrin,
	XDR			*xdrout
);

/*
*	Default signal handler for server shutdown (subroutine)
*/
OV_DLLFNCEXPORT void ov_ksserver_sighandler(
	int signal
);

/*
*	Send a service reply (subroutine)
*/
void ov_ksserver_sendreply(
	XDR			*xdrs,
	OV_TICKET	*pticket,
	OV_RESULT	*presult,
	xdrproc_t	encodefnc
);

/*
*	Execute the GetPP service (subroutine)
*/
void ov_ksserver_getpp(
	const OV_UINT		version,
	const OV_TICKET		*pticket,
	const OV_GETPP_PAR	*params,
	OV_GETPP_RES		*result
);

/*
*	Add an item to the list of projected properties (subroutine)
*/
OV_RESULT ov_ksserver_getpp_additem(
	const OV_UINT		version,
	const OV_TICKET		*pticket,
	OV_GETPP_RES		*result,
	const OV_ELEMENT	*pelem,
	const KS_OBJ_TYPE	typemask,
	const OV_STRING		namemask
);

/*
*	Execute the GetVar service (subroutine)
*/
void ov_ksserver_getvar(
	const OV_UINT		version,
	const OV_TICKET		*pticket,
	const OV_GETVAR_PAR	*params,
	OV_GETVAR_RES		*result
);

/*
*	Get a variable item of the GetVar service (subroutine)
*/
void ov_ksserver_getvar_getitem(
	const OV_UINT		version,
	const OV_TICKET		*pticket,
	OV_GETVAR_ITEM		*pitem,
	const OV_ELEMENT	*pelem
);

/*
*	Execute the SetVar service (subroutine)
*/
void ov_ksserver_setvar(
	const OV_UINT		version,
	const OV_TICKET		*pticket,
	const OV_SETVAR_PAR	*params,
	OV_SETVAR_RES		*result
);

/*
*	Set a variable item of the SetVar service (subroutine)
*/
OV_RESULT ov_ksserver_setvar_setitem(
	const OV_TICKET		*pticket,
	OV_SETVAR_ITEM		*pitem,
	const OV_ELEMENT	*pelem,
	OV_BOOL				init
);

/*
*	Execute the GetEP service (subroutine)
*/
void ov_ksserver_getep(
	const OV_UINT		version,
	const OV_TICKET		*pticket,
	const OV_GETEP_PAR	*params,
	OV_GETEP_RES		*result
);

/*
*	Add an item to the list of engineered properties (subroutine)
*/
OV_RESULT ov_ksserver_getep_additem(
	const OV_UINT		version,
	const OV_TICKET		*pticket,
	OV_GETEP_RES		*result,
	const OV_ELEMENT	*pelem,
	const KS_OBJ_TYPE	typemask,
	const OV_STRING		namemask
);

/*
*	Execute the Link service (subroutine)
*/
void ov_ksserver_link(
	const OV_UINT		version,
	const OV_TICKET		*pticket,
	const OV_LINK_PAR	*params,
	OV_LINK_RES			*result
);

/*
*	Link a link item of the Link service (subroutine)
*/
OV_RESULT ov_ksserver_link_linkitem(
	const OV_TICKET		*pticket,
	OV_ELEMENT			*plinkelem,
	OV_ELEMENT			*pelementelem,
	OV_ELEMENT			*pplaceelem,
	OV_ELEMENT			*poppositeplaceelem,
	OV_PLACEMENT_HINT	hint,
	OV_PLACEMENT_HINT	oppositehint
);

/*
*	Execute the DataExchange service (subroutine)
*/
void ov_ksserver_exgdata(
	const OV_UINT			version,
	const OV_TICKET			*pticket,
	const OV_EXGDATA_PAR	*params,
	OV_EXGDATA_RES			*result
);

/*
*	Execute the CreateObject service (subroutine)
*/
void ov_ksserver_createobject(
	const OV_UINT				version,
	const OV_TICKET				*pticket,
	const OV_CREATEOBJECT_PAR	*params,
	OV_CREATEOBJECT_RES			*result
);

/*
*	Initialize an object during instantiation in the CreateObject service
*/
OV_RESULT ov_ksserver_createobject_initobj(
	OV_INSTPTR_ov_object	pobj,
	OV_POINTER				pinfo
);

/*
*	Execute the DeleteObject service (subroutine)
*/
void ov_ksserver_deleteobject(
	const OV_UINT				version,
	const OV_TICKET				*pticket,
	const OV_DELETEOBJECT_PAR	*params,
	OV_DELETEOBJECT_RES			*result
);

/*
*	Execute the RenameObject service (subroutine)
*/
void ov_ksserver_renameobject(
	const OV_UINT				version,
	const OV_TICKET				*pticket,
	const OV_RENAMEOBJECT_PAR	*params,
	OV_RENAMEOBJECT_RES			*result
);

/*
*	Execute the GetCanonicalPath service (subroutine)
*/
void ov_ksserver_getcanonicalpath(
	const OV_UINT					version,
	const OV_TICKET					*pticket,
	const OV_GETCANONICALPATH_PAR	*params,
	OV_GETCANONICALPATH_RES			*result
);

/*
*	Execute the Unlink service (subroutine)
*/
void ov_ksserver_unlink(
	const OV_UINT		version,
	const OV_TICKET		*pticket,
	const OV_UNLINK_PAR	*params,
	OV_UNLINK_RES		*result
);

/*
*	Unlink an unlink item of the Unlink service (subroutine)
*/
OV_RESULT ov_ksserver_unlink_unlinkitem(
	const OV_TICKET		*pticket,
	OV_ELEMENT			*plinkelem,
	OV_ELEMENT			*pelementelem
);

/*
*	Execute the GetHist service (subroutine)
*/
void ov_ksserver_gethist(
	const OV_UINT			version,
	const OV_TICKET			*pticket,
	const OV_GETHIST_PAR	*params,
	OV_GETHIST_RES			*result
);

#ifdef __cplusplus
}
#endif

/*
*	C++ specific part for implementations using the C++ ACPLT/KS library
*	--------------------------------------------------------------------
*/

#if defined(OV_COMPILE_LIBOVKS) && defined(__cplusplus)

#include "plt/log.h"
#include "ks/server.h"

/*
*	Class OvPltLog
*/
class OvPltLog : public PltLog {
public:
    OvPltLog() {};
    ~OvPltLog() {};
    virtual void info(const char *msg) {
    	ov_logfile_info("ACPLT/KS: %s", msg);
    }
    virtual void debug(const char *msg) {
    	ov_logfile_debug("ACPLT/KS: %s", msg);
    }
    virtual void warning(const char *msg) {
    	ov_logfile_warning("ACPLT/KS: %s", msg);
    }
    virtual void error(const char *msg) {
    	ov_logfile_error("ACPLT/KS: %s", msg);
    }
    virtual void alert(const char *msg) {
    	ov_logfile_alert("ACPLT/KS: %s", msg);
    }
};

/*
*	Class OvKsAvTicket
*/
class OvKsAvTicket : public KsAvTicket {
public:
	// constructor
	OvKsAvTicket(XDR *xdrs) {
		ov_memstack_lock();
		if(ov_ksserver_xdr_OV_TICKET_PAR(xdrs, &_ticket)) {
			_result = OV_ERR_OK;
		} else {
			_result = OV_ERR_UNKNOWNAUTH;
		}
	}

	// accessors
	virtual enum_t xdrTypeCode() const { return _ticket.tickettype; }
	bool xdrDecodeVariant(XDR *) { return TRUE; }
	bool xdrEncodeVariant(XDR *) const { return TRUE; }
	virtual KS_RESULT result() const { return _result; }
	virtual bool canReadVar(const KsString &) const { return FALSE; }
	virtual bool canWriteVar(const KsString &) const { return FALSE; }
	OV_TICKET *ov_getTicket() { return &_ticket; }

protected:
	OV_RESULT	_result;
	OV_TICKET	_ticket;
};

/*
*	Class OvKsServer
*/
class OvKsServer : public KsServer {	
public:
	// constructor
	OvKsServer(OV_STRING servername, int port, OV_FNC_SIGHANDLER *sighandler)
	: KsServer(30, port), _servername(servername) {
		signal(SIGTERM, sighandler);
#if !PLT_SYSTEM_RMOS
		signal(SIGINT, sighandler);
#if !PLT_SYSTEM_NT
		signal(SIGHUP, sighandler);
#endif
#endif
	}

	// accessors
	virtual KsString getServerName() const { return KsString(_servername); }
	virtual KsString getServerDescription() const { return KsString(); }
	virtual KsString getServerVersion() const { return KsString(); }
	virtual KsString getVendorName () const { return KsString(); }

	// get A/V-Ticket
	KsAvTicket* getTicket(XDR* xdr) { return new OvKsAvTicket(xdr); }

	// set shutdown flag
	void setShutdownFlag(sig_atomic_t flag) { _shutdown_flag = flag; }

protected:
	// dispatch a service
	virtual void dispatch(u_long serviceId, KssTransport &transport,
		XDR *incomingXdr, KsAvTicket &ticket) {
		ov_ksserver_dispatch(serviceId, transport.getVersion(),
			(OV_TICKET*)&ticket, incomingXdr, (XDR*)&transport);
	}

private:
	OV_STRING _servername;
};

/*
*	Class OvKsResult
*/
class OvKsResult : public KsResult {	
public:
	// constructor
	OvKsResult(OV_POINTER result, xdrproc_t encodefnc)
		: _result(result), _encodefnc(encodefnc) {}

	// encode the result
	bool xdrEncode(XDR *xdr) const {
#ifdef OV_DEBUG
		if((_encodefnc)(xdr, _result)) {
			return TRUE;
		}
		Ov_Warning("error encoding request");
		return FALSE;
#else
		return (_encodefnc)(xdr, _result);
#endif
	}

protected:
	OV_POINTER	_result;
	xdrproc_t	_encodefnc;
};

#endif

#endif

/*
*	End of file
*/

