/*
 * jsonparsing.c
 *
 *  Created on: Dec 8, 2016
 *      Author: lars
 */

#include "jsonparsing.h"

#define MAX_NUMBUFFER 30
#define MAX_DATEBUFFER 30

// maximal index of constant arrays
#define JSON_EL_MAX 6
#define JSON_ES_MAX 5
#define JSON_ID_MAX 2
#define JSON_VIEW_MAX 9
#define JSON_VT_MAX 10
#define JSON_VIS_MAX 3

const SRV_String JSON_EL_STR [JSON_EL_MAX+1] = {
		{.data = ">" , .length = 1},
		{.data = ">=", .length = 2},
		{.data = "==", .length = 2},
		{.data = "!=", .length = 2},
		{.data = "<=", .length = 2},
		{.data = "<" , .length = 1},
		{.data = ""  , .length = 0}
};
const SRV_String JSON_ES_STR [JSON_ES_MAX+1] = {
		{.data = "Confirmation",	.length = 12},
		{.data = "Setting",			.length =  7},
		{.data = "Measurement", 	.length = 11},
		{.data = "Requirement", 	.length = 11},
		{.data = "" , .length = 0}
};

const SRV_String JSON_ID_STR [JSON_ID_MAX+1] = {
		{.data = "URI", .length = 3},
		{.data = "ISO", .length = 3},
		{.data = ""   , .length = 0}
};

const SRV_String JSON_VIEW_STR [JSON_VIEW_MAX+1] = {
		{.data = "Business",	.length =  8},
		{.data = "Construction",.length = 12},
		{.data = "Power",		.length =  5},
		{.data = "Functional",	.length = 10},
		{.data = "Location",	.length =  8},
		{.data = "Security",	.length =  8},
		{.data = "Network",		.length =  7},
		{.data = "Lifecycle",	.length =  9},
		{.data = "Human",		.length =  5},
		{.data = "",	.length = 0}
};

const SRV_String JSON_VT_STR [JSON_VT_MAX+1] = {
		{.data = "",		.length = 0},
		{.data = "NULL",	.length = 4},
		{.data = "BOOL",	.length = 4},
		{.data = "SINGLE",	.length = 6},
		{.data = "DOUBLE",	.length = 6},
		{.data = "INT32",	.length = 5},
		{.data = "INT64",	.length = 5},
		{.data = "UINT32",	.length = 6},
		{.data = "UINT64",	.length = 6},
		{.data = "STRING",	.length = 6},
		{.data = "DATETIME",.length = 8}
};

const SRV_String JSON_VIS_STR [JSON_VIS_MAX+1] = {
		{.data = "Private",		.length = 7},
		{.data = "Contract",	.length = 8},
		{.data = "Public",		.length = 6},
		{.data = "",	.length = 0}
};

static int jsoneq(const char *json, const jsmntok_t *tok, const char *s) {
	if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
			strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
		return 0;
	}
	return -1;
}

static int srvStrEq(const SRV_String* str1, const SRV_String* str2) {
	if(str1->length!=str2->length)
		return -1;
	return strncmp(str1->data, str2->data, str1->length);
}

JSON_RC tok2SrvStr(const char* js, const jsmntok_t* t, SRV_String* str){
	if(!(t->type==JSMN_STRING))
		return JSON_RC_WRONG_TYPE;
	if(!str)
		return JSON_RC_UNDEF;

	SRV_String_setCopy(str, js+t->start, t->end-t->start);

	return JSON_RC_OK;
}

SRV_valType_t getPrimType(const char* js, const jsmntok_t* t){

	SRV_valType_t type = SRV_VT_undefined;

	if(!(t->type==JSMN_PRIMITIVE))
		return SRV_VT_undefined;

	switch (js[t->start]){
	case 't':case 'T':case 'f':case 'F':
		type = SRV_VT_BOOL;
		break;
	case 'n':case 'N':
		type = SRV_VT_NULL;
		break;
	case '0':case '1':case '2':case '3':case '4':case '5':
	case '6':case '7':case '8':case '9':case '.':case '-':
		// number
		type = SRV_VT_INT64;
		for(int i = t->start; i<=t->end; i++){
			if(js[i]=='.' || js[i]=='e' || js[i]=='E'){
				type = SRV_VT_DOUBLE;
				break;
			}
		}
		break;
	default:
		type = SRV_VT_undefined;
	}

	return type;
}

JSON_RC tok2Prim(const char* js, const jsmntok_t* t, SRV_valType_t* type, void* value){

	if(!(t->type==JSMN_PRIMITIVE))
		return JSON_RC_WRONG_TYPE;

	SRV_valType_t tmpType = getPrimType(js, t);

	if(tmpType==SRV_VT_undefined)
		return JSON_RC_WRONG_VALUE;

	if(*type==SRV_VT_undefined){
		*type = tmpType;
	} else {
		// test if set type matches content
		switch(*type){
		case SRV_VT_BOOL:
			if(tmpType!=SRV_VT_BOOL)
				return JSON_RC_WRONG_VALUE;
			break;
		case SRV_VT_DOUBLE: case SRV_VT_SINGLE:
			if(!(tmpType==SRV_VT_DOUBLE || tmpType==SRV_VT_INT64))
				return JSON_RC_WRONG_VALUE;
			break;
		case SRV_VT_INT32: case SRV_VT_INT64:
		case SRV_VT_UINT32: case SRV_VT_UINT64:
			if(tmpType!=SRV_VT_INT64)
				return JSON_RC_WRONG_VALUE;
			break;
		case SRV_VT_NULL:
			if(tmpType!=SRV_VT_NULL)
				return JSON_RC_WRONG_VALUE;
			break;
		default:
			return JSON_RC_WRONG_VALUE;
		}
	}

	switch (*type){
	case SRV_VT_BOOL:
		switch(js[t->start]){
		case 'f':case 'F':
			if(value)
				*(bool*)value = false;
			break;
		case 't':case 'T':
			if(value)
				*(bool*)value = true;
			break;
		}
		break;
	case SRV_VT_NULL:
		value = NULL;
		break;
	case SRV_VT_DOUBLE:
		if(value)
			*(double*)value = strtod(&js[t->start], NULL);
		break;
	case SRV_VT_SINGLE:
		if(value)
			*(float*)value = strtof(&js[t->start], NULL);
		break;
	case SRV_VT_INT32:
		if(value)
			*(int32_t*)value = strtol(&js[t->start], NULL, 10);
		break;
	case SRV_VT_INT64:
		if(value)
			*(int64_t*)value = strtoll(&js[t->start], NULL, 10);
		break;
	case SRV_VT_UINT32:
		if(value)
			*(uint32_t*)value = strtoul(&js[t->start], NULL, 10);
		break;
	case SRV_VT_UINT64:
		if(value)
			*(uint64_t*)value = strtoull(&js[t->start], NULL, 10);
		break;
	default:
		return JSON_RC_WRONG_VALUE;
	}
	return JSON_RC_OK;
}

JSON_RC tok2Any(const char* js, const jsmntok_t* t, SRV_valType_t* type, void* value){
	JSON_RC rc;

	if(t->type==JSMN_STRING){
		if(*type==SRV_VT_DATETIME){
			SRV_String time;
			rc = tok2SrvStr(js, t, &time);
			if(rc)
				return rc;
			rc = ISOToDateTime(time.data, time.length, (SRV_DateTime*)value);
			SRV_String_deleteMembers(&time);
			if(rc)
				return rc;
		} else {
			rc = tok2SrvStr(js, t, (SRV_String*)value);
			if(rc)
				return rc;
			*type = SRV_VT_STRING;
		}
	} else if(t->type==JSMN_PRIMITIVE){
		rc = tok2Prim(js, t, type, value);
		if(rc)
			return rc;
	}
	return JSON_RC_OK;
}

JSON_RC tok2Int(const char* js, const jsmntok_t* t, void* value, int force32){
	SRV_valType_t type = SRV_VT_undefined;
	if(t->type!=JSMN_PRIMITIVE)
		return JSON_RC_WRONG_TYPE;

	tok2Prim(js, t, &type, NULL);
	if(type!=SRV_VT_INT64)
		return JSON_RC_WRONG_VALUE;

	if(force32){
		*(int*)value = strtol(&js[t->start], NULL, 10);
	} else {
		*(long long int*)value = strtoll(&js[t->start], NULL, 10);
	}

	return JSON_RC_OK;
}

JSON_RC tok2Double(const char* js, const jsmntok_t* t, double* value){
	SRV_valType_t type = SRV_VT_undefined;
	if(!(t->type==JSMN_PRIMITIVE))
		return JSON_RC_WRONG_TYPE;

	tok2Prim(js, t, &type, NULL);
	if(type!=SRV_VT_DOUBLE && type!=SRV_VT_INT64)
		return JSON_RC_WRONG_VALUE;

	*value = strtod(&js[t->start], NULL);

	return 0;
}

JSON_RC tok2Bool(const char* js, const jsmntok_t* t, bool* value){
	SRV_valType_t type = SRV_VT_undefined;
	if(!(t->type==JSMN_PRIMITIVE))
		return JSON_RC_WRONG_TYPE;

	tok2Prim(js, t, &type, NULL);
	if(type!=SRV_VT_BOOL)
		return JSON_RC_WRONG_VALUE;

	tok2Prim(js, t, &type, value);

	return 0;
}

JSON_RC tok2Status(const char* js, const jsmntok_t* t, status_t* status){
	return tok2Int(js, t, status, 1);
}

int json_append(SRV_String* js, int start, const char* str, int len, int maxLen){

	if(!str)
		return JSON_RC_BAD_INPUT;

	if(len<0)
		len = strlen(str);

	if(start<0)
		return start;

	if(js){
		if(maxLen<start+len)
			return JSON_RC_NO_SPACE;

		memcpy(js->data+start, str, len*sizeof(char));
		js->length = start+len;
	}

	return start+len;
}

int json_appendKey(SRV_String* js, int start, const char* str, int len, int maxLen, int* first){
	int pos;
	if(!first || *first){
		pos = json_append(js, start, "\"", 1, maxLen);
		if(first)
			*first = false;
	} else {
		pos = json_append(js, start, ",\"", 2, maxLen);
	}
	pos = json_append(js, pos, str, len, maxLen);
	pos = json_append(js, pos, "\":", 2, maxLen);
	return pos;
}

int json_appendString(SRV_String* js, int start, const char* str, int len, int maxLen){
	int pos = json_append(js, start, "\"", 1, maxLen);
	pos = json_append(js, pos, str, len, maxLen);
	pos = json_append(js, pos, "\"", 1, maxLen);
	return pos;
}

int json_appendSrvStr(SRV_String* js, int start, const SRV_String* str, int maxLen){
	int pos = json_append(js, start, "\"", 1, maxLen);
	pos = json_append(js, pos, str->data, str->length, maxLen);
	pos = json_append(js, pos, "\"", 1, maxLen);
	return pos;
}

