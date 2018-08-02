#include "json_helper.h"

#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "libov/ov_logfile.h"


int jsoneq(const char *json, const jsmntok_t *tok, const char *s) {
	if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
			strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
		return 0;
	}
	return -1;
}

OV_DLLFNCEXPORT void json_data_init(json_data* jsonData){
	jsonData->js = NULL;
	jsonData->num_token = 0;
	jsonData->token = NULL;
	return;
}

OV_DLLFNCEXPORT void json_data_deleteMembers(json_data* jsonData){
	ov_string_setvalue(&jsonData->js, NULL);
	if (jsonData->token)
		free(jsonData->token);
	jsonData->token = NULL;
	jsonData->num_token = 0;
	return;
}

OV_DLLFNCEXPORT void request_data_init(request_data *requestData){
	requestData->header.endpointReceiver = NULL;
	requestData->header.endpointSender = NULL;
	requestData->header.messageID = NULL;
	requestData->header.messageType = 0;
	requestData->header.protocolType = 0;
	json_data_init(&requestData->body);
	return;
}

OV_DLLFNCEXPORT void request_data_deleteMembers(request_data *requestData){
	ov_string_setvalue(&requestData->header.endpointReceiver, NULL);
	ov_string_setvalue(&requestData->header.endpointSender, NULL);
	ov_string_setvalue(&requestData->header.messageID, NULL);
	requestData->header.messageType = 0;
	requestData->header.protocolType = 0;
	json_data_deleteMembers(&requestData->body);
	return;
}

OV_DLLFNCEXPORT void response_data_init(response_data *responseData){
	responseData->header.endpointReceiver = NULL;
	responseData->header.endpointSender = NULL;
	responseData->header.messageID = NULL;
	responseData->header.referToMessageID = NULL;
	responseData->header.errorMessage = NULL;
	responseData->header.messageType = 0;
	responseData->header.errorFlag = FALSE;
	json_data_init(&responseData->body);
	return;
}

OV_DLLFNCEXPORT void response_data_deleteMembers(response_data *responseData){
	ov_string_setvalue(&responseData->header.referToMessageID, NULL);
	ov_string_setvalue(&responseData->header.endpointReceiver, NULL);
	ov_string_setvalue(&responseData->header.endpointSender, NULL);
	ov_string_setvalue(&responseData->header.messageID, NULL);
	ov_string_setvalue(&responseData->header.errorMessage, NULL);
	responseData->header.messageType = 0;
	responseData->header.errorFlag = FALSE;
	json_data_deleteMembers(&responseData->body);
	return;
}

