/*
*   $Id: ov_logfile.c,v 1.4 1999-08-28 15:55:54 dirk Exp $
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
#include "libov/ov_time.h"
#include "libov/ov_macros.h"

#include <stdarg.h>

#if OV_SYSTEM_MC164
#define memset xmemset
#endif

/*	----------------------------------------------------------------------	*/

/*
*	Constants
*/
#if OV_SYSTEM_NT
#define OV_REGISTRY_KEY		"SYSTEM\\CurrentControlSet\\Services\\EventLog\\Application\\"
#define OV_REGISTRY_NAME1	"EventMessageFile"
#define OV_REGISTRY_VALUE1	"%SystemRoot%\\system32\\libov.dll"
#define OV_REGISTRY_NAME2	"TypesSupported"
#define OV_REGISTRY_VALUE2	EVENTLOG_INFORMATION_TYPE | EVENTLOG_WARNING_TYPE | EVENTLOG_ERROR_TYPE
#endif

/*	----------------------------------------------------------------------	*/

/*
*	Global variables
*/
static char			msg[1024];

static OV_UINT		bufidx = OV_LOGFILE_MAXMSGS;
static OV_STRING	msgbuf[OV_LOGFILE_MAXMSGS];
static OV_TIME		timebuf[OV_LOGFILE_MAXMSGS];
static OV_STRING	id = NULL;

#if !OV_SYSTEM_MC164
static FILE			*logfile = NULL;
#endif

#if OV_SYSTEM_NT
static FILE 		ntlog;
static HANDLE		eventsource = NULL;
#endif

/*	----------------------------------------------------------------------	*/

