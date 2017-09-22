/* * nodeStoreInterface.c
 *
 *  Created on: 06.12.2016
 *      Author: ubuntu
 */

#ifndef OV_COMPILE_LIBRARY_openaasOPCUAInterface
#define OV_COMPILE_LIBRARY_openaasOPCUAInterface
#endif

#include "openaasOPCUAInterface.h"
#include "libov/ov_macros.h"
#include "ksbase.h"
#include "opcua.h"
#include "opcua_helpers.h"
#include "NoneTicketAuthenticator.h"
#include "libov/ov_path.h"
#include "libov/ov_memstack.h"
#include "ks_logfile.h"
#include "ua_identification_generated.h"
#include "ua_identification_generated_handling.h"
#include "ua_lifeCycleEntry_generated.h"
#include "ua_lifeCycleEntry_generated_handling.h"
#include "ua_propertyValueStatement_generated.h"
#include "ua_propertyValueStatement_generated_handling.h"
#include "openaas_helpers.h"


OV_DLLFNCEXPORT UA_StatusCode openaasOPCUAInterface_interface_MethodCallbackModelmanager(void *methodHandle, const UA_NodeId *objectId,
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
		tmpNodeId = openaasOPCUAInterface_interface_getAASNodeId(tmpOVAASId);

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
	}else if (ov_string_compare(funcName, "createSubModel") == OV_STRCMP_EQUAL){
		UA_Identification *tmpUAAASId = (UA_Identification*)(input[0].data);
		UA_Identification *tmpUAParentId = (UA_Identification*)(input[1].data);
		UA_Identification *tmpUAModelId = (UA_Identification*)(input[2].data);
		UA_String *tmpUAName = (UA_String*)(input[3].data);
		UA_UInt32 *tmpUARevision = (UA_UInt32*)(input[4].data);
		UA_UInt32 *tmpUAVersion = (UA_UInt32*)(input[5].data);

		IdentificationType tmpOVAASId;
		IdentificationType_init(&tmpOVAASId);
		copyOPCUAStringToOV(tmpUAAASId->idSpec, &(tmpOVAASId.IdSpec));
		tmpOVAASId.IdType = tmpUAAASId->idType;

		IdentificationType tmpOVParentId;
		IdentificationType_init(&tmpOVParentId);
		copyOPCUAStringToOV(tmpUAParentId->idSpec, &(tmpOVParentId.IdSpec));
		tmpOVParentId.IdType = tmpUAParentId->idType;

		IdentificationType tmpOVModelId;
		IdentificationType_init(&tmpOVModelId);
		copyOPCUAStringToOV(tmpUAModelId->idSpec, &(tmpOVModelId.IdSpec));
		tmpOVModelId.IdType = tmpUAModelId->idType;

		OV_STRING tmpOVName = NULL;
		copyOPCUAStringToOV(*tmpUAName, &tmpOVName);

		OV_UINT tmpOVRevision;
		tmpOVRevision = *tmpUARevision;

		OV_UINT tmpOVVersion;
		tmpOVVersion = *tmpUAVersion;

		result = openaas_modelmanager_createSubModel(tmpOVAASId, tmpOVParentId, tmpOVModelId, tmpOVName, tmpOVRevision, tmpOVVersion);

		UA_Variant_setScalarCopy(&output[0], &result, &UA_TYPES[UA_TYPES_STATUSCODE]);
		IdentificationType_deleteMembers(&tmpOVAASId);
		IdentificationType_deleteMembers(&tmpOVParentId);
		IdentificationType_deleteMembers(&tmpOVModelId);
		ov_database_free(tmpOVName);
		resultOV = OV_ERR_OK;
	}else if (ov_string_compare(funcName, "deleteSubModel") == OV_STRCMP_EQUAL){
		UA_Identification *tmpUAAASId = (UA_Identification*)(input[0].data);
		UA_Identification *tmpUASMId = (UA_Identification*)(input[1].data);

		IdentificationType tmpOVAASId;
		IdentificationType_init(&tmpOVAASId);
		copyOPCUAStringToOV(tmpUAAASId->idSpec, &(tmpOVAASId.IdSpec));
		tmpOVAASId.IdType = tmpUAAASId->idType;

		IdentificationType tmpOVSMId;
		IdentificationType_init(&tmpOVSMId);
		copyOPCUAStringToOV(tmpUASMId->idSpec, &(tmpOVSMId.IdSpec));
		tmpOVSMId.IdType = tmpUASMId->idType;

		result = openaas_modelmanager_deleteSubModel(tmpOVAASId, tmpOVSMId);

		UA_Variant_setScalarCopy(&output[0], &result, &UA_TYPES[UA_TYPES_STATUSCODE]);
		IdentificationType_deleteMembers(&tmpOVAASId);
		IdentificationType_deleteMembers(&tmpOVSMId);
		resultOV = OV_ERR_OK;
	}else if (ov_string_compare(funcName, "createPVSL") == OV_STRCMP_EQUAL){
		UA_Identification *tmpUAAASId = (UA_Identification*)(input[0].data);
		UA_Identification *tmpUAParentId = (UA_Identification*)(input[1].data);
		UA_String *tmpUAPVSLName = (UA_String*)(input[2].data);
		UA_UInt32 *tmpUAMask = (UA_UInt32*)(input[3].data);
		UA_Identification *tmpUACarrier = (UA_Identification*)(input[4].data);
		UA_ExpressionLogicEnum *tmpUAExpressionLogic = (UA_ExpressionLogicEnum*)(input[5].data);
		UA_ExpressionSemanticEnum *tmpUAExpressionSemantic = (UA_ExpressionSemanticEnum*)(input[6].data);
		UA_Identification *tmpUAPropertyId = (UA_Identification*)(input[7].data);
		UA_ViewEnum *tmpUAView = (UA_ViewEnum*)(input[8].data);
		UA_VisibilityEnum *tmpUAVisibility = (UA_VisibilityEnum*)(input[9].data);

		IdentificationType tmpOVAASId;
		IdentificationType_init(&tmpOVAASId);
		copyOPCUAStringToOV(tmpUAAASId->idSpec, &(tmpOVAASId.IdSpec));
		tmpOVAASId.IdType = tmpUAAASId->idType;

		IdentificationType tmpOVParentId;
		IdentificationType_init(&tmpOVParentId);
		copyOPCUAStringToOV(tmpUAParentId->idSpec, &(tmpOVParentId.IdSpec));
		tmpOVParentId.IdType = tmpUAParentId->idType;

		OV_STRING tmpOVPVSLName = NULL;
		copyOPCUAStringToOV(*tmpUAPVSLName, &tmpOVPVSLName);

		OV_UINT tmpOVMask;
		tmpOVMask = *tmpUAMask;

		IdentificationType tmpOVCarrier;
		IdentificationType_init(&tmpOVCarrier);
		copyOPCUAStringToOV(tmpUACarrier->idSpec, &(tmpOVCarrier.IdSpec));
		tmpOVCarrier.IdType = tmpUACarrier->idType;

		ExpressionLogicEnum tmpOVExpressionLogic;
		tmpOVExpressionLogic = *tmpUAExpressionLogic;

		ExpressionSemanticEnum  tmpOVExpressionSemantic;
		tmpOVExpressionSemantic = *tmpUAExpressionSemantic;

		IdentificationType tmpOVPropertyId;
		IdentificationType_init(&tmpOVPropertyId);
		copyOPCUAStringToOV(tmpUAPropertyId->idSpec, &(tmpOVPropertyId.IdSpec));
		tmpOVPropertyId.IdType = tmpUAPropertyId->idType;

		ViewEnum  tmpOVView;
		tmpOVView = *tmpUAView;

		VisibilityEnum tmpOVVisibility;
		tmpOVVisibility = *tmpUAVisibility;

		result = openaas_modelmanager_createPVSL(tmpOVAASId, tmpOVParentId, tmpOVPVSLName, tmpOVMask, tmpOVCarrier, tmpOVExpressionLogic, tmpOVExpressionSemantic, tmpOVPropertyId, tmpOVView, tmpOVVisibility);

		UA_Variant_setScalarCopy(&output[0], &result, &UA_TYPES[UA_TYPES_STATUSCODE]);
		IdentificationType_deleteMembers(&tmpOVAASId);
		IdentificationType_deleteMembers(&tmpOVParentId);
		ov_database_free(tmpOVPVSLName);
		IdentificationType_deleteMembers(&tmpOVCarrier);
		IdentificationType_deleteMembers(&tmpOVPropertyId);
		resultOV = OV_ERR_OK;
	}else if (ov_string_compare(funcName, "deletePVSL") == OV_STRCMP_EQUAL){
		UA_Identification *tmpUAAASId = (UA_Identification*)(input[0].data);
		UA_Identification *tmpUAPVSLId = (UA_Identification*)(input[1].data);

		IdentificationType tmpOVAASId;
		IdentificationType_init(&tmpOVAASId);
		copyOPCUAStringToOV(tmpUAAASId->idSpec, &(tmpOVAASId.IdSpec));
		tmpOVAASId.IdType = tmpUAAASId->idType;

		IdentificationType tmpOVPVSLId;
		IdentificationType_init(&tmpOVPVSLId);
		copyOPCUAStringToOV(tmpUAPVSLId->idSpec, &(tmpOVPVSLId.IdSpec));
		tmpOVPVSLId.IdType = tmpUAPVSLId->idType;


		result = openaas_modelmanager_deletePVSL(tmpOVAASId, tmpOVPVSLId);

		UA_Variant_setScalarCopy(&output[0], &result, &UA_TYPES[UA_TYPES_STATUSCODE]);
		IdentificationType_deleteMembers(&tmpOVAASId);
		IdentificationType_deleteMembers(&tmpOVPVSLId);
		resultOV = OV_ERR_OK;
	}else if (ov_string_compare(funcName, "createLCE") == OV_STRCMP_EQUAL){
		UA_Identification *tmpUAAASId = (UA_Identification*)(input[0].data);
		UA_LifeCycleEntry *tmpUALCE = (UA_LifeCycleEntry*)(input[1].data);

		IdentificationType tmpOVAASId;
		IdentificationType_init(&tmpOVAASId);
		copyOPCUAStringToOV(tmpUAAASId->idSpec, &(tmpOVAASId.IdSpec));
		tmpOVAASId.IdType = tmpUAAASId->idType;

		LifeCycleEntry lce;
		LifeCycleEntry_init(&lce);

		copyOPCUAStringToOV(tmpUALCE->creatingInstance.idSpec, &(lce.creatingInstance.IdSpec));
		lce.creatingInstance.IdType = tmpUALCE->creatingInstance.idType;

		copyOPCUAStringToOV(tmpUALCE->eventClass, &lce.eventClass);

		copyOPCUAStringToOV(tmpUALCE->subject, &lce.subject);

		copyOPCUAStringToOV(tmpUALCE->writingInstance.idSpec, &(lce.writingInstance.IdSpec));
		lce.writingInstance.IdType = tmpUALCE->writingInstance.idType;

		ov_VariantToAny(&(tmpUALCE->data.value), &lce.data);
		lce.data.time = ov_1601nsTimeToOvTime(tmpUALCE->data.sourceTimestamp);

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
		UA_LifeCycleEntry tmpUALCE;
		UA_LifeCycleEntry_init(&tmpUALCE);
		if (result == AASSTATUSCODE_GOOD){
			tmpUALCE.creatingInstance.idSpec = UA_String_fromChars(lce.writingInstance.IdSpec);
			tmpUALCE.creatingInstance.idType = lce.writingInstance.IdType;

			tmpUALCE.writingInstance.idSpec = UA_String_fromChars(lce.writingInstance.IdSpec);
			tmpUALCE.writingInstance.idType = lce.writingInstance.IdType;

			tmpUALCE.eventClass = UA_String_fromChars(lce.eventClass);

			tmpUALCE.subject = UA_String_fromChars(lce.subject);

			ov_AnyToVariant(&lce.data, &tmpUALCE.data.value);
			tmpUALCE.data.hasValue = TRUE;
			tmpUALCE.data.sourceTimestamp = ov_ovTimeTo1601nsTime(lce.data.time);
			tmpUALCE.data.hasSourceTimestamp = TRUE;
		}
		UA_Variant_setScalarCopy(&output[0], &result, &UA_TYPES[UA_TYPES_STATUSCODE]);
		UA_Variant_setScalarCopy(&output[1], &tmpUALCE, &UA_LIFECYCLEENTRY[UA_LIFECYCLEENTRY_LIFECYCLEENTRY]);

		UA_LifeCycleEntry_deleteMembers(&tmpUALCE);
		IdentificationType_deleteMembers(&tmpOVAASId);
		LifeCycleEntry_deleteMembers(&lce);
		resultOV = OV_ERR_OK;
	}else if (ov_string_compare(funcName, "getLCESimple") == OV_STRCMP_EQUAL){
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

		UA_Identification tmpUAWritingInstance;
		UA_Identification_init(&tmpUAWritingInstance);

		UA_String tmpUAEventClass;
		UA_String_init(&tmpUAEventClass);

		UA_String tmpUASubject;
		UA_String_init(&tmpUASubject);

		UA_DataValue tmpUAValue;
		UA_DataValue_init(&tmpUAValue);
		if (result == AASSTATUSCODE_GOOD){
			tmpUACreatingInstance.idSpec = UA_String_fromChars(lce.creatingInstance.IdSpec);
			tmpUACreatingInstance.idType = lce.creatingInstance.IdType;

			tmpUAWritingInstance.idSpec = UA_String_fromChars(lce.writingInstance.IdSpec);
			tmpUAWritingInstance.idType = lce.writingInstance.IdType;

			tmpUAEventClass = UA_String_fromChars(lce.eventClass);

			tmpUASubject = UA_String_fromChars(lce.subject);

			ov_AnyToVariant(&lce.data, &tmpUAValue.value);
			tmpUAValue.hasValue = TRUE;
			tmpUAValue.sourceTimestamp = ov_ovTimeTo1601nsTime(lce.data.time);
			tmpUAValue.hasSourceTimestamp = TRUE;
		}

		UA_Variant_setScalarCopy(&output[0], &result, &UA_TYPES[UA_TYPES_STATUSCODE]);
		UA_Variant_setScalarCopy(&output[1], &tmpUACreatingInstance, &UA_IDENTIFICATION[UA_IDENTIFICATION_IDENTIFICATION]);
		UA_Variant_setScalarCopy(&output[2], &tmpUAWritingInstance, &UA_IDENTIFICATION[UA_IDENTIFICATION_IDENTIFICATION]);
		UA_Variant_setScalarCopy(&output[3], &tmpUAEventClass, &UA_TYPES[UA_TYPES_STRING]);
		UA_Variant_setScalarCopy(&output[4], &tmpUASubject, &UA_TYPES[UA_TYPES_STRING]);
		UA_Variant_setScalarCopy(&output[5], &tmpUAValue, &UA_TYPES[UA_TYPES_DATAVALUE]);

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

		OV_UINT arrayDimension = 0;

		result = openaas_modelmanager_getLastLCEs(tmpOVAASId, tmpOVCount, &lce, &arrayDimension);

		UA_Variant_setScalarCopy(&output[0], &result, &UA_TYPES[UA_TYPES_STATUSCODE]);

		if (result == AASSTATUSCODE_GOOD){
			UA_LifeCycleEntry *tmpUALifeCycleEntry = UA_Array_new(arrayDimension,&UA_LIFECYCLEENTRY[UA_LIFECYCLEENTRY_LIFECYCLEENTRY]);

			for (OV_UINT i = 0; i < arrayDimension; i++){
				UA_LifeCycleEntry_init(&tmpUALifeCycleEntry[i]);
				tmpUALifeCycleEntry[i].creatingInstance.idSpec = UA_String_fromChars(lce[i].creatingInstance.IdSpec);
				tmpUALifeCycleEntry[i].creatingInstance.idType = lce[i].creatingInstance.IdType;
				tmpUALifeCycleEntry[i].writingInstance.idSpec = UA_String_fromChars(lce[i].writingInstance.IdSpec);
				tmpUALifeCycleEntry[i].writingInstance.idType = lce[i].writingInstance.IdType;
				tmpUALifeCycleEntry[i].eventClass = UA_String_fromChars(lce[i].eventClass);
				tmpUALifeCycleEntry[i].subject = UA_String_fromChars(lce[i].subject);
				ov_AnyToVariant(&lce[i].data, &tmpUALifeCycleEntry[i].data.value);
				tmpUALifeCycleEntry[i].data.hasValue = TRUE;
				tmpUALifeCycleEntry[i].data.sourceTimestamp = ov_ovTimeTo1601nsTime(lce[i].data.time);
				tmpUALifeCycleEntry[i].data.hasSourceTimestamp = TRUE;
				tmpUALifeCycleEntry[i].id = lce[i].lceId;
				}
			UA_Variant_setArrayCopy(&output[1], tmpUALifeCycleEntry, arrayDimension, &UA_LIFECYCLEENTRY[UA_LIFECYCLEENTRY_LIFECYCLEENTRY]);
			UA_Variant_setScalarCopy(&output[2], &arrayDimension, &UA_TYPES[UA_TYPES_UINT32]);
			for (OV_UINT i = 0; i < arrayDimension; i++){
				ov_database_free(lce[i].creatingInstance.IdSpec);
				ov_database_free(lce[i].eventClass);
				ov_database_free(lce[i].subject);
				ov_database_free(lce[i].writingInstance.IdSpec);
			}
			UA_Array_delete(tmpUALifeCycleEntry,arrayDimension,&UA_LIFECYCLEENTRY[UA_LIFECYCLEENTRY_LIFECYCLEENTRY]);
		}else{
			UA_LifeCycleEntry *tmpUALifeCycleEntry = UA_Array_new(0,&UA_LIFECYCLEENTRY[UA_LIFECYCLEENTRY_LIFECYCLEENTRY]);
			UA_Variant_setArrayCopy(&output[1], tmpUALifeCycleEntry, 0, &UA_LIFECYCLEENTRY[UA_LIFECYCLEENTRY_LIFECYCLEENTRY]);
			UA_Variant_setScalarCopy(&output[2], 0, &UA_TYPES[UA_TYPES_UINT32]);
			UA_Array_delete(tmpUALifeCycleEntry,0,&UA_LIFECYCLEENTRY[UA_LIFECYCLEENTRY_LIFECYCLEENTRY]);
		}
		IdentificationType_deleteMembers(&tmpOVAASId);
		resultOV = OV_ERR_OK;
	}else if (ov_string_compare(funcName, "setLCE") == OV_STRCMP_EQUAL){
		UA_Identification *tmpUAAASId = (UA_Identification*)(input[0].data);
		UA_UInt64 *tmpUALCEId = (UA_UInt64*)(input[1].data);
		UA_LifeCycleEntry *tmpUALCEData = (UA_LifeCycleEntry*)(input[2].data);

		IdentificationType tmpOVAASId;
		IdentificationType_init(&tmpOVAASId);
		copyOPCUAStringToOV(tmpUAAASId->idSpec, &(tmpOVAASId.IdSpec));
		tmpOVAASId.IdType = tmpUAAASId->idType;

		LifeCycleEntry lce;
		LifeCycleEntry_init(&lce);

		copyOPCUAStringToOV(tmpUALCEData->creatingInstance.idSpec, &(lce.creatingInstance.IdSpec));
		lce.creatingInstance.IdType = tmpUALCEData->creatingInstance.idType;

		copyOPCUAStringToOV(tmpUALCEData->eventClass, &lce.eventClass);

		copyOPCUAStringToOV(tmpUALCEData->subject, &lce.subject);

		copyOPCUAStringToOV(tmpUALCEData->writingInstance.idSpec, &(lce.writingInstance.IdSpec));
		lce.writingInstance.IdType = tmpUALCEData->writingInstance.idType;

		ov_VariantToAny(&(tmpUALCEData->data.value), &lce.data);
		lce.data.time = ov_1601nsTimeToOvTime(tmpUALCEData->data.sourceTimestamp);

		lce.lceId = *tmpUALCEId;

		result = openaas_modelmanager_setLCE(tmpOVAASId, lce);

		UA_Variant_setScalarCopy(&output[0], &result, &UA_TYPES[UA_TYPES_STATUSCODE]);
		IdentificationType_deleteMembers(&tmpOVAASId);
		LifeCycleEntry_deleteMembers(&lce);
		resultOV = OV_ERR_OK;
	}else if (ov_string_compare(funcName, "setLCESimple") == OV_STRCMP_EQUAL){
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

		ov_VariantToAny(&(tmpUAValue->value), &lce.data);
		lce.data.time = ov_1601nsTimeToOvTime(tmpUAValue->sourceTimestamp);

		result = openaas_modelmanager_setLCE(tmpOVAASId, lce);

		UA_Variant_setScalarCopy(&output[0], &result, &UA_TYPES[UA_TYPES_STATUSCODE]);
		IdentificationType_deleteMembers(&tmpOVAASId);
		LifeCycleEntry_deleteMembers(&lce);
		resultOV = OV_ERR_OK;
	}else if (ov_string_compare(funcName, "createPVS") == OV_STRCMP_EQUAL){
		UA_Identification *tmpUAAASId = (UA_Identification*)(input[0].data);
		UA_Identification *tmpUAListId = (UA_Identification*)(input[1].data);
		UA_String *tmpUAPVSName = (UA_String*)(input[2].data);
		UA_DataValue *tmpUAValue = (UA_DataValue*)(input[3].data);
		UA_UInt32 *tmpUAMask = (UA_UInt32*)(input[4].data);
		UA_Identification *tmpUACarrier = (UA_Identification*)(input[5].data);
		UA_ExpressionLogicEnum *tmpUAExpressionLogic = (UA_ExpressionLogicEnum*)(input[6].data);
		UA_ExpressionSemanticEnum *tmpUAExpressionSemantic = (UA_ExpressionSemanticEnum*)(input[7].data);
		UA_Identification *tmpUAPropertyId = (UA_Identification*)(input[8].data);
		UA_ViewEnum *tmpUAView = (UA_ViewEnum*)(input[9].data);
		UA_VisibilityEnum *tmpUAVisibility = (UA_VisibilityEnum*)(input[10].data);

		IdentificationType tmpOVAASId;
		IdentificationType_init(&tmpOVAASId);
		copyOPCUAStringToOV(tmpUAAASId->idSpec, &(tmpOVAASId.IdSpec));
		tmpOVAASId.IdType = tmpUAAASId->idType;

		IdentificationType tmpOVListId;
		IdentificationType_init(&tmpOVListId);
		copyOPCUAStringToOV(tmpUAListId->idSpec, &(tmpOVListId.IdSpec));
		tmpOVListId.IdType = tmpUAListId->idType;

		OV_STRING tmpOVPVSName = NULL;
		copyOPCUAStringToOV(*tmpUAPVSName, &tmpOVPVSName);

		OV_ANY tmpOVValue;
		ov_VariantToAny(&(tmpUAValue->value), &tmpOVValue);
		tmpOVValue.time = ov_1601nsTimeToOvTime(tmpUAValue->sourceTimestamp);

		OV_UINT tmpOVMask;
		tmpOVMask = *tmpUAMask;

		IdentificationType tmpOVCarrier;
		IdentificationType_init(&tmpOVCarrier);
		copyOPCUAStringToOV(tmpUACarrier->idSpec, &(tmpOVCarrier.IdSpec));
		tmpOVCarrier.IdType = tmpUACarrier->idType;

		OV_UINT tmpOVExpressionLogic;
		tmpOVExpressionLogic = *tmpUAExpressionLogic;

		OV_UINT tmpOVExpressionSemantic;
		tmpOVExpressionSemantic = *tmpUAExpressionSemantic;

		IdentificationType tmpOVPropertyId;
		IdentificationType_init(&tmpOVPropertyId);
		copyOPCUAStringToOV(tmpUAPropertyId->idSpec, &(tmpOVPropertyId.IdSpec));
		tmpOVPropertyId.IdType = tmpUAPropertyId->idType;

		OV_UINT tmpOVView;
		tmpOVView = *tmpUAView;

		OV_UINT tmpOVVisibility;
		tmpOVVisibility = *tmpUAVisibility;

		result = openaas_modelmanager_createPVS(tmpOVAASId, tmpOVListId, tmpOVPVSName, tmpOVValue, tmpOVMask, tmpOVCarrier, tmpOVExpressionLogic, tmpOVExpressionSemantic, tmpOVPropertyId, tmpOVView, tmpOVVisibility);

		UA_Variant_setScalarCopy(&output[0], &result, &UA_TYPES[UA_TYPES_STATUSCODE]);
		IdentificationType_deleteMembers(&tmpOVAASId);
		IdentificationType_deleteMembers(&tmpOVListId);
		ov_database_free(tmpOVPVSName);
		IdentificationType_deleteMembers(&tmpOVCarrier);
		IdentificationType_deleteMembers(&tmpOVPropertyId);
		Ov_SetAnyValue(&tmpOVValue, NULL);
		resultOV = OV_ERR_OK;
	}else if (ov_string_compare(funcName, "deletePVS") == OV_STRCMP_EQUAL){
		UA_Identification *tmpUAAASId = (UA_Identification*)(input[0].data);
		UA_Identification *tmpUAPVSId = (UA_Identification*)(input[1].data);

		IdentificationType tmpOVAASId;
		IdentificationType_init(&tmpOVAASId);
		copyOPCUAStringToOV(tmpUAAASId->idSpec, &(tmpOVAASId.IdSpec));
		tmpOVAASId.IdType = tmpUAAASId->idType;

		IdentificationType tmpOVPVSId;
		IdentificationType_init(&tmpOVPVSId);
		copyOPCUAStringToOV(tmpUAPVSId->idSpec, &(tmpOVPVSId.IdSpec));
		tmpOVPVSId.IdType = tmpUAPVSId->idType;


		result = openaas_modelmanager_deletePVS(tmpOVAASId, tmpOVPVSId);

		UA_Variant_setScalarCopy(&output[0], &result, &UA_TYPES[UA_TYPES_STATUSCODE]);
		IdentificationType_deleteMembers(&tmpOVAASId);
		IdentificationType_deleteMembers(&tmpOVPVSId);
		resultOV = OV_ERR_OK;
	}else if (ov_string_compare(funcName, "getPVS") == OV_STRCMP_EQUAL){
		UA_Identification *tmpUAAASId = (UA_Identification*)(input[0].data);
		UA_Identification *tmpUAPVSlId = (UA_Identification*)(input[1].data);

		IdentificationType tmpOVAASId;
		IdentificationType_init(&tmpOVAASId);
		copyOPCUAStringToOV(tmpUAAASId->idSpec, &(tmpOVAASId.IdSpec));
		tmpOVAASId.IdType = tmpUAAASId->idType;

		IdentificationType tmpOVPVSlId;
		IdentificationType_init(&tmpOVPVSlId);
		copyOPCUAStringToOV(tmpUAPVSlId->idSpec, &(tmpOVPVSlId.IdSpec));
		tmpOVPVSlId.IdType = tmpUAPVSlId->idType;

		OV_STRING tmpOVPVSName = NULL;
		IdentificationType tmpOVCarrierId;
		IdentificationType_init(&tmpOVCarrierId);
		ExpressionLogicEnum tmpOVExpressionLogic;
		ExpressionSemanticEnum tmpOVExpressionSemantic;
		IdentificationType tmpOVPropertyId;
		IdentificationType_init(&tmpOVPropertyId);
		ViewEnum tmpOVView;
		VisibilityEnum tmpOVVisibility;
		OV_ANY tmpOVValue;
		Ov_SetAnyValue(&tmpOVValue, NULL);

		result = openaas_modelmanager_getPVS(tmpOVAASId, tmpOVPVSlId, &tmpOVPVSName, &tmpOVCarrierId, &tmpOVExpressionLogic, &tmpOVExpressionSemantic, &tmpOVPropertyId, &tmpOVView, &tmpOVVisibility, &tmpOVValue);

		UA_String tmpUAPVSName;
		UA_String_init(&tmpUAPVSName);

		UA_Identification tmpUACarrierId;
		UA_Identification_init(&tmpUACarrierId);

		UA_ExpressionLogicEnum tmpUAExpressionLogic;
		UA_ExpressionLogicEnum_init(&tmpUAExpressionLogic);

		UA_ExpressionSemanticEnum tmpUAExpressionSemantic;
		UA_ExpressionSemanticEnum_init(&tmpUAExpressionSemantic);

		UA_Identification tmpUAPropertyId;
		UA_Identification_init(&tmpUAPropertyId);

		UA_ViewEnum tmpUAView;
		UA_ViewEnum_init(&tmpUAView);

		UA_VisibilityEnum tmpUAVisibility;
		UA_VisibilityEnum_init(&tmpUAVisibility);

		UA_DataValue tmpUAValue;
		UA_DataValue_init(&tmpUAValue);

		if (result == AASSTATUSCODE_GOOD){

			tmpUAPVSName = UA_String_fromChars(tmpOVPVSName);

			if (tmpOVCarrierId.IdSpec)
				tmpUACarrierId.idSpec = UA_String_fromChars(tmpOVCarrierId.IdSpec);
			tmpUACarrierId.idType = tmpOVCarrierId.IdType;

			tmpUAExpressionLogic = tmpOVExpressionLogic;

			tmpUAExpressionSemantic = tmpOVExpressionSemantic;

			if (tmpOVPropertyId.IdSpec)
				tmpUAPropertyId.idSpec = UA_String_fromChars(tmpOVPropertyId.IdSpec);
			tmpUAPropertyId.idType = tmpOVPropertyId.IdType;

			tmpUAView = tmpOVView;

			tmpUAVisibility = tmpOVVisibility;

			ov_AnyToVariant(&tmpOVValue, &tmpUAValue.value);
			tmpUAValue.hasValue = TRUE;
			tmpUAValue.sourceTimestamp = ov_ovTimeTo1601nsTime(tmpOVValue.time);
			tmpUAValue.hasSourceTimestamp = TRUE;
		}

		UA_Variant_setScalarCopy(&output[0], &result, &UA_TYPES[UA_TYPES_STATUSCODE]);
		UA_Variant_setScalarCopy(&output[1], &tmpUAPVSName, &UA_TYPES[UA_TYPES_STRING]);
		UA_Variant_setScalarCopy(&output[2], &tmpUACarrierId, &UA_IDENTIFICATION[UA_IDENTIFICATION_IDENTIFICATION]);
		UA_Variant_setScalarCopy(&output[3], &tmpUAExpressionLogic, &UA_PROPERTYVALUESTATEMENT[UA_PROPERTYVALUESTATEMENT_EXPRESSIONLOGICENUM]);
		UA_Variant_setScalarCopy(&output[4], &tmpUAExpressionSemantic, &UA_PROPERTYVALUESTATEMENT[UA_PROPERTYVALUESTATEMENT_EXPRESSIONSEMANTICENUM]);
		UA_Variant_setScalarCopy(&output[5], &tmpUAPropertyId, &UA_IDENTIFICATION[UA_IDENTIFICATION_IDENTIFICATION]);
		UA_Variant_setScalarCopy(&output[6], &tmpUAView, &UA_PROPERTYVALUESTATEMENT[UA_PROPERTYVALUESTATEMENT_VIEWENUM]);
		UA_Variant_setScalarCopy(&output[7], &tmpUAVisibility, &UA_PROPERTYVALUESTATEMENT[UA_PROPERTYVALUESTATEMENT_VISIBILITYENUM]);
		UA_Variant_setScalarCopy(&output[8], &tmpUAValue, &UA_TYPES[UA_TYPES_DATAVALUE]);


		UA_DataValue_deleteMembers(&tmpUAValue);
		UA_String_deleteMembers(&tmpUAPVSName);
		UA_Identification_deleteMembers(&tmpUACarrierId);
		UA_Identification_deleteMembers(&tmpUAPropertyId);
		IdentificationType_deleteMembers(&tmpOVAASId);
		IdentificationType_deleteMembers(&tmpOVPVSlId);
		ov_database_free(tmpOVPVSName);
		IdentificationType_deleteMembers(&tmpOVCarrierId);
		IdentificationType_deleteMembers(&tmpOVPropertyId);
		Ov_SetAnyValue(&tmpOVValue, NULL);
		resultOV = OV_ERR_OK;
	}else if (ov_string_compare(funcName, "setPVS") == OV_STRCMP_EQUAL){
		UA_Identification *tmpUAAASId = (UA_Identification*)(input[0].data);
		UA_Identification *tmpUAPVSId = (UA_Identification*)(input[1].data);
		UA_UInt32 *tmpUAMask = (UA_UInt32*)(input[2].data);
		UA_String *tmpUAPVSName = (UA_String*)(input[3].data);
		UA_Identification *tmpUACarrierId = (UA_Identification*)(input[8].data);
		UA_ExpressionLogicEnum *tmpUAExpressionLogic = (UA_ExpressionLogicEnum*)(input[5].data);
		UA_ExpressionSemanticEnum *tmpUAExpressionSemantic = (UA_ExpressionSemanticEnum*)(input[6].data);
		UA_Identification *tmpUAPropertyId = (UA_Identification*)(input[7].data);
		UA_ViewEnum *tmpUAView = (UA_ViewEnum*)(input[8].data);
		UA_VisibilityEnum *tmpUAVisibility = (UA_VisibilityEnum*)(input[9].data);
		UA_DataValue *tmpUAValue = (UA_DataValue*)(input[10].data);

		IdentificationType tmpOVAASId;
		IdentificationType_init(&tmpOVAASId);
		copyOPCUAStringToOV(tmpUAAASId->idSpec, &(tmpOVAASId.IdSpec));
		tmpOVAASId.IdType = tmpUAAASId->idType;

		IdentificationType tmpOVPVSId;
		IdentificationType_init(&tmpOVPVSId);
		copyOPCUAStringToOV(tmpUAPVSId->idSpec, &(tmpOVPVSId.IdSpec));
		tmpOVPVSId.IdType = tmpUAPVSId->idType;

		OV_UINT tmpOVMask;
		tmpOVMask = *tmpUAMask;

		OV_STRING tmpOVPVSName = NULL;
		copyOPCUAStringToOV(*tmpUAPVSName, &tmpOVPVSName);

		IdentificationType tmpOVCarrierId;
		IdentificationType_init(&tmpOVCarrierId);
		copyOPCUAStringToOV(tmpUACarrierId->idSpec, &(tmpOVCarrierId.IdSpec));
		tmpOVCarrierId.IdType = tmpUACarrierId->idType;

		ExpressionLogicEnum tmpOVExpressionLogic;
		tmpOVExpressionLogic = *tmpUAExpressionLogic;

		ExpressionSemanticEnum tmpOVExpressionSemantic;
		tmpOVExpressionSemantic = *tmpUAExpressionSemantic;

		IdentificationType tmpOVPropertyId;
		IdentificationType_init(&tmpOVPropertyId);
		copyOPCUAStringToOV(tmpUAPropertyId->idSpec, &(tmpOVPropertyId.IdSpec));
		tmpOVPropertyId.IdType = tmpUAPropertyId->idType;

		ViewEnum tmpOVView;
		tmpOVView = *tmpUAView;

		VisibilityEnum tmpOVVisibility;
		tmpOVVisibility = *tmpUAVisibility;

		OV_ANY tmpOVValue;
		ov_VariantToAny(&(tmpUAValue->value), &tmpOVValue);
		tmpOVValue.time = ov_1601nsTimeToOvTime(tmpUAValue->sourceTimestamp);


		result = openaas_modelmanager_setPVS(tmpOVAASId, tmpOVPVSId, tmpOVMask, tmpOVPVSName, tmpOVCarrierId, tmpOVExpressionLogic, tmpOVExpressionSemantic, tmpOVPropertyId, tmpOVView, tmpOVVisibility, tmpOVValue);

		UA_Variant_setScalarCopy(&output[0], &result, &UA_TYPES[UA_TYPES_STATUSCODE]);
		IdentificationType_deleteMembers(&tmpOVAASId);
		IdentificationType_deleteMembers(&tmpOVPVSId);
		ov_database_free(tmpOVPVSName);
		IdentificationType_deleteMembers(&tmpOVCarrierId);
		IdentificationType_deleteMembers(&tmpOVPropertyId);
		Ov_SetAnyValue(&tmpOVValue, NULL);
		resultOV = OV_ERR_OK;
	}else if (ov_string_compare(funcName, "triggerGetCoreData") == OV_STRCMP_EQUAL){/*
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

		//result = openaas_modelmanager_triggerGetCoreData(tmpOVSourceAAS, tmpOVTargetAAS);

		UA_Variant_setScalarCopy(&output[0], &result, &UA_TYPES[UA_TYPES_STATUSCODE]);
		IdentificationType_deleteMembers(&tmpOVSourceAAS);
		IdentificationType_deleteMembers(&tmpOVTargetAAS);*/
		resultOV = OV_ERR_OK;
	}else if (ov_string_compare(funcName, "startGetAssetLCEData") == OV_STRCMP_EQUAL){/*
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

		result = openaasOPCUAInterface_interface_startGetAssetLCEData(tmpOVAASId, tmpOVIPAddress, tmpOVAssetAASId);

		UA_Variant_setScalarCopy(&output[0], &result, &UA_TYPES[UA_TYPES_STATUSCODE]);
		IdentificationType_deleteMembers(&tmpOVAASId);
		IdentificationType_deleteMembers(&tmpOVAssetAASId);
		ov_database_free(tmpOVIPAddress);*/
		resultOV = OV_ERR_OK;
	}else if (ov_string_compare(funcName, "stopGetAssetLCEData") == OV_STRCMP_EQUAL){/*
		UA_Identification *tmpUAAASId = (UA_Identification*)(input[0].data);

		IdentificationType tmpOVAASId;
		IdentificationType_init(&tmpOVAASId);
		copyOPCUAStringToOV(tmpUAAASId->idSpec, &(tmpOVAASId.IdSpec));
		tmpOVAASId.IdType = tmpUAAASId->idType;

		result = openaasOPCUAInterface_interface_stopGetAssetLCEData(tmpOVAASId);

		UA_Variant_setScalarCopy(&output[0], &result, &UA_TYPES[UA_TYPES_STATUSCODE]);
		IdentificationType_deleteMembers(&tmpOVAASId);
		*/
		resultOV = OV_ERR_OK;
	}else{
		resultOV = OV_ERR_BADPARAM;
	}

    return resultOV;
}


