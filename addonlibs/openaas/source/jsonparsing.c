/*
 * jsonparsing.c
 *
 *  Created on: Dec 8, 2016
 *      Author: lars
 */

#include "jsonparsing.h"

#define MAX_NUMBUFFER 20
#define MAX_DATEBUFFER 30

// maximal index of constant arrays
#define JSON_RE_MAX 6
#define JSON_ES_MAX 5
#define JSON_ID_MAX 2
#define JSON_VIEW_MAX 9

const SRV_String JSON_RE_STR [JSON_RE_MAX+1] = {
		{.data = ">" , .length = 2},
		{.data = ">=", .length = 2},
		{.data = "==", .length = 2},
		{.data = "!=", .length = 2},
		{.data = "<=", .length = 2},
		{.data = "<" , .length = 2},
		{.data = ""  , .length = 0}
};
const SRV_String JSON_ES_STR [JSON_ES_MAX+1] = {
		{.data = "Assurance"  , .length = 9},
		{.data = "Setting"	  , .length = 7},
		{.data = "Measurement", .length = 11},
		{.data = "Requirement", .length = 11},
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

	SRV_String_setCopy(str, js+t->start, t->end-t->start);

	return JSON_RC_OK;
}

JSON_RC tok2Prim(const char* js, const jsmntok_t* t, SRV_valType_t* type, void* value){
	*type = SRV_VT_undefined;
	bool isfloat = false;
	if(!(t->type==JSMN_PRIMITIVE))
		return JSON_RC_WRONG_TYPE;

	switch (js[t->start]){
	case 't':case 'T':
		*type = SRV_VT_BOOL;
		if(value)
			*(bool*)value = true;
		break;
	case 'f':case 'F':
		*type = SRV_VT_BOOL;
		if(value)
			*(bool*)value = false;
		break;
	case 'n':case 'N':
		*type = SRV_VT_NULL;
		break;
	case '0':case '1':case '2':case '3':case '4':case '5':
	case '6':case '7':case '8':case '9':case '.':
		// number
		for(int i = t->start; i<=t->end; i++){
			if(js[i]=='.'){
				isfloat = true;
				break;
			}
		}
		if(isfloat){
			*type = SRV_VT_DOUBLE;
			if(value)
				*(double*)value = strtod(&js[t->start], NULL);
		} else {
			*type = SRV_VT_INT64;
			if(value)
				*(long long int*)value = strtoll(&js[t->start], NULL, 10);
		}
		break;
	default:
		return JSON_RC_WRONG_VALUE;
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

JSON_RC tok2Bool(const char* js, const jsmntok_t* t, uint8_t* value){
	SRV_valType_t type = SRV_VT_undefined;
	if(!(t->type==JSMN_PRIMITIVE))
		return JSON_RC_WRONG_TYPE;

	tok2Prim(js, t, &type, NULL);
	if(type!=SRV_VT_BOOL)
		return JSON_RC_WRONG_VALUE;

	tok2Prim(js, t, &type, value);

	return 0;
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
	case SRV_VT_INT32:
		bufLen = snprintf(numbuffer, MAX_NUMBUFFER, "%i", *(int*)value);
		if(!(bufLen<MAX_NUMBUFFER))
			return JSON_RC_NO_SPACE;
		pos = json_append(js, start, numbuffer, bufLen, maxLen);
		break;
	case SRV_VT_INT64:
		bufLen = snprintf(numbuffer, MAX_NUMBUFFER, "%lli", *(long long int*)value);
		if(!(bufLen<MAX_NUMBUFFER))
			return JSON_RC_NO_SPACE;
		pos = json_append(js, start, numbuffer, bufLen, maxLen);
		break;
	case SRV_VT_UINT32:
		bufLen = snprintf(numbuffer, MAX_NUMBUFFER, "%i", *(unsigned int*)value);
		if(!(bufLen<MAX_NUMBUFFER))
			return JSON_RC_NO_SPACE;
		pos = json_append(js, start, numbuffer, bufLen, maxLen);
		break;
	case SRV_VT_UINT64:
		bufLen = snprintf(numbuffer, MAX_NUMBUFFER, "%lli", *(unsigned long long int*)value);
		if(!(bufLen<MAX_NUMBUFFER))
			return JSON_RC_NO_SPACE;
		pos = json_append(js, start, numbuffer, bufLen, maxLen);
		break;
	case SRV_VT_SINGLE:
		bufLen = snprintf(numbuffer, MAX_NUMBUFFER, "%f", *(float*)value);
		if(!(bufLen<MAX_NUMBUFFER))
			return JSON_RC_NO_SPACE;
		pos = json_append(js, start, numbuffer, bufLen, maxLen);
		break;
	case SRV_VT_DOUBLE:
		bufLen = snprintf(numbuffer, MAX_NUMBUFFER, "%f", *(double*)value);
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

int dateTimeToISO(SRV_DateTime t, char* iso, int maxLen){
	int len;
	int psec = t % JSON_SEC_TO_DATETIME; // micro sec
	time_t sec = t / JSON_SEC_TO_DATETIME;
	struct tm* tm = gmtime(&sec);

	if(!psec){
		len = strftime(iso, maxLen, "%Y-%m-%dT%H:%M:%SZ", tm);
	} else {
		len = strftime(iso, maxLen, "%Y-%m-%dT%H:%M:%S.", tm);
		if(len)
			len += snprintf(iso+len, maxLen, "%06iZ", psec);
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
			for(;i<6;i++){
				psec = psec * 10;
			}
		else if (i>6) {
			for(;i>6;i--){
				psec = psec / 10;
			}
		}
	}

	unixTime = mktime(&tm);

	*jsonTime = unixTime * JSON_SEC_TO_DATETIME + psec;

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

	for(int i = 0; i < n && fields<7; i++) {
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
		} else if(jsoneq(str->data, &token[i], "Sender")==0) {
			i++;
			rc = jsonparseId(token, str, i,n , &(*header)->sender, &i);
			if(rc)
				return rc;
			fields |= 8;
		} else if(jsoneq(str->data, &token[i+1], "Receiver")==0) {
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
		jsonparseCreateAASReq(*srvStruct, token, str, n, startParam);
	} else if(strncmp(serviceName.data, "createAASRsp", serviceName.length)==0){
		*srvType = SRV_createAASRsp;
		*srvStruct = malloc(sizeof(createAASRsp_t));
		jsonparseCreateAASRsp(*srvStruct, token, str, n, startParam);
	} else if(strncmp(serviceName.data, "deleteAASReq", serviceName.length)==0){
		*srvType = SRV_deleteAASReq;
		*srvStruct = malloc(sizeof(deleteAASReq_t));
		jsonparseDeleteAASReq(*srvStruct, token, str, n, startParam);
	} else if(strncmp(serviceName.data, "deleteAASRsp", serviceName.length)==0){
		*srvType = SRV_deleteAASRsp;
		*srvStruct = malloc(sizeof(deleteAASRsp_t));
		jsonparseDeleteAASRsp(*srvStruct, token, str, n, startParam);
	} else if(strncmp(serviceName.data, "createPVSLReq", serviceName.length)==0){
		*srvType = SRV_createPVSLReq;
		*srvStruct = malloc(sizeof(createPVSLReq_t));
		jsonparseCreatePVSLReq(*srvStruct, token, str, n, startParam);
	} else if(strncmp(serviceName.data, "createPVSLRsp", serviceName.length)==0){
		*srvType = SRV_createPVSLRsp;
		*srvStruct = malloc(sizeof(createPVSLRsp_t));
		jsonparseCreatePVSLRsp(*srvStruct, token, str, n, startParam);
	} else if(strncmp(serviceName.data, "deletePVSLReq", serviceName.length)==0){
		*srvType = SRV_deletePVSLReq;
		*srvStruct = malloc(sizeof(deletePVSLReq_t));
		jsonparseDeletePVSLReq(*srvStruct, token, str, n, startParam);
	} else if(strncmp(serviceName.data, "deletePVSLRsp", serviceName.length)==0){
		*srvType = SRV_deletePVSLRsp;
		*srvStruct = malloc(sizeof(deletePVSLRsp_t));
		jsonparseDeletePVSLRsp(*srvStruct, token, str, n, startParam);
	} else if(strncmp(serviceName.data, "createPVSReq", serviceName.length)==0){
		*srvType = SRV_createPVSReq;
		*srvStruct = malloc(sizeof(createPVSReq_t));
		jsonparseCreatePVSReq(*srvStruct, token, str, n, startParam);
	} else if(strncmp(serviceName.data, "createPVSRsp", serviceName.length)==0){
		*srvType = SRV_createPVSRsp;
		*srvStruct = malloc(sizeof(createPVSRsp_t));
		jsonparseCreatePVSRsp(*srvStruct, token, str, n, startParam);
	} else if(strncmp(serviceName.data, "deletePVSReq", serviceName.length)==0){
		*srvType = SRV_deletePVSReq;
		*srvStruct = malloc(sizeof(deletePVSReq_t));
		jsonparseDeletePVSReq(*srvStruct, token, str, n, startParam);
	} else if(strncmp(serviceName.data, "deletePVSRsp", serviceName.length)==0){
		*srvType = SRV_deletePVSRsp;
		*srvStruct = malloc(sizeof(deletePVSRsp_t));
		jsonparseDeletePVSRsp(*srvStruct, token, str, n, startParam);
	} else if(strncmp(serviceName.data, "createLCEReq", serviceName.length)==0){
		*srvType = SRV_createLCEReq;
		*srvStruct = malloc(sizeof(createLCEReq_t));
		jsonparseCreateLCEReq(*srvStruct, token, str, n, startParam);
	} else if(strncmp(serviceName.data, "createLCERsp", serviceName.length)==0){
		*srvType = SRV_createLCERsp;
		*srvStruct = malloc(sizeof(createLCERsp_t));
		jsonparseCreateLCERsp(*srvStruct, token, str, n, startParam);
	} else if(strncmp(serviceName.data, "deleteLCEReq", serviceName.length)==0){
		*srvType = SRV_deleteLCEReq;
		*srvStruct = malloc(sizeof(deleteLCEReq_t));
		jsonparseDeleteLCEReq(*srvStruct, token, str, n, startParam);
	} else if(strncmp(serviceName.data, "deleteLCERsp", serviceName.length)==0){
		*srvType = SRV_deleteLCERsp;
		*srvStruct = malloc(sizeof(deleteLCERsp_t));
		jsonparseDeleteLCERsp(*srvStruct, token, str, n, startParam);
	} else if(strncmp(serviceName.data, "getPVSReq", serviceName.length)==0){
		*srvType = SRV_getPVSReq;
		*srvStruct = malloc(sizeof(getPVSReq_t));
		jsonparseGetPVSReq(*srvStruct, token, str, n, startParam);
	} else if(strncmp(serviceName.data, "getPVSRsp", serviceName.length)==0){
		*srvType = SRV_getPVSRsp;
		*srvStruct = malloc(sizeof(getPVSRsp_t));
		jsonparseGetPVSRsp(*srvStruct, token, str, n, startParam);
	} else if(strncmp(serviceName.data, "setPVSReq", serviceName.length)==0){
		*srvType = SRV_setPVSReq;
		*srvStruct = malloc(sizeof(setPVSReq_t));
		jsonparseSetPVSReq(*srvStruct, token, str, n, startParam);
	} else if(strncmp(serviceName.data, "setPVSRsp", serviceName.length)==0){
		*srvType = SRV_setPVSRsp;
		*srvStruct = malloc(sizeof(setPVSRsp_t));
		jsonparseSetPVSRsp(*srvStruct, token, str, n, startParam);
	} else if(strncmp(serviceName.data, "getLCEReq", serviceName.length)==0){
		*srvType = SRV_getLCEReq;
		*srvStruct = malloc(sizeof(getLCEReq_t));
		jsonparseGetLCEReq(*srvStruct, token, str, n, startParam);
	} else if(strncmp(serviceName.data, "getLCERsp", serviceName.length)==0){
		*srvType = SRV_getLCERsp;
		*srvStruct = malloc(sizeof(getLCERsp_t));
		jsonparseGetLCERsp(*srvStruct, token, str, n, startParam);
	} else if(strncmp(serviceName.data, "setLCEReq", serviceName.length)==0){
		*srvType = SRV_setLCEReq;
		*srvStruct = malloc(sizeof(setLCEReq_t));
		jsonparseSetLCEReq(*srvStruct, token, str, n, startParam);
	} else if(strncmp(serviceName.data, "setLCERsp", serviceName.length)==0){
		*srvType = SRV_setLCERsp;
		*srvStruct = malloc(sizeof(setLCERsp_t));
		jsonparseSetLCERsp(*srvStruct, token, str, n, startParam);
	}


	SRV_String_deleteMembers(&serviceName);
	free(token);
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
		} else if(jsoneq(str->data, &t[j], "IdSpec")){
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
		} else if(jsoneq(str->data, &t[j], "RelationalExpression")==0){
			j++;
			SRV_String tmpStr;
			rc = tok2SrvStr(str->data, &t[j], &tmpStr);
			if(rc)
				return rc;
			pvs->relationalExpression = SRV_RE_undefined;
			for(int i = 0; i < JSON_RE_MAX && pvs->relationalExpression == SRV_RE_undefined; i++){
				if(srvStrEq(&tmpStr, &JSON_RE_STR[i])==0){
					pvs->relationalExpression = i;
				}
			}
			SRV_String_deleteMembers(&tmpStr);
		} else if(jsoneq(str->data, &t[j], "View")==0){
			j++;
			SRV_String tmpStr;
			rc = tok2SrvStr(str->data, &t[j], &tmpStr);
			if(rc)
				return rc;
			pvs->view = SRV_VIEW_undefined;
			for(int i; i < JSON_VIEW_MAX && pvs->view == SRV_VIEW_undefined; i++){
				if(srvStrEq(&tmpStr, &JSON_VIEW_STR[i])==0)
					pvs->view = i;
			}
			SRV_String_deleteMembers(&tmpStr);
		} else if(jsoneq(str->data, &t[j], "Unit")==0){
			j++;
			rc = tok2SrvStr(str->data, &t[j], &pvs->unit);
			if(rc)
				return rc;
			pvs->hasUnit = true;
		} else if(jsoneq(str->data, &t[j], "PropertyReference")){
			j++;
			rc = jsonparseId(t, str, j, n, &pvs->propertyReference, &j);
			if(rc)
				return rc;
		} else if(jsoneq(str->data, &t[j], "Value")==0){
			j++;
			// parse value with type
			if(t[j].type==JSMN_STRING){
				tok2SrvStr(str->data, &t[j], pvs->value);
				pvs->valType = SRV_VT_STRING;
			} else if(t[j].type==JSMN_PRIMITIVE){
				tok2Prim(str->data, &t[j], &pvs->valType, pvs->value);
			}
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

	SRV_String time;

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
			rc = jsonparseId(t, str, j, n, &lce->creatingInstanceId, &j);
			if(rc)
				return rc;
		} else if(jsoneq(str->data, &t[j], "WritingInstanceId")==0){
			j++;
			rc = jsonparseId(t, str, j, n, &lce->writingInstanceId, &j);
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
			rc = tok2SrvStr(str->data, &t[j], &time);
			if(rc){
				return rc;
			}
			rc =  ISOToDateTime(time.data, time.length, &lce->dataTime);
			SRV_String_deleteMembers(&time);
			if(rc)
				return rc;
			lce->hastDataTime = true;
		} else if(jsoneq(str->data, &t[j], "Data")==0){
			j++;
			rc = tok2Prim(str->data, &t[j], &lce->dataType, lce->data);
			if(rc)
				return rc;
		}
		j++;
	}

	*last = j - 1;

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
	return -1;
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
	return -1;
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
	return -1;
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
	return -1;
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
					cPvsReq->pvs.propertyReference.idType!=SRV_IDT_undefined &&
					cPvsReq->pvs.relationalExpression!=SRV_RE_undefined &&
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
	return -1;
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
	return -1;
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
					cLceReq->lce.creatingInstanceId.idType!=SRV_IDT_undefined &&
					cLceReq->lce.writingInstanceId.idType!=SRV_IDT_undefined &&
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
	return -1;
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
	return -1;
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
	return -1;
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
					sPvsReq->pvs.propertyReference.idType!=SRV_IDT_undefined &&
					sPvsReq->pvs.expressionSemantic!=SRV_ES_undefined &&
					sPvsReq->pvs.relationalExpression!=SRV_RE_undefined &&
					sPvsReq->pvs.valType!=SRV_VT_undefined &&
					sPvsReq->pvs.view!=SRV_VIEW_undefined)
				fields |= 2;
		}
		i++;
	}

	if(fields!=7)
		return JSON_RC_MISSING_FIELD;

	return JSON_RC_OK;
}

