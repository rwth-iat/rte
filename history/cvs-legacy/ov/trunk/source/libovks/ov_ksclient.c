/*
*   $Id: ov_ksclient.c,v 1.9 2001-07-09 12:50:01 ansgar Exp $
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
*	12-Dec-2000 Dirk Meyer <dirk@plt.rwth-aachen.de>: Fixed a serious memory allocation bug.
*	09-Feb-2001 Ansgar Münnemann <ansgar@plt.rwth-aachen.de>: NULL check before use of callbackfnc.
*/

#define OV_COMPILE_LIBOVKS

#include "libovks/ov_ksclient.h"
#include "libov/ov_macros.h"

#if OV_SYSTEM_MC164
#define memset xmemset
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*	----------------------------------------------------------------------	*/

/*
*	Global variables: none-A/V-module
*/
static KS_AVMODULE	noneavmodule = { NULL, OV_TT_NONE };

/*	----------------------------------------------------------------------	*/

/*
*	Create a client connection object
*/
OV_DLLFNCEXPORT OV_KSCLIENT_CONNECTION* ov_ksclient_connection_create(
	OV_STRING 					hostname,
	OV_STRING 					servername
) {
	/*
	*	check parameters
	*/
	if(!hostname || !servername) {
		return NULL;
	}
	/*
	*	return new connection object
	*/
	return new OvKssInterKsServerConnection(hostname, servername);
}

/*	----------------------------------------------------------------------	*/

/*
*	Delete an existing client connection object
*/
OV_DLLFNCEXPORT void ov_ksclient_connection_delete(
	OV_KSCLIENT_CONNECTION		*pconn
) {
	if(pconn) {
		/*
		*	close the connection
		*/
		pconn->disconnect();
		/*
		*	delete connection object
		*/
		delete pconn;
	}
}

/*	----------------------------------------------------------------------	*/

/*
*	Open a client connection to an ACPLT/KS server
*/
OV_DLLFNCEXPORT OV_RESULT ov_ksclient_connection_open(
	OV_KSCLIENT_CONNECTION			*pconn,
	OV_FNC_KSCLIENT_OPENCALLBACK	*callbackfnc,
	void							*userdata
) {
	/*
	*	check parameters
	*/
	if(!pconn || !callbackfnc) {
		return OV_ERR_BADPARAM;
	}
	/*
	*	open the connection
	*/
	return pconn->connect(callbackfnc, userdata);
}

/*	----------------------------------------------------------------------	*/

/*
*	Close a client connection to an ACPLT/KS server
*/
OV_DLLFNCEXPORT void ov_ksclient_connection_close(
	OV_KSCLIENT_CONNECTION		*pconn
) {
	if(pconn) {
		/*
		*	close the connection
		*/
		pconn->disconnect();
	}
}

/*	----------------------------------------------------------------------	*/

/*
*	Send a request to an ACPLT/KS server on an open client connection
*/
OV_DLLFNCEXPORT OV_RESULT ov_ksclient_connection_sendrequest(
	OV_KSCLIENT_CONNECTION			*pconn,
	OV_KSCLIENT_SERVICE				*psvc,
	OV_FNC_KSCLIENT_REQUESTCALLBACK	*callbackfnc,
	void							*userdata
) {
	/*
	*	check parameters
	*/
	if(!pconn || !psvc || !callbackfnc) {
		return OV_ERR_BADPARAM;
	}
	/*
	*	send the request
	*/
	return pconn->sendrequest(psvc, callbackfnc, userdata);
}

/*	----------------------------------------------------------------------	*/

/*
*	Get the current state of a client connection
*/
OV_DLLFNCEXPORT OV_KSCLIENT_CONNECTION_STATE ov_ksclient_connection_getstate(
	OV_KSCLIENT_CONNECTION		*pconn
) {
	if(pconn) {
		switch(pconn->getState()) {
		case KssInterKsServerConnection::ISC_STATE_CLOSED:
			return OV_CCS_CLOSED;
		case KssInterKsServerConnection::ISC_STATE_OPEN:
			return OV_CCS_OPEN;
		case KssInterKsServerConnection::ISC_STATE_BUSY:
			return OV_CCS_BUSY;
		default:
			break;
		}
	}
	return OV_CCS_BUSY;
}

