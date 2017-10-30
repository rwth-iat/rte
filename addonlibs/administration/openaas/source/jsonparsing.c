/*
 * jsonparsing.c
 *
 *  Created on: Dec 8, 2016
 *      Author: lars
 */

#include "jsonparsing.h"

#define MAX_NUMBUFFER 30
#define MAX_DATEBUFFER 40

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


JSON_RC jsonGenGeneric(SRV_String* json, int* length,const SRV_msgHeader* header, const void* srvStruct, SRV_service_t srvType);

static int jsoneq(const char *json, const jsmntok_t *tok, const char *s) {
	if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
			strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
		return 0;
	}
	return -1;
}

time_t mkgmtime(struct tm* tm_lt){
	time_t lt;
	time_t offset;
	struct tm* tm_gmt;

	lt = mktime(tm_lt);

	tm_gmt = gmtime(&lt);

	offset = (tm_gmt->tm_hour - tm_lt->tm_hour)*3600 +
			(tm_gmt->tm_min - tm_lt->tm_min)*60;

	if(offset<-43200){
		offset += 86400;
	}else if(offset>43200){
		offset -= 86400;
	}

	return lt - offset;
}

JSON_RC ISOToDateTime(char* iso, int len, SRV_DateTime* jsonTime){
	//SRV_DateTime jsonTime;
	char* buf;
	time_t unixTime;
	int psec = 0;

	// need null terminator when string ends in 0-9, 'T', '-', ':', '.' ( valid for time format )
	if(len>0 && ((iso[len-1] >= '0' && iso[len-1] <= '9') ||
			iso[len] == 'T' || iso[len] == '-' || iso[len] == ':' || iso[len] == '.')){
		buf = malloc(len+1);
		if(!buf)
			return JSON_RC_MALLOC_FAIL;
		memcpy(buf, iso, len);
		buf[len] = '\0';
	} else {
		len = strlen(iso);
		buf = iso;
	}

	struct tm tm;
	memset(&tm, '\0', sizeof(tm));

	int year = -1;
	int mon = -1;
	int count_psec = 0;

	sscanf(buf, "%d-%d-%dT%d:%d:%d.%n", &year, &mon, &tm.tm_mday, &tm.tm_hour, &tm.tm_min, &tm.tm_sec, &count_psec);

	tm.tm_year = year - 1900;
	tm.tm_mon = mon -1;
	tm.tm_isdst = -1;

	// check time struct ranges
	if(tm.tm_year<70 || tm.tm_mon<0 || tm.tm_mon>11 || tm.tm_mday<1 || tm.tm_mday>31 ||
			tm.tm_hour<0 || tm.tm_hour>59 || tm.tm_min<0 || tm.tm_min>59 || tm.tm_sec<0 || tm.tm_sec>61){
		if(buf!=iso)
			free(buf);
		return JSON_RC_WRONG_VALUE;
	}


	if(mktime(&tm)<0){
		if(buf!=iso)
			free(buf);
		return JSON_RC_WRONG_VALUE;
	}

	unixTime = mkgmtime(&tm);

	if(count_psec>0){
		char* start = buf+count_psec;
		char* tail;

		psec = strtol(start, &tail, 10);
		int i = tail-start;
		if(i<7)
			for(;i<7;i++){
				psec = psec * 10;
			}
		else if (i>7) {
			for(;i>7;i--){
				psec = psec / 10;
			}
		}
	}

	*jsonTime = (unixTime * JSON_SEC_TO_DATETIME) + JSON_DATETIME_UNIXEPOCH + psec;

	if(buf!=iso)
		free(buf);

	return JSON_RC_OK;
}

//TODO escape special characters?
static JSON_RC tok2SrvStr(const char* js, const jsmntok_t* t, SRV_String* str){
	if(!(t->type==JSMN_STRING))
		return JSON_RC_WRONG_TYPE;
	if(!str)
		return JSON_RC_UNDEF;

	SRV_String_setCopy(str, js+t->start, t->end-t->start);

	return JSON_RC_OK;
}


static SRV_valType_t getPrimType(const char* js, const jsmntok_t* t){

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


static JSON_RC tok2Prim(const char* js, const jsmntok_t* t, SRV_valType_t* type, SRV_Any_t* value){

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
			value->vt_bool = false;
			break;
		case 't':case 'T':
			value->vt_bool = true;
			break;
		}
		break;
	case SRV_VT_NULL:
		break;
	case SRV_VT_DOUBLE:
		value->vt_double = strtod(&js[t->start], NULL);
		break;
	case SRV_VT_SINGLE:
		value->vt_single = strtof(&js[t->start], NULL);
		break;
	case SRV_VT_INT32:
		value->vt_int32 = strtol(&js[t->start], NULL, 10);
		break;
	case SRV_VT_INT64:
		value->vt_int64 = strtoll(&js[t->start], NULL, 10);
		break;
	case SRV_VT_UINT32:
		value->vt_uint32 = strtoul(&js[t->start], NULL, 10);
		break;
	case SRV_VT_UINT64:
		value->vt_uint64 = strtoull(&js[t->start], NULL, 10);
		break;
	default:
		return JSON_RC_WRONG_VALUE;
	}

	return JSON_RC_OK;
}


static JSON_RC tok2Any(const char* js, const jsmntok_t* t, SRV_extAny_t* value){
	JSON_RC rc = JSON_RC_OK;

	if(t->type==JSMN_STRING){
		if(value->type==SRV_VT_DATETIME){
			SRV_String time;
			rc = tok2SrvStr(js, t, &time);
			if(rc)
				return rc;
			rc = ISOToDateTime(time.data, time.length, &value->value.vt_datetime);
			SRV_String_deleteMembers(&time);
			if(rc)
				return rc;
		} else {
			rc = tok2SrvStr(js, t, &value->value.vt_string);
			if(rc)
				return rc;
			value->type = SRV_VT_STRING;
		}
	} else if(t->type==JSMN_PRIMITIVE){
		rc = tok2Prim(js, t, &value->type, &value->value);
		if(rc)
			return rc;
	}
	return JSON_RC_OK;
}


static JSON_RC tok2Int(const char* js, const jsmntok_t* t, void* value, SRV_valType_t intType){
	SRV_valType_t type = SRV_VT_undefined;
	if(t->type!=JSMN_PRIMITIVE)
		return JSON_RC_WRONG_TYPE;

	type = getPrimType(js, t);
	if(type!=SRV_VT_INT64)
		return JSON_RC_WRONG_VALUE;

	if(js[t->start]=='-' && (intType==SRV_VT_UINT32 || intType==SRV_VT_UINT64))
		return JSON_RC_WRONG_VALUE;

	switch(intType){
	case SRV_VT_INT32:
		*(int32_t*)value = strtol(&js[t->start], NULL, 10);
		break;
	case SRV_VT_INT64:
		*(int64_t*)value = strtoll(&js[t->start], NULL, 10);
		break;
	case SRV_VT_UINT32:
		*(uint32_t*)value = strtoul(&js[t->start], NULL, 10);
		break;
	case SRV_VT_UINT64:
		*(uint64_t*)value = strtoull(&js[t->start], NULL, 10);
		break;
	default:
		return JSON_RC_WRONG_TYPE;
	}

	return JSON_RC_OK;
}


/* unused
static JSON_RC tok2Double(const char* js, const jsmntok_t* t, double* value){
	SRV_valType_t type = SRV_VT_undefined;
	if(!(t->type==JSMN_PRIMITIVE))
		return JSON_RC_WRONG_TYPE;

	tok2Prim(js, t, &type, NULL);
	if(type!=SRV_VT_DOUBLE && type!=SRV_VT_INT64)
		return JSON_RC_WRONG_VALUE;

	*value = strtod(&js[t->start], NULL);

	return 0;
}*/


/* unused
static JSON_RC tok2Bool(const char* js, const jsmntok_t* t, bool* value){
	SRV_valType_t type = SRV_VT_undefined;
	if(!(t->type==JSMN_PRIMITIVE))
		return JSON_RC_WRONG_TYPE;

	tok2Prim(js, t, &type, NULL);
	if(type!=SRV_VT_BOOL)
		return JSON_RC_WRONG_VALUE;

	tok2Prim(js, t, &type, value);

	return 0;
}
*/

static JSON_RC tok2Status(const char* js, const jsmntok_t* t, status_t* status){

	return tok2Int(js, t, status, SRV_VT_INT32);
}