int json_appendAny(SRV_String* js, int start, const void* value, SRV_valType_t type, int maxLen){
	int pos;

	int bufLen;
	char numbuffer[MAX_NUMBUFFER];

	switch(type){
	case SRV_VT_BOOL:
		if(*(uint8_t*)value){
			pos = json_append(js, start, "true", -1, maxLen);
		} else {
			pos = json_append(js, start, "false", -1, maxLen);
		}
		break;
	case SRV_VT_NULL:
		pos = json_append(js, start, "null", -1, maxLen);
		break;
	case SRV_VT_STRING:
		pos = json_appendString(js, start, ((SRV_String*)value)->data, ((SRV_String*)value)->length, maxLen);
		break;
	case SRV_VT_DATETIME:
		pos = json_appendIsoTime(js, start, maxLen, *(SRV_DateTime*)value);
		break;
	case SRV_VT_INT32:
		bufLen = snprintf(numbuffer, MAX_NUMBUFFER, "%"PRIi32, *(int32_t*)value);
		if(!(bufLen<MAX_NUMBUFFER))
			return JSON_RC_NO_SPACE;
		pos = json_append(js, start, numbuffer, bufLen, maxLen);
		break;
	case SRV_VT_INT64:
		bufLen = snprintf(numbuffer, MAX_NUMBUFFER, "%"PRIi64, *(int64_t*)value);
		if(!(bufLen<MAX_NUMBUFFER))
			return JSON_RC_NO_SPACE;
		pos = json_append(js, start, numbuffer, bufLen, maxLen);
		break;
	case SRV_VT_UINT32:
		bufLen = snprintf(numbuffer, MAX_NUMBUFFER, "%"PRIu32, *(uint32_t*)value);
		if(!(bufLen<MAX_NUMBUFFER))
			return JSON_RC_NO_SPACE;
		pos = json_append(js, start, numbuffer, bufLen, maxLen);
		break;
	case SRV_VT_UINT64:
		bufLen = snprintf(numbuffer, MAX_NUMBUFFER, "%"PRIu64, *(uint64_t*)value);
		if(!(bufLen<MAX_NUMBUFFER))
			return JSON_RC_NO_SPACE;
		pos = json_append(js, start, numbuffer, bufLen, maxLen);
		break;
	case SRV_VT_SINGLE:
		bufLen = snprintf(numbuffer, MAX_NUMBUFFER, "%.9g", *(float*)value);
		if(!(bufLen<MAX_NUMBUFFER))
			return JSON_RC_NO_SPACE;
		pos = json_append(js, start, numbuffer, bufLen, maxLen);
		break;
	case SRV_VT_DOUBLE:
		bufLen = snprintf(numbuffer, MAX_NUMBUFFER, "%.17g", *(double*)value);
		if(!(bufLen<MAX_NUMBUFFER))
			return JSON_RC_NO_SPACE;
		pos = json_append(js, start, numbuffer, bufLen, maxLen);
		break;
	case SRV_VT_undefined:
		return JSON_RC_WRONG_TYPE;
		break;
	}
	return pos;
}

int json_appendStatus(SRV_String* js, int start, const status_t* status, int maxLen){
	return json_appendAny(js, start, status, SRV_VT_INT32, maxLen);
}

int dateTimeToISO(SRV_DateTime t, char* iso, int maxLen){
	int len;
	int psec = t % JSON_SEC_TO_DATETIME; // micro sec
	time_t sec = (t - JSON_DATETIME_UNIXEPOCH) / JSON_SEC_TO_DATETIME;
	struct tm* tm = gmtime(&sec);

	if(!psec){
		len = strftime(iso, maxLen, "%Y-%m-%dT%H:%M:%SZ", tm);
	} else {
		len = strftime(iso, maxLen, "%Y-%m-%dT%H:%M:%S.", tm);
		if(len)
			len += snprintf(iso+len, maxLen-len, "%07iZ", psec);
	}

	return len;
}

JSON_RC ISOToDateTime(char* iso, int len, SRV_DateTime* jsonTime){
	//SRV_DateTime jsonTime;
	char* res;
	char* buf;
	time_t unixTime;
	int psec = 0;

	// need null terminator when string ends in 0-9, 'T', '-', ':', '.' ( valid for time format )
	if(len>0 && ((iso[len-1] >= '0' && iso[len-1] <= '9') ||
			iso[len] == 'T' || iso[len] == '-' || iso[len] == ':' || iso[len] == '.')){
		buf = malloc(len+1);
		memcpy(buf, iso, len);
		buf[len] = '\0';
	} else {

		buf = iso;
	}

	struct tm tm;
	memset(&tm, '\0', sizeof(tm));

	res = strptime(buf, "%FT%TZ", &tm);
	if(res==NULL){
		res = strptime(buf, "%FT%T.", &tm);
		if(res==NULL){
			if(buf!=iso)
				free(buf);
			return JSON_RC_WRONG_VALUE;
		}
		char* tail;
		psec = strtol(res, &tail, 10);
		int i = tail-res;
		if(i<6)
			for(;i<7;i++){
				psec = psec * 10;
			}
		else if (i>7) {
			for(;i>6;i--){
				psec = psec / 10;
			}
		}
	}

	unixTime = mktime(&tm) + tm.tm_gmtoff;

	*jsonTime = (unixTime * JSON_SEC_TO_DATETIME) + JSON_DATETIME_UNIXEPOCH + psec;

	if(buf!=iso)
		free(buf);

	return JSON_RC_OK;
}

int json_appendIsoTime(SRV_String* js, int start, int maxLen, SRV_DateTime t){
	int pos;
	char buf[MAX_DATEBUFFER];
	int len = 0;

	len = dateTimeToISO(t, buf, MAX_DATEBUFFER);

	pos = json_appendString(js, start, buf, len, maxLen);


	return pos;
}



JSON_RC parseJson (const SRV_String* str, SRV_msgHeader** header, void** srvStruct, SRV_service_t* srvType) {

	JSON_RC rc;
	int n;
	jsmn_parser p;
	jsmntok_t* token;

	*srvStruct = NULL;
	*srvType = SRV_undefined;

	jsmn_init(&p);

	n = jsmn_parse(&p, str->data, str->length, NULL, 0);

	if(n<0){
		return -1;
	}

	token = malloc(n*sizeof(jsmntok_t));

	jsmn_init(&p);
	jsmn_parse(&p, str->data, str->length, token, n);

	// find service name, msgNo and parameter object
	*header = SRV_msgHeader_t_new();
	SRV_String serviceName;
	int startParam = -1;
	int fields = 0; // 1 serviceName; 2 serviceParameter; 4 msgNo; 8 Sender; 16 Receiver

	for(int i = 0; i < n && fields<31; i++) {
		if(jsoneq(str->data, &token[i], "serviceName")==0){
			i++;
			rc = tok2SrvStr(str->data, &token[i], &serviceName);
			if(rc)
				return rc;
			fields |= 1;
		} else if(jsoneq(str->data, &token[i], "serviceParameter")==0) {
			i++;
			if(token[i].type!=JSMN_OBJECT)
				return JSON_RC_WRONG_TYPE;

			startParam = i;
			fields |= 2;
		} else if(jsoneq(str->data, &token[i], "msgNo")==0) {
			i++;
			rc = tok2Int(str->data, &token[i], &(*header)->msgNo, 1);
			if(rc)
				return rc;
			fields |= 4;
		} else if(jsoneq(str->data, &token[i], "sender")==0) {
			i++;
			rc = jsonparseId(token, str, i,n , &(*header)->sender, &i);
			if(rc)
				return rc;
			fields |= 8;
		} else if(jsoneq(str->data, &token[i], "receiver")==0) {
			i++;
			rc = jsonparseId(token, str, i, n, &(*header)->receiver, &i);
			if(rc)
				return rc;
			fields |= 16;
		}
	}

	if(fields!=31)
		return JSON_RC_MISSING_FIELD;

	// call function to process message body
	if(strncmp(serviceName.data, "createAASReq", serviceName.length)==0){
		*srvType = SRV_createAASReq;
		*srvStruct = malloc(sizeof(createAASReq_t));
		rc = jsonparseCreateAASReq(*srvStruct, token, str, n, startParam);
	} else if(strncmp(serviceName.data, "createAASRsp", serviceName.length)==0){
		*srvType = SRV_createAASRsp;
		*srvStruct = malloc(sizeof(createAASRsp_t));
		rc = jsonparseCreateAASRsp(*srvStruct, token, str, n, startParam);
	} else if(strncmp(serviceName.data, "deleteAASReq", serviceName.length)==0){
		*srvType = SRV_deleteAASReq;
		*srvStruct = malloc(sizeof(deleteAASReq_t));
		rc = jsonparseDeleteAASReq(*srvStruct, token, str, n, startParam);
	} else if(strncmp(serviceName.data, "deleteAASRsp", serviceName.length)==0){
		*srvType = SRV_deleteAASRsp;
		*srvStruct = malloc(sizeof(deleteAASRsp_t));
		rc = jsonparseDeleteAASRsp(*srvStruct, token, str, n, startParam);
	} else if(strncmp(serviceName.data, "createPVSLReq", serviceName.length)==0){
		*srvType = SRV_createPVSLReq;
		*srvStruct = malloc(sizeof(createPVSLReq_t));
		rc = jsonparseCreatePVSLReq(*srvStruct, token, str, n, startParam);
	} else if(strncmp(serviceName.data, "createPVSLRsp", serviceName.length)==0){
		*srvType = SRV_createPVSLRsp;
		*srvStruct = malloc(sizeof(createPVSLRsp_t));
		rc = jsonparseCreatePVSLRsp(*srvStruct, token, str, n, startParam);
	} else if(strncmp(serviceName.data, "deletePVSLReq", serviceName.length)==0){
		*srvType = SRV_deletePVSLReq;
		*srvStruct = malloc(sizeof(deletePVSLReq_t));
		rc = jsonparseDeletePVSLReq(*srvStruct, token, str, n, startParam);
	} else if(strncmp(serviceName.data, "deletePVSLRsp", serviceName.length)==0){
		*srvType = SRV_deletePVSLRsp;
		*srvStruct = malloc(sizeof(deletePVSLRsp_t));
		rc = jsonparseDeletePVSLRsp(*srvStruct, token, str, n, startParam);
	} else if(strncmp(serviceName.data, "createPVSReq", serviceName.length)==0){
		*srvType = SRV_createPVSReq;
		*srvStruct = malloc(sizeof(createPVSReq_t));
		rc = jsonparseCreatePVSReq(*srvStruct, token, str, n, startParam);
	} else if(strncmp(serviceName.data, "createPVSRsp", serviceName.length)==0){
		*srvType = SRV_createPVSRsp;
		*srvStruct = malloc(sizeof(createPVSRsp_t));
		rc = jsonparseCreatePVSRsp(*srvStruct, token, str, n, startParam);
	} else if(strncmp(serviceName.data, "deletePVSReq", serviceName.length)==0){
		*srvType = SRV_deletePVSReq;
		*srvStruct = malloc(sizeof(deletePVSReq_t));
		rc = jsonparseDeletePVSReq(*srvStruct, token, str, n, startParam);
	} else if(strncmp(serviceName.data, "deletePVSRsp", serviceName.length)==0){
		*srvType = SRV_deletePVSRsp;
		*srvStruct = malloc(sizeof(deletePVSRsp_t));
		rc = jsonparseDeletePVSRsp(*srvStruct, token, str, n, startParam);
	} else if(strncmp(serviceName.data, "createLCEReq", serviceName.length)==0){
		*srvType = SRV_createLCEReq;
		*srvStruct = malloc(sizeof(createLCEReq_t));
		rc = jsonparseCreateLCEReq(*srvStruct, token, str, n, startParam);
	} else if(strncmp(serviceName.data, "createLCERsp", serviceName.length)==0){
		*srvType = SRV_createLCERsp;
		*srvStruct = malloc(sizeof(createLCERsp_t));
		rc = jsonparseCreateLCERsp(*srvStruct, token, str, n, startParam);
	} else if(strncmp(serviceName.data, "deleteLCEReq", serviceName.length)==0){
		*srvType = SRV_deleteLCEReq;
		*srvStruct = malloc(sizeof(deleteLCEReq_t));
		rc = jsonparseDeleteLCEReq(*srvStruct, token, str, n, startParam);
	} else if(strncmp(serviceName.data, "deleteLCERsp", serviceName.length)==0){
		*srvType = SRV_deleteLCERsp;
		*srvStruct = malloc(sizeof(deleteLCERsp_t));
		rc = jsonparseDeleteLCERsp(*srvStruct, token, str, n, startParam);
	} else if(strncmp(serviceName.data, "getPVSReq", serviceName.length)==0){
		*srvType = SRV_getPVSReq;
		*srvStruct = malloc(sizeof(getPVSReq_t));
		rc = jsonparseGetPVSReq(*srvStruct, token, str, n, startParam);
	} else if(strncmp(serviceName.data, "getPVSRsp", serviceName.length)==0){
		*srvType = SRV_getPVSRsp;
		*srvStruct = malloc(sizeof(getPVSRsp_t));
		rc = jsonparseGetPVSRsp(*srvStruct, token, str, n, startParam);
	} else if(strncmp(serviceName.data, "setPVSReq", serviceName.length)==0){
		*srvType = SRV_setPVSReq;
		*srvStruct = malloc(sizeof(setPVSReq_t));
		rc = jsonparseSetPVSReq(*srvStruct, token, str, n, startParam);
	} else if(strncmp(serviceName.data, "setPVSRsp", serviceName.length)==0){
		*srvType = SRV_setPVSRsp;
		*srvStruct = malloc(sizeof(setPVSRsp_t));
		rc = jsonparseSetPVSRsp(*srvStruct, token, str, n, startParam);
	} else if(strncmp(serviceName.data, "getLCEReq", serviceName.length)==0){
		*srvType = SRV_getLCEReq;
		*srvStruct = malloc(sizeof(getLCEReq_t));
		rc = jsonparseGetLCEReq(*srvStruct, token, str, n, startParam);
	} else if(strncmp(serviceName.data, "getLCERsp", serviceName.length)==0){
		*srvType = SRV_getLCERsp;
		*srvStruct = malloc(sizeof(getLCERsp_t));
		rc = jsonparseGetLCERsp(*srvStruct, token, str, n, startParam);
	} else if(strncmp(serviceName.data, "setLCEReq", serviceName.length)==0){
		*srvType = SRV_setLCEReq;
		*srvStruct = malloc(sizeof(setLCEReq_t));
		rc = jsonparseSetLCEReq(*srvStruct, token, str, n, startParam);
	} else if(strncmp(serviceName.data, "setLCERsp", serviceName.length)==0){
		*srvType = SRV_setLCERsp;
		*srvStruct = malloc(sizeof(setLCERsp_t));
		rc = jsonparseSetLCERsp(*srvStruct, token, str, n, startParam);
	} else if(strncmp(serviceName.data, "getCoreDataReq", serviceName.length)==0){
		*srvType = SRV_getCoreDataReq;
		*srvStruct = malloc(sizeof(getCoreDataReq_t));
		rc = jsonparseGetCoreDataReq(*srvStruct, token, str, n, startParam);
	} else if(strncmp(serviceName.data, "getCoreDataRsp", serviceName.length)==0){
		*srvType = SRV_getCoreDataRsp;
		*srvStruct = malloc(sizeof(getCoreDataRsp_t));
		rc = jsonparseGetCoreDataRsp(*srvStruct, token, str, n, startParam);
	} else {
		return JSON_RC_UNDEF;
	}

	SRV_String_deleteMembers(&serviceName);
	free(token);
	if(rc)
		return rc;

	return JSON_RC_OK;
}