/*	----------------------------------------------------------------------	*/

/*
*	Set timeouts for a client connection
*/
OV_DLLFNCEXPORT void ov_ksclient_connection_settimeouts(
	OV_KSCLIENT_CONNECTION		*pconn,
	const OV_UINT				timeout_open,
	const OV_UINT				timeout_request
) {
	if(pconn) {
		pconn->setTimeouts(timeout_open, timeout_request);
	}
}

/*	----------------------------------------------------------------------	*/

/*
*	Get timeouts for a client connection
*/
OV_DLLFNCEXPORT void ov_ksclient_connection_gettimeouts(
	OV_KSCLIENT_CONNECTION		*pconn,
	OV_UINT						*ptimeout_open,
	OV_UINT						*ptimeout_request
) {
	if(pconn) {
		pconn->getTimeouts(*ptimeout_open, *ptimeout_request);
	}
}

/*	----------------------------------------------------------------------	*/

/*
*	Get protocol version of a connection
*/
OV_DLLFNCEXPORT OV_UINT ov_ksclient_connection_getprotocolversion(
	OV_KSCLIENT_CONNECTION		*pconn
) {
	if(pconn) {
		return pconn->getProtocolVersion();
	}
	return 0;	
}	

/*	----------------------------------------------------------------------	*/

/*
*	Create a new service object
*/
OV_DLLFNCEXPORT OV_KSCLIENT_SERVICE *ov_ksclient_service_create(
	KS_SVC				serviceid,
	KS_AVMODULE			*pavmodule
) {
	/*
	*	local variables
	*/
	OV_KSCLIENT_SERVICE	*psvc;
	/*
	*	create a new service object on the heap
	*/
	psvc = Ov_HeapAlloc(OV_KSCLIENT_SERVICE);
	/*
	*	initialize the service structure
	*/
	if(psvc) {
		memset(psvc, 0, sizeof(OV_KSCLIENT_SERVICE));
		psvc->serviceid = serviceid;
		if(pavmodule) {
			psvc->pavmodule = pavmodule;
		} else {
			psvc->pavmodule = &noneavmodule;
		}
	}
	return psvc;
}

/*	----------------------------------------------------------------------	*/

/*
*	Delete an existing service object
*/
OV_DLLFNCEXPORT void ov_ksclient_service_delete(
	OV_KSCLIENT_SERVICE	*psvc
) {
	if(psvc) {
		/*
		*	if the service is still pending, close the connection
		*/
		if(psvc->pconn) {
			ov_ksclient_connection_close(psvc->pconn);
		}
		/*
		*	free the result objects
		*/
		ov_ksclient_service_freeresult(psvc);
		/*
		*	finally free the service object itself
		*/
		Ov_HeapFree(psvc);
	}
}

/*	----------------------------------------------------------------------	*/

/*
*	Free memory allocated for the result stored in a service object
*/
OV_DLLFNCEXPORT void ov_ksclient_service_freeresult(
	OV_KSCLIENT_SERVICE	*psvc
) {
	/*
	*	local variables
	*/
	XDR	xdrdummy;	/* a faked XDR stream */
	/*
	*	check parameters
	*/
	if(!psvc) {
		return;
	}
	/*
	*	set dummy xdr-stream operation mode to "free"
	*/
	xdrdummy.x_op = XDR_FREE;
	/*
	*	free the reply object(s)
	*/
	switch(psvc->serviceid) {
	case KS_GETPP:
		ov_ksclient_xdr_KS_GETPP_RES(&xdrdummy, &psvc->result.getpp);
		break;
	case KS_GETEP:
		ov_ksclient_xdr_KS_GETEP_RES(&xdrdummy, &psvc->result.getep);
		break;
	case KS_GETVAR:
		ov_ksclient_xdr_KS_GETVAR_RES(&xdrdummy, &psvc->result.getvar);
		break;
	case KS_SETVAR:
		ov_ksclient_xdr_KS_SETVAR_RES(&xdrdummy, &psvc->result.setvar);
		break;
	case KS_EXGDATA:
		ov_ksclient_xdr_KS_EXGDATA_RES(&xdrdummy, &psvc->result.exgdata);
		break;
	case KS_CREATEOBJECT:
		ov_ksclient_xdr_KS_CREATEOBJECT_RES(&xdrdummy, &psvc->result.createobject);
		break;
	case KS_DELETEOBJECT:
		ov_ksclient_xdr_KS_DELETEOBJECT_RES(&xdrdummy, &psvc->result.deleteobject);
		break;
	case KS_RENAMEOBJECT:
		ov_ksclient_xdr_KS_RENAMEOBJECT_RES(&xdrdummy, &psvc->result.renameobject);
		break;
	case KS_LINK:
		ov_ksclient_xdr_KS_LINK_RES(&xdrdummy, &psvc->result.link);
		break;
	case KS_UNLINK:
		ov_ksclient_xdr_KS_UNLINK_RES(&xdrdummy, &psvc->result.unlink);
		break;
	case KS_GETCANONICALPATH:
		ov_ksclient_xdr_KS_GETCANONICALPATH_RES(&xdrdummy, &psvc->result.getcanonicalpath);
		break;
	case KS_GETHIST:
		ov_ksclient_xdr_KS_GETHIST_RES(&xdrdummy, &psvc->result.gethist);
		break;
	default:
		break;
	}
	memset(&psvc->result, 0, sizeof(psvc->result));
}

