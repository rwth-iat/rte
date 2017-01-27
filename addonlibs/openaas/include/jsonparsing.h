/*
 * jsonparsing.h
 *
 *  Created on: Dec 8, 2016
 *      Author: lars
 */

#ifndef JSONPARSING_H_
#define JSONPARSING_H_

#define JSMN_STRICT
#define JSMN_PARENT_LINKS

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "jsmn.h"
#include "servicestructs.h"
//#include "ua_constants.h"

typedef int JSON_RC;

#define JSON_RC_OK 0;
#define JSON_RC_WRONG_TYPE -1
#define JSON_RC_MISSING_FIELD -2
#define JSON_RC_WRONG_VALUE -3
#define JSON_RC_NO_SPACE -4
#define JSON_RC_BAD_INPUT -5
#define JSON_RC_UNDEF -999
//typedef SRV_String JSON_String;

// helper functions
//static int jsoneq(const char *json, jsmntok_t *tok, const char *s);
JSON_RC tok2SrvStr(const char* js, const jsmntok_t* t, SRV_String* str);
JSON_RC tok2Prim(const char* js, const jsmntok_t* t, SRV_valType_t* type, void* value);
JSON_RC tok2Int(const char* js, const jsmntok_t* t, void* value, int force32);
JSON_RC tok2Double(const char* js, const jsmntok_t* t, double* value);
JSON_RC tok2Bool(const char* js, const jsmntok_t* t, uint8_t* value);

int json_append(SRV_String* js, int start, const char* str, int len, int maxLen);
int json_appendKey(SRV_String* js, int start, const char* str, int len, int maxLen, int* first);
int json_appendString(SRV_String* js, int start, const char* str, int len, int maxLen);
int json_appendSrvStr(SRV_String* js, int start, const SRV_String* str, int maxLen);
int json_appendAny(SRV_String* js, int start, const void* value, SRV_valType_t type, int maxLen);
int dateTimeToISO(SRV_DateTime t, char* iso, int maxLen);
JSON_RC ISOToDateTime(char* iso, int len, SRV_DateTime* jsonTime);
int json_appendIsoTime(SRV_String* js, int start, int maxLen, SRV_DateTime t);

// main parsing function
JSON_RC parseJson (const SRV_String* str, SRV_msgHeader** header, void** srvStruct, SRV_service_t* srvType);
JSON_RC genJson (SRV_String** js, const SRV_msgHeader* header, const void* srvStruct, SRV_service_t srvType);

// parsing functions

JSON_RC jsonparsePVSL(const jsmntok_t* t, const SRV_String* str, int start, int n, PVSL_t* pvsl, int* last);
JSON_RC jsonparsePVS(const jsmntok_t* t, const SRV_String* str, int start, int n, PVS_t* pvs, int* last);
JSON_RC jsonparseLCE(const jsmntok_t* t, const SRV_String* str, int start, int n, LCE_t* lce, int* last);
JSON_RC jsonparseId(const jsmntok_t* t, const SRV_String* str, int start, int n, SRV_ident_t* id, int* last);

JSON_RC jsonparseCreateAASReq(createAASReq_t* cAasReq, const jsmntok_t* t, const SRV_String* js, int n, int start);
JSON_RC jsonparseCreateAASRsp(createAASRsp_t* cAasRsp, const jsmntok_t* t, const SRV_String* js, int n, int start);
JSON_RC jsonparseDeleteAASReq(deleteAASReq_t* dAasReq, const jsmntok_t* t, const SRV_String* js, int n, int start);
JSON_RC jsonparseDeleteAASRsp(deleteAASRsp_t* dAasRsp, const jsmntok_t* t, const SRV_String* js, int n, int start);
JSON_RC jsonparseCreatePVSLReq(createPVSLReq_t* cPvslReq, const jsmntok_t* t, const SRV_String* js, int n, int start);
JSON_RC jsonparseCreatePVSLRsp(createPVSLRsp_t* cPvslRsp, const jsmntok_t* t, const SRV_String* js, int n, int start);
JSON_RC jsonparseDeletePVSLReq(deletePVSLReq_t* dPvslReq, const jsmntok_t* t, const SRV_String* js, int n, int start);
JSON_RC jsonparseDeletePVSLRsp(deletePVSLRsp_t* dPvslRsp, const jsmntok_t* t, const SRV_String* js, int n, int start);
JSON_RC jsonparseCreatePVSReq(createPVSReq_t* cPvsReq, const jsmntok_t* t, const SRV_String* js, int n, int start);
JSON_RC jsonparseCreatePVSRsp(createPVSRsp_t* cPvsRsp, const jsmntok_t* t, const SRV_String* js, int n, int start);
JSON_RC jsonparseDeletePVSReq(deletePVSReq_t* dPvsReq, const jsmntok_t* t, const SRV_String* js, int n, int start);
JSON_RC jsonparseDeletePVSRsp(deletePVSRsp_t* dPvsRsp, const jsmntok_t* t, const SRV_String* js, int n, int start);
JSON_RC jsonparseCreateLCEReq(createLCEReq_t* cLceReq, const jsmntok_t* t, const SRV_String* js, int n, int start);
JSON_RC jsonparseCreateLCERsp(createLCERsp_t* cLceRsp, const jsmntok_t* t, const SRV_String* js, int n, int start);
JSON_RC jsonparseDeleteLCEReq(deleteLCEReq_t* dLceReq, const jsmntok_t* t, const SRV_String* js, int n, int start);
JSON_RC jsonparseDeleteLCERsp(deleteLCERsp_t* dLceRsp, const jsmntok_t* t, const SRV_String* js, int n, int start);
JSON_RC jsonparseGetPVSReq(getPVSReq_t* gPvsReq, const jsmntok_t* t, const SRV_String* js, int n, int start);
JSON_RC jsonparseGetPVSRsp(getPVSRsp_t* gPvsRsp, const jsmntok_t* t, const SRV_String* js, int n, int start);
JSON_RC jsonparseSetPVSReq(setPVSReq_t* sPvsReq, const jsmntok_t* t, const SRV_String* js, int n, int start);
JSON_RC jsonparseSetPVSRsp(setPVSRsp_t* sPvsRsp, const jsmntok_t* t, const SRV_String* js, int n, int start);
JSON_RC jsonparseGetLCEReq(getLCEReq_t* gLceReq, const jsmntok_t* t, const SRV_String* js, int n, int start);
JSON_RC jsonparseGetLCERsp(getLCERsp_t* gLceRsp, const jsmntok_t* t, const SRV_String* js, int n, int start);
JSON_RC jsonparseSetLCEReq(setLCEReq_t* sLceReq, const jsmntok_t* t, const SRV_String* js, int n, int start);
JSON_RC jsonparseSetLCERsp(setLCERsp_t* sLceRsp, const jsmntok_t* t, const SRV_String* js, int n, int start);