JSON_RC jsonparseSetPVSRsp(setPVSRsp_t* sPvsRsp, const jsmntok_t* t, const SRV_String* js, int n, int start){
	return -1;
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
	return -1;
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
					sLceReq->lce.creatingInstanceId.idType!=SRV_IDT_undefined &&
					sLceReq->lce.writingInstanceId.idType!=SRV_IDT_undefined &&
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
	return -1;
}


JSON_RC jsonGenGeneric(SRV_String* json, int* length,const SRV_msgHeader* header, const void* srvStruct, SRV_service_t srvType){

	JSON_RC rc;

	//build header

	int pos = 0;

	int first = true;

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
	if(p->relationalExpression!=SRV_RE_undefined){
		pos = json_appendKey(json, pos, "LogicalExpression", -1, *length, &first);
		pos = json_appendSrvStr(json, pos, &JSON_RE_STR[p->relationalExpression], *length);
	}
	if(p->valType!=SRV_VT_undefined){
		pos = json_appendKey(json, pos, "Value", -1, *length, &first);
		// parse value
		pos = json_appendAny(json, pos, p->value, p->valType, *length);
	}
	if(p->hasUnit){
		pos = json_appendKey(json, pos, "Unit", -1, *length, &first);
		pos = json_appendSrvStr(json, pos, &p->unit, *length);
	}
	if(p->propertyReference.idType!=SRV_IDT_undefined){
		rc = jsonGenId(json, length, &pos, &p->propertyReference);
		if(rc)
			return rc;
	}
	if(p->view!=SRV_VIEW_undefined){
		pos = json_appendKey(json, pos, "View", -1, *length, &first);
		pos = json_appendSrvStr(json, pos, &JSON_VIEW_STR[p->view], *length);
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
	if(l->creatingInstanceId.idType!=SRV_IDT_undefined){
		pos = json_appendKey(json, pos, "CreatingInstanceId", -1, *length, &first);
		pos = jsonGenId(json, length, &pos, &l->creatingInstanceId);
	}
	if(l->writingInstanceId.idType!=SRV_IDT_undefined){
		pos = json_appendKey(json, pos, "WritingInstanceId", -1, *length, &first);
		pos = jsonGenId(json, length, &pos, &l->writingInstanceId);
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
		pos = json_appendKey(json, pos, "Data", -1, *length, &first);
		pos = json_appendAny(json, pos, l->data, l->dataType, *length);
	}
	if(l->hastDataTime){
		pos = json_appendKey(json, pos, "DataTime", -1, *length, &first);
		pos = json_appendIsoTime(json, pos, *length, l->dataTime);
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

	pos = json_appendKey(json, pos, "idType", -1, *length, &first);
	pos = json_appendSrvStr(json, pos, &JSON_ID_STR[i->idType], *length);

	pos = json_appendKey(json, pos, "idSpec", -1, *length, &first);
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

	if(json && json->length<*length)
		return JSON_RC_NO_SPACE;

	//add static structure
	pos = json_appendKey(json, pos, "serviceName", -1, *length, &first);
	pos = json_appendString(json, pos, serviceName, -1, *length);
	pos = json_appendKey(json, pos, "serviceParameter", -1, *length, &first);

	// add serviceParameter
	pos = json_append(json, pos, "{\"Status\":", -1, *length);
	pos = json_appendAny(json, pos, &status, SRV_VT_INT32, *length);
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

	if(json && json->length<*length)
		return JSON_RC_NO_SPACE;

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

	if(json && json->length<*length)
		return JSON_RC_NO_SPACE;

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

	if(json && json->length<*length)
		return JSON_RC_NO_SPACE;

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

	if(json && json->length<*length)
		return JSON_RC_NO_SPACE;

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

	if(json && json->length<*length)
		return JSON_RC_NO_SPACE;

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

	if(json && json->length<*length)
		return JSON_RC_NO_SPACE;

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

	if(json && json->length<*length)
		return JSON_RC_NO_SPACE;

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

	if(json && json->length<*length)
		return JSON_RC_NO_SPACE;

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

	if(json && json->length<*length)
		return JSON_RC_NO_SPACE;

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

	if(json && json->length<*length)
		return JSON_RC_NO_SPACE;

	//add static structure
	pos = json_append(json, pos, "\"serviceName\":\"getPVSRsp\",\"serviceParameter\":",-1 ,*length);

	// add serviceParameter
	pos = json_append(json, pos, "{\"Status\":", -1, *length);

	pos = json_appendAny(json, pos, &gPvsRsp->status, SRV_VT_INT32, *length);

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

	if(json && json->length<*length)
		return JSON_RC_NO_SPACE;

	//add static structure
	pos = json_append(json, pos, "\"serviceName\":\"setPvsReq\",\"serviceParameter\":{",-1 ,*length);

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

	if(json && json->length<*length)
		return JSON_RC_NO_SPACE;

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

	if(json && json->length<*length)
		return JSON_RC_NO_SPACE;

	//add static structure
	pos = json_append(json, pos, "\"serviceName\":\"getLCERsp\",\"serviceParameter\":",-1 ,*length);

	// add serviceParameter
	pos = json_append(json, pos, "{\"Status\":", -1, *length);
	pos = json_appendAny(json, pos, &gLceRsp->status, SRV_VT_INT32, *length);

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

	if(json && json->length<*length)
		return JSON_RC_NO_SPACE;

	//add static structure
	pos = json_append(json, pos, "\"serviceName\":\"setLceReq\",\"serviceParameter\":{",-1 ,*length);

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