JSON_RC genJson(SRV_String** js, const SRV_msgHeader* header, const void* srvStruct, SRV_service_t srvType){

	JSON_RC rc;
	int len;
	*js = NULL;

	rc = jsonGenGeneric(NULL, &len, header, srvStruct, srvType);

	if(rc)
		return rc;

	if(len<0)
		return -1;

	*js = malloc(sizeof(SRV_String));
	(*js)->data = malloc(len+1);
	(*js)->length = len;

	rc = jsonGenGeneric(*js, &len, header, srvStruct, srvType);

	(*js)->data[len] = '\0';

	return 0;
}

JSON_RC jsonparseId(const jsmntok_t* t, const SRV_String* str, int start, int n, SRV_ident_t* id, int* last){

	JSON_RC rc;

	if(start<0)
		return JSON_RC_UNDEF;
	if(t[start].type!=JSMN_OBJECT)
		return JSON_RC_WRONG_TYPE;

	int j = start+1;
	int fields = 0;

	while(j < n && t[j].start<=t[start].end){
		if(jsoneq(str->data, &t[j], "IdType")==0){
			j++;
			SRV_String tmpStr;
			tok2SrvStr(str->data, &t[j], &tmpStr);
			id->idType = SRV_IDT_undefined;
			for(int i = 0; i < JSON_ID_MAX && id->idType == SRV_IDT_undefined; i++){
				if(srvStrEq(&tmpStr, &JSON_ID_STR[i])==0)
					id->idType = i;
			}
			SRV_String_deleteMembers(&tmpStr);
			if(id->idType==SRV_IDT_undefined)
				return JSON_RC_WRONG_VALUE;
			fields |= 1;
		} else if(jsoneq(str->data, &t[j], "IdSpec")==0){
			j++;
			rc = tok2SrvStr(str->data, &t[j], &id->idSpec);
			if(rc)
				return rc;
			fields |= 2;
		}
		j++;
	}

	if(fields!=3)
		return JSON_RC_MISSING_FIELD;

	*last = j - 1;

	return JSON_RC_OK;
}

JSON_RC jsonparsePVSL(const jsmntok_t* t, const SRV_String* str, int start, int n, PVSL_t* pvsl, int* last){


	JSON_RC rc;

	if(start<0)
		return JSON_RC_UNDEF;
	if(t[start].type!=JSMN_OBJECT)
		return JSON_RC_WRONG_TYPE;

	int j = start+1;

	while(j < n && t[j].start<=t[start].end){
		if(jsoneq(str->data, &t[j], "Carrier")==0){
			j++;
			rc = jsonparseId(t, str, j, n, &pvsl->carrier, &j);
			if(rc)
				return rc;
		} else if(jsoneq(str->data, &t[j], "Name")==0){
			j++;
			rc = tok2SrvStr(str->data, &t[j], &pvsl->name);
			if(rc)
				return rc;
			pvsl->hasName = true;
		} else if(jsoneq(str->data, &t[j], "CreatingInstance")==0){
			j++;
			rc = jsonparseId(t, str, j, n, &pvsl->creatingInstance, &j);
			if(rc)
				return rc;
		} else if(jsoneq(str->data, &t[j], "CreationTime")==0){
			j++;
			SRV_String time;
			rc = tok2SrvStr(str->data, &t[j], &time);
			if(rc)
				return rc;
			rc =  ISOToDateTime(time.data, time.length, &pvsl->creationTime);
			SRV_String_deleteMembers(&time);
			if(rc)
				return rc;
			pvsl->hasCreationTime = true;
		} else if(jsoneq(str->data, &t[j], "PVS")==0){
			j++;
			if(t[j].type==JSMN_ARRAY){
				pvsl->numPvs = t[j].size;
				j++;
			} else {
				pvsl->numPvs = 1;
			}
			pvsl->pvs = malloc(sizeof(PVS_t)*pvsl->numPvs);
			for(uint k = 0; k<pvsl->numPvs; k++ ){
				PVS_t_init(&pvsl->pvs[k]);
				rc = jsonparsePVS(t, str, j, n, &pvsl->pvs[k], &j);
				if(rc)
					return rc;
				j++;
			}
			j--; //one to far
		}
		j++;
	}

	*last = j - 1;

	return JSON_RC_OK;
}

JSON_RC jsonparsePVS(const jsmntok_t* t, const SRV_String* str, int start, int n, PVS_t* pvs, int* last) {

	JSON_RC rc;

	if(start<0)
		return JSON_RC_UNDEF;
	if(t[start].type!=JSMN_OBJECT)
		return JSON_RC_WRONG_TYPE;

	int j = start+1;
	//int intLast = 0;

	while(j < n && t[j].start<=t[start].end){
		if(jsoneq(str->data, &t[j], "Name")==0){
			j++;
			rc = tok2SrvStr(str->data, &t[j], &pvs->name);
			if(rc)
				return rc;
			pvs->hasName = true;
		} else if(jsoneq(str->data, &t[j], "ExpressionSemantic")==0){
			j++;
			SRV_String tmpStr;
			rc = tok2SrvStr(str->data, &t[j], &tmpStr);
			if(rc)
				return rc;
			pvs->expressionSemantic = SRV_ES_undefined;
			for(int i = 0; i < JSON_ES_MAX && pvs->expressionSemantic == SRV_ES_undefined; i++) {
				if(srvStrEq(&tmpStr, &JSON_ES_STR[i])==0){
					pvs->expressionSemantic = i;
				}
			}
			SRV_String_deleteMembers(&tmpStr);
			if(pvs->expressionSemantic==SRV_ES_undefined)
				return JSON_RC_WRONG_VALUE;
		} else if(jsoneq(str->data, &t[j], "ExpressionLogic")==0){
			j++;
			SRV_String tmpStr;
			rc = tok2SrvStr(str->data, &t[j], &tmpStr);
			if(rc)
				return rc;
			pvs->expressionLogic = SRV_EL_undefined;
			for(int i = 0; i < JSON_EL_MAX && pvs->expressionLogic == SRV_EL_undefined; i++){
				if(srvStrEq(&tmpStr, &JSON_EL_STR[i])==0){
					pvs->expressionLogic = i;
				}
			}
			SRV_String_deleteMembers(&tmpStr);
			if(pvs->expressionLogic==SRV_EL_undefined)
				return JSON_RC_WRONG_VALUE;
		} else if(jsoneq(str->data, &t[j], "View")==0){
			j++;
			SRV_String tmpStr;
			rc = tok2SrvStr(str->data, &t[j], &tmpStr);
			if(rc)
				return rc;
			pvs->view = SRV_VIEW_undefined;
			for(int i=0; i < JSON_VIEW_MAX && pvs->view == SRV_VIEW_undefined; i++){
				if(srvStrEq(&tmpStr, &JSON_VIEW_STR[i])==0)
					pvs->view = i;
			}
			SRV_String_deleteMembers(&tmpStr);
			if(pvs->view==SRV_VIEW_undefined)
				return JSON_RC_WRONG_VALUE;
		} else if(jsoneq(str->data, &t[j], "Unit")==0){
			j++;
			rc = tok2SrvStr(str->data, &t[j], &pvs->unit);
			if(rc)
				return rc;
			pvs->hasUnit = true;
		} else if(jsoneq(str->data, &t[j], "ID")==0){
			j++;
			rc = jsonparseId(t, str, j, n, &pvs->ID, &j);
			if(rc)
				return rc;
		} else if(jsoneq(str->data, &t[j], "Visibility")==0){
			j++;
			SRV_String tmpStr;
			rc = tok2SrvStr(str->data, &t[j], &tmpStr);
			if(rc)
				return rc;
			pvs->visibility = SRV_VIS_undefined;
			for(int i=0; i < JSON_VIS_MAX && pvs->visibility == SRV_VIS_undefined; i++){
				if(srvStrEq(&tmpStr, &JSON_VIS_STR[i])==0)
					pvs->visibility = i;
			}
			SRV_String_deleteMembers(&tmpStr);
			if(pvs->view==SRV_VIEW_undefined)
				return JSON_RC_WRONG_VALUE;
		} else if(jsoneq(str->data, &t[j], "ValueTime")==0){
			j++;
			SRV_String time;
			rc = tok2SrvStr(str->data, &t[j], &time);
			if(rc)
				return rc;
			rc = ISOToDateTime(time.data, time.length, &pvs->valTime);
			if(rc)
				return rc;
			pvs->hasValTime = true;
		} else if(jsoneq(str->data, &t[j], "ValueType")==0){
			j++;
			SRV_String tmpStr;
			rc = tok2SrvStr(str->data, &t[j], &tmpStr);
			if(rc)
				return rc;
			for(int i = 1; i <= JSON_VT_MAX && pvs->valType == SRV_VT_undefined; i++){
				if(srvStrEq(&tmpStr, &JSON_VT_STR[i])==0)
					pvs->valType = i;
			}
			if(pvs->valType == SRV_VT_undefined)
				return JSON_RC_WRONG_VALUE;
			SRV_String_deleteMembers(&tmpStr);
			if(pvs->valType == SRV_VT_undefined)
				return JSON_RC_WRONG_VALUE;
		} else if(jsoneq(str->data, &t[j], "Value")==0){
			j++;
			if(pvs->valType==SRV_VT_undefined){
				//search rest of pvs for value type field
				for(int k = j+1; k<n && t[k].start<=t[start].end; k++){
					if(jsoneq(str->data, &t[k], "ValueType")==0){
						k++;
						SRV_String tmpStr;
						rc = tok2SrvStr(str->data, &t[k], &tmpStr);
						if(rc)
							return rc;
						for(int i = 1; i<=JSON_VT_MAX && pvs->valType==SRV_VT_undefined; i++){
							if(srvStrEq(&tmpStr, &JSON_VT_STR[i])==0)
								pvs->valType = i;
						}
						SRV_String_deleteMembers(&tmpStr);
						if(pvs->valType == SRV_VT_undefined)
							return JSON_RC_WRONG_VALUE;
					}
				}
			}

			rc = tok2Any(str->data, &t[j], &pvs->valType, pvs->value);
			if(rc)
				return rc;
		}
		j++;
	}

	// return last used token
	*last = j-1;

	return JSON_RC_OK;
}

