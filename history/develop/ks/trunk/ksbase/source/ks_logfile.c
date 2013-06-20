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

#if OV_SYSTEM_NT
#include <windows.h>
#else
#include <errno.h>
#include <string.h>
#endif

#if LOG_KS_TO_OV
#include "libov/ov_logfile.h"
#endif

#if LOG_KS || LOG_KS_INFO || LOG_KS_DEBUG || LOG_KS_WARNING || LOG_KS_ERROR || LOG_KS_COMPILE
static char			msg[1024];
#if LOG_KS_TO_OV
	static char msg2[1044];
#endif
#if OV_SYSTEM_NT
DLLFNCEXPORT void ks_logfile_print_sysMsg() {
    DWORD       eNum;
    TCHAR       sysMsg[256];
    TCHAR*      p;

    // INIT
    sysMsg[0] = 0;

    eNum = GetLastError( );
    FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
         NULL, eNum,
         MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
         sysMsg, 255, NULL );

    // Trim the end of the line and terminate it with a null
    p = sysMsg;
    while( ( *p > 31 ) || ( *p == 9 ) ) {
        ++p;
    }
    do {
        *p-- = 0;
    } while( ( p >= sysMsg ) && ( ( *p == '.' ) || ( *p < 33 ) ) );

    // Display the message
    ks_logfile_error("\tfailed with error %lu (%s)",
         (unsigned long)eNum, sysMsg);
}
#else
DLLFNCEXPORT void ks_logfile_print_sysMsg() {

    ks_logfile_error("\tfailed with error %lu (%s)",
         errno, strerror(errno));
}
#endif
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

#if LOG_KS_TO_OV
	snprintf(msg2, sizeof(msg2), "[KS] %s | %s", str, msg);
	ov_logfile_print(OV_MT_INFO, msg2);
#else
	fprintf(stdout,"[KS-Info] %s | %s\n", str, msg);
#if OV_SYSTEM_NT
/*
 *	added to handle output for eclipse consoles on windows
 * 	workaround for the bug https://bugs.eclipse.org/bugs/show_bug.cgi?id=173732
 *	we need to see whether this has performance issues
*/
	fflush(stdout);
#endif
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
#if LOG_KS_TO_OV
	snprintf(msg2, sizeof(msg2), "[KS] %s | %s", str, msg);
	ov_logfile_print(OV_MT_DEBUG, msg2);
#else
	fprintf(stdout,"[KS-Debug] %s | %s\n", str, msg);
#if OV_SYSTEM_NT
/*
 *	added to handle output for eclipse consoles on windows
 * 	workaround for the bug https://bugs.eclipse.org/bugs/show_bug.cgi?id=173732
 *	we need to see whether this has performance issues
*/
	fflush(stdout);
#endif
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
#if LOG_KS_TO_OV
	snprintf(msg2, sizeof(msg2), "[KS] %s | %s", str, msg);
	ov_logfile_print(OV_MT_WARNING, msg2);
#else
	fprintf(stdout,"[KS-Warning] %s | %s\n", str, msg);
#if OV_SYSTEM_NT
/*
 *	added to handle output for eclipse consoles on windows
 * 	workaround for the bug https://bugs.eclipse.org/bugs/show_bug.cgi?id=173732
 *	we need to see whether this has performance issues
*/
	fflush(stdout);
#endif
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
#if LOG_KS_TO_OV
	snprintf(msg2, sizeof(msg2), "[KS] %s | %s", str, msg);
	ov_logfile_print(OV_MT_ERROR, msg2);
#else
	fprintf(stdout,"[KS-Error] %s | %s\n", str, msg);
#if OV_SYSTEM_NT
/*
 *	added to handle output for eclipse consoles on windows
 * 	workaround for the bug https://bugs.eclipse.org/bugs/show_bug.cgi?id=173732
 *	we need to see whether this has performance issues
*/
	fflush(stdout);
#endif
#endif
	va_end(args);
#endif
}
