/*
 * openaas_helpers.c
 *
 *  Created on: 07.02.2017
 *      Author: ubuntu
 */

#include "openaas_helpers.h"

OV_BOOL IdentificationTypeEqual(IdentificationType *aasId, IdentificationType *aasId2){
	if (aasId->IdType == aasId2->IdType){
		if (ov_string_compare(aasId->IdSpec, aasId2->IdSpec) == OV_STRCMP_EQUAL)
			return true;
	}
	return false;
}

IdentificationType* IdentificationType_new(){
	IdentificationType* this = ov_database_malloc(sizeof(IdentificationType));
	IdentificationType_init(this);
	return this;
}
void IdentificationType_init(IdentificationType *this){
	this->IdSpec = NULL;
	this->IdType = 0;
}
void IdentificationType_deleteMembers(IdentificationType *this){
	ov_database_free(this->IdSpec);
	IdentificationType_init(this);
}
void IdentificationType_delete(IdentificationType *this){
	IdentificationType_deleteMembers(this);
	ov_database_free(this);
}

PropertyValueStatementList* PropertyValueStatementList_new(){
	PropertyValueStatementList* this = ov_database_malloc(sizeof(PropertyValueStatementList));
	PropertyValueStatementList_init(this);
	return this;
}
void PropertyValueStatementList_init(PropertyValueStatementList *this){
	IdentificationType_init(&this->Carrier);
	IdentificationType_init(&this->CreatingInstance);
	ov_time_gettime(&this->CreationTime);
	this->pvs = NULL;
	this->pvsNumber = 0;
}
void PropertyValueStatementList_deleteMembers(PropertyValueStatementList *this){
	IdentificationType_delete(&this->Carrier);
	IdentificationType_delete(&this->CreatingInstance);
	for (OV_UINT i = 0; i < this->pvsNumber; i++){
		PropertyValueStatement_deleteMembers(&(this->pvs)[i]);
	}
	PropertyValueStatementList_init(this);
}
void PropertyValueStatementList_delete(PropertyValueStatementList *this){
	PropertyValueStatementList_deleteMembers(this);
	ov_database_free(this);
}

PropertyValueStatement* PropertyValueStatement_new(){
	PropertyValueStatement* this = ov_database_malloc(sizeof(PropertyValueStatement));
	PropertyValueStatement_init(this);
	return this;
}

void PropertyValueStatement_init(PropertyValueStatement *this){
	this->ExpressionSemantic = 0;
	this->Visibility = 0;
	IdentificationType_init(&this->ID);
	this->pvsName = NULL;
	this->ExpressionLogic = 0;
	this->unit = NULL;
	this->view = 0;
	this->value.Value.value.valueunion.val_string = NULL;
}
void PropertyValueStatement_deleteMembers(PropertyValueStatement *this){
	IdentificationType_delete(&this->ID);
	ov_database_free(this->pvsName);
	ov_database_free(this->unit);
	PropertyValueStatement_init(this);
}
void PropertyValueStatement_delete(PropertyValueStatement *this){
	PropertyValueStatement_deleteMembers(this);
	free(this);
}

LifeCycleEntry* LifeCycleEntry_new(){
	LifeCycleEntry* this = ov_database_malloc(sizeof(LifeCycleEntry));
	LifeCycleEntry_init(this);
	return this;
}
void LifeCycleEntry_init(LifeCycleEntry *this){
	IdentificationType_init(&this->creatingInstance);
	IdentificationType_init(&this->writingInstance);
	this->eventClass = NULL;
	this->lceId = 0;
	this->subject = NULL;
	this->data.Value.value.valueunion.val_string = NULL;
}
void LifeCycleEntry_deleteMembers(LifeCycleEntry *this){
	IdentificationType_delete(&this->creatingInstance);
	IdentificationType_delete(&this->writingInstance);
	ov_database_free(this->eventClass);
	ov_database_free(this->subject);
}
void LifeCycleEntry_delete(LifeCycleEntry *this){
	LifeCycleEntry_deleteMembers(this);
	ov_database_free(this);
}



OV_RESULT decodeMSG(const SRV_String* str, SRV_msgHeader** header, void** srvStruct, SRV_service_t* srvType, SRV_encoding_t *encoding){

	if (strncmp(str->data, "JSON", 4) == 0){ // JSON Encoding
		SRV_String *tmpStr = SRV_String_new();
		tmpStr->data = malloc((str->length-4+1)*sizeof(char));
		tmpStr->length = str->length-4;
		memcpy(tmpStr->data, (str->data+4), (str->length-4)*sizeof(char));
		tmpStr->data[tmpStr->length] = '\0';
		JSON_RC resultJSON = parseJson(tmpStr, header, srvStruct, srvType);
		*encoding = SRV_JSON;
		if (resultJSON){
			return resultJSON;
		}
		SRV_String_delete(tmpStr);
	}else if (strncmp(str->data, "OPCB", 4) == 0){ // OPC UA binary Encoding
		return OV_ERR_NOTIMPLEMENTED;
	}else if (strncmp(str->data, "XMLT", 4) == 0){ // XML text Encoding
		return OV_ERR_NOTIMPLEMENTED;
	}else if (strncmp(str->data, "XMLB", 4) == 0){ // XML binary Encoding
		return OV_ERR_NOTIMPLEMENTED;
	}else{
		return OV_ERR_NOTIMPLEMENTED;
	}
	return OV_ERR_OK;
}

