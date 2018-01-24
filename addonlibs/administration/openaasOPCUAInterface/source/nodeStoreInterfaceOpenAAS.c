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
#include "servicesOPCUAInterface.h"


OV_DLLFNCEXPORT UA_StatusCode openaasOPCUAInterface_interface_MethodCallbackModelmanager(void *methodHandle, const UA_NodeId *objectId,
                     const UA_NodeId *sessionId, void *sessionHandle,
                     size_t inputSize, const UA_Variant *input,
                     size_t outputSize, UA_Variant *output) {
	OV_RESULT resultOV = OV_ERR_OK;
	AASStatusCode result = AASSTATUSCODE_GOOD;
	OV_STRING funcName = (OV_STRING)methodHandle;

	if (ov_string_compare(funcName, "getAASNodeId") == OV_STRCMP_EQUAL){
		if (input[0].data == NULL){
			UA_NodeId tmpNodeId;
			UA_NodeId_init(&tmpNodeId);
			UA_Variant_setScalarCopy(&output[0], &tmpNodeId, &UA_TYPES[UA_TYPES_NODEID]);
			UA_NodeId_deleteMembers(&tmpNodeId);
			resultOV = OV_ERR_OK;
		}
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
		if (input[0].data == NULL ||
			input[1].data == NULL ||
			input[2].data == NULL){
			result = AASSTATUSCODE_MISSINGARGUMENT;
			UA_Variant_setScalarCopy(&output[0], &result, &UA_TYPES[UA_TYPES_STATUSCODE]);
			resultOV = OV_ERR_OK;
		}
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
		if (input[0].data == NULL){
			result = AASSTATUSCODE_MISSINGARGUMENT;
			UA_Variant_setScalarCopy(&output[0], &result, &UA_TYPES[UA_TYPES_STATUSCODE]);
			resultOV = OV_ERR_OK;
		}
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
		if (input[0].data == NULL ||
			input[1].data == NULL ||
			input[2].data == NULL){
			result = AASSTATUSCODE_MISSINGARGUMENT;
			UA_Variant_setScalarCopy(&output[0], &result, &UA_TYPES[UA_TYPES_STATUSCODE]);
			resultOV = OV_ERR_OK;
		}
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
		if (input[0].data == NULL ||
			input[1].data == NULL){
			result = AASSTATUSCODE_MISSINGARGUMENT;
			UA_Variant_setScalarCopy(&output[0], &result, &UA_TYPES[UA_TYPES_STATUSCODE]);
			resultOV = OV_ERR_OK;
		}

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
		if (input[0].data == NULL ||
			input[1].data == NULL ||
			input[2].data == NULL ||
			input[3].data == NULL ||
			input[4].data == NULL ||
			input[5].data == NULL ||
			input[6].data == NULL ||
			input[7].data == NULL ||
			input[8].data == NULL ||
			input[9].data == NULL){
			result = AASSTATUSCODE_MISSINGARGUMENT;
			UA_Variant_setScalarCopy(&output[0], &result, &UA_TYPES[UA_TYPES_STATUSCODE]);
			resultOV = OV_ERR_OK;
		}

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
		if (input[0].data == NULL ||
			input[1].data == NULL){
			result = AASSTATUSCODE_MISSINGARGUMENT;
			UA_Variant_setScalarCopy(&output[0], &result, &UA_TYPES[UA_TYPES_STATUSCODE]);
			resultOV = OV_ERR_OK;
		}

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
	}else if (ov_string_compare(funcName, "createPVS") == OV_STRCMP_EQUAL){
		if (input[0].data == NULL ||
			input[1].data == NULL ||
			input[2].data == NULL ||
			input[3].data == NULL ||
			input[4].data == NULL ||
			input[5].data == NULL ||
			input[6].data == NULL ||
			input[7].data == NULL ||
			input[8].data == NULL ||
			input[9].data == NULL ||
			input[10].data == NULL){
			result = AASSTATUSCODE_MISSINGARGUMENT;
			UA_Variant_setScalarCopy(&output[0], &result, &UA_TYPES[UA_TYPES_STATUSCODE]);
			resultOV = OV_ERR_OK;
		}
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
		if (input[0].data == NULL ||
			input[1].data == NULL){
			result = AASSTATUSCODE_MISSINGARGUMENT;
			UA_Variant_setScalarCopy(&output[0], &result, &UA_TYPES[UA_TYPES_STATUSCODE]);
			resultOV = OV_ERR_OK;
		}
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
		if (input[0].data == NULL ||
			input[1].data == NULL){
			result = AASSTATUSCODE_MISSINGARGUMENT;
			UA_Variant_setScalarCopy(&output[0], &result, &UA_TYPES[UA_TYPES_STATUSCODE]);

			UA_String tmpUAPVSName;
			UA_String_init(&tmpUAPVSName);
			UA_Variant_setScalarCopy(&output[1], &tmpUAPVSName, &UA_TYPES[UA_TYPES_STRING]);
			UA_String_deleteMembers(&tmpUAPVSName);

			UA_Identification tmpUACarrierId;
			UA_Identification_init(&tmpUACarrierId);
			UA_Variant_setScalarCopy(&output[2], &tmpUACarrierId, &UA_IDENTIFICATION[UA_IDENTIFICATION_IDENTIFICATION]);
			UA_Identification_deleteMembers(&tmpUACarrierId);

			UA_ExpressionLogicEnum tmpUAExpressionLogic;
			UA_ExpressionLogicEnum_init(&tmpUAExpressionLogic);
			UA_Variant_setScalarCopy(&output[3], &tmpUAExpressionLogic, &UA_PROPERTYVALUESTATEMENT[UA_PROPERTYVALUESTATEMENT_EXPRESSIONLOGICENUM]);
			UA_ExpressionLogicEnum_deleteMembers(&tmpUAExpressionLogic);

			UA_ExpressionSemanticEnum tmpUAExpressionSemantic;
			UA_ExpressionSemanticEnum_init(&tmpUAExpressionSemantic);
			UA_Variant_setScalarCopy(&output[4], &tmpUAExpressionSemantic, &UA_PROPERTYVALUESTATEMENT[UA_PROPERTYVALUESTATEMENT_EXPRESSIONSEMANTICENUM]);
			UA_ExpressionSemanticEnum_deleteMembers(&tmpUAExpressionSemantic);

			UA_Identification tmpUAPropertyId;
			UA_Identification_init(&tmpUAPropertyId);
			UA_Variant_setScalarCopy(&output[5], &tmpUAPropertyId, &UA_IDENTIFICATION[UA_IDENTIFICATION_IDENTIFICATION]);
			UA_Identification_deleteMembers(&tmpUAPropertyId);

			UA_ViewEnum tmpUAView;
			UA_ViewEnum_init(&tmpUAView);
			UA_Variant_setScalarCopy(&output[6], &tmpUAView, &UA_PROPERTYVALUESTATEMENT[UA_PROPERTYVALUESTATEMENT_VIEWENUM]);
			UA_ViewEnum_deleteMembers(&tmpUAView);

			UA_VisibilityEnum tmpUAVisibility;
			UA_VisibilityEnum_init(&tmpUAVisibility);
			UA_Variant_setScalarCopy(&output[7], &tmpUAVisibility, &UA_PROPERTYVALUESTATEMENT[UA_PROPERTYVALUESTATEMENT_VISIBILITYENUM]);
			UA_VisibilityEnum_deleteMembers(&tmpUAVisibility);

			UA_DataValue tmpUAValue;
			UA_DataValue_init(&tmpUAValue);
			UA_Variant_setScalarCopy(&output[8], &tmpUAValue, &UA_TYPES[UA_TYPES_DATAVALUE]);
			UA_DataValue_deleteMembers(&tmpUAValue);

			resultOV = OV_ERR_OK;
			resultOV = OV_ERR_OK;
		}
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
		if (input[0].data == NULL ||
			input[1].data == NULL ||
			input[2].data == NULL ||
			input[3].data == NULL ||
			input[4].data == NULL ||
			input[5].data == NULL ||
			input[6].data == NULL ||
			input[7].data == NULL ||
			input[8].data == NULL ||
			input[9].data == NULL ||
			input[10].data == NULL){
			result = AASSTATUSCODE_MISSINGARGUMENT;
			UA_Variant_setScalarCopy(&output[0], &result, &UA_TYPES[UA_TYPES_STATUSCODE]);
			resultOV = OV_ERR_OK;
		}
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

static void OV_NodeStore_deleteNodestore(void *handle, UA_UInt16 namespaceIndex){

}

static void OV_NodeStore_deleteNode(UA_Node *node){
	if (node){
		if (node->nodeClass == UA_NODECLASS_METHOD){
			ov_string_setvalue((OV_STRING*) (&(((UA_MethodNode*) node)->methodHandle)),	NULL);
		}
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
	OV_STRING tmpString = NULL;
	OV_INSTPTR_ov_object pobj = NULL;
	OV_UINT len = 0;
	OV_UINT len2 = 0;
	OV_UINT len3 = 0;
	if (nodeId->identifier.string.data == NULL || nodeId->identifier.string.length == 0 || nodeId->identifierType != UA_NODEIDTYPE_STRING)
		return NULL;
	copyOPCUAStringToOV(nodeId->identifier.string, &tmpString);
	plist = ov_string_split(tmpString, "/", &len);
	plist2 = ov_string_split(plist[len-1], ".", &len2);
	plist3 = ov_string_split(tmpString, "||", &len3);


	if (len3 > 1){
		pobj = ov_path_getobjectpointer(plist3[0], 2);
		if (pobj == NULL){
			ov_string_freelist(plist);
			ov_string_freelist(plist2);
			ov_string_freelist(plist3);
			ov_string_setvalue(&tmpString, NULL);
			return NULL;
		}
		if (Ov_CanCastTo(openaas_modelmanager, pobj)){
			if (openaasOPCUAInterface_interface_ovModelManagerMethodNodeToOPCUA(NULL, nodeId, &opcuaNode) == UA_STATUSCODE_GOOD){
				ov_string_freelist(plist);
				ov_string_freelist(plist2);
				ov_string_freelist(plist3);
				ov_string_setvalue(&tmpString, NULL);
				return (UA_Node*) opcuaNode;
			}
		}
	}else if (len2 > 1){
		if (ov_string_compare(plist2[len2-1], "Views") == OV_STRCMP_EQUAL){
			if (openaasOPCUAInterface_interface_ovFolderNodeToOPCUA(NULL, nodeId, &opcuaNode) == UA_STATUSCODE_GOOD)
				tmpNode = opcuaNode;
		}else if (ov_string_compare(plist2[len2-1], "Header") == OV_STRCMP_EQUAL){
			if (openaasOPCUAInterface_interface_ovFolderNodeToOPCUA(NULL, nodeId, &opcuaNode) == UA_STATUSCODE_GOOD)
				tmpNode = opcuaNode;
		}else if (ov_string_compare(plist2[len2-1], "Body") == OV_STRCMP_EQUAL){
			if (openaasOPCUAInterface_interface_ovFolderNodeToOPCUA(NULL, nodeId, &opcuaNode) == UA_STATUSCODE_GOOD)
				tmpNode = opcuaNode;
		}else if (ov_string_compare(plist2[len2-1], "Revision") == OV_STRCMP_EQUAL){
			if (openaasOPCUAInterface_interface_ovSubModelVariablesNodeToOPCUA(NULL, nodeId, &opcuaNode) == UA_STATUSCODE_GOOD)
				tmpNode = opcuaNode;
		}else if (ov_string_compare(plist2[len2-1], "ModelId") == OV_STRCMP_EQUAL){
			if (openaasOPCUAInterface_interface_ovSubModelVariablesNodeToOPCUA(NULL, nodeId, &opcuaNode) == UA_STATUSCODE_GOOD)
				tmpNode = opcuaNode;
		}else if (ov_string_compare(plist2[len2-1], "Version") == OV_STRCMP_EQUAL){
			if (openaasOPCUAInterface_interface_ovSubModelVariablesNodeToOPCUA(NULL, nodeId, &opcuaNode) == UA_STATUSCODE_GOOD)
				tmpNode = opcuaNode;
		}
	}else{
		pobj = ov_path_getobjectpointer(tmpString, 2);
		if (pobj == NULL){
			ov_string_freelist(plist);
			ov_string_freelist(plist2);
			ov_string_freelist(plist3);
			ov_string_setvalue(&tmpString, NULL);
			return NULL;
		}

		if(Ov_CanCastTo(openaas_aas, pobj)){
			if (openaasOPCUAInterface_interface_ovAASNodeToOPCUA(NULL, nodeId, &opcuaNode) == UA_STATUSCODE_GOOD)
				tmpNode = opcuaNode;
		}else if(Ov_CanCastTo(openaas_SubModel, pobj)){
			if (openaasOPCUAInterface_interface_ovSubModelNodeToOPCUA(NULL, nodeId, &opcuaNode) == UA_STATUSCODE_GOOD)
				tmpNode = opcuaNode;
		}else if (Ov_CanCastTo(openaas_modelmanager, pobj)){
			if (openaasOPCUAInterface_interface_ovModelManagerNodeToOPCUA(NULL, nodeId, &opcuaNode) == UA_STATUSCODE_GOOD)
				tmpNode = opcuaNode;
		}else if(Ov_CanCastTo(openaas_Folder, pobj)){
			if (openaasOPCUAInterface_interface_ovFolderNodeToOPCUA(NULL, nodeId, &opcuaNode) == UA_STATUSCODE_GOOD)
				tmpNode = opcuaNode;
		}else {
		}
	}

	ov_string_freelist(plist);
	ov_string_freelist(plist2);
	ov_string_freelist(plist3);
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
