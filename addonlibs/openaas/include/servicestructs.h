/*
 * servicestructs.h
 *
 *  Created on: Jan 6, 2017
 *      Author: lars
 */

#ifndef SERVICESTRUCTS_H_
#define SERVICESTRUCTS_H_

//#include "ua_types.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define JSON_SEC_TO_DATETIME 10000000LL
#define JSON_DATETIME_UNIXEPOCH (11644473600LL * JSON_SEC_TO_DATETIME)


// types for sub-structures

typedef int status_t;
typedef uint64_t SRV_DateTime;

typedef struct {
	size_t length;
	char* data;
} SRV_String;

typedef enum {
	SRV_JSON,
	SRV_OPCB,
	SRV_XMLT,
	SRV_XMLB
} SRV_encoding_t;

typedef enum {
	SRV_VT_undefined,
	SRV_VT_NULL,
	SRV_VT_BOOL, 	// bool
	SRV_VT_SINGLE, 	// float
	SRV_VT_DOUBLE,	// double
	SRV_VT_INT32,	// int32
	SRV_VT_INT64,	// int64
	SRV_VT_UINT32,	// uint32
	SRV_VT_UINT64,	// uint64
	SRV_VT_STRING,	// SRV_String
	SRV_VT_DATETIME // uint64
} SRV_valType_t;

typedef enum {
	SRV_undefined,
	SRV_createAASReq,
	SRV_createAASRsp,
	SRV_deleteAASReq,
	SRV_deleteAASRsp,
	SRV_createPVSLReq,
	SRV_createPVSLRsp,
	SRV_deletePVSLReq,
	SRV_deletePVSLRsp,
	SRV_createPVSReq,
	SRV_createPVSRsp,
	SRV_deletePVSReq,
	SRV_deletePVSRsp,
	SRV_createLCEReq,
	SRV_createLCERsp,
	SRV_deleteLCEReq,
	SRV_deleteLCERsp,
	SRV_getPVSReq,
	SRV_getPVSRsp,
	SRV_setPVSReq,
	SRV_setPVSRsp,
	SRV_getLCEReq,
	SRV_getLCERsp,
	SRV_setLCEReq,
	SRV_setLCERsp,
	SRV_getCoreDataReq,
	SRV_getCoreDataRsp
} SRV_service_t;

typedef enum {
	SRV_ES_confirmation	= 0,
	SRV_ES_setting		= 1,
	SRV_ES_measurement	= 2,
	SRV_ES_requirement	= 3,
	SRV_ES_undefined	= 4
} SRV_exprSemEnum_t;

typedef enum {
	SRV_EL_GT	= 0,
	SRV_EL_GE	= 1,
	SRV_EL_EQ	= 2,
	SRV_EL_NE	= 3,
	SRV_EL_LE	= 4,
	SRV_EL_LT	= 5,
	SRV_EL_undefined = 6,
} SRV_exprLogEnum_t;

typedef enum {
	SRV_VIEW_BUSINESS,
	SRV_VIEW_CONSTRUCTION,
	SRV_VIEW_POWER,
	SRV_VIEW_FUNCTIONAL,
	SRV_VIEW_LOCATION,
	SRV_VIEW_SECURITY,
	SRV_VIEW_NETWORK,
	SRV_VIEW_LIFECYCLE,
	SRV_VIEW_HUMAN,
	SRV_VIEW_undefined
} SRV_viewEnum_t;

typedef enum {
	SRV_VIS_PRIVATE,
	SRV_VIS_CONTRACT,
	SRV_VIS_PUBLIC,
	SRV_VIS_undefined
} SRV_visibilityEnum_t;

typedef enum {
	SRV_IDT_URI	= 0,
	SRV_IDT_ISO	= 1,
	SRV_IDT_undefined 	= 2
} SRV_idEnum_t;

typedef struct {
	SRV_idEnum_t idType;
	SRV_String idSpec;
} SRV_ident_t;

typedef struct {
	SRV_exprSemEnum_t expressionSemantic;
	SRV_exprLogEnum_t expressionLogic;
	SRV_String name;
	SRV_String unit;
	SRV_viewEnum_t view;
	SRV_visibilityEnum_t visibility;
	SRV_ident_t ID;
	SRV_DateTime valTime;
	SRV_valType_t valType;
	void* value;
	bool hasName;
	bool hasUnit;
	bool hasValTime;
} PVS_t;

