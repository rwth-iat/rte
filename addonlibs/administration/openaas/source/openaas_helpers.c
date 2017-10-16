/*
 * openaas_helpers.c
 *
 *  Created on: 07.02.2017
 *      Author: ubuntu
 */

#include "openaas_helpers.h"

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
		SRV_String_delete(strtmp);
	}break;
	case SRV_OPCB: // OPC UA binary Encoding
		return OV_ERR_NOTIMPLEMENTED;
		break;
	case SRV_XMLT: // XML text Encoding
		return OV_ERR_NOTIMPLEMENTED;
		break;
	case SRV_XMLB: // XML binary Encoding
		return OV_ERR_NOTIMPLEMENTED;
	break;
	default:
		return OV_ERR_BADTYPE;
		break;
	}
	return OV_ERR_OK;
}

OV_RESULT serviceValueToOVDataValue(OV_ANY* value, SRV_valType_t valueType, void* serviceValue, SRV_DateTime dateTime){
	switch(valueType){
	case SRV_VT_BOOL:
		value->value.vartype = OV_VT_BOOL;
		value->value.valueunion.val_bool = *(bool*)serviceValue;
		break;
	case SRV_VT_DOUBLE:
		value->value.vartype = OV_VT_DOUBLE;
		value->value.valueunion.val_double = *(double*)serviceValue;
		break;
	case SRV_VT_INT32:
		value->value.vartype = OV_VT_INT;
		value->value.valueunion.val_int = *(int*)serviceValue;
		break;
	case SRV_VT_UINT32:
		value->value.vartype = OV_VT_UINT;
		value->value.valueunion.val_uint = *(int*)serviceValue;
		break;
	case SRV_VT_INT64:
	case SRV_VT_UINT64:
		return OV_ERR_NOTIMPLEMENTED;
		break;
	case SRV_VT_STRING:
		value->value.vartype = OV_VT_STRING;
		value->value.valueunion.val_string = NULL;
		ov_string_setvalue(&value->value.valueunion.val_string, (char*)serviceValue);
		break;
	case SRV_VT_DATETIME:
		value->value.vartype = OV_VT_TIME;
		value->value.valueunion.val_time = ov_1601nsTimeToOvTime(*(SRV_DateTime*)serviceValue);
	default:
		return OV_ERR_VARDEFMISMATCH;
		break;
	}
	value->time = ov_1601nsTimeToOvTime(dateTime);
	return OV_ERR_OK;
}

OV_RESULT OVDataValueToserviceValue(OV_ANY value, SRV_valType_t* valueType, void** serviceValue, SRV_DateTime* dateTime){

	switch(value.value.vartype){
	case OV_VT_BOOL:
		*valueType = SRV_VT_BOOL;
		*serviceValue = malloc(sizeof(bool));
		*(bool*)*serviceValue = value.value.valueunion.val_bool;
		break;
	case OV_VT_DOUBLE:
		*valueType = SRV_VT_DOUBLE;
		*serviceValue = malloc(sizeof(double));
		*(double*)*serviceValue = value.value.valueunion.val_double;
		break;
	case OV_VT_INT:
		*valueType = SRV_VT_INT32;
		*serviceValue = malloc(sizeof(int));
		*(int*)*serviceValue = value.value.valueunion.val_int;
		break;
	case OV_VT_UINT:
		*valueType = SRV_VT_UINT32;
		*serviceValue = malloc(sizeof(unsigned int));
		*(unsigned int*)*serviceValue = value.value.valueunion.val_uint;
	case OV_VT_STRING:
		*valueType = SRV_VT_STRING;
		*serviceValue = SRV_String_new();
		SRV_String_setCopy((SRV_String*)serviceValue, value.value.valueunion.val_string, ov_string_getlength(value.value.valueunion.val_string));
		break;
	case OV_VT_TIME:
		*valueType = SRV_VT_DATETIME;
		*serviceValue = malloc(sizeof(SRV_DateTime));
		*(SRV_DateTime*)*serviceValue = ov_ovTimeTo1601nsTime(value.value.valueunion.val_time);
	default:
		return OV_ERR_VARDEFMISMATCH;
		break;
	}
	*dateTime = ov_ovTimeTo1601nsTime(value.time);
	return OV_ERR_OK;
}


OV_BOOL getAASIdbyObjectPointer(OV_INSTPTR_openaas_aas pAAS, IdentificationType* pAASId){
	OV_INSTPTR_ov_object pchild = NULL;
	OV_INSTPTR_propertyValueStatement_CarrierId pCarrierId = NULL;
	if(!pAAS){
		return false;
	}

	Ov_ForEachChild(ov_containment, &pAAS->p_Header.p_Config, pchild){
		if (!Ov_CanCastTo(propertyValueStatement_CarrierId, pchild)){
			continue;
		}
		pCarrierId = Ov_DynamicPtrCast(propertyValueStatement_CarrierId, pchild);
		IdentificationType_init(pAASId);
		ov_string_setvalue(&pAASId->IdSpec, pCarrierId->v_IdSpec);
		pAASId->IdType = pCarrierId->v_IdType;
		return true;
	}
	return false;
}