OV_DLLFNCEXPORT OV_RESULT jsonTokenize(json_data* jsonData) {
	jsmn_parser p;

	jsmn_init(&p);

	jsonData->num_token = jsmn_parse(&p, jsonData->js, strlen(jsonData->js), NULL, 0);

	if(jsonData->num_token<0)
		return OV_ERR_BADINITPARAM;

	jsonData->token = malloc(jsonData->num_token*sizeof(jsmntok_t));
	if(!jsonData->token)
		return OV_ERR_GENERIC;

	jsmn_init(&p);
	jsmn_parse(&p, jsonData->js, strlen(jsonData->js), jsonData->token, jsonData->num_token);

	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT jsonRequestParse(request_data* requestData, const OV_STRING message) {
	json_data jsonData;
	json_data_init(&jsonData);
	jsonData.js = message;

	if(jsonTokenize(&jsonData) != OV_ERR_OK) {
		return OV_ERR_BADPARAM;
	}

	// Finding Header and Body Token
	OV_STRING_VEC tags;
	tags.value = NULL;
	tags.veclen = 0;
	Ov_SetDynamicVectorLength(&tags, 2, STRING);
	ov_string_setvalue(&tags.value[0], "header");
	ov_string_setvalue(&tags.value[1], "body");
	OV_UINT_VEC tokenIndex;
	tokenIndex.value = NULL;
	tokenIndex.veclen = 0;
	Ov_SetDynamicVectorLength(&tokenIndex, 2, UINT);

	jsonGetTokenIndexByTags(tags, jsonData, 0, &tokenIndex);

	// get tokenSize for header and body
	OV_UINT tokenSizeHeader = 0;
	OV_BOOL countHeaderToken = FALSE;
	OV_UINT tokenSizeBody = 0;
	OV_BOOL countBodyToken = FALSE;
	for (OV_UINT i = 0; i < jsonData.num_token; i++){
		if (i == tokenIndex.value[0]){ // start counting Header tokens
			countHeaderToken = TRUE;
		}
		if (countHeaderToken == TRUE){
			if (jsonData.token[i].parent != 0 || tokenSizeHeader == 0)
				tokenSizeHeader++;
			else
				countHeaderToken = FALSE;
		}
		if (i == tokenIndex.value[1]){ // start counting Body tokens
			countBodyToken = TRUE;
		}
		if (countBodyToken == TRUE){
			if (jsonData.token[i].parent != 0 || tokenSizeBody == 0)
				tokenSizeBody++;
			else
				countBodyToken = FALSE;
		}
	}
	// Parse Header
	json_data jsonData_header;
	json_data_init(&jsonData_header);
	char* tmpString = malloc(jsonData.token[tokenIndex.value[0]+1].end + 2 - jsonData.token[tokenIndex.value[0]].start);
	strncpy(tmpString, &jsonData.js[jsonData.token[tokenIndex.value[0]].start - 1], jsonData.token[tokenIndex.value[0]+1].end + 1 - jsonData.token[tokenIndex.value[0]].start);
	tmpString[jsonData.token[tokenIndex.value[0]+1].end - jsonData.token[tokenIndex.value[0]].start + 1] = '\0';
	ov_string_setvalue(&jsonData_header.js, tmpString);
	free(tmpString);
	tmpString = NULL;
	jsonData_header.num_token = tokenSizeHeader;
	jsonData_header.token = malloc(sizeof(jsmntok_t)*jsonData_header.num_token);
	for (OV_UINT i = 0; i < jsonData_header.num_token; i++){
		memcpy(&jsonData_header.token[i], &jsonData.token[tokenIndex.value[0]+i], sizeof(jsmntok_t));
		jsonData_header.token[i].start = jsonData_header.token[i].start - jsonData.token[tokenIndex.value[0]].start + 1;
		jsonData_header.token[i].end = jsonData_header.token[i].end - jsonData.token[tokenIndex.value[0]].start + 1;
		jsonData_header.token[i].parent = jsonData_header.token[i].parent - tokenIndex.value[0];
	}

	Ov_SetDynamicVectorLength(&tags, 5, STRING);
	OV_STRING_VEC values;
	values.value = NULL;
	values.veclen = 0;
	Ov_SetDynamicVectorLength(&values, 5, STRING);

	ov_string_setvalue(&tags.value[0], "endpointSender");
	ov_string_setvalue(&tags.value[1], "endpointReceiver");
	ov_string_setvalue(&tags.value[2], "messageID");
	ov_string_setvalue(&tags.value[3], "messageType");
	ov_string_setvalue(&tags.value[4], "protocolType");


	jsonGetValuesByTags(tags, jsonData_header, 1, &values);
	json_data_deleteMembers(&jsonData_header);

	ov_string_setvalue(&requestData->header.endpointSender, values.value[0]);
	ov_string_setvalue(&requestData->header.endpointReceiver, values.value[1]);
	ov_string_setvalue(&requestData->header.messageID, values.value[2]);
	requestData->header.messageType = atoi(values.value[3]);
	requestData->header.protocolType = atoi(values.value[4]);

	// Parse Body
	tmpString = malloc(jsonData.token[tokenIndex.value[1]+1].end + 2 - jsonData.token[tokenIndex.value[1]].start);
	strncpy(tmpString, &jsonData.js[jsonData.token[tokenIndex.value[1]].start - 1], jsonData.token[tokenIndex.value[1]+1].end + 1 - jsonData.token[tokenIndex.value[1]].start);
	tmpString[jsonData.token[tokenIndex.value[1]+1].end - jsonData.token[tokenIndex.value[1]].start + 1] = '\0';
	ov_string_setvalue(&requestData->body.js, tmpString);
	free(tmpString);
	tmpString = NULL;
	requestData->body.num_token = tokenSizeBody;
	requestData->body.token = malloc(sizeof(jsmntok_t)*requestData->body.num_token);
	for (OV_UINT i = 0; i < requestData->body.num_token; i++){
		memcpy(&requestData->body.token[i], &jsonData.token[tokenIndex.value[1]+i], sizeof(jsmntok_t));
		requestData->body.token[i].start = requestData->body.token[i].start - jsonData.token[tokenIndex.value[1]].start + 1;
		requestData->body.token[i].end = requestData->body.token[i].end - jsonData.token[tokenIndex.value[1]].start + 1;
		requestData->body.token[i].parent = requestData->body.token[i].parent - tokenIndex.value[1];
	}

	free(jsonData.token);
	jsonData.token = NULL;

	Ov_SetDynamicVectorLength(&tags, 0 , STRING);
	Ov_SetDynamicVectorLength(&values, 0 , STRING);
	Ov_SetDynamicVectorLength(&tokenIndex, 0 , UINT);

	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT jsonResponseParse(response_data* responseData, const OV_STRING message) {
	json_data jsonData;
	json_data_init(&jsonData);
	jsonData.js = message;

	if(jsonTokenize(&jsonData) != OV_ERR_OK) {
		return OV_ERR_BADPARAM;
	}

	// Finding Header and Body Token
	OV_STRING_VEC tags;
	tags.value = NULL;
	tags.veclen = 0;
	Ov_SetDynamicVectorLength(&tags, 2, STRING);
	ov_string_setvalue(&tags.value[0], "header");
	ov_string_setvalue(&tags.value[1], "body");
	OV_UINT_VEC tokenIndex;
	tokenIndex.value = NULL;
	tokenIndex.veclen = 0;
	Ov_SetDynamicVectorLength(&tokenIndex, 2, UINT);

	jsonGetTokenIndexByTags(tags, jsonData, 0, &tokenIndex);


	// get tokenSize for header and body
	OV_UINT tokenSizeHeader = 0;
	OV_BOOL countHeaderToken = FALSE;
	OV_UINT tokenSizeBody = 0;
	OV_BOOL countBodyToken = FALSE;
	for (OV_UINT i = 0; i < jsonData.num_token; i++){
		if (i == tokenIndex.value[0]){ // start counting Header tokens
			countHeaderToken = TRUE;
		}
		if (countHeaderToken == TRUE){
			if (jsonData.token[i].parent != 0 || tokenSizeHeader == 0)
				tokenSizeHeader++;
			else
				countHeaderToken = FALSE;
		}
		if (i == tokenIndex.value[1]){ // start counting Body tokens
			countBodyToken = TRUE;
		}
		if (countBodyToken == TRUE){
			if (jsonData.token[i].parent != 0 || tokenSizeBody == 0)
				tokenSizeBody++;
			else
				countBodyToken = FALSE;
		}
	}
	// Parse Header
	json_data jsonData_header;
	json_data_init(&jsonData_header);
	char* tmpString = malloc(jsonData.token[tokenIndex.value[0]+1].end + 2 - jsonData.token[tokenIndex.value[0]].start);
	strncpy(tmpString, &jsonData.js[jsonData.token[tokenIndex.value[0]].start-1], jsonData.token[tokenIndex.value[0]+1].end + 1 - jsonData.token[tokenIndex.value[0]].start);
	tmpString[jsonData.token[tokenIndex.value[0]+1].end - jsonData.token[tokenIndex.value[0]].start + 1] = '\0';
	ov_string_setvalue(&jsonData_header.js, tmpString);
	free(tmpString);
	tmpString = NULL;
	jsonData_header.num_token = tokenSizeHeader;
	jsonData_header.token = malloc(sizeof(jsmntok_t)*jsonData_header.num_token);
	for (OV_UINT i = 0; i < jsonData_header.num_token; i++){
		memcpy(&jsonData_header.token[i], &jsonData.token[tokenIndex.value[0]+i], sizeof(jsmntok_t));
		jsonData_header.token[i].start = jsonData_header.token[i].start - jsonData.token[tokenIndex.value[0]].start + 1;
		jsonData_header.token[i].end = jsonData_header.token[i].end - jsonData.token[tokenIndex.value[0]].start + 1;
		jsonData_header.token[i].parent = jsonData_header.token[i].parent - tokenIndex.value[0];
	}

	Ov_SetDynamicVectorLength(&tags, 7, STRING);
	OV_STRING_VEC values;
	values.value = NULL;
	values.veclen = 0;
	Ov_SetDynamicVectorLength(&values, 7, STRING);

	ov_string_setvalue(&tags.value[0], "endpointSender");
	ov_string_setvalue(&tags.value[1], "endpointReceiver");
	ov_string_setvalue(&tags.value[2], "messageID");
	ov_string_setvalue(&tags.value[3], "referToMessageID");
	ov_string_setvalue(&tags.value[4], "messageType");
	ov_string_setvalue(&tags.value[5], "errorFlag");
	ov_string_setvalue(&tags.value[6], "errorMessage");

	jsonGetValuesByTags(tags, jsonData_header, 1, &values);
	json_data_deleteMembers(&jsonData_header);

	if (values.value[0])
		ov_string_setvalue(&responseData->header.endpointSender, values.value[0]);
	if (values.value[1])
		ov_string_setvalue(&responseData->header.endpointReceiver, values.value[1]);
	if (values.value[2])
		ov_string_setvalue(&responseData->header.messageID, values.value[2]);
	if (values.value[3])
		ov_string_setvalue(&responseData->header.referToMessageID, values.value[3]);
	if (values.value[4])
		responseData->header.messageType = atoi(values.value[4]);
	if (values.value[5]){
		if (ov_string_compare(values.value[5], "true") == OV_STRCMP_EQUAL)
			responseData->header.errorFlag = TRUE;
		else
			responseData->header.errorFlag = FALSE;
	}
	if (values.value[6])
		ov_string_setvalue(&responseData->header.errorMessage, values.value[6]);

	// Parse Body
	tmpString = malloc(jsonData.token[tokenIndex.value[1]+1].end + 2 - jsonData.token[tokenIndex.value[1]].start);
	strncpy(tmpString, &jsonData.js[jsonData.token[tokenIndex.value[1]].start-1], jsonData.token[tokenIndex.value[1]+1].end + 1 - jsonData.token[tokenIndex.value[1]].start);
	tmpString[jsonData.token[tokenIndex.value[1]+1].end - jsonData.token[tokenIndex.value[1]].start + 1] = '\0';
	ov_string_setvalue(&responseData->body.js, tmpString);
	free(tmpString);
	tmpString = NULL;
	responseData->body.num_token = tokenSizeBody;
	responseData->body.token = malloc(sizeof(jsmntok_t)*responseData->body.num_token);
	for (OV_UINT i = 0; i < responseData->body.num_token; i++){
		memcpy(&responseData->body.token[i], &jsonData.token[tokenIndex.value[1]+i], sizeof(jsmntok_t));
		responseData->body.token[i].start = responseData->body.token[i].start - jsonData.token[tokenIndex.value[1]].start + 1;
		responseData->body.token[i].end = responseData->body.token[i].end - jsonData.token[tokenIndex.value[1]].start + 1;
		responseData->body.token[i].parent = responseData->body.token[i].parent - tokenIndex.value[1];
	}
	free(jsonData.token);
	jsonData.token = NULL;
	Ov_SetDynamicVectorLength(&tags, 0 , STRING);
	Ov_SetDynamicVectorLength(&values, 0 , STRING);
	Ov_SetDynamicVectorLength(&tokenIndex, 0 , UINT);

	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT jsonGetTokenIndexByTags(const OV_STRING_VEC tags, const json_data jsonData, const OV_UINT parentToken, OV_UINT_VEC* tokenIndex) {
	for(int i = parentToken; i < jsonData.num_token; i++) {
		for(int j = 0; j < tags.veclen; j++) {
			if (jsonData.token[i].parent != parentToken){
				break;
			}
			if(jsoneq(jsonData.js, &jsonData.token[i], tags.value[j])==0){
				tokenIndex->value[j] = i;
				break;
			}
		}
	}
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT jsonGetValuesByTags(const OV_STRING_VEC tags, const json_data jsonData, const OV_UINT parentToken, OV_STRING_VEC* values) {
	for(int i = parentToken; i < jsonData.num_token; i++) {
		for(int j = 0; j < tags.veclen; j++) {
			if (jsonData.token[i].parent != parentToken){
				break;
			}
			if(jsoneq(jsonData.js, &jsonData.token[i], tags.value[j])==0){
				jsonGetValueByToken(jsonData.js, &jsonData.token[i+1], &values->value[j]);
				break;
			}
		}
	}
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT jsonGetValuesAndTokenIndexByTags(const OV_STRING_VEC tags, const json_data jsonData, const OV_UINT parentToken, OV_STRING_VEC* values, OV_UINT_VEC* tokenIndex) {
	for(int i = parentToken; i < jsonData.num_token; i++) {
		for(int j = 0; j < tags.veclen; j++) {
			if (jsonData.token[i].parent != parentToken){
				break;
			}
			if(jsoneq(jsonData.js, &jsonData.token[i], tags.value[j])==0){
				jsonGetValueByToken(jsonData.js, &jsonData.token[i+1], &values->value[j]);
				tokenIndex->value[j] = i;
				break;
			}
		}
	}
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT jsonGetValueByToken(const char* js, const jsmntok_t* t, OV_STRING* str) {
	if(!(t->type==JSMN_STRING) || !str)
		return OV_ERR_BADPARAM;

	const char* jsStr = js+t->start;

	int len = t->end-t->start;
	int offset = 0;

	if (len == 0)
		return OV_ERR_OK;

	char* pString = Ov_HeapMalloc(len+1);
	if(!pString)
		return OV_ERR_DBOUTOFMEMORY;

	for(int pos = 0;pos < len; pos++){
		if(jsStr[pos] == '\\'){
			pos++;
			offset++;
			switch(jsStr[pos]){
			// replace escape sequences
			case 'b':
				pString[pos-offset] = '\b';
				break;
			case 'f':
				pString[pos-offset] = '\f';
				break;
			case 'n':
				pString[pos-offset] = '\n';
				break;
			case 'r':
				pString[pos-offset] = '\r';
				break;
			case 't':
				pString[pos-offset] = '\t';
				break;
			case '\"':
				pString[pos-offset] = '\"';
				break;
			case '\\':
				pString[pos-offset] = '\\';
				break;
			default:
				return OV_ERR_BADPARAM;
			}
		} else {
			pString[pos-offset] = jsStr[pos];
		}
	}

	OV_UINT strlen = len - offset;
	pString[strlen] = '\0';

	ov_string_setvalue(str, pString);

	Ov_HeapFree(pString);

	return OV_ERR_OK;
}


/**
int scmp(char *s1, char *s2) {
	while(*s1 != '\0' && *s1 == *s2) { s1++; s2++; }
	return (*s1 - *s2);
}

jsmntok_t* findTag(const char* js, jsmntok_t *tokens, const char* tag) {

jsmntok_t* findTag(const char* js, jsmntok_t *tokens, const char* tag) {
	for(int i=0; i < sizeof(tokens)/sizeof(jsmntok_t); i++) {
		char* tokStr = (char *)malloc(tokens[i].size);

		strncpy(tokStr, &js[tokens[i].start], (size_t)tokens[i].size);

		if(!scmp(tokStr, (char*)tag)) {
			return &tokens[i];
		}
		frer
	}
	return NULL;
}


char* getTagValue(const char* js, jsmntok_t tok, const char* tag) {
	return (char *)NULL;
}

**/
