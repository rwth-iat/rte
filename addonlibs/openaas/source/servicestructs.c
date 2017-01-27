/*
 * servicestructs.c
 *
 *  Created on: Jan 6, 2017
 *      Author: lars
 */

#include "servicestructs.h"

const size_t SRV_VALUEMEMSIZE = sizeof(SRV_String)<sizeof(double)?sizeof(double):sizeof(SRV_String);

SRV_String* SRV_String_new(){
	SRV_String* this = malloc(sizeof(SRV_String));
	SRV_String_init(this);
	return this;
}

SRV_String* SRV_String_copy(SRV_String* to, const SRV_String* from){
	// allocate memory if no destination was given
	if(!to){
		to = malloc(sizeof(SRV_String));
		if(!to)
			return NULL;
	}

	to->length = from->length;
	to->data = malloc(from->length+1);
	memcpy(to->data, from->data, from->length);
	to->data[to->length] = '\0';
	return to;
}

void SRV_String_init(SRV_String* this){
	this->data = NULL;
	this->length = 0;
}

void SRV_String_setCopy(SRV_String* srvstr, const char* str, int len){
	if(len<0)
		len = strlen(str);

	srvstr->data = malloc(len+1);
	srvstr->length = len;
	memcpy(srvstr->data, str, len);
	srvstr->data[len] = '\0';
}

void SRV_String_deleteMembers(SRV_String* this){
	if(this->data)
		free(this->data);
	this->data = NULL;
	this->length = 0;
}

void SRV_String_delete(SRV_String* this){
	free(this->data);
	free(this);
}

LCE_t* LCE_t_new(){
	LCE_t* this = malloc(sizeof(LCE_t));
	LCE_t_init(this);
	return this;
}

PVSL_t* PVSL_t_new(){
	PVSL_t* this = malloc(sizeof(PVSL_t));
	PVSL_t_init(this);
	return this;
}

PVS_t* PVS_t_new(){
	PVS_t* this = malloc(sizeof(PVS_t));
	PVS_t_init(this);
	return this;
}

SRV_ident_t* SRV_ident_t_new(){
	SRV_ident_t* this = malloc(sizeof(SRV_ident_t));
	SRV_ident_t_init(this);
	return this;
}

SRV_msgHeader* SRV_msgHeader_t_new(){
	SRV_msgHeader* this = malloc(sizeof(SRV_msgHeader));
	SRV_msgHeader_t_init(this);
	return this;
}

SRV_msgHeader* SRV_msgHeader_t_copy(SRV_msgHeader* header){
	SRV_msgHeader* this = SRV_msgHeader_t_new();
	memcpy(this, header, sizeof(SRV_msgHeader));

	SRV_String_copy(&this->sender.idSpec, &header->sender.idSpec);

	SRV_String_copy(&this->receiver.idSpec, &header->receiver.idSpec);

	return this;
}

SRV_msgHeader* SRV_msgHeader_t_reverseCopy(SRV_msgHeader* header){
	SRV_msgHeader* this = SRV_msgHeader_t_new();
	this->msgNo = header->msgNo;

	this->sender.idType = header->receiver.idType;
	SRV_String_copy(&this->sender.idSpec, &header->receiver.idSpec);

	this->receiver.idType = header->sender.idType;
	SRV_String_copy(&this->receiver.idSpec, &header->sender.idSpec);

	return this;
}