/*	----------------------------------------------------------------------	*/

/*
*	Encode parameters of a service into an XDR stream (subroutine)
*/
OV_BOOL ov_ksclient_service_encodeparams(
	OV_KSCLIENT_SERVICE	*psvc,
	XDR					*xdrout	
) {
	switch(psvc->serviceid) {
	case KS_GETPP:
		return ov_ksclient_xdr_KS_GETPP_PAR(xdrout, &psvc->params.getpp);
	case KS_GETEP:
		return ov_ksclient_xdr_KS_GETEP_PAR(xdrout, &psvc->params.getep);
	case KS_GETVAR:
		return ov_ksclient_xdr_KS_GETVAR_PAR(xdrout, &psvc->params.getvar);
	case KS_SETVAR:
		return ov_ksclient_xdr_KS_SETVAR_PAR(xdrout, &psvc->params.setvar);
	case KS_EXGDATA:
		return ov_ksclient_xdr_KS_EXGDATA_PAR(xdrout, &psvc->params.exgdata);
	case KS_CREATEOBJECT:
		return ov_ksclient_xdr_KS_CREATEOBJECT_PAR(xdrout, &psvc->params.createobject);
	case KS_DELETEOBJECT:
		return ov_ksclient_xdr_KS_DELETEOBJECT_PAR(xdrout, &psvc->params.deleteobject);
	case KS_RENAMEOBJECT:
		return ov_ksclient_xdr_KS_RENAMEOBJECT_PAR(xdrout, &psvc->params.renameobject);
	case KS_LINK:
		return ov_ksclient_xdr_KS_LINK_PAR(xdrout, &psvc->params.link);
	case KS_UNLINK:
		return ov_ksclient_xdr_KS_UNLINK_PAR(xdrout, &psvc->params.unlink);
	case KS_GETCANONICALPATH:
		return ov_ksclient_xdr_KS_GETCANONICALPATH_PAR(xdrout, &psvc->params.getcanonicalpath);
	case KS_GETHIST:
		return ov_ksclient_xdr_KS_GETHIST_PAR(xdrout, &psvc->params.gethist);
	default:
		break;
	}
	/*
	*	unknown service id
	*/
	return FALSE;
}

/*	----------------------------------------------------------------------	*/