static int json_append(SRV_String* js, int start, const char* str, int len, int maxLen){

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

//TODO escape special characters?
static int json_appendString(SRV_String* js, int start, const char* str, int len, int maxLen){
	int pos = json_append(js, start, "\"", 1, maxLen);
	pos = json_append(js, pos, str, len, maxLen);
	pos = json_append(js, pos, "\"", 1, maxLen);
	return pos;
}


static int json_appendIsoTime(SRV_String* js, int start, int maxLen, SRV_DateTime t){
	int pos;
	char buf[MAX_DATEBUFFER];
	int len = 0;

	len = dateTimeToISO(t, buf, MAX_DATEBUFFER);

	pos = json_appendString(js, start, buf, len, maxLen);

	return pos;
}


static int json_appendKey(SRV_String* js, int start, const char* str, int len, int maxLen, int* first){
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


static int json_appendSrvStr(SRV_String* js, int start, const SRV_String* str, int maxLen){
	int pos = json_append(js, start, "\"", 1, maxLen);
	pos = json_append(js, pos, str->data, str->length, maxLen);
	pos = json_append(js, pos, "\"", 1, maxLen);
	return pos;
}

static int json_appendExtAny(SRV_String* js, int start, const SRV_extAny_t* value, int maxLen){
	int pos = JSON_RC_UNDEF;

	int bufLen;
	char numbuffer[MAX_NUMBUFFER];

	switch(value->type){
	case SRV_VT_BOOL:
		if(value->value.vt_bool){
			pos = json_append(js, start, "true", -1, maxLen);
		} else {
			pos = json_append(js, start, "false", -1, maxLen);
		}
		break;
	case SRV_VT_NULL:
		pos = json_append(js, start, "null", -1, maxLen);
		break;
	case SRV_VT_STRING:
		pos = json_appendString(js, start, value->value.vt_string.data, value->value.vt_string.length, maxLen);
		break;
	case SRV_VT_DATETIME:
		pos = json_appendIsoTime(js, start, maxLen, value->value.vt_datetime);
		break;
	case SRV_VT_INT32:
		bufLen = snprintf(numbuffer, MAX_NUMBUFFER, "%"PRIi32, value->value.vt_int32);
		if(!(bufLen<MAX_NUMBUFFER))
			return JSON_RC_NO_SPACE;
		pos = json_append(js, start, numbuffer, bufLen, maxLen);
		break;
	case SRV_VT_INT64:
		bufLen = snprintf(numbuffer, MAX_NUMBUFFER, "%"PRIi64, value->value.vt_int64);
		if(!(bufLen<MAX_NUMBUFFER))
			return JSON_RC_NO_SPACE;
		pos = json_append(js, start, numbuffer, bufLen, maxLen);
		break;
	case SRV_VT_UINT32:
		bufLen = snprintf(numbuffer, MAX_NUMBUFFER, "%"PRIu32, value->value.vt_uint32);
		if(!(bufLen<MAX_NUMBUFFER))
			return JSON_RC_NO_SPACE;
		pos = json_append(js, start, numbuffer, bufLen, maxLen);
		break;
	case SRV_VT_UINT64:
		bufLen = snprintf(numbuffer, MAX_NUMBUFFER, "%"PRIu64, value->value.vt_uint64);
		if(!(bufLen<MAX_NUMBUFFER))
			return JSON_RC_NO_SPACE;
		pos = json_append(js, start, numbuffer, bufLen, maxLen);
		break;
	case SRV_VT_SINGLE:
		bufLen = snprintf(numbuffer, MAX_NUMBUFFER, "%.9g", value->value.vt_single);
		if(!(bufLen<MAX_NUMBUFFER))
			return JSON_RC_NO_SPACE;
		pos = json_append(js, start, numbuffer, bufLen, maxLen);
		break;
	case SRV_VT_DOUBLE:
		bufLen = snprintf(numbuffer, MAX_NUMBUFFER, "%.17g", value->value.vt_double);
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

static int json_appendInt(SRV_String* js, int start, SRV_valType_t type, const void* value, int maxLen){
	SRV_extAny_t extAny;
	switch(type){
	case SRV_VT_INT32:
		extAny.value.vt_int32 = *(int32_t*)value;
		break;
	case SRV_VT_INT64:
		extAny.value.vt_int64 = *(int64_t*)value;
		break;
	case SRV_VT_UINT32:
		extAny.value.vt_uint32 = *(uint32_t*)value;
		break;
	case SRV_VT_UINT64:
		extAny.value.vt_uint64 = *(uint64_t*)value;
		break;

	default:
		return JSON_RC_WRONG_TYPE;
	}
	extAny.type = type;
	return json_appendExtAny(js, start, &extAny, maxLen);
}

static int json_appendStatus(SRV_String* js, int start, const status_t* status, int maxLen){

	return json_appendInt(js, start, SRV_VT_INT32, status, maxLen);
}


JSON_RC jsonparseId(const jsmntok_t* t, const SRV_String* str, int start, int n, SRV_ident_t* id, int* last){

	JSON_RC rc = JSON_RC_OK;

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


JSON_RC jsonparsePVS(const jsmntok_t* t, const SRV_String* str, int start, int n, PVS_t* pvs, int* last) {

	JSON_RC rc = JSON_RC_OK;

	if(start<0)
		return JSON_RC_UNDEF;
	if(t[start].type!=JSMN_OBJECT)
		return JSON_RC_WRONG_TYPE;

	int j = start+1;

	while(j < n && t[j].start<=t[start].end){
		if(jsoneq(str->data, &t[j], "PreferredName")==0){
			j++;
			rc = tok2SrvStr(str->data, &t[j], &pvs->preferredName);
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
		} else if(jsoneq(str->data, &t[j], "CarrierId")==0){
			j++;
			rc = jsonparseId(t, str, j, n, &pvs->carrierId, &j);
			if(rc)
				return rc;
		} else if(jsoneq(str->data, &t[j], "PropertyId")==0){
			j++;
			rc = jsonparseId(t, str, j, n, &pvs->propertyId, &j);
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
			if(pvs->visibility==SRV_VIS_undefined)
				return JSON_RC_WRONG_VALUE;
		} else if(jsoneq(str->data, &t[j], "ValueTime")==0){
			j++;
			SRV_String time;
			rc = tok2SrvStr(str->data, &t[j], &time);
			if(rc)
				return rc;
			rc = ISOToDateTime(time.data, time.length, &pvs->value.time);
			SRV_String_deleteMembers(&time);
			if(rc)
				return rc;
			pvs->value.hasTime = true;
		} else if(jsoneq(str->data, &t[j], "ValueType")==0){
			j++;
			SRV_String tmpStr;
			rc = tok2SrvStr(str->data, &t[j], &tmpStr);
			if(rc)
				return rc;
			for(int i = 1; i <= JSON_VT_MAX && pvs->value.time == SRV_VT_undefined; i++){
				if(srvStrEq(&tmpStr, &JSON_VT_STR[i])==0)
					pvs->value.type = i;
			}
			if(pvs->value.type == SRV_VT_undefined)
				return JSON_RC_WRONG_VALUE;
			SRV_String_deleteMembers(&tmpStr);
			if(pvs->value.type == SRV_VT_undefined)
				return JSON_RC_WRONG_VALUE;
		} else if(jsoneq(str->data, &t[j], "Value")==0){
			j++;
			if(pvs->value.type==SRV_VT_undefined){
				//search rest of pvs for value type field
				for(int k = j+1; k<n && t[k].start<=t[start].end; k++){
					if(jsoneq(str->data, &t[k], "ValueType")==0){
						k++;
						SRV_String tmpStr;
						rc = tok2SrvStr(str->data, &t[k], &tmpStr);
						if(rc)
							return rc;
						for(int i = 1; i<=JSON_VT_MAX && pvs->value.type==SRV_VT_undefined; i++){
							if(srvStrEq(&tmpStr, &JSON_VT_STR[i])==0)
								pvs->value.type = i;
						}
						SRV_String_deleteMembers(&tmpStr);
						if(pvs->value.type == SRV_VT_undefined)
							return JSON_RC_WRONG_VALUE;
					}
				}
			}

			rc = tok2Any(str->data, &t[j], &pvs->value);
			if(rc)
				return rc;
		}
		j++;
	}

	// return last used token
	*last = j-1;

	return JSON_RC_OK;
}


JSON_RC jsonparsePVSL(const jsmntok_t* t, const SRV_String* str, int start, int n, PVSL_t* pvsl, int* last){


	JSON_RC rc = JSON_RC_OK;

	if(start<0)
		return JSON_RC_UNDEF;
	if(t[start].type!=JSMN_OBJECT)
		return JSON_RC_WRONG_TYPE;

	int j = start+1;

	while(j < n && t[j].start<=t[start].end){
		if(jsoneq(str->data, &t[j], "PreferredName")==0){
			j++;
			rc = tok2SrvStr(str->data, &t[j], &pvsl->preferredName);
			if(rc)
				return rc;
			pvsl->hasName = true;
		} else if(jsoneq(str->data, &t[j], "PropertyId")==0){
			j++;
			rc = jsonparseId(t, str, j, n, &pvsl->propertyId, &j);
			if(rc)
				return rc;
		} else if(jsoneq(str->data, &t[j], "CarrierId")==0){
			j++;
			rc = jsonparseId(t, str, j, n, &pvsl->carrierId, &j);
			if(rc)
				return rc;
		} else if(jsoneq(str->data, &t[j], "ExpressionSemantic")==0){
			j++;
			SRV_String tmpStr;
			rc = tok2SrvStr(str->data, &t[j], &tmpStr);
			if(rc)
				return rc;
			pvsl->expressionSemantic = SRV_ES_undefined;
			for(int i = 0; i < JSON_ES_MAX && pvsl->expressionSemantic == SRV_ES_undefined; i++) {
				if(srvStrEq(&tmpStr, &JSON_ES_STR[i])==0){
					pvsl->expressionSemantic = i;
				}
			}
			SRV_String_deleteMembers(&tmpStr);
			if(pvsl->expressionSemantic==SRV_ES_undefined)
				return JSON_RC_WRONG_VALUE;
		} else if(jsoneq(str->data, &t[j], "ExpressionLogic")==0){
			j++;
			SRV_String tmpStr;
			rc = tok2SrvStr(str->data, &t[j], &tmpStr);
			if(rc)
				return rc;
			pvsl->expressionLogic = SRV_EL_undefined;
			for(int i = 0; i < JSON_EL_MAX && pvsl->expressionLogic == SRV_EL_undefined; i++){
				if(srvStrEq(&tmpStr, &JSON_EL_STR[i])==0){
					pvsl->expressionLogic = i;
				}
			}
			SRV_String_deleteMembers(&tmpStr);
			if(pvsl->expressionLogic==SRV_EL_undefined)
				return JSON_RC_WRONG_VALUE;
		} else if(jsoneq(str->data, &t[j], "View")==0){
			j++;
			SRV_String tmpStr;
			rc = tok2SrvStr(str->data, &t[j], &tmpStr);
			if(rc)
				return rc;
			pvsl->view = SRV_VIEW_undefined;
			for(int i=0; i < JSON_VIEW_MAX && pvsl->view == SRV_VIEW_undefined; i++){
				if(srvStrEq(&tmpStr, &JSON_VIEW_STR[i])==0)
					pvsl->view = i;
			}
			SRV_String_deleteMembers(&tmpStr);
			if(pvsl->view==SRV_VIEW_undefined)
				return JSON_RC_WRONG_VALUE;
		} else if(jsoneq(str->data, &t[j], "Visibility")==0){
			j++;
			SRV_String tmpStr;
			rc = tok2SrvStr(str->data, &t[j], &tmpStr);
			if(rc)
				return rc;
			pvsl->visibility = SRV_VIS_undefined;
			for(int i=0; i < JSON_VIS_MAX && pvsl->visibility == SRV_VIS_undefined; i++){
				if(srvStrEq(&tmpStr, &JSON_VIS_STR[i])==0)
					pvsl->visibility = i;
			}
			SRV_String_deleteMembers(&tmpStr);
			if(pvsl->visibility==SRV_VIS_undefined)
				return JSON_RC_WRONG_VALUE;
		} else if(jsoneq(str->data, &t[j], "PVS")==0){
			j++;
			if(t[j].type==JSMN_ARRAY){
				pvsl->numPvs = t[j].size;
				j++;
			} else {
				pvsl->numPvs = 1;
			}
			pvsl->pvs = malloc(sizeof(PVS_t)*pvsl->numPvs);
			if(!pvsl->pvs)
				return JSON_RC_MALLOC_FAIL;
			for(uint32_t k = 0; k<pvsl->numPvs; k++ ){
				PVS_t_init(&pvsl->pvs[k]);
			}
			for(uint32_t k = 0; k<pvsl->numPvs; k++ ){
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


JSON_RC jsonparseLCE(const jsmntok_t* t, const SRV_String* str, int start, int n, LCE_t* lce, int* last){

	JSON_RC rc = JSON_RC_OK;

	if(start<0)
		return JSON_RC_UNDEF;
	if(t[start].type!=JSMN_OBJECT)
		return JSON_RC_WRONG_TYPE;

	int j = start + 1;

	while(j < n && t[j].start<=t[start].end){
		if(jsoneq(str->data, &t[j], "Id")==0){
			j++;
			rc = jsonparseId(t, str, j, n, &lce->id, &j);
			if(rc)
				return rc;
		} else if(jsoneq(str->data, &t[j], "CreatingInstance")==0){
			j++;
			rc = jsonparseId(t, str, j, n, &lce->creatingInstance, &j);
			if(rc)
				return rc;
		} else if(jsoneq(str->data, &t[j], "WritingInstance")==0){
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
		} else if(jsoneq(str->data, &t[j], "TimeStamp")==0){
			j++;
			SRV_String time;
			rc = tok2SrvStr(str->data, &t[j], &time);
			if(rc){
				return rc;
			}
			rc =  ISOToDateTime(time.data, time.length, &lce->value.time);
			SRV_String_deleteMembers(&time);
			if(rc)
				return rc;
			lce->value.hasTime = true;
		} else if(jsoneq(str->data, &t[j], "ValueType")==0){
			j++;
			SRV_String tmpStr;
			rc = tok2SrvStr(str->data, &t[j], &tmpStr);
			if(rc)
				return rc;
			for(int i = 1; i<=JSON_VT_MAX && lce->value.type==SRV_VT_undefined; i++){
				if(srvStrEq(&tmpStr, &JSON_VT_STR[i])==0)
					lce->value.type = i;
			}
			SRV_String_deleteMembers(&tmpStr);
			if(lce->value.type==SRV_VT_undefined)
				return JSON_RC_WRONG_VALUE;
		} else if(jsoneq(str->data, &t[j], "Value")==0){
			j++;
			if(lce->value.type==SRV_VT_undefined){
				for(int k = j+1; k < n && t[k].start<=t[start].end; k++){
					if(jsoneq(str->data, &t[k], "ValueType")==0){
						k++;
						SRV_String tmpStr;
						rc = tok2SrvStr(str->data, &t[k], &tmpStr);
						if(rc)
							return rc;
						for(int i = 1; i<=JSON_VT_MAX && lce->value.type==SRV_VT_undefined; i++){
							if(srvStrEq(&tmpStr, &JSON_VT_STR[i])==0)
								lce->value.type = i;
						}
						SRV_String_deleteMembers(&tmpStr);
						if(lce->value.type==SRV_VT_undefined)
							return JSON_RC_WRONG_VALUE;
					}
				}
			}

			rc = tok2Any(str->data, &t[j], &lce->value);
			if(rc)
				return rc;
		}
		j++;
	}

	*last = j - 1;

	return JSON_RC_OK;
}


JSON_RC jsonparseStatusRsp(status_t* status, const jsmntok_t* t, const SRV_String* js, int n, int start){

	JSON_RC rc = JSON_RC_OK;

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

	JSON_RC rc = JSON_RC_OK;

	if(start<0)
		return JSON_RC_UNDEF;
	if(t[start].type!=JSMN_OBJECT)
		return JSON_RC_WRONG_TYPE;

	createAASReq_t_init(cAasReq);
	int i = start + 1;
	int fields = 0;

	while(i < n && t[i].end<=t[start].end){
		if(jsoneq(js->data, &t[i], "AASId")==0){
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
		} else if(jsoneq(js->data, &t[i], "AASName")==0){
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

	JSON_RC rc = JSON_RC_OK;

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

//TODO SubModel
JSON_RC jsonparseCreateSubModelReq(createSubModelReq_t* cSMReq, const jsmntok_t* t, const SRV_String* js, int n, int start){

	JSON_RC rc = JSON_RC_OK;

	if(start<0)
		return JSON_RC_UNDEF;
	if(t[start].type!=JSMN_OBJECT)
		return JSON_RC_WRONG_TYPE;

	createSubModelReq_t_init(cSMReq);
	int i = start + 1;
	int fields = 0;

	while(i < n && t[i].end<=t[start].end){
		if(jsoneq(js->data, &t[i], "ParentId")==0){
			i++;
			rc = jsonparseId(t, js, i, n, &cSMReq->parentId, &i);
			if(rc)
				return rc;
			fields |= 1;
		} else if(jsoneq(js->data, &t[i], "ModelId")==0){
			i++;
			rc = jsonparseId(t, js, i, n, &cSMReq->modelId, &i);
			if(rc)
				return rc;
			fields |= 2;
		} else if(jsoneq(js->data, &t[i], "Name")==0){
			i++;
			rc = tok2SrvStr(js->data, &t[i], &cSMReq->name);
			if(rc)
				return rc;
			fields |= 4;
			cSMReq->hasName = true;
		} else if(jsoneq(js->data, &t[i], "Revision")==0){
			i++;
			rc = tok2Int(js->data, &t[i], &cSMReq->revision, SRV_VT_UINT32);
			if(rc)
				return rc;
			fields |= 8;
			cSMReq->hasRevision = true;
		} else if(jsoneq(js->data, &t[i], "Version")==0){
			i++;
			rc = tok2Int(js->data, &t[i], &cSMReq->version, SRV_VT_UINT32);
			if(rc)
				return rc;
			fields |= 16;
			cSMReq->hasVersion = true;
		}
		i++;
	}

	if(fields!=31)
		return JSON_RC_MISSING_FIELD;

	return JSON_RC_OK;
}

JSON_RC jsonparseCreateSubModelRsp(createSubModelRsp_t* cSMRsp, const jsmntok_t* t, const SRV_String* js, int n, int start){

	createSubModelRsp_t_init(cSMRsp);
	return jsonparseStatusRsp(&cSMRsp->status, t, js, n, start);
}

JSON_RC jsonparseDeleteSubModelReq(deleteSubModelReq_t* dSMReq, const jsmntok_t* t, const SRV_String* js, int n, int start){

	JSON_RC rc = JSON_RC_OK;

	if(start<0)
		return JSON_RC_UNDEF;
	if(t[start].type!=JSMN_OBJECT)
		return JSON_RC_WRONG_TYPE;

	deleteSubModelReq_t_init(dSMReq);
	int i = start + 1;
	int fields = 0;

	while(i < n && t[i].end<=t[start].end){
		if(jsoneq(js->data, &t[i], "Id")==0){
			i++;
			rc = jsonparseId(t, js, i, n, &dSMReq->modelId, &i);
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

JSON_RC jsonparseDeleteSubModelRsp(deleteSubModelRsp_t* dSMRsp, const jsmntok_t* t, const SRV_String* js, int n, int start){

	deleteSubModelRsp_t_init(dSMRsp);
	return jsonparseStatusRsp(&dSMRsp->status, t, js, n, start);
}

JSON_RC jsonparseCreatePVSLReq(createPVSLReq_t* cPvslReq, const jsmntok_t* t, const SRV_String* js, int n, int start){

	JSON_RC rc = JSON_RC_OK;

	if(start<0)
		return JSON_RC_UNDEF;
	if(t[start].type!=JSMN_OBJECT)
		return JSON_RC_WRONG_TYPE;

	createPVSLReq_t_init(cPvslReq);
	int i = start + 1;
	int fields = 0;

	while(i < n && t[i].end<=t[start].end){
		if(jsoneq(js->data, &t[i], "ParentId")==0){
			i++;
			rc = jsonparseId(t, js, i, n, &cPvslReq->parentId, &i);
			if(rc)
				return rc;
			fields |= 1;
		} else if(jsoneq(js->data, &t[i], "PVSL")==0){
			i++;
			rc = jsonparsePVSL(t, js, i, n, &cPvslReq->pvsl, &i);
			if(rc)
				return rc;
			if(cPvslReq->pvsl.hasName) // only name mandatory
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

	JSON_RC rc = JSON_RC_OK;

	if(start<0)
		return JSON_RC_UNDEF;
	if(t[start].type!=JSMN_OBJECT)
		return JSON_RC_WRONG_TYPE;

	deletePVSLReq_t_init(dPvslReq);
	int i = start + 1;
	int fields = 0;

	while(i < n && t[i].end<=t[start].end){
		if(jsoneq(js->data, &t[i], "Id")==0){
			i++;
			rc = jsonparseId(t, js, i, n, &dPvslReq->id, &i);
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

	JSON_RC rc = JSON_RC_OK;

	if(start<0)
		return JSON_RC_UNDEF;
	if(t[start].type!=JSMN_OBJECT)
		return JSON_RC_WRONG_TYPE;

	createPVSReq_t_init(cPvsReq);
	int i = start + 1;
	int fields = 0;

	while(i < n && t[i].end<=t[start].end){
		if(jsoneq(js->data, &t[i], "ListId")==0){
			i++;
			rc = jsonparseId(t, js, i, n, &cPvsReq->listId, &i);
			if(rc)
				return rc;
			fields |= 1;
		} else if(jsoneq(js->data, &t[i], "PVS")==0){
			i++;
			rc = jsonparsePVS(t, js, i, n, &cPvsReq->pvs, &i);
			if(rc)
				return rc;
			if(cPvsReq->pvs.hasName && cPvsReq->pvs.value.hasTime &&
					cPvsReq->pvs.value.type!=SRV_VT_undefined)
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
	JSON_RC rc = JSON_RC_OK;

	if(start<0)
		return JSON_RC_UNDEF;
	if(t[start].type!=JSMN_OBJECT)
		return JSON_RC_WRONG_TYPE;

	deletePVSReq_t_init(dPvsReq);
	int i = start + 1;
	int fields = 0;

	while(i < n && t[i].end<=t[start].end){
		if(jsoneq(js->data, &t[i], "Id")==0){
			i++;
			rc = jsonparseId(t, js, i, n, &dPvsReq->id, &i);
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


JSON_RC jsonparseDeletePVSRsp(deletePVSRsp_t* dPvsRsp, const jsmntok_t* t, const SRV_String* js, int n, int start){

	deletePVSRsp_t_init(dPvsRsp);
	return jsonparseStatusRsp(&dPvsRsp->status, t, js, n, start);
}


JSON_RC jsonparseCreateLCEReq(createLCEReq_t* cLceReq, const jsmntok_t* t, const SRV_String* js, int n, int start){
	JSON_RC rc = JSON_RC_OK;

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
			if(cLceReq->lce.hasEventClass && cLceReq->lce.hasSubject && cLceReq->lce.value.hasTime &&
					cLceReq->lce.creatingInstance.idType!=SRV_IDT_undefined &&
					cLceReq->lce.writingInstance.idType!=SRV_IDT_undefined &&
					cLceReq->lce.value.time!=SRV_VT_undefined)
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
	JSON_RC rc = JSON_RC_OK;

	if(start<0)
		return JSON_RC_UNDEF;
	if(t[start].type!=JSMN_OBJECT)
		return JSON_RC_WRONG_TYPE;

	deleteLCEReq_t_init(dLceReq);
	int i = start + 1;
	int fields = 0;

	while(i < n && t[i].end<=t[start].end){
		if(jsoneq(js->data, &t[i], "Id")==0){
			i++;
			rc = jsonparseId(t, js, i, n, &dLceReq->id, &i);
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
	JSON_RC rc = JSON_RC_OK;

	if(start<0)
		return JSON_RC_UNDEF;
	if(t[start].type!=JSMN_OBJECT)
		return JSON_RC_WRONG_TYPE;

	getPVSReq_t_init(gPvsReq);
	int i = start+1;
	int fields = 0;

	while(i < n && t[i].end<=t[start].end){
		if(jsoneq(js->data, &t[i], "Id")==0){
			i++;
			rc = jsonparseId(t, js, i, n, &gPvsReq->id, &i);
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


JSON_RC jsonparseGetPVSRsp(getPVSRsp_t* gPvsRsp, const jsmntok_t* t, const SRV_String* js, int n, int start){

	JSON_RC rc = JSON_RC_OK;

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
			//TODO name required?
			if( // gPvsRsp->pvs.hasName && gPvsRsp->pvs.value.hasTime &&
					gPvsRsp->pvs.expressionSemantic!=SRV_ES_undefined &&
					gPvsRsp->pvs.expressionLogic!=SRV_EL_undefined &&
					gPvsRsp->pvs.visibility!=SRV_VIS_undefined &&
					gPvsRsp->pvs.carrierId.idType!=SRV_IDT_undefined &&
					gPvsRsp->pvs.propertyId.idType!=SRV_IDT_undefined &&
					gPvsRsp->pvs.value.type!=SRV_VT_undefined &&
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
	JSON_RC rc = JSON_RC_OK;

	if(start<0)
		return JSON_RC_UNDEF;
	if(t[start].type!=JSMN_OBJECT)
		return JSON_RC_WRONG_TYPE;


	setPVSReq_t_init(sPvsReq);
	int i = start+1;
	int fields = 0;

	while(i < n && t[i].end<=t[start].end){
		if(jsoneq(js->data, &t[i], "Id")==0){
			i++;
			rc = jsonparseId(t, js, i, n, &sPvsReq->id, &i);
			if(rc)
				return rc;
			fields |= 1;
		} else if(jsoneq(js->data, &t[i], "PVS")==0){
			i++;
			rc = jsonparsePVS(t, js, i, n, &sPvsReq->pvs, &i);
			if(rc)
				return rc;
			// all subfields optional
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
	JSON_RC rc = JSON_RC_OK;

	if(start<0)
		return JSON_RC_UNDEF;
	if(t[start].type!=JSMN_OBJECT)
		return JSON_RC_WRONG_TYPE;

	getLCEReq_t_init(gLceReq);
	int i = start + 1;
	int fields = 0;

	while(i < n && t[i].end<=t[start].end){
		if(jsoneq(js->data, &t[i], "Id")==0){
			i++;
			rc = jsonparseId(t, js, i, n, &gLceReq->id, &i);
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

	JSON_RC rc = JSON_RC_OK;

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
		} else if(jsoneq(js->data, &t[i], "LCE")==0){
			i++;
			rc = jsonparseLCE(t, js, i, n, &gLceRsp->lce, &i);
			if(rc)
				return rc;
			if(gLceRsp->lce.hasEventClass && gLceRsp->lce.hasSubject && gLceRsp->lce.value.hasTime &&
					gLceRsp->lce.creatingInstance.idType!=SRV_IDT_undefined &&
					gLceRsp->lce.writingInstance.idType!=SRV_IDT_undefined &&
					gLceRsp->lce.value.type!=SRV_VT_undefined)
				fields |= 2;
		}
		i++;
	}

	if(fields!=3)
		return JSON_RC_MISSING_FIELD;

	return JSON_RC_OK;
}


JSON_RC jsonparseSetLCEReq(setLCEReq_t* sLceReq, const jsmntok_t* t, const SRV_String* js, int n, int start){

	JSON_RC rc = JSON_RC_OK;

	if(start<0)
		return JSON_RC_UNDEF;
	if(t[start].type!=JSMN_OBJECT)
		return JSON_RC_WRONG_TYPE;

	setLCEReq_t_init(sLceReq);
	int i = start + 1;
	int fields = 0;

	while(i < n && t[i].end<=t[start].end){
		if(jsoneq(js->data, &t[i], "Id")==0){
			i++;
			rc = jsonparseId(t, js, i, n, &sLceReq->id, &i);
			if(rc)
				return rc;
			fields |= 1;
		} if(jsoneq(js->data, &t[i], "LCE")==0){
			i++;
			rc = jsonparseLCE(t, js, i, n, &sLceReq->lce, &i);
			if(rc)
				return rc;
			if(sLceReq->lce.hasEventClass && sLceReq->lce.hasSubject && sLceReq->lce.value.hasTime &&
					sLceReq->lce.creatingInstance.idType!=SRV_IDT_undefined &&
					sLceReq->lce.writingInstance.idType!=SRV_IDT_undefined &&
					sLceReq->lce.value.type!=SRV_VT_undefined)
			fields |= 2;
		}
		i++;
	}

	if(fields!=3)
		return JSON_RC_MISSING_FIELD;

	return JSON_RC_OK;
}


JSON_RC jsonparseSetLCERsp(setLCERsp_t* sLceRsp, const jsmntok_t* t, const SRV_String* js, int n, int start){

	setLCERsp_t_init(sLceRsp);
	return jsonparseStatusRsp(&sLceRsp->status, t, js, n, start);
}


JSON_RC jsonparseGetCoreDataReq(getCoreDataReq_t* gCDReq, const jsmntok_t* t, const SRV_String* js, int n, int start){

	JSON_RC rc = JSON_RC_OK;

	if(start<0)
		return JSON_RC_UNDEF;
	if(t[start].type!=JSMN_OBJECT)
		return JSON_RC_WRONG_TYPE;

	getCoreDataReq_t_init(gCDReq);
	int i = start + 1;
	int fields = 0;

	while(i < n && t[i].end<=t[start].end){

		if(jsoneq(js->data, &t[i], "Visibility")==0){
			i++;
			SRV_String tmpStr;
			rc = tok2SrvStr(js->data, &t[i], &tmpStr);
			if(rc)
				return rc;
			gCDReq->visibility = SRV_VIS_undefined;
			for(int j=0; j < JSON_VIS_MAX && gCDReq->visibility == SRV_VIS_undefined; j++){
				if(srvStrEq(&tmpStr, &JSON_VIS_STR[j])==0)
					gCDReq->visibility = j;
			}
			SRV_String_deleteMembers(&tmpStr);
			if(gCDReq->visibility==SRV_VIS_undefined)
				return JSON_RC_WRONG_VALUE;
			fields |= 1;
		}
		i++;
	}

	if(fields!=1)
		return JSON_RC_MISSING_FIELD;

	return JSON_RC_OK;

}


JSON_RC jsonparseGetCoreDataRsp(getCoreDataRsp_t* gCDRsp, const jsmntok_t* t, const SRV_String* js, int n, int start){

	JSON_RC rc = JSON_RC_OK;

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
			if(!gCDRsp->pvsl)
				return JSON_RC_MALLOC_FAIL;
			for(uint32_t k = 0; k<gCDRsp->numPvsl; k++){
				PVSL_t_init(&gCDRsp->pvsl[k]);
			}
			for(uint32_t k = 0; k<gCDRsp->numPvsl; k++){
				rc = jsonparsePVSL(t, js, i, n, &gCDRsp->pvsl[k], &i);
				if(rc)
					return rc;
				if(!(gCDRsp->pvsl[k].hasName))
					return JSON_RC_MISSING_FIELD;
				PVS_t* pvs;
				for(uint32_t l = 0; l<gCDRsp->pvsl[k].numPvs; l++){
					pvs = &gCDRsp->pvsl[k].pvs[l];
					if(!(pvs->hasName && pvs->value.hasTime &&
							pvs->value.type!=SRV_VT_undefined))
						return JSON_RC_MISSING_FIELD;
				}
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


JSON_RC jsonGenPVS(SRV_String* json, int* length, int* ppos, const PVS_t* p){
	int pos = *ppos;
	int first = 1;
	JSON_RC rc = JSON_RC_OK;

	pos = json_append(json, pos, "{",1, *length);

	if(p->hasName){
		pos = json_appendKey(json, pos, "PreferredName", 13, *length, &first);
		pos = json_appendSrvStr(json, pos, &p->preferredName, *length);
	}
	if(p->propertyId.idType!=SRV_IDT_undefined){
		pos = json_appendKey(json, pos, "PropertyId", 10, *length, &first);
		rc = jsonGenId(json, length, &pos, &p->propertyId);
		if(rc)
			return rc;
	}
	if(p->expressionLogic!=SRV_EL_undefined){
		pos = json_appendKey(json, pos, "ExpressionLogic", 15, *length, &first);
		pos = json_appendSrvStr(json, pos, &JSON_EL_STR[p->expressionLogic], *length);
	}
	if(p->expressionSemantic!=SRV_ES_undefined){
		pos = json_appendKey(json, pos, "ExpressionSemantic", 18, *length, &first);
		pos = json_appendSrvStr(json, pos, &JSON_ES_STR[p->expressionSemantic], *length);
	}
	if(p->view!=SRV_VIEW_undefined){
		pos = json_appendKey(json, pos, "View", 4, *length, &first);
		pos = json_appendSrvStr(json, pos, &JSON_VIEW_STR[p->view], *length);
	}
	if(p->carrierId.idType!=SRV_IDT_undefined){
		pos = json_appendKey(json, pos, "CarrierId", 9, *length, &first);
		rc = jsonGenId(json, length, &pos, &p->carrierId);
		if(rc)
			return rc;
	}
	if(p->visibility!=SRV_VIS_undefined){
		pos = json_appendKey(json, pos, "Visibility", 10, *length, &first);
		pos = json_appendSrvStr(json, pos, &JSON_VIS_STR[p->visibility], *length);
	}
	if(p->value.type!=SRV_VT_undefined){
		pos = json_appendKey(json, pos, "ValueType", 9, *length, &first);
		pos = json_appendSrvStr(json, pos, &JSON_VT_STR[p->value.type], *length);

		pos = json_appendKey(json, pos, "Value", 5, *length, &first);
		pos = json_appendExtAny(json, pos, &p->value, *length);

		if(p->value.hasTime){
			pos = json_appendKey(json, pos, "ValueTime", 9, *length, &first);
			pos = json_appendIsoTime(json, pos, *length, p->value.time);
		}

	}

	pos = json_append(json, pos, "}", 1, *length);

	if(pos<0)
		return pos;

	*ppos = pos;
	return JSON_RC_OK;
}


JSON_RC jsonGenPVSL(SRV_String* json, int* length, int* ppos, const PVSL_t* p){

	JSON_RC rc = JSON_RC_OK;
	int pos = *ppos;
	int first = 1;

	pos = json_append(json, pos, "{", 1, *length);

	if(p->hasName){
		pos = json_appendKey(json, pos, "PreferredName", 13, *length, &first);
		pos = json_appendSrvStr(json, pos, &p->preferredName, *length);
	}
	if(p->propertyId.idType!=SRV_IDT_undefined){
		pos = json_appendKey(json, pos, "PropertyId", 10, *length, &first);
		rc = jsonGenId(json, length, &pos, &p->propertyId);
		if(rc)
			return rc;
	}
	if(p->expressionLogic!=SRV_EL_undefined){
		pos = json_appendKey(json, pos, "ExpressionLogic", 15, *length, &first);
		pos = json_appendSrvStr(json, pos, &JSON_EL_STR[p->expressionLogic], *length);
	}
	if(p->expressionSemantic!=SRV_ES_undefined){
		pos = json_appendKey(json, pos, "ExpressionSemantic", 18, *length, &first);
		pos = json_appendSrvStr(json, pos, &JSON_ES_STR[p->expressionSemantic], *length);
	}
	if(p->carrierId.idType!=SRV_IDT_undefined){
		pos = json_appendKey(json, pos, "CarrierId", 9, *length, &first);
		rc = jsonGenId(json, length, &pos, &p->carrierId);
		if(rc)
			return rc;
	}
	if(p->view!=SRV_VIEW_undefined){
		pos = json_appendKey(json, pos, "View", 4, *length, &first);
		pos = json_appendSrvStr(json, pos, &JSON_VIEW_STR[p->view], *length);
	}
	if(p->visibility!=SRV_VIS_undefined){
		pos = json_appendKey(json, pos, "Visibility", 10, *length, &first);
		pos = json_appendSrvStr(json, pos, &JSON_VIS_STR[p->visibility], *length);
	}
	if(p->numPvs){
		pos = json_appendKey(json, pos, "PVS", 3, *length, &first);
		pos = json_append(json, pos, "[", 1, *length);
		if(p->pvs){
			for(uint32_t k = 0; k < p->numPvs ; k++){
				if(k!=0)
					pos = json_append(json, pos, ",", 1, *length);
				rc = jsonGenPVS(json, length, &pos, &p->pvs[k]);
				if(rc)
					return rc;
			}
		}
		pos = json_append(json, pos, "]", 1, *length);
	}

	pos = json_append(json, pos, "}", 1, *length);

	if(pos<0)
		return pos;

	*ppos = pos;

	return JSON_RC_OK;
}


JSON_RC jsonGenLCE(SRV_String* json, int* length, int* ppos, const LCE_t* l){

	JSON_RC rc = JSON_RC_OK;
	int pos = *ppos;
	int first = 1;

	pos = json_append(json, pos, "{", 1, *length);

	if(l->id.idType!=SRV_IDT_undefined){
		pos = json_appendKey(json, pos, "Id", 2, *length, &first);
		rc = jsonGenId(json, length, &pos, &l->id);
	}
	if(l->creatingInstance.idType!=SRV_IDT_undefined){
		pos = json_appendKey(json, pos, "CreatingInstance", 16, *length, &first);
		rc = jsonGenId(json, length, &pos, &l->creatingInstance);
		if(rc)
			return rc;
	}
	if(l->writingInstance.idType!=SRV_IDT_undefined){
		pos = json_appendKey(json, pos, "WritingInstance", 15, *length, &first);
		rc = jsonGenId(json, length, &pos, &l->writingInstance);
		if(rc)
			return rc;
	}
	if(l->hasEventClass){
		pos = json_appendKey(json, pos, "EventClass", 10, *length, &first);
		pos = json_appendSrvStr(json, pos, &l->eventClass, *length);
	}
	if(l->hasSubject){
		pos = json_appendKey(json, pos, "Subject", 7, *length, &first);
		pos = json_appendSrvStr(json, pos, &l->subject, *length);
	}
	if(l->value.type!=SRV_VT_undefined){
		pos = json_appendKey(json, pos, "ValueType", 9, *length, &first);
		pos = json_appendSrvStr(json, pos, &JSON_VT_STR[l->value.type], *length);

		//TODO data time without data?
		if(l->value.hasTime){
			pos = json_appendKey(json, pos, "TimeStamp", 9, *length, &first);
			pos = json_appendIsoTime(json, pos, *length, l->value.time);
		}

		pos = json_appendKey(json, pos, "Value", 5, *length, &first);
		pos = json_appendExtAny(json, pos, &l->value, *length);
	}

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
	pos = json_appendKey(json, pos, "serviceName", 11, *length, &first);
	pos = json_appendString(json, pos, serviceName, -1, *length);
	pos = json_appendKey(json, pos, "serviceParameter", 16, *length, &first);

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

	JSON_RC rc = JSON_RC_OK;
	int pos = 0;
	int first = 1;

	//add static structure
	pos = json_append(json, pos, "\"serviceName\":\"createAASReq\",\"serviceParameter\":{", 49, *length);

	// add serviceParameter
	pos = json_appendKey(json, pos, "AASName", 7, *length, &first);
	pos = json_appendSrvStr(json, pos, &cAasReq->aasName, *length);

	pos = json_appendKey(json, pos, "AASId", 5, *length, &first);
	rc = jsonGenId(json, length, &pos, &cAasReq->aasId);
	if(rc)
		return rc;

	pos = json_appendKey(json, pos, "AssetId", 7, *length, &first);
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

	JSON_RC rc = JSON_RC_OK;
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

JSON_RC jsonGenCreateSubModelReq(SRV_String* json, int* length, const createSubModelReq_t* cSMReq){

	JSON_RC rc = JSON_RC_OK;
	int pos = 0;
	int first = 1;

	if(!(cSMReq->hasName && cSMReq->hasRevision && cSMReq->hasVersion))
		return JSON_RC_BAD_INPUT;

	//add static structure
	pos = json_append(json, pos, "\"serviceName\":\"createSubModelReq\",\"serviceParameter\":{",-1, *length);

	// add serviceParameter
	pos = json_appendKey(json, pos, "ParentId", 8, *length, &first);
	rc = jsonGenId(json, length, &pos, &cSMReq->modelId);
	if(rc)
		return rc;

	pos = json_appendKey(json, pos, "ModelId", 7, *length, &first);
	rc = jsonGenId(json, length, &pos, &cSMReq->modelId);
	if(rc)
		return rc;

	pos = json_appendKey(json, pos, "Name", 4, *length, &first);
	pos = json_appendSrvStr(json, pos, &cSMReq->name, *length);

	pos = json_appendKey(json, pos, "Revision", 8, *length, &first);
	pos = json_appendInt(json, pos, SRV_VT_UINT32, &cSMReq->revision, *length);

	pos = json_appendKey(json, pos, "Version", 7, *length, &first);
	pos = json_appendInt(json, pos, SRV_VT_UINT32, &cSMReq->version, *length);

	pos = json_append(json, pos, "}", 1, *length);

	if(pos<0)
		return pos;

	if(!json)
		*length = pos;

	return JSON_RC_OK;
}

JSON_RC jsonGenCreateSubModelRsp(SRV_String* json, int* length, const createSubModelRsp_t* cSMRsp){

	return jsonGenStatusRsp(json, length, "createSubModelRsp", cSMRsp->status);
}

JSON_RC jsonGenDeleteSubModelReq(SRV_String* json, int* length, const deleteSubModelReq_t* dSMReq){

	JSON_RC rc = JSON_RC_OK;
	int pos = 0;

	//add static structure
	pos = json_append(json, pos, "\"serviceName\":\"deleteSubModelReq\",\"serviceParameter\":{",-1, *length);

	// add serviceParameter
	pos = json_appendKey(json, pos, "Id", 2, *length, NULL);
	rc = jsonGenId(json, length, &pos, &dSMReq->modelId);
	if(rc)
		return rc;

	pos = json_append(json, pos, "}", 1, *length);

	if(pos<0)
		return pos;

	if(!json)
		*length = pos;

	return JSON_RC_OK;
}

JSON_RC jsonGenDeleteSubModelRsp(SRV_String* json, int* length, const deleteSubModelRsp_t* dSMRsp){

	return jsonGenStatusRsp(json, length, "deleteSubModelRsp", dSMRsp->status);
}


JSON_RC jsonGenCreatePVSLReq(SRV_String* json, int* length, const createPVSLReq_t* cPvslReq){

	JSON_RC rc = JSON_RC_OK;
	int pos = 0;
	int first = 1;

	//add static structure
	pos = json_append(json, pos, "\"serviceName\":\"createPVSLReq\",\"serviceParameter\":{",-1 ,*length);

	// add serviceParameter
	pos = json_appendKey(json, pos, "ParentId", 8, *length, &first);
	rc = jsonGenId(json, length, &pos, &cPvslReq->parentId);
	if(rc)
		return rc;

	pos = json_appendKey(json, pos, "PVSL", 4, *length, &first);
	rc = jsonGenPVSL(json, length, &pos, &cPvslReq->pvsl);
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

	JSON_RC rc = JSON_RC_OK;
	int pos = 0;
	int first = 1;

	//add static structure
	pos = json_append(json, pos, "\"serviceName\":\"deletePVSLReq\",\"serviceParameter\":{",-1 ,*length);

	// add serviceParameter
	pos = json_appendKey(json, pos, "Id", 2, *length, &first);
	rc = jsonGenId(json, length, &pos, &dPvslReq->id);
	if(rc)
		return rc;

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

	JSON_RC rc = JSON_RC_OK;
	int pos = 0;
	int first = 1;

	//add static structure
	pos = json_append(json, pos, "\"serviceName\":\"createPVSReq\",\"serviceParameter\":{",-1 ,*length);

	// add serviceParameter
	pos = json_appendKey(json, pos, "ListId", 6, *length, &first);
	rc = jsonGenId(json, length, &pos, &cPvsReq->listId);
	if(rc)
		return rc;

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

	JSON_RC rc = JSON_RC_OK;
	int pos = 0;
	int first = 1;

	//add static structure
	pos = json_append(json, pos, "\"serviceName\":\"deletePVSReq\",\"serviceParameter\":{",-1 ,*length);

	// add serviceParameter
	pos = json_appendKey(json, pos, "Id", 2, *length, &first);
	rc = jsonGenId(json, length, &pos, &dPvsReq->id);
	if(rc)
		return rc;

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

	JSON_RC rc = JSON_RC_OK;
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

	JSON_RC rc = JSON_RC_OK;
	int pos = 0;

	//add static structure
	pos = json_append(json, pos, "\"serviceName\":\"deleteLCEReq\",\"serviceParameter\":{",-1 ,*length);

	// add serviceParameter
	pos = json_appendKey(json, pos, "Id", 2, *length, NULL);
	rc = jsonGenId(json, length, &pos, &dLceReq->id);
		if(rc)
			return rc;

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

	JSON_RC rc = JSON_RC_OK;
	int pos = 0;
	int first = 1;

	//add static structure
	pos = json_append(json, pos, "\"serviceName\":\"getPVSReq\",\"serviceParameter\":{",-1 ,*length);

	// add serviceParameter
	pos = json_appendKey(json, pos, "Id", 2, *length, &first);
	rc = jsonGenId(json, length, &pos, &gPvsReq->id);
	if(rc)
		return rc;

	pos = json_append(json, pos, "}", 1, *length);

	if(pos<0)
		return pos;

	if(!json)
		*length = pos;

	return JSON_RC_OK;
}


JSON_RC jsonGenGetPVSRsp(SRV_String* json, int* length, const getPVSRsp_t* gPvsRsp) {

	JSON_RC rc = JSON_RC_OK;
	int pos = 0;
	int first = 1;

	//add static structure
	pos = json_append(json, pos, "\"serviceName\":\"getPVSRsp\",\"serviceParameter\":",-1 ,*length);

	// add serviceParameter
	pos = json_append(json, pos, "{", 1, *length);

	pos = json_appendKey(json, pos, "Status", 6, *length, &first);
	pos = json_appendStatus(json, pos, &gPvsRsp->status, *length);

	pos = json_appendKey(json, pos, "PVS", 3, *length, &first);
	rc = jsonGenPVS(json, length, &pos, &gPvsRsp->pvs);
	if(rc)
		return rc;

	pos = json_append(json, pos, "}", 1, *length);

	if(pos<0)
		return pos;

	if(!json)
		*length = pos;

	return JSON_RC_OK;
}


JSON_RC jsonGenSetPVSReq(SRV_String* json, int* length, const setPVSReq_t* sPvsReq){

	JSON_RC rc = JSON_RC_OK;
	int pos = 0;
	int first = 1;

	//add static structure
	pos = json_append(json, pos, "\"serviceName\":\"setPVSReq\",\"serviceParameter\":{",-1 ,*length);

	// add serviceParameter
	pos = json_appendKey(json, pos, "Id", 2, *length, &first);
	rc = jsonGenId(json, length, &pos, &sPvsReq->id);
	if(rc)
		return rc;

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

	JSON_RC rc = JSON_RC_OK;
	int pos = 0;

	//add static structure
	pos = json_append(json, pos, "\"serviceName\":\"getLCEReq\",\"serviceParameter\":{",-1 ,*length);

	// add serviceParameter
	pos = json_appendKey(json, pos, "Id", 2, *length, NULL);
	rc = jsonGenId(json, length, &pos, &gLceReq->id);
	if(rc)
		return rc;

	pos = json_append(json, pos, "}", 1, *length);

	if(pos<0)
		return pos;

	if(!json)
		*length = pos;

	return JSON_RC_OK;
}


JSON_RC jsonGenGetLCERsp(SRV_String* json, int* length, const getLCERsp_t* gLceRsp){

	JSON_RC rc = JSON_RC_OK;
	int pos = 0;
	int first = 1;

	//add static structure
	pos = json_append(json, pos, "\"serviceName\":\"getLCERsp\",\"serviceParameter\":{",-1 ,*length);

	// add serviceParameter
	pos = json_appendKey(json, pos, "Status", 6, *length, &first);
	pos = json_appendStatus(json, pos, &gLceRsp->status, *length);

	pos = json_appendKey(json, pos, "LCE", 3, *length, &first);
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

	JSON_RC rc = JSON_RC_OK;
	int pos = 0;
	int first = 1;

	//add static structure
	pos = json_append(json, pos, "\"serviceName\":\"setLCEReq\",\"serviceParameter\":{",-1 ,*length);

	// add serviceParameter
	pos = json_appendKey(json, pos, "Id", 2, *length, &first);
		rc = jsonGenId(json, length, &pos, &sLceReq->id);
		if(rc)
			return rc;

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
	pos = json_append(json, pos, "\"serviceName\":\"getCoreDataReq\",\"serviceParameter\":{",-1 ,*length);

	if(gCDReq->visibility!=SRV_VIS_undefined){
		pos = json_appendKey(json, pos, "Visibility", 10, *length, NULL);
		pos = json_appendSrvStr(json, pos, &JSON_VIS_STR[gCDReq->visibility], *length);
	}

	pos = json_append(json, pos, "}", 1 ,*length);

	if(pos<0)
		return pos;

	if(!json)
		*length = pos;

	return JSON_RC_OK;
}


JSON_RC jsonGenGetCoreDataRsp(SRV_String* json, int* length, const getCoreDataRsp_t* gCDRsp){

	JSON_RC rc = JSON_RC_OK;
	int pos = 0;
	int first = 1;

	//add static structure
	pos = json_append(json, pos, "\"serviceName\":\"getCoreDataRsp\",\"serviceParameter\":{",-1 ,*length);

	// add serviceParameter
	pos = json_appendKey(json, pos, "Status", 6, *length, &first);
	pos = json_appendStatus(json, pos, &gCDRsp->status, *length);

	pos = json_appendKey(json, pos, "PVSL", 4, *length, &first);
	pos = json_append(json, pos, "[", 1, *length);

	for(uint32_t i = 0; i<gCDRsp->numPvsl;i++){
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


JSON_RC jsonGenGeneric(SRV_String* json, int* length,const SRV_msgHeader* header, const void* srvStruct, SRV_service_t srvType){

	JSON_RC rc = JSON_RC_OK;

	if(json && (*length<0 || json->length<(unsigned int)*length))
		return JSON_RC_NO_SPACE;

	int pos = 0;
	int first = true;

	//build header
	pos = json_append(json, pos, "{", 1, *length);
	pos = json_appendKey(json, pos, "msgNo", 5, *length, &first);
	pos = json_appendInt(json, pos, SRV_VT_INT32, &header->msgNo, *length);

	pos = json_appendKey(json, pos, "sender", 6, *length, &first);
	rc = jsonGenId(json, length, &pos, &header->sender);
	if(rc)
		return rc;

	pos = json_appendKey(json, pos, "receiver", 8, *length, &first);
	rc = jsonGenId(json, length, &pos, &header->receiver);
	if(rc)
		return rc;

	pos = json_append(json, pos, ",", 1, *length);

	int subLen = *length - pos -1;
	SRV_String subJson;
	SRV_String* pSubJson = NULL;
	if(json){
		pSubJson = &subJson;
		pSubJson->data = json->data+pos;
		pSubJson->length = subLen;
	}

	switch (srvType){
	case SRV_createAASReq:
		rc = jsonGenCreateAASReq(pSubJson, &subLen, srvStruct);
		break;
	case SRV_createAASRsp:
		rc = jsonGenCreateAASRsp(pSubJson, &subLen, srvStruct);
		break;
	case SRV_deleteAASReq:
		rc = jsonGenDeleteAASReq(pSubJson, &subLen, srvStruct);
		break;
	case SRV_deleteAASRsp:
		rc = jsonGenDeleteAASRsp(pSubJson, &subLen, srvStruct);
		break;
	case SRV_createSubModelReq:
		rc = jsonGenCreateSubModelReq(pSubJson, &subLen, srvStruct);
		break;
	case SRV_createSubModelRsp:
		rc = jsonGenCreateSubModelRsp(pSubJson, &subLen, srvStruct);
		break;
	case SRV_deleteSubModelReq:
		rc = jsonGenDeleteSubModelReq(pSubJson, &subLen, srvStruct);
		break;
	case SRV_deleteSubModelRsp:
		rc = jsonGenDeleteSubModelRsp(pSubJson, &subLen, srvStruct);
		break;
	case SRV_createPVSLReq:
		rc = jsonGenCreatePVSLReq(pSubJson, &subLen, srvStruct);
		break;
	case SRV_createPVSLRsp:
		rc = jsonGenCreatePVSLRsp(pSubJson, &subLen, srvStruct);
		break;
	case SRV_deletePVSLReq:
		rc = jsonGenDeletePVSLReq(pSubJson, &subLen, srvStruct);
		break;
	case SRV_deletePVSLRsp:
		rc = jsonGenDeletePVSLRsp(pSubJson, &subLen, srvStruct);
		break;
	case SRV_createPVSReq:
		rc = jsonGenCreatePVSReq(pSubJson, &subLen, srvStruct);
		break;
	case SRV_createPVSRsp:
		rc = jsonGenCreatePVSRsp(pSubJson, &subLen, srvStruct);
		break;
	case SRV_deletePVSReq:
		rc = jsonGenDeletePVSReq(pSubJson, &subLen, srvStruct);
		break;
	case SRV_deletePVSRsp:
		rc = jsonGenDeletePVSRsp(pSubJson, &subLen, srvStruct);
		break;
	case SRV_createLCEReq:
		rc = jsonGenCreateLCEReq(pSubJson, &subLen, srvStruct);
		break;
	case SRV_createLCERsp:
		rc = jsonGenCreateLCERsp(pSubJson, &subLen, srvStruct);
		break;
	case SRV_deleteLCEReq:
		rc = jsonGenDeleteLCEReq(pSubJson, &subLen, srvStruct);
		break;
	case SRV_deleteLCERsp:
		rc = jsonGenDeleteLCERsp(pSubJson, &subLen, srvStruct);
		break;
	case SRV_getPVSReq:
		rc = jsonGenGetPVSReq(pSubJson, &subLen, srvStruct);
		break;
	case SRV_getPVSRsp:
		rc = jsonGenGetPVSRsp(pSubJson, &subLen, srvStruct);
		break;
	case SRV_setPVSReq:
		rc = jsonGenSetPVSReq(pSubJson, &subLen, srvStruct);
		break;
	case SRV_setPVSRsp:
		rc = jsonGenSetPVSRsp(pSubJson, &subLen, srvStruct);
		break;
	case SRV_getLCEReq:
		rc = jsonGenGetLCEReq(pSubJson, &subLen, srvStruct);
		break;
	case SRV_getLCERsp:
		rc = jsonGenGetLCERsp(pSubJson, &subLen, srvStruct);
		break;
	case SRV_setLCEReq:
		rc = jsonGenSetLCEReq(pSubJson, &subLen, srvStruct);
		break;
	case SRV_setLCERsp:
		rc = jsonGenSetLCERsp(pSubJson, &subLen, srvStruct);
		break;
	case SRV_getCoreDataReq:
		rc = jsonGenGetCoreDataReq(pSubJson, &subLen, srvStruct);
		break;
	case SRV_getCoreDataRsp:
		rc = jsonGenGetCoreDataRsp(pSubJson, &subLen, srvStruct);
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


JSON_RC parseJson (const SRV_String* str, SRV_msgHeader** header, void** srvStruct, SRV_service_t* srvType) {

	JSON_RC rc = JSON_RC_OK;
	int n;
	jsmn_parser p;
	jsmntok_t* token;

	*srvStruct = NULL;
	*srvType = SRV_undefined;

	if(!(str && header && srvStruct && srvType))
		return JSON_RC_BAD_INPUT;

	jsmn_init(&p);

	n = jsmn_parse(&p, str->data, str->length, NULL, 0);

	if(n<0){
		return JSON_RC_JSMN_ERROR;
	}

	token = malloc(n*sizeof(jsmntok_t));
	if(!token)
		return JSON_RC_MALLOC_FAIL;

	jsmn_init(&p);
	jsmn_parse(&p, str->data, str->length, token, n);

	// find service name, msgNo and parameter object
	*header = SRV_msgHeader_t_new();
	if(!*header){
		free(token);
		return JSON_RC_MALLOC_FAIL;
	}
	SRV_String serviceName;
	int startParam = -1;
	int fields = 0; // 1 serviceName; 2 serviceParameter; 4 msgNo; 8 Sender; 16 Receiver

	for(int i = 0; i < n && fields<31 && !rc; i++) {
		if(jsoneq(str->data, &token[i], "serviceName")==0){
			i++;
			rc = tok2SrvStr(str->data, &token[i], &serviceName);
			fields |= 1;
		} else if(jsoneq(str->data, &token[i], "serviceParameter")==0) {
			i++;
			if(token[i].type!=JSMN_OBJECT)
				rc = JSON_RC_WRONG_TYPE;

			startParam = i;
			fields |= 2;
		} else if(jsoneq(str->data, &token[i], "msgNo")==0) {
			i++;
			rc = tok2Int(str->data, &token[i], &(*header)->msgNo, SRV_VT_INT32);
			fields |= 4;
		} else if(jsoneq(str->data, &token[i], "sender")==0) {
			i++;
			rc = jsonparseId(token, str, i,n , &(*header)->sender, &i);
			fields |= 8;
		} else if(jsoneq(str->data, &token[i], "receiver")==0) {
			i++;
			rc = jsonparseId(token, str, i, n, &(*header)->receiver, &i);
			fields |= 16;
		}
	}

	if(fields!=31 && !rc)
		rc = JSON_RC_MISSING_FIELD;

	if(rc){
		free(token);
		SRV_String_deleteMembers(&serviceName);
		SRV_msgHeader_t_delete(*header);
		*header = NULL;
		return rc;
	}

	// call function to process message body
	if(strncmp(serviceName.data, "createAASReq", serviceName.length)==0){
		*srvType = SRV_createAASReq;
		*srvStruct = malloc(sizeof(createAASReq_t));
		if(!*srvStruct)
			rc = JSON_RC_MALLOC_FAIL;
		if(!rc)
			rc = jsonparseCreateAASReq(*srvStruct, token, str, n, startParam);
	} else if(strncmp(serviceName.data, "createAASRsp", serviceName.length)==0){
		*srvType = SRV_createAASRsp;
		*srvStruct = malloc(sizeof(createAASRsp_t));
		if(!*srvStruct)
			rc = JSON_RC_MALLOC_FAIL;
		if(!rc)
			rc = jsonparseCreateAASRsp(*srvStruct, token, str, n, startParam);
	} else if(strncmp(serviceName.data, "deleteAASReq", serviceName.length)==0){
		*srvType = SRV_deleteAASReq;
		*srvStruct = malloc(sizeof(deleteAASReq_t));
		if(!*srvStruct)
			rc = JSON_RC_MALLOC_FAIL;
		if(!rc)
			rc = jsonparseDeleteAASReq(*srvStruct, token, str, n, startParam);
	} else if(strncmp(serviceName.data, "deleteAASRsp", serviceName.length)==0){
		*srvType = SRV_deleteAASRsp;
		*srvStruct = malloc(sizeof(deleteAASRsp_t));
		if(!*srvStruct)
			rc = JSON_RC_MALLOC_FAIL;
		if(!rc)
			rc = jsonparseDeleteAASRsp(*srvStruct, token, str, n, startParam);

	}else if(strncmp(serviceName.data, "createSubModelReq", serviceName.length)==0){
		*srvType = SRV_createSubModelReq;
		*srvStruct = malloc(sizeof(createSubModelReq_t));
		if(!*srvStruct)
			rc = JSON_RC_MALLOC_FAIL;
		if(!rc)
			rc = jsonparseCreateSubModelReq(*srvStruct, token, str, n, startParam);
	}else if(strncmp(serviceName.data, "createSubModelRsp", serviceName.length)==0){
		*srvType = SRV_createSubModelRsp;
		*srvStruct = malloc(sizeof(createSubModelRsp_t));
		if(!*srvStruct)
			rc = JSON_RC_MALLOC_FAIL;
		if(!rc)
			rc = jsonparseCreateSubModelRsp(*srvStruct, token, str, n, startParam);
	}else if(strncmp(serviceName.data, "deleteSubModelReq", serviceName.length)==0){
		*srvType = SRV_deleteSubModelReq;
		*srvStruct = malloc(sizeof(deleteSubModelReq_t));
		if(!*srvStruct)
			rc = JSON_RC_MALLOC_FAIL;
		if(!rc)
			rc = jsonparseDeleteSubModelReq(*srvStruct, token, str, n, startParam);
	}else if(strncmp(serviceName.data, "deleteSubModelRsp", serviceName.length)==0){
		*srvType = SRV_deleteSubModelRsp;
		*srvStruct = malloc(sizeof(deleteSubModelRsp_t));
		if(!*srvStruct)
			rc = JSON_RC_MALLOC_FAIL;
		if(!rc)
			rc = jsonparseDeleteSubModelRsp(*srvStruct, token, str, n, startParam);
	} else if(strncmp(serviceName.data, "createPVSLReq", serviceName.length)==0){
		*srvType = SRV_createPVSLReq;
		*srvStruct = malloc(sizeof(createPVSLReq_t));
		if(!*srvStruct)
			rc = JSON_RC_MALLOC_FAIL;
		if(!rc)
			rc = jsonparseCreatePVSLReq(*srvStruct, token, str, n, startParam);
	} else if(strncmp(serviceName.data, "createPVSLRsp", serviceName.length)==0){
		*srvType = SRV_createPVSLRsp;
		*srvStruct = malloc(sizeof(createPVSLRsp_t));
		if(!*srvStruct)
			rc = JSON_RC_MALLOC_FAIL;
		if(!rc)
			rc = jsonparseCreatePVSLRsp(*srvStruct, token, str, n, startParam);
	} else if(strncmp(serviceName.data, "deletePVSLReq", serviceName.length)==0){
		*srvType = SRV_deletePVSLReq;
		*srvStruct = malloc(sizeof(deletePVSLReq_t));
		if(!*srvStruct)
			rc = JSON_RC_MALLOC_FAIL;
		if(!rc)
			rc = jsonparseDeletePVSLReq(*srvStruct, token, str, n, startParam);
	} else if(strncmp(serviceName.data, "deletePVSLRsp", serviceName.length)==0){
		*srvType = SRV_deletePVSLRsp;
		*srvStruct = malloc(sizeof(deletePVSLRsp_t));
		if(!*srvStruct)
			rc = JSON_RC_MALLOC_FAIL;
		if(!rc)
			rc = jsonparseDeletePVSLRsp(*srvStruct, token, str, n, startParam);
	} else if(strncmp(serviceName.data, "createPVSReq", serviceName.length)==0){
		*srvType = SRV_createPVSReq;
		*srvStruct = malloc(sizeof(createPVSReq_t));
		if(!*srvStruct)
			rc = JSON_RC_MALLOC_FAIL;
		if(!rc)
			rc = jsonparseCreatePVSReq(*srvStruct, token, str, n, startParam);
	} else if(strncmp(serviceName.data, "createPVSRsp", serviceName.length)==0){
		*srvType = SRV_createPVSRsp;
		*srvStruct = malloc(sizeof(createPVSRsp_t));
		if(!*srvStruct)
			rc = JSON_RC_MALLOC_FAIL;
		if(!rc)
			rc = jsonparseCreatePVSRsp(*srvStruct, token, str, n, startParam);
	} else if(strncmp(serviceName.data, "deletePVSReq", serviceName.length)==0){
		*srvType = SRV_deletePVSReq;
		*srvStruct = malloc(sizeof(deletePVSReq_t));
		if(!*srvStruct)
			rc = JSON_RC_MALLOC_FAIL;
		if(!rc)
			rc = jsonparseDeletePVSReq(*srvStruct, token, str, n, startParam);
	} else if(strncmp(serviceName.data, "deletePVSRsp", serviceName.length)==0){
		*srvType = SRV_deletePVSRsp;
		*srvStruct = malloc(sizeof(deletePVSRsp_t));
		if(!*srvStruct)
			rc = JSON_RC_MALLOC_FAIL;
		if(!rc)
			rc = jsonparseDeletePVSRsp(*srvStruct, token, str, n, startParam);
	} else if(strncmp(serviceName.data, "createLCEReq", serviceName.length)==0){
		*srvType = SRV_createLCEReq;
		*srvStruct = malloc(sizeof(createLCEReq_t));
		if(!*srvStruct)
			rc = JSON_RC_MALLOC_FAIL;
		if(!rc)
			rc = jsonparseCreateLCEReq(*srvStruct, token, str, n, startParam);
	} else if(strncmp(serviceName.data, "createLCERsp", serviceName.length)==0){
		*srvType = SRV_createLCERsp;
		*srvStruct = malloc(sizeof(createLCERsp_t));
		if(!*srvStruct)
			rc = JSON_RC_MALLOC_FAIL;
		if(!rc)
			rc = jsonparseCreateLCERsp(*srvStruct, token, str, n, startParam);
	} else if(strncmp(serviceName.data, "deleteLCEReq", serviceName.length)==0){
		*srvType = SRV_deleteLCEReq;
		*srvStruct = malloc(sizeof(deleteLCEReq_t));
		if(!*srvStruct)
			rc = JSON_RC_MALLOC_FAIL;
		if(!rc)
			rc = jsonparseDeleteLCEReq(*srvStruct, token, str, n, startParam);
	} else if(strncmp(serviceName.data, "deleteLCERsp", serviceName.length)==0){
		*srvType = SRV_deleteLCERsp;
		*srvStruct = malloc(sizeof(deleteLCERsp_t));
		if(!*srvStruct)
			rc = JSON_RC_MALLOC_FAIL;
		if(!rc)
			rc = jsonparseDeleteLCERsp(*srvStruct, token, str, n, startParam);
	} else if(strncmp(serviceName.data, "getPVSReq", serviceName.length)==0){
		*srvType = SRV_getPVSReq;
		*srvStruct = malloc(sizeof(getPVSReq_t));
		if(!*srvStruct)
			rc = JSON_RC_MALLOC_FAIL;
		if(!rc)
			rc = jsonparseGetPVSReq(*srvStruct, token, str, n, startParam);
	} else if(strncmp(serviceName.data, "getPVSRsp", serviceName.length)==0){
		*srvType = SRV_getPVSRsp;
		*srvStruct = malloc(sizeof(getPVSRsp_t));
		if(!*srvStruct)
			rc = JSON_RC_MALLOC_FAIL;
		if(!rc)
			rc = jsonparseGetPVSRsp(*srvStruct, token, str, n, startParam);
	} else if(strncmp(serviceName.data, "setPVSReq", serviceName.length)==0){
		*srvType = SRV_setPVSReq;
		*srvStruct = malloc(sizeof(setPVSReq_t));
		if(!*srvStruct)
			rc = JSON_RC_MALLOC_FAIL;
		if(!rc)
			rc = jsonparseSetPVSReq(*srvStruct, token, str, n, startParam);
	} else if(strncmp(serviceName.data, "setPVSRsp", serviceName.length)==0){
		*srvType = SRV_setPVSRsp;
		*srvStruct = malloc(sizeof(setPVSRsp_t));
		if(!*srvStruct)
			rc = JSON_RC_MALLOC_FAIL;
		if(!rc)
			rc = jsonparseSetPVSRsp(*srvStruct, token, str, n, startParam);
	} else if(strncmp(serviceName.data, "getLCEReq", serviceName.length)==0){
		*srvType = SRV_getLCEReq;
		*srvStruct = malloc(sizeof(getLCEReq_t));
		if(!*srvStruct)
			rc = JSON_RC_MALLOC_FAIL;
		if(!rc)
			rc = jsonparseGetLCEReq(*srvStruct, token, str, n, startParam);
	} else if(strncmp(serviceName.data, "getLCERsp", serviceName.length)==0){
		*srvType = SRV_getLCERsp;
		*srvStruct = malloc(sizeof(getLCERsp_t));
		if(!*srvStruct)
			rc = JSON_RC_MALLOC_FAIL;
		if(!rc)
			rc = jsonparseGetLCERsp(*srvStruct, token, str, n, startParam);
	} else if(strncmp(serviceName.data, "setLCEReq", serviceName.length)==0){
		*srvType = SRV_setLCEReq;
		*srvStruct = malloc(sizeof(setLCEReq_t));
		if(!*srvStruct)
			rc = JSON_RC_MALLOC_FAIL;
		if(!rc)
			rc = jsonparseSetLCEReq(*srvStruct, token, str, n, startParam);
	} else if(strncmp(serviceName.data, "setLCERsp", serviceName.length)==0){
		*srvType = SRV_setLCERsp;
		*srvStruct = malloc(sizeof(setLCERsp_t));
		if(!*srvStruct)
			rc = JSON_RC_MALLOC_FAIL;
		if(!rc)
			rc = jsonparseSetLCERsp(*srvStruct, token, str, n, startParam);
	} else if(strncmp(serviceName.data, "getCoreDataReq", serviceName.length)==0){
		*srvType = SRV_getCoreDataReq;
		*srvStruct = malloc(sizeof(getCoreDataReq_t));
		if(!*srvStruct)
			rc = JSON_RC_MALLOC_FAIL;
		if(!rc)
			rc = jsonparseGetCoreDataReq(*srvStruct, token, str, n, startParam);
	} else if(strncmp(serviceName.data, "getCoreDataRsp", serviceName.length)==0){
		*srvType = SRV_getCoreDataRsp;
		*srvStruct = malloc(sizeof(getCoreDataRsp_t));
		if(!*srvStruct)
			rc = JSON_RC_MALLOC_FAIL;
		if(!rc)
			rc = jsonparseGetCoreDataRsp(*srvStruct, token, str, n, startParam);
	} else {
		rc = JSON_RC_UNDEF;
	}

	SRV_String_deleteMembers(&serviceName);
	free(token);
	if(rc){
		if(*srvStruct){
			SRV_serviceGeneric_delete(*srvStruct, *srvType);
			*srvStruct = NULL;
		}
		return rc;
	}

	return JSON_RC_OK;
}


JSON_RC genJson(SRV_String** js, const SRV_msgHeader* header, const void* srvStruct, SRV_service_t srvType){

	JSON_RC rc = JSON_RC_OK;
	int len;

	if(!(js && header && srvStruct))
		return JSON_RC_BAD_INPUT;

	*js = NULL;

	rc = jsonGenGeneric(NULL, &len, header, srvStruct, srvType);

	if(rc)
		return rc;

	if(len<0)
		return JSON_RC_BAD_INPUT;

	*js = malloc(sizeof(SRV_String));
	if(!*js)
		return JSON_RC_MALLOC_FAIL;
	(*js)->data = malloc(len+1);
	if(!(*js)->data){
		//TODO should this be free?
		free(*js);
		return JSON_RC_MALLOC_FAIL;
	}
	(*js)->length = len;

	rc = jsonGenGeneric(*js, &len, header, srvStruct, srvType);

	(*js)->data[len] = '\0';

	return JSON_RC_OK;
}