/*
*	Open/create a logfile
*/
OV_RESULT OV_DLLFNCEXPORT ov_logfile_open(
	const OV_STRING	ident,
	OV_STRING		filename,
	OV_STRING		mode
) {
	id = ident;
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
void OV_DLLFNCEXPORT ov_logfile_logtostdout(
	const OV_STRING	ident
) {
	id = ident;
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
void OV_DLLFNCEXPORT ov_logfile_logtostderr(
	const OV_STRING	ident
) {
	id = ident;
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
	/*
	*	local variables
	*/
	HKEY 		hkey;
	DWORD		value2 = OV_REGISTRY_VALUE2;
	OV_STRING	key;
	OV_UINT		size;
	/*
	*	register the message file in the registry
	*/
	size = strlen(OV_REGISTRY_KEY)+((id)?(strlen(id)):(strlen("ACPLT/OV")))+1;
	key = (OV_STRING)Ov_HeapMalloc(size);
	if(key) {
		sprintf(key, "%s%s", OV_REGISTRY_KEY, ((id)?(id):("ACPLT/OV")));
		if(RegCreateKey(HKEY_LOCAL_MACHINE, key, &hkey) == ERROR_SUCCESS) {
			RegSetValueEx(hkey, OV_REGISTRY_NAME1, 0, REG_EXPAND_SZ,
				(LPBYTE)OV_REGISTRY_VALUE1, strlen(OV_REGISTRY_VALUE1)+1);
			RegSetValueEx(hkey, OV_REGISTRY_NAME2, 0, REG_DWORD,
				(LPBYTE)&value2, sizeof(DWORD));
			RegCloseKey(hkey);
		}
		Ov_HeapFree(key);
	}
	/*
	*	create event source
	*/
	id = ident;
	if(logfile) {
		ov_logfile_close();
	}
	logfile = &ntlog;
	eventsource = RegisterEventSource(0, ((id)?(id):("ACPLT/OV")));
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
	OV_MSG_TYPE		msgtype,
	const OV_STRING	msg
) {
	/*
	*	local variables
	*/
	OV_STRING	ident;
	OV_STRING	typetext;
	OV_STRING	output;
	/*
	*	create full message
	*/
	if(id) {
		ident = id;
	} else {
		ident = "ACPLT/OV";
	}
	switch(msgtype) {
	case OV_MT_INFO:
		typetext = "Info";
		break;
	case OV_MT_DEBUG:
		typetext = "Debug";
		break;
	case OV_MT_WARNING:
		typetext = "Warning";
		break;
	case OV_MT_ERROR:
		typetext = "Error";
		break;
	case OV_MT_ALERT:
		typetext = "Alert";
		break;
	default:
		typetext = "(unknown message type)";
		break;
	}
	output = (OV_STRING)Ov_HeapMalloc(strlen(ident)+strlen(typetext)+strlen(msg)+5);
	if(!output) {
		return;
	}
	sprintf(output, "[%s %s] %s", ident, typetext, msg);
	/*
	*	enter message into the buffer
	*/
	if(bufidx == OV_LOGFILE_MAXMSGS) {
		/*
		*	initialize the buffer
		*/
		memset(msgbuf, 0, sizeof(msgbuf));
		bufidx = 0;
	}
	msgbuf[bufidx] = (OV_STRING)Ov_HeapRealloc(msgbuf[bufidx], strlen(output)+1);
	strcpy(msgbuf[bufidx], output);
	ov_time_gettime(&timebuf[bufidx]);
	bufidx++;
	if(bufidx == OV_LOGFILE_MAXMSGS) {
		bufidx = 0;
	}
#if OV_SYSTEM_NT
	if(logfile == &ntlog) {
		LPCTSTR	messages[1];
		messages[0] = (LPCTSTR)output;
		/*
		*	report the message to the NT Logger
		*/
		switch(msgtype) {
		case OV_MT_WARNING:
			ReportEvent(eventsource, EVENTLOG_WARNING_TYPE,
				0, msgtype, 0, 1, 0, messages, 0);
			Ov_HeapFree(output);
			return;
		case OV_MT_ERROR:
			ReportEvent(eventsource, EVENTLOG_ERROR_TYPE,
				0, msgtype, 0, 1, 0, messages, 0);
			Ov_HeapFree(output);
			return;
		case OV_MT_INFO:
		case OV_MT_DEBUG:
		case OV_MT_ALERT:
		default:
			ReportEvent(eventsource, EVENTLOG_INFORMATION_TYPE,
				0, msgtype, 0, 1, 0, messages, 0);
			Ov_HeapFree(output);
			return;
		}
	}
#endif
#if !OV_SYSTEM_MC164
	/*
	*	print the message to the log file
	*/
	if(logfile) {
		fprintf(logfile, "%s\n", output);
	}
#endif
	Ov_HeapFree(output);
}

/*	----------------------------------------------------------------------	*/

/*
*	Print info to logfile
*/
void OV_DLLFNCEXPORT ov_logfile_info(
	const OV_STRING	format,
	...
) {
	/*
	*	local variables
	*/
	va_list	args;
	/*
	*	print text to logfile
	*/
	va_start(args, format);
#if OV_SYSTEM_UNIX
	vsnprintf(msg, sizeof(msg), format, args);
#else
	vsprintf(msg, format, args);
#endif
	va_end(args);
	ov_logfile_print(OV_MT_INFO, msg);
}

/*	----------------------------------------------------------------------	*/

/*
*	Print debug info to logfile
*/
void OV_DLLFNCEXPORT ov_logfile_debug(
	const OV_STRING	format,
	...
) {
	/*
	*	local variables
	*/
	va_list	args;
	/*
	*	print text to logfile
	*/
	va_start(args, format);
#if OV_SYSTEM_UNIX
	vsnprintf(msg, sizeof(msg), format, args);
#else
	vsprintf(msg, format, args);
#endif
	va_end(args);
	ov_logfile_print(OV_MT_DEBUG, msg);
}

/*	----------------------------------------------------------------------	*/

/*
*	Print warning to logfile
*/
void OV_DLLFNCEXPORT ov_logfile_warning(
	const OV_STRING	format,
	...
) {
	/*
	*	local variables
	*/
	va_list	args;
	/*
	*	print text to logfile
	*/
	va_start(args, format);
#if OV_SYSTEM_UNIX
	vsnprintf(msg, sizeof(msg), format, args);
#else
	vsprintf(msg, format, args);
#endif
	va_end(args);
	ov_logfile_print(OV_MT_WARNING, msg);
}

/*	----------------------------------------------------------------------	*/

/*
*	Print error to logfile
*/
void OV_DLLFNCEXPORT ov_logfile_error(
	const OV_STRING	format,
	...
) {
	/*
	*	local variables
	*/
	va_list	args;
	/*
	*	print text to logfile
	*/
	va_start(args, format);
#if OV_SYSTEM_UNIX
	vsnprintf(msg, sizeof(msg), format, args);
#else
	vsprintf(msg, format, args);
#endif
	va_end(args);
	ov_logfile_print(OV_MT_ERROR, msg);
}

/*	----------------------------------------------------------------------	*/

/*
*	Print alert to logfile
*/
void OV_DLLFNCEXPORT ov_logfile_alert(
	const OV_STRING	format,
	...
) {
	/*
	*	local variables
	*/
	va_list	args;
	/*
	*	print text to logfile
	*/
	va_start(args, format);
#if OV_SYSTEM_UNIX
	vsnprintf(msg, sizeof(msg), format, args);
#else
	vsprintf(msg, format, args);
#endif
	va_end(args);
	ov_logfile_print(OV_MT_ALERT, msg);
}

/*	----------------------------------------------------------------------	*/

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
) {
	/*
	*	local variables
	*/
	OV_UINT		i, j, k, count;
	OV_BOOL		mark[OV_LOGFILE_MAXMSGS];
	OV_INT		step;
	OV_UINT		start;
	/*
	*	determine whether to scan from oldest to newst messages
	*	or vice versa
	*/
	step = ov_time_compare(to, from);
	if(!step) {
		step = 1;
	}
	if(step > 0) {
		start = bufidx;
	} else {
		if(bufidx) {
			start = bufidx-1;
		} else {
			start = OV_LOGFILE_MAXMSGS-1;
		}
	}
	/*
	*	count and mark the messages
	*/
	for(i=0, k=start, count=0; (i<OV_LOGFILE_MAXMSGS) && (count<max_no_messages); i++) {
		mark[k] = FALSE;
		if(msgbuf[k]) {
			if((step*ov_time_compare(&timebuf[k], from) >= 0)
				&& (step*ov_time_compare(&timebuf[k], to) <= 0)
			) {
				mark[k] = TRUE;
				count++;
			}
		}
		if(step > 0) {
			if(k == OV_LOGFILE_MAXMSGS-1) {
				k = 0;
			} else {
				k++;
			}
		} else {
			if(k == 0) {
				k = OV_LOGFILE_MAXMSGS-1;
			} else {
				k--;
			}
		}
	}
	/*
	*	allocate memory for the result
	*/
	if(messages) {
		*messages = (OV_STRING*)ov_memstack_alloc(count*sizeof(OV_STRING));
		if(!*messages) {
			return OV_ERR_HEAPOUTOFMEMORY;
		}
	}
	if(times) {
		*times = (OV_TIME*)ov_memstack_alloc(count*sizeof(OV_TIME));
		if(!*times) {
			return OV_ERR_HEAPOUTOFMEMORY;
		}
	}
	/*
	*	enter the results
	*/
	*no_messages = count;
	for(i=0, j=0, k=start; (i<OV_LOGFILE_MAXMSGS) && (j<count); i++) {
		if(mark[k]) {
			if(messages) {
				(*messages)[j] = msgbuf[k];
			}
			if(times) {
				(*times)[j] = timebuf[k];
			}
			j++;
		}
		if(step > 0) {
			if(k == OV_LOGFILE_MAXMSGS-1) {
				k = 0;
			} else {
				k++;
			}
		} else {
			if(k == 0) {
				k = OV_LOGFILE_MAXMSGS-1;
			} else {
				k--;
			}
		}
	}
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/

