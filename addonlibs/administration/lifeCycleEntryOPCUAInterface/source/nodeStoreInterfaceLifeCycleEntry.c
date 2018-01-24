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
#include "lifeCycleEntry_helpers.h"


OV_DLLFNCEXPORT UA_StatusCode lifeCycleEntryOPCUAInterface_interface_MethodCallbackArchiv(void *methodHandle, const UA_NodeId *objectId,
                     const UA_NodeId *sessionId, void *sessionHandle,
                     size_t inputSize, const UA_Variant *input,
                     size_t outputSize, UA_Variant *output) {
	OV_RESULT resultOV = OV_ERR_OK;
	LCEStatusCode result = LCESTATUSCODE_GOOD;
	OV_STRING funcName = (OV_STRING)methodHandle;
	OV_PATH 				path;
	OV_INSTPTR_ov_object	pobj = NULL;
	OV_TICKET 				*pTicket = NULL;
	OV_VTBLPTR_ov_object	pVtblObj = NULL;
	OV_ACCESS				access;
	OV_ELEMENT				element;

	ov_memstack_lock();
	result = opcua_nodeStoreFunctions_resolveNodeIdToPath(*objectId, &path);
	if(result != UA_STATUSCODE_GOOD){
		ov_memstack_unlock();
		return result;
	}
	element = path.elements[path.size-1];
	ov_memstack_unlock();
	result = opcua_nodeStoreFunctions_getVtblPointerAndCheckAccess(&(element), pTicket, &pobj, &pVtblObj, &access);
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

		copyOPCUAStringToOV(tmpUALCE->creatingInstance.idSpec, &(lce.creatingInstance.IdSpec));
		lce.creatingInstance.IdType = tmpUALCE->creatingInstance.idType;

		copyOPCUAStringToOV(tmpUALCE->eventClass, &lce.eventClass);

		copyOPCUAStringToOV(tmpUALCE->subject, &lce.subject);

		copyOPCUAStringToOV(tmpUALCE->writingInstance.idSpec, &(lce.writingInstance.IdSpec));
		lce.writingInstance.IdType = tmpUALCE->writingInstance.idType;

		ov_VariantToAny(&(tmpUALCE->data.value), &lce.data);
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

		copyOPCUAStringToOV(*tmpUACreatingInstanceIdSpec, &(lce.creatingInstance.IdSpec));
		lce.creatingInstance.IdType = *tmpUACreatingInstanceIdType;

		copyOPCUAStringToOV(*tmpUAEventClass, &lce.eventClass);

		copyOPCUAStringToOV(*tmpUASubject, &lce.subject);

		copyOPCUAStringToOV(*tmpUAWritingInstanceIdSpec, &(lce.writingInstance.IdSpec));
		lce.writingInstance.IdType = *tmpUAWritingInstanceIdType;

		ov_VariantToAny(&(tmpUAValue->value), &lce.data);
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

			ov_AnyToVariant(&lce.data, &tmpUALCE.data.value);
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

			ov_AnyToVariant(&lce.data, &tmpUAValue.value);
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

		copyOPCUAStringToOV(tmpUALCEData->creatingInstance.idSpec, &(lce.creatingInstance.IdSpec));
		lce.creatingInstance.IdType = tmpUALCEData->creatingInstance.idType;

		copyOPCUAStringToOV(tmpUALCEData->eventClass, &lce.eventClass);

		copyOPCUAStringToOV(tmpUALCEData->subject, &lce.subject);

		copyOPCUAStringToOV(tmpUALCEData->writingInstance.idSpec, &(lce.writingInstance.IdSpec));
		lce.writingInstance.IdType = tmpUALCEData->writingInstance.idType;

		ov_VariantToAny(&(tmpUALCEData->data.value), &lce.data);
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

		copyOPCUAStringToOV(*tmpUACreatingInstanceIdSpec, &(lce.creatingInstance.IdSpec));
		lce.creatingInstance.IdType = *tmpUACreatingInstanceIdType;

		copyOPCUAStringToOV(*tmpUAEventClass, &lce.eventClass);

		copyOPCUAStringToOV(*tmpUASubject, &lce.subject);

		copyOPCUAStringToOV(*tmpUAWritingInstanceIdSpec, &(lce.writingInstance.IdSpec));
		lce.writingInstance.IdType = *tmpUAWritingInstanceIdType;

		ov_VariantToAny(&(tmpUAValue->value), &lce.data);
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
	OV_STRING tmpString = NULL;
	OV_INSTPTR_ov_object pobj = NULL;
	OV_STRING *plist = NULL;
	OV_STRING *plist2 = NULL;
	OV_UINT len = 0;
	OV_UINT len2 = 0;
	if (nodeId->identifier.string.data == NULL || nodeId->identifier.string.length == 0 || nodeId->identifierType != UA_NODEIDTYPE_STRING)
		return NULL;
	copyOPCUAStringToOV(nodeId->identifier.string, &tmpString);

	plist = ov_string_split(tmpString, "||", &len);
	plist2 = ov_string_split(tmpString, "|||", &len2);

	if (len2 > 1){
		pobj = ov_path_getobjectpointer(plist2[0], 2);
		if (pobj == NULL){
			ov_string_freelist(plist);
			ov_string_freelist(plist2);
			ov_string_setvalue(&tmpString, NULL);
			return NULL;
		}
		if (Ov_CanCastTo(lifeCycleEntry_LifeCycleArchive, pobj)){
			if (lifeCycleEntryOPCUAInterface_interface_ovLifeCycleEntriesNodeToOPCUA(NULL, nodeId, &opcuaNode) == UA_STATUSCODE_GOOD){
				ov_string_freelist(plist);
				ov_string_setvalue(&tmpString, NULL);
				return (UA_Node*) opcuaNode;
			}
		}
	}

	if (len > 1){
		pobj = ov_path_getobjectpointer(plist[0], 2);
		if (pobj == NULL){
			ov_string_freelist(plist);
			ov_string_freelist(plist2);
			ov_string_setvalue(&tmpString, NULL);
			return NULL;
		}
		if (Ov_CanCastTo(lifeCycleEntry_LifeCycleArchive, pobj)){
			if (lifeCycleEntryOPCUAInterface_interface_ovLifeCycleArchiveMethodNodeToOPCUA(NULL, nodeId, &opcuaNode) == UA_STATUSCODE_GOOD){
				ov_string_freelist(plist);
				ov_string_freelist(plist2);
				ov_string_setvalue(&tmpString, NULL);
				return (UA_Node*) opcuaNode;
			}
		}
	}

	ov_string_freelist(plist);
	ov_string_freelist(plist2);
	pobj = ov_path_getobjectpointer(tmpString, 2);
	if (pobj == NULL){
		ov_string_setvalue(&tmpString, NULL);
		return NULL;
	}

	if(Ov_CanCastTo(lifeCycleEntry_LifeCycleArchive, pobj)){
		if (lifeCycleEntryOPCUAInterface_interface_ovLifeCycleArchiveNodeToOPCUA(NULL, nodeId, &opcuaNode) == UA_STATUSCODE_GOOD)
			tmpNode = opcuaNode;
	}else if(Ov_CanCastTo(lifeCycleEntry_LifeCycleEntry, pobj)){
		if (lifeCycleEntryOPCUAInterface_interface_ovLifeCycleEntryNodeToOPCUA(NULL, nodeId, &opcuaNode) == UA_STATUSCODE_GOOD)
			tmpNode = opcuaNode;
	}

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
	UA_StatusCode 			result = UA_STATUSCODE_GOOD;
	OV_PATH 				path;
	OV_INSTPTR_ov_object	pobj = NULL;
	OV_TICKET 				*pTicket = NULL;
	OV_VTBLPTR_ov_object	pVtblObj = NULL;
	OV_ACCESS				access;

	ov_memstack_lock();
	result = opcua_nodeStoreFunctions_resolveNodeIdToPath(node->nodeId, &path);
	if(result != UA_STATUSCODE_GOOD){
		ov_memstack_unlock();
		return result;
	}
	result = opcua_nodeStoreFunctions_getVtblPointerAndCheckAccess(&(path.elements[path.size-1]), pTicket, &pobj, &pVtblObj, &access);
	ov_memstack_unlock();
	if(result != UA_STATUSCODE_GOOD){
		return result;
	}

	if (!pobj){
		if (Ov_CanCastTo(lifeCycleEntry_LifeCycleEntry, pobj)){ // LCE
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
			} while(TRUE);
		}
	}

	return UA_STATUSCODE_GOOD;
}
static void OV_NodeStore_iterate(void *handle, void* visitorHandle, UA_NodestoreInterface_nodeVisitor visitor){

}
static UA_StatusCode OV_NodeStore_linkNamespace(void *handle, UA_UInt16 namespaceIndex){
	return UA_STATUSCODE_BADNOTIMPLEMENTED;
}
static UA_StatusCode OV_NodeStore_unlinkNamespace(void *handle, UA_UInt16 namespaceIndex){
	return UA_STATUSCODE_BADNOTIMPLEMENTED;
}

UA_NodestoreInterface* lifeCycleEntryOPCUAInterface_interface_ovNodeStoreInterfaceLifeCycleEntryNew(void) {
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

void lifeCycleEntryOPCUAInterface_interface_ovNodeStoreInterfaceLifeCycleEntryDelete(UA_NodestoreInterface * nsi){
	if (nsi->handle)
		UA_free(nsi->handle);
}