void* SRV_serviceGeneric_new(SRV_service_t type){
	switch (type) {
	case SRV_createAASReq:
		return createAASReq_t_new();
	case SRV_createAASRsp:
		return createAASRsp_t_new();
	case SRV_deleteAASReq:
		return deleteAASReq_t_new();
	case SRV_deleteAASRsp:
		return deleteAASRsp_t_new();
	case SRV_createPVSLReq:
		return createPVSLReq_t_new();
	case SRV_createPVSLRsp:
		return createPVSLRsp_t_new();
	case SRV_deletePVSLReq:
		return deletePVSLReq_t_new();
	case SRV_deletePVSLRsp:
		return deletePVSLRsp_t_new();
	case SRV_createPVSReq:
		return createPVSReq_t_new();
	case SRV_createPVSRsp:
		return createPVSRsp_t_new();
	case SRV_deletePVSReq:
		return deletePVSReq_t_new();
	case SRV_deletePVSRsp:
		return deletePVSRsp_t_new();
	case SRV_createLCEReq:
		return createLCEReq_t_new();
	case SRV_createLCERsp:
		return createLCERsp_t_new();
	case SRV_deleteLCEReq:
		return deleteLCEReq_t_new();
	case SRV_deleteLCERsp:
		return deleteLCERsp_t_new();
	case SRV_getPVSReq:
		return getPVSReq_t_new();
	case SRV_getPVSRsp:
		return getPVSRsp_t_new();
	case SRV_setPVSReq:
		return setPVSReq_t_new();
	case SRV_setPVSRsp:
		return setPVSRsp_t_new();
	case SRV_getLCEReq:
		return getLCEReq_t_new();
	case SRV_getLCERsp:
		return getLCERsp_t_new();
	case SRV_setLCEReq:
		return setLCEReq_t_new();
	case SRV_setLCERsp:
		return setLCERsp_t_new();
	case SRV_undefined:default:
		return NULL;
	}
}

createAASReq_t* createAASReq_t_new(){
	createAASReq_t* this = malloc(sizeof(createAASReq_t));
	createAASReq_t_init(this);
	return this;
}

createAASRsp_t* createAASRsp_t_new(){
	createAASRsp_t* this = malloc(sizeof(createAASRsp_t));
	createAASRsp_t_init(this);
	return this;
}

createLCEReq_t* createLCEReq_t_new(){
	createLCEReq_t* this = malloc(sizeof(createLCEReq_t));
	createLCEReq_t_init(this);
	return this;
}

createLCERsp_t* createLCERsp_t_new(){
	createLCERsp_t* this = malloc(sizeof(createLCERsp_t));
	createLCERsp_t_init(this);
	return this;
}

createPVSLReq_t* createPVSLReq_t_new(){
	createPVSLReq_t* this = malloc(sizeof(createPVSLReq_t));
	createPVSLReq_t_init(this);
	return this;
}

createPVSLRsp_t* createPVSLRsp_t_new(){
	createPVSLRsp_t* this = malloc(sizeof(createPVSLRsp_t));
	createPVSLRsp_t_init(this);
	return this;
}

createPVSReq_t* createPVSReq_t_new(){
	createPVSReq_t* this = malloc(sizeof(createPVSReq_t));
	createPVSReq_t_init(this);
	return this;
}

createPVSRsp_t* createPVSRsp_t_new(){
	createPVSRsp_t* this = malloc(sizeof(createPVSRsp_t));
	createPVSRsp_t_init(this);
	return this;
}

deleteAASReq_t* deleteAASReq_t_new(){
	deleteAASReq_t* this = malloc(sizeof(deleteAASReq_t));
	deleteAASReq_t_init(this);
	return this;
}

deleteAASRsp_t* deleteAASRsp_t_new(){
	deleteAASRsp_t* this = malloc(sizeof(deleteAASRsp_t));
	deleteAASRsp_t_init(this);
	return this;
}

deleteLCEReq_t* deleteLCEReq_t_new(){
	deleteLCEReq_t* this = malloc(sizeof(deleteLCEReq_t));
	deleteLCEReq_t_init(this);
	return this;
}

deleteLCERsp_t* deleteLCERsp_t_new(){
	deleteLCERsp_t* this = malloc(sizeof(deleteLCERsp_t));
	deleteLCERsp_t_init(this);
	return this;
}

deletePVSLReq_t* deletePVSLReq_t_new(){
	deletePVSLReq_t* this = malloc(sizeof(deletePVSLReq_t));
	deletePVSLReq_t_init(this);
	return this;
}

deletePVSLRsp_t* deletePVSLRsp_t_new(){
	deletePVSLRsp_t* this = malloc(sizeof(deletePVSLRsp_t));
	deletePVSLRsp_t_init(this);
	return this;
}

