#ifndef __JSON_H_
#define __JSON_H_

#include "jsmn.h"
#include "libov/ov_macros.h"

typedef struct json_data{
    jsmntok_t* token;
    OV_UINT     num_token;
    OV_STRING  js;
} json_data;

typedef struct request_header {
    OV_STRING   endpointSender;
    OV_STRING   endpointReceiver;
    OV_STRING   messageID;
    OV_UINT     messageType;
    OV_UINT     protocolType;
}request_header;

typedef struct response_header {
    OV_STRING   endpointSender;
    OV_STRING   endpointReceiver;
    OV_STRING   messageID;
    OV_STRING   referToMessageID;
    OV_UINT     messageType;
    OV_BOOL     errorFlag;
    OV_STRING   errorMessage;
}response_header;


typedef struct request_data {
    request_header header;
    json_data   body;
}request_data;

typedef struct response_data {
    response_header header;
    json_data   body;
}response_data;


void json_data_init(json_data* jsonData);
void json_data_deleteMembers(json_data* jsonData);
void request_data_init(request_data *requestData);
void request_data_deleteMembers(request_data *requestData);
void response_data_init(response_data *responseData);
void response_data_deleteMembers(response_data *responseData);
int jsoneq(const char *json, const jsmntok_t *tok, const char *s);
OV_RESULT jsonTokenize(json_data* jsonData);
OV_RESULT jsonRequestParse(request_data* requestData, const OV_STRING message);
OV_RESULT jsonResponseParse(response_data* responseData, const OV_STRING message);
OV_RESULT jsonGetTokenIndexByTags(const OV_STRING_VEC tags, const json_data jsonData, const OV_UINT parentToken, OV_UINT_VEC* tokenIndex);
OV_RESULT jsonGetValuesByTags(const OV_STRING_VEC tags, const json_data jsonData, const OV_UINT parentToken, OV_STRING_VEC* values);
OV_RESULT jsonGetValuesAndTokenIndexByTags(const OV_STRING_VEC tags, const json_data jsonData, const OV_UINT parentToken, OV_STRING_VEC* values, OV_UINT_VEC* tokenIndex);
OV_RESULT jsonGetValueByToken(const char* js, const jsmntok_t* t, OV_STRING* str);
#endif