OV_DLLFNCEXPORT UA_StatusCode openaasOPCUAInterface_interface_MethodCallback(void *methodHandle, const UA_NodeId *objectId,
                     const UA_NodeId *sessionId, void *sessionHandle,
                     size_t inputSize, const UA_Variant *input,
                     size_t outputSize, UA_Variant *output) {
	UA_String tmpString;
	UA_String *tmpStringArray = NULL;
	UA_StatusCode result = UA_STATUSCODE_GOOD;

	OV_VTBLPTR_openaas_Service pvtable;
	OV_INSTPTR_openaas_Service pService = (OV_INSTPTR_openaas_Service)methodHandle;
	Ov_GetVTablePtr(openaas_Service, pvtable, pService);

	void **inputs = malloc(sizeof(void*)*inputSize);
	OV_UINT inputCounts = 0;
	for (int i = 0; i < inputSize; i++){
		inputCounts++;
		inputs[i] = NULL;
		if (input[i].arrayLength == 0){
			switch (input[i].type->typeIndex){
				case UA_TYPES_BOOLEAN:
				case UA_TYPES_INT32:
				case UA_TYPES_UINT32:
				case UA_TYPES_FLOAT:
				case UA_TYPES_DOUBLE:
					inputs[i] = input[i].data;
					break;
				case UA_TYPES_STRING:
					if ((*((UA_String*)(input[i].data))).length == 0){
						result = UA_STATUSCODE_BADARGUMENTSMISSING;
						goto cleanup;
					}
					inputs[i] = malloc(sizeof(OV_STRING));
					copyOPCUAStringToOV(*((UA_String*)(input[i].data)), (OV_STRING*)(inputs[i]));
					break;
				default:
					break;
			}
		}else{
			switch (input[i].type->typeIndex){
				case UA_TYPES_BOOLEAN:
					if (input[i].data == NULL){
						result = UA_STATUSCODE_BADARGUMENTSMISSING;
						goto cleanup;
					}
					inputs[i] = malloc(sizeof(OV_BOOL_VEC));
					(*(OV_BOOL_VEC*)(inputs[i])).veclen = input[i].arrayLength;
					(*(OV_BOOL_VEC*)(inputs[i])).value = input[i].data;
					break;
				case UA_TYPES_INT32:
					if (input[i].data == NULL){
						result = UA_STATUSCODE_BADARGUMENTSMISSING;
						goto cleanup;
					}
					inputs[i] = malloc(sizeof(OV_INT_VEC));
					(*(OV_INT_VEC*)(inputs[i])).veclen = input[i].arrayLength;
					(*(OV_INT_VEC*)(inputs[i])).value = input[i].data;
					break;
				case UA_TYPES_UINT32:
					if (input[i].data == NULL){
						result = UA_STATUSCODE_BADARGUMENTSMISSING;
						goto cleanup;
					}
					inputs[i] = malloc(sizeof(OV_UINT_VEC));
					(*(OV_UINT_VEC*)(inputs[i])).veclen = input[i].arrayLength;
					(*(OV_UINT_VEC*)(inputs[i])).value = input[i].data;
					break;
				case UA_TYPES_FLOAT:
					if (input[i].data == NULL){
						result = UA_STATUSCODE_BADARGUMENTSMISSING;
						goto cleanup;
					}
					inputs[i] = malloc(sizeof(OV_SINGLE_VEC));
					(*(OV_SINGLE_VEC*)(inputs[i])).veclen = input[i].arrayLength;
					(*(OV_SINGLE_VEC*)(inputs[i])).value = input[i].data;
					break;
				case UA_TYPES_DOUBLE:
					if (input[i].data == NULL){
						result = UA_STATUSCODE_BADARGUMENTSMISSING;
						goto cleanup;
					}
					inputs[i] = malloc(sizeof(OV_DOUBLE_VEC));
					(*(OV_DOUBLE_VEC*)(inputs[i])).veclen = input[i].arrayLength;
					(*(OV_DOUBLE_VEC*)(inputs[i])).value = input[i].data;
					break;
				case UA_TYPES_STRING:
					if (input[i].data == NULL){
						result = UA_STATUSCODE_BADARGUMENTSMISSING;
						goto cleanup;
					}
					inputs[i] = malloc(sizeof(OV_STRING_VEC));
					(*(OV_STRING_VEC*)(inputs[i])).veclen = input[i].arrayLength;
					(*(OV_STRING_VEC*)(inputs[i])).value = malloc(input[i].arrayLength*sizeof(OV_STRING));
					for (OV_UINT j = 0; j < input[i].arrayLength; j++){
						(*(OV_STRING_VEC*)(inputs[i])).value[j] = NULL;
						if (((UA_String*)(input[i].data))[j].length == 0){
							result = UA_STATUSCODE_BADARGUMENTSMISSING;
							goto cleanup;
						}
						copyOPCUAStringToOV(((UA_String*)(input[i].data))[j], &((*(OV_STRING_VEC*)(inputs[i])).value[j]));
					}
					break;
				default:
					break;
			}
		}
	}
	void **outputs = malloc(sizeof(void*)*outputSize);
	for (int i = 0; i < outputSize; i++){
		outputs[i] = NULL;
	}
	OV_UINT *typeArray= ov_database_malloc(sizeof(OV_UINT)*outputSize);
	pvtable->m_CallMethod(pService, inputSize, (const void**)inputs, outputSize, outputs, typeArray);


	for (int i = 0; i < outputSize; i++){
		switch (typeArray[i]){
		case OV_VT_BOOL:
			UA_Variant_setScalarCopy(&output[i], outputs[i], &UA_TYPES[UA_TYPES_BOOLEAN]);
			if (outputs[i])
				ov_database_free(outputs[i]);
			break;
		case OV_VT_INT:
			UA_Variant_setScalarCopy(&output[i], outputs[i], &UA_TYPES[UA_TYPES_INT32]);
			if (outputs[i])
				ov_database_free(outputs[i]);
			break;
		case OV_VT_UINT:
			UA_Variant_setScalarCopy(&output[i], outputs[i], &UA_TYPES[UA_TYPES_UINT32]);
			if (outputs[i])
				ov_database_free(outputs[i]);
			break;
		case OV_VT_SINGLE:
			UA_Variant_setScalarCopy(&output[i], outputs[i], &UA_TYPES[UA_TYPES_FLOAT]);
			if (outputs[i])
				ov_database_free(outputs[i]);
			break;
		case OV_VT_DOUBLE:
			UA_Variant_setScalarCopy(&output[i], outputs[i], &UA_TYPES[UA_TYPES_DOUBLE]);
			if (outputs[i])
				ov_database_free(outputs[i]);
			break;
		case OV_VT_STRING:
			tmpString = UA_String_fromChars(*(OV_STRING*)outputs[i]);
			UA_Variant_setScalarCopy(&output[i], &tmpString, &UA_TYPES[UA_TYPES_STRING]);
			UA_String_deleteMembers(&tmpString);
			if (*(OV_STRING*)outputs[i])
				ov_database_free(*(OV_STRING*)outputs[i]);
			if (outputs[i])
				ov_database_free(outputs[i]);
			break;
		case OV_VT_BOOL_VEC:
			UA_Variant_setArrayCopy(&output[i], (*(OV_GENERIC_VEC*)(outputs[i])).value, (*(OV_GENERIC_VEC*)(outputs[i])).veclen, &UA_TYPES[UA_TYPES_BOOLEAN]);
			if ((*(OV_GENERIC_VEC*)(outputs[i])).value)
				ov_database_free((*(OV_GENERIC_VEC*)(outputs[i])).value);
			if (outputs[i])
				ov_database_free(outputs[i]);
			break;
		case OV_VT_INT_VEC:
			UA_Variant_setArrayCopy(&output[i], (*(OV_GENERIC_VEC*)(outputs[i])).value, (*(OV_GENERIC_VEC*)(outputs[i])).veclen, &UA_TYPES[UA_TYPES_INT32]);
			if ((*(OV_GENERIC_VEC*)(outputs[i])).value)
				ov_database_free((*(OV_GENERIC_VEC*)(outputs[i])).value);
			if (outputs[i])
				ov_database_free(outputs[i]);
			break;
		case OV_VT_UINT_VEC:
			UA_Variant_setArrayCopy(&output[i], (*(OV_GENERIC_VEC*)(outputs[i])).value, (*(OV_GENERIC_VEC*)(outputs[i])).veclen, &UA_TYPES[UA_TYPES_UINT32]);
			if ((*(OV_GENERIC_VEC*)(outputs[i])).value)
				ov_database_free((*(OV_GENERIC_VEC*)(outputs[i])).value);
			if (outputs[i])
				ov_database_free(outputs[i]);
			break;
		case OV_VT_SINGLE_VEC:
			UA_Variant_setArrayCopy(&output[i], (*(OV_GENERIC_VEC*)(outputs[i])).value, (*(OV_GENERIC_VEC*)(outputs[i])).veclen, &UA_TYPES[UA_TYPES_FLOAT]);
			if ((*(OV_GENERIC_VEC*)(outputs[i])).value)
				ov_database_free((*(OV_GENERIC_VEC*)(outputs[i])).value);
			if (outputs[i])
				ov_database_free(outputs[i]);
			break;
		case OV_VT_DOUBLE_VEC:
			UA_Variant_setArrayCopy(&output[i], (*(OV_GENERIC_VEC*)(outputs[i])).value, (*(OV_GENERIC_VEC*)(outputs[i])).veclen, &UA_TYPES[UA_TYPES_DOUBLE]);
			if ((*(OV_GENERIC_VEC*)(outputs[i])).value)
				ov_database_free((*(OV_GENERIC_VEC*)(outputs[i])).value);
			if (outputs[i])
				ov_database_free(outputs[i]);
			break;
		case OV_VT_STRING_VEC:
			tmpStringArray = UA_Array_new((*(OV_GENERIC_VEC*)(outputs[i])).veclen, &UA_TYPES[UA_TYPES_STRING]);
			for (OV_UINT j = 0; j < (*(OV_GENERIC_VEC*)(outputs[i])).veclen; j++){
				tmpStringArray[j] = UA_String_fromChars((*(OV_STRING_VEC*)outputs[i]).value[j]);
			}
			UA_Variant_setArrayCopy(&output[i], tmpStringArray, (*(OV_GENERIC_VEC*)(outputs[i])).veclen, &UA_TYPES[UA_TYPES_STRING]);
			UA_Array_delete(tmpStringArray, (*(OV_GENERIC_VEC*)(outputs[i])).veclen,&UA_TYPES[UA_TYPES_STRING]);

			if ((*(OV_STRING_VEC*)(outputs[i])).value)
				for (int k = 0; k < (*(OV_STRING_VEC*)(outputs[i])).veclen; k++){
					ov_database_free(((*(OV_STRING_VEC*)(outputs[i])).value)[k]);
				}
			if ((*(OV_GENERIC_VEC*)(outputs[i])).value)
				ov_database_free((*(OV_GENERIC_VEC*)(outputs[i])).value);
			if (outputs[i])
				ov_database_free(outputs[i]);
			break;
		default:
			break;
		}
	}

	free(outputs);

	cleanup:
    	for (int i = 0; i < inputCounts; i++){
			if (input[i].arrayLength == 0){
				switch (input[i].type->typeIndex){
					case UA_TYPES_BOOLEAN:
					case UA_TYPES_INT32:
					case UA_TYPES_UINT32:
					case UA_TYPES_FLOAT:
					case UA_TYPES_DOUBLE:
						inputs[i] = NULL;
						break;
					case UA_TYPES_STRING:
						if (inputs[i] != NULL)
							ov_database_free(*(OV_STRING*)(inputs[i]));
						break;
					default:
						break;
				}
			}else{
				switch (input[i].type->typeIndex){
					case UA_TYPES_BOOLEAN:
						if (inputs[i] != NULL){
							(*(OV_BOOL_VEC*)(inputs[i])).veclen = 0;
							(*(OV_BOOL_VEC*)(inputs[i])).value = NULL;
							free(inputs[i]);
						}
						break;
					case UA_TYPES_INT32:
						if (inputs[i] != NULL){
							(*(OV_INT_VEC*)(inputs[i])).veclen = 0;
							(*(OV_INT_VEC*)(inputs[i])).value = NULL;
							free(inputs[i]);
						}
						break;
					case UA_TYPES_UINT32:
						if (inputs[i] != NULL){
							(*(OV_UINT_VEC*)(inputs[i])).veclen = 0;
							(*(OV_UINT_VEC*)(inputs[i])).value = NULL;
							free(inputs[i]);
						}
						break;
					case UA_TYPES_FLOAT:
						if (inputs[i] != NULL){
							(*(OV_SINGLE_VEC*)(inputs[i])).veclen = 0;
							(*(OV_SINGLE_VEC*)(inputs[i])).value = NULL;
							free(inputs[i]);
						}
						break;
					case UA_TYPES_DOUBLE:
						if (inputs[i] != NULL){
							(*(OV_DOUBLE_VEC*)(inputs[i])).veclen = 0;
							(*(OV_DOUBLE_VEC*)(inputs[i])).value = NULL;
							free(inputs[i]);
						}
						break;
					case UA_TYPES_STRING:
						if (inputs[i] != NULL){

							for (OV_UINT j = 0; j < (*(OV_STRING_VEC*)(inputs[i])).veclen; j++){
								if ((*(OV_STRING_VEC*)(inputs[i])).value[j] != NULL)
									ov_database_free((*(OV_STRING_VEC*)(inputs[i])).value[j]);
							}
							(*(OV_STRING_VEC*)(inputs[i])).value = NULL;
							(*(OV_STRING_VEC*)(inputs[i])).veclen = 0;
							free(inputs[i]);
						}
						break;
					default:
						break;
				}
			}
    	}
    free(inputs);

	return result;
}

