/*
*   $Id: ov_logfile.h,v 1.2 1999-07-26 16:14:07 dirk Exp $
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
*	14-May-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*/

#ifndef OV_LOGFILE_H_INCLUDED
#define OV_LOGFILE_H_INCLUDED

#include "libov/ov_ov.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
*	Maximum number of messages to keep for ov_logfile_getmessages
*/
#define OV_LOGFILE_MAXMSGS	100

/*
*	Open/create a logfile
*/
OV_RESULT OV_DLLFNCEXPORT ov_logfile_open(
	const OV_STRING	ident,
	OV_STRING	filename,
	OV_STRING	mode
);

/*
*	Close the logfile
*/
void OV_DLLFNCEXPORT ov_logfile_close(void);

/*
*	Log to stdout
*/
void OV_DLLFNCEXPORT ov_logfile_logtostdout(
	const OV_STRING	ident
);

/*
*	Log to stderr
*/
void OV_DLLFNCEXPORT ov_logfile_logtostderr(
	const OV_STRING	ident
);

/*
*	Log to the NT logger (Windows NT only)
*/
#if OV_SYSTEM_NT
void OV_DLLFNCEXPORT ov_logfile_logtontlog(
	const OV_STRING	ident
);
#endif

/*
*	Print text to logfile
*/
void OV_DLLFNCEXPORT ov_logfile_print(
	OV_MSG_TYPE		msgtype,
	const OV_STRING	msg
);

/*
*	Print info to logfile
*/
void OV_DLLFNCEXPORT ov_logfile_info(
	const OV_STRING	format,
	...
);

/*
*	Print debug info to logfile
*/
void OV_DLLFNCEXPORT ov_logfile_debug(
	const OV_STRING	format,
	...
);

/*
*	Print warning to logfile
*/
void OV_DLLFNCEXPORT ov_logfile_warning(
	const OV_STRING	format,
	...
);

/*
*	Print error to logfile
*/
void OV_DLLFNCEXPORT ov_logfile_error(
	const OV_STRING	format,
	...
);

/*
*	Print alert to logfile
*/
void OV_DLLFNCEXPORT ov_logfile_alert(
	const OV_STRING	format,
	...
);

/*
*	Get messages from the logfile
*	Note: you must call ov_memstack_lock() and ov_memstack_unlock() outside
*/
OV_RESULT OV_DLLFNCEXPORT ov_logfile_getmessages(
	OV_TIME		*from,
	OV_TIME		*to,
	OV_UINT		max_no_messages,
	OV_STRING	**messages,
	OV_TIME		**times,
	OV_UINT		*no_messages
);

#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif
/*
*	End of file
*/

