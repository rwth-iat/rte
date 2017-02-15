/******************************************************************************
 *
 *   FILE
 *   ----
 *   nodeStoreFunctions.c
 *
 *   History
 *   -------
 *   2014-10-21   File created
 *
 *******************************************************************************
 *
 *   This file is generated by the 'acplt_builder' command
 *
 ******************************************************************************/

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
#include "nodeset.h"
#include "ua_openaas_generated.h"
#include "ua_openaas_generated_handling.h"

extern OV_INSTPTR_openaas_nodeStoreFunctions pNodeStoreFunctions;




OV_DLLFNCEXPORT UA_StatusCode openaas_nodeStoreFunctions_ovCarrierNodeToOPCUA(
		void *handle, const UA_NodeId *nodeId, UA_Node** opcuaNode) {
	UA_Node 				*newNode = NULL;
	UA_StatusCode 			result = UA_STATUSCODE_GOOD;
	OV_PATH 				path;
	OV_PATH 				path2;
	UA_NodeClass 			*nodeClass = NULL;
	OV_STRING 				tmpString = NULL;
	OV_UINT 				len = 0;
	OV_STRING 				*plist = NULL;
	OV_ELEMENT				element;
	OV_ELEMENT				element2;

	copyOPCUAStringToOV(nodeId->identifier.string, &tmpString);
	plist = ov_string_split(tmpString, "|", &len);

	UA_NodeId tmpNodeId;
	UA_NodeId_init(&tmpNodeId);
	tmpNodeId.namespaceIndex = nodeId->namespaceIndex;
	tmpNodeId.identifierType = nodeId->identifierType;
	ov_string_setvalue(&tmpString, plist[0]);
	ov_string_append(&tmpString, ".CarrierString");
	copyOvStringToOPCUA(tmpString, &(tmpNodeId.identifier.string));

	ov_memstack_lock();
	result = opcua_nodeStoreFunctions_resolveNodeIdToPath(tmpNodeId, &path);
	if(result != UA_STATUSCODE_GOOD){
		ov_memstack_unlock();
		return result;
	}
	element = path.elements[path.size-1];
	ov_memstack_unlock();

	ov_string_setvalue(&tmpString, plist[0]);
	ov_string_append(&tmpString, ".CarrierType");
	copyOvStringToOPCUA(tmpString, &(tmpNodeId.identifier.string));
	ov_database_free(tmpString);

	ov_memstack_lock();
	result = opcua_nodeStoreFunctions_resolveNodeIdToPath(tmpNodeId, &path2);
	if(result != UA_STATUSCODE_GOOD){
		ov_memstack_unlock();
		return result;
	}
	element2 = path2.elements[path2.size-1];
	ov_memstack_unlock();

	nodeClass = UA_NodeClass_new();
	if(!nodeClass){
		result = ov_resultToUaStatusCode(OV_ERR_HEAPOUTOFMEMORY);
		return result;
	}

	*nodeClass = UA_NODECLASS_VARIABLE;
	newNode = (UA_Node*)UA_malloc(sizeof(UA_VariableNode));

	// Basic Attribute
	// BrowseName
	UA_QualifiedName qName;
	qName.name = UA_String_fromChars(plist[1]);
	qName.namespaceIndex = 0;
	newNode->browseName = qName;

	// Description
	UA_LocalizedText lText;
	lText.locale = UA_String_fromChars("en");
	lText.text = UA_String_fromChars("");
	newNode->description = lText;

	// DisplayName
	lText.locale = UA_String_fromChars("en");
	lText.text = UA_String_fromChars(plist[1]);
	newNode->displayName = lText;

	// NodeId
	newNode->nodeId = *nodeId;

	// NodeClass
	newNode->nodeClass 	= *nodeClass;

	// WriteMask
	newNode->writeMask 	= 0;

	// arrayDemensions
	((UA_VariableNode*)newNode)->arrayDimensionsSize = 0;
	((UA_VariableNode*)newNode)->arrayDimensions = UA_Array_new(((UA_VariableNode*)newNode)->arrayDimensionsSize, &UA_TYPES[UA_TYPES_INT32]);	/*	scalar or one dimension	*/

	// valuerank
	((UA_VariableNode*)newNode)->valueRank = 1;	/*	one dimension	*/

	// value
	UA_Identification tmpIdentification;
	UA_Identification_init(&tmpIdentification);
	copyOvStringToOPCUA(*(OV_STRING*)element.pvalue, &tmpIdentification.idSpec);
	tmpIdentification.idType = *(UA_IdEnum*)element2.pvalue;

	((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->type = &UA_OPENAAS[UA_OPENAAS_IDENTIFICATION];
	((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->data = UA_Identification_new();
	if (!((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->data){
		result = UA_STATUSCODE_BADOUTOFMEMORY;
		return result;
	}
	UA_Identification_copy(&tmpIdentification, ((UA_Variant*)&((UA_VariableNode*)newNode)->value.data.value.value)->data);
	UA_Identification_deleteMembers(&tmpIdentification);

	// accessLevel
	UA_Byte accessLevel = 0;
	accessLevel |= (1<<0);
	accessLevel |= (1<<1);

	((UA_VariableNode*)newNode)->accessLevel = accessLevel;
	// minimumSamplingInterval
	((UA_VariableNode*)newNode)->minimumSamplingInterval = -1;
	// historizing
	((UA_VariableNode*)newNode)->historizing = UA_FALSE;
	// dataType
	((UA_VariableNode*)newNode)->dataType.identifierType = UA_NODEIDTYPE_NUMERIC;
	((UA_VariableNode*)newNode)->dataType.namespaceIndex = pNodeStoreFunctions->v_NameSpaceIndexInformationModel;
	((UA_VariableNode*)newNode)->dataType.identifier.numeric = UA_NS2ID_IDENTIFICATION;

	// References
	OV_INSTPTR_ov_object pchild = NULL;
	size_t size_references = 0;
	Ov_ForEachChild(ov_containment, Ov_DynamicPtrCast(ov_domain,element.elemunion.pobj), pchild) {
		size_references++;
	}

	size_references = size_references + 2;// For Parent&TypeNode
	newNode->references = UA_calloc(size_references, sizeof(UA_ReferenceNode));
	newNode->referencesSize = size_references;
	// ParentNode
	newNode->references[0].referenceTypeId = UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY);
	newNode->references[0].isInverse = UA_TRUE;
	for (OV_UINT i = 0; i < len; i++)
		ov_database_free(plist[i]);
	len = 0;
	*plist = NULL;
	tmpString = NULL;
	copyOPCUAStringToOV(nodeId->identifier.string, &tmpString);
	plist = ov_string_split(tmpString, "|", &len);
	newNode->references[0].targetId = UA_EXPANDEDNODEID_STRING_ALLOC(pNodeStoreFunctions->v_NameSpaceIndexNodeStoreInterface, plist[0]);
	for (OV_UINT i = 0; i < len; i++)
		ov_database_free(plist[i]);
	ov_database_free(tmpString);

	// TypeNode
	newNode->references[1].referenceTypeId = UA_NODEID_NUMERIC(0, UA_NS0ID_HASTYPEDEFINITION);
	newNode->references[1].isInverse = UA_FALSE;
	newNode->references[1].targetId = UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_PROPERTYTYPE);

	size_t i = 1;

	Ov_ForEachChild(ov_containment, Ov_DynamicPtrCast(ov_domain,element.elemunion.pobj), pchild) {
		i++;
	}

	*opcuaNode = newNode;
	return UA_STATUSCODE_GOOD;
}

