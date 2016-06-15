/*
 * ov_logger.c
 *
 *  Created on: 26.05.2015
 *      Author: lars
 */

#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include "libov/ov_logfile.h"
#include "open62541.h"

#if LOG_UA
static char			logMsg[1024];
static char			logMsg2[1096];

#if !LOG_UA_TO_OV
static const char *LogLevelNames[6] = {"trace", "debug", "info", "warning", "error", "fatal"};
#endif
static const char *LogCategoryNames[6] = {"network", "securechannel", "session", "server", "client", "userland"};
#endif

static void ov_UAlogger(UA_LogLevel level, UA_LogCategory category, const char *msg, ...) {
#if LOG_UA
	time_t now;
	struct tm *ptr;
	char str[60];
	va_list	args;
	va_start(args, msg);
	now = time(NULL);
	ptr = localtime(&now);
	strftime(str ,100 , "%H:%M.%S",ptr);
	vsnprintf(logMsg, sizeof(logMsg), msg, args);
#if LOG_UA_TO_OV
	if(level == 0){
		snprintf(logMsg2, sizeof(logMsg2), "[UA-trace] %s | %s - %s", LogCategoryNames[category], str, logMsg);
	} else if(level >= 5){
		snprintf(logMsg2, sizeof(logMsg2), "[UA-fatal] %s | %s - %s", LogCategoryNames[category], str, logMsg);
	} else {
		snprintf(logMsg2, sizeof(logMsg2), "[UA] %s | %s - %s", LogCategoryNames[category], str, logMsg);
	}
	switch(level){
	case 0:
	case 1:
		ov_logfile_print(OV_MT_DEBUG, logMsg2);
		break;
	case 2:
		ov_logfile_print(OV_MT_INFO, logMsg2);
		break;
	case 3:
		ov_logfile_print(OV_MT_WARNING, logMsg2);
		break;
	default:
		ov_logfile_print(OV_MT_ERROR, logMsg2);
		break;
	}
#else
	fprintf(stdout,"[UA-%s] %s | %s - %s\n", LogLevelNames[level], LogCategoryNames[category], str, logMsg);
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

UA_Logger ov_UAlogger_new(void) {
	return ov_UAlogger;
}