JSON_RC jsonparseLCE(const jsmntok_t* t, const SRV_String* str, int start, int n, LCE_t* lce, int* last){

	JSON_RC rc;

	if(start<0)
		return JSON_RC_UNDEF;
	if(t[start].type!=JSMN_OBJECT)
		return JSON_RC_WRONG_TYPE;

	int j = start + 1;
	//int intLast = 0;

	while(j < n && t[j].start<=t[start].end){
		if(jsoneq(str->data, &t[j], "LCEId")==0){
			j++;
			rc = tok2Int(str->data, &t[j], &lce->lceId, 0);
			if(rc)
				return rc;
			lce->hasLceId = true;
		} else if(jsoneq(str->data, &t[j], "CreatingInstanceId")==0){
			j++;
			rc = jsonparseId(t, str, j, n, &lce->creatingInstance, &j);
			if(rc)
				return rc;
		} else if(jsoneq(str->data, &t[j], "WritingInstanceId")==0){
			j++;
			rc = jsonparseId(t, str, j, n, &lce->writingInstance, &j);
			if(rc)
				return rc;
		} else if(jsoneq(str->data, &t[j], "EventClass")==0){
			j++;
			rc = tok2SrvStr(str->data, &t[j], &lce->eventClass);
			if(rc)
				return rc;
			lce->hasEventClass = true;
		} else if(jsoneq(str->data, &t[j], "Subject")==0){
			j++;
			rc = tok2SrvStr(str->data, &t[j], &lce->subject);
			if(rc)
				return rc;
			lce->hasSubject = true;
		} else if(jsoneq(str->data, &t[j], "DataTime")==0){
			j++;
			SRV_String time;
			rc = tok2SrvStr(str->data, &t[j], &time);
			if(rc){
				return rc;
			}
			rc =  ISOToDateTime(time.data, time.length, &lce->dataTime);
			SRV_String_deleteMembers(&time);
			if(rc)
				return rc;
			lce->hastDataTime = true;
		} else if(jsoneq(str->data, &t[j], "DataType")==0){
			j++;
			SRV_String tmpStr;
			rc = tok2SrvStr(str->data, &t[j], &tmpStr);
			if(rc)
				return rc;
			for(int i = 1; i<=JSON_VT_MAX && lce->dataType!=SRV_VT_undefined; i++){
				if(srvStrEq(&tmpStr, &JSON_VT_STR[i])==0)
					lce->dataType = i;
			}
			SRV_String_deleteMembers(&tmpStr);
			if(lce->dataType==SRV_VT_undefined)
				return JSON_RC_WRONG_VALUE;
		} else if(jsoneq(str->data, &t[j], "Data")==0){
			j++;
			if(lce->dataType==SRV_VT_undefined){
				for(int k = j+1; k < n && t[k].start<=t[start].end; k++){
					if(jsoneq(str->data, &t[k], "DataType")==0){
						k++;
						SRV_String tmpStr;
						rc = tok2SrvStr(str->data, &t[k], &tmpStr);
						if(rc)
							return rc;
						for(int i = 1; i<=JSON_VT_MAX && lce->dataType==SRV_VT_undefined; i++){
							if(srvStrEq(&tmpStr, &JSON_VT_STR[i])==0)
								lce->dataType = i;
						}
						SRV_String_deleteMembers(&tmpStr);
						if(lce->dataType==SRV_VT_undefined)
							return JSON_RC_WRONG_VALUE;
					}
				}
			}

			rc = tok2Any(str->data, &t[j], &lce->dataType, lce->data);
			if(rc)
				return rc;
		}
		j++;
	}

	*last = j - 1;

	return JSON_RC_OK;
}

JSON_RC jsonparseStatusRsp(status_t* status, const jsmntok_t* t, const SRV_String* js, int n, int start){

	JSON_RC rc;

	if(start<0)
		return JSON_RC_UNDEF;
	if(t[start].type!=JSMN_OBJECT)
		return JSON_RC_WRONG_TYPE;

	int i = start + 1;
	int fields = 0;

	while(i < n && t[i].end<=t[start].end){
		if(jsoneq(js->data, &t[i], "Status")==0){
			i++;
			rc = tok2Status(js->data, &t[i], status);
			if(rc)
				return rc;
			fields |= 1;
		}
		i++;
	}

	if(fields!=1)
		return JSON_RC_MISSING_FIELD;

	return JSON_RC_OK;
}

JSON_RC jsonparseCreateAASReq(createAASReq_t* cAasReq, const jsmntok_t* t, const SRV_String* js, int n, int start){

	JSON_RC rc;

	if(start<0)
		return JSON_RC_UNDEF;
	if(t[start].type!=JSMN_OBJECT)
		return JSON_RC_WRONG_TYPE;

	createAASReq_t_init(cAasReq);
	int i = start + 1;
	int fields = 0;

	while(i < n && t[i].end<=t[start].end){
		if(jsoneq(js->data, &t[i], "ASSId")==0){
			i++;
			rc = jsonparseId(t, js, i, n, &cAasReq->aasId, &i);
			if(rc)
				return rc;
			fields |= 1;
		} if(jsoneq(js->data, &t[i], "AssetId")==0){
			i++;
			rc = jsonparseId(t, js, i, n, &cAasReq->assetId, &i);
			if(rc)
				return rc;
			fields |= 2;
		} else if(jsoneq(js->data, &t[i], "Name")==0){
			i++;
			rc = tok2SrvStr(js->data, &t[i], &cAasReq->aasName);
			if(rc)
				return rc;
			fields |= 4;
		}
		i++;
	}

	if(fields!=7)
		return JSON_RC_MISSING_FIELD;

	return JSON_RC_OK;
}

JSON_RC jsonparseCreateAASRsp(createAASRsp_t* cAasRsp, const jsmntok_t* t, const SRV_String* js, int n, int start){

	createAASRsp_t_init(cAasRsp);
	return jsonparseStatusRsp(&cAasRsp->status, t, js, n, start);
}

JSON_RC jsonparseDeleteAASReq(deleteAASReq_t* dAasReq, const jsmntok_t* t, const SRV_String* js, int n, int start){

	JSON_RC rc;

	if(start<0)
		return JSON_RC_UNDEF;
	if(t[start].type!=JSMN_OBJECT)
		return JSON_RC_WRONG_TYPE;

	deleteAASReq_t_init(dAasReq);
	int i = start + 1;
	int fields = 0;

	while(i < n && t[i].end<=t[start].end){
		if(jsoneq(js->data, &t[i], "AASId")==0){
			i++;
			rc = jsonparseId(t, js, i, n, &dAasReq->aasId, &i);
			if(rc)
				return rc;
			fields |= 1;
		}
		i++;
	}

	if(fields!=1)
		return JSON_RC_MISSING_FIELD;

	return JSON_RC_OK;
}

JSON_RC jsonparseDeleteAASRsp(deleteAASRsp_t* dAasRsp, const jsmntok_t* t, const SRV_String* js, int n, int start){

	deleteAASRsp_t_init(dAasRsp);
	return jsonparseStatusRsp(&dAasRsp->status, t, js, n, start);
}

JSON_RC jsonparseCreatePVSLReq(createPVSLReq_t* cPvslReq, const jsmntok_t* t, const SRV_String* js, int n, int start){

	JSON_RC rc;

	if(start<0)
		return JSON_RC_UNDEF;
	if(t[start].type!=JSMN_OBJECT)
		return JSON_RC_WRONG_TYPE;

	createPVSLReq_t_init(cPvslReq);
	int i = start + 1;
	int fields = 0;

	while(i < n && t[i].end<=t[start].end){
		if(jsoneq(js->data, &t[i], "PVSLName")==0){
			i++;
			rc = tok2SrvStr(js->data, &t[i], &cPvslReq->pvslName);
			if(rc)
				return rc;
			fields |= 1;
		} else if(jsoneq(js->data, &t[i+1], "Carrier")==0){
			i++;
			rc = jsonparseId(t, js, i, n, &cPvslReq->carrier, &i);
			if(rc)
				return rc;
			fields |= 2;
		}
		i++;
	}

	if(fields!=3)
		return JSON_RC_MISSING_FIELD;

	return JSON_RC_OK;
}

JSON_RC jsonparseCreatePVSLRsp(createPVSLRsp_t* cPvslRsp, const jsmntok_t* t, const SRV_String* js, int n, int start){

	createPVSLRsp_t_init(cPvslRsp);
	return jsonparseStatusRsp(&cPvslRsp->status, t, js, n, start);
}

