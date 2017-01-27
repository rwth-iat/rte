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
		tmpOVAASId.IdSpec = NULL;
		copyOPCUAStringToOV(tmpUAAASId->idSpec, &(tmpOVAASId.IdSpec));
		tmpOVAASId.IdType = tmpUAAASId->idType;

		UA_NodeId tmpNodeId = openaas_modelmanager_getAASNodeId(tmpOVAASId);

		output->data = &tmpNodeId;
		resultOV = OV_ERR_OK;
	}else if (ov_string_compare(funcName, "createAAS") == OV_STRCMP_EQUAL){
		UA_Identification *tmpUAAASId = (UA_Identification*)(input[0].data);
		UA_String *tmpUAName = (UA_String*)(input[1].data);
		UA_Identification *tmpUAAssetId = (UA_Identification*)(input[2].data);

		IdentificationType tmpOVAASId;
		tmpOVAASId.IdSpec = NULL;
		copyOPCUAStringToOV(tmpUAAASId->idSpec, &(tmpOVAASId.IdSpec));
		tmpOVAASId.IdType = tmpUAAASId->idType;

		OV_STRING tmpOVName = NULL;
		copyOPCUAStringToOV(*tmpUAName, &tmpOVName);

		IdentificationType tmpOVAssetId;
		tmpOVAssetId.IdSpec = NULL;
		copyOPCUAStringToOV(tmpUAAssetId->idSpec, &(tmpOVAssetId.IdSpec));
		tmpOVAssetId.IdType = tmpUAAssetId->idType;

		result = openaas_modelmanager_createAAS(tmpOVAASId, tmpOVName, tmpOVAssetId);

		output->data = &result;
		resultOV = OV_ERR_OK;
	}else if (ov_string_compare(funcName, "deleteAAS") == OV_STRCMP_EQUAL){
		UA_Identification *tmpUAAASId = (UA_Identification*)(input[0].data);

		IdentificationType tmpOVAASId;
		tmpOVAASId.IdSpec = NULL;
		copyOPCUAStringToOV(tmpUAAASId->idSpec, &(tmpOVAASId.IdSpec));
		tmpOVAASId.IdType = tmpUAAASId->idType;

		result = openaas_modelmanager_deleteAAS(tmpOVAASId);

		output->data = &result;
		resultOV = OV_ERR_OK;
	}else if (ov_string_compare(funcName, "createPVSL") == OV_STRCMP_EQUAL){
		UA_Identification *tmpUAAASId = (UA_Identification*)(input[0].data);
		UA_String *tmpUAPVSLName = (UA_String*)(input[1].data);
		UA_Identification *tmpUACarrier = (UA_Identification*)(input[2].data);

		IdentificationType tmpOVAASId;
		tmpOVAASId.IdSpec = NULL;
		copyOPCUAStringToOV(tmpUAAASId->idSpec, &(tmpOVAASId.IdSpec));
		tmpOVAASId.IdType = tmpUAAASId->idType;

		OV_STRING tmpOVPVSLName = NULL;
		copyOPCUAStringToOV(*tmpUAPVSLName, &tmpOVPVSLName);

		IdentificationType tmpOVCarrier;
		tmpOVCarrier.IdSpec = NULL;
		copyOPCUAStringToOV(tmpUACarrier->idSpec, &(tmpOVCarrier.IdSpec));
		tmpOVCarrier.IdType = tmpUACarrier->idType;

		result = openaas_modelmanager_createPVSL(tmpOVAASId, tmpOVPVSLName, tmpOVCarrier);

		output->data = &result;
		resultOV = OV_ERR_OK;
	}else if (ov_string_compare(funcName, "deletePVSL") == OV_STRCMP_EQUAL){
		UA_Identification *tmpUAAASId = (UA_Identification*)(input[0].data);
		UA_String *tmpUAPVSLName = (UA_String*)(input[1].data);

		IdentificationType tmpOVAASId;
		tmpOVAASId.IdSpec = NULL;
		copyOPCUAStringToOV(tmpUAAASId->idSpec, &(tmpOVAASId.IdSpec));
		tmpOVAASId.IdType = tmpUAAASId->idType;

		OV_STRING tmpOVPVSLName = NULL;
		copyOPCUAStringToOV(*tmpUAPVSLName, &tmpOVPVSLName);

		result = openaas_modelmanager_deletePVSL(tmpOVAASId, tmpOVPVSLName);

		output->data = &result;
		resultOV = OV_ERR_OK;
	}else if (ov_string_compare(funcName, "createLCE") == OV_STRCMP_EQUAL){
		UA_Identification *tmpUAAASId = (UA_Identification*)(input[0].data);
		UA_Identification *tmpUACreatingInstance = (UA_Identification*)(input[1].data);
		UA_Identification *tmpUAWritingInstance = (UA_Identification*)(input[2].data);
		UA_String *tmpUAEventClass = (UA_String*)(input[3].data);
		UA_String *tmpUASubject = (UA_String*)(input[4].data);
		UA_DataValue *tmpUAValue = (UA_DataValue*)(input[5].data);

		IdentificationType tmpOVAASId;
		tmpOVAASId.IdSpec = NULL;
		copyOPCUAStringToOV(tmpUAAASId->idSpec, &(tmpOVAASId.IdSpec));
		tmpOVAASId.IdType = tmpUAAASId->idType;

		IdentificationType tmpOVCreatingInstance;
		tmpOVCreatingInstance.IdSpec = NULL;
		copyOPCUAStringToOV(tmpUACreatingInstance->idSpec, &(tmpOVCreatingInstance.IdSpec));
		tmpOVCreatingInstance.IdType = tmpUACreatingInstance->idType;

		OV_STRING tmpOVEventClass = NULL;
		copyOPCUAStringToOV(*tmpUAEventClass, &tmpOVEventClass);

		OV_STRING tmpOVSubject = NULL;
		copyOPCUAStringToOV(*tmpUASubject, &tmpOVSubject);

		IdentificationType tmpOVWritingInstance;
		tmpOVWritingInstance.IdSpec = NULL;
		copyOPCUAStringToOV(tmpUAWritingInstance->idSpec, &(tmpOVWritingInstance.IdSpec));
		tmpOVWritingInstance.IdType = tmpUAWritingInstance->idType;

		DataValue tmpOVDataValue;
		ov_VariantToAny(&(tmpUAValue->value), &tmpOVDataValue.Value);
		tmpOVDataValue.TimeStamp = tmpUAValue->sourceTimestamp;

		result = openaas_modelmanager_createLCE(tmpOVAASId, tmpOVCreatingInstance, tmpOVWritingInstance, tmpOVEventClass, tmpOVSubject, tmpOVDataValue);

		output->data = &result;
		resultOV = OV_ERR_OK;
	}else if (ov_string_compare(funcName, "deleteLCE") == OV_STRCMP_EQUAL){
		UA_Identification *tmpUAAASId = (UA_Identification*)(input[0].data);
		UA_UInt64 *tmpUALCEId = (UA_UInt64*)(input[1].data);

		IdentificationType tmpOVAASId;
		tmpOVAASId.IdSpec = NULL;
		copyOPCUAStringToOV(tmpUAAASId->idSpec, &(tmpOVAASId.IdSpec));
		tmpOVAASId.IdType = tmpUAAASId->idType;

		OV_UINT64 tmpOVLCEId;
		tmpOVLCEId = *tmpUALCEId;

		result = openaas_modelmanager_deleteLCE(tmpOVAASId, tmpOVLCEId);

		output->data = &result;
		resultOV = OV_ERR_OK;
	}else if (ov_string_compare(funcName, "getLCE") == OV_STRCMP_EQUAL){
		UA_Identification *tmpUAAASId = (UA_Identification*)(input[0].data);
		UA_UInt64 *tmpUALCEId = (UA_UInt64*)(input[1].data);

		IdentificationType tmpOVAASId;
		tmpOVAASId.IdSpec = NULL;
		copyOPCUAStringToOV(tmpUAAASId->idSpec, &(tmpOVAASId.IdSpec));
		tmpOVAASId.IdType = tmpUAAASId->idType;

		OV_UINT64 tmpOVLCEId;
		tmpOVLCEId = *tmpUALCEId;

		IdentificationType tmpOVCreatingInstance;
		tmpOVCreatingInstance.IdSpec = NULL;

		OV_STRING tmpOVEventClass = NULL;

		OV_STRING tmpOVSubject = NULL;

		IdentificationType tmpOVWritingInstance;
		tmpOVWritingInstance.IdSpec = NULL;

		DataValue tmpOVDataValue;

		result = openaas_modelmanager_getLCE(tmpOVAASId, tmpOVLCEId, &tmpOVCreatingInstance, &tmpOVWritingInstance, &tmpOVEventClass, &tmpOVSubject, &tmpOVDataValue);

		UA_Identification tmpUACreatingInstance;
		copyOvStringToOPCUA(tmpOVCreatingInstance.IdSpec, &tmpUACreatingInstance.idSpec);
		tmpUACreatingInstance.idType = tmpOVCreatingInstance.IdType;

		UA_Identification tmpUAWritingInstance;
		copyOvStringToOPCUA(tmpOVWritingInstance.IdSpec, &tmpUAWritingInstance.idSpec);
		tmpUAWritingInstance.idType = tmpOVWritingInstance.IdType;

		UA_String tmpUAEventClass;
		copyOvStringToOPCUA(tmpOVEventClass, &tmpUAEventClass);

		UA_String tmpUASubject;
		copyOvStringToOPCUA(tmpOVSubject, &tmpUASubject);

		UA_DataValue tmpUAValue;
		ov_AnyToVariant(&tmpOVDataValue.Value, &tmpUAValue.value);
		tmpUAValue.sourceTimestamp = tmpOVDataValue.TimeStamp;

		output[0].data = &tmpUACreatingInstance;
		output[1].data = &tmpUAWritingInstance;
		output[2].data = &tmpUAEventClass;
		output[3].data = &tmpUASubject;
		output[4].data = &tmpUAValue;
		output[5].data = &result;
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
		tmpOVAASId.IdSpec = NULL;
		copyOPCUAStringToOV(tmpUAAASId->idSpec, &(tmpOVAASId.IdSpec));
		tmpOVAASId.IdType = tmpUAAASId->idType;

		OV_UINT64 tmpOVLCEId;
		tmpOVLCEId = *tmpUALCEId;

		IdentificationType tmpOVCreatingInstance;
		tmpOVCreatingInstance.IdSpec = NULL;
		copyOPCUAStringToOV(tmpUACreatingInstance->idSpec, &(tmpOVCreatingInstance.IdSpec));
		tmpOVCreatingInstance.IdType = tmpUACreatingInstance->idType;

		OV_STRING tmpOVEventClass = NULL;
		copyOPCUAStringToOV(*tmpUAEventClass, &tmpOVEventClass);

		OV_STRING tmpOVSubject = NULL;
		copyOPCUAStringToOV(*tmpUASubject, &tmpOVSubject);

		IdentificationType tmpOVWritingInstance;
		tmpOVWritingInstance.IdSpec = NULL;
		copyOPCUAStringToOV(tmpUAWritingInstance->idSpec, &(tmpOVWritingInstance.IdSpec));
		tmpOVWritingInstance.IdType = tmpUAWritingInstance->idType;

		DataValue tmpOVDataValue;
		ov_VariantToAny(&(tmpUAValue->value), &tmpOVDataValue.Value);
		tmpOVDataValue.TimeStamp = tmpUAValue->sourceTimestamp;

		result = openaas_modelmanager_setLCE(tmpOVAASId, tmpOVLCEId, tmpOVCreatingInstance, tmpOVWritingInstance, tmpOVEventClass, tmpOVSubject, tmpOVDataValue);

		output->data = &result;
		resultOV = OV_ERR_OK;
	}else if (ov_string_compare(funcName, "createPVS") == OV_STRCMP_EQUAL){
		UA_Identification *tmpUAAASId = (UA_Identification*)(input[0].data);
		UA_String *tmpUAPVSLName = (UA_String*)(input[1].data);
		UA_String *tmpUAPVSName = (UA_String*)(input[2].data);
		UA_RelationalExpressionEnum *tmpUARelationalExpression = (UA_RelationalExpressionEnum*)(input[3].data);
		UA_ExpressionSemanticEnum *tmpUAExpressionSemantic = (UA_ExpressionSemanticEnum*)(input[4].data);
		UA_DataValue *tmpUAValue = (UA_DataValue*)(input[5].data);
		UA_String *tmpUAUnit = (UA_String*)(input[6].data);
		UA_Identification *tmpUAPropertyReference = (UA_Identification*)(input[7].data);
		UA_ViewEnum *tmpUAView = (UA_ViewEnum*)(input[8].data);

		IdentificationType tmpOVAASId;
		tmpOVAASId.IdSpec = NULL;
		copyOPCUAStringToOV(tmpUAAASId->idSpec, &(tmpOVAASId.IdSpec));
		tmpOVAASId.IdType = tmpUAAASId->idType;

		OV_STRING tmpOVPVSLName = NULL;
		copyOPCUAStringToOV(*tmpUAPVSLName, &tmpOVPVSLName);

		OV_STRING tmpOVPVSName = NULL;
		copyOPCUAStringToOV(*tmpUAPVSName, &tmpOVPVSName);

		RelationalExpressionEnum tmpOVRelationalExpression;
		tmpOVRelationalExpression = *tmpUARelationalExpression;

		ExpressionSemanticEnum tmpOVExpressionSemantic;
		tmpOVExpressionSemantic = *tmpUAExpressionSemantic;

		OV_ANY tmpOVValue;
		ov_VariantToAny(&(tmpUAValue->value), &tmpOVValue);

		OV_STRING tmpOVUnit = NULL;
		copyOPCUAStringToOV(*tmpUAUnit, &tmpOVUnit);

		IdentificationType tmpOVProperyReference;
		tmpOVProperyReference.IdSpec = NULL;
		copyOPCUAStringToOV(tmpUAPropertyReference->idSpec, &(tmpOVProperyReference.IdSpec));
		tmpOVProperyReference.IdType = tmpUAPropertyReference->idType;

		ViewEnum tmpOVView;
		tmpOVView = *tmpUAView;

		result = openaas_modelmanager_createPVS(tmpOVAASId, tmpOVPVSLName, tmpOVPVSName, tmpOVRelationalExpression, tmpOVExpressionSemantic, tmpOVValue, tmpOVUnit, tmpOVProperyReference, tmpOVView);

		output->data = &result;
		resultOV = OV_ERR_OK;
	}else if (ov_string_compare(funcName, "deletePVS") == OV_STRCMP_EQUAL){
		UA_Identification *tmpUAAASId = (UA_Identification*)(input[0].data);
		UA_String *tmpUAPVSLName = (UA_String*)(input[1].data);
		UA_String *tmpUAPVSName = (UA_String*)(input[2].data);

		IdentificationType tmpOVAASId;
		tmpOVAASId.IdSpec = NULL;
		copyOPCUAStringToOV(tmpUAAASId->idSpec, &(tmpOVAASId.IdSpec));
		tmpOVAASId.IdType = tmpUAAASId->idType;

		OV_STRING tmpOVPVSLName = NULL;
		copyOPCUAStringToOV(*tmpUAPVSLName, &tmpOVPVSLName);

		OV_STRING tmpOVPVSName = NULL;
		copyOPCUAStringToOV(*tmpUAPVSName, &tmpOVPVSName);

		result = openaas_modelmanager_deletePVS(tmpOVAASId, tmpOVPVSLName, tmpOVPVSName);

		output->data = &result;
		resultOV = OV_ERR_OK;

	}else if (ov_string_compare(funcName, "getPVS") == OV_STRCMP_EQUAL){
		UA_Identification *tmpUAAASId = (UA_Identification*)(input[0].data);
		UA_String *tmpUAPVSLName = (UA_String*)(input[1].data);
		UA_String *tmpUAPVSName = (UA_String*)(input[2].data);

		IdentificationType tmpOVAASId;
		tmpOVAASId.IdSpec = NULL;
		copyOPCUAStringToOV(tmpUAAASId->idSpec, &(tmpOVAASId.IdSpec));
		tmpOVAASId.IdType = tmpUAAASId->idType;

		OV_STRING tmpOVPVSLName = NULL;
		copyOPCUAStringToOV(*tmpUAPVSLName, &tmpOVPVSLName);

		OV_STRING tmpOVPVSName = NULL;
		copyOPCUAStringToOV(*tmpUAPVSName, &tmpOVPVSName);

		RelationalExpressionEnum tmpOVRelationalExpression;

		ExpressionSemanticEnum tmpOVExpressionSemantic;

		OV_ANY tmpOVValue;

		OV_STRING tmpOVUnit = NULL;

		IdentificationType tmpOVProperyReference;
		tmpOVProperyReference.IdSpec = NULL;

		ViewEnum tmpOVView;

		result = openaas_modelmanager_getPVS(tmpOVAASId, tmpOVPVSLName, tmpOVPVSName, &tmpOVRelationalExpression, &tmpOVExpressionSemantic, &tmpOVValue, &tmpOVUnit, &tmpOVProperyReference, &tmpOVView);

		UA_RelationalExpressionEnum tmpUARelationalExpression;
		tmpUARelationalExpression = tmpOVRelationalExpression;

		UA_ExpressionSemanticEnum tmpUAExpressionSemantic;
		tmpUAExpressionSemantic = tmpOVExpressionSemantic;

		UA_DataValue tmpUAValue;
		ov_AnyToVariant(&tmpOVValue, &tmpUAValue.value);

		UA_String tmpUAUnit;
		copyOvStringToOPCUA(tmpOVUnit, &tmpUAUnit);

		UA_Identification tmpUAPropertyReference;
		copyOvStringToOPCUA(tmpOVProperyReference.IdSpec, &tmpUAPropertyReference.idSpec);
		tmpUAPropertyReference.idType = tmpOVProperyReference.IdType;

		UA_ViewEnum tmpUAView;
		tmpUAView = tmpOVView;

		output[0].data = &tmpUARelationalExpression;
		output[1].data = &tmpUAExpressionSemantic;
		output[2].data = &tmpUAValue;
		output[3].data = &tmpUAUnit;
		output[4].data = &tmpUAPropertyReference;
		output[5].data = &tmpUAView;
		output[6].data = &result;

		output->data = &result;
		resultOV = OV_ERR_OK;
	}else if (ov_string_compare(funcName, "setPVS") == OV_STRCMP_EQUAL){
		UA_Identification *tmpUAAASId = (UA_Identification*)(input[0].data);
		UA_String *tmpUAPVSLName = (UA_String*)(input[1].data);
		UA_String *tmpUAPVSName = (UA_String*)(input[2].data);
		UA_RelationalExpressionEnum *tmpUARelationalExpression = (UA_RelationalExpressionEnum*)(input[3].data);
		UA_ExpressionSemanticEnum *tmpUAExpressionSemantic = (UA_ExpressionSemanticEnum*)(input[4].data);
		UA_DataValue *tmpUAValue = (UA_DataValue*)(input[5].data);
		UA_String *tmpUAUnit = (UA_String*)(input[6].data);
		UA_Identification *tmpUAPropertyReference = (UA_Identification*)(input[7].data);
		UA_ViewEnum *tmpUAView = (UA_ViewEnum*)(input[8].data);

		IdentificationType tmpOVAASId;
		tmpOVAASId.IdSpec = NULL;
		copyOPCUAStringToOV(tmpUAAASId->idSpec, &(tmpOVAASId.IdSpec));
		tmpOVAASId.IdType = tmpUAAASId->idType;

		OV_STRING tmpOVPVSLName = NULL;
		copyOPCUAStringToOV(*tmpUAPVSLName, &tmpOVPVSLName);

		OV_STRING tmpOVPVSName = NULL;
		copyOPCUAStringToOV(*tmpUAPVSName, &tmpOVPVSName);

		RelationalExpressionEnum tmpOVRelationalExpression;
		tmpOVRelationalExpression = *tmpUARelationalExpression;

		ExpressionSemanticEnum tmpOVExpressionSemantic;
		tmpOVExpressionSemantic = *tmpUAExpressionSemantic;

		OV_ANY tmpOVValue;
		ov_VariantToAny(&(tmpUAValue->value), &tmpOVValue);

		OV_STRING tmpOVUnit = NULL;
		copyOPCUAStringToOV(*tmpUAUnit, &tmpOVUnit);

		IdentificationType tmpOVProperyReference;
		tmpOVProperyReference.IdSpec = NULL;
		copyOPCUAStringToOV(tmpUAPropertyReference->idSpec, &(tmpOVProperyReference.IdSpec));
		tmpOVProperyReference.IdType = tmpUAPropertyReference->idType;

		ViewEnum tmpOVView;
		tmpOVView = *tmpUAView;

		result = openaas_modelmanager_setPVS(tmpOVAASId, tmpOVPVSLName, tmpOVPVSName, tmpOVRelationalExpression, tmpOVExpressionSemantic, tmpOVValue, tmpOVUnit, tmpOVProperyReference, tmpOVView);

		output->data = &result;
		resultOV = OV_ERR_OK;
	}else{
		resultOV = OV_ERR_BADPARAM;
	}

    return resultOV;
}