/*
*	Decode the result of a service from an XDR stream (subroutine)
*/
OV_BOOL ov_ksclient_service_decoderesult(
	OV_KSCLIENT_SERVICE	*psvc,
	XDR					*xdrin
) {
	switch(psvc->serviceid) {
	case KS_GETPP:
		return ov_ksclient_xdr_KS_GETPP_RES(xdrin, &psvc->result.getpp);
	case KS_GETEP:
		return ov_ksclient_xdr_KS_GETEP_RES(xdrin, &psvc->result.getep);
	case KS_GETVAR:
		return ov_ksclient_xdr_KS_GETVAR_RES(xdrin, &psvc->result.getvar);
	case KS_SETVAR:
		return ov_ksclient_xdr_KS_SETVAR_RES(xdrin, &psvc->result.setvar);
	case KS_EXGDATA:
		return ov_ksclient_xdr_KS_EXGDATA_RES(xdrin, &psvc->result.exgdata);
	case KS_CREATEOBJECT:
		return ov_ksclient_xdr_KS_CREATEOBJECT_RES(xdrin, &psvc->result.createobject);
	case KS_DELETEOBJECT:
		return ov_ksclient_xdr_KS_DELETEOBJECT_RES(xdrin, &psvc->result.deleteobject);
	case KS_RENAMEOBJECT:
		return ov_ksclient_xdr_KS_RENAMEOBJECT_RES(xdrin, &psvc->result.renameobject);
	case KS_LINK:
		return ov_ksclient_xdr_KS_LINK_RES(xdrin, &psvc->result.link);
	case KS_UNLINK:
		return ov_ksclient_xdr_KS_UNLINK_RES(xdrin, &psvc->result.unlink);
	case KS_GETCANONICALPATH:
		return ov_ksclient_xdr_KS_GETCANONICALPATH_RES(xdrin, &psvc->result.getcanonicalpath);
	case KS_GETHIST:
		return ov_ksclient_xdr_KS_GETHIST_RES(xdrin, &psvc->result.gethist);
	default:
		break;
	}
	/*
	*	unknown service id
	*/
	return FALSE;
}

/*	----------------------------------------------------------------------	*/

#ifdef __cplusplus
}
#endif

/*
*	C++ specific part for implementations using the C++ ACPLT/KS library
*	--------------------------------------------------------------------
*/

#ifdef __cplusplus

