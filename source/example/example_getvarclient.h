/*
*   $Id: example_getvarclient.h,v 1.3 2004-05-19 14:49:14 ansgar Exp $
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
*/

#ifndef EXAMPLE_GETVARCLIENT_H_INCLUDED
#define EXAMPLE_GETVARCLIENT_H_INCLUDED

#include "example.h"
#include "libov/ov_scheduler.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
*	Execute function
*/
OV_DLLFNCEXPORT void example_getvarclient_execute(
	OV_INSTPTR_ov_object 	pobj
);


/*
*	Open a connection
*/
void example_getvarclient_openconn(
	OV_INSTPTR_example_getvarclient	pgvc
);

/*
*	Callback function after opening a connection
*/
void example_getvarclient_opencb(
	OV_KSCLIENT_CONNECTION			*pconn,
	OV_RESULT 						result,
	void							*userdata
);

/*
*	Send a getvar request
*/
void example_getvarclient_sendreq(
	OV_INSTPTR_example_getvarclient	pgvc
);

/*
*	Callback function after sending a request
*/
void example_getvarclient_sendreqcb(
	OV_KSCLIENT_CONNECTION			*pconn,
	OV_RESULT 						result,
	OV_KSCLIENT_SERVICE				*psvc,
	void							*userdata
);

#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif
/*
*	End of file
*/