JSON_RC jsonparseDeletePVSLReq(deletePVSLReq_t* dPvslReq, const jsmntok_t* t, const SRV_String* js, int n, int start){

	JSON_RC rc;

	if(start<0)
		return JSON_RC_UNDEF;
	if(t[start].type!=JSMN_OBJECT)
		return JSON_RC_WRONG_TYPE;

	deletePVSLReq_t_init(dPvslReq);
	int i = start + 1;
	int fields = 0;

	while(i < n && t[i].end<=t[start].end){
		if(jsoneq(js->data, &t[i], "PVSLName")==0){
			i++;
			rc = tok2SrvStr(js->data, &t[i], &dPvslReq->pvslName);
			if(rc)
				return rc;
			fields |= 1;
		}
		i++;
	}

	if(fields!=1)
		return JSON_RC_MISSING_FIELD;

	return JSON_RC_OK;
}

JSON_RC jsonparseDeletePVSLRsp(deletePVSLRsp_t* dPvslRsp, const jsmntok_t* t, const SRV_String* js, int n, int start){

	deletePVSLRsp_t_init(dPvslRsp);
	return jsonparseStatusRsp(&dPvslRsp->status, t, js, n, start);
}

JSON_RC jsonparseCreatePVSReq(createPVSReq_t* cPvsReq, const jsmntok_t* t, const SRV_String* js, int n, int start){

	JSON_RC rc;

	if(start<0)
		return JSON_RC_UNDEF;
	if(t[start].type!=JSMN_OBJECT)
		return JSON_RC_WRONG_TYPE;

	createPVSReq_t_init(cPvsReq);
	int i = start + 1;
	int fields = 0;

	while(i < n && t[i].end<=t[start].end){
		if(jsoneq(js->data, &t[i], "PVSLName")==0){
			i++;
			rc = tok2SrvStr(js->data, &t[i], &cPvsReq->pvslName);
			if(rc)
				return rc;
			fields |= 1;
		} else if(jsoneq(js->data, &t[i], "PVS")==0){
			i++;
			rc = jsonparsePVS(t, js, i, n, &cPvsReq->pvs, &i);
			if(rc)
				return rc;
			if(cPvsReq->pvs.hasName && cPvsReq->pvs.hasUnit &&
					cPvsReq->pvs.expressionSemantic!=SRV_ES_undefined &&
					cPvsReq->pvs.ID.idType!=SRV_IDT_undefined &&
					cPvsReq->pvs.expressionLogic!=SRV_EL_undefined &&
					cPvsReq->pvs.valType!=SRV_VT_undefined &&
					cPvsReq->pvs.view!=SRV_VIEW_undefined)
				fields |= 2;
		}
		i++;
	}

	if(fields!=3)
		return JSON_RC_MISSING_FIELD;

	return JSON_RC_OK;
}

JSON_RC jsonparseCreatePVSRsp(createPVSRsp_t* cPvsRsp, const jsmntok_t* t, const SRV_String* js, int n, int start){

	createPVSRsp_t_init(cPvsRsp);
	return jsonparseStatusRsp(&cPvsRsp->status, t, js, n, start);
}

JSON_RC jsonparseDeletePVSReq(deletePVSReq_t* dPvsReq, const jsmntok_t* t, const SRV_String* js, int n, int start){
	JSON_RC rc;

	if(start<0)
		return JSON_RC_UNDEF;
	if(t[start].type!=JSMN_OBJECT)
		return JSON_RC_WRONG_TYPE;

	deletePVSReq_t_init(dPvsReq);
	int i = start + 1;
	int fields = 0;

	while(i < n && t[i].end<=t[start].end){
		if(jsoneq(js->data, &t[i], "PVSLName")==0){
			i++;
			rc = tok2SrvStr(js->data, &t[i], &dPvsReq->pvslName);
			if(rc)
				return rc;
			fields |= 1;
		} else if(jsoneq(js->data, &t[i], "PVSName")==0){
			i++;
			rc = tok2SrvStr(js->data, &t[i], &dPvsReq->pvsName);
			if(rc)
				return rc;
			fields |= 2;
		}
		i++;
	}

	if(fields!=3)
		return JSON_RC_MISSING_FIELD;

	return JSON_RC_OK;
}

JSON_RC jsonparseDeletePVSRsp(deletePVSRsp_t* dPvsRsp, const jsmntok_t* t, const SRV_String* js, int n, int start){

	deletePVSRsp_t_init(dPvsRsp);
	return jsonparseStatusRsp(&dPvsRsp->status, t, js, n, start);
}

JSON_RC jsonparseCreateLCEReq(createLCEReq_t* cLceReq, const jsmntok_t* t, const SRV_String* js, int n, int start){
	JSON_RC rc;

	if(start<0)
		return JSON_RC_UNDEF;
	if(t[start].type!=JSMN_OBJECT)
		return JSON_RC_WRONG_TYPE;

	createLCEReq_t_init(cLceReq);
	int i = start + 1;
	int fields = 0;

	while(i < n && t[i].end<=t[start].end){
		if(jsoneq(js->data, &t[i], "LCE")==0){
			i++;
			rc = jsonparseLCE(t, js, i, n, &cLceReq->lce, &i);
			if(rc)
				return rc;
			// check if all necessary sub fields are set
			if(cLceReq->lce.hasEventClass && cLceReq->lce.hasSubject && cLceReq->lce.hastDataTime &&
					cLceReq->lce.creatingInstance.idType!=SRV_IDT_undefined &&
					cLceReq->lce.writingInstance.idType!=SRV_IDT_undefined &&
					cLceReq->lce.dataType!=SRV_VT_undefined)
				fields |= 1;
		}
		i++;
	}

	if(fields!=1)
		return JSON_RC_MISSING_FIELD;

	return JSON_RC_OK;
}

JSON_RC jsonparseCreateLCERsp(createLCERsp_t* cLceRsp, const jsmntok_t* t, const SRV_String* js, int n, int start){

	createLCERsp_t_init(cLceRsp);
	return jsonparseStatusRsp(&cLceRsp->status, t, js, n, start);
}

JSON_RC jsonparseDeleteLCEReq(deleteLCEReq_t* dLceReq, const jsmntok_t* t, const SRV_String* js, int n, int start){
	JSON_RC rc;

	if(start<0)
		return JSON_RC_UNDEF;
	if(t[start].type!=JSMN_OBJECT)
		return JSON_RC_WRONG_TYPE;

	deleteLCEReq_t_init(dLceReq);
	int i = start + 1;
	int fields = 0;

	while(i < n && t[i].end<=t[start].end){
		if(jsoneq(js->data, &t[i], "LCEId")==0){
			i++;
			rc = tok2Int(js->data, &t[i], &dLceReq->lceId, 0);
			if(rc)
				return rc;
			fields |= 1;
		}
		i++;
	}

	if(fields!=1)
		return JSON_RC_MISSING_FIELD;

	return JSON_RC_OK;
}

JSON_RC jsonparseDeleteLCERsp(deleteLCERsp_t* dLceRsp, const jsmntok_t* t, const SRV_String* js, int n, int start){

	deleteLCERsp_t_init(dLceRsp);
	return jsonparseStatusRsp(&dLceRsp->status, t, js, n, start);
}

JSON_RC jsonparseGetPVSReq(getPVSReq_t* gPvsReq, const jsmntok_t* t, const SRV_String* js, int n, int start){
	JSON_RC rc;

	if(start<0)
		return JSON_RC_UNDEF;
	if(t[start].type!=JSMN_OBJECT)
		return JSON_RC_WRONG_TYPE;

	getPVSReq_t_init(gPvsReq);
	int i = start+1;
	int fields = 0;

	while(i < n && t[i].end<=t[start].end){
		if(jsoneq(js->data, &t[i], "PVSLName")==0){
			i++;
			rc = tok2SrvStr(js->data, &t[i], &gPvsReq->pvslName);
			if(rc)
				return rc;
			fields |= 1;
		} else if(jsoneq(js->data, &t[i], "PVSName")==0){
			i++;
			rc = tok2SrvStr(js->data, &t[i], &gPvsReq->pvsName);
			if(rc)
				return rc;
			fields |= 2;
		}
		i++;
	}

	if(fields!=3)
		return JSON_RC_MISSING_FIELD;

	return JSON_RC_OK;
}

JSON_RC jsonparseGetPVSRsp(getPVSRsp_t* gPvsRsp, const jsmntok_t* t, const SRV_String* js, int n, int start){

	JSON_RC rc;

	if(start<0)
		return JSON_RC_UNDEF;
	if(t[start].type!=JSMN_OBJECT)
		return JSON_RC_WRONG_TYPE;

	getPVSRsp_t_init(gPvsRsp);
	int i = start + 1;
	int fields = 0;

	while(i < n && t[i].end<=t[start].end){
		if(jsoneq(js->data, &t[i], "Status")==0){
			i++;
			rc = tok2Status(js->data, &t[i], &gPvsRsp->status);
			if(rc)
				return rc;
			fields |= 1;
		} else if(jsoneq(js->data, &t[i], "PVS")==0){
			i++;
			rc = jsonparsePVS(t, js, i, n, &gPvsRsp->pvs, &i);
			if(rc)
				return rc;
			if(gPvsRsp->pvs.hasName && gPvsRsp->pvs.hasUnit && gPvsRsp->pvs.valType!=SRV_VT_undefined &&
					gPvsRsp->pvs.ID.idType!=SRV_IDT_undefined &&
					gPvsRsp->pvs.expressionSemantic!=SRV_ES_undefined &&
					gPvsRsp->pvs.expressionLogic!=SRV_EL_undefined &&
					gPvsRsp->pvs.view!=SRV_VIEW_undefined)
				fields |= 2;

		}
		i++;
	}

	if(fields!=3)
		return JSON_RC_MISSING_FIELD;

	return JSON_RC_OK;
}

JSON_RC jsonparseSetPVSReq(setPVSReq_t* sPvsReq, const jsmntok_t* t, const SRV_String* js, int n, int start){
	JSON_RC rc;

	if(start<0)
		return JSON_RC_UNDEF;
	if(t[start].type!=JSMN_OBJECT)
		return JSON_RC_WRONG_TYPE;


	setPVSReq_t_init(sPvsReq);
	int i = start+1;
	int fields = 0;

	while(i < n && t[i].end<=t[start].end){
		if(jsoneq(js->data, &t[i], "PVSLName")==0){
			i++;
			rc = tok2SrvStr(js->data, &t[i], &sPvsReq->pvslName);
			if(rc)
				return rc;
			fields |= 1;
		/*} else if(jsoneq(js->data, &t[i], "PVSName")==0){
			i++;
			// which name
			rc = tok2SrvStr(js->data, &t[i], &sPvsReq->pvsName);
			if(rc)
				return rc;
			fields |= 2;*/
		} else if(jsoneq(js->data, &t[i], "PVS")==0){
			i++;
			rc = jsonparsePVS(t, js, i, n, &sPvsReq->pvs, &i);
			if(rc)
				return rc;
			// check if all necessary sub fields are set
			if(sPvsReq->pvs.hasName && sPvsReq->pvs.hasUnit &&
					sPvsReq->pvs.ID.idType!=SRV_IDT_undefined &&
					sPvsReq->pvs.expressionSemantic!=SRV_ES_undefined &&
					sPvsReq->pvs.expressionLogic!=SRV_EL_undefined &&
					sPvsReq->pvs.valType!=SRV_VT_undefined &&
					sPvsReq->pvs.view!=SRV_VIEW_undefined)
				fields |= 2;
		}
		i++;
	}

	if(fields!=3)
		return JSON_RC_MISSING_FIELD;

	return JSON_RC_OK;
}

