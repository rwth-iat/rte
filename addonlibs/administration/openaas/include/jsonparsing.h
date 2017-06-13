/*
 * jsonparsing.h
 *
 *  Created on: Dec 8, 2016
 *      Author: lars
 */

#ifndef JSONPARSING_H_
#define JSONPARSING_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <inttypes.h>

#include "jsmn.h"
#include "servicestructs.h"
//#include "ua_constants.h"

#define JSON_SEC_TO_DATETIME 10000000LL
#define JSON_DATETIME_UNIXEPOCH (11644473600LL * JSON_SEC_TO_DATETIME)

typedef int JSON_RC;

#define JSON_RC_OK 0;
#define JSON_RC_WRONG_TYPE -1
#define JSON_RC_MISSING_FIELD -2
#define JSON_RC_WRONG_VALUE -3
#define JSON_RC_NO_SPACE -4
#define JSON_RC_BAD_INPUT -5
#define JSON_RC_MALLOC_FAIL -6
#define JSON_RC_JSMN_ERROR -7
#define JSON_RC_UNDEF -999

// main parsing function
JSON_RC parseJson (const SRV_String* str, SRV_msgHeader** header, void** srvStruct, SRV_service_t* srvType);
JSON_RC genJson (SRV_String** js, const SRV_msgHeader* header, const void* srvStruct, SRV_service_t srvType);

#ifdef JSON_TEST
int dateTimeToISO(SRV_DateTime t, char* iso, int maxLen);
JSON_RC ISOToDateTime(char* iso, int len, SRV_DateTime* jsonTime);
#endif

extern const SRV_String JSON_EL_STR[];

extern const SRV_String JSON_ES_STR[];

extern const SRV_String JSON_ID_STR[];

extern const SRV_String JSON_VIEW_STR[];

extern const SRV_String JSON_VT_STR[];

extern const SRV_String JSON_VIS_STR[];

#endif /* JSONPARSING_H_ */