static void OV_NodeStore_deleteNodestore(void *handle){

}

static void OV_NodeStore_deleteNode(UA_Node *node){
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
	OV_NodeStore_releaseNode(node);
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
				if (ov_string_compare(tmpPart.elemunion.pvar->v_identifier, "RelationalExpression") == OV_STRCMP_EQUAL)
					*(UA_UInt32*)tmpPart.pvalue = tmpPropertyValueStatement->relationalExpression;
				if (ov_string_compare(tmpPart.elemunion.pvar->v_identifier, "Unit") == OV_STRCMP_EQUAL)
					copyOPCUAStringToOV(tmpPropertyValueStatement->unit, ((OV_STRING*)tmpPart.pvalue));
				if (ov_string_compare(tmpPart.elemunion.pvar->v_identifier, "Value") == OV_STRCMP_EQUAL)
					ov_VariantToAny(&tmpPropertyValueStatement->value, (OV_ANY*)tmpPart.pvalue);
				if (ov_string_compare(tmpPart.elemunion.pvar->v_identifier, "PropertyReferenceString") == OV_STRCMP_EQUAL)
					copyOPCUAStringToOV(tmpPropertyValueStatement->propertyReference.idSpec, ((OV_STRING*)tmpPart.pvalue));
				if (ov_string_compare(tmpPart.elemunion.pvar->v_identifier, "PropertyReferenceType") == OV_STRCMP_EQUAL)
					*(UA_UInt32*)tmpPart.pvalue = tmpPropertyValueStatement->propertyReference.idType;
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
				if (ov_string_compare(tmpPart.elemunion.pvar->v_identifier, "View") == OV_STRCMP_EQUAL)
					*(UA_UInt32*)tmpPart.pvalue = tmpLifeCycleEntry->view;
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
    UA_free(nsi->handle);
    ov_database_free(nsi);
}