// generating functions
JSON_RC jsonGenGeneric(SRV_String* json, int* length,const SRV_msgHeader* header, const void* srvStruct, SRV_service_t srvType);

JSON_RC jsonGenPVSL(SRV_String* json, int* length, int* ppos, const PVSL_t* p);
JSON_RC jsonGenPVS(SRV_String* json, int* length, int* ppos, const PVS_t* p);
JSON_RC jsonGenLCE(SRV_String* json, int* length, int* ppos, const LCE_t* l);
JSON_RC jsonGenId(SRV_String* json, int* length, int* ppos, const SRV_ident_t* i);

JSON_RC jsonGenCreateAASReq(SRV_String* json, int* length, const createAASReq_t* cAasReq);
JSON_RC jsonGenCreateAASRsp(SRV_String* json, int* length, const createAASRsp_t* cAasRsp);
JSON_RC jsonGenDeleteAASReq(SRV_String* json, int* length, const deleteAASReq_t* dAasReq);
JSON_RC jsonGenDeleteAASRsp(SRV_String* json, int* length, const deleteAASRsp_t* dAasRsp);
JSON_RC jsonGenCreatePVSLReq(SRV_String* json, int* length, const createPVSLReq_t* cPvslReq);
JSON_RC jsonGenCreatePVSLRsp(SRV_String* json, int* length, const createPVSLRsp_t* cPvslRsp);
JSON_RC jsonGenDeletePVSLReq(SRV_String* json, int* length, const deletePVSLReq_t* dPvslReq);
JSON_RC jsonGenDeletePVSLRsp(SRV_String* json, int* length, const deletePVSLRsp_t* dPvslRsp);
JSON_RC jsonGenCreatePVSReq(SRV_String* json, int* length, const createPVSReq_t* cPvsReq);
JSON_RC jsonGenCreatePVSRsp(SRV_String* json, int* length, const createPVSRsp_t* cPvsRsp);
JSON_RC jsonGenDeletePVSReq(SRV_String* json, int* length, const deletePVSReq_t* dPvsReq);
JSON_RC jsonGenDeletePVSRsp(SRV_String* json, int* length, const deletePVSRsp_t* dPvsRsp);
JSON_RC jsonGenCreateLCEReq(SRV_String* json, int* length, const createLCEReq_t* cLceReq);
JSON_RC jsonGenCreateLCERsp(SRV_String* json, int* length, const createLCERsp_t* cLceRsp);
JSON_RC jsonGenDeleteLCEReq(SRV_String* json, int* length, const deleteLCEReq_t* dLceReq);
JSON_RC jsonGenDeleteLCERsp(SRV_String* json, int* length, const deleteLCERsp_t* dLceRsp);
JSON_RC jsonGenGetPVSReq(SRV_String* json, int* length, const getPVSReq_t* gPvsReq);
JSON_RC jsonGenGetPVSRsp(SRV_String* json, int* length, const getPVSRsp_t* gPvsRsp);
JSON_RC jsonGenSetPVSReq(SRV_String* json, int* length, const setPVSReq_t* sPvsReq);
JSON_RC jsonGenSetPVSRsp(SRV_String* json, int* length, const setPVSRsp_t* sPvsRsp);
JSON_RC jsonGenGetLCEReq(SRV_String* json, int* length, const getLCEReq_t* gLceReq);
JSON_RC jsonGenGetLCERsp(SRV_String* json, int* length, const getLCERsp_t* gLceRsp);
JSON_RC jsonGenSetLCEReq(SRV_String* json, int* length, const setLCEReq_t* sLceReq);
JSON_RC jsonGenSetLCERsp(SRV_String* json, int* length, const setLCERsp_t* sLceRsp);

char* strptime(const char *s, const char *fmt, struct tm *tp);

#endif /* JSONPARSING_H_ */
