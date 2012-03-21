/** @file 
  * ksapi Logging capability.
 * Currently, this is a wrapper around the ov_logfile logging facility. 
 * As defined by precompiler it might ignore to print anything, which 
 * results in an performance boost
 * 
 * This is a intermediate solution until Log4ACPLT exists,
 * which will use this to create Log-Message objects inside the OV Server
*/

#include "ksapi_logfile.h"
#include "libov/ov_logfile.h"
#include <stdarg.h>
#include <time.h>

#if LOG_OV || LOG_OV_INFO
static char			msg[1024];
#endif
/**
*	Print info to logfile
*/
OV_DLLFNCEXPORT void ksapi_logfile_info(
	const OV_STRING	format,
	...
) {
#if LOG_OV || LOG_OV_INFO
	time_t now;
	char str[60];
	struct tm *ptr;
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
	ov_logfile_info("%s | %s", str, msg);
	va_end(args);
#endif
}

/**
*	Print debug info to logfile
*/
OV_DLLFNCEXPORT void ksapi_logfile_debug(
	const OV_STRING	format,
	...
) {
#if LOG_OV || LOG_OV_DEBUG
	va_list	args;
	va_start(args, format);
#if OV_SYSTEM_UNIX && !OV_SYSTEM_SOLARIS
	vsnprintf(msg, sizeof(msg), format, args);
#else
	vsprintf(msg, format, args);
#endif
	ov_logfile_print(OV_MT_DEBUG, msg);
	va_end(args);
#endif
}

/**
*	Print warning to logfile
*/
OV_DLLFNCEXPORT void ksapi_logfile_warning(
	const OV_STRING	format,
	...
) {
#if LOG_OV || LOG_OV_WARNING
	va_list	args;
	va_start(args, format);
#if OV_SYSTEM_UNIX && !OV_SYSTEM_SOLARIS
	vsnprintf(msg, sizeof(msg), format, args);
#else
	vsprintf(msg, format, args);
#endif
	ov_logfile_print(OV_MT_WARNING, msg);
	va_end(args);
#endif
}

/**
*	Print error to logfile
*/
OV_DLLFNCEXPORT void ksapi_logfile_error(
	const OV_STRING	format,
	...
) {
#if LOG_OV || LOG_OV_ERROR
	va_list	args;
	va_start(args, format);
#if OV_SYSTEM_UNIX && !OV_SYSTEM_SOLARIS
	vsnprintf(msg, sizeof(msg), format, args);
#else
	vsprintf(msg, format, args);
#endif
	ov_logfile_print(OV_MT_ERROR, msg);
	va_end(args);
#endif
}
