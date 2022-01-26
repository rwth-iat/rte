/*
 * ov_logger.c
 *
 *  Created on: 26.05.2015
 *      Author: lars
 */
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include "ov_logfile.h"

#ifdef UA_ENABLE_AMALGAMATION
#include <open62541.h>
#else
#include <open62541/plugin/log.h>
#endif

static char			logMsg[1024];
static char			logMsg2[1101];
static const char *LogCategoryNames[8] = {"Network", "Channel", "Session", "Server", "Client", "User", "Security", "Eventloop"};

static void ov_UAlogger(void* context, UA_LogLevel level, UA_LogCategory category, const char *msg, va_list args) {
#if OV_UA_ENABLE_LOG && OV_UA_USE_OV_LOGGER 
	time_t now;
	struct tm *ptr;
	char str[60];
	now = time(NULL);
	ptr = localtime(&now);
	strftime(str ,100 , "%H:%M.%S",ptr);
	vsnprintf(logMsg, sizeof(logMsg), msg, args);
	if(level == 0){
		snprintf(logMsg2, sizeof(logMsg2), "[UA-Trace] %s\t | %s - %s", LogCategoryNames[category], str, logMsg);
	} else if(level >= 5){
		snprintf(logMsg2, sizeof(logMsg2), "[UA-Fatal] %s\t | %s - %s", LogCategoryNames[category], str, logMsg);
	} else {
		snprintf(logMsg2, sizeof(logMsg2), "[UA] %s\t | %s - %s", LogCategoryNames[category], str, logMsg);
	}
	switch(level){
	case UA_LOGLEVEL_TRACE:
	case UA_LOGLEVEL_DEBUG:
		ov_logfile_print(OV_MT_DEBUG, logMsg2);
		break;
	case UA_LOGLEVEL_INFO:
		ov_logfile_print(OV_MT_INFO, logMsg2);
		break;
	case UA_LOGLEVEL_WARNING:
		ov_logfile_print(OV_MT_WARNING, logMsg2);
		break;
	case UA_LOGLEVEL_ERROR:
	case UA_LOGLEVEL_FATAL:
	default:
		ov_logfile_print(OV_MT_ERROR, logMsg2);
		break;
	}
#endif /*OV_UA_USE_OV_LOGGER*/
	va_end(args);
}

UA_Logger opcua_ovUAlogger_new(void) {
	UA_Logger logger;
	logger.context = NULL;
	logger.log = ov_UAlogger;
	logger.clear = NULL;
	return logger;
}