deletePVSReq_t* deletePVSReq_t_new(){
	deletePVSReq_t* this = malloc(sizeof(deletePVSReq_t));
	deletePVSReq_t_init(this);
	return this;
}

deletePVSRsp_t* deletePVSRsp_t_new(){
	deletePVSRsp_t* this = malloc(sizeof(deletePVSRsp_t));
	deletePVSRsp_t_init(this);
	return this;
}

getLCEReq_t* getLCEReq_t_new(){
	getLCEReq_t* this = malloc(sizeof(getLCEReq_t));
	getLCEReq_t_init(this);
	return this;
}

getLCERsp_t* getLCERsp_t_new(){
	getLCERsp_t* this = malloc(sizeof(getLCERsp_t));
	getLCERsp_t_init(this);
	return this;
}

getPVSReq_t* getPVSReq_t_new(){
	getPVSReq_t* this = malloc(sizeof(getPVSReq_t));
	getPVSReq_t_init(this);
	return this;
}

getPVSRsp_t* getPVSRsp_t_new(){
	getPVSRsp_t* this = malloc(sizeof(getPVSRsp_t));
	getPVSRsp_t_init(this);
	return this;
}

setLCEReq_t* setLCEReq_t_new(){
	setLCEReq_t* this = malloc(sizeof(setLCEReq_t));
	setLCEReq_t_init(this);
	return this;
}

setLCERsp_t* setLCERsp_t_new(){
	setLCERsp_t* this = malloc(sizeof(setLCERsp_t));
	setLCERsp_t_init(this);
	return this;
}

setPVSReq_t* setPVSReq_t_new(){
	setPVSReq_t* this = malloc(sizeof(setPVSReq_t));
	setPVSReq_t_init(this);
	return this;
}

setPVSRsp_t* setPVSRsp_t_new(){
	setPVSRsp_t* this = malloc(sizeof(setPVSRsp_t));
	setPVSRsp_t_init(this);
	return this;
}



void SRV_ident_t_init (SRV_ident_t* this){
	this->idType = SRV_IDT_undefined;
	SRV_String_init(&this->idSpec);
}

void PVSL_t_init (PVSL_t* this){
	SRV_ident_t_init(&this->carrierId);
	SRV_String_init(&this->name);
	this->hasName = false;
}

void PVS_t_init (PVS_t* this){
	SRV_String_init(&this->name);
	SRV_String_init(&this->unit);
	SRV_ident_t_init(&this->propertyReference);
	this->expressionSemantic = SRV_ES_undefined;
	this->relationalExpression = SRV_RE_undefined;
	this->valType = SRV_VT_undefined;
	this->value = malloc(SRV_VALUEMEMSIZE);
	this->view = SRV_VIEW_undefined;
	this->hasName = false;
	this->hasUnit =false;

}

void LCE_t_init (LCE_t* this){
	SRV_ident_t_init(&this->creatingInstanceId);
	SRV_ident_t_init(&this->writingInstanceId);
	SRV_String_init(&this->eventClass);
	SRV_String_init(&this->subject);
	this->data = malloc(SRV_VALUEMEMSIZE);
	this->dataTime = 0;
	this->dataType = SRV_VT_undefined;
	this->lceId = 0;
	this->hasEventClass = false;
	this->hasLceId = false;
	this->hasSubject = false;
	this->hastDataTime = false;
}

void SRV_msgHeader_t_init (SRV_msgHeader* this){
	SRV_ident_t_init(&this->receiver);
	SRV_ident_t_init(&this->sender);
	this->msgNo = 0;
}