JSON_RC jsonparseSetPVSRsp(setPVSRsp_t* sPvsRsp, const jsmntok_t* t, const SRV_String* js, int n, int start){

	setPVSRsp_t_init(sPvsRsp);
	return jsonparseStatusRsp(&sPvsRsp->status, t, js, n, start);
}

JSON_RC jsonparseGetLCEReq(getLCEReq_t* gLceReq, const jsmntok_t* t, const SRV_String* js, int n, int start){
	JSON_RC rc;

	if(start<0)
		return JSON_RC_UNDEF;
	if(t[start].type!=JSMN_OBJECT)
		return JSON_RC_WRONG_TYPE;

	getLCEReq_t_init(gLceReq);
	int i = start + 1;
	int fields = 0;

	while(i < n && t[i].end<=t[start].end){
		if(jsoneq(js->data, &t[i], "LCEId")==0){
			i++;
			rc = tok2Int(js->data, &t[i], &gLceReq->lceId, 0);
			if(rc)
				return rc;
			fields |= 1;
		}
		i++;
	}

	if(fields!=1)
		return JSON_RC_MISSING_FIELD;

	return JSON_RC_OK;
}

JSON_RC jsonparseGetLCERsp(getLCERsp_t* gLceRsp, const jsmntok_t* t, const SRV_String* js, int n, int start){

	JSON_RC rc;

	if(start<0)
		return JSON_RC_UNDEF;
	if(t[start].type!=JSMN_OBJECT)
		return JSON_RC_WRONG_TYPE;

	getLCERsp_t_init(gLceRsp);
	int i = start + 1;
	int fields = 0;

	while(i < n && t[i].end<=t[start].end){
		if(jsoneq(js->data, &t[i], "Status")==0){
			i++;
			rc = tok2Status(js->data, &t[i], &gLceRsp->status);
			if(rc)
				return rc;
			fields |= 1;
		} else if(jsoneq(js->data, &t[i], "")==0){
			i++;
			rc = jsonparseLCE(t, js, i, n, &gLceRsp->lce, &i);
			if(rc)
				return rc;
			if(gLceRsp->lce.hasEventClass && gLceRsp->lce.hasLceId && gLceRsp->lce.hasSubject &&
					gLceRsp->lce.hastDataTime && gLceRsp->lce.dataType!=SRV_VT_undefined &&
					gLceRsp->lce.creatingInstance.idType!=SRV_IDT_undefined &&
					gLceRsp->lce.writingInstance.idType!=SRV_IDT_undefined)
				fields |= 2;
		}
		i++;
	}

	if(fields!=3)
		return JSON_RC_MISSING_FIELD;

	return JSON_RC_OK;
}

JSON_RC jsonparseSetLCEReq(setLCEReq_t* sLceReq, const jsmntok_t* t, const SRV_String* js, int n, int start){

	JSON_RC rc;

	if(start<0)
		return JSON_RC_UNDEF;
	if(t[start].type!=JSMN_OBJECT)
		return JSON_RC_WRONG_TYPE;

	setLCEReq_t_init(sLceReq);
	int i = start + 1;
	int fields = 0;

	while(i < n && t[i].end<=t[start].end){
		/*if(jsoneq(js->data, &t[i], "LCEId")==0){
			i++;
			// which LCEId
			rc = tok2Int(js->data, &t[i], &sLceReq->lceId, 0);
			if(rc)
				return rc;
			fields |= ;
		} else*/
		if(jsoneq(js->data, &t[i], "LCE")==0){
			i++;
			rc = jsonparseLCE(t, js, i, n, &sLceReq->lce, &i);
			if(rc)
				return rc;
			if(sLceReq->lce.hasLceId && sLceReq->lce.hasEventClass && sLceReq->lce.hasSubject &&
					sLceReq->lce.hastDataTime &&
					sLceReq->lce.creatingInstance.idType!=SRV_IDT_undefined &&
					sLceReq->lce.writingInstance.idType!=SRV_IDT_undefined &&
					sLceReq->lce.dataType!=SRV_VT_undefined)
			fields |= 1;
		}
		i++;
	}

	if(fields!=1)
		return JSON_RC_MISSING_FIELD;

	return JSON_RC_OK;
}

JSON_RC jsonparseSetLCERsp(setLCERsp_t* sLceRsp, const jsmntok_t* t, const SRV_String* js, int n, int start){

	setLCERsp_t_init(sLceRsp);
	return jsonparseStatusRsp(&sLceRsp->status, t, js, n, start);
}

JSON_RC jsonparseGetCoreDataReq(getCoreDataReq_t* gCDReq, const jsmntok_t* t, const SRV_String* js, int n, int start){
	//nothing to do;
	return JSON_RC_OK;
}

JSON_RC jsonparseGetCoreDataRsp(getCoreDataRsp_t* gCDRsp, const jsmntok_t* t, const SRV_String* js, int n, int start){

	JSON_RC rc;

	if(start<0)
		return JSON_RC_UNDEF;
	if(t[start].type!=JSMN_OBJECT)
		return JSON_RC_WRONG_TYPE;

	getCoreDataRsp_t_init(gCDRsp);
	int i = start + 1;
	int fields = 0;

	while(i < n && t[i].end<=t[start].end){
		if(jsoneq(js->data, &t[i], "Status")==0){
			i++;
			rc = tok2Status(js->data, &t[i], &gCDRsp->status);
			if(rc)
				return rc;
			fields |=1;
		} else if(jsoneq(js->data, &t[i], "PVSL")==0){
			i++;
			if(t[i].type==JSMN_ARRAY){
				gCDRsp->numPvsl = t[i].size;
				i++;
			} else {
				gCDRsp->numPvsl = 1;
			}
			gCDRsp->pvsl = malloc(sizeof(PVSL_t)*gCDRsp->numPvsl);
			for(uint k = 0; k<gCDRsp->numPvsl; k++){
				rc = jsonparsePVSL(t, js, i, n, &gCDRsp->pvsl[k], &i);
				if(rc)
					return rc;
				i++;
			}
			fields |= 2;
			i--;// one to far
		}
		i++;
	}

	if(fields!=3)
		return JSON_RC_MISSING_FIELD;

	return JSON_RC_OK;
}

JSON_RC jsonGenGeneric(SRV_String* json, int* length,const SRV_msgHeader* header, const void* srvStruct, SRV_service_t srvType){

	JSON_RC rc;

	if(json && (*length<0 || json->length<(uint)*length))
		return JSON_RC_NO_SPACE;

	int pos = 0;
	int first = true;

	//build header
	pos = json_append(json, pos, "{", 1, *length);
	pos = json_appendKey(json, pos, "msgNo", -1, *length, &first);
	pos = json_appendAny(json, pos, &header->msgNo, SRV_VT_INT32, *length);

	pos = json_appendKey(json, pos, "sender", -1, *length, &first);
	rc = jsonGenId(json, length, &pos, &header->sender);
	if(rc)
		return rc;

	pos = json_appendKey(json, pos, "receiver", -1, *length, &first);
	rc = jsonGenId(json, length, &pos, &header->receiver);
	if(rc)
		return rc;

	pos = json_append(json, pos, ",", 1, *length);

	int subLen = *length - pos -1;
	SRV_String* subJson = NULL;
	if(json){
		subJson = SRV_String_new();
		subJson->data = json->data+pos;
		subJson->length = subLen;
	}

	switch (srvType){
	case SRV_createAASReq:
		rc = jsonGenCreateAASReq(subJson, &subLen, srvStruct);
		break;
	case SRV_createAASRsp:
		rc = jsonGenCreateAASRsp(subJson, &subLen, srvStruct);
		break;
	case SRV_deleteAASReq:
		rc = jsonGenDeleteAASReq(subJson, &subLen, srvStruct);
		break;
	case SRV_deleteAASRsp:
		rc = jsonGenDeleteAASRsp(subJson, &subLen, srvStruct);
		break;
	case SRV_createPVSLReq:
		rc = jsonGenCreatePVSLReq(subJson, &subLen, srvStruct);
		break;
	case SRV_createPVSLRsp:
		rc = jsonGenCreatePVSLRsp(subJson, &subLen, srvStruct);
		break;
	case SRV_deletePVSLReq:
		rc = jsonGenDeletePVSLReq(subJson, &subLen, srvStruct);
		break;
	case SRV_deletePVSLRsp:
		rc = jsonGenDeletePVSLRsp(subJson, &subLen, srvStruct);
		break;
	case SRV_createPVSReq:
		rc = jsonGenCreatePVSReq(subJson, &subLen, srvStruct);
		break;
	case SRV_createPVSRsp:
		rc = jsonGenCreatePVSRsp(subJson, &subLen, srvStruct);
		break;
	case SRV_deletePVSReq:
		rc = jsonGenDeletePVSReq(subJson, &subLen, srvStruct);
		break;
	case SRV_deletePVSRsp:
		rc = jsonGenDeletePVSRsp(subJson, &subLen, srvStruct);
		break;
	case SRV_createLCEReq:
		rc = jsonGenCreateLCEReq(subJson, &subLen, srvStruct);
		break;
	case SRV_createLCERsp:
		rc = jsonGenCreateLCERsp(subJson, &subLen, srvStruct);
		break;
	case SRV_deleteLCEReq:
		rc = jsonGenDeleteLCEReq(subJson, &subLen, srvStruct);
		break;
	case SRV_deleteLCERsp:
		rc = jsonGenDeleteLCERsp(subJson, &subLen, srvStruct);
		break;
	case SRV_getPVSReq:
		rc = jsonGenGetPVSReq(subJson, &subLen, srvStruct);
		break;
	case SRV_getPVSRsp:
		rc = jsonGenGetPVSRsp(subJson, &subLen, srvStruct);
		break;
	case SRV_setPVSReq:
		rc = jsonGenSetPVSReq(subJson, &subLen, srvStruct);
		break;
	case SRV_setPVSRsp:
		rc = jsonGenSetPVSRsp(subJson, &subLen, srvStruct);
		break;
	case SRV_getLCEReq:
		rc = jsonGenGetLCEReq(subJson, &subLen, srvStruct);
		break;
	case SRV_getLCERsp:
		rc = jsonGenGetLCERsp(subJson, &subLen, srvStruct);
		break;
	case SRV_setLCEReq:
		rc = jsonGenSetLCEReq(subJson, &subLen, srvStruct);
		break;
	case SRV_setLCERsp:
		rc = jsonGenSetLCERsp(subJson, &subLen, srvStruct);
		break;
	case SRV_getCoreDataReq:
		rc = jsonGenGetCoreDataReq(subJson, &subLen, srvStruct);
		break;
	case SRV_getCoreDataRsp:
		rc = jsonGenGetCoreDataRsp(subJson, &subLen, srvStruct);
		break;
	case SRV_undefined:
		return JSON_RC_WRONG_VALUE;
		break;
	}

	pos += subLen;

	pos = json_append(json, pos, "}", 1, *length);

	if(pos<0)
		return pos;

	if(!json)
		*length = pos;

	return rc;
}