OV_RESULT encodeMSG(SRV_String** str, const SRV_msgHeader *header, const void* srvStruct, SRV_service_t srvType, SRV_encoding_t encoding){
	SRV_String* strtmp = NULL;
	switch(encoding){
	case SRV_JSON:{ // JSON Encoding
		JSON_RC resultJSON = genJson(&strtmp, header, srvStruct, srvType);
		if (resultJSON){
			return resultJSON;
		}
		*str = malloc(sizeof(SRV_String));
		(*str)->data = malloc(strtmp->length+4+1);
		(*str)->length = strtmp->length+4;
		memcpy((*str)->data, "JSON", 4*sizeof(char));
		memcpy((*str)->data+4, strtmp->data, (strtmp->length)*sizeof(char));
		(*str)->data[(*str)->length] = '\0';
	}break;
	case SRV_OPCB: // OPC UA binary Encoding
		*str = malloc(sizeof(SRV_String));
		(*str)->data = malloc(strtmp->length+4);
		(*str)->length = strtmp->length+4;
		memcpy((*str)->data, "OPCB", 4*sizeof(char));
		memcpy((*str)->data+4, strtmp->data, (strtmp->length)*sizeof(char));
		break;
	case SRV_XMLT: // XML text Encoding
		*str = malloc(sizeof(SRV_String));
		(*str)->data = malloc(strtmp->length+4);
		(*str)->length = strtmp->length+4;
		memcpy((*str)->data, "XMLT", 4*sizeof(char));
		memcpy((*str)->data+4, strtmp->data, (strtmp->length)*sizeof(char));
		break;
	case SRV_XMLB: // XML binary Encoding
		*str = malloc(sizeof(SRV_String));
		(*str)->data = malloc(strtmp->length+4);
		(*str)->length = strtmp->length+4;
		memcpy((*str)->data, "XMLB", 4*sizeof(char));
		memcpy((*str)->data+4, strtmp->data, (strtmp->length)*sizeof(char));
		break;
	default:
		return OV_ERR_BADTYPE;
		break;
	}
	return OV_ERR_OK;
}

OV_RESULT serviceValueToOVDataValue(DataValue* value, SRV_valType_t valueType, void* serviceValue, SRV_DateTime dateTime){
	switch(valueType){
	case SRV_VT_BOOL:
		value->Value.value.vartype = OV_VT_BOOL;
		value->Value.value.valueunion.val_bool = *(bool*)serviceValue;
		break;
	case SRV_VT_DOUBLE:
		value->Value.value.vartype = OV_VT_DOUBLE;
		value->Value.value.valueunion.val_double = *(double*)serviceValue;
		break;
	case SRV_VT_INT32:
		value->Value.value.vartype = OV_VT_INT;
		value->Value.value.valueunion.val_int = *(int*)serviceValue;
		break;
	case SRV_VT_UINT32:
		value->Value.value.vartype = OV_VT_UINT;
		value->Value.value.valueunion.val_uint = *(int*)serviceValue;
		break;
	case SRV_VT_INT64:
	case SRV_VT_UINT64:
		return OV_ERR_NOTIMPLEMENTED;
		break;
	case SRV_VT_STRING:
		value->Value.value.vartype = OV_VT_STRING;
		value->Value.value.valueunion.val_string = NULL;
		ov_string_setvalue(&value->Value.value.valueunion.val_string, (char*)serviceValue);
		break;
	case SRV_VT_DATETIME:
		value->Value.value.vartype = OV_VT_TIME;
		value->Value.value.valueunion.val_time = ov_1601nsTimeToOvTime(*(SRV_DateTime*)serviceValue);
	default:
		return OV_ERR_VARDEFMISMATCH;
		break;
	}
	value->TimeStamp = ov_1601nsTimeToOvTime(dateTime);
	return OV_ERR_OK;
}

OV_RESULT OVDataValueToserviceValue(DataValue value, SRV_valType_t* valueType, void** serviceValue, SRV_DateTime* dateTime){

	switch(value.Value.value.vartype){
	case OV_VT_BOOL:
		*valueType = SRV_VT_BOOL;
		*serviceValue = malloc(sizeof(bool));
		*(bool*)*serviceValue = value.Value.value.valueunion.val_bool;
		break;
	case OV_VT_DOUBLE:
		*valueType = SRV_VT_DOUBLE;
		*serviceValue = malloc(sizeof(double));
		*(double*)*serviceValue = value.Value.value.valueunion.val_double;
		break;
	case OV_VT_INT:
		*valueType = SRV_VT_INT32;
		*serviceValue = malloc(sizeof(int));
		*(int*)*serviceValue = value.Value.value.valueunion.val_int;
		break;
	case OV_VT_UINT:
		*valueType = SRV_VT_UINT32;
		*serviceValue = malloc(sizeof(unsigned int));
		*(unsigned int*)*serviceValue = value.Value.value.valueunion.val_uint;
	case OV_VT_STRING:
		*valueType = SRV_VT_STRING;
		*serviceValue = SRV_String_new();
		SRV_String_setCopy((SRV_String*)serviceValue, value.Value.value.valueunion.val_string, ov_string_getlength(value.Value.value.valueunion.val_string));
		break;
	case OV_VT_TIME:
		*valueType = SRV_VT_DATETIME;
		*serviceValue = malloc(sizeof(SRV_DateTime));
		*(SRV_DateTime*)*serviceValue = ov_ovTimeTo1601nsTime(value.Value.value.valueunion.val_time);
	default:
		return OV_ERR_VARDEFMISMATCH;
		break;
	}
	*dateTime = ov_ovTimeTo1601nsTime(value.TimeStamp);
	return OV_ERR_OK;
}
