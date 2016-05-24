/*
*   $Id: ov_ksclient.h,v 1.9 2007-04-24 14:11:29 martin Exp $
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
*	26-Feb-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*	03-May-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: Mayor revision.
*/

#ifndef OV_KSCLIENT_H_INCLUDED
#define OV_KSCLIENT_H_INCLUDED

#include "libovks/ov_ksclient_xdr.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
*	OV_KSCLIENT_CONNECTION:
*	-----------------------
*	Client connection to an ACPLT/KS server used by ACPLT/OV as ACPLT/KS client.
*/
#if defined(OV_COMPILE_LIBOVKS) && defined(__cplusplus)
typedef class OvKssInterKsServerConnection	OV_KSCLIENT_CONNECTION;
#else
typedef void*								OV_KSCLIENT_CONNECTION;
#endif

/*
*	OV_KSCLIENT_SERVICE:
*	--------------------
*	Service used by ACPLT/OV as ACPLT/KS client (forward declaration).
*/
typedef struct OV_KSCLIENT_SERVICE OV_KSCLIENT_SERVICE;

/*
*	OV_KSCLIENT_CONNECTION_STATE:
*	-----------------------------
*	State of a client connection to an ACPLT/KS server.
*/
#define OV_CCS_CLOSED	0x00000001		/* connection is closed */
#define OV_CCS_OPEN		0x00000002		/* connection is open */
#define OV_CCS_BUSY		0x00000004		/* connection is busy */

typedef enum_t OV_KSCLIENT_CONNECTION_STATE;

/*
*	OV_FNC_KSCLIENT_OPENCALLBACK:
*	-----------------------------
*	Callback function used when asynchronously opening a connection to an
*	ACPLT/KS server.
*/
typedef void OV_FNC_KSCLIENT_OPENCALLBACK(
	OV_KSCLIENT_CONNECTION			*pconn,
	OV_RESULT 						result,
	OV_POINTER						userdata
);

/*
*	OV_FNC_KSCLIENT_REQUESTCALLBACK:
*	--------------------------------
*	Callback function used when asynchronously sending a request to an
*	ACPLT/KS server.
*/
typedef void OV_FNC_KSCLIENT_REQUESTCALLBACK(
	OV_KSCLIENT_CONNECTION			*pconn,
	OV_RESULT 						result,
	OV_KSCLIENT_SERVICE				*psvc,
	OV_POINTER						userdata
);

/*
*	OV_KSCLIENT_SERVICE:
*	--------------------
*	Service used by ACPLT/OV as ACPLT/KS client.
*/
struct OV_KSCLIENT_SERVICE {
	/* the service ID (e.g. KS_GETPP) */
	KS_SVC						serviceid;

	/* the service request parameters */
	union {
		KS_GETPP_PAR			getpp;
		KS_GETEP_PAR			getep;
		KS_GETVAR_PAR			getvar;
		KS_SETVAR_PAR			setvar;
		KS_EXGDATA_PAR			exgdata;
		KS_CREATEOBJECT_PAR		createobject;
		KS_DELETEOBJECT_PAR		deleteobject;
		KS_RENAMEOBJECT_PAR		renameobject;
		KS_LINK_PAR				link;
		KS_UNLINK_PAR			unlink;
		KS_GETCANONICALPATH_PAR	getcanonicalpath;
		KS_GETHIST_PAR			gethist;
	}	params;

	/* the result of the service request */
	union {
		KS_GETPP_RES			getpp;
		KS_GETEP_RES			getep;
		KS_GETVAR_RES			getvar;
		KS_SETVAR_RES			setvar;
		KS_EXGDATA_RES			exgdata;
		KS_CREATEOBJECT_RES		createobject;
		KS_DELETEOBJECT_RES		deleteobject;
		KS_RENAMEOBJECT_RES		renameobject;
		KS_LINK_RES				link;
		KS_UNLINK_RES			unlink;
		KS_GETCANONICALPATH_RES	getcanonicalpath;
		KS_GETHIST_RES			gethist;
	}	result;

	/* the A/V module used */
	KS_AVMODULE				*pavmodule;

	/* the associated connection, if the service is pending */
	OV_KSCLIENT_CONNECTION	*pconn;
};

/*
*	Create a client connection object
*/
OV_DLLFNCEXPORT OV_KSCLIENT_CONNECTION* ov_ksclient_connection_create(
	OV_STRING 						hostname,
	OV_STRING 						servername
);

/*
*	Delete an existing client connection object
*/
OV_DLLFNCEXPORT void ov_ksclient_connection_delete(
	OV_KSCLIENT_CONNECTION			*pconn
);

