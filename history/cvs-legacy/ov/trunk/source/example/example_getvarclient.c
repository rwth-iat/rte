/*
*   $Id: example_getvarclient.c,v 1.6 2001-07-09 12:49:28 ansgar Exp $
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
*	04-May-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*	07-Jun-2001 J.Nagelmann <nagelmann@ltsoft.de>: Changes for Sun Solaris.
*/

#ifndef OV_COMPILE_LIBRARY_example
#define OV_COMPILE_LIBRARY_example
#endif

#include <stdarg.h>

#include "example_getvarclient.h"
#include "libov/ov_string.h"
#include "libov/ov_result.h"
#include "libov/ov_path.h"
#include "libov/ov_logfile.h"
#include "libov/ov_macros.h"

/*	----------------------------------------------------------------------	*/

/*
*	Print out a message (subroutine)
*/
void example_getvarclient_message(
	OV_INSTPTR_example_getvarclient	pgvc,
	const OV_STRING	format,
	...
) {
	/*
	*	local variables
	*/
	static char	msg[1024];
	va_list		args;
	/*
	*	create string containing the message
	*/
	va_start(args, format);
#if OV_SYSTEM_UNIX && !OV_SYSTEM_SOLARIS
	vsnprintf(msg, sizeof(msg), format, args);
#else
	vsprintf(msg, format, args);
#endif
	va_end(args);
	/*
	*	print out the message
	*/
	ov_memstack_lock();
	ov_logfile_info("%s: %s", ov_path_getcanonicalpath(
		Ov_PtrUpCast(ov_object, pgvc), 2), msg);
	ov_memstack_unlock();
}

/*	----------------------------------------------------------------------	*/

/*
*	Accessor functions
*/
OV_DLLFNCEXPORT OV_STRING example_getvarclient_host_get(
	OV_INSTPTR_example_getvarclient	pgvc
) {
	example_getvarclient_message(pgvc, "You asked for the hostname!");
	return pgvc->v_host;
}