static void OV_NodeStore_deleteNodestore(void *handle, UA_UInt16 namespaceIndex){

}

static void OV_NodeStore_deleteNode(UA_Node *node){
	if (node){
		OV_STRING tmpString = NULL;
		if (node->nodeId.identifierType == UA_NODEIDTYPE_STRING)
			copyOPCUAStringToOV(node->nodeId.identifier.string, &tmpString);
		else
			ov_string_print(&tmpString, "%i", node->nodeId.identifier.numeric);

		ov_logfile_error("Delete Node: %s", tmpString);
		ov_string_setvalue(&tmpString, NULL);
		UA_Node_deleteMembersAnyNodeClass(node);
	}
	UA_free(node);
}
static void OV_NodeStore_releaseNode(void *handle, const UA_Node *node){
	OV_NodeStore_deleteNode((UA_Node*)node);
}

static UA_Node * OV_NodeStore_newNode(UA_NodeClass nodeClass){ //TODO add nodestore handle? --> move nodeStore from static context to main
    //allocate memory for a new node
	//UA_Node *newNode = NULL;
	//newNode = (UA_Node*) ov_database_malloc(sizeof(UA_ObjectNode));
	//newNode->nodeClass = nodeClass;
    return NULL;
}
static const UA_Node * OV_NodeStore_getNode(void *handle, const UA_NodeId *nodeId){
	UA_Node * tmpNode = NULL;
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
	if (nodeId->identifier.string.data == NULL || nodeId->identifier.string.length == 0 || nodeId->identifierType != UA_NODEIDTYPE_STRING)
		return NULL;
	copyOPCUAStringToOV(nodeId->identifier.string, &tmpString);
	plist = ov_string_split(tmpString, "/", &len);
	plist2 = ov_string_split(plist[len-1], ".", &len2);
	plist3 = ov_string_split(tmpString, "||", &len3);
	plist4 = ov_string_split(tmpString, "|", &len4);

	if (len3 > 1){
		pobj = ov_path_getobjectpointer(plist3[0], 2);
		if (pobj == NULL){
			ov_string_freelist(plist);
			ov_string_freelist(plist2);
			ov_string_freelist(plist3);
			ov_string_freelist(plist4);
			ov_string_setvalue(&tmpString, NULL);
			return NULL;
		}
		if (Ov_CanCastTo(openaas_modelmanager, pobj)){
			if (openaasOPCUAInterface_interface_ovModelManagerMethodNodeToOPCUA(NULL, nodeId, &opcuaNode) == UA_STATUSCODE_GOOD){
				ov_string_freelist(plist);
				ov_string_freelist(plist2);
				ov_string_freelist(plist3);
				ov_string_freelist(plist4);
				ov_string_setvalue(&tmpString, NULL);
				return (UA_Node*) opcuaNode;
			}
		}
	}else if (len4 > 1 ){
		pobj = ov_path_getobjectpointer(plist4[0], 2);
		if (pobj == NULL){
			ov_string_freelist(plist);
			ov_string_freelist(plist2);
			ov_string_freelist(plist3);
			ov_string_freelist(plist4);
			ov_string_setvalue(&tmpString, NULL);
			return NULL;
		}
	}else{
		pobj = ov_path_getobjectpointer(tmpString, 2);
		if (pobj == NULL){
			ov_string_freelist(plist);
			ov_string_freelist(plist2);
			ov_string_freelist(plist3);
			ov_string_freelist(plist4);
			ov_string_setvalue(&tmpString, NULL);
			return NULL;
		}
	}

	if (ov_string_compare(plist2[len2-1], "Views") == OV_STRCMP_EQUAL){
		if (openaasOPCUAInterface_interface_ovViewsNodeToOPCUA(NULL, nodeId, &opcuaNode) == UA_STATUSCODE_GOOD)
			tmpNode = opcuaNode;
	}else if (ov_string_compare(plist2[len2-1], "Header") == OV_STRCMP_EQUAL){
		if (openaasOPCUAInterface_interface_ovHeaderNodeToOPCUA(NULL, nodeId, &opcuaNode) == UA_STATUSCODE_GOOD)
			tmpNode = opcuaNode;
	}else if (ov_string_compare(plist2[len2-1], "Body") == OV_STRCMP_EQUAL){
		if (openaasOPCUAInterface_interface_ovBodyNodeToOPCUA(NULL, nodeId, &opcuaNode) == UA_STATUSCODE_GOOD)
			tmpNode = opcuaNode;
	}else if(Ov_CanCastTo(openaas_aas, pobj)){
		if (openaasOPCUAInterface_interface_ovAASNodeToOPCUA(NULL, nodeId, &opcuaNode) == UA_STATUSCODE_GOOD)
			tmpNode = opcuaNode;
	}else if(Ov_CanCastTo(openaas_SubModel, pobj)){
		if (openaasOPCUAInterface_interface_ovSubModelNodeToOPCUA(NULL, nodeId, &opcuaNode) == UA_STATUSCODE_GOOD)
			tmpNode = opcuaNode;
	}else if (Ov_CanCastTo(openaas_modelmanager, pobj)){
		if (openaasOPCUAInterface_interface_ovModelManagerNodeToOPCUA(NULL, nodeId, &opcuaNode) == UA_STATUSCODE_GOOD)
			tmpNode = opcuaNode;
	}

	ov_string_freelist(plist);
	ov_string_freelist(plist2);
	ov_string_freelist(plist3);
	ov_string_freelist(plist4);
	ov_string_setvalue(&tmpString, NULL);
	return tmpNode;
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
	return UA_STATUSCODE_BADNOTIMPLEMENTED;
}
static void OV_NodeStore_iterate(void *handle, void* visitorHandle, UA_NodestoreInterface_nodeVisitor visitor){

}
static UA_StatusCode OV_NodeStore_linkNamespace(void *handle, UA_UInt16 namespaceIndex){
	return UA_STATUSCODE_BADNOTIMPLEMENTED;
}
static UA_StatusCode OV_NodeStore_unlinkNamespace(void *handle, UA_UInt16 namespaceIndex){
	return UA_STATUSCODE_BADNOTIMPLEMENTED;
}