/*
*	Open a client connection to an ACPLT/KS server
*/
OV_DLLFNCEXPORT OV_RESULT ov_ksclient_connection_open(
	OV_KSCLIENT_CONNECTION			*pconn,
	OV_FNC_KSCLIENT_OPENCALLBACK	*callbackfnc,
	void							*userdata
);

/*
*	Close a client connection to an ACPLT/KS server
*/
OV_DLLFNCEXPORT void ov_ksclient_connection_close(
	OV_KSCLIENT_CONNECTION			*pconn
);

/*
*	Send a request to an ACPLT/KS server on an open client connection
*/
OV_DLLFNCEXPORT OV_RESULT ov_ksclient_connection_sendrequest(
	OV_KSCLIENT_CONNECTION			*pconn,
	OV_KSCLIENT_SERVICE				*psvc,
	OV_FNC_KSCLIENT_REQUESTCALLBACK	*callbackfnc,
	void							*userdata
);

/*
*	Get the current state of a client connection
*/
OV_DLLFNCEXPORT OV_KSCLIENT_CONNECTION_STATE ov_ksclient_connection_getstate(
	OV_KSCLIENT_CONNECTION			*pconn
);

/*
*	Set timeouts for a client connection
*/
void OV_DLLFNCEXPORT ov_ksclient_connection_settimeouts(
	OV_KSCLIENT_CONNECTION			*pconn,
	const OV_UINT					timeout_open,
	const OV_UINT					timeout_request
);

/*
*	Get timeouts for a client connection
*/
void OV_DLLFNCEXPORT ov_ksclient_connection_gettimeouts(
	OV_KSCLIENT_CONNECTION			*pconn,
	OV_UINT							*ptimeout_open,
	OV_UINT							*ptimeout_request
);

/*
*	Get protocol version of a connection
*/
OV_DLLFNCEXPORT OV_UINT ov_ksclient_connection_getprotocolversion(
	OV_KSCLIENT_CONNECTION		*pconn
);

/*
*	Create a new service object
*/
OV_DLLFNCEXPORT OV_KSCLIENT_SERVICE *ov_ksclient_service_create(
	KS_SVC				serviceid,
	KS_AVMODULE			*pavmodule
);

/*
*	Delete an existing service object
*/
OV_DLLFNCEXPORT void ov_ksclient_service_delete(
	OV_KSCLIENT_SERVICE	*psvc
);

/*
*	Free memory allocated for the result stored in a service object
*/
OV_DLLFNCEXPORT void ov_ksclient_service_freeresult(
	OV_KSCLIENT_SERVICE	*psvc
);

/*
*	Encode parameters of a service into an XDR stream (subroutine)
*/
OV_BOOL ov_ksclient_service_encodeparams(
	OV_KSCLIENT_SERVICE	*psvc,
	XDR					*xdrout	
);

/*
*	Decode the result of a service from an XDR stream (subroutine)
*/
OV_BOOL ov_ksclient_service_decoderesult(
	OV_KSCLIENT_SERVICE	*psvc,
	XDR					*xdrin
);

#ifdef __cplusplus
}	/* extern "C" */
#endif

/*
*	C++ specific part for implementations using the C++ ACPLT/KS library
*	--------------------------------------------------------------------
*/

#if defined(OV_COMPILE_LIBOVKS) && defined(__cplusplus)

#include "ks/interserver.h"

/*
*	Class OvKssInterKsServerConnection
*/
class OvKssInterKsServerConnection : public KssInterKsServerConnection {	
public:
	// constructor
	OvKssInterKsServerConnection(OV_STRING hostname, OV_STRING servername)
		: KssInterKsServerConnection(hostname, servername), _psvc(NULL)
		 {}

	// open a connection
	KS_RESULT connect(OV_FNC_KSCLIENT_OPENCALLBACK *callbackfnc, void *userdata);

	// close a connection
	void disconnect(void);

	// send a request
	KS_RESULT sendrequest(OV_KSCLIENT_SERVICE *psvc,
		OV_FNC_KSCLIENT_REQUESTCALLBACK *callbackfnc, void *userdata);

	// asynchronously pay attention (after connect or sendrequest)
	virtual void async_attention(KssInterKsServerConnectionOperations op);

private:
	OV_KSCLIENT_SERVICE				*_psvc;
	OV_FNC_KSCLIENT_OPENCALLBACK	*_opencallbackfnc;
	OV_FNC_KSCLIENT_REQUESTCALLBACK	*_sendrequestcallbackfnc;
	void							*_userdata;
};

#endif	/* __cplusplus */

#endif

/*
*	End of file
*/