OV_DLLFNCEXPORT OV_RESULT example_getvarclient_host_set(
	OV_INSTPTR_example_getvarclient	pgvc,
	const OV_STRING					value
) {
	example_getvarclient_message(pgvc, "You set the hostname!");
	ov_string_setvalue(&pgvc->v_host, value);
	/*
	*	delete the connection and the service object
	*/
	if(pgvc->v_psvc) {
		ov_ksclient_service_delete(pgvc->v_psvc);
		pgvc->v_psvc = NULL;
	}
	if(pgvc->v_pconn) {
		ov_ksclient_connection_delete(pgvc->v_pconn);
		pgvc->v_pconn = NULL;
	}
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_STRING example_getvarclient_server_get(
	OV_INSTPTR_example_getvarclient	pgvc
) {
	example_getvarclient_message(pgvc, "You asked for the servername!");
	return pgvc->v_server;
}

OV_DLLFNCEXPORT OV_RESULT example_getvarclient_server_set(
	OV_INSTPTR_example_getvarclient	pgvc,
	const OV_STRING					value
) {
	example_getvarclient_message(pgvc, "You set the servername!");
	ov_string_setvalue(&pgvc->v_server, value);
	/*
	*	delete the connection and the service object
	*/
	if(pgvc->v_psvc) {
		ov_ksclient_service_delete(pgvc->v_psvc);
		pgvc->v_psvc = NULL;
	}
	if(pgvc->v_pconn) {
		ov_ksclient_connection_delete(pgvc->v_pconn);
		pgvc->v_pconn = NULL;
	}
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_STRING example_getvarclient_path_get(
	OV_INSTPTR_example_getvarclient	pgvc
) {
	example_getvarclient_message(pgvc, "You asked for the variable path!");
	return pgvc->v_path;
}

OV_DLLFNCEXPORT OV_RESULT example_getvarclient_path_set(
	OV_INSTPTR_example_getvarclient	pgvc,
	const OV_STRING					value
) {
	example_getvarclient_message(pgvc, "You set the variable path!");
	ov_string_setvalue(&pgvc->v_path, value);
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
*	Start up the getvarclient object
*/
OV_DLLFNCEXPORT void example_getvarclient_startup(
	OV_INSTPTR_ov_object 	pobj
) {
	/*
	*	local variables
	*/
	OV_INSTPTR_example_getvarclient	pgvc = Ov_StaticPtrCast(example_getvarclient, pobj);
	/*
	*	do what the base class does first
	*/
	ov_object_startup(pobj);
	/*
	*	register the execute function with the scheduler
	*/
	Ov_WarnIfNot(Ov_OK(ov_scheduler_register(pobj, example_getvarclient_execute)));
	/*
	*	initialize the connection and the service object pointers
	*/
	pgvc->v_pconn = NULL;
	pgvc->v_psvc = NULL;
}

/*	----------------------------------------------------------------------	*/

/*
*	Shut down the getvarclient object
*/
OV_DLLFNCEXPORT void example_getvarclient_shutdown(
	OV_INSTPTR_ov_object 	pobj
) {
	/*
	*	local variables
	*/
	OV_INSTPTR_example_getvarclient	pgvc = Ov_StaticPtrCast(example_getvarclient, pobj);
	/*
	*	unregister the execute function with the scheduler
	*/
	ov_scheduler_unregister(pobj);
	/*
	*	delete the connection and the service object
	*/
	if(pgvc->v_psvc) {
		ov_ksclient_service_delete(pgvc->v_psvc);
		pgvc->v_psvc = NULL;
	}
	if(pgvc->v_pconn) {
		ov_ksclient_connection_delete(pgvc->v_pconn);
		pgvc->v_pconn = NULL;
	}
	/*
	*	finally do what the base class does
	*/	
	ov_object_shutdown(pobj);
}

/*	----------------------------------------------------------------------	*/

/*
*	Execute the active main method of the getvarclient object
*/
OV_DLLFNCEXPORT void example_getvarclient_execute(
	OV_INSTPTR_ov_object 	pobj
) {
	/*
	*	local variables
	*/
	OV_INSTPTR_example_getvarclient	pgvc = Ov_StaticPtrCast(example_getvarclient, pobj);
	OV_TIME_SPAN					timespan;
	/*
	*	reschedule in 5 seconds
	*/
	timespan.secs = 5;
	timespan.usecs = 0;
	ov_scheduler_setreleventtime(pobj, &timespan);
	/*
	*	create a connection object if none exists yet
	*/
	if(!pgvc->v_pconn) {
		pgvc->v_pconn = ov_ksclient_connection_create(pgvc->v_host,
			pgvc->v_server);
	}
	/*
	*	open the connection if it is closed and send a request if it is open
	*/
	if(pgvc->v_pconn) {
		switch(ov_ksclient_connection_getstate(pgvc->v_pconn)) {
		case OV_CCS_CLOSED:
			example_getvarclient_openconn(pgvc);
			break;
		case OV_CCS_OPEN:
			example_getvarclient_sendreq(pgvc);
			break;
		default:
			example_getvarclient_message(pgvc, "The connection is busy.");
			break;
		}
	} else {
		example_getvarclient_message(pgvc, "Could not create a connection :-(.");
	}
}

/*	----------------------------------------------------------------------	*/

/*
*	Open a connection
*/
void example_getvarclient_openconn(
	OV_INSTPTR_example_getvarclient	pgvc
) {
	example_getvarclient_message(pgvc, "Trying to open a connection...");
	ov_ksclient_connection_open(pgvc->v_pconn, example_getvarclient_opencb,
		pgvc);
}

/*	----------------------------------------------------------------------	*/

/*
*	Callback function after opening a connection
*/
void example_getvarclient_opencb(
	OV_KSCLIENT_CONNECTION			*pconn,
	OV_RESULT 						result,
	void							*userdata
) {
	/*
	*	local variables
	*/
	OV_INSTPTR_example_getvarclient	pgvc = Ov_StaticPtrCast(example_getvarclient, userdata);
	/*
	*	is the connection open or what?
	*/
	if(Ov_Fail(result)) {
		example_getvarclient_message(pgvc, "Could not open connection :-(.");
		return;
	}
	example_getvarclient_message(pgvc, "Connection open!");
	example_getvarclient_sendreq(pgvc);
}

/*	----------------------------------------------------------------------	*/

/*
*	Send a getvar request
*/
void example_getvarclient_sendreq(
	OV_INSTPTR_example_getvarclient	pgvc
) {
	example_getvarclient_message(pgvc, "Trying to send a request...");
	if(!pgvc->v_psvc) {
		pgvc->v_psvc = ov_ksclient_service_create(KS_GETVAR, NULL);
	}
	if(pgvc->v_psvc) {
		if(!pgvc->v_path) {
			ov_string_setvalue(&pgvc->v_path, "/");
		}
		pgvc->v_psvc->params.getvar.identifiers_len = 1;
		pgvc->v_psvc->params.getvar.identifiers_val = &pgvc->v_path;
		ov_ksclient_connection_sendrequest(pgvc->v_pconn, pgvc->v_psvc,
			example_getvarclient_sendreqcb, pgvc);
	} else {
		example_getvarclient_message(pgvc, "Could not create a service object :-(.");
	}
}

/*	----------------------------------------------------------------------	*/

/*
*	Callback function after sending a request
*/
void example_getvarclient_sendreqcb(
	OV_KSCLIENT_CONNECTION			*pconn,
	OV_RESULT 						result,
	OV_KSCLIENT_SERVICE				*psvc,
	void							*userdata
) {
	/*
	*	local variables
	*/
	OV_INSTPTR_example_getvarclient	pgvc = Ov_StaticPtrCast(example_getvarclient, userdata);
	/*
	*	did we get a result?
	*/
	if(Ov_Fail(result)) {
		example_getvarclient_message(pgvc, "Could not receive service reply :-(.");
		return;
	}
	example_getvarclient_message(pgvc, "Got reply!");
	if(psvc->result.getvar.result != KS_ERR_OK) {
		example_getvarclient_message(pgvc, "Could not receive service reply :-(.");
		goto EXIT;
	}
	if(psvc->result.getvar.items_val[0].result != KS_ERR_OK) {
		example_getvarclient_message(pgvc, "Could not read variable :-(. Reason: %s.", ov_result_getresulttext(
			psvc->result.getvar.items_val[0].result));
		goto EXIT;
	}
	switch(psvc->result.getvar.items_val[0].var_current_props.value.vartype) {
	case OV_VT_STRING:
		example_getvarclient_message(pgvc, "It's a string: \"%s\".", psvc->result.getvar.items_val[0].
			var_current_props.value.valueunion.val_string);
		break;
	default:
		example_getvarclient_message(pgvc, "I got a value, but I will only display string values...");
	}
	/*
	*	delete the service result for later reuse of the service object
	*/
EXIT:
	ov_ksclient_service_freeresult(psvc);
}

/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/