UA_NodestoreInterface* openaasOPCUAInterface_interface_ovNodeStoreInterfaceOpenAASNew(void) {
	UA_NodestoreInterface *nsi = ov_database_malloc(sizeof(UA_NodestoreInterface));
    nsi->handle =        	NULL;
    nsi->deleteNodestore =  (UA_NodestoreInterface_deleteNodeStore) 		OV_NodeStore_deleteNodestore;
    nsi->newNode =       	(UA_NodestoreInterface_newNode)     OV_NodeStore_newNode;
    nsi->deleteNode =    	(UA_NodestoreInterface_deleteNode)  OV_NodeStore_deleteNode;
    nsi->insertNode =       	(UA_NodestoreInterface_insertNode)      OV_NodeStore_insertNode;
    nsi->getNode =          	(UA_NodestoreInterface_getNode)         OV_NodeStore_getNode;
    nsi->getNodeCopy =      	(UA_NodestoreInterface_getNodeCopy)     OV_NodeStore_getCopyNode;
    nsi->replaceNode =      	(UA_NodestoreInterface_replaceNode)     OV_NodeStore_replaceNode;
    nsi->removeNode =       	(UA_NodestoreInterface_removeNode)      OV_NodeStore_removeNode;
    nsi->iterate =       (UA_NodestoreInterface_iterate)     OV_NodeStore_iterate;
    nsi->releaseNode =       	(UA_NodestoreInterface_releaseNode)     OV_NodeStore_releaseNode;
    nsi->linkNamespace =       (UA_NodestoreInterface_linkNamespace)     OV_NodeStore_linkNamespace;
	nsi->unlinkNamespace =       	(UA_NodestoreInterface_unlinkNamespace)     OV_NodeStore_unlinkNamespace;
	return nsi;
}

void openaasOPCUAInterface_interface_ovNodeStoreInterfaceOpenAASDelete(UA_NodestoreInterface * nsi){
	if (nsi->handle)
		UA_free(nsi->handle);
}