void SRV_serviceGeneric_init(void* this, SRV_service_t type){
	switch(type){
	case SRV_createAASReq:
		createAASReq_t_init(this);
		break;
	case SRV_createAASRsp:
		createAASRsp_t_init(this);
		break;
	case SRV_deleteAASReq:
		deleteAASReq_t_init(this);
		break;
	case SRV_deleteAASRsp:
		deleteAASRsp_t_init(this);
		break;
	case SRV_createPVSLReq:
		createPVSLReq_t_init(this);
		break;
	case SRV_createPVSLRsp:
		createPVSLRsp_t_init(this);
		break;
	case SRV_deletePVSLReq:
		deletePVSLReq_t_init(this);
		break;
	case SRV_deletePVSLRsp:
		deletePVSLRsp_t_init(this);
		break;
	case SRV_createPVSReq:
		createPVSReq_t_init(this);
		break;
	case SRV_createPVSRsp:
		createPVSRsp_t_init(this);
		break;
	case SRV_deletePVSReq:
		deletePVSReq_t_init(this);
		break;
	case SRV_deletePVSRsp:
		deletePVSRsp_t_init(this);
		break;
	case SRV_createLCEReq:
		createLCEReq_t_init(this);
		break;
	case SRV_createLCERsp:
		createLCERsp_t_init(this);
		break;
	case SRV_deleteLCEReq:
		deleteLCEReq_t_init(this);
		break;
	case SRV_deleteLCERsp:
		deleteLCERsp_t_init(this);
		break;
	case SRV_getPVSReq:
		getPVSReq_t_init(this);
		break;
	case SRV_getPVSRsp:
		getPVSRsp_t_init(this);
		break;
	case SRV_setPVSReq:
		setPVSReq_t_init(this);
		break;
	case SRV_setPVSRsp:
		setPVSRsp_t_init(this);
		break;
	case SRV_getLCEReq:
		getLCEReq_t_init(this);
		break;
	case SRV_getLCERsp:
		getLCERsp_t_init(this);
		break;
	case SRV_setLCEReq:
		setLCEReq_t_init(this);
		break;
	case SRV_setLCERsp:
		setLCERsp_t_init(this);
		break;
	case SRV_undefined:default:
		break;
	}
	return;
}

void createAASReq_t_init (createAASReq_t* this){
	SRV_ident_t_init(&this->aasId);
	SRV_ident_t_init(&this->assetId);
	SRV_String_init(&this->aasName);
}
void createAASRsp_t_init (createAASRsp_t* this){
	this->status = 0;
}
void deleteAASReq_t_init (deleteAASReq_t* this){
	SRV_ident_t_init(&this->aasId);
}
void deleteAASRsp_t_init (deleteAASRsp_t* this){
	this->status = 0;
}
void createPVSLReq_t_init (createPVSLReq_t* this){
	SRV_ident_t_init(&this->carrier);
	SRV_String_init(&this->pvslName);
}
void createPVSLRsp_t_init (createPVSLRsp_t* this){
	this->status = 0;
}
void deletePVSLReq_t_init (deletePVSLReq_t* this){
	SRV_String_init(&this->pvslName);
}
void deletePVSLRsp_t_init (deletePVSLRsp_t* this){
	this->status = 0;
}
void createPVSReq_t_init (createPVSReq_t* this){
	PVS_t_init(&this->pvs);
	SRV_String_init(&this->pvslName);
}
void createPVSRsp_t_init (createPVSRsp_t* this){
	this->status = 0;
}
void deletePVSReq_t_init (deletePVSReq_t* this){
	SRV_String_init(&this->pvslName);
	SRV_String_init(&this->pvsName);
}
void deletePVSRsp_t_init (deletePVSRsp_t* this){
	this->status = 0;
}
void createLCEReq_t_init (createLCEReq_t* this){
	LCE_t_init(&this->lce);
}
void createLCERsp_t_init (createLCERsp_t* this){
	this->status = 0;
}
void deleteLCEReq_t_init (deleteLCEReq_t* this){
	this->lceId = 0;
}
void deleteLCERsp_t_init (deleteLCERsp_t* this){
	this->status = 0;
}
void getPVSReq_t_init (getPVSReq_t* this){
	SRV_String_init(&this->pvslName);
	SRV_String_init(&this->pvsName);
}
void getPVSRsp_t_init (getPVSRsp_t* this){
	PVS_t_init(&this->pvs);
	this->status = 0;
}
void setPVSReq_t_init (setPVSReq_t* this){
	PVS_t_init(&this->pvs);
	SRV_String_init(&this->pvslName);
	//SRV_String_init(&this->pvsName);
}
void setPVSRsp_t_init (setPVSRsp_t* this){
	this->status = 0;
}
void getLCEReq_t_init (getLCEReq_t* this){
	this->lceId = 0;
}
void getLCERsp_t_init (getLCERsp_t* this){
	LCE_t_init(&this->lce);
	this->status = 0;
}
void setLCEReq_t_init (setLCEReq_t* this){
	LCE_t_init(&this->lce);
	//this->lceId = 0;
}
void setLCERsp_t_init (setLCERsp_t* this){
	this->status = 0;
}



