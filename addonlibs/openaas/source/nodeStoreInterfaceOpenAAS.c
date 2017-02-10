/* * nodeStoreInterface.c
 *
 *  Created on: 06.12.2016
 *      Author: ubuntu
 */

#ifndef OV_COMPILE_LIBRARY_openaas
#define OV_COMPILE_LIBRARY_openaas
#endif

#include "openaas.h"
#include "libov/ov_macros.h"
#include "ksbase.h"
#include "opcua.h"
#include "opcua_helpers.h"
#include "NoneTicketAuthenticator.h"
#include "libov/ov_path.h"
#include "libov/ov_memstack.h"
#include "ks_logfile.h"
#include "ua_openaas_generated.h"
#include "ua_openaas_generated_handling.h"
#include "openaas_helpers.h"


OV_DLLFNCEXPORT UA_StatusCode openaas_nodeStoreFunctions_MethodCallbackStandard(void *methodHandle, const UA_NodeId *objectId,
                     const UA_NodeId *sessionId, void *sessionHandle,
                     size_t inputSize, const UA_Variant *input,
                     size_t outputSize, UA_Variant *output) {
	OV_RESULT resultOV = OV_ERR_OK;
	AASStatusCode result = AASSTATUSCODE_GOOD;
	OV_STRING funcName = (OV_STRING)methodHandle;

	if (ov_string_compare(funcName, "getAASNodeId") == OV_STRCMP_EQUAL){
		UA_Identification *tmpUAAASId = (UA_Identification*)(input[0].data);

		IdentificationType tmpOVAASId;
		IdentificationType_init(&tmpOVAASId);
		copyOPCUAStringToOV(tmpUAAASId->idSpec, &(tmpOVAASId.IdSpec));
		tmpOVAASId.IdType = tmpUAAASId->idType;

		UA_NodeId tmpNodeId;
		UA_NodeId_init(&tmpNodeId);
		tmpNodeId = openaas_modelmanager_getAASNodeId(tmpOVAASId);

		UA_Variant_setScalarCopy(&output[0], &tmpNodeId, &UA_TYPES[UA_TYPES_NODEID]);
		UA_NodeId_deleteMembers(&tmpNodeId);
		IdentificationType_deleteMembers(&tmpOVAASId);
		resultOV = OV_ERR_OK;
	}else if (ov_string_compare(funcName, "createAAS") == OV_STRCMP_EQUAL){
		UA_Identification *tmpUAAASId = (UA_Identification*)(input[0].data);
		UA_String *tmpUAName = (UA_String*)(input[1].data);
		UA_Identification *tmpUAAssetId = (UA_Identification*)(input[2].data);

		IdentificationType tmpOVAASId;
		IdentificationType_init(&tmpOVAASId);
		copyOPCUAStringToOV(tmpUAAASId->idSpec, &(tmpOVAASId.IdSpec));
		tmpOVAASId.IdType = tmpUAAASId->idType;

		OV_STRING tmpOVName = NULL;
		copyOPCUAStringToOV(*tmpUAName, &tmpOVName);

		IdentificationType tmpOVAssetId;
		IdentificationType_init(&tmpOVAssetId);
		copyOPCUAStringToOV(tmpUAAssetId->idSpec, &(tmpOVAssetId.IdSpec));
		tmpOVAssetId.IdType = tmpUAAssetId->idType;

		result = openaas_modelmanager_createAAS(tmpOVAASId, tmpOVName, tmpOVAssetId);

		UA_Variant_setScalarCopy(&output[0], &result, &UA_TYPES[UA_TYPES_STATUSCODE]);
		IdentificationType_deleteMembers(&tmpOVAASId);
		IdentificationType_deleteMembers(&tmpOVAssetId);
		ov_database_free(tmpOVName);
		resultOV = OV_ERR_OK;
	}else if (ov_string_compare(funcName, "deleteAAS") == OV_STRCMP_EQUAL){
		UA_Identification *tmpUAAASId = (UA_Identification*)(input[0].data);

		IdentificationType tmpOVAASId;
		IdentificationType_init(&tmpOVAASId);
		copyOPCUAStringToOV(tmpUAAASId->idSpec, &(tmpOVAASId.IdSpec));
		tmpOVAASId.IdType = tmpUAAASId->idType;

		result = openaas_modelmanager_deleteAAS(tmpOVAASId);

		UA_Variant_setScalarCopy(&output[0], &result, &UA_TYPES[UA_TYPES_STATUSCODE]);
		IdentificationType_deleteMembers(&tmpOVAASId);
		resultOV = OV_ERR_OK;
	}else if (ov_string_compare(funcName, "createPVSL") == OV_STRCMP_EQUAL){
		UA_Identification *tmpUAAASId = (UA_Identification*)(input[0].data);
		UA_String *tmpUAPVSLName = (UA_String*)(input[1].data);
		UA_Identification *tmpUACarrier = (UA_Identification*)(input[2].data);
		UA_Identification *tmpUACreatingInstance = (UA_Identification*)(input[2].data);

		IdentificationType tmpOVAASId;
		IdentificationType_init(&tmpOVAASId);
		copyOPCUAStringToOV(tmpUAAASId->idSpec, &(tmpOVAASId.IdSpec));
		tmpOVAASId.IdType = tmpUAAASId->idType;

		OV_STRING tmpOVPVSLName = NULL;
		copyOPCUAStringToOV(*tmpUAPVSLName, &tmpOVPVSLName);

		IdentificationType tmpOVCarrier;
		IdentificationType_init(&tmpOVCarrier);
		copyOPCUAStringToOV(tmpUACarrier->idSpec, &(tmpOVCarrier.IdSpec));
		tmpOVCarrier.IdType = tmpUACarrier->idType;

		IdentificationType tmpOVCreatingInstance;
		IdentificationType_init(&tmpOVCreatingInstance);
		copyOPCUAStringToOV(tmpUACreatingInstance->idSpec, &(tmpOVCreatingInstance.IdSpec));
		tmpOVCreatingInstance.IdType = tmpUACreatingInstance->idType;

		result = openaas_modelmanager_createPVSL(tmpOVAASId, tmpOVPVSLName, tmpOVCarrier, tmpOVCreatingInstance);

		UA_Variant_setScalarCopy(&output[0], &result, &UA_TYPES[UA_TYPES_STATUSCODE]);
		IdentificationType_deleteMembers(&tmpOVAASId);
		ov_database_free(tmpOVPVSLName);
		IdentificationType_deleteMembers(&tmpOVCarrier);
		resultOV = OV_ERR_OK;
	}else if (ov_string_compare(funcName, "deletePVSL") == OV_STRCMP_EQUAL){
		UA_Identification *tmpUAAASId = (UA_Identification*)(input[0].data);
		UA_String *tmpUAPVSLName = (UA_String*)(input[1].data);

		IdentificationType tmpOVAASId;
		IdentificationType_init(&tmpOVAASId);
		copyOPCUAStringToOV(tmpUAAASId->idSpec, &(tmpOVAASId.IdSpec));
		tmpOVAASId.IdType = tmpUAAASId->idType;

		OV_STRING tmpOVPVSLName = NULL;
		copyOPCUAStringToOV(*tmpUAPVSLName, &tmpOVPVSLName);

		result = openaas_modelmanager_deletePVSL(tmpOVAASId, tmpOVPVSLName);

		UA_Variant_setScalarCopy(&output[0], &result, &UA_TYPES[UA_TYPES_STATUSCODE]);
		IdentificationType_deleteMembers(&tmpOVAASId);
		ov_database_free(tmpOVPVSLName);
		resultOV = OV_ERR_OK;
	}else if (ov_string_compare(funcName, "createLCE") == OV_STRCMP_EQUAL){
		UA_Identification *tmpUAAASId = (UA_Identification*)(input[0].data);
		UA_Identification *tmpUACreatingInstance = (UA_Identification*)(input[1].data);
		UA_Identification *tmpUAWritingInstance = (UA_Identification*)(input[2].data);
		UA_String *tmpUAEventClass = (UA_String*)(input[3].data);
		UA_String *tmpUASubject = (UA_String*)(input[4].data);
		UA_DataValue *tmpUAValue = (UA_DataValue*)(input[5].data);

		IdentificationType tmpOVAASId;
		IdentificationType_init(&tmpOVAASId);
		copyOPCUAStringToOV(tmpUAAASId->idSpec, &(tmpOVAASId.IdSpec));
		tmpOVAASId.IdType = tmpUAAASId->idType;

		LifeCycleEntry lce;
		LifeCycleEntry_init(&lce);

		copyOPCUAStringToOV(tmpUACreatingInstance->idSpec, &(lce.creatingInstance.IdSpec));
		lce.creatingInstance.IdType = tmpUACreatingInstance->idType;

		copyOPCUAStringToOV(*tmpUAEventClass, &lce.eventClass);

		copyOPCUAStringToOV(*tmpUASubject, &lce.subject);

		copyOPCUAStringToOV(tmpUAWritingInstance->idSpec, &(lce.writingInstance.IdSpec));
		lce.writingInstance.IdType = tmpUAWritingInstance->idType;

		ov_VariantToAny(&(tmpUAValue->value), &lce.data.Value);
		lce.data.TimeStamp = ov_1601nsTimeToOvTime(tmpUAValue->sourceTimestamp);

		result = openaas_modelmanager_createLCE(tmpOVAASId, lce);

		UA_Variant_setScalarCopy(&output[0], &result, &UA_TYPES[UA_TYPES_STATUSCODE]);
		IdentificationType_deleteMembers(&tmpOVAASId);
		LifeCycleEntry_deleteMembers(&lce);
		resultOV = OV_ERR_OK;
	}else if (ov_string_compare(funcName, "deleteLCE") == OV_STRCMP_EQUAL){
		UA_Identification *tmpUAAASId = (UA_Identification*)(input[0].data);
		UA_UInt64 *tmpUALCEId = (UA_UInt64*)(input[1].data);

		IdentificationType tmpOVAASId;
		IdentificationType_init(&tmpOVAASId);
		copyOPCUAStringToOV(tmpUAAASId->idSpec, &(tmpOVAASId.IdSpec));
		tmpOVAASId.IdType = tmpUAAASId->idType;

		OV_UINT64 tmpOVLCEId;
		tmpOVLCEId = *tmpUALCEId;

		result = openaas_modelmanager_deleteLCE(tmpOVAASId, tmpOVLCEId);

		UA_Variant_setScalarCopy(&output[0], &result, &UA_TYPES[UA_TYPES_STATUSCODE]);
		IdentificationType_deleteMembers(&tmpOVAASId);
		resultOV = OV_ERR_OK;
	}else if (ov_string_compare(funcName, "getLCE") == OV_STRCMP_EQUAL){
		UA_Identification *tmpUAAASId = (UA_Identification*)(input[0].data);
		UA_UInt64 *tmpUALCEId = (UA_UInt64*)(input[1].data);

		IdentificationType tmpOVAASId;
		IdentificationType_init(&tmpOVAASId);
		copyOPCUAStringToOV(tmpUAAASId->idSpec, &(tmpOVAASId.IdSpec));
		tmpOVAASId.IdType = tmpUAAASId->idType;

		OV_UINT64 tmpOVLCEId;
		tmpOVLCEId = *tmpUALCEId;

		LifeCycleEntry lce;
		LifeCycleEntry_init(&lce);

		result = openaas_modelmanager_getLCE(tmpOVAASId, tmpOVLCEId, &lce);

		UA_Identification tmpUACreatingInstance;
		UA_Identification_init(&tmpUACreatingInstance);
		copyOvStringToOPCUA(lce.creatingInstance.IdSpec, &tmpUACreatingInstance.idSpec);
		tmpUACreatingInstance.idType = lce.creatingInstance.IdType;

		UA_Identification tmpUAWritingInstance;
		UA_Identification_init(&tmpUAWritingInstance);
		copyOvStringToOPCUA(lce.writingInstance.IdSpec, &tmpUAWritingInstance.idSpec);
		tmpUAWritingInstance.idType = lce.writingInstance.IdType;

		UA_String tmpUAEventClass;
		UA_String_init(&tmpUAEventClass);
		copyOvStringToOPCUA(lce.eventClass, &tmpUAEventClass);

		UA_String tmpUASubject;
		UA_String_init(&tmpUASubject);
		copyOvStringToOPCUA(lce.subject, &tmpUASubject);

		UA_DataValue tmpUAValue;
		UA_DataValue_init(&tmpUAValue);
		ov_AnyToVariant(&lce.data.Value, &tmpUAValue.value);
		tmpUAValue.sourceTimestamp = ov_ovTimeTo1601nsTime(lce.data.TimeStamp);

		UA_Variant_setScalarCopy(&output[0], &tmpUACreatingInstance, &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
		UA_Variant_setScalarCopy(&output[1], &tmpUAWritingInstance, &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
		UA_Variant_setScalarCopy(&output[2], &tmpUAEventClass, &UA_TYPES[UA_TYPES_STRING]);
		UA_Variant_setScalarCopy(&output[3], &tmpUASubject, &UA_TYPES[UA_TYPES_STRING]);
		UA_Variant_setScalarCopy(&output[4], &tmpUAValue, &UA_TYPES[UA_TYPES_DATAVALUE]);
		UA_Variant_setScalarCopy(&output[5], &result, &UA_TYPES[UA_TYPES_STATUSCODE]);

		UA_Identification_deleteMembers(&tmpUACreatingInstance);
		UA_Identification_deleteMembers(&tmpUAWritingInstance);
		UA_String_deleteMembers(&tmpUAEventClass);
		UA_String_deleteMembers(&tmpUASubject);
		UA_DataValue_deleteMembers(&tmpUAValue);
		IdentificationType_deleteMembers(&tmpOVAASId);
		LifeCycleEntry_deleteMembers(&lce);
		resultOV = OV_ERR_OK;
	}else if (ov_string_compare(funcName, "getLastLCEs") == OV_STRCMP_EQUAL){
		UA_Identification *tmpUAAASId = (UA_Identification*)(input[0].data);
		UA_UInt32 *tmpUACount = (UA_UInt32*)(input[1].data);

		IdentificationType tmpOVAASId;
		IdentificationType_init(&tmpOVAASId);
		copyOPCUAStringToOV(tmpUAAASId->idSpec, &(tmpOVAASId.IdSpec));
		tmpOVAASId.IdType = tmpUAAASId->idType;

		OV_UINT tmpOVCount;
		tmpOVCount = *tmpUACount;

		LifeCycleEntry *lce = NULL;

		OV_UINT arrayDimension;

		result = openaas_modelmanager_getLastLCEs(tmpOVAASId, tmpOVCount, &lce, &arrayDimension);

		UA_LifeCycleEntry *tmpUALifeCycleEntry = UA_Array_new(arrayDimension,&UA_OPENAAS[UA_OPENAAS_LIFECYCLEENTRY]);

		for (OV_UINT i = 0; i < arrayDimension; i++){
			UA_LifeCycleEntry_init(&tmpUALifeCycleEntry[i]);
			copyOvStringToOPCUA(lce[i].creatingInstance.IdSpec, &tmpUALifeCycleEntry[i].creatingInstance.idSpec);
			tmpUALifeCycleEntry[i].creatingInstance.idType = lce[i].creatingInstance.IdType;
			copyOvStringToOPCUA(lce[i].writingInstance.IdSpec, &tmpUALifeCycleEntry[i].writingInstance.idSpec);
			tmpUALifeCycleEntry[i].writingInstance.idType = lce[i].writingInstance.IdType;
			copyOvStringToOPCUA(lce[i].eventClass, &tmpUALifeCycleEntry[i].eventClass);
			copyOvStringToOPCUA(lce[i].subject, &tmpUALifeCycleEntry[i].subject);
			ov_AnyToVariant(&lce[i].data.Value, &tmpUALifeCycleEntry[i].data.value);
			tmpUALifeCycleEntry[i].data.sourceTimestamp = ov_ovTimeTo1601nsTime(lce[i].data.TimeStamp);
			tmpUALifeCycleEntry[i].id = lce[i].lceId;
			}

		UA_Variant_setScalarCopy(&output[0], &result, &UA_TYPES[UA_TYPES_STATUSCODE]);
		UA_Variant_setArrayCopy(&output[1], tmpUALifeCycleEntry, arrayDimension, &UA_OPENAAS[UA_OPENAAS_LIFECYCLEENTRY]);
		UA_Variant_setScalarCopy(&output[2], &arrayDimension, &UA_TYPES[UA_TYPES_UINT32]);

		for (OV_UINT i = 0; i < arrayDimension; i++){
			UA_LifeCycleEntry_deleteMembers(&tmpUALifeCycleEntry[i]);
			LifeCycleEntry_deleteMembers(&lce[i]);
		}
		ov_database_free(lce);
		UA_free(tmpUALifeCycleEntry);
		IdentificationType_deleteMembers(&tmpOVAASId);
		resultOV = OV_ERR_OK;
	}else if (ov_string_compare(funcName, "setLCE") == OV_STRCMP_EQUAL){
		UA_Identification *tmpUAAASId = (UA_Identification*)(input[0].data);
		UA_UInt64 *tmpUALCEId = (UA_UInt64*)(input[1].data);
		UA_Identification *tmpUACreatingInstance = (UA_Identification*)(input[2].data);
		UA_Identification *tmpUAWritingInstance = (UA_Identification*)(input[3].data);
		UA_String *tmpUAEventClass = (UA_String*)(input[4].data);
		UA_String *tmpUASubject = (UA_String*)(input[5].data);
		UA_DataValue *tmpUAValue = (UA_DataValue*)(input[6].data);

		IdentificationType tmpOVAASId;
		IdentificationType_init(&tmpOVAASId);
		copyOPCUAStringToOV(tmpUAAASId->idSpec, &(tmpOVAASId.IdSpec));
		tmpOVAASId.IdType = tmpUAAASId->idType;

		LifeCycleEntry lce;
		LifeCycleEntry_init(&lce);

		lce.lceId = *tmpUALCEId;

		copyOPCUAStringToOV(tmpUACreatingInstance->idSpec, &(lce.creatingInstance.IdSpec));
		lce.creatingInstance.IdType = tmpUACreatingInstance->idType;

		copyOPCUAStringToOV(*tmpUAEventClass, &lce.eventClass);

		copyOPCUAStringToOV(*tmpUASubject, &lce.subject);

		copyOPCUAStringToOV(tmpUAWritingInstance->idSpec, &(lce.writingInstance.IdSpec));
		lce.writingInstance.IdType = tmpUAWritingInstance->idType;

		ov_VariantToAny(&(tmpUAValue->value), &lce.data.Value);
		lce.data.TimeStamp = ov_1601nsTimeToOvTime(tmpUAValue->sourceTimestamp);

		result = openaas_modelmanager_setLCE(tmpOVAASId, lce);

		UA_Variant_setScalarCopy(&output[0], &result, &UA_TYPES[UA_TYPES_STATUSCODE]);
		IdentificationType_deleteMembers(&tmpOVAASId);
		LifeCycleEntry_deleteMembers(&lce);
		resultOV = OV_ERR_OK;
	}else if (ov_string_compare(funcName, "createPVS") == OV_STRCMP_EQUAL){
		UA_Identification *tmpUAAASId = (UA_Identification*)(input[0].data);
		UA_String *tmpUAPVSLName = (UA_String*)(input[1].data);
		UA_String *tmpUAPVSName = (UA_String*)(input[2].data);
		UA_ExpressionLogicEnum *tmpUAExpressionLogic = (UA_ExpressionLogicEnum*)(input[3].data);
		UA_ExpressionSemanticEnum *tmpUAExpressionSemantic = (UA_ExpressionSemanticEnum*)(input[4].data);
		UA_DataValue *tmpUAValue = (UA_DataValue*)(input[5].data);
		UA_String *tmpUAUnit = (UA_String*)(input[6].data);
		UA_Identification *tmpUAID = (UA_Identification*)(input[7].data);
		UA_ViewEnum *tmpUAView = (UA_ViewEnum*)(input[8].data);
		UA_VisibilityEnum *tmpUAVisibility = (UA_VisibilityEnum*)(input[9].data);

		IdentificationType tmpOVAASId;
		IdentificationType_init(&tmpOVAASId);
		copyOPCUAStringToOV(tmpUAAASId->idSpec, &(tmpOVAASId.IdSpec));
		tmpOVAASId.IdType = tmpUAAASId->idType;

		OV_STRING tmpOVPVSLName = NULL;
		copyOPCUAStringToOV(*tmpUAPVSLName, &tmpOVPVSLName);

		PropertyValueStatement pvs;
		PropertyValueStatement_init(&pvs);

		copyOPCUAStringToOV(*tmpUAPVSName, &pvs.pvsName);

		pvs.ExpressionLogic = *tmpUAExpressionLogic;

		pvs.ExpressionSemantic = *tmpUAExpressionSemantic;

		ov_VariantToAny(&(tmpUAValue->value), &pvs.value.Value);
		pvs.value.TimeStamp = ov_1601nsTimeToOvTime(tmpUAValue->sourceTimestamp);

		copyOPCUAStringToOV(*tmpUAUnit, &pvs.unit);

		copyOPCUAStringToOV(tmpUAID->idSpec, &(pvs.ID.IdSpec));
		pvs.ID.IdType = tmpUAID->idType;

		pvs.view = *tmpUAView;

		pvs.Visibility = *tmpUAVisibility;

		result = openaas_modelmanager_createPVS(tmpOVAASId, tmpOVPVSLName, pvs);

		UA_Variant_setScalarCopy(&output[0], &result, &UA_TYPES[UA_TYPES_STATUSCODE]);
		IdentificationType_deleteMembers(&tmpOVAASId);
		ov_database_free(tmpOVPVSLName);
		PropertyValueStatement_deleteMembers(&pvs);
		resultOV = OV_ERR_OK;
	}else if (ov_string_compare(funcName, "deletePVS") == OV_STRCMP_EQUAL){
		UA_Identification *tmpUAAASId = (UA_Identification*)(input[0].data);
		UA_String *tmpUAPVSLName = (UA_String*)(input[1].data);
		UA_String *tmpUAPVSName = (UA_String*)(input[2].data);

		IdentificationType tmpOVAASId;
		IdentificationType_init(&tmpOVAASId);
		copyOPCUAStringToOV(tmpUAAASId->idSpec, &(tmpOVAASId.IdSpec));
		tmpOVAASId.IdType = tmpUAAASId->idType;

		OV_STRING tmpOVPVSLName = NULL;
		copyOPCUAStringToOV(*tmpUAPVSLName, &tmpOVPVSLName);

		OV_STRING tmpOVPVSName = NULL;
		copyOPCUAStringToOV(*tmpUAPVSName, &tmpOVPVSName);

		result = openaas_modelmanager_deletePVS(tmpOVAASId, tmpOVPVSLName, tmpOVPVSName);

		UA_Variant_setScalarCopy(&output[0], &result, &UA_TYPES[UA_TYPES_STATUSCODE]);
		IdentificationType_deleteMembers(&tmpOVAASId);
		ov_database_free(tmpOVPVSLName);
		ov_database_free(tmpOVPVSName);
		resultOV = OV_ERR_OK;

	}else if (ov_string_compare(funcName, "getPVS") == OV_STRCMP_EQUAL){
		UA_Identification *tmpUAAASId = (UA_Identification*)(input[0].data);
		UA_String *tmpUAPVSLName = (UA_String*)(input[1].data);
		UA_String *tmpUAPVSName = (UA_String*)(input[2].data);

		IdentificationType tmpOVAASId;
		IdentificationType_init(&tmpOVAASId);
		copyOPCUAStringToOV(tmpUAAASId->idSpec, &(tmpOVAASId.IdSpec));
		tmpOVAASId.IdType = tmpUAAASId->idType;

		OV_STRING tmpOVPVSLName = NULL;
		copyOPCUAStringToOV(*tmpUAPVSLName, &tmpOVPVSLName);

		OV_STRING tmpOVPVSName = NULL;
		copyOPCUAStringToOV(*tmpUAPVSName, &tmpOVPVSName);

		PropertyValueStatement pvs;
		PropertyValueStatement_init(&pvs);

		result = openaas_modelmanager_getPVS(tmpOVAASId, tmpOVPVSLName, tmpOVPVSName, &pvs);

		UA_ExpressionLogicEnum tmpUAExpressionLogic;
		tmpUAExpressionLogic = pvs.ExpressionLogic;

		UA_ExpressionSemanticEnum tmpUAExpressionSemantic;
		tmpUAExpressionSemantic = pvs.ExpressionSemantic;

		UA_DataValue tmpUAValue;
		UA_DataValue_init(&tmpUAValue);
		ov_AnyToVariant(&pvs.value.Value, &tmpUAValue.value);
		tmpUAValue.sourceTimestamp = ov_ovTimeTo1601nsTime(pvs.value.TimeStamp);

		UA_String tmpUAUnit;
		UA_String_init(&tmpUAUnit);
		copyOvStringToOPCUA(pvs.unit, &tmpUAUnit);

		UA_Identification tmpUAID;
		UA_Identification_init(&tmpUAID);
		copyOvStringToOPCUA(pvs.ID.IdSpec, &tmpUAID.idSpec);
		tmpUAID.idType = pvs.ID.IdType;

		UA_ViewEnum tmpUAView;
		tmpUAView = pvs.view;

		UA_VisibilityEnum tmpUAVisibility;
		tmpUAVisibility = pvs.Visibility;

		UA_Variant_setScalarCopy(&output[0], &tmpUAExpressionLogic, &UA_OPENAAS[UA_OPENAAS_EXPRESSIONLOGICENUM]);
		UA_Variant_setScalarCopy(&output[1], &tmpUAExpressionSemantic, &UA_OPENAAS[UA_OPENAAS_EXPRESSIONSEMANTICENUM]);
		UA_Variant_setScalarCopy(&output[2], &tmpUAValue, &UA_TYPES[UA_TYPES_DATAVALUE]);
		UA_Variant_setScalarCopy(&output[3], &tmpUAUnit, &UA_TYPES[UA_TYPES_STRING]);
		UA_Variant_setScalarCopy(&output[4], &tmpUAID, &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION]);
		UA_Variant_setScalarCopy(&output[5], &tmpUAView, &UA_OPENAAS[UA_OPENAAS_VIEWENUM]);
		UA_Variant_setScalarCopy(&output[6], &tmpUAVisibility, &UA_OPENAAS[UA_OPENAAS_VISIBILITYENUM]);
		UA_Variant_setScalarCopy(&output[7], &result, &UA_TYPES[UA_TYPES_STATUSCODE]);

		UA_DataValue_deleteMembers(&tmpUAValue);
		UA_String_deleteMembers(&tmpUAUnit);
		UA_Identification_deleteMembers(&tmpUAID);
		IdentificationType_deleteMembers(&tmpOVAASId);
		ov_database_free(tmpOVPVSLName);
		ov_database_free(tmpOVPVSName);
		PropertyValueStatement_deleteMembers(&pvs);
		resultOV = OV_ERR_OK;
	}else if (ov_string_compare(funcName, "setPVS") == OV_STRCMP_EQUAL){
		UA_Identification *tmpUAAASId = (UA_Identification*)(input[0].data);
		UA_String *tmpUAPVSLName = (UA_String*)(input[1].data);
		UA_String *tmpUAPVSName = (UA_String*)(input[2].data);
		UA_ExpressionLogicEnum *tmpUAExpressionLogic = (UA_ExpressionLogicEnum*)(input[3].data);
		UA_ExpressionSemanticEnum *tmpUAExpressionSemantic = (UA_ExpressionSemanticEnum*)(input[4].data);
		UA_DataValue *tmpUAValue = (UA_DataValue*)(input[5].data);
		UA_String *tmpUAUnit = (UA_String*)(input[6].data);
		UA_Identification *tmpUAID = (UA_Identification*)(input[7].data);
		UA_ViewEnum *tmpUAView = (UA_ViewEnum*)(input[8].data);
		UA_VisibilityEnum *tmpUAVisibility = (UA_VisibilityEnum*)(input[9].data);

		IdentificationType tmpOVAASId;
		IdentificationType_init(&tmpOVAASId);
		copyOPCUAStringToOV(tmpUAAASId->idSpec, &(tmpOVAASId.IdSpec));
		tmpOVAASId.IdType = tmpUAAASId->idType;

		OV_STRING tmpOVPVSLName = NULL;
		copyOPCUAStringToOV(*tmpUAPVSLName, &tmpOVPVSLName);

		PropertyValueStatement pvs;
		PropertyValueStatement_init(&pvs);

		copyOPCUAStringToOV(*tmpUAPVSName, &pvs.pvsName);

		pvs.ExpressionLogic = *tmpUAExpressionLogic;

		pvs.ExpressionSemantic = *tmpUAExpressionSemantic;

		ov_VariantToAny(&(tmpUAValue->value), &pvs.value.Value);
		pvs.value.TimeStamp = ov_1601nsTimeToOvTime(tmpUAValue->sourceTimestamp);

		copyOPCUAStringToOV(*tmpUAUnit, &pvs.unit);

		copyOPCUAStringToOV(tmpUAID->idSpec, &(pvs.ID.IdSpec));
		pvs.ID.IdType = tmpUAID->idType;

		pvs.view = *tmpUAView;

		pvs.Visibility = *tmpUAVisibility;

		result = openaas_modelmanager_setPVS(tmpOVAASId, tmpOVPVSLName, pvs);

		UA_Variant_setScalarCopy(&output[0], &result, &UA_TYPES[UA_TYPES_STATUSCODE]);
		IdentificationType_deleteMembers(&tmpOVAASId);
		ov_database_free(tmpOVPVSLName);
		PropertyValueStatement_deleteMembers(&pvs);
		resultOV = OV_ERR_OK;
	}else if (ov_string_compare(funcName, "triggerGetCoreData") == OV_STRCMP_EQUAL){
		UA_Identification *tmpUASourceAAS = (UA_Identification*)(input[0].data);
		UA_Identification *tmpUATargetAAS = (UA_Identification*)(input[1].data);

		IdentificationType tmpOVSourceAAS;
		IdentificationType_init(&tmpOVSourceAAS);
		copyOPCUAStringToOV(tmpUASourceAAS->idSpec, &(tmpOVSourceAAS.IdSpec));
		tmpOVSourceAAS.IdType = tmpUASourceAAS->idType;

		IdentificationType tmpOVTargetAAS;
		IdentificationType_init(&tmpOVTargetAAS);
		copyOPCUAStringToOV(tmpUATargetAAS->idSpec, &(tmpOVTargetAAS.IdSpec));
		tmpOVTargetAAS.IdType = tmpUATargetAAS->idType;

		result = openaas_modelmanager_triggerGetCoreData(tmpOVSourceAAS, tmpOVTargetAAS);

		UA_Variant_setScalarCopy(&output[0], &result, &UA_TYPES[UA_TYPES_STATUSCODE]);
		IdentificationType_deleteMembers(&tmpOVSourceAAS);
		IdentificationType_deleteMembers(&tmpOVTargetAAS);
		resultOV = OV_ERR_OK;
	}else if (ov_string_compare(funcName, "startGetAssetLCEData") == OV_STRCMP_EQUAL){
		UA_Identification *tmpUAAASId = (UA_Identification*)(input[0].data);
		UA_String *tmpUAIPAddress = (UA_String*)(input[1].data);
		UA_Identification *tmpUAAssetAASId = (UA_Identification*)(input[2].data);

		IdentificationType tmpOVAASId;
		IdentificationType_init(&tmpOVAASId);
		copyOPCUAStringToOV(tmpUAAASId->idSpec, &(tmpOVAASId.IdSpec));
		tmpOVAASId.IdType = tmpUAAASId->idType;

		OV_STRING tmpOVIPAddress = NULL;
		copyOPCUAStringToOV(*tmpUAIPAddress, &tmpOVIPAddress);

		IdentificationType tmpOVAssetAASId;
		IdentificationType_init(&tmpOVAssetAASId);
		copyOPCUAStringToOV(tmpUAAssetAASId->idSpec, &(tmpOVAssetAASId.IdSpec));
		tmpOVAssetAASId.IdType = tmpUAAssetAASId->idType;

		result = openaas_modelmanager_startGetAssetLCEData(tmpOVAASId, tmpOVIPAddress, tmpOVAssetAASId);

		UA_Variant_setScalarCopy(&output[0], &result, &UA_TYPES[UA_TYPES_STATUSCODE]);
		IdentificationType_deleteMembers(&tmpOVAASId);
		IdentificationType_deleteMembers(&tmpOVAssetAASId);
		ov_database_free(tmpOVIPAddress);
		resultOV = OV_ERR_OK;
	}else if (ov_string_compare(funcName, "stopGetAssetLCEData") == OV_STRCMP_EQUAL){
		UA_Identification *tmpUAAASId = (UA_Identification*)(input[0].data);

		IdentificationType tmpOVAASId;
		IdentificationType_init(&tmpOVAASId);
		copyOPCUAStringToOV(tmpUAAASId->idSpec, &(tmpOVAASId.IdSpec));
		tmpOVAASId.IdType = tmpUAAASId->idType;

		result = openaas_modelmanager_stopGetAssetLCEData(tmpOVAASId);

		UA_Variant_setScalarCopy(&output[0], &result, &UA_TYPES[UA_TYPES_STATUSCODE]);
		IdentificationType_deleteMembers(&tmpOVAASId);
	}else{
		resultOV = OV_ERR_BADPARAM;
	}

    return resultOV;
}

static void OV_NodeStore_deleteNodestore(void *handle){

}

static void OV_NodeStore_deleteNode(UA_Node *node){
	//UA_Node_deleteMembersAnyNodeClass(node);
	ov_database_free(node);
}
static void OV_NodeStore_releaseNode(UA_Node *node){
	OV_NodeStore_deleteNode(node);
}

static UA_Node * OV_NodeStore_newNode(UA_NodeClass nodeClass){ //TODO add nodestore handle? --> move nodeStore from static context to main
    //allocate memory for a new node
	//UA_Node *newNode = NULL;
	//newNode = (UA_Node*) ov_database_malloc(sizeof(UA_ObjectNode));
	//newNode->nodeClass = nodeClass;
    return NULL;
}
static const UA_Node * OV_NodeStore_getNode(void *handle, const UA_NodeId *nodeId){
	UA_Node* opcuaNode = NULL;
	OV_STRING *plist = NULL;
	OV_STRING *plist2 = NULL;
	OV_STRING *plist3 = NULL;
	OV_STRING *plist4 = NULL;
	OV_STRING tmpString = NULL;
	OV_INSTPTR_ov_object pobj = NULL;
	OV_UINT len = 0;
	OV_UINT len2 = 0;
	OV_UINT len3 = 0;
	OV_UINT len4 = 0;
	copyOPCUAStringToOV(nodeId->identifier.string, &tmpString);
	plist = ov_string_split(tmpString, "/", &len);
	plist2 = ov_string_split(plist[len-1], ".", &len2);
	plist3 = ov_string_split(tmpString, "||", &len3);
	plist4 = ov_string_split(tmpString, "|", &len4);

	if (len3 > 1){
		pobj = ov_path_getobjectpointer(plist3[0], 2);
		if (pobj == NULL)
			return NULL;
		if (openaas_nodeStoreFunctions_ovModelManagerMethodNodeToOPCUA(NULL, nodeId, &opcuaNode) == UA_STATUSCODE_GOOD)
			return (UA_Node*) opcuaNode;
	}else if (len4 > 1 ){
		pobj = ov_path_getobjectpointer(plist4[0], 2);
		if (pobj == NULL)
			return NULL;
	}else{
		pobj = ov_path_getobjectpointer(tmpString, 2);
		if (pobj == NULL)
			return NULL;
	}

	if (ov_string_compare(plist[len-1], "openAAS") == OV_STRCMP_EQUAL){
			if (openaas_nodeStoreFunctions_ovOpenAASFolderNodeToOPCUA(NULL, nodeId, &opcuaNode) == UA_STATUSCODE_GOOD)
				return opcuaNode;
	}else if (ov_string_compare(plist[len-1], "AASFolder") == OV_STRCMP_EQUAL){
		if (openaas_nodeStoreFunctions_ovAASFolderNodeToOPCUA(NULL, nodeId, &opcuaNode) == UA_STATUSCODE_GOOD)
			return opcuaNode;
	}else if (ov_string_compare(plist2[len2-1], "Views") == OV_STRCMP_EQUAL){
		if (openaas_nodeStoreFunctions_ovViewsNodeToOPCUA(NULL, nodeId, &opcuaNode) == UA_STATUSCODE_GOOD)
			return opcuaNode;
	}else if (ov_string_compare(plist2[len2-1], "Header") == OV_STRCMP_EQUAL){
		if (openaas_nodeStoreFunctions_ovHeaderNodeToOPCUA(NULL, nodeId, &opcuaNode) == UA_STATUSCODE_GOOD)
			return opcuaNode;
	}else if (ov_string_compare(plist2[len2-1], "Body") == OV_STRCMP_EQUAL){
		if (openaas_nodeStoreFunctions_ovBodyNodeToOPCUA(NULL, nodeId, &opcuaNode) == UA_STATUSCODE_GOOD)
			return opcuaNode;
	}else if (ov_string_compare(plist4[len4-1], "Carrier") == OV_STRCMP_EQUAL){
		if (openaas_nodeStoreFunctions_ovCarrierNodeToOPCUA(NULL, nodeId, &opcuaNode) == UA_STATUSCODE_GOOD)
			return opcuaNode;
	}else if(Ov_CanCastTo(openaas_aas, pobj)){
		if (openaas_nodeStoreFunctions_ovOpenAASNodeToOPCUA(NULL, nodeId, &opcuaNode) == UA_STATUSCODE_GOOD)
			return opcuaNode;
	}else if(Ov_CanCastTo(openaas_LifeCycleArchive, pobj)){
		if (openaas_nodeStoreFunctions_ovLifeCycleArchiveNodeToOPCUA(NULL, nodeId, &opcuaNode) == UA_STATUSCODE_GOOD)
			return (UA_Node*) opcuaNode;
	}else if(Ov_CanCastTo(openaas_LifeCycleEntry, pobj)){
		if (openaas_nodeStoreFunctions_ovLifeCycleEntryNodeToOPCUA(NULL, nodeId, &opcuaNode) == UA_STATUSCODE_GOOD)
			return (UA_Node*) opcuaNode;
	}else if(Ov_CanCastTo(openaas_PropertyValueStatementList, pobj)){
		if (openaas_nodeStoreFunctions_ovPropertyValueStatementListNodeToOPCUA(NULL, nodeId, &opcuaNode) == UA_STATUSCODE_GOOD)
			return (UA_Node*) opcuaNode;
	}else if(Ov_CanCastTo(openaas_PropertyValueStatement, pobj)){
		if (openaas_nodeStoreFunctions_ovPropertyValueStatementNodeToOPCUA(NULL, nodeId, &opcuaNode) == UA_STATUSCODE_GOOD)
			return (UA_Node*) opcuaNode;
	}else if (Ov_CanCastTo(openaas_modelmanager, pobj)){
		if (openaas_nodeStoreFunctions_ovModelManagerNodeToOPCUA(NULL, nodeId, &opcuaNode) == UA_STATUSCODE_GOOD)
			return (UA_Node*) opcuaNode;
	}
	return NULL;
}
static UA_Node * OV_NodeStore_getCopyNode(void *handle, const UA_NodeId *nodeId){
	return (UA_Node*)OV_NodeStore_getNode(handle, nodeId);
}
static UA_StatusCode OV_NodeStore_removeNode(void *handle, const UA_NodeId *nodeId){
	return UA_STATUSCODE_BADNOTIMPLEMENTED;
	//OV_INSTPTR_ov_object pobj = opcua_nodeStoreFunctions_resolveNodeIdToOvObject((UA_NodeId*)nodeId);
	//if (pobj != NULL){
	//	Ov_DeleteObject(pobj);
	//}
    //return UA_STATUSCODE_GOOD;
}
static UA_StatusCode OV_NodeStore_insertNode(void *handle, UA_Node *node, UA_NodeId *parrentNode){
	return UA_STATUSCODE_BADNOTIMPLEMENTED;
	//OV_INSTPTR_ov_object pobj = opcua_nodeStoreFunctions_resolveNodeIdToOvObject(&(node->nodeId));
	//if (pobj != NULL)
	//	return UA_STATUSCODE_BADNODEIDEXISTS;

	//return OV_NodeStore_insert(handle, node, parrentNode);
}
static UA_StatusCode OV_NodeStore_replaceNode(void *handle, UA_Node *node){
	UA_StatusCode 			result = UA_STATUSCODE_GOOD;
	OV_PATH 				path;
	OV_PATH 				path2;
	OV_STRING 				tmpString = NULL;
	OV_STRING 				tmpString2 = NULL;
	OV_UINT 				len = 0;
	OV_STRING 				*plist = NULL;
	OV_INSTPTR_ov_object	pobj = NULL;
	OV_TICKET 				*pTicket = NULL;
	OV_VTBLPTR_ov_object	pVtblObj = NULL;
	OV_ACCESS				access;

	copyOPCUAStringToOV(node->nodeId.identifier.string, &tmpString);
	plist = ov_string_split(tmpString, "|", &len);

	if (len > 1){ //Carrier
		UA_NodeId tmpNodeId;
		UA_NodeId_init(&tmpNodeId);
		tmpNodeId.namespaceIndex = node->nodeId.namespaceIndex;
		tmpNodeId.identifierType = node->nodeId.identifierType;
		ov_string_setvalue(&tmpString2, plist[0]);
		ov_string_append(&tmpString2, ".CarrierString");
		copyOvStringToOPCUA(tmpString2, &(tmpNodeId.identifier.string));

		ov_memstack_lock();
		result = opcua_nodeStoreFunctions_resolveNodeIdToPath(tmpNodeId, &path);
		if(result != UA_STATUSCODE_GOOD){
			ov_memstack_unlock();
			return result;
		}

		ov_string_setvalue(&tmpString2, plist[0]);
		ov_string_append(&tmpString2, ".CarrierType");
		copyOvStringToOPCUA(tmpString2, &(tmpNodeId.identifier.string));

		result = opcua_nodeStoreFunctions_resolveNodeIdToPath(tmpNodeId, &path2);
		if(result != UA_STATUSCODE_GOOD){
			ov_memstack_unlock();
			return result;
		}

		UA_Identification *tmpIdentification = (UA_Identification *)(((UA_Variant*)&((UA_VariableNode*)node)->value.data.value.value)->data);

		copyOPCUAStringToOV(tmpIdentification->idSpec, ((OV_STRING*)(path.elements[path.size-1].pvalue)));
		*((UA_IdEnum*)path2.elements[path.size-1].pvalue) = tmpIdentification->idType;
	}else { // PVS or LCE
		ov_memstack_lock();
		result = opcua_nodeStoreFunctions_resolveNodeIdToPath(node->nodeId, &path);
		if(result != UA_STATUSCODE_GOOD){
			ov_memstack_unlock();
			return result;
		}
		result = opcua_nodeStoreFunctions_getVtblPointerAndCheckAccess(&(path.elements[path.size-1]), pTicket, &pobj, &pVtblObj, &access);
		if(result != UA_STATUSCODE_GOOD){
			ov_memstack_unlock();
			return result;
		}

		if (Ov_CanCastTo(openaas_PropertyValueStatement, pobj)){ // PVS
			OV_ELEMENT tmpPart;
			tmpPart.elemtype = OV_ET_NONE;
			OV_ELEMENT tmpParrent;
			tmpParrent.pobj = pobj;
			tmpParrent.elemtype = OV_ET_OBJECT;
			UA_PropertyValueStatement *tmpPropertyValueStatement = ((UA_PropertyValueStatement*)(((UA_Variant*)&((UA_VariableNode*)node)->value.data.value.value)->data));
			do {
				ov_element_getnextpart(&tmpParrent, &tmpPart, OV_ET_VARIABLE);
				if (tmpPart.elemtype == OV_ET_NONE)
					break;
				if (ov_string_compare(tmpPart.elemunion.pvar->v_identifier, "ExpressionSemantic") == OV_STRCMP_EQUAL)
					*(UA_UInt32*)tmpPart.pvalue = tmpPropertyValueStatement->expressionSemantic;
				if (ov_string_compare(tmpPart.elemunion.pvar->v_identifier, "ExpressionLogic") == OV_STRCMP_EQUAL)
					*(UA_UInt32*)tmpPart.pvalue = tmpPropertyValueStatement->expressionLogic;
				if (ov_string_compare(tmpPart.elemunion.pvar->v_identifier, "Unit") == OV_STRCMP_EQUAL)
					copyOPCUAStringToOV(tmpPropertyValueStatement->unit, ((OV_STRING*)tmpPart.pvalue));
				if (ov_string_compare(tmpPart.elemunion.pvar->v_identifier, "Value") == OV_STRCMP_EQUAL)
					ov_VariantToAny(&tmpPropertyValueStatement->value, (OV_ANY*)tmpPart.pvalue);
				if (ov_string_compare(tmpPart.elemunion.pvar->v_identifier, "IDIdString") == OV_STRCMP_EQUAL)
					copyOPCUAStringToOV(tmpPropertyValueStatement->iD.idSpec, ((OV_STRING*)tmpPart.pvalue));
				if (ov_string_compare(tmpPart.elemunion.pvar->v_identifier, "IDIdType") == OV_STRCMP_EQUAL)
					*(UA_UInt32*)tmpPart.pvalue = tmpPropertyValueStatement->iD.idType;
				if (ov_string_compare(tmpPart.elemunion.pvar->v_identifier, "View") == OV_STRCMP_EQUAL)
					*(UA_UInt32*)tmpPart.pvalue = tmpPropertyValueStatement->view;
			} while(true);
		}else if (Ov_CanCastTo(openaas_LifeCycleEntry, pobj)){ // LCE
			OV_ELEMENT tmpPart;
			tmpPart.elemtype = OV_ET_NONE;
			OV_ELEMENT tmpParrent;
			tmpParrent.pobj = pobj;
			tmpParrent.elemtype = OV_ET_OBJECT;
			UA_LifeCycleEntry *tmpLifeCycleEntry = ((UA_LifeCycleEntry*)(((UA_Variant*)&((UA_VariableNode*)node)->value.data.value.value)->data));
			do {
				ov_element_getnextpart(&tmpParrent, &tmpPart, OV_ET_VARIABLE);
				if (tmpPart.elemtype == OV_ET_NONE)
					break;
				if (ov_string_compare(tmpPart.elemunion.pvar->v_identifier, "CreatingInstanceString") == OV_STRCMP_EQUAL)
					copyOPCUAStringToOV(tmpLifeCycleEntry->creatingInstance.idSpec, (OV_STRING*)tmpPart.pvalue);
				if (ov_string_compare(tmpPart.elemunion.pvar->v_identifier, "CreatingInstanceType") == OV_STRCMP_EQUAL)
					*(UA_UInt32*)tmpPart.pvalue = tmpLifeCycleEntry->creatingInstance.idType;
				if (ov_string_compare(tmpPart.elemunion.pvar->v_identifier, "WritingInstanceString") == OV_STRCMP_EQUAL)
					copyOPCUAStringToOV(tmpLifeCycleEntry->writingInstance.idSpec, (OV_STRING*)tmpPart.pvalue);
				if (ov_string_compare(tmpPart.elemunion.pvar->v_identifier, "WritingInstanceType") == OV_STRCMP_EQUAL)
					*(UA_UInt32*)tmpPart.pvalue = tmpLifeCycleEntry->writingInstance.idType;
				if (ov_string_compare(tmpPart.elemunion.pvar->v_identifier, "Data") == OV_STRCMP_EQUAL)
					ov_VariantToAny(&tmpLifeCycleEntry->data.value, (OV_ANY*)tmpPart.pvalue);
				if (ov_string_compare(tmpPart.elemunion.pvar->v_identifier, "TimeStamp") == OV_STRCMP_EQUAL)
					*(UA_DateTime*)tmpPart.pvalue = tmpLifeCycleEntry->data.sourceTimestamp;
				if (ov_string_compare(tmpPart.elemunion.pvar->v_identifier, "Subject") == OV_STRCMP_EQUAL)
					copyOPCUAStringToOV(tmpLifeCycleEntry->subject, (OV_STRING*)tmpPart.pvalue);
				if (ov_string_compare(tmpPart.elemunion.pvar->v_identifier, "EventClass") == OV_STRCMP_EQUAL)
					copyOPCUAStringToOV(tmpLifeCycleEntry->eventClass, (OV_STRING*)tmpPart.pvalue);
			} while(true);
		}
	}

	return UA_STATUSCODE_GOOD;
}


void
openaas_nodeStoreFunctions_ovNodeStoreInterfaceOpenAASNew(UA_NodestoreInterface* nsi) {
    nsi->handle =        	NULL;
    nsi->deleteNodeStore =  (UA_NodestoreInterface_delete) 		OV_NodeStore_deleteNodestore;
    nsi->newNode =       	(UA_NodestoreInterface_newNode)     OV_NodeStore_newNode;
    nsi->deleteNode =    	(UA_NodestoreInterface_deleteNode)  OV_NodeStore_deleteNode;
    nsi->insert =       	(UA_NodestoreInterface_insert)      OV_NodeStore_insertNode;
    nsi->get =          	(UA_NodestoreInterface_get)         OV_NodeStore_getNode;
    nsi->getCopy =      	(UA_NodestoreInterface_getCopy)     OV_NodeStore_getCopyNode;
    nsi->replace =      	(UA_NodestoreInterface_replace)     OV_NodeStore_replaceNode;
    nsi->remove =       	(UA_NodestoreInterface_remove)      OV_NodeStore_removeNode;
    //nsi->iterateNode =       (UA_NodestoreInterface_iterateNode)     OV_NodeStore_iterateNode;
    nsi->release =       	(UA_NodestoreInterface_release)     OV_NodeStore_releaseNode;
}

void
openaas_nodeStoreFunctions_ovNodeStoreInterfaceOpenAASDelete(UA_NodestoreInterface * nsi){
	if (nsi->handle)
		UA_free(nsi->handle);
}
