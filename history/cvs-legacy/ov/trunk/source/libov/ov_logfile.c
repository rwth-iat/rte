/*
*   $Id: ov_logfile.c,v 1.1 1999-07-19 15:02:13 dirk Exp $
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

#define OV_COMPILE_LIBOV

#include "libov/ov_logfile.h"

#include <stdarg.h>

/*	----------------------------------------------------------------------	*/

/*
*	Global variable: the logfile handle
*/
#if !OV_SYSTEM_MC164
static FILE			*logfile = NULL;
#endif

#if OV_SYSTEM_NT
static FILE 		ntlog;
static HANDLE		eventsource = NULL;
static char			msg[1024];
LPCTSTR				messages[1] = { (LPCTSTR)msg };
#endif

/*	----------------------------------------------------------------------	*/

/*
*	Open/create a logfile
*/
OV_RESULT OV_DLLFNCEXPORT ov_logfile_open(
	OV_STRING	filename,
	OV_STRING	mode
) {
#if !OV_SYSTEM_MC164
	/*
	*	test if logfile already open
	*/
	if(logfile) {
		ov_logfile_close();
	}
	logfile = stdout;
	/*
	*	open a new logfile
	*/
	logfile = fopen(filename, mode);
	if(!logfile) {
		return OV_ERR_CANTOPENFILE;
	}
#endif
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
*	Close the logfile
*/
void OV_DLLFNCEXPORT ov_logfile_close(void) {
#if !OV_SYSTEM_MC164
	if(logfile) {
		if((logfile != stdout) && (logfile != stderr)
#if OV_SYSTEM_NT
			&& (logfile != &ntlog)
#endif
		) {
			fclose(logfile);
		}
		logfile = NULL;
#if OV_SYSTEM_NT
		if(eventsource) {
			DeregisterEventSource(eventsource);
			eventsource = NULL;
		}
#endif
	}
#endif
}

/*	----------------------------------------------------------------------	*/

/*
*	Log to stdout
*/
void OV_DLLFNCEXPORT ov_logfile_logtostdout(void) {
#if !OV_SYSTEM_MC164
	if(logfile) {
		ov_logfile_close();
	}
	logfile = stdout;
#endif
}

/*	----------------------------------------------------------------------	*/

/*
*	Log to stderr
*/
void OV_DLLFNCEXPORT ov_logfile_logtostderr(void) {
#if !OV_SYSTEM_MC164
	if(logfile) {
		ov_logfile_close();
	}
	logfile = stderr;
#endif
}

/*	----------------------------------------------------------------------	*/

/*
*	Log to the NT logger (Windows NT only)
*/
#if OV_SYSTEM_NT
void OV_DLLFNCEXPORT ov_logfile_logtontlog(
	const OV_STRING	ident
) {
	if(logfile) {
		ov_logfile_close();
	}
	logfile = &ntlog;
	if(ident) {
		eventsource = RegisterEventSource(0, ident);
	} else {
		eventsource = RegisterEventSource(0, "ACPLT/OV");
	}
	if(!eventsource) {
		ov_logfile_close();
	}
}
#endif

/*	----------------------------------------------------------------------	*/

/*
*	Print text to logfile
*/
void OV_DLLFNCEXPORT ov_logfile_print(
	const OV_STRING	format,
	...
) {
#if !OV_SYSTEM_MC164
	/*
	*	local variables
	*/
	va_list	args;
	/*
	*	print text to logfile
	*/
	va_start(args, format);
#if OV_SYSTEM_NT
	if(logfile == &ntlog) {
		vsprintf(msg, format, args);
		ReportEvent(eventsource, EVENTLOG_INFORMATION_TYPE, 0, 0, 0,
			1, 0, messages, 0);
	}
#endif
	vfprintf(logfile, format, args);
#endif
}

/*	----------------------------------------------------------------------	*/

/*
*	Print info to logfile
*/
void OV_DLLFNCEXPORT ov_logfile_info(
	const OV_STRING	format,
	...
) {
#if !OV_SYSTEM_MC164
	/*
	*	local variables
	*/
	va_list	args;
	/*
	*	print text to logfile
	*/
	va_start(args, format);
#if OV_SYSTEM_NT
	if(logfile == &ntlog) {
		vsprintf(msg, format, args);
		ReportEvent(eventsource, EVENTLOG_INFORMATION_TYPE, 0, 0, 0,
			1, 0, messages, 0);
	}
#endif
	fprintf(logfile, "[ACPLT/OV Info] ");
	vfprintf(logfile, format, args);
	fprintf(logfile, "\n");
#endif
}

/*	----------------------------------------------------------------------	*/

/*
*	Print warning to logfile
*/
void OV_DLLFNCEXPORT ov_logfile_warning(
	const OV_STRING	format,
	...
) {
#if !OV_SYSTEM_MC164
	/*
	*	local variables
	*/
	va_list	args;
	/*
	*	print text to logfile
	*/
	va_start(args, format);
#if OV_SYSTEM_NT
	if(logfile == &ntlog) {
		vsprintf(msg, format, args);
		ReportEvent(eventsource, EVENTLOG_WARNING_TYPE, 0, 0, 0,
			1, 0, messages, 0);
	}
#endif
	fprintf(logfile, "[ACPLT/OV Warning] ");
	vfprintf(logfile, format, args);
	fprintf(logfile, "\n");
#endif
}

/*	----------------------------------------------------------------------	*/

/*
*	Print error to logfile
*/
void OV_DLLFNCEXPORT ov_logfile_error(
	const OV_STRING	format,
	...
) {
#if !OV_SYSTEM_MC164
	/*
	*	local variables
	*/
	va_list	args;
	/*
	*	print text to logfile
	*/
	va_start(args, format);
#if OV_SYSTEM_NT
	if(logfile == &ntlog) {
		vsprintf(msg, format, args);
		ReportEvent(eventsource, EVENTLOG_ERROR_TYPE, 0, 0, 0,
			1, 0, messages, 0);
	}
#endif
	fprintf(logfile, "[ACPLT/OV Error] ");
	vfprintf(logfile, format, args);
	fprintf(logfile, "\n");
#endif
}

/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/