void SRV_ident_t_deleteMembers (SRV_ident_t* this){
	SRV_String_deleteMembers(&this->idSpec);
}

void PVSL_t_deleteMembers (PVSL_t* this){
	SRV_ident_t_deleteMembers(&this->carrierId);
	SRV_String_deleteMembers(&this->name);
}

void PVS_t_deleteMembers (PVS_t* this){
	SRV_String_deleteMembers(&this->name);
	SRV_String_deleteMembers(&this->unit);
	SRV_ident_t_deleteMembers(&this->propertyReference);
	if(this->value){
		if(this->valType==SRV_VT_STRING)
			SRV_String_deleteMembers(this->value);
		free(this->value);
		this->value = NULL;
	}
}

void LCE_t_deleteMembers (LCE_t* this){
	SRV_ident_t_deleteMembers(&this->creatingInstanceId);
	SRV_ident_t_deleteMembers(&this->writingInstanceId);
	SRV_String_deleteMembers(&this->eventClass);
	SRV_String_deleteMembers(&this->subject);
	if(this->data){
		if(this->dataType==SRV_VT_STRING)
			SRV_String_deleteMembers(this->data);
		free(this->data);
		this->data = NULL;
	}
}

void SRV_msgHeader_t_deleteMembers (SRV_msgHeader* this){
	SRV_ident_t_deleteMembers(&this->receiver);
	SRV_ident_t_deleteMembers(&this->sender);
}


void SRV_serviceGeneric_deleteMembers(void* this, SRV_service_t type){
	switch(type){
	case SRV_createAASReq:
		createAASReq_t_deleteMembers(this);
		break;
	case SRV_createAASRsp:
		//createAASRsp_t_deleteMembers(this);
		break;
	case SRV_deleteAASReq:
		deleteAASReq_t_deleteMembers(this);
		break;
	case SRV_deleteAASRsp:
		//deleteAASRsp_t_deleteMembers(this);
		break;
	case SRV_createPVSLReq:
		createPVSLReq_t_deleteMembers(this);
		break;
	case SRV_createPVSLRsp:
		//createPVSLRsp_t_deleteMembers(this);
		break;
	case SRV_deletePVSLReq:
		deletePVSLReq_t_deleteMembers(this);
		break;
	case SRV_deletePVSLRsp:
		//deletePVSLRsp_t_deleteMembers(this);
		break;
	case SRV_createPVSReq:
		createPVSReq_t_deleteMembers(this);
		break;
	case SRV_createPVSRsp:
		//createPVSRsp_t_deleteMembers(this);
		break;
	case SRV_deletePVSReq:
		deletePVSReq_t_deleteMembers(this);
		break;
	case SRV_deletePVSRsp:
		//deletePVSRsp_t_deleteMembers(this);
		break;
	case SRV_createLCEReq:
		createLCEReq_t_deleteMembers(this);
		break;
	case SRV_createLCERsp:
		//createLCERsp_t_deleteMembers(this);
		break;
	case SRV_deleteLCEReq:
		//deleteLCEReq_t_deleteMembers(this);
		break;
	case SRV_deleteLCERsp:
		//deleteLCERsp_t_deleteMembers(this);
		break;
	case SRV_getPVSReq:
		getPVSReq_t_deleteMembers(this);
		break;
	case SRV_getPVSRsp:
		getPVSRsp_t_deleteMembers(this);
		break;
	case SRV_setPVSReq:
		setPVSReq_t_deleteMembers(this);
		break;
	case SRV_setPVSRsp:
		//setPVSRsp_t_deleteMembers(this);
		break;
	case SRV_getLCEReq:
		//getLCEReq_t_deleteMembers(this);
		break;
	case SRV_getLCERsp:
		getLCERsp_t_deleteMembers(this);
		break;
	case SRV_setLCEReq:
		setLCEReq_t_deleteMembers(this);
		break;
	case SRV_setLCERsp:
		//setLCERsp_t_deleteMembers(this);
		break;
	case SRV_undefined:default:
		break;
	}
	return;
}