JSON_RC jsonGenPVSL(SRV_String* json, int* length, int* ppos, const PVSL_t* p){

	JSON_RC rc;
	int pos = *ppos;
	int first = 1;

	pos = json_append(json, pos, "{", 1, *length);

	if(p->hasName){
		pos = json_appendKey(json, pos, "Name", -1, *length, &first);
		pos = json_appendSrvStr(json, pos, &p->name, *length);
	}
	if(p->carrier.idType!=SRV_IDT_undefined){
		pos = json_appendKey(json, pos, "Carrier", -1, *length, &first);
		rc = jsonGenId(json, length, &pos, &p->carrier);
		if(rc)
			return rc;
	}
	if(p->creatingInstance.idType!=SRV_IDT_undefined){
		pos = json_appendKey(json, pos, "CreatingInstance", -1, *length, &first);
		rc = jsonGenId(json, length, &pos, &p->creatingInstance);
		if(rc)
			return rc;
	}
	if(p->hasCreationTime){
		pos = json_appendKey(json, pos, "CreationTime", -1, *length, &first);
		pos = json_appendIsoTime(json, pos, *length, p->creationTime);
	}
	pos = json_appendKey(json, pos, "PVS", 3, *length, &first);
	pos = json_append(json, pos, "[", 1, *length);
	if(p->pvs){
		for(uint k = 0; k < p->numPvs ; k++){
			if(k!=0)
				pos = json_append(json, pos, ",", 1, *length);
			rc = jsonGenPVS(json, length, &pos, &p->pvs[k]);
			if(rc)
				return rc;
		}
	}

	pos = json_append(json, pos, "]}", 2, *length);

	if(pos<0)
		return pos;

	*ppos = pos;

	return JSON_RC_OK;
}

JSON_RC jsonGenPVS(SRV_String* json, int* length, int* ppos, const PVS_t* p){
	int pos = *ppos;
	int first = 1;
	JSON_RC rc;

	pos = json_append(json, pos, "{",-1, *length);

	if(p->hasName){
		pos = json_appendKey(json, pos, "Name", -1, *length, &first);
		pos = json_appendSrvStr(json, pos, &p->name, *length);
	}
	if(p->expressionSemantic!=SRV_ES_undefined){
		pos = json_appendKey(json, pos, "ExpressionSemantic", -1, *length, &first);
		pos = json_appendSrvStr(json, pos, &JSON_ES_STR[p->expressionSemantic], *length);
	}
	if(p->expressionLogic!=SRV_EL_undefined){
		pos = json_appendKey(json, pos, "ExpressionLogic", -1, *length, &first);
		pos = json_appendSrvStr(json, pos, &JSON_EL_STR[p->expressionLogic], *length);
	}
	if(p->valType!=SRV_VT_undefined){
		pos = json_appendKey(json, pos, "ValueType", -1, *length, &first);
		pos = json_appendSrvStr(json, pos, &JSON_VT_STR[p->valType], *length);

		pos = json_appendKey(json, pos, "Value", -1, *length, &first);
		pos = json_appendAny(json, pos, p->value, p->valType, *length);

		if(p->hasValTime){
			pos = json_appendKey(json, pos, "ValueTime", 9, *length, &first);
			pos = json_appendIsoTime(json, pos, *length, p->valTime);
		}

	}
	if(p->hasUnit){
		pos = json_appendKey(json, pos, "Unit", -1, *length, &first);
		pos = json_appendSrvStr(json, pos, &p->unit, *length);
	}
	if(p->ID.idType!=SRV_IDT_undefined){
		pos = json_appendKey(json, pos, "ID", -1, *length, &first);
		rc = jsonGenId(json, length, &pos, &p->ID);
		if(rc)
			return rc;
	}
	if(p->view!=SRV_VIEW_undefined){
		pos = json_appendKey(json, pos, "View", -1, *length, &first);
		pos = json_appendSrvStr(json, pos, &JSON_VIEW_STR[p->view], *length);
	}
	if(p->visibility!=SRV_VIS_undefined){
		pos = json_appendKey(json, pos, "Visibility", 10, *length, &first);
		pos = json_appendSrvStr(json, pos, &JSON_VIS_STR[p->visibility], *length);
	}

	pos = json_append(json, pos, "}",-1, *length);

	if(pos<0)
		return pos;

	*ppos = pos;
	return JSON_RC_OK;
}

JSON_RC jsonGenLCE(SRV_String* json, int* length, int* ppos, const LCE_t* l){

	int pos = *ppos;
	int first = 1;

	if(l->hasLceId){
		pos = json_appendKey(json, pos, "LCEId", -1, *length, &first);
		pos = json_appendAny(json, pos, &l->lceId, SRV_VT_INT64, *length);
	}
	if(l->creatingInstance.idType!=SRV_IDT_undefined){
		pos = json_appendKey(json, pos, "CreatingInstanceId", -1, *length, &first);
		pos = jsonGenId(json, length, &pos, &l->creatingInstance);
	}
	if(l->writingInstance.idType!=SRV_IDT_undefined){
		pos = json_appendKey(json, pos, "WritingInstanceId", -1, *length, &first);
		pos = jsonGenId(json, length, &pos, &l->writingInstance);
	}
	if(l->hasEventClass){
		pos = json_appendKey(json, pos, "EventClass", -1, *length, &first);
		pos = json_appendSrvStr(json, pos, &l->eventClass, *length);
	}
	if(l->hasSubject){
		pos = json_appendKey(json, pos, "Subject", -1, *length, &first);
		pos = json_appendSrvStr(json, pos, &l->subject, *length);
	}
	if(l->dataType!=SRV_VT_undefined){
		pos = json_appendKey(json, pos, "DataType", -1, *length, &first);
		pos = json_appendSrvStr(json, pos, &JSON_VT_STR[l->dataType], *length);

		pos = json_appendKey(json, pos, "Data", -1, *length, &first);
		pos = json_appendAny(json, pos, l->data, l->dataType, *length);

		//TODO data time without data?
		if(l->hastDataTime){
			pos = json_appendKey(json, pos, "DataTime", -1, *length, &first);
			pos = json_appendIsoTime(json, pos, *length, l->dataTime);
		}
	}


	if(pos<0)
		return pos;

	*ppos = pos;
	return JSON_RC_OK;
}

JSON_RC jsonGenId(SRV_String* json, int* length, int* ppos, const SRV_ident_t* i){

	int pos = *ppos;
	int first = 1;
	if(i->idType==SRV_IDT_undefined || !(i->idSpec.data))
		return JSON_RC_BAD_INPUT;

	pos = json_append(json, pos, "{", 1, *length);

	pos = json_appendKey(json, pos, "IdType", -1, *length, &first);
	pos = json_appendSrvStr(json, pos, &JSON_ID_STR[i->idType], *length);

	pos = json_appendKey(json, pos, "IdSpec", -1, *length, &first);
	pos = json_appendSrvStr(json, pos, &i->idSpec, *length);

	pos = json_append(json, pos, "}", 1, *length);

	if(pos<0)
		return pos;

	*ppos = pos;

	return JSON_RC_OK;
}

JSON_RC jsonGenStatusRsp(SRV_String* json, int* length, const char* serviceName, status_t status){

	int pos = 0;
	int first = 1;

	//add static structure
	pos = json_appendKey(json, pos, "serviceName", -1, *length, &first);
	pos = json_appendString(json, pos, serviceName, -1, *length);
	pos = json_appendKey(json, pos, "serviceParameter", -1, *length, &first);

	// add serviceParameter
	pos = json_append(json, pos, "{\"Status\":", -1, *length);
	pos = json_appendStatus(json, pos, &status, *length);
	pos = json_append(json, pos, "}", 1, *length);

	if(pos<0)
		return pos;

	if(!json)
		*length = pos;

	return JSON_RC_OK;
}

JSON_RC jsonGenCreateAASReq(SRV_String* json, int* length, const createAASReq_t* cAasReq){

	JSON_RC rc;
	int pos = 0;
	int first = 1;

	//add static structure
	pos = json_append(json, pos, "\"serviceName\":\"createAASReq\",\"serviceParameter\":{",-1 ,*length);

	// add serviceParameter
	pos = json_appendKey(json, pos, "AASName", -1, *length, &first);
	pos = json_appendSrvStr(json, pos, &cAasReq->aasName, *length);

	pos = json_appendKey(json, pos, "AASId", -1, *length, &first);
	rc = jsonGenId(json, length, &pos, &cAasReq->aasId);
	if(rc)
		return rc;

	pos = json_appendKey(json, pos, "AssetId", -1, *length, &first);
	rc = jsonGenId(json, length, &pos, &cAasReq->assetId);
	if(rc)
	return rc;

	pos = json_append(json, pos, "}", 1, *length);

	if(pos<0)
		return pos;

	if(!json)
		*length = pos;

	return JSON_RC_OK;
}

JSON_RC jsonGenCreateAASRsp(SRV_String* json, int* length, const createAASRsp_t* cAasRsp){

	return jsonGenStatusRsp(json, length, "createAASRsp", cAasRsp->status);
}

JSON_RC jsonGenDeleteAASReq(SRV_String* json, int* length, const deleteAASReq_t* dAasReq){

	JSON_RC rc;
	int pos = 0;

	//add static structure
	pos = json_append(json, pos, "\"serviceName\":\"deleteAASReq\",\"serviceParameter\":{",-1, *length);

	// add serviceParameter
	pos = json_appendKey(json, pos, "AASId", -1, *length, NULL);
	rc = jsonGenId(json, length, &pos, &dAasReq->aasId);
	if(rc)
		return rc;

	pos = json_append(json, pos, "}", 1, *length);

	if(pos<0)
		return pos;

	if(!json)
		*length = pos;

	return JSON_RC_OK;
}

JSON_RC jsonGenDeleteAASRsp(SRV_String* json, int* length, const deleteAASRsp_t* dAasRsp){

	return jsonGenStatusRsp(json, length, "deleteAASRsp", dAasRsp->status);
}

JSON_RC jsonGenCreatePVSLReq(SRV_String* json, int* length, const createPVSLReq_t* cPvslReq){

	JSON_RC rc;
	int pos = 0;
	int first = 1;

	//add static structure
	pos = json_append(json, pos, "\"serviceName\":\"createPVSLReq\",\"serviceParameter\":{",-1 ,*length);

	// add serviceParameter
	pos = json_appendKey(json, pos, "PVSLName", -1, *length, &first);
	pos = json_appendSrvStr(json, pos, &cPvslReq->pvslName, *length);

	pos = json_appendKey(json, pos, "Carrier", -1, *length, &first);
	rc = jsonGenId(json, length, &pos, &cPvslReq->carrier);
	if(rc)
		return rc;

	pos = json_append(json, pos, "}", 1, *length);

	if(pos<0)
		return pos;

	if(!json)
		*length = pos;

	return JSON_RC_OK;
}

JSON_RC jsonGenCreatePVSLRsp(SRV_String* json, int* length, const createPVSLRsp_t* cPvslRsp){

	return jsonGenStatusRsp(json, length, "createPVSLRsp", cPvslRsp->status);
}

JSON_RC jsonGenDeletePVSLReq(SRV_String* json, int* length, const deletePVSLReq_t* dPvslReq){

	int pos = 0;

	//add static structure
	pos = json_append(json, pos, "\"serviceName\":\"deletePVSLReq\",\"serviceParameter\":{",-1 ,*length);

	// add serviceParameter
	pos = json_appendKey(json, pos, "PVSLName", 8, *length, NULL);
	pos = json_appendSrvStr(json, pos, &dPvslReq->pvslName, *length);

	pos = json_append(json, pos, "}", 1, *length);

	if(pos<0)
		return pos;

	if(!json)
		*length = pos;

	return JSON_RC_OK;
}