typedef struct {
	SRV_String name;
	SRV_ident_t carrier;
	SRV_ident_t creatingInstance;
	SRV_DateTime creationTime;
	bool hasName;
	bool hasCreationTime;
	PVS_t* pvs;
	uint32_t numPvs;
} PVSL_t;

typedef struct {
	uint64_t lceId;
	SRV_ident_t creatingInstance;
	SRV_ident_t writingInstance;
	SRV_String eventClass;
	SRV_String subject;
	SRV_DateTime dataTime;
	SRV_valType_t dataType;
	void* data;
	bool hasLceId;
	bool hasEventClass;
	bool hasSubject;
	bool hastDataTime;
} LCE_t;

// structs for requests and responses

typedef struct {
	int msgNo;
	SRV_ident_t sender;
	SRV_ident_t receiver;
} SRV_msgHeader;

typedef struct {
	SRV_ident_t aasId;
	SRV_String aasName;
	SRV_ident_t assetId;
} createAASReq_t;

typedef struct {
	status_t status;
} createAASRsp_t;


typedef struct {
	SRV_ident_t aasId;
} deleteAASReq_t;

typedef struct {
	status_t status;
} deleteAASRsp_t;


typedef struct {
	//SRV_ident_t aasId;
	SRV_String pvslName;
	SRV_ident_t carrier;
} createPVSLReq_t;

typedef struct {
	status_t status;
} createPVSLRsp_t;


typedef struct {
	//SRV_ident_t aasId;
	SRV_String pvslName;
} deletePVSLReq_t;

typedef struct {
	status_t status;
} deletePVSLRsp_t;


typedef struct {
	//SRV_ident_t aasId;
	SRV_String pvslName;
	PVS_t pvs;
} createPVSReq_t;

typedef struct {
	status_t status;
} createPVSRsp_t;


typedef struct {
	//SRV_ident_t aasId;
	SRV_String pvslName;
	SRV_String pvsName;
} deletePVSReq_t;

typedef struct {
	status_t status;
} deletePVSRsp_t;


typedef struct {
	//SRV_ident_t aasId;
	LCE_t lce;
} createLCEReq_t;

typedef struct {
	status_t status;
} createLCERsp_t;

typedef struct {
	//SRV_ident_t aasId;
	int lceId;
} deleteLCEReq_t;

typedef struct {
	status_t status;
} deleteLCERsp_t;


typedef struct {
	//SRV_ident_t aasId;
	SRV_String pvslName;
	SRV_String pvsName;
} getPVSReq_t;

typedef struct {
	status_t status;
	PVS_t pvs;
} getPVSRsp_t;

typedef struct {
	//SRV_ident_t aasId;
	SRV_String pvslName;
	//SRV_String pvsName;
	PVS_t pvs;
} setPVSReq_t;

typedef struct {
	status_t status;
} setPVSRsp_t;


typedef struct {
	//SRV_ident_t aasId;
	uint64_t lceId;
} getLCEReq_t;

typedef struct {
	status_t status;
	LCE_t lce;
} getLCERsp_t;

typedef struct {
	//SRV_ident_t aasId;
	//uint64_t lceId;
	LCE_t lce;
} setLCEReq_t;

typedef struct {
	status_t status;
} setLCERsp_t;

typedef struct {
	uint8_t dummy;
} getCoreDataReq_t;

typedef struct {
	status_t status;
	PVSL_t* pvsl;
	uint32_t numPvsl;
} getCoreDataRsp_t;

SRV_String* SRV_String_new();
SRV_String* SRV_String_copy(SRV_String* to, const SRV_String* from);
void SRV_String_init(SRV_String* this);
void SRV_String_setCopy(SRV_String* srvstr, const char* str, int len);
void SRV_String_deleteMembers(SRV_String* this);
void SRV_String_delete(SRV_String* this);

SRV_ident_t* SRV_ident_t_new();
PVSL_t* PVSL_t_new();
PVS_t* PVS_t_new();
LCE_t* LCE_t_new();