void createAASReq_t_deleteMembers (createAASReq_t* this){
	SRV_ident_t_deleteMembers(&this->aasId);
	SRV_ident_t_deleteMembers(&this->assetId);
	SRV_String_deleteMembers(&this->aasName);
}
void createAASRsp_t_deleteMembers (createAASRsp_t* this){}
void deleteAASReq_t_deleteMembers (deleteAASReq_t* this){
	SRV_ident_t_deleteMembers(&this->aasId);
}
void deleteAASRsp_t_deleteMembers (deleteAASRsp_t* this){}
void createPVSLReq_t_deleteMembers (createPVSLReq_t* this){
	SRV_ident_t_deleteMembers(&this->carrier);
	SRV_String_deleteMembers(&this->pvslName);
}
void createPVSLRsp_t_deleteMembers (createPVSLRsp_t* this){}
void deletePVSLReq_t_deleteMembers (deletePVSLReq_t* this){
	SRV_String_deleteMembers(&this->pvslName);
}
void deletePVSLRsp_t_deleteMembers (deletePVSLRsp_t* this){}
void createPVSReq_t_deleteMembers (createPVSReq_t* this){
	PVS_t_deleteMembers(&this->pvs);
	SRV_String_deleteMembers(&this->pvslName);
}
void createPVSRsp_t_deleteMembers (createPVSRsp_t* this){}
void deletePVSReq_t_deleteMembers (deletePVSReq_t* this){
	SRV_String_deleteMembers(&this->pvslName);
	SRV_String_deleteMembers(&this->pvsName);
}
void deletePVSRsp_t_deleteMembers (deletePVSRsp_t* this){}
void createLCEReq_t_deleteMembers (createLCEReq_t* this){
	LCE_t_deleteMembers(&this->lce);
}
void createLCERsp_t_deleteMembers (createLCERsp_t* this){}
void deleteLCEReq_t_deleteMembers (deleteLCEReq_t* this){}
void deleteLCERsp_t_deleteMembers (deleteLCERsp_t* this){}
void getPVSReq_t_deleteMembers (getPVSReq_t* this){
	SRV_String_deleteMembers(&this->pvslName);
	SRV_String_deleteMembers(&this->pvsName);
}
void getPVSRsp_t_deleteMembers (getPVSRsp_t* this){
	PVS_t_deleteMembers(&this->pvs);
}
void setPVSReq_t_deleteMembers (setPVSReq_t* this){
	PVS_t_deleteMembers(&this->pvs);
	SRV_String_deleteMembers(&this->pvslName);
	//SRV_String_deleteMembers(&this->pvsName);
}
void setPVSRsp_t_deleteMembers (setPVSRsp_t* this){}
void getLCEReq_t_deleteMembers (getLCEReq_t* this){}
void getLCERsp_t_deleteMembers (getLCERsp_t* this){
	LCE_t_deleteMembers(&this->lce);
}
void setLCEReq_t_deleteMembers (setLCEReq_t* this){
	LCE_t_deleteMembers(&this->lce);
}
void setLCERsp_t_deleteMembers (setLCERsp_t* this){}



void SRV_ident_t_delete (SRV_ident_t* this){
	SRV_ident_t_deleteMembers(this);
	free(this);
}

