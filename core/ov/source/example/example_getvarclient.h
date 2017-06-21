/*
*   $Id: example_getvarclient.h,v 1.3 2004-05-19 14:49:14 ansgar Exp $
*
*   Copyright (C) 1998
*   Lehrstuhl fuer Prozessleittechnik,
*   D-52056 Aachen, Germany.
*   All rights reserved.
*
*   Author: Dirk Meyer <dirk@plt.rwth-aachen.de>
*
*   This file is part of the ACPLT/OV Package 
*   
*   Licensed under the Apache License, Version 2.0 (the "License");
*   you may not use this file except in compliance with the License.
*   You may obtain a copy of the License at
*
*       http://www.apache.org/licenses/LICENSE-2.0
*
*   Unless required by applicable law or agreed to in writing, software
*   distributed under the License is distributed on an "AS IS" BASIS,
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*   See the License for the specific language governing permissions and
*   limitations under the License.
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