SRV_msgHeader* SRV_msgHeader_t_new();
SRV_msgHeader* SRV_msgHeader_t_copy(SRV_msgHeader* header);
SRV_msgHeader* SRV_msgHeader_t_reverseCopy(SRV_msgHeader* header);


void* SRV_serviceGeneric_new(SRV_service_t type);

createAASReq_t* createAASReq_t_new();
createAASRsp_t* createAASRsp_t_new();
deleteAASReq_t* deleteAASReq_t_new();
deleteAASRsp_t* deleteAASRsp_t_new();
createPVSLReq_t* createPVSLReq_t_new();
createPVSLRsp_t* createPVSLRsp_t_new();
deletePVSLReq_t* deletePVSLReq_t_new();
deletePVSLRsp_t* deletePVSLRsp_t_new();
createPVSReq_t* createPVSReq_t_new();
createPVSRsp_t* createPVSRsp_t_new();
deletePVSReq_t* deletePVSReq_t_new();
deletePVSRsp_t* deletePVSRsp_t_new();
createLCEReq_t* createLCEReq_t_new();
createLCERsp_t* createLCERsp_t_new();
deleteLCEReq_t* deleteLCEReq_t_new();
deleteLCERsp_t* deleteLCERsp_t_new();
getPVSReq_t* getPVSReq_t_new();
getPVSRsp_t* getPVSRsp_t_new();
setPVSReq_t* setPVSReq_t_new();
setPVSRsp_t* setPVSRsp_t_new();
getLCEReq_t* getLCEReq_t_new();
getLCERsp_t* getLCERsp_t_new();
setLCEReq_t* setLCEReq_t_new();
setLCERsp_t* setLCERsp_t_new();
getCoreDataReq_t* getCoreDataReq_t_new();
getCoreDataRsp_t* getCoreDataRsp_t_new();

void SRV_ident_t_init (SRV_ident_t* this);
void PVSL_t_init (PVSL_t* this);
void PVS_t_init (PVS_t* this);
void LCE_t_init (LCE_t* this);

void SRV_msgHeader_t_init (SRV_msgHeader* this);

void SRV_serviceGeneric_init(void* this, SRV_service_t type);

void createAASReq_t_init (createAASReq_t* this);
void createAASRsp_t_init (createAASRsp_t* this);
void deleteAASReq_t_init (deleteAASReq_t* this);
void deleteAASRsp_t_init (deleteAASRsp_t* this);
void createPVSLReq_t_init (createPVSLReq_t* this);
void createPVSLRsp_t_init (createPVSLRsp_t* this);
void deletePVSLReq_t_init (deletePVSLReq_t* this);
void deletePVSLRsp_t_init (deletePVSLRsp_t* this);
void createPVSReq_t_init (createPVSReq_t* this);
void createPVSRsp_t_init (createPVSRsp_t* this);
void deletePVSReq_t_init (deletePVSReq_t* this);
void deletePVSRsp_t_init (deletePVSRsp_t* this);
void createLCEReq_t_init (createLCEReq_t* this);
void createLCERsp_t_init (createLCERsp_t* this);
void deleteLCEReq_t_init (deleteLCEReq_t* this);
void deleteLCERsp_t_init (deleteLCERsp_t* this);
void getPVSReq_t_init (getPVSReq_t* this);
void getPVSRsp_t_init (getPVSRsp_t* this);
void setPVSReq_t_init (setPVSReq_t* this);
void setPVSRsp_t_init (setPVSRsp_t* this);
void getLCEReq_t_init (getLCEReq_t* this);
void getLCERsp_t_init (getLCERsp_t* this);
void setLCEReq_t_init (setLCEReq_t* this);
void setLCERsp_t_init (setLCERsp_t* this);
void getCoreDataReq_t_init(getCoreDataReq_t* this);
void getCoreDataRsp_t_init(getCoreDataRsp_t* this);

void SRV_ident_t_deleteMembers (SRV_ident_t* this);
void PVSL_t_deleteMembers (PVSL_t* this);
void PVS_t_deleteMembers (PVS_t* this);
void LCE_t_deleteMembers (LCE_t* this);

void SRV_msgHeader_t_deleteMembers (SRV_msgHeader* this);

void SRV_serviceGeneric_deleteMembers(void* this, SRV_service_t type);