void PVSL_t_delete (PVSL_t* this){
	PVSL_t_deleteMembers(this);
	free(this);
}
void PVS_t_delete (PVS_t* this){
	PVS_t_deleteMembers(this);
	free(this);
}
void LCE_t_delete (LCE_t* this){
	LCE_t_deleteMembers(this);
	free(this);
}

void SRV_msgHeader_t_delete (SRV_msgHeader* this){
	SRV_msgHeader_t_deleteMembers(this);
	free(this);
}


void SRV_serviceGeneric_delete (void* this, SRV_service_t type){
	SRV_serviceGeneric_deleteMembers(this, type);
	free(this);
}

void createAASReq_t_delete (createAASReq_t* this){
	createAASReq_t_deleteMembers(this);
	free(this);
}
void createAASRsp_t_delete (createAASRsp_t* this){
	//createAASRsp_t_deleteMembers(this);
	free(this);
}
void deleteAASReq_t_delete (deleteAASReq_t* this){
	deleteAASReq_t_deleteMembers(this);
	free(this);
}
void deleteAASRsp_t_delete (deleteAASRsp_t* this){
	//deleteAASRsp_t_deleteMembers(this);
	free(this);
}
void createPVSLReq_t_delete (createPVSLReq_t* this){
	createPVSLReq_t_deleteMembers(this);
	free(this);
}
void createPVSLRsp_t_delete (createPVSLRsp_t* this){
	//createPVSLRsp_t_deleteMembers(this);
	free(this);
}
void deletePVSLReq_t_delete (deletePVSLReq_t* this){
	deletePVSLReq_t_deleteMembers(this);
	free(this);
}
void deletePVSLRsp_t_delete (deletePVSLRsp_t* this){
	//deletePVSLRsp_t_deleteMembers(this);
	free(this);
}
void createPVSReq_t_delete (createPVSReq_t* this){
	createPVSReq_t_deleteMembers(this);
	free(this);
}
void createPVSRsp_t_delete (createPVSRsp_t* this){
	//createPVSRsp_t_deleteMembers(this);
	free(this);
}
void deletePVSReq_t_delete (deletePVSReq_t* this){
	deletePVSReq_t_deleteMembers(this);
	free(this);
}
void deletePVSRsp_t_delete (deletePVSRsp_t* this){
	//deletePVSRsp_t_deleteMembers(this);
	free(this);
}
void createLCEReq_t_delete (createLCEReq_t* this){
	createLCEReq_t_deleteMembers(this);
	free(this);
}
void createLCERsp_t_delete (createLCERsp_t* this){
	//createLCERsp_t_deleteMembers(this);
	free(this);
}
void deleteLCEReq_t_delete (deleteLCEReq_t* this){
	//deleteLCEReq_t_deleteMembers(this);
	free(this);
}
void deleteLCERsp_t_delete (deleteLCERsp_t* this){
	//deleteLCERsp_t_deleteMembers(this);
	free(this);
}
void getPVSReq_t_delete (getPVSReq_t* this){
	getPVSReq_t_deleteMembers(this);
	free(this);
}
void getPVSRsp_t_delete (getPVSRsp_t* this){
	getPVSRsp_t_deleteMembers(this);
	free(this);
}
void setPVSReq_t_delete (setPVSReq_t* this){
	setPVSReq_t_deleteMembers(this);
	free(this);
}
void setPVSRsp_t_delete (setPVSRsp_t* this){
	//setPVSRsp_t_deleteMembers(this);
	free(this);
}
void getLCEReq_t_delete (getLCEReq_t* this){
	//getLCEReq_t_deleteMembers(this);
	free(this);
}
void getLCERsp_t_delete (getLCERsp_t* this){
	getLCERsp_t_deleteMembers(this);
	free(this);
}
void setLCEReq_t_delete (setLCEReq_t* this){
	setLCEReq_t_deleteMembers(this);
	free(this);
}
void setLCERsp_t_delete (setLCERsp_t* this){
	//setLCERsp_t_deleteMembers(this);
	free(this);
}
