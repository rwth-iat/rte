/** @file 
  * ks Logging capability.
 * Currently, this is a wrapper around the ov_logfile logging facility or normal printf.
 * As defined by precompiler it might ignore to print anything, which 
 * results in an performance boost
 * 
*/

#include "ks_logfile.h"
#ifdef OV
#include "libov/ov_logfile.h"
#endif
#include <stdarg.h>
#include <time.h>

#if LOG_KS || LOG_KS_INFO || LOG_KS_DEBUG || LOG_KS_WARNING || LOG_KS_ERROR
static char			msg[1024];
#endif

/**
  * Print info to logfile
  */
void ks_logfile_info(
	const char*	format,
	...
) {
#if LOG_KS || LOG_KS_INFO
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
#ifdef OV
	ov_logfile_info("%s | %s", str, msg);
#else
	printf("[Info] %s | %s", str, msg);
#endif
	va_end(args);
#endif
}

/**
*	Print debug info to logfile
*/
void ks_logfile_debug(
	const char*	format,
	...
) {
#if LOG_KS || LOG_KS_DEBUG
#ifndef OV
	time_t now;
	struct tm *ptr;
	char str[60];
#endif
	va_list	args;
	va_start(args, format);
#if OV_SYSTEM_UNIX && !OV_SYSTEM_SOLARIS
	vsnprintf(msg, sizeof(msg), format, args);
#else
	vsprintf(msg, format, args);
#endif
#ifdef OV
	ov_logfile_print(OV_MT_DEBUG, msg);
#else
	now = time(NULL);
	ptr = localtime(&now);
	strftime(str ,100 , "%H:%M.%S",ptr);
	printf("[Debug] %s | %s", str, msg);
#endif
	va_end(args);
#endif
}

/**
*	Print warning to logfile
*/
void ks_logfile_warning(
	const char*	format,
	...
) {
#if LOG_KS || LOG_KS_WARNING
#ifndef OV
	time_t now;
	struct tm *ptr;
	char str[60];
#endif
	va_list	args;
	va_start(args, format);
#if OV_SYSTEM_UNIX && !OV_SYSTEM_SOLARIS
	vsnprintf(msg, sizeof(msg), format, args);
#else
	vsprintf(msg, format, args);
#endif
#ifdef OV
	ov_logfile_print(OV_MT_WARNING, msg);
#else
	now = time(NULL);
	ptr = localtime(&now);
	strftime(str ,100 , "%H:%M.%S",ptr);
	printf("[Warning] %s | %s", str, msg);
#endif
	va_end(args);
#endif
}

/**
*	Print error to logfile
*/
void ks_logfile_error(
	const char*	format,
	...
) {
#if LOG_KS || LOG_KS_ERROR
#ifndef OV
	time_t now;
	struct tm *ptr;
	char str[60];
#endif
	va_list	args;
	va_start(args, format);
#if OV_SYSTEM_UNIX && !OV_SYSTEM_SOLARIS
	vsnprintf(msg, sizeof(msg), format, args);
#else
	vsprintf(msg, format, args);
#endif
#ifdef OV
	ov_logfile_print(OV_MT_ERROR, msg);
#else
	now = time(NULL);
	ptr = localtime(&now);
	strftime(str ,100 , "%H:%M.%S",ptr);
	printf("[Warning] %s | %s", str, msg);
#endif
	va_end(args);
#endif
}