JSON_RC jsonGenDeletePVSLRsp(SRV_String* json, int* length, const deletePVSLRsp_t* dPvslRsp){

	return jsonGenStatusRsp(json, length, "deletePVSLRsp", dPvslRsp->status);
}

JSON_RC jsonGenCreatePVSReq(SRV_String* json, int* length, const createPVSReq_t* cPvsReq){

	JSON_RC rc;
	int pos = 0;
	int first = 1;

	//add static structure
	pos = json_append(json, pos, "\"serviceName\":\"createPVSReq\",\"serviceParameter\":{",-1 ,*length);

	// add serviceParameter
	pos = json_appendKey(json, pos, "PVSLName", 8, *length, &first);
	pos = json_appendSrvStr(json, pos, &cPvsReq->pvslName, *length);

	pos = json_appendKey(json, pos, "PVS", 3, *length, &first);
	rc = jsonGenPVS(json, length, &pos, &cPvsReq->pvs);
	if(rc)
		return rc;

	pos = json_append(json, pos, "}", 1, *length);

	if(pos<0)
		return pos;

	if(!json)
		*length = pos;

	return JSON_RC_OK;
}

JSON_RC jsonGenCreatePVSRsp(SRV_String* json, int* length, const createPVSRsp_t* cPvsRsp){

	return jsonGenStatusRsp(json, length, "createPVSRsp", cPvsRsp->status);
}

JSON_RC jsonGenDeletePVSReq(SRV_String* json, int* length, const deletePVSReq_t* dPvsReq){

	int pos = 0;
	int first = 1;

	//add static structure
	pos = json_append(json, pos, "\"serviceName\":\"deletePVSReq\",\"serviceParameter\":{",-1 ,*length);

	// add serviceParameter
	pos = json_appendKey(json, pos, "PVSLName", 8, *length, &first);
	pos = json_appendSrvStr(json, pos, &dPvsReq->pvslName, *length);

	pos = json_appendKey(json, pos, "PVSName", 7, *length, &first);
	pos = json_appendSrvStr(json, pos, &dPvsReq->pvsName, *length);

	pos = json_append(json, pos, "}", 1, *length);

	if(pos<0)
		return pos;

	if(!json)
		*length = pos;

	return JSON_RC_OK;
}

JSON_RC jsonGenDeletePVSRsp(SRV_String* json, int* length, const deletePVSRsp_t* dPvsRsp){

	return jsonGenStatusRsp(json, length, "deletePVSRsp", dPvsRsp->status);
}

JSON_RC jsonGenCreateLCEReq(SRV_String* json, int* length, const createLCEReq_t* cLceReq){

	JSON_RC rc;
	int pos = 0;

	//add static structure
	pos = json_append(json, pos, "\"serviceName\":\"createLCEReq\",\"serviceParameter\":{",-1 ,*length);

	// add serviceParameter
	pos = json_appendKey(json, pos, "LCE", 3, *length, NULL);
	rc = jsonGenLCE(json, length, &pos, &cLceReq->lce);
	if(rc)
		return rc;

	pos = json_append(json, pos, "}", 1, *length);

	if(pos<0)
		return pos;

	if(!json)
		*length = pos;

	return JSON_RC_OK;
}

JSON_RC jsonGenCreateLCERsp(SRV_String* json, int* length, const createLCERsp_t* cLceRsp){

	return jsonGenStatusRsp(json, length, "createLCERsp", cLceRsp->status);
}

JSON_RC jsonGenDeleteLCEReq(SRV_String* json, int* length, const deleteLCEReq_t* dLceReq){

	int pos = 0;

	//add static structure
	pos = json_append(json, pos, "\"serviceName\":\"deleteLCEReq\",\"serviceParameter\":{",-1 ,*length);

	// add serviceParameter
	pos = json_appendKey(json, pos, "LCEId", 5, *length, NULL);
	pos = json_appendAny(json, pos, &dLceReq->lceId, SRV_VT_INT64, *length);

	pos = json_append(json, pos, "}", 1, *length);

	if(pos<0)
		return pos;

	if(!json)
		*length = pos;

	return JSON_RC_OK;
}

JSON_RC jsonGenDeleteLCERsp(SRV_String* json, int* length, const deleteLCERsp_t* dLceRsp){

	return jsonGenStatusRsp(json, length, "deleteLCERsp", dLceRsp->status);
}

JSON_RC jsonGenGetPVSReq(SRV_String* json, int* length, const getPVSReq_t* gPvsReq){

	int pos = 0;
	int first = 1;

	//add static structure
	pos = json_append(json, pos, "\"serviceName\":\"getPVSReq\",\"serviceParameter\":{",-1 ,*length);

	// add serviceParameter
	pos = json_appendKey(json, pos, "PVSLName", 8, *length, &first);
	pos = json_appendSrvStr(json, pos, &gPvsReq->pvslName, *length);

	pos = json_appendKey(json, pos, "PVSName", 7, *length, &first);
	pos = json_appendSrvStr(json, pos, &gPvsReq->pvsName, *length);

	pos = json_append(json, pos, "}", 1, *length);

	if(pos<0)
		return pos;

	if(!json)
		*length = pos;

	return JSON_RC_OK;
}

JSON_RC jsonGenGetPVSRsp(SRV_String* json, int* length, const getPVSRsp_t* gPvsRsp) {

	JSON_RC rc;
	int pos = 0;

	//add static structure
	pos = json_append(json, pos, "\"serviceName\":\"getPVSRsp\",\"serviceParameter\":",-1 ,*length);

	// add serviceParameter
	pos = json_append(json, pos, "{\"Status\":", -1, *length);

	pos = json_appendStatus(json, pos, &gPvsRsp->status, *length);

	pos = json_append(json, pos, ",\"PVS\":",-1, *length);

	rc = jsonGenPVS(json, length, &pos, &gPvsRsp->pvs);
	if(rc)
		return rc;

	pos = json_append(json, pos, "}",-1,*length);

	if(pos<0)
		return pos;

	if(!json)
		*length = pos;

	return JSON_RC_OK;
}

JSON_RC jsonGenSetPVSReq(SRV_String* json, int* length, const setPVSReq_t* sPvsReq){

	JSON_RC rc;
	int pos = 0;
	int first = 1;

	//add static structure
	pos = json_append(json, pos, "\"serviceName\":\"setPVSReq\",\"serviceParameter\":{",-1 ,*length);

	// add serviceParameter
	pos = json_appendKey(json, pos, "PVSLName", 8, *length, &first);
	pos = json_appendSrvStr(json, pos, &sPvsReq->pvslName, *length);

	//pos = json_appendKey(json, pos, "PVSName", 7, *length, &first);
	//pos = json_appendSrvStr(json, pos, &sPvsReq->pvsName, *length);

	pos = json_appendKey(json, pos, "PVS", 3, *length, &first);
	rc = jsonGenPVS(json, length, &pos, &sPvsReq->pvs);
	if(rc)
		return rc;

	pos = json_append(json, pos, "}", 1, *length);

	if(pos<0)
		return pos;

	if(!json)
		*length = pos;

	return JSON_RC_OK;
}

JSON_RC jsonGenSetPVSRsp(SRV_String* json, int* length, const setPVSRsp_t* sPvsRsp) {

	return jsonGenStatusRsp(json, length, "setPVSRsp", sPvsRsp->status);
}

JSON_RC jsonGenGetLCEReq(SRV_String* json, int* length, const getLCEReq_t* gLceReq){

	int pos = 0;

	//add static structure
	pos = json_append(json, pos, "\"serviceName\":\"getLCEReq\",\"serviceParameter\":{",-1 ,*length);

	// add serviceParameter
	pos = json_appendKey(json, pos, "LCEId", 5, *length, NULL);
	pos = json_appendAny(json, pos, &gLceReq->lceId, SRV_VT_INT64, *length);

	pos = json_append(json, pos, "}", 1, *length);

	if(pos<0)
		return pos;

	if(!json)
		*length = pos;

	return JSON_RC_OK;
}

JSON_RC jsonGenGetLCERsp(SRV_String* json, int* length, const getLCERsp_t* gLceRsp){

	JSON_RC rc;
	int pos = 0;

	//add static structure
	pos = json_append(json, pos, "\"serviceName\":\"getLCERsp\",\"serviceParameter\":",-1 ,*length);

	// add serviceParameter
	pos = json_append(json, pos, "{\"Status\":", -1, *length);
	pos = json_appendStatus(json, pos, &gLceRsp->status, *length);

	pos = json_append(json, pos, "\"LCE\":", -1, *length);
	rc = jsonGenLCE(json, length, &pos, &gLceRsp->lce);
	if(rc)
		return rc;

	pos = json_append(json, pos, "}", 1, *length);

	if(pos<0)
		return pos;

	if(!json)
		*length = pos;

	return JSON_RC_OK;
}

JSON_RC jsonGenSetLCEReq(SRV_String* json, int* length, const setLCEReq_t* sLceReq){

	JSON_RC rc;
	int pos = 0;
	int first = 1;

	//add static structure
	pos = json_append(json, pos, "\"serviceName\":\"setLCEReq\",\"serviceParameter\":{",-1 ,*length);

	// add serviceParameter
	//pos = json_appendKey(json, pos, "LCEId", 5, *length, &first);
	//pos = json_appendAny(json, pos, &sLceReq->lceId, SRV_VT_INT64, *length);

	pos = json_appendKey(json, pos, "LCE", 3, *length, &first);
	rc = jsonGenLCE(json, length, &pos, &sLceReq->lce);
	if(rc)
		return rc;

	pos = json_append(json, pos, "}", 1, *length);

	if(pos<0)
		return pos;

	if(!json)
		*length = pos;

	return JSON_RC_OK;
}

JSON_RC jsonGenSetLCERsp(SRV_String* json, int* length, const setLCERsp_t* sLceRsp){

	return jsonGenStatusRsp(json, length, "setLCERsp", sLceRsp->status);
}

JSON_RC jsonGenGetCoreDataReq(SRV_String* json, int* length, const getCoreDataReq_t* gCDReq){

	int pos = 0;

	//add static structure
	pos = json_append(json, pos, "\"serviceName\":\"getCoreDataReq\",\"serviceParameter\":{}",-1 ,*length);

	if(pos<0)
		return pos;

	if(!json)
		*length = pos;

	return JSON_RC_OK;
}

JSON_RC jsonGenGetCoreDataRsp(SRV_String* json, int* length, const getCoreDataRsp_t* gCDRsp){

	JSON_RC rc;
	int pos = 0;
	int first = 1;

	//add static structure
	pos = json_append(json, pos, "\"serviceName\":\"getCoreDataRsp\",\"serviceParameter\":{",-1 ,*length);

	// add serviceParameter
	pos = json_appendKey(json, pos, "Status", 6, *length, &first);
	pos = json_appendStatus(json, pos, &gCDRsp->status, *length);

	pos = json_appendKey(json, pos, "PVSL", 4, *length, &first);
	pos = json_append(json, pos, "[", 1, *length);

	for(uint i = 0; i<gCDRsp->numPvsl;i++){
		if(i!=0)
			pos = json_append(json, pos, ",", 1, *length);
		rc = jsonGenPVSL(json, length, &pos, &gCDRsp->pvsl[i]);
		if(rc)
			return rc;
	}

	pos = json_append(json, pos, "]}", 2, *length);

	if(pos<0)
		return pos;

	if(!json)
		*length = pos;

	return JSON_RC_OK;
}
