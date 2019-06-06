/* * nodeStoreInterface.c
 *
 *  Created on: 06.12.2016
 *      Author: ubuntu
 */

#ifndef OV_COMPILE_LIBRARY_lifeCycleEntryOPCUAInterface
#define OV_COMPILE_LIBRARY_lifeCycleEntryOPCUAInterface
#endif

#include "lifeCycleEntryOPCUAInterface.h"
#include "libov/ov_macros.h"
#include "opcua.h"
#include "opcua_helpers.h"
#include "ua_lifeCycleEntry_generated.h"
#include "ua_lifeCycleEntry_generated_handling.h"

OV_DLLFNCEXPORT UA_StatusCode lifeCycleEntryOPCUAInterface_interface_MethodCallbackArchiv(UA_Server *server,
                     const UA_NodeId *sessionId, void *sessionContext,
					 const UA_NodeId *methodId, void *methodContext,
					 const UA_NodeId *objectId, void *objectContext,
                     size_t inputSize, const UA_Variant *input,
                     size_t outputSize, UA_Variant *output) {
	OV_RESULT resultOV = OV_ERR_OK;
	LCEStatusCode result = LCESTATUSCODE_GOOD;
	OV_STRING funcName = (OV_STRING)methodContext;
	OV_PATH 				path;
	OV_INSTPTR_ov_object	pobj = NULL;
	OV_VTBLPTR_ov_object	pVtblObj = NULL;
	OV_ACCESS				access;
	OV_ELEMENT				element;

	ov_memstack_lock();
	result = opcua_helpers_resolveNodeIdToPath(*objectId, &path);
	if(result != UA_STATUSCODE_GOOD){
		ov_memstack_unlock();
		return result;
	}
	element = path.elements[path.size-1];
	ov_memstack_unlock();
	result = opcua_helpers_getVtblPointerAndCheckAccess(&(element), &pobj, &pVtblObj, &access);
	if(result != UA_STATUSCODE_GOOD){
		return result;
	}

	OV_INSTPTR_lifeCycleEntry_LifeCycleArchive pLCEArchiv = NULL;
	pLCEArchiv = Ov_StaticPtrCast(lifeCycleEntry_LifeCycleArchive, pobj);

	if (ov_string_compare(funcName, "createLCE") == OV_STRCMP_EQUAL){
		if (input[0].data == NULL){
			result = LCESTATUSCODE_BADMISSINGARGUMENTS;
			UA_Variant_setScalarCopy(&output[0], &result, &UA_TYPES[UA_TYPES_STATUSCODE]);
			resultOV = OV_ERR_OK;
		}
		UA_LifeCycleEntry *tmpUALCE = (UA_LifeCycleEntry*)(input[0].data);

		LifeCycleEntry lce;
		LifeCycleEntry_init(&lce);

		opcua_helpers_copyUAStringToOV(tmpUALCE->creatingInstance.idSpec, &(lce.creatingInstance.IdSpec));
		lce.creatingInstance.IdType = tmpUALCE->creatingInstance.idType;

		opcua_helpers_copyUAStringToOV(tmpUALCE->eventClass, &lce.eventClass);

		opcua_helpers_copyUAStringToOV(tmpUALCE->subject, &lce.subject);

		opcua_helpers_copyUAStringToOV(tmpUALCE->writingInstance.idSpec, &(lce.writingInstance.IdSpec));
		lce.writingInstance.IdType = tmpUALCE->writingInstance.idType;

		opcua_helpers_UAVariantToOVAny(&(tmpUALCE->data.value), &lce.data);
		lce.data.time = ov_1601nsTimeToOvTime(tmpUALCE->data.sourceTimestamp);

		result = lifeCycleEntry_LifeCycleArchive_createLCE(pLCEArchiv, lce);

		UA_Variant_setScalarCopy(&output[0], &result, &UA_TYPES[UA_TYPES_STATUSCODE]);
		LifeCycleEntry_deleteMembers(&lce);
		resultOV = OV_ERR_OK;
	}else if (ov_string_compare(funcName, "deleteLCE") == OV_STRCMP_EQUAL){
		if (input[0].data == NULL){
			result = LCESTATUSCODE_BADMISSINGARGUMENTS;
			UA_Variant_setScalarCopy(&output[0], &result, &UA_TYPES[UA_TYPES_STATUSCODE]);
			resultOV = OV_ERR_OK;
		}
		UA_UInt64 *tmpUALCEId = (UA_UInt64*)(input[0].data);

		OV_UINT64 tmpOVLCEId;
		tmpOVLCEId = *tmpUALCEId;

		result = lifeCycleEntry_LifeCycleArchive_deleteLCE(pLCEArchiv, tmpOVLCEId);

		UA_Variant_setScalarCopy(&output[0], &result, &UA_TYPES[UA_TYPES_STATUSCODE]);
		resultOV = OV_ERR_OK;
	}else if (ov_string_compare(funcName, "createLCESimple") == OV_STRCMP_EQUAL){
		if (input[0].data == NULL ||
			input[1].data == NULL ||
			input[2].data == NULL ||
			input[3].data == NULL ||
			input[4].data == NULL ||
			input[5].data == NULL ||
			input[6].data == NULL){
			result = LCESTATUSCODE_BADMISSINGARGUMENTS;
			UA_Variant_setScalarCopy(&output[0], &result, &UA_TYPES[UA_TYPES_STATUSCODE]);
			resultOV = OV_ERR_OK;
		}
		UA_UInt32 *tmpUACreatingInstanceIdType = (UA_UInt32*)(input[0].data);
		UA_String *tmpUACreatingInstanceIdSpec = (UA_String*)(input[1].data);
		UA_UInt32 *tmpUAWritingInstanceIdType = (UA_UInt32*)(input[2].data);
		UA_String *tmpUAWritingInstanceIdSpec = (UA_String*)(input[3].data);
		UA_DataValue *tmpUAValue = (UA_DataValue*)(input[4].data);
		UA_String *tmpUASubject = (UA_String*)(input[5].data);
		UA_String *tmpUAEventClass = (UA_String*)(input[6].data);

		LifeCycleEntry lce;
		LifeCycleEntry_init(&lce);

		opcua_helpers_copyUAStringToOV(*tmpUACreatingInstanceIdSpec, &(lce.creatingInstance.IdSpec));
		lce.creatingInstance.IdType = *tmpUACreatingInstanceIdType;

		opcua_helpers_copyUAStringToOV(*tmpUAEventClass, &lce.eventClass);

		opcua_helpers_copyUAStringToOV(*tmpUASubject, &lce.subject);

		opcua_helpers_copyUAStringToOV(*tmpUAWritingInstanceIdSpec, &(lce.writingInstance.IdSpec));
		lce.writingInstance.IdType = *tmpUAWritingInstanceIdType;

		opcua_helpers_UAVariantToOVAny(&(tmpUAValue->value), &lce.data);
		lce.data.time = ov_1601nsTimeToOvTime(tmpUAValue->sourceTimestamp);

		result = lifeCycleEntry_LifeCycleArchive_createLCE(pLCEArchiv, lce);

		UA_Variant_setScalarCopy(&output[0], &result, &UA_TYPES[UA_TYPES_STATUSCODE]);
		LifeCycleEntry_deleteMembers(&lce);
		resultOV = OV_ERR_OK;
	}else if (ov_string_compare(funcName, "getLCE") == OV_STRCMP_EQUAL){
		if (input[0].data == NULL){
			result = LCESTATUSCODE_BADMISSINGARGUMENTS;
			UA_Variant_setScalarCopy(&output[0], &result, &UA_TYPES[UA_TYPES_STATUSCODE]);
			resultOV = OV_ERR_OK;
		}
		UA_UInt64 *tmpUALCEId = (UA_UInt64*)(input[0].data);

		OV_UINT64 tmpOVLCEId;
		tmpOVLCEId = *tmpUALCEId;

		LifeCycleEntry lce;
		LifeCycleEntry_init(&lce);

		result = lifeCycleEntry_LifeCycleArchive_getLCE(pLCEArchiv, tmpOVLCEId, &lce);
		UA_LifeCycleEntry tmpUALCE;
		UA_LifeCycleEntry_init(&tmpUALCE);
		if (result == LCESTATUSCODE_GOOD){
			tmpUALCE.creatingInstance.idSpec = UA_String_fromChars(lce.writingInstance.IdSpec);
			tmpUALCE.creatingInstance.idType = lce.writingInstance.IdType;

			tmpUALCE.writingInstance.idSpec = UA_String_fromChars(lce.writingInstance.IdSpec);
			tmpUALCE.writingInstance.idType = lce.writingInstance.IdType;

			tmpUALCE.eventClass = UA_String_fromChars(lce.eventClass);

			tmpUALCE.subject = UA_String_fromChars(lce.subject);

			opcua_helpers_ovAnyToUAVariant(&lce.data, &tmpUALCE.data.value);
			tmpUALCE.data.hasValue = TRUE;
			tmpUALCE.data.sourceTimestamp = ov_ovTimeTo1601nsTime(lce.data.time);
			tmpUALCE.data.hasSourceTimestamp = TRUE;
		}
		UA_Variant_setScalarCopy(&output[0], &result, &UA_TYPES[UA_TYPES_STATUSCODE]);
		UA_Variant_setScalarCopy(&output[1], &tmpUALCE, &UA_LIFECYCLEENTRY[UA_LIFECYCLEENTRY_LIFECYCLEENTRY]);

		UA_LifeCycleEntry_deleteMembers(&tmpUALCE);
		LifeCycleEntry_deleteMembers(&lce);
		resultOV = OV_ERR_OK;
	}else if (ov_string_compare(funcName, "getLCESimple") == OV_STRCMP_EQUAL){
		if (input[0].data == NULL){
			result = LCESTATUSCODE_BADMISSINGARGUMENTS;
			UA_Variant_setScalarCopy(&output[0], &result, &UA_TYPES[UA_TYPES_STATUSCODE]);

			UA_Identification tmpUACreatingInstance;
			UA_Identification_init(&tmpUACreatingInstance);
			UA_Variant_setScalarCopy(&output[1], &tmpUACreatingInstance.idType, &UA_TYPES[UA_TYPES_UINT32]);
			UA_Variant_setScalarCopy(&output[2], &tmpUACreatingInstance.idSpec, &UA_TYPES[UA_TYPES_STRING]);
			UA_Identification_deleteMembers(&tmpUACreatingInstance);

			UA_Identification tmpUAWritingInstance;
			UA_Identification_init(&tmpUAWritingInstance);
			UA_Variant_setScalarCopy(&output[3], &tmpUAWritingInstance.idType, &UA_TYPES[UA_TYPES_UINT32]);
			UA_Variant_setScalarCopy(&output[4], &tmpUAWritingInstance.idSpec, &UA_TYPES[UA_TYPES_STRING]);
			UA_Identification_deleteMembers(&tmpUAWritingInstance);

			UA_String tmpUAEventClass;
			UA_String_init(&tmpUAEventClass);
			UA_Variant_setScalarCopy(&output[7], &tmpUAEventClass, &UA_TYPES[UA_TYPES_STRING]);
			UA_String_deleteMembers(&tmpUAEventClass);

			UA_String tmpUASubject;
			UA_String_init(&tmpUASubject);
			UA_Variant_setScalarCopy(&output[6], &tmpUASubject, &UA_TYPES[UA_TYPES_STRING]);
			UA_String_deleteMembers(&tmpUASubject);

			UA_DataValue tmpUAValue;
			UA_DataValue_init(&tmpUAValue);
			UA_Variant_setScalarCopy(&output[5], &tmpUAValue, &UA_TYPES[UA_TYPES_DATAVALUE]);
			UA_DataValue_deleteMembers(&tmpUAValue);

			resultOV = OV_ERR_OK;
		}
		UA_UInt64 *tmpUALCEId = (UA_UInt64*)(input[0].data);

		OV_UINT64 tmpOVLCEId;
		tmpOVLCEId = *tmpUALCEId;

		LifeCycleEntry lce;
		LifeCycleEntry_init(&lce);

		result = lifeCycleEntry_LifeCycleArchive_getLCE(pLCEArchiv, tmpOVLCEId, &lce);

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
		if (result == LCESTATUSCODE_GOOD){
			tmpUACreatingInstance.idSpec = UA_String_fromChars(lce.creatingInstance.IdSpec);
			tmpUACreatingInstance.idType = lce.creatingInstance.IdType;

			tmpUAWritingInstance.idSpec = UA_String_fromChars(lce.writingInstance.IdSpec);
			tmpUAWritingInstance.idType = lce.writingInstance.IdType;

			tmpUAEventClass = UA_String_fromChars(lce.eventClass);

			tmpUASubject = UA_String_fromChars(lce.subject);

			opcua_helpers_ovAnyToUAVariant(&lce.data, &tmpUAValue.value);
			tmpUAValue.hasValue = TRUE;
			tmpUAValue.sourceTimestamp = ov_ovTimeTo1601nsTime(lce.data.time);
			tmpUAValue.hasSourceTimestamp = TRUE;
		}

		UA_Variant_setScalarCopy(&output[0], &result, &UA_TYPES[UA_TYPES_STATUSCODE]);
		UA_Variant_setScalarCopy(&output[1], &tmpUACreatingInstance.idType, &UA_TYPES[UA_TYPES_UINT32]);
		UA_Variant_setScalarCopy(&output[2], &tmpUACreatingInstance.idSpec, &UA_TYPES[UA_TYPES_STRING]);
		UA_Variant_setScalarCopy(&output[3], &tmpUAWritingInstance.idType, &UA_TYPES[UA_TYPES_UINT32]);
		UA_Variant_setScalarCopy(&output[4], &tmpUAWritingInstance.idSpec, &UA_TYPES[UA_TYPES_STRING]);
		UA_Variant_setScalarCopy(&output[5], &tmpUAValue, &UA_TYPES[UA_TYPES_DATAVALUE]);
		UA_Variant_setScalarCopy(&output[6], &tmpUASubject, &UA_TYPES[UA_TYPES_STRING]);
		UA_Variant_setScalarCopy(&output[7], &tmpUAEventClass, &UA_TYPES[UA_TYPES_STRING]);

		UA_Identification_deleteMembers(&tmpUACreatingInstance);
		UA_Identification_deleteMembers(&tmpUAWritingInstance);
		UA_String_deleteMembers(&tmpUAEventClass);
		UA_String_deleteMembers(&tmpUASubject);
		UA_DataValue_deleteMembers(&tmpUAValue);
		LifeCycleEntry_deleteMembers(&lce);
		resultOV = OV_ERR_OK;
	}else if (ov_string_compare(funcName, "getLastLCEs") == OV_STRCMP_EQUAL){
		if (input[0].data == NULL){
			result = LCESTATUSCODE_BADMISSINGARGUMENTS;
			UA_Variant_setScalarCopy(&output[0], &result, &UA_TYPES[UA_TYPES_STATUSCODE]);
			UA_LifeCycleEntry *tmpUALifeCycleEntry = UA_Array_new(0,&UA_LIFECYCLEENTRY[UA_LIFECYCLEENTRY_LIFECYCLEENTRY]);
			UA_Variant_setArrayCopy(&output[1], tmpUALifeCycleEntry, 0, &UA_LIFECYCLEENTRY[UA_LIFECYCLEENTRY_LIFECYCLEENTRY]);
			UA_Variant_setScalarCopy(&output[2], 0, &UA_TYPES[UA_TYPES_UINT32]);
			UA_Array_delete(tmpUALifeCycleEntry,0,&UA_LIFECYCLEENTRY[UA_LIFECYCLEENTRY_LIFECYCLEENTRY]);
			resultOV = OV_ERR_OK;
		}
		UA_UInt32 *tmpUACount = (UA_UInt32*)(input[0].data);

		OV_UINT tmpOVCount;
		tmpOVCount = *tmpUACount;

		LifeCycleEntry *lce = NULL;

		OV_UINT arrayDimension = 0;

		result = lifeCycleEntry_LifeCycleArchive_getLastLCEs(pLCEArchiv, tmpOVCount, &lce, &arrayDimension);

		UA_Variant_setScalarCopy(&output[0], &result, &UA_TYPES[UA_TYPES_STATUSCODE]);

		if (result == LCESTATUSCODE_GOOD){
			UA_LifeCycleEntry *tmpUALifeCycleEntry = UA_Array_new(arrayDimension,&UA_LIFECYCLEENTRY[UA_LIFECYCLEENTRY_LIFECYCLEENTRY]);

			for (OV_UINT i = 0; i < arrayDimension; i++){
				UA_LifeCycleEntry_init(&tmpUALifeCycleEntry[i]);
				tmpUALifeCycleEntry[i].creatingInstance.idSpec = UA_String_fromChars(lce[i].creatingInstance.IdSpec);
				tmpUALifeCycleEntry[i].creatingInstance.idType = lce[i].creatingInstance.IdType;
				tmpUALifeCycleEntry[i].writingInstance.idSpec = UA_String_fromChars(lce[i].writingInstance.IdSpec);
				tmpUALifeCycleEntry[i].writingInstance.idType = lce[i].writingInstance.IdType;
				tmpUALifeCycleEntry[i].eventClass = UA_String_fromChars(lce[i].eventClass);
				tmpUALifeCycleEntry[i].subject = UA_String_fromChars(lce[i].subject);
				opcua_helpers_ovAnyToUAVariant(&lce[i].data, &tmpUALifeCycleEntry[i].data.value);
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
		resultOV = OV_ERR_OK;
	}else if (ov_string_compare(funcName, "setLCE") == OV_STRCMP_EQUAL){
		if (input[0].data == NULL ||
			input[1].data == NULL){
			result = LCESTATUSCODE_BADMISSINGARGUMENTS;
			UA_Variant_setScalarCopy(&output[0], &result, &UA_TYPES[UA_TYPES_STATUSCODE]);
			resultOV = OV_ERR_OK;
		}
		UA_UInt64 *tmpUALCEId = (UA_UInt64*)(input[0].data);
		UA_LifeCycleEntry *tmpUALCEData = (UA_LifeCycleEntry*)(input[1].data);

		LifeCycleEntry lce;
		LifeCycleEntry_init(&lce);

		opcua_helpers_copyUAStringToOV(tmpUALCEData->creatingInstance.idSpec, &(lce.creatingInstance.IdSpec));
		lce.creatingInstance.IdType = tmpUALCEData->creatingInstance.idType;

		opcua_helpers_copyUAStringToOV(tmpUALCEData->eventClass, &lce.eventClass);

		opcua_helpers_copyUAStringToOV(tmpUALCEData->subject, &lce.subject);

		opcua_helpers_copyUAStringToOV(tmpUALCEData->writingInstance.idSpec, &(lce.writingInstance.IdSpec));
		lce.writingInstance.IdType = tmpUALCEData->writingInstance.idType;

		opcua_helpers_UAVariantToOVAny(&(tmpUALCEData->data.value), &lce.data);
		lce.data.time = ov_1601nsTimeToOvTime(tmpUALCEData->data.sourceTimestamp);

		lce.lceId = *tmpUALCEId;

		result = lifeCycleEntry_LifeCycleArchive_setLCE(pLCEArchiv, lce);

		UA_Variant_setScalarCopy(&output[0], &result, &UA_TYPES[UA_TYPES_STATUSCODE]);
		LifeCycleEntry_deleteMembers(&lce);
		resultOV = OV_ERR_OK;
	}else if (ov_string_compare(funcName, "setLCESimple") == OV_STRCMP_EQUAL){
		if (input[0].data == NULL ||
			input[1].data == NULL ||
			input[2].data == NULL ||
			input[3].data == NULL ||
			input[4].data == NULL ||
			input[5].data == NULL ||
			input[6].data == NULL ||
			input[7].data == NULL){
			result = LCESTATUSCODE_BADMISSINGARGUMENTS;
			UA_Variant_setScalarCopy(&output[0], &result, &UA_TYPES[UA_TYPES_STATUSCODE]);
			resultOV = OV_ERR_OK;
		}
		UA_UInt64 *tmpUALCEId = (UA_UInt64*)(input[0].data);
		UA_UInt32 *tmpUACreatingInstanceIdType = (UA_UInt32*)(input[1].data);
		UA_String *tmpUACreatingInstanceIdSpec = (UA_String*)(input[2].data);
		UA_UInt32 *tmpUAWritingInstanceIdType = (UA_UInt32*)(input[3].data);
		UA_String *tmpUAWritingInstanceIdSpec = (UA_String*)(input[4].data);
		UA_DataValue *tmpUAValue = (UA_DataValue*)(input[5].data);
		UA_String *tmpUASubject = (UA_String*)(input[6].data);
		UA_String *tmpUAEventClass = (UA_String*)(input[7].data);

		LifeCycleEntry lce;
		LifeCycleEntry_init(&lce);

		lce.lceId = *tmpUALCEId;

		opcua_helpers_copyUAStringToOV(*tmpUACreatingInstanceIdSpec, &(lce.creatingInstance.IdSpec));
		lce.creatingInstance.IdType = *tmpUACreatingInstanceIdType;

		opcua_helpers_copyUAStringToOV(*tmpUAEventClass, &lce.eventClass);

		opcua_helpers_copyUAStringToOV(*tmpUASubject, &lce.subject);

		opcua_helpers_copyUAStringToOV(*tmpUAWritingInstanceIdSpec, &(lce.writingInstance.IdSpec));
		lce.writingInstance.IdType = *tmpUAWritingInstanceIdType;

		opcua_helpers_UAVariantToOVAny(&(tmpUAValue->value), &lce.data);
		lce.data.time = ov_1601nsTimeToOvTime(tmpUAValue->sourceTimestamp);

		result = lifeCycleEntry_LifeCycleArchive_setLCE(pLCEArchiv, lce);

		UA_Variant_setScalarCopy(&output[0], &result, &UA_TYPES[UA_TYPES_STATUSCODE]);
		LifeCycleEntry_deleteMembers(&lce);
		resultOV = OV_ERR_OK;
	}else{
		resultOV = OV_ERR_BADPARAM;
	}

    return resultOV;
}

static void lifeCycleEntryOPCUAInterface_deleteNodestore(void *context){

}

static void lifeCycleEntryOPCUAInterface_deleteNode(void *context, UA_Node *node){
	if (node){
		if (node->context)
			ov_string_setvalue((OV_STRING*)&(node->context), NULL);
		UA_Node_deleteMembers(node);
	}
	UA_free(node);
}
static void lifeCycleEntryOPCUAInterface_releaseNode(void *context, const UA_Node *node){
	lifeCycleEntryOPCUAInterface_deleteNode(context, (UA_Node*)node);
}

static UA_Node * lifeCycleEntryOPCUAInterface_newNode(void *context, UA_NodeClass nodeClass){
    //allocate memory for a new node
	//UA_Node *newNode = NULL;
	//newNode = (UA_Node*) ov_database_malloc(sizeof(UA_ObjectNode));
	//newNode->nodeClass = nodeClass;
    return NULL;
}
static const UA_Node * lifeCycleEntryOPCUAInterface_getNode(void *context, const UA_NodeId *nodeId){
	UA_Node * tmpNode = NULL;
	UA_Node* opcuaNode = NULL;
	OV_STRING tmpString = NULL;
	OV_INSTPTR_ov_object pobj = NULL;
	OV_STRING *plist = NULL;
	OV_STRING *plist2 = NULL;
	OV_UINT len = 0;
	OV_UINT len2 = 0;
	if (nodeId->identifier.string.data == NULL || nodeId->identifier.string.length == 0 || nodeId->identifierType != UA_NODEIDTYPE_STRING)
		return NULL;
	opcua_helpers_copyUAStringToOV(nodeId->identifier.string, &tmpString);

	plist = ov_string_split(tmpString, "||", &len);
	plist2 = ov_string_split(tmpString, "|||", &len2);

	ov_memstack_lock();
	if (len2 > 1){
		pobj = ov_path_getobjectpointer(plist2[0], 2);
		ov_string_freelist(plist2);
		if (pobj == NULL){
			ov_string_freelist(plist);
			ov_string_setvalue(&tmpString, NULL);
			ov_memstack_unlock();
			return NULL;
		}

		if (Ov_CanCastTo(lifeCycleEntry_LifeCycleArchive, pobj)){
			if (lifeCycleEntryOPCUAInterface_interface_ovLifeCycleEntriesNodeToOPCUA(context, nodeId, &opcuaNode) == UA_STATUSCODE_GOOD){
				ov_string_freelist(plist);
				ov_string_setvalue(&tmpString, NULL);
				ov_memstack_unlock();
				return (UA_Node*) opcuaNode;
			}
		}
	}
	ov_string_freelist(plist2);
	if (len > 1){
		pobj = ov_path_getobjectpointer(plist[0], 2);
		if (pobj == NULL){
			ov_string_freelist(plist);
			ov_string_setvalue(&tmpString, NULL);
			ov_memstack_unlock();
			return NULL;
		}
		if (Ov_CanCastTo(lifeCycleEntry_LifeCycleArchive, pobj)){
			if (lifeCycleEntryOPCUAInterface_interface_ovLifeCycleArchiveMethodNodeToOPCUA(context, nodeId, &opcuaNode) == UA_STATUSCODE_GOOD){
				ov_string_freelist(plist);
				ov_string_setvalue(&tmpString, NULL);
				ov_memstack_unlock();
				return (UA_Node*) opcuaNode;
			}
		}
	}

	ov_string_freelist(plist);
	pobj = ov_path_getobjectpointer(tmpString, 2);
	if (pobj == NULL){
		ov_string_setvalue(&tmpString, NULL);
		ov_memstack_unlock();
		return NULL;
	}

	if(Ov_CanCastTo(lifeCycleEntry_LifeCycleArchive, pobj)){
		if (lifeCycleEntryOPCUAInterface_interface_ovLifeCycleArchiveNodeToOPCUA(context, nodeId, &opcuaNode) == UA_STATUSCODE_GOOD)
			tmpNode = opcuaNode;
	}else if(Ov_CanCastTo(lifeCycleEntry_LifeCycleEntry, pobj)){
		if (lifeCycleEntryOPCUAInterface_interface_ovLifeCycleEntryNodeToOPCUA(context, nodeId, &opcuaNode) == UA_STATUSCODE_GOOD)
			tmpNode = opcuaNode;
	}
	ov_memstack_unlock();
	ov_string_setvalue(&tmpString, NULL);
	return tmpNode;
}
static UA_StatusCode lifeCycleEntryOPCUAInterface_getCopyNode(void *context, const UA_NodeId *nodeId, UA_Node ** nodeOut){
	UA_Node* node = (UA_Node*) lifeCycleEntryOPCUAInterface_getNode(context, nodeId);
	if(node == NULL)
		return UA_STATUSCODE_BADNODEIDUNKNOWN;
	*nodeOut = node;
	return UA_STATUSCODE_GOOD;
}
static UA_StatusCode lifeCycleEntryOPCUAInterface_removeNode(void *context, const UA_NodeId *nodeId){
	return UA_STATUSCODE_BADNOTIMPLEMENTED;
	//OV_INSTPTR_ov_object pobj = opcua_nodeStoreFunctions_resolveNodeIdToOvObject((UA_NodeId*)nodeId);
	//if (pobj != NULL){
	//	Ov_DeleteObject(pobj);
	//}
    //return UA_STATUSCODE_GOOD;
}
static UA_StatusCode lifeCycleEntryOPCUAInterface_insertNode(void *context, UA_Node *node, UA_NodeId *parrentNode){
	return UA_STATUSCODE_BADNOTIMPLEMENTED;
	//OV_INSTPTR_ov_object pobj = opcua_nodeStoreFunctions_resolveNodeIdToOvObject(&(node->nodeId));
	//if (pobj != NULL)
	//	return UA_STATUSCODE_BADNODEIDEXISTS;

	//return lifeCycleEntryOPCUAInterface_insert(context, node, parrentNode);
}
static UA_StatusCode lifeCycleEntryOPCUAInterface_replaceNode(void *context, UA_Node *node){
	return UA_STATUSCODE_BADNOTIMPLEMENTED;
}
static void lifeCycleEntryOPCUAInterface_iterate(void *context, UA_NodestoreVisitor visitor, void* visitorHandle){

}

UA_NodestoreInterface* lifeCycleEntryOPCUAInterface_interface_ovNodeStoreInterfaceLifeCycleEntryNew(OV_INSTPTR_lifeCycleEntryOPCUAInterface_interface context) {
	UA_NodestoreInterface* trafo = UA_malloc(sizeof(UA_NodestoreInterface));
	if(trafo == NULL)
		return NULL;
    trafo->context =          context;
    trafo->newNode =       	  lifeCycleEntryOPCUAInterface_newNode;
    trafo->deleteNode =    	  lifeCycleEntryOPCUAInterface_deleteNode;
    trafo->deleteNodestore =  lifeCycleEntryOPCUAInterface_deleteNodestore;

    trafo->getNode =          lifeCycleEntryOPCUAInterface_getNode;
    trafo->releaseNode =      lifeCycleEntryOPCUAInterface_releaseNode;

    trafo->getNodeCopy =      lifeCycleEntryOPCUAInterface_getCopyNode;
    trafo->insertNode =       lifeCycleEntryOPCUAInterface_insertNode;
    trafo->replaceNode =      lifeCycleEntryOPCUAInterface_replaceNode;

    trafo->removeNode =       lifeCycleEntryOPCUAInterface_removeNode;

    trafo->iterate =          lifeCycleEntryOPCUAInterface_iterate;
    return trafo;
}

void lifeCycleEntryOPCUAInterface_interface_ovNodeStoreInterfaceLifeCycleEntryDelete(UA_NodestoreInterface * store){
	store->context = NULL;
	UA_free(store);
}