/*
*	Class OvKssInterKsServerConnection: Open a connection
*/
KS_RESULT OvKssInterKsServerConnection::connect(
	OV_FNC_KSCLIENT_OPENCALLBACK	*callbackfnc,
	void							*userdata
) {
	/*
	*	local variables
	*/
	KS_RESULT	result;
	/*
	*	if the connection is not closed, quit with an error
	*/
	if(getState() != ISC_STATE_CLOSED) {
		return KS_ERR_GENERIC;
	}
	/*
	*	set the callback function and the user's data
	*/
	_sendrequestcallbackfnc = NULL;
	_opencallbackfnc = callbackfnc;
	_userdata = userdata;
	/*
	*	open the connection
	*/
	if(!open()) {
		result = getLastResult();
		if(result == KS_ERR_OK) {
			return KS_ERR_GENERIC;
		}
		return result;
	}
	/*
	*	reset the service object
	*/
	_psvc = NULL;
	/*
	*	we are done, connection will (hopefully) be open soon.
	*/
	return KS_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
*	Class OvKssInterKsServerConnection: close a connection
*/
void OvKssInterKsServerConnection::disconnect(void) {
	/*
	*	if there is a pending service, call the user's sendrequest-callback
	*	function with a KS_ERR_GENERIC result.
	*/
	if(_psvc) {
		if (_sendrequestcallbackfnc) {
			_sendrequestcallbackfnc(this, KS_ERR_GENERIC, _psvc, _userdata);
			_sendrequestcallbackfnc = NULL;
		}
		_psvc->pconn = NULL;
	}
	/*
	*	reset the service object ptr
	*/
	_psvc = NULL;
	/*
	*	close the connection
	*/
	close();
}

/*	----------------------------------------------------------------------	*/

/*
*	Class OvKssInterKsServerConnection: send a request
*/
KS_RESULT OvKssInterKsServerConnection::sendrequest(
	OV_KSCLIENT_SERVICE				*psvc,
	OV_FNC_KSCLIENT_REQUESTCALLBACK	*callbackfnc,
	void							*userdata
) {
	/*
	*	local variables
	*/
	bool_t		ok;
	XDR			*xdr;
	/*
	*	if the connection is not open, quit with an error
	*/
	if(getState() != ISC_STATE_OPEN) {
		return KS_ERR_GENERIC;
	}
	/*
	*	set the service structure, the callback function and the user's data
	*/
	_psvc = psvc;
	_sendrequestcallbackfnc = callbackfnc;
	_userdata = userdata;
	/*
	*	send the request
	*/
	xdr = getXdr();
	if(!beginSend(psvc->serviceid)) {
		return getLastResult();
	}
	ok = ov_ksclient_xdr_KS_AVMODULE_PAR(xdr, psvc->pavmodule);
	if(!ok) {
		return KS_ERR_GENERIC;
	}
	if(!ov_ksclient_service_encodeparams(psvc, xdr)) {
		return KS_ERR_GENERIC;
	}
	if(!endSend()) {
		return getLastResult();
	}
	psvc->pconn = this;
	/*
	*	we are done, the result will (hopefully) be here soon.
	*/
	return KS_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
*	Class OvKssInterKsServerConnection: asynchronously pay attention
*/
void OvKssInterKsServerConnection::async_attention(
	KssInterKsServerConnectionOperations op
) {
	/*
	*	local variables
	*/
	KS_RESULT	result;
	/*
	*	to which operation do we have to pay attention?
	*/
	switch(op) {
	case ISC_OP_OPEN:
		/*
		*	a ov_ksclient_connection_open() was completed in the background,
		*	call the user's callback function.
		*/
		if(getState() == ISC_STATE_OPEN) {
			result = KS_ERR_OK;
		} else {
			result = getLastResult();
			if(result == KS_ERR_OK) {
				result = KS_ERR_GENERIC;
			}
		}
		/* call the user's callback function */
		if (_opencallbackfnc) {
			_opencallbackfnc(this, result, _userdata);
			_opencallbackfnc = NULL;
		}
		return;

	case ISC_OP_CALL: {
		/*
		*	a ov_ksclient_connection_sendrequest() was completed in the
		*	background. Now try to decode the reply and call the user's
		*	callback function.
		*/
		OV_KSCLIENT_SERVICE	*psvc = _psvc;
		KS_AVMODULE			avresult;
		bool_t				ok;
		XDR					*xdr;
		/*
		*	check for errors
		*/
		if (psvc) {
			/*
			*	reset the connection ptr of the service object and the service object ptr
			*/
			_psvc->pconn = NULL;
			_psvc = NULL;
			result = getLastResult();
			if(result != KS_ERR_OK) {
				/* close the connection and call callback fnc */
				close();
				if (_sendrequestcallbackfnc) {
					_sendrequestcallbackfnc(this, result, psvc, _userdata);
					_sendrequestcallbackfnc = NULL;
				}
				return;
			}
			/*
			*	receive the reply
			*/
			xdr = getXdr();
			if(!beginReceive()) {
				if (_sendrequestcallbackfnc) {
					_sendrequestcallbackfnc(this, getLastResult(), psvc, _userdata);
					_sendrequestcallbackfnc = NULL;
				}
				return;
			}
			ok = ov_ksclient_xdr_KS_AVMODULE_RES(xdr, &avresult);
			if(ok) {
				if(avresult.type == OV_TT_NONE) {
					psvc->pavmodule->type = OV_TT_NONE;
				} else {
					if(avresult.type != psvc->pavmodule->type) {
						ok = FALSE;
					}
				}
			}
			if(!ok) {
				if (_sendrequestcallbackfnc) {
					_sendrequestcallbackfnc(this, KS_ERR_GENERIC, psvc, _userdata);
					_sendrequestcallbackfnc = NULL;
				}
				return;
			}
			if(!ov_ksclient_service_decoderesult(psvc, xdr)) {
				if (_sendrequestcallbackfnc) {
					_sendrequestcallbackfnc(this, KS_ERR_GENERIC, psvc, _userdata);
					_sendrequestcallbackfnc = NULL;
				}
				return;
			}
			if(!endReceive()) {
				if (_sendrequestcallbackfnc) {
				_sendrequestcallbackfnc(this, getLastResult(), psvc, _userdata);
					_sendrequestcallbackfnc = NULL;
				}
				return;
			}
			if (_sendrequestcallbackfnc) {
				_sendrequestcallbackfnc(this, result, psvc, _userdata);
				_sendrequestcallbackfnc = NULL;
			}
		}
		else Ov_Warning("attention operation encountered in an incorrect way");
		return;
	}

	default:
		break;
	}
	Ov_Warning("unknown attention operation encountered");
}

/*	----------------------------------------------------------------------	*/

#endif

/*
*	End of file
*/

