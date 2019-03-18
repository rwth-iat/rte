/*
 * openaas_helpers.c
 *
 *  Created on: 07.02.2017
 *      Author: ubuntu
 */

#ifndef OV_COMPILE_LIBRARY_openaas
#define OV_COMPILE_LIBRARY_openaas
#endif

#include "openaas_helpers.h"
#include "libov/ov_time.h"
#include "libov/ov_logfile.h"

OV_RESULT decodeMSG(const SRV_String* str, SRV_msgHeader** header, void** srvStruct, SRV_service_t* srvType, SRV_encoding_t *encoding){

	//if (strncmp(str->data, "JSON", 4) == 0){ // JSON Encoding
		SRV_String *tmpStr = SRV_String_new();
		tmpStr->data = malloc((str->length-4+1)*sizeof(char));
		tmpStr->length = str->length-4;
		memcpy(tmpStr->data, (str->data+4), (str->length-4)*sizeof(char));
		tmpStr->data[tmpStr->length] = '\0';
		JSON_RC resultJSON = parseJson(tmpStr, header, srvStruct, srvType);
		SRV_String_delete(tmpStr);
		*encoding = SRV_JSON;
		if (resultJSON){
			return resultJSON;
		}
//	}else if (strncmp(str->data, "OPCB", 4) == 0){ // OPC UA binary Encoding
		//return OV_ERR_NOTIMPLEMENTED;
	//}else if (strncmp(str->data, "XMLT", 4) == 0){ // XML text Encoding
//		return OV_ERR_NOTIMPLEMENTED;
//	}else if (strncmp(str->data, "XMLB", 4) == 0){ // XML binary Encoding
		//return OV_ERR_NOTIMPLEMENTED;
	//}else{
//		return OV_ERR_NOTIMPLEMENTED;
//	}
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

OV_RESULT serviceValueToOVDataValue(OV_ANY* value, const SRV_extAny_t* serviceValue){
	switch(serviceValue->type){
	case SRV_VT_BOOL:
		value->value.vartype = OV_VT_BOOL;
		value->value.valueunion.val_bool = serviceValue->value.vt_bool;
		break;
	case SRV_VT_SINGLE:
		value->value.vartype = OV_VT_SINGLE;
		value->value.valueunion.val_single = serviceValue->value.vt_single;
		break;
	case SRV_VT_DOUBLE:
		value->value.vartype = OV_VT_DOUBLE;
		value->value.valueunion.val_double = serviceValue->value.vt_double;
		break;
	case SRV_VT_INT32:
		value->value.vartype = OV_VT_INT;
		value->value.valueunion.val_int = serviceValue->value.vt_int32;
		break;
	case SRV_VT_UINT32:
		value->value.vartype = OV_VT_UINT;
		value->value.valueunion.val_uint = serviceValue->value.vt_uint32;
		break;
	case SRV_VT_INT64:
	case SRV_VT_UINT64:
		return OV_ERR_NOTIMPLEMENTED;
		break;
	case SRV_VT_STRING:
		value->value.vartype = OV_VT_STRING;
		value->value.valueunion.val_string = NULL;
		ov_string_setvalue(&value->value.valueunion.val_string, serviceValue->value.vt_string.data);
		break;
	case SRV_VT_DATETIME:
		value->value.vartype = OV_VT_TIME;
		value->value.valueunion.val_time = ov_1601nsTimeToOvTime(serviceValue->value.vt_datetime);
		break;
	default:
		return OV_ERR_VARDEFMISMATCH;
		break;
	}
	if(serviceValue->hasTime)
		value->time = ov_1601nsTimeToOvTime(serviceValue->time);

	return OV_ERR_OK;
}

OV_RESULT OVDataValueToserviceValue(OV_ANY value, SRV_extAny_t* serviceValue){

	switch(value.value.vartype & OV_VT_KSMASK){
	case OV_VT_BOOL:
		serviceValue->type = SRV_VT_BOOL;
		serviceValue->value.vt_bool = value.value.valueunion.val_bool;
		break;
	case OV_VT_SINGLE:
		serviceValue->type =SRV_VT_SINGLE;
		serviceValue->value.vt_single = value.value.valueunion.val_single;
		break;
	case OV_VT_DOUBLE:
		serviceValue->type = SRV_VT_DOUBLE;
		serviceValue->value.vt_double = value.value.valueunion.val_double;
		break;
	case OV_VT_INT:
		serviceValue->type = SRV_VT_INT32;
		serviceValue->value.vt_int32 = value.value.valueunion.val_int;
		break;
	case OV_VT_UINT:
		serviceValue->type = SRV_VT_UINT32;
		serviceValue->value.vt_uint32 = value.value.valueunion.val_uint;
		break;
	case OV_VT_STRING:
		serviceValue->type = SRV_VT_STRING;
		SRV_String_setCopy(&serviceValue->value.vt_string, value.value.valueunion.val_string, ov_string_getlength(value.value.valueunion.val_string));
		break;
	case OV_VT_TIME:
		serviceValue->type = SRV_VT_DATETIME;
		serviceValue->value.vt_datetime = ov_ovTimeTo1601nsTime(value.value.valueunion.val_time);
		break;
	default:
		return OV_ERR_VARDEFMISMATCH;
		break;
	}
	serviceValue->time = ov_ovTimeTo1601nsTime(value.time);
	serviceValue->hasTime = true;
	return OV_ERR_OK;
}

AASStatusCode checkForEmbeddingAAS(IdentificationType aasId, IdentificationType objectId){
	OV_INSTPTR_ov_object ptr = NULL;
	OV_INSTPTR_openaas_aas paas = NULL;
	OV_INSTPTR_ov_object ptr2 = NULL;
	OV_INSTPTR_ov_object ptr3 = NULL;
	OV_INSTPTR_ov_object ptr4 = NULL;

	ptr = ov_path_getobjectpointer(openaas_modelmanager_AASConvertListGet(aasId), 2);
	if(!ptr){
		return AASSTATUSCODE_BADAASID;
	}
	paas = Ov_StaticPtrCast(openaas_aas, ptr);

	if (!paas){
		return AASSTATUSCODE_BADAASID;
	}
	if (objectId.IdType != URI){
		return AASSTATUSCODE_BADPARENTID;
	}
	ptr2 = ov_path_getobjectpointer(objectId.IdSpec, 2);
	if (!ptr2)
		return AASSTATUSCODE_BADPARENTID;

	ptr3 = Ov_StaticPtrCast(ov_object, Ov_GetParent(ov_containment, ptr2));
	if (!ptr3){
		ptr3 = ptr2->v_pouterobject;
	}
	do{
		if (paas == Ov_StaticPtrCast(openaas_aas, ptr3)){
			return AASSTATUSCODE_GOOD;
		}
		ptr4 = Ov_StaticPtrCast(ov_object, Ov_GetParent(ov_containment, ptr3));
		if (!ptr4){
			ptr4 = ptr3->v_pouterobject;
		}
		ptr3 = ptr4;
	}while (ptr3);
	return AASSTATUSCODE_BADPARENTID;
}

OV_DLLFNCEXPORT OV_STRING getStatementsInJSON(OV_INSTPTR_openaas_aas paas){
	OV_INSTPTR_openaas_SubModel pSubmodel = NULL;
	OV_INSTPTR_propertyValueStatement_PropertyValueStatementList pPVSList = NULL;
	OV_INSTPTR_propertyValueStatement_PropertyValueStatement pPVS = NULL;
	OV_INSTPTR_propertyValueStatement_CarrierId pCarrierID = NULL;
	OV_INSTPTR_propertyValueStatement_PropertyId pPropertyID = NULL;
	OV_INSTPTR_propertyValueStatement_ExpressionSemantic pExpressionSemantic = NULL;
	OV_INSTPTR_propertyValueStatement_ExpressionLogic pExpressionLogic = NULL;
	OV_INSTPTR_ov_object pObject = NULL;
	OV_INSTPTR_ov_object pObject2 = NULL;
	OV_STRING statementString = NULL;
	OV_STRING tmpString = NULL;
	OV_UINT stringLength = 0;
	OV_BOOL first = TRUE;
	if (!paas){
		return NULL;
	}

	OV_UINT stringLengthCarrierID = strlen("{\"CarrierID\":\",\"");
	OV_UINT stringLengthCarrierID2 = strlen(",{\"CarrierID\":\",\"");
	OV_UINT stringLengthPropertyID = strlen("{\"PropertyID\":\",\"");
	OV_UINT stringLengthExpressionSemantic = strlen(",\"ExpressionSemantic\":\"\"");
	OV_UINT stringLengthExpressionSemanticA = strlen(",\"ExpressionSemantic\":\"Assurance\"");
	OV_UINT stringLengthExpressionSemanticR = strlen(",\"ExpressionSemantic\":\"Requirement\"");
	OV_UINT stringLengthExpressionSemanticM = strlen(",\"ExpressionSemantic\":\"Measurement\"");
	OV_UINT stringLengthExpressionSemanticS = strlen(",\"ExpressionSemantic\":\"Setting\"");
	OV_UINT stringLengthRelation = strlen("{\"Relation\":\"\"");
	OV_UINT stringLengthRelationG = strlen("{\"Relation\":\">\"");
	OV_UINT stringLengthRelationGE = strlen("{\"Relation\":\">=\"");
	OV_UINT stringLengthRelationEQ = strlen("{\"Relation\":\"==\"");
	OV_UINT stringLengthRelationNE = strlen("{\"Relation\":\"!=\"");
	OV_UINT stringLengthRelationLE = strlen("{\"Relation\":\"<=\"");
	OV_UINT stringLengthRelationL = strlen("{\"Relation\":\"<\"");
	OV_UINT stringLengthValue = strlen("{\"Value\":\"\"");
	OV_UINT stringLengthValueType = strlen("{\"ValueType\":\"\"");
	OV_UINT stringLengthValueTypeB = strlen("{\"Value\":\"Boolean\"");
	OV_UINT stringLengthValueTypeN = strlen("{\"Value\":\"Numeric\"");
	OV_UINT stringLengthValueTypeT = strlen("{\"Value\":\"Text\"");
	OV_UINT stringLengthSubModel = strlen("{\"SubModel\":\"\"");
	OV_UINT stringLengthStatements = strlen("\"statements\": []");

	stringLength += stringLengthStatements;
	Ov_ForEachChildEx(ov_containment, &paas->p_Header, pSubmodel, openaas_SubModel){
		Ov_ForEachChildEx(ov_containment, pSubmodel, pPVSList, propertyValueStatement_PropertyValueStatementList){
			Ov_ForEachChild(ov_containment, pPVSList, pObject){
				if (Ov_CanCastTo(propertyValueStatement_CarrierId, pObject)){
					pCarrierID = Ov_DynamicPtrCast(propertyValueStatement_CarrierId, pObject);
				}
				if (Ov_CanCastTo(propertyValueStatement_PropertyId, pObject)){
					pPropertyID = Ov_DynamicPtrCast(propertyValueStatement_PropertyId, pObject);
				}
				if (Ov_CanCastTo(propertyValueStatement_ExpressionSemantic, pObject)){
					pExpressionSemantic = Ov_DynamicPtrCast(propertyValueStatement_ExpressionSemantic, pObject);
				}
				if (Ov_CanCastTo(propertyValueStatement_ExpressionLogic, pObject)){
					pExpressionLogic = Ov_DynamicPtrCast(propertyValueStatement_ExpressionLogic, pObject);
				}
				if (Ov_CanCastTo(propertyValueStatement_PropertyValueStatement, pObject)){
					pPVS = Ov_DynamicPtrCast(propertyValueStatement_PropertyValueStatement, pObject);
					Ov_ForEachChild(ov_containment, pPVS, pObject2){
						if (Ov_CanCastTo(propertyValueStatement_CarrierId, pObject2)){
							pCarrierID = Ov_DynamicPtrCast(propertyValueStatement_CarrierId, pObject2);
						}
						if (Ov_CanCastTo(propertyValueStatement_PropertyId, pObject2)){
							pPropertyID = Ov_DynamicPtrCast(propertyValueStatement_PropertyId, pObject2);
						}
						if (Ov_CanCastTo(propertyValueStatement_ExpressionSemantic, pObject2)){
							pExpressionSemantic = Ov_DynamicPtrCast(propertyValueStatement_ExpressionSemantic, pObject2);
						}
						if (Ov_CanCastTo(propertyValueStatement_ExpressionLogic, pObject2)){
							pExpressionLogic = Ov_DynamicPtrCast(propertyValueStatement_ExpressionLogic, pObject2);
						}
					}
					if (first == TRUE){
						stringLength += stringLengthCarrierID + 1 + strlen(pCarrierID->v_IdSpec);
						first = FALSE;
					}else{
						stringLength += stringLengthCarrierID2 + 1 + strlen(pCarrierID->v_IdSpec);
					}
					stringLength += stringLengthPropertyID + 1 + strlen(pPropertyID->v_IdSpec);

					switch(pExpressionSemantic->v_ExpressionSemanticEnum){
						case 0:
							stringLength += stringLengthExpressionSemanticA;
						break;
						case 1:
							stringLength += stringLengthExpressionSemanticS;
						break;
						case 2:
							stringLength += stringLengthExpressionSemanticM;
						break;
						case 3:
							stringLength += stringLengthExpressionSemanticR;
						break;
						default:
							stringLength += stringLengthExpressionSemantic;
						break;
					}

					switch(pExpressionLogic->v_ExpressionLogicEnum){
						case 0:
							stringLength += stringLengthRelationG;
						break;
						case 1:
							stringLength += stringLengthRelationGE;
						break;
						case 2:
							stringLength += stringLengthRelationEQ;
						break;
						case 3:
							stringLength += stringLengthRelationNE;
						break;
						case 4:
							stringLength += stringLengthRelationLE;
						break;
						case 5:
							stringLength += stringLengthRelationL;
						break;
						default:
							stringLength += stringLengthRelation;
						break;
					}
					switch(pPVS->v_Value.value.vartype & OV_VT_KSMASK){
						case OV_VT_BOOL:
							ov_string_print(&tmpString, "%d", pPVS->v_Value.value.valueunion.val_bool);
							stringLength += stringLengthValue + strlen(tmpString);
							stringLength += stringLengthValueTypeB;
							break;
						case OV_VT_SINGLE:
							ov_string_print(&tmpString, "%f", pPVS->v_Value.value.valueunion.val_single);
							stringLength += stringLengthValue + strlen(tmpString);
							stringLength += stringLengthValueTypeN;
							break;
						case OV_VT_DOUBLE:
							ov_string_print(&tmpString, "%lf", pPVS->v_Value.value.valueunion.val_double);
							stringLength += stringLengthValue + strlen(tmpString);
							stringLength += stringLengthValueTypeN;
							break;
						case OV_VT_INT:
							ov_string_print(&tmpString, "%d", pPVS->v_Value.value.valueunion.val_int);
							stringLength += stringLengthValue + strlen(tmpString);
							stringLength += stringLengthValueTypeN;
							break;
						case OV_VT_UINT:
							ov_string_print(&tmpString, "%u", pPVS->v_Value.value.valueunion.val_uint);
							stringLength += stringLengthValue + strlen(tmpString);
							stringLength += stringLengthValueTypeN;
							break;
						case OV_VT_STRING:
							if (pPVS->v_Value.value.valueunion.val_string != NULL){
								ov_string_print(&tmpString, "%s", pPVS->v_Value.value.valueunion.val_string);
								stringLength += stringLengthValue + strlen(tmpString);
							}else
								stringLength += stringLengthValue;

							stringLength += stringLengthValueTypeT;
							break;
						default:
							stringLength += stringLengthValue;
							stringLength += stringLengthValueType;
							break;
					}
					ov_string_print(&tmpString, "%s", pSubmodel->v_identifier);
					stringLength += stringLengthSubModel + strlen(tmpString);
					ov_string_setvalue(&tmpString, NULL);
				}
			}
		}
	}
	Ov_ForEachChildEx(ov_containment, &paas->p_Body, pSubmodel, openaas_SubModel){
		Ov_ForEachChildEx(ov_containment, pSubmodel, pPVSList, propertyValueStatement_PropertyValueStatementList){
			Ov_ForEachChild(ov_containment, pPVSList, pObject){
				if (Ov_CanCastTo(propertyValueStatement_CarrierId, pObject)){
					pCarrierID = Ov_DynamicPtrCast(propertyValueStatement_CarrierId, pObject);
				}
				if (Ov_CanCastTo(propertyValueStatement_PropertyId, pObject)){
					pPropertyID = Ov_DynamicPtrCast(propertyValueStatement_PropertyId, pObject);
				}
				if (Ov_CanCastTo(propertyValueStatement_ExpressionSemantic, pObject)){
					pExpressionSemantic = Ov_DynamicPtrCast(propertyValueStatement_ExpressionSemantic, pObject);
				}
				if (Ov_CanCastTo(propertyValueStatement_ExpressionLogic, pObject)){
					pExpressionLogic = Ov_DynamicPtrCast(propertyValueStatement_ExpressionLogic, pObject);
				}
				if (Ov_CanCastTo(propertyValueStatement_PropertyValueStatement, pObject)){
					pPVS = Ov_DynamicPtrCast(propertyValueStatement_PropertyValueStatement, pObject);
					Ov_ForEachChild(ov_containment, pPVS, pObject2){
						if (Ov_CanCastTo(propertyValueStatement_CarrierId, pObject2)){
							pCarrierID = Ov_DynamicPtrCast(propertyValueStatement_CarrierId, pObject2);
						}
						if (Ov_CanCastTo(propertyValueStatement_PropertyId, pObject2)){
							pPropertyID = Ov_DynamicPtrCast(propertyValueStatement_PropertyId, pObject2);
						}
						if (Ov_CanCastTo(propertyValueStatement_ExpressionSemantic, pObject2)){
							pExpressionSemantic = Ov_DynamicPtrCast(propertyValueStatement_ExpressionSemantic, pObject2);
						}
						if (Ov_CanCastTo(propertyValueStatement_ExpressionLogic, pObject2)){
							pExpressionLogic = Ov_DynamicPtrCast(propertyValueStatement_ExpressionLogic, pObject2);
						}
					}

					if (first == TRUE){
						stringLength += stringLengthCarrierID + 1 + strlen(pCarrierID->v_IdSpec);
						first = FALSE;
					}else{
						stringLength += stringLengthCarrierID2 + 1 + strlen(pCarrierID->v_IdSpec);
					}
					stringLength += stringLengthPropertyID + 1 + strlen(pPropertyID->v_IdSpec);

					switch(pExpressionSemantic->v_ExpressionSemanticEnum){
						case 0:
							stringLength += stringLengthExpressionSemanticA;
						break;
						case 1:
							stringLength += stringLengthExpressionSemanticS;
						break;
						case 2:
							stringLength += stringLengthExpressionSemanticM;
						break;
						case 3:
							stringLength += stringLengthExpressionSemanticR;
						break;
						default:
							stringLength += stringLengthExpressionSemantic;
						break;
					}

					switch(pExpressionLogic->v_ExpressionLogicEnum){
						case 0:
							stringLength += stringLengthRelationG;
						break;
						case 1:
							stringLength += stringLengthRelationGE;
						break;
						case 2:
							stringLength += stringLengthRelationEQ;
						break;
						case 3:
							stringLength += stringLengthRelationNE;
						break;
						case 4:
							stringLength += stringLengthRelationLE;
						break;
						case 5:
							stringLength += stringLengthRelationL;
						break;
						default:
							stringLength += stringLengthRelation;
						break;
					}
					switch(pPVS->v_Value.value.vartype & OV_VT_KSMASK){
						case OV_VT_BOOL:
							ov_string_print(&tmpString, "%d", pPVS->v_Value.value.valueunion.val_bool);
							stringLength += stringLengthValue + strlen(tmpString);
							stringLength += stringLengthValueTypeB;
							break;
						case OV_VT_SINGLE:
							ov_string_print(&tmpString, "%f", pPVS->v_Value.value.valueunion.val_single);
							stringLength += stringLengthValue + strlen(tmpString);
							stringLength += stringLengthValueTypeN;
							break;
						case OV_VT_DOUBLE:
							ov_string_print(&tmpString, "%lf", pPVS->v_Value.value.valueunion.val_double);
							stringLength += stringLengthValue + strlen(tmpString);
							stringLength += stringLengthValueTypeN;
							break;
						case OV_VT_INT:
							ov_string_print(&tmpString, "%d", pPVS->v_Value.value.valueunion.val_int);
							stringLength += stringLengthValue + strlen(tmpString);
							stringLength += stringLengthValueTypeN;
							break;
						case OV_VT_UINT:
							ov_string_print(&tmpString, "%u", pPVS->v_Value.value.valueunion.val_uint);
							stringLength += stringLengthValue + strlen(tmpString);
							stringLength += stringLengthValueTypeN;
							break;
						case OV_VT_STRING:
							if (pPVS->v_Value.value.valueunion.val_string != NULL){
								ov_string_print(&tmpString, "%s", pPVS->v_Value.value.valueunion.val_string);
								stringLength += stringLengthValue + strlen(tmpString);
							}else
								stringLength += stringLengthValue;

							stringLength += stringLengthValueTypeT;
							break;
						default:
							stringLength += stringLengthValue;
							stringLength += stringLengthValueType;
							break;
					}
					ov_string_print(&tmpString, "%s", pSubmodel->v_identifier);
					stringLength += stringLengthSubModel + strlen(tmpString);
					ov_string_setvalue(&tmpString, NULL);
				}
			}
		}
	}

	first = TRUE;
	OV_STRING pc = NULL;
	statementString = ov_database_malloc(stringLength+1);
	pc = statementString;
	pc += sprintf(pc, "\"statements\": [");
	Ov_ForEachChildEx(ov_containment, &paas->p_Header, pSubmodel, openaas_SubModel){
		Ov_ForEachChildEx(ov_containment, pSubmodel, pPVSList, propertyValueStatement_PropertyValueStatementList){
			Ov_ForEachChild(ov_containment, pPVSList, pObject){
				if (Ov_CanCastTo(propertyValueStatement_CarrierId, pObject)){
					pCarrierID = Ov_DynamicPtrCast(propertyValueStatement_CarrierId, pObject);
				}
				if (Ov_CanCastTo(propertyValueStatement_PropertyId, pObject)){
					pPropertyID = Ov_DynamicPtrCast(propertyValueStatement_PropertyId, pObject);
				}
				if (Ov_CanCastTo(propertyValueStatement_ExpressionSemantic, pObject)){
					pExpressionSemantic = Ov_DynamicPtrCast(propertyValueStatement_ExpressionSemantic, pObject);
				}
				if (Ov_CanCastTo(propertyValueStatement_ExpressionLogic, pObject)){
					pExpressionLogic = Ov_DynamicPtrCast(propertyValueStatement_ExpressionLogic, pObject);
				}
				if (Ov_CanCastTo(propertyValueStatement_PropertyValueStatement, pObject)){
					pPVS = Ov_DynamicPtrCast(propertyValueStatement_PropertyValueStatement, pObject);
					Ov_ForEachChild(ov_containment, pPVS, pObject2){
						if (Ov_CanCastTo(propertyValueStatement_CarrierId, pObject2)){
							pCarrierID = Ov_DynamicPtrCast(propertyValueStatement_CarrierId, pObject2);
						}
						if (Ov_CanCastTo(propertyValueStatement_PropertyId, pObject2)){
							pPropertyID = Ov_DynamicPtrCast(propertyValueStatement_PropertyId, pObject2);
						}
						if (Ov_CanCastTo(propertyValueStatement_ExpressionSemantic, pObject2)){
							pExpressionSemantic = Ov_DynamicPtrCast(propertyValueStatement_ExpressionSemantic, pObject2);
						}
						if (Ov_CanCastTo(propertyValueStatement_ExpressionLogic, pObject2)){
							pExpressionLogic = Ov_DynamicPtrCast(propertyValueStatement_ExpressionLogic, pObject2);
						}
					}
					if (first == TRUE){
						pc += sprintf(pc, "{\"CarrierID\":\"%i,%s\"", pCarrierID->v_IdType, pCarrierID->v_IdSpec);
						first = FALSE;
					}else{
						pc += sprintf(pc, ",{\"CarrierID\":\"%i,%s\"", pCarrierID->v_IdType, pCarrierID->v_IdSpec);
					}
					pc += sprintf(pc, ",\"PropertyID\":\"%i,%s\"", pPropertyID->v_IdType, pPropertyID->v_IdSpec);

					switch(pExpressionSemantic->v_ExpressionSemanticEnum){
						case 0:
							pc += sprintf(pc, ",\"ExpressionSemantic\":\"Assurance\"");
						break;
						case 1:
							pc += sprintf(pc, ",\"ExpressionSemantic\":\"Setting\"");
						break;
						case 2:
							pc += sprintf(pc, ",\"ExpressionSemantic\":\"Measurement\"");
						break;
						case 3:
							pc += sprintf(pc, ",\"ExpressionSemantic\":\"Requirement\"");
						break;
						default:
							pc += sprintf(pc, ",\"ExpressionSemantic\":\"\"");
						break;
					}

					switch(pExpressionLogic->v_ExpressionLogicEnum){
						case 0:
							pc += sprintf(pc, ",\"Relation\":\">\"");
						break;
						case 1:
							pc += sprintf(pc, ",\"Relation\":\">=\"");
						break;
						case 2:
							pc += sprintf(pc, ",\"Relation\":\"==\"");
						break;
						case 3:
							pc += sprintf(pc, ",\"Relation\":\"!=\"");
						break;
						case 4:
							pc += sprintf(pc, ",\"Relation\":\"<=\"");
						break;
						case 5:
							pc += sprintf(pc, ",\"Relation\":\"<\"");
						break;
						default:
							pc += sprintf(pc, ",\"Relation\":\"\"");
						break;
					}
					switch(pPVS->v_Value.value.vartype & OV_VT_KSMASK){
						case OV_VT_BOOL:
							pc += sprintf(pc, ",\"Value\":\"%d\"", pPVS->v_Value.value.valueunion.val_bool);
							pc += sprintf(pc, ",\"ValueType\":\"Boolean\"");
							break;
						case OV_VT_SINGLE:
							pc += sprintf(pc, ",\"Value\":\"%f\"", pPVS->v_Value.value.valueunion.val_single);
							pc += sprintf(pc, ",\"ValueType\":\"Numeric\"");
							break;
						case OV_VT_DOUBLE:
							pc += sprintf(pc, ",\"Value\":\"%lf\"", pPVS->v_Value.value.valueunion.val_double);
							pc += sprintf(pc, ",\"ValueType\":\"Numeric\"");
							break;
						case OV_VT_INT:
							pc += sprintf(pc, ",\"Value\":\"%d\"", pPVS->v_Value.value.valueunion.val_int);
							pc += sprintf(pc, ",\"ValueType\":\"Numeric\"");
							break;
						case OV_VT_UINT:
							pc += sprintf(pc, ",\"Value\":\"%u\"", pPVS->v_Value.value.valueunion.val_uint);
							pc += sprintf(pc, ",\"ValueType\":\"Numeric\"");
							break;
						case OV_VT_STRING:
							if (pPVS->v_Value.value.valueunion.val_string != NULL)
								pc += sprintf(pc, ",\"Value\":\"%s\"", pPVS->v_Value.value.valueunion.val_string);
							else
								pc += sprintf(pc, ",\"Value\":\"\"");

							pc += sprintf(pc, ",\"ValueType\":\"Text\"");
							break;
						default:
							pc += sprintf(pc, ",\"Value\":\"\"");
							pc += sprintf(pc, ",\"ValueType\":\"\"");
							break;
					}
					pc += sprintf(pc, ",\"SubModel\":\"%s\"}", pSubmodel->v_identifier);
				}
			}
		}
	}
	Ov_ForEachChildEx(ov_containment, &paas->p_Body, pSubmodel, openaas_SubModel){
		Ov_ForEachChildEx(ov_containment, pSubmodel, pPVSList, propertyValueStatement_PropertyValueStatementList){
			Ov_ForEachChild(ov_containment, pPVSList, pObject){
				if (Ov_CanCastTo(propertyValueStatement_CarrierId, pObject)){
					pCarrierID = Ov_DynamicPtrCast(propertyValueStatement_CarrierId, pObject);
				}
				if (Ov_CanCastTo(propertyValueStatement_PropertyId, pObject)){
					pPropertyID = Ov_DynamicPtrCast(propertyValueStatement_PropertyId, pObject);
				}
				if (Ov_CanCastTo(propertyValueStatement_ExpressionSemantic, pObject)){
					pExpressionSemantic = Ov_DynamicPtrCast(propertyValueStatement_ExpressionSemantic, pObject);
				}
				if (Ov_CanCastTo(propertyValueStatement_ExpressionLogic, pObject)){
					pExpressionLogic = Ov_DynamicPtrCast(propertyValueStatement_ExpressionLogic, pObject);
				}
				if (Ov_CanCastTo(propertyValueStatement_PropertyValueStatement, pObject)){
					pPVS = Ov_DynamicPtrCast(propertyValueStatement_PropertyValueStatement, pObject);
					Ov_ForEachChild(ov_containment, pPVS, pObject2){
						if (Ov_CanCastTo(propertyValueStatement_CarrierId, pObject2)){
							pCarrierID = Ov_DynamicPtrCast(propertyValueStatement_CarrierId, pObject2);
						}
						if (Ov_CanCastTo(propertyValueStatement_PropertyId, pObject2)){
							pPropertyID = Ov_DynamicPtrCast(propertyValueStatement_PropertyId, pObject2);
						}
						if (Ov_CanCastTo(propertyValueStatement_ExpressionSemantic, pObject2)){
							pExpressionSemantic = Ov_DynamicPtrCast(propertyValueStatement_ExpressionSemantic, pObject2);
						}
						if (Ov_CanCastTo(propertyValueStatement_ExpressionLogic, pObject2)){
							pExpressionLogic = Ov_DynamicPtrCast(propertyValueStatement_ExpressionLogic, pObject2);
						}
					}
					if (first == TRUE){
						pc += sprintf(pc, "{\"CarrierID\":\"%i,%s\"", pCarrierID->v_IdType, pCarrierID->v_IdSpec);
						first = FALSE;
					}else{
						pc += sprintf(pc, ",{\"CarrierID\":\"%i,%s\"", pCarrierID->v_IdType, pCarrierID->v_IdSpec);
					}
					pc += sprintf(pc, ",\"PropertyID\":\"%i,%s\"", pPropertyID->v_IdType, pPropertyID->v_IdSpec);

					switch(pExpressionSemantic->v_ExpressionSemanticEnum){
						case 0:
							pc += sprintf(pc, ",\"ExpressionSemantic\":\"Assurance\"");
						break;
						case 1:
							pc += sprintf(pc, ",\"ExpressionSemantic\":\"Setting\"");
						break;
						case 2:
							pc += sprintf(pc, ",\"ExpressionSemantic\":\"Measurement\"");
						break;
						case 3:
							pc += sprintf(pc, ",\"ExpressionSemantic\":\"Requirement\"");
						break;
						default:
							pc += sprintf(pc, ",\"ExpressionSemantic\":\"\"");
						break;
					}

					switch(pExpressionLogic->v_ExpressionLogicEnum){
						case 0:
							pc += sprintf(pc, ",\"Relation\":\">\"");
						break;
						case 1:
							pc += sprintf(pc, ",\"Relation\":\">=\"");
						break;
						case 2:
							pc += sprintf(pc, ",\"Relation\":\"==\"");
						break;
						case 3:
							pc += sprintf(pc, ",\"Relation\":\"!=\"");
						break;
						case 4:
							pc += sprintf(pc, ",\"Relation\":\"<=\"");
						break;
						case 5:
							pc += sprintf(pc, ",\"Relation\":\"<\"");
						break;
						default:
							pc += sprintf(pc, ",\"Relation\":\"\"");
						break;
					}
					switch(pPVS->v_Value.value.vartype & OV_VT_KSMASK){
						case OV_VT_BOOL:
							pc += sprintf(pc, ",\"Value\":\"%d\"", pPVS->v_Value.value.valueunion.val_bool);
							pc += sprintf(pc, ",\"ValueType\":\"Boolean\"");
							break;
						case OV_VT_SINGLE:
							pc += sprintf(pc, ",\"Value\":\"%f\"", pPVS->v_Value.value.valueunion.val_single);
							pc += sprintf(pc, ",\"ValueType\":\"Numeric\"");
							break;
						case OV_VT_DOUBLE:
							pc += sprintf(pc, ",\"Value\":\"%lf\"", pPVS->v_Value.value.valueunion.val_double);
							pc += sprintf(pc, ",\"ValueType\":\"Numeric\"");
							break;
						case OV_VT_INT:
							pc += sprintf(pc, ",\"Value\":\"%d\"", pPVS->v_Value.value.valueunion.val_int);
							pc += sprintf(pc, ",\"ValueType\":\"Numeric\"");
							break;
						case OV_VT_UINT:
							pc += sprintf(pc, ",\"Value\":\"%u\"", pPVS->v_Value.value.valueunion.val_uint);
							pc += sprintf(pc, ",\"ValueType\":\"Numeric\"");
							break;
						case OV_VT_STRING:
							if (pPVS->v_Value.value.valueunion.val_string != NULL)
								pc += sprintf(pc, ",\"Value\":\"%s\"", pPVS->v_Value.value.valueunion.val_string);
							else
								pc += sprintf(pc, ",\"Value\":\"\"");

							pc += sprintf(pc, ",\"ValueType\":\"Text\"");
							break;
						default:
							pc += sprintf(pc, ",\"Value\":\"\"");
							pc += sprintf(pc, ",\"ValueType\":\"\"");
							break;
					}
					pc += sprintf(pc, ",\"SubModel\":\"%s\"}", pSubmodel->v_identifier);
				}
			}
		}
	}
	pc += sprintf(pc, "]");
	return statementString;
}

