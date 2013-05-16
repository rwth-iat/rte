/** @file 
  * ks Logging capability.
 * Currently, this is a wrapper around printf.
 * As defined by precompiler it might ignore to print anything, which 
 * results in an performance boost
 * 
*/

#include "ks_logfile.h"
#include <stdarg.h>
#include <time.h>
#include <stdio.h>

#if LOG_KS || LOG_KS_INFO || LOG_KS_DEBUG || LOG_KS_WARNING || LOG_KS_ERROR
static char			msg[1024];
#endif

/**
  * Print info to logfile
  */
DLLFNCEXPORT void ks_logfile_info(
	const char*	format,
	...
) {
#if LOG_KS || LOG_KS_INFO || LOG_KS_COMPILE
	time_t now;
	struct tm *ptr;
	char str[60];
	va_list	args;
	va_start(args, format);
	now = time(NULL);
	ptr = localtime(&now);
	strftime(str ,100 , "%H:%M.%S",ptr);
#if OV_SYSTEM_UNIX && !OV_SYSTEM_SOLARIS
	vsnprintf(msg, sizeof(msg), format, args);
#else
	vsprintf(msg, format, args);
#endif
	fprintf(stdout, "[KS-Info] %s | %s\n", str, msg);
#if OV_SYSTEM_NT
/*
 *	added to handle output for eclipse consoles on windows
 * 	workaround for the bug https://bugs.eclipse.org/bugs/show_bug.cgi?id=173732
 *	we need to see whether this has performance issues
*/
	fflush(stdout);
#endif
	va_end(args);
#endif
}


/**
*	Print debug info to logfile
*/
DLLFNCEXPORT void ks_logfile_debug(
	const char*	format,
	...
) {
#if LOG_KS || LOG_KS_DEBUG || LOG_KS_COMPILE
	time_t now;
	struct tm *ptr;
	char str[60];
	va_list	args;
	va_start(args, format);
#if OV_SYSTEM_UNIX && !OV_SYSTEM_SOLARIS
	vsnprintf(msg, sizeof(msg), format, args);
#else
	vsprintf(msg, format, args);
#endif
	now = time(NULL);
	ptr = localtime(&now);
	strftime(str ,100 , "%H:%M.%S",ptr);
	fprintf(stdout, "[KS-Debug] %s | %s\n", str, msg);
#if OV_SYSTEM_NT
/*
 *	added to handle output for eclipse consoles on windows
 * 	workaround for the bug https://bugs.eclipse.org/bugs/show_bug.cgi?id=173732
 *	we need to see whether this has performance issues
*/
	fflush(stdout);
#endif
	va_end(args);
#endif
}

/**
*	Print warning to logfile
*/
DLLFNCEXPORT void ks_logfile_warning(
	const char*	format,
	...
) {
#if LOG_KS || LOG_KS_WARNING || LOG_KS_COMPILE
	time_t now;
	struct tm *ptr;
	char str[60];
	va_list	args;
	va_start(args, format);
#if OV_SYSTEM_UNIX && !OV_SYSTEM_SOLARIS
	vsnprintf(msg, sizeof(msg), format, args);
#else
	vsprintf(msg, format, args);
#endif
	now = time(NULL);
	ptr = localtime(&now);
	strftime(str ,100 , "%H:%M.%S",ptr);
	fprintf(stdout, "[KS-Warning] %s | %s\n", str, msg);
#if OV_SYSTEM_NT
/*
 *	added to handle output for eclipse consoles on windows
 * 	workaround for the bug https://bugs.eclipse.org/bugs/show_bug.cgi?id=173732
 *	we need to see whether this has performance issues
*/
	fflush(stdout);
#endif
	va_end(args);
#endif
}

/**
*	Print error to logfile
*/
DLLFNCEXPORT void ks_logfile_error(
	const char*	format,
	...
) {
#if LOG_KS || LOG_KS_ERROR || LOG_KS_COMPILE
	time_t now;
	struct tm *ptr;
	char str[60];
	va_list	args;
	va_start(args, format);
#if OV_SYSTEM_UNIX && !OV_SYSTEM_SOLARIS
	vsnprintf(msg, sizeof(msg), format, args);
#else
	vsprintf(msg, format, args);
#endif
	now = time(NULL);
	ptr = localtime(&now);
	strftime(str ,100 , "%H:%M.%S",ptr);
	fprintf(stdout, "[KS-Error] %s | %s\n", str, msg);
#if OV_SYSTEM_NT
/*
 *	added to handle output for eclipse consoles on windows
 * 	workaround for the bug https://bugs.eclipse.org/bugs/show_bug.cgi?id=173732
 *	we need to see whether this has performance issues
*/
	fflush(stdout);
#endif
	va_end(args);
#endif
}