void createAASReq_t_deleteMembers (createAASReq_t* this);
void createAASRsp_t_deleteMembers (createAASRsp_t* this);
void deleteAASReq_t_deleteMembers (deleteAASReq_t* this);
void deleteAASRsp_t_deleteMembers (deleteAASRsp_t* this);
void createPVSLReq_t_deleteMembers (createPVSLReq_t* this);
void createPVSLRsp_t_deleteMembers (createPVSLRsp_t* this);
void deletePVSLReq_t_deleteMembers (deletePVSLReq_t* this);
void deletePVSLRsp_t_deleteMembers (deletePVSLRsp_t* this);
void createPVSReq_t_deleteMembers (createPVSReq_t* this);
void createPVSRsp_t_deleteMembers (createPVSRsp_t* this);
void deletePVSReq_t_deleteMembers (deletePVSReq_t* this);
void deletePVSRsp_t_deleteMembers (deletePVSRsp_t* this);
void createLCEReq_t_deleteMembers (createLCEReq_t* this);
void createLCERsp_t_deleteMembers (createLCERsp_t* this);
void deleteLCEReq_t_deleteMembers (deleteLCEReq_t* this);
void deleteLCERsp_t_deleteMembers (deleteLCERsp_t* this);
void getPVSReq_t_deleteMembers (getPVSReq_t* this);
void getPVSRsp_t_deleteMembers (getPVSRsp_t* this);
void setPVSReq_t_deleteMembers (setPVSReq_t* this);
void setPVSRsp_t_deleteMembers (setPVSRsp_t* this);
void getLCEReq_t_deleteMembers (getLCEReq_t* this);
void getLCERsp_t_deleteMembers (getLCERsp_t* this);
void setLCEReq_t_deleteMembers (setLCEReq_t* this);
void setLCERsp_t_deleteMembers (setLCERsp_t* this);
void getCoreDataReq_t_deleteMembers(getCoreDataReq_t* this);
void getCoreDataRsp_t_deleteMembers(getCoreDataRsp_t* this);

void SRV_ident_t_delete (SRV_ident_t* this);
void PVSL_t_delete (PVSL_t* this);
void PVS_t_delete (PVS_t* this);
void LCE_t_delete (LCE_t* this);

void SRV_msgHeader_t_delete (SRV_msgHeader* this);

void SRV_serviceGeneric_delete (void* this, SRV_service_t type);

void createAASReq_t_delete (createAASReq_t* this);
void createAASRsp_t_delete (createAASRsp_t* this);
void deleteAASReq_t_delete (deleteAASReq_t* this);
void deleteAASRsp_t_delete (deleteAASRsp_t* this);
void createPVSLReq_t_delete (createPVSLReq_t* this);
void createPVSLRsp_t_delete (createPVSLRsp_t* this);
void deletePVSLReq_t_delete (deletePVSLReq_t* this);
void deletePVSLRsp_t_delete (deletePVSLRsp_t* this);
void createPVSReq_t_delete (createPVSReq_t* this);
void createPVSRsp_t_delete (createPVSRsp_t* this);
void deletePVSReq_t_delete (deletePVSReq_t* this);
void deletePVSRsp_t_delete (deletePVSRsp_t* this);
void createLCEReq_t_delete (createLCEReq_t* this);
void createLCERsp_t_delete (createLCERsp_t* this);
void deleteLCEReq_t_delete (deleteLCEReq_t* this);
void deleteLCERsp_t_delete (deleteLCERsp_t* this);
void getPVSReq_t_delete (getPVSReq_t* this);
void getPVSRsp_t_delete (getPVSRsp_t* this);
void setPVSReq_t_delete (setPVSReq_t* this);
void setPVSRsp_t_delete (setPVSRsp_t* this);
void getLCEReq_t_delete (getLCEReq_t* this);
void getLCERsp_t_delete (getLCERsp_t* this);
void setLCEReq_t_delete (setLCEReq_t* this);
void setLCERsp_t_delete (setLCERsp_t* this);
void getCoreDataReq_t_delete(getCoreDataReq_t* this);
void getCoreDataRsp_t_delete(getCoreDataRsp_t* this);

#endif /* SERVICESTRUCTS_H_ */
